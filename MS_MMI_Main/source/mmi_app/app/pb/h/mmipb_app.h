/*****************************************************************************
** File Name:      mmipb_app.h                                               *
** Author:                                                                   *
** Date:           11/09/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe phonebook                   *
**                 这个文件主要处理应用逻辑，applet管理.                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2004        baokun yin       Create
******************************************************************************/

#ifndef _MMIPB_APP_H_
#define _MMIPB_APP_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_queue.h"
#include "mmi_signal_ext.h"
#include "mmipb_interface.h"
#include "mmipb_export.h"
#include "mmifilearray_export.h"
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
#define MMIPB_MAX_APPLET_COUNT         5//允许同时存在5个pb applet实体
// 记录电话簿模块提供的显示列表的信息
typedef struct _MMIPB_PRIVACY_INFO_T
{
    BOOLEAN                 is_unprotect;//TRUE:已经解锁，FALSE:还没有解锁
    BOOLEAN                 is_cl_unprotect;//TRUE:CL,已经解锁，FALSE:还没有解锁

} MMIPB_PRIVACY_INFO_T;
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef  void(*MMIPB_UPDATE_CALLBACK_FUNC) ( MMI_HANDLE_T applet_handle, uint16  msg_id, MMIPB_ERROR_E error_code,MMIPB_OPC_E opc_type,uint16 count);
typedef  void(*MMIPB_SEARCH_CALLBACK_FUNC) (void* data_ptr);
  
#ifdef SEARCH_SUPPORT
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
typedef struct { 
//    void*  match_data_ptr;
    wchar search_str[MMISEARCH_MAX_KEYWORD_LENS+1];
//    MMI_HANDLE_T  search_win_handle;
    MMI_HANDLE_T   applet_handle;
    uint32     max_num;
	uint32     find_num;
    MMIPB_SEARCH_CALLBACK_FUNC callback;
}MMIPB_SEARCH_DATA_T;
#endif
#endif

// 操作电话簿与其它模块接口的类型
typedef enum _MMIPB_CALL_TYPE_E
{
	MMIPB_CALL_TYPE_MAKECALL_AUDIO,      // 语音电话,@zhaohui add
	#ifdef VT_SUPPORT
	MMIPB_CALL_TYPE_MAKECALL_VIDEO,//可视电话,@zhaohui add
	#endif
#ifdef MMI_IP_CALL_SUPPORT
	MMIPB_CALL_TYPE_MAKEIPCALL,    // IP拨号	
#endif
#ifdef BT_DIALER_SUPPORT
	MMIPB_CALL_TYPE_MAKEBTCALL,    // BT拨号
#endif
    MMIPB_CALL_TYPE_MAX
}MMIPB_CALL_TYPE_E;

typedef enum
{
    MMIPB_SEND_BY_SMS,
    MMIPB_SEND_BY_MMS,
    MMIPB_SEND_BY_EMAIL,
    MMIPB_SEND_BY_BT,
    MMIPB_SEND_TYPE_MAX
}MMIPB_SEND_TYPE_E;

typedef enum
{
    MMIPB_OPERATE_UPDATE,
    MMIPB_OPERATE_SUCCESS,
    MMIPB_OPERATE_PARTLY_SUCCESS,
    MMIPB_OPERATE_FAIL,
}MMIPB_UPDATE_MSG;

//PB模块的主要状态
typedef enum
{
    MMIPB_ONE_LINE_TIPS,
    MMIPB_DUAL_LINE,
}MMIPB_DISPLAY_MODE_TYPE_E;
//PB模块的主要状态
typedef enum
{
    MMIPB_STATUS_READY=0x0, //PB可用状态
    MMIPB_STATUS_SIM_NOT_READY = 0x1, //SIM卡没有初始化完成
    MMIPB_STATUS_SORTING = 0x2,//正在排序
    MMIPB_STATUS_SORT_EXIT = 0x3,//排序退出
}MMIPB_STATUS_E;



typedef struct _MMIPB_SEL_NODE
{
    uint16 entry_id;
    uint16 storage_id;
    uint16 num_index;
    uint8  sel_field;//选择的域
    uint8 is_in_group;//高四位：原来是否在群组中 1/0
                      //低四位：操作后是否在链表中1/0
    BOOLEAN is_manage_group;
    struct _MMIPB_SEL_NODE*  next;//当前操作类型
     
}MMIPB_SEL_NODE_T;//选择多个选项时，每个选项节点的结构

typedef enum _MMIPB_PBLIST_TYPE_E
{
    MMIPB_ALL_GROUP_CONTACTS,//所有联系人
    MMIPB_SINGLE_GROUP_CONTACTS,//单个分组联系人	
    MMIPB_MOSTUSED_CONTACTS,//常用联系人
//lzk mark
#ifdef BT_PBAP_SUPPORT
    MMIPB_BT_CONTACTS, //蓝牙联系人 
#endif
    MMIPB_SIM_DN_CONTACTS,//SIM卡特定号码联系人
    MMIPB_WITHOUT_CONTACTS_LIST,//不需要列表
    MMIPB_CONTACTS_TYPE_MAX
}MMIPB_CONTACTS_TYP_E;


typedef enum //密码验证后进入哪个pb list窗口
{
    MMIPB_OPEN_MAIN_LIST,            
    MMIPB_ADD_ENTRY,
    MMIPB_ADD_TO_EXIST_ENTRY,
    MMIPB_OPEN_VCARD,
    MMIPB_OPEN_FAVOURITE_VIEW,
    MMIPB_OPEN_MSISDN_ENTRY_LIST,
    MMIPB_SIGNLE_SELECT_LIST,
    MMIPB_MUTI_SELECT_LIST,
    MMIPB_FUNCTION_MAX
} MMIPB_FUNCTION_TYPE_E;



typedef BOOLEAN (*MMIPB_ENTRY_FUNC)(MMI_HANDLE_T applet_handle);

typedef struct
{
    MMIPB_SEL_NODE_T*   sel_table_head;//每个handle对应的选项列表的表头
    MMIPB_SEL_NODE_T*   sel_table_tail;//每个handle对应的选项列表的表尾
    wchar  file_path[MMIFILE_FULL_PATH_MAX_LEN + 1];//用于记录导出 vcard的路径
    uint16 to_storage_id;//用于记录复制到位置的storage
    uint8  group_id;//group manage
    uint16 sel_count;//选择列表的节点数
    uint16 opc_count;//处理的是第几个选择列表节点
    uint16 error_count;//
}MMIPB_HANDLE_DATA_T;


typedef struct
{
    uint16 to_storage_id;//用于记录复制到位置的storage
    uint8  group_id;//group manage
    uint8  sel_field;
    uint16 entry_id;
    uint16 storage_id;
    uint16 num_index;
    wchar   file_path[MMIFILE_FULL_PATH_MAX_LEN + 1];//用于记录导出 vcard的路径
    uint8  is_in_group;//高四位：原来是否在群组中 1/0
                      //低四位：操作后是否在链表中1/0
    BOOLEAN is_manage_group;
}MMIPB_SEL_DATA;
//用户选择的文件信息
typedef struct
{
    uint32     total_file_count;//FILEARRAY中总的文件个数
    uint32     curent_index;    //当前操作的文件在FILEARRAY的位置
    FILEARRAY  file_array;     //保存所有要操作的文件的数组
}MMIPB_SELECT_FILE_T;

typedef struct 
{
    //MMI_HANDLE_T cmd_handle;//当前操作handle
    MMIPB_OPC_E  cmd;//当前操作类型
    MMIPB_HANDLE_T    handle;//每一个applet和一个用于操作的handle绑定，注意这个不是applet_handle
    MMIPB_SUB_FUNCTION_TYPE_E  app_type;//
    MMIPB_LIST_TYPE_E list_type;//signle, mutil, normal
    MMIPB_CONTACTS_TYP_E contacts_type;//当前群组列表页面 
    PHONEBOOK_STORAGE_E  sim_contact_type;//sim卡记录类型：主要是为了区分MSISDN, FDN, SDN，其他情况可以忽略该成员参数
    MMIPB_DISPLAY_MODE_TYPE_E display_mode; //单行还是双行
    MMIPB_LIST_FIELD_TYPE_E field_disp_type[MMIPB_TAB_MAX_NUM];//
    MMI_WIN_ID_T   active_win_id;//当前活动窗口
    uint16           cur_group_id;//当前所在的组别
    MMIPB_SEARCH_TYPE_T   search_type;
    uint16 		max_select_count;
    uint16      split_num;//
    wchar       search_str[MMIPB_MUL_SEARCH_MAX_CHAR+2];
    BOOLEAN     is_subStrSearch;//是否精准搜索(模糊搜索和精确搜索分开的情况)
    wchar       sub_search_str[MMIPB_MAX_STRING_LEN +1];//用于搜索模块进行联系人搜索时时使用
   // BOOLEAN     is_not_need_open_pb;
    MMIPB_FUNCTION_TYPE_E function_type;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    MMIPB_BCD_NUMBER_T    add_number_t;
    MMIPB_MAIL_T          add_mail_t;
    MMIPB_CUSTOM_TEXT_T   add_url;
    MMIPB_CUSTOM_PHOTO_T   add_pic;
    MMIPB_ADD_TYPE_T      add_type;
#endif
    MN_DUAL_SYS_E         dual_sys;
    uint16      cur_entry_id;
    uint16      cur_storage_id;
    uint16      cur_num_index;
    uint16      copy_from_index;
    uint16      copy_to_index;
    BOOLEAN     is_all_select;
    BOOLEAN     is_sns;
    BOOLEAN     is_need_pin_verify;//是否需要PIN2码，主要用在fdn操作，其他情况都为false
    BOOLEAN     is_need_protect_verify;//是否需要检查隐私保护
    #if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
    BOOLEAN     is_enter_from_callsetting;//是否从callsetting模块进入MSISDN
    #endif
    MMIPB_HANDLE_DATA_T *select_list;//选择的联系人的索引表
    MMIPB_SELECT_FILE_T select_files;//选择的文件表
    MMIPB_ENTRY_FUNC entry_func;
    MMIPB_RETURN_CALLBACK_FUNC selelect_callback;
    MMIPB_UPDATE_CALLBACK_FUNC   update_callback;
    
}MMIPB_INSTANCE_T;

typedef struct 
{
    CAF_APPLET_T caf_applet;
    MMIPB_INSTANCE_T instance;
}MMIPB_APPLET_T;   



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : 设置pb是否在隐私保护状态
//  Global resource dependence : 
//  Author: maryxiao
//  Note: //TRUE:已经解锁，FALSE:还没有解?
/*****************************************************************************/
PUBLIC void MMIPB_SetProtectStatus(BOOLEAN status);
/*****************************************************************************/
//  Description : 获得pb的隐私保护状态
//  Global resource dependence : 
//  Author: maryxiao
//  Note: //TRUE:已经解锁，FALSE:还没有解?
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetProtectStatus(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_SetCLUnProtect(BOOLEAN is_unprotect);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetCLUnProtect(void);
/*****************************************************************************/
//  Description : register PB  applet info
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIPB_StartApplet(MMIPB_INSTANCE_T *instance_ptr);

/*****************************************************************************/
//  Description : unregister PB  applet info
//  Global resource dependence : 
//  Author:maryxiao
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CloseApplet(MMI_HANDLE_T applet_handle);

/*****************************************************************************/
//  Description : init PB applet queue
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_InitAppletQueue(void);
/*****************************************************************************/
//  Description : PB applet queue
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMI_QUEUE_NODE_T *MMIPB_GetAppletQueue(void);

/*****************************************************************************/
//  Description : PB applet queue
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:applet_handle_ptr: OUT
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_GetCurAppletHandle(MMI_HANDLE_T* applet_handle_ptr);
/*****************************************************************************/
//  Description : 创建选择列表的节点
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:创建选择列表：
                        //for export  只需要给file_path参数的storage id 和 entry id赋值
                        //for copy  需要给to_storage_id复制， 以及需要给 contact 参数赋值
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_CreateSelNode(MMI_HANDLE_T handle,MMIPB_SEL_DATA* sel_data);
/*****************************************************************************/
//  Description : 删除选择列表头结点
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIPB_FindSelNode(MMI_HANDLE_T  handle, uint16 entry_id, uint16 storage_id, uint16 num_index);



/*****************************************************************************/
//  Description : 删除特定的选择链表节点
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIPB_DeleteSelNode(MMI_HANDLE_T  handle, uint16 entry_id, uint16 storage_id, uint16 num_index);
/*****************************************************************************/
//  Description : 得到选择链表的头结点
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_SEL_NODE_T* MMIPB_GetSelNode(MMI_HANDLE_T  handle, uint16 entry_id, uint16 storage_id, uint16 num_index);
/*****************************************************************************/
//  Description : 选择列表是否存在节点
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_HasSelNode(MMI_HANDLE_T  handle);
/*****************************************************************************/
//  Description : 删除选择列表头结点
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_DeleteSelHeadNode(MMI_HANDLE_T  handle);
/*****************************************************************************/
//  Description : 删除所有选择列表的节点
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DeleteAllSelNode(MMI_HANDLE_T  handle);
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : 保存用户选择的文件数组信息
//                Save selected files information
//  Global resource dependence : 
//  Author:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SaveSelectFileInfo(MMIPB_HANDLE_T handle,
                                        FILEARRAY file_array);

/*****************************************************************************/
//  Description : 获取选择的下一个文件的全路径
//  Global resource dependence : 
//  Author:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetNextSelectFileName(MMI_HANDLE_T handle,
                                           uint16 max_file_name_len,
                                           wchar *file_name_ptr);

/*****************************************************************************/
//  Description : 清除选择的文件数组信息
//  Global resource dependence : 
//  Author:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_CleanSelectFileInfo(MMIPB_HANDLE_T handle);
#endif

/*****************************************************************************/
//  Description : ADD  CONTACT
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContact(MMIPB_CONTACT_T *contact_ptr);
/*****************************************************************************/
//  Description : 删除图片id信息或铃声id文件  
//  Global resource
//  Author: 
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DeleteContactInfoFile(
                                           uint16 entry_id,
                                           uint16 storage_id,
                                           MMIPB_FILE_TYPE file_type//IN
                                           );


/*****************************************************************************/
//  Description : delete  CONTACT
//  Global resource dependence : none
//  Author: 
//  Note: 可作为接口，提供给其它模块调用
/*****************************************************************************/

PUBLIC MMIPB_ERROR_E MMIPB_DeleteContact(uint16 entry_id, uint16 storage_id);
/*****************************************************************************/
//  Description : MODIFY  CONTACT
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ModifyContact(MMIPB_CONTACT_T *contact_ptr);
/*****************************************************************************/
//  Description : 异步新增联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:给外部接口调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback);
/******************************************************************************/
//  Description : 给pb applet调用的异步新增联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactAsynForPB(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr);
/*****************************************************************************/
//  Description : 异步 编辑联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:给外部接口调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ModifyContactAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback);
/*****************************************************************************/
//  Description : 异步 编辑联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:给外部接口调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ModifyContactAsynForPB(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr);
/*****************************************************************************/
//  Description : 异步删除单个不再选择列表中的记录用于非多选状态
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ModifyUnListContact(MMIPB_HANDLE_T handle,MMIPB_CONTACT_T* contact_ptr);
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : 导出联系人
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ExportVcard(uint16 entry_id, uint16 storage_id, const wchar *path_name_ptr);
/*****************************************************************************/
// Description :异步导出联系人
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ExportFirstVcardAsyn(
                                        MMIPB_HANDLE_T handle, 
                                        uint16 entry_id, 
                                        uint16 storage_id, 
                                        const wchar *path_name_ptr, 
                                        MMIPB_ACCESS_CALLBACK_FUNC callback
                                        );

/*****************************************************************************/
// Description :异步导出下一个联系人
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ExportNextVcardAsyn(
                                        MMIPB_HANDLE_T handle, 
                                        uint16 entry_id, 
                                        uint16 storage_id, 
                                        MMIFILE_HANDLE file_handle, 
                                        MMIPB_ACCESS_CALLBACK_FUNC callback
                                        );

/*****************************************************************************/
// Description :导出结束
// Global resource dependence :
// Author
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ExportVcardEnd(
                                        MMIPB_HANDLE_T handle 
                                        );

/*****************************************************************************/
//  Description : 异步导出单个不再选择列表中的记录用于非多选状态
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ExportUnListContact(MMIPB_HANDLE_T handle,uint16 entry_id, uint16 storage_id, wchar* path);
/*****************************************************************************/
// Description : 异步导出联系人
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ExportContactAsynForPB(MMIPB_HANDLE_T handle, wchar* path);
/*****************************************************************************/
// Description : 导入联系人
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ImportVcard(
                                   // uint16      to_storage_id,      
                                    const wchar *path_name_ptr,//IN
                                    MMIPB_CONTACT_T* contact_ptr//OUT
                                    );
/*****************************************************************************/
// Description : 导入联系人
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SaveVcard(
                                    uint16      to_storage_id,      
                                    MMIPB_CONTACT_T* contact_ptr//IN
                                    );
/*****************************************************************************/
// Description : 异步导入联系人
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ImportVcardAsyn(MMIPB_HANDLE_T handle, //IN
                                           const wchar *path_name_ptr,//IN
                                           MMIPB_ACCESS_CALLBACK_FUNC callback
                                           );

/*****************************************************************************/
// Description : 异步导入联系人
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ImportContactAsynForPB(MMIPB_HANDLE_T handle);
#endif
/****************************************************************************
*/
//  Description : 异步删除联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:给外部接口调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteContactAsyn(
                                                MMIPB_HANDLE_T handle, 
                                                uint16 entry_id,
                                                uint16 storage_id, 
                                                MMIPB_ACCESS_CALLBACK_FUNC callback
                                                );
/*****************************************************************************/
//  Description : 异步删除单个不再选择列表中的记录用于非多选状态
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteUnListContact(MMIPB_HANDLE_T handle,uint16 entry_id, uint16 storage_id);
/*****************************************************************************/
//  Description : 给pb applet调用的异步删除联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteContactAsynForPB(MMIPB_HANDLE_T handle);
//  Description : 获得联系人信息
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetContactInfo(uint16 entry_id, uint16 storage_id, MMIPB_CONTACT_T* contact_ptr);
/*****************************************************************************/
//  Description : 复制联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_CopyContact(uint16 to_storage_id, uint16 from_entry_id, uint16 from_storage_id);
/*****************************************************************************/
//  Description : 异步复制联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:提供给外部接口调用
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_CopyContactAsyn(
                                              MMIPB_HANDLE_T handle,
                                              uint16         dest_storage_id,
                                              MMIPB_ACCESS_CALLBACK_FUNC callback
                                              );

/*****************************************************************************/
//  Description :写群组名字
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteGroupName(MMIPB_NAME_T* name,  uint8 group_id);
/*****************************************************************************/
//  Description : 异步删除单个不再选择列表中的记录用于非多选状态
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_CopyOneContactAsyn(MMIPB_HANDLE_T handle, 
                                              uint16 to_storage_id,
                                              uint16 entry_id, 
                                              uint16 storage_id,
                                              uint8  copy_field);

/*****************************************************************************/
//  Description : 给pb applet调用的异步复制联系人
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_CopyMultiContactAsyn(MMIPB_HANDLE_T handle, uint16 to_storage_id);
/*****************************************************************************/
//  Description :读取群组名字
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadGroupName(MMIPB_NAME_T* name,  uint8 group_id);
#ifdef MMIPB_MOST_USED_SUPPORT
 /*****************************************************************************/
//  Description : 收藏联系人
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactToFavourite(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T* contact_ptr);
 /*****************************************************************************/
//  Description : 收藏联系人
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactToFavouriteAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T* contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback);
 /*****************************************************************************/
//  Description : 收藏联系人
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactToFavouriteAsynForPB(MMIPB_HANDLE_T handle);

/*****************************************************************************/
//  Description : 异步add单个不再选择列表中的记录用于非多选状态
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddUnlistContactFromFavourite(MMIPB_HANDLE_T handle, uint16 entry_id, uint16 storage_id);
 /*****************************************************************************/
//  Description : 取消收藏联系人
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromFavourite(MMI_HANDLE_T applet_handle,MMIPB_CONTACT_T* contact_ptr);
 /*****************************************************************************/
//  Description : 取消收藏联系人
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromFavouriteAsyn(MMIPB_HANDLE_T handle,MMIPB_CONTACT_T* contact_ptr,MMIPB_ACCESS_CALLBACK_FUNC callback);
 /*****************************************************************************/
//  Description : 取消收藏联系人
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromFavouriteAsynForPB(MMIPB_HANDLE_T handle);
/*****************************************************************************/
//  Description : 异步删除单个不再选择列表中的记录用于非多选状态
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveUnlistContactFromFavourite(MMIPB_HANDLE_T handle, uint16 entry_id, uint16 storage_id);
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
 /*****************************************************************************/
//  Description : add to group
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactToGroup(MMIPB_HANDLE_T handle ,MMIPB_CONTACT_T* contact_ptr,uint8 group_id);
 /*****************************************************************************/
//  Description : add to group
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactToGroupAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T* contact_ptr, uint8 group_id,MMIPB_ACCESS_CALLBACK_FUNC callback);

 /*****************************************************************************/
//  Description : remove from group
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromGroup(MMIPB_HANDLE_T handle,MMIPB_CONTACT_T* contact_ptr,uint8 group_id );
 /*****************************************************************************/
//  Description :  remove from group
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromGroupAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T* contact_ptr, uint8 group_id,MMIPB_ACCESS_CALLBACK_FUNC callback);
 /*****************************************************************************/
//  Description : add to group
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ManageGroupContactAsynForPB(MMIPB_HANDLE_T handle, uint8 group_id);

 /*****************************************************************************/
//  Description : add to group
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ManageUnlistGroupContact(MMIPB_HANDLE_T handle, 
                                                    uint8 group_id, 
                                                    uint16 entry_id, 
                                                    uint16 storage_id, 
                                                    BOOLEAN is_in_group);
#endif
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
LOCAL void HandleDeleteContactResult(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error);
/*****************************************************************************/
//  Description :写群组名字
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_WriteGroupName(MMIPB_NAME_T* name,  uint8 group_id);
/*****************************************************************************/
//  Description :群组是否有效
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_IsUserGroupIdValid(uint32 group_id);
/*****************************************************************************/
//  Description :新增群组时获得可用的群组id
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetFreeUserGroupId(void);
/*****************************************************************************/
//  Description : 获得applet关联的选择列表的节点数,不区分group
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC uint16  MMIPB_GetSelectCount(MMIPB_HANDLE_T handle);

/*****************************************************************************/
// Description : 返回最大可以存储的nv contacts的数目
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMaxContactNumOfNV(void);
/*****************************************************************************/
// Description : 返回最大可以存储的sim contacts的数目
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMaxContactNumOfSim(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
// Description : 取得每个sim卡里面已存有的联系人个数
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetSimContactCount(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
// Description : 取得SIM卡或手机中每个组的记录数
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetContactNumOfGroup(uint16 group_index);

/*****************************************************************************/
//  Description : 初始化Phonebook的全局变量、回调函数等
//  Global resource dependence : g_mmipb_entry_list, g_mmipb_nv_t, g_mmipb_app
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_InitApp (
						void
						);
/*****************************************************************************/
//  Description : MMIAPIPB_InitSimStatus
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_InitSimStatus(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
//  Description : 发消息给task调用函数进行联系人的重新排序
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_BeginToSort(void);
/*****************************************************************************/
//  Description : Resort Phonebook,
//  Global resource dependence : 
//  Author: baokun.yin
//RETURN: 
//  Note: 这个命令会引起pb的立即重新排序，如果sim都ready，不需要等CALL_READY和SIM_NOT_READY
/*****************************************************************************/
PUBLIC void MMIPB_ResortPB(void);

/*****************************************************************************/
//  Description : Reset Group info and write to NV
//  Global resource dependence : s_mmipb_group_info
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_ResetGroupInfo(void);

/*****************************************************************************/
//  Description : register PB  applet info
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_RegAppletInfo(void);

/*****************************************************************************/
//  Description : re_initialize Phonebook,
//  Global resource dependence : 
//  Author: baokun.yin
//RETURN: 
//  Note: 这个命令会等ALL_READY和SIM_NOT_READY后再进行重排
//  [特别提示Warnning]:调用这个重排接口，一定要保证sim卡会重新发ALL_READY和SIM_NOT_READY，即sim会重新初始化
/*****************************************************************************/
PUBLIC void MMIPB_ReinitPB(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
//  Description : 进行联系人的排序
//  Global resource dependence : g_mmipb_entry_list
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_InitEntryList(void);
/*****************************************************************************/
//  Description : 获得电话簿是否可以被其它模块调用
//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsPbReady //RETURN: TRUE, 可以使用
(
   void
);

/*****************************************************************************/
//  Description : 判断给应用是否允许打开
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsPermitOpen(MMIPB_LIST_TYPE_E app_type);
#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : USIM的扩展信息是否已经读完
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsUSIMReady(void);
/*****************************************************************************/
//  Description : 要读取的USIM的扩展信息
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_ReadUSIMExtCnf(APP_MN_READ_ADDITIONAL_ENTRY_CNF_T *usim_ext_ptr);
/*****************************************************************************/
//  Description : 该函数用于处理接收到的APP_MN_UPDATE_USER_INFO_CNF消息
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_USIMUpdateCnf(APP_MN_UPDATE_USER_INFO_CNF_T *update_result);
/*****************************************************************************/
//  Description : 该函数用于处理接收到的APP_MN_DEL_USER_INFO_CNF消息
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_USIMDeleteCnf(APP_MN_UPDATE_USER_INFO_CNF_T *delete_result);
#endif
/*****************************************************************************/
//  Description : 获取PB的状态
//  Global resource dependence : none
//  Note: 
//  author: baokun.yin
/*****************************************************************************/
PUBLIC void MMIPB_SetPBStatus(MMIPB_STATUS_E status);
/*****************************************************************************/
//  Description : 设置pb状态
//  Global resource dependence : none
//  Note: 
//  author: baokun.yin
/*****************************************************************************/
PUBLIC MMIPB_STATUS_E MMIPB_GetPBStatus(void);
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 设置电话号码的被使用

//  Note: 更新该号码的使用频率。
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SetNumberUsed(MMIPB_BCD_NUMBER_T *number_ptr);
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
PUBLIC BOOLEAN  MMIPB_GetSelectContact(MMIPB_HANDLE_T handle,MMIPB_CONTACT_T * contact_ptr, uint16 index, uint16 * num_index);
/*****************************************************************************/
//  Description : 读取搜索到的第index+1个联系人姓名。
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_GetSelectName(MMIPB_HANDLE_T handle,MMI_STRING_T *name_str, uint16 max_name_len,uint16 index);
/*****************************************************************************/
//  Description : 读取搜索到的第index+1个联系人email。
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

PUBLIC BOOLEAN  MMIPB_GetSelectEmail(MMIPB_HANDLE_T handle,MMI_STRING_T *mail_ptr, uint16 max_mail_len,uint16 index);
/*****************************************************************************/
//  Description : 读取搜索到的第index+1个联系人号码。
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_GetSelectNumber(MMIPB_HANDLE_T handle, MMIPB_BCD_NUMBER_T *nameber_ptr, uint16 max_number_len,  uint16 index);
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : 读取搜索到的第index+1个联系人vcard。
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_GetSelectVcard(MMIPB_HANDLE_T handle, uint8  *data_ptr, uint16 max_buf_len,  uint16 index);
#endif
/*****************************************************************************/
//  Description :   打断PB
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_AbortPB(BOOLEAN is_abort);

/*****************************************************************************/
//  Discription: This function is used to send signal from mmi to mmi
//   
//  Global resource dependence: None
//  Author: baokun yin
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC  void MMIPB_SendSignal(uint32 sig_id);

/*****************************************************************************/
//  Description : 从BYTE中获得NUMBER PLAN
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
MN_NUMBER_PLAN_E MMIPB_GetNumberPlanFromUint8
(
 uint8   npi_ton     //IN:
 );


/*****************************************************************************/
//  Description : send sms
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SendSMSByNumber(
					   MMIPB_BCD_NUMBER_T number_t
					  );

/*****************************************************************************/
//  Discription: is sns group
//  Global resource dependence: none 
//  Author: maryxiao
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsSNSGroup(uint16 group_id);

/*****************************************************************************/
//  Description : 把sim卡位置映射为group id
//  Global resource dependence : none
//  Author: Michael.Shi
//  Note: 
/*****************************************************************************/
PUBLIC PB_GROUP_LIST_E MMIPB_GetSimGroupID(MN_DUAL_SYS_E dual);

/*****************************************************************************/
//  Description : get contact id and storage id by index
//  Global resource dependence : none
//  Author: Michael.Shi
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetContactIDandStorageID(uint8 group_id,
                                              uint16 index,
                                              uint16 *contact_id_ptr,//[OUT]
                                              uint16 *storage_id_ptr//[OUT]
                                              );

/*****************************************************************************/
//  Description : get custom photo path name
//  Global resource dependence : none
//  Author: 
//  Note:	path_ptr length should be larger than MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetCustomPhotoPathByNumber(
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
PUBLIC BOOLEAN MMIPB_GetCustomRingFileByNumber(
											  MMIPB_BCD_NUMBER_T *bcd_num_ptr,
											  BOOLEAN  *is_fixed_id,
											  wchar	   *path_ptr,
											  uint32   *path_len_ptr,
											  uint32   *fixed_id
											  );

#ifdef ASP_SUPPORT
/*****************************************************************************/
//  Description : get custom photo path name
//  Global resource dependence : none
//  Author: 
//  Note:	path_ptr length should be larger than MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetCustomASPFileByNumber(
											  MMIPB_BCD_NUMBER_T *bcd_num_ptr,
											  BOOLEAN  *is_fixed_id,
											  wchar	   *path_ptr,
											  uint32   *path_len_ptr,
											  uint32   *fixed_id
											  );
#endif
/*****************************************************************************/
//  Description : 保存组
//  Global resource dependence : s_mmipb_group_info
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SaveGroup  //RETURN: TRUE，操作成功; FALSE，操作失败
(
 MMIPB_NAME_T    *group_name_ptr,    //IN: 群组的命名
 uint8	        group_id ,           //IN: 群组的id号
 BOOLEAN             is_valid
 );
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
 /*****************************************************************************/
//  Description :  
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SendGroupMsg(MMIPB_HANDLE_T handle);
/*****************************************************************************/
//  Description :get used phone group count
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetUsedGroupIdInfor(uint8 *id_ptr);
#endif
/*****************************************************************************/
//  Description :设置群组
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:IN :group_id
        //OUT: group_ptr
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SetGroupInfo( uint8 group_id, MMIPB_GROUP_T* group_ptr);

/*****************************************************************************/
//  Description :读取群组
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetGroupInfo( uint8 group_id, MMIPB_GROUP_T* group_ptr);

 /*****************************************************************************/
//  Description : delete all pb info 
//  Author: 
//  Note: for dm cmcc 
/*****************************************************************************/
PUBLIC void MMIPB_DeleteAllPBInfo(void);
/*****************************************************************************/
//  Description : APP_MN_SIM_NOT_READY_IND
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_HandlePbSimNotReadyInd(DPARAM  param);

/*****************************************************************************/
//  Description : Make call
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_CallNumber
(
    MN_DUAL_SYS_E dual_sys,
    MMIPB_CALL_TYPE_E  interface_type, //IN:
    uint8                *call_number_ptr, //IN:
    MMI_STRING_T         *call_name_ptr//IN:contact_name 
);
#ifdef  SNS_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:IN :group_id
        //OUT: group_ptr
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetSnsContactInfo(uint16 sns_type, 
                                             uint16 index, 
                                             MMIPB_CONTACT_T* contact_ptr );
/*****************************************************************************/
//  Description : 更新sns联系人
//  Global resource dependence : 
//  Author:starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_LoadSnsContact(void);

/*****************************************************************************/
//  Description : : 向PB task发送消息,更新sns联系人
//  Global resource dependence : 
//  Author: starmer.sun
//  Note
/*****************************************************************************/
PUBLIC void MMIPB_StartUpdateSnsContact(MMI_HANDLE_T applet_handle);
/*****************************************************************************/
//  Description : 更新sns联系人
//  Global resource dependence : 
//  Author:starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UpdateSns(void);
#endif
/*****************************************************************************/
//  Description : get storage free count
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetStorageFreeSpace(uint16 storage_id, 
                                               uint16 *free_num_ptr);
/*****************************************************************************/
//  Description :获得自定义群组个数
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetUserGroupCount(void);
/*****************************************************************************/
//  Description :make call by bcdnumber
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_CallBCDNumber(
									  MN_DUAL_SYS_E dual_sys,
    MMIPB_CALL_TYPE_E call_type,
    MMIPB_CONTACT_T *contact_ptr,
    uint16 num_index
	);
/*****************************************************************************/
//  Description : send mms by bcd number
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC void MMIPB_SendMMSByBcdNumber(MMIPB_BCD_NUMBER_T bcd_number);

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : Read vcard
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadVcardToContact(
    wchar  *file_name,//IN
    MMIPB_CONTACT_T *contact_ptr//OUT
    );

/*****************************************************************************/
//  Description : send vcard by sms or bt and so pn
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/***************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SendVcard( 
    MMIPB_SEND_TYPE_E send_type,
    MMIPB_CONTACT_T *contact_ptr
    );
#endif

/*****************************************************************************/
//  Description :获取MSISD的數目
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMSISDNum(void);

/*****************************************************************************/
//  Description :获取MSISDNumber
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetMSISDNumberArray(MMIPB_BCD_NUMBER_T *number, int16 max_num);
/*****************************************************************************/
//  Description :获取指定SIM的MSISDNumber
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetMSISDNumberArrayBySim(MMIPB_BCD_NUMBER_T *number,MN_DUAL_SYS_E dual_sys, int16 max_num);

/*****************************************************************************/
// Description : 返回最大可以存储的contacts的数目
// Global resource dependence :
// Author:maryxiao
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMaxContactNum(void);

/*****************************************************************************/
//  Description : Get Contact Number
//  Global resource dependence : none
//  Author:
//  Note: 
/***************************************************************************/
PUBLIC uint16 MMIPB_GetContactNumberNum(    MMIPB_CONTACT_T* contact_ptr, 
    uint16* index_ptr);

/*****************************************************************************/
//  Description :get phone nunber by bcd_number
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetPhoneNumberByBcdNumber(
    MMIPB_BCD_NUMBER_T  bcd_number,
    uint8* tele_num,
    uint16 len);

    
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 删除全部常用联系人
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/

PUBLIC MMIPB_ERROR_E MMIPB_DeleteAllMostUsedContact(void);
#endif
/*****************************************************************************/
//  Description : Get pb status whether it can add contact 
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note: 
/***************************************************************************/
PUBLIC BOOLEAN MMIPB_PbIsFull(void);

#ifdef MCARE_V31_SUPPORT

PUBLIC uint16 MCARE_MMIPB_GetUsedRecordNumByStorage(uint16 storage_id);
PUBLIC uint16 MCARE_MMIPB_GetUsedContactNumOfNV(void);

PUBLIC uint16 MCARE_MMIPB_GetMaxRecordNumByStorage(uint16 storage_id);

#endif//#ifdef MCARE_V31_SUPPORT

/*****************************************************************************/
//  Description : THE INTERFACE for autotest only
//  Global resource dependence : none
//  Author:maryxiao
//  Note: 
/***************************************************************************/
PUBLIC BOOLEAN MMIPB_FillPhoneContactFull(void);
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : judge whether export contact isnot
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsExportContactOpc(void);
#endif

#ifdef BT_PBAP_SUPPORT
/*******************************************************************************/
//  Description :联系人同步，添加至通讯录
//  Parameter: contact_ptr[in]: 记录相关信息
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIBT_AddContact(MMIPB_CONTACT_T *contact_ptr);
/*******************************************************************************/
//  Description : 建立带头节点的单链表，返回头结点
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_InitPBAPDataList(void);
/*******************************************************************************/
//  Description :解析vcard buffer data  
//  Parameter: data_ptr[in]: 用于保存UTF-8编码的vCard条目数据，编码符合vCard2.1规范的规定。
//			   data_len[in]: 该条vCard记录的大小
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
PUBLIC void MMIPB_NotifyPBStartParser(MMIPB_PBAP_PARSER_CALLBACK_FUNC callback);
/*****************************************************************************/
//  Description : insert PBAP data into link list
//  Global resource dependence : none
//  Author: Allen.Lee
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_InsertPBAPDataToList(void* buff,uint16 buff_len);
/*****************************************************************************/
//  Description : parse pbap link list data
//  Global resource dependence : none
//  Author: Allen.Lee
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ParsePbapData(void);
/*****************************************************************************/
//  Description : 清除全部蓝牙联系人
//  Global resource dependence : none
//  Author: Yanyan.an
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ClearAllBtContactForPbap(void);
/*****************************************************************************/
//  Description :清除全部蓝牙联系人
//  Global resource dependence : none
//  Author: Yanyan.an
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ClearAllBtContact(MMIPB_APPLET_T* applet_ptr);
/*******************************************************************************/
//  Description : free all pbap list buffer
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMIPB_FreeAllPbapData(void);

/*****************************************************************************/
//  Description : handle name sort in pb
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_HandleSortCnf();
#endif
#ifdef   __cplusplus
    }
#endif

#endif


