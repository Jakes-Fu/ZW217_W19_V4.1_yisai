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
//  Description : Phonebook��ʼ��
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
//  Note: ������������pb�����������������sim��ready������Ҫ��CALL_READY��SIM_NOT_READY
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
//  Note: ���������ALL_READY��SIM_NOT_READY���ٽ�������
//  [�ر���ʾWarnning]:����������Žӿڣ�һ��Ҫ��֤sim�������·�ALL_READY��SIM_NOT_READY����sim�����³�ʼ��
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
    // ��ʼ������Ϣ�б�
    MMIPB_ResetGroupInfo();
}
#endif
/*****************************************************************************/
//  Description : ����handle
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
//  Description : ����handle
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
//  Description : PB����ں���������pb�����ڣ����ݲ�������ʾPB���б�
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
//  Note: ����Ϊ�ӿڣ��ṩ������ģ�����
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
// Description : �첽������ϵ��
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
// Description : ������ϵ��
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
//  Description : ������ϵ��
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
//  Description :дȺ������
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
//  Description : �첽������ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_AddContactAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback)
{
    MMIPB_AbortPB(FALSE);
    return MMIPB_AddContactAsyn(handle, contact_ptr, callback);
}

/*****************************************************************************/
//  Description : �첽 �༭��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ModifyContactAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback)
{
    MMIPB_AbortPB(FALSE);
    return MMIPB_ModifyContactAsyn(handle, contact_ptr, callback);
}

/*****************************************************************************/
//  Description : �첽ɾ����ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:���ⲿ�ӿڵ���
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
//  Description : �첽������ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:�ṩ���ⲿ�ӿڵ��ã���ʱ���θýӿڣ�����
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
// Description : ������ϵ��
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
//  Description : ��ȡ�������ĵ�index+1����ϵ�ˡ�
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
//  Description : ��ȡ�������ĵ�index+1����ϵ��������
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_GetSelectName(MMIPB_HANDLE_T handle,MMI_STRING_T *name_str, uint16 max_name_len,uint16 index)
{
    return MMIPB_GetSelectName(handle, name_str, max_name_len, index);
}

/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ��email��
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

PUBLIC BOOLEAN  MMIAPIPB_GetSelectEmail(MMIPB_HANDLE_T handle,MMI_STRING_T *mail_ptr, uint16 max_mail_len,uint16 index)
{
    return MMIPB_GetSelectEmail(handle, mail_ptr, max_mail_len, index);
}

/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ�˺��롣
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
//  Description : ��ȡ�������ĵ�index+1����ϵ��vcard��
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
//  Description : FDN�б���
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
//  Description : PB�ⲿ���Һ�����ͨ���ַ������ҷ�����������ϵ���б�
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
                        list_type, //IN: �����б������
                        field_display_type,
                        (MMIPB_SEARCH_NAME + MMIPB_SEARCH_NUMBER + MMIPB_SEARCH_EMAIL),
                        1,//����Ƕ�ѡʱ�����������˶�ѡ��������
                        search_str,
                        callback
                        );
}

/*****************************************************************************/
//  Description : ����������ַ������Һ����а����ô��ļ�¼�������а�������ĸƥ��ļ�¼
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: ���������������ʹ����ɺ�Ҫ����MMIAPIPB_SearchExit�ͷ�
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
//  Description : �ⲿ�����˳������ⲿ�����˳��ǣ�����ӿ�һ��Ҫ����
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
//  Description : �������ص��ĵ�index����0��ʼ������¼���������ͺ���
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
//  Description : �������ص��ĵ�index����0��ʼ������¼��contact_id and storage id
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: Ϊ�ⲿ���н�pbѡ���
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetSearchEntryID(MMIPB_HANDLE_T handle,                                       
                                       uint32 index,
                                       uint16 *contact_id_ptr,
                                       uint16 *storage_id_ptr)
{
    return MMIPB_GetSearchEntryID(handle, index, contact_id_ptr, storage_id_ptr);
}

/*****************************************************************************/
//  Description : �������ص��ĵ�index����0��ʼ������¼��Сͼ��
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
//  Description : ���õ绰����ı�ʹ��

//  Note: ���¸ú����ʹ��Ƶ�ʡ�
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
//  Description : ����Ӳ˵��Ĵ��ڡ�
//  Global resource dependence : g_mmipb_bcd_number_t
//  Author: 
//  Note: �˺���������Ϣ�Ͳ�����ȡ���벢�����ʱ����ã�
//        ��ͨ����¼���뱣�浽�绰���е��ã�����ģ����á�
//        ������ģ�����ʱ����Ҫ������룻����ģ�����ʱ�����ô����롣
/*****************************************************************************/

void MMIAPIPB_AddContactWin
(
    void	*data_ptr,	    //IN: ����ĵ绰����
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
//  Description : Ϊͨ����¼�򿪼�¼�б��ڡ�
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: �˺�����ͨ����¼�����뱣�浽��������ʱ���á�
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
//  Description : ��ô洢λ������Ӧ��������Ⱥ��ID��ͼƬID
//  Global resource dependence : none
//  Parameter:  bcd_number : ����ĵ绰����[IN]
//              name_ptr:  ����[OUT]
//              max_name_len:��������󳤶�(���ֽڼ�)  [IN]
//              is_use_default_name:�Ƿ���Ҫ����ȱʡ����[IN]
//   RETURN: TRUE, �����ɹ���FALSE, ����ʧ��
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
//  Description : ��ȡ���������Ӧ�ĺ�������
//  Global resource dependence : none
//  Parameter:  bcd_number : ����ĵ绰����[IN]
//              type_ptr:  ��������[OUT]
//              max_name_len:���������ַ�������󳤶�(��wchar��)  [IN]
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
//  Description : ��õ绰���Ƿ���Ա�����ģ�����
//  Global resource dependence : 
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

BOOLEAN MMIAPIPB_IsPbReady(void) //RETURN: TRUE, ����ʹ��
{
    return MMIPB_IsPbReady();
}


/*****************************************************************************/
//  Description :��ȡȺ������
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
//  Description :	���PB
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_AbortPB(void)
{
      MMIPB_AbortPB(TRUE);
}

/*****************************************************************************/
//  Description : ��ȡ���豸��storage id
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
//  Description : ����ϵ����Ϣת��Ϊvcard���ݸ�ʽ
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
//  Description : ��vcard����ת��Ϊ��ϵ����Ϣ��ͨ�ø�ʽ
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
// Description : ����VCard data,���򿪸��ļ�
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
// Description : ����VCard data,���򿪸��ļ�
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
//  Description : �绰�����ְ��ƥ��
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: 0:���������ܹ�ְ��ƥ�䣬��������ƥ��
/*****************************************************************************/
PUBLIC int32 MMIAPIPB_BcdSmartCmp(const MMIPB_BCD_NUMBER_T *bcd_number1, const MMIPB_BCD_NUMBER_T *bcd_number2)
{
	return MMIPB_BcdSmartCmp(bcd_number1, bcd_number2);
}

/*****************************************************************************/
//  Description : ��bcd��ʽ�ĵ绰����ת��Ϊ�ַ���
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
//  Description : ���ַ���ת��Ϊbcd��ʽ�ĵ绰����
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
//  Description : �����ϵ����Ϣ
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_GetContactInfo(uint16 entry_id, uint16 storage_id, MMIPB_CONTACT_T* contact_ptr)
{
    return MMIPB_GetContactInfo(entry_id, storage_id, contact_ptr);
}

/*****************************************************************************/
//  Description : ��ͨ��ϵ�˵Ļ�����Ϣ
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_CONTACT_BASE_INFO_T MMIAPIPB_GetContactBaseInfo(uint16 index)
{
    return MMIPB_GetContactBaseInfo(index);
}


/*****************************************************************************/
//  Description : ��ÿ��е�entry_id
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetFreeContactId		//RETURN: ���е�entry_id
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
//  Description : ����ֻ��м�¼���ܸ���
//  Global resource dependence : g_mmipb_nv_t
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC uint16 MMIAPIPB_GetPhoneContactCount(void)
{
    return MMIPB_GetContactCount(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT));
}
/*****************************************************************************/
//  Description : ���SIM���к�����ܸ���
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
//  Description :   ��ñ�������
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
//  Description : �򿪴��ڣ���ʾPBȫ����¼
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
// 	Description : ȡ��sim���ض������б�Ĵ�������
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
//  Description : �򿪱����������ô���
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
//  Description :��ȡMSISD�Ĕ�Ŀ
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetMSISDNum(void)
{
    return MMIPB_GetMSISDNum();
}

/*****************************************************************************/
//  Description :��ȡMSISDNumber
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_GetMSISDNumberArray(MMIPB_BCD_NUMBER_T *number, int16 max_num)
{
    MMIPB_GetMSISDNumberArray(number, max_num);
}


/*****************************************************************************/
//  Description :��ȡָ��SIM��MSISDNumber
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_GetMSISDNumberArrayBySim(MMIPB_BCD_NUMBER_T *number,MN_DUAL_SYS_E dual_sys, int16 max_num)
{
    MMIPB_GetMSISDNumberArrayBySim(number, dual_sys, max_num);
}
/*****************************************************************************/
//  Description : �жϵ绰�����Ƿ���Ч
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
//  Description : ���applet������ѡ���б�Ľڵ���,������group
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC uint16  MMIAPIPB_GetSelectCount(MMI_HANDLE_T handle)
{
    return MMIPB_GetSelectCount(handle);
}

/*****************************************************************************/
//  Description : ��ú���Ϊ��ͨ��ʽʱ����󳤶�
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
//  Note:str_pos�Ŀռ�����ǵ����߷����
//  Return:��str_pos���ظ���PB���趨�Ĳ����㷨��string2��string1��ƥ����ַ���λ������
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_Wstrstrpos(const MMI_STRING_T * string1,//IN
								   const MMI_STRING_T * string2,//IN
								   MMI_STRING_T       * str_pos,//OUT
								   BOOLEAN             is_char//TRUE:�ַ����ң�FALSE�����ֲ���
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
            //�ȴӸ��ֵ�ͷƥ��
            if(MMIAPICOM_Wstrncmp(&letters_str->full_letters[letters_str->full_letters_len], 
                &lower_string.wstr_ptr[full_cmp_len], 
                MIN(lower_string.wstr_len - full_cmp_len, full_letter_len)))
            {
                //��ƥ�䣬���¿�ʼ
                full_cmp_len = 0;
                str_pos->wstr_len = 0;
            }
            else
            {
                full_cmp_len += MIN(lower_string.wstr_len - full_cmp_len, full_letter_len);            
                str_pos->wstr_ptr[str_pos->wstr_len] = i;
                str_pos->wstr_len++;
            }
            //��ͷƥ�䲻�ɹ������м�ƥ��
            if(full_cmp_len == 0 && full_letter_len >1)
            {
                //���ܴ��м�ƥ��
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
            if((!is_punct && pre_is_punct) //ǰһ���Ƿ��ţ�������ַ������������ַ�            
                ||full_letter_len > 1)//����Ƿ����֣���һ���ַ�����Ϊ���ַ�
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
                        //��ƥ�䣬���¿�ʼ
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
                //ƥ���ַ�ƥ�����
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
//  Description : �첽������Ĵ���
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
// 	Description : ����������pb�����ӿ�
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
// 	Description : ����������pb�����ӿ�
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
// 	Description : ����������pb�����ӿ�
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
// Description : ���������Դ洢��contacts����Ŀ
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
//  Description : ���ݼ�¼uid ������Ӧ��¼��entry_id��storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetcontactIdByUid(uint32 uid, uint16* entry_id_ptr, uint16* storage_id_ptr)
{
    return MMIPB_GetcontactIdByUid(uid, entry_id_ptr, storage_id_ptr);
}


/*****************************************************************************/
//  Description : ���ݼ�¼uid ������Ӧ��¼��entry_id��storage_id
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
//  Description :��ϵ��ͬ������ʼ��pbap�����б�
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
//  Description :��ϵ��ͬ������ʼ��pbap�����б�
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
//  Description : ���ȫ��������ϵ��
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:���ⲿ�ӿڵ���
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
//  Description : �������timer
//  Global resource dependence : 
//  Author:allen.lee
//  Note:���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC void MMIAPIPB_StopPBAPTimer(void)
{
    MMIPB_StopPBAPTimer();
}
#endif