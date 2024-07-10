/*****************************************************************************
** File Name:      mmipb_app.h                                               *
** Author:                                                                   *
** Date:           11/09/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe phonebook                   *
**                 ����ļ���Ҫ����Ӧ���߼���applet����.                     *
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
#define MMIPB_MAX_APPLET_COUNT         5//����ͬʱ����5��pb appletʵ��
// ��¼�绰��ģ���ṩ����ʾ�б����Ϣ
typedef struct _MMIPB_PRIVACY_INFO_T
{
    BOOLEAN                 is_unprotect;//TRUE:�Ѿ�������FALSE:��û�н���
    BOOLEAN                 is_cl_unprotect;//TRUE:CL,�Ѿ�������FALSE:��û�н���

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

// �����绰��������ģ��ӿڵ�����
typedef enum _MMIPB_CALL_TYPE_E
{
	MMIPB_CALL_TYPE_MAKECALL_AUDIO,      // �����绰,@zhaohui add
	#ifdef VT_SUPPORT
	MMIPB_CALL_TYPE_MAKECALL_VIDEO,//���ӵ绰,@zhaohui add
	#endif
#ifdef MMI_IP_CALL_SUPPORT
	MMIPB_CALL_TYPE_MAKEIPCALL,    // IP����	
#endif
#ifdef BT_DIALER_SUPPORT
	MMIPB_CALL_TYPE_MAKEBTCALL,    // BT����
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

//PBģ�����Ҫ״̬
typedef enum
{
    MMIPB_ONE_LINE_TIPS,
    MMIPB_DUAL_LINE,
}MMIPB_DISPLAY_MODE_TYPE_E;
//PBģ�����Ҫ״̬
typedef enum
{
    MMIPB_STATUS_READY=0x0, //PB����״̬
    MMIPB_STATUS_SIM_NOT_READY = 0x1, //SIM��û�г�ʼ�����
    MMIPB_STATUS_SORTING = 0x2,//��������
    MMIPB_STATUS_SORT_EXIT = 0x3,//�����˳�
}MMIPB_STATUS_E;



typedef struct _MMIPB_SEL_NODE
{
    uint16 entry_id;
    uint16 storage_id;
    uint16 num_index;
    uint8  sel_field;//ѡ�����
    uint8 is_in_group;//����λ��ԭ���Ƿ���Ⱥ���� 1/0
                      //����λ���������Ƿ���������1/0
    BOOLEAN is_manage_group;
    struct _MMIPB_SEL_NODE*  next;//��ǰ��������
     
}MMIPB_SEL_NODE_T;//ѡ����ѡ��ʱ��ÿ��ѡ��ڵ�Ľṹ

typedef enum _MMIPB_PBLIST_TYPE_E
{
    MMIPB_ALL_GROUP_CONTACTS,//������ϵ��
    MMIPB_SINGLE_GROUP_CONTACTS,//����������ϵ��	
    MMIPB_MOSTUSED_CONTACTS,//������ϵ��
//lzk mark
#ifdef BT_PBAP_SUPPORT
    MMIPB_BT_CONTACTS, //������ϵ�� 
#endif
    MMIPB_SIM_DN_CONTACTS,//SIM���ض�������ϵ��
    MMIPB_WITHOUT_CONTACTS_LIST,//����Ҫ�б�
    MMIPB_CONTACTS_TYPE_MAX
}MMIPB_CONTACTS_TYP_E;


typedef enum //������֤������ĸ�pb list����
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
    MMIPB_SEL_NODE_T*   sel_table_head;//ÿ��handle��Ӧ��ѡ���б�ı�ͷ
    MMIPB_SEL_NODE_T*   sel_table_tail;//ÿ��handle��Ӧ��ѡ���б�ı�β
    wchar  file_path[MMIFILE_FULL_PATH_MAX_LEN + 1];//���ڼ�¼���� vcard��·��
    uint16 to_storage_id;//���ڼ�¼���Ƶ�λ�õ�storage
    uint8  group_id;//group manage
    uint16 sel_count;//ѡ���б�Ľڵ���
    uint16 opc_count;//������ǵڼ���ѡ���б�ڵ�
    uint16 error_count;//
}MMIPB_HANDLE_DATA_T;


typedef struct
{
    uint16 to_storage_id;//���ڼ�¼���Ƶ�λ�õ�storage
    uint8  group_id;//group manage
    uint8  sel_field;
    uint16 entry_id;
    uint16 storage_id;
    uint16 num_index;
    wchar   file_path[MMIFILE_FULL_PATH_MAX_LEN + 1];//���ڼ�¼���� vcard��·��
    uint8  is_in_group;//����λ��ԭ���Ƿ���Ⱥ���� 1/0
                      //����λ���������Ƿ���������1/0
    BOOLEAN is_manage_group;
}MMIPB_SEL_DATA;
//�û�ѡ����ļ���Ϣ
typedef struct
{
    uint32     total_file_count;//FILEARRAY���ܵ��ļ�����
    uint32     curent_index;    //��ǰ�������ļ���FILEARRAY��λ��
    FILEARRAY  file_array;     //��������Ҫ�������ļ�������
}MMIPB_SELECT_FILE_T;

typedef struct 
{
    //MMI_HANDLE_T cmd_handle;//��ǰ����handle
    MMIPB_OPC_E  cmd;//��ǰ��������
    MMIPB_HANDLE_T    handle;//ÿһ��applet��һ�����ڲ�����handle�󶨣�ע���������applet_handle
    MMIPB_SUB_FUNCTION_TYPE_E  app_type;//
    MMIPB_LIST_TYPE_E list_type;//signle, mutil, normal
    MMIPB_CONTACTS_TYP_E contacts_type;//��ǰȺ���б�ҳ�� 
    PHONEBOOK_STORAGE_E  sim_contact_type;//sim����¼���ͣ���Ҫ��Ϊ������MSISDN, FDN, SDN������������Ժ��Ըó�Ա����
    MMIPB_DISPLAY_MODE_TYPE_E display_mode; //���л���˫��
    MMIPB_LIST_FIELD_TYPE_E field_disp_type[MMIPB_TAB_MAX_NUM];//
    MMI_WIN_ID_T   active_win_id;//��ǰ�����
    uint16           cur_group_id;//��ǰ���ڵ����
    MMIPB_SEARCH_TYPE_T   search_type;
    uint16 		max_select_count;
    uint16      split_num;//
    wchar       search_str[MMIPB_MUL_SEARCH_MAX_CHAR+2];
    BOOLEAN     is_subStrSearch;//�Ƿ�׼����(ģ�������;�ȷ�����ֿ������)
    wchar       sub_search_str[MMIPB_MAX_STRING_LEN +1];//��������ģ�������ϵ������ʱʱʹ��
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
    BOOLEAN     is_need_pin_verify;//�Ƿ���ҪPIN2�룬��Ҫ����fdn���������������Ϊfalse
    BOOLEAN     is_need_protect_verify;//�Ƿ���Ҫ�����˽����
    #if defined(MMIPB_SIMDN_SUPPORT) && defined(MMI_DUALSIM_CALL_DIVERT_SUPPORT)
    BOOLEAN     is_enter_from_callsetting;//�Ƿ��callsettingģ�����MSISDN
    #endif
    MMIPB_HANDLE_DATA_T *select_list;//ѡ�����ϵ�˵�������
    MMIPB_SELECT_FILE_T select_files;//ѡ����ļ���
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
//  Description : ����pb�Ƿ�����˽����״̬
//  Global resource dependence : 
//  Author: maryxiao
//  Note: //TRUE:�Ѿ�������FALSE:��û�н�?
/*****************************************************************************/
PUBLIC void MMIPB_SetProtectStatus(BOOLEAN status);
/*****************************************************************************/
//  Description : ���pb����˽����״̬
//  Global resource dependence : 
//  Author: maryxiao
//  Note: //TRUE:�Ѿ�������FALSE:��û�н�?
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
//  Description : ����ѡ���б�Ľڵ�
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:����ѡ���б�
                        //for export  ֻ��Ҫ��file_path������storage id �� entry id��ֵ
                        //for copy  ��Ҫ��to_storage_id���ƣ� �Լ���Ҫ�� contact ������ֵ
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_CreateSelNode(MMI_HANDLE_T handle,MMIPB_SEL_DATA* sel_data);
/*****************************************************************************/
//  Description : ɾ��ѡ���б�ͷ���
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIPB_FindSelNode(MMI_HANDLE_T  handle, uint16 entry_id, uint16 storage_id, uint16 num_index);



/*****************************************************************************/
//  Description : ɾ���ض���ѡ������ڵ�
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIPB_DeleteSelNode(MMI_HANDLE_T  handle, uint16 entry_id, uint16 storage_id, uint16 num_index);
/*****************************************************************************/
//  Description : �õ�ѡ�������ͷ���
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_SEL_NODE_T* MMIPB_GetSelNode(MMI_HANDLE_T  handle, uint16 entry_id, uint16 storage_id, uint16 num_index);
/*****************************************************************************/
//  Description : ѡ���б��Ƿ���ڽڵ�
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_HasSelNode(MMI_HANDLE_T  handle);
/*****************************************************************************/
//  Description : ɾ��ѡ���б�ͷ���
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_DeleteSelHeadNode(MMI_HANDLE_T  handle);
/*****************************************************************************/
//  Description : ɾ������ѡ���б�Ľڵ�
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DeleteAllSelNode(MMI_HANDLE_T  handle);
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : �����û�ѡ����ļ�������Ϣ
//                Save selected files information
//  Global resource dependence : 
//  Author:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SaveSelectFileInfo(MMIPB_HANDLE_T handle,
                                        FILEARRAY file_array);

/*****************************************************************************/
//  Description : ��ȡѡ�����һ���ļ���ȫ·��
//  Global resource dependence : 
//  Author:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetNextSelectFileName(MMI_HANDLE_T handle,
                                           uint16 max_file_name_len,
                                           wchar *file_name_ptr);

/*****************************************************************************/
//  Description : ���ѡ����ļ�������Ϣ
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
//  Description : ɾ��ͼƬid��Ϣ������id�ļ�  
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
//  Note: ����Ϊ�ӿڣ��ṩ������ģ�����
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
//  Description : �첽������ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback);
/******************************************************************************/
//  Description : ��pb applet���õ��첽������ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactAsynForPB(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr);
/*****************************************************************************/
//  Description : �첽 �༭��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ModifyContactAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback);
/*****************************************************************************/
//  Description : �첽 �༭��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ModifyContactAsynForPB(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr);
/*****************************************************************************/
//  Description : �첽ɾ����������ѡ���б��еļ�¼���ڷǶ�ѡ״̬
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ModifyUnListContact(MMIPB_HANDLE_T handle,MMIPB_CONTACT_T* contact_ptr);
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : ������ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ExportVcard(uint16 entry_id, uint16 storage_id, const wchar *path_name_ptr);
/*****************************************************************************/
// Description :�첽������ϵ��
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
// Description :�첽������һ����ϵ��
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
// Description :��������
// Global resource dependence :
// Author
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ExportVcardEnd(
                                        MMIPB_HANDLE_T handle 
                                        );

/*****************************************************************************/
//  Description : �첽������������ѡ���б��еļ�¼���ڷǶ�ѡ״̬
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ExportUnListContact(MMIPB_HANDLE_T handle,uint16 entry_id, uint16 storage_id, wchar* path);
/*****************************************************************************/
// Description : �첽������ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ExportContactAsynForPB(MMIPB_HANDLE_T handle, wchar* path);
/*****************************************************************************/
// Description : ������ϵ��
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
// Description : ������ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SaveVcard(
                                    uint16      to_storage_id,      
                                    MMIPB_CONTACT_T* contact_ptr//IN
                                    );
/*****************************************************************************/
// Description : �첽������ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ImportVcardAsyn(MMIPB_HANDLE_T handle, //IN
                                           const wchar *path_name_ptr,//IN
                                           MMIPB_ACCESS_CALLBACK_FUNC callback
                                           );

/*****************************************************************************/
// Description : �첽������ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ImportContactAsynForPB(MMIPB_HANDLE_T handle);
#endif
/****************************************************************************
*/
//  Description : �첽ɾ����ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteContactAsyn(
                                                MMIPB_HANDLE_T handle, 
                                                uint16 entry_id,
                                                uint16 storage_id, 
                                                MMIPB_ACCESS_CALLBACK_FUNC callback
                                                );
/*****************************************************************************/
//  Description : �첽ɾ����������ѡ���б��еļ�¼���ڷǶ�ѡ״̬
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteUnListContact(MMIPB_HANDLE_T handle,uint16 entry_id, uint16 storage_id);
/*****************************************************************************/
//  Description : ��pb applet���õ��첽ɾ����ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteContactAsynForPB(MMIPB_HANDLE_T handle);
//  Description : �����ϵ����Ϣ
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetContactInfo(uint16 entry_id, uint16 storage_id, MMIPB_CONTACT_T* contact_ptr);
/*****************************************************************************/
//  Description : ������ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_CopyContact(uint16 to_storage_id, uint16 from_entry_id, uint16 from_storage_id);
/*****************************************************************************/
//  Description : �첽������ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:�ṩ���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_CopyContactAsyn(
                                              MMIPB_HANDLE_T handle,
                                              uint16         dest_storage_id,
                                              MMIPB_ACCESS_CALLBACK_FUNC callback
                                              );

/*****************************************************************************/
//  Description :дȺ������
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteGroupName(MMIPB_NAME_T* name,  uint8 group_id);
/*****************************************************************************/
//  Description : �첽ɾ����������ѡ���б��еļ�¼���ڷǶ�ѡ״̬
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
//  Description : ��pb applet���õ��첽������ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_CopyMultiContactAsyn(MMIPB_HANDLE_T handle, uint16 to_storage_id);
/*****************************************************************************/
//  Description :��ȡȺ������
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadGroupName(MMIPB_NAME_T* name,  uint8 group_id);
#ifdef MMIPB_MOST_USED_SUPPORT
 /*****************************************************************************/
//  Description : �ղ���ϵ��
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactToFavourite(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T* contact_ptr);
 /*****************************************************************************/
//  Description : �ղ���ϵ��
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactToFavouriteAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T* contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback);
 /*****************************************************************************/
//  Description : �ղ���ϵ��
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactToFavouriteAsynForPB(MMIPB_HANDLE_T handle);

/*****************************************************************************/
//  Description : �첽add��������ѡ���б��еļ�¼���ڷǶ�ѡ״̬
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddUnlistContactFromFavourite(MMIPB_HANDLE_T handle, uint16 entry_id, uint16 storage_id);
 /*****************************************************************************/
//  Description : ȡ���ղ���ϵ��
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromFavourite(MMI_HANDLE_T applet_handle,MMIPB_CONTACT_T* contact_ptr);
 /*****************************************************************************/
//  Description : ȡ���ղ���ϵ��
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromFavouriteAsyn(MMIPB_HANDLE_T handle,MMIPB_CONTACT_T* contact_ptr,MMIPB_ACCESS_CALLBACK_FUNC callback);
 /*****************************************************************************/
//  Description : ȡ���ղ���ϵ��
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromFavouriteAsynForPB(MMIPB_HANDLE_T handle);
/*****************************************************************************/
//  Description : �첽ɾ����������ѡ���б��еļ�¼���ڷǶ�ѡ״̬
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
//  Description :дȺ������
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_WriteGroupName(MMIPB_NAME_T* name,  uint8 group_id);
/*****************************************************************************/
//  Description :Ⱥ���Ƿ���Ч
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_IsUserGroupIdValid(uint32 group_id);
/*****************************************************************************/
//  Description :����Ⱥ��ʱ��ÿ��õ�Ⱥ��id
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetFreeUserGroupId(void);
/*****************************************************************************/
//  Description : ���applet������ѡ���б�Ľڵ���,������group
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC uint16  MMIPB_GetSelectCount(MMIPB_HANDLE_T handle);

/*****************************************************************************/
// Description : ���������Դ洢��nv contacts����Ŀ
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMaxContactNumOfNV(void);
/*****************************************************************************/
// Description : ���������Դ洢��sim contacts����Ŀ
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMaxContactNumOfSim(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
// Description : ȡ��ÿ��sim�������Ѵ��е���ϵ�˸���
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetSimContactCount(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
// Description : ȡ��SIM�����ֻ���ÿ����ļ�¼��
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetContactNumOfGroup(uint16 group_index);

/*****************************************************************************/
//  Description : ��ʼ��Phonebook��ȫ�ֱ������ص�������
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
//  Description : ����Ϣ��task���ú���������ϵ�˵���������
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
//  Note: ������������pb�����������������sim��ready������Ҫ��CALL_READY��SIM_NOT_READY
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
//  Note: ���������ALL_READY��SIM_NOT_READY���ٽ�������
//  [�ر���ʾWarnning]:����������Žӿڣ�һ��Ҫ��֤sim�������·�ALL_READY��SIM_NOT_READY����sim�����³�ʼ��
/*****************************************************************************/
PUBLIC void MMIPB_ReinitPB(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
//  Description : ������ϵ�˵�����
//  Global resource dependence : g_mmipb_entry_list
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_InitEntryList(void);
/*****************************************************************************/
//  Description : ��õ绰���Ƿ���Ա�����ģ�����
//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsPbReady //RETURN: TRUE, ����ʹ��
(
   void
);

/*****************************************************************************/
//  Description : �жϸ�Ӧ���Ƿ������
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsPermitOpen(MMIPB_LIST_TYPE_E app_type);
#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : USIM����չ��Ϣ�Ƿ��Ѿ�����
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsUSIMReady(void);
/*****************************************************************************/
//  Description : Ҫ��ȡ��USIM����չ��Ϣ
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_ReadUSIMExtCnf(APP_MN_READ_ADDITIONAL_ENTRY_CNF_T *usim_ext_ptr);
/*****************************************************************************/
//  Description : �ú������ڴ�����յ���APP_MN_UPDATE_USER_INFO_CNF��Ϣ
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_USIMUpdateCnf(APP_MN_UPDATE_USER_INFO_CNF_T *update_result);
/*****************************************************************************/
//  Description : �ú������ڴ�����յ���APP_MN_DEL_USER_INFO_CNF��Ϣ
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_USIMDeleteCnf(APP_MN_UPDATE_USER_INFO_CNF_T *delete_result);
#endif
/*****************************************************************************/
//  Description : ��ȡPB��״̬
//  Global resource dependence : none
//  Note: 
//  author: baokun.yin
/*****************************************************************************/
PUBLIC void MMIPB_SetPBStatus(MMIPB_STATUS_E status);
/*****************************************************************************/
//  Description : ����pb״̬
//  Global resource dependence : none
//  Note: 
//  author: baokun.yin
/*****************************************************************************/
PUBLIC MMIPB_STATUS_E MMIPB_GetPBStatus(void);
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : ���õ绰����ı�ʹ��

//  Note: ���¸ú����ʹ��Ƶ�ʡ�
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SetNumberUsed(MMIPB_BCD_NUMBER_T *number_ptr);
#endif

/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ�ˡ�
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: OUT: contact_ptr
//        OUT: num_index
//        IN: handle
//        IN: index
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_GetSelectContact(MMIPB_HANDLE_T handle,MMIPB_CONTACT_T * contact_ptr, uint16 index, uint16 * num_index);
/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ��������
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_GetSelectName(MMIPB_HANDLE_T handle,MMI_STRING_T *name_str, uint16 max_name_len,uint16 index);
/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ��email��
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

PUBLIC BOOLEAN  MMIPB_GetSelectEmail(MMIPB_HANDLE_T handle,MMI_STRING_T *mail_ptr, uint16 max_mail_len,uint16 index);
/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ�˺��롣
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_GetSelectNumber(MMIPB_HANDLE_T handle, MMIPB_BCD_NUMBER_T *nameber_ptr, uint16 max_number_len,  uint16 index);
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ��vcard��
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_GetSelectVcard(MMIPB_HANDLE_T handle, uint8  *data_ptr, uint16 max_buf_len,  uint16 index);
#endif
/*****************************************************************************/
//  Description :   ���PB
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
//  Description : ��BYTE�л��NUMBER PLAN
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
//  Description : ��sim��λ��ӳ��Ϊgroup id
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
//  Description : ������
//  Global resource dependence : s_mmipb_group_info
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SaveGroup  //RETURN: TRUE�������ɹ�; FALSE������ʧ��
(
 MMIPB_NAME_T    *group_name_ptr,    //IN: Ⱥ�������
 uint8	        group_id ,           //IN: Ⱥ���id��
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
//  Description :����Ⱥ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:IN :group_id
        //OUT: group_ptr
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SetGroupInfo( uint8 group_id, MMIPB_GROUP_T* group_ptr);

/*****************************************************************************/
//  Description :��ȡȺ��
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
//  Description : ����sns��ϵ��
//  Global resource dependence : 
//  Author:starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_LoadSnsContact(void);

/*****************************************************************************/
//  Description : : ��PB task������Ϣ,����sns��ϵ��
//  Global resource dependence : 
//  Author: starmer.sun
//  Note
/*****************************************************************************/
PUBLIC void MMIPB_StartUpdateSnsContact(MMI_HANDLE_T applet_handle);
/*****************************************************************************/
//  Description : ����sns��ϵ��
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
//  Description :����Զ���Ⱥ�����
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
//  Description :��ȡMSISD�Ĕ�Ŀ
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMSISDNum(void);

/*****************************************************************************/
//  Description :��ȡMSISDNumber
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetMSISDNumberArray(MMIPB_BCD_NUMBER_T *number, int16 max_num);
/*****************************************************************************/
//  Description :��ȡָ��SIM��MSISDNumber
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetMSISDNumberArrayBySim(MMIPB_BCD_NUMBER_T *number,MN_DUAL_SYS_E dual_sys, int16 max_num);

/*****************************************************************************/
// Description : ���������Դ洢��contacts����Ŀ
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
//  Description : ɾ��ȫ��������ϵ��
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
//  Description :��ϵ��ͬ���������ͨѶ¼
//  Parameter: contact_ptr[in]: ��¼�����Ϣ
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIBT_AddContact(MMIPB_CONTACT_T *contact_ptr);
/*******************************************************************************/
//  Description : ������ͷ�ڵ�ĵ���������ͷ���
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_InitPBAPDataList(void);
/*******************************************************************************/
//  Description :����vcard buffer data  
//  Parameter: data_ptr[in]: ���ڱ���UTF-8�����vCard��Ŀ���ݣ��������vCard2.1�淶�Ĺ涨��
//			   data_len[in]: ����vCard��¼�Ĵ�С
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
//  Description : ���ȫ��������ϵ��
//  Global resource dependence : none
//  Author: Yanyan.an
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ClearAllBtContactForPbap(void);
/*****************************************************************************/
//  Description :���ȫ��������ϵ��
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


