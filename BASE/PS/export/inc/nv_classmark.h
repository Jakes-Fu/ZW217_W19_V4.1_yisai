/******************************************************************************
 ** File Name:      nv_classmark.h                                            *
 ** Author:         Jenny Wang                                                *
 ** Date:           08/10/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    These source file includes the implenmention code of the  *
 **                 CSN.1 Message Encode and Decode                           *         
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 08/10/2002     Jenny.Wang       Create.                                   *
 ******************************************************************************/
#ifndef __NV_CLASSMARK_H__
#define __NV_CLASSMARK_H__

#include "sci_types.h"



#define SIZE_OF_MODEL_NAME	    40
#define SIZE_OF_FACTURY_NAME	40


typedef struct {
	uint8	    revision_level;		            // 0 = Reserved for phase 1; 1 = phase 2; 2 = R99 or later; 3 = Reserved for future use
	BOOLEAN		support_early_classmark;        // true = support Controlled early classmark sending.
	BOOLEAN		pseudo_sync_handover_supported;	// true = support pseudo_sync handover see spec4.08 sec 3.4.4.2
	uint8       ss_screen_ind;			        // SS screening indicator,See spec 24008 10.5.5.12,two bits for supplementary service.
	BOOLEAN     ss_version_ind;                 // 1 = SS-Protocol version 3 is supported,0 = not support version 3
	BOOLEAN		mt_sms_supported;		        // See spec 4.08 section 10.5.1.6	1 = support mobile terminated short message.
	BOOLEAN		vbs_supported;			        // 1 = voice broadcast service is supported.
	BOOLEAN     vgcs_supported;			        // 1 = voice group call service is supported.
	BOOLEAN     classmark3_supported;           // 1 = Classmark 3 supported 
    BOOLEAN     lcs_va_capability;              // indicates the support of the LCS value added location request notification via PS domain
	BOOLEAN     ucs2_treatment;		            // 3gpp 24008. ucs2 treatment,0 default alphabet over UCS2;1 no preference of UCS2 and default alphabet
    BOOLEAN     solsa_capability;               // indicates the support of SoLSA
	BOOLEAN     cm_service_prompt;              // indicates the support of "Network initiated MO CM connection request"
	BOOLEAN     ext_meas_supported;	            // 1 = support extend measurement report.	See spec4.08 table 10.5.7
	BOOLEAN     sms_value_flag;                 // indicator of include sms_value and sm_value,1:inlcude;0:exclude.
	uint8		sms_value;				        // The time needed to switch from one radio channel to another(PLL lock time).
										        // 0 = 1/4 timeslot; 1 = 2/4 timeslot; 2 = 3/4 timeslot; ... 15 = 16/4 time slot
										        // For ADI rf board. the value = 1.
	uint8		sm_value;				        // the time needed to switch from one radiop channel to another and perform a ncell power measurement.
										        // See spec4.08 table 10.5.7   The value meaning is same as sms_value.
	uint8       gsm_multislot_class;            // Multislot class about CS service
	BOOLEAN     ms_pos_method_supported;        // indicates whether the MS supports Positioning Method or not for the provision of Location Services
    uint8       ms_pos_method;                  // indicates the Positioning Method(s) supported by the mobile station.
    BOOLEAN  	sms_gprs_supported;	            // true = SMS over GPRS is supported,dualsim not support.
    BOOLEAN		pfc_mode;                       // indicates 1 whether Mobile station does support BSS packet flow procedures.
    BOOLEAN     dtm_supported;                  // indicates whether support DTM.
    BOOLEAN     ctm_supported;                  // indicates whether support CTM text telephony.
    
    //encription algrithm
    BOOLEAN		a51_supported;			// true = support encryption algorism a51
	BOOLEAN		a52_supported;			// true = support encryption algorism a52
	BOOLEAN 	a53_supported;          // true = support encryption algorism a53
	BOOLEAN		a54_supported;          // true = support encryption algorism a54
	BOOLEAN		a55_supported;          // true = support encryption algorism a55
	BOOLEAN     a56_supported;          // true = support encryption algorism a56
	BOOLEAN     a57_supported;          // true = support encryption algorism a57
	BOOLEAN		gea1_supported;	        // true = GPRS encription algrithm GEA/1 
	BOOLEAN		gea2_supported;         // true = GPRS encription algrithm GEA/2 
	BOOLEAN		gea3_supported;         // true = GPRS encription algrithm GEA/3 
	BOOLEAN		gea4_supported;         // true = GPRS encription algrithm GEA/4 
	BOOLEAN 	gea5_supported;         // true = GPRS encription algrithm GEA/5 
	BOOLEAN 	gea6_supported;         // true = GPRS encription algrithm GEA/6 
	BOOLEAN 	gea7_supported;         // true = GPRS encription algrithm GEA/7 
	
	//DRX parameter
	uint8 		split_pg_cycle;			// See spec/4.08 table 10.5.139. 	range(0-98)
	BOOLEAN 	split_on_ccch;			// 1 = ms support split paging cycle on CCCH( base station shall support it as well)
	uint8		non_drx_timer;		    // See spec/4.08 table 10.5.139. 
	
	//Radio Access Technology supported
	BOOLEAN     p_gsm_supported;              // true = support P_GSM band
	BOOLEAN     e_gsm_supported;              // true = support E_GSM band 
	BOOLEAN     r_gsm_supported;              // true = support R_GSM band 
	BOOLEAN     dcs_supported;                // true = support DCS band
	BOOLEAN     pcs_supported;                // true = support PCS band 
	BOOLEAN		gsm_850_supported;            // true = support 850 band 
	BOOLEAN     umts_fdd_ra_tech_cap;         // indicates whether support "UMTS FDD"
    BOOLEAN     umts_384mcps_tdd_ra_tech_cap; // indicates whether support "UMTS 3.84 Mcps TDD"
    BOOLEAN     cdma2000_ra_tech_cap;         // indicates whether support "CDMA2000"
    BOOLEAN     umts_128mcps_tdd_ra_tech_cap; // indicates whether support "UMTS 1.28 Mcps TDD"
	
	//gmsk power class
	uint8		e_gsm_pwr_class;		      // 0 = class1; 1 = class2 ... maxim value = 4 
	uint8		dcs_pwr_class;			      // 0 = class1; 1 = class2 ... maxim value = 2
	uint8		pcs_pwr_class;			      // 0 = class1; 1 = class2 ... maxim value = 2
	uint8		gsm_850_pwr_class;		      // 0 = class1; 1 = class2 ... maxim value = 2
	
	//8psk power class
    uint8		psk8_gsm900_pwr_class;		// 0 = 8PSK modulation not supported for uplink; 1 = Power class E1 ... maxim value = 3 
	uint8		psk8_dcs_pwr_class;			// 0 = 8PSK modulation not supported for uplink; 1 = Power class E1 ... maxim value = 3
	uint8		psk8_pcs_pwr_class;			// 0 = 8PSK modulation not supported for uplink; 1 = Power class E1 ... maxim value = 3
	uint8		psk8_gsm850_pwr_class;		// 0 = 8PSK modulation not supported for uplink; 1 = Power class E1 ... maxim value = 3
	
    //gprs and egprs parameters
    uint8		gprs_multislot_class;		// Class 1 - 12 see spec 5.2. annex B
	BOOLEAN     gprs_ext_dynamic_supported; // 0 - not support; 1-support ext dynamic alloc
	BOOLEAN     egprs_supported;            // support egprs or not
	BOOLEAN     egprs_class_type;           // 0 for egprs modulation class2,support 8psk bothlink;1 for class 1,support downlink 8psk only
	uint8       egprs_multislot_class;      // Class 1 - 12 see spec 5.2. annex B
	BOOLEAN     egprs_ext_dynamic_supported;// 0 - not support; 1-support ext dynamic alloc
	BOOLEAN     egprs_gmsk_only_mode;       // indicates whether support "Modulation based multislot class"  
	BOOLEAN     ext_ul_tbf_supported;       // indicates whether support Extended uplink TBF or not
    BOOLEAN     nacc_supported;             // indicates whether support Network Assisted Cell Change or not
    
    // Chanel mode supported
	BOOLEAN     channel_type_flag;          // 0: FULL_RATE_ONLY 1: DUAL_RATE
    BOOLEAN     half_rate_prefer_flag;      // TRUE: prefer half rate; FALSE: prefer full rate 
	BOOLEAN 	full_rate_v1_supported;     // GSM full rate speech version 1,FS is must
	BOOLEAN 	full_rate_v2_supported;     // GSM full rate speech version 2,EFS 
	BOOLEAN		full_rate_v3_supported;     // GSM full rate speech version 3,AHS 
	BOOLEAN     half_rate_v1_supported;     // GSM half rate speech version 1,HS
	BOOLEAN     half_rate_v3_supported;     // GSM half rate speech version 3,AHS
	BOOLEAN 	data_12k_supported;         // GSM circuit data transfer,not supported
	BOOLEAN 	data_6k_supported;          // GSM circuit data transfer,not supported
	BOOLEAN 	data_3k_supported;          // GSM circuit data transfer,not supported
	
	uint8       rf_sensitivity;             // RF ref power level,used to filter ARFCN in power sweep result  
    
    uint16		ms_class;                       // indicates the mode of operation:Class-A,Class-B,Class-CG,Class-CC
    int16		model_type;                     // 0 means mobile, 1 means module
    char 		model_name[SIZE_OF_MODEL_NAME];	// A string of the handset model name
                                                // The last char of model name (model_name[SIZE_OF_MODEL_NAME-1]) 
                                                // is used as a mask to show which card is allowed to receive paging 
                                                // when master card is in tbf mode.
                                                // The value of model_name[SIZE_OF_MODEL_NAME-1]&(1<<i) indicate the permission for 
                                                // card i."1"means "allowed", "0" means "not allowed".
	char 		manufaturer_name[SIZE_OF_FACTURY_NAME];
    BOOLEAN     gmm_readytimer_flag;         // indicates whether requeste READY timer
    uint16      gmm_readytimer_value;          // indicates the READY timer in second unit
    BOOLEAN     band_filter_on;
    uint16      start_arfcn;
    uint16      end_arfcn;
    
}FIXED_NVITEM_T;

typedef FIXED_NVITEM_T PS_CLASSMARK_T;

typedef PS_CLASSMARK_T *PS_CLASSMARK_PTR;

extern void	*NVITEM_GetClassmarkAddr(void);


#endif // end of __NV_CLASSMARK_H__
	
