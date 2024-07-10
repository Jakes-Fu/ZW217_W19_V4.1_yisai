
/****************************************************************************
** File Name:      mmipb_app.c                                          *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file deal with all pb applets, operation process, internal message *
**                 ����ļ���Ҫ����Ӧ���߼���applet����.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2011        baokun yin         Create
** 
****************************************************************************/
#define _MMIPB_APP_C_  


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
//#ifdef WIN32

#include "mmi_app_pb_trc.h"
#include "mmk_app.h"
#include "mmi_appmsg.h"
#include "mmi_applet_table.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmipb_app.h"
#include "mmi_queue.h"
#include "mmifdn_export.h"
#include "mmifmm_export.h"
#include "mn_type.h"
#include "mmipb_interface.h"
#include "mmk_type.h"
#include "mmipb_task.h"
#include "mmipb_adapt.h"
#include "mmivcard_export.h"
#include "mmipb_mem.h"
#include "mmifilearray_export.h"
#include "mmicc_export.h"
#include "mmisms_export.h"
#include "mmiphone_export.h"
#ifdef IKEYBACKUP_SUPPORT
#include "mmiikeybackup_api.h"
#endif
#ifdef SNS_SUPPORT
#include "mmisns_srv.h"
#include "mmisns_sqlite.h"
#endif
#if defined (MMI_FDN_DIAL_TO_PANEL_SUPPORT) || defined (MMI_LDN_SUPPORT)
#include "mmicl_export.h"
#endif
#include "mmimms_export.h"
#include "mmipb_interface.h"
#include "mmipb_search.h"
#include "mmiudisk_export.h"
#include "mmiim.h"
#ifdef BLUETOOTH_SUPPORT
#include "mmibt_export.h"
#endif
#ifdef MMIEMAIL_SUPPORT
#include "email_text.h"
#ifdef MMI_VCARD_SUPPORT
#include "mmiemail_export.h"
#endif
#endif
#ifdef BT_PBAP_SUPPORT
#include "mmicl_export.h"
#include "mmipb_view.h"
#endif

#include "mmicc_image.h"
#include "mmipb_text.h"
#include "mmipb_image.h"
#include "mmipb_datalist.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIPB_FUNCTION_TEST_MODE

#define MMIPB_SMS_VCARD_MAX_8BIT_SPLIT_LEN     128 

#ifdef MMI_PB_MINI_SUPPORT
#ifdef TCARD_PB_SUPPORT
#define MMIPB_CACHE_SIZE                        (1024*50)//50K
#else
#define MMIPB_CACHE_SIZE                        (1024*20)//20K
#endif
#elif PB_SUPPORT_LOW_MEMORY/*lint !e553*/
#ifdef TCARD_PB_SUPPORT   
#define MMIPB_CACHE_SIZE                        (1024*100)//100K
#else
#define MMIPB_CACHE_SIZE                        (1024*50)//50K
#endif
#else
#ifdef TCARD_PB_SUPPORT
#define MMIPB_CACHE_SIZE                        (1024*200)//200K
#else
#define MMIPB_CACHE_SIZE                        (1024*100)//100K
#endif
#endif

#ifdef BT_PBAP_SUPPORT
#define PBAP_CONTACT_NUM_COUNT          5
#define LEN                             sizeof(List)
#define VCARD_BEGIN_TAGS                "BEGIN:VCARD"   //��ʼ��ǩ
#define VCARD_END_TAGS                  "END:VCARD"   //�������ǩ
#endif

#ifdef BT_PBAP_SUPPORT
typedef struct List{
    void  *data;       //�ڵ�����
    uint16  len;       //���ݳ���
    struct List *next; //ָ���¸��ڵ�
}List;
#endif

#ifdef BT_PBAP_SUPPORT
LOCAL List *s_list_head = PNULL;
LOCAL char   *s_vcard_buff_ptr = PNULL;      //��¼��������vcard����
LOCAL uint32  s_sync_contact_count = 0;   //��¼�ɹ�ͬ����ϵ�˵ĸ���
LOCAL  uint32 s_node_number = 1;
LOCAL MMIPB_PBAP_PARSER_CALLBACK_FUNC parser_callback = PNULL;
#endif

MMI_APPLICATION_T   g_mmipb_app;    // phonebook��ʵ��
#ifdef CMCC_VCARD_SUPPORT
LOCAL BOOLEAN     s_mmipb_is_usim_addtion_ready = FALSE;
#endif
LOCAL MMIPB_STATUS_E s_mmpb_status = MMIPB_STATUS_SIM_NOT_READY;//PB״̬
LOCAL BOOLEAN       s_mmipb_sim_ready[MMI_DUAL_SYS_MAX] = {0};
LOCAL BOOLEAN       s_mmipb_is_abort  =FALSE;

LOCAL MMIPB_PRIVACY_INFO_T  g_mmipb_privacy_info;
LOCAL wchar   s_vcard_full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get phone book applet structure pointer
//  Global resource dependence : 
//  Author:baokun 
//  Note:
/*****************************************************************************/
LOCAL MMIPB_APPLET_T *GetPBApplet(MMIPB_HANDLE_T handle);

/*****************************************************************************/
//  Description : ��ȡҪ���Ƶ�Ŀ����ϵ����Ϣ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDestContactInfo(
                                  uint16 src_contact_id, //IN:
                                  uint16 src_storage_id, //IN:
                                  uint16 dest_storge_id, //IN:
                                  uint8 src_field,//IN:
                                  uint8 *left_src_field_ptr,//OUT
                                  MMIPB_CONTACT_T *dest_contact_ptr//OUT
                                  );
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : �첽����һ����ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
LOCAL MMIPB_ERROR_E ImportOneVcardAsyn(MMIPB_HANDLE_T    handle, //IN
                                       MMIFILE_HANDLE    file_handle,//IN
                                       MMIPB_ACCESS_CALLBACK_FUNC callback
                                       );
/*****************************************************************************/
//  Description : Set Current Vcard File Full Path
//  Global resource dependence : 
//  Author:yanyan.an
//  Note:
/*****************************************************************************/
LOCAL void MMIPB_SetCurVcardFileFullPath(
                                        wchar *s_vcard_full_path_name, 
                                        const wchar *full_path_name, 
                                        uint32 full_path_max_len);
/*****************************************************************************/
//  Description : Get Current Vcard File Full Path
//  Global resource dependence : 
//  Author:yanyan.an
//  Note:
/*****************************************************************************/
LOCAL wchar* MMIPB_GetCurVcardFileFullPath(void);
/*****************************************************************************/
//  Description : Delete Current Vcard File
//  Global resource dependence : 
//  Author:yanyan.an
//  Note:
/*****************************************************************************/
LOCAL void MMIPB_DeleteCurVcardFile(void);
#endif
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : ����һ����ϵ��
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
LOCAL MMIPB_ERROR_E SaveContact(                                    
                                MMIPB_CONTACT_T   *contact_ptr//IN                              
                                );                                  
#endif
#ifdef IKEYBACKUP_SUPPORT
/*****************************************************************************/
//  Description : һ����ԭע�ắ��
//  Global resource dependence : none
//  Author:
//  Note: 
/***************************************************************************/
LOCAL BOOLEAN ikeybackup_register(void);
#endif
/*****************************************************************************/
//  Description : the process message function of the PB
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  Applet_HandleEvent(    
                                       IAPPLET_T*          iapplet_ptr,
                                       MMI_MESSAGE_ID_E    msg_id, 
                                       DPARAM              param
                                  );

/*****************************************************************************/
//  Description : applet handle  compare function
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  CmpAppletHandle(void *data_ptr1, //[IN]
                               void *data_ptr2//[IN]
                                       );

/*****************************************************************************/
//  Description : deal with finishe sort  confirm
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
LOCAL void HandleSortCnf(void);

/*****************************************************************************/
//  Description : �绰�����µĻص�����
//  Global resource dependence 
//  Author:
//  Note:  
/*****************************************************************************/
// LOCAL void UpdateEventCallback(MMIEVENT_PB_EVENT_E event);
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : �жϱ����ļ�����ռ��Ƿ���
// Global resource dependence :
// Author:xiaohua.liu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckDiskSpaceForSaveVCard(MMIFILE_DEVICE_E file_dev);
#endif
/*****************************************************************************/
//  Description : is all sim are ready
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAllSimReady(void);

/*****************************************************************************/
//  Description :����ѡ���б����һ���ڵ���¼�
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
//LOCAL void MMIPB_HandleSelNodeSuccess(MMIPB_HANDLE_T handle);
/*****************************************************************************/
//  Description :����ѡ���б����һ���ڵ���¼�
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
//LOCAL void MMIPB_HandleSelNodeFail(MMIPB_HANDLE_T handle);
/*****************************************************************************/
//  Description : is the contact will be add is valid
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void mmipb_app_callback(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error);


/*****************************************************************************/
//  Description : ���մ����storage������¼���뵽�洢���С�
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: ����Ϊ�ӿڣ��ṩ������ģ�����
/*****************************************************************************/

LOCAL MMIPB_ERROR_E MMIPB_UpdateContact( MMIPB_CONTACT_T *contact_ptr);
/*****************************************************************************/
//  Description : �첽�޸Ļ���������ϵ��
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/

LOCAL MMIPB_ERROR_E MMIPB_UpdateContactAsyn(
                                            MMIPB_HANDLE_T handle, 
                                            MMIPB_CONTACT_T *contact_ptr, 
                                            MMIPB_ACCESS_CALLBACK_FUNC callback
                                            );
/*****************************************************************************/
//  Description : ɾ������ѡ���б�
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
LOCAL void DeleteSelectList(MMIPB_HANDLE_DATA_T*  select_list_ptr);

#ifdef BT_PBAP_SUPPORT
/*******************************************************************************/
//  Description :��ϵ��ͬ���������ͨѶ¼
//  Parameter: contact_ptr[in]: ��¼�����Ϣ
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL MMIPB_ERROR_E AddContactForPbap(MMIPB_CONTACT_T *contact_ptr, MMIPB_CUSTOM_PHOTO_T *photo_ptr);
#if 0
/*******************************************************************************/
//  Description :save buffer to file 
//  Parameter: buff[in]: Ҫ���������buffer
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL void MMIPB_SaveBufferToFile(const char* buff);
#endif
/*******************************************************************************/
//  Description : free all pbap list buffer
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL void freeallpbapdata(void);
/*******************************************************************************/
//  Description:������ͷ�ڵ�ĵ���������ͷ���
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL List* InitList(void);
/*******************************************************************************/
//  Description:�������ĩβ���һ��Ԫ��
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL int InsertList(List *L, void* buff,uint16 buff_len);
/*******************************************************************************/
//  Description:������Ա�L�е�����Ԫ�أ����ͷŵ�����L�����еĽ�㣬ʹ֮��Ϊһ���ձ�
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL void ClearList(List *pHead);
/*******************************************************************************/
//  Description:��ȡ�������е�i��Ԫ��
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL int GetElem(List *L, int i, void** buff, uint16 *len);
/*******************************************************************************/
//  Description:�жϵ������Ƿ�Ϊ��
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL int IsListEmpty(List *L);
/*******************************************************************************/
//  Description:�ж�����ڵ����
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL int ListNodeNum(List *L);
#endif

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

//applet������
LOCAL const CAF_STATIC_APPLET_T s_mmipb_applet_array[] = 
{
    { GUID_NAME_DEF(SPRD_PB_APPLET_ID), sizeof(MMIPB_APPLET_T), Applet_HandleEvent, 
        IMAGE_MAINMENU_ICON_PB, STXT_MAIN_PB},
};

//applet����Ϣ, ��mmi_applet_table.c extern
LOCAL const CAF_STATIC_APPLET_INFO_T g_mmipb_applet_info = 
{
    s_mmipb_applet_array,
        ARR_SIZE(s_mmipb_applet_array)
};
//applet ����
MMI_QUEUE_NODE_T *s_mmipb_applet_queue = PNULL; 
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/   


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get phone book applet structure pointer
//  Global resource dependence : 
//  Author:baokun 
//  Note:
/*****************************************************************************/
LOCAL MMIPB_APPLET_T *GetPBApplet(MMIPB_HANDLE_T handle)
{
    MMIPB_HANDLE_NODE_T handle_node = {0};

    if(MMIPB_GetHandleContent(handle, &handle_node))
    {
         return (MMIPB_APPLET_T*)MMK_GetAppletPtr(handle_node.applet_handle);
    }
    return PNULL;
}
/*****************************************************************************/
//  Description : ɾ������ѡ���б�
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
LOCAL void DeleteSelectList(MMIPB_HANDLE_DATA_T*  select_list_ptr)
{
    MMIPB_SEL_NODE_T*  cur_sel_node_ptr = PNULL;

    if(PNULL != select_list_ptr)
    {
        while(PNULL!=select_list_ptr->sel_table_head)
        {
            cur_sel_node_ptr = select_list_ptr->sel_table_head->next;
            MMIPB_MFREE(select_list_ptr->sel_table_head);
            select_list_ptr->sel_table_head = cur_sel_node_ptr;
            
        }
        select_list_ptr->sel_count = 0;
        select_list_ptr->sel_table_tail =PNULL;
    }
}

/*****************************************************************************/
//  Description : ����pb�Ƿ�����˽����״̬
//  Global resource dependence : 
//  Author: maryxiao
//  Note: //TRUE:�Ѿ�������FALSE:��û�н�?
/*****************************************************************************/
PUBLIC void MMIPB_SetProtectStatus(BOOLEAN status)
{
    g_mmipb_privacy_info.is_unprotect = status;
}

/*****************************************************************************/
//  Description : ���pb����˽����״̬
//  Global resource dependence : 
//  Author: maryxiao
//  Note: //TRUE:�Ѿ�������FALSE:��û�н�?
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetProtectStatus(void)
{
    return g_mmipb_privacy_info.is_unprotect;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_SetCLUnProtect(BOOLEAN is_unprotect)
{
    g_mmipb_privacy_info.is_unprotect = is_unprotect;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetCLUnProtect(void)
{
    return g_mmipb_privacy_info.is_unprotect;
}
/*****************************************************************************/
//  Description : register PB  applet info
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_RegAppletInfo(void)
{
    MMI_RegAppletInfo(MMI_MODULE_PB, &g_mmipb_applet_info);
}

/*****************************************************************************/
//  Description : register PB  applet info
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIPB_StartApplet(MMIPB_INSTANCE_T *instance_ptr)
{
    MMI_APPLET_START_T start ={0};
    MMI_HANDLE_T applet_handle = 0;

    //SCI_TRACE_LOW:"MMIPB_StartApplet"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_258_112_2_18_2_43_5_19,(uint8*)"");
    start.guid = SPRD_PB_APPLET_ID;
    start.param_ptr = instance_ptr;
    start.size_of_param = sizeof(MMIPB_INSTANCE_T);
    start.state = MMI_APPLET_STATE_DYNAMIC|MMI_APPLET_STATE_CLOSE_WITH_WIN;
    
    if(MMIQUEUE_QueueIsFull(s_mmipb_applet_queue))
    {
        return applet_handle;
    }

    applet_handle = MMK_StartApplet( &start); 
    return applet_handle;
   
}


/*****************************************************************************/
//  Description : unregister PB  applet info
//  Global resource dependence : 
//  Author:maryxiao
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CloseApplet(MMI_HANDLE_T applet_handle)
{

   // MMK_SendMsg(applet_handle, MSG_APPLET_SUSPEND, PNULL);
    MMK_CloseApplet(applet_handle);
}

/*****************************************************************************/
//  Description : init PB applet queue
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_InitAppletQueue(void)
{
    s_mmipb_applet_queue = MMIQUEUE_InitQueue(MMIPB_MAX_APPLET_COUNT, sizeof(MMI_HANDLE_T));
}

/*****************************************************************************/
//  Description : PB applet queue
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMI_QUEUE_NODE_T *MMIPB_GetAppletQueue(void)
{
    return s_mmipb_applet_queue;
}

/*****************************************************************************/
//  Description : PB applet queue
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:applet_handle_ptr: OUT
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_GetCurAppletHandle(MMI_HANDLE_T* applet_handle_ptr)
{
    BOOLEAN  ret = FALSE;
    uint32 index = MMIQUEUE_INVALID_INDEX;
    if(PNULL!=s_mmipb_applet_queue)
    {
    
        index = MMIQUEUE_GetCurrentIndex(s_mmipb_applet_queue);
        if(MMIQUEUE_INVALID_INDEX != index)
        {

        
            *applet_handle_ptr = *(MMI_HANDLE_T *)MMIQUEUE_PeekQueue(s_mmipb_applet_queue, index);
        
            if(0 != *applet_handle_ptr)
            {
                ret = TRUE;
            }
        }
        
    }
    return ret;

}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void ProtectCallback(BOOLEAN is_permitted, DPARAM param_ptr)
{
    MMI_HANDLE_T applet_handle = 0;
    MMIPB_APPLET_T*  applet_ptr=   PNULL;

    if (PNULL != param_ptr)
    {
        applet_handle = *(MMI_HANDLE_T*)param_ptr;
    }

    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
	if ( is_permitted && applet_ptr != PNULL)
	{
		//������֤�ɹ�
        if(
            applet_ptr->instance.entry_func != PNULL
            &&applet_ptr->instance.entry_func(applet_handle))
        {
            MMIPB_SetProtectStatus(TRUE);
        }
        else
        {
            MMK_CloseApplet(applet_handle);
        }		
	}
	else
	{
		//������֤ʧ��
        MMK_CloseApplet(applet_handle);
	}  
}


/*****************************************************************************/
//  Description : the process message function of the PB
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  Applet_HandleEvent(    
                                       IAPPLET_T*          iapplet_ptr,
                                       MMI_MESSAGE_ID_E    msg_id, 
                                       DPARAM              param
                                       )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)iapplet_ptr;
    MMIPB_INSTANCE_T *instance_ptr = PNULL;
    MMIPB_HANDLE_T     handle = PNULL;
    MMIPB_HANDLE_NODE_T handle_node = {0};
    MMI_HANDLE_T       applet_handle = PNULL;
    BOOLEAN            is_protected = FALSE;
    instance_ptr = (MMIPB_INSTANCE_T *)param;
        
    applet_handle = (MMI_HANDLE_T)applet_ptr->caf_applet.app_handle;
    //SCI_TRACE_LOW:"mmi Applet_HandleEvent msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_353_112_2_18_2_43_5_20,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_APPLET_START:
        //MMK_CreateFmWin((uint32*)MMI_FM_MAIN_WIN_TAB,PNULL);
        if(applet_ptr != PNULL)
        {
#ifdef SNS_SUPPORT            
            MMIPB_OpenSNSDB(0);/*lint !e718 !e18*/
#endif
            if(instance_ptr != PNULL)
            {
                SCI_MEMCPY(&applet_ptr->instance, instance_ptr, sizeof(MMIPB_INSTANCE_T));
            }
            if(instance_ptr != PNULL)
            {
                handle = MMIPB_CreateHandle(instance_ptr->cmd, applet_handle);//ÿһ��applet��һ�����ڲ�����handle�󶨣�
            }
            applet_ptr->instance.handle=handle;   
            MMIQUEUE_InQueue(s_mmipb_applet_queue, &applet_handle);
            if(PNULL != instance_ptr && instance_ptr->entry_func != PNULL)
            {           
                //start main entry
                //first check protect
                if((!MMIPB_GetProtectStatus())&&(MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_PB)))//��Ҫ�����˽����������˽�����Ѿ�����
                {
                    if(applet_ptr->instance.is_need_protect_verify)
                    {
                        MMIAPISET_ValidatePrivacy(MMISET_PROTECT_PB, 
                             ProtectCallback,//MMIPB_DoOperationWithoutProtect,
                             &applet_handle,
                             sizeof(applet_handle)
                             );
                        is_protected = TRUE;
                    }
                    
                }
                if(!is_protected
                   &&(!applet_ptr->instance.entry_func(applet_handle))
                 )
                {
                    //ʧ�ܣ�fail
                    MMK_CloseApplet(applet_handle);
                }
            }            
        }
        
        
        break;
        
    case MSG_APPLET_STOP: 
        if(applet_ptr != PNULL)
        {
            MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
            if(MMIPB_GetHandleContent(applet_ptr->instance.handle, &handle_node))
            {
#ifdef MMI_VCARD_SUPPORT  
                if(MMIPB_OPC_EXPORT == handle_node.cmd_type)
                {
                    MMIPB_ExportVcardEnd(applet_ptr->instance.handle);
                }
#endif
            }
            MMIPB_CloseHandle(applet_ptr->instance.handle);
            MMIQUEUE_RemoveNode(s_mmipb_applet_queue, MMIQUEUE_GetNodeIndex(s_mmipb_applet_queue, &applet_ptr->caf_applet.app_handle,CmpAppletHandle));
        } 
        if(MMIQUEUE_QueueIsEmpty(s_mmipb_applet_queue))//���е�applet�˳��� ������˽����Ϊ��û�н�����״̬
        {
#ifdef SNS_SUPPORT            
            MMIPB_CloseSNSDB();/*lint !e718 !e18*/
#endif
            MMIPB_SetCLUnProtect(FALSE);
            MMIPB_SetProtectStatus(FALSE);
        }
       
        break;
    case MSG_APPLET_SUSPEND:
        
        break;
    case MSG_APPLET_RESUME: 
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : applet handle  compare function
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  CmpAppletHandle(void *data_ptr1, void *data_ptr2)
{
    BOOLEAN result = FALSE;
    
    if(data_ptr1 != PNULL && data_ptr2 != PNULL)
    {
        if(*(MMI_HANDLE_T *)data_ptr1 == *(MMI_HANDLE_T *)data_ptr2)
        {
            result = TRUE;
        }
    }
    return result;
    
}

/*****************************************************************************/
//  Description : ����ѡ���б�Ľڵ�
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:����ѡ���б�
                        //for export  ֻ��Ҫ��file_path������storage id �� entry id��ֵ
                        //for copy  ��Ҫ��to_storage_id���ƣ� �Լ���Ҫ�� contact ������ֵ
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_CreateSelNode(MMI_HANDLE_T handle, MMIPB_SEL_DATA* sel_data)
{
    BOOLEAN ret = FALSE;
    MMIPB_SEL_NODE_T* s_sel_node_ptr = PNULL;
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);
    
    if(applet_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_CreateSelNode PNULL == handle_node_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_453_112_2_18_2_43_5_21,(uint8*)"");
        return FALSE;
    }

    //+for coverity
    if(PNULL == sel_data)
    {
        SCI_TRACE_LOW("[mmipb] MMIPB_CreateSelNode PNULL == sel_data");
        return FALSE;
    }
MMI_CheckAllocatedMemInfo();
    s_sel_node_ptr = MMIPB_MALLOC(sizeof(MMIPB_SEL_NODE_T));
MMI_CheckAllocatedMemInfo();
    if(PNULL== s_sel_node_ptr)
    {
        //SCI_TRACE_LOW:"MMIPB_CreateSelNode PNULL== s_sel_node_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_459_112_2_18_2_43_5_22,(uint8*)"");
        return FALSE;
    }
    else
    {
        SCI_MEMSET(s_sel_node_ptr, 0x00, sizeof(MMIPB_SEL_NODE_T));
        if(PNULL == applet_ptr->instance.select_list)//��handle�󶨵�ѡ���б�����ݽṹ��û�з���
        {
        
            applet_ptr->instance.select_list = MMIPB_MALLOC(sizeof(MMIPB_HANDLE_DATA_T));
            if(PNULL == applet_ptr->instance.select_list)
            {
                //SCI_TRACE_LOW:"MMIPB_CreateSelNode PNULL== handle_node_ptr ->select_listr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_471_112_2_18_2_43_5_23,(uint8*)"");
                return FALSE;
            }
            SCI_MEMSET(applet_ptr->instance.select_list, 0x00, sizeof(MMIPB_HANDLE_DATA_T));
        }

        applet_ptr->instance.select_list->to_storage_id =sel_data->to_storage_id;
        applet_ptr->instance.select_list->group_id   = sel_data->group_id;        
        MMIAPICOM_Wstrcpy (applet_ptr->instance.select_list->file_path, sel_data->file_path);
        s_sel_node_ptr->entry_id = sel_data->entry_id;
        s_sel_node_ptr->storage_id = sel_data->storage_id;
        s_sel_node_ptr->num_index = sel_data->num_index;
        s_sel_node_ptr->is_in_group = sel_data->is_in_group;
        s_sel_node_ptr->sel_field = sel_data->sel_field;
        s_sel_node_ptr->is_manage_group = sel_data->is_manage_group;
        if(PNULL == applet_ptr->instance.select_list->sel_table_head )//ѡ���б�û�д���
        {
            applet_ptr->instance.select_list->sel_table_tail = s_sel_node_ptr;
            applet_ptr->instance.select_list->sel_table_head = s_sel_node_ptr;
            applet_ptr->instance.select_list->sel_table_head->next = PNULL;
            applet_ptr->instance.select_list->sel_table_tail->next = PNULL;
            applet_ptr->instance.select_list->sel_count = 1;
            applet_ptr->instance.select_list->error_count = 0;
        }
        else
        {
            applet_ptr->instance.select_list->sel_table_tail->next = s_sel_node_ptr;
            applet_ptr->instance.select_list->sel_table_tail = applet_ptr->instance.select_list->sel_table_tail->next;
            applet_ptr->instance.select_list->sel_table_tail->next = PNULL;
            applet_ptr->instance.select_list->sel_count++;
        }       
    }
MMI_CheckAllocatedMemInfo();
    return ret;
}

/*****************************************************************************/
//  Description : �õ�ѡ�������ͷ���
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_SEL_NODE_T* MMIPB_GetSelNode(MMI_HANDLE_T  handle, uint16 entry_id, uint16 storage_id, uint16 num_index)
{
    MMIPB_SEL_NODE_T*  sel_node_ptr =PNULL;
    //int             i = 0;
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);
    if(applet_ptr == PNULL)
    {
        SCI_TRACE_LOW("MMIPB] MMIPB_GetHeadSelNode PNULL == applet_ptr");
        return sel_node_ptr;
    }
    if(PNULL == applet_ptr->instance.select_list)
    {
        SCI_TRACE_LOW("MMIPB] MMIPB_GetHeadSelNode PNULL == applet_ptr->instance.select_list");
        return sel_node_ptr;
    }
    sel_node_ptr = applet_ptr->instance.select_list->sel_table_head;
    while(PNULL != sel_node_ptr)
    {
        if( (entry_id==sel_node_ptr->entry_id) &&(storage_id == sel_node_ptr->storage_id) && (num_index == sel_node_ptr->num_index))
        {
            break;
        }
        else
        {
            sel_node_ptr= sel_node_ptr->next;
        }
    }
   
   return sel_node_ptr;
}  
/*****************************************************************************/
//  Description :����ѡ���б�ͷ���
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIPB_FindSelNode(MMI_HANDLE_T  handle, uint16 entry_id, uint16 storage_id, uint16 num_index)
{
    BOOLEAN    ret = FALSE;
    MMIPB_SEL_NODE_T*  sel_node_ptr =PNULL;
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);

    if(applet_ptr == PNULL)
    {
        return FALSE;
    }

    if(PNULL == applet_ptr->instance.select_list)
    {
        return FALSE;
    }
    sel_node_ptr = applet_ptr->instance.select_list->sel_table_head;
    while(PNULL!=sel_node_ptr)
    {
        if((sel_node_ptr->entry_id == entry_id)&&(sel_node_ptr->storage_id == storage_id) && (sel_node_ptr->num_index == num_index))
        {
            if(sel_node_ptr->is_manage_group)
            {
                if(sel_node_ptr->is_in_group &0X0F)
                {
                    ret = TRUE;
                }
            }
            else
            {
            
                ret = TRUE;
            }
            break;
        }
        sel_node_ptr=sel_node_ptr->next;
    }
    return ret;
    
}



/*****************************************************************************/
//  Description : ɾ���ض���ѡ������ڵ�
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIPB_DeleteSelNode(MMI_HANDLE_T  handle, uint16 entry_id, uint16 storage_id, uint16 num_index)
{
    BOOLEAN    ret = FALSE;
    MMIPB_SEL_NODE_T* sel_node_ptr =PNULL;
    MMIPB_SEL_NODE_T* pre_sel_node_ptr = PNULL;
    MMIPB_APPLET_T* applet_ptr = GetPBApplet(handle);

    if(PNULL == applet_ptr || applet_ptr->instance.select_list == PNULL)
    {
        return ret;
    }
    if(applet_ptr->instance.select_list->sel_count == 0)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_DeleteSelHeadNode handle_data_ptr->sel_count %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_613_112_2_18_2_43_5_27,(uint8*)"d", applet_ptr->instance.select_list->sel_count);
        return FALSE;
    }
    sel_node_ptr = applet_ptr->instance.select_list->sel_table_head;
    while(PNULL!=sel_node_ptr)
    {
        if((sel_node_ptr->entry_id == entry_id)&&(sel_node_ptr->storage_id == storage_id)&& (sel_node_ptr->num_index == num_index))
        {
            if(PNULL != pre_sel_node_ptr)
            {
                if(sel_node_ptr->next == PNULL)//ɾ��βָ��
                {
                    applet_ptr->instance.select_list->sel_table_tail = pre_sel_node_ptr;
                }
                pre_sel_node_ptr->next = sel_node_ptr->next;
            }
            else //ɾ��ͷָ��
            {
                applet_ptr->instance.select_list->sel_table_head = sel_node_ptr->next;
            }
            MMIPB_MFREE(sel_node_ptr);
            sel_node_ptr = PNULL;
            applet_ptr->instance.select_list->sel_count--;
            ret = TRUE;
            break;
        }
        pre_sel_node_ptr =sel_node_ptr;
        sel_node_ptr=sel_node_ptr->next;
    }
    return ret;
}
/*****************************************************************************/
//  Description : ѡ���б��Ƿ���ڽڵ�
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_HasSelNode(MMI_HANDLE_T  handle)
{
    BOOLEAN           ret = FALSE; 
    MMIPB_APPLET_T *applet_ptr= GetPBApplet(handle);

    if(applet_ptr== PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_HasSelNode PNULL == handle_node_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_663_112_2_18_2_43_5_28,(uint8*)"");
        return FALSE;
    }
    if(PNULL == applet_ptr->instance.select_list)
    {
        return FALSE;
    }
    if(PNULL != applet_ptr->instance.select_list->sel_table_head)//ѡ���б�û�д���
    {
        ret = TRUE;
    }
    return ret;
}
/*****************************************************************************/
//  Description : ɾ��ѡ���б�ͷ���
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_DeleteSelHeadNode(MMI_HANDLE_T  handle)
{
    MMIPB_SEL_NODE_T*  sel_node_ptr =PNULL;
    MMIPB_APPLET_T *applet_ptr= GetPBApplet(handle);

    if(applet_ptr== PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_DeleteSelHeadNode PNULL == handle_node_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_690_112_2_18_2_43_5_29,(uint8*)"");
        return;
    }

    if(PNULL == applet_ptr->instance.select_list)return;

    if(PNULL == applet_ptr->instance.select_list->sel_table_head)//ѡ���б�û�д���
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_DeleteSelNode list is PNULL handle %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_699_112_2_18_2_43_5_30,(uint8*)"d", handle);
    }
    else
    {
       sel_node_ptr = applet_ptr->instance.select_list->sel_table_head;
       applet_ptr->instance.select_list->sel_table_head = applet_ptr->instance.select_list->sel_table_head->next;

       MMIPB_MFREE(sel_node_ptr);
       sel_node_ptr = PNULL;

       if(applet_ptr->instance.select_list->sel_count>0)
       {       
            applet_ptr->instance.select_list->sel_count--;
       } 
    }
    return;
}

/*****************************************************************************/
//  Description : ɾ������ѡ���б�Ľڵ�
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DeleteAllSelNode(MMI_HANDLE_T  handle)
{
    MMIPB_HANDLE_NODE_T   handle_node = {0};
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);

    if(!MMIPB_GetHandleContent(handle, &handle_node))
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_DeleteAllSelNode PNULL == handle_node_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_740_112_2_18_2_43_5_31,(uint8*)"");
        return FALSE;
    }
#ifdef MMI_VCARD_SUPPORT      
    if(MMIPB_OPC_IMPORT == handle_node.cmd_type)
    {
        //import destory file array
        MMIPB_CleanSelectFileInfo(handle);
    }
    //else //��֤����cmd_type��˳���ߵ����´���
#endif
    {
        if(applet_ptr != PNULL && applet_ptr->instance.select_list != PNULL)
        {
            DeleteSelectList(applet_ptr->instance.select_list);
            MMIPB_MFREE(applet_ptr->instance.select_list);
            applet_ptr->instance.select_list = PNULL;
        }        
    }
    return TRUE;
}


#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : �����û�ѡ����ļ�������Ϣ
//                Save selected files information
//  Global resource dependence : 
//  Author:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SaveSelectFileInfo(MMIPB_HANDLE_T handle,
                                        FILEARRAY file_array)
{
    BOOLEAN result = FALSE;
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);

    if(applet_ptr != PNULL)
    {
        SCI_MEMSET(&applet_ptr->instance.select_files, 0x00, sizeof(MMIPB_SELECT_FILE_T));
        applet_ptr->instance.select_files.total_file_count = MMIAPIFILEARRAY_GetArraySize(file_array);
        applet_ptr->instance.select_files.curent_index = 0;
        applet_ptr->instance.select_files.file_array = file_array;
        result = TRUE;
    }
    return result;
}


/*****************************************************************************/
//  Description : ��ȡѡ�����һ���ļ���ȫ·��
//  Global resource dependence : 
//  Author:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetNextSelectFileName(MMI_HANDLE_T handle,
                                           uint16 max_file_name_len,
                                           wchar *file_name_ptr)
{
    BOOLEAN result = FALSE;
    FILEARRAY_DATA_T file_data = {0};
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);
    
    if(applet_ptr != PNULL)
    {             
        if(MMIAPIFILEARRAY_Read(applet_ptr->instance.select_files.file_array,
            applet_ptr->instance.select_files.curent_index,
            &file_data))
        {
            if(file_name_ptr != PNULL)
            {
                MMIAPICOM_Wstrncpy(file_name_ptr, file_data.filename, MIN(max_file_name_len, file_data.name_len));
                result = TRUE;
            } 
            applet_ptr->instance.select_files.curent_index++;
        }
    }     
    return result;     
}

/*****************************************************************************/
//  Description : ���ѡ����ļ�������Ϣ
//  Global resource dependence : 
//  Author:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_CleanSelectFileInfo(MMIPB_HANDLE_T handle)
{
    BOOLEAN result = FALSE;
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);

    if(applet_ptr != PNULL)
    {
        MMIAPIFILEARRAY_Destroy(&applet_ptr->instance.select_files.file_array);
        SCI_MEMSET(&applet_ptr->instance.select_files, 0x00, sizeof(applet_ptr->instance.select_files));
        result = TRUE;
    }

    return result;    
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E HandleImportContactResult(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    MMIPB_HANDLE_NODE_T   handle_node = {0};
    MMIPB_APPLET_T*        applet_ptr = PNULL;
    MMI_HANDLE_T            applet_handle = NULL;
    uint16                 count = 0;
    MMIPB_ERROR_E          ret = MMIPB_ERROR_ERROR;
    MMIPB_UPDATE_MSG       msg_id = 0;
    
    if(!MMIPB_GetHandleContent(handle, &handle_node))
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_902_112_2_18_2_43_6_32,(uint8*)"");
        return MMIPB_ERROR_ERROR;
    }
    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(handle_node.applet_handle);
    if(PNULL == applet_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }

    applet_handle = applet_ptr->caf_applet.app_handle;
    count = applet_ptr->instance.select_files.curent_index;
    if(MMIPB_ERROR_SUCCESS == error)
    {
        MMK_PBTriggerEvent(MMIEVENT_PB_UPDATE);
    }
    else 
    {
        //read vcard fail
        ret = error;        
    }
    if(ret != MMIPB_ERROR_NO_SPACE)
    {
        ret = MMIPB_ImportContactAsynForPB(handle);
        if(MMIPB_ERROR_NO_SPACE == ret)
        {
            error = MMIPB_ERROR_PHONE_NO_SPACE;
        }
    }
    //send msg to windows
    if(MMIPB_ERROR_PENDING == ret)  
    {
        msg_id = MMIPB_OPERATE_UPDATE;            
    }
    else if(MMIPB_ERROR_NO_ENTRY == ret)
    {
        msg_id = MMIPB_OPERATE_SUCCESS;
    }
    else
    {
        msg_id = MMIPB_OPERATE_FAIL;
    }
    if(MMIPB_ERROR_PHONE_NO_SPACE == ret
       || MMIPB_ERROR_NO_SPACE == ret
       || MMIPB_ERROR_NO_ENTRY == ret
       )
    {
        MMIPB_DeleteAllSelNode(handle);
    }
    if(applet_ptr->instance.update_callback != PNULL)
    {
        applet_ptr->instance.update_callback(applet_handle, msg_id, error,MMIPB_OPC_IMPORT,count);
    }
    SCI_TRACE_LOW("[MMIPB] HandleImportContactResult msg_id 0x%x error 0x%x ret 0x%x", msg_id, error, ret);
    return error;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
LOCAL void HandleExportContactResult(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    MMIPB_APPLET_T* applet_ptr = GetPBApplet(handle);
    MMI_HANDLE_T    applet_handle = NULL;
    MMIPB_HANDLE_NODE_T handle_node = {0};
    uint16         error_count = 0;
    uint16         sel_count = 0;
    MMIPB_ERROR_E  ret = MMIPB_ERROR_ERROR;
	uint32		finished_count = 0; //��¼�ѳɹ�����ļ�¼��Ŀ
    if(PNULL == applet_ptr || applet_ptr->instance.select_list == PNULL)
    {
        SCI_TRACE_LOW("[MMIPB] HandleExportContactResult PNULL ");
        return;
    }
    SCI_TRACE_LOW("yanyan HandleExportContactResult error:%d", error);
    applet_handle = applet_ptr->caf_applet.app_handle;
	finished_count = applet_ptr->instance.select_list->opc_count; 
    applet_ptr->instance.select_list->opc_count++;
    MMIPB_DeleteSelHeadNode(handle);
    if(error == MMIPB_ERROR_NO_SPACE)
    {
        MMIPB_ExportVcardEnd(handle);
        MMIPB_DeleteAllSelNode(handle); 
        if(applet_ptr->instance.update_callback != PNULL)
        {
            applet_ptr->instance.update_callback(applet_handle, 
                MMIPB_OPERATE_FAIL, 
                MMIPB_ERROR_FMM_NO_SPACE,
                MMIPB_OPC_EXPORT,
                0);
        }

		if(0 == finished_count)
		{
            MMIPB_DeleteCurVcardFile();
		}

    }
    else if(PNULL!=applet_ptr->instance.select_list->sel_table_head)//ѡ�������л���nodeҪ����copy����
    {
        if(applet_ptr->instance.update_callback != PNULL)
        {
            applet_ptr->instance.update_callback(applet_handle, 
                MMIPB_OPERATE_UPDATE, 
                error,
                MMIPB_OPC_EXPORT,
                applet_ptr->instance.select_list->opc_count);
            
        }
        applet_ptr = GetPBApplet(handle);
        if(MMIPB_GetHandleContent(handle, &handle_node) 
          && (applet_ptr != PNULL) 
          && applet_ptr->instance.select_list != PNULL
          && applet_ptr->instance.select_list->sel_table_head != PNULL)
        {
         ret = MMIPB_ExportNextVcardAsyn(handle, 
                        applet_ptr->instance.select_list->sel_table_head->entry_id, 
                        applet_ptr->instance.select_list->sel_table_head->storage_id,
                       handle_node.access_point.file_handle, 
                       handle_node.callback);
        }
         if(ret != MMIPB_ERROR_PENDING)
         {
             MMIPB_ExportVcardEnd(handle);
         }
    }
    else
    {
        
        MMIPB_ExportVcardEnd(handle);
        error_count = applet_ptr->instance.select_list->error_count;
        sel_count = applet_ptr->instance.select_list->sel_count;
        MMIPB_DeleteAllSelNode(handle);
//         if(applet_ptr->instance.select_list != PNULL)
//         {
//             MMIPB_MFREE(applet_ptr->instance.select_list);
//             applet_ptr->instance.select_list = PNULL;
//         }
        if(MMIPB_ERROR_SUCCESS == error)
        {
               // MMIAPIPB_UpdateList(contact_ptr);
            if(error_count == 0)
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle ,MMIPB_OPERATE_SUCCESS, error,MMIPB_OPC_EXPORT,0);
                }
                //��applet���ɹ���Ϣ
            }
            else
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle, 
                        MMIPB_OPERATE_PARTLY_SUCCESS, 
                        error,
                        MMIPB_OPC_EXPORT,
                        error_count);
                }
                //��applet�����ֳɹ���Ϣ
            }

        }
        else
        {
            error_count++;
            if(sel_count> error_count)//�������ĿС���ܹ�Ҫcopy����Ŀ
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle, 
                        MMIPB_OPERATE_PARTLY_SUCCESS, 
                        error,
                        MMIPB_OPC_EXPORT,
                        error_count);
                }
                //��applet�����ֳɹ���Ϣ
            }
            else
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_FAIL, error,MMIPB_OPC_EXPORT,0);
                }
                //��applet��ʧ����Ϣ
            }
        }
    }
    SCI_TRACE_LOW("[MMIPB] HandleExportContactResult error %d ret %d", error, ret); 
}
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
LOCAL void HandleManageGroupResult(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    MMIPB_APPLET_T        *applet_ptr = GetPBApplet(handle);
    MMI_HANDLE_T          applet_handle = NULL;
    MMIPB_ERROR_E         ret = MMIPB_ERROR_ERROR;
    uint16         error_count = 0;
    uint16         sel_count = 0;

    if(PNULL == applet_ptr || applet_ptr->instance.select_list == PNULL)
    {
        return;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;
    applet_ptr->instance.select_list->opc_count++;
    MMIPB_DeleteSelHeadNode(handle);
    if(PNULL!=applet_ptr->instance.select_list->sel_table_head)//ѡ�������л���nodeҪ����ɾ������
    {
        if(applet_ptr->instance.update_callback != PNULL)
        {
            applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_UPDATE, error,MMIPB_OPC_MANAGE_GROUP,applet_ptr->instance.select_list->opc_count);
        }
        applet_ptr = GetPBApplet(handle);
        if(applet_ptr != PNULL &&applet_ptr->instance.select_list != PNULL)
        {
            ret = MMIPB_ManageGroupContactAsynForPB(handle, applet_ptr->instance.select_list->group_id); 
        }
    }

    applet_ptr = GetPBApplet(handle);
    if(ret == MMIPB_ERROR_PENDING)// to to next
    {
        //nothing to do
    }
    else if( (applet_ptr != PNULL)
            &&(applet_ptr->instance.select_list != PNULL)
            &&((ret == MMIPB_ERROR_NO_ENTRY) || (PNULL==applet_ptr->instance.select_list->sel_table_head))
            ) //no entry
    {

        error_count = applet_ptr->instance.select_list->error_count;
        sel_count = applet_ptr->instance.select_list->sel_count;  
        MMIPB_DeleteAllSelNode(handle);

        if(MMIPB_ERROR_SUCCESS == error)
        {
            if(error_count == 0)
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_SUCCESS, error,MMIPB_OPC_MANAGE_GROUP,0);
                }
                //��applet���ɹ���Ϣ
            }
            else
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_PARTLY_SUCCESS, error,MMIPB_OPC_MANAGE_GROUP,error_count);
                }
                //��applet�����ֳɹ���Ϣ
            }

        }
        else
        {
            error_count++;
            if(sel_count> error_count)//�������ĿС���ܹ�Ҫɾ������Ŀ
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_PARTLY_SUCCESS, error,MMIPB_OPC_MANAGE_GROUP,error_count);
                }
                //��applet�����ֳɹ���Ϣ
            }
            else
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle,MMIPB_OPERATE_FAIL, error,MMIPB_OPC_MANAGE_GROUP,0);
                }
                //��applet��ʧ����Ϣ
            }
        }
    } 

}

#endif

#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
LOCAL void HandleRemoveFavouriteResult(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    MMIPB_APPLET_T*        applet_ptr = GetPBApplet(handle);
    MMI_HANDLE_T            applet_handle = NULL;
    
    if(PNULL == applet_ptr || applet_ptr->instance.select_list == PNULL)
    {
        return;
    }

    applet_handle = applet_ptr->caf_applet.app_handle;
    applet_ptr->instance.select_list->opc_count++;
    MMIPB_DeleteSelHeadNode(handle);
    if(PNULL!=applet_ptr->instance.select_list->sel_table_head)//ѡ�������л���nodeҪ����ɾ������
    {
        if(applet_ptr->instance.update_callback != PNULL)
        {
            applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_UPDATE, error,MMIPB_OPC_REMOVE_FAVOURITE,applet_ptr->instance.select_list->opc_count);
        }
        MMIPB_RemoveContactFromFavouriteAsynForPB(handle);
        
    }
    else
    {
        uint16 error_count = applet_ptr->instance.select_list->error_count;
        uint16 sel_count = applet_ptr->instance.select_list->sel_count;

        MMIPB_DeleteAllSelNode(handle);
        if(MMIPB_ERROR_SUCCESS == error)
        {
            if(error_count == 0)
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_SUCCESS, error,MMIPB_OPC_REMOVE_FAVOURITE,0);
                }
                //��applet���ɹ���Ϣ
            }
            else
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_PARTLY_SUCCESS, error,MMIPB_OPC_REMOVE_FAVOURITE,error_count);
                }
                //��applet�����ֳɹ���Ϣ
            }

        }
        else
        {
            error_count++;
            if(sel_count> error_count)//�������ĿС���ܹ�Ҫɾ������Ŀ
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_PARTLY_SUCCESS, error,MMIPB_OPC_REMOVE_FAVOURITE,error_count);
                }
                //��applet�����ֳɹ���Ϣ
            }
            else
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle,MMIPB_OPERATE_FAIL, error,MMIPB_OPC_REMOVE_FAVOURITE,0);
                }
                //��applet��ʧ����Ϣ
            }
        }
    }

}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
LOCAL void HandleAddFavouriteResult(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);
    MMI_HANDLE_T   applet_handle = NULL;
    uint16        error_count = 0;
    uint16        sel_count = 0;
    if(PNULL == applet_ptr || PNULL == applet_ptr->instance.select_list)
    {
        return;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;
    applet_ptr->instance.select_list->opc_count++;
    MMIPB_DeleteSelHeadNode(handle);
    if(PNULL!=applet_ptr->instance.select_list->sel_table_head)//ѡ�������л���nodeҪ����ɾ������
    {
        if(applet_ptr->instance.update_callback != PNULL)
        {
            applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_UPDATE, error,MMIPB_OPC_ADD_FAVOURITE,applet_ptr->instance.select_list->opc_count);
        }
        MMIPB_AddContactToFavouriteAsynForPB(handle);
        
    }
    else
    {
        if(applet_ptr->instance.select_list != PNULL)
        {
            error_count = applet_ptr->instance.select_list->error_count;
            sel_count = applet_ptr->instance.select_list->sel_count;
            MMIPB_DeleteAllSelNode(handle);
        }

        if(MMIPB_ERROR_SUCCESS == error)
        {
                if(error_count == 0)
                {
                    if(applet_ptr->instance.update_callback != PNULL)
                    {
                        applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_SUCCESS, error,MMIPB_OPC_ADD_FAVOURITE,0);
                    }
                    //��applet���ɹ���Ϣ
                }
                else
                {
                    if(applet_ptr->instance.update_callback != PNULL)
                    {
                        applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_PARTLY_SUCCESS, error,MMIPB_OPC_ADD_FAVOURITE,error_count);
                    }
                    //��applet�����ֳɹ���Ϣ
                }
        }
        else
        {
             error_count++;
             if(sel_count> error_count)//�������ĿС���ܹ�Ҫɾ������Ŀ
             {
                 if(applet_ptr->instance.update_callback != PNULL)
                 {
                     applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_PARTLY_SUCCESS, error,MMIPB_OPC_ADD_FAVOURITE,error_count);
                 }
                 //��applet�����ֳɹ���Ϣ
             }
             else
             {
                 if(applet_ptr->instance.update_callback != PNULL)
                 {
                     applet_ptr->instance.update_callback(applet_handle,MMIPB_OPERATE_FAIL, error,MMIPB_OPC_ADD_FAVOURITE,0);
                 }
                 //��applet��ʧ����Ϣ
             }
        }
    }

}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E MMIPB_GetNoSpaceError(uint16 to_storage_id)
{

    uint16 storage = MMIPB_GET_STORAGE(to_storage_id);
    MMIPB_ERROR_E  error = MMIPB_ERROR_NO_SPACE;
    switch(storage)
    {
    case MMIPB_STORAGE_FLASH:
        error = MMIPB_ERROR_FMM_NO_SPACE;
        break;
    case MMIPB_STORAGE_PHONE:
        error = MMIPB_ERROR_PHONE_NO_SPACE;
        break;
    case MMIPB_STORAGE_SIM1:
#ifdef FDN_SUPPORT
        if(MMIAPIFDN_Is2GFdnEnable(MN_DUAL_SYS_1))
        {
            error = MMIPB_ERROR_FDN_ENABLE;
        }
        else
#endif
        {
            error = MMIPB_ERROR_SIM1_NO_SPACE;
        }
        break;
    case MMIPB_STORAGE_SIM2:
#ifdef FDN_SUPPORT
        if(MMIAPIFDN_Is2GFdnEnable(MN_DUAL_SYS_1 +1))
        {
            error = MMIPB_ERROR_FDN_ENABLE;
        }
        else
#endif
        {
            error = MMIPB_ERROR_SIM2_NO_SPACE;
        }
        break;
    case MMIPB_STORAGE_SIM3:
#ifdef FDN_SUPPORT
        if(MMIAPIFDN_Is2GFdnEnable(MN_DUAL_SYS_1 +2))
        {
            error = MMIPB_ERROR_FDN_ENABLE;
        }
        else
#endif
        {
            error = MMIPB_ERROR_SIM3_NO_SPACE;
        }
        break;
    case MMIPB_STORAGE_SIM4:
#ifdef FDN_SUPPORT
        if(MMIAPIFDN_Is2GFdnEnable(MN_DUAL_SYS_1 +3))
        {
            error = MMIPB_ERROR_FDN_ENABLE;
        }
        else
#endif
        {
            error = MMIPB_ERROR_SIM4_NO_SPACE;
        }
        break;
    default:
        break;
    }
    return error;
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
LOCAL void HandleCopyContactResult(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    MMIPB_APPLET_T      *applet_ptr = GetPBApplet(handle);
    MMI_HANDLE_T        applet_handle = NULL;
    MMIPB_ERROR_E       ret = MMIPB_ERROR_ERROR;
    uint16              error_count = 0;
    uint16              sel_count = 0;

    if(PNULL == applet_ptr || PNULL == applet_ptr->instance.select_list)
    {
        return;
    }
    applet_handle = applet_ptr->caf_applet.app_handle;

    if(applet_ptr->instance.update_callback != PNULL)
    {
        applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_UPDATE, error,MMIPB_OPC_COPY,applet_ptr->instance.select_list->opc_count);
    }
    applet_ptr = GetPBApplet(handle);
    //fix bug932834 ��  ���copy��ɾ��sel table�е����ݣ���ʼ�¸����ݵ�copy
    applet_ptr->instance.select_list->opc_count++;
    MMIPB_DeleteSelHeadNode(handle);
    //�������
    if(applet_ptr!= PNULL && applet_ptr->instance.select_list != PNULL)
    {
        ret = MMIPB_CopyMultiContactAsyn(handle, applet_ptr->instance.select_list->to_storage_id);
    }
    if(MMIPB_ERROR_PENDING !=ret)
    {
        //���ƽ���
        applet_ptr = GetPBApplet(handle);
        if(applet_ptr != PNULL && applet_ptr->instance.select_list != PNULL)
        {
            sel_count = applet_ptr->instance.select_list->sel_count;
            error_count = applet_ptr->instance.select_list->error_count;           
        }
 
        MMIPB_DeleteAllSelNode(handle);
        if((MMIPB_ERROR_SUCCESS != ret)
            && (MMIPB_ERROR_NO_ENTRY != ret)
            )
        {
            //������һ��ʧ��
            if(applet_ptr != PNULL && applet_ptr->instance.update_callback != PNULL)
            {
                applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_FAIL, ret,MMIPB_OPC_COPY,0);   
            }
        }
        else
        {
            //�ɹ�     
            
            if(MMIPB_ERROR_SUCCESS == error)
            {
                if(error_count == 0)
                {
                    if(applet_ptr != PNULL && applet_ptr->instance.update_callback != PNULL)
                    {
                        applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_SUCCESS, error,MMIPB_OPC_COPY,0);
                    }
                    //��applet���ɹ���Ϣ
                }
                else
                {
                    if(applet_ptr != PNULL && applet_ptr->instance.update_callback != PNULL)
                    {
                        applet_ptr->instance.update_callback(applet_handle, 
                            MMIPB_OPERATE_PARTLY_SUCCESS, 
                            error,
                            MMIPB_OPC_COPY,
                            error_count);
                    }
                    //��applet�����ֳɹ���Ϣ
                }                
            }
            else
            {
                error_count++;
                if(sel_count > error_count)//�������ĿС���ܹ�Ҫcopy����Ŀ
                {
                    if(applet_ptr != PNULL && applet_ptr->instance.update_callback != PNULL)
                    {
                        applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_PARTLY_SUCCESS, error,MMIPB_OPC_COPY, error_count);
                    }
                    //��applet�����ֳɹ���Ϣ
                }
                else
                {
                    if(applet_ptr != PNULL && applet_ptr->instance.update_callback != PNULL)
                    {                    
                        applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_FAIL, error,MMIPB_OPC_COPY,0);
                    }
                    //��applet��ʧ����Ϣ
                }
            }

        }         

    }


}




/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
LOCAL void HandleDeleteContactResult(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    MMIPB_APPLET_T*        applet_ptr = GetPBApplet(handle);
    MMI_HANDLE_T            applet_handle = NULL;
    uint16 error_count = 0;
    uint16 sel_count = 0;

    if(PNULL == applet_ptr || PNULL == applet_ptr->instance.select_list)
    {
        return;
    }

    applet_handle = applet_ptr->caf_applet.app_handle;

    applet_ptr->instance.select_list->opc_count++;
    MMIPB_DeleteSelHeadNode(handle);
    if(PNULL!=applet_ptr->instance.select_list->sel_table_head)//ѡ�������л���nodeҪ����ɾ������
    {
        if(applet_ptr->instance.update_callback != PNULL)
        {
            applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_UPDATE, error,MMIPB_OPC_DELETE,applet_ptr->instance.select_list->opc_count);
        }
        MMIPB_DeleteContactAsynForPB(handle);
        
    }
    else
    {
        if(applet_ptr->instance.select_list != PNULL)
        {
            error_count = applet_ptr->instance.select_list->error_count;
            sel_count = applet_ptr->instance.select_list->sel_count;
            MMIPB_DeleteAllSelNode(handle);
        }
 
        if(MMIPB_ERROR_SUCCESS == error)
        {
                if(error_count == 0)
                {
                    if(applet_ptr->instance.update_callback != PNULL)
                    {
                        applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_SUCCESS, error,MMIPB_OPC_DELETE,0);
                        //��applet���ɹ���Ϣ
                    }
                }
                else
                {
                    if(applet_ptr->instance.update_callback != PNULL)
                    {
                        applet_ptr->instance.update_callback(applet_handle, 
                            MMIPB_OPERATE_PARTLY_SUCCESS, 
                            error,
                            MMIPB_OPC_DELETE,
                            error_count);
                    }
                    //��applet�����ֳɹ���Ϣ
                }

        }
        else
        {
            error_count++;
            if(sel_count> error_count)//�������ĿС���ܹ�Ҫɾ������Ŀ
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_PARTLY_SUCCESS, error,MMIPB_OPC_DELETE,error_count);
                }
                //��applet�����ֳɹ���Ϣ
            }
            else
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle,MMIPB_OPERATE_FAIL, error,MMIPB_OPC_DELETE,0);
                }
                //��applet��ʧ����Ϣ
            }
        }
    }
  
}

/*****************************************************************************/
//  Description :����ѡ���б����һ���ڵ���¼�
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
LOCAL void MMIPB_HandleSelNodeFunc(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    MMIPB_HANDLE_NODE_T   handle_node = {0};
    MMIPB_APPLET_T*        applet_ptr = GetPBApplet(handle);
    MMI_HANDLE_T            applet_handle = PNULL;

    if(!MMIPB_GetHandleContent(handle, &handle_node))
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == handle_node_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_1552_112_2_18_2_43_7_39,(uint8*)"");
        return;
    }

    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_HandleSelNodeSuccess PNULL == applet_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_1558_112_2_18_2_43_7_40,(uint8*)"");
        return;
    }
   SCI_TRACE_LOW("yanyan MMIPB_HandleSelNodeFunc cmd:%d, error:%d", applet_ptr->instance.cmd, error);
   if(MMIPB_ERROR_CANCEL == error)
   {
#ifdef MMI_VCARD_SUPPORT  
        if(MMIPB_OPC_EXPORT == applet_ptr->instance.cmd)
        {
            MMIPB_ExportVcardEnd(handle);
            MMIPB_DeleteAllSelNode(handle);
        }
#endif
       if( applet_ptr->instance.update_callback != PNULL)
       {
           applet_ptr->instance.update_callback(handle_node.applet_handle, MMIPB_OPERATE_PARTLY_SUCCESS, error,applet_ptr->instance.cmd,0);
       }
       return;
		  
   }
    applet_handle = applet_ptr->caf_applet.app_handle;
   
    switch(applet_ptr->instance.cmd)
    {
    case MMIPB_OPC_ADD:
        if(PNULL != applet_ptr->instance.select_list)
        {        
            applet_ptr->instance.select_list->opc_count++;
            MMIPB_DeleteSelHeadNode(handle);
            if(applet_ptr->instance.select_list->sel_count == 0)
            {
                MMIPB_MFREE(applet_ptr->instance.select_list);
                applet_ptr->instance.select_list = PNULL;
            }
        }
        if(MMIPB_ERROR_SUCCESS == error)
        {
            if(applet_ptr->instance.update_callback != PNULL)
            {
                applet_ptr->instance.update_callback(applet_handle,MMIPB_OPERATE_SUCCESS,error,MMIPB_OPC_ADD ,0);
            }
            //��applet���ɹ���Ϣ
        }
        else
        {
            if(applet_ptr->instance.update_callback != PNULL)
            {
                applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_FAIL,error, MMIPB_OPC_ADD, 0);
            }
            //��applet��ʧ����Ϣ
        }
        
        break;

    case MMIPB_OPC_DELETE:
        HandleDeleteContactResult(handle, error);

        break;
    case MMIPB_OPC_EDIT:
        if(PNULL != applet_ptr->instance.select_list)
        {
            applet_ptr->instance.select_list->opc_count++;
            MMIPB_DeleteSelHeadNode(handle);
            if(applet_ptr->instance.select_list->sel_count == 0)
            {
                MMIPB_MFREE(applet_ptr->instance.select_list);
                applet_ptr->instance.select_list = PNULL;
            }
        }
        if(MMIPB_ERROR_SUCCESS == error)
        {
            if(applet_ptr->instance.update_callback != PNULL)
            {
                applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_SUCCESS, error,MMIPB_OPC_EDIT,0);
                //��applet���ɹ���Ϣ
            }
        }
        else
        {
            if(applet_ptr->instance.update_callback != PNULL)
            {
                applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_FAIL, error,MMIPB_OPC_EDIT,0);
            }
            //��applet��ʧ����Ϣ
        }

        break;

    case MMIPB_OPC_COPY:
        HandleCopyContactResult(handle, error);
        break;
#ifdef MMI_VCARD_SUPPORT
    case MMIPB_OPC_IMPORT:
        //read vcard callback  
        HandleImportContactResult(handle, error);      
        break;
    case MMIPB_OPC_EXPORT:
        HandleExportContactResult(handle, error);
        break;
#endif
#ifdef MMIPB_MOST_USED_SUPPORT
     case MMIPB_OPC_ADD_FAVOURITE:
         HandleAddFavouriteResult(handle, error);
         break;
     case MMIPB_OPC_REMOVE_FAVOURITE:
         HandleRemoveFavouriteResult(handle, error);
         break;
#endif         
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
     case MMIPB_OPC_MANAGE_GROUP:
         HandleManageGroupResult(handle, error);
         break;

#endif        
    default:
        if(PNULL != handle_node.data_ptr)
        {
            SCI_FREE(handle_node.data_ptr);
            handle_node.data_ptr =   PNULL;
            MMIPB_SetHandleContent(handle, &handle_node);
        }
        break;
    }

    
    
}

/*****************************************************************************/
//  Description : �жϸ�Ӧ���Ƿ������
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsPermitOpen(MMIPB_LIST_TYPE_E app_type)
{
    uint32 i = 0;
    BOOLEAN result = TRUE;
    MMI_HANDLE_T applet_handle = NULL;
    MMIPB_APPLET_T*    applet_ptr = PNULL;

    if(MMIPB_LIST_NORMAL_MAIN == app_type)
    {
        //������Ӧ�ò���Ҫ����
        if(s_mmipb_applet_queue != PNULL)
        {
            for (i=0; i< s_mmipb_applet_queue->current_count; i++)
            {
                applet_handle =  *(MMI_HANDLE_T *)MMIQUEUE_PeekQueue(s_mmipb_applet_queue, i);
                applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);  
                if(PNULL != applet_ptr)
                {
                    if(app_type == applet_ptr->instance.list_type)
                    {
                        MMIPB_CloseApplet(applet_handle);
                        result = TRUE;
                        
                        break;
                    }
                }
            } 
        }
    }
    return result;
}

/*****************************************************************************/
//  Description : is the contact will be add is valid
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void mmipb_app_callback(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{

    MMIPB_HANDLE_NODE_T   handle_node = {0};

    if(!MMIPB_GetHandleContent(handle, &handle_node))
    {
        return;
    }
    //SCI_TRACE_LOW:"[MMIPB] mmipb_app_callback handle %d error %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_1718_112_2_18_2_43_7_41,(uint8*)"dd", handle, error);
    if(s_mmipb_is_abort)
    {        
        if(PNULL != handle_node.data_ptr)
        {
            SCI_FREE(handle_node.data_ptr);
            handle_node.data_ptr =   PNULL;
            MMIPB_SetHandleContent(handle, &handle_node);
        }
        MMIPB_HandleSelNodeFunc(handle, MMIPB_ERROR_CANCEL);                
        s_mmipb_is_abort = FALSE;
    }
    else
    {
        MMIPB_HandleSelNodeFunc(handle, error);
    }
}

/*****************************************************************************/
//  Description : ���ò�������
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
LOCAL void MMIPB_SetOpcType(MMIPB_HANDLE_T handle, MMIPB_OPC_E opc_type)
{

    MMI_HANDLE_T applet_handle =NULL;
    MMIPB_APPLET_T* applet_ptr = PNULL;
    MMIPB_HANDLE_NODE_T   handle_node = {0};
    if(!MMIPB_GetHandleContent(handle, &handle_node))
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_SetOpcType PNULL == handle_node_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_1787_112_2_18_2_43_8_43,(uint8*)"");
        return;
    }
    handle_node.cmd_type = opc_type;
    MMIPB_SetHandleContent(handle, &handle_node);
    applet_handle = handle_node.applet_handle;
    applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == applet_ptr)
    {
        return;
    }
    applet_ptr->instance.cmd = opc_type;
}
/*****************************************************************************/
//  Description : ADD  CONTACT
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContact(MMIPB_CONTACT_T *contact_ptr)
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;

    
    if((!MMIPB_IsPbReady()))
    {
     //SCI_TRACE_LOW:"[mmipb] MMIPB_AddContact not ready !"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_1810_112_2_18_2_43_8_44,(uint8*)"");
     return MMIPB_ERROR_NO_INITIAL;
    }

    if(!MMIPB_IsContactValid(contact_ptr))
    {
        return MMIPB_ERROR_INVALID_FORMAT;
    }
    
    if(contact_ptr->contact_id == 0)
    {
        contact_ptr->contact_id = MMIPB_GetFreeContactId(contact_ptr->storage_id);
    }
    
    if(MMIPB_ERROR_SUCCESS !=MMIPB_GetStorageFreeSpace(contact_ptr->storage_id, PNULL))
    {
        return MMIPB_ERROR_NO_SPACE;
    }

    ret = MMIPB_UpdateContact(contact_ptr); 

    return ret;

}


/*****************************************************************************/
//  Description : ���մ����storage������¼���뵽�洢���С�
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: ����Ϊ�ӿڣ��ṩ������ģ�����
/*****************************************************************************/

LOCAL MMIPB_ERROR_E MMIPB_UpdateContact( MMIPB_CONTACT_T *contact_ptr)   //RETURN: TRUE, ���³ɹ���FALSE, ����ʧ��
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    MMIPB_ACCESS_POINT_T access_point = {0};
    
    if(PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_UpdatePhonebookEntry (PNULL == contact_ptr) !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_1861_112_2_18_2_43_8_45,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;
    }



    access_point.contact.contact_id = contact_ptr->contact_id;
    access_point.contact.storage_id = contact_ptr->storage_id;
    result  = MMIPB_Write(MMIPB_ACCESS_CONTACT,access_point,sizeof(MMIPB_CONTACT_T),contact_ptr);
    if(MMIPB_ERROR_SUCCESS == result)
    {
        MMK_PBTriggerEvent(MMIEVENT_PB_UPDATE);       
    }        
    //SCI_TRACE_LOW:"[MMIPB][PB] MMIPB_UpdatePhonebookEntry result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_1874_112_2_18_2_43_8_46,(uint8*)"d", result);
    return (result);
}


/*****************************************************************************/
//  Description : delete  CONTACT
//  Global resource dependence : none
//  Author: 
//  Note: ����Ϊ�ӿڣ��ṩ������ģ�����
/*****************************************************************************/

PUBLIC MMIPB_ERROR_E MMIPB_DeleteContact(uint16 entry_id, uint16 storage_id)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    MMIPB_ACCESS_POINT_T access_point = {0};

	 if((!MMIPB_IsPbReady()))
	 {
		 //SCI_TRACE_LOW:"[mmipb] MMIAPIPB_DeleteContact not ready !"
		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_1893_112_2_18_2_43_8_47,(uint8*)"");
		 return MMIPB_ERROR_NO_INITIAL;
	 }
    //SCI_TRACE_LOW:"[MMIPB] MMIAPIPB_DeleteContact, storage=%d, entry_id=%d,"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_1896_112_2_18_2_43_8_48,(uint8*)"dd", storage_id, entry_id);
    access_point.contact.contact_id = entry_id;
    access_point.contact.storage_id = storage_id;
    result = MMIPB_Delete(MMIPB_ACCESS_CONTACT, access_point);
    MMK_PBTriggerEvent(MMIEVENT_PB_DELETE);
    return (result);
}
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : ɾ��ȫ��������ϵ��
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/

PUBLIC MMIPB_ERROR_E MMIPB_DeleteAllMostUsedContact(void)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    
    result = MMIPB_CleanAllMostUsedContact();
    //SCI_TRACE_LOW:"[MMIPB] MMIPB_DeleteAllMostUsedContact result %"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_1914_112_2_18_2_43_8_49,(uint8*)"", result);
    return (result);
}
#endif
/*****************************************************************************/
//  Description : MODIFY  CONTACT
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ModifyContact(MMIPB_CONTACT_T *contact_ptr)
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;

    if((!MMIPB_IsPbReady()))
    {
     //SCI_TRACE_LOW:"[mmipb] MMIPB_AddContact not ready !"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_1928_112_2_18_2_43_8_50,(uint8*)"");
     return MMIPB_ERROR_NO_INITIAL;
    }
    
    if(!MMIPB_IsContactValid(contact_ptr))
    {
        return MMIPB_ERROR_INVALID_FORMAT;
    }
    

    if(
        (MMIPB_IsExistValidNumber(contact_ptr->number,MMIPB_MAX_NV_PHONE_NUM))
        || (contact_ptr->name.wstr_len > 0)
        )
    {
        
       ret = MMIPB_UpdateContact(contact_ptr); 
    }

    return ret;
}



/*****************************************************************************/
//  Description : �첽�޸Ļ���������ϵ��
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/

LOCAL MMIPB_ERROR_E MMIPB_UpdateContactAsyn(
                                            MMIPB_HANDLE_T handle, 
                                            MMIPB_CONTACT_T *contact_ptr, 
                                            MMIPB_ACCESS_CALLBACK_FUNC callback
                                            )   //RETURN: TRUE, ���³ɹ���FALSE, ����ʧ��
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    MMIPB_ACCESS_POINT_T   access_point = {0};
    MMIPB_CONTACT_T *      update_contact_ptr =PNULL;
    
    if(PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_UpdateContactAsyn (PNULL == contact_ptr) !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_1971_112_2_18_2_43_8_51,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;
    }
    update_contact_ptr= SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    if(PNULL == update_contact_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_UpdateContactAsyn (PNULL == update_contact_ptr) !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_1977_112_2_18_2_43_8_52,(uint8*)"");
        return MMIPB_ERROR_NO_SPACE;
    }
    SCI_MEMSET(update_contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    SCI_MEMCPY(update_contact_ptr, contact_ptr, sizeof(MMIPB_CONTACT_T));
    access_point.contact.contact_id = contact_ptr->contact_id;
    access_point.contact.storage_id = contact_ptr->storage_id;
    result  = MMIPB_WriteAsyn(handle, MMIPB_ACCESS_CONTACT,access_point,update_contact_ptr, sizeof(MMIPB_CONTACT_T),callback);
    if(result != MMIPB_ERROR_PENDING)
    {
        SCI_FREE(update_contact_ptr);
        update_contact_ptr = PNULL;
    }
    //SCI_TRACE_LOW:"[MMIPB][PB] MMIPB_UpdateContactAsyn result %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_1990_112_2_18_2_43_8_53,(uint8*)"d", result);
    return (result);
}


/*****************************************************************************/
//  Description : �첽������ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback)
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;

    MMIPB_SetOpcType(handle, MMIPB_OPC_ADD);
    if((!MMIPB_IsPbReady()))
    {
     //SCI_TRACE_LOW:"[mmipb] MMIPB_AddContact not ready !"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2004_112_2_18_2_43_8_54,(uint8*)"");
     return MMIPB_ERROR_NO_INITIAL;
    }
    
    if(!MMIPB_IsContactValid(contact_ptr))
    {
        return MMIPB_ERROR_INVALID_FORMAT;
    }
 
    if(MMIPB_ERROR_SUCCESS !=MMIPB_GetStorageFreeSpace(contact_ptr->storage_id, PNULL))
    {
        return MMIPB_ERROR_NO_SPACE;
    }
    if(contact_ptr->contact_id == 0)
    {
        contact_ptr->contact_id = MMIPB_GetFreeContactId(contact_ptr->storage_id);
    }

    ret = MMIPB_UpdateContactAsyn(handle, contact_ptr, callback); 
    if(ret != MMIPB_ERROR_PENDING)
    {
        //SCI_TRACE_LOW:"[mmipb]MMIPB_AddContactAsyn, ret %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2034_112_2_18_2_43_8_55,(uint8*)"d", ret);
        MMIPB_DeleteAllSelNode(handle);
    }
    return ret;    
}

#ifdef  SNS_SUPPORT
LOCAL uint32 LoadSnsContact(void)
{    
    uint32 i = 0;
    MMIPB_CONTACT_T *contact_info = PNULL; 

    if(!MMIPB_IsSnsAccountLogIn())
    {
        return i;
    }

    contact_info = SCI_ALLOC_APPZ(sizeof(MMIPB_CONTACT_T));
    while(MMIPB_ERROR_SUCCESS == MMIPB_GetSnsContactInfo(0, i, contact_info) 
        && i < MMIPB_SNS_MAX_RECORD_NUM
        && MMIPB_STATUS_SORTING == MMIPB_GetPBStatus())
    {
        i++;
        MMIPB_InsertContactToList(contact_info);
        SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
    }
    SCI_Free(contact_info);
    
    return i;
}
/*****************************************************************************/
//  Description : ����sns��ϵ��
//  Global resource dependence : 
//  Author:starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UpdateSns(void)
{   
    uint32 total_count = 0;
    MMIPB_DeleteAllSNSContactListNode();

    MMIPB_CloseSNSDB();/*lint !e718 !e18*/
    MMIPB_UpdateSnsContact();       
    MMIPB_OpenSNSDB(0);/*lint !e718 !e18*/

    total_count = LoadSnsContact();
    SCI_TRACE_LOW("[mmipb] MMIPB_UpdateSns tatal count = %d",total_count);
}

/*****************************************************************************/
//  Description : ����sns��ϵ��
//  Global resource dependence : 
//  Author:starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_LoadSnsContact(void)
{   
    uint32 total_count = 0;
    MMIPB_DeleteAllSNSContactListNode();


    total_count = LoadSnsContact();
    SCI_TRACE_LOW("[mmipb] MMIPB_LoadSnsContact tatal count = %d",total_count);
}

/*****************************************************************************/
//  Description : ����sns��ϵ��
//  Global resource dependence : 
//  Author:starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UpdateSnsByGroupId(uint8 group_id)
{   
    uint32 total_count = 0;
    MMIPB_DeleteAllSNSContactListNode();

    MMIPB_CloseSNSDB();/*lint !e718 !e18*/
    MMIPB_UpdateSnsContactByGroupId(group_id);       
    MMIPB_OpenSNSDB(0);/*lint !e718 !e18*/

    total_count = LoadSnsContact();
    SCI_TRACE_LOW("[mmipb] MMIPB_UpdateSns tatal count = %d",total_count);
}
/*****************************************************************************/
//  Description : : ��PB task������Ϣ,����sns��ϵ��
//  Global resource dependence : 
//  Author: starmer.sun
//  Note
/*****************************************************************************/
PUBLIC void MMIPB_StartUpdateSnsContact(MMI_HANDLE_T applet_handle)
{      
    MMIPB_APPLET_T* applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL != applet_ptr)
    {
        applet_ptr->instance.cmd = MMIPB_OPC_GET_SNS;
    }
    MMIPB_SetPBStatus(MMIPB_STATUS_SORTING);
    MMIPB_SendSignalToPBTask(MMIPB_SIG_SNS_UPDATE, 0);
}        
#endif


/*****************************************************************************/
//  Description : ��pb applet���õ��첽������ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactAsynForPB(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr)
{
    MMIPB_AbortPB(FALSE);
    return MMIPB_AddContactAsyn(handle, contact_ptr, mmipb_app_callback);

}

/*****************************************************************************/
//  Description : send mms by bcd number
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC void MMIPB_SendMMSByBcdNumber(MMIPB_BCD_NUMBER_T bcd_number)
{
    uint16 num_len = 0;
    uint8 tele_num[MMIPB_NUMBER_MAX_STRING_LEN+2] = {0};
    wchar num_buf[MMIPB_NUMBER_MAX_STRING_LEN+2] = {0};
    MMI_STRING_T send_to = {0};
            
            
    SCI_MEMSET(tele_num,0,sizeof(tele_num));
            
    num_len = MMIPB_GetPhoneNumberByBcdNumber(
                bcd_number, tele_num, MMIPB_NUMBER_MAX_STRING_LEN+2);

    if(num_len>0)
    {
        send_to.wstr_len = num_len; 
        send_to.wstr_ptr = num_buf;
        MMIAPICOM_StrToWstr(tele_num,num_buf);
    }
    MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_PB_SEND);
    MMIAPIMMS_AnswerMMS(MMI_DUAL_SYS_MAX,PNULL,&send_to);  
}
/*****************************************************************************/
//  Description : send sms
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SendSMSByNumber(
					   MMIPB_BCD_NUMBER_T number_t
					  )
{
	MN_NUMBER_TYPE_E number_type = 0;
	MMIPB_STRING_T number_str = {0};
	
	number_type = MMIPB_GetNumberTypeFromUint8(number_t.npi_ton);
	number_str.strlen = MMIAPICOM_GenDispNumber(
		(number_type == MN_NUM_TYPE_ALPHANUMERIC ? MN_NUM_TYPE_UNKNOW : number_type),//MMIPB_GetNumberTypeFromUint8(bcd_number_t.npi_ton),
		number_t.number_len,
		number_t.number,
		number_str.strbuf,
		MMIPB_MAX_STRING_LEN+2
		);

    MMIAPISMS_ClearDestNum();
    MMIAPISMS_SetAddressToMessageEx(number_str.strbuf, number_str.strlen);
	MMIAPISMS_WriteNewMessage(MMI_DUAL_SYS_MAX, PNULL, number_str.strbuf, number_str.strlen);

	return TRUE;
}
/*****************************************************************************/
//  Description : �첽 �༭��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ModifyContactAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback)
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;

    MMIPB_SetOpcType(handle, MMIPB_OPC_EDIT);
    if((!MMIPB_IsPbReady()))
    {
     //SCI_TRACE_LOW:"[mmipb] MMIPB_AddContact not ready !"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2114_112_2_18_2_43_8_56,(uint8*)"");
     return MMIPB_ERROR_NO_INITIAL;
    }
     if(PNULL == contact_ptr)
     {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_ModifyContactAsyn PNULL == contact_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2119_112_2_18_2_43_8_57,(uint8*)"");
        return ret;
     }    
    if(!MMIPB_IsContactValid(contact_ptr))
    {
        return MMIPB_ERROR_INVALID_FORMAT;
    }
    if(
        (MMIPB_IsExistValidNumber(contact_ptr->number,MMIPB_MAX_NV_PHONE_NUM))
        || (contact_ptr->name.wstr_len > 0)
        )
    {
        ret = MMIPB_UpdateContactAsyn(handle, contact_ptr, callback); 
        if(ret != MMIPB_ERROR_PENDING)
        {
           //SCI_TRACE_LOW:"[mmipb]MMIPB_ModifyContactAsyn, ret %d"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2147_112_2_18_2_43_9_58,(uint8*)"d", ret);
           MMIPB_DeleteAllSelNode(handle);
        } 
    }

    return ret;    
}

/*****************************************************************************/
//  Description : �첽 �༭��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ModifyContactAsynForPB(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr)
{
    MMIPB_AbortPB(FALSE);
    return MMIPB_ModifyContactAsyn(handle, contact_ptr, mmipb_app_callback);
}

/*****************************************************************************/
//  Description : �첽ɾ����������ѡ���б��еļ�¼���ڷǶ�ѡ״̬
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ModifyUnListContact(MMIPB_HANDLE_T handle,MMIPB_CONTACT_T* contact_ptr)
{
    MMIPB_SEL_DATA sel_data = {0};
    sel_data.entry_id = contact_ptr->contact_id;
    sel_data.storage_id = contact_ptr->storage_id;
   MMIPB_CreateSelNode(handle,&sel_data);
   return MMIPB_ModifyContactAsynForPB(handle, contact_ptr);
}
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : ������ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ExportVcard(uint16 entry_id, uint16 storage_id, const wchar *path_name_ptr)
{

    MMIPB_ERROR_E             ret = MMIPB_ERROR_ERROR;

    MMIFILE_ERROR_E           file_ret = 0;
    MMIFILE_HANDLE            file_handle = 0;
    MMIPB_ACCESS_POINT_T      access_point = {0};
    MMIPB_CONTACT_T *         mmipb_contact_ptr = PNULL;
    wchar                     *vcard_name_ptr =PNULL;
    wchar                     *full_path_name_ptr = PNULL;

    mmipb_contact_ptr= SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    if(PNULL == mmipb_contact_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_ExportVcard (PNULL == mmipb_contact_ptr) !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2193_112_2_18_2_43_9_59,(uint8*)"");
        return MMIPB_ERROR_NO_SPACE;
    }
    SCI_MEMSET(mmipb_contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    ret = MMIPB_GetContactInfo(entry_id, storage_id, mmipb_contact_ptr);
    if(MMIPB_ERROR_SUCCESS != ret)
    {
        //SCI_TRACE_LOW:"[mmipb]  MMIPB_ExportVcard  error ret %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2200_112_2_18_2_43_9_60,(uint8*)"d", ret);
        SCI_FREE(mmipb_contact_ptr);
        return ret;
    }
    vcard_name_ptr = SCI_ALLOCA(sizeof(wchar)*VCARD_NAME_MAX_LEN);
    if(PNULL != vcard_name_ptr)
    {
        SCI_MEMSET(vcard_name_ptr, 0x00, sizeof(wchar)*VCARD_NAME_MAX_LEN);
        MMIAPIVCARD_GetVcardFileName(mmipb_contact_ptr, MMIPB_GET_CONTACT_TYPE(storage_id), vcard_name_ptr);
    }
    full_path_name_ptr = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN +1));
    if(PNULL != full_path_name_ptr)
    {
        SCI_MEMSET(full_path_name_ptr, 0x00, sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN +1));
        MMIAPICOM_Wstrcpy(full_path_name_ptr,path_name_ptr); 
        MMIAPICOM_Wstrcat(full_path_name_ptr, L"\\");
        if(PNULL != vcard_name_ptr)
        {
            MMIAPICOM_Wstrcat(full_path_name_ptr, vcard_name_ptr);
        }
        MMIAPICOM_GetNoExistFileName(full_path_name_ptr, MMIAPICOM_Wstrlen(full_path_name_ptr));        
    }
    file_ret = MMIAPIFMM_CreateFileAsyn(full_path_name_ptr,                                     
        SFS_MODE_WRITE|SFS_MODE_CREATE_NEW|SFS_MODE_APPEND,                                                 
        PNULL,                                                                              
        PNULL,                                                                              
        0,                                                                                  
        0,                                                                                  
        &file_handle);  
    
    if(file_ret == SFS_ERROR_NONE)
    {
        ret = MMIPB_Write(MMIPB_ACCESS_VCARD, access_point, sizeof(MMIPB_CONTACT_T), mmipb_contact_ptr);        
    }
    else
    {
        ret = MMIPB_GetErrorType(file_ret);
    }
    if(PNULL != vcard_name_ptr)
    {
        SCI_FREE(vcard_name_ptr);
    }
    if(PNULL != full_path_name_ptr)
    {
        SCI_FREE(full_path_name_ptr);
    }
    SCI_FREE(mmipb_contact_ptr);
    MMIFILE_CloseFile(file_handle);
 
    return ret; 
	
}

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
                                        )
{

    MMIPB_ERROR_E             ret = MMIPB_ERROR_ERROR;

    wchar   full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = 0;
    
    MMIPB_SetOpcType(handle, MMIPB_OPC_EXPORT);       
    if((!MMIPB_IsPbReady()))
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_AddContact not ready !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2261_112_2_18_2_43_9_61,(uint8*)"");
        return MMIPB_ERROR_NO_INITIAL;
    }

    MMIAPIVCARD_GetVcardDefaultFileName(path_name_ptr, full_path_name, MMIFILE_FULL_PATH_MAX_LEN);
    MMIPB_SetCurVcardFileFullPath(s_vcard_full_path_name, full_path_name, MMIFILE_FULL_PATH_MAX_LEN);
    file_ret = MMIAPIFMM_CreateFileAsyn(full_path_name,                                     
        SFS_MODE_WRITE|SFS_MODE_CREATE_NEW|SFS_MODE_APPEND,                                                 
        PNULL,                                                                              
        PNULL,                                                                              
        0,                                                                                  
        0,                                                                                  
        &file_handle);  
    
    if(file_ret == SFS_ERROR_NONE)
    {
        ret = MMIPB_ExportNextVcardAsyn(handle, entry_id, storage_id, file_handle, callback);
    }
    else
    {
        ret = MMIPB_GetErrorType(file_ret);
    }        

    if(ret != MMIPB_ERROR_PENDING)
    {        
        MMIFILE_CloseFile(file_handle);
        MMIPB_DeleteAllSelNode(handle);
    }
    SCI_TRACE_LOW("[MMIPB]MMIPB_ExportFirstVcardAsyn ret %d file_ret 0x%x file_handle 0x%x", ret, file_ret, file_handle);
    return ret; 
	
}

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
                                        )
{
    MMIPB_CONTACT_T *contact_ptr= SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    MMIPB_ERROR_E  ret = MMIPB_ERROR_ERROR;
    MMIPB_ACCESS_POINT_T access_point = {0};

    if(PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_UpdateContactAsyn (PNULL == mmipb_contact_ptr) !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2267_112_2_18_2_43_9_62,(uint8*)"");
        return MMIPB_ERROR_NO_SPACE;
    }
    SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    ret = MMIPB_GetContactInfo(entry_id, storage_id, contact_ptr);
    if(MMIPB_ERROR_SUCCESS == ret)
    {
        access_point.file_handle = file_handle;
        ret = MMIPB_WriteAsyn(handle, MMIPB_ACCESS_VCARD, access_point, contact_ptr, sizeof(MMIPB_CONTACT_T), callback);          
    }
    else
    {
        SCI_FREE(contact_ptr);
        MMIFILE_CloseFile(file_handle);
    }
    SCI_TRACE_LOW("[MMIPB]MMIPB_ExportNextVcardAsyn ret %d file_handle 0x%x", ret, file_handle);
    return ret;

}

/*****************************************************************************/
// Description :��������
// Global resource dependence :
// Author
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ExportVcardEnd(
                                        MMIPB_HANDLE_T handle 
                                        )
{
    MMIPB_HANDLE_NODE_T handle_node = {0};

    if(MMIPB_GetHandleContent(handle, &handle_node))
    {
        MMIFILE_CloseFile(handle_node.access_point.file_handle);
    }
    SCI_TRACE_LOW("[MMIPB]MMIPB_ExportVcardEnd");
    return MMIPB_ERROR_SUCCESS;
}

/*****************************************************************************/
//  Description : �첽������������ѡ���б��еļ�¼���ڷǶ�ѡ״̬
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ExportUnListContact(MMIPB_HANDLE_T handle,uint16 entry_id, uint16 storage_id, wchar* path)
{
    MMIPB_SEL_DATA sel_data = {0};
    if((!MMIPB_IsPbReady()))
    {
     //SCI_TRACE_LOW:"[mmipb] MMIPB_AddContact not ready !"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2319_112_2_18_2_43_9_65,(uint8*)"");
     return MMIPB_ERROR_NO_INITIAL;
    }
    sel_data.entry_id = entry_id;
    sel_data.storage_id = storage_id;
    if(PNULL != path)
    {
    
        MMIAPICOM_Wstrcpy(sel_data.file_path, path);
    }
    MMIPB_CreateSelNode(handle,&sel_data);
    return MMIPB_ExportContactAsynForPB(handle, path);
}

/*****************************************************************************/
// Description : �첽������ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ExportContactAsynForPB(MMIPB_HANDLE_T handle, wchar* path)
{
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);
   
    if((!MMIAPIPB_IsPbReady()))
    {
        //SCI_TRACE_LOW:"[mmipb] MMIAPIPB_ExportContactAsyn not ready !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2344_112_2_18_2_43_9_66,(uint8*)"");
        return MMIPB_ERROR_NO_INITIAL;
    }    

    if(PNULL != applet_ptr 
      && PNULL !=applet_ptr->instance.select_list 
      &&PNULL != applet_ptr->instance.select_list->sel_table_head)
    {
        MMIPB_AbortPB(FALSE);
        MMIAPICOM_Wstrncpy(applet_ptr->instance.select_list->file_path, path, MMIAPICOM_Wstrlen(path));
        return MMIPB_ExportFirstVcardAsyn(
            handle,
            applet_ptr->instance.select_list->sel_table_head->entry_id, 
            applet_ptr->instance.select_list->sel_table_head->storage_id,
            path,
            mmipb_app_callback);
    }
    else
    {
        //SCI_TRACE_LOW:"[mmipb]MMIPB_ExportContactAsyn there is not contact to delete"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2368_112_2_18_2_43_9_68,(uint8*)"");
        return MMIPB_ERROR_NO_ENTRY;
    }    
}

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
                                    )
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    MMIPB_ACCESS_POINT_T      access_point = {0};
    uint16        entry_id   = 0;
    uint16 to_storage_id = (MMIPB_NORMAL_PHONE_CONTACT<<16)+MMIPB_STORAGE_PHONE;
    MMIFILE_HANDLE file_handle = PNULL;
    MMIFILE_ERROR_E file_ret = 0;
    if((!MMIPB_IsPbReady()))
    {
     //SCI_TRACE_LOW:"[mmipb] MMIPB_AddContact not ready !"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2392_112_2_18_2_43_9_69,(uint8*)"");
     return MMIPB_ERROR_NO_INITIAL;
    }    

    file_ret = MMIAPIFMM_CreateFileAsyn(path_name_ptr, 
        SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,
        PNULL,
        PNULL,
        0,
        0,
        &file_handle); 
    
    if(SFS_ERROR_NONE == file_ret)
    {
        access_point.file_handle = file_handle;/*lint !e605*/
        if(MMIPB_ERROR_SUCCESS !=MMIPB_GetStorageFreeSpace(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT), PNULL)
             &&MMIPB_ERROR_SUCCESS == MMIPB_Read(MMIPB_ACCESS_VCARD, access_point, sizeof(MMIPB_CONTACT_T), contact_ptr)
             )

        {
            entry_id = MMIPB_GetFreeContactId(to_storage_id);
            access_point.contact.contact_id = entry_id;
            access_point.contact.storage_id = to_storage_id;
            contact_ptr->contact_id = entry_id;
            contact_ptr->storage_id = to_storage_id;
            ret = MMIPB_Write(MMIPB_ACCESS_CONTACT, access_point, sizeof(MMIPB_CONTACT_T), contact_ptr);                      
        }
        MMIFILE_CloseFile(file_handle);
        MMK_PBTriggerEvent(MMIEVENT_PB_UPDATE);
    }    
    else
    {
       ret = MMIPB_ERROR_FILE_NOT_EXIST;
    }
    return ret;
}



/*****************************************************************************/
// Description : ������ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SaveVcard(
                                    uint16      to_storage_id,      
                                    MMIPB_CONTACT_T* contact_ptr//IN
                                    )
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    MMIPB_ACCESS_POINT_T      access_point = {0};
    uint16        entry_id   = 0;

    ret = MMIPB_GetStorageFreeSpace(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT), PNULL);
    if(MMIPB_ERROR_SUCCESS == ret)
    {
        entry_id = MMIPB_GetFreeContactId(to_storage_id);
        access_point.contact.contact_id = entry_id;
        access_point.contact.storage_id = to_storage_id;
        contact_ptr->contact_id = entry_id;
        contact_ptr->storage_id = to_storage_id;
        ret = MMIPB_Write(MMIPB_ACCESS_CONTACT, access_point, sizeof(MMIPB_CONTACT_T), contact_ptr);
        MMK_PBTriggerEvent(MMIEVENT_PB_UPDATE);
    }
    return ret;
}
/*****************************************************************************/
// Description : ����һ����ϵ��
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
LOCAL MMIPB_ERROR_E SaveContact(                                    
                                MMIPB_CONTACT_T   *contact_ptr//IN                                
                                )
{
    MMIPB_ERROR_E       ret = MMIPB_ERROR_ERROR;

    if(contact_ptr != PNULL)
    {
        contact_ptr->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
        ret = MMIPB_AddContact(contact_ptr);
    }
    return ret;
}
/*****************************************************************************/
// Description : �첽����һ����ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
LOCAL MMIPB_ERROR_E ImportOneVcardAsyn(MMIPB_HANDLE_T    handle, //IN
                                           MMIFILE_HANDLE    file_handle,//IN
                                           MMIPB_ACCESS_CALLBACK_FUNC callback
                                           )
{
    MMIPB_ACCESS_POINT_T access_point = {0};
    MMIPB_ERROR_E  ret = MMIPB_ERROR_ERROR;
    MMIPB_CONTACT_T *contact_ptr = PNULL;

    if(s_mmipb_is_abort)
    {
       return MMIPB_ERROR_SUCCESS;
    }
    else 
    {
        contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
        if(contact_ptr != PNULL)
        {
            SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
            access_point.file_handle = file_handle;
            ret = MMIPB_ReadAsyn(handle, MMIPB_ACCESS_VCARD, access_point, contact_ptr, sizeof(MMIPB_CONTACT_T),  callback);              
            if(ret != MMIPB_ERROR_PENDING)
            {
                SCI_FREE(contact_ptr);
                MMIFILE_CloseFile(file_handle);
            }
        }
    }
    SCI_TRACE_LOW("[MMIPB]ImportOneVcardAsyn  file_handle 0x%x ret %d", file_handle, ret);
    return ret;
}

/*****************************************************************************/
// Description : �첽������ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ImportVcardAsyn(MMIPB_HANDLE_T handle, //IN
                                              const wchar *path_name_ptr,//IN                                              
                                              MMIPB_ACCESS_CALLBACK_FUNC callback
                                              )
{
    MMIFILE_HANDLE  file_handle = 0;

    MMIPB_SetOpcType(handle, MMIPB_OPC_IMPORT);
    if((!MMIPB_IsPbReady()))
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_AddContact not ready !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2451_112_2_18_2_43_9_71,(uint8*)"");
        return MMIPB_ERROR_NO_INITIAL;
    }

   MMIAPIFMM_CreateFileAsyn(path_name_ptr, 
        SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,
        PNULL,
        PNULL,
        0,
        0,
        &file_handle);          
    return ImportOneVcardAsyn(handle, file_handle, callback);
}


/*****************************************************************************/
// Description : �첽������ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ImportContactAsynForPB(MMIPB_HANDLE_T handle)
{
    wchar            file_full_path[MMIFMM_FILE_FULL_PATH_MAX_LEN + 1] = {0}; //�ļ����ʵ�ȫ·�� 
    MMIPB_ERROR_E    ret = MMIPB_ERROR_ERROR;

    if(MMIPB_GetNextSelectFileName(handle,MMIFMM_FILE_FULL_PATH_MAX_LEN, file_full_path))
    {
        ret = MMIPB_ImportVcardAsyn(handle,                                         
                                    file_full_path,
                                    mmipb_app_callback);

    }
    else
    {
        //SCI_TRACE_LOW:"[mmipb]MMIPB_ExportContactAsyn there is not contact to delete"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2518_112_2_18_2_43_9_74,(uint8*)"");
        ret = MMIPB_ERROR_NO_ENTRY;
    }    
    return ret;
}
/*****************************************************************************/
//  Description : Set Current Vcard File Full Path
//  Global resource dependence : 
//  Author:yanyan.an
//  Note:
/*****************************************************************************/
LOCAL void MMIPB_SetCurVcardFileFullPath(
                                        wchar *s_vcard_full_path_name, 
                                        const wchar *full_path_name, 
                                        uint32 full_path_max_len)
{
    if(PNULL == s_vcard_full_path_name || PNULL == full_path_name)
    {
        SCI_TRACE_LOW("MMIPB_SetCurVcardFileFullPath param error");
        return;
    }

    MMIAPICOM_Wstrncpy(
        s_vcard_full_path_name, 
        full_path_name, 
        MIN(MMIAPICOM_Wstrlen(full_path_name), full_path_max_len)
        );
}
/*****************************************************************************/
//  Description : Get Current Vcard File Full Path
//  Global resource dependence : 
//  Author:yanyan.an
//  Note:
/*****************************************************************************/
LOCAL wchar* MMIPB_GetCurVcardFileFullPath(void)
{
    return s_vcard_full_path_name;
}
/*****************************************************************************/
//  Description : Delete Current Vcard File
//  Global resource dependence : 
//  Author:yanyan.an
//  Note:
/*****************************************************************************/
LOCAL void MMIPB_DeleteCurVcardFile(void)
{
    MMIAPIFMM_DeleteFile(MMIPB_GetCurVcardFileFullPath(), PNULL);    
    SCI_MEMSET (s_vcard_full_path_name , 0, ((MMIFILE_FULL_PATH_MAX_LEN + 1) * sizeof(wchar)));
}
#endif
/*****************************************************************************/
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
                                                )
{

    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    MMIPB_ACCESS_POINT_T access_point = {0};

    MMIPB_SetOpcType(handle, MMIPB_OPC_DELETE);
	 if((!MMIPB_IsPbReady()))
	 {
		 //SCI_TRACE_LOW:"[mmipb] MMIPB_DeleteContact not ready !"
		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2543_112_2_18_2_43_9_75,(uint8*)"");
		 return MMIPB_ERROR_NO_INITIAL;
	 }

    access_point.contact.contact_id = entry_id;
    access_point.contact.storage_id = storage_id;
    
    //SCI_TRACE_LOW:"[MMIPB] MMIPB_DeleteContactNode, entry_id=%d, storage_id %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2550_112_2_18_2_43_9_76,(uint8*)"dd", entry_id, storage_id);
    result = MMIPB_DeleteAsyn(handle,MMIPB_ACCESS_CONTACT,access_point, callback);        
    if(result != MMIPB_ERROR_PENDING)
    {
       //SCI_TRACE_LOW:"[mmipb]MMIPB_DeleteContactAsyn, result %d"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2555_112_2_18_2_43_9_77,(uint8*)"d", result);
       MMIPB_DeleteAllSelNode(handle);
    }
    return (result);
        
}


/*****************************************************************************/
//  Description : �첽ɾ����������ѡ���б��еļ�¼���ڷǶ�ѡ״̬
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteUnListContact(MMIPB_HANDLE_T handle,uint16 entry_id, uint16 storage_id)
{
    MMIPB_SEL_DATA sel_data = {0};
    sel_data.entry_id = entry_id;
    sel_data.storage_id = storage_id;

    MMIPB_CreateSelNode(handle,&sel_data);
    return MMIPB_DeleteContactAsynForPB(handle);
}

/*****************************************************************************/
//  Description : ��pb applet���õ��첽ɾ����ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteContactAsynForPB(MMIPB_HANDLE_T handle)
{
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);

    if(applet_ptr != PNULL
       && applet_ptr->instance.select_list != PNULL 
       && PNULL != applet_ptr->instance.select_list->sel_table_head)
    {
    
        return MMIPB_DeleteContactAsyn(
                                         handle,
                                         applet_ptr->instance.select_list->sel_table_head->entry_id, 
                                         applet_ptr->instance.select_list->sel_table_head->storage_id,
                                         mmipb_app_callback);
    }
    else
    {
        //SCI_TRACE_LOW:"[mmipb]MMIPB_DeleteContactAsynForPB there is not contact to delete"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2608_112_2_18_2_43_9_80,(uint8*)"");
        return MMIPB_ERROR_NO_ENTRY;
    }
}





/*****************************************************************************/
//  Description : �����ϵ����Ϣ
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetContactInfo(uint16 entry_id, uint16 storage_id, MMIPB_CONTACT_T* contact_ptr)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    MMIPB_ACCESS_POINT_T access_point = {0};
#ifdef SNS_SUPPORT
    MMIPB_CONTACT_BASE_INFO_T base_info = {0};
    uint8 group_id = 0;
#endif
    
    if(PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_GetContactInfo not ready !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2631_112_2_18_2_43_10_81,(uint8*)"");
        return MMIPB_ERROR_INVALID_PARAM;
    }
#ifdef SNS_SUPPORT
    if(MMIPB_IsSNSContact(storage_id))
    {
        base_info = MMIPB_GetContactBaseInfo(MMIPB_GetContactListIndex(entry_id, storage_id));
        MMIPB_GetAllGroupIDs(base_info.group, &group_id, 1);
        MMIPB_GetSnsTypeByGroupId(group_id, &access_point.sns_contact.sns_type);
        result = MMIPB_GetSnsContactInfo(access_point.sns_contact.sns_type, entry_id, contact_ptr);
    }
    else
#endif
    {
        access_point.contact.contact_id = entry_id;
        access_point.contact.storage_id = storage_id;  
        result = MMIPB_Read(MMIPB_ACCESS_CONTACT,access_point,sizeof(MMIPB_CONTACT_T),contact_ptr); //����������Ҫ����usim
    }
    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
LOCAL uint32 GetCopyGroupByStorageId(uint16 storage_id)
{
    uint32 group = 0;

    if(MMIPB_IsSIMContact(storage_id))
    {
        group = 1<<(MMIPB_GET_STORAGE(storage_id)-MMIPB_STORAGE_SIM1+PB_GROUP_SIM_1);
    }
    else
    {
        group = 0;
    }
    return group;
}

/*****************************************************************************/
//  Description : ������ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_CopyContact(uint16 to_storage_id, uint16 from_entry_id, uint16 from_storage_id)
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    MMIPB_CONTACT_T   *dest_contact_ptr = PNULL; 
    uint8            copy_field = MMIPB_FEILD_FLAG_MASK_ALL;

    if((!MMIPB_IsPbReady()))
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_GetContactInfo not ready !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2681_112_2_18_2_43_10_82,(uint8*)"");
        return MMIPB_ERROR_NO_INITIAL;
    }
     
    dest_contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(dest_contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    
    //��Դ��ϵ����Ϣ���õ�Ŀ����ϵ����Ϣ��
    while(copy_field > 0)
    {
        if(GetDestContactInfo(from_entry_id,
            from_storage_id,
            to_storage_id,
            copy_field,
            &copy_field,
            dest_contact_ptr
            )
            )
        {
            ret = MMIPB_UpdateContact(dest_contact_ptr);
            if(ret != MMIPB_ERROR_SUCCESS)
            {
                break;                    
            }
            
        }
        else
        {
            ret = MMIPB_ERROR_NO_ENTRY;
            break;
        }
    }
    
    SCI_FREE(dest_contact_ptr);
    return ret;
}

/*****************************************************************************/
//  Description : ��ȡҪ���Ƶ�Ŀ����ϵ����Ϣ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDestContactInfo(
                                  uint16 src_contact_id, //IN:
                                  uint16 src_storage_id, //IN:
                                  uint16 dest_storage_id, //IN:
                                  uint8  src_field,//IN:
                                  uint8 *left_src_field_ptr,//OUT
                                  MMIPB_CONTACT_T *dest_contact_ptr//OUT
                                  )
{
    MMIPB_CONTACT_T *src_contact_ptr = PNULL; 
    MMIPB_ACCESS_POINT_T access_point = {0};
    BOOLEAN         result = FALSE;
    uint8          no_copied_feild = src_field;
    MMIPB_FIELD_USAGE_INOF_T dest_usage_info = {0};
    uint8                  i = 0;
    uint8                  dest_num_index = 0;
    uint16                 valid_index[MMIPB_MAX_NV_PHONE_NUM] = {0};//bug355329
    uint16                 number_index[MMIPB_MAX_NV_PHONE_NUM] = {0};//bug355329

    if(dest_contact_ptr == PNULL || left_src_field_ptr == PNULL)
    {
        if(left_src_field_ptr != PNULL)
        {
            *left_src_field_ptr = 0;
        }
        return FALSE;
    }

    src_contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(src_contact_ptr == PNULL)
    {
//        if(left_src_field_ptr != PNULL)
        {
            *left_src_field_ptr = 0;
        }
        return FALSE;
    }
    //ȡ�ü�¼δ���Ƶ���
    if(no_copied_feild > 0)
    {
        //Ŀ���¼����δ���Ƶ���
        //read source contact
        SCI_MEMSET(src_contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
        access_point.contact.contact_id = src_contact_id;
        access_point.contact.storage_id = src_storage_id;
        if(MMIPB_ERROR_SUCCESS == MMIPB_Read(MMIPB_ACCESS_CONTACT, access_point, sizeof(MMIPB_CONTACT_T), src_contact_ptr))
        {
            SCI_MEMSET(dest_contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
            //set contact id and storage id
            dest_contact_ptr->contact_id = MMIPB_GetFreeContactId(dest_storage_id);
            dest_contact_ptr->storage_id = dest_storage_id;
            //copy contact name 
            *left_src_field_ptr &= (~MMIPB_FEILD_FLAG_MASK_NAME);//ȥ��������
            SCI_MEMCPY(&dest_contact_ptr->name, &src_contact_ptr->name, sizeof(dest_contact_ptr->name));
            
            MMIPB_GetFieldUageInfo(dest_storage_id, dest_contact_ptr->contact_id, &dest_usage_info);
            //���ƺ���
            for(i =0; i < MMIPB_MAX_NV_PHONE_NUM; i++)
            {
                if(no_copied_feild & (MMIPB_FEILD_MASK_SINGLE_NUMBER << i))
                {
                    //Դ��ϵ����Ϣ�и���û�б�����
                    if(MMIPB_IsValidPhoneNumber(src_contact_ptr->number[i].npi_ton, src_contact_ptr->number[i].number_len))
                    {
                        //�ú�������Ч��
                        if(dest_usage_info.number_free_flag[dest_num_index])
                        {
                            //Ŀ����ϵ����Ϣ������Ч�ĺ�����
                            *left_src_field_ptr &= (~(MMIPB_FEILD_MASK_SINGLE_NUMBER << i));/*lint !e502*///ȥ������
                            SCI_MEMCPY(&dest_contact_ptr->number[dest_num_index], &src_contact_ptr->number[i], sizeof(dest_contact_ptr->number[dest_num_index]));                            
                            dest_contact_ptr->number[dest_num_index].number_len = 
                                           MIN(dest_contact_ptr->number[dest_num_index].number_len, (MMIPB_GetNumberMaxLen(dest_storage_id,dest_num_index + 1,0))>>1);/*lint !e666*/  

#ifdef CMCC_VCARD_SUPPORT
                            if(MMIPB_IsUsim(dest_storage_id))
                            {
                                uint8 aas_id = MMIPB_GetAasIDByNumType(src_contact_ptr->num_type[i]);
                                
                                if(aas_id >= MMIPB_ReadUSIMAASCount(MMIPB_GetDualSysByStorageId(dest_storage_id)))
                                {
                                    aas_id = 2;
                                }
                                dest_contact_ptr->num_type[dest_num_index] = MMIPB_GetNumTypeByAasID(aas_id);
                            }
                            else
                            {
                                dest_contact_ptr->num_type[dest_num_index] = src_contact_ptr->num_type[i];
                            }
                            
#else                       
                            dest_contact_ptr->num_type[dest_num_index] = src_contact_ptr->num_type[i];
#endif
                            dest_num_index++;
                        }
                    }
                    else
                    {
                        SCI_TRACE_LOW("yanyan.an invalidNumber num_type[%d]:%d, number_free_flag[%d]:%d", i, src_contact_ptr->num_type[i],dest_num_index,dest_usage_info.number_free_flag[dest_num_index]);
                        *left_src_field_ptr &= (~(MMIPB_FEILD_MASK_SINGLE_NUMBER << i));/*lint !e502*///ȥ������
                        if(dest_usage_info.number_free_flag[dest_num_index])
                            {
                                dest_contact_ptr->num_type[dest_num_index] = src_contact_ptr->num_type[i];
                                dest_num_index++;
                            }
                    }
                }
            }
#ifdef MMIPB_MAIL_SUPPORT
            //����email
            *left_src_field_ptr &= (~MMIPB_FEILD_FLAG_MASK_MAIL);//ȥ��email��
            if((no_copied_feild & MMIPB_FEILD_FLAG_MASK_MAIL)
               &&dest_usage_info.mail_free_flag
               &&src_contact_ptr->mail.wstr_len > 0
              )
            {
                SCI_MEMCPY(&dest_contact_ptr->mail, &src_contact_ptr->mail, sizeof(dest_contact_ptr->mail));
                dest_contact_ptr->mail.wstr_len = MIN(dest_contact_ptr->mail.wstr_len, MMIPB_GetMaxAlphaLen(dest_storage_id, MMIPB_CONTACT_EMAIL)); /*lint !e666*/                  
            }
#endif
            //���Ʒ�����Ϣ
            if(dest_usage_info.group_free_falg)
            {
                dest_contact_ptr->union_t.group = GetCopyGroupByStorageId(dest_storage_id);
            }
#ifndef PB_SUPPORT_LOW_MEMORY
            //���Ʊ���¼
            if(dest_usage_info.memo_free_flag && src_contact_ptr->memo.wstr_len > 0)
            {
                SCI_MEMCPY(&dest_contact_ptr->memo, &src_contact_ptr->memo, sizeof(src_contact_ptr->memo));
            }

            if(dest_usage_info.birth_free_flag && src_contact_ptr->birth.wstr_len > 0)
            {
                SCI_MEMCPY(&dest_contact_ptr->birth, &src_contact_ptr->birth, sizeof(src_contact_ptr->birth));
            }
            if(dest_usage_info.url_free_flag && src_contact_ptr->url.wstr_len> 0)
            {
                SCI_MEMCPY(&dest_contact_ptr->url, &src_contact_ptr->url, sizeof(src_contact_ptr->url));
            }
#endif
#ifdef MMIPB_IM_MESSAGE_SUPPORT
           if(dest_usage_info.im_free_flag && src_contact_ptr->imessager.wstr_len > 0)
            {
                SCI_MEMCPY(&dest_contact_ptr->imessager, &src_contact_ptr->imessager, sizeof(src_contact_ptr->imessager));
            }
#endif
#ifdef SNS_SUPPORT
            if(dest_usage_info.sign_free_flag && src_contact_ptr->sign.wstr_len> 0)
            {
                SCI_MEMCPY(&dest_contact_ptr->sign, &src_contact_ptr->sign, sizeof(src_contact_ptr->sign));
            }
#endif            
#ifdef CMCC_VCARD_SUPPORT
            if(dest_usage_info.sne_free_flag && src_contact_ptr->alias.wstr_len> 0)
            {
                SCI_MEMCPY(&dest_contact_ptr->alias, &src_contact_ptr->alias, sizeof(src_contact_ptr->alias));
            }
#endif
        }
    }
	//��Ҫ����DEST�ж�Ӧ��SRC�ĵ�һ��ARN��������Ч�ģ�����SRC������Ϊ�յ����
	SCI_TRACE_LOW("GetDestContactInfo dest_name_len=%d,dest_adn_number_len=%d",dest_contact_ptr->name.wstr_len,dest_contact_ptr->number[0].number_len);
	if(0==dest_contact_ptr->name.wstr_len && 0==dest_contact_ptr->number[0].number_len)//dest��name�Լ�adn�ֶ�Ϊ��
	{
		if(0 < MMIPB_GetContactNumberNum(src_contact_ptr, number_index) && 0!=number_index[0])//SRC��ANRû��ADN
		{
			if(dest_usage_info.number_free_flag[0]//DEST��ADN��Ч
				&&*left_src_field_ptr & (MMIPB_FEILD_MASK_SINGLE_NUMBER << number_index[0]))//�����һ��ANR��û�п���
			{
				//������һ��ANR�ֶε�DEST��ADN��
                *left_src_field_ptr &= (~(MMIPB_FEILD_MASK_SINGLE_NUMBER << number_index[0]));/*lint !e502*///ȥ������
                SCI_MEMCPY(&dest_contact_ptr->number[0], &src_contact_ptr->number[number_index[0]], sizeof(dest_contact_ptr->number[0]));                            
                dest_contact_ptr->number[0].number_len = 
                               MIN(dest_contact_ptr->number[0].number_len, (MMIPB_GetNumberMaxLen(dest_storage_id,1,0))>>1);/*lint !e666*/  
                dest_contact_ptr->num_type[0] = MMIPB_ADN_NUMBER;
			}
		}
	}
    SCI_FREE(src_contact_ptr);
    if((src_field &MMIPB_FEILD_FLAG_MASK_NAME) && dest_contact_ptr->name.wstr_len > 0)//��һ�θ��ƣ�������Ϊ��
    {
        result = TRUE;
    }
	else if(0 < MMIPB_GetContactNumberNum(dest_contact_ptr, valid_index))//��adn����anr
	{
		if(0==valid_index[0])//ADN����
		{
	        result = TRUE;
		}
		else if(dest_usage_info.number_free_flag[0])//ADN�ֶ���Ч���ǲ�����
		{
			//�ѵ�һ�����븴�Ƶ�ADN�ֶ���
            SCI_MEMCPY(&dest_contact_ptr->number[0], &dest_contact_ptr->number[valid_index[0]], sizeof(dest_contact_ptr->number[0]));                            
            dest_contact_ptr->number[0].number_len = 
                           MIN(dest_contact_ptr->number[valid_index[0]].number_len, (MMIPB_GetNumberMaxLen(dest_storage_id,1,0))>>1);/*lint !e666*/  
			//ɾ��ԭ���ĺ���
            SCI_MEMSET(&dest_contact_ptr->number[valid_index[0]], 0x00, sizeof(dest_contact_ptr->number[valid_index[0]]));
	        result = TRUE;
			SCI_TRACE_LOW("GetDestContactInfo ADN dest_contact_ptr->number[0].number_len=%d",dest_contact_ptr->number[0].number_len);
		}
		else
		{
	        //ADN��Ч���Ǿ�ֻ��ANR�������������
	        *left_src_field_ptr = 0;
	        result = FALSE;
		}
	}
    else 
    {
        //�ü�¼û�пɸ��Ƶ����ݣ����������
        *left_src_field_ptr = 0;
        result = FALSE;
    }
    return result;
}

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
                                              )
{	
    MMIPB_CONTACT_T *dest_contact_ptr = PNULL;  
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);

    MMIPB_SetOpcType(handle, MMIPB_OPC_COPY);
    if(applet_ptr == PNULL || applet_ptr->instance.select_list == PNULL || applet_ptr->instance.select_list->sel_table_head == PNULL)
    {
        return MMIPB_ERROR_NO_ENTRY;
    }
    if(MMIPB_ERROR_SUCCESS !=MMIPB_GetStorageFreeSpace(dest_storage_id, PNULL))
    {
        //no space to copy, exit copy
        return MMIPB_GetNoSpaceError(dest_storage_id);
    }

    applet_ptr->instance.select_list->to_storage_id = dest_storage_id;

    dest_contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(dest_contact_ptr == PNULL)
    {
        //no memory, exit copy
        return MMIPB_ERROR_NO_SPACE;
    }
   
    SCI_MEMSET(dest_contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    //��Դ��ϵ����Ϣ���õ�Ŀ����ϵ����Ϣ��
    if(GetDestContactInfo(applet_ptr->instance.select_list->sel_table_head->entry_id,
                       applet_ptr->instance.select_list->sel_table_head->storage_id,
                       dest_storage_id,
                       applet_ptr->instance.select_list->sel_table_head->sel_field,
                       &applet_ptr->instance.select_list->sel_table_head->sel_field,
                       dest_contact_ptr
                      )
     )
    {
        //ִ�и���
        ret = MMIPB_UpdateContactAsyn(handle, dest_contact_ptr, callback);  
        if(ret != MMIPB_ERROR_PENDING)
        {
           //SCI_TRACE_LOW:"[mmipb]MMIPB_CopyContactAsyn, ret %d"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2945_112_2_18_2_43_10_85,(uint8*)"d", ret);
        } 
    }
    else
    {
        ret = MMIPB_ERROR_NO_ENTRY;
    }
    SCI_FREE(dest_contact_ptr);
    return ret;    
}



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
                                              uint8  copy_field)
{            
    MMIPB_SEL_DATA       sel_data = {0};

    sel_data.to_storage_id = to_storage_id;
    sel_data.entry_id = entry_id;
    sel_data.storage_id = storage_id;
    sel_data.sel_field = copy_field;
    MMIPB_CreateSelNode(handle,&sel_data);
    return MMIPB_CopyMultiContactAsyn(handle, to_storage_id);   
}
/*****************************************************************************/
//  Description : ��pb applet���õ��첽������ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_CopyMultiContactAsyn(MMIPB_HANDLE_T handle, uint16 to_storage_id)
{
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);
    MMIPB_ERROR_E         ret = MMIPB_ERROR_ERROR;

    if((!MMIPB_IsPbReady()))
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_GetContactInfo not ready !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_2988_112_2_18_2_43_10_86,(uint8*)"");
        return MMIPB_ERROR_NO_INITIAL;
    }

    if(applet_ptr != PNULL && applet_ptr->instance.select_list != PNULL)
    {
        while(PNULL != applet_ptr->instance.select_list->sel_table_head)
        {
            ret = MMIPB_CopyContactAsyn(
                handle, 
                to_storage_id,
                mmipb_app_callback
                );
            if(MMIPB_ERROR_NO_ENTRY == ret)
            {
                //��ǰ��¼�Ѿ�������ɣ�ȡѡ����е���һ��Ҫ���Ƶļ�¼
                applet_ptr->instance.select_list->opc_count++;
                MMIPB_DeleteSelHeadNode(handle);
            }
            else
            {
                break;
            }            
        }
        //fix bug932834 ����sel table�е�����Ϊ�գ���˵���Ѿ�copy��ɣ�û�м�¼��Ҫcopy������MMIPB_ERROR_NO_ENTRY
        if(PNULL == applet_ptr->instance.select_list->sel_table_head)
        {
            ret = MMIPB_ERROR_NO_ENTRY;
        }
    }
    if(ret == MMIPB_ERROR_SUCCESS || ret == MMIPB_ERROR_PENDING)
    {
        if(applet_ptr != PNULL 
          &&(PNULL!= applet_ptr->instance.select_list) 
          &&(0 == applet_ptr->instance.select_list->opc_count)
          )
        {
            //��һ�����ƣ����Ƽ���Ϊ1
            applet_ptr->instance.select_list->opc_count = 1;
        }
    }
    return ret;
}


/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:IN :group_id
        //OUT: group_ptr
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetGroupInfo( uint8 group_id, MMIPB_GROUP_T* group_ptr)
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    MMIPB_ACCESS_POINT_T access_point = {0};
    
    access_point.group_id = group_id;
    ret = MMIPB_Read(MMIPB_ACCESS_GROUP, access_point, sizeof(MMIPB_GROUP_T), group_ptr);
    return ret;
}

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:IN :group_id
        //OUT: group_ptr
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SetGroupInfo( uint8 group_id, MMIPB_GROUP_T* group_ptr)
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    MMIPB_ACCESS_POINT_T access_point = {0};
    
    access_point.group_id = group_id;
    ret = MMIPB_Write(MMIPB_ACCESS_GROUP, access_point, sizeof(MMIPB_GROUP_T), group_ptr);
    return ret;
}

/*****************************************************************************/
//  Description :����Զ���Ⱥ�����
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetUserGroupCount(void)
{
    uint8 group_id = 0;
    uint16 group_num =0;
    MMIPB_GROUP_T group_info={0};
    for(group_id =MMIPB_SYSTEM_GROUP_NUM; group_id<(MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM); group_id++) 
    {
        MMIPB_GetGroupInfo(group_id, &group_info);
        if(group_info.is_valid)
        {
            group_num++;
        }

    }
    return group_num;

}
#endif
/*****************************************************************************/
//  Description :��ȡȺ������
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadGroupName(MMIPB_NAME_T* name,  uint8 group_id)
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    MMIPB_GROUP_T group_info ={0};
    MMIPB_ACCESS_POINT_T access_point = {0};
    
    access_point.group_id = group_id;
    ret = MMIPB_Read(MMIPB_ACCESS_GROUP, access_point, sizeof(MMIPB_GROUP_T), &group_info);
    if(ret == MMIPB_ERROR_SUCCESS && name != PNULL)
    {
       SCI_MEMCPY(name, &(group_info.group_name), sizeof(MMIPB_NAME_T));
    }
    return ret;

}


#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description :дȺ������
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteGroupName(MMIPB_NAME_T* name,  uint8 group_id)
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    MMIPB_GROUP_T group_info ={0};
    MMIPB_ACCESS_POINT_T access_point = {0};

    if(name->wstr_len>0)
    {
    
        access_point.group_id = group_id;
        ret = MMIPB_Read(MMIPB_ACCESS_GROUP, access_point, sizeof(MMIPB_GROUP_T), &group_info);         
        SCI_MEMCPY(name, &(group_info.group_name), sizeof(MMIPB_NAME_T));
        ret = MMIPB_Write(MMIPB_ACCESS_GROUP, access_point, sizeof(MMIPB_GROUP_T), &group_info); 
    }
    else
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_WriteGroupName name->wstr_len %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3125_112_2_18_2_43_11_88,(uint8*)"d", name->wstr_len);
    }
    return ret;

}
/*****************************************************************************/
//  Description :get used phone group count
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetUsedGroupIdInfor(uint8 *id_ptr)
{
    uint8 remainder  = 0;   
    uint8 group_id = 0;
    MMIPB_GROUP_T group_info={0};
    
    if(PNULL == id_ptr)
    {
        return FALSE;
    }
    
    for(group_id = 0;group_id<PB_GROUP_MAX;group_id++)
    {
        remainder = group_id%8;
        id_ptr[group_id/8] |= (1<<(7-remainder));

    }
    
    for(group_id =MMIPB_SYSTEM_GROUP_NUM; group_id<(MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM); group_id++) 
    {
    	SCI_MEMSET(&group_info,0,sizeof(MMIPB_GROUP_T));
        MMIPB_GetGroupInfo(group_id, &group_info);
        if(group_info.is_valid)
        {
            remainder = group_id%8;
            id_ptr[group_id/8] |= (1<<(7-remainder));
        }
    }
	
    return TRUE;
}
#endif
#ifdef SNS_SUPPORT 
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:IN :group_id
        //OUT: group_ptr
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetSnsContactInfo(uint16 sns_type, uint16 index, MMIPB_CONTACT_T* contact_ptr )
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    MMIPB_ACCESS_POINT_T access_point = {0};
    
    access_point.sns_contact.index = index, 
    access_point.sns_contact.sns_type = sns_type;
    ret = MMIPB_Read(MMIPB_ACCESS_SNS, access_point, sizeof(MMIPB_CONTACT_T), contact_ptr);
            

    return ret;
}
#endif

/*****************************************************************************/
//  Description : ���applet������ѡ���б�Ľڵ���,������group
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC uint16  MMIPB_GetSelectCount(MMI_HANDLE_T handle)
{
     MMIPB_APPLET_T* applet_ptr = GetPBApplet(handle);
     
     if(PNULL == applet_ptr)
     {
         //SCI_TRACE_LOW:"[mmipb] MMIPB_GetSelectCount PNULL == applet_ptr !"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3176_112_2_18_2_43_11_91,(uint8*)"");
         return 0;
     }
     if(MMIPB_OPC_IMPORT==applet_ptr->instance.cmd)
     {                
         return applet_ptr->instance.select_files.total_file_count;   
     }
     else
     {
         
         if(applet_ptr->instance.select_list == PNULL)
         {
             return 0;
         }
         else
         {
             
             return applet_ptr->instance.select_list->sel_count;
         }
     }
}

/*****************************************************************************/
// Description : ���������Դ洢��nv contacts����Ŀ
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMaxContactNumOfNV(void)
{
    MMIPB_STORAGE_INFO_T storage_info = MMIPB_GetContactStorageInfo(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT));
    return storage_info.max_record_num;
}

/*****************************************************************************/
// Description : ���������Դ洢��sim contacts����Ŀ
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMaxContactNumOfSim(MN_DUAL_SYS_E dual_sys)
{
    MMIPB_STORAGE_INFO_T storage_info =  MMIPB_GetContactStorageInfo(MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + dual_sys, PHONEBOOK_SIM_STORAGE));

    return storage_info.max_record_num;
}

/*****************************************************************************/
// Description : ���������Դ洢��contacts����Ŀ
// Global resource dependence :
// Author:maryxiao
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMaxContactNum(void)
{
    uint16 num = MMIPB_GetContactStorageSpace(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT));
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;

     for(i = 0; i <MMI_DUAL_SYS_MAX; i++)
     {
         if(MMIAPIPHONE_GetSimExistedStatus(i))
         {
            num += MMIPB_GetMaxContactNumOfSim(i);
         }
     }
     return num;
}

/*****************************************************************************/
// Description : ���������Դ洢����Ŀ
// Global resource dependence :
// Author:maryxiao
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetStorageMaxCount(uint16 storage_id)
{
     return MMIPB_GetContactStorageSpace(storage_id);

}


/*****************************************************************************/
// Description : ȡ��ÿ��sim�������Ѵ��е���ϵ�˸���
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetSimContactCount(MN_DUAL_SYS_E dual_sys)
{
    /*MMIPB_CONTACT_LIST_INFO_T   contact_list_info = {0};
    MMIPB_ACCESS_POINT_T        access_point = {0};
    MMIPB_Read(MMIPB_ACCESS_CONTACT_LIST, access_point, sizeof(MMIPB_CONTACT_LIST_INFO_T), &contact_list_info);
    return contact_list_info.storage_count[dual_sys+MMIPB_STORAGE_SIM1];*/
    uint16 storage_id = 0;
    storage_id = (PHONEBOOK_SIM_STORAGE<<8) | ((MMIPB_STORAGE_SIM1+dual_sys) & 0X00FF);
  return MMIPB_GetContactCount(storage_id);
}


/*****************************************************************************/
// Description : ȡ��SIM�����ֻ���ÿ����ļ�¼��
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetContactNumOfGroup(uint16 group_index)
{
    /*MMIPB_CONTACT_LIST_INFO_T   contact_list_info = {0};
    MMIPB_ACCESS_POINT_T        access_point = {0};
    MMIPB_Read(MMIPB_ACCESS_CONTACT_LIST, access_point, sizeof(MMIPB_CONTACT_LIST_INFO_T), &contact_list_info);
    return contact_list_info.group_count[group_index];*/
    return MMIPB_GetGroupContactCount(group_index);
}

/*****************************************************************************/
//  Description : APP_MN_CALL_READY_IND
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_HandleCallReadyCnf(DPARAM param)
{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    APP_MN_CALL_READY_IND_T *call_ready_param_ptr = 0;
#endif
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    call_ready_param_ptr = (APP_MN_CALL_READY_IND_T *)param;
    
    if(PNULL == call_ready_param_ptr || call_ready_param_ptr->dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_HandleCallReadyCnf call_ready_param_ptr 0x%x "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3313_112_2_18_2_43_11_93,(uint8*)"d", call_ready_param_ptr);
        return;
    }
    dual_sys = call_ready_param_ptr->dual_sys;
#else
    dual_sys = MN_DUAL_SYS_1;
#endif
    
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_HandleCallReadyCnf dual_sys %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3321_112_2_18_2_43_11_94,(uint8*)"d",dual_sys);
  
    //set this sim ready
    s_mmipb_sim_ready[dual_sys] = TRUE;

	if(IsAllSimReady() && MMIPB_STATUS_SIM_NOT_READY == MMIPB_GetPBStatus())
	{
		//only when all sim ready and pb not ready, permot to sort
		MMIPB_BeginToSort();
	}
}


/*****************************************************************************/
//  Description : is all sim are ready
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAllSimReady(void)
{
    uint32 i = 0;
    BOOLEAN result = FALSE;

    for( i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(s_mmipb_sim_ready[i] == FALSE)
        {
            break;        
        }
    }
    if(i == MMI_DUAL_SYS_MAX)
    {
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//  Description : ADNUpdate��Ϣ
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void HandleADNUpdateInd(void* param)
{
    APP_MN_ADN_UPDATE_CNF_T     *adn_update_cnf_ptr = (APP_MN_ADN_UPDATE_CNF_T *)param;
    MMIPB_ACCESS_POINT_T access_point = {0};
    MMIPB_HANDLE_NODE_T  handle_node = {0};
    MMIPB_CONTACT_T *contact = PNULL;  
    MMIPB_ERROR_E        cnf_ret = MMIPB_ERROR_ERROR;
    BOOLEAN              is_cur = FALSE;    

    
    contact = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(contact, 0x00, sizeof(MMIPB_CONTACT_T));
    if(adn_update_cnf_ptr != PNULL)
    {
        access_point.contact.contact_id = adn_update_cnf_ptr->entry_id;
        access_point.contact.storage_id = MMIPB_GetStorageID(adn_update_cnf_ptr->dual_sys + MMIPB_STORAGE_SIM1, PHONEBOOK_SIM_STORAGE);
        SCI_TRACE_LOW("MMIPB HandleADNUpdateInd adn_update_cnf_ptr->is_ok %d", adn_update_cnf_ptr->is_ok);
        if(adn_update_cnf_ptr->is_ok)
        {
            if(MMIPB_ERROR_SUCCESS == MMIPB_Read(MMIPB_ACCESS_CONTACT, access_point, sizeof(MMIPB_CONTACT_T), contact))
            {
                if(MMIPB_IsContactValid(contact))
                {
                    MMIPB_RemoveContactFromList(access_point.contact.contact_id, access_point.contact.storage_id);
                    MMIPB_InsertContactToList(contact);
                    MMK_PBTriggerEvent(MMIEVENT_PB_UPDATE);
                }
                else
                {
                    SCI_TRACE_LOW("MMIPB not MMIPB_IsContactValid ");
                }
             }
            else
            {
                SCI_TRACE_LOW("MMIPB HandleADNUpdateInd MMIPB_Read error");
                MMIPB_RemoveContactFromList(access_point.contact.contact_id, access_point.contact.storage_id);
                MMK_PBTriggerEvent(MMIEVENT_PB_DELETE);
            }
            cnf_ret = MMIPB_ERROR_SUCCESS;
        }
        if(MN_ADN_UPDATE == adn_update_cnf_ptr->operate_type)
        {
            //add or edit
            MMIPB_WriteTrigger(access_point.contact.contact_id, access_point.contact.storage_id, cnf_ret);                                   
        }
        else
        {
            MMIPB_DeleteTrigger(access_point.contact.contact_id, access_point.contact.storage_id, cnf_ret);                                   
        }
        is_cur = MMIPB_IsCurrentOperation(adn_update_cnf_ptr->entry_id ,access_point.contact.storage_id);
        //if(is_cur)
        {
            //MMIPB_HandleAdnUpdateCnf(param);
            MMIPB_GetHandleContent(MMIPB_GetCurrentHandle(), &handle_node);
            if(handle_node.data_ptr != PNULL)
            {
                SCI_FREE(handle_node.data_ptr);
                handle_node.data_ptr = PNULL;
                MMIPB_SetHandleContent(MMIPB_GetCurrentHandle(), &handle_node);
            }            
            MMIPB_CmdConfirm(&handle_node, MMIPB_GetCurrentHandle(), cnf_ret);
        }
    }
    SCI_FREE(contact);

}

/*****************************************************************************/
//  Description : MsisdnUpdateCnf��Ϣ
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
LOCAL void  HandleMsisdnUpdateCnf(void* param)
{
    APP_MN_MSISDN_UPDATE_CNF_T  *msisdn_update_cnf_ptr  = (APP_MN_MSISDN_UPDATE_CNF_T *)param;
    MMIPB_ERROR_E        cnf_ret = MMIPB_ERROR_ERROR;
    MMIPB_HANDLE_NODE_T  handle_node = {0};
    if(msisdn_update_cnf_ptr->is_ok)
    {
        cnf_ret = MMIPB_ERROR_SUCCESS;
    }
    if(MN_ADN_UPDATE == msisdn_update_cnf_ptr->operate_type)
    {
        MMIPB_WriteTrigger(msisdn_update_cnf_ptr->entry_id, MMIPB_GetStorageID(msisdn_update_cnf_ptr->dual_sys + MMIPB_STORAGE_SIM1, PHONEBOOK_MSISDN_STORAGE), cnf_ret);                
    }
    else
    {
        MMIPB_DeleteTrigger(msisdn_update_cnf_ptr->entry_id, MMIPB_GetStorageID(msisdn_update_cnf_ptr->dual_sys + MMIPB_STORAGE_SIM1, PHONEBOOK_MSISDN_STORAGE), cnf_ret);                
    }
                
    {
        MMIPB_GetHandleContent(MMIPB_GetCurrentHandle(), &handle_node);
        MMIPB_CmdConfirm(&handle_node, MMIPB_GetCurrentHandle(), cnf_ret);
    }
    if(MMIPB_GetHandleContent(MMIPB_GetCurrentHandle(), &handle_node))  //modify for coverity
    {            
        if(handle_node.data_ptr != PNULL)
        {
            SCI_FREE(handle_node.data_ptr);
            handle_node.data_ptr = PNULL;
            MMIPB_SetHandleContent(MMIPB_GetCurrentHandle(), &handle_node);
        }
    }
}
/*****************************************************************************/
//  Description : ����PS����Ϣ
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePBPsMsg 
(
    PWND    app_ptr, 
    uint16  msg_id, 
    DPARAM  param
)
{

     MMI_RESULT_E result = MMI_RESULT_TRUE;
     MMIPB_CNF_MSG_T *cnf_ptr = PNULL;
 #ifdef MMI_VCARD_SUPPORT
     MMIPB_ERROR_E  ret = 0;
 #endif

    //SCI_TRACE_LOW:"[MMIPB]HandlePBPsMsg 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3456_112_2_18_2_43_11_95,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case APP_MN_CALL_READY_IND:
        //SCI_TRACE_LOW:"[MMIPB]HandlePBPsMsg MMIPB_HandleCallReadyCnf"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3461_112_2_18_2_43_11_96,(uint8*)"");
        MMIPB_HandleCallReadyCnf(param);	
#ifdef FDN_SUPPORT
        MMIAPIFDN_HandleCallReadyInd(param);
        if(MMIAPIFDN_GetReReadPhoneBookState())
        {
            MMIAPIFDN_SetReReadPhoneBookState(FALSE);
            MMIAPIPB_ResortPB();
        }
#endif

#ifdef MMI_LDN_SUPPORT
        MMIAPICL_HandleLdnReadyInd(param);
#endif
        result = MMI_RESULT_FALSE;
        break;

    case APP_MN_SIM_NOT_READY_IND:
        //SCI_TRACE_LOW:"[MMIPB]HandlePBPsMsg APP_MN_SIM_NOT_READY_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3475_112_2_18_2_43_11_97,(uint8*)"");
		MMIPB_HandlePbSimNotReadyInd(param);
		result = MMI_RESULT_FALSE;
        break;
#ifdef SEARCH_SUPPORT
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
    case MMI_PB_SEARCH_CNF:
        cnf_ptr = (MMIPB_CNF_MSG_T *)param;
        if(cnf_ptr != PNULL)
        {
            MMIPB_SEARCH_DATA_T* search_data_ptr = cnf_ptr->handle_node.data_ptr;
            if(PNULL != search_data_ptr && PNULL != search_data_ptr->callback)
            {
                search_data_ptr->callback(search_data_ptr);
            }
        }
        break;
#endif
#endif 
    case MMI_PB_SORT_CNF:
        SCI_TRACE_LOW("[MMIPB]HandlePBPsMsg MMI_PB_SORT_CNF "); 
        //PB��ʼ������
        HandleSortCnf();
        break;
    case APP_MN_MSISDN_UPDATE_CNF://usim �����������
        HandleMsisdnUpdateCnf(param);
#if defined (MMIPB_SIMDN_SUPPORT) && defined (BLUETOOTH_SUPPORT) && defined (BT_BQB_SUPPORT)
        if(MMIAPIBT_IsBtOn())//�����ǰ�������ڴ�״̬����֪ͨ���±�������
        {
            MMIAPIBT_SetMyPhoneNumber();
        }
#endif
        break;
#ifdef SNS_SUPPORT
    case MMI_PB_UPDATE_SNS_ACTIVITY_CNF:
        {
            MMIPB_HANDLE_T applet_handle = 0;
            MMIPB_APPLET_T*  applet_ptr= PNULL; 
            SCI_TRACE_LOW("[MMIPB]HandlePBPsMsg MMI_PB_UPDATE_SNS_ACTIVITY_CNF "); 
            MMIPB_GetCurAppletHandle(&applet_handle);
            applet_ptr = (MMIPB_APPLET_T* )MMK_GetAppletPtr(applet_handle);
            if((PNULL!= applet_ptr) && (PNULL!= applet_ptr->instance.update_callback))
            {
                applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_SUCCESS, MMIPB_ERROR_SUCCESS,MMIPB_OPC_UPDATE_SNS_ACTIVITY,0);
            }
        }
        break;
#endif


	case APP_MN_ADN_UPDATE_CNF:
        HandleADNUpdateInd(param);
		break;
#ifdef CMCC_VCARD_SUPPORT
	case APP_MN_READ_ADDITIONAL_ENTRY_CNF:
		//read addtional entry confirm
        MMIPB_ReadUSIMExtCnf((APP_MN_READ_ADDITIONAL_ENTRY_CNF_T *)param);      
		break;
	case APP_MN_UPDATE_USER_INFO_CNF:
        //add or copy entry to usim confirm
		MMIPB_USIMUpdateCnf((APP_MN_UPDATE_USER_INFO_CNF_T*)param);
		break;
	case APP_MN_DEL_USER_INFO_CNF:
		//delete usim entry confirm
		MMIPB_USIMDeleteCnf((APP_MN_UPDATE_USER_INFO_CNF_T *)param);
		break;
#endif
    case MMI_PB_READ_CNF:
        //����Ϣ��confirm
        cnf_ptr = (MMIPB_CNF_MSG_T *)param;
        if(cnf_ptr != PNULL)
        {  
#ifdef MMI_VCARD_SUPPORT  
            if(MMIPB_OPC_IMPORT == cnf_ptr->handle_node.cmd_type)
            {                 
                MMIPB_SetTaskStatus(FALSE);
                if(MMIPB_ERROR_SUCCESS == cnf_ptr->ret)
                {
                    ret = SaveContact((MMIPB_CONTACT_T *)cnf_ptr->handle_node.data_ptr);
                }

                if(cnf_ptr->handle_node.data_ptr != PNULL)
                {
                    SCI_FREE(cnf_ptr->handle_node.data_ptr);
                    cnf_ptr->handle_node.data_ptr = PNULL;
                    MMIPB_SetHandleContent(cnf_ptr->handle, &cnf_ptr->handle_node);
                } 
                if(MMIPB_ERROR_SUCCESS == cnf_ptr->ret)
                {
                    if(MMIPB_ERROR_NO_SPACE != ret)
                    {
                        ret = ImportOneVcardAsyn(cnf_ptr->handle, 
                            cnf_ptr->handle_node.access_point.file_handle, 
                            cnf_ptr->handle_node.callback);
                    }
                    else
                    {
                        cnf_ptr->ret = MMIPB_ERROR_NO_SPACE;
                    }
                }                   
            }
            if(ret != MMIPB_ERROR_PENDING)
            {                
                if(MMIPB_OPC_IMPORT == cnf_ptr->handle_node.cmd_type)
                {
                    MMIFILE_CloseFile(cnf_ptr->handle_node.access_point.file_handle);
                }
                MMIPB_CmdConfirm(&cnf_ptr->handle_node, cnf_ptr->handle, cnf_ptr->ret);
            }
#else
            MMIPB_CmdConfirm(&cnf_ptr->handle_node, cnf_ptr->handle, cnf_ptr->ret);  
#endif
        }
        break;
    case MMI_PB_WRITE_CNF:
        //д��Ϣ��confirm
        cnf_ptr = (MMIPB_CNF_MSG_T *)param;
        if(cnf_ptr != PNULL)
        {
            //handle_ptr = MMIPB_GetHandleContent(cnf_ptr->handle);
            if(cnf_ptr->handle_node.data_ptr != PNULL)
            {
                SCI_FREE(cnf_ptr->handle_node.data_ptr);
                cnf_ptr->handle_node.data_ptr = PNULL;
                if(!MMIPB_SetHandleContent(cnf_ptr->handle, &cnf_ptr->handle_node))
                {
                    MMIPB_DeleteAllSelNode(cnf_ptr->handle);
                }
            }
            if(MMIPB_ACCESS_CONTACT == cnf_ptr->handle_node.access_type)
            {
                MMK_PBTriggerEvent(MMIEVENT_PB_UPDATE);
            }
            MMIPB_CmdConfirm(&cnf_ptr->handle_node, cnf_ptr->handle, cnf_ptr->ret);
           
        }        
        break;
    case MMI_PB_DELETE_CNF:
        //ɾ����Ϣ��confirm
        cnf_ptr = (MMIPB_CNF_MSG_T *)param;
        if(cnf_ptr != PNULL)
        {
            MMIPB_CmdConfirm(&cnf_ptr->handle_node, cnf_ptr->handle, cnf_ptr->ret);
            if(MMIPB_ACCESS_CONTACT == cnf_ptr->handle_node.access_type)
            {
                MMK_PBTriggerEvent(MMIEVENT_PB_UPDATE);
            } 
        }          
        break;

#ifdef BT_PBAP_SUPPORT
    case MMI_PB_PBAP_CONTACT_READ_CNF:
        {
            void *data_ptr = PNULL;
            uint16 len = 0;
            MMIPB_PBAP_PARSER_RESULT_T result = {0};/*lint !e578*/
            
            SCI_TRACE_LOW("MMI_PB_PBAP_CONTACT_READ_CNF");
            cnf_ptr = (MMIPB_CNF_MSG_T *)param;
            
            if(NULL == parser_callback)
            {
                return MMI_RESULT_FALSE;
            }
            
            //canceled
            if(MMIAPIBT_IsPbapCancel())
            { 
                SCI_TRACE_LOW("MMIPB: HandlePBPsMsg node parser canceled, SendFinishToBT");
                result.ret = MMIPB_PBAP_PARSER_CANCELED;
                result.sync_count = s_sync_contact_count;
                parser_callback(&result);                       
                //freeallpbapdata();
                MMIAPIBT_SetPbapCancel(FALSE);
                parser_callback = NULL;                        
            }
            //no space
            else if((PNULL != cnf_ptr)&&(MMIPB_ERROR_NO_SPACE == cnf_ptr->ret))
            {
                SCI_TRACE_LOW("MMIPB: HandlePBPsMsg node parser no space, SendFinishToBT");
                result.ret = MMIPB_PBAP_PARSER_NO_SPACE;
                result.sync_count = s_sync_contact_count;
                parser_callback(&result);
                //freeallpbapdata();
                parser_callback = NULL;                        
            }
            //no elem anymore
            else if(0 == GetElem(s_list_head, s_node_number, &data_ptr, &len))
            {
                SCI_TRACE_LOW("MMIPB: HandlePBPsMsg node parser Finished, SendFinishToBT");
                result.ret = MMIPB_PBAP_PARSER_FINISHED;
                result.sync_count = s_sync_contact_count;
                parser_callback(&result);
                //freeallpbapdata();
                parser_callback = NULL;
            }
            //send buffer to pb_task
            else
            {
//                      MMIPB_SendSignalToPBTask(MMIPB_SIG_PBAP_CONTACT_READ, 0);
            }
        }
        break;
    case MMI_PB_PBAP_CALLLOG_READ_CNF:
        {
            MMICL_HandlePBAPReadCnf(param);
        }
        break;                
#endif
	        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

	return (result);
}

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify PB
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifyPBFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param)
{
    //SCI_TRACE_LOW:"[MMIPB]: HandleSIMPlugNotifyPBFunc dual_sys=%d, notify_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3564_112_2_18_2_43_11_98,(uint8*)"dd", dual_sys, notify_event);

    switch (notify_event)
    {
    case MMIPHONE_NOTIFY_SIM_PLUG_IN:
    case MMIPHONE_NOTIFY_SIM_PLUG_OUT:    
        //��applet�������д��ڵ���Ϣ
        s_mmipb_sim_ready[dual_sys] = FALSE;
        if(MMIPB_STATUS_SORTING == MMIPB_GetPBStatus())
        {
            MMIPB_SetPBStatus(MMIPB_STATUS_SORT_EXIT);
            
        }
        else if(MMIPB_STATUS_READY == MMIPB_GetPBStatus())
        {
            MMIPB_SetPBStatus(MMIPB_STATUS_SIM_NOT_READY);            
        }
        break;
    default:
        break;
    }
}




/*****************************************************************************/
//  Description : ע��sim�������Ϣ��callback
//  Global resource dependence : g_mmipb_entry_list
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
LOCAL void MMIPB_RegSimCallBack()
{
    MMIPHONE_SIM_PLUG_NOTIFY_INFO_T sim_plug_notify_info = {0};
    sim_plug_notify_info.module_id = MMI_MODULE_PB;
    sim_plug_notify_info.notify_func = HandleSIMPlugNotifyPBFunc;
    MMIAPIPHONE_RegisterSIMPlugNotifyInfo(&sim_plug_notify_info);
}
#endif

/*****************************************************************************/
//  Description : ��ʼ��Phonebook��ȫ�ֱ������ص�������
//  Global resource dependence : g_mmipb_entry_list, g_mmipb_nv_t, g_mmipb_app
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_InitApp (
						void
						)
{
    uint32 i = 0;
    uint8* mem_addr = PNULL;
    APP_MEM_CFG_T  mem_cfg = {0};

    MMIPB_SetPBStatus(MMIPB_STATUS_SIM_NOT_READY);
    
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        s_mmipb_sim_ready[i] = FALSE;
    }
    // ��ʼ��g_mmipb_app
    g_mmipb_app.ProcessMsg     = HandlePBPsMsg; 	//���ô���Э��ջ��Ϣ�Ļص�����  
#ifdef CMCC_VCARD_SUPPORT
     s_mmipb_is_usim_addtion_ready = FALSE;
#endif
    //�ȴ���Ϣ���еĳ�ʼ��
    MMIPB_InitWaitMsgQueue();
    //����Task
    MMIPB_CreateTask();
    //����ͬ��event
    MMIPB_CreateEvent();
    // ��ʼ��NV�еĵ绰��¼
#ifdef SIM_PLUG_IN_SUPPORT    
    MMIPB_RegSimCallBack();
#endif
#ifdef IKEYBACKUP_SUPPORT
    //ע��һ����ԭ����
    ikeybackup_register();
#endif

    MMIPB_ALLOCContactList();
#ifdef MMIPB_MEMPOOL_SUPPORT
    mem_addr = SCI_ALLOC_CONST(MMIPB_CACHE_SIZE);
    if(mem_addr != PNULL)
    {
        SCI_MEMSET(mem_addr, 0x00, MMIPB_CACHE_SIZE);
        MMIPB_SetMempoolHandle(SCI_CreateMemPool("MMIPB_POOL", mem_addr, MMIPB_CACHE_SIZE, &mem_cfg));      
    }
#endif
    MMIPB_InitInterface();
}

/*****************************************************************************/
//  Description : MMIAPIPB_InitSimStatus
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_InitSimStatus(MN_DUAL_SYS_E dual_sys)
{
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_InitSimStatus  dual_sys %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3659_112_2_18_2_43_12_99,(uint8*)"d", dual_sys);
        return;
    }
    s_mmipb_sim_ready[dual_sys] = TRUE;
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_InitSimStatus dual_sys %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3663_112_2_18_2_43_12_100,(uint8*)"d",dual_sys);
}

/*****************************************************************************/
//  Description : APP_MN_SIM_NOT_READY_IND
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC void MMIPB_HandlePbSimNotReadyInd(DPARAM  param)
{
    APP_MN_SIM_NOT_READY_IND_T       *sim_not_ready_param_ptr = 0;
    MN_DUAL_SYS_E           dual_sys = MN_DUAL_SYS_1;
    BOOLEAN                 is_need_response = FALSE;
    
    sim_not_ready_param_ptr = (APP_MN_SIM_NOT_READY_IND_T *)param;
    
    if(PNULL == sim_not_ready_param_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_HandlePbSimNotReadyInd (PNULL == sim_not_ready_param_ptr) !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3681_112_2_18_2_43_12_101,(uint8*)"");
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if(sim_not_ready_param_ptr->dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_HandlePbSimNotReadyInd sim_not_ready_param_ptr->dual_sys %d!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3688_112_2_18_2_43_12_102,(uint8*)"d", sim_not_ready_param_ptr->dual_sys);
        return;
    }
    dual_sys = sim_not_ready_param_ptr->dual_sys;
#else
    dual_sys = MN_DUAL_SYS_1;
#endif
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_HandlePbSimNotReadyInd dual_sys %d sim_not_ready_param_ptr->reason %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3695_112_2_18_2_43_12_103,(uint8*)"dd",dual_sys, sim_not_ready_param_ptr->reason);
    
    switch(sim_not_ready_param_ptr->reason)
    {
    case MNSIM_ERROR_REJECT_BY_NET:
    case MNSIM_ERROR_IMSI_INVALID:
#ifdef MMI_SIM_LOCK_SUPPORT //sim lock ��������
    case MNSIM_ERROR_SIM_DATA_INTEGRITY_FAIL:
    case MNSIM_ERROR_SIM_NETWORT_LOCKED:
    case MNSIM_ERROR_SIM_NETWORT_SUBSET_LOCKED:
    case MNSIM_ERROR_SIM_SP_LOCKED:
    case MNSIM_ERROR_SIM_CORPORATE_LOCKED:
    case MNSIM_ERROR_SIM_USER_LOCKED:
#endif    
        is_need_response = FALSE;
        break;
        
    default:
        is_need_response = TRUE;
        break;
    }
    if(is_need_response)
    {		
        s_mmipb_sim_ready[dual_sys] = TRUE;
		if(IsAllSimReady() && MMIPB_STATUS_SIM_NOT_READY == MMIPB_GetPBStatus())
		{
			//only when all sim ready and pb not ready, permit to sort
			MMIPB_BeginToSort();
		}
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_HandlePbSimNotReadyInd is_need_response %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3727_112_2_18_2_43_12_104,(uint8*)"d",is_need_response);
    }
}

/*****************************************************************************/
//  Description : deal with finishe sort  confirm
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
LOCAL void HandleSortCnf(void)
{
    MMIPB_STATUS_E status = MMIPB_GetPBStatus();    
    MMIPB_HANDLE_T applet_handle = 0;
    MMIPB_APPLET_T*  applet_ptr= PNULL;

    if(MMIPB_STATUS_SIM_NOT_READY == status)
    {
        //�����������������������Ҫ��������
        if(IsAllSimReady())
        {
            MMIPB_BeginToSort();
        }
    }
    else if(MMIPB_STATUS_SORT_EXIT == status)
    {
        //�����������������������Ҫ��������
        if(IsAllSimReady())
        {
            MMIPB_BeginToSort();
        }
        else
        {
            //����sim not ready״̬
            MMIPB_SetPBStatus(MMIPB_STATUS_SIM_NOT_READY);
        }
    }
    else if(MMIPB_STATUS_SORTING == status)
    {
        //������ɣ�������������û����������������������
        MMIPB_SetPBStatus(MMIPB_STATUS_READY);
        MMK_PBTriggerEvent(MMIEVENT_PB_UPDATE);
        MMIPB_GetCurAppletHandle(&applet_handle);
        applet_ptr = (MMIPB_APPLET_T* )MMK_GetAppletPtr(applet_handle);
        if(applet_ptr != PNULL)
        {
            if(applet_ptr->instance.update_callback != PNULL)
            {
#ifdef  SNS_SUPPORT
                if(applet_ptr->instance.cmd == MMIPB_OPC_GET_SNS)
                {
                applet_ptr->instance.update_callback(applet_handle, 
                                                     MMIPB_OPERATE_SUCCESS, 
                                                     MMIPB_ERROR_SUCCESS,
                                                     MMIPB_OPC_GET_SNS,
                                                     1);
                applet_ptr->instance.cmd =MMIPB_OPC_ADD;
                }
                else

#endif
                {
                
                applet_ptr->instance.update_callback(applet_handle, 
                                                     MMIPB_OPERATE_SUCCESS, 
                                                     MMIPB_ERROR_SUCCESS,
                                                     MMIPB_OPC_FIND,
                                                     1);
                }
            }
            
            //MMK_SendMsg(applet_ptr->instance.active_win_id, MMI_PB_RELOAD_SEARCH_LIST_MSG, 0);
        }
    //��USIM��չ��Ϣ
#ifdef CMCC_VCARD_SUPPORT
        //��ȡusim��չ��Ϣ
        {
            uint16 contact_id = 0;
            uint16 storage_id = 0; 
            
            if(MMIPB_GetNextNotInitEntryID(&contact_id, &storage_id))
            {
                //read next usim entry ans and email
                MMIPB_ReadUSIMAddtionalAsyn(MMIPB_GetDualSysByStorageId(storage_id), contact_id);
            }
            else
            {
                s_mmipb_is_usim_addtion_ready = TRUE;
            }
        }
#endif
#if defined(BT_DIALER_SUPPORT) && defined (BT_PBAP_SUPPORT)

        if(FALSE == BT_GetState())
        {
            MMIPB_ClearAllBtContactForPbap();
        }
        else if(FALSE == MMIAPIBT_IsConnectDevice())
        {
            MMIPB_ClearAllBtContactForPbap();
        }
#endif
    }
#if defined(MMIPB_FUNCTION_TEST_MODE) && defined (WIN32)   
    //mmipb_test_group();
    //MMIPB_InterfeceTest();
    //MMIPB_TestSearch();
    //mmipb_test_storage();
    //MMIPB_ExportTest();
    //MMIPB_DeleteAllMostUsedContact();
    //MMIPB_CopyContact(3, 1, 1);
#endif
}

/*****************************************************************************/
//  Description : handle name sort in pb
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_HandleSortCnf()
{
    HandleSortCnf();
}

/*****************************************************************************/
//  Description : �绰�����µĻص�����
//  Global resource dependence 
//  Author:
//  Note:  
/*****************************************************************************/
// LOCAL void UpdateEventCallback(MMIEVENT_PB_EVENT_E event)
// {
//     if(MMIEVENT_PB_ADD == event
//       ||MMIEVENT_PB_UPDATE == event
//       ||MMIEVENT_PB_DELETE == event
//        )
//     {
//         //
// 
//     }
// }
/*****************************************************************************/
//  Description : ����Ϣ��task���ú���������ϵ�˵���������
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_BeginToSort(void)
{
#ifndef WIN32
    //����Ϊsorting״̬
    MMIPB_SetPBStatus(MMIPB_STATUS_SORTING);
    MMIPB_SendSignalToPBTask(MMIPB_SIG_SORT, 0);
    //PB_SendInitSimToInitTask();
#else
    MMIPB_SetPBStatus(MMIPB_STATUS_SORTING);
    MMIPB_InitEntryList();
    MMIPB_HandleSortCnf();
#endif
}


/*****************************************************************************/
//  Description : Resort Phonebook,
//  Global resource dependence : 
//  Author: baokun.yin
//RETURN: 
//  Note: ������������pb�����������������sim��ready������Ҫ��CALL_READY��SIM_NOT_READY
/*****************************************************************************/
PUBLIC void MMIPB_ResortPB(void)
{
    MMIPB_STATUS_E status = MMIPB_GetPBStatus();
    
    if(MMIPB_STATUS_READY == status)
    {
        //ready״̬�£������Ѿ���������
        MMIPB_BeginToSort();
    }
    else if(MMIPB_STATUS_SORTING == status)
    {
        //���������У��ȴ������������������
        MMIPB_SetPBStatus(MMIPB_STATUS_SORT_EXIT);
    }
//     else
//     {
//         //��MMIPB_STATUS_SIM_NOT_READY����Ϊ���ʱ�򣬻�û�����򣬲���Ҫ���š�
//     }
}

/*****************************************************************************/
//  Description : re_initialize Phonebook,
//  Global resource dependence : 
//  Author: baokun.yin
//RETURN: 
//  Note: ���������ALL_READY��SIM_NOT_READY���ٽ�������
//  [�ر���ʾWarnning]:����������Žӿڣ�һ��Ҫ��֤sim�������·�ALL_READY��SIM_NOT_READY����sim�����³�ʼ��
/*****************************************************************************/
PUBLIC void MMIPB_ReinitPB(MN_DUAL_SYS_E dual_sys)
{
    MMIPB_STATUS_E status = MMIPB_GetPBStatus();

    if(dual_sys< MMI_DUAL_SYS_MAX)
    {
        s_mmipb_sim_ready[dual_sys] = FALSE;
    }
    if(MMIPB_STATUS_SORTING == status)
    {
        MMIPB_SetPBStatus(MMIPB_STATUS_SORT_EXIT);
    }
    else if(MMIPB_STATUS_READY == status)
    {
        MMIPB_SetPBStatus(MMIPB_STATUS_SIM_NOT_READY);
    }
}

/*****************************************************************************/
//  Description : ������ϵ�˵�����
//  Global resource dependence : g_mmipb_entry_list
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_InitEntryList(void)
{
    MN_DUAL_SYS_E           sim_type  = MN_DUAL_SYS_1;
    uint16                     i=0;
    uint16                  storage_id = 0;
    MMIPB_ACCESS_POINT_T    access_point = {0};
    MMIPB_ERROR_E           ret = MMIPB_ERROR_ERROR;
    MMIPB_CONTACT_T *contact_info = PNULL;  
    MMIPB_STORAGE_INFO_T    storage_info = {0};
    MMISET_LANGUAGE_TYPE_E  language_type = 0;
  
#ifndef MMIPB_MEMPOOL_SUPPORT
    uint8* mem_addr = PNULL;
    APP_MEM_CFG_T  mem_cfg = {0};
#endif
    
#ifdef  SNS_SUPPORT
    MMISNS_Sqlite_BuildMutex();
    MMISNS_Sqlite_CleanAll();
    MMISNS_InitPBData();

    MMIPB_OpenSNSDB(PNULL);
#endif
//�����˶���ͬʱע��MMIPB_InitApp�з����ڴ�صĵط�
#ifndef MMIPB_MEMPOOL_SUPPORT
    mem_addr = SCI_ALLOCA(MMIPB_CACHE_SIZE);
    if(mem_addr != PNULL)
    {
        SCI_MEMSET(mem_addr, 0x00, MMIPB_CACHE_SIZE);
        MMIPB_SetMempoolHandle(SCI_CreateMemPool("MMIPB_POOL", mem_addr, MMIPB_CACHE_SIZE, &mem_cfg));      
    }
#endif
    
    //��ʼ�����ݱ�
    MMIPB_InitInterface();
    MMIAPISET_GetLanguageType(&language_type);
    MMIAPIIM_InitLangKeyTable(language_type);
    contact_info = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));

#ifdef MMI_SIM_CONTACT_SUPPORT //del sim contacts init
    //sim���ļ�¼, 
    for(sim_type = MN_DUAL_SYS_1; sim_type < MMI_DUAL_SYS_MAX; sim_type++)
    {
        // ����entry_id����sim���е����м�¼
        if (MMIAPIPHONE_GetSimExistedStatus(sim_type))
        {
             storage_info = MMIPB_GetContactStorageInfo(MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + sim_type, PHONEBOOK_SIM_STORAGE));
             SCI_TRACE_LOW("[mmipb] MMIPB_InitEntryList sim%d.max_record_num = %d",sim_type,storage_info.max_record_num);
            for (i=1; (i<= MIN(storage_info.max_record_num, MMIPB_SIM_MAX_RECORD_NUM)) && (MMIPB_STATUS_SORTING == MMIPB_GetPBStatus()); i++)
            {
                if(0 == (i%100))
                {
                    SCI_Sleep(1);
                }
            
                access_point.contact.contact_id = i;
                storage_id = (PHONEBOOK_SIM_STORAGE<<8) | ((MMIPB_STORAGE_SIM1+sim_type) & 0X00FF);
                access_point.contact.storage_id = storage_id;
                SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
                ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,access_point,sizeof(MMIPB_CONTACT_T),contact_info); //����������Ҫ����usim
                if(MMIPB_ERROR_SUCCESS == ret && (MMIPB_IsContactValid(contact_info)))
                {
                    MMIPB_InsertContactToList(contact_info);
                }
            } 
        }
        else
        {
            SCI_TRACE_LOW("[mmipb] MMIPB_InitEntryList sim%d not exist",sim_type);
        }
    }
#endif
    //nv��¼
    storage_info = MMIPB_GetContactStorageInfo(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE , MMIPB_NORMAL_PHONE_CONTACT));
    SCI_TRACE_LOW("[mmipb] MMIPB_InitEntryList MMIPB_STORAGE_PHONE max_record_num = %d",storage_info.max_record_num);
    for (i=1; (i<= storage_info.max_record_num) && (MMIPB_STATUS_SORTING == MMIPB_GetPBStatus()); i++)
    {
        if(0 == (i%10))
        {
            SCI_Sleep(1);
        }
        access_point.contact.contact_id = i;
        storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
        access_point.contact.storage_id = storage_id;
        SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
        ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,access_point,sizeof(MMIPB_CONTACT_T),contact_info);
        if(MMIPB_ERROR_SUCCESS == ret && (MMIPB_IsContactValid(contact_info)))
        {
            MMIPB_InsertContactToList(contact_info);
#ifdef MMIPB_UID_SUPPORT
            if(contact_info->uid > MMIPB_GetMaxUid())
            {
                MMIPB_SetMaxUid(contact_info->uid);
            }
#endif
        }
        else if(MMIPB_ERROR_NO_ENTRY == ret)
        {
              //no contact exist in nv or tcard
        	break;
        }

    }

#ifdef MMIPB_MOST_USED_SUPPORT
    //������ϵ��
    {
        for (i=1; (i <= MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS) && (MMIPB_STATUS_SORTING == MMIPB_GetPBStatus()); i++)
        {
            access_point.contact.contact_id = i;
            storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_MOSTUSED_PHONE_CONTACT);//(MMIPB_MOSTUSED_PHONE_CONTACT<<8) | (MMIPB_STORAGE_PHONE & 0X00FF); 
            access_point.contact.storage_id = storage_id;
            SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
            ret = MMIPB_Read(MMIPB_ACCESS_CONTACT,access_point,sizeof(MMIPB_CONTACT_T),contact_info);
            if(MMIPB_ERROR_SUCCESS == ret && (MMIPB_IsContactValid(contact_info)))
            {
                MMIPB_InsertContactToList(contact_info);
            }
        }
    }
    SCI_TRACE_LOW("[mmipb] MMIPB_InitEntryList MOSTUSED_Contact contact num = %d",i);
#endif

#ifdef SNS_SUPPORT    
    //���sns��ϵ��
    #ifdef MMI_PDA_SUPPORT
    i = LoadSnsContact();
    #endif
    //SCI_TRACE_LOW:"[MMIPB][SNS] MMIPB_InitEntryList read sns contact num = %d"    
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_3995_112_2_18_2_43_12_108,(uint8*)"d", i);
    
    MMIPB_CloseSNSDB();   
#endif
    SCI_FREE(contact_info);
#ifndef MMIPB_MEMPOOL_SUPPORT
    MMIPB_FreeCache();
    MMIPB_SetMempoolHandle(PNULL);      
    SCI_FREE(mem_addr);
#endif
}

/*****************************************************************************/
//  Description : ��õ绰���Ƿ���Ա�����ģ�����
//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsPbReady //RETURN: TRUE, ����ʹ��
(
   void
)
{
    BOOLEAN result = FALSE;
    if(MMIPB_STATUS_READY == MMIPB_GetPBStatus())
    {
        result = TRUE;
    }
    return result;
}
#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : USIM����չ��Ϣ�Ƿ��Ѿ�����
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsUSIMReady(void)
{
    return s_mmipb_is_usim_addtion_ready;
}
/*****************************************************************************/
//  Description : Ҫ��ȡ��USIM����չ��Ϣ
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_ReadUSIMExtCnf(APP_MN_READ_ADDITIONAL_ENTRY_CNF_T *usim_ext_ptr)
{
    PHONEBOOK_ADDITIONAL_ENTRY_T addtional_entry = {0};
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    uint16  entry_id = 0;
    uint16  storage_id = 0;
    uint16  i = 0;
    
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_ReadUSIMExtCnf enter usim_ext_ptr %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4038_112_2_18_2_43_12_109,(uint8*)"d",usim_ext_ptr);
    if(usim_ext_ptr != PNULL && usim_ext_ptr->dual_sys < MMI_DUAL_SYS_MAX)
    {
        addtional_entry.entry_id = usim_ext_ptr->adn_entry_id;
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_ReadUSIMExtCnf usim_ext_ptr->adn_entry_id %d dual_sys"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4042_112_2_18_2_43_12_110,(uint8*)"d",usim_ext_ptr->adn_entry_id, usim_ext_ptr->dual_sys);
        for(i = 0; i < MIN(MMIPB_ReadUSIMFreeANRCount(usim_ext_ptr->dual_sys), (MMIPB_MAX_NV_PHONE_NUM -1)); i++)/*lint !e666*/
        {
            //SCI_TRACE_LOW:"[MMIPB]MMIPB_ReadUSIMExtCnf usim_ext_ptr->add_info.anr_ret[%d] %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4045_112_2_18_2_43_12_111,(uint8*)"dd",i, usim_ext_ptr->add_info.anr_ret[i]);
            
            if(usim_ext_ptr->add_info.anr_ret[i])
            {
                addtional_entry.anr_number[i] = usim_ext_ptr->add_info.anr_number[i];
            }
        }
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_ReadUSIMExtCnf usim_ext_ptr->add_info.email_ret %d email len %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4052_112_2_18_2_43_12_112,(uint8*)"dd",usim_ext_ptr->add_info.email_ret, usim_ext_ptr->add_info.email_addr.address_len);
        if(usim_ext_ptr->add_info.email_ret)
        {
            addtional_entry.email_addr = usim_ext_ptr->add_info.email_addr; 
        }
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_ReadUSIMExtCnf usim_ext_ptr->add_info.sne_ret %d sne len %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4057_112_2_18_2_43_12_113,(uint8*)"dd",usim_ext_ptr->add_info.sne_ret, usim_ext_ptr->add_info.sne_alpha.alpha_num);
        if(usim_ext_ptr->add_info.sne_ret)
        {
            addtional_entry.sne_alpha = usim_ext_ptr->add_info.sne_alpha; 
        }
        contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
        if(contact_ptr != PNULL)
        {
            SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
            contact_ptr->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + usim_ext_ptr->dual_sys, PHONEBOOK_SIM_STORAGE);
            if(MMIPB_ERROR_SUCCESS == MMIPB_USIMAddtionalContactToContact(
                &addtional_entry,//[IN]
                contact_ptr//[OUT]                                            
                )
              )
            {
                MMIPB_SaveUSIMAddtionContact(contact_ptr->contact_id, contact_ptr->storage_id, contact_ptr);
            } 
            SCI_FREE(contact_ptr);
        }
       
        if(MMIPB_GetNextNotInitEntryID(&entry_id, &storage_id))
        {
            //read next usim entry ans and email
            MMIPB_ReadUSIMAddtionalAsyn(MMIPB_GetDualSysByStorageId(storage_id), entry_id);
        }
        else
        {
            s_mmipb_is_usim_addtion_ready = TRUE;
        }
    }
}


/*****************************************************************************/
//  Description : �ú������ڴ�����յ���APP_MN_UPDATE_USER_INFO_CNF��Ϣ
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_USIMUpdateCnf(APP_MN_UPDATE_USER_INFO_CNF_T *update_result)
{
    BOOLEAN             handle_ret = FALSE;
    MMIPB_ACCESS_POINT_T access_point = {0};
    MMIPB_CONTACT_T *contact = PNULL;  
    MMIPB_CONTACT_T     *src_contact_ptr = PNULL;
    MMIPB_HANDLE_NODE_T handle_node = {0};
    uint8                anr_index = 0;
    
    if(update_result == PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB] HandleEntryUpdateResult (update_result == PNULL)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4105_112_2_18_2_43_12_114,(uint8*)"");
        return ;
    }
    handle_ret = MMIPB_GetHandleContent(MMIPB_GetCurrentHandle(), &handle_node);
    access_point.contact.storage_id = MMIPB_GetStorageID(update_result->dual_sys + MMIPB_STORAGE_SIM1, PHONEBOOK_SIM_STORAGE);
    if((OPERATE_SUCCESS != update_result->update_result.adn_result && update_result->update_result.adn_flag))
    {
        //adn number exist, but add fail stop add and copy
        //SCI_TRACE_LOW:"[MMIPB]HandleEntryUpdateResult update_result %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4113_112_2_18_2_43_12_115,(uint8*)"d", update_result);
        MMIPB_WriteTrigger(update_result->adn_entry_id, access_point.contact.storage_id, MMIPB_ERROR_ERROR);
        MMIPB_CmdConfirm(&handle_node, MMIPB_GetCurrentHandle(), MMIPB_ERROR_ERROR);
    }
    else if(update_result->dual_sys < MMI_DUAL_SYS_MAX)
    {        
        contact = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
        SCI_MEMSET(contact, 0x00, sizeof(MMIPB_CONTACT_T));
        //if adn exist, add adn number to entry list
        MMIPB_WriteTrigger(update_result->adn_entry_id, access_point.contact.storage_id, MMIPB_ERROR_SUCCESS);
        if(update_result->update_result.adn_flag)
        {
            access_point.contact.contact_id = update_result->adn_entry_id;
            
            //����������ɾ��ԭ���ļ�¼
            MMIPB_RemoveContactFromList(access_point.contact.contact_id, access_point.contact.storage_id);
            MMIPB_Read(MMIPB_ACCESS_CONTACT, access_point, sizeof(MMIPB_CONTACT_T), contact);
            contact->is_read_ext = TRUE;
            if((MMIPB_IsContactValid(contact)))
            {
                //��������µļ�¼����������
                MMIPB_InsertContactToList(contact);                
            }
            MMK_PBTriggerEvent(MMIEVENT_PB_UPDATE);
        }

        if(!handle_ret)
        {
            MMIPB_CmdConfirm(&handle_node, MMIPB_GetCurrentHandle(), MMIPB_ERROR_ERROR);
             SCI_FREE(contact);
            return;
        }

        if(handle_node.data_ptr == PNULL)
        {
            MMIPB_CmdConfirm(&handle_node, MMIPB_GetCurrentHandle(), MMIPB_ERROR_ERROR);
             SCI_FREE(contact);
            return;
        }
        //SCI_MEMCPY(contact, handle_node.data_ptr, sizeof(MMIPB_CONTACT_T));
        src_contact_ptr = (MMIPB_CONTACT_T *)handle_node.data_ptr;
        for(anr_index = 0; anr_index < MIN(MMIPB_ReadUSIMFreeANRCount(update_result->dual_sys), (MMIPB_MAX_NV_PHONE_NUM -1)); anr_index++)/*lint !e666*/
        {
 
            if((OPERATE_SUCCESS == update_result->update_result.anr_result[anr_index]
                || (!update_result->update_result.anr_flag[anr_index])
                ) /*add success*/
                                  /*this anr be added*/
            )
            {
                //SCI_MEMSET(&contact->number[anr_index +1], 0x00, sizeof(MMIPB_BCD_NUMBER_T));
                contact->num_type[anr_index +1] = src_contact_ptr->num_type[anr_index +1];
                SCI_MEMCPY(&contact->number[anr_index +1], &src_contact_ptr->number[anr_index +1], sizeof(MMIPB_BCD_NUMBER_T));  
                
            }
        }
#ifdef MMIPB_MAIL_SUPPORT
        if(OPERATE_SUCCESS ==update_result->update_result.email_result 
          ||(!update_result->update_result.email_flag))
        {
            //Email 
            SCI_MEMCPY(&contact->mail, &src_contact_ptr->mail, sizeof(MMIPB_MAIL_T));  
        }
#endif
#ifdef CMCC_VCARD_SUPPORT
        if(OPERATE_SUCCESS !=update_result->update_result.sne_result 
          ||(!update_result->update_result.sne_flag))
        {
            //alias 
            SCI_MEMCPY(&contact->alias, &src_contact_ptr->alias, sizeof(MMIPB_NAME_T));  
        }
#endif       
        MMIPB_SaveUSIMAddtionContact(update_result->adn_entry_id, access_point.contact.storage_id, contact);        
        SCI_FREE(contact);
    }
    MMIPB_GetHandleContent(MMIPB_GetCurrentHandle(), &handle_node);
    if(handle_node.data_ptr != PNULL  )
    {
        SCI_FREE(handle_node.data_ptr);
        handle_node.data_ptr = PNULL;
        MMIPB_SetHandleContent(MMIPB_GetCurrentHandle(), &handle_node);
    }
    MMIPB_CmdConfirm(&handle_node, MMIPB_GetCurrentHandle(), MMIPB_ERROR_SUCCESS);
}


/*****************************************************************************/
//  Description : �ú������ڴ�����յ���APP_MN_DEL_USER_INFO_CNF��Ϣ
//  Global resource dependence : none
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_USIMDeleteCnf(APP_MN_UPDATE_USER_INFO_CNF_T *delete_result)
{
    uint16               contact_id = 0;
    uint16               storage_id = 0;
    MMIPB_ERROR_E        result = MMIPB_ERROR_ERROR;
    MMIPB_HANDLE_NODE_T   handle_node = {0};
    if(PNULL == delete_result)
    {
        //SCI_TRACE_LOW:"[MMIPB] delete_result PNULL == delete_result !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4199_112_2_18_2_43_13_117,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"[MMIPB]HandleEntryDeleteResult, delete_result adn_result%d dual_sys %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4202_112_2_18_2_43_13_118,(uint8*)"dd", delete_result->update_result.adn_result, delete_result->dual_sys);
    storage_id = MMIPB_GetStorageID(delete_result->dual_sys + MMIPB_STORAGE_SIM1, PHONEBOOK_SIM_STORAGE);
    contact_id = delete_result->adn_entry_id;
    //MMIPB_DeleteTrigger();
	if(OPERATE_SUCCESS == delete_result->update_result.adn_result || (!delete_result->update_result.adn_flag))
	{
		//adn number delte success or adn not exist---need delete this entry from entry list
        result = MMIPB_RemoveContactFromList(contact_id , storage_id);
        MMK_PBTriggerEvent(MMIEVENT_PB_DELETE);
 	}
    //if(MMIPB_IsCurrentOperation(contact_id , storage_id))
    MMIPB_GetHandleContent(MMIPB_GetCurrentHandle(), &handle_node);
    {
        MMIPB_CmdConfirm(&handle_node, MMIPB_GetCurrentHandle(), result);
    }
}
#endif

/*****************************************************************************/
//  Description : ��ȡPB��״̬
//  Global resource dependence : none
//  Note: 
//  author: baokun.yin
/*****************************************************************************/
PUBLIC void MMIPB_SetPBStatus(MMIPB_STATUS_E status)
{
    SCI_DisableIRQ();
    s_mmpb_status = status;
    SCI_RestoreIRQ();
    //SCI_TRACE_LOW:"[MMIPB] MMIPB_SetPBStatus status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4232_112_2_18_2_43_13_119,(uint8*)"d", status);
}

/*****************************************************************************/
//  Description : ����pb״̬
//  Global resource dependence : none
//  Note: 
//  author: baokun.yin
/*****************************************************************************/
PUBLIC MMIPB_STATUS_E MMIPB_GetPBStatus(void)
{
    MMIPB_STATUS_E status = 0;

    //SCI_DisableIRQ();
    status = s_mmpb_status;
    //SCI_RestoreIRQ();
    //SCI_TRACE_LOW:"[MMIPB] MMIPB_GetPBStatus status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4246_112_2_18_2_43_13_120,(uint8*)"d", status);
    return status;
}

#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : ���õ绰����ı�ʹ��

//  Note: ���¸ú����ʹ��Ƶ�ʡ�
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SetNumberUsed(MMIPB_BCD_NUMBER_T *number_ptr)
{
    uint16 entry_id = 0;
    uint16 storage_id = 0;
    uint16  count = 0;
    uint16  index = 0;
    MMIPB_MOSTUSED_CONTACT_NODE_T  contact_node = {0};
    MMIPB_CONTACT_T *contact = PNULL;  
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    
    if(!MMIAPIPB_IsPbReady())
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_SetNumberUsed pb not ready !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4263_112_2_18_2_43_13_121,(uint8*)"");
        return FALSE;
    }
    
    if(number_ptr != PNULL && MMIPB_IsValidPhoneNumber(number_ptr->npi_ton,number_ptr->number_len))
    {	        
        count = MMIPB_GetAllMostUsedContactCount();
        contact = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
        SCI_MEMSET(contact, 0x00, sizeof(MMIPB_CONTACT_T));
        if(MMIPB_SearchPhoneNumberInMostUsedlist(number_ptr, &entry_id, &storage_id))//�����Ѿ��ڳ�����ϵ����
        {
            MMIPB_GetContactInfo(entry_id, storage_id, contact);
            contact->union_t.used_freq = contact->union_t.used_freq +  0x10000;
        }
        else if(count >= MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS)
        {
            index = count-1;
            contact_node =  MMIPB_GetMostUsedContactBaseInfo(index);
            MMIPB_DeleteContact(contact_node.entry_id, contact_node.storage_id);
            SCI_MEMCPY(&contact->number[0] ,number_ptr, sizeof(MMIPB_BCD_NUMBER_T));
            contact->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_MOSTUSED_PHONE_CONTACT);
            contact->union_t.used_freq = contact->union_t.used_freq +count;
        }
        else
        {
            SCI_MEMCPY(&contact->number[0] ,number_ptr, sizeof(MMIPB_BCD_NUMBER_T));
            contact->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_MOSTUSED_PHONE_CONTACT);
            contact->union_t.used_freq = contact->union_t.used_freq +count;
        } 
        ret = MMIPB_AddContact(contact);
        if(MMIPB_ERROR_SUCCESS == ret)
        {
            MMIPB_APPLET_T*    applet_ptr = PNULL;
            MMI_HANDLE_T applet_handle =NULL;
            MMIPB_GetCurAppletHandle(&applet_handle);
            applet_ptr = (MMIPB_APPLET_T* )MMK_GetAppletPtr(applet_handle);
            if(applet_ptr != PNULL)
            {
                if(applet_ptr->instance.update_callback != PNULL)
                {
                    applet_ptr->instance.update_callback(applet_handle, 
                                                         MMIPB_OPERATE_SUCCESS, 
                                                         MMIPB_ERROR_SUCCESS,
                                                         MMIPB_OPC_ADD_MOSTUSED,
                                                         1);
                }
            }
        }
        SCI_FREE(contact);
    }
    return TRUE;
}
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
PUBLIC BOOLEAN  MMIPB_GetSelectContact(MMIPB_HANDLE_T handle,MMIPB_CONTACT_T * contact_ptr, uint16 index, uint16 * num_index)
{
    BOOLEAN           ret = FALSE;
    int                 i = 0;
    MMIPB_SEL_NODE_T*  s_sel_node_ptr = PNULL;
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);

    if(PNULL == contact_ptr )
    {
        //SCI_TRACE_LOW:"[mmipb]MMIPB_ReadSearchedName PNULL == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4320_112_2_18_2_43_13_122,(uint8*)"");
        return ret;
    }

    if(PNULL == applet_ptr || applet_ptr->instance.select_list == PNULL)
    {
        //SCI_TRACE_LOW:"[mmipb]MMIPB_ReadSearchedName PNULL == mmipb_handle_data"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4335_112_2_18_2_43_13_124,(uint8*)"");
        return ret;
    }
    s_sel_node_ptr = applet_ptr->instance.select_list->sel_table_head;

    for(i = 0; i<index; i++)
    {
        s_sel_node_ptr = s_sel_node_ptr->next;
    }

    if(PNULL == s_sel_node_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb]MMIPB_ReadSearchedName index %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4347_112_2_18_2_43_13_125,(uint8*)"d", index);
        return ret;
    }
    
    
    if( MMIPB_ERROR_SUCCESS ==  MMIPB_GetContactInfo(s_sel_node_ptr->entry_id, s_sel_node_ptr->storage_id, contact_ptr))
    {
        if(PNULL != num_index)
        {
        
            *num_index = s_sel_node_ptr->num_index;
        }
        ret =  TRUE;
    }
    return ret;
}


/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ��������
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_GetSelectName(MMIPB_HANDLE_T handle,MMI_STRING_T *name_str, uint16 max_name_len,uint16 index)
{
    MMIPB_CONTACT_T *contact_info = PNULL;  
    BOOLEAN           ret = FALSE;
    MMI_STRING_T     str = {0}; 
    wchar            name_wstr[MMIPB_MAX_STRING_LEN] = {0};

    
    contact_info = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
    
    if(MMIPB_GetSelectContact(handle, contact_info, index, PNULL))
    {
        str.wstr_ptr = name_wstr;
        MMIPB_ConvertNameToString(
            &str,
            &(contact_info->name)
            );
        MMIAPICOM_Wstrncpy(name_str->wstr_ptr, str.wstr_ptr, max_name_len);
        name_str->wstr_len =MIN(max_name_len, str.wstr_len);
        ret = TRUE;
    }

    SCI_FREE(contact_info);
    return ret;
}


/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ��email��
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

PUBLIC BOOLEAN  MMIPB_GetSelectEmail(MMIPB_HANDLE_T handle,MMI_STRING_T *mail_ptr, uint16 max_mail_len,uint16 index)
{
    MMIPB_CONTACT_T *contact_info = PNULL;  
    BOOLEAN           ret = FALSE;	
    uint16           num_index= 0;
     if(PNULL == mail_ptr)
     {
         //SCI_TRACE_LOW:"[mmipb] MMIPB_ReadSearchedName PNULL == nameber_ptr"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4409_112_2_18_2_43_13_126,(uint8*)"");
         return ret;
     }

    contact_info = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
     if(MMIPB_GetSelectContact(handle, contact_info, index, &num_index))
     {

        if(num_index == MMIPB_MAX_NV_PHONE_NUM)
        {
#ifdef MMIPB_MAIL_SUPPORT
            MMIAPICOM_Wstrncpy(mail_ptr->wstr_ptr, contact_info->mail.wstr, max_mail_len);
            mail_ptr->wstr_len =MIN(max_mail_len, contact_info->mail.wstr_len);
            ret = TRUE;
#else
            ret = FALSE;
#endif
        }
    }

    SCI_FREE(contact_info);
    return ret;

}

/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ�˺��롣
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_GetSelectNumber(MMIPB_HANDLE_T handle, MMIPB_BCD_NUMBER_T *nameber_ptr, uint16 max_number_len,  uint16 index)
{
    MMIPB_CONTACT_T *contact_ptr = PNULL;  
    BOOLEAN           ret = FALSE;
    uint16            num_index = 0;
    uint16            len = 0;

     if(PNULL == nameber_ptr)
     {
         //SCI_TRACE_LOW:"[mmipb] MMIPB_ReadSearchedName PNULL == nameber_ptr"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4441_112_2_18_2_43_13_127,(uint8*)"");
         return ret;
     }

    contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_GetSelectNumber PNULL == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4448_112_2_18_2_43_13_128,(uint8*)"");
        return FALSE;

    }
    SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    if(MMIPB_GetSelectContact(handle, contact_ptr, index, &num_index))
    {
        if(num_index < MMIPB_MAX_NV_PHONE_NUM)
        {
        
            len = MIN(contact_ptr->number[num_index].number_len, max_number_len);

            //contact_info->number[num_index].number[len] ='\0';
            nameber_ptr->number_len = len;
            SCI_MEMCPY(nameber_ptr, &contact_ptr->number[num_index], sizeof(MMIPB_BCD_NUMBER_T));
            ret = TRUE;
        }
    }

    SCI_FREE(contact_ptr);
    return ret;
}
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ��vcard��
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_GetSelectVcard(MMIPB_HANDLE_T handle, uint8  *data_ptr, uint16 max_buf_len,  uint16 index)
{
    MMIPB_CONTACT_T *contact_ptr = PNULL;  
    BOOLEAN           ret = FALSE;
    uint16            num_index = 0;   
    uint32            data_len = 0;

    if(PNULL == data_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] MMIPB_GetSelectVcard PNULL == data_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4482_112_2_18_2_43_13_129,(uint8*)"");
        return ret;
    }

    contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));

    if(PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"mmipb, MMIPB_GetSelectVcardPNULL == contact_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4490_112_2_18_2_43_13_130,(uint8*)"");
	 return ret;	
    }
    SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    if(MMIPB_GetSelectContact(handle, contact_ptr, index, &num_index))
    {
        MMIPB_ContactToVcard(contact_ptr,max_buf_len, data_ptr, &data_len);
        ret = TRUE;
    }

    SCI_FREE(contact_ptr);
    return ret;
}
#endif
/*****************************************************************************/
//  Description :   ���PB
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_AbortPB(BOOLEAN is_abort)
{

    MMIPB_HANDLE_T applet_handle = 0;
    MMIPB_APPLET_T* applet_ptr  = PNULL;
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_AbortPB is_abort %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4513_112_2_18_2_43_13_131,(uint8*)"d", is_abort);
    if(is_abort)
    {

        MMIPB_GetCurAppletHandle(&applet_handle);
        applet_ptr = (MMIPB_APPLET_T* )MMK_GetAppletPtr(applet_handle);
        if(applet_ptr != PNULL)
        {
        MMIPB_DeleteAllSelNode(applet_ptr->instance.handle);
        }
    }

     //SCI_TRACE_LOW:"[MMIPB]MMIPB_AbortPB is_abort %d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4525_112_2_18_2_43_13_132,(uint8*)"d", is_abort);

     s_mmipb_is_abort = is_abort;
}

// @baokun fix cr87685 begin
/*****************************************************************************/
//  Discription: This function is used to send signal from mmi to mmi
//   
//  Global resource dependence: None
//  Author: baokun yin
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC  void MMIPB_SendSignal(uint32 sig_id)
{
	   MmiSignalS *signal = PNULL;
       
       //creat signal
       MMI_CREATE_SIGNAL(signal,(uint16)sig_id,sizeof(MmiSignalS),P_APP);
           //send signal
       MMI_SEND_SIGNAL(signal, P_APP);
}





/*****************************************************************************/
//  Description : ��BYTE�л��NUMBER PLAN
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
MN_NUMBER_PLAN_E MMIPB_GetNumberPlanFromUint8
(
 uint8   npi_ton     //IN:
 )
{
    return ((MN_NUMBER_PLAN_E)(npi_ton & 0x0f));
}


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
                                              )
{
    uint16 contact_id = 0;
    uint16 storage_id = 0;
#ifdef MMIPB_MOST_USED_SUPPORT 
    if(PB_GROUP_MOSTUSED == group_id)
    {
        MMIPB_MOSTUSED_CONTACT_NODE_T mostused_contact_node = {0};
        mostused_contact_node = MMIPB_GetMostUsedContactBaseInfo(index);

        contact_id = mostused_contact_node.entry_id;
        storage_id =mostused_contact_node.storage_id;
    }
    else
#endif
    {
        MMIPB_CONTACT_BASE_INFO_T contact_node ={0};
        contact_node = MMIPB_GetContactBaseInfo(index);
        contact_id = contact_node.contact_id;
        storage_id =contact_node.storage_id;        
    }
    if(contact_id_ptr != PNULL)
    {
        *contact_id_ptr = contact_id;
        
    }
    if(storage_id_ptr != PNULL)
    {
        *storage_id_ptr = storage_id;
    }

}

/*****************************************************************************/
//  Description : ��sim��λ��ӳ��Ϊgroup id
//  Global resource dependence : none
//  Author: Michael.Shi
//  Note: 
/*****************************************************************************/
PUBLIC PB_GROUP_LIST_E MMIPB_GetSimGroupID(MN_DUAL_SYS_E dual)
{
    PB_GROUP_LIST_E group_id = PB_GROUP_SIM_1;

   if(dual < MMI_DUAL_SYS_MAX)
   {
        group_id = PB_GROUP_SIM_1+(dual-MN_DUAL_SYS_1);/*lint !e656*/
   }
        
   return group_id;
}

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
                                                )
{
    BOOLEAN           ret = FALSE;
    uint16                  entry_id = 0;
    uint16                  storage_id = 0;
    MMIPB_CUSTOM_PHOTO_T *custom_photo_ptr = PNULL;	
    MMIPB_CONTACT_T *contact_info = PNULL;  

    if(path_ptr == PNULL || bcd_num_ptr == PNULL || path_len_ptr == PNULL || file_size_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_GetCustomPhotoPathByNumber invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4639_112_2_18_2_43_14_133,(uint8*)"");
        return ret;
    }
    if(!MMIPB_SearchPhoneNumber(bcd_num_ptr, &entry_id, &storage_id, PNULL))
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetCustomPhotoPathByNumber not find !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4644_112_2_18_2_43_14_134,(uint8*)"");
        return ret;
    }


    contact_info = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
    custom_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_CUSTOM_PHOTO_T));
    if(custom_photo_ptr != PNULL)
    {
        if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactInfo(entry_id, storage_id, contact_info))
        {
            if (MMIPB_IsInNvGroupByStorage(storage_id)
                            && (contact_info->photo_id_flag & MMIPB_PHOTO_ID_EXIST) 
                            && MMIPB_ReadPhotoFile(entry_id, storage_id,custom_photo_ptr)
            )
            {
                if(custom_photo_ptr->is_valid)
                {
                    if (MMIAPIFMM_IsFileExist(custom_photo_ptr->file_name,(uint16)custom_photo_ptr->file_name_len) 
                                                    && (custom_photo_ptr->file_name_len <= MMIPB_MAX_CUSTOM_PHOTO_FILE_SIZE)
                        )
                        {
                            MMIAPICOM_Wstrncpy(path_ptr, custom_photo_ptr->file_name, custom_photo_ptr->file_name_len);
                            *path_len_ptr = custom_photo_ptr->file_name_len;
                            *file_size_ptr = custom_photo_ptr->file_size;
                            ret = TRUE;
                        }
                        else
                        {
                            //MMIPB_DeleteContactInfoFile(entry_id, storage_id, MMIPB_PHOTO_FILE);
                            ret = FALSE;
                        }
                }
            }
            else
            {
                ret = FALSE;
            }
        }
        SCI_FREE(custom_photo_ptr);
    }
    else
    {
        SCI_TRACE_LOW("MMIPB_GetCustomPhotoPathByNumber custom_photo_ptr != PNULL ");
    }
    SCI_FREE(contact_info);
    return ret;
}
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
											  )
{

    uint16                  entry_id = 0;
    uint16                  storage_id = 0;	
    MMIPB_CONTACT_T *contact_info = PNULL;  
    MMIPB_CUSTOM_RING_T custom_ring = {0};
    BOOLEAN              result = FALSE;
    
    if(path_ptr == PNULL || bcd_num_ptr == PNULL || path_len_ptr == PNULL || is_fixed_id == PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_GetCustomASPFileByNumber invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4711_112_2_18_2_43_14_135,(uint8*)"");
        return result;
    }

    //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetCustomASPFileByNumber entered"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4715_112_2_18_2_43_14_136,(uint8*)"");
	
    if(!MMIPB_SearchPhoneNumber(bcd_num_ptr, &entry_id, &storage_id, PNULL))
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetCustomASPFileByNumber not find !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4719_112_2_18_2_43_14_137,(uint8*)"");
        return result;
    }

    *is_fixed_id = FALSE;    
    
    contact_info = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
    if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactInfo(entry_id, storage_id, contact_info))
    {
        if (MMIPB_IsInNvGroupByStorage(storage_id) && (contact_info->asp_id_flag & MMIPB_ASP_ID_EXIST))
        {
            if(contact_info->asp_id_flag & MMIPB_ASP_FIX_ID_EXIST)
            {
                *is_fixed_id = TRUE;
                if(fixed_id != PNULL)
                {
                *fixed_id = (uint32)(contact_info->asp_id_flag & MMIPB_ASP_FIX_ID_MASK);
                result = TRUE;
                }
            }
            else if(MMIPB_ReadASPFile(entry_id,storage_id, &custom_ring))
            {
                if(custom_ring.is_valid)
                {
                    if (MMIAPIFMM_IsFileExist(custom_ring.file_name,(uint16)custom_ring.file_name_len) && (custom_ring.file_name_len <= MMIPB_MAX_CUSTOM_RING_FILE_SIZE))
                    {
                        MMIAPICOM_Wstrncpy(path_ptr, custom_ring.file_name, custom_ring.file_name_len);
                        *path_len_ptr = custom_ring.file_name_len;
                        result = TRUE;
                    }
//                 else
//                 {
//                     //MMIPB_DeleteContactInfoFile(entry_id, storage_id, MMIPB_ASP_FILE);
//                     //SCI_TRACE_LOW:"[MMIPB] MMIPB_GetCustomASPFileByNumber has deleted asp pb"
//                     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4753_112_2_18_2_43_14_138,(uint8*)"");
//                 }
                }
            }
        }
    }
   
    SCI_FREE(contact_info);
    return result;

}
#endif

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : get group ring path name
//  Global resource dependence : none
//  Author: 
//  Note:	path_ptr length should be larger than MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetGroupRing(
                                  MMIPB_CONTACT_T* contact_ptr,
                                  BOOLEAN  *is_fixed_id,
								  wchar	   *path_ptr,
								  uint32   *path_len_ptr,
								  uint32   *fixed_id
                                  )
{
    int group_count = 0;
    int i = 0;
    uint8   group_id_arrary[MMIPB_MAX_GROUP_NUM] = {0};
    MMIPB_GROUP_T group_info = {0};

    group_count = MMIPB_GetAllGroupIDs(contact_ptr->union_t.group, group_id_arrary, MMIPB_MAX_GROUP_NUM);
    for(i=0; i<group_count; i++)
    {
        MMIPB_GetGroupInfo(group_id_arrary[i], &group_info);
        if(group_info.ring_id_flag& MMIPB_RING_ID_EXIST)
        {

            //uint16   file_name[MMIPB_MAX_CUSTOM_RING_PATH_LEN] = {0};
            //uint16  file_name_len = 0;
            MMIPB_CUSTOM_RING_T custom_ring={0};
            if(group_info.ring_id_flag & MMIPB_FIXED_RING_FLAG)
            {
                if( (group_info.ring_id_flag & MMIPB_FIXED_RING_ID_MASK)  <= MMISET_RING_FIXED_DEFAUT_NUM )
                {
                    *is_fixed_id = TRUE;
                    if(fixed_id != PNULL)
                    {
                        *fixed_id = (uint32)(group_info.ring_id_flag & MMIPB_FIXED_RING_ID_MASK);
                        return TRUE;
                    }
                }

                
            }

            else if(MMIPB_ReadGroupRingFile(group_info.group_id, &custom_ring))
            {
                if (MMIAPIFMM_IsFileExist(custom_ring.file_name,(uint16)(custom_ring.file_name_len))
                   && (custom_ring.file_name_len <= MMIPB_MAX_CUSTOM_RING_FILE_SIZE))
                {
                        MMIAPICOM_Wstrncpy(path_ptr, custom_ring.file_name, custom_ring.file_name_len);
                        *path_len_ptr = custom_ring.file_name_len;
                        return TRUE;
                }
//                 else
//                 {
//                     MMIPB_DeleteGroupRingInfo(group_id_arrary[i]);
//                 }
            }
        }
    }
    return FALSE;
}
#endif
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
											  )
{
    BOOLEAN              result = FALSE;
    uint16                  entry_id = 0;
    uint16                  storage_id = 0;	
    MMIPB_CONTACT_T *contact_info = PNULL;  
    MMIPB_CUSTOM_RING_T custom_ring = {0};
    //int i = 0;
    
    if(path_ptr == PNULL || bcd_num_ptr == PNULL || path_len_ptr == PNULL || is_fixed_id == PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_GetCustomRingFileByNumber invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4852_112_2_18_2_43_14_139,(uint8*)"");
        return result;
    }

    //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetCustomRingFileByNumber entered"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4856_112_2_18_2_43_14_140,(uint8*)"");
	
    if(!MMIPB_SearchPhoneNumber(bcd_num_ptr, &entry_id, &storage_id, PNULL))
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_GetCustomRingFileByNumber not find !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4860_112_2_18_2_43_14_141,(uint8*)"");
        return result;
    }

    *is_fixed_id = FALSE;    
    
    contact_info = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
    if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactInfo(entry_id, storage_id, contact_info))
    {
  
        if (MMIPB_IsInNvGroupByStorage(storage_id) && (contact_info->ring_id_flag & MMIPB_RING_ID_EXIST))
        {
            if(contact_info->ring_id_flag & MMIPB_FIXED_RING_FLAG)
            {
                *is_fixed_id = TRUE;
                if(fixed_id != PNULL)
                {
                *fixed_id = (uint32)(contact_info->ring_id_flag & MMIPB_FIXED_RING_ID_MASK);
                result = TRUE;
                }
            }
            else if(MMIPB_ReadRingFile(entry_id,storage_id, &custom_ring))
            {
                if(custom_ring.is_valid)
                {
                    if (MMIAPIFMM_IsFileExist(custom_ring.file_name,(uint16)custom_ring.file_name_len) && (custom_ring.file_name_len <= MMIPB_MAX_CUSTOM_RING_FILE_SIZE))
                    {
                        MMIAPICOM_Wstrncpy(path_ptr, custom_ring.file_name, custom_ring.file_name_len);
                        *path_len_ptr = custom_ring.file_name_len;
                        result = TRUE;
                    }
//                 else
//                 {
//                     MMIPB_DeleteContactInfoFile(entry_id, storage_id, MMIPB_RING_FILE);
//                     //SCI_TRACE_LOW:"[MMIPB] MMIPB_GetCustomRingFileByNumber has deleted  pb"
//                     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_4895_112_2_18_2_43_14_142,(uint8*)"");
//                 }
                }
            }
        }
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
        else
        {
            result = MMIPB_GetGroupRing(contact_info, is_fixed_id, path_ptr, path_len_ptr, fixed_id);
        }
#endif        


        

    }
    SCI_FREE(contact_info);
    return result;
}

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description :Ⱥ���Ƿ���Ч
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPB_IsUserGroupIdValid(uint32 group_id)
{
     MMIPB_ACCESS_POINT_T access_point = {0};
      MMIPB_GROUP_T group = {0};
     access_point.group_id = group_id;
    MMIPB_Read(MMIPB_ACCESS_GROUP, access_point, sizeof(group), &group);
    return group.is_valid;
}

/*****************************************************************************/
//  Description :����Ⱥ��ʱ��ÿ��õ�Ⱥ��id
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetFreeUserGroupId(void)
{
    int i = MMIPB_SYSTEM_GROUP_NUM;
    MMIPB_ACCESS_POINT_T access_point = {0};
    MMIPB_GROUP_T group = {0};
    
    for(i=MMIPB_SYSTEM_GROUP_NUM; i<MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM; i++)
    {
        SCI_MEMSET(&group, 0X00, sizeof(MMIPB_GROUP_T));
        access_point.group_id = i;
        MMIPB_Read(MMIPB_ACCESS_GROUP, access_point, sizeof(group), &group);
        if(FALSE == group.is_valid)
        {
            break;
        }

    }

    return i;

}

/*****************************************************************************/
//  Description : ������
//  Global resource dependence : s_mmipb_group_info
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SaveGroup  //RETURN: TRUE�������ɹ�; FALSE������ʧ��
(
 MMIPB_NAME_T    *group_name_ptr,    //IN: Ⱥ�������
 uint8	        group_id,            //IN: Ⱥ���id��
 BOOLEAN             is_valid
 )
{

    MMIPB_GROUP_T group = {0};
    BOOLEAN result = FALSE;
    MMIPB_ACCESS_POINT_T access_point = {0};
    
    if(group_name_ptr != PNULL)
    {
        access_point.group_id = group_id;
        group.group_id = group_id;
        group.is_valid = is_valid;
        SCI_MEMCPY(&group.group_name, group_name_ptr, sizeof(group.group_name));
        if(MMIPB_ERROR_SUCCESS == MMIPB_Write(MMIPB_ACCESS_GROUP, access_point, sizeof(group), &group))
        {
            result = TRUE;
        }
    }

    return result;
}

 /*****************************************************************************/
//  Description : add to group
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactToGroup(MMIPB_HANDLE_T handle ,MMIPB_CONTACT_T* contact_ptr,uint8 group_id)
{
    contact_ptr->union_t.group = contact_ptr->union_t.group | 1<<group_id;
    return MMIPB_ModifyContact(contact_ptr);
}


 /*****************************************************************************/
//  Description : add to group
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactToGroupAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T* contact_ptr, uint8 group_id,MMIPB_ACCESS_CALLBACK_FUNC callback)
{

    contact_ptr->union_t.group = contact_ptr->union_t.group | 1<<group_id;
    MMIPB_SetOpcType(handle, MMIPB_OPC_MANAGE_GROUP);
   return MMIPB_UpdateContactAsyn(handle, contact_ptr, callback); 
}

 /*****************************************************************************/
//  Description : add to group
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ManageGroupContactAsynForPB(MMIPB_HANDLE_T handle, uint8 group_id)
{
     MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);
     MMIPB_CONTACT_T        contact_info = {0};
     uint8 is_in_group = 0;

    if(PNULL == applet_ptr || applet_ptr->instance.select_list == PNULL)
    {
        return MMIPB_ERROR_ERROR;
    }
    applet_ptr->instance.select_list->group_id = group_id;
    if(PNULL != applet_ptr->instance.select_list->sel_table_head)
    {
    
        is_in_group = applet_ptr->instance.select_list->sel_table_head->is_in_group;
        while((is_in_group>>4) == (is_in_group&0x0F))
        {
             
             MMIPB_DeleteSelHeadNode(handle);
             if(PNULL != applet_ptr->instance.select_list->sel_table_head)
             {
                is_in_group = applet_ptr->instance.select_list->sel_table_head->is_in_group;
             }
             else
             {
                 break;
             }
             
        }
        if(PNULL == applet_ptr->instance.select_list->sel_table_head)
        {

            return MMIPB_ERROR_NO_ENTRY;
        }
        is_in_group = applet_ptr->instance.select_list->sel_table_head->is_in_group;
        if((is_in_group>>4) != (is_in_group&0x0F))//�ó�Ա��Ҫ����
        {
            MMIPB_GetContactInfo(applet_ptr->instance.select_list->sel_table_head->entry_id, applet_ptr->instance.select_list->sel_table_head->storage_id, &contact_info);
            if(is_in_group&0x0F)//��ӵ�����
            {
                return MMIPB_AddContactToGroupAsyn(
                                                 handle,
                                                 &contact_info, 
                                                 group_id,
                                                 mmipb_app_callback);
            }
            else//�������Ƴ�
            {
                return MMIPB_RemoveContactFromGroupAsyn(
                                                 handle,
                                                 &contact_info, 
                                                 group_id,
                                                 mmipb_app_callback);
            }
        }


    }
//    else
//    {
//        //SCI_TRACE_LOW:"[mmipb]MMIPB_AddContactToGroupAsynForPB there is not contact"
//        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_5061_112_2_18_2_43_14_145,(uint8*)"");
//        return MMIPB_ERROR_NO_ENTRY;
//     }
    return MMIPB_ERROR_NO_ENTRY;
}



 /*****************************************************************************/
//  Description : add to group
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ManageUnlistGroupContact(MMIPB_HANDLE_T handle, 
                                                    uint8 group_id, 
                                                    uint16 entry_id, 
                                                    uint16 storage_id, 
                                                    BOOLEAN is_in)
{
    MMIPB_SEL_DATA sel_data = {0};
    uint8          is_in_group =0;
    if(is_in)
    {
        is_in_group = 0x01;
    }
    else
    {
        is_in_group = 0x10;
    }
    sel_data.entry_id = entry_id;
    sel_data.storage_id = storage_id;
    sel_data.is_in_group = is_in_group;
    MMIPB_CreateSelNode(handle,&sel_data);
    return MMIPB_ManageGroupContactAsynForPB(handle, group_id);   
}

 /*****************************************************************************/
//  Description : remove from group
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromGroup(MMIPB_HANDLE_T handle,MMIPB_CONTACT_T* contact_ptr,uint8 group_id )
{
    contact_ptr->union_t.group = (contact_ptr->union_t.group)&(~(uint32)(1<<group_id));
    return MMIPB_ModifyContact( contact_ptr);
}


 /*****************************************************************************/
//  Description :  remove from group
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromGroupAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T* contact_ptr, uint8 group_id,MMIPB_ACCESS_CALLBACK_FUNC callback)
{
    contact_ptr->union_t.group =(contact_ptr->union_t.group)&(~(uint32)(1<<group_id));
    MMIPB_SetOpcType(handle, MMIPB_OPC_MANAGE_GROUP);
   return MMIPB_UpdateContactAsyn(handle, contact_ptr, callback); 
}





 /*****************************************************************************/
//  Description :  
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SendGroupMsg(MMIPB_HANDLE_T handle)
{

    MMIPB_HANDLE_NODE_T handle_node = {0};
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);
    //MMIPB_CONTACT_T  contact_info = {0};
    //MMIPB_SEL_NODE_T*   node_ptr = PNULL;
    //MMIPB_BCD_NUMBER_T number_t = {0};
    //MN_NUMBER_TYPE_E number_type = 0;
    //MMIPB_STRING_T number_str = {0};
    //MMI_STRING_T  addr_str = {0};
    MMI_STRING_T   mail_str = {0};
    MMIPB_BCD_NUMBER_T bcd_num = {0};
    //wchar*         addr_buf=PNULL;
    int          i=0;
    MMISMS_DEST_ADDR_INFO_T dest_info = {0};

     if(!MMIPB_GetHandleContent(handle, &handle_node))
     {
         //SCI_TRACE_LOW:"[mmipb] MMIPB_SendGroupManage PNULL == handle_node_ptr !"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_5129_112_2_18_2_43_14_146,(uint8*)"");
         return MMIPB_ERROR_ERROR;
     }

    if(PNULL == applet_ptr || PNULL == applet_ptr->instance.select_list)
    {
        return MMIPB_ERROR_ERROR;
    }  

    MMIAPISMS_ClearDestNum();
    for(i=0;i<MMIPB_GetSelectCount(handle); i++)
    {
        wchar mailbuf[MMIPB_MAX_MAIL_LEN+1]={0};
        mail_str.wstr_ptr = mailbuf;/*lint !e733*/
        if(MMIPB_GetSelectEmail(handle, &mail_str, MMIPB_MAX_MAIL_LEN, i))
        {
            uint8 str_buf[MMIPB_MAX_MAIL_LEN+1]={0};
            MMIAPICOM_WstrToStr(mail_str.wstr_ptr, str_buf);
            MMIAPISMS_SetAddressToMessage(str_buf, SCI_STRLEN(str_buf),&dest_info.dest_list);/*lint !e64*/
        }
        else
        {
            uint8 numbuf[MMIPB_NUMBER_MAX_STRING_LEN+2]={0};
            MMIPB_GetSelectNumber(handle, &bcd_num, MMIPB_BCD_NUMBER_MAX_LEN, i);
            MMIPB_BCDNumberToUTF8(&bcd_num, MMIPB_NUMBER_MAX_STRING_LEN + 2, numbuf);
            MMIAPISMS_SetAddressToMessage(numbuf, SCI_STRLEN(numbuf),&dest_info.dest_list);/*lint !e64*/            
        }
    }
    MMIPB_DeleteAllSelNode(handle);

    MMIAPISMS_SendGroupSms(MMI_DUAL_SYS_MAX,PNULL,&dest_info);
    
    return MMIPB_ERROR_SUCCESS;

}
#endif
#ifdef MMIPB_MOST_USED_SUPPORT
 /*****************************************************************************/
//  Description : �ղ���ϵ��
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactToFavourite(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T* contact_ptr)
{
    contact_ptr->union_t.group = contact_ptr->union_t.group | 1<<PB_GROUP_FAVROUTE;
    return MMIPB_ModifyContact(contact_ptr);
}


 /*****************************************************************************/
//  Description : �ղ���ϵ��
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactToFavouriteAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T* contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback)
{
    MMIPB_AbortPB(FALSE);
    contact_ptr->union_t.group = contact_ptr->union_t.group | 1<<PB_GROUP_FAVROUTE;
    MMIPB_SetOpcType(handle, MMIPB_OPC_ADD_FAVOURITE);
    return MMIPB_UpdateContactAsyn(handle, contact_ptr, callback); 
}



 /*****************************************************************************/
//  Description : �ղ���ϵ��
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContactToFavouriteAsynForPB(MMIPB_HANDLE_T handle)
{
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);
    MMIPB_CONTACT_T        contact_info = {0}; 

    if((PNULL !=applet_ptr) 
       && PNULL != applet_ptr->instance.select_list    
      && (PNULL != applet_ptr->instance.select_list->sel_table_head))
    {
    
        MMIPB_GetContactInfo(applet_ptr->instance.select_list->sel_table_head->entry_id, 
            applet_ptr->instance.select_list->sel_table_head->storage_id, &contact_info);
        return MMIPB_AddContactToFavouriteAsyn(
                                         handle,
                                         &contact_info, 
                                         mmipb_app_callback);
    }
    else
    {
        //SCI_TRACE_LOW:"[mmipb]MMIPB_AddContactToFavouriteAsynForPB there is not contact to delete"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_5221_112_2_18_2_43_15_149,(uint8*)"");
        return MMIPB_ERROR_NO_ENTRY;
    }
}


/*****************************************************************************/
//  Description : �첽add��������ѡ���б��еļ�¼���ڷǶ�ѡ״̬
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddUnlistContactFromFavourite(MMIPB_HANDLE_T handle, uint16 entry_id, uint16 storage_id)
{
    MMIPB_SEL_DATA sel_data = {0};
    sel_data.is_in_group =0;
    sel_data.entry_id = entry_id;
    sel_data.storage_id = storage_id;  
    MMIPB_CreateSelNode(handle,&sel_data);
    return MMIPB_AddContactToFavouriteAsynForPB(handle);
}
 /*****************************************************************************/
//  Description : ȡ���ղ���ϵ��
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromFavourite(MMI_HANDLE_T applet_handle,MMIPB_CONTACT_T* contact_ptr)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    //MMIPB_APPLET_T *applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
    if(PNULL == contact_ptr)
    {
        return MMIPB_ERROR_NO_ENTRY;
    }
    contact_ptr->union_t.group = (contact_ptr->union_t.group)&(~(1<<PB_GROUP_FAVROUTE));
    result = MMIPB_ModifyContact(contact_ptr);
   /* if(applet_ptr != PNULL && applet_ptr->instance.update_callback != PNULL)
    {
        applet_ptr->instance.update_callback(applet_handle, MMIPB_OPERATE_SUCCESS, error,MMIPB_OPC_REMOVE_FAVOURITE,1);
    }*/
    return result;
}




 /*****************************************************************************/
//  Description : ȡ���ղ���ϵ��
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromFavouriteAsyn(MMIPB_HANDLE_T handle,MMIPB_CONTACT_T* contact_ptr,MMIPB_ACCESS_CALLBACK_FUNC callback)
{
    MMIPB_AbortPB(FALSE);
    contact_ptr->union_t.group = (contact_ptr->union_t.group)&(~(1<<PB_GROUP_FAVROUTE));
    MMIPB_SetOpcType(handle, MMIPB_OPC_REMOVE_FAVOURITE);
    return MMIPB_UpdateContactAsyn(handle, contact_ptr, callback); 
}



 /*****************************************************************************/
//  Description : ȡ���ղ���ϵ��
//  Author: 
//  Note: maryxiao
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveContactFromFavouriteAsynForPB(MMIPB_HANDLE_T handle)
{    
    MMIPB_CONTACT_T       contact_info ={0};
    MMIPB_APPLET_T *applet_ptr = GetPBApplet(handle);

    if(applet_ptr != PNULL
       && applet_ptr->instance.select_list != PNULL 
       && PNULL != applet_ptr->instance.select_list->sel_table_head)
    {
    
        MMIPB_GetContactInfo(applet_ptr->instance.select_list->sel_table_head->entry_id, 
            applet_ptr->instance.select_list->sel_table_head->storage_id, 
            &contact_info);
        return MMIPB_RemoveContactFromFavouriteAsyn(
                                         handle,
                                         &contact_info, 
                                         mmipb_app_callback);
    }
    else
    {
        //SCI_TRACE_LOW:"[mmipb]MMIPB_AddContactToFavouriteAsynForPB there is not contact to delete"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_5309_112_2_18_2_43_15_152,(uint8*)"");
        return MMIPB_ERROR_NO_ENTRY;
    }
}
/*****************************************************************************/
//  Description : �첽ɾ����������ѡ���б��еļ�¼���ڷǶ�ѡ״̬
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_RemoveUnlistContactFromFavourite(MMIPB_HANDLE_T handle, uint16 entry_id, uint16 storage_id)
{
    MMIPB_SEL_DATA sel_data = {0};
    sel_data.is_in_group =0;
    sel_data.entry_id = entry_id;
    sel_data.storage_id = storage_id;  
    MMIPB_CreateSelNode(handle,&sel_data);
    return MMIPB_RemoveContactFromFavouriteAsynForPB(handle);
}
#endif
 /*****************************************************************************/
//  Description : delete all pb info 
//  Author: 
//  Note: for dm cmcc 
/*****************************************************************************/
PUBLIC void MMIPB_DeleteAllPBInfo(void)
{
    uint16 i  = 0;
    MMIPB_ACCESS_POINT_T access_point = {0};
    //delete all phone contact
    access_point.contact.storage_id =MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT); 
    for(i = 1; i <= MMIPB_NV_MAX_RECORD_NUM; i ++)
    {
        access_point.contact.contact_id = i;
        MMIPB_Delete(MMIPB_ACCESS_CONTACT, access_point);
    }
#ifdef MMIPB_MOST_USED_SUPPORT 
    //delete all mostused contact
    access_point.contact.storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE,MMIPB_MOSTUSED_PHONE_CONTACT);
    for(i = 1; i < MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS; i ++)
    {
        access_point.contact.contact_id = i;
        MMIPB_Delete(MMIPB_ACCESS_CONTACT, access_point);
    }
#endif
    MMK_PBTriggerEvent(MMIEVENT_PB_DELETE);
}




#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : Reset Group info and write to NV
//  Global resource dependence : s_mmipb_group_info
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_ResetGroupInfo(void)
 {
     int i = 0;
     MMIPB_ACCESS_POINT_T      access_point = {0};
     MMIPB_GROUP_T             group_info ={0};
     for(i=0; i< MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM; i++)
     {
        access_point.group_id = i;
       SCI_MEMSET(&group_info.group_name, 0X00, sizeof(MMIPB_NAME_T));
       MMIPB_Write(MMIPB_ACCESS_GROUP, access_point, sizeof(MMIPB_GROUP_T), &group_info);
    }
 }
#endif

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
    MMI_STRING_T         *call_name_ptr //IN:contact_name 
)
{
	 //MN_NUMBER_TYPE_E  number_type = 0;//
     CC_CALL_SIM_TYPE_E call_sim_type = dual_sys + CC_SIM1_CALL;
     uint32 call_type = CC_CALL_NORMAL_CALL;

    if(PNULL == call_number_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]MakeNumberCallByBCDNumber number_str_ptr == PNULL !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_5389_112_2_18_2_43_15_153,(uint8*)"");
        return;
    }
	
    switch (interface_type)
    {
    case MMIPB_CALL_TYPE_MAKECALL_AUDIO://�����绰
        call_type = CC_CALL_NORMAL_CALL;
        break;
        
#ifdef VT_SUPPORT
    case MMIPB_CALL_TYPE_MAKECALL_VIDEO://���ӵ绰
        call_type = CC_CALL_VIDEO_CALL; 
        break;
#endif

#ifdef MMI_IP_CALL_SUPPORT
    case MMIPB_CALL_TYPE_MAKEIPCALL:
        call_type = CC_CALL_IP_CALL; 
        break;
#endif
        
    default:
        break;
    }

    MMIAPICC_MakeCall(
        dual_sys,
        call_number_ptr,
        (uint8)strlen(call_number_ptr),/*lint !e64*/
        PNULL,
        call_name_ptr,
        call_sim_type,
        call_type,
        PNULL
        ); 
}

/*****************************************************************************/
//  Description : get storage free count
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetStorageFreeSpace(uint16 storage_id, 
                                               uint16 *free_num_ptr)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_NO_SPACE;
    uint16        space = MMIPB_GetContactStorageSpace(storage_id);

    if(MMIPB_IsSimRecord(storage_id)
        &&(!MMIAPIPHONE_GetSimExistedStatus(MMIPB_GetDualSysByStorageId(storage_id)))
        )
    {
        //sim contact not exist
        result = MMIPB_ERROR_SIM_ERROR;
    }
    else if(MMIPB_IsPhoneContact(storage_id)
           ||MMIPB_IsSimRecord(storage_id)
           )
    {
        if(MMIPB_GetContactCount(storage_id) < space)
        {
            result = MMIPB_ERROR_SUCCESS;
            if(free_num_ptr != PNULL)
            {
                *free_num_ptr = space - MMIPB_GetContactCount(storage_id);
            }
        }
    }
    else
    {
        result = MMIPB_ERROR_SUCCESS;
        if(PNULL != free_num_ptr)
        {
            *free_num_ptr = 1;
        }
    }
   return result;
}

/*****************************************************************************/
//  Description :get phone nunber by bcd_number
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetPhoneNumberByBcdNumber(
    MMIPB_BCD_NUMBER_T  bcd_number,
    uint8* tele_num,
    uint16 len)
{
    uint16 tele_len = 0;
    MN_NUMBER_TYPE_E  number_type = 0;
    
    if(MMIPB_IsValidPhoneNumber(bcd_number.npi_ton, bcd_number.number_len))
    {
        number_type = MMIPB_GetNumberTypeFromUint8(bcd_number.npi_ton);

    
        // ������ת��Ϊ�ַ���
        tele_len = MMIAPICOM_GenDispNumber(
                                        (number_type == MN_NUM_TYPE_ALPHANUMERIC ? MN_NUM_TYPE_UNKNOW : number_type),//MMIPB_GetNumberTypeFromUint8(bcd_number_ptr->npi_ton),
                                        bcd_number.number_len,
                                        bcd_number.number,
                                        tele_num,
                                        MIN(len,MMIPB_NUMBER_MAX_STRING_LEN+2)
                                        );
    }
    return tele_len;
}
/*****************************************************************************/
//  Description :make dial by bcd_number
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/

PUBLIC void MMIPB_CallBCDNumber(
    MN_DUAL_SYS_E dual_sys,
    MMIPB_CALL_TYPE_E call_type,
    MMIPB_CONTACT_T* contact_ptr,
    uint16 num_index
    )
{
    
    MMI_STRING_T    name_str = {0};
    wchar name_buf[MMIPB_NAME_MAX_LEN] = {0};
    uint8 tele_num[MMIPB_NUMBER_MAX_STRING_LEN + 2] = {0};
    uint8 tele_len = 0;
    
    MMIPB_BCD_NUMBER_T  bcd_number_t = {0};
    //MMIPB_APPLET_T *applet_ptr = PNULL;
    
    //SCI_TRACE_LOW:"[MMIPB]MMIPB_CallBCDNumber !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_5530_112_2_18_2_43_15_154,(uint8*)"");

    if(PNULL == contact_ptr)
    {
        return;
    }
    if(MMIPB_IsSDNContact(contact_ptr->storage_id))
    {
        dual_sys = MMIPB_GetDualSysByStorageId(contact_ptr->storage_id);
    }
	if(num_index< MMIPB_MAX_NV_PHONE_NUM)
	{
	    bcd_number_t = contact_ptr->number[num_index];
	}

    tele_len = MMIPB_GetPhoneNumberByBcdNumber(bcd_number_t, tele_num, MMIPB_NUMBER_MAX_STRING_LEN + 2);
    
    name_str.wstr_ptr = name_buf;

    name_str.wstr_len = MIN(contact_ptr->name.wstr_len, MMIPB_NAME_MAX_LEN);
    MMIAPICOM_Wstrncpy(name_str.wstr_ptr, contact_ptr->name.wstr,name_str.wstr_len);
#ifdef BT_DIALER_SUPPORT
        if (MMIPB_CALL_TYPE_MAKEBTCALL == call_type)
        {
            MMIAPICC_MakeBtCall(tele_num, tele_len);
        }
        else
#endif
            MMIPB_CallNumber(dual_sys, call_type,tele_num,&name_str);
    
}

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : �жϱ����ļ�����ռ��Ƿ���
// Global resource dependence :
// Author:xiaohua.liu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckDiskSpaceForSaveVCard(MMIFILE_DEVICE_E file_dev)
{
    BOOLEAN    bRet = TRUE;
    uint32     space = 0;
    if(MMI_DEVICE_NUM > file_dev)
    {
        space = MMI_GetFreeSpace(file_dev);
        if ((VCARD_MIN_NEED_SPACE >> 10) > space)
        {
            bRet = FALSE;
        }
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}

/*****************************************************************************/
//  Description : Read vcard
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadVcardToContact(
    wchar  *file_name,//IN
    MMIPB_CONTACT_T *contact_ptr//OUT
    )
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    MMIFILE_ERROR_E file_ret = 0;
    MMIFILE_HANDLE   file_handle = 0;
    MMIPB_ACCESS_POINT_T access_point = {0};

    file_ret = MMIAPIFMM_CreateFileAsyn(file_name, 
        SFS_MODE_READ|SFS_MODE_OPEN_EXISTING,
        PNULL,
        PNULL,
        0,
        0,
        &file_handle);
    if(file_ret == SFS_ERROR_NONE)
    {
        access_point.file_handle = file_handle;
        ret= MMIPB_Read(MMIPB_ACCESS_VCARD, access_point, sizeof(MMIPB_CONTACT_T), contact_ptr);  
        MMIFILE_CloseFile(file_handle);
    }
    else
    {
        ret = MMIPB_GetErrorType(file_ret);
    }
    return ret;
}
/*****************************************************************************/
//  Description : send vcard by sms or bt and so pn
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note: 
/***************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SendVcard(
    MMIPB_SEND_TYPE_E send_type,
    MMIPB_CONTACT_T *contact_ptr
    )
{
    MMIPB_ERROR_E send_error = MMIPB_ERROR_SUCCESS;
    MMIFILE_ERROR_E file_ret = 0;
    MMIFILE_HANDLE  file_handle = 0;
    MMIFILE_FILE_INFO_T      *file_info_ptr = PNULL;
    MMIFMM_BT_SENDFILE_INFO_T bt_file_info = {0};
    MMIPB_ACCESS_POINT_T      access_point = {0};
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_UDISK;
    //wchar                     *full_path_name_ptr = PNULL;
    if(MMIPB_SEND_BY_SMS != send_type)
    {
        //send by Bluetooth
        if (MMIAPIUDISK_UdiskIsRun())
        {
            //udisk is run
            send_error = MMIPB_ERROR_UDISK_ISRUN;
        }
        else
        {
            file_dev = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);
            
            // �жϱ����ļ�����ռ��Ƿ���
            if(!CheckDiskSpaceForSaveVCard(file_dev))
            {
                if(!MMIAPIFMM_GetDeviceTypeStatus((file_dev)))
                {
                    send_error = MMIPB_ERROR_NO_DEV;
                }
                else
                {
                    send_error = MMIPB_ERROR_NO_SPACE;
                }
            }
            else
            {
                file_info_ptr = SCI_ALLOCA(sizeof(MMIFILE_FILE_INFO_T));
                if(file_info_ptr == PNULL)
                {
                    return MMIPB_ERROR_NO_SPACE;
                }
                SCI_MEMSET(file_info_ptr, 0x00, sizeof(MMIFILE_FILE_INFO_T));
                SCI_MEMSET((void *)bt_file_info.filepath_name, 0, ((MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(uint16)));
                MMIAPIVCARD_GetVcardFileName(contact_ptr, 
                    MMIPB_GET_CONTACT_TYPE(contact_ptr->storage_id), 
                    file_info_ptr->file_name);


                MMIAPICOM_CombineDefaultFullPathByFilename(file_dev, 
                    file_info_ptr->file_name, 
                    MMIAPICOM_Wstrlen(file_info_ptr->file_name), 
                    (wchar *)bt_file_info.filepath_name, 
                    MMIFILE_FULL_PATH_MAX_LEN);
                
                MMIAPICOM_GetNoExistFileName(bt_file_info.filepath_name, MMIAPICOM_Wstrlen(bt_file_info.filepath_name));        
                file_ret = MMIAPIFMM_CreateFileAsyn(bt_file_info.filepath_name, 
                    SFS_MODE_WRITE|SFS_MODE_CREATE_NEW,
                    PNULL,
                    PNULL,
                    0,
                    0,
                    &file_handle); 
                if(SFS_ERROR_NONE == file_ret)
                {
                    access_point.file_handle = file_handle;
                    send_error= MMIPB_Write(MMIPB_ACCESS_VCARD, access_point, sizeof(MMIPB_CONTACT_T), contact_ptr);
                    MMIFILE_CloseFile(file_handle);
                }
                else
                {
                    send_error = MMIPB_GetErrorType(file_ret);
                }
                if(MMIPB_ERROR_SUCCESS == send_error)
                {
                    //save success 
                    bt_file_info.filepath_len = MMIAPICOM_Wstrlen(bt_file_info.filepath_name);
                    SCI_MEMSET(file_info_ptr, 0x00, sizeof(MMIFILE_FILE_INFO_T));
                    if(MMIPB_SEND_BY_MMS == send_type)
                    {
                        MMIAPIFMM_GetFileInfoFormFullPath(bt_file_info.filepath_name, bt_file_info.filepath_len, file_info_ptr);
                        MMIAPIMMS_EditMMSFromFile(file_info_ptr, MMIFMM_FILE_TYPE_VCARD);                            
                    }
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
                    else if (MMIPB_SEND_BY_EMAIL== send_type)
                    {
                        if(MMIAPIFMM_GetFileInfoFormFullPath(bt_file_info.filepath_name, bt_file_info.filepath_len, file_info_ptr))  //should check the size
                        {
                            MMIAPIEMAIL_EditEMAILFromFile(file_info_ptr);
                        }
                    }
#endif
                    else
                    {
                        MMIAPIFMM_GetFileInfo(bt_file_info.filepath_name,
                            bt_file_info.filepath_len,
                            &file_info_ptr->file_size,
                            NULL, NULL
                            );
                        bt_file_info.file_size = file_info_ptr->file_size;
                        bt_file_info.is_temp_file = TRUE;
                        MMIAPIBT_SendMultiFile(&bt_file_info, 1);
                        //MMIAPIBT_SendFile(full_file_name,full_file_len, file_info.file_size,TRUE); 
                    }
                }//save success 
                SCI_FREE(file_info_ptr);

            }//space enough
        }//udisk not run    
    }                        
    else //if(MMIPB_SEND_BY_SMS == send_type) //������/*lint !e774*/
    {
        uint32 vcard_len = 0; 
        
        MMIAPISMS_ClearDestNum();
        
        MMIAPIVCARD_BuildInit();
        MMIAPIVCARD_BuildVCard(contact_ptr);
        
        vcard_len = strlen((char*)MMIAPIVCARD_GetBuildData());
        if((MMIPB_SMS_VCARD_MAX_8BIT_SPLIT_LEN*MMISMS_SPLIT_MAX_NUM) >= vcard_len)
        {
            MMIAPISMS_EnterSendOptionMenu((ADD_DATA)MMIVC_SMS_SEND_VCARD);/*lint !e746 !e718*/
        }
        else
        {
            //data too large            
            MMIAPIVCARD_BuildFree();  
            send_error = MMIPB_ERROR_SMS_TOO_LARGE;
        }
    }
    return send_error;
}
#endif

#if (defined IKEYBACKUP_SUPPORT) || (defined BT_PBAP_SUPPORT)
/*****************************************************************************/
//  Description : �ж��Ƿ�Ϊ�ظ���contact
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
LOCAL BOOLEAN is_same_contact(MMIPB_CONTACT_T *contact_ptr)
{
    uint16       total_count = MMIPB_GetAllNormalContactCount();
    uint32       i = 0;
    uint32       bcd_num = 0;
    MMI_STRING_T str_1 = {0};
    MMI_STRING_T str_2 = {0};
    wchar        num_wchar1[MMIPB_MAX_STRING_LEN] = {0};
    wchar        num_wchar2[MMIPB_MAX_STRING_LEN] = {0};
    MMIPB_NAME_NUMBER_MAIL_T contact_info = {0};
    MMIPB_CONTACT_BASE_INFO_T contact_node_info = {0};

    if(PNULL == contact_ptr)
    {
        return FALSE;
    }

    SCI_TRACE_LOW("[mmipb]:entry is_same_contact\n");

    for(i=0; i<total_count; i++)
    {
        MMIPB_GetContactNameNumberByIndex(&contact_info, i);
        contact_node_info = MMIPB_GetContactBaseInfo(i);
        
        if(MMIPB_IsPhoneContact(contact_node_info.storage_id))
        {
            str_1.wstr_len = contact_info.name.wstr_len;
            str_1.wstr_ptr = contact_info.name.wstr;
            str_2.wstr_len = contact_ptr->name.wstr_len;
            str_2.wstr_ptr = contact_ptr->name.wstr;
            
            //�ж������Ƿ���ͬ
            if(str_1.wstr_len == str_2.wstr_len
                && 0 == MMIAPICOM_Wstrncmp(str_1.wstr_ptr, str_2.wstr_ptr, str_2.wstr_len))
            {
                //Ϊ�ַ���ָ���洢���������Ӱ��name
                str_1.wstr_ptr = num_wchar1;   
                str_2.wstr_ptr = num_wchar2;

                //�ж����еĺ����Ƿ���ͬ
                for(bcd_num = 0; bcd_num < MMIPB_MAX_NV_PHONE_NUM; bcd_num++)
                {                   
                    if(contact_info.number_t[bcd_num].number_len != contact_ptr->number[bcd_num].number_len)
                    {
                        break;
                    }
                    str_1.wstr_len = 0;
                    str_2.wstr_len = 0;
                    MMIAPICOM_BcdToWchar(str_1.wstr_ptr, &str_1.wstr_len, MMIPB_MAX_STRING_LEN, &(contact_info.number_t[bcd_num]));
                    MMIAPICOM_BcdToWchar(str_2.wstr_ptr, &str_2.wstr_len, MMIPB_MAX_STRING_LEN, &(contact_ptr->number[bcd_num])); 
                    if(str_1.wstr_len != str_2.wstr_len
                       || 0 != MMIAPICOM_Wstrncmp(str_1.wstr_ptr, str_2.wstr_ptr, str_2.wstr_len))
                    {
                        break;
                    }                       
                }
                
                if(bcd_num >= MMIPB_MAX_NV_PHONE_NUM)
                {
                    SCI_TRACE_LOW("[mmipb] is_same_contact same\n");
                    return TRUE;
                }
            }   
        }
    }

    return FALSE;
}
#endif

#ifdef IKEYBACKUP_SUPPORT
/*****************************************************************************/
//  Description : PB���ݺ���
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
LOCAL void ikeybackupcallback(
                               MMIIKB_BACKUP_CALLBACK_PARAM_IN_T *param_in_ptr,
                               MMIIKB_BACKUP_CALLBACK_PARAM_OUT_T *param_out_ptr
                              )
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIPB_CONTACT_BASE_INFO_T base_info = {0};
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    uint16 total_count = MMIPB_GetContactCount(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT));
    //uint16 i = 0;

    if (PNULL == param_in_ptr
        || PNULL == param_out_ptr)
    {        
        //SCI_TRACE_LOW:"[MMIPB] ikeybackupcallback invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_5723_112_2_18_2_43_16_155,(uint8*)"");
        return;
    }
    
    if (MMIIKB_TYPE_PHONEBOOK!= param_in_ptr->type
        || MMIIKB_OPERATE_TYPE_BUF != param_in_ptr->op_type)
    {
        //not pb backup request
        //SCI_TRACE_LOW:"[MMIPB] ikeybackupcallback type error !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_5731_112_2_18_2_43_16_156,(uint8*)"");
        error_code = MMIIKB_STATUS_INVALID_PARAM;
    }
    else
    {
        //pb backup request

        //set backup parameter
        SCI_TRACE_LOW("[MMIPB] ikeybackupcallback param_in_ptr->number %d", param_in_ptr->number);
        param_out_ptr->type = param_in_ptr->type;
        param_out_ptr->op_type = param_in_ptr->op_type;
        if(param_in_ptr->number >= total_count)
        {
            error_code = MMIIKB_STATUS_INVALID_PARAM;
        }
        else
        {
            base_info = MMIPB_GetPhoneContactBaseInfo(param_in_ptr->number);
            contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
            if(PNULL != contact_ptr)
            {
                param_out_ptr->is_need_free=TRUE;
                SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
                if(MMIPB_ERROR_SUCCESS == MMIPB_GetContactInfo(base_info.contact_id, base_info.storage_id, contact_ptr))
                {
                    param_out_ptr->buf_ptr=(void*)contact_ptr;                    
                    param_out_ptr->len=sizeof(MMIPB_CONTACT_T);
                    param_out_ptr->number=1;
                    param_out_ptr->is_finished=FALSE;
                    error_code = MMIIKB_STATUS_SUCCESS;
                }
            }

            if((param_in_ptr->number+1)== total_count)
            {
                param_out_ptr->is_finished=TRUE;
            }            
        }
    }
    
    param_out_ptr->error_code = error_code;

    return;  
}

/*****************************************************************************/
//  Description : ��ñ�������
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
LOCAL void gettotalvount_Callback(
    MMIIKB_GET_TOTAL_OUT_T *param_out_ptr
)
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_SUCCESS;

    param_out_ptr->total = MMIPB_GetContactCount(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT));   
    param_out_ptr->total_type = MMIIKB_TOTAL_TYPE_NUMBER;
    
    param_out_ptr->error_code = error_code;
}


/*****************************************************************************/
//  Description : ��ԭʱ�ж��Ƿ�Ϊ�ظ���contact
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
// LOCAL BOOLEAN is_same_contact(MMIPB_CONTACT_T *contact_ptr)
// {
//     uint16 total_count = MMIPB_GetAllNormalContactCount();
//     int i =0;
//     MMIPB_NAME_NUMBER_MAIL_T contact_info = {0};
//     MMIPB_CONTACT_BASE_INFO_T contact_node_info = {0};
// 
//     if(PNULL == contact_ptr)
//     {
//         return FALSE;
//     }
//     for(i=0; i<total_count; i++)
//     {
//       SCI_MEMSET(&contact_info, 0x00 ,sizeof(MMIPB_NAME_NUMBER_MAIL_T));
//       MMIPB_GetContactNameNumberByIndex(&contact_info, i);
//       contact_node_info = MMIPB_GetContactBaseInfo(i);
//       if(MMIPB_IsPhoneContact(contact_node_info.storage_id)&& !memcmp(&contact_info.name, &(contact_ptr->name), sizeof(MMIPB_NAME_T))
//           && !memcmp(contact_info.number_t, (contact_ptr->number), sizeof(MMIPB_BCD_NUMBER_T)*MMIPB_MAX_NV_PHONE_NUM))
//       {
//           SCI_TRACE_LOW("[mmipb] is_same_contact same");
//           return TRUE;
//       }
// 
//     }
// 
//     return FALSE;
// }
/*****************************************************************************/
//  Description : ��ԭ����
//  Global resource dependence : none
//  Author: 
//  Note: 
/***************************************************************************/
LOCAL void ikeyrestore_callback(
                                MMIIKB_RESTORE_CALLBACK_PARAM_IN_T *param_in_ptr,
                                MMIIKB_RESTORE_CALLBACK_PARAM_OUT_T *param_out_ptr
                               )
{
    MMIIKB_ERROR_E error_code = MMIIKB_STATUS_INVALID_PARAM;
    MMIPB_CONTACT_T *contact_ptr = PNULL;
    
    SCI_TRACE_LOW("[MMIPB] enter  ikeyrestore_callback!");
    
    if (PNULL == param_in_ptr
        || PNULL == param_out_ptr)
    {
        SCI_TRACE_LOW("[MMIPB] ikeyrestore_callback invalid param !");
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_5807_112_2_18_2_43_16_157,(uint8*)"");
        return;
    }
    

    param_out_ptr->type=param_in_ptr->type;
    param_out_ptr->op_type=param_in_ptr->op_type;
    
    if (MMIIKB_TYPE_PHONEBOOK!= param_in_ptr->type
        || MMIIKB_OPERATE_TYPE_BUF != param_in_ptr->op_type
        || 1 != param_in_ptr->number
        || PNULL == param_in_ptr->buf_ptr
        )
    {
        error_code = MMIIKB_STATUS_INVALID_PARAM;
        SCI_TRACE_LOW("[MMIPB] ikeyrestore_callback param_in_ptr->type %d, param_in_ptr->op_type %d, param_in_ptr->number %d, param_in_ptr->buf_ptr %d", param_in_ptr->type,param_in_ptr->op_type, param_in_ptr->number, param_in_ptr->buf_ptr);
    }
    else
    {
        
        contact_ptr = (MMIPB_CONTACT_T *)param_in_ptr->buf_ptr;
        if(is_same_contact(contact_ptr))
        {
            error_code = MMIIKB_STATUS_SUCCESS;
        }
        else if(PNULL != contact_ptr)
        {
            contact_ptr->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
            contact_ptr->contact_id = MMIPB_GetFreeContactId(contact_ptr->storage_id);
            if(MMIPB_ERROR_SUCCESS == MMIPB_GetStorageFreeSpace(contact_ptr->storage_id, PNULL))
            {
                if(MMIPB_ERROR_SUCCESS == MMIPB_UpdateContact(contact_ptr))
                {
                    error_code = MMIIKB_STATUS_SUCCESS;
                }
            }
            else
            {
                //pb��¼��
                error_code = MMIIKB_STATUS_PB_FULL;
            }
        }        
    }
    if(MMIIKB_STATUS_SUCCESS == error_code)
    {
        param_out_ptr->suc_cnt = 1;
        param_out_ptr->err_cnt = 0;
    }
    else
    {
        param_out_ptr->suc_cnt = 0;
        param_out_ptr->err_cnt = 1;
    }

    SCI_TRACE_LOW("[MMIPB] ikeyrestore_callback error_code %d !", error_code);
    param_out_ptr->error_code = error_code;
    return;

}

/*****************************************************************************/
//  Description : һ����ԭע�ắ��
//  Global resource dependence : none
//  Author:
//  Note: 
/***************************************************************************/
LOCAL BOOLEAN ikeybackup_register(void)
{
    BOOLEAN is_ret = FALSE;
    MMIIKB_TYPE_PROCESS_PARAM_T ikb_param = {0};
    
    ikb_param.type = MMIIKB_TYPE_PHONEBOOK;
    ikb_param.op_type = MMIIKB_OPERATE_TYPE_BUF;
    ikb_param.backup_call_back = ikeybackupcallback;
    ikb_param.is_backup_asyn = FALSE;
    ikb_param.restore_call_back = ikeyrestore_callback;
    ikb_param.is_restore_asyn = FALSE;
    ikb_param.get_total_call_back = gettotalvount_Callback;
    is_ret = MMIAPIIKB_RegTypeFunc(&ikb_param);
    
    return is_ret;
}

#endif
/*****************************************************************************/
//  Description :��ȡMSISD�Ĕ�Ŀ
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetMSISDNum(void)
{
    MMIPB_STORAGE_INFO_T            msisdn_info  = {0};	
    MN_DUAL_SYS_E                   start_dual_sys = MN_DUAL_SYS_1;
    MN_DUAL_SYS_E                   end_dual_sys =MMI_DUAL_SYS_MAX-1;/*lint !e778*/
    uint16                          num = 0;
    for( ; start_dual_sys <= end_dual_sys; start_dual_sys++)
    {
         if(MMIAPIPHONE_GetSimExistedStatus(start_dual_sys))
         {
			SCI_MEMSET(&msisdn_info, 0x00, sizeof(MMIPB_STORAGE_INFO_T));
            msisdn_info  = MMIPB_GetContactStorage(start_dual_sys, PHONEBOOK_MSISDN_STORAGE); 
            num += msisdn_info.used_record_num;
         }
    }
	return num;
}


/*****************************************************************************/
//  Description :��ȡMSISDNumber
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetMSISDNumberArray(MMIPB_BCD_NUMBER_T *number, int16 max_num)
{
    MMIPB_STORAGE_INFO_T            msisdn_info  = {0};	
    MN_DUAL_SYS_E                   start_dual_sys = MN_DUAL_SYS_1;
    MN_DUAL_SYS_E                   end_dual_sys =MMI_DUAL_SYS_MAX-1;
    int16                           i=0;
    int16                           j=0;
    uint16                          entry_id = 0;
    uint16                          storage_id = 0;
    MMIPB_CONTACT_T* contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] MMIAPIPB_GetMSISDNumber, PNULL == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_5921_112_2_18_2_43_16_158,(uint8*)"");
        return;
    }
    SCI_MEMSET(contact_ptr, 0X00, sizeof(MMIPB_CONTACT_T));
    

    for( ; start_dual_sys <= end_dual_sys; start_dual_sys++)
    {
         if(MMIAPIPHONE_GetSimExistedStatus(start_dual_sys))
         {
			SCI_MEMSET(&msisdn_info, 0x00, sizeof(MMIPB_STORAGE_INFO_T));
            msisdn_info  = MMIPB_GetContactStorage(start_dual_sys, PHONEBOOK_MSISDN_STORAGE);    
            for (i = 1; i <= msisdn_info.max_record_num ; i++)
            {
                storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + start_dual_sys, PHONEBOOK_MSISDN_STORAGE);
                entry_id = i;
                MMIPB_GetContactInfo(entry_id, storage_id, contact_ptr);
                if(MMIPB_IsValidPhoneNumber(contact_ptr->number[0].npi_ton, contact_ptr->number[0].number_len))
                {
                
                    if(j>max_num)
                    {
                        //SCI_TRACE_LOW:"[mmipb] MMIPB_GetMSISDNumber j %d, max_num %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_5943_112_2_18_2_43_16_159,(uint8*)"dd", j, max_num);
                        break;
                    }
                    else
                    {
                        SCI_MEMCPY(&number[j],&contact_ptr->number[0], sizeof(MMIPB_BCD_NUMBER_T));
                        j++;
                    }
                }
            }
         }
    }
    SCI_FREE(contact_ptr);
}


/*****************************************************************************/
//  Description :��ȡָ��SIM��MSISDNumber
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GetMSISDNumberArrayBySim(MMIPB_BCD_NUMBER_T *number,MN_DUAL_SYS_E dual_sys, int16 max_num)
{
    MMIPB_STORAGE_INFO_T            msisdn_info  = {0};	
    int16                           i=0;
    int16                           j=0;
    uint16                          entry_id = 0;
    uint16                          storage_id = 0;
    MMIPB_CONTACT_T* contact_ptr = SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == contact_ptr)
    {
        //SCI_TRACE_LOW:"[mmipb] MMIAPIPB_GetMSISDNumber, PNULL == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_5973_112_2_18_2_43_16_160,(uint8*)"");
        return;
    }
    SCI_MEMSET(contact_ptr, 0X00, sizeof(MMIPB_CONTACT_T));
    

     if(MMIAPIPHONE_GetSimExistedStatus(dual_sys))
     {
		SCI_MEMSET(&msisdn_info, 0x00, sizeof(MMIPB_STORAGE_INFO_T));
        msisdn_info  = MMIPB_GetContactStorage(dual_sys, PHONEBOOK_MSISDN_STORAGE);    
        for (i = 1; i <= msisdn_info.max_record_num ; i++)
        {
            storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + dual_sys, PHONEBOOK_MSISDN_STORAGE);
            entry_id = i;
            MMIPB_GetContactInfo(entry_id, storage_id, contact_ptr);
            if(MMIPB_IsValidPhoneNumber(contact_ptr->number[0].npi_ton, contact_ptr->number[0].number_len))
            {
            
                if(j>max_num)
                {
                    //SCI_TRACE_LOW:"[mmipb] MMIPB_GetMSISDNumberBySim j %d, max_num %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_APP_5993_112_2_18_2_43_16_161,(uint8*)"dd", j, max_num);
                    break;
                }
                else
                {
                
                    SCI_MEMCPY(&number[j],&contact_ptr->number[0], sizeof(MMIPB_BCD_NUMBER_T));
                    j++;
                }
            }
        }
     }
    SCI_FREE(contact_ptr);
}
/*****************************************************************************/
//  Description : Get Contact Number
//  Global resource dependence : none
//  Author:
//  Note: 
/***************************************************************************/
PUBLIC uint16 MMIPB_GetContactNumberNum(
    MMIPB_CONTACT_T* contact_ptr, 
    uint16* index_ptr)
{
    int i =0;
    int j =0;

    for(i=0; i<MMIPB_MAX_NV_PHONE_NUM; i++)
    {
    
        if(MMIPB_IsValidPhoneNumber(
            contact_ptr->number[i].npi_ton, 
            contact_ptr->number[i].number_len)
            )
        {

            index_ptr[j] = i;
            j++;

        }
    }
    return j;
}

/*****************************************************************************/
//  Description : Get pb status whether it can add contact 
//  Global resource dependence : none
//  Author:xiaohua.liu
//  Note: 
/***************************************************************************/
PUBLIC BOOLEAN MMIPB_PbIsFull(void)
{
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
    
    uint16 storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
    if(MMIPB_GetContactCount(storage_id) < MMIPB_GetContactStorageSpace(storage_id))

    {
       return FALSE;
    }
    for(dual_sys = MN_DUAL_SYS_1;dual_sys<MMI_DUAL_SYS_MAX;dual_sys++)
    {
        if(MMIAPIPHONE_GetSimExistedStatus(dual_sys))
        {
            if(MMIPB_GetSimContactCount(dual_sys)< MMIPB_GetMaxContactNumOfSim(dual_sys))
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}


#ifdef MCARE_V31_SUPPORT

PUBLIC uint16 MCARE_MMIPB_GetUsedRecordNumByStorage(uint16 storage_id)
{
     MMIPB_STORAGE_INFO_T storage_info = MMIPB_GetContactStorageInfo(storage_id);
     return storage_info.used_record_num;
}
PUBLIC uint16 MCARE_MMIPB_GetUsedContactNumOfNV(void)
{
    MMIPB_STORAGE_INFO_T storage_info =  MMIPB_GetContactStorageInfo(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT));

    return storage_info.used_record_num;
}
PUBLIC uint16 MCARE_MMIPB_GetMaxRecordNumByStorage(uint16 storage_id)
{
     MMIPB_STORAGE_INFO_T storage_info = MMIPB_GetContactStorageInfo(storage_id);
     return storage_info.max_record_num;
}
#endif//#ifdef MCARE_V31_SUPPORT

/*****************************************************************************/
//  Description : THE INTERFACE for autotest only
//  Global resource dependence : none
//  Author:maryxiao
//  Note: define the diff name and diff telphone number 
/***************************************************************************/
PUBLIC BOOLEAN MMIPB_FillPhoneContactFull(void)
{
    uint16              used_num = 0;
    uint16              max_num = 0;
    uint16              add_num = 0;
    int i =0;
    MMIPB_CONTACT_T      contact_info = {0};
   // MMI_STRING_T        name_str ={0};
    uint8 num_buf[MMIPB_NUMBER_MAX_STRING_LEN + 1] = {0};
    uint8 char_name_buf[MMIPB_NAME_MAX_LEN + 1] = {0};
    wchar name_buf[MMIPB_NAME_MAX_LEN + 1] = {0};
    MMI_PARTY_NUMBER_T  party_num = {0};


    if((!MMIPB_IsPbReady()))
    {
        SCI_TRACE_LOW("MMIPB_FillPhoneContactFull PB NOT READY");
        return FALSE;
    }

    used_num = MMIPB_GetContactCount(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT));
    max_num = MMIPB_GetContactStorageSpace(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT)); 
    add_num = max_num - used_num;

    for (i = 0; i < add_num && i <= 9999; i++)
    {
        SCI_MEMSET(num_buf, 0x0, sizeof(num_buf));
        SCI_MEMSET(name_buf, 0x0, sizeof(name_buf));
        SCI_MEMSET(&contact_info, 0x0, sizeof(contact_info));
        
        sprintf((char*)num_buf, "123%04d", i);        
        if (MMIAPICOM_GenPartyNumber((uint8 *)(num_buf), 7, &party_num))
        {
            contact_info.number[0].npi_ton = (uint8)((party_num.number_type << 0X04) | MMIPB_DEFAULT_NPI);
            contact_info.number[0].number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
            SCI_MEMCPY(contact_info.number[0].number, party_num.bcd_num, sizeof(party_num.bcd_num));
        }
        
        sprintf((char*)char_name_buf, "a%04d", i);
        MMIAPICOM_StrToWstr(char_name_buf, name_buf);
        contact_info.name.wstr_len = MMIAPICOM_Wstrlen(name_buf);
        contact_info.storage_id = 1;
        MMIAPICOM_Wstrncpy(contact_info.name.wstr, name_buf, contact_info.name.wstr_len);
        
        contact_info.contact_id = 0;
        MMIPB_AddContact(&contact_info);
    }
    
    return TRUE;
}

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : judge whether export contact isnot
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsExportContactOpc(void)
{
    MMI_HANDLE_T		applet_handle = 0;
    MMIPB_APPLET_T*		applet_ptr = PNULL;
    BOOLEAN				result = FALSE;
    
    if(MMIPB_GetCurAppletHandle(&applet_handle))
    {
        applet_ptr = (MMIPB_APPLET_T*)MMK_GetAppletPtr(applet_handle);
        if((applet_ptr != PNULL) && (MMIPB_OPC_EXPORT == applet_ptr->instance.cmd))
        {
            result =  TRUE;			
        }
        
    }
    return result;
}
#endif

#ifdef BT_PBAP_SUPPORT
/*******************************************************************************/
//  Description :����vcard buffer data  
//  Parameter: data_ptr[in]: ���ڱ���UTF-8�����vCard��Ŀ���ݣ��������vCard2.1�淶�Ĺ涨��
//			   data_len[in]: ����vCard��¼�Ĵ�С
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
PUBLIC void MMIPB_NotifyPBStartParser(MMIPB_PBAP_PARSER_CALLBACK_FUNC callback)
{
    MMIPB_PBAP_PARSER_RESULT_T result = {0};
    parser_callback = callback;
	result.ret = MMIPB_PBAP_PARSER_FINISHED;
	result.sync_count = s_sync_contact_count;
	parser_callback(&result);
	//freeallpbapdata();
	parser_callback = NULL;
	SCI_TRACE_LOW("[MMIPB] MMIPB_NotifyPBStartParser buff = pnull");
}
/*******************************************************************************/
//  Description :����vcard buffer data  
//  Parameter: data_ptr[in]: ���ڱ���UTF-8�����vCard��Ŀ���ݣ��������vCard2.1�淶�Ĺ涨��
//			   data_len[in]: ����vCard��¼�Ĵ�С
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL MMIPB_ERROR_E MMIPB_PBAPVcardToContact(
									 uint8 *data_ptr,//[IN]
                                     uint32 data_len,//[IN]
                                     MMIPB_CONTACT_T *contact_ptr, //[OUT]
                                     MMIPB_BCD_NUMBER_T *number_array, //[out] //��������
                                     uint8 *num_type_array, //[out] //������������
                                     uint8 num_array_len //[int] �������
                                      )
{
#ifdef MMI_VCARD_SUPPORT
    MMIPB_ERROR_E result = MMIPB_ERROR_SUCCESS;
    VCARD_ERROR_E vcard_ret = VCARD_NO_ERROR;

    if(PNULL == data_ptr||0==data_len||PNULL == contact_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }
    SCI_MEMSET(contact_ptr, 0, sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(number_array, 0, num_array_len * sizeof(MMIPB_BCD_NUMBER_T));
    //��ʼ����
    vcard_ret = MMIAPIVCARD_VcardDataToContactEx(data_ptr, data_len, contact_ptr, PNULL, number_array, num_type_array, num_array_len);
    if(VCARD_NO_ERROR != vcard_ret)
    {
        if(VCARD_FORMAT_ERROR == vcard_ret)
        {
            result = MMIPB_ERROR_INVALID_FORMAT;
        }
        else if(VCARD_EMPTY_FILE == vcard_ret)
        {
            result = MMIPB_ERROR_NO_ENTRY;
        }
        else if(VCARD_SPACE_FULL == vcard_ret)
        {
            result = MMIPB_ERROR_NO_SPACE;
        }
        else 
        {
            result = MMIPB_ERROR_ERROR;
        }
    }
    return result;
#else
	return MMIPB_ERROR_ERROR;
#endif    
}
#if 0
/*******************************************************************************/
//  Description :save buffer to file 
//  Parameter: buff[in]: Ҫ���������buffer
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL void MMIPB_SaveBufferToFile(const char* buff)
{
    wchar  path_name_ptr[] = L"E:";
    wchar   full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = 0;
    uint32 bytes_written = 0;
	
	if (PNULL == buff)
	{
		SCI_TRACE_LOW("[MMIPB] MMIPB_SaveBufferToFile buff = pnull");
		return;
	}

    MMIAPIVCARD_GetVcardDefaultFileName(path_name_ptr, full_path_name, MMIFILE_FULL_PATH_MAX_LEN);

	SCI_TRACE_LOW("[MMIPB] MMIPB_SaveBufferToFile full_path_name: %s", full_path_name);

    file_ret = MMIAPIFMM_CreateFileAsyn(full_path_name,                                     
        SFS_MODE_WRITE|SFS_MODE_CREATE_NEW|SFS_MODE_APPEND,                                                 
        PNULL,                                                                              
        PNULL,                                                                              
        0,                                                                                  
        0,                                                                                  
        &file_handle);  

	SCI_TRACE_LOW("[MMIPB] MMIPB_SaveBufferToFile file_ret: %d", file_ret);

    if(SFS_ERROR_NONE == file_ret)
    {
        file_ret = MMIAPIFMM_WriteFile(file_handle, (char*)buff, strlen(buff), &bytes_written, PNULL);                        
    }

	SCI_TRACE_LOW("[MMIPB] MMIPB_SaveBufferToFile file_handle: %x, bytes_written: %d, file_ret: %d", file_handle, bytes_written, file_ret);

    MMIFILE_CloseFile(file_handle);

}
#endif
/*******************************************************************************/
//  Description :parse buffer data ,then save to contact
//  Parameter: buff[in]: vcard buffer data
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddDataToContact(const char*  buff     //[in]                                      
                                        ) 
{
	char*   p_searched_ptr = PNULL;
    uint32  offset_len = 0;
    char*   search_ptr = buff;
//    char*   search_next_ptr = buff;   
	uint32	extract_len = 0;
	char*	extract_buff_ptr = PNULL;
    MMIPB_CONTACT_T contact = {0};
    MMIPB_ERROR_E   ret = MMIPB_ERROR_ERROR;
	char*	search_endTag_ptr = PNULL;
//	BOOLEAN	find_pair = TRUE;
    char*	merge_buff_ptr = PNULL; 
	uint32  merge_buff_len = 0;
    char*   rn_property = "\r\n"; 
//	char*   rn_ptr = PNULL;
    MMIPB_BCD_NUMBER_T number_array[PBAP_CONTACT_NUM_COUNT] = {0};
    uint8   num_type_array[PBAP_CONTACT_NUM_COUNT] = {0};
    uint8   num_array_len = PBAP_CONTACT_NUM_COUNT; 
    uint8   index = 0;

    if(PNULL == buff)
    {
        SCI_TRACE_LOW("[MMIPB] MMIPB_AddDataToContact param error");
        return MMIPB_ERROR_ERROR;
    }
    
    if (PNULL != s_vcard_buff_ptr)
    {
        merge_buff_len = strlen((char*)s_vcard_buff_ptr) + strlen((char*)buff);
    }
    else
    {
        merge_buff_len = strlen((char*)buff);
    }

    //����ƴ�Ӻ� ����buffer
	merge_buff_ptr = SCI_ALLOC_APP(merge_buff_len + 1);

	if (PNULL == merge_buff_ptr)
	{
		SCI_TRACE_LOW("[MMIPB] MMIPB_AddDataToContact merge_buff_ptr Alloc fail!");
        
        if(PNULL != s_vcard_buff_ptr)
        {
	        SCI_FREE(s_vcard_buff_ptr);
        }

		return MMIPB_ERROR_ERROR;
	}

	SCI_TRACE_LOW("[MMIPB] MMIPB_AddDataToContact merge_buff_len:%d", merge_buff_len);

	SCI_MEMSET(merge_buff_ptr, 0x00, merge_buff_len + 1);

	//����������������, ��Ҫ�Ƚ� ��һ�������е�data �� ��һ��buffer�е�data ƴ������
    if (PNULL != s_vcard_buff_ptr)
	{
		SCI_TRACE_LOW("[MMIPB] MMIPB_AddDataToContact s_vcard_buff_ptr has data");

	    strncpy(merge_buff_ptr, s_vcard_buff_ptr, strlen((char*)s_vcard_buff_ptr));

        //�ͷ���һ������buffer
	    SCI_FREE(s_vcard_buff_ptr);	
    }
    
	strncat(merge_buff_ptr, buff, strlen((char*)buff));
    
    MMI_CheckAllocatedMemInfo();
    do
    {
		/************************************************************************/
		/* whether phone stroage has free space or not                           */
		/************************************************************************/
        if(MMIPB_ERROR_NO_SPACE == MMIPB_GetStorageFreeSpace(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT), PNULL))
        {
            ret = MMIPB_ERROR_NO_SPACE;
            break;
        }

		//ƫ�Ƴ���
		p_searched_ptr = &merge_buff_ptr[offset_len];
      
		//�ж��Ƿ�ֻʣ��"\r\n"
		if (0 == strcmp(rn_property, p_searched_ptr))
		{
            ret = MMIPB_ERROR_SUCCESS;
            break;
		}
		
        /************************************************************************/
        //buffer ��ȡ��������:
        //BEGIN:VCARD��END:VCARD������Գ���
        //�Ȳ���BEGIN:VCARD���ٲ���END:VCARD 
        //���BEGIN:VCARD��END:VCARD������,˵����������������ֱ�Ӷ�����н���
        //����˵�����ݲ���������Ҫ����һ��buffer������һ�����
        /************************************************************************/        
        {
            //��BEGIN:VCARD 
			search_ptr = strstr(p_searched_ptr, VCARD_BEGIN_TAGS);
            SCI_TRACE_LOW("MMIPB_AddDataToContact start find BEGIN:VCARD = %ld",search_ptr);
   
			if (PNULL != search_ptr)
			{        
                //��END:VCARD 
                search_endTag_ptr = strstr(search_ptr, VCARD_END_TAGS);
				SCI_TRACE_LOW("MMIPB_AddDataToContact start find END:VCARD = %ld",search_endTag_ptr);

                if (PNULL != search_endTag_ptr)
        		{
        		    //begin and end �������ҵ��������
					//�˴�ʹ��p_searched_ptr������search_ptr������ƫ����
        			extract_len = (strlen((char*)p_searched_ptr) - strlen((char*)search_endTag_ptr) + strlen((char*)VCARD_END_TAGS));	

                    extract_buff_ptr = SCI_ALLOC_APP(extract_len + 1);

                    if (PNULL == extract_buff_ptr)
                    {
                        SCI_TRACE_LOW("MMIPB_AddDataToContact extract_buff_ptr alloc fail");

                        ret = MMIPB_ERROR_ERROR;
                        break;
                    }

                    SCI_MEMSET(extract_buff_ptr, 0x00, (extract_len + 1));

                    strncpy(extract_buff_ptr, p_searched_ptr, extract_len);

                    //����һ����¼������
                    ret = MMIPB_PBAPVcardToContact((uint8 *)extract_buff_ptr, (uint32)extract_len, &contact, number_array, num_type_array, num_array_len);

                    SCI_TRACE_LOW("[MMIPB] MMIPB_AddDataToContact MMIPB_PBAPVcardToContact, ret:%d", ret);

                    SCI_FREE(extract_buff_ptr);
                    
                    if(MMIPB_ERROR_SUCCESS != ret)
                    {
                        ret = MMIPB_ERROR_ERROR;
                        break;
                    }

                    SCI_MEMSET(contact.number, 0x00, sizeof(MMIPB_BCD_NUMBER_T) * MMIPB_MAX_NV_PHONE_NUM);
                    SCI_MEMSET(contact.num_type, 0X00, sizeof(uint8) * MMIPB_MAX_NV_PHONE_NUM);

                    if(PNULL != number_array) /*lint !e506 !e774*/
                    {
                        for(index = 0; index < num_array_len; index++)
                        {
							//if (0 != strlen((char*)number_array[index].number))
                            if (0 != number_array[index].number_len)
							{
								SCI_MEMCPY(&contact.number[0], &(number_array[index]), sizeof(MMIPB_BCD_NUMBER_T));
								contact.num_type[0] = num_type_array[index];
							}
                            ret = AddContactForPbap(&contact, PNULL);

                            if(MMIPB_ERROR_SUCCESS != ret)
                            {
                                 break;
                            }

                        }
                    }

                    SCI_TRACE_LOW("[MMIPB] AddContactForPbap MMIPB_AddContactForPbap, ret:%d", ret);

                    if(MMIPB_ERROR_SUCCESS != ret)
                	{
                        break;
                	}
                    //��������ɹ���index��һ��������һ����¼�Ľ���
                    s_sync_contact_count++;

        		    offset_len += extract_len;           

                    continue;
        		}

			}
           
            //���ݲ���������Ҫ�Ȼ������� ps:����ÿ��buffer��������ļ�¼
            extract_len = strlen((char*)p_searched_ptr);

			if (PNULL != s_vcard_buff_ptr)
			{
				SCI_FREE(s_vcard_buff_ptr);
			}

			s_vcard_buff_ptr = SCI_ALLOC_APP(extract_len + 1);

			if (PNULL == s_vcard_buff_ptr)
			{
                ret = MMIPB_ERROR_ERROR;
                break;
			}

			SCI_MEMSET(s_vcard_buff_ptr, 0x00, extract_len + 1);

			strncpy(s_vcard_buff_ptr, p_searched_ptr, extract_len);

            ret = MMIPB_ERROR_SUCCESS;
            break;
            //�������ݽ���
        }
    }    
    while(offset_len < strlen((char*)merge_buff_ptr));

    MMI_CheckAllocatedMemInfo();

	if (PNULL != merge_buff_ptr) /*lint !e774*/
	{
		SCI_FREE(merge_buff_ptr);
	}
    
    SCI_TRACE_LOW("[MMIPB] MMIPB_AddDataToContact s_sync_contact_count = %d,ret = %d", s_sync_contact_count,ret); 
    return ret;  		
}
/*******************************************************************************/
//  Description :��ϵ��ͬ���������ͨѶ¼
//  Parameter: contact_ptr[in]: ��¼�����Ϣ
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL MMIPB_ERROR_E AddContactForPbap(MMIPB_CONTACT_T *contact_ptr, MMIPB_CUSTOM_PHOTO_T *photo_ptr)
{
    MMIPB_CONTACT_T     contact_info = {0};
    MMIPB_ERROR_E       error_code = MMIPB_ERROR_ERROR;
    
    SCI_TRACE_LOW("[MMIPB] enter MMIBT_AddContact!");
    
    if (PNULL == contact_ptr)
    {
        SCI_TRACE_LOW("[MMIPB] MMIBT_AddContact invalid param !");
        return MMIPB_ERROR_ERROR;
    }

    contact_info = *(MMIPB_CONTACT_T *)contact_ptr;
    
    if(is_same_contact(&contact_info))
    {
        error_code = MMIPB_ERROR_SUCCESS;
    }
    else
    {
        contact_info.storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
        
        contact_info.contact_id = MMIPB_GetFreeContactId(contact_info.storage_id);

        contact_info.is_bt_contact = TRUE;
        SCI_TRACE_LOW("AddContactForPbap storage_id:%d, contact_id:%d",contact_info.storage_id, contact_info.contact_id);
        if(MMIPB_ERROR_SUCCESS == MMIPB_GetStorageFreeSpace(contact_info.storage_id, PNULL))
        {
            if(MMIPB_ERROR_SUCCESS == MMIPB_UpdateContact(&contact_info))
            {
#ifdef MMI_CONTACT_PHOTO_SUPPORT
                SCI_TRACE_LOW("AddContactForPbap MMI_CONTACT_PHOTO_SUPPORT");
                MMIPB_SavePhotoInfoByStorageId(contact_info.contact_id, contact_info.storage_id, photo_ptr);
#endif
                error_code = MMIPB_ERROR_SUCCESS;
            }
        }
        else
        {
            //pb��¼��
            error_code = MMIPB_ERROR_NO_SPACE;
        }
    }

    SCI_TRACE_LOW("[MMIPB] MMIBT_AddContact error_code %d !", error_code);
    
    return error_code;
}
/*******************************************************************************/
//  Description : ������ͷ�ڵ�ĵ���������ͷ���
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_InitPBAPDataList(void)
{				
	if(PNULL == s_list_head)
	{
		s_list_head = InitList();
	}
	else if(!IsListEmpty(s_list_head))
	{   
	    ClearList(s_list_head);
	}

	if(PNULL == s_list_head)
	{
		return FALSE;
	}
    s_node_number = 1;
	return TRUE;
}

/*****************************************************************************/
//  Description : insert PBAP data into link list
//  Global resource dependence : none
//  Author: Allen.Lee
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_InsertPBAPDataToList(void* buff,uint16 buff_len)
{
    MMIPB_CNF_MSG_T     cnf_msg = {0};
	
	if(0 == InsertList(s_list_head, buff,buff_len))
	{
		SCI_TRACE_LOW("MMIPB_InsertPBAPDataToList failed");

		return FALSE;
	}

    cnf_msg.ret = MMIPB_ParsePbapData();

    MMIPB_SendSignalToMMITask(MMI_PB_PBAP_CONTACT_READ_CNF, &cnf_msg);

	return TRUE;
}

/*****************************************************************************/
//  Description : parse pbap link list data
//  Global resource dependence : none
//  Author: Allen.Lee
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ParsePbapData(void)
{   
    char *data_ptr = PNULL;
    uint16 len = 0;
    
    GetElem(s_list_head, s_node_number, &data_ptr, &len);
    s_node_number++;
    //MMIPB_SaveBufferToFile(data_ptr);
    return MMIPB_AddDataToContact(data_ptr);
}

/*******************************************************************************/
//  Description : free all pbap list buffer
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
LOCAL void freeallpbapdata(void)
{
    ClearList(s_list_head);
    s_sync_contact_count = 0;
    s_node_number = 1;
    SCI_FREE(s_vcard_buff_ptr);
}

/*******************************************************************************/
//  Description : free all pbap list buffer
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney.wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMIPB_FreeAllPbapData(void)
{
    freeallpbapdata();
}

/*******************************************************************************/
//  Description : ������ͷ�ڵ�ĵ���������ͷ���
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL List* InitList(void) 
{   
    List *h;  /* h �����ͷ�ڵ��ָ��*/

    h=(List*)SCI_ALLOC_APPZ(LEN);

    if(PNULL == h)  /*����ռ䲢���*/
        return 0;

    h->data = 0;    /*�ѱ�ͷ�ڵ���������ÿ�*/
    h->next = PNULL; /*�ѱ�ͷ���������ÿ�*/
    h->len = 0;
    return h;
}
/*******************************************************************************/
//  Description : �������ĩβ���һ��Ԫ��
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL int InsertList(List *L, void* buff,uint16 buff_len)
{
    List *pInsert;
    List *pHead;
    List *pTmp;  //����һ����ʱ����������ŵ�һ���ڵ� 

    pHead = L;
    pTmp = pHead;
    

    pInsert = (List*)SCI_ALLOC_APPZ(LEN);//����һ���½ڵ� 
    
    if(PNULL == pInsert)  /*����ռ䲢���*/
    {
        return 0;
    } 

    pInsert->data = SCI_ALLOC_APPZ(buff_len+1);

	if(PNULL == pInsert->data)
	{
		SCI_FREE(pInsert);
		return 0;
	}

	SCI_MEMCPY(pInsert->data,buff,buff_len);
    pInsert->len = buff_len;
    pInsert->next = PNULL;
	
    while(pHead->next != PNULL)
    {
        pHead = pHead->next;
    }

    pHead->next = pInsert; //������ĩβ�ڵ����һ���ָ������ӵĽڵ� 
    L = pTmp;
    return 1;
}

/*******************************************************************************/
//  Description:������Ա�L�е�����Ԫ�أ����ͷŵ�����L�����еĽ�㣬ʹ֮��Ϊһ���ձ�
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL void ClearList(List *pHead) 
{ 
    List *pNext;            //����һ����pHead���ڽڵ� 
    List *pHead_backup = pHead;
  
    if(pHead == PNULL) 
    { 
        return; 
    } 
	pHead = pHead->next;
	
    while(pHead!= PNULL) 
    { 
        pNext = pHead->next;//������һ����ָ�� 
        SCI_FREE(pHead->data);
        SCI_FREE(pHead); 
        pHead = pNext;      //��ͷ���� 
    } 

	pHead_backup->next = PNULL;
} 
/*******************************************************************************/
//  Description:��ȡ�������е�i��Ԫ��
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL int GetElem(List *L, int i, void** buff, uint16 *len)
{
    int j = 1;
    List *p = L->next;

    while(p && (j < i))
    {
        p = p->next;
        ++j;
    }

    if(PNULL == p || (j > i))
    {
        return 0;
    }
    else
    {
        *buff = p->data;
        *len = p->len;
        return 1;
    }
}
/*******************************************************************************/
//  Description:�жϵ������Ƿ�Ϊ��
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL int IsListEmpty(List *L)
{
    return(PNULL == (L->next));
}
/*******************************************************************************/
//  Description:�ж�����ڵ����
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL int ListNodeNum(List *L)
{
    List *p = L;
    int i = 0;
    while(PNULL != p->next)
    {
       i++;
       p = p->next;
    }
    return i;
}
/*****************************************************************************/
//  Description : ���ȫ��������ϵ��
//  Global resource dependence : none
//  Author: Yanyan.an
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ClearAllBtContactForPbap(void)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
	uint16 cur_index = 0;
	MMIPB_CONTACT_BASE_INFO_T find_data = {0};
    MMIPB_HANDLE_T handle = 0;
	MMIPB_HANDLE_T applet_handle = 0;
    
    MMIPB_SearchQSortList(PNULL, (1<<PB_GROUP_BT), 0, TRUE);
    
	result = MMIPB_FindFirstBtContact(&cur_index, &find_data, &handle);
    
    while(result == MMIPB_ERROR_SUCCESS)
    {
        result= MMIPB_DeleteContact(find_data.contact_id, find_data.storage_id);
        if(MMIPB_ERROR_SUCCESS == result)
        {
			MMIPB_FindClose(handle);
            result = MMIPB_FindFirstBtContact(&cur_index, &find_data, &handle);
        }
    }
	
    if(handle)
    {
        MMIPB_FindClose(handle);
    }
	//update current list win
	MMIPB_GetCurAppletHandle(&applet_handle);	
	MMIPB_UpdateCurrentList(applet_handle, MMIPB_OPC_DELETE); 	
	
    return MMIPB_ERROR_SUCCESS;
}
/*****************************************************************************/
//  Description :���ȫ��������ϵ��
//  Global resource dependence : none
//  Author: Yanyan.an
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ClearAllBtContact(MMIPB_APPLET_T* applet_ptr)
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    uint16 cur_index = 0;
    MMIPB_CONTACT_BASE_INFO_T find_data = {0};
    MMIPB_HANDLE_T handle = 0;
//    MMIPB_ACCESS_POINT_T access_point = {0};
    MMIPB_SEL_DATA sel_data = {0};
	
    if(PNULL == applet_ptr)
    {
        SCI_TRACE_LOW("MMIPB_ClearAllBtContact PNULL == applet_ptr");
        return result;
    }
	
    if(MMIPB_ERROR_SUCCESS != MMIPB_FindFirstBtContact(&cur_index, &find_data, &handle))
    {
        SCI_TRACE_LOW("[MMIPB] MMIPB_ClearAllBtContact can't find any contact to insert");
        return MMIPB_ERROR_NO_ENTRY;
    }
	
    do
    {
		sel_data.entry_id = find_data.contact_id;
		sel_data.storage_id = find_data.storage_id;			
		MMIPB_CreateSelNode(applet_ptr->instance.handle, &sel_data);   
    }while(MMIPB_FindNextBtContact(handle, &cur_index, &find_data) == MMIPB_ERROR_SUCCESS);
	
    MMIPB_AbortPB(FALSE);
    result = MMIPB_DeleteContactAsynForPB(applet_ptr->instance.handle);
	
	if (handle)
	{
		MMIPB_FindClose(handle);
	}
	
	return result;  
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
PUBLIC uint32 WatchPB_GetImageIdByFlag(uint8 photo_id_flag, BOOLEAN in_call)
{
    MMI_IMAGE_ID_T      contactImageId = NULL;
    MMI_IMAGE_ID_T      incallImageId = NULL;

    switch(photo_id_flag)
    {
        case PB_PROTRAIT_FATHER:
        {
            contactImageId = IAMGE_CONTACT_PORTRAIT_FATHER;
            incallImageId = IAMGE_CALL_PORTRAIT_FATHER;
            break;
        }
        case PB_PROTRAIT_MOTHER:
        {
            contactImageId = IAMGE_CONTACT_PORTRAIT_MOTHER;
            incallImageId = IAMGE_CALL_PORTRAIT_MOTHER;
            break;
        }
        case PB_PROTRAIT_GRANDPA:
        {
            contactImageId = IAMGE_CONTACT_PORTRAIT_GRANDPA;
            incallImageId = IAMGE_CALL_PORTRAIT_GRANDPA;
            break;
        }
        case PB_PROTRAIT_GRANDMA:
        {
            contactImageId = IAMGE_CONTACT_PORTRAIT_GRANDMA;
            incallImageId = IAMGE_CALL_PORTRAIT_GRANDMA;
            break;
        }
        case PB_PROTRAIT_TEACHER:
        {
            contactImageId = IAMGE_CONTACT_PORTRAIT_TEACHER;
            incallImageId = IAMGE_CALL_PORTRAIT_TEACHER;
            break;
        }
        case PB_PROTRAIT_CLASSMATE:
        {
#ifdef SCREEN_SHAPE_CIRCULAR
            contactImageId = IAMGE_CONTACT_PORTRAIT_CLASSMATE;
            incallImageId = IAMGE_CALL_PORTRAIT_CLASSMATE;
#else
            //��BROTHER����classmate
            contactImageId = IAMGE_CONTACT_PORTRAIT_BROTHER;
            incallImageId = IAMGE_CALL_PORTRAIT_BROTHER;
#endif
            break;
        }
#ifdef SCREEN_SHAPE_CIRCULAR
        case PB_PROTRAIT_SISTER:
        {
            contactImageId = IAMGE_CONTACT_PORTRAIT_SISTER;
            incallImageId = IAMGE_CALL_PORTRAIT_SISTER;
            break;
        }
        case PB_PROTRAIT_BROTHER:
        {
            contactImageId = IAMGE_CONTACT_PORTRAIT_BROTHER;
            incallImageId = IAMGE_CALL_PORTRAIT_BROTHER;
            break;
        } 
#endif
        case PB_PROTRAIT_STRANGER:
        {
            contactImageId = IAMGE_CONTACT_PORTRAIT_STRANGER;
            incallImageId = IAMGE_CALL_PORTRAIT_STRANGER;
            break;
        }
        case PB_PROTRAIT_EMERGRNCY:
        {
            incallImageId = IAMGE_CALL_PORTRAIT_EMERGENCY;
            break;
        }
        default:
            break;
    }
    if(in_call)
    {
        return incallImageId;
    }
    else
    {
        return contactImageId;
    }
}

/*****************************************************************************/
//  Description : get name(or number) and imageId for display
//  Parameter: [In] tele_num & tele_len
//             [Out] (wchar)in_str
//             [Return] image number
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC uint8 WatchPB_GetDispInfoContent(uint8 *tele_num, uint8 tele_len, wchar *in_str, BOOLEAN is_mo_call)
{
    MMI_STRING_T        nameStr = {0};
    uint8               imageId = 0;
    BOOLEAN             isEmergencyCall = FALSE;

    if((tele_num == PNULL) || (in_str == PNULL))
    {
        return FALSE;
    }

    isEmergencyCall = MMIAPICC_IsEmergencyNum(tele_num, tele_len, MMICC_GetWantCallDualSys(), FALSE);

    //get info for display
    if(isEmergencyCall && is_mo_call )
    {
#ifdef MODEM_SUPPORT_RDA
        if(0 != tele_len)
        {
            MMI_STRNTOWSTR(in_str,MMIPB_NUMBER_MAX_STRING_LEN,tele_num,tele_len,tele_len);
        }
#else
        MMI_GetLabelTextByLang(TXT_PB_EMERGENCY, &nameStr);
        MMI_WSTRNCPY(in_str,MMIPB_NAME_MAX_LEN,(const wchar*)nameStr.wstr_ptr, nameStr.wstr_len,nameStr.wstr_len);
#endif
        imageId = PB_PROTRAIT_EMERGRNCY;
    }
    else
    {
        if (0 != tele_len)
        {
            uint8     i = 0;
            uint32    contactNum = WatchPB_SearchPresetContact();
            MMIPB_CONTACT_T pInfo = {0};
            uint32    arrayIdx[WATCHPB_CONTACT_NUM_MAX] = {0};
            uint8     numBuf[MMIPB_NUMBER_MAX_STRING_LEN+2] = {0};
            uint8     numLen = 0;

            WatchPB_GetPresetContactArray(arrayIdx);
            for(i = 0; i < contactNum; i++)
            {
                WatchPB_GetPresetContactInfoByIndex(arrayIdx[i], &pInfo);
                MMIPB_BCDNumberToUTF8(pInfo.number, MMIPB_NUMBER_MAX_STRING_LEN, numBuf);
                numLen = MMIPB_GetBCDNumberStrlen(&pInfo.number);
                if(!MMIAPICOM_StrCmp(tele_num, tele_len, &numBuf, numLen))
                {
                    imageId = pInfo.photo_id_flag;
                    MMI_WSTRNCPY(in_str,MMIPB_NAME_MAX_LEN,&pInfo.name.wstr, pInfo.name.wstr_len, pInfo.name.wstr_len);
                    break;
                }
            }
        }
    }
    //no name in pb and is not emergency
    if (imageId == 0)
    {
        imageId = PB_PROTRAIT_STRANGER;
        if(0 != tele_len)
        {
            MMI_STRNTOWSTR(in_str,MMIPB_NUMBER_MAX_STRING_LEN,tele_num,tele_len,tele_len);
        }
        else
        {
            MMI_STRING_T  dispStr = {0};
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_UNKNOW_NUM, &dispStr);
            MMI_WSTRNCPY(in_str, MMIPB_NAME_MAX_LEN, dispStr.wstr_ptr, dispStr.wstr_len, dispStr.wstr_len);
        }
    }

    return imageId;
}

PUBLIC BOOLEAN WatchPB_GetContactInfoByBcd(MMIPB_BCD_NUMBER_T *bcd_num, MMIPB_CONTACT_T *contact_ptr)
{
    MMIPB_BCD_NUMBER_T bcd_info = {0};
    uint8 i = 0;
    uint32 contactNum = WatchPB_SearchPresetContact();
    uint32 arrayIdx[WATCHPB_CONTACT_NUM_MAX] = {0};
    MMIPB_CONTACT_T pInfo = {0};
    uint8               imageId = 0;

    if(bcd_num==PNULL||contact_ptr ==PNULL)
    {
        return FALSE;
    }

    bcd_info = *(MMIPB_BCD_NUMBER_T *)bcd_num;

    WatchPB_GetPresetContactArray(arrayIdx);

    //search the same number in the preset contact and set contact photo
    for(i=0;i<contactNum;i++)
    {
        WatchPB_GetPresetContactInfoByIndex(arrayIdx[i], &pInfo);

        //compare two numbers to see if same
        if(0 == MMIAPIPB_BcdSmartCmp(bcd_num, pInfo.number))//Fix bug:1365123
        {
            SCI_MEMSET(contact_ptr,0,sizeof(MMIPB_CONTACT_T));
            SCI_MEMCPY(contact_ptr,&pInfo,sizeof(MMIPB_CONTACT_T));
            return TRUE;
        }
    }

    return FALSE;
}


