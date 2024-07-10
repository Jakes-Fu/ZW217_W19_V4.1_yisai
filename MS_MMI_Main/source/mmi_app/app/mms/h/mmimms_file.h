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
#define MMIMMS_LIST_VER		        "MMSVER.00.00.03_BE"  //�洢�ļ���ʽ�汾��
#else
#define MMIMMS_LIST_VER		        "MMSVER.00.00.03_LE"  //�洢�ļ���ʽ�汾��
#endif
#define MMIMMS_LIST_VER_LEN	        18

#define MMIMMS_FILE_NAME_LEN        MMS_FILE_NAME_LEN                  //���Ŵ洢�ļ�������
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
//MMS LIST�ļ�head
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
    MMISMS_ORDER_INFO_T *list_head_ptr;             //���в�������
    MMIMMS_AUTOLIST_T   *autolist_head_ptr;         //�Զ����ض��в�������
    MMIMMS_AUTOLIST_T   *manuallist_head_ptr;       //�ֶ��շ���������
    uint32              total_num;                  //��������
    uint32              unsettled_num;              //δ���ز�����Ŀ
    uint32              unread_num;                 //δ��������Ŀ
}MMIMMS_LISTINFO_T;

 
typedef struct MMIMMS_PUSH_FILE_T_tag
{
    uint32  size;                                   //���Ŵ�С
    uint32  expiry_time;		                    //expiry time
    uint8   url[MMS_MAX_URL_LENGTH+1];                    //MMS���ڵ�URL
    uint8   sender[MMIMMS_MAX_ADDRESS_LEN+1];       //������
    wchar   subject[MMIMMS_MAX_SUBJECT_LEN];      //MMS����
    uint16  subject_len;
}MMIMMS_PUSH_FILE_T;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ��ȡδ���ز��ŵ���Ŀ
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetUnsettleNum(void);

/*****************************************************************************/
//  Description : ��ȡδ�����ŵ���Ŀ
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetUnreadNum(void);

/*****************************************************************************/
//  Description : ��ȡ������Ŀ
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetTotalNum(void);

/*****************************************************************************/
//  Description : ��ȡ�����ܴ�С
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetTotalSize(void);

/*****************************************************************************/
//  Description : ��ȡ�Զ�����������׸���Ŀ
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_AUTOLIST_T* MMIMMS_GetAutoListFirstItem(void);

/*****************************************************************************/
//  Description : ��ȡ�ֶ��շ�������׸���Ŀ
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_AUTOLIST_T* MMIMMS_GetManualListFirstItem(void);

/*****************************************************************************/
//  Description : �ж��Զ�����LIST�Ƿ�������
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsAutolistWithItem(void);

/*****************************************************************************/
//  Description : ������ʼ���Զ���������
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_InitAutoList(void);

/*****************************************************************************/
//  Description : ������Ŀ���Զ���������
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
//  Description : ������Ŀ���ֶ��շ�����
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
//  Description : ɾ���Զ������������Ŀ
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DeleteAutoListItem(int32 record_id);

/*****************************************************************************/
//  Description : ɾ���ֶ��շ��������Ŀ
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
//  Description : �ͷ������Զ������������ֶ��շ�����
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_FreeSendRecvList(void);

/*****************************************************************************/
//  Description : �Ƿ񵽴��������
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
//  Description : ����̲ʺ�һ�е����в�����Ŀ,�ͷ�list�ڴ�
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
//  Note:  ������ɾ��������Ŀ������LIST
/*****************************************************************************/
PUBLIC void MMIMMS_DeleteMMSAndSaveList(
                                        int32 record_id
                                        );

/*****************************************************************************/
//  Description : delete mms  ���Ƕ�ѡɾ����Ч�����⣬�ú���ֻ���ڴ��е�LIST���޸ģ�����LIST�ı��洦��
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
//  Note: ����LIST ������MMS��ɾ��PUSH
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
// 	Description: ����Ψһ�ļ���
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


