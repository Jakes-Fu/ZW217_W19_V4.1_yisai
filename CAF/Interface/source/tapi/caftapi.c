#include "caf_trc.h"
#ifdef DYNAMIC_MODULE_SUPPORT
/****************************************************************************
** File Name:      caftapi.c                                               *
** Author:         Wei.Zhou                                                *
** Date:           02/06/2009                                              *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 10/2008        Wei.Zhou          Create                                 *
****************************************************************************/


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "cafmodule_gen.h"
#include "mn_api.h"
#include "mmk_app.h"
#include "sig_code.h"
#include "mmi_common.h"
#include "mmi_signal_ext.h"
#include "mmipdp_export.h"
#include "mmk_shell.h"
#include "mmicom_string.h"
#include "mmi_module.h"
#include "mmk_ext_app.h"
#include "mmk_handle_internal.h"
#include "caftapi.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define     MMI_MODULE_TAPI                        0x8000

#define     ITAPI_DEFAULT_EXT_MARK                 0x1b

#define     ITAPI_SMS_MAX_MESSAGE_LEN              (ITAPI_SMS_SPLIT_MAX_NUM * 153)
#define     ITAPI_SMS_HEAD_LENGTH                  5   // the length of SMS head
#define     ITAPI_SMS_HEAD_IEI_POS                 0   // the pos of SMS indicate
#define     ITAPI_SMS_HEAD_IEL_POS                 1   // the pos of SMS indicate length
#define     ITAPI_SMS_HEAD_REF_NUM_POS             2   // the pos of reference number
#define     ITAPI_SMS_HEAD_MAX_NUM_POS             3   // the pos of max number
#define     ITAPI_SMS_HEAD_SEQ_NUM_POS             4   // the pos sequence number   
#define     ITAPI_SMS_HEAD_IEI                     0x00  // the indicate of SMS 
#define     ITAPI_SMS_HEAD_IEL                     0x03  // the length of SMS indicate

#define     ITAPI_SMS_JAVA_MAX_UCS2_BYTE_LEN       134
#define     ITAPI_SMS_JAVA_MAX_UCS2_SPLIT_LEN      128
#define     ITAPI_SMS_JAVA_MAX_DEF_BYTE_LEN        154
#define     ITAPI_SMS_JAVA_MAX_DEF_SPLIT_LEN       147
#define     ITAPI_SMS_JAVA_MAX_8BIT_BYTE_LEN       134
#define     ITAPI_SMS_JAVA_MAX_8BIT_SPLIT_LEN      128

#define     ITAPI_SMS_VCARD_MAX_UCS2_BYTE_LEN      ITAPI_SMS_JAVA_MAX_UCS2_BYTE_LEN
#define     ITAPI_SMS_VCARD_MAX_UCS2_SPLIT_LEN     ITAPI_SMS_JAVA_MAX_UCS2_SPLIT_LEN
#define     ITAPI_SMS_VCARD_MAX_DEF_BYTE_LEN       ITAPI_SMS_JAVA_MAX_DEF_BYTE_LEN
#define     ITAPI_SMS_VCARD_MAX_DEF_SPLIT_LEN      ITAPI_SMS_JAVA_MAX_DEF_SPLIT_LEN
#define     ITAPI_SMS_VCARD_MAX_8BIT_BYTE_LEN      ITAPI_SMS_JAVA_MAX_8BIT_BYTE_LEN
#define     ITAPI_SMS_VCARD_MAX_8BIT_SPLIT_LEN     ITAPI_SMS_JAVA_MAX_8BIT_SPLIT_LEN

#define     ITAPI_SMS_MAX_8BIT_BYTE_LEN            140
#define     ITAPI_SMS_MAX_8BIT_SPLIT_LEN           134

#define     ITAPI_SMS_MAX_DEF_BYTE_LEN             160
#define     ITAPI_SMS_MAX_DEF_SPLIT_LEN            153

#define     ITAPI_SMS_MAX_UCS2_BYTE_LEN            140
#define     ITAPI_SMS_MAX_UCS2_SPLIT_LEN           134

/**--------------------------------------------------------------------------*
**                         STURCT DEFINITION                                *
**--------------------------------------------------------------------------*/
typedef struct _ITAPI_REG_ITEM_T
{
    CAF_HANDLE_T dest_handle;
    uint16       ps_msg;
    uint16       unused;
}ITAPI_REG_ITEM_T;

typedef struct _ITAPI_SMS_CTRL_T
{
    ITAPI_SMS_MO_SMS_T          tapi_sms;
    MN_SMS_MO_SMS_T             mn_sms;
    MnSmsMtIndS                 *mn_cnf_ptr;
    MN_DUAL_SYS_E               dual_sys;
    MN_SMS_STORAGE_E            storage;
    MN_SMS_PATH_E               sms_path;
    MN_SMS_STATUS_E             status;
    MN_SMS_ALPHABET_TYPE_E      alphabet_type;      // the alphabet type
    uint32                      total_num;
    uint32                      seq_num;
    uint32                      save_num;
    uint16                      record_id_arr[ITAPI_SMS_SPLIT_MAX_NUM];
    ITAPI_SMS_USER_DATA_HEAD_T  user_data_header_t;
    uint16                      data_length;
    uint16                      split_sms_length[ITAPI_SMS_SPLIT_MAX_NUM];
    uint8                       ascii_content[ITAPI_SMS_MAX_MESSAGE_LEN];
    uint8                       default_data[ITAPI_SMS_MAX_MESSAGE_LEN * 2];
    MMI_PARTY_NUMBER_T          party_number;
    MN_SMS_USER_VALID_DATA_T    mn_user_valid_data;
    MN_SMS_USER_DATA_HEAD_T     user_data_header;
}ITAPI_SMS_CTRL_T;

typedef struct
{
    CAF_VTBL_T(ITAPI_T)     *vtbl_ptr; // º¯ÊýÁÐ±í
    CAF_GUID_T              guid;
    uint32                  ref_num;
    int32                   retrant;
    int32                   netstate;
    BOOLEAN                 sendneedsave;   /* while sending! */
    BOOLEAN                 need_posttransfer;
    MN_DUAL_SYS_E           dual_sys;
    ITAPI_SMS_CTRL_T        *sms_ctrl_ptr;
    ITAPI_REG_ITEM_T        **tapi_reg_items_pptr;
}CAFTAPI_T;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_E dual_sys);

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : tapi class created.
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFITAPI_ClassNew(CAF_GUID_T guid, void** object_pptr);

/*****************************************************************************/
//  Description : tapi query interface
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFITAPI_QueryInterface(ITAPI_T * tapi_ptr, CAF_GUID_T guid, void ** object_pptr );

/*****************************************************************************/
//  Description : tapi add ref
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFITAPI_AddRef(ITAPI_T * tapi_ptr);

/*****************************************************************************/
//  Description : tapi release
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFITAPI_Release(ITAPI_T * tapi_ptr);

/*****************************************************************************/
//  Description : data service built
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_MakeVoiceCall(ITAPI_T * tapi_ptr, const char * pszNumber, void *param_ptr, char *user_name_ptr, char *psw_ptr);

/*****************************************************************************/
//  Description : data service ready?
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_IsDataServiceReady(ITAPI_T * tapi_ptr);

/*****************************************************************************/
//  Description : pdp net connect
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_NetConnect(ITAPI_T * tapi_ptr, uint8 *apn_ptr, char *user_name_ptr, char *psw_ptr);

/*****************************************************************************/
//  Description : pdp net disconnect
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CAFITAPI_NetDisconnect(uint32 app_handler);

/*****************************************************************************/
//  Description : treat the message from pdp
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void CAFITAPI_HandlePsMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : treat the message from l4
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CAFITAPI_HandlePrePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : Item initialize
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_InitRegItems(ITAPI_T * tapi_ptr);

/*****************************************************************************/
//  Description : Item uninitialize
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_UnInitRegItems(ITAPI_T * tapi_ptr);

/*****************************************************************************/
//  Description : transfer tapi pointer to regapp handle
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SetCnfTapiPtr(ITAPI_T * tapi_ptr);

/*****************************************************************************/
//  Description : register the message to handle by start and end message.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_RegisterMsg(ITAPI_T * tapi_ptr, CAF_HANDLE_T dest_handle, uint16 start_msg, uint16 end_msg);

/*****************************************************************************/
//  Description : get the handle by tapi message.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL CAF_HANDLE_T CAFITAPI_GetHandleByMsg(PWND app_ptr, uint16 msg_id);

/*****************************************************************************/
//  Description : Get the SMS max support number and used number in the selected 
//                system and storage
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_GetSmsNum(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 *max_sms_num_ptr, uint16 *used_sms_num_ptr);

/*****************************************************************************/
//  Description : Get the SMS state and the max support number in the 
//                destination storage
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_GetSmsStatus(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 *max_sms_num_ptr, ITAPI_SMS_STATUS_E *status_ptr);

/*****************************************************************************/
//  Description : set storage priority, SIM first or ME first
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_SetStoragePriority(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, BOOLEAN is_sim_first);

/*****************************************************************************/
//  Description : get storage priority, sim first or me first
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_GetStoragePriority(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, BOOLEAN *is_sim_first_ptr);

/*****************************************************************************/
//  Description : Read the the SMS according to the record_id 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_ReadSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 record_id);

/*****************************************************************************/
//  Description : update the SMS status in destination storage, the operation 
//                can delete an SMS or change the state from read to be read.
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_UpdateSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 record_id, ITAPI_SMS_STATUS_E status);

/*****************************************************************************/
//  Description : APP originate an MO short message, send out SMS 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:   
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_SendSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_MO_SMS_T *sms_ptr, ITAPI_SMS_STORAGE_E sms_write, ITAPI_SMS_PATH_E sms_path, BOOLEAN is_more);

/*****************************************************************************/
//  Description : originate an PDU MO short message, send out SMS 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:   
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_SendPduSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_ADDR_T *addr_ptr, ITAPI_L3_MSG_UNIT_T  *tpdu_ptr, BOOLEAN is_more);

/*****************************************************************************/
//  Description : save the MO SMS to the destination storage 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_WriteSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, ITAPI_SMS_STATUS_E status, ITAPI_SMS_MO_SMS_T *sms_ptr);

/*****************************************************************************/
//  Description : Set SC Address in SIM card
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_SetSmsCenterAddr(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_CALLED_NUMBER_T *tp_sc_addr_ptr);

/*****************************************************************************/
//  Description : Read SC Address which stored in SIM card
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_GetSmsCenterAddr(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_CALLED_NUMBER_T *tp_sc_addr_ptr);

/*****************************************************************************/
//  Description : set the default index of parameters which save in SIM
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_GetErrCode(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys);

/*****************************************************************************/
//  Description : judge whether receive sms
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_SetSmsReceiveFunc(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, BOOLEAN  is_receive_sms);

/*****************************************************************************/
//  Description : enable the msg transfer to the post process or not
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_EnableMsgPostWork(ITAPI_T * tapi_ptr, BOOLEAN  need_posttransfer);

/*****************************************************************************/
//  Description : Get PLMN information
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_PHONE_GetPlmnInfo(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_PLMN_INFO_T *tapi_plmn_info_ptr);

/*****************************************************************************/
//  Description : Get RSSI information
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_PHONE_GetRssi(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_PHONE_SCELL_MEAS_T *tapi_cell_info_ptr);

/*****************************************************************************/
//  Description : Get IMEI information
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_PHONE_GetImei(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, uint8 *imei_ptr, uint16 *imei_len_ptr, uint16 imei_max_len);

/*****************************************************************************/
//  Description : Get dual sim status
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SIM_GetDualSimStatus(ITAPI_T * tapi_ptr, BOOLEAN *is_sim1_ok_ptr, BOOLEAN *is_sim2_ok_ptr);

/*****************************************************************************/
//  Description : Get IMSI information
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SIM_GetImsi(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_IMSI_T *imsi_ptr);

/*****************************************************************************/
//  Description : APP task start a MO call.only one call is established at one 
//                time. 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_CALL_StartCall(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_CALL_TYPE_E call_type, ITAPI_CALLED_NUMBER_T *called_num_ptr, ITAPI_CALL_CLIR_E clir_type);

/*****************************************************************************/
//  Description : APP task accept the incoming call
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_CALL_ConnectCall(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, uint8 call_id);

/*****************************************************************************/
//  Description :  APP task disconnect the call indicated by call_id
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_CALL_DisConnectCall(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, uint8 call_id);

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//global variable declare
PUBLIC MMI_APPLICATION_T       g_caf_tapi_app = {CAFITAPI_HandlePrePsMsg, CT_APPLICATION, PNULL};       // g_caf_tapi_app define

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

// functions table


LOCAL const CAF_VTBL_T(ITAPI_T) s_tapi_vtbl =
{
    CAFITAPI_QueryInterface,
    CAFITAPI_AddRef,
    CAFITAPI_Release,
    CAFITAPI_MakeVoiceCall,
    CAFITAPI_IsDataServiceReady,
    CAFITAPI_RegisterMsg,
    CAFITAPI_EnableMsgPostWork,
    CAFITAPI_SMS_GetSmsNum,
    CAFITAPI_SMS_GetSmsStatus,
    CAFITAPI_SMS_SetStoragePriority,
    CAFITAPI_SMS_GetStoragePriority,
    CAFITAPI_SMS_ReadSms,
    CAFITAPI_SMS_UpdateSms,
    CAFITAPI_SMS_SendSms,
    CAFITAPI_SMS_SendPduSms,
    CAFITAPI_SMS_WriteSms,
    CAFITAPI_SMS_SetSmsCenterAddr,
    CAFITAPI_SMS_GetSmsCenterAddr,
    CAFITAPI_SMS_GetErrCode,
    CAFITAPI_SMS_SetSmsReceiveFunc,
    CAFITAPI_PHONE_GetPlmnInfo,
    CAFITAPI_PHONE_GetRssi,
    CAFITAPI_PHONE_GetImei,
    CAFITAPI_SIM_GetDualSimStatus,
    CAFITAPI_SIM_GetImsi,
    CAFITAPI_CALL_StartCall,
    CAFITAPI_CALL_ConnectCall,
    CAFITAPI_CALL_DisConnectCall
};


LOCAL const CAF_STATIC_CLASS_T s_tapi_class_list[] = 
{
    { SPRD_GUID_TAPI,      CAFITAPI_ClassNew,    0}
};

const CAF_STATIC_CLASS_INFO_T g_tapi_static_class_info = 
{ 
    (CAF_STATIC_CLASS_T*)s_tapi_class_list, 
    sizeof(s_tapi_class_list) / sizeof(s_tapi_class_list[0])
};


LOCAL const ITAPI_REG_ITEM_T s_reg_sms_msgs[] =
{
    {PNULL, MSG_SMS_READ_SMS_CNF, 0},
    {PNULL, MSG_SMS_UPDATE_SMS_STATE_CNF, 0},
    {PNULL, MSG_SMS_SEND_SMS_CNF, 0},
    {PNULL, MSG_SMS_WRITE_SMS_CNF, 0},
    {PNULL, MSG_SMS_WRITE_SMS_PARAM_CNF, 0},
    {PNULL, MSG_SMS_SET_SMS_RECEIVE_FUNC_CNF, 0},
    {PNULL, MSG_SMS_MEM_FULL_IND, 0},
    {PNULL, MSG_SMS_STATUS_REPORT_IND, 0},
    {PNULL, MSG_SMS_RECEIVE_IND, 0},
    {PNULL, MSG_SMS_SERVICE_END, 0},
    {PNULL, 0, 0}
};

LOCAL const ITAPI_REG_ITEM_T s_reg_call_msgs[] =
{
    {PNULL, MSG_CALL_START_IND, 0},
    {PNULL, MSG_CALL_SETUP_COMPLETE_IND, 0},
    {PNULL, MSG_CALL_SETUP_COMPLETE_CNF, 0},
    {PNULL, MSG_CALL_DISCONNECTED_IND, 0},
    {PNULL, MSG_CALL_SETUP_IND, 0},
    {PNULL, MSG_CALL_ERR_IND, 0},
    {PNULL, MSG_CALL_SERVICE_END, 0},
    {PNULL, 0, 0}
};

LOCAL const ITAPI_REG_ITEM_T s_reg_phone_msgs[] =
{
    {PNULL, 0, 0}
};

LOCAL const ITAPI_REG_ITEM_T s_reg_ss_msgs[] =
{
    {PNULL, 0, 0}
};

LOCAL const ITAPI_REG_ITEM_T s_reg_sim_msgs[] =
{
    {PNULL, 0, 0}
};

LOCAL const ITAPI_REG_ITEM_T * const s_all_service_tapi_reg_ptrs[] =
{
    s_reg_sms_msgs,
    s_reg_call_msgs,
    s_reg_phone_msgs,
    s_reg_ss_msgs,
    s_reg_sim_msgs
};

LOCAL const uint16 s_all_service_tapi_reg_items[] =
{
    sizeof(s_reg_sms_msgs)/sizeof(s_reg_sms_msgs[0]),
    sizeof(s_reg_call_msgs)/sizeof(s_reg_call_msgs[0]),
    sizeof(s_reg_phone_msgs)/sizeof(s_reg_phone_msgs[0]),
    sizeof(s_reg_ss_msgs)/sizeof(s_reg_ss_msgs[0]),
    sizeof(s_reg_sim_msgs)/sizeof(s_reg_sim_msgs[0])
};

LOCAL ITAPI_STATE_E s_gprs_state;

/*****************************************************************************/
//  Description : tapi class created.
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFITAPI_ClassNew( 
                                      CAF_GUID_T guid, 
                                      void**     object_pptr 
                                      )
{
    CAF_RESULT_E  result  = CAF_RESULT_NOT_SUPPORT;
    CAFTAPI_T     *caftapi_ptr = PNULL;
    
    if( SPRD_GUID_TAPI == guid )
    {
        caftapi_ptr = SCI_ALLOC_APP(sizeof(CAFTAPI_T));
        
        SCI_MEMSET((char *)caftapi_ptr, 0, sizeof(CAFTAPI_T));
        caftapi_ptr->vtbl_ptr = (CAF_VTBL_T(ITAPI_T)*)&s_tapi_vtbl;
        
        caftapi_ptr->guid       = guid;
        caftapi_ptr->ref_num    = 1;
        caftapi_ptr->retrant    = 0;
        caftapi_ptr->netstate   = ITAPI_STATE_INIT;

        s_gprs_state  = ITAPI_STATE_INIT;
        CAFITAPI_SetCnfTapiPtr((ITAPI_T *)caftapi_ptr);
        CAFITAPI_InitRegItems((ITAPI_T *)caftapi_ptr);
        result = CAF_RESULT_SUCCEEDED;
    }
    *object_pptr = caftapi_ptr;
    return result;
}

/*****************************************************************************/
//  Description : tapi query interface
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E CAFITAPI_QueryInterface(ITAPI_T * tapi_ptr, CAF_GUID_T guid, void ** object_pptr )
{
    CAF_RESULT_E result = CAF_RESULT_FAILED;
    CAFTAPI_T *caftapi_ptr = (CAFTAPI_T *)tapi_ptr;
    
    if ( IUNKNOWN_ID == guid ||  caftapi_ptr->guid == guid )
    {
        *object_pptr = tapi_ptr;
        
        result = CAF_RESULT_SUCCEEDED;
    }
    if( result )
    {
        IUNKNOWN_AddRef( *object_pptr );
    }
    return result;
}

/*****************************************************************************/
//  Description : tapi add ref
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFITAPI_AddRef(ITAPI_T * tapi_ptr)
{
    SCI_ASSERT(tapi_ptr);
    return ++(((CAFTAPI_T *)tapi_ptr)->ref_num);
}

/*****************************************************************************/
//  Description : tapi release
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFITAPI_Release(ITAPI_T * tapi_ptr)
{
    CAFTAPI_T * caftapi_ptr = (CAFTAPI_T *)tapi_ptr;

    SCI_ASSERT(tapi_ptr);    
    if ( --caftapi_ptr->ref_num )
    {
        return( caftapi_ptr->ref_num );
    }
    else
    {
        if(caftapi_ptr->retrant)
        {
            #ifndef  WIN32
            /*
            MNGPRS_ResetAndDeactivePdpContextEx(caftapi_ptr->dual_sys);
            */
            CAFITAPI_NetDisconnect(MMI_MODULE_TAPI);
            #endif
        }
        CAFITAPI_SetCnfTapiPtr((ITAPI_T *)PNULL);
        CAFITAPI_UnInitRegItems(tapi_ptr);
        SCI_FREE( caftapi_ptr );
        return 0;
    }
}


/*****************************************************************************/
//  Description : data service built
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
int32 CAFITAPI_MakeVoiceCall(ITAPI_T * tapi_ptr, const char * pszNumber, void *param_ptr, char *user_name_ptr, char *psw_ptr)
{
    CAFTAPI_T     *caftapi_ptr;

    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);

    caftapi_ptr = (CAFTAPI_T *)tapi_ptr;

    CAFITAPI_SetCnfTapiPtr(tapi_ptr);
    /* Only for data call, wait for pdp active */
    if(!strcmp(pszNumber, "*99***1#"))
    {
        if(caftapi_ptr->retrant == 0)
        {
            uint32 i = 0;
#ifndef  WIN32
            /*  Param_ptr is access point */
            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (MMIAPIPHONE_IsSimAvailable(i))
                {
                    caftapi_ptr->dual_sys =(MN_DUAL_SYS_E)i;
                    break;
                }
                else
                {
                    caftapi_ptr->dual_sys = MN_DUAL_SYS_1;
                }
            }            

            /*
            if(ERR_MNGPRS_NO_ERR == MNGPRS_SetAndActivePdpContextEx(caftapi_ptr->dual_sys, (uint8* )param_ptr,POC_E))
            */
            if(ITAPI_SUCCESS == CAFITAPI_NetConnect(tapi_ptr, (uint8* )param_ptr, user_name_ptr, psw_ptr))
            {
                caftapi_ptr->retrant++;
                return ITAPI_SUCCESS;
            }
            else
            {
                return ITAPI_ERROR;
            }
#else
            caftapi_ptr->retrant++;
            return ITAPI_SUCCESS;
#endif
        }
        else
        {
            return ITAPI_EALREADY;
        }
    }
    return ITAPI_ERROR;
}

/*****************************************************************************/
//  Description : data service ready?
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
int32 CAFITAPI_IsDataServiceReady(ITAPI_T * tapi_ptr)
{
    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
#ifdef  WIN32
    return ITAPI_STATE_ACTIVED;
#else
    //SCI_TRACE_LOW:"caftapi.c:CAFITAPI_IsDataServiceReady, s_gprs_state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_634_112_2_17_23_1_6_7,(uint8*)"d", s_gprs_state);
    if( s_gprs_state == ITAPI_STATE_ACTIVED )
    {
        return ITAPI_STATE_ACTIVED;
    }
    else if(s_gprs_state == ITAPI_STATE_NOSIM)
    {
        return ITAPI_STATE_NOSIM;
    }
    else
    {
        return ITAPI_STATE_BUSY;
    }
#endif
}

/*****************************************************************************/
//  Description : pdp net connect
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_NetConnect(ITAPI_T * tapi_ptr, uint8 *apn_ptr, char *user_name_ptr, char *psw_ptr)
{
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    
    app_info.app_handler = MMI_MODULE_TAPI;
    app_info.apn_ptr = (char*)apn_ptr;
    app_info.user_name_ptr = user_name_ptr;
    app_info.psw_ptr = psw_ptr;
    app_info.dual_sys = ((CAFTAPI_T *)tapi_ptr)->dual_sys;
    app_info.priority = 3;
    app_info.handle_msg_callback = CAFITAPI_HandlePsMsg;
    if(!MMIAPIPDP_Active(&app_info))
    {
        return ITAPI_ERROR;
    }
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : pdp net disconnect
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CAFITAPI_NetDisconnect(uint32 app_handler)
{
    return MMIAPIPDP_Deactive(app_handler);
}

/*****************************************************************************/
//  Description : treat the message from pdp
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void CAFITAPI_HandlePsMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    SCI_ASSERT(PNULL != msg_ptr);
    
    //SCI_TRACE_LOW:"[CAFTAPI]:HandleMMSPDPMsg msg_id=%d, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_689_112_2_17_23_1_6_8,(uint8*)"dd",msg_ptr->msg_id, msg_ptr->result);
    switch(msg_ptr->msg_id) 
    {
        case MMIPDP_ACTIVE_CNF:
            if(MN_GPRS_ERR_SUCCESS == msg_ptr->result)
            {
                s_gprs_state = ITAPI_STATE_ACTIVED;
            }
            break;
        case MMIPDP_DEACTIVE_CNF:
        case MMIPDP_DEACTIVE_IND:
            s_gprs_state = ITAPI_STATE_SIMRDY;
            break;
        default:
            break;
    }
}

/*****************************************************************************/
//  Description : transfer tapi pointer to regapp handle
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SetCnfTapiPtr(ITAPI_T * tapi_ptr)
{
    g_caf_tapi_app.adddata_ptr = (ADD_DATA)tapi_ptr;
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : Item initialize
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_InitRegItems(ITAPI_T * tapi_ptr)
{
    uint16     i;
    CAFTAPI_T  *caftapi_ptr = (CAFTAPI_T *)tapi_ptr;
    
    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    
    //SCI_TRACE_LOW:"Reg item service ptrs is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_729_112_2_17_23_1_6_9,(uint8*)"d", sizeof(s_all_service_tapi_reg_ptrs)/sizeof(s_all_service_tapi_reg_ptrs[0]));
    caftapi_ptr->tapi_reg_items_pptr = (ITAPI_REG_ITEM_T **)SCI_ALLOC_APP((sizeof(s_all_service_tapi_reg_ptrs)/sizeof(s_all_service_tapi_reg_ptrs[0])) * sizeof(ITAPI_REG_ITEM_T *));
    
    for(i = 0; i < sizeof(s_all_service_tapi_reg_ptrs)/sizeof(s_all_service_tapi_reg_ptrs[0]); i++)
    {
        //SCI_TRACE_LOW:"Every service msg nums is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_734_112_2_17_23_1_6_10,(uint8*)"d", s_all_service_tapi_reg_items[i]);
        caftapi_ptr->tapi_reg_items_pptr[i] = (ITAPI_REG_ITEM_T *)SCI_ALLOC_APP(s_all_service_tapi_reg_items[i] * sizeof(ITAPI_REG_ITEM_T));
        memcpy(caftapi_ptr->tapi_reg_items_pptr[i], s_all_service_tapi_reg_ptrs[i], s_all_service_tapi_reg_items[i] * sizeof(ITAPI_REG_ITEM_T));
    }
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : Item uninitialize
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_UnInitRegItems(ITAPI_T * tapi_ptr)
{
    uint16     i;
    CAFTAPI_T  *caftapi_ptr = (CAFTAPI_T *)tapi_ptr;
    
    SCI_ASSERT(tapi_ptr);
    
    //SCI_TRACE_LOW:"Reg item service ptrs is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_753_112_2_17_23_1_6_11,(uint8*)"d", sizeof(s_all_service_tapi_reg_ptrs)/sizeof(s_all_service_tapi_reg_ptrs[0]));
    
    for(i = 0; i < sizeof(s_all_service_tapi_reg_ptrs)/sizeof(s_all_service_tapi_reg_ptrs[0]); i++)
    {
        //SCI_TRACE_LOW:"Every service msg nums is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_757_112_2_17_23_1_6_12,(uint8*)"d", s_all_service_tapi_reg_items[i]);
        SCI_FREE(caftapi_ptr->tapi_reg_items_pptr[i]);
        caftapi_ptr->tapi_reg_items_pptr[i] = PNULL;
    }
    
    SCI_FREE(caftapi_ptr->tapi_reg_items_pptr);
    caftapi_ptr->tapi_reg_items_pptr = PNULL;
    
    return ITAPI_SUCCESS;
}


/*****************************************************************************/
//  Description : get the item structure pointer by start and end message.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL ITAPI_REG_ITEM_T *CAFITAPI_GetServiceItemPtrs(ITAPI_T * tapi_ptr, uint16 start_msg, uint16 end_msg)
{
    ITAPI_REG_ITEM_T    *tapi_reg_items_ptr = PNULL;
    CAFTAPI_T *caftapi_ptr = (CAFTAPI_T *)tapi_ptr;

    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    SCI_ASSERT(start_msg <= end_msg);

    if((start_msg >= MSG_SMS_SERVICE_START) && (end_msg <= MSG_SMS_SERVICE_END))
    {
        tapi_reg_items_ptr = caftapi_ptr->tapi_reg_items_pptr[0];
    }
    else if((start_msg >= MSG_CALL_SERVICE_START) && (end_msg <= MSG_CALL_SERVICE_END))
    {
        tapi_reg_items_ptr = caftapi_ptr->tapi_reg_items_pptr[1];
    }
    else if((start_msg >= MSG_PHONE_SERVICE_START) && (end_msg <= MSG_PHONE_SERVICE_END))
    {
        tapi_reg_items_ptr = caftapi_ptr->tapi_reg_items_pptr[2];
    }
    else if((start_msg >= MSG_SS_SERVICE_START) && (end_msg <= MSG_SS_SERVICE_END))
    {
        tapi_reg_items_ptr = caftapi_ptr->tapi_reg_items_pptr[3];
    }
    else if((start_msg >= MSG_SIM_SERVICE_START) && (end_msg <= MSG_SIM_SERVICE_END))
    {
        tapi_reg_items_ptr = caftapi_ptr->tapi_reg_items_pptr[4];
    }
    else
    {
        /*
        SCI_ASSERT(tapi_reg_items_ptr);
        */
        return PNULL;
    }
    
    return tapi_reg_items_ptr;
}

/*****************************************************************************/
//  Description : register the message to handle by start and end message.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_RegisterMsg(ITAPI_T * tapi_ptr, CAF_HANDLE_T dest_handle, uint16 start_msg, uint16 end_msg)
{
    uint16              i;
    ITAPI_REG_ITEM_T    *tapi_reg_items_ptr;
    
    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);

    tapi_reg_items_ptr = CAFITAPI_GetServiceItemPtrs(tapi_ptr, start_msg, end_msg);
    if(tapi_reg_items_ptr)
    {
        i = (uint16)(start_msg & 0x00ff);
        end_msg &= (uint16)0x00ff;
        for(; i <= end_msg; i++)
        {
            if(tapi_reg_items_ptr[i].ps_msg)
            {
                tapi_reg_items_ptr[i].dest_handle = dest_handle;
            }
        }
        
        if((i == end_msg + 1) && !tapi_reg_items_ptr[end_msg].dest_handle)
        {
            return ITAPI_ERROR;
        }
        
        return ITAPI_SUCCESS;
    }
    return ITAPI_ERROR;
}

/*****************************************************************************/
//  Description : get the handle by tapi message.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL CAF_HANDLE_T CAFITAPI_GetHandleByMsg(PWND app_ptr, uint16 msg_id)
{
    uint16              i;
    ITAPI_REG_ITEM_T    *tapi_reg_items_ptr = PNULL;
    ITAPI_T             *tapi_ptr = PNULL;

    tapi_ptr = (ITAPI_T *)(((MMI_APPLICATION_T *)app_ptr)->adddata_ptr);
    if(tapi_ptr)
    {
        tapi_reg_items_ptr = CAFITAPI_GetServiceItemPtrs(tapi_ptr, msg_id, msg_id);
        if(tapi_reg_items_ptr)
        {
            i = (uint16)(msg_id & 0xff);
            return tapi_reg_items_ptr[i].dest_handle;
        }
    }
    return PNULL;
}

/*****************************************************************************/
//  Description : get the tapi pointer by reg app pwnd.
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL CAFTAPI_T *CAFITAPI_GetTAPIptr(PWND app_ptr)
{
    CAFTAPI_T     *caftapi_ptr = PNULL;
    caftapi_ptr = (CAFTAPI_T *)(((MMI_APPLICATION_T *)app_ptr)->adddata_ptr);
    return caftapi_ptr;
}

/*****************************************************************************/
//  Description : the following api convert the CAF enum or structure to MN layer.
//                then convert the MN layer enum or structure to CAF layer
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E ConvertCAFToMN_DUAL_E(ITAPI_DUAL_E dual_sys)
{
    MN_DUAL_SYS_E dual_sys_mn = MN_DUAL_SYS_1;

    if( dual_sys <= ITAPI_DUAL_MAX )
    {
        dual_sys_mn = MN_DUAL_SYS_1 + dual_sys;
    }
    else
    {
        //SCI_TRACE_LOW:"ConvertCAFToMN_DUAL_E,ERROR:caf_dual_sys[%d],mn_dual_sys[%d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_897_112_2_17_23_1_6_13,(uint8*)"dd",dual_sys,dual_sys_mn);

        SCI_ASSERT(FALSE); // assert!!!
    }

    return dual_sys_mn;
}

LOCAL ITAPI_DUAL_E ConvertMNToCAF_DUAL_E(MN_DUAL_SYS_E dual_sys)
{
    ITAPI_DUAL_E tapi_dual_sys = ITAPI_DUAL1;
    
    if(dual_sys <= MMI_DUAL_SYS_MAX)
    {
        tapi_dual_sys = ITAPI_DUAL1 + dual_sys;
    }
    else
    {
        //SCI_TRACE_LOW:"ConvertMNToCAF_DUAL_E,ERROR:caf_dual_sys_mn[%d],tapi_dual_sys[%d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_915_112_2_17_23_1_6_14,(uint8*)"dd",dual_sys,tapi_dual_sys);

        SCI_ASSERT(FALSE); // assert!!!
    }
    
    // deleted for multisim compatible api ...
    #if 0
    switch(dual_sys)
    {
        case MN_DUAL_SYS_1:
            tapi_dual_sys = ITAPI_DUAL1;
            break;
        case MN_DUAL_SYS_2:
            tapi_dual_sys = ITAPI_DUAL2;
            break;
        case MN_DUAL_SYS_MAX:
            tapi_dual_sys = ITAPI_DUAL_MAX;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }
    #endif
    
    return tapi_dual_sys;
}

LOCAL MN_SMS_STORAGE_E ConvertCAFToMN_STORAGE_E(ITAPI_SMS_STORAGE_E storage)
{
    MN_SMS_STORAGE_E storage_mn = MN_SMS_NO_STORED;
    
    switch(storage)
    {
        case ITAPI_SMS_NO_STORED:
            storage_mn = MN_SMS_NO_STORED;
            break;
        case ITAPI_SMS_STORAGE_ME:
            storage_mn = MN_SMS_STORAGE_ME;
            break;
        case ITAPI_SMS_STORAGE_SIM:
            storage_mn = MN_SMS_STORAGE_SIM;
            break;
        case ITAPI_SMS_STORAGE_ALL:
            storage_mn = MN_SMS_STORAGE_ALL;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }
    
    return storage_mn;
}

LOCAL ITAPI_SMS_STORAGE_E ConvertMNToCAF_STORAGE_E(MN_SMS_STORAGE_E storage)
{
    ITAPI_SMS_STORAGE_E tapi_storage = ITAPI_SMS_NO_STORED;
    
    switch(storage)
    {
        case MN_SMS_NO_STORED:
            tapi_storage = ITAPI_SMS_NO_STORED;
            break;
        case MN_SMS_STORAGE_ME:
            tapi_storage = ITAPI_SMS_STORAGE_ME;
            break;
        case MN_SMS_STORAGE_SIM:
            tapi_storage = ITAPI_SMS_STORAGE_SIM;
            break;
        case MN_SMS_STORAGE_ALL:
            tapi_storage = ITAPI_SMS_STORAGE_ALL;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }
    
    return tapi_storage;
}


LOCAL ITAPI_SMS_STATUS_E ConvertMNToCAF_SMS_STATUS_E(MN_SMS_STATUS_E status)
{
    ITAPI_SMS_STATUS_E itapi_status = ITAPI_SMS_FREE_SPACE;
    
    switch(status)
    {
        case MN_SMS_FREE_SPACE:
            itapi_status = ITAPI_SMS_FREE_SPACE;
            break;
        case MN_SMS_MT_READED:
            itapi_status = ITAPI_SMS_MT_READED;
            break;
        case MN_SMS_MT_TO_BE_READ:
            itapi_status = ITAPI_SMS_MT_TO_BE_READ;
            break;
        case MN_SMS_MO_TO_BE_SEND:
            itapi_status = ITAPI_SMS_MO_TO_BE_SEND;
            break;
        case MN_SMS_MO_SR_NOT_REQUEST:
            itapi_status = ITAPI_SMS_MO_SR_NOT_REQUEST;
            break;
        case MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED:
            itapi_status = ITAPI_SMS_MO_SR_REQUESTED_NOT_RECEIVED;
            break;
        case MN_SMS_MO_SR_RECEIVED_NOT_STORE:
            itapi_status = ITAPI_SMS_MO_SR_RECEIVED_NOT_STORE;
            break;
        case MN_SMS_MO_SR_RECEIVED_AND_STORED:
            itapi_status = ITAPI_SMS_MO_SR_RECEIVED_AND_STORED;
            break;
        case MN_SMS_MT_SR_READED:
            itapi_status = ITAPI_SMS_MT_SR_READED;
            break;
        case MN_SMS_MT_SR_TO_BE_READ:
            itapi_status = ITAPI_SMS_MT_SR_TO_BE_READ;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }
    
    return itapi_status;
}

LOCAL MN_SMS_STATUS_E ConvertCAFToMN_SMS_STATUS_E(ITAPI_SMS_STATUS_E status)
{
    MN_SMS_STATUS_E status_mn = MN_SMS_FREE_SPACE;
    
    switch(status)
    {
        case ITAPI_SMS_FREE_SPACE:
            status_mn = MN_SMS_FREE_SPACE;
            break;
        case ITAPI_SMS_MT_READED:
            status_mn = MN_SMS_MT_READED;
            break;
        case ITAPI_SMS_MT_TO_BE_READ:
            status_mn = MN_SMS_MT_TO_BE_READ;
            break;
        case ITAPI_SMS_MO_TO_BE_SEND:
            status_mn = MN_SMS_MO_TO_BE_SEND;
            break;
        case ITAPI_SMS_MO_SR_NOT_REQUEST:
            status_mn = MN_SMS_MO_SR_NOT_REQUEST;
            break;
        case ITAPI_SMS_MO_SR_REQUESTED_NOT_RECEIVED:
            status_mn = MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED;
            break;
        case ITAPI_SMS_MO_SR_RECEIVED_NOT_STORE:
            status_mn = MN_SMS_MO_SR_RECEIVED_NOT_STORE;
            break;
        case ITAPI_SMS_MO_SR_RECEIVED_AND_STORED:
            status_mn = MN_SMS_MO_SR_RECEIVED_AND_STORED;
            break;
        case ITAPI_SMS_MT_SR_READED:
            status_mn = MN_SMS_MT_SR_READED;
            break;
        case ITAPI_SMS_MT_SR_TO_BE_READ:
            status_mn = MN_SMS_MT_SR_TO_BE_READ;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }
    
    return status_mn;
}

LOCAL MN_SMS_PATH_E ConvertCAFToMN_SMS_PATH_E(ITAPI_SMS_PATH_E sms_path)
{
    MN_SMS_PATH_E sms_path_mn = MN_SMS_GSM_PATH;
    
    switch(sms_path)
    {
        case ITAPI_SMS_GSM_PATH:
            sms_path_mn = MN_SMS_GSM_PATH;
            break;
        case ITAPI_SMS_GPRS_PATH:
            sms_path_mn = MN_SMS_GPRS_PATH;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }
    
    return sms_path_mn;
}

LOCAL MN_NUMBER_TYPE_E ConvertCAFToMN_NUMBER_TYPE_E(ITAPI_NUMBER_TYPE_E number_type)
{
    MN_NUMBER_TYPE_E number_type_mn = MN_NUM_TYPE_UNKNOW;
    
    switch(number_type)
    {
        case ITAPI_NUM_TYPE_UNKNOW:
            number_type_mn = MN_NUM_TYPE_UNKNOW;
            break;
        case ITAPI_NUM_TYPE_INTERNATIONAL:
            number_type_mn = MN_NUM_TYPE_INTERNATIONAL;
            break;
        case ITAPI_NUM_TYPE_NATIONAL:
            number_type_mn = MN_NUM_TYPE_NATIONAL;
            break;
        case ITAPI_NUM_TYPE_NETWORK:
            number_type_mn = MN_NUM_TYPE_NETWORK;
            break;
        case ITAPI_NUM_TYPE_DEDICATECODE_SHORTCODE:
            number_type_mn = MN_NUM_TYPE_DEDICATECODE_SHORTCODE;
            break;
        case ITAPI_NUM_TYPE_ALPHANUMERIC:
            number_type_mn = MN_NUM_TYPE_ALPHANUMERIC;
            break;
        case ITAPI_NUM_TYPE_ABBREVIATE:
            number_type_mn = MN_NUM_TYPE_ABBREVIATE;
            break;
        case ITAPI_NUM_TYPE_RESERVED:
            number_type_mn = MN_NUM_TYPE_RESERVED;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }
    
    return number_type_mn;
}

LOCAL ITAPI_NUMBER_TYPE_E ConvertMNToCAF_NUMBER_TYPE_E(MN_NUMBER_TYPE_E number_type)
{
    ITAPI_NUMBER_TYPE_E tapi_number_type = ITAPI_NUM_TYPE_UNKNOW;
    
    switch(number_type)
    {
        case MN_NUM_TYPE_UNKNOW:
            tapi_number_type = ITAPI_NUM_TYPE_UNKNOW;
            break;
        case MN_NUM_TYPE_INTERNATIONAL:
            tapi_number_type = ITAPI_NUM_TYPE_INTERNATIONAL;
            break;
        case MN_NUM_TYPE_NATIONAL:
            tapi_number_type = ITAPI_NUM_TYPE_NATIONAL;
            break;
        case MN_NUM_TYPE_NETWORK:
            tapi_number_type = ITAPI_NUM_TYPE_NETWORK;
            break;
        case MN_NUM_TYPE_DEDICATECODE_SHORTCODE:
            tapi_number_type = ITAPI_NUM_TYPE_DEDICATECODE_SHORTCODE;
            break;
        case MN_NUM_TYPE_ALPHANUMERIC:
            tapi_number_type = ITAPI_NUM_TYPE_ALPHANUMERIC;
            break;
        case MN_NUM_TYPE_ABBREVIATE:
            tapi_number_type = ITAPI_NUM_TYPE_ABBREVIATE;
            break;
        case MN_NUM_TYPE_RESERVED:
            tapi_number_type = ITAPI_NUM_TYPE_RESERVED;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }
    
    return tapi_number_type;
}

LOCAL ITAPI_SMS_ALPHABET_TYPE_E ConvertMNToCAF_SMS_ALPHABET_TYPE_E(MN_SMS_ALPHABET_TYPE_E alphabet_type)
{
    ITAPI_SMS_ALPHABET_TYPE_E tapi_alphabet_type = ITAPI_SMS_DEFAULT_ALPHABET;
    
    switch(alphabet_type)
    {
        case MN_SMS_DEFAULT_ALPHABET:
            tapi_alphabet_type = ITAPI_SMS_DEFAULT_ALPHABET;
            break;
        case MN_SMS_8_BIT_ALPHBET:
            tapi_alphabet_type = ITAPI_SMS_8_BIT_ALPHBET;
            break;
        case MN_SMS_UCS2_ALPHABET:
            tapi_alphabet_type = ITAPI_SMS_UCS2_ALPHABET;
            break;
        case MN_SMS_RESERVED_ALPHABET:
            tapi_alphabet_type = ITAPI_SMS_RESERVED_ALPHABET;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }
    
    return tapi_alphabet_type;
}

LOCAL ITAPI_SMS_CAUSE_E ConvertMNToCAF_SMS_CAUSE_E(MN_SMS_CAUSE_E cause)
{
    ITAPI_SMS_CAUSE_E tapi_cause = ITAPI_SMS_OPERATE_SUCCESS;
    
    switch(cause)
    {
        case MN_SMS_OPERATE_SUCCESS:
            tapi_cause = ITAPI_SMS_OPERATE_SUCCESS;
            break;
        case MN_SMS_UNSPEC_ERROR:
            tapi_cause = ITAPI_SMS_UNSPEC_ERROR;
            break;
        case MN_SMS_SEND_FAILURE:
            tapi_cause = ITAPI_SMS_SEND_FAILURE;
            break;
        case MN_SMS_DEFAULT_PARAM_ERROR:
            tapi_cause = ITAPI_SMS_DEFAULT_PARAM_ERROR;
            break;
        case MN_SMS_ME_MEM_FULL:
            tapi_cause = ITAPI_SMS_ME_MEM_FULL;
            break;
        case MN_SMS_ME_NOT_SUPPORT:
            tapi_cause = ITAPI_SMS_ME_NOT_SUPPORT;
            break;
        case MN_SMS_ME_DELETE_FAILURE:
            tapi_cause = ITAPI_SMS_ME_DELETE_FAILURE;
            break;
        case MN_SMS_SIM_NOT_READY:
            tapi_cause = ITAPI_SMS_SIM_NOT_READY;
            break;
        case MN_SMS_SIM_MEM_FULL:
            tapi_cause = ITAPI_SMS_SIM_MEM_FULL;
            break;
        case MN_SMS_SIM_ACCESS_FAILED:
            tapi_cause = ITAPI_SMS_SIM_ACCESS_FAILED;
            break;
        case MN_SMS_SIM_READ_FAILED:
            tapi_cause = ITAPI_SMS_SIM_READ_FAILED;
            break;
        case MN_SMS_SIM_UPDATE_FAILED:
            tapi_cause = ITAPI_SMS_SIM_UPDATE_FAILED;
            break;
        case MN_SMS_SIM_DELETE_FAILURE:
            tapi_cause = ITAPI_SMS_SIM_DELETE_FAILURE;
            break;
        case MN_SMS_RECORD_OUT_OF_RANGE:
            tapi_cause = ITAPI_SMS_RECORD_OUT_OF_RANGE;
            break;
        case MN_SMS_NOT_RIGHT_STORAGE:
            tapi_cause = ITAPI_SMS_NOT_RIGHT_STORAGE;
            break;
        case MN_SMS_SIM_SAVE_STATUS_FREE:
            tapi_cause = ITAPI_SMS_SIM_SAVE_STATUS_FREE;
            break;
        case MN_SMS_MN_BUSY:
        case MN_SMS_WRITE_NV_FAILED:
        case MN_SMS_SIM_PATTERN_NOT_FIND:
        case MN_SMS_SIM_SEEK_FAILED:
        case MN_SMS_SIM_SELECT_FAILED:
        case MN_SMS_SIM_PURGE_FAILED:
        case MN_SMS_SIM_SERVICE_DISABLED:
        case MN_SMS_SIM_STK_NOT_ALLOWED:
        case MN_SMS_SIM_UNKNOW_FAILED:
        case MN_SMS_SIM_FDN_FAILED:
            tapi_cause = ITAPI_SMS_UNKNOW_FAILED;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }
    
    return tapi_cause;
}

LOCAL ITAPI_SMS_MEM_FULL_STATUS_E ConvertMNToCAF_SMS_MEM_FULL_STATUS_E(MN_SMS_MEM_FULL_STATUS_E mem_status)
{
    ITAPI_SMS_MEM_FULL_STATUS_E tapi_mem_status = ITAPI_SMS_ME_FULL;
    
    switch(mem_status)
    {
        case MN_SMS_ME_FULL:
            tapi_mem_status = ITAPI_SMS_ME_FULL;
            break;
        case MN_SMS_SIM_FULL:
            tapi_mem_status = ITAPI_SMS_SIM_FULL;
            break;
        case MN_SMS_ALL_FULL:
            tapi_mem_status = ITAPI_SMS_ALL_FULL;
            break;
        case MN_SMS_MEM_AVAIL:
            tapi_mem_status = ITAPI_SMS_MEM_AVAIL;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }

    return tapi_mem_status;
}

LOCAL ITAPI_SMS_TP_STATUS_E ConvertMNToCAF_SMS_TP_STATUS_E(MN_SMS_TP_STATUS_E tp_status)
{
    ITAPI_SMS_TP_STATUS_E tapi_tp_status = ITAPI_SMS_RECEIVED_BY_SME;
    
    switch(tp_status)
    {
        case MN_SMS_RECEIVED_BY_SME:
            tapi_tp_status = ITAPI_SMS_RECEIVED_BY_SME;
            break;
        case MN_SMS_NOT_CONFIRM_RECEIVED:
            tapi_tp_status = ITAPI_SMS_NOT_CONFIRM_RECEIVED;
            break;
        case MN_SMS_SM_REPLACED_BY_SC:
            tapi_tp_status = ITAPI_SMS_SM_REPLACED_BY_SC;
            break;
        case MN_SMS_SEND_LATER:
            tapi_tp_status = ITAPI_SMS_SEND_LATER;
            break;
        case MN_SMS_NOT_SEND_AGAIN:
            tapi_tp_status = ITAPI_SMS_NOT_SEND_AGAIN;
            break;
        case MN_SMS_INVALID_STATUS_REPORT:
            tapi_tp_status = ITAPI_SMS_INVALID_STATUS_REPORT;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }
    return tapi_tp_status;
}

LOCAL ITAPI_PHONE_PLMN_STATUS_E ConvertMNToCAF_PHONE_PLMN_STATUS_E(MN_PHONE_PLMN_STATUS_E plmn_status)
{
    ITAPI_PHONE_PLMN_STATUS_E tapi_plmn_status = ITAPI_PLMN_NO_SERVICE;
    
    switch(plmn_status)
    {
        case PLMN_NO_SERVICE:
            tapi_plmn_status = ITAPI_PLMN_NO_SERVICE;
            break;
        case PLMN_NORMAL_GSM_ONLY:
            tapi_plmn_status = ITAPI_PLMN_NORMAL_GSM_ONLY;
            break;
        case PLMN_NORMAL_GPRS_ONLY:
            tapi_plmn_status = ITAPI_PLMN_NORMAL_GPRS_ONLY;
            break;
        case PLMN_NORMAL_GSM_GPRS_BOTH:
            tapi_plmn_status = ITAPI_PLMN_NORMAL_GSM_GPRS_BOTH;
            break;
        case PLMN_EMERGENCY_ONLY:
            tapi_plmn_status = ITAPI_PLMN_EMERGENCY_ONLY;
            break;
        case PLMN_EMERGENCY_SIM_INVALID:
            tapi_plmn_status = ITAPI_PLMN_EMERGENCY_SIM_INVALID;
            break;
        case PLMN_EMERGENCY_GPRS_ONLY:
            tapi_plmn_status = ITAPI_PLMN_EMERGENCY_GPRS_ONLY;
            break;
        case PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY:
            tapi_plmn_status = ITAPI_PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY;
            break;
        case PLMN_REGISTER_SERVICE:
            tapi_plmn_status = ITAPI_PLMN_REGISTER_SERVICE;
            break;
        case PLMN_REGISTER_GPRS_ONLY:
            tapi_plmn_status = ITAPI_PLMN_REGISTER_GPRS_ONLY;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }

    return tapi_plmn_status;
}

LOCAL MN_CALL_TYPE_E ConvertCAFToMN_CALL_TYPE_E(ITAPI_CALL_TYPE_E call_type)
{
    MN_CALL_TYPE_E mn_call_type = MN_CALL_TYPE_NONE;
    
    switch(call_type)
    {
        case ITAPI_CALL_TYPE_NORMAL:
            mn_call_type = MN_CALL_TYPE_NORMAL;
            break;
        case ITAPI_CALL_TYPE_EMERGENCY:
            mn_call_type = MN_CALL_TYPE_EMERGENCY;
            break;
        case ITAPI_CALL_TYPE_NONE:
            mn_call_type = MN_CALL_TYPE_NONE;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }

    return mn_call_type;
}

LOCAL MN_CALL_CLIR_E ConvertCAFToMN_CALL_CLIR_E(ITAPI_CALL_CLIR_E clir_type)
{
    MN_CALL_CLIR_E mn_clir_type = MN_CALL_CLIR_NETWORK_DECIDE;
    
    switch(clir_type)
    {
        case ITAPI_CALL_CLIR_SUPPRESSION:
            mn_clir_type = MN_CALL_CLIR_SUPPRESSION;
            break;
        case ITAPI_CALL_CLIR_INVOCATION:
            mn_clir_type = MN_CALL_CLIR_INVOCATION;
            break;
        case ITAPI_CALL_CLIR_NETWORK_DECIDE:
            mn_clir_type = MN_CALL_CLIR_NETWORK_DECIDE;
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }
    
    return mn_clir_type;
}

LOCAL ITAPI_CALL_DISCONNECT_CAUSE_E ConvertMNToCAF_CALL_DISCONNECT_CAUSE_E(MN_CALL_DISCONNECT_CALL_CAUSE_E disconnected_cause)
{
    ITAPI_CALL_DISCONNECT_CAUSE_E tapi_disconnected_cause = ITAPI_CAUSE_UNASSIGNED_NO;
    
    switch(disconnected_cause)
    {
        case MN_CAUSE_UNASSIGNED_NO:
            tapi_disconnected_cause = ITAPI_CAUSE_UNASSIGNED_NO;
            break;
        case MN_CAUSE_NORMAL_CLEARING:
            tapi_disconnected_cause = ITAPI_CAUSE_NORMAL_CLEARING;
            break;
        case MN_CAUSE_USER_BUSY:
            tapi_disconnected_cause = ITAPI_CAUSE_USER_BUSY;
            break;
        case MN_CAUSE_NO_USER_RESPONDING:
            tapi_disconnected_cause = ITAPI_CAUSE_NO_USER_RESPONDING;
            break;
        case MN_CAUSE_ALERTING_NO_ANSWER:
            tapi_disconnected_cause = ITAPI_CAUSE_ALERTING_NO_ANSWER;
            break;
        case MN_CAUSE_CALL_REJECTED:
            tapi_disconnected_cause = ITAPI_CAUSE_CALL_REJECTED;
            break;
        case MN_CAUSE_NUMBER_CHANGED:
            tapi_disconnected_cause = ITAPI_CAUSE_NUMBER_CHANGED;
            break;
        case MN_CAUSE_EMERGENCY_CALL_ONLY:
            tapi_disconnected_cause = ITAPI_CAUSE_EMERGENCY_CALL_ONLY;
            break;
        case MN_CAUSE_CALL_HAS_DISCONNECTED:
            tapi_disconnected_cause = ITAPI_CAUSE_CALL_HAS_DISCONNECTED;
            break;
        case MN_CAUSE_REMOTE_USER_DISCONNECT:
            tapi_disconnected_cause = ITAPI_CAUSE_REMOTE_USER_DISCONNECT;
            break;
        case MN_CAUSE_NETWORK_REJECT:
            tapi_disconnected_cause = ITAPI_CAUSE_NETWORK_REJECT;
            break;
        case MN_CAUSE_NO_CELL:
            tapi_disconnected_cause = ITAPI_CAUSE_NO_CELL;
            break;
        case MN_CAUSE_SUPPLEMENT_NOT_PROVIDE:
            tapi_disconnected_cause = ITAPI_CAUSE_SUPPLEMENT_NOT_PROVIDE;
            break;
        case MN_CAUSE_RESOURCES_UNAV:
            tapi_disconnected_cause = ITAPI_CAUSE_RESOURCES_UNAV;
            break;
        case MN_CAUSE_NO_ROUTE_TO_DEST:
        case MN_CAUSE_CHAN_UNACCEPTABLE:
        case MN_CAUSE_OPER_DETERM_BARRING:
        case MN_CAUSE_NONSEL_USER_CLRING:
        case MN_CAUSE_DEST_OUT_OF_ORDER:
        case MN_CAUSE_INVALID_NO_FORMAT:
        case MN_CAUSE_FACILITY_REJECTED:
        case MN_CAUSE_RSP_TO_STATUS_ENQ:
        case MN_CAUSE_NORMAL_UNSPECIFIED:
        case MN_CAUSE_NO_CIRC_CHAN_AV:
        case MN_CAUSE_NET_OUT_OF_ORDER:
        case MN_CAUSE_TEMP_FAILURE:
        case MN_CAUSE_SWITCH_CONGESTION:
        case MN_CAUSE_ACC_INFO_DISCARDED:
        case MN_CAUSE_REQ_CIRC_CHAN_UNAV:
        case MN_CAUSE_QOS_UNAV:
        case MN_CAUSE_REQ_FAC_NOT_SUBSCR:
        case MN_CAUSE_CUG_INCOMING_BARRED:
        case MN_CAUSE_BEAR_CAP_NOT_AUTH:
        case MN_CAUSE_BEAR_CAP_UNAV:
        case MN_CAUSE_SERV_OPT_UNAV:
        case MN_CAUSE_BEAR_SVC_NOT_IMPL:
        case MN_CAUSE_ACM_EQ_OR_GT_ACMMAX:
        case MN_CAUSE_REQ_FACIL_NOT_IMPL:
        case MN_CAUSE_ONLY_RESTRIC_DIG_AV:
        case MN_CAUSE_SVC_OPT_NOT_IMPL:
        case MN_CAUSE_INVALID_TI:
        case MN_CAUSE_USER_NOT_IN_CUG:
        case MN_CAUSE_INCOMPAT_DEST:
        case MN_CAUSE_INVALID_TRANSIT_NET:
        case MN_CAUSE_INVALID_MSG_SEMANTIC:
        case MN_CAUSE_MAND_IE_ERROR:
        case MN_CAUSE_MSG_NONEXISTENT:
        case MN_CAUSE_MSG_GEN_ERROR:
        case MN_CAUSE_IE_NONEXISTENT:
        case MN_CAUSE_INVALID_CONDITION_IE:
        case MN_CAUSE_MSG_INCOMPAT_STATE:
        case MN_CAUSE_RECOV_ON_TIMER_EXP:
        case MN_CAUSE_PROTOCOL_ERROR:
        case MN_CAUSE_INTERWORKING:
        case MN_CAUSE_AUTHENTICATION_REJ:
        case MN_CAUSE_IMSI_DETACH:
        case MN_CAUSE_T3230_EXPIRY:
        case MN_CAUSE_RR_CONNECTIN_ERROR:
        case MN_CAUSE_LOW_FAILURE:
        case MN_CAUSE_SIM_STK_NOT_ALLOWED:
            tapi_disconnected_cause = ITAPI_CAUSE_UNKNOW;
            break;
        default:
            /* for pc lint */
            if(disconnected_cause == 13)
            {
                tapi_disconnected_cause = ITAPI_CAUSE_NORMAL_CLEARING;
            }
            else
            {
                SCI_ASSERT(FALSE);
            }
            break;
    }
    
    return tapi_disconnected_cause;
}

LOCAL int32 ConvertCAFToMN_NUMBER_T(ITAPI_CALLED_NUMBER_T *tapi_addr_ptr, MN_CALLED_NUMBER_T *mn_addr_ptr)
{
    mn_addr_ptr->number_type = ConvertCAFToMN_NUMBER_TYPE_E(tapi_addr_ptr->number_type);
    mn_addr_ptr->number_plan = MN_NUM_PLAN_ISDN_TELE;
    mn_addr_ptr->num_len = tapi_addr_ptr->num_len;
    SCI_MEMCPY(mn_addr_ptr->party_num, tapi_addr_ptr->party_num, ITAPI_MAX_ADDR_BCD_LEN);

    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_NUMBER_T(MN_CALLED_NUMBER_T *mn_addr_ptr, ITAPI_CALLED_NUMBER_T *tapi_addr_ptr)
{
    tapi_addr_ptr->number_type = ConvertMNToCAF_NUMBER_TYPE_E(mn_addr_ptr->number_type);
    tapi_addr_ptr->num_len = MMIAPICOM_GenDispNumber(mn_addr_ptr->number_type, (uint8)mn_addr_ptr->num_len, mn_addr_ptr->party_num, tapi_addr_ptr->party_num, ITAPI_MAX_ADDR_BCD_LEN);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_SMS_USER_VALID_DATA_T(MN_SMS_USER_VALID_DATA_T *mn_user_data_ptr, ITAPI_SMS_USER_DATA_T *tapi_user_data_ptr, MN_SMS_ALPHABET_TYPE_E alphabet_type, MN_SMS_STATUS_E status)
{
    uint8 i;

    if(alphabet_type == MN_SMS_DEFAULT_ALPHABET)
    {
        tapi_user_data_ptr->length = (uint16)mn_user_data_ptr->length;
        MMIAPICOM_StrToWstr((const uint8*)mn_user_data_ptr->user_valid_data_arr, tapi_user_data_ptr->user_data_arr);
    }
    else if(alphabet_type == MN_SMS_UCS2_ALPHABET)
    {
        tapi_user_data_ptr->length = (uint16)(mn_user_data_ptr->length / 2);
#ifdef WIN32
        if((status == MN_SMS_MT_READED) || (status == MN_SMS_MT_TO_BE_READ))
        {
            for(i = 0; i < tapi_user_data_ptr->length; i++)
            {
                tapi_user_data_ptr->user_data_arr[i] = (wchar_t)(mn_user_data_ptr->user_valid_data_arr[2 * i + 1] + (uint16)(mn_user_data_ptr->user_valid_data_arr[2 * i] << 8));
            }
        }
        else if(status == MN_SMS_MO_TO_BE_SEND)
        {
            for(i = 0; i < tapi_user_data_ptr->length; i++)
            {
                tapi_user_data_ptr->user_data_arr[i] = (wchar_t)(mn_user_data_ptr->user_valid_data_arr[2 * i] + (uint16)(mn_user_data_ptr->user_valid_data_arr[2 * i + 1] << 8));
            }
        }
        else
        {
            //SCI_TRACE_LOW:"status is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_1585_112_2_17_23_1_8_15,(uint8*)"d", status);
            SCI_ASSERT(FALSE);
        }
#else
        for(i = 0; i < tapi_user_data_ptr->length; i++)
        {
            tapi_user_data_ptr->user_data_arr[i] = (wchar_t)(mn_user_data_ptr->user_valid_data_arr[2 * i + 1] + (uint16)(mn_user_data_ptr->user_valid_data_arr[2 * i] << 8));
        }
#endif
    }
    else if(alphabet_type == MN_SMS_8_BIT_ALPHBET)
    {
        tapi_user_data_ptr->length = (uint16)mn_user_data_ptr->length;
        MMI_STRNTOWSTR(tapi_user_data_ptr->user_data_arr, ITAPI_SMS_USER_DATA_LENGTH, (const uint8*)mn_user_data_ptr->user_valid_data_arr, MN_SMS_MAX_USER_VALID_DATA_LENGTH, tapi_user_data_ptr->length);
    }
    else
    {
        SCI_ASSERT(FALSE);
    }

    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertCAFToMN_SMS_TIME_STAMP_T(ITAPI_SMS_TIME_STAMP_T *tapi_time_stamp_ptr, MN_SMS_TIME_STAMP_T *mn_time_stamp_ptr)
{
    mn_time_stamp_ptr->year     = tapi_time_stamp_ptr->year;
    mn_time_stamp_ptr->month    = tapi_time_stamp_ptr->month;
    mn_time_stamp_ptr->day      = tapi_time_stamp_ptr->day;
    mn_time_stamp_ptr->hour     = tapi_time_stamp_ptr->hour;
    mn_time_stamp_ptr->minute   = tapi_time_stamp_ptr->minute;
    mn_time_stamp_ptr->second   = tapi_time_stamp_ptr->second;
    mn_time_stamp_ptr->timezone = tapi_time_stamp_ptr->timezone;
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_SMS_TIME_STAMP_T(MN_SMS_TIME_STAMP_T *mn_time_stamp_ptr, ITAPI_SMS_TIME_STAMP_T *tapi_time_stamp_ptr)
{
    tapi_time_stamp_ptr->year     = mn_time_stamp_ptr->year;
    tapi_time_stamp_ptr->month    = mn_time_stamp_ptr->month;
    tapi_time_stamp_ptr->day      = mn_time_stamp_ptr->day;
    tapi_time_stamp_ptr->hour     = mn_time_stamp_ptr->hour;
    tapi_time_stamp_ptr->minute   = mn_time_stamp_ptr->minute;
    tapi_time_stamp_ptr->second   = mn_time_stamp_ptr->second;
    tapi_time_stamp_ptr->timezone = mn_time_stamp_ptr->timezone;
    
    return ITAPI_SUCCESS;
}


LOCAL int32 ConvertCAFToMN_MOSMS_T(ITAPI_SMS_MO_SMS_T *tapi_sms_ptr, MN_SMS_MO_SMS_T *mn_sms_ptr)
{
    mn_sms_ptr->status_report_is_request = tapi_sms_ptr->status_report_is_request;
    mn_sms_ptr->dest_addr_present = TRUE;
    mn_sms_ptr->pid_present       = TRUE;
    mn_sms_ptr->time_format_e     = MN_SMS_TP_VPF_RELATIVE_FORMAT;
    mn_sms_ptr->pid_e             = MN_SMS_PID_DEFAULT_TYPE;

    ConvertCAFToMN_NUMBER_T(&tapi_sms_ptr->dest_addr_t, &mn_sms_ptr->dest_addr_t);
    ConvertCAFToMN_SMS_TIME_STAMP_T(&tapi_sms_ptr->time_stamp_t, &mn_sms_ptr->time_stamp_t);
    mn_sms_ptr->time_valid_period.time_second = 50000000;

    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_MOSMS_T(MN_SMS_MO_SMS_T *mn_sms_ptr, ITAPI_SMS_T *tapi_sms_ptr)
{
    tapi_sms_ptr->status_report_is_request = mn_sms_ptr->status_report_is_request;
    tapi_sms_ptr->alphabet_type            = ConvertMNToCAF_SMS_ALPHABET_TYPE_E(mn_sms_ptr->dcs.alphabet_type);
    ConvertMNToCAF_NUMBER_T(&mn_sms_ptr->dest_addr_t, &tapi_sms_ptr->dest_addr_t);
    ConvertMNToCAF_SMS_TIME_STAMP_T(&mn_sms_ptr->time_stamp_t, &tapi_sms_ptr->time_stamp_t);
    
    return ITAPI_SUCCESS;
}


LOCAL int32 ConvertCAFToMN_SMS_ADDR_T(ITAPI_SMS_ADDR_T *tapi_addr_ptr, MN_SMS_ADDR_T *mn_addr_ptr)
{
    mn_addr_ptr->length = tapi_addr_ptr->length;
    SCI_MEMCPY(mn_addr_ptr->addr_arr_t, tapi_addr_ptr->addr_arr_t, ITAPI_SMS_ADDR_MAX_LEN);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertCAFToMN_L3_MSG_UNIT_T(ITAPI_L3_MSG_UNIT_T *tapi_tpdu_ptr, MN_L3_MSG_UNIT_T *mn_tpdu_ptr)
{
    mn_tpdu_ptr->length = tapi_tpdu_ptr->length;
    SCI_MEMCPY(mn_tpdu_ptr->l3_msg, tapi_tpdu_ptr->l3_msg, ITAPI_MAX_L3_MESSAGE_SIZE);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_L3_MSG_UNIT_T(MN_L3_MSG_UNIT_T *mn_tpdu_ptr, ITAPI_L3_MSG_UNIT_T *tapi_tpdu_ptr)
{
    tapi_tpdu_ptr->length = mn_tpdu_ptr->length;
    SCI_MEMCPY(tapi_tpdu_ptr->l3_msg, mn_tpdu_ptr->l3_msg, ITAPI_MAX_L3_MESSAGE_SIZE);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_SMS_STATUS_REPORT_T(MN_SMS_STATUS_REPORT_T *mn_status_report_ptr, ITAPI_SMS_T *itapi_status_report_ptr)
{
    itapi_status_report_ptr->tp_status = ConvertMNToCAF_SMS_TP_STATUS_E(mn_status_report_ptr->tp_status);
    ConvertMNToCAF_NUMBER_T(&mn_status_report_ptr->dest_addr_t, &itapi_status_report_ptr->dest_addr_t);
    ConvertMNToCAF_SMS_TIME_STAMP_T(&mn_status_report_ptr->tp_dt, &itapi_status_report_ptr->time_stamp_t);

    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_MTSMS_T(MN_SMS_MT_SMS_T *mn_sms_ptr, ITAPI_SMS_T *tapi_sms_ptr)
{
    ConvertMNToCAF_NUMBER_T(&mn_sms_ptr->origin_addr_t, &tapi_sms_ptr->dest_addr_t);
    ConvertMNToCAF_SMS_TIME_STAMP_T(&mn_sms_ptr->time_stamp_t, &tapi_sms_ptr->time_stamp_t);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_READSMS_CNF_T(MnReadSmsCnfS *mn_cnf_ptr, ITAPI_READSMS_CNF_T *tapi_cnf_ptr)
{
    ERR_MNSMS_CODE_E         nErr = ERR_MNSMS_NONE;
    MN_SMS_USER_DATA_HEAD_T  user_data_header_t = {0};
    MN_SMS_USER_VALID_DATA_T user_valid_data_t = {0};

    if(MN_SMS_OPERATE_SUCCESS == mn_cnf_ptr->cause)
    {
        tapi_cnf_ptr->storage    = ConvertMNToCAF_STORAGE_E(mn_cnf_ptr->storage);
        tapi_cnf_ptr->record_id  = mn_cnf_ptr->record_id;
        tapi_cnf_ptr->item_sys   = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->item_sys);
        tapi_cnf_ptr->sms_t.status = ConvertMNToCAF_SMS_STATUS_E(mn_cnf_ptr->status);

        if((mn_cnf_ptr->status == MN_SMS_MT_READED) || \
           (mn_cnf_ptr->status == MN_SMS_MT_TO_BE_READ))
        {
            ConvertMNToCAF_MTSMS_T(&mn_cnf_ptr->sms_t_u.mt_sms_t, &tapi_cnf_ptr->sms_t);
            MNSMS_DecodeUserDataEx(mn_cnf_ptr->item_sys, mn_cnf_ptr->sms_t_u.mt_sms_t.user_head_is_exist, &mn_cnf_ptr->sms_t_u.mt_sms_t.dcs, &mn_cnf_ptr->sms_t_u.mt_sms_t.user_data_t, &user_data_header_t, &user_valid_data_t);
            /* judge MMS or not */
            nErr = MNSMS_IsSupportMMSEx(mn_cnf_ptr->item_sys, user_data_header_t.length, user_data_header_t.user_header_arr, &tapi_cnf_ptr->is_mms);
            //SCI_TRACE_LOW:"ConvertMNToCAF_READSMS_CNF_T is_mms is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_1722_112_2_17_23_1_8_16,(uint8*)"d", tapi_cnf_ptr->is_mms);
            ConvertMNToCAF_SMS_USER_VALID_DATA_T(&user_valid_data_t, &tapi_cnf_ptr->sms_t.user_data_t, mn_cnf_ptr->sms_t_u.mt_sms_t.dcs.alphabet_type, mn_cnf_ptr->status);
        }
        else if(mn_cnf_ptr->status == MN_SMS_MO_TO_BE_SEND)
        {
            ConvertMNToCAF_MOSMS_T(&mn_cnf_ptr->sms_t_u.mo_sms_t, &tapi_cnf_ptr->sms_t);
            MNSMS_DecodeUserDataEx(mn_cnf_ptr->item_sys, mn_cnf_ptr->sms_t_u.mo_sms_t.user_head_is_exist, &mn_cnf_ptr->sms_t_u.mo_sms_t.dcs, &mn_cnf_ptr->sms_t_u.mo_sms_t.user_data_t, &user_data_header_t, &user_valid_data_t);
            ConvertMNToCAF_SMS_USER_VALID_DATA_T(&user_valid_data_t, &tapi_cnf_ptr->sms_t.user_data_t, mn_cnf_ptr->sms_t_u.mo_sms_t.dcs.alphabet_type, mn_cnf_ptr->status);
        }
        else if((mn_cnf_ptr->status == MN_SMS_MO_SR_RECEIVED_AND_STORED) || \
                (mn_cnf_ptr->status == MN_SMS_MT_SR_READED) || \
                (mn_cnf_ptr->status == MN_SMS_MT_SR_TO_BE_READ))
        {
            ConvertMNToCAF_SMS_STATUS_REPORT_T(&mn_cnf_ptr->sms_t_u.report_sms_t, &tapi_cnf_ptr->sms_t);
        }
        else if((mn_cnf_ptr->status == MN_SMS_MO_SR_NOT_REQUEST) || \
            (mn_cnf_ptr->status == MN_SMS_MO_SR_REQUESTED_NOT_RECEIVED) || \
            (mn_cnf_ptr->status == MN_SMS_MO_SR_RECEIVED_NOT_STORE))
        {

        }
        else if(mn_cnf_ptr->status == MN_SMS_FREE_SPACE)
        {

        }
        else
        {
            SCI_ASSERT(FALSE);
        }
        return ITAPI_SUCCESS;
    }

    return ITAPI_ERROR;
}

LOCAL int32 ConvertMNToCAF_UPDATESMS_CNF_T(MnUpdateSmsStateCnfS *mn_cnf_ptr, ITAPI_UPDATESMS_CNF_T *tapi_cnf_ptr)
{
    tapi_cnf_ptr->cause      = ConvertMNToCAF_SMS_CAUSE_E(mn_cnf_ptr->cause);
    tapi_cnf_ptr->record_id  = mn_cnf_ptr->record_id;
    tapi_cnf_ptr->storage    = ConvertMNToCAF_STORAGE_E(mn_cnf_ptr->storage);
    tapi_cnf_ptr->dual_sys   = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->dual_sys);

    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_SENDSMS_CNF_T(MnSendSmsCnfS *mn_cnf_ptr, ITAPI_SENDSMS_CNF_T *tapi_cnf_ptr)
{
    tapi_cnf_ptr->cause     = ConvertMNToCAF_SMS_CAUSE_E(mn_cnf_ptr->cause);
    tapi_cnf_ptr->tp_mr     = mn_cnf_ptr->tp_mr;
    tapi_cnf_ptr->dual_sys  = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->dual_sys);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_WRITESMS_CNF_T(MnWriteSmsCnfS *mn_cnf_ptr, ITAPI_WRITESMS_CNF_T *tapi_cnf_ptr)
{
    tapi_cnf_ptr->cause     = ConvertMNToCAF_SMS_CAUSE_E(mn_cnf_ptr->cause);
    tapi_cnf_ptr->storage   = ConvertMNToCAF_STORAGE_E(mn_cnf_ptr->storage);
    tapi_cnf_ptr->dual_sys  = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->dual_sys);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_WRITESMSPARAM_CNF_T(MnWriteSmsParamCnfS *mn_cnf_ptr, ITAPI_WRITESMSPARAM_CNF_T *tapi_cnf_ptr)
{
    tapi_cnf_ptr->cause     = ConvertMNToCAF_SMS_CAUSE_E(mn_cnf_ptr->cause);
    tapi_cnf_ptr->record_id = mn_cnf_ptr->record_id;
    tapi_cnf_ptr->dual_sys  = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->dual_sys);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_SETSMSRECEIVEFUN_CNF_T(MnSetSmsReceiveFuncCnf *mn_cnf_ptr, ITAPI_SETSMSRECEIVEFUN_CNF_T *tapi_cnf_ptr)
{
    tapi_cnf_ptr->cause          = ConvertMNToCAF_SMS_CAUSE_E(mn_cnf_ptr->cause);
    tapi_cnf_ptr->is_receive_sms = mn_cnf_ptr->is_receive_sms;
    tapi_cnf_ptr->dual_sys       = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->dual_sys);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_MEMFULLSMS_IND_T(MnMemFullSmsInd *mn_cnf_ptr, ITAPI_MEMFULLSMS_IND_T *tapi_cnf_ptr)
{
    tapi_cnf_ptr->mem_status = ConvertMNToCAF_SMS_MEM_FULL_STATUS_E(mn_cnf_ptr->mem_status);
    tapi_cnf_ptr->dual_sys   = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->dual_sys);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_SMSREPORT_IND_T(MnSmsReportIndS *mn_cnf_ptr, ITAPI_SMSREPORT_IND_T *tapi_cnf_ptr)
{
    tapi_cnf_ptr->storage    = ConvertMNToCAF_STORAGE_E(mn_cnf_ptr->storage);
    tapi_cnf_ptr->record_id  = mn_cnf_ptr->record_id;
    tapi_cnf_ptr->dual_sys   = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->dual_sys);
    ConvertMNToCAF_L3_MSG_UNIT_T(&mn_cnf_ptr->msg_data, &tapi_cnf_ptr->msg_data);
    ConvertMNToCAF_SMS_STATUS_REPORT_T(&mn_cnf_ptr->status_report, &tapi_cnf_ptr->sms_t);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_CALL_START_IND_T(MnCcCallStartIndS *mn_cnf_ptr, ITAPI_CALL_START_IND_T *tapi_cnf_ptr)
{
    tapi_cnf_ptr->call_id   = mn_cnf_ptr->call_id;
    tapi_cnf_ptr->dual_sys  = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->dual_sys);
    ConvertMNToCAF_NUMBER_T(&mn_cnf_ptr->called_num, &tapi_cnf_ptr->called_num);

    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_CALL_SETUP_COMPLETE_IND_T(MnCcSetupCompleteIndS *mn_cnf_ptr, ITAPI_CALL_SETUP_COMPLETE_IND_T *tapi_cnf_ptr)
{
    tapi_cnf_ptr->call_id   = mn_cnf_ptr->call_id;
    tapi_cnf_ptr->dual_sys  = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->dual_sys);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_CALL_SETUP_COMPLETE_CNF_T(MnCcSetupCompleteCnfS *mn_cnf_ptr, ITAPI_CALL_SETUP_COMPLETE_CNF_T *tapi_cnf_ptr)
{
    tapi_cnf_ptr->call_id   = mn_cnf_ptr->call_id;
    tapi_cnf_ptr->dual_sys  = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->dual_sys);
    tapi_cnf_ptr->connected_num_present = mn_cnf_ptr->connected_num_present;
    tapi_cnf_ptr->connected_num.number_type = ConvertMNToCAF_NUMBER_TYPE_E(mn_cnf_ptr->connected_num.number_type);
    tapi_cnf_ptr->connected_num.num_len = MMIAPICOM_GenDispNumber(mn_cnf_ptr->connected_num.number_type, (uint8)mn_cnf_ptr->connected_num.num_len, (uint8 *)mn_cnf_ptr->connected_num.party_num, tapi_cnf_ptr->connected_num.party_num, ITAPI_MAX_ADDR_BCD_LEN);

    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_CALL_DISCONNECTED_IND_T(MnCcCallDisconnectdIndS *mn_cnf_ptr, ITAPI_CALL_DISCONNECTED_IND_T *tapi_cnf_ptr)
{
    tapi_cnf_ptr->call_id   = mn_cnf_ptr->call_id;
    tapi_cnf_ptr->dual_sys  = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->dual_sys);
    tapi_cnf_ptr->disconnected_cause  = ConvertMNToCAF_CALL_DISCONNECT_CAUSE_E(mn_cnf_ptr->disconnected_cause);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_CALL_ERR_IND_T(MnCcCallErrIndS *mn_cnf_ptr, ITAPI_CALL_ERR_IND_T *tapi_cnf_ptr)
{
    tapi_cnf_ptr->call_id   = mn_cnf_ptr->call_id;
    tapi_cnf_ptr->dual_sys  = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->dual_sys);
    tapi_cnf_ptr->err_cause  = ConvertMNToCAF_CALL_DISCONNECT_CAUSE_E(mn_cnf_ptr->error_cause);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_PLMN_INFO_T(MSG_CAMP_STATUS_T *mn_plmn_info_ptr, ITAPI_PLMN_INFO_T *tapi_plmn_info_ptr)
{
    tapi_plmn_info_ptr->plmn_status     = ConvertMNToCAF_PHONE_PLMN_STATUS_E((MN_PHONE_PLMN_STATUS_E)mn_plmn_info_ptr->plmn_status);
    tapi_plmn_info_ptr->plmn_is_roaming = mn_plmn_info_ptr->plmn_is_roaming;
    tapi_plmn_info_ptr->mcc             = mn_plmn_info_ptr->mcc;
    tapi_plmn_info_ptr->mnc             = mn_plmn_info_ptr->mnc;
    tapi_plmn_info_ptr->mnc_digit_num   = mn_plmn_info_ptr->mnc_digit_num;
    tapi_plmn_info_ptr->lac             = mn_plmn_info_ptr->lac;
    tapi_plmn_info_ptr->cell_id         = mn_plmn_info_ptr->cell_id;
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_SCELL_MEAS_T(MN_PHONE_SCELL_MEAS_T *mn_cell_info_ptr, ITAPI_PHONE_SCELL_MEAS_T *tapi_cell_info_ptr)
{
    if(mn_cell_info_ptr->is_traffic_mode)
    {
        if(mn_cell_info_ptr->is_dtx_used)
        {
            tapi_cell_info_ptr->rxqual = mn_cell_info_ptr->rxqual_sub;
        }
        else
        {
            tapi_cell_info_ptr->rxqual = mn_cell_info_ptr->rxqual_full;
        }
    }
    else
    {
        tapi_cell_info_ptr->rxqual = 99;
    }

    tapi_cell_info_ptr->rxlev   = mn_cell_info_ptr->rxlev;
    tapi_cell_info_ptr->cell_id = mn_cell_info_ptr->cell_id;
    tapi_cell_info_ptr->lac     = mn_cell_info_ptr->lac;
    tapi_cell_info_ptr->arfcn   = mn_cell_info_ptr->arfcn;
    tapi_cell_info_ptr->rxlev >>= 1;
    
    return ITAPI_SUCCESS;
}

LOCAL void ConvertBcdToDigitalStr(uint8 length, uint8 *bcd_ptr, uint8 *digital_ptr)
{
    int32         i;
    uint8         temp;
    
    // get the first digital
    temp = (uint8)((*bcd_ptr >> 4) &0x0f);
    if (temp >= 0x0a)
    {
        *digital_ptr = (uint8)((temp - 0x0a) + 'A');
    }
    else
    {
        *digital_ptr = (uint8)(temp + '0');
    }
    
    bcd_ptr++;
    digital_ptr++;
    for (i=0; i<(length - 1); i++)
    {
        temp = *bcd_ptr;
        temp &= 0x0f;
        if (temp >= 0x0a)
        {
            *digital_ptr = (uint8)((temp - 0x0a) + 'A');
        }
        else
        {
            *digital_ptr = (uint8)(temp + '0');
        }
        digital_ptr++;
        temp = *bcd_ptr;
        temp = (uint8)((temp & 0xf0) >> 4);
        if ((temp == 0x0f) && (i == (length -1)))
        {
            *digital_ptr = '\0';  
            return;
        }
        else if (temp>=0x0a)
        {
            *digital_ptr = (uint8)((temp - 0x0a) + 'A');
        }
        else
        {
            *digital_ptr = (uint8)(temp + '0');
        }
        digital_ptr++;
        bcd_ptr++;
    }
    *digital_ptr = '\0';
}

LOCAL int32 ConvertMNToCAF_IMSI_T(MN_IMSI_T *mn_imsi_ptr, ITAPI_IMSI_T *tapi_imsi_ptr)
{
    tapi_imsi_ptr->imsi_len = mn_imsi_ptr->imsi_len;
    ConvertBcdToDigitalStr(mn_imsi_ptr->imsi_len, mn_imsi_ptr->imsi_val, tapi_imsi_ptr->imsi_val);
    
    return ITAPI_SUCCESS;
}

LOCAL int32 ConvertMNToCAF_CALL_IND_T(MnCcSetupIndS *mn_cnf_ptr, ITAPI_CALL_SETUP_IND_T *tapi_cnf_ptr)
{
    tapi_cnf_ptr->call_id               = mn_cnf_ptr->call_id;
    tapi_cnf_ptr->dual_sys              = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->dual_sys);
    tapi_cnf_ptr->calling_num_present   = mn_cnf_ptr->calling_num_present;
    if(mn_cnf_ptr->calling_num_present)
    {
        tapi_cnf_ptr->called_num.number_type = ConvertMNToCAF_NUMBER_TYPE_E(mn_cnf_ptr->calling_num.number_type);
        tapi_cnf_ptr->called_num.num_len = MMIAPICOM_GenDispNumber(mn_cnf_ptr->calling_num.number_type, (uint8)mn_cnf_ptr->calling_num.num_len, (uint8 *)mn_cnf_ptr->calling_num.party_num, tapi_cnf_ptr->called_num.party_num, ITAPI_MAX_ADDR_BCD_LEN);
    }
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : judge the CAF handle is valid or not.
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CAFITAPI_IsValidHandle(CAF_HANDLE_T handle)
{
    if(PNULL == handle)
    {
        return FALSE;
    }

    if(PNULL == MMK_FindBaseNode( handle ))
    {
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : Get the SMS max support number and used number in the selected 
//                system and storage
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_GetSmsNum(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 *max_sms_num_ptr, uint16 *used_sms_num_ptr)
{
    ERR_MNSMS_CODE_E result;
    MN_DUAL_SYS_E    dual_sys_mn;
    MN_SMS_STORAGE_E storage_mn;
    
    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    
    dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);
    storage_mn  = ConvertCAFToMN_STORAGE_E(storage);
    result = MNSMS_GetSmsNumEx(dual_sys_mn, storage_mn, max_sms_num_ptr, used_sms_num_ptr);
    if(ERR_MNSMS_NONE != result)
    {
        return ITAPI_ERROR;
    }
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : Get the SMS state and the max support number in the 
//                destination storage
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_GetSmsStatus(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 *max_sms_num_ptr, ITAPI_SMS_STATUS_E *status_ptr)
{
    uint16           i;
    ERR_MNSMS_CODE_E result;
    MN_DUAL_SYS_E    dual_sys_mn;
    MN_SMS_STORAGE_E storage_mn;

    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    
    dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);
    storage_mn  = ConvertCAFToMN_STORAGE_E(storage);
    result = MNSMS_GetSmsStatusEx(dual_sys_mn, storage_mn, max_sms_num_ptr, (MN_SMS_STATUS_E *)status_ptr);
    if(ERR_MNSMS_NONE != result)
    {
        return ITAPI_ERROR;
    }

    if(status_ptr)
    {
        for(i = 0; i < *max_sms_num_ptr; i++)
        {
            *(status_ptr + i) = ConvertMNToCAF_SMS_STATUS_E((MN_SMS_STATUS_E)*(status_ptr + i));
        }
    }

    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : set storage priority, SIM first or ME first
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_SetStoragePriority(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, BOOLEAN is_sim_first)
{
    ERR_MNSMS_CODE_E result;
    MN_DUAL_SYS_E    dual_sys_mn;

    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);

    dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);
    result = MNSMS_SetStoragePriorityEx(dual_sys_mn, is_sim_first);
    if(ERR_MNSMS_NONE != result)
    {
        return ITAPI_ERROR;
    }

    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : get storage priority, sim first or me first
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_GetStoragePriority(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, BOOLEAN *is_sim_first_ptr)
{
    ERR_MNSMS_CODE_E result;
    MN_DUAL_SYS_E    dual_sys_mn;
    
    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    
    dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);
    result = MNSMS_GetStoragePriorityEx(dual_sys_mn, is_sim_first_ptr);
    if(ERR_MNSMS_NONE != result)
    {
        return ITAPI_ERROR;
    }
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : Read the the SMS according to the record_id 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_ReadSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 record_id)
{
    ERR_MNSMS_CODE_E result;
    MN_DUAL_SYS_E    dual_sys_mn;
    MN_SMS_STORAGE_E storage_mn;
    
    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    
    CAFITAPI_SetCnfTapiPtr(tapi_ptr);
    dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);
    storage_mn  = ConvertCAFToMN_STORAGE_E(storage);
    result = MNSMS_ReadSmsEx(dual_sys_mn, storage_mn, record_id);
    if(ERR_MNSMS_NONE != result)
    {
        return ITAPI_ERROR;
    }

    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : update the SMS status in destination storage, the operation 
//                can delete an SMS or change the state from read to be read.
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_UpdateSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, uint16 record_id, ITAPI_SMS_STATUS_E status)
{
    ERR_MNSMS_CODE_E result;
    MN_DUAL_SYS_E    dual_sys_mn;
    MN_SMS_STORAGE_E storage_mn;
    MN_SMS_STATUS_E  status_mn;
    
    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    
    CAFITAPI_SetCnfTapiPtr(tapi_ptr);
    dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);
    storage_mn  = ConvertCAFToMN_STORAGE_E(storage);
    status_mn   = ConvertCAFToMN_SMS_STATUS_E(status);
    result = MNSMS_UpdateSmsStatusEx(dual_sys_mn, storage_mn, record_id, status_mn);
    if(ERR_MNSMS_NONE != result)
    {
        return ITAPI_ERROR;
    }

    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : Convert the wchart msg to ascii and get alpabet 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:   
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_ConvertMsgContent(ITAPI_T * tapi_ptr)
{
    BOOLEAN  is_ucs2 = FALSE;
    CAFTAPI_T * caftapi_ptr = (CAFTAPI_T *)tapi_ptr;
    MN_SMS_ALPHABET_TYPE_E  alphabet_type = MN_SMS_DEFAULT_ALPHABET;

    is_ucs2 = (BOOLEAN)!MMIAPICOM_IsASCIIString(caftapi_ptr->sms_ctrl_ptr->tapi_sms.user_data_t.user_data_arr, caftapi_ptr->sms_ctrl_ptr->tapi_sms.user_data_t.length);

    alphabet_type = (is_ucs2) ? MN_SMS_UCS2_ALPHABET : MN_SMS_DEFAULT_ALPHABET;

    if(caftapi_ptr->sms_ctrl_ptr->tapi_sms.user_data_t.length)
    {
        if (MN_SMS_DEFAULT_ALPHABET == alphabet_type)
        {
            caftapi_ptr->sms_ctrl_ptr->tapi_sms.user_data_t.length = (uint16)MIN(caftapi_ptr->sms_ctrl_ptr->tapi_sms.user_data_t.length, ITAPI_SMS_MAX_MESSAGE_LEN);
            MMI_WSTRNTOSTR((uint8*)caftapi_ptr->sms_ctrl_ptr->ascii_content, ITAPI_SMS_MAX_MESSAGE_LEN, caftapi_ptr->sms_ctrl_ptr->tapi_sms.user_data_t.user_data_arr, ITAPI_SMS_MAX_MESSAGE_LEN, caftapi_ptr->sms_ctrl_ptr->tapi_sms.user_data_t.length);
            // translate the ascii to default according GSM03.38
            caftapi_ptr->sms_ctrl_ptr->data_length = MMIAPICOM_Ascii2default(caftapi_ptr->sms_ctrl_ptr->ascii_content, caftapi_ptr->sms_ctrl_ptr->default_data, caftapi_ptr->sms_ctrl_ptr->tapi_sms.user_data_t.length);
            caftapi_ptr->sms_ctrl_ptr->data_length = (uint16)MIN(caftapi_ptr->sms_ctrl_ptr->data_length, ITAPI_SMS_MAX_MESSAGE_LEN);
            SCI_MEMSET(caftapi_ptr->sms_ctrl_ptr->ascii_content, 0, ITAPI_SMS_MAX_MESSAGE_LEN);
            SCI_MEMCPY(caftapi_ptr->sms_ctrl_ptr->ascii_content, caftapi_ptr->sms_ctrl_ptr->default_data, caftapi_ptr->sms_ctrl_ptr->data_length);
        }
        else if(MN_SMS_UCS2_ALPHABET == alphabet_type)
        {
            caftapi_ptr->sms_ctrl_ptr->data_length = (uint16)MIN(caftapi_ptr->sms_ctrl_ptr->tapi_sms.user_data_t.length << 1, ITAPI_SMS_MAX_MESSAGE_LEN);
            SCI_MEMSET(caftapi_ptr->sms_ctrl_ptr->ascii_content, 0, ITAPI_SMS_MAX_MESSAGE_LEN);
            SCI_MEMCPY(caftapi_ptr->sms_ctrl_ptr->ascii_content, (uint8 *)caftapi_ptr->sms_ctrl_ptr->tapi_sms.user_data_t.user_data_arr, caftapi_ptr->sms_ctrl_ptr->data_length);
        }
    }

    caftapi_ptr->sms_ctrl_ptr->alphabet_type = alphabet_type;
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : Get limit length by alpabet 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:   
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_GetLenLimitByAlpha(MN_SMS_ALPHABET_TYPE_E alpha, ITAPI_SMS_LEN_LIMIT_E len_limit_type, uint8 *split_len_ptr, uint8 *limit_len_ptr)
{
    SCI_ASSERT(PNULL != split_len_ptr);
    SCI_ASSERT(PNULL != limit_len_ptr);
    
    // get the limited condition according to the alphabet
    switch (alpha)
    {
        case MN_SMS_UCS2_ALPHABET:
            if(ITAPI_LEN_LIMIT_JAVA == len_limit_type)
            {
                (*limit_len_ptr) = ITAPI_SMS_JAVA_MAX_UCS2_BYTE_LEN;
                (*split_len_ptr) = ITAPI_SMS_JAVA_MAX_UCS2_SPLIT_LEN;
            }
            else if(ITAPI_LEN_LIMIT_DM == len_limit_type)
            {
                (*limit_len_ptr) = ITAPI_SMS_VCARD_MAX_UCS2_BYTE_LEN;
                (*split_len_ptr) = ITAPI_SMS_VCARD_MAX_UCS2_SPLIT_LEN;
            }
            else
            {  
                if (ITAPI_LEN_LIMIT_VCARD == len_limit_type)
                {
                    (*limit_len_ptr) = ITAPI_SMS_VCARD_MAX_UCS2_BYTE_LEN;
                    (*split_len_ptr) = ITAPI_SMS_VCARD_MAX_UCS2_SPLIT_LEN;
                }
                else
                {
                    (*limit_len_ptr) = ITAPI_SMS_MAX_UCS2_BYTE_LEN;
                    (*split_len_ptr) = ITAPI_SMS_MAX_UCS2_SPLIT_LEN;
                }
            }
            break;
        case MN_SMS_DEFAULT_ALPHABET:
            if(ITAPI_LEN_LIMIT_JAVA == len_limit_type)
            {
                (*limit_len_ptr) = ITAPI_SMS_JAVA_MAX_DEF_BYTE_LEN;
                (*split_len_ptr) = ITAPI_SMS_JAVA_MAX_DEF_SPLIT_LEN;
            }
            else if (ITAPI_LEN_LIMIT_DM == len_limit_type)
            {
                (*limit_len_ptr) = ITAPI_SMS_VCARD_MAX_DEF_BYTE_LEN;
                (*split_len_ptr) = ITAPI_SMS_VCARD_MAX_DEF_SPLIT_LEN;
            }
            else
            {
                if (ITAPI_LEN_LIMIT_VCARD == len_limit_type)
                {
                    (*limit_len_ptr) = ITAPI_SMS_VCARD_MAX_DEF_BYTE_LEN;
                    (*split_len_ptr) = ITAPI_SMS_VCARD_MAX_DEF_SPLIT_LEN;
                }
                else
                {
                    (*limit_len_ptr) = ITAPI_SMS_MAX_DEF_BYTE_LEN;
                    (*split_len_ptr) = ITAPI_SMS_MAX_DEF_SPLIT_LEN;
                }
            }
            break;
        case MN_SMS_8_BIT_ALPHBET:
            if(ITAPI_LEN_LIMIT_JAVA == len_limit_type)
            {
                (*limit_len_ptr) = ITAPI_SMS_JAVA_MAX_8BIT_BYTE_LEN;
                (*split_len_ptr) = ITAPI_SMS_JAVA_MAX_8BIT_SPLIT_LEN;
            }
            else if(ITAPI_LEN_LIMIT_DM == len_limit_type)
            {
                (*limit_len_ptr) = ITAPI_SMS_VCARD_MAX_8BIT_BYTE_LEN;
                (*split_len_ptr) = ITAPI_SMS_VCARD_MAX_8BIT_SPLIT_LEN;
            }
            else
            {
                if (ITAPI_LEN_LIMIT_VCARD == len_limit_type)
                {
                    (*limit_len_ptr) = ITAPI_SMS_VCARD_MAX_8BIT_BYTE_LEN;
                    (*split_len_ptr) = ITAPI_SMS_VCARD_MAX_8BIT_SPLIT_LEN;
                }
                else
                {
                    (*limit_len_ptr) = ITAPI_SMS_MAX_8BIT_BYTE_LEN;
                    (*split_len_ptr) = ITAPI_SMS_MAX_8BIT_SPLIT_LEN;
                }
            }
            break;
        default:
            SCI_ASSERT(FALSE);
            break;
    }
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//    Description : to split the long SMS 
//    Global resource dependence : none
//    Author: Wei.zhou
//    Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_SplitLongMsg(ITAPI_T * tapi_ptr)
{
    CAFTAPI_T * caftapi_ptr = (CAFTAPI_T *)tapi_ptr;
    uint8       limit_len = 0;
    uint8       split_len = 0;
    uint32      total_num = 0;

    CAFITAPI_SMS_GetLenLimitByAlpha(caftapi_ptr->sms_ctrl_ptr->alphabet_type, ITAPI_LEN_LIMIT_DEFAULT, &split_len, &limit_len);

    if (limit_len < caftapi_ptr->sms_ctrl_ptr->data_length)
    {
        if (MN_SMS_DEFAULT_ALPHABET == caftapi_ptr->sms_ctrl_ptr->alphabet_type)
        {
            uint32  i = 0;
            uint32  start_pos = 0;
            uint8   sms_length = 0;
            
            for (i = 0; i < MMISMS_SPLIT_MAX_NUM; i++)
            {
                sms_length = (uint8)MIN(split_len, (caftapi_ptr->sms_ctrl_ptr->data_length - start_pos));
                
                if (start_pos + sms_length < caftapi_ptr->sms_ctrl_ptr->data_length)
                {
                    if (ITAPI_DEFAULT_EXT_MARK == caftapi_ptr->sms_ctrl_ptr->ascii_content[start_pos + sms_length - 1])
                    {
                        start_pos = start_pos + sms_length - 1;
                    }
                    else
                    {
                        start_pos = start_pos + sms_length;
                    }
                }
                else
                {
                    i++;
                    break;
                }
            }
            total_num = i;
        }
        else
        {
            total_num = MMIAPICOM_CeilValue(caftapi_ptr->sms_ctrl_ptr->data_length, split_len);
        }
    }
    else
    {
        total_num = 1;
    }
    caftapi_ptr->sms_ctrl_ptr->seq_num   = 1;
    caftapi_ptr->sms_ctrl_ptr->total_num = total_num;

    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//    Description : to set the user data head
//    Global resource dependence : none
//    Author: Wei.zhou
//    Note: 
/*****************************************************************************/
PUBLIC int32 CAFITAPI_SMS_InitUserDataHead(ITAPI_T * tapi_ptr)
{
    uint8       pos     = 0;
    uint8       ref_num = 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    CAFTAPI_T * caftapi_ptr = (CAFTAPI_T *)tapi_ptr;
    
    SCI_ASSERT((0 < caftapi_ptr->sms_ctrl_ptr->total_num) && (caftapi_ptr->sms_ctrl_ptr->total_num <= ITAPI_SMS_SPLIT_MAX_NUM));
    SCI_ASSERT((0 < caftapi_ptr->sms_ctrl_ptr->seq_num) && (caftapi_ptr->sms_ctrl_ptr->seq_num <= caftapi_ptr->sms_ctrl_ptr->total_num));

    if(caftapi_ptr->sms_ctrl_ptr->total_num > 1)
    {
        caftapi_ptr->sms_ctrl_ptr->mn_sms.user_head_is_exist = TRUE;
        // set the head of user data according to spec
        if((caftapi_ptr->sms_ctrl_ptr->user_data_header.length > 0) &&
           (caftapi_ptr->sms_ctrl_ptr->user_data_header.user_header_arr[ITAPI_SMS_HEAD_IEL_POS] > 0))
        {
            pos = caftapi_ptr->sms_ctrl_ptr->user_data_header.length;
        }
        
        MMINV_READ(MMI_MODULE_SMS << 16, &ref_num, return_value);
        caftapi_ptr->sms_ctrl_ptr->user_data_header.length += ITAPI_SMS_HEAD_LENGTH;
        caftapi_ptr->sms_ctrl_ptr->user_data_header.user_header_arr[pos + ITAPI_SMS_HEAD_IEI_POS] = ITAPI_SMS_HEAD_IEI;
        caftapi_ptr->sms_ctrl_ptr->user_data_header.user_header_arr[pos + ITAPI_SMS_HEAD_IEL_POS] = ITAPI_SMS_HEAD_IEL;
        caftapi_ptr->sms_ctrl_ptr->user_data_header.user_header_arr[pos + ITAPI_SMS_HEAD_REF_NUM_POS] = ref_num;
        caftapi_ptr->sms_ctrl_ptr->user_data_header.user_header_arr[pos + ITAPI_SMS_HEAD_MAX_NUM_POS] = (uint8)caftapi_ptr->sms_ctrl_ptr->total_num;
        caftapi_ptr->sms_ctrl_ptr->user_data_header.user_header_arr[pos + ITAPI_SMS_HEAD_SEQ_NUM_POS] = (uint8)caftapi_ptr->sms_ctrl_ptr->seq_num;
    }

    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//    Description : produce the number encode
//    Global resource dependence : none
//    Author: Wei.zhou
//    Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_ProduceNumber(ITAPI_T * tapi_ptr)
{
    CAFTAPI_T * caftapi_ptr = (CAFTAPI_T *)tapi_ptr;

    if(caftapi_ptr->sms_ctrl_ptr->tapi_sms.dest_addr_t.num_len > 0)
    {
        MMIAPICOM_GenPartyNumber((uint8*)caftapi_ptr->sms_ctrl_ptr->tapi_sms.dest_addr_t.party_num, caftapi_ptr->sms_ctrl_ptr->tapi_sms.dest_addr_t.num_len, &caftapi_ptr->sms_ctrl_ptr->party_number);
        SCI_MEMSET(caftapi_ptr->sms_ctrl_ptr->mn_sms.dest_addr_t.party_num, 0, sizeof(caftapi_ptr->sms_ctrl_ptr->mn_sms.dest_addr_t.party_num));
        caftapi_ptr->sms_ctrl_ptr->mn_sms.dest_addr_t.num_len     = (uint16)MIN( caftapi_ptr->sms_ctrl_ptr->party_number.num_len, MN_MAX_ADDR_BCD_LEN);
        SCI_MEMCPY(caftapi_ptr->sms_ctrl_ptr->mn_sms.dest_addr_t.party_num, caftapi_ptr->sms_ctrl_ptr->party_number.bcd_num, caftapi_ptr->sms_ctrl_ptr->mn_sms.dest_addr_t.num_len);
    }

    return ITAPI_SUCCESS;
}


/*****************************************************************************/
// Description : Set current seq number
// Global resource dependence:
// Author: Wei.zhou
// Note: 
/*****************************************************************************/
LOCAL void CAFITAPI_SMS_SetCurrentSeqNum(MN_SMS_USER_DATA_HEAD_T *head_ptr, uint8 seq_num)
{
    uint8 pos = 0, i = 0;
    uint8 iei_num = 0;
    int index = -1;
    MN_SMS_USER_HEAD_T decode_head = {0};
    MN_DUAL_SYS_E dual_sys= MN_DUAL_SYS_1;
    
    SCI_ASSERT( PNULL != head_ptr );
    
    MNSMS_DecodeUserDataHeadEx(dual_sys, head_ptr, &decode_head);
    iei_num = decode_head.iei_num;
    for (i = 0;i < iei_num;i++)
    {
        if (MN_SMS_CONCAT_8_BIT_REF_NUM == decode_head.iei_arr[i].iei)
        {
            index = i;
            break;
        }
        else
        {
            pos += decode_head.iei_arr[i].length + 1;
        }
    }
    if (-1 != index )
    {
        head_ptr->user_header_arr[pos+i + ITAPI_SMS_HEAD_SEQ_NUM_POS] = seq_num;
    }
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &decode_head);
    
    return;
}

/*****************************************************************************/
//    Description : produce the number encode
//    Global resource dependence : none
//    Author: Wei.zhou
//    Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_GetUserData(ITAPI_T * tapi_ptr, uint32 seq_num)
{
    CAFTAPI_T  *caftapi_ptr = (CAFTAPI_T *)tapi_ptr;
    uint8       split_len = 0;
    uint8       limit_len = 0;
    uint8       msg_len = 0;
    uint16      start_pos = 0;
    uint32      i = 0;

    SCI_PASSERT((0 < seq_num) && (ITAPI_SMS_SPLIT_MAX_NUM >= seq_num), ("TAPI_SMS: seq_num = %d", seq_num));
    
    // get the limit condition according to the alphabet
    CAFITAPI_SMS_GetLenLimitByAlpha(caftapi_ptr->sms_ctrl_ptr->alphabet_type, ITAPI_LEN_LIMIT_DEFAULT, &split_len, &limit_len);
    msg_len = (uint8)((0 < caftapi_ptr->sms_ctrl_ptr->user_data_header.length) ? split_len : limit_len);
    
    // count the start pos
    for (i = 0; i < seq_num - 1; i++)
    {
        start_pos = (uint16)(start_pos + caftapi_ptr->sms_ctrl_ptr->split_sms_length[i]);
    }
    SCI_ASSERT(start_pos <= caftapi_ptr->sms_ctrl_ptr->data_length) ;
    
    // get the length of user data
    caftapi_ptr->sms_ctrl_ptr->mn_user_valid_data.length = (uint8)MIN(msg_len, (caftapi_ptr->sms_ctrl_ptr->data_length - start_pos));
    
    if (MN_SMS_DEFAULT_ALPHABET == caftapi_ptr->sms_ctrl_ptr->alphabet_type)
    {
        if(ITAPI_DEFAULT_EXT_MARK == caftapi_ptr->sms_ctrl_ptr->ascii_content[start_pos + caftapi_ptr->sms_ctrl_ptr->mn_user_valid_data.length - 1])
        {
            caftapi_ptr->sms_ctrl_ptr->mn_user_valid_data.length = (uint8)(caftapi_ptr->sms_ctrl_ptr->mn_user_valid_data.length - 1);
        }
    }
    caftapi_ptr->sms_ctrl_ptr->split_sms_length[seq_num - 1] = caftapi_ptr->sms_ctrl_ptr->mn_user_valid_data.length;
   
    //  copy the valid data to the param
    SCI_ASSERT(caftapi_ptr->sms_ctrl_ptr->mn_user_valid_data.length <= MN_SMS_MAX_USER_VALID_DATA_LENGTH);

    SCI_MEMSET(caftapi_ptr->sms_ctrl_ptr->mn_user_valid_data.user_valid_data_arr, 0, sizeof(caftapi_ptr->sms_ctrl_ptr->mn_user_valid_data.user_valid_data_arr));
    SCI_MEMCPY(caftapi_ptr->sms_ctrl_ptr->mn_user_valid_data.user_valid_data_arr, 
        &(caftapi_ptr->sms_ctrl_ptr->ascii_content[start_pos]),
        caftapi_ptr->sms_ctrl_ptr->mn_user_valid_data.length);
    
    // get the information of user data
    // set the head of user data    
    if (0 < caftapi_ptr->sms_ctrl_ptr->user_data_header.length)
    {
        CAFITAPI_SMS_SetCurrentSeqNum(&caftapi_ptr->sms_ctrl_ptr->user_data_header, (uint8)seq_num);
        
        SCI_ASSERT(caftapi_ptr->sms_ctrl_ptr->user_data_header.length <= MN_SMS_USER_DATA_LENGTH);
    }

    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : to check whether it has the next SMS to send according to the 
//                head of user data
//  Global resource dependence :
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CAFITAPI_SMS_IsExistNextSMS(ITAPI_T * tapi_ptr)
{
    BOOLEAN     is_next_sms = FALSE;
    MN_SMS_USER_DATA_HEAD_T *head_ptr = PNULL;
    uint8       i = 0;
    uint8       max_num = 0, seq_num = 0;
    uint8       iei_num = 0;
    MN_SMS_USER_HEAD_T decode_head = {0};
    MN_DUAL_SYS_E dual_sys= MN_DUAL_SYS_1;
    CAFTAPI_T  *caftapi_ptr = (CAFTAPI_T *)tapi_ptr;
    
    head_ptr = &caftapi_ptr->sms_ctrl_ptr->user_data_header;
    MNSMS_DecodeUserDataHeadEx(dual_sys, head_ptr, &decode_head);
    iei_num = decode_head.iei_num;
    for(i = 0; i < iei_num; i++)
    {
        if (MN_SMS_CONCAT_8_BIT_REF_NUM == decode_head.iei_arr[i].iei)
        {
            max_num = decode_head.iei_arr[i].iei_data_t[1];
            seq_num = decode_head.iei_arr[i].iei_data_t[2];
            if (max_num > seq_num)
            {
                is_next_sms = TRUE;
                break;
            }
        }
    }
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &decode_head);
    
    return (is_next_sms);
}

/*****************************************************************************/
// Description : Get current seq number
// Global resource dependence:
// Author:Wei.zhou
// Note: 
/*****************************************************************************/
LOCAL uint8 CAFITAPI_SMS_GetCurSeqNum(ITAPI_T * tapi_ptr)
{
    uint8               i = 0;
    uint8               seq_num = 1;
    uint8               iei_num = 0;
    MN_SMS_USER_HEAD_T  decode_head = {0};
    MN_DUAL_SYS_E       dual_sys= MN_DUAL_SYS_1;
    CAFTAPI_T * caftapi_ptr = (CAFTAPI_T *)tapi_ptr;
    
    MNSMS_DecodeUserDataHeadEx(dual_sys, &caftapi_ptr->sms_ctrl_ptr->user_data_header, &decode_head);
    iei_num = decode_head.iei_num;
    for(i = 0; i < iei_num; i++)
    {
        if(MN_SMS_CONCAT_8_BIT_REF_NUM == decode_head.iei_arr[i].iei)
        {
            seq_num = decode_head.iei_arr[i].iei_data_t[2];
            break;
        }
    }
    //SCI_TRACE_LOW:"SMS: Get_Current_Seq_Num =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_2559_112_2_17_23_1_10_17,(uint8*)"d", seq_num);
    MNSMS_FreeUserDataHeadSpaceEx(dual_sys, &decode_head);
    
    return seq_num;
}

/*****************************************************************************/
//  Description : Send a sequence MO short message
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:   
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_SendSequenceSms(ITAPI_T * tapi_ptr, uint32 seq_num)
{
    CAFTAPI_T * caftapi_ptr = (CAFTAPI_T *)tapi_ptr;
    MN_DUAL_SYS_E           dual_sys;
    MN_SMS_STORAGE_E        storage;
    MN_SMS_PATH_E           sms_path;
    MN_SMS_ALPHABET_TYPE_E  alphabet_type;
    ERR_MNSMS_CODE_E        result;

    CAFITAPI_SMS_GetUserData(tapi_ptr, seq_num);
    dual_sys = caftapi_ptr->sms_ctrl_ptr->dual_sys;
    storage  = caftapi_ptr->sms_ctrl_ptr->storage;
    sms_path = caftapi_ptr->sms_ctrl_ptr->sms_path;
    
    alphabet_type = caftapi_ptr->sms_ctrl_ptr->alphabet_type;
    SCI_MEMSET(&caftapi_ptr->sms_ctrl_ptr->mn_sms.user_data_t, 0, sizeof(caftapi_ptr->sms_ctrl_ptr->mn_sms.user_data_t));
    MNSMS_EncodeUserDataEx(dual_sys, caftapi_ptr->sms_ctrl_ptr->mn_sms.user_head_is_exist, alphabet_type, &caftapi_ptr->sms_ctrl_ptr->user_data_header, &caftapi_ptr->sms_ctrl_ptr->mn_user_valid_data, &(caftapi_ptr->sms_ctrl_ptr->mn_sms.user_data_t));
    
    // $$$$$$
    caftapi_ptr->sms_ctrl_ptr->mn_sms.dcs.alphabet_type = alphabet_type;
    CAFITAPI_SetCnfTapiPtr(tapi_ptr);
    result = MNSMS_AppSendSmsEx(dual_sys, &caftapi_ptr->sms_ctrl_ptr->mn_sms, storage, sms_path, FALSE);
    //SCI_TRACE_LOW:"MNSMS_AppSendSmsEx, result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_2591_112_2_17_23_1_10_18,(uint8*)"d", result);
    if(ERR_MNSMS_NONE != result)
    {
        SCI_FREE(caftapi_ptr->sms_ctrl_ptr);
        caftapi_ptr->sms_ctrl_ptr = PNULL;
        return ITAPI_ERROR;
    }
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : APP originate an MO short message, send out SMS 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:   
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_SendSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_MO_SMS_T *sms_ptr, ITAPI_SMS_STORAGE_E sms_write, ITAPI_SMS_PATH_E sms_path, BOOLEAN is_more)
{
    CAFTAPI_T * caftapi_ptr = (CAFTAPI_T *)tapi_ptr;
    int32 result = ITAPI_ERROR;

    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);

    /* avoid renter */
    if(caftapi_ptr->sms_ctrl_ptr)
    {
        return result;
    }

    caftapi_ptr->sms_ctrl_ptr = (ITAPI_SMS_CTRL_T *)SCI_ALLOC_APP(sizeof(ITAPI_SMS_CTRL_T));
    SCI_MEMSET(caftapi_ptr->sms_ctrl_ptr, 0, sizeof(ITAPI_SMS_CTRL_T));
    SCI_MEMCPY(&caftapi_ptr->sms_ctrl_ptr->tapi_sms, sms_ptr, sizeof(ITAPI_SMS_MO_SMS_T));

    caftapi_ptr->sms_ctrl_ptr->dual_sys = ConvertCAFToMN_DUAL_E(dual_sys);
    caftapi_ptr->sms_ctrl_ptr->storage  = ConvertCAFToMN_STORAGE_E(sms_write);
    caftapi_ptr->sms_ctrl_ptr->sms_path = ConvertCAFToMN_SMS_PATH_E(sms_path);

    /* the API will produce two message, APP_MN_SEND_SMS_CNF, APP_MN_WRITE_SMS_CNF */
    if(ITAPI_SMS_NO_STORED != caftapi_ptr->sms_ctrl_ptr->storage)
    {
        caftapi_ptr->sendneedsave = TRUE;
    }
    else
    {
        caftapi_ptr->sendneedsave = FALSE;
    }

    /* sms content wchar to char */
    CAFITAPI_SMS_ConvertMsgContent(tapi_ptr);
    CAFITAPI_SMS_SplitLongMsg(tapi_ptr);
    /* exist long sms or not, ok to init header */
    CAFITAPI_SMS_InitUserDataHead(tapi_ptr);

    ConvertCAFToMN_MOSMS_T(sms_ptr, &caftapi_ptr->sms_ctrl_ptr->mn_sms);
    CAFITAPI_SMS_ProduceNumber(tapi_ptr);

    /* send the 1st sms */
    result = CAFITAPI_SMS_SendSequenceSms(tapi_ptr, 1);

    return result;
}

/*****************************************************************************/
//  Description : originate an PDU MO short message, send out SMS 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:   
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_SendPduSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_ADDR_T *addr_ptr, ITAPI_L3_MSG_UNIT_T  *tpdu_ptr, BOOLEAN is_more)
{
    ERR_MNSMS_CODE_E result;
    MN_DUAL_SYS_E    dual_sys_mn;
    MN_SMS_ADDR_T    addr_mn = {0};
    MN_L3_MSG_UNIT_T tpdu_mn = {0};

    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);

    CAFITAPI_SetCnfTapiPtr(tapi_ptr);
    dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);
    ConvertCAFToMN_SMS_ADDR_T(addr_ptr, &addr_mn);
    ConvertCAFToMN_L3_MSG_UNIT_T(tpdu_ptr, &tpdu_mn);
    result = MNSMS_SendATSmsPDUEx(dual_sys_mn, &addr_mn, &tpdu_mn, is_more);
    if(ERR_MNSMS_NONE != result)
    {
        return ITAPI_ERROR;
    }
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : Send a sequence MO short message
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:   
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_WriteSequenceSms(ITAPI_T * tapi_ptr, uint32 seq_num)
{
    CAFTAPI_T * caftapi_ptr = (CAFTAPI_T *)tapi_ptr;
    MN_DUAL_SYS_E           dual_sys;
    MN_SMS_STORAGE_E        storage;
    MN_SMS_STATUS_E         status;
    MN_SMS_ALPHABET_TYPE_E  alphabet_type;
    ERR_MNSMS_CODE_E        result;

    CAFITAPI_SMS_GetUserData(tapi_ptr, seq_num);
    dual_sys = caftapi_ptr->sms_ctrl_ptr->dual_sys;
    storage  = caftapi_ptr->sms_ctrl_ptr->storage;
    status   = caftapi_ptr->sms_ctrl_ptr->status;
    
    alphabet_type = caftapi_ptr->sms_ctrl_ptr->alphabet_type;
    SCI_MEMSET(&caftapi_ptr->sms_ctrl_ptr->mn_sms.user_data_t, 0, sizeof(caftapi_ptr->sms_ctrl_ptr->mn_sms.user_data_t));
    MNSMS_EncodeUserDataEx(dual_sys, FALSE, alphabet_type, &caftapi_ptr->sms_ctrl_ptr->user_data_header, &caftapi_ptr->sms_ctrl_ptr->mn_user_valid_data, &(caftapi_ptr->sms_ctrl_ptr->mn_sms.user_data_t));
    
    // $$$$$$
    CAFITAPI_SetCnfTapiPtr(tapi_ptr);
    caftapi_ptr->sms_ctrl_ptr->mn_sms.dcs.alphabet_type = alphabet_type;
    result = MNSMS_WriteMoSmsEx(dual_sys, storage, status, &caftapi_ptr->sms_ctrl_ptr->mn_sms);

    if(ERR_MNSMS_NONE != result)
    {
        SCI_FREE(caftapi_ptr->sms_ctrl_ptr);
        caftapi_ptr->sms_ctrl_ptr = PNULL;
        return ITAPI_ERROR;
    }
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : save the MO SMS to the destination storage 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_WriteSms(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_SMS_STORAGE_E storage, ITAPI_SMS_STATUS_E status, ITAPI_SMS_MO_SMS_T *sms_ptr)
{
    CAFTAPI_T * caftapi_ptr = (CAFTAPI_T *)tapi_ptr;
    int32     result = ITAPI_ERROR;

    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);

    /* avoid renter */
    if(caftapi_ptr->sms_ctrl_ptr)
    {
        return result;
    }

    caftapi_ptr->sms_ctrl_ptr = (ITAPI_SMS_CTRL_T *)SCI_ALLOC_APP(sizeof(ITAPI_SMS_CTRL_T));
    SCI_MEMSET(caftapi_ptr->sms_ctrl_ptr, 0, sizeof(ITAPI_SMS_CTRL_T));
    SCI_MEMCPY(&caftapi_ptr->sms_ctrl_ptr->tapi_sms, sms_ptr, sizeof(ITAPI_SMS_MO_SMS_T));

    caftapi_ptr->sms_ctrl_ptr->dual_sys = ConvertCAFToMN_DUAL_E(dual_sys);
    caftapi_ptr->sms_ctrl_ptr->storage  = ConvertCAFToMN_STORAGE_E(storage);
    caftapi_ptr->sms_ctrl_ptr->status   = ConvertCAFToMN_SMS_STATUS_E(status);

    caftapi_ptr->sendneedsave = FALSE;
    /* sms content wchar to char */
    CAFITAPI_SMS_ConvertMsgContent(tapi_ptr);
    CAFITAPI_SMS_SplitLongMsg(tapi_ptr);
    /* exist long sms or not, ok to init header */
    CAFITAPI_SMS_InitUserDataHead(tapi_ptr);

    ConvertCAFToMN_MOSMS_T(sms_ptr, &caftapi_ptr->sms_ctrl_ptr->mn_sms);
    CAFITAPI_SMS_ProduceNumber(tapi_ptr);

    /* write the 1st sms */
    result = CAFITAPI_SMS_WriteSequenceSms(tapi_ptr, 1);

    return ITAPI_SUCCESS;
}


/*****************************************************************************/
//  Description : Set SC Address in SIM card
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_SetSmsCenterAddr(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_CALLED_NUMBER_T *tp_sc_addr_ptr)
{
    ERR_MNSMS_CODE_E result;
    MN_DUAL_SYS_E    dual_sys_mn;
    MN_CALLED_NUMBER_T  mn_tp_sc_addr;
    MMI_PARTY_NUMBER_T  party_number;
    
    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    
    if(0 == tp_sc_addr_ptr->num_len)
    {
        return ITAPI_ERROR;
    }

    dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);

    MMIAPICOM_GenPartyNumber((uint8*)tp_sc_addr_ptr->party_num , tp_sc_addr_ptr->num_len, &party_number);
    mn_tp_sc_addr.number_plan = MN_NUM_PLAN_ISDN_TELE;
    mn_tp_sc_addr.number_type = party_number.number_type;
    mn_tp_sc_addr.num_len     = (uint16)MIN( party_number.num_len, MN_MAX_ADDR_BCD_LEN);
    SCI_MEMCPY(mn_tp_sc_addr.party_num, party_number.bcd_num, mn_tp_sc_addr.num_len);

    CAFITAPI_SetCnfTapiPtr(tapi_ptr);
    result = MNSMS_SetScAddrEx(dual_sys_mn, &mn_tp_sc_addr);
    if(ERR_MNSMS_NONE != result)
    {
        return ITAPI_ERROR;
    }
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : Read SC Address which stored in SIM card
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_GetSmsCenterAddr(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_CALLED_NUMBER_T *tp_sc_addr_ptr)
{
    ERR_MNSMS_CODE_E    result;
    MN_DUAL_SYS_E       dual_sys_mn;
    MN_CALLED_NUMBER_T  mn_tp_sc_addr;

    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    
    dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);
    result = MNSMS_GetScAddrEx(dual_sys_mn, &mn_tp_sc_addr);
    if(ERR_MNSMS_NONE != result)
    {
        return ITAPI_ERROR;
    }
    if(tp_sc_addr_ptr)
    {
        ConvertMNToCAF_NUMBER_T(&mn_tp_sc_addr, tp_sc_addr_ptr);
    }
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : set the default index of parameters which save in SIM
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_GetErrCode(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys)
{
    ERR_MNSMS_CODE_E result;
    MN_DUAL_SYS_E    dual_sys_mn;
    
    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    
    dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);
    result = MNSMS_GetErrCodeEx(dual_sys_mn);
    if(ERR_MNSMS_NONE != result)
    {
        return ITAPI_ERROR;
    }
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : judge whether receive sms
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_SetSmsReceiveFunc(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, BOOLEAN  is_receive_sms)
{
    ERR_MNSMS_CODE_E result;
    MN_DUAL_SYS_E    dual_sys_mn;
    
    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    
    CAFITAPI_SetCnfTapiPtr(tapi_ptr);
    dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);
    result = MNSMS_SetSmsReceiveFuncEx(dual_sys_mn, is_receive_sms);
    if(ERR_MNSMS_NONE != result)
    {
        return ITAPI_ERROR;
    }
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : enable the msg transfer to the other process or not
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_EnableMsgPostWork(ITAPI_T * tapi_ptr, BOOLEAN  need_posttransfer)
{
    CAFTAPI_T * caftapi_ptr = (CAFTAPI_T *)tapi_ptr;

    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);

    caftapi_ptr->need_posttransfer = need_posttransfer;

    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : to handle the signal APP_MN_SMS_IND
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SMS_HandleSMSMTInd(MnSmsMtIndS *mn_cnf_ptr, ITAPI_SMSMT_IND_T *tapi_cnf_ptr)
{
    ERR_MNSMS_CODE_E         nErr = ERR_MNSMS_NONE;
    MN_SMS_USER_DATA_HEAD_T  user_data_header_t = {0};
    MN_SMS_USER_VALID_DATA_T user_valid_data_t  = {0};
    
    MNSMS_DecodeUserDataEx(mn_cnf_ptr->dual_sys, mn_cnf_ptr->sms_t.user_head_is_exist, &mn_cnf_ptr->sms_t.dcs, &mn_cnf_ptr->sms_t.user_data_t, &user_data_header_t, &user_valid_data_t);
    ConvertMNToCAF_SMS_USER_VALID_DATA_T(&user_valid_data_t, &tapi_cnf_ptr->sms_t.user_data_t, mn_cnf_ptr->sms_t.dcs.alphabet_type, MN_SMS_MT_TO_BE_READ);

    tapi_cnf_ptr->sms_t.alphabet_type = ConvertMNToCAF_SMS_ALPHABET_TYPE_E(mn_cnf_ptr->sms_t.dcs.alphabet_type);
    /* judge MMS or not */
    nErr = MNSMS_IsSupportMMSEx(mn_cnf_ptr->dual_sys, user_data_header_t.length, user_data_header_t.user_header_arr, &tapi_cnf_ptr->is_new_mms);
    //SCI_TRACE_LOW:"CAFITAPI_SMS_HandleSMSMTInd is_mms is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_2901_112_2_17_23_1_10_19,(uint8*)"d", tapi_cnf_ptr->is_new_mms);
    tapi_cnf_ptr->sms_t.dest_addr_t.num_len = MMIAPICOM_GenNetDispNumber(mn_cnf_ptr->sms_t.origin_addr_t.number_type, (uint8)MIN((ITAPI_MAX_ADDR_BCD_LEN >> 1), mn_cnf_ptr->sms_t.origin_addr_t.num_len), mn_cnf_ptr->sms_t.origin_addr_t.party_num, tapi_cnf_ptr->sms_t.dest_addr_t.party_num, (ITAPI_MAX_ADDR_BCD_LEN + 2));
    tapi_cnf_ptr->sms_t.dest_addr_t.number_type = ConvertMNToCAF_NUMBER_TYPE_E(mn_cnf_ptr->sms_t.origin_addr_t.number_type);
    ConvertMNToCAF_SMS_TIME_STAMP_T(&mn_cnf_ptr->sms_t.time_stamp_t, &tapi_cnf_ptr->sms_t.time_stamp_t);
    tapi_cnf_ptr->cause     = ConvertMNToCAF_SMS_CAUSE_E(mn_cnf_ptr->cause);
    tapi_cnf_ptr->record_id = mn_cnf_ptr->record_id;
    tapi_cnf_ptr->dual_sys  = ConvertMNToCAF_DUAL_E(mn_cnf_ptr->dual_sys);
    tapi_cnf_ptr->storage   = ConvertMNToCAF_STORAGE_E(mn_cnf_ptr->storage);

    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : Get PLMN information
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_PHONE_GetPlmnInfo(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_PLMN_INFO_T *tapi_plmn_info_ptr)
{
    MN_DUAL_SYS_E    dual_sys_mn;
    MSG_CAMP_STATUS_T plmn_info = {0};

    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);

    dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);
    MNPHONE_GetPlmnInfoEx(dual_sys_mn, &plmn_info);
    ConvertMNToCAF_PLMN_INFO_T(&plmn_info, tapi_plmn_info_ptr);

    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : Get RSSI information
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_PHONE_GetRssi(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_PHONE_SCELL_MEAS_T *tapi_cell_info_ptr)
{
    MN_DUAL_SYS_E    dual_sys_mn;
    MN_PHONE_SCELL_MEAS_T scell = {0};

    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    
    dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);
    scell = MNPHONE_GetRSSIEx(dual_sys_mn);
    ConvertMNToCAF_SCELL_MEAS_T(&scell, tapi_cell_info_ptr);
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : Get IMEI information
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_PHONE_GetImei(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, uint8 *imei_ptr, uint16 *imei_len_ptr, uint16 imei_max_len)
{
    MN_DUAL_SYS_E   dual_sys_mn;
    MN_IMEI_T       imei = {0};

    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    if(imei_ptr && imei_len_ptr)
    {
        *imei_len_ptr = 0;
        
        if(imei_max_len > (MN_MAX_IMEI_LENGTH << 1))
        {
            dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);
            if(MNNV_GetIMEIEx(dual_sys_mn, imei))
            {
                *imei_len_ptr = MN_MAX_IMEI_LENGTH << 1;
                MMIAPICOM_BcdToStr(PACKED_LSB_FIRST,imei,MN_MAX_IMEI_LENGTH<<1,(char*)imei_ptr);
                
                return ITAPI_SUCCESS;
            }
        }
    }
   
    return ITAPI_ERROR;
}

/*****************************************************************************/
//  Description : Get dual sim status
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SIM_GetDualSimStatus(ITAPI_T * tapi_ptr, BOOLEAN *is_sim1_ok_ptr, BOOLEAN *is_sim2_ok_ptr)
{
    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);

    *is_sim1_ok_ptr = MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1);

#ifdef MMI_MULTI_SIM_SYS_DUAL
    *is_sim2_ok_ptr = MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_2);
#endif
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : Get IMSI information
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note: 
/*****************************************************************************/
LOCAL int32 CAFITAPI_SIM_GetImsi(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_IMSI_T *imsi_ptr)
{
    MN_DUAL_SYS_E   dual_sys_mn;
    MN_IMSI_T       imsi = {0};
    
    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    
    dual_sys_mn = ConvertCAFToMN_DUAL_E(dual_sys);
    imsi = MNSIM_GetImsiEx(dual_sys_mn);
    ConvertMNToCAF_IMSI_T(&imsi, imsi_ptr);
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : APP task start a MO call.only one call is established at one 
//                time. 
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_CALL_StartCall(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, ITAPI_CALL_TYPE_E call_type, ITAPI_CALLED_NUMBER_T *called_num_ptr, ITAPI_CALL_CLIR_E clir_type)
{
    MN_RETURN_RESULT_E  result;
    MN_CALLED_NUMBER_T  dest_addr;
    MN_DUAL_SYS_E       dual_sys_mn;
    MN_CALL_TYPE_E      mn_call_type;
    MN_CALL_CLIR_E      mn_clir_type;
    MMI_PARTY_NUMBER_T  party_number;

    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);

    CAFITAPI_SetCnfTapiPtr(tapi_ptr);
    dual_sys_mn  = ConvertCAFToMN_DUAL_E(dual_sys);
    mn_call_type = ConvertCAFToMN_CALL_TYPE_E(call_type);
    mn_clir_type = ConvertCAFToMN_CALL_CLIR_E(clir_type);
    MMIAPICOM_GenPartyNumber((uint8*)called_num_ptr->party_num , called_num_ptr->num_len, &party_number);
    dest_addr.number_plan = MN_NUM_PLAN_ISDN_TELE;
    dest_addr.number_type = party_number.number_type;
    dest_addr.num_len     = (uint16)MIN( party_number.num_len, MN_MAX_ADDR_BCD_LEN);
    SCI_MEMCPY(dest_addr.party_num, party_number.bcd_num, dest_addr.num_len);

    result = MNCALL_StartCallEx(dual_sys_mn, mn_call_type, &dest_addr, PNULL, mn_clir_type, PNULL);
    if(MN_RETURN_FAILURE == result)
    {
        return ITAPI_ERROR;
    }
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : APP task accept the incoming call
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_CALL_ConnectCall(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, uint8 call_id)
{
    MN_RETURN_RESULT_E  result;
    MN_DUAL_SYS_E       dual_sys_mn;
    
    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    
    CAFITAPI_SetCnfTapiPtr(tapi_ptr);
    dual_sys_mn  = ConvertCAFToMN_DUAL_E(dual_sys);
    result = MNCALL_ConnectCallEx(dual_sys_mn, call_id, PNULL);
    if(MN_RETURN_FAILURE == result)
    {
        return ITAPI_ERROR;
    }

    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description :  APP task disconnect the call indicated by call_id
//  Global resource dependence : none
//  Author: Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL int32 CAFITAPI_CALL_DisConnectCall(ITAPI_T * tapi_ptr, ITAPI_DUAL_E dual_sys, uint8 call_id)
{
    MN_RETURN_RESULT_E  result;
    MN_DUAL_SYS_E       dual_sys_mn;
    
    SCI_ASSERT(tapi_ptr);
    SCI_ASSERT(((CAFTAPI_T *)tapi_ptr)->ref_num);
    
    CAFITAPI_SetCnfTapiPtr(tapi_ptr);
    dual_sys_mn  = ConvertCAFToMN_DUAL_E(dual_sys);
    result = MNCALL_DisconnectCallEx(dual_sys_mn, call_id, MN_CAUSE_NORMAL_CLEARING, PNULL);
    if(MN_RETURN_FAILURE == result)
    {
        return ITAPI_ERROR;
    }
    
    return ITAPI_SUCCESS;
}

/*****************************************************************************/
//  Description : treat the message from l4
//  Global resource dependence : 
//  Author:Wei.zhou
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CAFITAPI_HandlePrePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    CAF_HANDLE_T dest_handle = PNULL;

    //check the param
    //SCI_TRACE_LOW:"caftapi.c:enter function CAFITAPI_HandlePrePsMsg(), msg_id = %lx"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_3110_112_2_17_23_1_11_20,(uint8*)"d", msg_id);
    SCI_PASSERT(PNULL != app_ptr, ("caftapi.c: CAFITAPI_HandlePrePsMsg() app_ptr == PNULL"));
    
    switch( msg_id )
    {
        case APP_MN_SIM_READY_IND:
            //SCI_TRACE_LOW:"APP_MN_SIM_READY_IND %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_3116_112_2_17_23_1_11_21,(uint8*)"d", s_gprs_state);
            s_gprs_state = ITAPI_STATE_SIMRDY;
            break;
        case APP_MN_SIM_NOT_READY_IND:
            //SCI_TRACE_LOW:"APP_MN_SIM_NOT_READY_IND %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_3120_112_2_17_23_1_11_22,(uint8*)"d", s_gprs_state);
            if(s_gprs_state != ITAPI_STATE_SIMRDY)
            {
                s_gprs_state = ITAPI_STATE_NOSIM;
            }
            break;
        case APP_MN_SMS_READY_IND:
            break;
        case APP_MN_READ_SMS_CNF:
            {
                MnReadSmsCnfS *mn_cnf_ptr = (MnReadSmsCnfS *)param;
                ITAPI_READSMS_CNF_T *tapi_cnf_ptr = PNULL;

                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_SMS_READ_SMS_CNF);
                if(CAFITAPI_IsValidHandle(dest_handle))
                {
                    /* allocate temperate structure to the handle process */
                    tapi_cnf_ptr = (ITAPI_READSMS_CNF_T *)SCI_ALLOC_APP(sizeof(ITAPI_READSMS_CNF_T));
                    SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_READSMS_CNF_T));
                    if(ITAPI_SUCCESS == ConvertMNToCAF_READSMS_CNF_T(mn_cnf_ptr, tapi_cnf_ptr))
                    {
                        ISHELL_SendMessage(dest_handle, MSG_SMS_READ_SMS_CNF, (void *)tapi_cnf_ptr);
                    }
                    else
                    {
                        ISHELL_SendMessage(dest_handle, MSG_SMS_READ_SMS_CNF, NULL);
                    }
                    SCI_FREE(tapi_cnf_ptr);
                    result = MMI_RESULT_TRUE;
                }
            }
            break;
        case APP_MN_UPDATE_SMS_STATE_CNF:
            {
                MnUpdateSmsStateCnfS *mn_cnf_ptr = (MnUpdateSmsStateCnfS *)param;
                ITAPI_UPDATESMS_CNF_T *tapi_cnf_ptr = PNULL;
                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_SMS_UPDATE_SMS_STATE_CNF);
                if(CAFITAPI_IsValidHandle(dest_handle))
                {
                    tapi_cnf_ptr = (ITAPI_UPDATESMS_CNF_T *)SCI_ALLOC_APP(sizeof(ITAPI_UPDATESMS_CNF_T));
                    SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_UPDATESMS_CNF_T));
                    ConvertMNToCAF_UPDATESMS_CNF_T(mn_cnf_ptr, tapi_cnf_ptr);
                    ISHELL_SendMessage(dest_handle, MSG_SMS_UPDATE_SMS_STATE_CNF, (void *)tapi_cnf_ptr);
                    SCI_FREE(tapi_cnf_ptr);
                    result = MMI_RESULT_TRUE;
                }
            }
            break;
        case APP_MN_SEND_SMS_CNF:
            {
                uint32        seq_num;
                MnSendSmsCnfS *mn_cnf_ptr = (MnSendSmsCnfS *)param;
                CAFTAPI_T     *caftapi_ptr = CAFITAPI_GetTAPIptr(app_ptr);

                //SCI_TRACE_LOW:"APP_MN_SEND_SMS_CNF send cause =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_3174_112_2_17_23_1_11_23,(uint8*)"d", mn_cnf_ptr->cause);
                if((PNULL != caftapi_ptr) && (PNULL != caftapi_ptr->sms_ctrl_ptr))
                {
                    if(MN_SMS_OPERATE_SUCCESS == mn_cnf_ptr->cause)
                    {
                        seq_num = (uint32)CAFITAPI_SMS_GetCurSeqNum((ITAPI_T *)caftapi_ptr);
                        if(CAFITAPI_SMS_IsExistNextSMS((ITAPI_T *)caftapi_ptr))
                        {
                            //SCI_TRACE_LOW:"APP_MN_SEND_SMS_CNF exist next sms"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_3182_112_2_17_23_1_11_24,(uint8*)"");
                            CAFITAPI_SMS_SendSequenceSms((ITAPI_T *)caftapi_ptr, seq_num + 1);
                        }
                        else
                        {
                            ITAPI_SENDSMS_CNF_T *tapi_cnf_ptr = PNULL;
                            
                            dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_SMS_SEND_SMS_CNF);
                            if(CAFITAPI_IsValidHandle(dest_handle))
                            {
                                tapi_cnf_ptr = (ITAPI_SENDSMS_CNF_T *)SCI_ALLOC_APP(sizeof(ITAPI_SENDSMS_CNF_T));
                                SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_SENDSMS_CNF_T));
                                ConvertMNToCAF_SENDSMS_CNF_T(mn_cnf_ptr, tapi_cnf_ptr);
                                ISHELL_SendMessage(dest_handle, MSG_SMS_SEND_SMS_CNF, (void *)tapi_cnf_ptr);
                                SCI_FREE(tapi_cnf_ptr);
                            }
                            SCI_FREE(caftapi_ptr->sms_ctrl_ptr);
                            caftapi_ptr->sms_ctrl_ptr = PNULL;
                        }
                    }
                    else
                    {
                        SCI_FREE(caftapi_ptr->sms_ctrl_ptr);
                        caftapi_ptr->sms_ctrl_ptr = PNULL;
                    }
                    result = MMI_RESULT_TRUE;
                }
            }
            break;
        case APP_MN_WRITE_SMS_CNF:
            {
                uint32        seq_num;
                MnWriteSmsCnfS *mn_cnf_ptr = (MnWriteSmsCnfS *)param;
                CAFTAPI_T     *caftapi_ptr = CAFITAPI_GetTAPIptr(app_ptr);

                if(caftapi_ptr)
                {
                    /* sending sms need save */
                    if(caftapi_ptr->sendneedsave)
                    {
                        result = MMI_RESULT_TRUE;
                        break;
                    }

                    if(caftapi_ptr->sms_ctrl_ptr)
                    {
                        if(MN_SMS_OPERATE_SUCCESS == mn_cnf_ptr->cause)
                        {
                            caftapi_ptr->sms_ctrl_ptr->record_id_arr[caftapi_ptr->sms_ctrl_ptr->save_num++] = mn_cnf_ptr->record_id;
                            seq_num = (uint32)CAFITAPI_SMS_GetCurSeqNum((ITAPI_T *)caftapi_ptr);
                            if(CAFITAPI_SMS_IsExistNextSMS((ITAPI_T *)caftapi_ptr))
                            {
                                //SCI_TRACE_LOW:"APP_MN_WRITE_SMS_CNF exist next sms"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_3234_112_2_17_23_1_11_25,(uint8*)"");
                                CAFITAPI_SMS_WriteSequenceSms((ITAPI_T *)caftapi_ptr, seq_num + 1);
                            }
                            else
                            {
                                ITAPI_WRITESMS_CNF_T *tapi_cnf_ptr = PNULL;
                                
                                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_SMS_WRITE_SMS_CNF);
                                if(CAFITAPI_IsValidHandle(dest_handle))
                                {
                                    tapi_cnf_ptr = (ITAPI_WRITESMS_CNF_T *)SCI_ALLOC_APP(sizeof(ITAPI_WRITESMS_CNF_T));
                                    SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_WRITESMS_CNF_T));
                                    ConvertMNToCAF_WRITESMS_CNF_T(mn_cnf_ptr, tapi_cnf_ptr);
                                    tapi_cnf_ptr->save_num = caftapi_ptr->sms_ctrl_ptr->save_num;
                                    SCI_MEMCPY(&tapi_cnf_ptr->record_id_arr, &caftapi_ptr->sms_ctrl_ptr->record_id_arr, sizeof(tapi_cnf_ptr->record_id_arr));
                                    ISHELL_SendMessage(dest_handle, MSG_SMS_WRITE_SMS_CNF, (void *)tapi_cnf_ptr);
                                    SCI_FREE(tapi_cnf_ptr);
                                }
                            }
                        }
                        else
                        {
                            SCI_FREE(caftapi_ptr->sms_ctrl_ptr);
                            caftapi_ptr->sms_ctrl_ptr = PNULL;
                        }
                        result = MMI_RESULT_TRUE;
                    }
                }
            }
            break;
        case APP_MN_WRITE_SMS_PARAM_CNF:
            {
                MnWriteSmsParamCnfS *mn_cnf_ptr = (MnWriteSmsParamCnfS *)param;
                ITAPI_WRITESMSPARAM_CNF_T *tapi_cnf_ptr = PNULL;

                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_SMS_WRITE_SMS_PARAM_CNF);
                if(CAFITAPI_IsValidHandle(dest_handle))
                {
                    tapi_cnf_ptr = (ITAPI_WRITESMSPARAM_CNF_T *)SCI_ALLOC_APP(sizeof(ITAPI_WRITESMSPARAM_CNF_T));
                    SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_WRITESMSPARAM_CNF_T));
                    ConvertMNToCAF_WRITESMSPARAM_CNF_T(mn_cnf_ptr, tapi_cnf_ptr);
                    ISHELL_SendMessage(dest_handle, MSG_SMS_WRITE_SMS_PARAM_CNF, (void *)tapi_cnf_ptr);
                    SCI_FREE(tapi_cnf_ptr);
                    result = MMI_RESULT_TRUE;
                }
            }
            break;
        case APP_MN_SET_SMS_RECEIVE_FUNC_CNF:
            {
                MnSetSmsReceiveFuncCnf *mn_cnf_ptr = (MnSetSmsReceiveFuncCnf *)param;
                ITAPI_SETSMSRECEIVEFUN_CNF_T *tapi_cnf_ptr = PNULL;
                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_SMS_SET_SMS_RECEIVE_FUNC_CNF);
                if(CAFITAPI_IsValidHandle(dest_handle))
                {
                    tapi_cnf_ptr = (ITAPI_SETSMSRECEIVEFUN_CNF_T *)SCI_ALLOC_APP(sizeof(ITAPI_SETSMSRECEIVEFUN_CNF_T));
                    SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_SETSMSRECEIVEFUN_CNF_T));
                    ConvertMNToCAF_SETSMSRECEIVEFUN_CNF_T(mn_cnf_ptr, tapi_cnf_ptr);
                    ISHELL_SendMessage(dest_handle, MSG_SMS_SET_SMS_RECEIVE_FUNC_CNF, (void *)tapi_cnf_ptr);
                    SCI_FREE(tapi_cnf_ptr);
                    result = MMI_RESULT_TRUE;
                }
            }
            break;
        case APP_MN_SMS_MEM_FULL_IND:
            {
                MnMemFullSmsInd *mn_cnf_ptr = (MnMemFullSmsInd *)param;
                ITAPI_MEMFULLSMS_IND_T *tapi_cnf_ptr = PNULL;
                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_SMS_MEM_FULL_IND);
                if(CAFITAPI_IsValidHandle(dest_handle))
                {
                    tapi_cnf_ptr = (ITAPI_MEMFULLSMS_IND_T *)SCI_ALLOC_APP(sizeof(ITAPI_MEMFULLSMS_IND_T));
                    SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_MEMFULLSMS_IND_T));
                    ConvertMNToCAF_MEMFULLSMS_IND_T(mn_cnf_ptr, tapi_cnf_ptr);
                    ISHELL_SendMessage(dest_handle, MSG_SMS_MEM_FULL_IND, (void *)tapi_cnf_ptr);
                    SCI_FREE(tapi_cnf_ptr);
                    result = MMI_RESULT_TRUE;
                }
            }
            break;
        case APP_MN_SMS_STATUS_REPORT_IND:
            {
                MnSmsReportIndS *mn_cnf_ptr = (MnSmsReportIndS *)param;
                ITAPI_SMSREPORT_IND_T *tapi_cnf_ptr = PNULL;
                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_SMS_STATUS_REPORT_IND);
                if(CAFITAPI_IsValidHandle(dest_handle))
                {
                    tapi_cnf_ptr = (ITAPI_SMSREPORT_IND_T *)SCI_ALLOC_APP(sizeof(ITAPI_SMSREPORT_IND_T));
                    SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_SMSREPORT_IND_T));
                    ConvertMNToCAF_SMSREPORT_IND_T(mn_cnf_ptr, tapi_cnf_ptr);
                    ISHELL_SendMessage(dest_handle, MSG_SMS_STATUS_REPORT_IND, (void *)tapi_cnf_ptr);
                    SCI_FREE(tapi_cnf_ptr);
                    result = MMI_RESULT_TRUE;
                }
            }
            break;
        case APP_MN_SMS_IND:
            {
                CAFTAPI_T  *caftapi_ptr = CAFITAPI_GetTAPIptr(app_ptr);
                MnSmsMtIndS *mn_cnf_ptr = (MnSmsMtIndS *)param;
                ITAPI_SMSMT_IND_T *tapi_cnf_ptr = PNULL;

                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_SMS_RECEIVE_IND);
                if(CAFITAPI_IsValidHandle(dest_handle))
                {
                    tapi_cnf_ptr = (ITAPI_SMSMT_IND_T *)SCI_ALLOC_APP(sizeof(ITAPI_SMSMT_IND_T));
                    SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_SMSMT_IND_T));
                    CAFITAPI_SMS_HandleSMSMTInd(mn_cnf_ptr, tapi_cnf_ptr);
                    ISHELL_SendMessage(dest_handle, MSG_SMS_RECEIVE_IND, (void *)tapi_cnf_ptr);
                    SCI_FREE(tapi_cnf_ptr);
                    if(!caftapi_ptr->need_posttransfer)
                    {
                        result = MMI_RESULT_TRUE;
                    }
                }
            }
            break;
             /*  mo 2, mt 3   */
        case APP_MN_PROGRESS_IND:   
            /*  mo 3, mt 4   */
        case APP_MN_SYNC_IND:
            /*  mt 2   */
        case APP_MN_ALERTING_IND:
        case APP_MN_CONNECTING_IND:
        case APP_MN_DISCONNECTING_IND:
            {
                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_CALL_SETUP_IND);
                if(CAFITAPI_IsValidHandle(dest_handle))
                {
                    result = MMI_RESULT_TRUE;
                }
                else
                {
                    dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_CALL_START_IND);
                    if(CAFITAPI_IsValidHandle(dest_handle))
                    {
                        result = MMI_RESULT_TRUE;
                    }
                }
            }
            break;
            /*  mt 1   */
        case APP_MN_SETUP_IND:
            {
                MnCcSetupIndS *mn_cnf_ptr = (MnCcSetupIndS *)param;
                ITAPI_CALL_SETUP_IND_T   *tapi_cnf_ptr = PNULL;
                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_CALL_SETUP_IND);
                if(CAFITAPI_IsValidHandle(dest_handle))
                {
                    /* MT message */
                    tapi_cnf_ptr = (ITAPI_CALL_SETUP_IND_T *)SCI_ALLOC_APP(sizeof(ITAPI_CALL_SETUP_IND_T));
                    SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_CALL_SETUP_IND_T));
                    ConvertMNToCAF_CALL_IND_T(mn_cnf_ptr, tapi_cnf_ptr);
                    ISHELL_SendMessage(dest_handle, MSG_CALL_SETUP_IND, (void *)tapi_cnf_ptr);
                    SCI_FREE(tapi_cnf_ptr);
                    result = MMI_RESULT_TRUE;
                }
            }
            break;
            /*  mo 4(answer, from mt)   */
        case APP_MN_SETUP_COMPLETE_CNF:
            {
                MnCcSetupCompleteCnfS *mn_cnf_ptr = (MnCcSetupCompleteCnfS *)param;
                ITAPI_CALL_SETUP_COMPLETE_CNF_T *tapi_cnf_ptr = PNULL;
                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_CALL_SETUP_COMPLETE_CNF);
                if(CAFITAPI_IsValidHandle(dest_handle))
                {
                    tapi_cnf_ptr = (ITAPI_CALL_SETUP_COMPLETE_CNF_T *)SCI_ALLOC_APP(sizeof(ITAPI_CALL_SETUP_COMPLETE_CNF_T));
                    SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_CALL_SETUP_COMPLETE_CNF_T));
                    ConvertMNToCAF_CALL_SETUP_COMPLETE_CNF_T(mn_cnf_ptr, tapi_cnf_ptr);
                    ISHELL_SendMessage(dest_handle, MSG_CALL_SETUP_COMPLETE_CNF, (void *)tapi_cnf_ptr);
                    SCI_FREE(tapi_cnf_ptr);
                    result = MMI_RESULT_TRUE;
                }
            }
            break;
            /*  mo 1   */
        case APP_MN_CALL_START_IND:
            {
                MnCcCallStartIndS *mn_cnf_ptr = (MnCcCallStartIndS *)param;
                ITAPI_CALL_START_IND_T *tapi_cnf_ptr = PNULL;
                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_CALL_START_IND);
                if(CAFITAPI_IsValidHandle(dest_handle))
                {
                    tapi_cnf_ptr = (ITAPI_CALL_START_IND_T *)SCI_ALLOC_APP(sizeof(ITAPI_CALL_START_IND_T));
                    SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_CALL_START_IND_T));
                    ConvertMNToCAF_CALL_START_IND_T(mn_cnf_ptr, tapi_cnf_ptr);
                    ISHELL_SendMessage(dest_handle, MSG_CALL_START_IND, (void *)tapi_cnf_ptr);
                    SCI_FREE(tapi_cnf_ptr);
                    result = MMI_RESULT_TRUE;
                }
            }
            break;
            /* connect confirm, from MNCALL_ConnectCallEx ??? */
        case APP_MN_SETUP_COMPLETE_IND:
            {
                MnCcSetupCompleteIndS *mn_cnf_ptr = (MnCcSetupCompleteIndS *)param;
                ITAPI_CALL_SETUP_COMPLETE_IND_T *tapi_cnf_ptr = PNULL;
                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_CALL_SETUP_COMPLETE_IND);
                if(CAFITAPI_IsValidHandle(dest_handle))
                {
                    tapi_cnf_ptr = (ITAPI_CALL_SETUP_COMPLETE_IND_T *)SCI_ALLOC_APP(sizeof(ITAPI_CALL_SETUP_COMPLETE_IND_T));
                    SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_CALL_SETUP_COMPLETE_IND_T));
                    ConvertMNToCAF_CALL_SETUP_COMPLETE_IND_T(mn_cnf_ptr, tapi_cnf_ptr);
                    ISHELL_SendMessage(dest_handle, MSG_CALL_SETUP_COMPLETE_IND, (void *)tapi_cnf_ptr);
                    SCI_FREE(tapi_cnf_ptr);
                    result = MMI_RESULT_TRUE;
                }
            }
            break;
            /* mo 4(disconnect), mt 5  */
        case APP_MN_CALL_DISCONNECTED_IND:
            {
                MnCcCallDisconnectdIndS *mn_cnf_ptr = (MnCcCallDisconnectdIndS *)param;
                ITAPI_CALL_DISCONNECTED_IND_T *tapi_cnf_ptr = PNULL;
                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_CALL_DISCONNECTED_IND);
                if(CAFITAPI_IsValidHandle(dest_handle))
                {
                    tapi_cnf_ptr = (ITAPI_CALL_DISCONNECTED_IND_T *)SCI_ALLOC_APP(sizeof(ITAPI_CALL_DISCONNECTED_IND_T));
                    SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_CALL_DISCONNECTED_IND_T));
                    ConvertMNToCAF_CALL_DISCONNECTED_IND_T(mn_cnf_ptr, tapi_cnf_ptr);
                    ISHELL_SendMessage(dest_handle, MSG_CALL_DISCONNECTED_IND, (void *)tapi_cnf_ptr);
                    SCI_FREE(tapi_cnf_ptr);
                    result = MMI_RESULT_TRUE;
                }
            }
            break;
        case APP_MN_CALL_ERR_IND:
            {
                MnCcCallErrIndS  *mn_cnf_ptr = (MnCcCallErrIndS *)param;
                ITAPI_CALL_ERR_IND_T *tapi_cnf_ptr = PNULL;
                dest_handle = CAFITAPI_GetHandleByMsg(app_ptr, MSG_CALL_ERR_IND);
                //SCI_TRACE_LOW:"msg APP_MN_CALL_ERR_IND, err is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,CAFTAPI_3465_112_2_17_23_1_12_26,(uint8*)"d", mn_cnf_ptr->error_cause);
                if(CAFITAPI_IsValidHandle(dest_handle))
                {
                    tapi_cnf_ptr = (ITAPI_CALL_ERR_IND_T *)SCI_ALLOC_APP(sizeof(ITAPI_CALL_ERR_IND_T));
                    SCI_MEMSET(tapi_cnf_ptr, 0, sizeof(ITAPI_CALL_ERR_IND_T));
                    ConvertMNToCAF_CALL_ERR_IND_T(mn_cnf_ptr, tapi_cnf_ptr);
                    ISHELL_SendMessage(dest_handle, MSG_CALL_ERR_IND, (void *)tapi_cnf_ptr);
                    SCI_FREE(tapi_cnf_ptr);
                    result = MMI_RESULT_TRUE;
                }
            }
            break;
        default:
            break;
    }
    
    return result;
}
#endif