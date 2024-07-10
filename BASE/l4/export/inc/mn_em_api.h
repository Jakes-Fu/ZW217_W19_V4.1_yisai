/******************************************************************************
 ** File Name:      mn_em_api.h                                               *
 ** Author:         george.yuan                                               *
 ** Date:           11/30/2010                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    this head file is for interface of mn engineering mode    *
 **                 to call by mmi eng dyna and MN self                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 **11/30/2010     george.yuan       Create.                                   *
 ******************************************************************************/

#if !defined (_MN_EM_API_H)
#define _MN_EM_API_H
#include "sci_types.h"

/* +cr213593,+cr247560 */
#include "rr_api.h"
/* -cr213593,-cr247560 */



#if 1
/********************************************************************
//  Function Name: mn_al_em_log_masking_callb_func
//  Description: the callbck function for log masking
********************************************************************/
extern uint32 mn_al_em_log_masking_callb_func(   uint8 **dest, /*output*/
                                                                        uint16 *dest_len, 
                                                                        const uint8 *src, /*input*/
                                                                        uint16 src_len);

/********************************************************************
//  Function Name: mn_al_em_call_service_callb_func
//  Description: the callbck function for calling service
********************************************************************/
extern uint32 mn_al_em_call_service_callb_func(   uint8 **dest, 
                                                                        uint16 *dest_len, 
                                                                        const uint8 *src, 
                                                                        uint16 src_len);
                                                                        
/********************************************************************
//   Function Name: mn_al_em_ps_service_callb_func
//  Description: the callbck function for ps service
********************************************************************/
extern  uint32 mn_al_em_ps_service_callb_func(   uint8 **dest, 
                                                                        uint16 *dest_len, 
                                                                        const uint8 *src, 
                                                                        uint16 src_len);

/********************************************************************
//  Function Name: l1_dsp_para_log_handler
//  Description: This function will return the add of param s_t312_param.
********************************************************************/
#ifndef WIN32
extern uint32 l1_dsp_para_log_handler(uint8 **dest, 
							   uint16 *dest_len, 
							   const uint8 *src, 
							   uint16 src_len);
#endif
/********************************************************************
//  Function Name: mm_em_param_req_callb_func
//  Description : when receiving EM_NAS _PARAM_REQ, mm will call the 
                         api for acquiring serving cell related information saved 
                         in nas. and send cnf for sending result to sme.
********************************************************************/
extern  uint32 mm_em_param_req_callb_func(   uint8 **dest, /*output*/
                                                                            uint16 *dest_len, 
                                                                            const uint8 *src, /*input*/
                                                                            uint16 src_len);

/********************************************************************
//  Function Name: l2_em_statinfo_handler
//  Description: This function is used to get l2 state infomation.
********************************************************************/
extern uint32 l2_em_statinfo_handler( uint8 **dest, /*output*/
                                   uint16 *dest_len, 
                                   const uint8 *src, /*input*/
                                   uint16 src_len );

/********************************************************************
//  Function Name: mm_em_get_identity_req_callb_func
//  Description: when receiving EM_GET_MOBILE_IDENTITY_REQ, mm will 
                        call the handler for acquiring some mobile identity info saved
                        in nas. and sent these result to sme through cnf message.
********************************************************************/
extern uint32 mm_em_get_identity_req_callb_func(   uint8 **dest, /*output*/
                                                                            uint16 *dest_len, 
                                                                            const uint8 *src, /*input*/
                                                                            uint16 src_len);

/********************************************************************
//  Function Name: rrc_bcfe_em_lock_freq_handler
//  Description: This function is used to lock freq
********************************************************************/
extern uint32 rrc_bcfe_em_lock_freq_handler( uint8 **dest, 
                                                                        uint16 *dest_len, 
                                                                        const uint8 *src, 
                                                                        uint16 src_len);

/********************************************************************
//  Function Name: rrc_dcfe_em_ue_state
//  Description: This function is used to get ue states
********************************************************************/
extern uint32 rrc_dcfe_em_ue_state(uint8         **dest, 
                                                               uint16        *dest_len, 
                                                               const uint8   *src, 
                                                               uint16        src_len);

/********************************************************************
//  Function Name: rrc_bcfe_em_process_cell_resel_info_handler
//  Description: This function is used to get cell reselction from rrc bcfe
********************************************************************/
extern uint32 rrc_bcfe_em_process_cell_resel_info_handler( uint8 **dest, 
                                                                        uint16 *dest_len, 
                                                                        const uint8 *src, 
                                                                        uint16 src_len);

/********************************************************************
//  Function Name: mmi_get_em_bcfe_statistic
//  Description: This function is used to get bcfe statistic info
********************************************************************/
extern void mmi_get_em_bcfe_statistic(ITEM_T *item_ptr, uint32 index);

/*CR187193*/
extern uint32 mmi_get_em_dcfe_pagecnt_btw_losc_and_oosa( int32 multi_sys );
extern BOOLEAN  MNGPRS_GetPPPLinkFlag(void);
extern void     MNGPRS_SetPPPLinkFlag(BOOLEAN ppp_lint_flag);
extern uint32   MNGPRS_GetPPPLinkIP(void);
extern void     MNGPRS_SetPPPLinkIP(uint32 ipaddr);
extern uint32   MNGPRS_GetPPPLinkDNS(void);
extern void     MNGPRS_SetPPPLintDNS(uint32 ipaddr);
extern uint32   MNGPRS_GetPPPLinkGateway(void);
extern void     MNGPRS_SetPPPLinkGateway(uint32 ipaddr);
extern uint32   MNGPRS_GetPPPLinkSubnetMask(void);
extern void     MNGPRS_SetPPPLinkSubnetMask(uint32 ipaddr);

/* +CR142255 */
/*extern uint32 mn_al_em_get_em_enable_flag();*/
extern void rlc_em_handle_statinfo (void);
extern void pdcp_em_stat_ind(void);
extern uint32 mm_em_param_req_timer_handler( int32 multi_sys );
extern uint32 mm_em_get_identity_timer_handler( int32 multi_sys );
extern uint32 rrc_dcfe_em_ue_state_timer_handler( int32 multi_sys );
extern uint32 rrc_bcfe_em_process_cell_resel_info_timer_handler( int32 multi_sys);
/* +CR144989*/

extern void rrc_bcfe_trr_reset_dm_test_param(void);
extern void TRR_dcfe_reset_params(uint8 index);



#if 0
extern kal_bool get_em_cfg_struct (em_cfg_struct  *em_cfg_params        
                        );
#endif
/* -CR144989*/   
/* -CR142255 */

/*****************************************************************************/
// 	Description : This is the interface function to get whether to allow logging 
//                of max download rate
//	Global resource dependence : none
//  Author:	Pizer.Fan
//	Note: 
/*****************************************************************************/
extern uint16  MNGPRS_GetMaxLogDlRate(void);

/*****************************************************************************/
// 	Description : This is the interface function to set whether to allow logging 
//                of max download rate
//	Global resource dependence : none
//  Author:	Pizer.Fan
//	Note: 
/*****************************************************************************/
extern void  MNGPRS_SetMaxLogDlRate(uint16 max_log_dl_rate);

extern void RLCIF_GetQosOfRlc(int32 multi_sys);


/********************************************************************
//   Function Name: MNDM_GetGsmStackTestarams
//  Description: 
// author: Martin.Jiang
********************************************************************/
void MNDM_GetStackTestParams(void);   

/*+cr151554*/
extern void TRR_GetEmParam(uint32 index,uint32 *value_ptr);
extern void TRR_SetEmParam(uint32 index,uint32 value);

/*-cr151554*/

/*+cr158980*/
void DualMode_TL1_SetEmParam(uint32 index, uint32 value);

void DualMode_TL1_GetEmParam(uint32 index, uint32 * value_ptr);
/*-cr158980*/
/*+CR169534 Additional Modification*/
extern void DualMode_TL1_ResetEmParam(void);
/*-CR169534 Additional Modification*/


/*+cr193169*/
extern void TRR_dcfe_set_params(uint8 index, uint32 value);
extern void TRR_dcfe_get_params(uint8 index, uint32 *value);
/*-cr193169*/
/*+CR216221*/
extern void TRR_SetEmParam_ex(uint32 index, void *value_ptr);
extern void TRR_GetEmParam_ex(uint32 index,void ** value_ptr);
/*-CR216221*/


/*+CR202264*/
/*-CR202264*/                    

/*+ add for customer check */

extern void mm_em_get_identity_req(ITEM_T *item_t_ptr,uint32 index);
/*- add for customer check */
#endif


extern PUBLIC uint32 mac_em_get_stat_req_count(void);

extern PUBLIC void mac_em_get_stat_req(ITEM_T *item, uint32  index);

 

extern PUBLIC uint32 rlc_em_get_stat_req_count(void);

extern PUBLIC void rlc_em_get_stat_req(ITEM_T *item_t_ptr,uint32 index);

 
extern PUBLIC uint32 pdcp_em_stat_get_item_count(void);

extern PUBLIC void pdcp_em_stat_get_item(ITEM_T *item_t_ptr,uint32 index);


extern PUBLIC uint32 mmi_get_em_lock_freq_count(void);

extern PUBLIC void mmi_get_em_lock_freq(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void mmi_set_em_lock_freq(ITEM_T *item_ptr, uint32 index);

 

extern PUBLIC uint32 mmi_get_em_lock_cell_count(void);

extern PUBLIC void mmi_get_em_lock_cell(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void mmi_set_em_lock_cell(ITEM_T *item_ptr, uint32 index);

 

extern PUBLIC uint32 mmi_get_em_serv_cell_info_count(void);

extern PUBLIC void mmi_get_em_serv_cell_info(ITEM_T *item_ptr, uint32 index);

 

extern PUBLIC uint32 mmi_get_em_neigbour_cell_count(void);

extern PUBLIC void mmi_get_em_neigbour_cell_info(ITEM_T *item_ptr, uint32 index);

 

extern PUBLIC uint32 mmi_get_em_cell_resel_num_count(void);

extern PUBLIC void mmi_get_em_cell_resel_num(ITEM_T *item_ptr, uint32 index);
 

extern PUBLIC void mm_em_get_identity_req(ITEM_T *item_ptr, uint32 index);

extern PUBLIC uint32 mm_em_get_identity_req_count(void);


extern PUBLIC void mm_em_get_param_req(ITEM_T *item_ptr, uint32 index);

 

extern PUBLIC uint32 mm_em_get_param_req_count(void);

/* +cr191140 */
extern PUBLIC void mm_em_get_abnormal_req(ITEM_T *item_ptr, uint32 index);

extern PUBLIC uint32 mm_em_get_abnormal_req_count(void);
/* -cr191140 */
/* +cr215513*/ 
extern PUBLIC void mm_em_get_lu_rau_num_req(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 mm_em_get_lu_rau_num_req_count(void);
/* -cr215513 */

extern PUBLIC uint32 SystemInfo_GetL1Meas_Count(void);

extern PUBLIC void SystemInfo_GetL1Meas_State_Req(ITEM_T *item_ptr, uint32 index);
 

extern PUBLIC uint32 SystemInfo_GetL1MeasOther_Count(void);

extern PUBLIC void SystemInfo_GetL1MeasOther_State_Req(ITEM_T *item_ptr, uint32 index);
 

extern PUBLIC uint32 SystemInfo_GetL1Power_Count(void);

extern PUBLIC void SystemInfo_GetL1Power_State_Req(ITEM_T *item_ptr, uint32 index);

 

extern PUBLIC uint32 SystemInfo_GetL1Delta_Count(void);

extern PUBLIC void SystemInfo_GetL1Delta_State_Req(ITEM_T *item_ptr, uint32 index);

 

extern PUBLIC uint32 SystemInfo_GetL1Ma_Count(void);

extern PUBLIC void SystemInfo_GetL1Ma_State_Req(ITEM_T *item_ptr, uint32 index);

 

extern PUBLIC uint32 SystemInfo_GetL1Bler_Count(void);

extern PUBLIC void SystemInfo_GetL1Bler_State_Req(ITEM_T *item_ptr, uint32 index);


/*+cr233014*/
extern PUBLIC uint32 em_get_l1_dsp_para_count(void);

extern PUBLIC void em_get_l1_dsp_para(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void em_set_l1_dsp_para(ITEM_T *item_ptr, uint32 index);
/*-cr233014*/


extern PUBLIC uint32 em_get_debug_log_stat_count(void);

extern PUBLIC void em_get_debug_log_stat(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void em_set_debug_log_stat(ITEM_T *item_ptr, uint32 index);



extern PUBLIC uint32 em_get_mask_log_count(void);

extern PUBLIC void em_get_mask_log(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void em_set_mask_log(ITEM_T *item_ptr, uint32 index);



extern PUBLIC uint32 rrc_dcfe_em_get_ue_state_count(void);

extern PUBLIC void rrc_dcfe_em_get_ue_state(ITEM_T *item_t_ptr, uint32 index);

//@yongwei.he add begin for cr95875
extern PUBLIC void em_get_auto_attach_power_on_flag(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void em_set_auto_attach_power_on_flag(ITEM_T *item_ptr, uint32 index);

extern PUBLIC uint32 em_get_auto_attach_power_on_flag_count(void);
//@yongwei.he add end for cr95875

//@yongwei.he add begin for cr9371
extern PUBLIC uint32 mmi_get_em_unlock_freq_cell_count(void);
extern PUBLIC void mmi_get_em_unlock_freq_cell(ITEM_T *item_ptr, uint32 index);
extern PUBLIC void mmi_set_em_unlock_freq_cell(ITEM_T *item_ptr, uint32 index);
//@yongwei.he add end for cr96371

/* +cr96947 */
extern PUBLIC void em_get_integrity_enable_flag(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void em_set_integrity_enable_flag(ITEM_T *item_ptr, uint32 index);

extern PUBLIC uint32 em_get_integrity_enable_flag_count(void);

/* +cr181390 */   
/*
extern PUBLIC void em_get_fta_opt_enable_flag(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void em_set_fta_opt_enable_flag(ITEM_T *item_ptr, uint32 index);

extern PUBLIC uint32 em_get_fta_opt_enable_flag_count(void);

extern PUBLIC void em_get_real_nw_opt_enable_flag(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void em_set_real_nw_opt_enable_flag(ITEM_T *item_ptr, uint32 index);

extern PUBLIC uint32 em_get_real_nw_opt_enable_flag_count(void);
*/
/* -cr181390 */   
/* -cr96947 */

/* +cr97057 */
/* +cr199401 delete AOC flag */
/*extern PUBLIC void em_get_aoc_enable_flag(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void em_set_aoc_enable_flag(ITEM_T *item_ptr, uint32 index);

extern PUBLIC uint32 em_get_aoc_enable_flag_count(void); */
/* -cr199401 delete AOC flag */
/* -cr97057 */

/*+cr97909*/
extern PUBLIC void em_get_attach_type(ITEM_T *item_ptr, uint32 index);
extern PUBLIC void em_execute_attach_with_specific_type(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_attach_type_count(void);


extern PUBLIC void em_get_detach_type(ITEM_T *item_ptr, uint32 index);
extern PUBLIC void em_execute_detach_with_specific_type(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_detach_type_count(void);


extern PUBLIC void em_get_pdp_activate_param(ITEM_T *item_ptr, uint32 index);
extern PUBLIC void em_execute_pdp_activate(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_pdp_activate_param_count(void);

extern PUBLIC void em_get_pdp_deactivate_param(ITEM_T *item_ptr, uint32 index);
extern PUBLIC void em_execute_pdp_deactivate(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_pdp_deactivate_param_count(void);
/*-cr97909*/

/*+CR105931*/
extern PUBLIC uint32 em_get_qos_param_count(void);

extern PUBLIC void em_get_browser_qos_param(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void em_set_browser_qos_param(ITEM_T *item_ptr, uint32 index);	
extern PUBLIC void em_get_mms_qos_param(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void em_set_mms_qos_param(ITEM_T *item_ptr, uint32 index);	
extern PUBLIC void em_get_download_qos_param(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void em_set_download_qos_param(ITEM_T *item_ptr, uint32 index);	
extern PUBLIC void em_get_streaming_qos_param(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void em_set_streaming_qos_param(ITEM_T *item_ptr, uint32 index);	
extern PUBLIC void em_get_im_qos_param(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void em_set_im_qos_param(ITEM_T *item_ptr, uint32 index);	
extern PUBLIC void em_get_poc_qos_param(ITEM_T *item_ptr, uint32 index);

extern PUBLIC void em_set_poc_qos_param(ITEM_T *item_ptr, uint32 index);	
/*-CR105931*/

/* +CR161092 */
extern PUBLIC uint32 em_get_dummy_nv_gsm_rr_param_count(void);
extern PUBLIC void em_get_gsm_rr_param(ITEM_T *item_ptr, uint32 index);
extern PUBLIC void em_set_gsm_rr_param(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_dummy_nv_gsm_layer1_param_count(void);
extern PUBLIC void em_get_gsm_layer1_param(ITEM_T *item_ptr, uint32 index);
extern PUBLIC void em_set_gsm_layer1_param(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_dummy_nv_td_bcfe_param_count(void);
extern PUBLIC void em_get_td_bcfe_param(ITEM_T *item_ptr, uint32 index);
extern PUBLIC void em_set_td_bcfe_param(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_dummy_nv_td_layer1_param_count(void);
extern PUBLIC void em_get_td_layer1_param(ITEM_T *item_ptr, uint32 index);
extern PUBLIC void em_set_td_layer1_param(ITEM_T *item_ptr, uint32 index);
/* -cr223863 */
//extern PUBLIC uint32 em_get_dummy_nv_reserved_param_count(void);    /*lint !e752 */
extern PUBLIC void em_get_reserved_param(ITEM_T *item_ptr, uint32 index);
//extern PUBLIC void em_set_reserved_param(ITEM_T *item_ptr, uint32 index);   /*lint !e752 */
/* -cr223863 */

extern PUBLIC uint32 em_get_cell_resel_stat_count(void);
extern PUBLIC void em_get_cell_resel_stat(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_cell_handover_stat_count(void);
extern PUBLIC void em_get_cell_handover_stat(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_out_of_serving_area_stat_count(void);
extern PUBLIC void em_get_out_of_serving_area_stat(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_cell_search_stat_count(void);
extern PUBLIC void em_get_cell_search_stat(ITEM_T *item_ptr, uint32 index);

extern PUBLIC uint32 mmi_get_em_neigbour_gsm_cell_count(void);
extern PUBLIC void mmi_get_em_neigbour_gsm_cell_info(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_neigbour_td_cell_count(void);
extern PUBLIC void em_get_neigbour_td_cell_info(ITEM_T *item_ptr, uint32 index);

extern PUBLIC uint32 em_get_gsm_monitor_item_count(void);
extern PUBLIC void em_get_gsm_monitor_item(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_td_monitor_item_count(void);
extern PUBLIC void em_get_td_monitor_item(ITEM_T *item_ptr, uint32 index);

/* -CR161092 */ 

/* +cr168406 */
PUBLIC uint32 em_get_2g_3g_time_count(void);
PUBLIC void em_get_2g_3g_time(ITEM_T *item_ptr, uint32 index);
/* -cr168406 */

/*+cr191140 */
extern PUBLIC uint32 em_get_rf_search_meas_count(void);
extern PUBLIC void em_get_rf_search_meas(ITEM_T *item_ptr, uint32 index);
/*-cr191140 */
/* +cr166898 */
extern PUBLIC void em_get_ps_test_mode(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_ps_test_mode_count(void);
extern PUBLIC void em_set_ps_test_mode(ITEM_T *item_ptr, uint32 index);
/* -cr166898 */
/* +cr169449 */

extern PUBLIC void em_get_stored_band_supp(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_stored_band_supp_count(void);
extern PUBLIC void em_set_stored_band_supp(ITEM_T *item_ptr, uint32 index);
/* -cr169449 */
/* +cr166784 */
/* +cr181390 */ 
/*
extern PUBLIC uint32 em_get_wakeup_condition_for_dsp_item_count(void);*/
extern PUBLIC void em_get_wakeup_condition_for_dsp_stat(ITEM_T *item_ptr, uint32 index);
/*extern PUBLIC void em_set_wakeup_condition_for_dsp_stat(ITEM_T *item_ptr, uint32 index);
*/
/* -cr181390 */  
/* -cr166784 */
/* +CR168886 */
extern PUBLIC void em_get_mem_deinit_assert(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_mem_deinit_assert_count(void);
extern PUBLIC void em_set_mem_deinit_assert(ITEM_T *item_ptr, uint32 index);
/* -CR168886 */

/* +cr198069 */ 
extern PUBLIC void em_get_amr2_enable(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_amr2_enable_count(void);
extern PUBLIC void em_set_amr2_enable(ITEM_T *item_ptr, uint32 index);
/* -cr198069 */

/* +CR180723 */
//extern PUBLIC void em_get_armlog_saveinUdisk(ITEM_T *item_ptr, uint32 index);   /*lint !e752 */
//extern PUBLIC uint32 em_get_armlog_saveinUdisk_count(void);     /*lint !e752 */
//extern PUBLIC void em_set_armlog_saveinUdisk(ITEM_T *item_ptr, uint32 index);   /*lint !e752 */
extern PUBLIC void em_get_armlog_dynamic_saveinUdisk(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_armlog_dynamic_saveinUdisk_count(void);
extern PUBLIC void em_set_armlog_dynamic_saveinUdisk(ITEM_T *item_ptr, uint32 index);
/* +CR180723 */

/* +cr171120 */
extern PUBLIC void em_get_dsp_log_switch(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_dsp_log_switch_count(void);
extern PUBLIC void em_set_dsp_log_switch(ITEM_T *item_ptr, uint32 index);
/* -cr1171120 */

/* +cr173855 */
extern PUBLIC void em_get_usim_drv_log_switch(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_usim_drv_log_switch_count(void);
extern PUBLIC void em_set_usim_drv_log_switch(ITEM_T *item_ptr, uint32 index);
/* -cr173855 */

/* +cr172713 */
extern PUBLIC void em_get_fband_enable(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_fband_enable_count(void);
extern PUBLIC void em_set_fband_enable(ITEM_T *item_ptr, uint32 index);
/* -cr172713 */

/* +CR200414 */
//extern PUBLIC void em_get_fta_opt_enable_flag(ITEM_T *item_ptr, uint32 index);
/* +CR200414 */

/* +cr201742 */  
PUBLIC uint32 em_get_tddo_enable_count(void);
PUBLIC void em_get_tddo_enable(ITEM_T *item_ptr, uint32 index);
PUBLIC void em_set_tddo_enable(ITEM_T *item_ptr, uint32 index);
/* -cr201742 */


/* +ncr121932 */  
extern PUBLIC void em_get_edge_enable(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_edge_enable_count(void);
extern PUBLIC void em_set_edge_enable(ITEM_T *item_ptr, uint32 index);
extern PUBLIC void em_get_hsdpa_hsupa_enable(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_hsdpa_hsupa_enable_count(void);
extern PUBLIC void em_set_hsdpa_hsupa_enable(ITEM_T *item_ptr, uint32 index);
extern PUBLIC void em_get_gsm_voice_code_format_enable(ITEM_T *item_ptr, uint32 index);
extern PUBLIC uint32 em_get_gsm_voice_code_format_enable_count(void);
extern PUBLIC void em_set_gsm_voice_code_format_enable(ITEM_T *item_ptr, uint32 index);
/* -ncr121932 */

/*****************************************************************************/
// 	Description: Send GSM Parameter to computer
//	Global resource dependence : none
//  Author: jinqiu.zhu
//  Note:
/*****************************************************************************/
extern void RRENG_Em_Process_GSM_Param_Handler(int32 multi_sys);
/*****************************************************************************/
// 	Description: Engineer Mode Phase2: Send GSM Parameter to computer
//	Global resource dependence : none
//  Author: Yu Sun
//  Note:
/*****************************************************************************/
extern void RRENG_Em2_Process_GSM_Param_Handler(int32 multi_sys);

#endif /* end of _MN_EM_API_H */
