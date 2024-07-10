/****************************************************************************
** File Name:      mmimms_file.h                                           *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

#ifndef _MMIMMS_FILE_H
#define _MMIMMS_FILE_H

/**---------------------------------------------------------------------------*
 **                         Include Files                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmimms_edit.h"
#include "mmimms_net.h"
#include "mmimms_export.h"
#include "os_api.h"
//#include "mMmsProvide.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#if !defined WIN32 && !defined CHIP_ENDIAN_LITTLE
#define MMIMMS_LIST_VER		        "MMSVER.00.00.03_BE"  //存储文件格式版本号
#else
#define MMIMMS_LIST_VER		        "MMSVER.00.00.03_LE"  //存储文件格式版本号
#endif
#define MMIMMS_LIST_VER_LEN	        18

#define MMIMMS_FILE_NAME_LEN        MMS_FILE_NAME_LEN                  //彩信存储文件名长度
#define MMIMMS_BLOCK_WATERMARK      (20*1024)
#define MMIMMS_FILE_CREATE_COUNT    10
#define MMIMMS_1_HOUR_SECONDS       3600 
#define MMIMMS_12_HOUR_SECONDS      (3600 * 12)
#define MMIMMS_1_DAY_SECONDS        (3600 * 24)
#define MMIMMS_1_WEEK_SECONDS       (3600 * 24 * 7)
#define MMIMMS_BUFFER_LIST_NUM       50  
#define LOCAL_STR_HUNDRED_LEN       100

#define MMIMMS_ALLOC(size)   MMIMMS_AllocInBlock(size, __FILE__, __LINE__)
//#define MMIMMS_ALLOC         SCI_ALLOC_APP
#define MMIMMS_FREE(ptr)     (MMIMMS_FreeInBlock(ptr), ptr=PNULL)
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
//MMS LIST文件head
typedef struct MMIMMS_LIST_FILE_HEAD_T_tag
{
    uint8               version[MMIMMS_LIST_VER_LEN+1];
    uint32              total_num;
}MMIMMS_LIST_FILE_HEAD_T;

typedef struct MMIMMS_MSG_DELETEMMS_END_T_tag
{
    int32		   record_id;
    MMIMMS_ERROR_E delete_error;
}MMIMMS_MSG_DELETEMMS_END_T;

typedef struct MMIMMS_AUTOLIST_T_tag
{
    MMIMMS_SENDRECV_TYPE_E          send_type;    //TRUE send; FALSE download
    int32                           record_id;
    MN_DUAL_SYS_E                   dual_sys;
    struct MMIMMS_AUTOLIST_T_tag    *next;
}MMIMMS_AUTOLIST_T;

typedef struct MMIMMS_LISTINFO_T_tag
{
    MMISMS_ORDER_INFO_T *list_head_ptr;             //所有彩信链表
    MMIMMS_AUTOLIST_T   *autolist_head_ptr;         //自动下载队列彩信链表
    MMIMMS_AUTOLIST_T   *manuallist_head_ptr;       //手动收发队列链表
    uint32              total_num;                  //彩信总数
    uint32              unsettled_num;              //未下载彩信数目
    uint32              unread_num;                 //未读彩信数目
}MMIMMS_LISTINFO_T;

 
typedef struct MMIMMS_PUSH_FILE_T_tag
{
    uint32  size;                                   //彩信大小
    uint32  expiry_time;		                    //expiry time
    uint8   url[MMS_MAX_URL_LENGTH+1];                    //MMS所在的URL
    uint8   sender[MMIMMS_MAX_ADDRESS_LEN+1];       //发件人
    wchar   subject[MMIMMS_MAX_SUBJECT_LEN];      //MMS主题
    uint16  subject_len;
}MMIMMS_PUSH_FILE_T;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 获取未下载彩信的数目
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetUnsettleNum(void);

/*****************************************************************************/
//  Description : 获取未读彩信的数目
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetUnreadNum(void);

/*****************************************************************************/
//  Description : 获取彩信数目
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetTotalNum(void);

/*****************************************************************************/
//  Description : 获取彩信总大小
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetTotalSize(void);

/*****************************************************************************/
//  Description : 获取自动下载链表的首个条目
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_AUTOLIST_T* MMIMMS_GetAutoListFirstItem(void);

/*****************************************************************************/
//  Description : 获取手动收发链表的首个条目
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_AUTOLIST_T* MMIMMS_GetManualListFirstItem(void);

/*****************************************************************************/
//  Description : 判断自动下载LIST是否有内容
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsAutolistWithItem(void);

/*****************************************************************************/
//  Description : 开机初始化自动下载链表
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_InitAutoList(void);

/*****************************************************************************/
//  Description : 新增条目到自动下载链表
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_AddAutoList(
                               MN_DUAL_SYS_E dual_sys ,
                               int32 record_id,
                               MMIMMS_SENDRECV_TYPE_E  send_type
                               );

/*****************************************************************************/
//  Description : 新增条目到手动收发链表
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_AddManualList(
                                    MN_DUAL_SYS_E dual_sys ,
                                    int32      record_id,
                                    MMIMMS_SENDRECV_TYPE_E  send_type
                                    );

/*****************************************************************************/
//  Description : 删除自动下载链表的条目
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DeleteAutoListItem(int32 record_id);

/*****************************************************************************/
//  Description : 删除手动收发链表的条目
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DeleteManualListItem(int32 record_id);

/*****************************************************************************/
//  Description : create mms dir if not exist
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_CreateMMSDir(void);

/*****************************************************************************/
//  Description : 释放整个自动下载链表与手动收发链表
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_FreeSendRecvList(void);

/*****************************************************************************/
//  Description : 是否到达最大数量
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsMaxNum(void);

/*****************************************************************************/
//  Description : read mms list file to memory 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_ReadMMSListFile(void);

/*****************************************************************************/
//  Description : 清除短彩合一中的所有彩信条目,释放list内存
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_FreeMMSList(void);

/*****************************************************************************/
//  Description : insert all mms to sms order list 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_InsertMMSListToOrderList(void);

/*****************************************************************************/
//  Description : insert one mms to sms order list 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_InsertMMSItemToOrderList(
                                               MMISMS_ORDER_INFO_T *order_ptr,
                                               int32				record_id
                                               );

/*****************************************************************************/
//  Description : read push file 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_ReadPush(
                               MMIMMS_PUSH_FILE_T * push_file_ptr,
                               int32				record_id
                               );

/*****************************************************************************/
//  Description : read mms file 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_EDIT_DOCUMENT_T * MMIMMS_ReadMMS(
                                               int32				record_id
                                               );

/*****************************************************************************/
//  Description : read mms file 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_LoadMMSToVfile(
                                     int32      record_id
                                     );

/*****************************************************************************/
//  Description : save mms list file 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_SaveMMSListFile(void);

/*****************************************************************************/
//  Description : save push file 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIMMS_SavePush(
                             MN_DUAL_SYS_E dual_sys ,
                             MMISMS_MO_MT_TYPE_E  mo_mt_type,
                             const MMS_NOTIFICATION_IND_MSG_T *msg_ptr,
                             const uint8 *sender_ptr
                             );

/*****************************************************************************/
//  Description : save mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_SaveMMS(
                                     MN_DUAL_SYS_E              dual_sys,
                                     MMISMS_MO_MT_TYPE_E        mo_mt_type,
                                     MMIMMS_EDIT_DOCUMENT_T *   editdoc_ptr
                                     );

/*****************************************************************************/
//  Description : save mms content
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_SaveMMSContent(
                                            uint8	*filename,
                                            BOOLEAN is_text,
                                            wchar * full_path_ptr
#ifdef DRM_SUPPORT
                                            ,uint16 selectIndex
#endif
                                            );

/*****************************************************************************/
//  Description : delete mms  and save list
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:  本函数删除彩信条目并保存LIST
/*****************************************************************************/
PUBLIC void MMIMMS_DeleteMMSAndSaveList(
                                        int32 record_id
                                        );

/*****************************************************************************/
//  Description : delete mms  考虑多选删除的效率问题，该函数只对内存中的LIST做修改，不做LIST的保存处理
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DeleteMMS(
                             int32				record_id
                             );

/*****************************************************************************/
//     Description : delete all files in MMS directory
//    Global resource dependence : 
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_DeleteAllMMSFiles(void);

/*****************************************************************************/
//  Description : whether mms push is in queue or not
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsInQueue(int32 record_id);

/*****************************************************************************/
//  Description : delete all mms in the box 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_INFO_T *MMIMMS_GetListItemByID(
                                                   int32				record_id
                                                   );

/*****************************************************************************/
//  Description : init mms block memory
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_InitBlock(void);

/*****************************************************************************/
//  Description : mms alloc in block
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void *MMIMMS_AllocInBlock(
                                 uint32         size,
                                 const char     *file_ptr, 
                                 uint32         line
                                 );

/*****************************************************************************/
//  Description : destroy mms block memory 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DestroyBlock(void);

/*****************************************************************************/
//  Description : mms free in block
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void  MMIMMS_FreeInBlock(
                                void 		*ptr
                                );

/*****************************************************************************/
//  Description : change mms type
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_ChangeRecordType(
                                      int32                 record_id,
                                      MMISMS_MO_MT_TYPE_E   mo_mt_type
                                      );

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsRecordIDExist(int32 record_id);

/*****************************************************************************/
//  Description : integrate mms head and body 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IntegrateMMS(
                                   MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                   );

/*****************************************************************************/
//  Description : decode received mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note: 保存LIST ，保存MMS，删除PUSH
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_UpdateRecvItem(int32 record_id);

/*****************************************************************************/
//  Description : Parse MMS, get mms body and head 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_ParseMMS(
                               uint8 *fname,
                               MMIMMS_EDIT_DOCUMENT_T * edit_doc
                               );

/*****************************************************************************/
//  Description : get vf file length
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32  MMIMMS_GetVfFileLength(uint8 *fname);

/*****************************************************************************/
// 	Description: 生成唯一文件名
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMMS_GenUniName(uint8 *fname,uint16 max_len);

/*****************************************************************************/
//  Description : get mms file full path for write
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetMMSFilePathForWrite(
                                             const wchar    *file_name_ptr,         //[IN]
                                             uint16         file_name_len,          //[IN]
                                             wchar          *full_path_name_ptr,    //[OUT]
                                             uint16         *full_path_len_ptr,      //[OUT]
                                             uint32         size                    //[IN]
                                             );
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIMMS_FILE_H*/


