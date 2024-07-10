/*****************************************************************************
** File Name:      mmipb_interface.h                                          *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file  define synchronous and asynchronous access interface,
**                 management of this interface and handles, and data tables.
**                 ����ļ���Ҫ�Ƕ�������Ҫͬ�����첽���ʵĽӿڣ�������ʽӿڵĹ���
**                 handle�Ĺ������ݱ�Ĺ����                  *
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

#define MMIPB_MAX_HANDLE_NUM  30 //ϵͳ���������30��handle����

#define MMIPB_MAX_WAIT_CMD_NUM 10 //ϵͳ���������10����Ϣ�ȴ�

//PBģ�����Ҫ��������
typedef enum
{
    MMIPB_OPC_ADD= 0x1,//���
    MMIPB_OPC_DELETE= 0x2,//ɾ��
    MMIPB_OPC_EDIT= 0x3,//�༭
    MMIPB_OPC_COPY= 0x4,//����
    MMIPB_OPC_EXPORT= 0x5,//����
    MMIPB_OPC_IMPORT= 0x6, //����
    MMIPB_OPC_FIND  = 0x7, //����
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
    MMIPB_ACCESS_CONTACT,          //���ʼ�¼
#ifdef CMCC_VCARD_SUPPORT
    MMIPB_ACCESS_USIM_AAS,          //����usim��չ��������
#endif
#ifdef MMI_VCARD_SUPPORT
    MMIPB_ACCESS_VCARD,            //����vcard 
#endif
    MMIPB_ACCESS_GROUP,            //���ʷ���
#ifdef SNS_SUPPORT    
    MMIPB_ACCESS_SNS,
#endif    
    MMIPB_ACCESS_NUM
}MMIPB_ACCESS_E;

//��дɾ������
#define MMIPB_READ_MODE           0x1//��ģʽ
#define MMIPB_WRITE_MODE          0x2//дģʽ
#define MMIPB_DELETE_MODE         0x4//ɾ��ģʽ

//��ϵ��ID��Ϣ������Ψһ��־����ϵ�ˡ�
typedef struct _MMIPB_CONTACE_ID_T
{
    uint16  contact_id;        
    uint16  storage_id;            
}MMIPB_CONTACE_ID_T;

//sns��ϵ��ID��Ϣ������Ψһ��־����ϵ�ˡ�
typedef struct _MMIPB_SNS_ID_T
{
    uint16  index; //����       
    uint16  sns_type;//sns����            
}MMIPB_SNS_ID_T;

#ifdef CMCC_VCARD_SUPPORT
//��ϵ����չ��������λ����Ϣ��
typedef struct _MMIPB_AAS_INDEX_T
{
    MN_DUAL_SYS_E  dual_sys;        
    uint16         aas_index;            
}MMIPB_AAS_INDEX_T;
#endif

//���ݷ��ʵ�
typedef union
{
    MMIPB_CONTACE_ID_T contact;//��¼id��Ϣ
#ifdef CMCC_VCARD_SUPPORT
    MMIPB_AAS_INDEX_T  aas_index_t;//��չ��������λ����Ϣ
#endif
    uint16  list_index;//��¼������λ��
    uint16  group_id;//����
    MMIFILE_HANDLE file_handle;//�򿪵��ļ����
    //wchar   *file_path_ptr; //�ļ����ʵ�ȫ·��   
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

//handle��Ӧ�����ݽṹ
typedef struct  
{
    MMIPB_OPC_E         cmd_type;//��������
    uint32              group_mask;//Ⱥ��
    MMIPB_ACCESS_E      access_type;//��������
    MMIPB_ACCESS_POINT_T access_point;//����λ��
    void                *data_ptr;//��������ָ��
    uint32              data_len;//���ݳ���
    MMIPB_ACCESS_CALLBACK_FUNC callback;//�첽�����Ļص�����
    MMI_HANDLE_T        applet_handle;//applet handle
}MMIPB_HANDLE_NODE_T;

//handle��������
 typedef struct
{
    uint16             handle_count;//Handle����Ŀ
    uint16             cur_index;//Handle�ĵ�ǰ����
    MMIPB_HANDLE_NODE_T *handle_table[MMIPB_MAX_HANDLE_NUM];//Handle����
}MMIPB_HANDLE_ARRAY_T;

 //�ȴ�ִ�е��첽�������нڵ�
 typedef struct
 {
     uint32               pb_mode;     //ģʽ������д����ɾ��ģʽ
     MMIPB_HANDLE_T       cmd_handle; //���
 }MMIPB_MSG_QUEUE_NODE_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : �ӿڲ��ʼ��
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_InitInterface(void);

/*****************************************************************************/
//  Description :�ȴ���Ϣ���еĳ�ʼ��
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_InitWaitMsgQueue(void);


/*****************************************************************************/
//  Description : �����Ƿ���ͬһӦ�÷���
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsCurrentOperation(
                              uint16 contact_id,
                              uint16 storage_id
                              );


/*****************************************************************************/
//  Description : ����ͬ����event
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CreateEvent(void);

/*****************************************************************************/
//  Description : ����д��������
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_SetWriteError(MMIPB_ERROR_E error);

/*****************************************************************************/
//  Description : ��д��������
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetWriteError(void);

/*****************************************************************************/
//  Description : ����ɾ����������
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_SetDeleteError(MMIPB_ERROR_E error);

/*****************************************************************************/
//  Description : ��ɾ����������
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetDeleteError(void);

/*****************************************************************************/
//  Description : ͬ��д��confirm
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
//  Description : ͬ��ɾ����confirm
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
//  Description : ��Ϣ��ȡ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_Read (
                          MMIPB_ACCESS_E access_type,//[IN]��Ϣ����
                          MMIPB_ACCESS_POINT_T access_point,//[IN]��Ϣλ����Ϣ
                          uint32 data_len,//[IN]buf��󳤶�
                          void *out_data_ptr//[OUT]��ȡ������
                          );

/*****************************************************************************/
//  Description : д��Ϣ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_Write (
                           MMIPB_ACCESS_E access_type,//[IN]��Ϣ����
                           MMIPB_ACCESS_POINT_T access_point,//[IN]��Ϣλ����Ϣ
                           uint32 data_len,//[IN]д������ݳ���
                           void *write_data_ptr//[IN]д�������
                           );


/*****************************************************************************/
//  Description : ɾ����Ϣ
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
//  Description : ɾ��ȫ��������ϵ��
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_CleanAllMostUsedContact(void);
#endif
/*****************************************************************************/
//  Description : ����PB��Handle
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_HANDLE_T MMIPB_CreateHandle (
                           MMIPB_OPC_E  cmd_type,
                           MMI_HANDLE_T applet_handle
                           );

/*****************************************************************************/
//  Description : ����PB��Handle
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CloseHandle (
                               MMIPB_HANDLE_T handle
                               );

/*****************************************************************************/
//  Description : ��ȡhandle����Ӧ�Ľṹ��ָ��
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetHandleContent(                                       
                                       MMIPB_HANDLE_T handle,//[IN]
                                       MMIPB_HANDLE_NODE_T *node_ptr//[OUT]
                                       );

/*****************************************************************************/
//  Description : ����handle����Ӧ�Ľṹ��ֵ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SetHandleContent(MMIPB_HANDLE_T handle, MMIPB_HANDLE_NODE_T *handle_node_ptr);

/*****************************************************************************/
//  Description : �첽��Ϣ��ȡ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:Ŀǰ�����ڼ�¼��vcard�Ķ�ȡ
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadAsyn (
                              MMIPB_HANDLE_T handle,//[IN]handle id
                              MMIPB_ACCESS_E access_type,//[IN]��������
                              MMIPB_ACCESS_POINT_T access_point,//[IN]��Ϣλ����Ϣ
                              void *data_ptr,//[OUT]����ָ��
                              uint32 data_len,//[IN]���ݵ���󳤶�
                              MMIPB_ACCESS_CALLBACK_FUNC callback//[IN]�ص�����
                              );

/*****************************************************************************/
//  Description : �첽д��Ϣ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:Ŀǰ�����ڼ�¼��vcard�ı���
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
//  Description : �첽��Ϣɾ��
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:Ŀǰ�����ڼ�¼���첽ɾ��
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteAsyn (
                                MMIPB_HANDLE_T handle,
                                MMIPB_ACCESS_E access_type,
                                MMIPB_ACCESS_POINT_T access_point,
                                MMIPB_ACCESS_CALLBACK_FUNC callback
                                );

/*****************************************************************************/
//  Description : �첽������Ĵ���
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
//  Description : ��ȡ��ǰ��handle id
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_HANDLE_T MMIPB_GetCurrentHandle(void);


/*****************************************************************************/
//  Description : ��ϵ����Ϣ���浽������ݿ�
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertContactToList( 
                                               MMIPB_CONTACT_T *contact_ptr
                                               );

/*****************************************************************************/
//  Description : �ӱ�����ݿ���ɾ��һ���ڵ�
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromList(
                            uint16 contact_id,
                            uint16 storage_id
                            );

/*****************************************************************************/
//  Description : ���num_type��Ӧ�������ַ���
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetNumberTypeString(uint16 storage_id,
    uint8  num_type,  //IN:
    MMI_STRING_T *type_str//OUT
);

/*****************************************************************************/
//  Description : ���ĳһ���ͺ������󳤶�
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetNumberMaxLen(uint16 storage_id,                                  
                                   uint8 num_type,
                                   uint8 orignal_len//�ú���ԭ���ĳ��ȣ��������ĺ�����0
                                  );

/*****************************************************************************/
//  Description : �����ϵ�˶�Ӧ�ֶε����alpha����
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetMaxAlphaLen(uint16 storage_id,                                  
                                  MMIPB_CONTACT_DOMAIN_E feild//��ϵ����
                                  );

/*****************************************************************************/
//  Description : �����ϵ�˶�Ӧ�ֶε����wstr����
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetMaxWstrLen(uint16 storage_id,//                                   
                                  MMIPB_CONTACT_DOMAIN_E feild//��ϵ����
                                 );

/*****************************************************************************/
//  Description : ������ͨ��ϵ�˸���
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllNormalContactCount(void);
#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : ���г�����ϵ�˸���
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllMostUsedContactCount(void);
#endif
/*****************************************************************************/
//  Description : ������ͨ�绰����
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllNormalContactPhoneCount(void);

/*****************************************************************************/
//  Description : ������ͨmail����
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllNormalContactMailCount(void);

/*****************************************************************************/
//  Description : ĳ���洢�豸����ϵ�˸���
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
//  Description : ���instacemsg_type��Ӧ�������ַ���
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
//  Description : ���instacemsg_type��Ӧ�������ַ���
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIPB_GetImTypeTextId(uint8  im_type );
/*****************************************************************************/
//  Description : ĳ����������ϵ�˸���
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetGroupContactCount
(
    uint16 group_id
);


/*****************************************************************************/
//  Description : ĳ�������е绰����
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetGroupContactPhoneCount
(
    uint16 group_id
);

/*****************************************************************************/
//  Description : ĳ��������mail����
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
//  Description : ���ҵ�һ���ɼ�����ϵ��
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindFirstContact(
                                            uint32  group_mask,//[IN]
                                            uint16 storage_id, //Ҫ���ҵ���ϵ������
                                            uint16  *cur_index_ptr,//[OUT]
                                            MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                                            MMIPB_CONTACT_BASE_INFO_T *find_data_ptr,//[OUT] 
                                            MMIPB_HANDLE_T *handle_ptr//[OUT]
                                            );
#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : ���ҵ�һ���ɼ��ĳ�����ϵ��
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
//  Description : �ر�һ������
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindClose(
                                     MMIPB_HANDLE_T handle_ptr//[IN]
                                     );


/*****************************************************************************/
//  Description : ������һ���ɼ���¼
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_FindNextContact( MMIPB_HANDLE_T handle, 
                                            uint32  group_mask,//[IN]
                                            uint16  storage_id, //Ҫ���ҵ���ϵ������
                                            uint16  *cur_index_ptr,//[OUT]
                                            MMIPB_CONTACT_VALID_NUM_MAIL_INDEX_T *valid_index_array,//[OUT]
                                            MMIPB_CONTACT_BASE_INFO_T *find_data_ptr//[OUT] 
                                           );
#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : ������һ���ɼ�������ϵ�˼�¼
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
//  Description : ��ͨ��ϵ�˵Ļ�����Ϣ
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_CONTACT_BASE_INFO_T MMIPB_GetContactBaseInfo(uint16 index);

/*****************************************************************************/
//  Description : �ֻ���ϵ�˵Ļ�����Ϣ
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_CONTACT_BASE_INFO_T MMIPB_GetPhoneContactBaseInfo(uint16 index);

#ifdef MMIPB_MOST_USED_SUPPORT 
/*****************************************************************************/
//  Description : ������ϵ�˵Ļ�����Ϣ
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_MOSTUSED_CONTACT_NODE_T MMIPB_GetMostUsedContactBaseInfo(uint16 index);
#endif
/*****************************************************************************/
//  Description : �����ϵ�˶�Ӧ�ֶεĿռ�ʹ�����
//  Global resource dependence :
//  Author: 
//  Note: ÿ���ֶ���Ӧ����пռ䣬����ΪTRUE������ΪFALSE
//        ͨ���ú������Ի�ȡ���������ֶΣ�mail�ֶΣ������ֶε�ʹ�����
//  Return:���contact_idΪ0����ʾ����������һ���µ�id
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetFieldUageInfo(
                                 uint16 storage_id,//                                   
                                 uint16 contact_id,//��ϵ����
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
//  Description : ��sns���ݿ�
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
//  Description : ����pb�Ƿ�æ״̬
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_SetTaskStatus(BOOLEAN is_busy);

#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : ��ȡusim ֧�ֵ���չ��������͸���
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIPB_ReadUSIMAASCount(
                           MN_DUAL_SYS_E dual_sys
                           );

/*****************************************************************************/
//  Description : �첽��ȡUSIM ��չ��Ϣ
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadUSIMAddtionalAsyn(
                                           MN_DUAL_SYS_E dual_sys,
                                           uint16        contact_id
                                           );

/*****************************************************************************/
//  Description : USIM ��չ��Ϣ���浽�������кͺ�����ұ���
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
//  Description : ��ȡusim ֧�ֵ���չ����Ŀ��ø���
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
//  Description : ����������ϵ�˸���
//  Global resource dependence : 
//  Author:Yanyan.an 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetAllBtContactCount(void);
/*****************************************************************************/
//  Description : ���ҵ�һ���ɼ���������ϵ��
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
//  Description : ������һ���ɼ���������ϵ��
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


