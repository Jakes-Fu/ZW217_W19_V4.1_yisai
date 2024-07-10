/*************************************************************************
 ** File Name:      mmisms_order.h                                       *
 ** Author:         liming.deng                                          *
 ** Date:           2007/8/23                                            *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about ordering sms    *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2007/8/23      liming.deng      Create.                              *
*************************************************************************/

#ifndef _MMISMSAPP_INTERFACE_H_
#define _MMISMSAPP_INTERFACE_H_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmisms_export.h"
#include "mmisms_app.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/                            
//typedef for srv operations
typedef enum
{
   SRV_OPERATION_READ,
   SRV_OPERATION_SAVE,
   SRV_OPERATION_REPLACE,
   SRV_OPERATION_WRITE,
   SRV_OPERATION_COPY,
   SRV_OPERATION_DELETE,
   SRV_OPERATION_SEND,
   SRV_OPERATION_MOVE,
   SRV_OPERATION_MAX
}SRV_OPERATION_ENUM_E;

typedef struct
{
    BOOLEAN result;                     /* Result of the operation: TRUE: success; FALSE: failed */
    SRV_OPERATION_ENUM_E operation;     /* indicate the operation (read/save/Delete...)*/
    MMISMS_READ_MSG_T *opreation_cb;    /* Callback data*/
    MMISMS_ORDER_ID_T oper_order_id;
    void *user_data;                    /* User data */
} SMS_READ_CALLBACK_STRUCT_T;

typedef struct
{
    BOOLEAN result;                     /* Result of the operation: TRUE: success; FALSE: failed */
    SRV_OPERATION_ENUM_E operation;     /* indicate the operation (read/save/Delete...)*/
	uint32 cause;         /* oper cause type*/
    MMISMS_READ_MSG_T *opreation_cb;    /* Callback data*/
    void *user_data;                    /* User data */
} SMSSRV_CALLBACK_STRUCT_T;

typedef struct
{
    BOOLEAN result;
}SMSDATASRV_OPERATE_RESULT_T;

typedef struct 
{
    MN_DUAL_SYS_E      dual_sys;
    MN_SMS_STORAGE_E   storage;   
    MN_SMS_RECORD_ID_T record_id;
    MMISMS_ORDER_ID_T  oper_order_id;
}SMS_READ_OPERATE_DATA_T;

typedef struct 
{
    MMISMS_ORDER_ID_T move_order_id;
    MMISMS_OPER_STATUS_E oper_status;  //²Ù×÷ÀàÐÍ
}SMS_MOVE_OPERATE_DATA_T;
typedef struct 
{
    MMISMS_ORDER_ID_T copy_order_id;
}SMS_COPY_OPERATE_DATA_T;

typedef struct 
{
    MN_DUAL_SYS_E      dual_sys;
    BOOLEAN            is_resend;
    MMISMS_DEST_ADDR_INFO_T*  sms_addr;
    wchar         *save_string_ptr;    //edit_string
    uint32        save_strlen;    
}SMS_SEND_OPERATE_DATA_T;

typedef struct 
{
    MN_DUAL_SYS_E      dual_sys;
    MMISMS_DEST_ADDR_INFO_T*  sms_addr;
    wchar         *save_string_ptr;    //edit_string
    uint32        save_strlen;
    BOOLEAN       is_needLoadSaved; 
    BOOLEAN       is_use_global_data;
    BOOLEAN       is_timer_msg;
}SMS_SAVE_OPERATE_DATA_T;

typedef BOOLEAN (*SmsSrvCallbackFunc)(SMSSRV_CALLBACK_STRUCT_T* callback_data);

typedef struct
{
    SMSSRV_CALLBACK_STRUCT_T oper_cb_data;
    SmsSrvCallbackFunc       callback_func;
}SRV_OPERATION_DATA_T;

typedef struct SMSDATA_SRV_HANDLE_TAG
{
    MN_SMS_RECORD_ID_T record_id; // the sms position
    MN_SMS_STORAGE_E   storage;   // where the corresponded message is saved
    SRV_OPERATION_ENUM_E oper_type;
    SRV_OPERATION_DATA_T oper_data;
}SMSDATA_SRV_HANDLE_T;

typedef struct SMSDATA_SRV_HANDLE_TAG* SMS_HANDLE_T;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
/******************************************************************************
//  Description : sms handle sysn callback
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC BOOLEAN SMSSRV_Sync_Callback(
    SRV_OPERATION_ENUM_E operation,
    BOOLEAN result,
    uint32 cause,
    void *opreation_cb,
    void *user_data,
    SmsSrvCallbackFunc callback_func);

/******************************************************************************
//  Description : sms api: Delete
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_DeleteSms(
    MMISMS_ORDER_ID_T order_id,
    SmsSrvCallbackFunc delte_callback,
    void *action_data,
    void *user_data);

/*****************************************************************************/
//     Description : delete cnf call back
//                  deleting the related MO SMS box
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMSAPP_HandleDeleteCnf(MN_SMS_RECORD_ID_T record_id,MN_SMS_STORAGE_E   storage);

/*****************************************************************************/
//     Description : delete cnf call back
//                  deleting the related MO SMS box
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMSAPP_HandleMoveCnf(MMISMS_OPER_ERR_E oper_err);

/*****************************************************************************/
//     Description : delete cnf call back
//                  deleting the related MO SMS box
//    Global resource dependence : 
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMSAPP_HandleCopyCnf(MMISMS_OPER_ERR_E oper_err);

/******************************************************************************
//  Description : sms api: save sms
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_SaveSms(
    SMS_SAVE_OPERATE_DATA_T op_data,
    SmsSrvCallbackFunc save_callback,
    void *action_data,
    void *user_data);

 
 /******************************************************************************
//  Description : sms api: Replace sms
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_ReplaceSms(
    SMS_SAVE_OPERATE_DATA_T op_data,
    SmsSrvCallbackFunc save_callback,
    void *action_data,
    void *user_data);


#ifdef MMI_SMS_EXPORT_SUPPORT
/******************************************************************************
//  Description : sms api: Export SMS
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_ReadExportSms(MMISMS_ORDER_ID_T oper_order_id,
    SmsSrvCallbackFunc read_callback,
    void *action_data,
    void *user_data);
#endif    

/******************************************************************************
//  Description : sms api: Read sms common api
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_ReadSms(SMS_READ_OPERATE_DATA_T op_data,
    SmsSrvCallbackFunc read_callback,
    void *action_data,
    void *user_data);

/******************************************************************************
//  Description : sms api: Read Chat sms
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_ReadChatSms(MMISMS_ORDER_ID_T oper_order_id,
    SmsSrvCallbackFunc read_callback,
    void *action_data,
    void *user_data);

#ifdef MMI_TIMERMSG_SUPPORT
/******************************************************************************
//  Description : sms api: Read timer sms
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_ReadTimerSms(MMISMS_ORDER_ID_T oper_order_id,
    SmsSrvCallbackFunc read_callback,
    void *action_data,
    void *user_data);    
#endif

/******************************************************************************
//  Description : sms api: Read one sms by id
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_ReadOneSms(MMISMS_ORDER_ID_T oper_order_id,
    SmsSrvCallbackFunc read_callback,
    void *action_data,
    void *user_data);    


#ifdef MMI_SMS_REPLY_PATH_SUPPORT
/******************************************************************************
//  Description : sms api: Read sms to get sms center
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_ReadSmsCenter(MMISMS_ORDER_ID_T oper_order_id,
    SmsSrvCallbackFunc read_callback,
    void *action_data,
    void *user_data);
#endif    

/******************************************************************************
//  Description : sms api:send sms with data
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_SendSmsWithData(
                                MN_DUAL_SYS_E    dual_sys,
                                BOOLEAN        is_re_send ,
                                MMISMS_DEST_ADDR_INFO_T*  sms_addr,
                                wchar         *save_string_ptr,    
                                uint32        save_strlen,    
                                SmsSrvCallbackFunc read_callback
                               );
/******************************************************************************
//  Description : sms api:send sms with data
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_SendSmsWithContent(
                                MN_DUAL_SYS_E    dual_sys,
                                BOOLEAN        is_re_send ,
                                MMISMS_SMSEDIT_CONTENT_T*  edit_content,
                                SmsSrvCallbackFunc read_callback
                               );

/******************************************************************************
//  Description : sms api: send sms by global content
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_SendSmsWithGlobalContent(
                                MN_DUAL_SYS_E    dual_sys,
                                BOOLEAN          is_re_send
                               );

PUBLIC BOOLEAN MMISMSAPP_HandleSendCnf(BOOLEAN is_succss);
/******************************************************************************
//  Description : sms cnf handler: Read cnf
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC BOOLEAN MMISMSAPP_HandleReadCnf(MN_SMS_RECORD_ID_T record_id,MN_SMS_STORAGE_E   storage,BOOLEAN is_success);

/******************************************************************************
//  Description : sms cnf handler: save cnf
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC BOOLEAN MMISMSAPP_HandleSaveCnf(BOOLEAN is_succss);

/******************************************************************************
//  Description : sms cnf handler: Replace cnf
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/  
PUBLIC BOOLEAN MMISMSAPP_HandleReplaceCnf(MN_SMS_RECORD_ID_T record_id,MN_SMS_STORAGE_E   storage,BOOLEAN is_success);

/******************************************************************************
//  Description : sms api: move sms
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_MoveSms(SMS_MOVE_OPERATE_DATA_T op_data,
                                    SmsSrvCallbackFunc move_callback,
                                    void *action_data,
                                    void *user_data);


/******************************************************************************
//  Description : sms api: copy sms
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
PUBLIC SMS_HANDLE_T MMISMSAPP_CopySms(SMS_COPY_OPERATE_DATA_T op_data,
                                SmsSrvCallbackFunc move_callback,
                                void *action_data,
                                void *user_data);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif 
