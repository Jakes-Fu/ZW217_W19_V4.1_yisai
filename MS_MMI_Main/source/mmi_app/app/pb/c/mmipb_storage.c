/****************************************************************************
** File Name:      mmipb_storage.c                                          *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file deal with the physical device access function *
**                 处理各种物理存储设备的访问
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2011        baokun yin         Create
** 
****************************************************************************/
#define _MMIPB_STORAGE_C_  


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmi_app_pb_trc.h"
#include "mmipb_storage.h"
#include "mmipb_adapt.h"
#include "mmipb_nv.h"
#include "mmifmm_export.h"
#include "mmipb_text.h"
#include "mmi_text.h"
#ifdef SNS_SUPPORT
#include "mmisns_sqlite.h"
#include "mmisns_srv.h"
#endif
#ifdef FDN_SUPPORT
#include "mmifdn_export.h"
#endif

#include "mmi_modu_main.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMINV_READ_PHONE_CONTACT(_item_id, _addr, _ret_value)           \
{                                                                       \
    if ((MMINV_PHONEBOOK_MAIN_LAST_ENTRY < (_item_id))                  \
    || (MMINV_PHONEBOOK_MAIN_FIRST_ENTRY > (_item_id)))                 \
    {                                                                   \
    SCI_TRACE_MID("mmipb_mnapi: MMINV_READ_PHONEBOOK item_id = %d",     \
    (_item_id));                                                        \
    _ret_value = MN_RETURN_FAILURE;                                     \
    }                                                                   \
    _ret_value = MMI_ReadNVItemEx(                                      \
    (_item_id),                                                         \
    sizeof(MMIPB_CONTACT_PHONE_T),                                      \
    (void*)(_addr)                                                      \
    );                                                                  \
}  
//写手机联系人                                                                     
#define MMINV_WRITE_PHONE_CONTACT(_item_id, _addr,_ret_value)           \
{                                                                       \
    if ((MMINV_PHONEBOOK_MAIN_LAST_ENTRY < (_item_id))                  \
    || (MMINV_PHONEBOOK_MAIN_FIRST_ENTRY > (_item_id)))                 \
    {                                                                   \
    SCI_TRACE_MID("mmipb_mnapi: MMINV_WRITE_PHONEBOOK item_id = %d",    \
    (_item_id));                                                        \
    _ret_value = FALSE;                                                 \
    }                                                                   \
    else                                                                \
    {                                                                   \
        MMI_WriteNVItemEx(                                              \
        _item_id,                                                       \
        sizeof(MMIPB_CONTACT_PHONE_T),                                  \
        (void*)(_addr) );                                               \
        _ret_value = TRUE;                                              \
    }                                                                   \
}
// 从常用联系人中读一条记录
#ifdef MMIPB_MOST_USED_SUPPORT
#define MMINV_READ_MOSTUSED_PHONEBOOK(item_id, _addr, _ret_value)       \
{                                                                       \
    _ret_value = MMI_ReadNVItemEx(                                      \
    (item_id),                                                          \
    sizeof(MMIPB_CONTACT_MOSTUSED_T),                                   \
    (void*)(_addr)                                                      \
    );                                                                  \
}                                                                       \

// 写一条常用联系人记录 
#define MMINV_WRITE_MOSTUSED_PHONEBOOK(item_id, _addr,_ret_value)       \
{                                                                       \
    MMI_WriteNVItemEx(                                                  \
    item_id,                                                            \
    sizeof(MMIPB_CONTACT_MOSTUSED_T),                                   \
    (void*)(_addr) );                                                   \
    _ret_value = TRUE;                                                  \
}
#endif


#ifdef MMIPB_GROUP_MANAGE_SUPPORT
// 从手机中读一条组信息
#define MMINV_READ_GROUP(_item_id, _addr, _ret_value)               \
{                                                                   \
    if ((MMINV_PHONEBOOK_GROUP_MAX < (_item_id))                    \
    || (MMINV_PHONEBOOK_GROUP_0 > (_item_id)))                      \
    {                                                               \
    SCI_TRACE_MID("mmipb: MMINV_READ_GROUP item_id = %d",           \
    (_item_id));                                                    \
    _ret_value = MN_RETURN_FAILURE;                                 \
    }                                                               \
                                                                    \
    _ret_value = MMI_ReadNVItemEx(                                  \
    (_item_id),                                                     \
    sizeof(MMIPB_GROUP_T),                                          \
    (void*)(_addr)                                                  \
    );                                                              \
}                                                                   \
    
// 向手机中写一条组信息
#define MMINV_WRITE_GROUP(_item_id, _addr)                          \
{                                                                   \
    if ((MMINV_PHONEBOOK_GROUP_MAX < (_item_id))                    \
    || (MMINV_PHONEBOOK_GROUP_0 > (_item_id)))                      \
    {                                                               \
    SCI_TRACE_MID("mmipb: MMINV_WRITE_GROUP item_id = %d",          \
    (_item_id));                                                    \
    }                                                               \
                                                                    \
    MMI_WriteNVItemEx(                                              \
    (_item_id),                                                     \
    sizeof(MMIPB_GROUP_T),                                          \
    (void *)(_addr) );                                              \
} 
#endif


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#if 0 //def SNS_SUPPORT
typedef enum
{
    MMISNS_ACCOUNT_SINA,
    MMISNS_ACCOUNT_FACEBOOK,
    MMISNS_ACCOUNT_TWITER,
    MMISNS_ACCOUNT_MAX
}MMISNS_ACCOUNT_TYPE_E;
#endif
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
#ifdef MMIPB_UID_SUPPORT  
LOCAL uint32 g_phonecontact_uid=0; 
#endif

#ifdef CMCC_VCARD_SUPPORT
LOCAL MMIPB_NAME_T s_mmipb_aas_str_tab[MMI_DUAL_SYS_MAX][MMI_AAS_PHONE_NUM];
LOCAL uint8   s_mmipb_ass_type_num[MMI_DUAL_SYS_MAX] = {0};
#endif
#ifdef SNS_SUPPORT
LOCAL IMMISns *s_mmipb_sns_handle = PNULL;//sns数据库handle
#endif
#ifdef TCARD_PB_SUPPORT
LOCAL SCI_MUTEX_PTR  s_tcard_handle_mutex_p = PNULL;//T卡文件句柄的同步保护
LOCAL MMIFILE_HANDLE s_tcard_handle = SFS_INVALID_HANDLE;
LOCAL wchar s_tfile_path[] = {'T','F','C', 0};
LOCAL wchar s_tfile_name[] = {'c','o','n','t','a','c','t','.','l','i','s','t',0};
#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
#ifdef TCARD_PB_SUPPORT

/*****************************************************************************/
//  Description : get tcard file handle
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_HANDLE GetTCardContactFileHandle(uint32 access_mode);

/*****************************************************************************/
//  Description : close tcard file handle
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL void CloseTcardFileHandle(void);

/*****************************************************************************/
//  Description : get tcard file
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTCardContactFile(wchar *file_name_ptr, uint16 name_max_len);

/*****************************************************************************/
//  Description : write tcard contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E  WriteTCardContact(
                                       uint16 entry_id,
                                       uint16 storage_id,
                                       MMIPB_CONTACT_PHONE_T *contact_ptr//[IN]
                                       );

/*****************************************************************************/
//  Description : read tcard contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E  ReadTCardContact(
                                       uint16 entry_id,
                                       uint16 storage_id,
                                       MMIPB_CONTACT_PHONE_T *contact_ptr//[OUT]
                                       );


#endif

#ifdef MMIPB_UID_SUPPORT
/*****************************************************************************/
//  Description : 获得联系人对应字段的最大uid
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetMaxUid(void)
{

    return g_phonecontact_uid;

}



/*****************************************************************************/
//  Description : set联系人对应字段的最大uid
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_SetMaxUid(uint32 uid)
{
    SCI_DisableIRQ();
    g_phonecontact_uid = uid;
    SCI_RestoreIRQ();
}
#endif
//#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:starmer.sun
//  Note:
/*****************************************************************************/
//LOCAL BOOLEAN IsSnsAccountLogIn(uint8 group_id);
//#endif
/*****************************************************************************/
//  Description : 获得联系人对应字段的最大alpha长度
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetContactMaxAlphaLen(PHONEBOOK_STORAGE_E storage, 
                                  MN_DUAL_SYS_E dual_sys,
                                  MMIPB_CONTACT_DOMAIN_E feild//联系人域
                                  )
{
    uint8   max_len = 0;
    MMIPB_STORAGE_INFO_T    storage_info_t = MMIPB_GetContactStorage(dual_sys, storage);
    
    switch(feild)
    {
    case MMIPB_CONTACT_NUMBER:
        //get number max len
        max_len = MIN((storage_info_t.max_num_len << 1) , MMIPB_NUMBER_MAX_STRING_LEN);
        break;
    case MMIPB_CONTACT_NAME:
        //get name max alpha length
        max_len = MIN(storage_info_t.max_alpha_len, MMIPB_NAME_MAX_LEN);
        break;
#ifdef MMIPB_MAIL_SUPPORT //MMPB_MAIL_SUPPORT
    case MMIPB_CONTACT_EMAIL:
        //get eamil max alpha length
        max_len = storage_info_t.max_email_alpha_len;
        break;
#endif
#ifdef CMCC_VCARD_SUPPORT
    case MMIPB_CONTACT_SNE:
        //get eamil max alpha length
        max_len = storage_info_t.max_sne_alpha_len;      
        break;
#endif
    default:
        break;
    }

    //SCI_TRACE_LOW:"[MMIPB][MMIPB]MMIPB_GetMaxAlphaLen storage %d dual_sys %d feild %d max_len %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_STORAGE_195_112_2_18_2_44_8_292,(uint8*)"dddd",storage, dual_sys,feild, max_len);
    return max_len;
}

/*****************************************************************************/
//  Description : 获得联系人对应字段的最大wstr长度
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetContactMaxWstrLen(PHONEBOOK_STORAGE_E storage,// 
                                  MN_DUAL_SYS_E dual_sys,//卡
                                  MMIPB_CONTACT_DOMAIN_E feild//联系人域
                                 )
{
    uint8   max_len = MMIPB_GetContactMaxAlphaLen(storage, dual_sys, feild);
    
    if(max_len > 0)
    {
        //name and sne max wstr len
        if(MMIPB_CONTACT_NAME == feild || MMIPB_CONTACT_SNE == feild)
        {
            if(storage == PHONEBOOK_SIM_STORAGE
               ||storage == PHONEBOOK_FDN_STORAGE
               ||storage == PHONEBOOK_LND_STORAGE
               ||storage == PHONEBOOK_MDN_STORAGE
               ||storage == PHONEBOOK_MSISDN_STORAGE
               ||storage == PHONEBOOK_SDN_STORAGE
              )
            {
                //sim card name length and sne length when decode to 81,82,80, must used one bytes to save decode type
                //如果是sim卡的姓名字段和别名字段，如果是编码成81，82，82格式的编码，需要多占用一个字节来保存编码类型
                max_len--;
            }
            max_len = max_len/sizeof(wchar);
        }        
    }   
    
    return max_len;
}

/*****************************************************************************/
//  Description : read phone contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadPhoneContact(uint16 entry_id,
                                            uint16 storage_id,
                                            MMIPB_CONTACT_PHONE_T *contact_ptr//[OUT]
                                            )
{
    MN_RETURN_RESULT_E  nv_result   = MN_RETURN_FAILURE;
    MMIPB_ERROR_E       ret = MMIPB_ERROR_ERROR;
    
    if(contact_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"MMIPB_ReadPhoneContact invalid param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_STORAGE_248_112_2_18_2_44_8_293,(uint8*)"");
        return ret;
    }

    if(entry_id <= MMINV_MAX_PHONEBOOK_RECORDS)
    {
        MMINV_READ_PHONE_CONTACT(
            (PB_NV_ITEM_E)(MMINV_PHONEBOOK_MAIN_FIRST_ENTRY + entry_id-1),
            contact_ptr,
            nv_result
            );
    }
#ifdef TCARD_PB_SUPPORT
    else
    {
        //read tcard flash
        ret = ReadTCardContact(entry_id, storage_id, contact_ptr);        
    }
#endif
    contact_ptr->entry_id = entry_id;
    contact_ptr->storage_id = storage_id;

    if(MN_RETURN_SUCCESS == nv_result)
    {
        ret = MMIPB_ERROR_SUCCESS;
    }
    return ret;
}
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : read mostused contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadMostUsedContact(
                                               uint16 entry_id,
                                               uint16 storage_id,
                                               MMIPB_CONTACT_MOSTUSED_T *contact_ptr//[OUT]
                                               )
{
    MN_RETURN_RESULT_E  nv_result   = MN_RETURN_FAILURE;
    MMIPB_ERROR_E       ret = MMIPB_ERROR_NO_ENTRY;
    
    if(contact_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"MMIPB_ReadMostUsedContact invalid param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_STORAGE_283_112_2_18_2_44_8_294,(uint8*)"");
        return ret;
    }
    contact_ptr->entry_id = entry_id;
    contact_ptr->storage_id = storage_id;

    if(entry_id <= MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS)
    {
        MMINV_READ_MOSTUSED_PHONEBOOK(
            (PB_NV_ITEM_E)(MMINV_PHONEBOOK_MOSTUSED_FIRST_ENTRY + entry_id-1),
            contact_ptr,
            nv_result
            );
    }
    if(MN_RETURN_SUCCESS == nv_result)
    {
        ret = MMIPB_ERROR_SUCCESS;
    }
    return ret;
}
#endif
/*****************************************************************************/
//  Description : read sim contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadSIMContact(
                                          uint16 entry_id,
                                          MN_DUAL_SYS_E dual_sys,
                                          PHONEBOOK_STORAGE_E storage,
                                          PHONEBOOK_ENTRY_T   *contact_ptr//[OUT]
                                          )
{
    MMIPB_ERROR_E       ret = MMIPB_ERROR_ERROR;
    
    if(contact_ptr == PNULL || dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIPB_ReadSIMContact invalid param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_STORAGE_319_112_2_18_2_44_8_295,(uint8*)"");
        return ret;
    }

    contact_ptr->entry_id = entry_id;
#ifdef WIN32
    if(PHONEBOOK_SDN_STORAGE == storage)
    {
        contact_ptr->alpha[0] = 'S';
        contact_ptr->alpha[1] = 'D';
        contact_ptr->alpha[2] = 'N';
        contact_ptr->alpha[3] = '0' + entry_id;
        contact_ptr->alpha_len = 4;
        contact_ptr->number[0] = 0x21;
        contact_ptr->number[1] = 0x43;
        contact_ptr->number_len = 2;
        return MMIPB_ERROR_SUCCESS;
                
    }
#ifdef CMCC_VCARD_SUPPORT
    else if(PHONEBOOK_SIM_STORAGE == storage && MNSIM_Is_UsimEx(dual_sys))
    {
        //SCI_ASSERT(0);
        if (ERR_MNDATAMAG_NO_ERR == PHONEBOOK_ReadUSIMPhonebookEntry(
            dual_sys,
            storage,
            contact_ptr
            )
            )
        {
            return MMIPB_ERROR_SUCCESS;
        }
        return MMIPB_ERROR_ERROR;
    }
#endif
#endif
    if (ERR_MNDATAMAG_NO_ERR == PHONEBOOK_ReadPhonebookEntryEx(
        dual_sys,
        storage,
        contact_ptr
        )
        )
    {
        ret = MMIPB_ERROR_SUCCESS;
    }
    
    return ret;
}

/*****************************************************************************/
//  Description : read group information
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadGroup(
                                     uint16      group_id,
                                     MMIPB_GROUP_T *group_ptr//[OUT]
                                    )
{
    MN_RETURN_RESULT_E  nv_result   = MN_RETURN_FAILURE;
    MMIPB_ERROR_E       ret = MMIPB_ERROR_NO_ENTRY;
    MMI_STRING_T       group_name_str = {0};
    MMI_TEXT_ID_T      group_name_id[MMIPB_SYSTEM_GROUP_NUM] = 
                                            {
                                                    TXT_COMMON_ALL,			//全部
                                                   // TXT_PB_UNCLASS_GROUP,			//未分类
                                            #if  !defined(MMI_MULTI_SIM_SYS_SINGLE) 
                                                    TXT_SIM_SEL_SIM1,            //SIM1 // @baokun chang TXT_COMM_SIM to TXT_SIM_SEL_SIM11
                                                    TXT_SIM_SEL_SIM2,            //SIM2
                                            #if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
                                                    TXT_SIM_SEL_SIM3,            //SIM3 
                                            #if defined(MMI_MULTI_SIM_SYS_QUAD)
                                                    TXT_SIM_SEL_SIM4,            //SIM4 
                                            #endif
                                            #endif
                                            #else
                                                    TXT_COMM_SIM,
                                            #endif
                                                    TXT_PB_DEFAULT_GROUP_1,		//家庭
                                                    TXT_PB_DEFAULT_GROUP_2,		//朋友
                                                    TXT_PB_DEFAULT_GROUP_3,		//同事
                                                    TXT_PB_DEFAULT_GROUP_4,		//同学
#ifdef MMIPB_MOST_USED_SUPPORT                                                    
                                                    TXT_PB_MOSTUSED_GROUP,      //常用联系人
                                                    TXT_PB_MOSTUSED_GROUP   //favourite
#endif
                                            };
#ifdef SNS_SUPPORT    
    MMI_TEXT_ID_T sns_group_text_id[MMIPB_SNS_GROUP_NUM] = 
    {
        TXT_MAINMENU_ICON_SINA,
	    TXT_COMMON_TIWTTER,
        TXT_COMMON_FACEBOOK
    };
#endif
    if(PNULL == group_ptr)
    {
        SCI_TRACE_LOW("[mmipb] MMIPB_ReadGroup PNULL == group_ptr");
        return MMIPB_ERROR_INVALID_PARAM;
    }
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    if( group_id < MMIPB_USER_GROUP_START + MMIPB_USER_GROUP_NUM)
    {
        //user defined group, read it from nv
        MMINV_READ_GROUP(
            (PB_NV_ITEM_E)(MMINV_PHONEBOOK_GROUP_0 + group_id), 
            group_ptr, 
            nv_result
            );

        if(group_ptr->is_valid)
        {
            //user defined group
            ret = MMIPB_ERROR_SUCCESS;
        }
    }
#endif    
    //system group, get it name from resource
    if(group_id < MMIPB_SYSTEM_GROUP_NUM)
    {
        MMI_GetLabelTextByLang(
            group_name_id[group_id],
            &group_name_str
            );
        
        group_ptr->group_name.wstr_len = MIN(group_name_str.wstr_len, MMIPB_NAME_MAX_LEN);
        MMIAPICOM_Wstrncpy(group_ptr->group_name.wstr, group_name_str.wstr_ptr, group_ptr->group_name.wstr_len);   
        group_ptr->is_valid = TRUE;
        ret = MMIPB_ERROR_SUCCESS; 
    }

#ifdef SNS_SUPPORT    
    else if((group_id<MMIPB_GROUP_SNS_MAX) && (group_id >= MMIPB_GROUP_SNS_SINA))
    {
        //sns 
        if(IsSnsAccountLogIn(group_id))
        {
            MMI_GetLabelTextByLang(
                sns_group_text_id[group_id-MMIPB_USER_GROUP_NUM - MMIPB_SYSTEM_GROUP_NUM],
                &group_name_str
                );
            
            group_ptr->group_name.wstr_len = MIN(group_name_str.wstr_len, MMIPB_NAME_MAX_LEN);
            MMIAPICOM_Wstrncpy(group_ptr->group_name.wstr, group_name_str.wstr_ptr, group_ptr->group_name.wstr_len);   
            group_ptr->is_valid = TRUE;
            ret = MMIPB_ERROR_SUCCESS;  
         }
    }
#endif    

    if( MMIPB_ERROR_SUCCESS == ret)
    {
        group_ptr->group_id =group_id;
    }
    return ret;
}

/*****************************************************************************/
//  Description : write phone contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WritePhoneContact(uint16 entry_id,
                                             uint16 storage_id,
                                             MMIPB_CONTACT_PHONE_T *contact_ptr//[IN]
                                             )
{
    BOOLEAN            nv_result   = FALSE;
    MMIPB_ERROR_E      ret = MMIPB_ERROR_NO_SPACE;

    if(entry_id == 0 || contact_ptr ==PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_WritePhoneContact entry_id %d !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_STORAGE_505_112_2_18_2_44_8_297,(uint8*)"d", entry_id);
        return MMIPB_ERROR_INVALID_PARAM;
    }
    contact_ptr->entry_id = entry_id;
    contact_ptr->storage_id = storage_id;

    if(entry_id <= MMINV_MAX_PHONEBOOK_RECORDS)
    {
        MMINV_WRITE_PHONE_CONTACT(
            (PB_NV_ITEM_E)(MMINV_PHONEBOOK_MAIN_FIRST_ENTRY + entry_id -1),
            (void*)contact_ptr,
            nv_result
            );
        
        if( TRUE == nv_result )
        {
            ret = MMIPB_ERROR_SUCCESS;     
        }
        else
        {
            ret = MMIPB_ERROR_ERROR;            
        }
    }
#ifdef TCARD_PB_SUPPORT
    else
    {
        //save to tcard file
        ret = WriteTCardContact(entry_id, storage_id, contact_ptr);
    }
#endif    

    
    return ret;
}

#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : write mostused contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteMostUsedContact(uint16 entry_id,
                                                uint16 storage_id,
                                                MMIPB_CONTACT_MOSTUSED_T *contact_ptr//[IN]
                                                )
{
    BOOLEAN            nv_result   = FALSE;
    MMIPB_ERROR_E      ret = MMIPB_ERROR_ERROR;
    
    if(entry_id > MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS || entry_id == 0 || contact_ptr ==PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_WritePhoneContact entry_id %d !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_STORAGE_538_112_2_18_2_44_8_298,(uint8*)"d", entry_id);
        return MMIPB_ERROR_INVALID_PARAM;
    }
    contact_ptr->entry_id = entry_id;
    contact_ptr->storage_id = storage_id;    
    MMINV_WRITE_MOSTUSED_PHONEBOOK(
        (PB_NV_ITEM_E)(MMINV_PHONEBOOK_MOSTUSED_FIRST_ENTRY + entry_id -1),
        (void*)contact_ptr,
        nv_result
        );
    if( TRUE == nv_result )
    {
        ret = MMIPB_ERROR_SUCCESS;
    }
    
    return ret;
}
#endif

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : Open sns database
//  Global resource dependence : 
//  Author:maryxiao
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CreateSNSDBHandle(
                                       uint16 sns_type                                           
                                        )
{

    if(s_mmipb_sns_handle == PNULL)
    {
		IMMISNS_CreateObject(&s_mmipb_sns_handle);  
        if(PNULL!=s_mmipb_sns_handle)
        {
        
		    IMMISNS_CreateSNSDB(s_mmipb_sns_handle);
        }
    }
    else
    {
        if(!IMMISNS_IsDbOpen(s_mmipb_sns_handle))
        {
            //如果数据库没有打开，需要先打开sns数据库
            IMMISNS_CreateSNSDB(s_mmipb_sns_handle);
        }  
    }
}

/*****************************************************************************/
//  Description : close sns database
//  Global resource dependence : 
//  Author:maryxiao
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CloseSNSDBHandle(void)
{
    if(s_mmipb_sns_handle != PNULL)
    {
        IMMISNS_DestroySNSDB(s_mmipb_sns_handle);
        if(s_mmipb_sns_handle)
        {
            IMMISNS_Release((IMMISns  *)s_mmipb_sns_handle);
        }
        else
        {
            SCI_TRACE_LOW("MMIPB_CloseSNSDBHandle happen error");
        }
        s_mmipb_sns_handle = PNULL;
    }
}
/*****************************************************************************/
//  Description : read sns contact
//  Global resource dependence : 
//  Author:maryxiao
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadSnsContact(
                                            uint16 sns_type,
                                            uint16  index,
                                            MMIPB_SNS_CONTACT_T* data_ptr//[OUT]
                                            )
{
    MMIPB_ERROR_E       ret = MMIPB_ERROR_ERROR;
    uint16              len = 0;
    MMISNS_SQLITE_RESULT search_handle = {0}; 
    
    if(data_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"MMIPB_ReadVcardContact invalid param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_STORAGE_641_112_2_18_2_44_9_299,(uint8*)"");
        return ret;
    }


    search_handle.uiNum = 1;
    search_handle.uiOffset = index;//(uint16)iSearch-1;

    if(s_mmipb_sns_handle == PNULL
       ||(!IMMISNS_IsDbOpen(s_mmipb_sns_handle))
      )
    {
        //如果数据库没有打开，需要先打开sns数据库
        MMIPB_CreateSNSDBHandle(sns_type);
    }
    
    if(s_mmipb_sns_handle != PNULL)
    {
        IMMISNS_GetDataByDb(s_mmipb_sns_handle, &search_handle); /*lint !e64*/
    }    
    
    if(search_handle.nRow==1)
    {
    
        data_ptr->nRow = search_handle.nRow;
        data_ptr->uiOffset = index;
#ifndef PB_SUPPORT_LOW_MEMORY        
        len = MMIPB_SNS_BIRTH_LEN-1;
        if(PNULL != search_handle.search_result[0].birth)
        {
        
            strncpy(data_ptr->search_result.birth, search_handle.search_result[0].birth,len);
        }
 #endif
        len = MMIPB_SNS_NAME_LEN-1;
        if(PNULL != search_handle.search_result[0].name)
        {
        
            strncpy(data_ptr->search_result.name, search_handle.search_result[0].name,len);
        }

        len = MMIPB_SNS_EMAIL_LEN-1;
        if(PNULL != search_handle.search_result[0].email)
        {
        
            strncpy(data_ptr->search_result.email, search_handle.search_result[0].email,len);
        }

        len = MMIPB_SNS_PHOTO_PATH_LEN-1;
        if(PNULL != search_handle.search_result[0].file_path)
        {
        
            strncpy(data_ptr->search_result.file_path, search_handle.search_result[0].file_path,len);
        }

        len = MMIPB_SNS_SIGN_LEN-1;
        if(PNULL != search_handle.search_result[0].sign)
        {
        
            strncpy(data_ptr->search_result.sign, search_handle.search_result[0].sign,len);
        }
        
        len =MMIPB_SNS_TYPE_LEN-1;
        if((PNULL != search_handle.search_result[0].type))
        {
        
            data_ptr->search_result.type = *search_handle.search_result[0].type;/*lint !e64 !e571*/
        }
        
        len =MMIPB_SNS_USERID_LEN-1;
        if((PNULL != search_handle.search_result[0].user_id))
        {
        
            strncpy(data_ptr->search_result.user_id, search_handle.search_result[0].user_id, len);
        }
    }
    if(s_mmipb_sns_handle != PNULL)
    {
        IMMISNS_ResetDBUerPara(s_mmipb_sns_handle, &search_handle);/*lint !e64*/
    }
//     //销毁
//     IMMISNS_DestroySNSDB(pMe, &search_handle);    
//     
//     if(pMe)
//     {
//         IMMISNS_Release(pMe);
//         pMe = PNULL;
//     }


    if(data_ptr->nRow == 1)
    {
        ret = MMIPB_ERROR_SUCCESS;
    }
    else
    {
        ret =MMIPB_ERROR_ERROR;
    }  
    return ret;
}

#endif
/*****************************************************************************/
//  Description : write sim contact
//  Global resource dependence : sns
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteSIMContact(										  
                                           uint16 entry_id,
                                           MN_DUAL_SYS_E dual_sys,
                                           PHONEBOOK_STORAGE_E storage,
                                           PHONEBOOK_ENTRY_T  *contact_ptr//[IN]
                                           )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    
    if(contact_ptr != PNULL && dual_sys < MMI_DUAL_SYS_MAX)
    {
        contact_ptr->entry_id = entry_id;
        if (ERR_MNDATAMAG_NO_ERR == PHONEBOOK_UpdatePhonebookEntryEx(
            dual_sys,
            storage,
            contact_ptr
            )
            )
        {
            result = MMIPB_ERROR_PENDING;
        }
    }
    
    return result;
}
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : write vcard contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteVcardContact(										  
                                             MMIFILE_HANDLE file_handle,
                                             uint32 data_len,
                                             void* data_ptr//[IN]
                                             )
{
    MMIFILE_ERROR_E file_ret = SFS_ERROR_NONE;
    MMIPB_ERROR_E   ret = MMIPB_ERROR_ERROR;
    
    if(file_handle == PNULL || data_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_WriteVcardContact invalid param !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_STORAGE_787_112_2_18_2_44_9_300,(uint8*)"");
        return ret;
    }

    file_ret = MMIAPIFMM_WriteFile(file_handle, data_ptr, data_len, &data_len, PNULL);
    if(SFS_ERROR_NONE == file_ret)
    {
        ret = MMIPB_ERROR_SUCCESS;
    }
    else if(SFS_ERROR_NOT_EXIST == file_ret)
    {
        ret = MMIPB_ERROR_FILE_NOT_EXIST;
    }
    else if(SFS_ERROR_NO_SPACE == file_ret)
    {
        ret = MMIPB_ERROR_NO_SPACE;
    }

    return ret;   
}

#endif

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : write group information
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteGroup(
                                     uint16      group_id,
                                     MMIPB_GROUP_T *group_ptr//[IN]
                                    )
{
    MMIPB_ERROR_E       result = MMIPB_ERROR_ERROR;

    if(group_id < MMIPB_USER_GROUP_START + MMIPB_USER_GROUP_NUM && group_ptr != PNULL)
    {
        MMINV_WRITE_GROUP(
            (MMINV_PHONEBOOK_GROUP_0 + group_id),
            group_ptr
            );
        result = MMIPB_ERROR_SUCCESS;       
    }

    return result;
}
#endif
/*****************************************************************************/
//  Description : delete phone contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeletePhoneContact(uint16 entry_id,
                                             uint16 storage_id
                                             )
{
    MMIPB_CONTACT_PHONE_T *contact_ptr = (MMIPB_CONTACT_PHONE_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_PHONE_T));
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    
    if(contact_ptr != PNULL)
    {
        SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_PHONE_T));

        {
        
            contact_ptr->entry_id = entry_id;
            contact_ptr->storage_id = storage_id;
            ret = MMIPB_WritePhoneContact(entry_id, storage_id, contact_ptr);
        }
        SCI_FREE(contact_ptr);
    }
    return ret;
}
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : delete most used contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteMostContact(uint16 entry_id,
                                             uint16 storage_id
                                             )
{
    MMIPB_CONTACT_MOSTUSED_T *contact_ptr = (MMIPB_CONTACT_MOSTUSED_T *)SCI_ALLOCA(sizeof(MMIPB_CONTACT_MOSTUSED_T));
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    
    if(contact_ptr != PNULL)
    {
        SCI_MEMSET(contact_ptr, 0x00, sizeof(MMIPB_CONTACT_MOSTUSED_T));
        contact_ptr->entry_id = entry_id;
        contact_ptr->storage_id = storage_id;
        ret = MMIPB_WriteMostUsedContact(entry_id, storage_id, contact_ptr);
        SCI_FREE(contact_ptr);
    }
    return ret;
}
#endif
/*****************************************************************************/
//  Description : delete sim contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteSIMContact( uint16 entry_id,
                                             MN_DUAL_SYS_E dual_sys,
                                             PHONEBOOK_STORAGE_E storage
                                             )
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    
    if ((dual_sys < MMI_DUAL_SYS_MAX) && ERR_MNDATAMAG_NO_ERR == PHONEBOOK_DeletePhonebookEntryEx(
        dual_sys,
        storage,
        (uint32)entry_id    // IN:
        )
        )
    {
        ret = MMIPB_ERROR_PENDING;
    }
    return ret;
}


#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : delete group information
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteGroup(
                                     uint16      group_id
                                    )
{
    MMIPB_ERROR_E       result = MMIPB_ERROR_ERROR;
    MMIPB_GROUP_T      group = {0};

    if(group_id < MMIPB_USER_GROUP_START + MMIPB_USER_GROUP_NUM)
    {
        MMINV_WRITE_GROUP(
            (MMINV_PHONEBOOK_GROUP_0 + group_id),
            &group
            );
        result = MMIPB_ERROR_SUCCESS;       
    }

    return result;
}
#endif
/*****************************************************************************/
//  Description : 获得电话簿的储存信息
//  Global resource dependence : s_mmipb_nv_info_t
//  Author: 
//  Note: 
/*****************************************************************************/
MMIPB_STORAGE_INFO_T MMIPB_GetContactStorage //RETURN: 电话簿信息
(
 MN_DUAL_SYS_E       dual_sys,
 PHONEBOOK_STORAGE_E storage             //IN: 存储器类型
 )
{
    PHONEBOOK_INFO_T 	    sim_phonebook_info = {0};
    MMIPB_STORAGE_INFO_T 	phonebook_info = {0};
#ifdef CMCC_VCARD_SUPPORT
    PHONEBOOK_VCARD_STATUS_T usim_storage_info = {0};
#endif   

#ifndef WIN32
#if defined(MMI_PB_EXT_NUM_SUPPORT) && defined(MMIPB_3G_EXT_NUM_SUPPORT)
    PHONEBOOK_EXT_INFO_T        sim_phonebook_ext_info = {0};
#endif
#endif

    SCI_MEMSET(&phonebook_info, 0, sizeof(MMIPB_STORAGE_INFO_T));
    switch (storage)
    {
    case PHONEBOOK_SIM_STORAGE:
    case PHONEBOOK_MSISDN_STORAGE:
    case PHONEBOOK_FDN_STORAGE:
    case PHONEBOOK_SDN_STORAGE:
    case PHONEBOOK_MDN_STORAGE: 
    case PHONEBOOK_LND_STORAGE:    
    case PHONEBOOK_BDN_STORAGE:   
        if(dual_sys < MMI_DUAL_SYS_MAX)
        {
            sim_phonebook_info              = PHONEBOOK_GetFileInfoEx(dual_sys, storage);
            phonebook_info.is_valid         = (BOOLEAN)(sim_phonebook_info.is_phonebook_exist && sim_phonebook_info.is_user_need);
            phonebook_info.max_alpha_len    = sim_phonebook_info.max_alpha_len;

#ifdef MMI_PB_EXT_NUM_SUPPORT
            if (sim_phonebook_info.current_dn_empty_ext_num > 0)
            {
                phonebook_info.max_num_len = MMIPB_BCD_NUMBER_MAX_LEN;
            }
			else            
#endif
            {
                phonebook_info.max_num_len = sim_phonebook_info.max_normal_num_len;
            }

//ARM下，获取3G下扩展号码长度
#ifndef WIN32
#if defined(MMI_PB_EXT_NUM_SUPPORT) && defined(MMIPB_3G_EXT_NUM_SUPPORT)
            
            SCI_TRACE_LOW("yanyan.an MMIPB_GetContactStorage dualmode enable");
            sim_phonebook_ext_info       = PHONEBOOK_GetExtInfo(dual_sys, storage);
            SCI_TRACE_LOW("yanyan.an MMIPB_GetContactStorage ext1 max_record_num:%d, used_record_num:%d",
                         sim_phonebook_ext_info.max_record_num,
                         sim_phonebook_ext_info.used_record_num
                         );
            if(TRUE == sim_phonebook_ext_info.is_ext_exist)
            {
                if(sim_phonebook_ext_info.max_record_num <= sim_phonebook_ext_info.used_record_num)
                {
                    SCI_TRACE_LOW("yanyan.an MMIPB_GetContactStorage ext1 no space!");
                    phonebook_info.max_num_len = sim_phonebook_info.max_normal_num_len;
                }
                else
                {                
                    SCI_TRACE_LOW("yanyan.an MMIPB_GetContactStorage ext1 has space!");
                    phonebook_info.max_num_len = MMIPB_BCD_NUMBER_MAX_LEN;
                }
                
            }
            else            
#endif
#endif
            {
                phonebook_info.max_num_len = sim_phonebook_info.max_normal_num_len;
            }

#ifdef WIN32
            if(PHONEBOOK_SDN_STORAGE == storage)
            {
                sim_phonebook_info.max_record_num = 2;
            }
#endif
#ifdef FDN_SUPPORT
            if(MMIAPIFDN_Is2GFdnEnable(dual_sys) && (PHONEBOOK_SIM_STORAGE == storage))
            {
                SCI_TRACE_LOW("[mary] MMIPB_GetContactStorage dual_sys %d MMIAPIFDN_Is2GFdnEnable ! ",dual_sys);

                //fdn actived, ADN cannot list
                phonebook_info.max_record_num   = 0;
                phonebook_info.used_record_num  = 0;
            }
            else
#endif
            {
                phonebook_info.max_record_num   = MIN(sim_phonebook_info.max_record_num, MMIPB_SIM_MAX_RECORD_NUM);
                phonebook_info.used_record_num  = MIN(sim_phonebook_info.used_record_num, MMIPB_SIM_MAX_RECORD_NUM);            
            }
#ifdef CMCC_VCARD_SUPPORT
            if(PHONEBOOK_SIM_STORAGE == storage && MNSIM_Is_UsimEx(dual_sys))
            {
                PHONEBOOK_Get3GPhonebookStatusEx(dual_sys, &usim_storage_info);
#ifdef MMIPB_MAIL_SUPPORT
                phonebook_info.max_email_alpha_len = MIN(usim_storage_info.storage_usage[0].email_usage.rec_len, MMIPB_MAX_MAIL_LEN);
#endif
                phonebook_info.max_sne_alpha_len = MIN(usim_storage_info.storage_usage[0].sne_usage.rec_len, MMIPB_NAME_MAX_LEN);
            }
            else
#endif
            {
#ifdef MMIPB_MAIL_SUPPORT
                phonebook_info.max_email_alpha_len = 0;
#endif
#ifdef CMCC_VCARD_SUPPORT
                phonebook_info.max_sne_alpha_len = 0;
#endif
            }
        }

        break;
        
    case PHONEBOOK_NV_STORAGE:
        // 给phonebook_info填充关于手机中电话簿的一些限制。
        phonebook_info.is_valid = TRUE;
        phonebook_info.max_alpha_len = MMIPB_NAME_MAX_LEN;
        phonebook_info.max_num_len = MMIPB_NUMBER_MAX_STRING_LEN >> 1;
#ifdef TCARD_PB_SUPPORT
	{
		phonebook_info.max_record_num = MMIPB_MAX_CONTACT_COUNT;

	}
#else
	{
		phonebook_info.max_record_num = MMIPB_NV_MAX_RECORD_NUM;
	}
#endif

        phonebook_info.used_record_num = 0;
#ifdef MMIPB_MAIL_SUPPORT
        phonebook_info.max_email_alpha_len = MMIPB_MAX_MAIL_LEN;
#endif
#ifdef CMCC_VCARD_SUPPORT
        phonebook_info.max_sne_alpha_len = MMIPB_NAME_MAX_LEN;
#endif

        break;
        
    default:
        //SCI_TRACE_LOW:"mmipb_mnapi: MMIPB_GetPhonebookInfo storage = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_STORAGE_1023_112_2_18_2_44_9_301,(uint8*)"d", storage);
        break;
    }
    
    return (phonebook_info);
}


/*****************************************************************************/
//  Description : 获得电话簿的空间信息
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  uint16 MMIPB_GetStorageSpace //RETURN: 电话簿信息
(
 MN_DUAL_SYS_E       dual_sys,
 PHONEBOOK_STORAGE_E storage             //IN: 存储器类型
 )
{
    uint16 space = 0;
    PHONEBOOK_INFO_T 	    sim_phonebook_info = {0};

    switch (storage)
    {
    case PHONEBOOK_SIM_STORAGE:
    case PHONEBOOK_MSISDN_STORAGE:
    case PHONEBOOK_FDN_STORAGE:
    case PHONEBOOK_SDN_STORAGE:
    case PHONEBOOK_MDN_STORAGE: 
    case PHONEBOOK_LND_STORAGE:    
    case PHONEBOOK_BDN_STORAGE:   
        if(dual_sys < MMI_DUAL_SYS_MAX)
        {
            sim_phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, storage);
            space = MIN(sim_phonebook_info.max_record_num, MMIPB_SIM_MAX_RECORD_NUM);   //Bug 104090 
        }
        break;
        
    case PHONEBOOK_NV_STORAGE:
        space = MMIPB_NV_MAX_RECORD_NUM;
#ifdef TCARD_PB_SUPPORT
        {
            MMIFILE_HANDLE sfs_handle = 0;
            uint32  high_size = 0;
            uint32  file_size = 0;

            //device free space
            MMIAPIFMM_GetDeviceFreeSpace(
                MMIAPIFMM_GetDevicePath(MMIAPIFMM_GetFirstValidSD()),
                MMIFILE_DEVICE_LEN,
                &high_size,
                &file_size
                );
            if(high_size > 0)
            {
                file_size = 0xFFFFFFFF;
            }
            else
            {
                //file size
                sfs_handle = GetTCardContactFileHandle(SFS_MODE_READ |SFS_MODE_OPEN_EXISTING);
                if(sfs_handle > 0)
                {
                    file_size += MMIAPIFMM_GetFileSize(sfs_handle);
                }
            }
            space += MIN(((file_size)/sizeof(MMIPB_CONTACT_PHONE_T)), MMIPB_TCARD_MAX_RECORD_NUM);
        }
#endif
        break;
        
    default:
        //SCI_TRACE_LOW:"mmipb_mnapi: MMIPB_GetPhonebookInfo storage = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_STORAGE_1023_112_2_18_2_44_9_301,(uint8*)"d", storage);
        break;
    }
    
    return (space);
}

#ifdef TCARD_PB_SUPPORT
/*****************************************************************************/
//  Description : create tcard file mutex
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_CreateTcardHandleMutex(void)
{
    if(PNULL != s_tcard_handle_mutex_p)
    {
        SCI_DeleteMutex(s_tcard_handle_mutex_p);
        s_tcard_handle_mutex_p = PNULL;
    }
    s_tcard_handle_mutex_p = SCI_CreateMutex("s_tcard_handle_mutex_p", SCI_INHERIT);
}

/*****************************************************************************/
//  Description : init tcard file handle
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_InitTcardFileHandle(void)
{
    SCI_GetMutex(s_tcard_handle_mutex_p, SCI_WAIT_FOREVER); 
    CloseTcardFileHandle();
    SCI_PutMutex(s_tcard_handle_mutex_p);
}

/*****************************************************************************/
//  Description : tcard file exist
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsTcardFileExist(void)
{
    BOOLEAN result = FALSE;
    wchar *file_name_ptr = PNULL;

    file_name_ptr = (wchar *)SCI_ALLOCAZ(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN +1));
    if(file_name_ptr != PNULL)
    {
        if(GetTCardContactFile(file_name_ptr, MMIFILE_FILE_NAME_MAX_LEN))
        {
            result = MMIAPIFMM_IsFileExist(file_name_ptr, MMIAPICOM_Wstrlen(file_name_ptr));
        }
        SCI_FREE(file_name_ptr);    
    }
    return result;
}

/*****************************************************************************/
//  Description : close tcard file handle
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL void CloseTcardFileHandle(void)
{
    if(SFS_INVALID_HANDLE != s_tcard_handle)
    {
        MMIAPIFMM_CloseFile(s_tcard_handle);
        s_tcard_handle = SFS_INVALID_HANDLE;
    }
}
/*****************************************************************************/
//  Description : get tcard file
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTCardContactFile(wchar *file_name_ptr, uint16 name_max_len)
{
    BOOLEAN result = FALSE;    
    MMIFILE_DEVICE_E device = MMIAPIFMM_GetFirstValidSD();
    uint16 name_len = name_max_len;

    if(device < MMI_DEVICE_NUM && file_name_ptr != PNULL)
    {
        result =  MMIAPIFMM_CombineFullPath(
                                 MMIAPIFMM_GetDevicePath(device),
                                 MMIAPIFMM_GetDevicePathLen(device),
                                 s_tfile_path,
                                 MMIAPICOM_Wstrlen(s_tfile_path),
                                 s_tfile_name,
                                 MMIAPICOM_Wstrlen(s_tfile_name),
                                 file_name_ptr,
                                 &name_len
                                 );
    }
     
    return result;
}

/*****************************************************************************/
//  Description : get tcard file handle
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_HANDLE GetTCardContactFileHandle(uint32 access_mode)
{
    wchar *file_name_ptr = PNULL;

    if(SFS_MODE_WRITE & access_mode)
    {
        //写操作，每次都需要重新打开，防止异常发生时，文件没有保存
        CloseTcardFileHandle();                 
    }
    if(SFS_INVALID_HANDLE == s_tcard_handle)
    {
        file_name_ptr = (wchar *)SCI_ALLOCAZ(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN +1));
        if(file_name_ptr != PNULL)
        {            
            if(SFS_MODE_WRITE & access_mode)
            {
                //hide its folder
                if(MMIAPIFMM_CombineFullPathEx(
                    file_name_ptr,
                    MMIFILE_FILE_NAME_MAX_LEN,
                    MMIAPIFMM_GetFirstValidSD(),
                    s_tfile_path, 
                    PNULL
                    )
                    )
                {
                    if(!MMIAPIFMM_IsFolderExist(file_name_ptr, MMIAPICOM_Wstrlen(file_name_ptr)))
                    {
                        if(SFS_ERROR_NONE == MMIAPIFMM_CreateDirectory(file_name_ptr))
                        {
                            MMIAPIFMM_SetAttr(file_name_ptr, MMIAPICOM_Wstrlen(file_name_ptr), TRUE, FALSE, FALSE, FALSE);
                        }
                    }
                } 
                SCI_MEMSET(file_name_ptr, 0x00, sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN +1));
            }

            if(GetTCardContactFile(file_name_ptr, MMIFILE_FILE_NAME_MAX_LEN))
            {
                s_tcard_handle = MMIAPIFMM_CreateFile(file_name_ptr, access_mode, PNULL, PNULL);
            }
            SCI_FREE(file_name_ptr);
        }
    }  
    return s_tcard_handle;
}

/*****************************************************************************/
//  Description : write tcard contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E  WriteTCardContact(
                                       uint16 entry_id,
                                       uint16 storage_id,
                                       MMIPB_CONTACT_PHONE_T *contact_ptr//[IN]
                                       )
{
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = 0;
    uint32 offset = 0;
    uint32 bytes_written = 0;
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    
    SCI_GetMutex(s_tcard_handle_mutex_p, SCI_WAIT_FOREVER);     
    if(entry_id > MMINV_MAX_PHONEBOOK_RECORDS && entry_id <= MMIPB_MAX_CONTACT_COUNT && contact_ptr != PNULL)
    {
        contact_ptr->entry_id = entry_id;
        contact_ptr->storage_id = storage_id;            
        
        offset = (entry_id - MMINV_MAX_PHONEBOOK_RECORDS -1)*sizeof(MMIPB_CONTACT_PHONE_T);
        file_handle = GetTCardContactFileHandle(SFS_MODE_WRITE |SFS_MODE_OPEN_ALWAYS);                    
        if(file_handle != SFS_INVALID_HANDLE)
        {
            file_ret = MMIAPIFMM_SetFilePointer(file_handle, offset, SFS_SEEK_BEGIN);
            if(file_ret != SFS_ERROR_NONE)
            {
                //文件大小不过，需要重新设置文件大小，以保证能够跳跃保存联系人
                file_ret = MMIAPIFMM_SetFileSize(file_handle, offset);
                file_ret = MMIAPIFMM_SetFilePointer(file_handle, offset, SFS_SEEK_BEGIN);
            }
            if(SFS_ERROR_NONE == file_ret)
            {
                file_ret = MMIAPIFMM_WriteFile(file_handle, contact_ptr, sizeof(MMIPB_CONTACT_PHONE_T), &bytes_written, PNULL);                        
            }
            CloseTcardFileHandle();
            ret = MMIPB_GetErrorType(file_ret);            
        }
    }
    SCI_PutMutex(s_tcard_handle_mutex_p);
    return ret;
}

/*****************************************************************************/
//  Description : read tcard contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
LOCAL MMIPB_ERROR_E  ReadTCardContact(
                                       uint16 entry_id,
                                       uint16 storage_id,
                                       MMIPB_CONTACT_PHONE_T *contact_ptr//[OUT]
                                       )
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_NO_ENTRY;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = 0;
    wchar *file_name_ptr = PNULL;
    uint32 offset = 0;

    SCI_GetMutex(s_tcard_handle_mutex_p, SCI_WAIT_FOREVER);         
    if(entry_id > MMINV_MAX_PHONEBOOK_RECORDS && entry_id <= MMIPB_MAX_CONTACT_COUNT && contact_ptr)
    {
        offset = (entry_id - MMINV_MAX_PHONEBOOK_RECORDS -1)*sizeof(MMIPB_CONTACT_PHONE_T);        
        file_handle = GetTCardContactFileHandle(SFS_MODE_READ |SFS_MODE_OPEN_EXISTING); 
        if(file_handle != SFS_INVALID_HANDLE)
        {
            if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, offset, SFS_SEEK_BEGIN))
            {
                if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, contact_ptr, sizeof(MMIPB_CONTACT_PHONE_T), &offset, PNULL))
                {
                    ret = MMIPB_ERROR_SUCCESS;
                }
                else
                {
                    ret = MMIPB_ERROR_NO_ENTRY;
                }
            }
        }
        
    }
    SCI_PutMutex(s_tcard_handle_mutex_p);
    return ret;
}
#endif

/*****************************************************************************/
//  Description : 获得sns联系人个数
//  Author: 
//  Note: 
/*****************************************************************************/
/*
PUBLIC uint32 MMIPB_GetSNSContactsCount(uint16 sns_type)
{
     uint32    total_count = 0;
#ifdef SNS_SUPPORT 
    MMISNS_SQLITE_RESULT search_handle = {0}; 

    if(s_mmipb_sns_handle == PNULL
       ||(!IMMISNS_IsDbOpen(s_mmipb_sns_handle))
      )
    {
        //如果数据库没有打开，需要先打开sns数据库
        MMIPB_CreateSNSDBHandle(sns_type);
    }    
    //IMMISNS_CreateObject(&s_mmipb_sns_handle);
    //IMMISNS_CreateSNSDB(s_mmipb_sns_handle);
    IMMISNS_GetTotalNumByDb(s_mmipb_sns_handle, &total_count);
    IMMISNS_ResetDBUerPara(s_mmipb_sns_handle, &search_handle);
#endif    
    return total_count;
}
*/

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : 获得sns账户信息(email)
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetSnsAccountType(uint32 group_id)
{
    if(MMIPB_GROUP_SNS_SINA == group_id)
    {
        return MMISNS_ACCOUNT_SINA;
    }
    else if(MMIPB_GROUP_SNS_TIWTTER == group_id)
    {
        return MMISNS_ACCOUNT_TWITER;
    }
    else if(MMIPB_GROUP_SNS_FACEBOOK == group_id)
    {
        return MMISNS_ACCOUNT_FACEBOOK;
    }
    else
    {
        return MMISNS_ACCOUNT_MAX;
    }    
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:starmer.sun
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN IsSnsAccountLogIn(uint8 group_id)
{
    uint8 sns_account = 0;
    BOOLEAN is_login = FALSE;
    
    if(PNULL == s_mmipb_sns_handle)
    {
        return is_login;
    }
    MMIPB_CreateSNSDBHandle(0);
    sns_account = GetSnsAccountType(group_id);
    IMMISNS_CheckUserLogIn(s_mmipb_sns_handle,sns_account,&is_login);/*lint !e64*/
    
    return is_login;
} 

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:starmer.sun
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsSnsAccountLogIn(void)
{
    uint8 group_id = MMIPB_GROUP_SNS_SINA;
    BOOLEAN is_login = FALSE;
    
    for(group_id = MMIPB_GROUP_SNS_SINA; group_id < MMIPB_GROUP_SNS_MAX; group_id++)
    {
        is_login = IsSnsAccountLogIn(group_id);
        if(TRUE == is_login)
        {
            break;
        }
    }
    
    return is_login;
} 

/*****************************************************************************/
//  Description : 获得sns账户信息(email)的长度
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC uint32  MMIPB_GetSNSAccountEmailLen(void)
{
    return MMI_EMAIL_LEN ;
}

/*****************************************************************************/
//  Description : 获得sns账户信息(email)
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetSNSAccountEmail(uint8 group_id,                //in 
                                         MMI_STRING_T* email)     //out
{
    uint32 sns_account_type = 0;
    uint8*  userAccount_ptr = PNULL;

    if(group_id >= MMIPB_GROUP_SNS_MAX || group_id < MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM)
    {
        return FALSE;
    }

    sns_account_type = GetSnsAccountType(group_id);

    if(PNULL == s_mmipb_sns_handle)
    {
        IMMISNS_CreateObject(&s_mmipb_sns_handle);
    }

    userAccount_ptr = SCI_ALLOC_APPZ(MMIPB_GetSNSAccountEmailLen());
    IMMISNS_PBGetAccountNumber(s_mmipb_sns_handle, sns_account_type, userAccount_ptr);

    email->wstr_len = MIN(email->wstr_len,strlen(userAccount_ptr));       /*lint !e64 !e666*/     
    MMIAPICOM_StrToWstr(userAccount_ptr,email->wstr_ptr);
    
//    if(userAccount_ptr != PNULL)
    {
        SCI_FREE(userAccount_ptr);        
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : 获得sns账户活动更新信息
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetSNSActiveInfo(uint8  group_id,        //in    
                                 uint16 contact_id,           //in : index
                                 MMIPB_NAME_T name,    //in 
                                 void  **activity_ptr)     //out
{
    uint8 num = 0;
    uint32 sns_account_type = 0;
    MMIPB_SNS_CONTACT_T* data_ptr = PNULL;
    
    sns_account_type = GetSnsAccountType(group_id);       
    if(PNULL == s_mmipb_sns_handle)
    {
        IMMISNS_CreateObject(&s_mmipb_sns_handle);
    }   
    data_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_SNS_CONTACT_T));
    MMIPB_ReadSnsContact(PNULL,contact_id,data_ptr);    
    IMMISNS_PBGetActiveInfo(s_mmipb_sns_handle, sns_account_type, data_ptr->search_result.user_id, name.wstr, &num, activity_ptr);/*lint !e64*/
    SCI_Free(data_ptr);
    
    return num;   
}

/*****************************************************************************/
//  Description : 
//  Author: MARY.XIAO
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_GotoSNSActiveWin(uint8  group_id,        //in    
                                 uint16 contact_id)           //in : index  
{
    uint32 sns_account_type = 0;
    MMIPB_SNS_CONTACT_T* data_ptr = PNULL;
    
    sns_account_type = GetSnsAccountType(group_id);       
    if(PNULL == s_mmipb_sns_handle)
    {
        IMMISNS_CreateObject(&s_mmipb_sns_handle);
    }   
    data_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_SNS_CONTACT_T));
    MMIPB_ReadSnsContact(PNULL,contact_id,data_ptr);  
    //IMMISNS_AddRef(s_mmipb_sns_handle);
    IMMISNS_PBGotoActiveWin(s_mmipb_sns_handle, MMISNS_ACCOUNT_SINA, data_ptr->search_result.user_id);/*lint !e64*/
    SCI_Free(data_ptr);
    
   
}

/*****************************************************************************/
//  Description : 更新sns联系人信息
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UpdateSnsContact(void)
{
    uint32 sns_account_type = 0;
    uint8 i =0;
   
    MMISNS_Sqlite_CleanAll();
    for(i = MMIPB_GROUP_SNS_SINA; i < MMIPB_GROUP_SNS_MAX; i++)
    {
        sns_account_type =GetSnsAccountType(i);
        MMISNS_UpDatePBFriends(sns_account_type);/*lint !e64 !e718 !e746 !e628*/
    }
}
/*****************************************************************************/
//  Description : 更新sns联系人信息
//  Author: starmer.sun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_UpdateSnsContactByGroupId(uint8 group_id)
{
    uint32 sns_account_type = 0;
    //uint8 i =0;
    MMIPB_CloseSNSDB();
    MMISNS_Sqlite_CleanAll();
    MMIPB_OpenSNSDB(0);
    sns_account_type =GetSnsAccountType(group_id);
    MMISNS_UpDatePBFriends(sns_account_type);/*lint !e64 !e718 !e746 !e628*/

}

#endif

/*****************************************************************************/
//  Description : 获得手机联系人对应字段的空间使用情况
//  Global resource dependence :
//  Author: 
//  Note: 每个字段相应如果有空间，设置为TRUE，否则为FALSE
//        通过该函数可以获取各个号码字段，mail字段，别名字段的使用情况
//  Return:
/*****************************************************************************/
PUBLIC void MMIPB_GetPhoneContactFieldUageInfo(                                
                                 MMIPB_FIELD_USAGE_INOF_T *free_ptr
                                 )
{
     uint32 i  = 0;
     if(free_ptr != PNULL)
     {
         for(i = 0; i < MMIPB_MAX_NV_PHONE_NUM; i++)
         {
             free_ptr->number_free_flag[i] = TRUE;
         }
         free_ptr->photo_free_flag = TRUE;
         free_ptr->group_free_falg = TRUE;
         free_ptr->ring_free_falg = TRUE;
#ifdef ASP_SUPPORT         
         free_ptr->asp_free_falg = TRUE;
#endif
#ifdef MMIPB_MAIL_SUPPORT
         free_ptr->mail_free_flag = TRUE;
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
         free_ptr->memo_free_flag = TRUE;
         free_ptr->birth_free_flag = TRUE;
         free_ptr->url_free_flag = TRUE;
#endif
#ifdef CMCC_VCARD_SUPPORT
         free_ptr->sne_free_flag = FALSE;
#endif        
     //    free_ptr->title_free_flag = TRUE;
#ifdef SNS_SUPPORT
         free_ptr->sign_free_flag = FALSE;
#endif        
#ifdef MMIPB_IM_MESSAGE_SUPPORT
         free_ptr->im_free_flag = TRUE;
#endif
     }
     return;
}
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : 获得SNS联系人对应字段的空间使用情况
//  Global resource dependence :
//  Author: 
//  Note: 每个字段相应如果有空间，设置为TRUE，否则为FALSE
//        通过该函数可以获取各个号码字段，mail字段，别名字段的使用情况
//  Return:
/*****************************************************************************/
PUBLIC void MMIPB_GetSNSContactFieldUageInfo(                                
                                 MMIPB_FIELD_USAGE_INOF_T *free_ptr
                                 )
{
     uint32 i  = 0;
     if(free_ptr != PNULL)
     {
         for(i = 0; i < MMIPB_MAX_NV_PHONE_NUM; i++)
         {
             free_ptr->number_free_flag[i] = FALSE;
         }
         free_ptr->photo_free_flag = FALSE;
         free_ptr->group_free_falg = FALSE;
         free_ptr->ring_free_falg = FALSE;
#ifdef ASP_SUPPORT         
         free_ptr->asp_free_falg = FALSE;
#endif
#ifdef MMIPB_MAIL_SUPPORT
         free_ptr->mail_free_flag = TRUE;
#endif
#ifdef CMCC_VCARD_SUPPORT
         free_ptr->sne_free_flag = FALSE;
#endif
#ifndef PB_SUPPORT_LOW_MEMORY 
         free_ptr->birth_free_flag = TRUE;
         free_ptr->memo_free_flag = FALSE;
         free_ptr->url_free_flag = TRUE;
#endif
#ifdef SNS_SUPPORT
         free_ptr->sign_free_flag = TRUE;
#endif 
#ifdef MMIPB_IM_MESSAGE_SUPPORT
         free_ptr->im_free_flag = FALSE;
#endif
     }
     return;
}
#endif
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : 获得常用联系人对应字段的空间使用情况
//  Global resource dependence :
//  Author: 
//  Note: 每个字段相应如果有空间，设置为TRUE，否则为FALSE
//        通过该函数可以获取各个号码字段，mail字段，别名字段的使用情况
//  Return:
/*****************************************************************************/
PUBLIC void MMIPB_GetMostUsedContactFieldUageInfo(                                
                                 MMIPB_FIELD_USAGE_INOF_T *free_ptr
                                 )
{
     uint32 i  = 0;
     if(free_ptr != PNULL)
     {
         for(i = 0; i < MMIPB_MAX_NV_PHONE_NUM; i++)
         {
             free_ptr->number_free_flag[i] = FALSE;
         }
         free_ptr->number_free_flag[0] = TRUE;
         free_ptr->photo_free_flag = FALSE;
         free_ptr->group_free_falg = FALSE;
         free_ptr->ring_free_falg = FALSE;
#ifdef ASP_SUPPORT         
         free_ptr->asp_free_falg = FALSE;
#endif
#ifdef MMIPB_MAIL_SUPPORT
         free_ptr->mail_free_flag = FALSE;
#endif
#ifdef CMCC_VCARD_SUPPORT
         free_ptr->sne_free_flag = FALSE;
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
         free_ptr->birth_free_flag = FALSE;
         free_ptr->memo_free_flag = FALSE;
         free_ptr->url_free_flag = FALSE;
#endif
#ifdef SNS_SUPPORT         
         free_ptr->sign_free_flag = FALSE;
#endif
#ifdef MMIPB_IM_MESSAGE_SUPPORT
         free_ptr->im_free_flag = FALSE;
#endif
     }
     return;
}
#endif
/*****************************************************************************/
//  Description : 获得sim卡联系人对应字段的空间使用情况
//  Global resource dependence :
//  Author: 
//  Note: 每个字段相应如果有空间，设置为TRUE，否则为FALSE
//        通过该函数可以获取各个号码字段，mail字段，别名字段的使用情况
//  Return:如果contact_id为0，表示新增，返回一个新的id
/*****************************************************************************/
PUBLIC void MMIPB_GetSIMContactFieldUageInfo(   
                                 MN_DUAL_SYS_E dual_sys,
                                 PHONEBOOK_STORAGE_E storage,
                                 MMIPB_FIELD_USAGE_INOF_T *free_ptr
                                 )
{
    if(free_ptr == PNULL)
    {
        return;            
    }
    SCI_TRACE_LOW("xiaohua.liu MMIPB_GetSIMContactFieldUageInfo enter\n");
    SCI_MEMSET(free_ptr->number_free_flag, FALSE, sizeof(free_ptr->number_free_flag));
#ifdef CMCC_VCARD_SUPPORT
    if(PHONEBOOK_SIM_STORAGE == storage && MNSIM_Is_UsimEx(dual_sys))/*lint !e718 !e746 !e628*/
    {
        uint8 free_field = (MMI_ANR1_MASK|MMI_ANR2_MASK|MMI_ANR3_MASK|MMI_EMAIL_MASK);        
        uint8 i = 0;
        PHONEBOOK_VCARD_STATUS_T usim_storage_info = {0};

        SCI_TRACE_LOW("xiaohua.liu MMIPB_GetSIMContactFieldUageInfo cmcc_vcard_support\n");
        free_field = PHONEBOOK_AppCheckAddSpaceEx(dual_sys, free_field);
#ifdef MMIPB_MAIL_SUPPORT
        
        if(free_field & MMI_EMAIL_MASK)/*lint !e774*/
        {
            free_ptr->mail_free_flag = TRUE;
            SCI_TRACE_LOW("xiaohua.liu MMIPB_GetSIMContactFieldUageInfo mail is true\n");
        
        }
        else
        {
            free_ptr->mail_free_flag = FALSE;
            SCI_TRACE_LOW("xiaohua.liu MMIPB_GetSIMContactFieldUageInfo mail is false 1\n");
        
        }
#endif
        //adn always true
        SCI_TRACE_LOW("xiaohua.liu MMIPB_GetSIMContactFieldUageInfo 2222\n");
    free_ptr->number_free_flag[0] = TRUE;
        //set anr flag
        for(i = 0; i < MIN(MMIPB_GetUSIMFreeANRCount(dual_sys), (MMIPB_MAX_NV_PHONE_NUM -1)); i++)/*lint !e666*/
        {
            if((1 << i) & free_field)
            {
                free_ptr->number_free_flag[i+1] = TRUE;
            }
        }
        //set sne flag
        free_ptr->sne_free_flag = FALSE;
        PHONEBOOK_Get3GPhonebookStatusEx(dual_sys, &usim_storage_info);
        for(i = 0; i < MIN(PHONEBOOK_MAX_USAGE_STORAGE_NUM, usim_storage_info.storage_num); i++)
        {
            if(usim_storage_info.storage_usage[i].sne_usage.free_space_num >0 
              &&usim_storage_info.storage_usage[i].sne_usage.rec_len >0 
              )
            {
                free_ptr->sne_free_flag = TRUE;
                break;
            }
        }
    }
    else
#endif
    {        
        SCI_TRACE_LOW("xiaohua.liu MMIPB_GetSIMContactFieldUageInfo 3333\n");
    free_ptr->number_free_flag[0] = TRUE;
#ifdef MMIPB_MAIL_SUPPORT
        free_ptr->mail_free_flag = FALSE;
SCI_TRACE_LOW("xiaohua.liu MMIPB_GetSIMContactFieldUageInfo mail is false 2\n");
        
#endif
#ifdef CMCC_VCARD_SUPPORT
        free_ptr->sne_free_flag = FALSE;            
#endif
    }
    free_ptr->photo_free_flag = FALSE;
    free_ptr->group_free_falg = FALSE;
    free_ptr->ring_free_falg = FALSE;
#ifdef ASP_SUPPORT         
    free_ptr->asp_free_falg = FALSE;
#endif
#ifndef PB_SUPPORT_LOW_MEMORY    
    free_ptr->birth_free_flag = FALSE;
    free_ptr->memo_free_flag = FALSE;
    free_ptr->url_free_flag = FALSE;
#endif
#ifdef SNS_SUPPORT
    free_ptr->sign_free_flag = FALSE;
#endif    
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    free_ptr->im_free_flag = FALSE;
#endif
    return;    
}
#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : read usim additional contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadUSIMAdditionalContact(
                                          uint16 entry_id,
                                          MN_DUAL_SYS_E dual_sys
                                          )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;

    if(ERR_MNDATAMAG_NO_ERR == PHONEBOOK_ReadUsimAdditionalEntryEx(dual_sys, entry_id))
    {
        result = MMIPB_ERROR_ERROR;
    }
    return result;
}

/*****************************************************************************/
//  Description : delete usim contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_DeleteUSIMContact( uint16 entry_id,
                                             MN_DUAL_SYS_E dual_sys,
                                             PHONEBOOK_STORAGE_E storage,
                                             BOOLEAN anr_flag[MNSIM_ANR_MAX_NUM],
                                             BOOLEAN email_flag,
                                             BOOLEAN sne_flag
                                             )
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    /* fix compile error, modified by zhigang.peng 2019-07-15 */
    if(PHONEBOOK_SIM_STORAGE == storage &&!PHONEBOOK_GetAdnTelecomStatus(dual_sys))
    {
			if (ERR_MNDATAMAG_NO_ERR == PHONEBOOK_Delete3GPhonebookEx(dual_sys, 
                                                                      anr_flag, 
                                                                      email_flag, 
                                                                      sne_flag,
                                                                      (uint32)entry_id))
            {
                ret = MMIPB_ERROR_PENDING;
            }

    }
    else
    {
        SCI_TRACE_LOW("yanyan MMIPB_DeleteUSIMContact Delete 2G phonebook");
        if (ERR_MNDATAMAG_NO_ERR == PHONEBOOK_DeletePhonebookEntryEx(
            dual_sys,
            storage,
            (uint32)entry_id    // IN:
            )
            )
        {
            ret = MMIPB_ERROR_PENDING;
        }
    }

    return ret;
}

/*****************************************************************************/
//  Description : write usim contact
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteUSIMContact(										  
                                           uint16 entry_id,
                                           MN_DUAL_SYS_E dual_sys,
                                           PHONEBOOK_STORAGE_E storage,
			                               BOOLEAN	anr_flag[MN_MAX_USIM_ANR_NUM],
			                               BOOLEAN	email_flag,
			                               BOOLEAN  sne_flag,
			                               BOOLEAN	adn_flag,
                                           PHONEBOOK_ENTRY_T  *contact_ptr,//[IN]
                                           PHONEBOOK_ADDITIONAL_ENTRY_T	*additional_entry_ptr//[IN]
                                           )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_ERROR;
    ERR_MNDATAMAG_CODE_E sim_ret = ERR_MNDATAMAG_NO_ERR;
    /* fix compile error, modified by zhigang.peng 2019-07-15 */
    if(PHONEBOOK_SIM_STORAGE == storage &&!PHONEBOOK_GetAdnTelecomStatus(dual_sys))
    {        
     sim_ret = (ERR_MNDATAMAG_CODE_E) PHONEBOOK_Update3GPhonebookEx(
        dual_sys,
        contact_ptr,  
        anr_flag,	 //in
        email_flag, //in
        sne_flag,
        adn_flag,//TRUE, //in//bug355329
        additional_entry_ptr);
     
       SCI_TRACE_LOW("[MMIPB]MMIPB_WriteUSIMContact sim_ret:%d", sim_ret);
       
       if(sim_ret 
           &(MNPHONE_NO_ADN_SPACE | MNPHONE_NO_ANR1_SPACE| MNPHONE_NO_ANR2_SPACE| MNPHONE_NO_ANR3_SPACE| MNPHONE_NO_EMAIL_SPACE |MNPHONE_NO_SNE_SPACE)
          )
       {
           result = MMIPB_ERROR_NO_SPACE;
       }
       else if(sim_ret == MNPHONE_NO_ERR_SPACE)
       {

           result = MMIPB_ERROR_PENDING;
       }
    }
    else
    {
        SCI_TRACE_LOW("yanyan MMIPB_WriteUSIMContact Update 2G phonebook");
        if (ERR_MNDATAMAG_NO_ERR == PHONEBOOK_UpdatePhonebookEntryEx(
            dual_sys,
            storage,
            contact_ptr
            )
            )
        {
            result = MMIPB_ERROR_PENDING;
        }
    }
    
    SCI_TRACE_LOW("[MMIPB]MMIPB_WriteUSIMContact result = %d", result);
    return result;
}


/*****************************************************************************/
//  Description : 读取USIM 扩展号码的类型字符串
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_ReadUSIMAASString (
                           MN_DUAL_SYS_E dual_sys,//[IN]
                           uint8         aas_index,//[IN]
                           MMIPB_NAME_T *name_ptr//[OUT]
                           )
{
     PHONEBOOK_AAS_ENTRY_T* aas_entry_arr = PNULL;
     MMIPB_ERROR_E          result = MMIPB_ERROR_INVALID_PARAM;
     //uint8 count = 0;
     uint16 wstr_len = 0;
     uint32 i = 0;
     MMI_STRING_T  default_aas_string = {0};

     if(dual_sys < MMI_DUAL_SYS_MAX && name_ptr != PNULL)
     {
         if(s_mmipb_ass_type_num[dual_sys] == 0)
         {
             //没有初始化，需要从usim中读取
             aas_entry_arr = (PHONEBOOK_AAS_ENTRY_T *)SCI_ALLOCA(sizeof(PHONEBOOK_AAS_ENTRY_T)*MMI_AAS_PHONE_NUM);
             if(aas_entry_arr != PNULL)
             {
                 SCI_MEMSET(aas_entry_arr, 0x00, sizeof(PHONEBOOK_AAS_ENTRY_T)*MMI_AAS_PHONE_NUM);
                 SCI_MEMSET(&s_mmipb_aas_str_tab[dual_sys], 0x00, sizeof(s_mmipb_aas_str_tab[dual_sys]));
                 s_mmipb_ass_type_num[dual_sys] = PHONEBOOK_ReadUsimAasEx(dual_sys ,MMI_AAS_PHONE_NUM, (PHONEBOOK_AAS_ENTRY_T*) aas_entry_arr);
                 for( i = 0; i < MIN(s_mmipb_ass_type_num[dual_sys],MMI_AAS_PHONE_NUM); i++)
                 {
                     MMIPB_GSMAlphabetToWstr(aas_entry_arr[i].alpha, 
                                             aas_entry_arr[i].alpha_len,
                                             s_mmipb_aas_str_tab[dual_sys][i].wstr,
                                             &wstr_len,MMIPB_NAME_MAX_LEN
                                             );
                      s_mmipb_aas_str_tab[dual_sys][i].wstr_len = wstr_len;
                 }

                 SCI_FREE(aas_entry_arr);
             }
         }
         
        SCI_TRACE_BUF("MMIPB_ReadUSIMAASString aas_str[0]: ", s_mmipb_aas_str_tab[dual_sys][0].wstr, s_mmipb_aas_str_tab[dual_sys][0].wstr_len*sizeof(wchar));
        SCI_TRACE_BUF("MMIPB_ReadUSIMAASString aas_str[1]: ", s_mmipb_aas_str_tab[dual_sys][1].wstr, s_mmipb_aas_str_tab[dual_sys][1].wstr_len*sizeof(wchar));
        SCI_TRACE_BUF("MMIPB_ReadUSIMAASString aas_str[2]: ", s_mmipb_aas_str_tab[dual_sys][2].wstr, s_mmipb_aas_str_tab[dual_sys][2].wstr_len*sizeof(wchar));
         
         SCI_TRACE_LOW("[MMIPB] type_count:%d,s_mmipb_aas_str_tab[%d][%d].wstr_len:%d", s_mmipb_ass_type_num[dual_sys],dual_sys,aas_index,s_mmipb_aas_str_tab[dual_sys][aas_index].wstr_len);
         if(aas_index < MMI_AAS_PHONE_NUM)
         {
            SCI_MEMCPY(name_ptr, &s_mmipb_aas_str_tab[dual_sys][aas_index], sizeof(MMIPB_NAME_T));            
            result = MMIPB_ERROR_SUCCESS;
         }
     }
     return result;
}

/*****************************************************************************/
//  Description : 写USIM 扩展号码的类型字符串
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:本功能有3G PS还没有支持，暂时保留
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_WriteUSIMAASString(
                           MN_DUAL_SYS_E dual_sys,//[IN]
                           uint8         aas_index,//[IN]
                           MMIPB_NAME_T *name_ptr//[OUT]
                           )
{
    return MMIPB_ERROR_INVALID_PARAM;
}


/*****************************************************************************/
//  Description : 读取usim 支持的扩展号码的类型个数
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetUSIMAASCount(
                           MN_DUAL_SYS_E dual_sys
                           )
{
     PHONEBOOK_AAS_ENTRY_T* aas_entry_arr = PNULL;
     uint8 count = 0;
     uint16 wstr_len = 0;
     uint32 i = 0;

     if(dual_sys < MMI_DUAL_SYS_MAX)
     {
         if(s_mmipb_ass_type_num[dual_sys] == 0)
         {
             //没有初始化，需要从usim中读取
             aas_entry_arr = (PHONEBOOK_AAS_ENTRY_T *)SCI_ALLOCA(sizeof(PHONEBOOK_AAS_ENTRY_T)*MMI_AAS_PHONE_NUM);
             if(aas_entry_arr != PNULL)
             {
                 SCI_MEMSET(aas_entry_arr, 0x00, sizeof(PHONEBOOK_AAS_ENTRY_T)*MMI_AAS_PHONE_NUM);
                 SCI_MEMSET(&s_mmipb_aas_str_tab[dual_sys], 0x00, sizeof(s_mmipb_aas_str_tab[dual_sys]));
                 s_mmipb_ass_type_num[dual_sys] = PHONEBOOK_ReadUsimAasEx(dual_sys ,MMI_AAS_PHONE_NUM, (PHONEBOOK_AAS_ENTRY_T*) aas_entry_arr);
                 for( i = 0; i < MIN(s_mmipb_ass_type_num[dual_sys], MMI_AAS_PHONE_NUM); i++)
                 {
                     MMIPB_GSMAlphabetToWstr(aas_entry_arr[i].alpha, 
                                             aas_entry_arr[i].alpha_len,
                                             s_mmipb_aas_str_tab[dual_sys][i].wstr,
                                             &wstr_len,MMIPB_NAME_MAX_LEN
                                             );
                      s_mmipb_aas_str_tab[dual_sys][i].wstr_len = wstr_len;
                 }

                 SCI_FREE(aas_entry_arr);

             }
         }
         count = s_mmipb_ass_type_num[dual_sys];
     }
     SCI_TRACE_LOW("[MMIPB] MMIPB_GetUSIMAASCount count : %d", count);
     return count;
}

/*****************************************************************************/
//  Description : 读取usim 支持的扩展号码的可用个数
//  Global resource dependence : 
//  Author:yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetUSIMFreeANRCount(
								   MN_DUAL_SYS_E dual_sys
                           )
{
    PHONEBOOK_VCARD_STATUS_T usim_storage_info = {0};
	uint8  count = 0;
	uint8   i = 0;
   
	if(MNSIM_Is_UsimEx(dual_sys))
	{
		PHONEBOOK_Get3GPhonebookStatusEx(dual_sys, &usim_storage_info);

		for (i = 0; i < MNSIM_ANR_MAX_NUM; i++)
		{
			if((0 < usim_storage_info.storage_usage[0].anr_usage[i].free_space_num)
				&&(0 < usim_storage_info.storage_usage[0].anr_usage[i].rec_len))
			{
				count++;
			}
		}
	}
    SCI_TRACE_LOW("[MMIPB] MMIPB_GetUSIMANRCount anr_usage[0].free_space_num : %d", usim_storage_info.storage_usage[0].anr_usage[0].free_space_num);
    SCI_TRACE_LOW("[MMIPB] MMIPB_GetUSIMANRCount anr_usage[1].free_space_num : %d", usim_storage_info.storage_usage[0].anr_usage[1].free_space_num);
    SCI_TRACE_LOW("[MMIPB] MMIPB_GetUSIMANRCount anr_usage[2].free_space_num : %d", usim_storage_info.storage_usage[0].anr_usage[2].free_space_num);
    SCI_TRACE_LOW("[MMIPB] MMIPB_GetUSIMANRCount count : %d", count);
    
	return count;
}
#endif

