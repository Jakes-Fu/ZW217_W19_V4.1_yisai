/****************************************************************************
** File Name:      mmipb_testcase.c                                          *
** Author:                                                                 *
** Date:           12/01/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file deal with all test case code in pb             *
**                 pbÄ£¿éËùÓÐµÄ²âÊÔÓÃÀý
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/2011        baokun yin         Create
** 
****************************************************************************/
#define _MMIPB_STORAGE_C_  

#define MMIPB_FUNCTION_TEST_MODE

#if defined(MMIPB_FUNCTION_TEST_MODE) && defined (WIN32)
/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmipb_storage.h"
#include "mmipb_nv.h"
#include "mmifmm_export.h"
#include "mmipb_text.h"
#include "mmi_text.h"
#include "mmipb_interface.h"
#include "mmipb_export.h"
#include "mmipb_search.h"
#include "sig_code.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


#ifdef CMCC_VCARD_SUPPORT
#define SIM_MAX_NUM 100
PHONEBOOK_ADDITIONAL_ENTRY_T s_sim_addtion_entry[SIM_MAX_NUM] = {0};
PHONEBOOK_ENTRY_T  s_sim_adn_entry[SIM_MAX_NUM] = {0};
#endif
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/   

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

LOCAL void readcallback(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    MMIPB_ACCESS_POINT_T access_point = {0};
     MMIPB_HANDLE_NODE_T mmipb_handle_node = {0};
   
     MMIPB_GetHandleContent(handle, &mmipb_handle_node);
    

    if(mmipb_handle_node.data_ptr != PNULL)
    {
        SCI_FREE(mmipb_handle_node.data_ptr);
        mmipb_handle_node.data_ptr = PNULL;
    }
    MMIPB_CloseHandle(handle);


}
LOCAL void writecallback(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    MMIPB_ACCESS_POINT_T access_point = {0};
     MMIPB_HANDLE_NODE_T mmipb_handle_node = {0};
   
     MMIPB_GetHandleContent(handle, &mmipb_handle_node);    
    //MMIPB_InsertContactToList((MMIPB_CONTACT_T *)mmipb_handle_node_ptr->data_ptr);
    if(mmipb_handle_node.data_ptr != PNULL)
    {
        SCI_FREE(mmipb_handle_node.data_ptr);
        mmipb_handle_node.data_ptr = PNULL;
    }
    MMIPB_CloseHandle(handle);
}
LOCAL void deletecallback(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    MMIPB_ACCESS_POINT_T access_point = {0};
     MMIPB_HANDLE_NODE_T mmipb_handle_node = {0};
   
     MMIPB_GetHandleContent(handle, &mmipb_handle_node);  
    
    //MMIPB_RemoveContactFromList(mmipb_handle_node_ptr->access_point.contact.contact_id, 
      //                          mmipb_handle_node_ptr->access_point.contact.storage_id);
    if(mmipb_handle_node.data_ptr != PNULL)
    {
        SCI_FREE(mmipb_handle_node.data_ptr);
        mmipb_handle_node.data_ptr = PNULL;
    }
    MMIPB_CloseHandle(handle);
}

#ifdef CMCC_VCARD_SUPPORT
////////////usim test
/*****************************************************************************
 	Description : read  aas usim
	Global resource dependence : mnsim_context_g
       Author:       bin.li
       Modified by: Eddie.Wang
	Note:
*****************************************************************************/
uint8 PHONEBOOK_ReadUsimAasEx (
                    MN_DUAL_SYS_E  dual_sys,
				    uint8 rec_num,
				    PHONEBOOK_AAS_ENTRY_T *aas_entry_ptr
				    )
{
    uint32 i = 0;
   if(aas_entry_ptr != PNULL)
   {
       for(i = 0; i < PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN; i++)
       {
           aas_entry_ptr->alpha[i]= 'a' + i;
       }
       aas_entry_ptr->alpha_len = PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN;
   }
   return rec_num;
}

/*****************************************************************************
**Description : Update ADN and all its accociated additional info. such as ANR, SNE and EMAIL	*
**Global resource dependence : mnsim_context_g									*
**Author:       Eddie.Wang														*
**Note:																		*
*****************************************************************************/
uint8 PHONEBOOK_Update3GPhonebookEx (
                MN_DUAL_SYS_E                   dual_sys,
			    PHONEBOOK_ENTRY_T      			*entry_ptr,
			    BOOLEAN							anr_flag[MN_MAX_USIM_ANR_NUM],
			    BOOLEAN							email_flag,
			    BOOLEAN                         sne_flag,
			    BOOLEAN							adn_flag,
			    PHONEBOOK_ADDITIONAL_ENTRY_T	*additional_entry_ptr
			    )
{
    APP_MN_UPDATE_USER_INFO_CNF_T msg = {0};
    APP_MN_UPDATE_USER_INFO_CNF_T *sig_ptr = PNULL;
    uint32 i = 0;
    static uint32 s_id = 1;



    if(entry_ptr->entry_id)
    {
        msg.adn_entry_id = entry_ptr->entry_id;
    }
    else
    {
      msg.adn_entry_id = MMIPB_GetFreeContactId(PB_GROUP_SIM_1);
    }

    if(msg.adn_entry_id > SIM_MAX_NUM)
    {
        return ERR_MNDATAMAG_OUT_OF_RANGE;
    }
    SCI_MEMCPY(&s_sim_adn_entry[msg.adn_entry_id -1], entry_ptr, sizeof(PHONEBOOK_ENTRY_T));
    s_sim_adn_entry[msg.adn_entry_id -1].entry_id = msg.adn_entry_id;
    SCI_MEMCPY(&s_sim_addtion_entry[msg.adn_entry_id -1], additional_entry_ptr, sizeof(PHONEBOOK_ADDITIONAL_ENTRY_T));
    s_sim_addtion_entry[msg.adn_entry_id -1].entry_id = msg.adn_entry_id;
    msg.dual_sys = dual_sys;
    msg.update_result.adn_entry_id = msg.adn_entry_id;
    msg.update_result.adn_flag = adn_flag;//;
    msg.update_result.adn_result = 0;
    for(i  =0; i < 3;i++)
    {
        msg.update_result.anr_flag[i] = anr_flag[i];
        msg.update_result.anr_result[i] = OPERATE_SUCCESS;
    }
    msg.update_result.email_flag = email_flag;
    msg.update_result.email_result = 0;
    msg.update_result.sne_flag = sne_flag;
    msg.update_result.sne_result = 1;
    MMI_CREATE_SIGNAL(sig_ptr,
        APP_MN_UPDATE_USER_INFO_CNF,
        sizeof(APP_MN_UPDATE_USER_INFO_CNF_T),
        SCI_IdentifyThread()
        );
    SCI_MEMCPY(&sig_ptr->update_result, &msg.update_result, sizeof(msg.update_result));  
    sig_ptr->adn_entry_id = msg.adn_entry_id;
    sig_ptr->dual_sys = dual_sys;
    MMI_SEND_SIGNAL(sig_ptr, P_APP);
    return ERR_MNDATAMAG_NO_ERR;
}
/*****************************************************************************
**Description : Delete ADN and all its accociated additional info. such as ANR, SNE and EMAIL  *
**Global resource dependence : mnsim_context_g                  *
**Author:       Eddie.Wang                            *
**Note:      Add additional info. flag on 23/06/2010. TRUE: need to delete; FALSE: dont delete due to
                the specific field is invalid  already in SIM                            *
*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_Delete3GPhonebookEx (
        MN_DUAL_SYS_E       dual_sys,
        BOOLEAN             anr_flag[MNSIM_ANR_MAX_NUM],
        BOOLEAN             email_flag,
        BOOLEAN             sne_flag,
        uint32              entry_id
        )
{
    APP_MN_UPDATE_USER_INFO_CNF_T msg = {0};
    APP_MN_DEL_USER_INFO_CNF_T *sig_ptr = PNULL;
    uint32 i = 0;
    
    msg.adn_entry_id = entry_id;
    msg.dual_sys = dual_sys;
    msg.update_result.adn_entry_id = entry_id;
    msg.update_result.adn_flag = TRUE;
    msg.update_result.adn_result = 0;

    SCI_MEMSET(&s_sim_addtion_entry[entry_id -1], 0x00, sizeof(PHONEBOOK_ADDITIONAL_ENTRY_T));
    for(i  =0; i < 3;i++)
    {
        msg.update_result.anr_flag[i] = anr_flag[i];
        msg.update_result.anr_result[i] = 0;
    }
    msg.update_result.email_flag = email_flag;
    msg.update_result.email_result = 0;
    msg.update_result.sne_flag = sne_flag;
    msg.update_result.sne_result = 1;
    MMI_CREATE_SIGNAL(sig_ptr,
        APP_MN_DEL_USER_INFO_CNF,
        sizeof(APP_MN_DEL_USER_INFO_CNF_T),
        SCI_IdentifyThread()
        );
    SCI_MEMCPY(&sig_ptr->update_result, &msg.update_result, sizeof(msg.update_result));  
    sig_ptr->adn_entry_id = entry_id;
    sig_ptr->dual_sys = dual_sys;
    MMI_SEND_SIGNAL(sig_ptr, P_APP);
    return ERR_MNDATAMAG_NO_ERR;

}

/*****************************************************************************/
// 	Description : get phonebook specify entry 
//	Global resource dependence : none
//  Author:       fancier.fan
//  Modify:       Tracy Zhang
//	Note:         
/*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_ReadUSIMPhonebookEntry(   // false if no entry about the id
    MN_DUAL_SYS_E       dual_sys,       // IN
    PHONEBOOK_STORAGE_E storage,    // IN:
    PHONEBOOK_ENTRY_T   *entry_ptr  // IN/OUT:entry data
)
{
	if(entry_ptr != PNULL && entry_ptr->entry_id < SIM_MAX_NUM)
    {
        SCI_MEMCPY(entry_ptr, &s_sim_adn_entry[entry_ptr->entry_id -1], sizeof(PHONEBOOK_ENTRY_T));
        return ERR_MNDATAMAG_NO_ERR;

    }
	return ERR_MNDATAMAG_IS_NULL;
}

/*****************************************************************************
**Description : Read additional user info. such as ANR, SNE and EMAIL						*
**Global resource dependence : mnsim_context_g									*
**Author:       Eddie.Wang														*
**Note:																		*
*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_ReadUsimAdditionalEntryEx (
                MN_DUAL_SYS_E   dual_sys,
				uint32	        entry_id
				)
{
    
    APP_MN_READ_ADDITIONAL_ENTRY_CNF_T msg ={0};
    APP_MN_READ_ADDITIONAL_ENTRY_CNF_T *sig_ptr = PNULL;
    uint32 i = 0;
    static uint32 s_id = 1;

    if(entry_id == 0)
    {
        msg.add_info.adn_entry_id = s_id++;        
    }
    else
    {
        msg.add_info.adn_entry_id = entry_id;        
    }
    if(entry_id > SIM_MAX_NUM)
    {
        return ERR_MNDATAMAG_OUT_OF_RANGE;
    }
    for(i = 0; i < MIN(MN_MAX_USIM_ANR_NUM, (MMIPB_MAX_NV_PHONE_NUM -1)); i++)
    {
        msg.add_info.anr_number[i] = s_sim_addtion_entry[entry_id -1].anr_number[i];
        msg.add_info.anr_ret[i] = TRUE;
    }
    msg.add_info.email_addr = s_sim_addtion_entry[entry_id -1].email_addr;
    msg.add_info.email_ret = TRUE;
    
    msg.add_info.sne_alpha = s_sim_addtion_entry[entry_id -1].sne_alpha;
    msg.add_info.sne_ret = s_sim_addtion_entry;
    MMI_CREATE_SIGNAL(sig_ptr,
        APP_MN_READ_ADDITIONAL_ENTRY_CNF,
        sizeof(APP_MN_READ_ADDITIONAL_ENTRY_CNF_T),
        SCI_IdentifyThread()
        );
    SCI_MEMCPY(&sig_ptr->add_info, &msg.add_info, sizeof(msg.add_info));  
    sig_ptr->adn_entry_id = entry_id;
    sig_ptr->dual_sys = dual_sys;
    MMI_SEND_SIGNAL(sig_ptr, P_APP);
    return ERR_MNDATAMAG_NO_ERR;
}
/*****************************************************************************
*    Description : Find out all entries in entire Phonebooks by ALPHA
*    Global resource dependence : mnsim_context_g
*    Author:       Eddie.Wang
*    Note:         Support SIM storage only
*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_FindPhonebookEntryIDsByAlphaEx(
    MN_DUAL_SYS_E           dual_sys,
    PHONEBOOK_STORAGE_E     storage,        /* IN:*/
    MN_STRING_T             *alpha_ptr,     /* IN:*/
    uint16                  *entry_ids,     /* IN/OUT: entry ids*/
    uint16                  *no_entry_ids    /* IN/OUT: number of entry ids, [IN]must greater or equal to used_rec_no.*/
    )
{
    
}

/*****************************************************************************
 	Description :  is usim or sim
	Global resource dependence : none
       Author: bin.li
	Note:
*****************************************************************************/
BOOLEAN MNSIM_Is_UsimEx(
                      MN_DUAL_SYS_E dual_sys
                      )
{
    return TRUE;
}

/*****************************************************************************
**Description : Retrive VCARD space status in curent USIM            *
**Global resource dependence : mnsim_context_g                  *
**Author:     Eddie.Wang                            *
**Note:    Currently spport ADN EXT1 SNE ANR and EMAIL                                 *
*****************************************************************************/
ERR_MNDATAMAG_CODE_E PHONEBOOK_Get3GPhonebookStatusEx(
                        MN_DUAL_SYS_E  dual_sys,
                        PHONEBOOK_VCARD_STATUS_T *vcard_status
                        )
{
    PHONEBOOK_EF_USAGE_T adn_usage = {8,8,8};
    PHONEBOOK_EF_USAGE_T anr_usage[MNSIM_ANR_MAX_NUM] = {{8,8,8}, {8,8,8}, {8,8,8}};
    PHONEBOOK_EF_USAGE_T email_usage = {8,8,8};
    PHONEBOOK_EF_USAGE_T sne_usage  = {8,8,8};
    PHONEBOOK_EF_USAGE_T ext1_usage = {8,8,8};
    int32 i = 0;

    if(vcard_status != PNULL)
    {
        vcard_status->storage_num = 3;
        for(i  = 0; i < 3; i++)
        {
            vcard_status->storage_usage[i].adn_usage =adn_usage;
            SCI_MEMCPY(&vcard_status->storage_usage[i].anr_usage, &anr_usage, sizeof(anr_usage));
            vcard_status->storage_usage[i].email_usage = email_usage;
            vcard_status->storage_usage[i].ext1_usage = ext1_usage;
            vcard_status->storage_usage[i].sne_usage = sne_usage;
        }
    }

    return ERR_MNDATAMAG_NO_ERR;
    
}

/*****************************************************************************
**Description : check additonal entries' space            *
**Global resource dependence : mnsim_context_g                  *
**Author:     Eddie.Wang                            *
**Note:        for MMI b4 update add. enties                           *
*****************************************************************************/
uint8 PHONEBOOK_AppCheckAddSpaceEx(
                        MN_DUAL_SYS_E  dual_sys,
                        uint8 add_preop)
{
    return 15;
}

PUBLIC void MMIPB_TestUsim(void)
{
    int32 i  = 0;
    MMIPB_ACCESS_POINT_T access_point = {0};
    MMIPB_CONTACT_T      contact = {0};
    MMIPB_HANDLE_T handle = 0;
    wchar buf[300] = {0};

//     for(i = 0; i< 3; i++)
//     {
//         MMIPB_ReadUSIMAddtionalAsyn(MN_DUAL_SYS_1, 1);
//     }

    access_point.contact.contact_id = 0;
    access_point.contact.storage_id = MMIPB_STORAGE_SIM1;
    contact.union_t.group = 1 << PB_GROUP_SIM_1;

   /* for(i =0; i < 10; i++)
    {
        MMIAPICOM_Wstrncpy(contact.name.wstr, L"²âÊÔ", 2);
        contact.name.wstr[2] = 't' + i;
        contact.name.wstr_len = 3;
        MMIAPICOM_Wstrncpy(contact.address.wstr, L"²âÊÔaddress", 9);
        contact.address.wstr_len = 9;
        contact.number[0].npi_ton = 1;
        contact.number[0].number[0] = 0x21; 
        contact.number[0].number[0] = 0x43;
        contact.number[0].number_len = 2;
        contact.num_type[0] = 1;
        contact.number[4].npi_ton = 1;
        contact.number[4].number[0] = 0x55; 
        contact.number[4].number[0] = 0x66;
        contact.number[4].number_len = 2+ i;
        contact.num_type[4] = 5;
        contact.mail.wstr_len = MMIAPICOM_Wstrlen(L"test@163.com");
        contact.contact_id = MMIPB_GetFreeContactId(PB_GROUP_SIM_1);
        contact.storage_id = access_point.contact.storage_id;
        MMIAPICOM_Wstrncpy(contact.mail.wstr, L"test@163.com", contact.mail.wstr_len);
        MMIPB_Write(MMIPB_ACCESS_CONTACT, access_point, sizeof(contact), &contact);           
    }  
    */
    access_point.contact.contact_id = 1;
    access_point.contact.storage_id = MMIPB_STORAGE_SIM1;
    SCI_MEMSET(&contact, 0x00, sizeof(contact));
   // MMIPB_Read(MMIPB_ACCESS_CONTACT, access_point, sizeof(contact), &contact);

    //handle = MMIPB_CreateHandle(MMIPB_OPC_ADD, 0);
    //MMIPB_DeleteAsyn(handle, MMIPB_ACCESS_CONTACT, access_point, PNULL);
    //handle = MMIPB_CreateHandle(MMIPB_OPC_COPY, 0);
    //contact.contact_id = MMIPB_GetFreeContactId(PB_GROUP_SIM_1);
    //MMIPB_CopyContactAsyn()
    //MMIPB_WriteAsyn(handle, MMIPB_ACCESS_CONTACT, access_point,  &contact, sizeof(contact),PNULL);
    //aas
    access_point.aas_index_t.dual_sys = MN_DUAL_SYS_1;
    access_point.aas_index_t.aas_index = 1;
    //MMIPB_Read(MMIPB_ACCESS_USIM_AAS, access_point, sizeof(buf), buf);
    //MMIPB_Write(MMIPB_ACCESS_USIM_AAS, access_point, sizeof(buf), buf);
    //MMIPB_Delete(MMIPB_ACCESS_USIM_AAS, access_point);
    //MMIPB_ReadUSIMAddtionalFromContactList(                                       
     //                                          1,
     //                                           MMIPB_STORAGE_SIM1,
      //                                          &contact
       //                                        );
}
#endif
#endif