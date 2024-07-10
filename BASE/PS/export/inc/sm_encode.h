/******************************************************************************
 ** File Name:      sm_encode.h                                               *
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
 **                 It contains SM encode function declaration.               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 12/10/2001     Karin.li         Create.                                   *
 ** 05/22.2002     Karin.li         Modification.                             *  
 ******************************************************************************/
#ifndef _SM_ENCODE_H_
#define _SM_ENCODE_H_

#include "gsm_gprs.h"
#include "sm_data.h"

/* Direction:			MS to network */
#define ACTIVATE_PDP_CONTEXT_REQUEST        65
#define REQUEST_PDP_CONTEXT_ACTIVATION_REJ  69
#define MT_MODIFY_PDP_CONTEXT_ACCEPT        73
#define MO_MODIFY_PDP_CONTEXT_REQUEST       74
#define ACTIVATE_SEC_PDP_CONTEXT_REQUEST    77


#define MIN_NSAPI       5
#define MAX_NSAPI       15
#define MIN_LLC_SAPI    3
#define MAX_LLC_SAPI    11


#define MIN_TI  0
#define ERR_TI  7
#define MAX_TI  14


/*********************************************************************************/
// 	Description : This function is used to encode msg:ACTIVATE_PDP_CONTEXT_REQUEST
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.1
/*********************************************************************************/
extern void SMENCODE_EncodeActpdpcontextreq(					     	    	    // no return
											SM_ACT_PDP_CNTXT_REQ_T*  act_pdp_req,   // ACTIVATE PDP CONTEXT REQUEST struct to be encoded
											PDU_STRUCT_T*            sm_pdu_struct, // message struct send to GMM
											SMMAIN_DATA_T*           sm_data_ptr
											);

/*********************************************************************************/
// 	Description : This function is used to encode message:
//  REQUEST_PDP_CONTEXT_ACTIVATION_REJECT
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.8
/*********************************************************************************/
extern void SMENCODE_EncodeActpdpcontextactrej(                                             // no return
											   SM_REQ_PDP_CNTXT_ACT_REJ_T* req_pdp_act_rej, // REQUEST PDP CONTEXT ACTIVATION REJECT struct to be encoded
											   PDU_STRUCT_T*               sm_pdu_struct    // message struct send to GMM 
											  );

/*********************************************************************************/
// 	Description : This function is used to encode message:
//  DEACTIVATE_PDP_CONTEXT_REQUEST
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.14
/*********************************************************************************/
extern void SMENCODE_EncodeDeactpdpcontextreq(                                          // no return
											  SM_DEACT_PDP_CNTXT_REQ_T* deact_pdp_req,  // DEACTIVATE PDP CONTEXT REQUEST struct to be encoded
											  PDU_STRUCT_T*             sm_pdu_struct   // messzge struct send to GMM
											  );

/*********************************************************************************/
// 	Description : This function is used to encode message:
//  DEACTIVATE_PDP_CONTEXT_ACCEPT
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.15
/*********************************************************************************/
extern void SMENCODE_EncodeDeactpdpcontextacc(                                          //  no return
											  SM_DEACT_PDP_CNTXT_ACC_T* deact_pdp_acc,  // DEACTIVATE PDP CONTEXT ACCEPT struct to be encoded
											  PDU_STRUCT_T*             sm_pdu_struct   // message struct send to GMM
											  );

/*********************************************************************************/
// 	Description : This function is used to encode message:
//  MT_MODIFY_PDP_CONTEXT_ACCEPT
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.11
/*********************************************************************************/
 extern void SMENCODE_EncodeMtmodifypdpcontextacc(                                            // no return
												  SM_MT_MOD_PDP_CNTXT_ACC_T* mt_mod_pdp_acc,  // MT MODIFY PDP CONTEXT ACCEPT struct to be encoded
												  PDU_STRUCT_T*              sm_pdu_struct    // message struct send to GMM
												  );

 /*********************************************************************************/
// 	Description : This function is used to encode message:
//  MO_MODIFY_PDP_CONTEXT_REQUEST
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.10
/*********************************************************************************/
extern void SMENCODE_EncodeMomodifypdpcontextreq(                                           // no return
												 SM_MO_MOD_PDP_CNTXT_REQ_T* mo_mod_pdp_req, // MO MODIFY PDP CONTEXT REQUEST struct to be encoded
												 PDU_STRUCT_T*              sm_pdu_struct,  // message starut send to GMM
												 SMMAIN_DATA_T*             sm_data_ptr
												 );

/*********************************************************************************/
// 	Description : This function is used to encode message:
//  ACTIVATE_SEC_PDP_CONTEXT_REQUEST
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.4
/*********************************************************************************/
extern void SMENCODE_EncodeActsecpdpcontextreq(                               // no return   
								 SM_ACT_SEC_PDP_CNTXT_REQ_T* act_sec_pdp_req, // ACTIVATE SEC PDP CONTEXT REQUEST struct to be encoded
								 PDU_STRUCT_T*               sm_pdu_struct,   // message struct send to GMM
								 SMMAIN_DATA_T*              sm_data_ptr
								 );

/*********************************************************************************/
// 	Description : This function is used to encode message:
//  SM_STATUS
//  Author: Karin Li
//	Note: REF 3GPP TS 24.008/9.5.21
/*********************************************************************************/

void SMENCODE_EncodeSmstatus(                                    // no return
							 SM_STATUS_T*        sm_status,      // SM STATUS struct to be encoded
							 PDU_STRUCT_T*       sm_pdu_struct   // message struct send to GMM 
							 );
							 
/*********************************************************************************/
// 	Description : This function is used to compare two PDP Context. 
//                If the pdp address and the apn of a pdp context is
//                same as the pdp address and the apn of another,they
//                are the same.
//  Author: shaohua xia
//	Note: 
/*********************************************************************************/
BOOLEAN SMENCODE_IsSamePdpcontext(                                 //return a boolean value
                                  PDP_ADDRESS_T*     pdp_addr1,    //the pdp address of a pdp context
                                  APN_T*             apn1,         //the access point name of a pdp context
                                  PDP_ADDRESS_T*     pdp_addr2,    //the pdp address of another pdp context
                                  APN_T*             apn2          //the access point name of another pdp context
                                 );

/*********************************************************************************/
// 	Description : This function is used to compare APN
//  SM_STATUS
//  Author: Hyman wu
//	Note: 
/*********************************************************************************/
BOOLEAN SMENCODE_IsSameAPN(APN_T* apn1, APN_T* apn2);                                 

#endif // end of _SM_ENCODE_H_