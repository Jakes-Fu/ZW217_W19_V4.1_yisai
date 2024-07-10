/*************************************************************************
 ** File Name:      mmisms_nv.h                                          *
 ** Author:         Tracy Zhang                                          *
 ** Date:           2006/10/24                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about NV              *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/10/24     Tracy Zhang      Create.                              *
*************************************************************************/

#ifndef _MMISMS_NV_H_
#define _MMISMS_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"
#include "mn_type.h"
#include "mmismscb_control.h"
#include "mmi_custom_define.h"
#include "mmisms_export.h"
#ifdef MMI_TIMERMSG_SUPPORT
#include "mmiacc_event.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

typedef uint8 MMINV_SMS_REFERENCE_NUM_T;
typedef BOOLEAN MMINV_SMS_STATUS_REPORT_T;
typedef BOOLEAN MMINV_SMS_REPLY_PATH_T;

typedef uint8     MMINV_SMS_AUTOSAVE_T;
#define MMINV_SMS_SETTING_PROMPT    0
#define MMINV_SMS_SETTING_SAVE        1
#define MMINV_SMS_SETTING_NOTSAVE    2

#define MMISMS_SECURITY_PWD_MIN_LEN         1
#define MMISMS_SECURITY_PWD_MAX_LEN         8
#define MMISMS_SECURITY_PWD_DEFAULT_LEN    4
#ifdef MMI_TIMERMSG_SUPPORT
#define MMISMS_TIMERMSG_MAX_ITEM			50
#endif

typedef int32       MMINV_SMS_VALID_PERIOD_T;

typedef BOOLEAN     MMINV_SMS_BROWSER_INFO_T;

typedef BOOLEAN     MMINV_SMS_IS_EXTRACT_T;

#if defined(MMI_SMSCB_SUPPORT)
typedef BOOLEAN     MMINV_SMSCB_ENABLE_T;                            //是否开启小区广播

typedef MMISMS_CB_LANGUAGE_T MMINV_SMSCB_LANG_T;                     //小区广播语言选择列表

typedef MMISMS_CB_CHANNEL_T MMINV_SMSCB_CHANNEL_T;                   //小区广播信道选择列表

#ifdef MMI_ETWS_SUPPORT
typedef MMISMS_CB_MESSAGE_ARRAY_T MMINV_SMSCB_MESSAGE_ARRAY_T;
#endif

#endif

typedef uint8 MMINV_SMS_SIM_SETMODE_T; 

typedef struct
{
    BOOLEAN   is_default_word;
    uint8   wstr_len;
    wchar wstr_data[ MMISMS_FREQUENT_WORD_LEN + 1];
}MMI_NV_SMS_PHRASE_T;

typedef struct 
{
    BOOLEAN is_locked[MMINV_MAX_SMS_RECORDS];
}MMINV_SMS_LOCK_STATE_T;

typedef struct
{
    MMI_NV_SMS_PHRASE_T word[ MMISMS_FREQUENT_WORD_NUM ];
}MMINV_SMS_FREQUENT_WORD_T;

typedef struct
{
    MMISMS_FOLDER_TYPE_E    folder_type[MMINV_MAX_SMS_RECORDS];                              
}MMINV_SMS_FOLDER_TYPE_T;

typedef struct  
{
    uint8       pwd[MMISMS_SECURITY_PWD_MAX_LEN];   //安全保护密码
    uint8       pwd_len;									//密码长度
}MMISMS_SECURITYBOX_PWD_T;

#if defined(MMI_SMS_EXPORT_SUPPORT)
typedef struct
{
    wchar file_name[ MMIFILE_LONG_FULL_PATH_MAX_LEN + 1 ];
}MMINV_SMS_EXPORT_FILENAME_T;
#endif

#ifdef MMI_TIMERMSG_SUPPORT
typedef struct
{
    MMIACC_SMART_EVENT_FAST_TABLE_T fast_table;
    uint32 event_id;
    uint16 record_id;

    struct 
    {
        uint16 storage:2;//MN_SMS_STORAGE_E(存储位置)
        uint16 dual_sys:3;//MN_DUAL_SYS_E(多卡信息)
        uint16 mo_mt_type:4;//MMISMS_MO_MT_TYPE_E(信箱类型)
        uint16 is_in_use:1;//BOOLEAN(是否已使用)
    } flag;

}MMINV_SMS_TIMERMSG_T;
#endif


typedef enum
{
    MMINV_SMS_REFERENCE_NUM = MMI_MODULE_SMS << 16,
    MMINV_SMS_STATUS_REPORT_FLAG,
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    MMINV_SMS_REPLY_PATH_FLAG,
#endif
#ifndef MMISMS_CHATMODE_ONLY_SUPPORT
    MMINV_SMS_AUTOSAVE_FLAG,
#endif    
    MMINV_SMS_VALID_PERIOD,
    MMINV_SMS_SAVE_POS,
    MMINV_SMS_PREFERRED_CONNECTION,
    MMINV_SMS_FREQUENT_WORD,
#ifdef TTS_SUPPORT
    MMINV_SMS_TTS_FLAG,
    MMINV_SMS_TTS_VOL,
#endif
#ifdef HERO_ENGINE_TTS_SUPPORT    
    MMINV_TTS_HERO_FLAG,
#endif

    MMINV_SMS_LOCK_STATE,
    MMINV_SMS_BROWSER_INFO,
    MMINV_SMS_BROWSER_INFO_SL_OPERATION,
    MMINV_SMS_IS_EXTRACT,

#if defined(MMI_SMSCB_SUPPORT)
    MMINV_SMSCB_ENABLE,
    MMINV_SMSCB_CHANNEL_LIST,
    MMINV_SMSCB_LANGUAGE_LIST,
#ifdef MMI_ETWS_SUPPORT
    MMINV_SMSCB_MESSAGE_LIST_NUM,
#endif
#endif

    MMINV_SMS_ORDER_TYPE,

    MMINV_SMS_FOLDER_TYPE,
    MMINV_SMS_SECURITY_PWD,
#if defined(MMI_SMS_EXPORT_SUPPORT)
    MMINV_SMS_EXPORT_METHOD,
    MMINV_SMS_EXPORT_PATH,
#endif
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    MMINV_SMS_FONT_SIZE,
#endif
#ifdef MMI_TIMERMSG_SUPPORT
    MMINV_SMS_TIMERMSG,
#endif
 
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
    //auto signature flag
    MMINV_SMS_AUTO_SIGNATURE_FLAG,
    MMINV_SMS_AUTO_SIGNATURE_CONTENT,
#endif

#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    MMINV_SMS_SIM_SETMODE,
#endif
#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
    MMINV_SMS_CONCATENATED_SMS,
#endif
#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
    MMINV_SMS_ALPHABET_MANAGEMENT,
#endif
#ifdef MMI_SMS_FIX_SEND_SIM
        MMINV_SMS_FIX_SIM_SEND_ON,
        MMINV_SMS_FIX_SIM_SEND_SIMSYS,
#endif

#ifdef MMI_MSG_RESEND
    MMINV_SMS_RESEND_FLAG,
#endif
#ifdef MMI_SMS_VOICE_MAIL_SUPPORT
    MMINV_SMS_VM_FLAG,
#endif
    MMINV_SMS_RESEND_FLAG,
    MMINV_SMSCB_CELL_BROADCAST,
    MMINV_SMSCB_CELL_BROADCAST_CHANNEL,
    MMIUSERNV_SMSCB_MESSAGE_LIST_NUM,
    MMIUSERNV_SMSCB_MESSAGE_LIST_FIRST,

    // SMS item
    MMINV_SMS_NV_FIRST_ENTRY,
    MMINV_SMS_NV_LASTT_ENTRY = MMINV_SMS_NV_FIRST_ENTRY + MMINV_MAX_SMS_RECORDS -1,
}SMS_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
//     Description : register set module NV len and max item
//    Global resource dependence : none
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_RegNv(void);

/*****************************************************************************/
//     Description :read SMS in NV by MMI
//    Global resource dependence :  
//  Author:alex.ye
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_NV_ReadSmsItem(                                        //RETURN:
                                    uint16                        rid,        //IN:
                                    MN_SMS_MESSAGE_ITEM_T    *item_ptr    //OUT:
                                    );

/*****************************************************************************/
//     Description :write SMS in NV by mmi
//    Global resource dependence :  
//  Author:alex.ye
//    Note:
/*****************************************************************************/                                    
BOOLEAN MMIAPISMS_NV_WriteSmsItem(
                                     BOOLEAN    is_need_sync,
                                     uint16     rid,
                                     MN_SMS_MESSAGE_ITEM_T    *item_ptr
                                    );

/*****************************************************************************/
//     Description :set max SMS num in NV by MMI
//    Global resource dependence :  
//  Author:jian.ma
//    Note:
/*****************************************************************************/      
BOOLEAN MMISMS_NV_SetMaxSmsNum(                                            //RETURN:
                                     MN_SMS_RECORD_ID_T        *max_sms_num_ptr    //OUT:
                                     );

/*****************************************************************************/
//     Description :delete all SMS item in NV by MMI
//    Global resource dependence :  
//  Author:jian.ma
//    Note:
/*****************************************************************************/    
BOOLEAN MMISMS_NV_DelAllSmsItem(                                            //RETURN:
                                      MN_SMS_RECORD_ID_T    sms_in_me_valid_num    //IN:
                                      );

/*****************************************************************************/
//     Description : to init the flag of status report
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_NV_GetSmsIsLocked(MN_SMS_RECORD_ID_T record_id);

/*****************************************************************************/
//     Description : to Set the status report flag
//    Global resource dependence : g_mmisms_global
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
void MMISMS_NV_SetSmsLockState(MN_SMS_RECORD_ID_T record_id, BOOLEAN is_locked);

/*****************************************************************************/
// 	Description : Read Folder Type Manager
//	Global resource dependence : none
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_NV_ReadFolderTypeManager(MMINV_SMS_FOLDER_TYPE_T *folder_type_ptr);

/*****************************************************************************/
// 	Description :  Write Folder Type Manager
//	Global resource dependence : none
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_WriteFolderTypeManager(MMINV_SMS_FOLDER_TYPE_T *folder_type_ptr);

/*****************************************************************************/
// 	Description : 设置安全信箱密码
//	Global resource dependence : 
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_NV_SetSecurityBoxPwd(
                                 const wchar *pwd_ptr,
                                 uint16     pwd_len
                                 );

#if defined(MMI_SMS_EXPORT_SUPPORT)
/*****************************************************************************/
// 	Description : Read Export path
//	Global resource dependence : none
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_ReadExportPath(wchar *path_ptr);

/*****************************************************************************/
// 	Description : Write Export path
//	Global resource dependence : none
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_WriteExportPath(wchar *path_ptr);

/*****************************************************************************/
// 	Description : Read Export Method type
//	Global resource dependence : none
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_ReadExportMethodType(MMISMS_SETTING_PATH_TYPE *method_type);

/*****************************************************************************/
// 	Description : Write Export Method type
//	Global resource dependence : none
//  Author: fengming.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_WriteExportMethodType(MMISMS_SETTING_PATH_TYPE method_type);
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: get securitybox default key
//  return: 
/*****************************************************************************/
PUBLIC char* MMISMS_GetSecurityDefaultPwd(void);

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description :read SMSCB in NV by MMI
//    Global resource dependence :
//  Author:Candice
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_NV_ReadSmsCBItem(                                    //RETURN:
                              uint16                    rid,        //IN:
                              MMINV_SMSCB_MESSAGE_ARRAY_T    *item_ptr    //OUT:
                              );

/*****************************************************************************/
//     Description :write SMSCB in NV by MMI
//    Global resource dependence :
//  Author:Candice
//    Note:
/*****************************************************************************/
BOOLEAN MMIAPISMS_NV_WriteSmsCBItem(                                            //RETURN:
                               BOOLEAN                    is_need_sync,    //IN:
                               uint16                     rid,            //IN:
                               MMINV_SMSCB_MESSAGE_ARRAY_T        *item_ptr        //IN:
                               );
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISMS_NV_H_

