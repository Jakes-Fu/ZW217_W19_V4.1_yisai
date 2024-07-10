/******************************************************************************
 ** File Name:      atc_plus_gprs.h                                           *
 ** Author:         Ivan.Yin                                                  *
 ** DATE:           06/10/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains functions definitions about GPRS ATC.  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/10/2002     Steven.Yao       Created.                                  *
 ******************************************************************************/

#ifndef _ATC_PLUS_GPRS_H_
#define  _ATC_PLUS_GPRS_H_


/*******************************************************************************
 **                        ATC Header Files                                    *
 ******************************************************************************/
#include "atc.h"
#include "mn_type.h"
#include "atc_common.h"
#include "atc_tcp_if.h"

/*******************************************************************************
 **                         Common NUM define                                  *
 ******************************************************************************/
#define MAX_PDP_ADDRESS_LEN 20
//Hyman we set at pdp max num as MN:MN_GPRS_MAX_PDP_ID_NUM
#define MAX_PDP_CONTEXT_COUNT 15
#define ERR_INVALID_INDEX 21
#define MAX_PCO_LEN 253
#define MAX_PDP_TYPE_LEN 7
#define MAX_APN_LEN 100
#define MAX_PAP_USER_LEN 64
#define MAX_PAP_PASSWD_LEN 64

/*the expected event check id used for deactivating all pdp contexts*/
#define ATC_CHECKID_FOR_DEACTIVATE_ALL_PDP      0xff

/*******************************************************************************
 **                         Type declaration                              *
 ******************************************************************************/


//record the PDP context
typedef struct
{
    BOOLEAN  param_used[7];
    int      pdp_id;                    // a numeric parameter which specifies a particular PDP context definition.
    uint8    pdp_type[MAX_PDP_TYPE_LEN];//a string parameter which specifies the type of packet data protocol
    uint8    apn[MAX_APN_LEN+1];        // a string parameter which is a logical name that is used to select the GGSN or the external packet data network
    MN_GPRS_PDP_ADDR_T pdp_address;     //a string parameter that identifies the MT in the address space applicable to the PDP
    int      d_comp;                    //a numeric parameter that controls PDP data compression(0,1)
    int      h_comp;                    //a numeric parameter that controls PDP header compression(0,1)
    uint8    pco[MAX_PCO_LEN+1];

} ATC_SET_PDP_CONTEXT_REQ_T;

//record the secondary PDP context
typedef struct
{
    BOOLEAN  param_used[4];
    int      cid;                    // a numeric parameter which specifies a particular PDP context definition.
    int      p_cid;               //a numeric parameter which specifies a particular PDP context definition which has been specified by use of the +CGDCONT command
    int      d_comp;                    //a numeric parameter that controls PDP data compression(0,1)
    int      h_comp;                    //a numeric parameter that controls PDP header compression(0,1)

} ATC_SET_SECONDARY_PDP_CONTEXT_REQ_T;

//record the qos info
typedef struct
{
    BOOLEAN param_used[6];
    int     pdp_id;
    uint16  precedence;
    uint16  delay;
    uint16  reliability;
    uint16  peak_throughput;
    uint16  mean_throughput;
} ATC_SET_QOS_REQ_T;

typedef struct _ATC_GPRS_EV_REPORT_DETAIL_T
{
    MN_GPRS_EVENT_REPROT_INFO_T info;
    uint8   buf[MAX_ATC_RSP_LEN];
    uint32  len;
} ATC_GPRS_EV_REPORT_DETAIL_T;
//record the qos info
/*
typedef struct
{
    BOOLEAN  param_used[6];
    int pdp_id;// a numeric parameter which specifies a particular PDP context definition
    uint16 precedence;
    uint16 delay;
    uint16 reliability;
    uint16 peak_throughput;
    uint16 mean_throughput;

} ATC_SET_MIN_QOS_REQ_T;
*/


typedef struct ATC_NEGOTIATED_QOS_REQ_T_s
{
    BOOLEAN is_all;
    uint8   num;
    uint32  pdp_id[MN_GPRS_MAX_PDP_CONTEXT_COUNT];
} ATC_NEGOTIATED_QOS_REQ_T;

typedef struct
{
    BOOLEAN is_all;
    uint32  pdp_id[MN_GPRS_MAX_PDP_CONTEXT_COUNT]; //xgxu convert int to uint32
} ATC_PDP_ACT_REQ_T;

typedef struct
{
    BOOLEAN is_all;
    uint32  pdp_id[MN_GPRS_MAX_PDP_CONTEXT_COUNT];//xgxu convert int to uint32
} ATC_SHOW_PDP_ADDR_REQ_T;

typedef struct
{
    int    l2p;
    uint32 pdp_id[MN_GPRS_MAX_PDP_CONTEXT_COUNT];//xgxu convert int to uint32
} ATC_ENTER_DATA_STATE_REQ_T;

typedef struct
{
    int     response;
    int     l2p;
    BOOLEAN is_all;
    uint32  pdp_id;                       //xgxu convert int to uint32
} ATC_MANUAL_RESP_NET_T;

typedef struct
{
    int resp_mode;
} ATC_AUTO_RESP_NET_T;

typedef struct
{
    uint8 *ms_class;
} ATC_SET_MS_CLASS_REQ_T;

typedef struct
{
    int service_type;
} ATC_SELECT_SMS_SERVICE_T;

typedef struct
{
    int status_mode;
} ATC_NETWORK_REG_STATUS_T;

typedef struct
{
    int mode;
    int buffer;
} ATC_SET_EVENT_REPORT_T;

typedef struct
{
    uint32       cgauth_cid;
    MN_PCO_AUTH_TYPE_E       cgauth_type;
    uint32     cgauth_user[MAX_PAP_USER_LEN +1];
    uint32     cgauth_passwd[MAX_PAP_PASSWD_LEN +1];
}ATC_CGAUTH_CONF_NV_T;


/*******************************************************************************
 **                         Function declaration                               *
 ******************************************************************************/
/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGDCONT( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Shijun.cui
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGPCO( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

/*****************************************************************************/
//  Description : This function initializes the nv static variables for auth param
//  Global resource dependence :
//  Author: vicky.wang      
//  Note:
/*****************************************************************************/
void ATC_InitAUTHConfig(void);

/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: 
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGAUTH( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);
/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGQREQ( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);
/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGQMIN( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);
/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGATT( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);
/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGACT( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);
/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGPADDR( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);
/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGDATA( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);
/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGAUTO( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

#ifndef ATC_SAT_ENABLE
/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGCLASS( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGANS( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGSMS(// return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+CGTFT, set Traffic Flow Template
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  Wanghui
//  Note :
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGTFT( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+CGEQNEG, get negoiated QoS
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  Pizer.Fan
//  Note :
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGEQNEG( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessSSST( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION: Spreadtrum send gprs for FTA test
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessSGPRSDATA( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);


/******************************************************************************/
//  DESCRIPTION: Spreadtrum auto attach or not
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessSAUTOATT( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

#ifdef _SUPPORT_GPRS_
/*************************************************************************/
/*    Description: send dummy data for AT+SGPRSDATA  */
/*    Global resource dependence : */
/*    Author: chun jiang*/
/*    Note:  Modified by Elvis.xu*/
/*************************************************************************/
extern void ATC_SendDummyDataReqEx(uint16 nsapi);

/*************************************************************************/
/*    Description: send dummy data for AT+SGPRSDATA  */
/*    Global resource dependence : */
/*    Author: Elvis.xu*/
/*    Note:  */
/*************************************************************************/
extern void ATC_FreeDummyData(uint16 nsapi);
#endif
#endif

/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGREG(// return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);
/******************************************************************************/
//  DESCRIPTION:
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGEREP( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

extern ATC_STATUS ATC_ProcessD( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);
/******************************************************************************/
//  DESCRIPTION:
//           data configure about gprs data service
//  Global     : atc_config_ptr
//  AUTHOR: xu.xiaogang
//  Note : None
//  History    :
//    xu.xiaogang 2004-1-12 add actual process
/******************************************************************************/
extern ATC_STATUS ATC_ProcessSDATACONF( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);
/******************************************************************************/
//  DESCRIPTION:
//           open or close the socket
//  Global     : atc_config_ptr
//  AUTHOR: xu.xiaogang
//  Note : None
//  History    :
//    xu.xiaogang 2004-1-12 add actual process
/******************************************************************************/
extern ATC_STATUS ATC_ProcessSDATASTART( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);
/******************************************************************************/
//  DESCRIPTION:
//           send data use opened socket
//  Global     : atc_config_ptr
//  AUTHOR: xu.xiaogang
//  Note : None
//  History    :
//    xu.xiaogang 2004-1-12 add actual process
/******************************************************************************/
extern ATC_STATUS ATC_ProcessSDATASEND( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION: Spreadtrum attach or detach
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Ivan.Yin
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessSATT( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+CGDSCONT, set secondary pdp context
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  Wanghui
//  Note :
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGDSCONT( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+CGEQREQ, set 3G Qos
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  Wanghui
//  Note :
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGEQREQ( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+CGEQMIN, set 3G min Qos
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  Wanghui
//  Note :
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGEQMIN( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION: This function handles command AT+PSRABR, set Traffic Flow Template
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  Wanghui
//  Note :
/******************************************************************************/
extern ATC_STATUS ATC_ProcessPSRABR( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
);


/******************************************************************************/
//  DESCRIPTION: This function handles APP_MN_DEACTIVATE_PDP_CONTEXT_IND
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  minqian.qian
//  Note :
/******************************************************************************/
extern void ATC_ProcessDeactivatePDPContextIND(
    ATC_CONFIG_T                        *atc_config_ptr,            // point to the ATC_CONFIG_T struct.
    APP_MN_GPRS_DEACTIVATE_IND_T    *sig_ptr
);

/******************************************************************************/
//  DESCRIPTION: This function handles PDP context deactivate cnf
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  minqian.qian
//  Note :
/******************************************************************************/
extern void ATC_ProcessDeactivatePDPContextCNF(
    ATC_CONFIG_T                        *atc_config_ptr,            // point to the ATC_CONFIG_T struct.
    APP_MN_GPRS_DEACTIVATE_CNF_T    *sig_ptr,
    BOOLEAN                         recv_event_result
);

/******************************************************************************/
//  DESCRIPTION: This function handles APP_MN_ACTIVATE_PDP_CONTEXT_CNF
//  Global resource dependence : atc_config_ptr
//  AUTHOR:  minqian.qian
//  Note :
/******************************************************************************/
extern ATC_STATUS ATC_ProcessActivatePDPContextCNF(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_GPRS_EXT_T         *sig_ptr,
    BOOLEAN                                 recv_event_result
);

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+SIPCONFIG, get pdp's IP configure info
//  Global resource dependence : atc_config_ptr
//  AUTHOR: Elvis.xu
//  Note :
/******************************************************************************/
extern ATC_STATUS ATC_ProcessSIPCONFIG( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+SGFD, force grps detach
//  Global resource dependence : atc_config_ptr
//  AUTHOR: 
//  Note :
/******************************************************************************/
extern ATC_STATUS ATC_ProcessSGFD( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);

/*****************************************************************************/
//  Description : This function handle the APP_MN_GPRS_ATTACH_RESULT signal
//  Global resource dependence : none
//  Author:       Elvis.Xu
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessGprsAttachResult(
    ATC_CONFIG_T            *atc_config_ptr,// point to the ATC_CONFIG_T struct.
    APP_MN_GPRS_ATTACH_RESULT_T *sig_ptr        // point to the APP_MN_GPRS_ATTACH_RESULT signal
);

/*****************************************************************************/
//  Description : This function handle the APP_MN_GPRS_DETACH_RESULT signal
//  Global resource dependence : none
//  Author:       Elvis.Xu
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessGprsDetachResult(
    ATC_CONFIG_T            *atc_config_ptr,// point to the ATC_CONFIG_T struct.
    APP_MN_GPRS_DETACH_RESULT_T *sig_ptr        // point to the APP_MN_GPRS_DETACH_RESULT signal
);

#ifdef HUAWEI_6500_MODEM
/*****************************************************************************/
//  Description : This function handle the ATC_PPP_ACTIVE_PDP_REQ signal
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessPPPActReq(
    ATC_CONFIG_T            *atc_config_ptr,// point to the ATC_CONFIG_T struct.
    PPP_ACTIVE_PDP_CFG_T *sig_ptr        // point to the ATC_PPP_ACTIVE_PDP_REQ signal
);
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:       wuding.yang
//  Note:
/*****************************************************************************/
extern ERR_MNGPRS_CODE_E ATC_ActivatePdpContextEx(//return whether success or failure
                ATC_CONFIG_T            *atc_config_ptr,
                MN_DUAL_SYS_E dual_sys,
                BOOLEAN is_all,         //if all PDP contexts need activate.
                uint32 pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT], // the identity of the PDP contexts will be activated.
                MN_PS_SERVICE_TYPE_E ps_service_type,
                MN_PS_SERVICE_RAT_E  ps_service_rat
                );

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CSTT, set pco
//  Global resource dependence : atc_global_info_ptr
//  AUTHOR: 
//  Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCSTT( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CIICR, activate pdp context
//  Global resource dependence : atc_config_ptr
//  AUTHOR: 
//  Note : 
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCIICR( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CIPSHUT, deactivate pdp context
//  Global resource dependence : atc_config_ptr
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessCIPSHUT( // return the handle result
    ATC_CONFIG_T         *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T  *cur_cmd_info    // point to the atc cmd info
);

/******************************************************************************/
//  DESCRIPTION:This function handles command AT+CIFSR,
//                Show PDP address
//
//  Global: atc_config_ptr
//  AUTHOR: 
//  Note : 
/******************************************************************************/
ATC_STATUS ATC_ProcessCIFSR( // return the handle result
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    ATC_CUR_INFO_LIST_T *cur_cmd_info    // point to the atc cmd info
);


#endif
