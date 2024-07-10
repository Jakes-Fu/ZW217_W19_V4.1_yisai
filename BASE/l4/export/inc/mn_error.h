/******************************************************************************
 ** File Name:      mn_error.h                                                *
 ** Author:         Fancier fan                                               *
 ** Date:           05/28/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *                                                      
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 05/28/2002     Fancier fan      Create.                                   *
 ******************************************************************************/
#ifndef _MN_ERROR_H_
#define _MN_ERROR_H_

#define MNPHONE_NO_ERR_SPACE       0            //return success
#define MNPHONE_NO_ANR1_SPACE     1             //ANR1 is full
#define MNPHONE_NO_ANR2_SPACE     2             //ANR2 is full
#define MNPHONE_NO_ANR3_SPACE     4             //ANR3 is full
#define MNPHONE_NO_ADN_SPACE       8            //ADN is full
#define MNPHONE_NO_EMAIL_SPACE   16             //EMAIL is full
#define MNPHONE_NO_SNE_SPACE       32             //SNE is full  
#define MNPHONE_API_ERROR             128           //return error   

typedef enum _ERR_MNSMS_CODE_E
{
    ERR_MNSMS_NONE = 0,             					//return success
    ERR_MNSMS_OUT_OF_RANGE = ((0x0010 << 16) + 0),      	// length is out of range
	ERR_MNSMS_NOT_RIGHT_STORAGE,						// storage is not right
	ERR_MNSMS_SIM_NOT_READY,							// sim card intializing not ready
	ERR_MNSMS_NOT_RIGHT_PARAM,							// not right parameter
	ERR_MNSMS_SIM_SET_ERROR,							// set the parameter error
	ERR_MNSMS_NOT_RIGHT_STATUS,    						// API run failure, user must input right sms status
	ERR_MNSMS_PTR_IS_NULL,    							// API run failure, the pointer is null
    ERR_MNSMS_ME_NOT_SUPPORT,							// not support in me
    ERR_MNSMS_NO_SC_ADDRESS,       						// API run failure, user must write default SC_Address or input SC_Address while send message
    ERR_MNSMS_NO_DEST_ADDRESS,     						// API run failure, user must input default destination_address or input destination address while send message
    ERR_MNSMS_NO_PID,              						// API run failure, user must input default Protocol_indentify or input Protocol_indentify while send message
    ERR_MNSMS_NO_VAILID_TIME,      						// API run failure, user must input default valid_time or input valid_time while send message
    ERR_MNSMS_WRITE_ME_FAILURE,    						// API run failure, the operation that user write default parameter value to ME is failure
    ERR_MNSMS_ME_NOT_READY,								// ME intializing not ready
    ERR_MNSMS_FDN_FAILED,                               // API run failure, the fdn check failed
    ERR_MNSMS_FDN_SCA_FAILED,                           // API run failure, the fdn check for SC Address failed
    ERR_MNSMS_FDN_DEST_NUM_FAILED                       // API run failure, the fdn check for Destination Number failed
}ERR_MNSMS_CODE_E;

typedef enum _ERR_MNSS_CODE_E
{
    ERR_MNSS_NO_ERR = 0,              					// API run successfully, no error 
    ERR_MNSS_PTR_IS_NULL = ((0x0020 << 16) + 0),       	// AAPI run failure, the pointer is null
    ERR_MNSS_INVALID_NO_REPLY_TIME,     				// API run failure, no invalid no_reply_time.
    ERR_MNSS_INVALID_FORWARD_NUM_LEN,     				// API run failure, no invalid forward number length.
    ERR_MNSS_INVALID_SS_CODE,     						// API run failure, no invalid ss code.
    ERR_MNSS_NOT_NUMERIC,     							// API run failure, the number is not numeric.
    ERR_MNSS_INVALID_DCS,     							// API run failure, not right dcs.
    ERR_MNSS_LEN_OUT_OF_RANGE,              			// API run failure, length is out of range
    ERR_MNSS_WRITE_ME_FAILURE,  						// API run failure, the operation that user write default parameter value to ME is failure
    ERR_MNSS_INVALID_SERVICE_TYPE,						// API run failure, the service type is invalid
    ERR_MNSS_INVALID_OPERATION,                         // API run failure, the operation is not allowed
    ERR_MNSS_FDN_FAILED                                 // API run failure, the fdn check failed
}ERR_MNSS_CODE_E;

typedef enum _MNSS_SENDSS_RESULT_E
{
    MNSS_SUCCESS,              					        // API run successfully, no error 
    MNSS_INFORM_STK,       	                            // The SS String need be controlled by SIM
    MNSS_FAILURE,     				                    // API run failure, no invalid no_reply_time.
    MNSS_FDN_FAILED       				                // API run failure, the fdn check failed
}MNSS_SENDSS_RESULT_E;

typedef enum _MNSIM_RESULT_E
{
    MNSIM_SUCCESS,              					   
    MNSIM_FILE_NOT_EXSIT,    
    MNSIM_ERROR     			
}MNSIM_RESULT_E;

typedef enum _ERR_MNPHONE_CODE_E
{
    ERR_MNPHONE_NO_ERR = 0,              				// API run successfully, no error 
    ERR_MNPHONE_PTR_IS_NULL = ((0x0030 << 16) + 0),       // AAPI run failure, the pointer is null
    ERR_MNPHONE_NOT_RIGHT_PARAM,     					// API run failure, not right parameter
    ERR_MNPHONE_OUT_OF_RANGE,              				// API run failure, length is out of range
    ERR_MNPHONE_WRITE_ME_FAILURE  						// API run failure, the operation that user write default parameter value to ME is failure
}ERR_MNPHONE_CODE_E;


typedef enum _ERR_MNDATAMAG_CODE_E
{
	ERR_MNDATAMAG_NO_ERR = 0,              				// API run successfully, no error 
    ERR_MNDATAMAG_IS_NULL = ((0x0040 << 16) + 0),       	// AAPI run failure, the pointer is null
    ERR_MNDATAMAG_WRITE_READ_NV_ERROR,     				// API run failure, write or read nv error
    ERR_MNDATAMAG_OUT_OF_RANGE,              			// API run failure, length is out of range
    ERR_MNDATAMAG_LEN_IS_ZERO,							// API run failure, length is zero
    ERR_MNDATAMAG_RETURN_ERROR,							// API run failure, call function error
    ERR_MNDATAMAG_SERVICE_NOT_EXIT,						// API run failure, service is not exit
    ERR_MNDATAMAG_WRITE_NV_ERROR,						// API run failure, write to nv error
    ERR_MNDATAMAG_NOT_REALIZE							// API run failure, the function not realize.
}ERR_MNDATAMAG_CODE_E;

typedef enum _ERR_MNCALL_CODE_E
{
	ERR_MNCALL_NO_ERR = 0,              				// API run successfully, no error 
    ERR_MNCALL_PTR_IS_NULL = ((0x0050 << 16) + 0),       	// API run failure, the pointer is null
    ERR_MNCALL_FAIL_BY_FDN,     						// API run failure, failed because of FDN
    ERR_MNCALL_OUT_OF_RANGE,              				// API run failure, length is out of range
    ERR_MNCALL_LOOP_BACK_IS_ON,                         // API run failure, loop back is on
    ERR_MNCALL_BC_CONFILIT                              // API run failure, loop back is on
}ERR_MNCALL_CODE_E;

/* enum type for mn gprs api return value */
typedef enum _ERR_MNGPRS_CODE_E
{
   ERR_MNGPRS_NO_ERR = 0,						//函数调用成功
   ERR_MNGPRS_CALL_FAILURE = ((0x0090 << 16) + 0),//函数调用失败
   ERR_MNGPRS_INP_NULL,							//输入的参数为空
   ERR_MNGPRS_OUT_MEMORY,						//内存分配失败
   ERR_MNGPRS_NO_VALID_PARAM,                   //输入参数指示无有效参数 
   ERR_MNGPRS_INVALID_PDP_ID,                   //输入pdp上下文标识无效
   ERR_MNGPRS_INVALID_PDP_TYPE,					//输入pdp type参数无效
   ERR_MNGPRS_INVALID_APN,						//输入APN参数无效
   ERR_MNGPRS_INVALID_PDP_ADDR,					//输入PDP ADDR参数无效
   ERR_MNGPRS_NOT_SUPPORT_COMP,					//输入COMP参数不支持
   ERR_MNGPRS_INVALID_PCO,						//输入PCO参数无效
   ERR_MNGPRS_INVALID_RELIABILITY,				//输入RELIABILITY参数无效
   ERR_MNGPRS_INVALID_PEAK,						//输入PEAK参数无效
   ERR_MNGPRS_INVALID_DELAY,
   ERR_MNGPRS_INVALID_PRECEDENCE,
   ERR_MNGPRS_INVALID_MEAN,
   ERR_MNGPRS_INVALID_PARA_LEN,
   ERR_MNGPRS_INVALID_PDP_STATE
	
}ERR_MNGPRS_CODE_E;

#endif