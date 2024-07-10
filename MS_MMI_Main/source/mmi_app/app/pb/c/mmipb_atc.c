/*****************************************************************************
** File Name:      mmipb_atc.c                                               *
** Author:         Tracy Zhang                                               *
** Date:           15/11/2004                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe mmi phonebook files that    *
**                 sync witch pc.                                            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2004        Tracy Zhang      Create
******************************************************************************/

#define _MMIPB_ATC_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_pb_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
//#include <math.h>
#ifdef MMI_SYNCTOOL_V3
#include "mmipb_atc.h"
//#include "mmipb_atc.h"
//#include "mmipb_mnapi.h"
#include "mmipb_app.h"
//#include "mmipb_internal.h"

#include "mmi_common.h"
#include "mmi_nv.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmk_app.h"
#include "mmi_appmsg.h"
#include "mmipb_id.h"
#include "mmi_custom_define.h"
#include "mmipb_view.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//pb
#define PB_MAX_MAIL_LEN 	        MMIPB_MAX_MAIL_LEN //30
#define PB_MAX_NV_PHONE_NUM	        MMIPB_MAX_NV_PHONE_NUM

//group
#define	GROUP_NAME_MAX_LEN		    MMIPB_GROUP_NAME_MAX_LEN//20
/*
PB_GROUP_LIST_E pb_sync_sim_ids[] = {
         PB_GROUP_SIM_1,         //SIM卡1
#ifndef MMI_MULTI_SIM_SYS_SINGLE
         PB_GROUP_SIM_2,             //SIM卡2
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
         PB_GROUP_SIM_3,             //SIM卡3
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
         PB_GROUP_SIM_4,             //SIM卡4
#endif
#endif
        };
*/

LOCAL uint8 pb_sim_pos[] = {
        PB_SYNC_POS_SIM_1,           
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        PB_SYNC_POS_SIM_2,           
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        PB_SYNC_POS_SIM_3,         
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        PB_SYNC_POS_SIM_4,     
#endif
#endif
        };

  
      
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : pb upate callback
//	Global resource dependence : none
//  Author:      baokun
//	Note:
/*****************************************************************************/
LOCAL void update_callback(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error);

/*****************************************************************************/
// 	Description : 复制同步工具的电话簿记录到要保存的电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
LOCAL void CopySyncEntryToStoreEntry
(
    MMIPB_CONTACT_T     *pb_contact_ptr,      //OUT:
    PB_PHONEBOOK_ENTRY_T        *pb_item_ptr   //IN:
);


 /**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 提供电话簿信息
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
BOOLEAN MMIPB_SyncHeadInfo
(
    PB_HEAD_INFO_T  *pb_head_info_ptr,  //OUT:
    uint8	 	    pos           //IN:
)
{
    uint16  byte_num = 0;

    if(PNULL == pb_head_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] PNULL == pb_head_info_ptr !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_113_112_2_18_2_43_17_162,(uint8*)"");
        return FALSE;
    }
    if (!MMIPB_IsPbReady())
    {
        return (FALSE);
    }
    //pb_ptr = &g_mmipb_entry_list;
    
    pb_head_info_ptr->is_valid          = TRUE;
    pb_head_info_ptr->max_num_len       = MIN(MMIPB_NUMBER_MAX_STRING_LEN, MMI_SYNC_PB_NUM_MAX_LEN);
    pb_head_info_ptr->max_alpha_len = MIN(MMIPB_NAME_MAX_LEN, MMI_SYNC_PB_NAME_MAX_LEN);
    if(PB_SYNC_POS_ME == pos)
    {
        pb_head_info_ptr->max_email_len     = MIN(MMIPB_MAX_MAIL_LEN, MMI_SYNC_PB_MAX_MAIL_LEN);
        pb_head_info_ptr->max_memo_len = MMIPB_MAX_CUSTOM_TEXT_LEN;
        pb_head_info_ptr->max_record_num = MIN(MMIPB_GetContactStorageSpace(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT)), MMI_SYNC_PB_MAX_ENTRY_NUM);
        pb_head_info_ptr->used_record_num   = MIN(MMIPB_GetContactCount(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT)), MMI_SYNC_PB_MAX_ENTRY_NUM);//pb_ptr->sort_t.count;
    }
    else 
    {
        uint32 i = MN_DUAL_SYS_1;
        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if(pb_sim_pos[i] == pos)  
            {
                pb_head_info_ptr->max_record_num = MMIPB_GetMaxContactNumOfSim(i);
                pb_head_info_ptr->used_record_num   = MMIPB_GetContactNumOfGroup(pb_sim_pos[i]);
                break;
            }
        }       
    }

    SCI_MEMSET(pb_head_info_ptr->all_entry_id, 0x0, sizeof(pb_head_info_ptr->all_entry_id));
	if(PB_SYNC_POS_ME == pos)
	{
		byte_num = MMIPB_GetMaxContactNumOfNV()>> 3;
		if (0 < MMIPB_GetMaxContactNumOfNV()% 8) 
		{
			byte_num++;
		}        
		MMIPB_GetContactIdUsedInfoOfNV((uint8 *)pb_head_info_ptr->all_entry_id,
					MIN(byte_num, (MMI_SYNC_PB_MAX_ENTRY_NUM/8))
					);
	}
	else
	{
        uint32 i = MN_DUAL_SYS_1;
        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if(pb_sim_pos[i] == pos)  
            {
                MMIPB_GetContactIdUsedInfoOfSIM(i, 
                                                pb_head_info_ptr->all_entry_id,
                                                MMIPB_GetMaxContactNumOfSim(i)
                                                );
                
                break;
            }
        }		
	}
    return (TRUE);
}


/*****************************************************************************/
// 	Description : 读电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
BOOLEAN MMIPB_SyncRead
(
    PB_PHONEBOOK_ENTRY_T     *pb_item_ptr,  //OUT:
    uint8	 	    pos           //IN:
)
{
    uint16                   i = 0;
    BOOLEAN                 result = FALSE;
    MMIPB_ERROR_E res = MMIPB_ERROR_SUCCESS;
    
    uint16     storage_id = 0;
    MMIPB_CONTACT_T *pb_contact_ptr = PNULL;
    MMIPB_STRING_T          *num_string_ptr = PNULL;

    if(PNULL == pb_item_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_SyncRead  PNULL == pb_item_info_ptr !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_215_112_2_18_2_43_17_164,(uint8*)"");
        return FALSE;
    }
    //    SCI_ASSERT(0 < pb_item_info_ptr->entry_id);
    pb_contact_ptr = (MMIPB_CONTACT_T*)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == pb_contact_ptr)
    {
    	return FALSE;
    }
    num_string_ptr = (MMIPB_STRING_T *)SCI_ALLOCA(sizeof(MMIPB_STRING_T));
    if(PNULL == num_string_ptr)
    {
        SCI_FREE(pb_contact_ptr);
    	return FALSE;
    }

    SCI_MEMSET(pb_contact_ptr, 0, sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(num_string_ptr, 0, sizeof(MMIPB_STRING_T));

    if(0 == pb_item_ptr->entry_id)
    {
        SCI_FREE(num_string_ptr);
    	SCI_FREE(pb_contact_ptr);
    	result = FALSE;
    }

    else if (!MMIPB_IsPbReady())
    {
        SCI_FREE(num_string_ptr);
    	SCI_FREE(pb_contact_ptr);
    	result = FALSE;
    }
    else
    {
        if(PB_SYNC_POS_ME== pos)
        {
            storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);	
            //SCI_TRACE_LOW:"MMIPB_SyncRead, read from nv"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_249_112_2_18_2_43_17_165,(uint8*)"");
        }
        else
        {
            uint32 dual_sys = MN_DUAL_SYS_1;
            for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
            {
                if(pb_sim_pos[dual_sys] == pos)  
                {
                    storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1+dual_sys, PHONEBOOK_SIM_STORAGE);
                    break;
                }
            }
        }	

    
       res = MMIAPIPB_GetContactInfo(pb_item_ptr->entry_id,
        storage_id,
        pb_contact_ptr
        );
	
	if (MMIPB_ERROR_SUCCESS == res)
	{
	        result = TRUE;
            pb_item_ptr->entry_id   = pb_contact_ptr->contact_id;
        	
        	//copy name
  /*          MMIPB_WstrToInternalString(pb_contact_ptr->name.wstr,
                pb_contact_ptr->name.wstr_len,
                 pb_item_info_ptr->entry_name.alpha, 
                 &(pb_item_info_ptr->entry_name.alpha_len), 
                 &pb_item_info_ptr->entry_name.is_ucs2,
                 MMI_SYNC_PB_NAME_MAX_LEN
                 );       	
                 */
                 MMIAPICOM_Wstrncpy(pb_item_ptr->entry_name.alpha,pb_contact_ptr->name.wstr,
                 	MIN(pb_contact_ptr->name.wstr_len,MMI_SYNC_PB_NAME_MAX_LEN));
		pb_item_ptr->entry_name.alpha_len = pb_contact_ptr->name.wstr_len;
			
#ifdef MMIPB_MAIL_SUPPORT            
        	// copy mail
         /*   MMIPB_WstrToInternalString(pb_contact_ptr->mail.wstr,
                pb_contact_ptr->mail.wstr_len,
                 (char*)pb_item_ptr->mail.mail, 
                 &(pb_item_ptr->mail.mail_len),
                 &pb_item_ptr->mail.is_ucs2,
                 MMI_SYNC_PB_MAX_MAIL_LEN
                 );   
                 */
                 MMIAPICOM_Wstrncpy(pb_item_ptr->mail.mail,pb_contact_ptr->mail.wstr,
                 	MIN(pb_contact_ptr->mail.wstr_len,MMI_SYNC_PB_MAX_MAIL_LEN));
		        pb_item_ptr->mail.mail_len= pb_contact_ptr->mail.wstr_len;
#endif
        	// copy phonenumber
            for (i=0; i<PB_MAX_NV_PHONE_NUM; i++)
            {
                pb_item_ptr->number_t[i].number_type = pb_contact_ptr->num_type[i];
                num_string_ptr->strlen = MMIAPICOM_GenDispNumber(
        			MMIPB_GetNumberTypeFromUint8(pb_contact_ptr->number[i].npi_ton),/*lint !e661*/
        			pb_contact_ptr->number[i].number_len,/*lint !e661*/
        			pb_contact_ptr->number[i].number,/*lint !e661 !e662*/
        			num_string_ptr->strbuf,
        			MMIPB_MAX_STRING_LEN+2
        			);
                pb_item_ptr->number_t[i].number_len = num_string_ptr->strlen;
                SCI_MEMCPY(
        			pb_item_ptr->number_t[i].number,
        			num_string_ptr->strbuf,
        			num_string_ptr->strlen
        			);
        	}
        	 pb_item_ptr->group = pb_contact_ptr->union_t.group;
	    }		
    }
    SCI_FREE(num_string_ptr);
    SCI_FREE(pb_contact_ptr);
    return (result);
}

/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:   xiaohua.liu
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_GetPbGroupIdFromStr(PB_PHONEBOOK_ENTRY_T *pb_entry_ptr,char* begin_pos,char* end_pos)
{
	char group_str[20] = {0};
	char temp_buf[20] = {0};
	uint16 group_id = 0;
	uint8 infor_len = 0;
	uint8 tag_len;
//	uint16 sim_num = sizeof(pb_sync_sim_ids)/sizeof(pb_sync_sim_ids[0]);

	if(PNULL == pb_entry_ptr||PNULL == begin_pos||PNULL ==end_pos)
	{
		return;
	}
		infor_len = end_pos - begin_pos;
            tag_len = SCI_STRLEN(PB_GROUP_TAG);
        //    begin_pos = cmd+tag_len;
            strncpy(group_str,begin_pos+tag_len,infor_len-tag_len);
		if(SCI_STRLEN(group_str)>1)
		{
			begin_pos = group_str;
			end_pos = strstr(begin_pos,(CHAR *)MMI_COMMA_SYMBOL);
			while(PNULL!=end_pos&&end_pos>begin_pos)
			{
				SCI_MEMSET(temp_buf,0,sizeof(temp_buf));
				infor_len = end_pos - begin_pos;
				strncpy(temp_buf,begin_pos,infor_len);
				group_id = (PB_GROUP_LIST_E)atoi(temp_buf);
				if(group_id>=PB_GROUP_HOME)//pb_sync_sim_ids[sim_num-1])
				{	
                    			pb_entry_ptr->group |= 1<<group_id;
				}
				end_pos++;
                    		begin_pos=end_pos;
                    		end_pos = strstr(begin_pos,(CHAR *)MMI_COMMA_SYMBOL);
                	}
			group_id = (PB_GROUP_LIST_E)atoi(begin_pos);
			if(group_id>=PB_GROUP_HOME)//pb_sync_sim_ids[sim_num-1])
			{	
				pb_entry_ptr->group |= 1<<group_id;
			}
            	}
		else
		{
			group_id = (PB_GROUP_LIST_E)atoi(group_str);
                	if(group_id>=PB_GROUP_HOME)//pb_sync_sim_ids[sim_num-1])
			{	
				pb_entry_ptr->group |= 1<<group_id;
			}
		}
           
        }	
/*****************************************************************************/
// 	Description : 添加电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
MMIPB_ERROR_E MMIPB_SyncAdd
(
    uint8	 	    pos,           //IN:
    PB_PHONEBOOK_ENTRY_T    *pb_item_ptr,  //IN:
    uint16* contact_id_ptr
)
{
    MMIPB_ERROR_E res = MMIPB_ERROR_ERROR;
    
    uint16     storage_id = 0;
    MMIPB_CONTACT_T *pb_contact_ptr = PNULL;
    
    if(PNULL == pb_item_ptr|| 0 == contact_id_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }

    //SCI_TRACE_LOW:"[MMIPB]MMIPB_SyncAdd pb_item_info_ptr %d entry_id !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_335_112_2_18_2_43_17_166,(uint8*)"d",pb_item_ptr->entry_id);


    
    
    if (!MMIPB_IsPbReady())
    {
        return (MMIPB_ERROR_ERROR);
    }
    pb_contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == pb_contact_ptr)
    {
    	return MMIPB_ERROR_ERROR;
    }
    SCI_MEMSET(pb_contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    
    CopySyncEntryToStoreEntry(
                                pb_contact_ptr,
                                pb_item_ptr
                                );
	
    if(PB_SYNC_POS_ME== pos)
    {
        storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
        //SCI_TRACE_LOW:"MMIPB_SyncAdd, add to nv"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_362_112_2_18_2_43_17_167,(uint8*)"");
    }
    else
    {
        uint32 i = MN_DUAL_SYS_1;		
        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if(pb_sim_pos[i] == pos)  
            {
                storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + i ,PHONEBOOK_SIM_STORAGE);
                pb_contact_ptr->union_t.group = 1 <<pb_sim_pos[i];
                          
                break;
            }
        }

    }
    pb_contact_ptr->storage_id = storage_id;


    if(MMIPB_ERROR_SUCCESS == MMIPB_GetStorageFreeSpace(pb_contact_ptr->storage_id, PNULL))
    {
    	pb_contact_ptr->contact_id = MMIPB_GetFreeContactId(pb_contact_ptr->storage_id);
	    *contact_id_ptr = pb_contact_ptr->contact_id;
		if(MMIPB_IsPhoneContact(pb_contact_ptr->storage_id))
		{
			res = MMIAPIPB_AddContact( pb_contact_ptr);
		}
		else
		{
			MMIPB_HANDLE_T handle = PNULL;
			handle = MMIPB_CreateHandle(MMIPB_OPC_ADD, 0);
			if(handle != PNULL)
			{
				res = MMIAPIPB_AddContactAsyn(handle, pb_contact_ptr, update_callback);
			}
		}
     }

    SCI_FREE(pb_contact_ptr);

    return (res);
}


/*****************************************************************************/
// 	Description : 复制同步工具的电话簿记录到要保存的电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
LOCAL void CopySyncEntryToStoreEntry
(
    MMIPB_CONTACT_T *pb_contact_ptr,      //OUT:
    PB_PHONEBOOK_ENTRY_T *pb_item_ptr   //IN:
)
{
    uint16               i = 0;
    MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};

    MN_NUMBER_PLAN_E npi = MN_NUM_PLAN_ISDN_TELE;
    
    if((PNULL == pb_contact_ptr) || (PNULL == pb_item_ptr))
    {
        //SCI_TRACE_LOW:"[MMIPB] CopySyncEntryToStoreEntry (PNULL == pb_entry_ptr) || (PNULL == pb_item_info_ptr) !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_413_112_2_18_2_43_17_168,(uint8*)"");
        return;
    }
    pb_contact_ptr->contact_id = pb_item_ptr->entry_id;

    // copy name
/*    MMIPB_InternalStringToWstr(pb_item_info_ptr->entry_name.alpha, 
        pb_item_info_ptr->entry_name.alpha_len, 
        pb_item_info_ptr->entry_name.is_ucs2,
        MMIPB_NAME_MAX_LEN,
        pb_contact_ptr->name.wstr, 
        &(pb_contact_ptr->name.wstr_len)        
        );
    */
    MMIAPICOM_Wstrncpy(pb_contact_ptr->name.wstr,pb_item_ptr->entry_name.alpha,
    	MIN(pb_item_ptr->entry_name.alpha_len,MMIPB_NAME_MAX_LEN));
	pb_contact_ptr->name.wstr_len = pb_item_ptr->entry_name.alpha_len;
    
#ifdef MMIPB_MAIL_SUPPORT
    // copy mail
 /*   MMIPB_InternalStringToWstr(pb_item_ptr->mail.mail, 
        pb_item_ptr->mail.mail_len, 
        pb_item_ptr->mail.is_ucs2,
        MMIPB_NAME_MAX_LEN,
        pb_contact_ptr->mail.wstr, 
        &(pb_contact_ptr->mail.wstr_len)
        );*/
        MMIAPICOM_Wstrncpy(pb_contact_ptr->mail.wstr,pb_item_ptr->mail.mail,
    	MIN(pb_item_ptr->mail.mail_len,MMIPB_MAX_MAIL_LEN));
	pb_contact_ptr->mail.wstr_len = pb_item_ptr->mail.mail_len;
#endif
    // copy phonenumber
    for (i=0; i<PB_MAX_NV_PHONE_NUM; i++)
    {
        if (!MMIAPICOM_GenPartyNumber(
                                pb_item_ptr->number_t[i].number,
                                pb_item_ptr->number_t[i].number_len,
                                &party_num
                                )
            )
        {
            party_num.number_type   = MN_NUM_TYPE_UNKNOW;
            party_num.num_len       = 0;
        }

        MMIPB_GSMNumberToBCDNumber(
                                    &party_num.number_type,
                                    &npi,
                                    party_num.bcd_num,
                                    (uint8)party_num.num_len,
                                    &(pb_contact_ptr->number[i])
                                    );
        pb_contact_ptr->num_type[i] = pb_item_ptr->number_t[i].number_type;
    }

	//set group id
    	pb_contact_ptr->union_t.group = pb_item_ptr->group;
//	pb_contact_ptr->union_t.group  |= 1 << PB_GROUP_ALL;

	//set custom photo , ring and text
	//pb_entry_ptr->custom_photo.is_valid = FALSE;
	//pb_entry_ptr->custom_ring.is_valid = FALSE;
#ifndef PB_SUPPORT_LOW_MEMORY
	if(pb_item_ptr->memo.note_len>0)
	{
		MMIAPICOM_Wstrncpy(pb_contact_ptr->memo.wstr,pb_item_ptr->memo.note, pb_item_ptr->memo.note_len);
		pb_contact_ptr->memo.wstr_len = pb_item_ptr->memo.note_len;
	}
#endif    
	//pb_entry_ptr->custom_text.is_valid = FALSE;

}


/*****************************************************************************/
// 	Description : 删除电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
MMIPB_ERROR_E MMIPB_SyncDelete
(
    uint8	 	pos,        //IN:
    uint16      entry_id    //IN:
)
{
    //BOOLEAN             result = FALSE;
    MMIPB_ERROR_E res = MMIPB_ERROR_ERROR;
    
    uint16  storage_id = 0;
//    MN_DUAL_SYS_E		sys = MMI_DUAL_SYS_MAX;
  //  uint16              i = 0;
   // uint16              entry_index = 0;
    
    if(0 == entry_id)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_SyncDelete entry_id %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_496_112_2_18_2_43_17_169,(uint8*)"d", entry_id);
        return MMIPB_ERROR_ERROR;
    }    
    if (!MMIPB_IsPbReady())
    {
        return (MMIPB_ERROR_ERROR);
    }
     if(PB_SYNC_POS_ME== pos)
    {
        storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
        //SCI_TRACE_LOW:"MMIPB_SyncDelete from nv"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_506_112_2_18_2_43_17_170,(uint8*)"");
		res = MMIAPIPB_DeleteContact(entry_id,storage_id);
    }
    else
    {
        uint32 dual_sys = MN_DUAL_SYS_1;
		MMIPB_HANDLE_T handle = PNULL;
        for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
        {
            if(pb_sim_pos[dual_sys] == pos)  
            {
                storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + dual_sys, PHONEBOOK_SIM_STORAGE);
                break;
            }
        }
		handle = MMIPB_CreateHandle(MMIPB_OPC_DELETE, 0);
		if(handle != PNULL)
		{
			res = MMIPB_DeleteContactAsyn(handle, entry_id,storage_id, update_callback);
		}
    }	
    
    return (res);
}


/*****************************************************************************/
// 	Description : 修改电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
MMIPB_ERROR_E MMIPB_SyncModify 
(
    uint8	 	pos,        //IN:
    PB_PHONEBOOK_ENTRY_T    *pb_item_ptr   //IN: 记录
)
{
    MMIPB_ERROR_E      res = MMIPB_ERROR_SUCCESS;
    
    //uint16			storage_id = 0;
    MMIPB_CONTACT_T		*pb_contact_ptr = PNULL;
//    uint8 group_id = 0;
	
    
    if(PNULL == pb_item_ptr || 0 == pb_item_ptr->entry_id )
    {
        //SCI_TRACE_LOW("[MMIPB]MMIPB_SyncModify entry_id %d !", pb_item_info_ptr->entry_id);
        return MMIPB_ERROR_ERROR;
    }     
    if (!MMIPB_IsPbReady())
    {
        return (MMIPB_ERROR_ERROR);
    }
    
    pb_contact_ptr = (MMIPB_CONTACT_T *) SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == pb_contact_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }
    SCI_MEMSET(pb_contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
	
    CopySyncEntryToStoreEntry(
                                pb_contact_ptr,
                                pb_item_ptr
                                );

     if(PB_SYNC_POS_ME== pos)
    {
        pb_contact_ptr->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
        //SCI_TRACE_LOW:"MMIPB_SyncModify in nv"
		res = MMIAPIPB_ModifyContact(pb_contact_ptr);
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_576_112_2_18_2_43_18_171,(uint8*)"");
    }
    else
    {
        uint32 i = MN_DUAL_SYS_1;
		MMIPB_HANDLE_T handle = PNULL; 
        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if(pb_sim_pos[i] == pos)  
            {            
                pb_contact_ptr->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + i, PHONEBOOK_SIM_STORAGE);
                    
                break;
            }
        }
		//modify sync
		handle = MMIPB_CreateHandle(MMIPB_OPC_EDIT, 0);
		if(handle != PNULL)
		{
			res = MMIAPIPB_ModifyContactAsyn(handle, pb_contact_ptr, update_callback);
		}
		else
		{
			res = MMIPB_ERROR_ERROR;
		}
    }	
    
	
    
    SCI_FREE(pb_contact_ptr);
    
    return (res);
}


/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncHeadInfo ( 
							GROUP_HEAD_INFO_T *  group_head_info_ptr  // OUT
							)
{
//	uint32  i = 0;

    if(NULL == group_head_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPBGROUP_SyncHeadInfo (NULL == group_head_info_ptr) !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_618_112_2_18_2_43_18_172,(uint8*)"");
        return FALSE;
    }
    
    group_head_info_ptr->max_name_len = GROUP_NAME_MAX_LEN;
    

    group_head_info_ptr->max_group_num = MMIPB_USER_GROUP_NUM;//MMI_SYNC_GROUP_MAX_NUM;
    group_head_info_ptr->max_group_num+=PB_GROUP_MAX;
//    group_head_info_ptr->used_num = MMIPB_SYSTEM_GROUP_NUM+MMIPB_GetUserGroupCount();
    #ifdef MMIPB_GROUP_MANAGE_SUPPORT
    MMIPB_GetUsedGroupIdInfor((uint8*)&group_head_info_ptr->all_group_id);
    #endif
    //判断群组的参数最大值是否超过了mmi at传输最大值
    if((MMI_SYNC_GROUP_MAX_NUM < group_head_info_ptr->max_group_num) || (MMI_SYNC_GROUP_NAME_MAX_LEN < group_head_info_ptr->max_name_len))
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPBGROUP_SyncHeadInfo max_group_num %d max_name_len %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_631_112_2_18_2_43_18_173,(uint8*)"dd", group_head_info_ptr->max_group_num, group_head_info_ptr->max_name_len);
        return FALSE;
    }


    return TRUE;
}


/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncRead( 
                GROUP_ENTRY_T *  group_info_ptr  // IN & OUT
							)
{

    MMIPB_GROUP_T pb_group_info = {0};
    MMIPB_ERROR_E res = MMIPB_ERROR_SUCCESS;
    BOOLEAN result = FALSE;
    
    if(PNULL == group_info_ptr)
    {
        return result;
    }
    res = MMIPB_ReadGroup(group_info_ptr->group_id,&pb_group_info);
	if(MMIPB_ERROR_SUCCESS == res&&pb_group_info.is_valid)	
    {
        if(group_info_ptr->group_id<PB_GROUP_MAX)
        {
            group_info_ptr->is_fixed_group = TRUE;
        }
        MMIAPICOM_Wstrncpy(group_info_ptr->group_name,
            pb_group_info.group_name.wstr,pb_group_info.group_name.wstr_len);
        group_info_ptr->name_len = pb_group_info.group_name.wstr_len;
        result = TRUE;
	}
	return result;
	

}


/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncAdd( 
							GROUP_ENTRY_T *group_info_ptr,  // IN
							uint32* id_out_ptr
							)
{
	uint32 group_id = 0;
	
    	MMIPB_GROUP_T pb_group_info = {0};
    	BOOLEAN result = FALSE;
	MMIPB_ERROR_E res = MMIPB_ERROR_SUCCESS;
    
    #ifdef MMIPB_GROUP_MANAGE_SUPPORT    
    if(NULL == group_info_ptr)
    {
    	return FALSE;
    }
	
    group_id = MMIPB_GetFreeUserGroupId();
    if(group_id<(MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM))
    {
        group_info_ptr->group_id = group_id;
		MMIAPICOM_Wstrncpy(pb_group_info.group_name.wstr,group_info_ptr->group_name,
          group_info_ptr->name_len);
		pb_group_info.group_name.wstr_len = group_info_ptr->name_len;

        pb_group_info.is_valid = TRUE;
        *id_out_ptr = group_id;
        res = MMIPB_SetGroupInfo(group_id,&pb_group_info);
        if(MMIPB_ERROR_SUCCESS == res)    
        {
            result = TRUE;
        }
    }
    else
    {
        result = FALSE;
    }
    #endif
    return result;
}

/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncDelete( 
							uint32           group_id  // IN
							)
{
    MMIPB_HANDLE_T pbHandle = 0;
	//do not support this function
	if(group_id<=MMIPB_SYSTEM_GROUP_NUM)
    {
        return FALSE;
	}
    else
    {
        pbHandle = MMIPB_CreateHandle(MMIPB_OPC_DELETE, 0);
        #if defined(MMIPB_GROUP_MANAGE_SUPPORT)
        MMIPB_DeleteUserGroup((MMIPB_HANDLE_T)pbHandle,group_id);
        return TRUE;
        #else
       return FALSE;
        #endif
    }
}

/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncModify( 
                            uint16           group_id,    //IN:
							GROUP_ENTRY_T *  group_info_ptr  // IN 
							)
{
    BOOLEAN result = FALSE;
    MMIPB_ERROR_E res = MMIPB_ERROR_SUCCESS;
    MMIPB_GROUP_T pb_group_info = {0};
        
    #ifdef MMIPB_GROUP_MANAGE_SUPPORT
    if(PNULL==group_info_ptr)
    {
        return FALSE;
    }
    MMIAPICOM_Wstrncpy(pb_group_info.group_name.wstr,group_info_ptr->group_name,
        group_info_ptr->name_len);
    pb_group_info.group_id = group_id;
    
    res = MMIPB_SetGroupInfo(group_id,(MMIPB_GROUP_T*)&pb_group_info);
    if(MMIPB_ERROR_SUCCESS == res)    
    {
       result = TRUE; 
    }
    else
    {
        result = FALSE;
    }
    #endif
    return result;
}

#endif

#ifdef MMI_SYNCTOOL_SUPPORT
#include "mmipb_atc.h"
#include "mmipb_atc.h"
//#include "mmipb_mnapi.h"
#include "mmipb_app.h"
//#include "mmipb_internal.h"

#include "mmi_common.h"
#include "mmi_nv.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmk_app.h"
#include "mmi_appmsg.h"
#include "mmipb_id.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//pb
#define PB_MAX_MAIL_LEN 	        40 //30
#define PB_MAX_NV_PHONE_NUM	        MMIPB_MAX_NV_PHONE_NUM //CR177009

//group
#define	GROUP_NAME_MAX_LEN		    16 //20


uint8 pb_group_ids[] = {
//        PB_GROUP_UNCLASS,       //未分类  
        PB_GROUP_HOME,			//家庭
        PB_GROUP_FRIEND,		//朋友
        PB_GROUP_COLLEAGUE,		//同事
        PB_GROUP_SCHOOLMATE,	//同学
        //PB_GROUP_BLACKLIST,		//黑名单
//        PB_GROUP_OTHER1,		//其他1
 //       PB_GROUP_OTHER2,		//其他2
 //       PB_GROUP_OTHER3,		//其他3
        };

LOCAL uint8 pb_sim_pos[] = {
        PB_SYNC_POS_SIM_1,           
        PB_SYNC_POS_SIM_2,           
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
        PB_SYNC_POS_SIM_3,         
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        PB_SYNC_POS_SIM_4,     
#endif
        };

     PB_GROUP_LIST_E pb_sim_group_ids[] = {
         PB_GROUP_SIM_1,         //SIM卡1
#ifndef MMI_MULTI_SIM_SYS_SINGLE
         PB_GROUP_SIM_2,             //SIM卡2
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
         PB_GROUP_SIM_3,             //SIM卡3
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
         PB_GROUP_SIM_4,             //SIM卡4
#endif
#endif
        };

      
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : pb upate callback
//	Global resource dependence : none
//  Author:      baokun
//	Note:
/*****************************************************************************/
LOCAL void update_callback(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error);

/*****************************************************************************/
// 	Description : 复制同步工具的电话簿记录到要保存的电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
LOCAL void CopySyncEntryToStoreEntry
(
    MMIPB_CONTACT_T     *pb_entry_ptr,      //OUT:
    PB_PHONEBOOK_ENTRY_T        *pb_item_info_ptr   //IN:
);


 /**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 提供电话簿信息
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
BOOLEAN MMIPB_SyncHeadInfo
(
    PB_HEAD_INFO_T  *pb_head_info_ptr,  //OUT:
    uint8	 	    pos           //IN:
)
{
    uint16  byte_num = 0;

    if(PNULL == pb_head_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] PNULL == pb_head_info_ptr !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_113_112_2_18_2_43_17_162,(uint8*)"");
        return FALSE;
    }
    if (!MMIPB_IsPbReady())
    {
        return (FALSE);
    }
    //pb_ptr = &g_mmipb_entry_list;
    
    pb_head_info_ptr->is_valid          = TRUE;
    pb_head_info_ptr->max_num_len       = MIN(MMIPB_NUMBER_MAX_STRING_LEN, MMI_SYNC_PB_NUM_MAX_LEN);/*lint !e506*/
    pb_head_info_ptr->max_email_len     = MIN(PB_MAX_MAIL_LEN, MMI_SYNC_PB_MAX_MAIL_LEN);/*lint !e506*/
    if(PB_SYNC_POS_ME == pos)
    {
        pb_head_info_ptr->max_alpha_len =MIN(MMIPB_NAME_MAX_LEN, MMI_SYNC_PB_NAME_MAX_LEN);/*lint !e506*/
        pb_head_info_ptr->max_record_num = MIN(MMIPB_GetMaxContactNumOfNV(), MMI_SYNC_PB_MAX_ENTRY_NUM);/*lint !e666*/
        pb_head_info_ptr->used_record_num   = MIN(MMI_SYNC_PB_MAX_ENTRY_NUM, MMIPB_GetContactCount(MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT)));/*lint !e666*/
    }
    else 
    {
        uint32 i = MN_DUAL_SYS_1;
        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if(pb_sim_pos[i] == pos)  
            {
                pb_head_info_ptr->max_alpha_len = MIN(MMIPB_NAME_MAX_LEN,MMI_SYNC_PB_NAME_MAX_LEN);/*lint !e506*/
                pb_head_info_ptr->max_record_num = MIN(MMIPB_GetMaxContactNumOfSim(i), MMI_SYNC_PB_MAX_ENTRY_NUM);/*lint !e666*/
                pb_head_info_ptr->used_record_num   = MIN(MMI_SYNC_PB_MAX_ENTRY_NUM,MMIPB_GetContactNumOfGroup(pb_sim_group_ids[i]));/*lint !e666*/
                break;
            }
        }       
    }

    SCI_MEMSET(pb_head_info_ptr->all_entry_id, 0x0, sizeof(uint8)*(MMI_SYNC_PB_MAX_ENTRY_NUM/8));
	if(PB_SYNC_POS_ME == pos)
	{
		byte_num = MMIPB_GetMaxContactNumOfNV()>> 3;
		if (0 < MMIPB_GetMaxContactNumOfNV()% 8) 
		{
			byte_num++;
		}        
		MMIPB_GetContactIdUsedInfoOfNV(pb_head_info_ptr->all_entry_id,
					MIN(byte_num, (MMI_SYNC_PB_MAX_ENTRY_NUM/8))
					);
	}
	else
	{
        uint32 i = MN_DUAL_SYS_1;
        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if(pb_sim_pos[i] == pos)  
            {
                MMIPB_GetContactIdUsedInfoOfSIM(i, 
                                                pb_head_info_ptr->all_entry_id,
                                                MMIPB_GetMaxContactNumOfSim(i)
                                                );
                
                break;
            }
        }		
	}
    return (TRUE);
}


/*****************************************************************************/
// 	Description : 读电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
BOOLEAN MMIPB_SyncRead
(
    PB_PHONEBOOK_ENTRY_T     *pb_item_info_ptr,  //OUT:
    uint8	 	    pos           //IN:
)
{
    uint16                   i = 0;
    BOOLEAN                 result = FALSE;
    MMIPB_ERROR_E res = MMIPB_ERROR_SUCCESS;
    
    uint16     storage_id = 0;
    MMIPB_CONTACT_T *pb_contact_ptr = PNULL;
    MMIPB_STRING_T          *num_string_ptr = PNULL;
	uint16  max_num = 0;

    if(PNULL == pb_item_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_SyncRead  PNULL == pb_item_info_ptr !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_215_112_2_18_2_43_17_164,(uint8*)"");
        return FALSE;
    }
    //    SCI_ASSERT(0 < pb_item_info_ptr->entry_id);
    pb_contact_ptr = (MMIPB_CONTACT_T*)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == pb_contact_ptr)
    {
    	return FALSE;
    }
    num_string_ptr = (MMIPB_STRING_T *)SCI_ALLOCA(sizeof(MMIPB_STRING_T));
    if(PNULL == num_string_ptr)
    {
        SCI_FREE(pb_contact_ptr);
    	return FALSE;
    }

    SCI_MEMSET(pb_contact_ptr, 0, sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(num_string_ptr, 0, sizeof(MMIPB_STRING_T));

    if(0 == pb_item_info_ptr->entry_id)
    {
        result = FALSE;
    }

    else if (!MMIPB_IsPbReady())
    {
        result = FALSE;
    }
    else
    {
        if(PB_SYNC_POS_ME== pos)
        {
            storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);	
            //SCI_TRACE_LOW:"MMIPB_SyncRead, read from nv"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_249_112_2_18_2_43_17_165,(uint8*)"");
        }
        else
        {
            uint32 dual_sys = MN_DUAL_SYS_1;
            for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
            {
                if(pb_sim_pos[dual_sys] == pos)  
                {
                    storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1+dual_sys, PHONEBOOK_SIM_STORAGE);
                    break;
                }
            }
        }	

    
       res = MMIAPIPB_GetContactInfo(pb_item_info_ptr->entry_id,
        storage_id,
        pb_contact_ptr
        );
	
	if (MMIPB_ERROR_SUCCESS == res)
	{
	        result = TRUE;
            pb_item_info_ptr->entry_id   = pb_contact_ptr->contact_id;
        	
        	//copy name
            MMIPB_WstrToInternalString(pb_contact_ptr->name.wstr,
                pb_contact_ptr->name.wstr_len,
                 pb_item_info_ptr->entry_name.alpha, 
                 &(pb_item_info_ptr->entry_name.alpha_len), 
                 &pb_item_info_ptr->entry_name.is_ucs2,
                 MMI_SYNC_PB_NAME_MAX_LEN
                 );       	
        	// copy mail
        #ifdef MMIPB_MAIL_SUPPORT
            MMIPB_WstrToInternalString(pb_contact_ptr->mail.wstr,
                pb_contact_ptr->mail.wstr_len,
                 pb_item_info_ptr->mail.mail, 
                 &(pb_item_info_ptr->mail.mail_len),
                 &pb_item_info_ptr->mail.is_ucs2,
                 MMI_SYNC_PB_MAX_MAIL_LEN
                 );    
        #endif
        	// copy phonenumber
        	max_num = MIN(PB_MAX_NV_PHONE_NUM, MMIPB_MAX_NV_PHONE_NUM);
            max_num = MIN(max_num, MMI_SYNC_PB_MAX_NV_PHONE_NUM);
            for (i=0; i<max_num; i++)
            {
                num_string_ptr->strlen = MMIAPICOM_GenDispNumber(
        			MMIPB_GetNumberTypeFromUint8(pb_contact_ptr->number[i].npi_ton),/*lint !e661*/
        			pb_contact_ptr->number[i].number_len,/*lint !e661*/
        			pb_contact_ptr->number[i].number,/*lint !e661 !e662*/
        			num_string_ptr->strbuf,
        			MMIPB_MAX_STRING_LEN+2
        			);
                pb_item_info_ptr->number_t[i].number_len = num_string_ptr->strlen;
                SCI_MEMCPY(
        			pb_item_info_ptr->number_t[i].number,
        			num_string_ptr->strbuf,
        			num_string_ptr->strlen
        			);
        	}
        	MMIPB_GetAllGroupIDs(pb_contact_ptr->union_t.group, &pb_item_info_ptr->group_id, 1);
	}		
    }
    SCI_FREE(num_string_ptr);
    SCI_FREE(pb_contact_ptr);
    return (result);
}


/*****************************************************************************/
// 	Description : 添加电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
MMIPB_ERROR_E MMIPB_SyncAdd
(
    uint8	 	    pos,           //IN:
    PB_PHONEBOOK_ENTRY_T    *pb_item_info_ptr   //IN:
)
{
    //BOOLEAN result = FALSE;
    MMIPB_ERROR_E res = MMIPB_ERROR_ERROR;
    
    //uint16     storage_id = 0;
    MMIPB_CONTACT_T *pb_contact_ptr = PNULL;

    //modify for coverity
    if(PNULL == pb_item_info_ptr || 0 == pb_item_info_ptr->entry_id)
    {
        return MMIPB_ERROR_ERROR;
    }

    //SCI_TRACE_LOW:"[MMIPB]MMIPB_SyncAdd pb_item_info_ptr %d entry_id !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_335_112_2_18_2_43_17_166,(uint8*)"d", pb_item_info_ptr, pb_item_info_ptr->entry_id);
    
    if (!MMIPB_IsPbReady())
    {
        return (MMIPB_ERROR_ERROR);
    }
    pb_contact_ptr = (MMIPB_CONTACT_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == pb_contact_ptr)
    {
    	return MMIPB_ERROR_ERROR;
    }
    SCI_MEMSET(pb_contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
    
    CopySyncEntryToStoreEntry(
                                pb_contact_ptr,
                                pb_item_info_ptr
                                );
	
    if(PB_SYNC_POS_ME== pos)
    {
        pb_contact_ptr->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
        //SCI_TRACE_LOW:"MMIPB_SyncAdd, add to nv"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_362_112_2_18_2_43_17_167,(uint8*)"");
		if(MMIPB_ERROR_SUCCESS == MMIPB_GetStorageFreeSpace(pb_contact_ptr->storage_id,  PNULL))
		{
			res = MMIAPIPB_AddContact( pb_contact_ptr);			
		}
    }
    else
    {
        uint32 i = MN_DUAL_SYS_1;
		MMIPB_HANDLE_T handle = PNULL;
        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if(pb_sim_pos[i] == pos)  
            {
                pb_contact_ptr->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + i ,PHONEBOOK_SIM_STORAGE);
                pb_contact_ptr->union_t.group = 1 << pb_sim_group_ids[i];
                          
                break;
            }
        }
		if(MMIPB_ERROR_SUCCESS == MMIPB_GetStorageFreeSpace(pb_contact_ptr->storage_id,  PNULL))
		{
			handle = MMIPB_CreateHandle(MMIPB_OPC_ADD, 0);
			if(handle != PNULL)
			{
				res = MMIAPIPB_AddContactAsyn(handle, pb_contact_ptr, update_callback);
			}
		}
    }
   
    SCI_FREE(pb_contact_ptr);


    return (res);
}


/*****************************************************************************/
// 	Description : 复制同步工具的电话簿记录到要保存的电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
LOCAL void CopySyncEntryToStoreEntry
(
    MMIPB_CONTACT_T *pb_contact_ptr,      //OUT:
    PB_PHONEBOOK_ENTRY_T *pb_item_info_ptr   //IN:
)
{
    uint16               i = 0;
    MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};

    MN_NUMBER_PLAN_E npi = MN_NUM_PLAN_ISDN_TELE;
    
    if((PNULL == pb_contact_ptr) || (PNULL == pb_item_info_ptr))
    {
        //SCI_TRACE_LOW:"[MMIPB] CopySyncEntryToStoreEntry (PNULL == pb_entry_ptr) || (PNULL == pb_item_info_ptr) !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_413_112_2_18_2_43_17_168,(uint8*)"");
        return;
    }
    pb_contact_ptr->contact_id = pb_item_info_ptr->entry_id;

    // copy name
    MMIPB_InternalStringToWstr(pb_item_info_ptr->entry_name.alpha, 
        pb_item_info_ptr->entry_name.alpha_len, 
        pb_item_info_ptr->entry_name.is_ucs2,
        MMIPB_NAME_MAX_LEN,
        pb_contact_ptr->name.wstr, 
        &(pb_contact_ptr->name.wstr_len)        
        );
    

    pb_contact_ptr->union_t.group  = 1 << PB_GROUP_ALL;

    // copy mail
#ifdef MMIPB_MAIL_SUPPORT
    MMIPB_InternalStringToWstr(pb_item_info_ptr->mail.mail, 
        pb_item_info_ptr->mail.mail_len, 
        pb_item_info_ptr->mail.is_ucs2,
        MMIPB_NAME_MAX_LEN,
        pb_contact_ptr->mail.wstr, 
        &(pb_contact_ptr->mail.wstr_len)
        );
#endif
    // copy phonenumber
    for (i=0; i<MIN(PB_MAX_NV_PHONE_NUM, MMI_SYNC_PB_MAX_NV_PHONE_NUM); i++)
    {
        if (!MMIAPICOM_GenPartyNumber(
                                pb_item_info_ptr->number_t[i].number,
                                pb_item_info_ptr->number_t[i].number_len,
                                &party_num
                                )
            )
        {
            party_num.number_type   = MN_NUM_TYPE_UNKNOW;
            party_num.num_len       = 0;
        }

        MMIPB_GSMNumberToBCDNumber(
                                    &party_num.number_type,
                                    &npi,
                                    party_num.bcd_num,
                                    (uint8)party_num.num_len,
                                    &(pb_contact_ptr->number[i])
                                    );
    }

	//set group id
    pb_contact_ptr->union_t.group= (1 << pb_item_info_ptr->group_id);

	//set custom photo , ring and text
	//pb_entry_ptr->custom_photo.is_valid = FALSE;
	//pb_entry_ptr->custom_ring.is_valid = FALSE;
#ifndef PB_SUPPORT_LOW_MEMORY
    pb_contact_ptr->memo.wstr_len = 0;
#endif    
	//pb_entry_ptr->custom_text.is_valid = FALSE;

}


/*****************************************************************************/
// 	Description : 删除电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
MMIPB_ERROR_E MMIPB_SyncDelete
(
    uint8	 	pos,        //IN:
    uint16      entry_id    //IN:
)
{
    //BOOLEAN             result = FALSE;
    MMIPB_ERROR_E res = MMIPB_ERROR_ERROR;
    
    uint16  storage_id = 0;
   // MN_DUAL_SYS_E		sys = MMI_DUAL_SYS_MAX;
   // uint16              i = 0;
  //  uint16              entry_index = 0;
    
    if(0 == entry_id)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_SyncDelete entry_id %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_496_112_2_18_2_43_17_169,(uint8*)"d", entry_id);
        return FALSE;
    }    
    if (!MMIPB_IsPbReady())
    {
        return (FALSE);
    }
     if(PB_SYNC_POS_ME== pos)
    {
        storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
        //SCI_TRACE_LOW:"MMIPB_SyncDelete from nv"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_506_112_2_18_2_43_17_170,(uint8*)"");
		res = MMIAPIPB_DeleteContact(entry_id,storage_id);
    }
    else
    {
        uint32 dual_sys = MN_DUAL_SYS_1;
		MMIPB_HANDLE_T handle = PNULL;
        for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
        {
            if(pb_sim_pos[dual_sys] == pos)  
            {
                storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + dual_sys, PHONEBOOK_SIM_STORAGE);
                break;
            }
        }
		handle = MMIPB_CreateHandle(MMIPB_OPC_DELETE, 0);
		if(handle != PNULL)
		{
			res = MMIAPIPB_DeleteContactAsyn(handle, entry_id, storage_id, update_callback);
		}
    }	

    return (res);
}


/*****************************************************************************/
// 	Description : 修改电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
MMIPB_ERROR_E MMIPB_SyncModify 
(
    uint8	 	pos,        //IN:
    PB_PHONEBOOK_ENTRY_T    *pb_item_info_ptr   //IN: 记录
)
{
    MMIPB_ERROR_E      res = MMIPB_ERROR_SUCCESS;
    
    //uint16			storage_id = 0;
    MMIPB_CONTACT_T		*pb_contact_ptr = PNULL;
    //uint8 group_id = 0;
	
    
    if(PNULL == pb_item_info_ptr || 0 == pb_item_info_ptr->entry_id )
    {
        //SCI_TRACE_LOW("[MMIPB]MMIPB_SyncModify entry_id %d !", pb_item_info_ptr->entry_id);
        return MMIPB_ERROR_ERROR;
    }     
    if (!MMIPB_IsPbReady())
    {
        return (MMIPB_ERROR_ERROR);
    }
    
    pb_contact_ptr = (MMIPB_CONTACT_T *) SCI_ALLOCA(sizeof(MMIPB_CONTACT_T));
    if(PNULL == pb_contact_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }
    SCI_MEMSET(pb_contact_ptr, 0x00, sizeof(MMIPB_CONTACT_T));
	
    CopySyncEntryToStoreEntry(
                                pb_contact_ptr,
                                pb_item_info_ptr
                                );

     if(PB_SYNC_POS_ME== pos)
    {
        pb_contact_ptr->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
        //SCI_TRACE_LOW:"MMIPB_SyncModify in nv"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_576_112_2_18_2_43_18_171,(uint8*)"");		
		res = MMIAPIPB_ModifyContact(pb_contact_ptr);
    }
    else
    {
        uint32 i = MN_DUAL_SYS_1;
		MMIPB_HANDLE_T handle = PNULL;

        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if(pb_sim_pos[i] == pos)  
            {            
                pb_contact_ptr->storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + i, PHONEBOOK_SIM_STORAGE);
                    
                break;
            }
        }
		//modify sync
		handle = MMIPB_CreateHandle(MMIPB_OPC_EDIT, 0);
		if(handle != PNULL)
		{
			res = MMIAPIPB_ModifyContactAsyn(handle, pb_contact_ptr, update_callback);
		}
		else
		{
			res = MMIPB_ERROR_ERROR;
		}		
    }	
    SCI_FREE(pb_contact_ptr);
    
    return (res);
}


/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncHeadInfo ( 
							GROUP_HEAD_INFO_T *  group_head_info_ptr  // OUT
							)
{
	uint8	group_count = 0;
    uint32  i = 0;

    if(NULL == group_head_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPBGROUP_SyncHeadInfo (NULL == group_head_info_ptr) !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_618_112_2_18_2_43_18_172,(uint8*)"");
        return FALSE;
    }
    
    group_head_info_ptr->max_name_len = GROUP_NAME_MAX_LEN;
    group_count = sizeof(pb_group_ids)/sizeof(*pb_group_ids);

    group_head_info_ptr->max_group_num = group_count;
    group_head_info_ptr->used_num = group_count;

    //判断群组的参数最大值是否超过了mmi at传输最大值
    if((MMI_SYNC_GROUP_MAX_NUM < group_head_info_ptr->max_group_num) || (MMI_SYNC_GROUP_NAME_MAX_LEN < group_head_info_ptr->max_name_len))
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPBGROUP_SyncHeadInfo max_group_num %d max_name_len %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ATC_631_112_2_18_2_43_18_173,(uint8*)"dd", group_head_info_ptr->max_group_num, group_head_info_ptr->max_name_len);
        return FALSE;
    }

    //all group entry id, start from 0
    for(i=0; i<group_count; i++)
    {
        group_head_info_ptr->all_entry_id[i] = pb_group_ids[i];
    }

    return TRUE;
}


/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncRead( 
                            uint16           entry_id,    //IN:
                GROUP_ENTRY_T *  group_item_info_ptr  // IN & OUT
							)
{

    MMIPB_GROUP_T pb_group_info = {0};
    MMIPB_ERROR_E res = MMIPB_ERROR_SUCCESS;
    BOOLEAN result = FALSE;
    
    if(PNULL == group_item_info_ptr)
    {
        return result;
    }
    res = MMIPB_ReadGroup(entry_id,&pb_group_info);
	
	if(MMIPB_ERROR_SUCCESS == res&&pb_group_info.is_valid)	
    {
    	
        if(entry_id<PB_GROUP_MAX)
        {
            group_item_info_ptr->is_fixed_group = TRUE;
        }
	  if(MMIAPICOM_IsASCIIString(pb_group_info.group_name.wstr,pb_group_info.group_name.wstr_len))
	  {
	  	group_item_info_ptr->is_ucs2 = FALSE;
	  	MMIAPICOM_WstrToStr(pb_group_info.group_name.wstr,group_item_info_ptr->group_name);
		group_item_info_ptr->name_len = pb_group_info.group_name.wstr_len;
	  }
	  else
	  {
	  	group_item_info_ptr->is_ucs2 = TRUE;
	  	SCI_MEMCPY(group_item_info_ptr->group_name,pb_group_info.group_name.wstr,
			MIN(MMI_SYNC_GROUP_NAME_MAX_LEN,pb_group_info.group_name.wstr_len*2));
		group_item_info_ptr->name_len = pb_group_info.group_name.wstr_len*2;
	  }	  
	  group_item_info_ptr->member_num = MMIPB_GetGroupContactCount(pb_group_info.group_id);
		
// 	group_item_info_ptr->name_len = GUI_WstrToUTF8(group_item_info_ptr->group_name, 
// 			MMI_SYNC_GROUP_NAME_MAX_LEN,pb_group_info.group_name.wstr,
// 			pb_group_info.group_name.wstr_len);

// 	        MMIAPICOM_Wstrncpy(group_item_info_ptr->group_name,
//             pb_group_info.group_name.wstr,pb_group_info.group_name.wstr_len);
//         	group_item_info_ptr->name_len = pb_group_info.group_name.wstr_len;
        
        result = TRUE;
	}
	return result;
	

}


/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncAdd( 
							GROUP_ENTRY_T *group_item_info_ptr  // IN
							)
{
    //SCI_ASSERT(NULL != group_item_info_ptr);

	//do not support this function
    return FALSE;
}

/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncDelete( 
							uint16           entry_id  // IN
							)
{
	//do not support this function
    return FALSE;
}

/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncModify( 
                            uint16           entry_id,    //IN:
							GROUP_ENTRY_T *  group_item_info_ptr  // IN 
							)
{
//compelte later 
    return FALSE;
}
#endif

#if defined(MMI_SYNCTOOL_SUPPORT) || defined(MMI_SYNCTOOL_V3)
/*****************************************************************************/
// 	Description : pb upate callback
//	Global resource dependence : none
//  Author:      baokun
//	Note:
/*****************************************************************************/
LOCAL void update_callback(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
	BOOLEAN result = FALSE;

	MMIPB_CloseHandle(handle);
	if(MMIPB_ERROR_SUCCESS == error)
	{
		result = TRUE;
	}
	
	MMIAPIATC_Response(result);
}
#endif
