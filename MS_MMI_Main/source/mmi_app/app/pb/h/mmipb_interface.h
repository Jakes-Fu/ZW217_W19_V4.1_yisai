/*****************************************************************************
** File Name:      mmipb_interface.h                                          *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file  define synchronous and asynchronous access interface,
**                 management of this interface and handles, and data tables.
**                 这个文件主要是定义了主要同步和异步访问的接口，这个访问接口的管理，
**                 handle的管理，数据表的管理等                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2004        baokun yin       Create
******************************************************************************/

#ifndef _MMIPB_INTERFACE_H_
#define _MMIPB_INTERFACE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_custom_define.h"
#include "mmipb_common.h"
#include "mmipb_adapt.h"
#include "mmipb_datalist.h"
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

typedef uint32  MMIPB_HANDLE_T;

#define MMIPB_MAX_HANDLE_NUM  30 //系统中最多允许30个handle共存

#define MMIPB_MAX_WAIT_CMD_NUM 10 //系统中最多允许10个消息等待

//PB模块的主要操作类型
typedef enum
{
    MMIPB_OPC_ADD= 0x1,//添加
    MMIPB_OPC_DELETE= 0x2,//删除
    MMIPB_OPC_EDIT= 0x3,//编辑
    MMIPB_OPC_COPY= 0x4,//复制
    MMIPB_OPC_EXPORT= 0x5,//导入
    MMIPB_OPC_IMPORT= 0x6, //导出
    MMIPB_OPC_FIND  = 0x7, //查找
    MMIPB_OPC_ADD_FAVOURITE  = 0x8, 
    MMIPB_OPC_REMOVE_FAVOURITE  = 0x9,
    MMIPB_OPC_MANAGE_GROUP  = 0xa,
#ifdef  SNS_SUPPORT
    MMIPB_OPC_GET_SNS  = 0xb,
    MMIPB_OPC_UPDATE_SNS_ACTIVITY =0xc,
#endif
    MMIPB_OPC_ADD_MOSTUSED  = 0xd, 
}MMIPB_OPC_E; 

typedef enum
{
    MMIPB_ACCESS_CONTACT,          //访问记录
#ifdef CMCC_VCARD_SUPPORT
    MMIPB_ACCESS_USIM_AAS,          //访问usim扩展号码类型
#endif
#ifdef MMI_VCARD_SUPPORT
    MMIPB_ACCESS_VCARD,            //访问vcard 
#endif
    MMIPB_ACCESS_GROUP,            //访问分组
#ifdef SNS_SUPPORT    
    MMIPB_ACCESS_SNS,
#endif    
    MMIPB_ACCESS_NUM
}MMIPB_ACCESS_E;

//读写删除类型
#define MMIPB_READ_MODE           0x1//读模式
#define MMIPB_WRITE_MODE          0x2//写模式
#define MMIPB_DELETE_MODE         0x4//删除模式

//联系人ID信息，可以唯一标志该联系人。
typedef struct _MMIPB_CONTACE_ID_T
{
    uint16  contact_id;        
    uint16  storage_id;            
}MMIPB_CONTACE_ID_T;

//sns联系人ID信息，可以唯一标志该联系人。
typedef struct _MMIPB_SNS_ID_T
{
    uint16  index; //索引       
    uint16  sns_type;//sns类型            
}MMIPB_SNS_ID_T;

#ifdef CMCC_VCARD_SUPPORT
//联系人扩展号码类型位置信息。
typedef struct _MMIPB_AAS_INDEX_T
{
    MN_DUAL_SYS_E  dual_sys;        
    uint16         aas_index;            
}MMIPB_AAS_INDEX_T;
#endif

//数据访问点
typedef union
{
    MMIPB_CONTACE_ID_T contact;//记录id信息
#ifdef CMCC_VCARD_SUPPORT
    MMIPB_AAS_INDEX_T  aas_index_t;//扩展号码类型位置信息
#endif
    uint16  list_index;//记录缓冲表的位置
    uint16  group_id;//分组
    MMIFILE_HANDLE file_handle;//打开的文件句柄
    //wchar   *file_path_ptr; //文件访问的全路径   
    MMIPB_SNS_ID_T  sns_contact;
} MMIPB_ACCESS_POINT_T;

#ifdef BT_PBAP_SUPPORT
typedef enum _MMIPB_PBAP_PARSER_RESULT_E
{
    MMIPB_PBAP_PARSER_FINISHED,
    MMIPB_PBAP_PARSER_NO_SPACE,
    MMIPB_PBAP_PARSER_CANCELED   
}MMIPB_PBAP_PARSER_RESULT_E;
typedef struct _MMIPB_PBAP_PARSER_RESULT_T
{
    MMIPB_PBAP_PARSER_RESULT_E ret;
    uint16 sync_count;
}MMIPB_PBAP_PARSER_RESULT_T;
#endif

typedef void(*MMIPB_RETURN_CALLBACK_FUNC)(MMIPB_HANDLE_T handle);
typedef  void(*MMIPB_ACCESS_CALLBACK_FUNC) (MMIPB_HANDLE_T handle, MMIPB_ERROR_E error);

#ifdef BT_PBAP_SUPPORT
typedef void(*MMIPB_PBAP_PARSER_CALLBACK_FUNC)(MMIPB_PBAP_PARSER_RESULT_T *result);
#endif

//handle对应的数据结构
typedef struct  
{
    MMIPB_OPC_E         cmd_type;//操作类型
    uint32              group_mask;//群组
    MMIPB_ACCESS_E      access_type;//访问类型
    MMIPB_ACCESS_POINT_T access_point;//访问位置
    void                *data_ptr;//操作数据指针
    uint32              data_len;//数据长度
    MMIPB_ACCESS_CALLBACK_FUNC callback;//异步操作的回调函数
    MMI_HANDLE_T        applet_handle;//applet handle
}MMIPB_HANDLE_NODE_T;

//handle数组类型
 typedef struct
{
    uint16             handle_count;//Handle的数目
    uint16             cur_index;//Handle的当前索引
    MMIPB_HANDLE_NODE_T *handle_table[MMIPB_MAX_HANDLE_NUM];//Handle数组
}MMIPB_HANDLE_ARRAY_T;

 //等待执行的异步操作队列节点
 typedef struct
 {
     uint32               pb_mode;     //模式：读，写或者删除模式
     MMIPB_HANDLE_T       cmd_handle; //句柄
 }MMIPB_MSG_QUEUE_NODE_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 接口层初始化
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_InitInterface(void);

/*****************************************************************************/
//  Description :等待消息队列的初始化
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_InitWaitMsgQueue(void);


/*****************************************************************************/
//  Description : 操作是否是同一应用发起
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsCurrentOperation(
                              uint16 contact_id,
                              uint16 storage_id
                              );


/*****************************************************************************/
//  Description : 创建同步的event
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CreateEvent(void);

/*****************************************************************************/
//  Description : 设置写错误类型
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_SetWriteError(MMIPB_ERROR_E error);

/*****************************************************************************/
//  Description : 读写错误类型
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetWriteError(void);

/*****************************************************************************/
//  Description : 设置删除错误类型
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_SetDeleteError(MMIPB_ERROR_E error);

/*****************************************************************************/
//  Description : 读删除错误类型
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetDeleteError(void);

/*****************************************************************************/
//  Description : 同步写的confirm
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_WriteTrigger(
                           uint16 contact_id,
                           uint16 storage_id,
                           MMIPB_ERROR_E error
                           );

/*****************************************************************************/
//  Description : 同步删除的confirm
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_DeleteTrigger(
                           uint16 contact_id,
                           uint16 storage_id,
                           MMIPB_ERROR_E error
                           );

/*****************************************************************************/
//  Description : 信息读取
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_Read (
                          MMIPB_ACCESS_E access_type,//[IN]信息类型
                          MMIPB_ACCESS_POINT_T access_point,//[IN]信息位置信息
                          uint32 data_len,//[IN]buf最大长度
                          void *out_data_ptr//[OUT]读取的数据
                          );

/*****************************************************************************/
//  Description : 写信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_Write (
                           MMIPB_ACCESS_E access_type,//[IN]信息类型
                           MMIPB_ACCESS_POINT_T access_point,//[IN]信息位置信息
                           uint32 data_len,//[IN]写入的数据长度
                           void *write_data_ptr//[IN]写入的数据
                           );


/*****************************************************************************/
//  Description : 删除信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_Delete(
                            MMIPB_ACCESS_E access_type,
                            MMIPB_ACCESS_POINT_T access_point
                            );
#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : 删除全部常用联系人
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_CleanAllMostUsedContact(void);
#endif
/*****************************************************************************/
//  Description : 创建PB的Handle
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_HANDLE_T MMIPB_CreateHandle (
                           MMIPB_OPC_E  cmd_type,
                           MMI_HANDLE_T applet_handle
                           );

/*****************************************************************************/
//  Description : 销毁PB的Handle
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CloseHandle (
                               MMIPB_HANDLE_T handle
                               );

/*****************************************************************************/
//  Description : 获取handle所对应的结构体指针
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetHandleContent(                                       
                                       MMIPB_HANDLE_T handle,//[IN]
                                       MMIPB_HANDLE_NODE_T *node_ptr//[OUT]
                                       );

/*****************************************************************************/
//  Description : 设置handle所对应的结构体值
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SetHandleContent(MMIPB_HANDLE_T handle, MMIPB_HANDLE_NODE_T *handle_node_ptr);

/*****************************************************************************/
//  Description : 异步信息读取
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:目前仅用于记录和vcard的读取
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadAsyn (
                              MMIPB_HANDLE_T handle,//[IN]handle id
                              MMIPB_ACCESS_E access_type,//[IN]访问类型
                              MMIPB_ACCESS_POINT_T access_point,//[IN]信息位置信息
                              void *data_ptr,//[OUT]数据指针
                              uint32 data_len,//[IN]数据的最大长度
                              MMIPB_ACCESS_CALLBACK_FUNC callback//[IN]回调函数
                              );

/*****************************************************************************/
//  Description : 异步写信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:目前仅用于记录和vcard的保存
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteAsyn (
                               MMIPB_HANDLE_T handle,
                               MMIPB_ACCESS_E access_type,
                               MMIPB_ACCESS_POINT_T access_point,
                               void *data_ptr,
                               uint32 data_len,
                               MMIPB_ACCESS_CALLBACK_FUNC callback
                               );

/*****************************************************************************/
//  Description : 异步信息删除
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:目前仅用于记录的异步删除
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteAsyn (
                                MMIPB_HANDLE_T handle,
                                MMIPB_ACCESS_E access_type,
                                MMIPB_ACCESS_POINT_T access_point,
                                MMIPB_ACCESS_CALLBACK_FUNC callback
                                );

/*****************************************************************************/
//  Description : 异步操作后的处理
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_CmdConfirm (
                              MMIPB_HANDLE_NODE_T *handle_node_ptr,//IN
                              MMIPB_HANDLE_T handle,//[IN]handle id
                              MMIPB_ERROR_E  result
                              );

/*****************************************************************************/
//  Description : 获取当前的handle id
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_HANDLE_T MMIPB_GetCurrentHandle(void);


/*****************************************************************************/
//  Description : 联系人信息保存到表或数据库
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertContactToList( 
                                               MMIPB_CONTACT_T *contact_ptr
                                               );

/*****************************************************************************/
//  Description : 从表或数据库中删除一个节点
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromList(
                            uint16 contact_id,
                            uint16 storage_id
                            );

/*****************************************************************************/
//  Description : 获得num_type对应的类型字符串
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetNumberTypeString(uint16 storage_id,
    uint8  num_type,  //IN:
    MMI_STRING_T *type_str//OUT
);

/*****************************************************************************/
//  Description : 获得某一类型号码的最大长度
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetNumberMaxLen(uint16 storage_id,                                  
                                   uint8 num_type,
                                   uint8 orignal_len//该号码原来的长度，是新增的号码填0
                                  );

/*****************************************************************************/
//  Description : 获得联系人对应字段的最大alpha长度
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetMaxAlphaLen(uint16 storage_id,                                  
                                  MMIPB_CONTACT_DOMAIN_E feild//联系人域
                                  );

/*****************************************************************************/
//  Description : 获得联系人对应字段的最大wstr长度
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetMaxWstrLen(uint16 storage_id,//                                   
                                  MMIPB_CONTACT_DOMAIN_E feild//联系人域
                                 );

/*****************************************************************************/
//  Description : 所有普通联系人个数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllNormalContactCount(void);
#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : 所有常用联系人个数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllMostUsedContactCount(void);
#endif
/*****************************************************************************/
//  Description : 所有普通电话总数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllNormalContactPhoneCount(void);

/*****************************************************************************/
//  Description : 所有普通mail总数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllNormalContactMailCount(void);

/*****************************************************************************/
//  Description : 某个存储设备中联系人个数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetContactCount 
(
    uint16 storage_id
);
#ifdef MMIPB_IM_MESSAGE_SUPPORT
/*****************************************************************************/
//  Description : 获得instacemsg_type对应的类型字符串
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetImTypeString(
    uint8  im_type,  //IN:
    MMI_STRING_T *type_str//OUT
);
#endif

/*****************************************************************************/
//  Description : 获得instacemsg_type对应的类型字符串
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIPB_GetImTypeTextId(uint8  im_type );
/*****************************************************************************/
//  Description : 某个分组中联系人个数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetGroupContactCount
(
    uint16 group_id
);


/*****************************************************************************/
//  Description : 某个分组中电话个数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetGroupContactPhoneCount
(
    uint16 group_id
);

/*****************************************************************************/
//  Description : 某个分组中mail个数
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetGroupContactMailCount
(
    uint16 grp_id
);

/*****************************************************************************/
//  Description : Delete group all contacts from list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveOneGroupContactsFromList(uint32 group);

/*****************************************************************************/
//  Description : 查找第一个可见的联系人
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindFirstContact(
                                            uint32  group_mask,//[IN]
                                            uint16 storage_id, //要查找的联系人类型
                                            uint16  *cur_index_ptr,//[OUT]
                                            MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                                            MMIPB_CONTACT_BASE_INFO_T *find_data_ptr,//[OUT] 
                                            MMIPB_HANDLE_T *handle_ptr//[OUT]
                                            );
#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : 查找第一个可见的常用联系人
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindFirstMostUsedContact(      
                                            uint16  *cur_index_ptr,//[OUT]
                                            MMIPB_MOSTUSED_CONTACT_NODE_T *find_data_ptr,//[OUT] 
                                            MMIPB_HANDLE_T *handle_ptr//[OUT]
                                            );
#endif
/*****************************************************************************/
//  Description : 关闭一个查找
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindClose(
                                     MMIPB_HANDLE_T handle_ptr//[IN]
                                     );


/*****************************************************************************/
//  Description : 查找下一个可见记录
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindNextContact( MMIPB_HANDLE_T handle, 
                                            uint32  group_mask,//[IN]
                                            uint16  storage_id, //要查找的联系人类型
                                            uint16  *cur_index_ptr,//[OUT]
                                            MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                                            MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT] 
                                           );
#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : 查找下一个可见常用联系人记录
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindNextMostContact(MMIPB_HANDLE_T handle, 
                                              uint16  *cur_index_ptr,//[OUT]
                                              MMIPB_MOSTUSED_CONTACT_NODE_T *find_data_ptr//[OUT] 
                                              );
#endif
/*****************************************************************************/
//  Description : 普通联系人的基本信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_CONTACT_BASE_INFO_T MMIPB_GetContactBaseInfo(uint16 index);

/*****************************************************************************/
//  Description : 手机联系人的基本信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_CONTACT_BASE_INFO_T MMIPB_GetPhoneContactBaseInfo(uint16 index);

#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : 常用联系人的基本信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_MOSTUSED_CONTACT_NODE_T MMIPB_GetMostUsedContactBaseInfo(uint16 index);
#endif
/*****************************************************************************/
//  Description : 获得联系人对应字段的空间使用情况
//  Global resource dependence :
//  Author: 
//  Note: 每个字段相应如果有空间，设置为TRUE，否则为FALSE
//        通过该函数可以获取各个号码字段，mail字段，别名字段的使用情况
//  Return:如果contact_id为0，表示新增，返回一个新的id
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetFieldUageInfo(
                                 uint16 storage_id,//                                   
                                 uint16 contact_id,//联系人域
                                 MMIPB_FIELD_USAGE_INOF_T *free_ptr
                                 );
/*****************************************************************************/
//  Description : get max number num of contact bcdnumber
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetMaxNumbernum(uint16 storage_id,uint8 *number_num);

/*****************************************************************************/
//  Description : get all phone's group mask
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetAllPhoneGroup(void);

/*****************************************************************************/
//  Description : get all sim's group mask
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetAllSIMGroup(void);

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : get all sns's group mask
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetAllSNSGroup(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 GetGroupIdBySnsType(uint16 sns_type);

/*****************************************************************************/
//  Description : 打开sns数据库
//  
//  NOTE: 
/*****************************************************************************/
PUBLIC void MMIPB_OpenSNSDB(uint16 sns_type);

/*****************************************************************************/
//  Description : close sns database
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CloseSNSDB(void);
#endif



/*****************************************************************************/
//  Description : 设置pb是否忙状态
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_SetTaskStatus(BOOLEAN is_busy);

#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : 读取usim 支持的扩展号码的类型个数
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIPB_ReadUSIMAASCount(
                           MN_DUAL_SYS_E dual_sys
                           );

/*****************************************************************************/
//  Description : 异步读取USIM 扩展信息
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadUSIMAddtionalAsyn(
                                           MN_DUAL_SYS_E dual_sys,
                                           uint16        contact_id
                                           );

/*****************************************************************************/
//  Description : USIM 扩展信息保存到索引表中和号码查找表中
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SaveUSIMAddtionContact(                                       
                                            uint16       contact_id,
                                            uint16       storage_id,
                                            MMIPB_CONTACT_T *contact_ptr
                                           );
/*****************************************************************************/
//  Description : 读取usim 支持的扩展号码的可用个数
//  Global resource dependence : 
//  Author:yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIPB_ReadUSIMFreeANRCount(
                           MN_DUAL_SYS_E dual_sys
                           );

#endif

#ifdef BT_PBAP_SUPPORT 
/*****************************************************************************/
//  Description : 所有蓝牙联系人个数
//  Global resource dependence : 
//  Author:Yanyan.an 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllBtContactCount(void);
/*****************************************************************************/
//  Description : 查找第一个可见的蓝牙联系人
//  Global resource dependence : 
//  Author:Yanyan.an 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindFirstBtContact(
											  uint16  *cur_index_ptr,//[OUT]
											  MMIPB_CONTACT_BASE_INFO_T *find_data_ptr,//[OUT] 
											  MMIPB_HANDLE_T *handle_ptr//[OUT]
											  );
/*****************************************************************************/
//  Description : 查找下一个可见的蓝牙联系人
//  Global resource dependence : 
//  Author:Yanyan.an 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindNextBtContact(MMIPB_HANDLE_T handle, 
											 uint16  *cur_index_ptr,//[OUT]
											 MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT] 
											 );
#endif
#ifdef   __cplusplus
    }
#endif

#endif


