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

/*******************************************************************************
 **                         Common NUM define                                  *
 ******************************************************************************/
 #define MAX_PDP_ADDRESS_LEN 20
 //Hyman we set at pdp max num as MN:MN_GPRS_MAX_PDP_ID_NUM
 #define MAX_PDP_CONTEXT_COUNT 15
 #define ERR_INVALID_INDEX 21
 #define MAX_PCO_LEN 253
 #define MAX_PDP_TYPE_LEN 5
 #define MAX_APN_LEN 100
 #define MAX_PAP_USER_LEN 64
 #define MAX_PAP_PASSWD_LEN 64


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


typedef struct
{
    BOOLEAN is_all;
    uint32  pdp_id[MAX_PDP_CONTEXT_COUNT]; //xgxu convert int to uint32 
}ATC_PDP_ACT_REQ_T;

typedef struct
{
    BOOLEAN is_all;
    uint32  pdp_id[MAX_PDP_CONTEXT_COUNT];//xgxu convert int to uint32 
}ATC_SHOW_PDP_ADDR_REQ_T;

typedef struct
{
    int    l2p;
    uint32 pdp_id[MAX_PDP_CONTEXT_COUNT];//xgxu convert int to uint32 
}ATC_ENTER_DATA_STATE_REQ_T;

typedef struct
{
    int     response;
    int     l2p;
    BOOLEAN is_all;
    uint32  pdp_id;                       //xgxu convert int to uint32 
}ATC_MANUAL_RESP_NET_T;

typedef struct
{
    int resp_mode;
}ATC_AUTO_RESP_NET_T;

typedef struct
{
    uint8 *ms_class;
}ATC_SET_MS_CLASS_REQ_T;

typedef struct
{
    int service_type;
}ATC_SELECT_SMS_SERVICE_T;

typedef struct
{
    int status_mode;
}ATC_NETWORK_REG_STATUS_T;

typedef struct
{
    int mode;
    int buffer;
}ATC_SET_EVENT_REPORT_T;


typedef struct ATC_DATA_INFO_T_s
{
    struct ATC_DATA_INFO_T_s *next;
    uint8 *data_ptr;
    uint32 data_size;
}ATC_DATA_INFO_T;

typedef struct ATC_DATA_PENDING_LIST_T_s
{
    ATC_DATA_INFO_T *head;
    ATC_DATA_INFO_T *tail;
}ATC_DATA_PENDING_LIST_T;

/*******************************************************************************
 **                         Function declaration                               * 
 ******************************************************************************/
/******************************************************************************/ 
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGDCONT( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/******************************************************************************/ 
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Shijun.cui
//    Note : None
/******************************************************************************/
extern ATC_STATUS ATC_ProcessCGPCO( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/******************************************************************************/ 
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGQREQ( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );
/******************************************************************************/  
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGQMIN( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );
/******************************************************************************/  
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGATT( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );
/******************************************************************************/  
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGACT( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );
/******************************************************************************/  
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: winnie.zhu
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGCMOD( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );
/******************************************************************************/ 
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGDATA( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

/******************************************************************************/ 
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGSMS(// return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );
/******************************************************************************/ 
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGREG(// return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );
/******************************************************************************/ 
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGEREP( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

extern ATC_STATUS ATC_ProcessD( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                             );

#ifdef _GPRS_FULL_FUNCTION_
/******************************************************************************/ 
//    DESCRIPTION:
//           data configure about gprs data service
//    Global     : atc_config_ptr 
//    AUTHOR: 
//    Note : None
//    History    :
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessSDATACONF( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                             );
/******************************************************************************/ 
//    DESCRIPTION:
//           open or close the socket
//    Global     : atc_config_ptr 
//    AUTHOR: 
//    Note : None
//    History    :
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessSDATASTART( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                             );
/******************************************************************************/ 
//    DESCRIPTION:
//           send data use opened socket
//    Global     : atc_config_ptr 
//    AUTHOR: 
//    Note : None
//    History    :
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessSDATASEND( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                             );
#endif /*_GPRS_FULL_FUNCTION_*/

/******************************************************************************/ 
//    DESCRIPTION:
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessSSST( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                             );
                             
/******************************************************************************/  
//    DESCRIPTION: Spreadtrum attach or detach
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessSATT( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                            );

/******************************************************************************/  
//    DESCRIPTION: Spreadtrum auto attach or not
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessSAUTOATT( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                            );
                            
/******************************************************************************/  
//    DESCRIPTION: Spreadtrum send gprs for FTA test
//    Global resource dependence : atc_config_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessSGPRSDATA( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                            );

/******************************************************************************/  
//    DESCRIPTION: This function handles command AT+CGEQREQ, set 3G Qos
//    Global resource dependence : atc_config_ptr 
//    AUTHOR:  king.li
//    Note : 
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGEQREQ( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                            );

/******************************************************************************/  
//    DESCRIPTION: This function handles command AT+CGEQMIN, set 3G min Qos
//    Global resource dependence : atc_config_ptr 
//    AUTHOR:  King.Li
//    Note : 
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGEQMIN( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                            );
#if 0
#ifdef _SUPPORT_GPRS_       
/*****************************************************************************/
//  Description : This function handle the APP_MN_ACTIVATE_PDP_CONTEXT_IND signal and return 
//                the unsolicited result code. 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessActPdpContextInd(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                APP_MN_GPRS_T       *sig_ptr         // point to the APP_MN_SETUP_IND signal
                );
#endif
#endif
/******************************************************************************/  
//    DESCRIPTION: This function handle signal APP_MN_DEACTIVATE_PDP_CONTEXT_IND
//    Global resource dependence : atc_config_ptr 
//    AUTHOR:
//    Note : 
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessDeactPdpContextInd(
                        ATC_CONFIG_T        *atc_config_ptr,
                        APP_MN_GPRS_T       *sig_ptr,
                        ATC_GLOBAL_INFO_T   *atc_global_info_ptr
                        );

#ifdef _SUPPORT_GPRS_
/*****************************************************************************/
//  Description : This function handle the APP_MN_ACTIVATE_PDP_CONTEXT_CNF signal and return 
//                the unsolicited result code. 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessActPdpContextCnf(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                APP_MN_GPRS_T       *sig_ptr,        // point to the APP_MN_SETUP_IND signal
                ATC_GLOBAL_INFO_T   *atc_global_info_ptr
                );

/*****************************************************************************/
//  Description : This function indicate is to send detach request if no pdp 
//                is activated
//  Global resource dependence : none
//  Author:       winnie.zhu
//  Note:
/*****************************************************************************/
extern void ATC_SendDetachReq(MN_DUAL_SYS_E dual_sys);

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

/******************************************************************************/ 
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGPADDR( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );

#if 0
/******************************************************************************/ 
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGAUTO( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );
/******************************************************************************/ 
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGANS( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );
/******************************************************************************/ 
//    DESCRIPTION:
//    Global resource dependence : atc_global_info_ptr 
//    AUTHOR: Ivan.Yin
//    Note : None
/******************************************************************************/ 
extern ATC_STATUS ATC_ProcessCGCLASS( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                ATC_CUR_INFO_LIST_T *cur_cmd_info // point to the atc cmd info
                );
#endif
#endif
