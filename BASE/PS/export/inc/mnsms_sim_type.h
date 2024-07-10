/******************************************************************************
 ** File Name:      mnsms_sim_type.h                                          *
 ** Author:         Jenny Wang                                                *
 ** Date:           08/10/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    The file define the mnsms data stucture                   *         
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 08/10/2002     Jenny.Wang       Create.                                   *
 ******************************************************************************/
#ifndef _MNSMS_SIM_TYPE_H_
#define _MNSMS_SIM_TYPE_H_

#include "mn_type.h"
//#include "mn_main.h"
#ifdef PRODUCT_DM
#include "sim_file_structure.h"
#endif
#define SMS_IN_SIM_HEAD_LEN				23

//#define SMS_SIM_STATUS_REPORT_LEN		29
#define MAX_NUM_OF_TP_MR				0xFF


typedef uint8 SMS_SIM_SMS_ARR_T[MN_SMS_SIM_SMS_LEN];
//typedef uint8 SMS_SIM_STATUS_REPORT_ARR_T[SMS_SIM_STATUS_REPORT_LEN];

typedef struct 
{
    MN_SMS_STATUS_E		sms_status;
	SMS_SIM_SMS_ARR_T	sms_sim_sm_arr ;
} SIM_SMS_T , * SIM_SMS_PTR_T;
 
/*  --george mn header file integration--  */
//#define  SMS_ADDR_MAX_LEN  12
#ifdef PRODUCT_DM
typedef uint8  SMS_ADDR_ARR_T[SMS_ADDR_MAX_LEN];
#endif
typedef struct 
{
	BOOLEAN						alpha_id_present;
	SIM_ALPHA_ID				alpha_id_t;
	BOOLEAN						tp_dest_addr_present;
	SMS_ADDR_ARR_T				tp_dest_addr_arr;                                 
	BOOLEAN						tp_sc_addr_present;
	SMS_ADDR_ARR_T				tp_sc_addr_arr;
	BOOLEAN						tp_protocol_id_present;
	uint8						tp_protocol_id;
	BOOLEAN						tp_dcs_present;                   //dcs --- data coding scheme
	uint8						tp_dcs;
	BOOLEAN						tp_vp_present;                   //vp ---- validity period 
	MN_SMS_RELATIVE_FORMAT_VP_T	relative_format_vp;
} SMS_PARAM_T , *SMS_PARAM_PTR_T ;

typedef struct
{
	MN_SMS_TP_STATUS_E tp_status_e;
}SMS_SIM_STATUS_REPORT_T;

/**************************Signal between mnsms with sim*****************/
typedef struct
{
	SIGNAL_VARS
} SIM_SMS_GET_SMS_NUM_REQ_T;	// read sms states in SIM

typedef struct 
{
	SIGNAL_VARS
	MN_SMS_CAUSE_E cause;									// read SIM SMS states operation's result
	uint8          sms_in_sim_valid_num;					// the number of Short Message in SIM
} SIM_SMS_GET_SMS_NUM_CNF_T;

typedef struct 
{
	SIGNAL_VARS 
	MN_SMS_RECORD_ID_T record_id;
} SIM_SMS_READ_PARAM_REQ_T;

typedef struct
{
	SIGNAL_VARS
	MN_SMS_CAUSE_E		cause;
	MN_SMS_RECORD_ID_T	record_id;
	SMS_PARAM_T			param_t;
} SIM_SMS_READ_PARAM_CNF_T;


typedef struct 
{
	SIGNAL_VARS 
    MN_SMS_RECORD_ID_T	record_id;		
} SIM_SMS_READ_SMS_REQ_T;

typedef struct
{
	SIGNAL_VARS
	MN_SMS_CAUSE_E		cause;
	MN_SMS_RECORD_ID_T	record_id;
	SIM_SMS_T			sim_sms_t;
} SIM_SMS_READ_SMS_CNF_T;

typedef struct 
{
	SIGNAL_VARS
	//MN_SMS_STATUS_E   state ;
	SIM_SMS_T			sim_sms_t;
    BOOLEAN             is_long_sms;
} SIM_SMS_WRITE_SMS_REQ_T;

typedef struct 
{
	SIGNAL_VARS 
	MN_SMS_CAUSE_E		cause;
	MN_SMS_RECORD_ID_T	record_id;
	SIM_SMS_T			sim_sms_t;
} SIM_SMS_WRITE_SMS_CNF_T;

typedef struct SIM_SMS_REPLACE_REQ_INFO_T_s
{
	MN_SMS_RECORD_ID_T	record_id;
	SIM_SMS_T			sim_sms_t;
} SIM_SMS_REPLACE_REQ_INFO_T;

typedef struct SIM_SMS_DELETE_REQ_INFO_T_s
{
	uint8	            num;
	MN_SMS_RECORD_ID_T  delete_list[255];
} SIM_SMS_DELETE_REQ_INFO_T;

typedef struct SIM_SMS_REPLACE_SMS_REQ_T_s
{
	SIGNAL_VARS
    SIM_SMS_REPLACE_REQ_INFO_T  replace_info;
    SIM_SMS_DELETE_REQ_INFO_T   delete_info;
} SIM_SMS_REPLACE_SMS_REQ_T;

typedef struct 
{
	SIGNAL_VARS 
	MN_SMS_CAUSE_E		cause;
} SIM_SMS_SET_VMW_FLAG_CNF_T;

typedef struct SIM_SMS_REPLACE_CNF_INFO_T_s
{
	MN_SMS_CAUSE_E		cause;
	MN_SMS_RECORD_ID_T	record_id;
	SIM_SMS_T			sim_sms_t;
} SIM_SMS_REPLACE_CNF_INFO_T;

typedef struct SIM_SMS_DELETE_CNF_INFO_T_s
{
	MN_SMS_CAUSE_E		delete_cause;
	MN_SMS_RECORD_ID_T  delete_id;
} SIM_SMS_DELETE_CNF_INFO_T;

typedef struct // for replace sms sim
{
	SIGNAL_VARS 
    SIM_SMS_REPLACE_CNF_INFO_T  replace_cnf;
	uint8	                    delete_num;
    SIM_SMS_DELETE_CNF_INFO_T   delete_cnf[255];
} SIM_SMS_REPLACE_SMS_CNF_T;

typedef struct 
{
	SIGNAL_VARS
	MN_SMS_RECORD_ID_T	record_id;
	SIM_SMS_T			sim_sms_t;
} SIM_SMS_EDIT_SMS_REQ_T;

typedef struct 
{
	SIGNAL_VARS 
	MN_SMS_CAUSE_E		cause;
	MN_SMS_RECORD_ID_T	record_id;
	MN_SMS_STATUS_E		status;
} SIM_SMS_EDIT_SMS_CNF_T;

typedef struct
{
	SIGNAL_VARS
	MN_SMS_RECORD_ID_T	record_id;
	MN_SMS_STATUS_E		status;
} SIM_SMS_UPDATE_SMS_STATUS_REQ_T;

typedef struct
{
	SIGNAL_VARS 
	MN_SMS_CAUSE_E		cause;
	MN_SMS_RECORD_ID_T	record_id;
	MN_SMS_STATUS_E		status;
} SIM_SMS_UPDATE_SMS_STATUS_CNF_T;

typedef struct 
{
	SIGNAL_VARS
	MN_SMS_RECORD_ID_T	record_id;
} SIM_SMS_READ_STATUS_REPORT_REQ_T;

typedef struct
{
	SIGNAL_VARS
	MN_SMS_CAUSE_E				cause;
	MN_SMS_RECORD_ID_T			record_id;
	SMS_SIM_STATUS_REPORT_ARR_T status_report_arr; 
} SIM_SMS_READ_STATUS_REPORT_CNF_T;

typedef struct 
{
	SIGNAL_VARS 
	MN_SMS_RECORD_ID_T			record_id;
    SMS_SIM_STATUS_REPORT_ARR_T status_report_arr;   
} SIM_SMS_WRITE_STATUS_REPORT_REQ_T;

typedef struct 
{
	SIGNAL_VARS
	MN_SMS_CAUSE_E		cause;
	MN_SMS_RECORD_ID_T	record_id;
} SIM_SMS_WRITE_STATUS_REPORT_CNF_T;

typedef struct
{
	SIGNAL_VARS
} SIM_SMS_READ_SMSS_REQ_T;

typedef struct
{
	SIGNAL_VARS
	MN_SMS_CAUSE_E	cause;
	MN_SMS_TP_MR_T	tp_mr;
	BOOLEAN			mem_is_full;
} SIM_SMS_READ_SMSS_CNF_T;

typedef struct
{
	SIGNAL_VARS
	MN_SMS_TP_MR_T	tp_mr; 
	BOOLEAN			mem_is_full;
} SIM_SMS_WRITE_SMSS_REQ_T;

typedef struct
{
	SIGNAL_VARS
	MN_SMS_CAUSE_E cause;
} SIM_SMS_WRITE_SMSS_CNF_T;

typedef struct
{
	SIGNAL_VARS
    MN_SMS_RECORD_ID_T	record_id;
	SMS_PARAM_T			param_t;
} SIM_SMS_WRITE_PARAM_REQ_T;

typedef struct
{
	SIGNAL_VARS
	MN_SMS_CAUSE_E		cause;
	MN_SMS_RECORD_ID_T	record_id;
} SIM_SMS_WRITE_PARAM_CNF_T;

#endif	/*END_MNSMS_SIM_TYPE_H_*/
