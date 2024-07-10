/******************************************************************************
 ** File Name:      sm_decode.h                                               *
 ** Author:         Karin.li                                                  *
 ** Date:           12/10/2001                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This is a initial version.                                *
 **                 It contains SM decode function declaration.               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 12/10/2001     Karin.li         Create.                                   *
 ** 05/22/2002     Karin.li         Modification.                             *  
 ******************************************************************************/
#ifndef _SM_DECODE_H_
#define _SM_DECODE_H_

#include "gsm_gprs.h"
#include "sm_data.h"

/*****************************************************************************/
// 	Description : This function is used to decode all the msg header received 
//  from GMM and return the error type if decode unsuccessful
//  Author: Karin Li
//	Note: The return value is error type
/*****************************************************************************/
extern uint16 SMDECODE_DecodeMsghdr(                               // error type
									uint32        sm_pdu_size,     // message length(byte number)
									uint8*        sm_pdu,          // message byte pointer
									SM_MSG_HDR_T* msg_header       // message header struct poninter
									);

/*******************************************************************************/
// 	Description :  This function is used to decode message:
//  ACTIVATE_PDP_CONTEXT_ACCEPT received from GMM and return the error type
//  if decode unsuccessful
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.2 
/*******************************************************************************/
extern uint16 SMDECODE_DecodeActpdpcontextacc(                                     // error type
								              SM_ACT_PDP_CNTXT_ACC_T* act_pdp_acc, // output struct
											  uint8*                  sm_pdu,      // message byte pointer
											  uint32                  sm_pdu_size, // message length(byte number),include header
											  BOOLEAN                 ti_ext_flag  // indicate whether TI is extented
											  );
											  
/*******************************************************************************/
// 	Description :  This function is used to decode message:
//  ACTIVATE_PDP_CONTEXT_ACCEPT received from GMM and return the error type
//  if decode unsuccessful
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.2 
/*******************************************************************************/
extern uint16 SMDECODE_DecodeActpdpcontextaccNew(                                     // error type
								                 SM_ACT_PDP_CNTXT_ACC_T* act_pdp_acc, // output struct
											     uint8*                  sm_pdu,      // message byte pointer
											     uint32                  sm_pdu_size, // message length(byte number),include header
											     BOOLEAN                 ti_ext_flag  // indicate whether TI is extented
											    );

/*******************************************************************************/
// 	Description : This function is used to decode message:
//  ACTIVATE_PDP_CONTEXT_REJECT received from GMM and return the error type
//  if decode unsuccessful
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.3 
/*******************************************************************************/
extern uint16 SMDECODE_DecodeActpdpcontextrej(                                     // error type
											  SM_ACT_PDP_CNTXT_REJ_T* act_pdp_rej, // output struct
											  uint8*                  sm_pdu,      // message byte pointer
											  uint32                  sm_pdu_size, // message length(byte number),include header
											  BOOLEAN                 ti_ext_flag  // indicate whether TI is extented
											  );

/*******************************************************************************/
// 	Description : This function is used to decode message:
//  REQUEST_PDP_CONTEXT_ACTIVATION received from GMM and return the error type
//  if decode unsuccessful
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.7 
/*******************************************************************************/
extern uint16 SMDECODE_DecodeReqpdpcontextact(                                      // error type
											  SM_REQ_PDP_CNTXT_ACT_T* req_pdp_act,  // output struct
											  uint8*                  sm_pdu,       // message byte pointer
											  uint32                  sm_pdu_size,  // message length(byte number),include header
											  BOOLEAN                 ti_ext_flag   // indicate whether TI is extented
											  );

/*******************************************************************************/
// 	Description : This function is used to decode message:
//  DEACTIVATE_PDP_CONTEXT_REQUEST received from GMM and return the error type
//  if decode unsuccessful
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.14 
/*******************************************************************************/
extern uint16 SMDECODE_DecodeDeactpdpcontextreq(                                         // error type
												SM_DEACT_PDP_CNTXT_REQ_T* deact_pdp_req, // output struct
												uint8*                    sm_pdu,        // message byte pointer
												uint32                    sm_pdu_size,   // message length(byte number),include header
											    BOOLEAN                   ti_ext_flag    // indicate whether TI is extented
												);

/*******************************************************************************/
// 	Description : This function is used to decode message:
//  MT_MODIFY_PDP_CONTEXT_REQUEST received from GMM and return the error type
//  if decode unsuccessful
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.9 
/*******************************************************************************/
extern uint16 SMDECODE_DecodeMtmodifypdpcontextreq(                                           // error type
												   SM_MT_MOD_PDP_CNTXT_REQ_T* mt_mod_pdp_req, // output struct
												   uint8*                     sm_pdu,         // message byte pointer
												   uint32                     sm_pdu_size,    // message length(byte number),include header
												   BOOLEAN                    ti_ext_flag     // indicate whether TI is extented
												   );

/*******************************************************************************/
// 	Description : This function is used to decode message:
//  MO_MODIFY_PDP_CONTEXT_ACCEPT received from GMM and return the error type
//  if decode unsuccessful
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.12 
/*******************************************************************************/
extern uint16 SMDECODE_DecodeMomodifypdpcontextacc(                                           // error type
												   SM_MO_MOD_PDP_CNTXT_ACC_T* mo_mod_pdp_acc, // output struct
												   uint8*                     sm_pdu,         // message byte pointer  
												   uint32                     sm_pdu_size,    // message length(byte number),include header
												   BOOLEAN                    ti_ext_flag     // indicate whether TI is extented
												   );

/*******************************************************************************/
// 	Description : This function is used to decode message:
//  MODIFY_PDP_CONTEXT_REJECT, ACTIVATE_SEC_PDP_CONTEXT_REJECT received from 
//  GMM and return the error type if decode unsuccessful.
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.6 & 9.5.13 
/*******************************************************************************/
extern uint16 SMDECODE_DecodeModifyactsecpdpcontextrej(                                                          // error type
													   SM_MOD_OR_ACT_SEC_PDP_CNTXT_REJ_T* mod_or_act_sec_pdp_rej,// output struct 
													   uint8*                             sm_pdu,                // message byte pointer
													   uint32                             sm_pdu_size,           // message length
													   BOOLEAN                            ti_ext_flag            // indicate whether TI is extented
													   );

/*******************************************************************************/
// 	Description : This function is used to decode message:
//  ACTIVATE_SEC_PDP_CONTEXT_ACCEPT received from GMM and return the error type
//  if decode unsuccessful
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.5 
/*******************************************************************************/
extern uint16 SMDECODE_DecodeActsecpdpcontextacc(                                             // error type
												 SM_ACT_SEC_PDP_CNTXT_ACC_T* act_sec_pdp_acc, // output struct 
												 uint8*                      sm_pdu,          // message byte pointer
												 uint32                      sm_pdu_size,     // message length
												 BOOLEAN                     ti_ext_flag      // indicate whether TI is extented
												 );

/*******************************************************************************/
// 	Description : This function is used to decode message:
//  ASM_STATUS received from GMM and return the error type
//  if decode unsuccessful
//  Author: xiaonian.li
//	Note: REF 3GPP TS 24.008/9.5.21
/*******************************************************************************/
extern uint16 SMDECODE_DecodeSmStatus(                                     // error type
							   SM_STATUS_T             *sm_status, // output struct
							   uint8*                  sm_pdu_ptr,  // message byte pointer
							   uint32                  sm_pdu_size, // message length(byte number),include header
							   BOOLEAN                 ti_ext_flag  // indicate whether TI is extented
							   );

/*******************************************************************************/
// 	Description : This function is used to compare qos with min qos value
//  Author: Hyman.wu
//	Note: spec 4.08 10.5.6.5 
/*******************************************************************************/
BOOLEAN SMDECODE_IsHigherMinQos(     
                              TOTAL_QOS_T  qos, // qos parameter
                              NSAPI_NUM_T   nsapi, // nsapi
                              SMMAIN_DATA_T *sm_data_ptr
                              );
/*****************************************************************************/
//  Description : transfer from APN string to DNS domain name format
//  Global resource dependence : none
//  Author:hyman.wu
//  Note: 
//  spec 23003,9.1 
//  Following RFC 1035 [15] the labels should consist only of the alphabetic 
//  characters (A-Z and a-z), digits (0-9) and the dash (-). 
//  The case of alphabetic characters is not significant.
/*****************************************************************************/
int32 APNString2DNSDomainBuffer(uint8* str,     //input string end with '\0'
								uint16 len,	    //the format:.cmnet,input length
							    uint8* buffer); //dns domain buffer


/*****************************************************************************/
//  Description : transfer from DNS domain name format to APN string
//  Global resource dependence : none
//  Author:hyman.wu
//  Note: 
//  spec 23003,9.1 
//  Following RFC 1035 [15] the labels should consist only of the alphabetic 
//  characters (A-Z and a-z), digits (0-9) and the dash (-). 
//  The case of alphabetic characters is not significant.
/*****************************************************************************/
void DNSDomainBuffer2APNString(uint8* buffer,  //input buffer pointer
							   uint16 len,     //input buffer length
							   uint8* str) ;     //return char string
							  										 

#endif //end of _SM_DECODE_H_