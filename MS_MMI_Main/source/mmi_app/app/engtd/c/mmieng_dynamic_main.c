/*****************************************************************************
** File Name:      mmieng_Dynamic_main.c                                     *
** Author:         yongwei.he                                                *
** Date:           2007-5-18                                                 *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe dynamic engineering mode,   *
based on sword kim, from Lenovo mobile, Incorporated.                    
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007-6-20      yongwei.he       add 6 items for zte test 
******************************************************************************/
//#include "std_header.h"

#define _MMIENG_DYNAMIC_MAIN_C_

#include "mmi_app_engtd_trc.h"
#ifdef ENGTD_SUPPORT
/**--------------------------------------------------------------------------*
Include Files      
**--------------------------------------------------------------------------*/
#include "mmieng_Dynamic_main.h"
#include "mmiengtd_text.h"
#include "mmiengtd_id.h"
//#include "engineer_zte.h"
#include "mmiengtd_win.h"
#include "mmieng_win.h"
#include "guiedit.h"

#include "mn_em_api.h"
#include "mmiengtd_internal.h"

/**--------------------------------------------------------------------------*
			EXTERNAL DECLARE
 **--------------------------------------------------------------------------*/
//extern const MMI_ENG_DISPLAY_T mmi_eng_display;

/**--------------------------------------------------------------------------*
					MACRO DEFINITION
**--------------------------------------------------------------------------*/

#define FORM_MMI_STRING(MMI_STRING,STR_P,IS_UCS2)\
	{\
	MMI_STRING.wstr_ptr=STR_P;\
	MMI_STRING.wstr_len=MMIAPICOM_Wstrlen(STR_P);\
	}

#define FORM_GUILIST_ITEM(ITEMDATA,STR,LEN,IS_UCS2,SF1,SF2,SF3,IMG_ID)\
{\
	SCI_MEMSET(&ITEMDATA, 0, sizeof(ITEMDATA));\
	ITEMDATA.item_content[0].item_data.text_buffer.wstr_len =MIN( (uint16)LEN, GUILIST_STRING_MAX_NUM);\
	ITEMDATA.item_content[0].item_data.text_buffer.wstr_ptr=STR;\
	ITEMDATA.softkey_id[0] = SF1;\
	ITEMDATA.softkey_id[1] = SF2;\
	ITEMDATA.softkey_id[2] = SF3;\
	ITEMDATA.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;\
}

LOCAL uint8 s_mmieng_dyna_timer_id = 0;	//@yongwei.he add
/**--------------------------------------------------------------------------*
LOCAL FUNCTION DECLARE
**--------------------------------------------------------------------------*/
/********************************************************************************
NAME:			DynaEngMainMenuWinHandleMsg
DESCRIPTION:	
AUTHOR:	yongwei.he		
DATE:		2007-5-18	
********************************************************************************/
LOCAL MMI_RESULT_E DynaEngMainMenuWinHandleMsg(
											   MMI_WIN_ID_T win_id,
											   MMI_MESSAGE_ID_E msg_id,
											   DPARAM param
											   );
/********************************************************************************
NAME:			DynaEngShowParamValueWinHandleMsg
DESCRIPTION:	
AUTHOR:	yongwei.he		
DATE:		2007-5-18	
********************************************************************************/
LOCAL MMI_RESULT_E DynaEngShowParamValueWinHandleMsg(
											   MMI_WIN_ID_T win_id,
											   MMI_MESSAGE_ID_E msg_id,
											   DPARAM param
											   );
/********************************************************************************
NAME:			DynaEngItemListWinHandleMsg
DESCRIPTION:	
AUTHOR:	yongwei.he		
DATE:		2007-5-18	
********************************************************************************/
LOCAL MMI_RESULT_E DynaEngItemListWinHandleMsg(
											   MMI_WIN_ID_T win_id,
											   MMI_MESSAGE_ID_E msg_id,
											   DPARAM param
											   );
/********************************************************************************
NAME:			DynaEngEditWinHandleMsg
DESCRIPTION:	
AUTHOR:	yongwei.he		
DATE:		2007-5-18	
********************************************************************************/
LOCAL MMI_RESULT_E DynaEngEditWinHandleMsg(
										   MMI_WIN_ID_T win_id,
										   MMI_MESSAGE_ID_E msg_id,
										   DPARAM param
										   );
										   /********************************************************************************
										   NAME:			DynaEngValueListWinHandleMsg
										   DESCRIPTION:	
										   AUTHOR:	yongwei.he		
										   DATE:		2007-5-18	
********************************************************************************/
LOCAL MMI_RESULT_E DynaEngValueListWinHandleMsg(
												MMI_WIN_ID_T win_id,
												MMI_MESSAGE_ID_E msg_id,
												DPARAM param
												);

LOCAL void FindSubArrIndexByNodeID( uint16 * subfunc_arr_node_index,uint32 node_id);
LOCAL BOOLEAN CheckParam(ITEM_T* item);

/**--------------------------------------------------------------------------*
TYPE AND CONSTANT
**--------------------------------------------------------------------------*/
//MMI 实现操作菜单结构
//主功能ID 
typedef enum 
{
	L1_INFO,
	GMM_PARAM,
	L2_STAT_INFO, 
	UE_IDENTITY,
	LOCK_FREQ,
	GET_RRC_STATE,
	CELL_SEL_RESEL,
	MASK_LOG,
/* +cr181390 */  
	/*TURN_ON_OFF_LOG,*/
/* -cr181390 */  	
//@yongwei.he add begin for cr95875	
	AUTO_ATTACH_POWER_ON,
//@yongwei.he add end for cr95875

/* +CR96974 george add if 0 10.01*/
#if 0
/* -CR96974 */
//@yongwei.he add begin for cr96371	
        UNLOCK_FREQ_CELL,
//@yongwei.he add begin for cr96371	
/* +CR96974 */
#endif
/* -CR96974 */
      /* +CR96974 */
      INTEGRITY_ENABLE_FLAG,
/* +cr181390 */       
      /*FTA_OPT_ENABLE_FLAG,
      REAL_NW_OPT_ENABLE_FLAG,*/
/* -cr181390 */       
      /* -CR96974 */
    /* +CR105216 */
    QOS_INFO,
    /* -CR105216 */ 
    /* +CR161092 */
    DUMMY_NV_OF_DM,
    DM_PROCESS_STAT,
    /* -CR161092 */

    /* +cr166898 */
    PS_TEST_MODE,
    /* -cr166898 */

    /* +CR168886 */
    PS_MEM_DEINIT_ASSERT,
    /* -CR168886 */
    /* +CR180723 */
    SAVE_SMEBUFF_UDISK,
    /* -CR180723 */
/* +cr181390 */   
    /*WAKEUP_CONDITION_FOR_DSP, //cr166784*/
/* -cr181390 */       
	DYNAENG_MAINFUNC_MAX_ID
}DYNAENG_MAINFUNC_ID_E;     /*lint !e751 */


//主功能列表，包括对应ID，名称

 DYNAENG_MAINFUNC_T g_dynaeng_mainfunc_arr[] = 

{
/*lint -save -e785 */
       {L1_INFO, L"l1 info"},

       {GMM_PARAM, L"mm/gmm info"},

       {L2_STAT_INFO,L"l2 statistic info"},

       {UE_IDENTITY, L"ue identity"},

       {LOCK_FREQ, L"lock freq"},

       {GET_RRC_STATE, L"get rrc state"},

       {CELL_SEL_RESEL, L"cell sel resel"},

	{MASK_LOG, L"mask log"},
/* +cr181390 */  	
       /*{TURN_ON_OFF_LOG,L"turn on/off log"},*/
/* +cr181390 */         
//@yongwei.he add begin for cr95875       
       {AUTO_ATTACH_POWER_ON, L"auto attach power on flag"},
//@yongwei.he add end for cr95875 
//@yongwei.he add begin for cr96371	
//        {UNLOCK_FREQ_CELL,  "unlock freq cell"},
//@yongwei.he add end for cr96371
        /* +cr96947 */
        {INTEGRITY_ENABLE_FLAG, L"integrity, aoc etc flag"},  /* george added aoc*/
/* +cr181390 */           
        /*{FTA_OPT_ENABLE_FLAG, L"fta_opt enable flag"},
        {REAL_NW_OPT_ENABLE_FLAG, L"real_nw_opt enable flag"},*/
/* -cr181390 */           
        /* -cr96947 */
	    /* +CR105216 */
	    {QOS_INFO, L"qos info"},
	    /*lint -restore */
	    /* -CR105216 */ 
        /* +CR161092 */
        {DUMMY_NV_OF_DM, L"dm dummy nv info"},
        {DM_PROCESS_STAT, L"dm process stat"},
        /* -CR161092 */

        /* +cr166898 */        
        /* +cr181390 */
        {PS_TEST_MODE, L"debug mode"},
        /* -cr166898 */        
        /*{WAKEUP_CONDITION_FOR_DSP, L"rrm test"},     //cr166784*/
        /* -cr181390 */
        /* +CR168886 */
        {PS_MEM_DEINIT_ASSERT, L"PS mem assert"},
        /* -CR168886 */
        /* +CR180723 */
        {SAVE_SMEBUFF_UDISK, L"save armlog in sdCard"},
        /* -CR180723 */
}; 

#if 0 /*#ifndef _WIN32*/
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
extern PUBLIC uint32 em_get_wakeup_condition_for_dsp_item_count(void);
extern PUBLIC void em_get_wakeup_condition_for_dsp_stat(ITEM_T *item_ptr, uint32 index);
extern PUBLIC void em_set_wakeup_condition_for_dsp_stat(ITEM_T *item_ptr, uint32 index);
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
#endif

PUBLIC uint32 em_get_2g_3g_time_count(void);
PUBLIC void em_get_2g_3g_time(ITEM_T *item_ptr, uint32 index);
//子功能列表，包括对应主功能ID，子功能名称，Get 函数，Set 函数

 DYNAENG_SUBFUNC_T g_dynaeng_subfunc_arr[] = 

{ 
/*lint -save -e785 */
    {L1_INFO, L"l1 meas", SystemInfo_GetL1Meas_Count, SystemInfo_GetL1Meas_State_Req, NULL},

    {L1_INFO, L"l1 meas other", SystemInfo_GetL1MeasOther_Count, SystemInfo_GetL1MeasOther_State_Req, NULL},

    {L1_INFO, L"l1 power", SystemInfo_GetL1Power_Count, SystemInfo_GetL1Power_State_Req, NULL},

    {L1_INFO, L"l1 delta", SystemInfo_GetL1Delta_Count, SystemInfo_GetL1Delta_State_Req, NULL},

    {L1_INFO, L"l1 ma", SystemInfo_GetL1Ma_Count, SystemInfo_GetL1Ma_State_Req, NULL},

    {L1_INFO, L"l1 bler", SystemInfo_GetL1Bler_Count, SystemInfo_GetL1Bler_State_Req, NULL},
    
/*+cr233014*/
    {L1_INFO, L"l1 dsp para", em_get_l1_dsp_para_count, em_get_l1_dsp_para, em_set_l1_dsp_para},
/*-cr233014*/
 

    {GMM_PARAM, L"nas param", mm_em_get_param_req_count, mm_em_get_param_req, NULL},
    /* +cr191140 */
    {GMM_PARAM, L"nas abnormal info", mm_em_get_abnormal_req_count, mm_em_get_abnormal_req, NULL},
    /* -cr191140 */

    /* +cr215513 */
    {GMM_PARAM, L"lu and rau number", mm_em_get_lu_rau_num_req_count, mm_em_get_lu_rau_num_req, NULL},
    /* -cr215513 */

    {L2_STAT_INFO, L"MAC state info", mac_em_get_stat_req_count, mac_em_get_stat_req, NULL},

    {L2_STAT_INFO, L"RLC state info", rlc_em_get_stat_req_count, rlc_em_get_stat_req, NULL},

    {L2_STAT_INFO, L"PDCP state info", pdcp_em_stat_get_item_count, pdcp_em_stat_get_item, NULL},


    {UE_IDENTITY, L"ue identity", mm_em_get_identity_req_count, mm_em_get_identity_req, NULL},
      

    {LOCK_FREQ, L"lock freq", mmi_get_em_lock_freq_count, mmi_get_em_lock_freq, mmi_set_em_lock_freq} ,

    {LOCK_FREQ, L"lock cell", mmi_get_em_lock_cell_count, mmi_get_em_lock_cell, mmi_set_em_lock_cell} ,
    //@yongwei.he add begin for cr96371	
    {LOCK_FREQ, L"unlock freq", mmi_get_em_unlock_freq_cell_count, mmi_get_em_unlock_freq_cell, mmi_set_em_unlock_freq_cell},
    //@yongwei.he add end for cr96371	

    {GET_RRC_STATE, L"get rrc state", rrc_dcfe_em_get_ue_state_count, rrc_dcfe_em_get_ue_state, NULL} ,

    
    {CELL_SEL_RESEL, L"service cell ", mmi_get_em_serv_cell_info_count,mmi_get_em_serv_cell_info, NULL },

    {CELL_SEL_RESEL, L"neighbour cell", mmi_get_em_neigbour_cell_count,mmi_get_em_neigbour_cell_info, NULL },

    {CELL_SEL_RESEL, L"cell resel num", mmi_get_em_cell_resel_num_count,mmi_get_em_cell_resel_num, NULL },
    /* +CR161092 */
    {CELL_SEL_RESEL, L"gsm mead config and gsm neigbour cell", mmi_get_em_neigbour_gsm_cell_count,mmi_get_em_neigbour_gsm_cell_info, NULL },
    {CELL_SEL_RESEL, L"td mead config and td neigbour cell", em_get_neigbour_td_cell_count,em_get_neigbour_td_cell_info, NULL },    
    /* -CR161092 */
	  
    {MASK_LOG, L"mask log", em_get_mask_log_count, em_get_mask_log, em_set_mask_log},
    

/* +cr181390 */    
    /*{TURN_ON_OFF_LOG, L"turn on/off log", em_get_debug_log_stat_count, em_get_debug_log_stat, em_set_debug_log_stat},*/
/* -cr181390 */   	    

    {AUTO_ATTACH_POWER_ON, L"auto attach power on flag", em_get_auto_attach_power_on_flag_count, em_get_auto_attach_power_on_flag, em_set_auto_attach_power_on_flag},

    /* +cr96947 */
    {INTEGRITY_ENABLE_FLAG, L"integrity enable flag", em_get_integrity_enable_flag_count, em_get_integrity_enable_flag, em_set_integrity_enable_flag},	

    /* +cr97057 */
    /*{INTEGRITY_ENABLE_FLAG, L"aoc enable flag", em_get_aoc_enable_flag_count, em_get_aoc_enable_flag, em_set_aoc_enable_flag},	*/ /* cr199401 */
    /* -cr97057*/
     /*+cr97909*/
    {INTEGRITY_ENABLE_FLAG, L"attach with specific type", em_get_attach_type_count, em_get_attach_type, em_execute_attach_with_specific_type},
    {INTEGRITY_ENABLE_FLAG, L"detach with specific type", em_get_detach_type_count, em_get_detach_type, em_execute_detach_with_specific_type},
    {INTEGRITY_ENABLE_FLAG, L"pdp activate", em_get_pdp_activate_param_count, em_get_pdp_activate_param, em_execute_pdp_activate},
    {INTEGRITY_ENABLE_FLAG, L"pdp deactivate", em_get_pdp_deactivate_param_count, em_get_pdp_deactivate_param, em_execute_pdp_deactivate},
    /*-cr97909*/

	/* +cr198069 */  
	{INTEGRITY_ENABLE_FLAG, L"amr2 enable", em_get_amr2_enable_count, em_get_amr2_enable, em_set_amr2_enable},
	/* -cr198069 */



/* +cr181390 */
    /*{FTA_OPT_ENABLE_FLAG, L"fta_opt enable flag", em_get_fta_opt_enable_flag_count, em_get_fta_opt_enable_flag, em_set_fta_opt_enable_flag},	
    {REAL_NW_OPT_ENABLE_FLAG, L"real_nw_opt enable flag", em_get_real_nw_opt_enable_flag_count, em_get_real_nw_opt_enable_flag, em_set_real_nw_opt_enable_flag},*/
/* -cr181390 */
    /* -cr96947 */
	/*+CR105931*/
	{QOS_INFO, L"browser qos", em_get_qos_param_count, em_get_browser_qos_param, em_set_browser_qos_param},
	{QOS_INFO, L"mms qos", em_get_qos_param_count, em_get_mms_qos_param, em_set_mms_qos_param},
	{QOS_INFO, L"download qos", em_get_qos_param_count, em_get_download_qos_param, em_set_download_qos_param},
	{QOS_INFO, L"streaming qos", em_get_qos_param_count, em_get_streaming_qos_param, em_set_streaming_qos_param},
	{QOS_INFO, L"im qos", em_get_qos_param_count, em_get_im_qos_param, em_set_im_qos_param},
	{QOS_INFO, L"poc qos", em_get_qos_param_count, em_get_poc_qos_param, em_set_poc_qos_param},
	/*-CR105931*/
    /* +CR161092 */
	{DUMMY_NV_OF_DM, L"gsm rr", em_get_dummy_nv_gsm_rr_param_count, em_get_gsm_rr_param, em_set_gsm_rr_param},
	{DUMMY_NV_OF_DM, L"gsm layer1", em_get_dummy_nv_gsm_layer1_param_count, em_get_gsm_layer1_param, em_set_gsm_layer1_param},
	{DUMMY_NV_OF_DM, L"td bcfe", em_get_dummy_nv_td_bcfe_param_count, em_get_td_bcfe_param, em_set_td_bcfe_param},
	{DUMMY_NV_OF_DM, L"td layer1", em_get_dummy_nv_td_layer1_param_count, em_get_td_layer1_param, em_set_td_layer1_param},
    /* +cr223863 */
	//{DUMMY_NV_OF_DM, L"reserved", em_get_dummy_nv_reserved_param_count, em_get_reserved_param, em_set_reserved_param},
    /* -cr223863 */
	
	{DM_PROCESS_STAT, L"cell resel stat", em_get_cell_resel_stat_count, em_get_cell_resel_stat, NULL},
	{DM_PROCESS_STAT, L"cell handover stat", em_get_cell_handover_stat_count, em_get_cell_handover_stat, NULL},
	{DM_PROCESS_STAT, L"out of serving area stat", em_get_out_of_serving_area_stat_count, em_get_out_of_serving_area_stat, NULL},
	{DM_PROCESS_STAT, L"cell search stat", em_get_cell_search_stat_count, em_get_cell_search_stat, NULL},

    {DM_PROCESS_STAT, L"l1 gsm monitor in td", em_get_gsm_monitor_item_count, em_get_gsm_monitor_item, NULL},
    {DM_PROCESS_STAT, L"l1 td monitor in gsm", em_get_td_monitor_item_count, em_get_td_monitor_item, NULL},    

    /* -CR161092 */ 

    /* +cr168406 */
    {DM_PROCESS_STAT, L"2g 3g time", em_get_2g_3g_time_count, em_get_2g_3g_time, NULL},    
    /* +cr168406 */

    /*+cr191140 */
    {DM_PROCESS_STAT, L"RF search meas", em_get_rf_search_meas_count, em_get_rf_search_meas, NULL},    
    /*-cr191140 */

    /* +cr166898 */
    /* +cr181390 */
    {PS_TEST_MODE, L"debug mode", em_get_ps_test_mode_count, em_get_ps_test_mode, em_set_ps_test_mode},
    /* +cr181390 */
    /* +cr172713 */
    {PS_TEST_MODE, L"F band capability config", em_get_fband_enable_count, em_get_fband_enable, em_set_fband_enable},
    /* -cr172713 */

    /* +cr169449 */

    {PS_TEST_MODE, L"F band OLPC config", em_get_stored_band_supp_count, em_get_stored_band_supp, em_set_stored_band_supp},
    /* -cr169449 */
    /* -cr166898 */
    /* +cr171120 */
    {PS_TEST_MODE, L"DSP log switch", em_get_dsp_log_switch_count, em_get_dsp_log_switch, em_set_dsp_log_switch},
    /* -cr171120 */
/* +cr173855 */
    {PS_TEST_MODE, L"USIM drv log switch", em_get_usim_drv_log_switch_count, em_get_usim_drv_log_switch, em_set_usim_drv_log_switch},
/* -cr173855 */
/* +cr181390 */    
    {PS_TEST_MODE, L"ARM log switch", em_get_debug_log_stat_count, em_get_debug_log_stat, em_set_debug_log_stat},
/* -cr181390 */

    /* +cr201742 */  
	{PS_TEST_MODE, L"TDDO switch", em_get_tddo_enable_count, em_get_tddo_enable, em_set_tddo_enable},
	/* -cr201742 */

    /* +CR168886 */
/* +cr181390 */
    /*{WAKEUP_CONDITION_FOR_DSP, L"rrm test", em_get_wakeup_condition_for_dsp_item_count, em_get_wakeup_condition_for_dsp_stat, em_set_wakeup_condition_for_dsp_stat},*/
/* -cr181390 */
    {PS_MEM_DEINIT_ASSERT, L"PS mem deinit assert", em_get_mem_deinit_assert_count, em_get_mem_deinit_assert, em_set_mem_deinit_assert},
    /* -CR168886 */
/* +CR180723 */
    /*{SAVE_SMEBUFF_UDISK, L"static save armlog in sdCard", em_get_armlog_saveinUdisk_count, em_get_armlog_saveinUdisk, em_set_armlog_saveinUdisk},*/
    {SAVE_SMEBUFF_UDISK, L"dynamic save armlog in sdCard", em_get_armlog_dynamic_saveinUdisk_count, em_get_armlog_dynamic_saveinUdisk, em_set_armlog_dynamic_saveinUdisk}
/* -CR180723 */
/*lint -restore */
}; 


#define DYNAENG_MAINFUNC_ARR_COUNT  (sizeof(g_dynaeng_mainfunc_arr)/sizeof(DYNAENG_MAINFUNC_T))
#define DYNAENG_SUBFUNC_ARR_COUNT    (sizeof(g_dynaeng_subfunc_arr)/sizeof(DYNAENG_SUBFUNC_T))

static SELECTED_DATA_T selected_data;


//main menu from dynamic menutable
WINDOW_TABLE(MMIENG_DYNA_MAIN_MENU_WIN_TAB) =
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_TITLE( TXT_ENG_DYNA_MODE),
	WIN_FUNC((uint32)DynaEngMainMenuWinHandleMsg),
	WIN_ID(MMIENG_DYNA_MAIN_MENU_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	END_WIN
};

//param list window
WINDOW_TABLE(MMIENG_DYNA_ITEM_LIST_WIN_TAB) =
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)DynaEngItemListWinHandleMsg),
	WIN_ID(MMIENG_DYNA_ITEM_LIST_WIN_ID),
    WIN_TITLE(TXT_NULL),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_DYNA_ITEM_LIST_LISTBOX_CTRL_ID),       
	END_WIN
};
//show param value window
WINDOW_TABLE(MMIENG_DYNA_SHOW_PARAM_VALUE_WIN_TAB) =
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)DynaEngShowParamValueWinHandleMsg),
	WIN_ID(MMIENG_DYNA_SHOW_PARAM_VALUE_WIN_ID),
    WIN_TITLE(TXT_NULL),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_TEXT_CTRL(MMIENG_DYNA_PARAM_VALUE_TEXTBOX_CTRL_ID),
    END_WIN
};

//param value list window
WINDOW_TABLE(MMIENG_DYNA_VALUE_LIST_WIN_TAB) =
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)DynaEngValueListWinHandleMsg),
	WIN_ID(MMIENG_DYNA_VALUE_LIST_WIN_ID),
    WIN_TITLE(TXT_NULL),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_DYNA_VALUE_LIST_LISTBOX_CTRL_ID),        			
	END_WIN
};

//edit window
WINDOW_TABLE(MMIENG_DYNA_EDIT_WIN_TAB) =
{
	WIN_FUNC((uint32)DynaEngEditWinHandleMsg),
	WIN_ID(MMIENG_DYNA_EDIT_WIN_ID),
    WIN_TITLE(TXT_NULL),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
	
	CREATE_EDIT_DIGITAL_CTRL(5,MMIENG_DYNA_VALUE_EDITBOX_CTRL_ID),
	END_WIN
};

/*===============================================================================
IMPLEMENTATION
===============================================================================*/
/********************************************************************************
NAME:			MMIEng_Dynamic_main
DESCRIPTION:	
AUTHOR:	yongwei.he		
DATE:		2007-5-18	
********************************************************************************/
void MMIEng_Dynamic_main(void)
{
    GUI_BOTH_RECT_T      both_rect = {0};

    MMK_CreateWin((uint32*) MMIENG_DYNA_MAIN_MENU_WIN_TAB, PNULL );
    
    both_rect = MMITHEME_GetWinClientBothRect(MMIENG_DYNA_MAIN_MENU_WIN_ID);
    //creat dynamic menu
    /* +cr180723 */
    GUIMENU_CreatDynamicEx(80,&both_rect,MMIENG_DYNA_MAIN_MENU_WIN_ID,MMIENG_DYNA_MENU_CTRL_ID,GUIMENU_STYLE_THIRD);
    /* -cr180723 */
}

/********************************************************************************
NAME:			DynaEngMainMenuWinHandleMsg
DESCRIPTION:	
AUTHOR:	yongwei.he		
DATE:		2007-5-18	
********************************************************************************/
LOCAL MMI_RESULT_E DynaEngMainMenuWinHandleMsg(
											   MMI_WIN_ID_T win_id,
											   MMI_MESSAGE_ID_E msg_id,
											   DPARAM param
											   )
{
	//MMI_CTRL_ID_T ctrl_id = MMIENG_DYNA_MENU_CTRL_ID;
	uint16 i,j = 0;
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	//uint16 cur_idx = 0;
	MMI_STRING_T ktext;
    GUIMENU_DYNA_ITEM_T node_item = {0};
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
		{
			GUI_MENUTREE_NODE_DATA_T menu_data={0};
			MMI_STRING_T title_text = {0};
			MMI_GetLabelTextByLang(TXT_ENG_DYNA_MODE, &title_text);
			//uint8 menu_data_count=sizeof(g_dynaeng_mainfunc_arr)*sizeof(g_dynaeng_subfunc_arr);
			//GUI_MENUTREE_NODE_DATA_T * menu_data=SCI_ALLOC_APP(sizeof(GUI_MENUTREE_NODE_DATA_T)*menu_data_count);
			GUIMENU_SetMenuTitle( &title_text,  MMIENG_DYNA_MENU_CTRL_ID);
			for (i = 0; i <DYNAENG_MAINFUNC_ARR_COUNT; i++)
			{
				//menu_data.name_ptr=g_dynaeng_mainfunc_arr[i].name;
				menu_data.parent_node_id=0;
				menu_data.index=i;
				menu_data.node_id++;
			
				ktext.wstr_len = MMIAPICOM_Wstrlen(g_dynaeng_mainfunc_arr[i].name);
				ktext.wstr_ptr =g_dynaeng_mainfunc_arr[i].name;
				//插入节点
                node_item.item_text_ptr = &ktext;
				GUIMENU_InsertNode( menu_data.index,//顺序号
					menu_data.node_id, //节点编号
					menu_data.parent_node_id, //父节点编号
					&node_item, //文本
					MMIENG_DYNA_MENU_CTRL_ID);
				
				//now main menu node id is the parent node id of sub menu
				menu_data.parent_node_id=menu_data.node_id;
				
				for (j = 0; j< DYNAENG_SUBFUNC_ARR_COUNT; j++)		
				{
					if(g_dynaeng_mainfunc_arr[i].id==g_dynaeng_subfunc_arr[j].parent_id)
					{
						//menu_data.name_ptr=g_dynaeng_subfunc_arr[j].name;
						
						
						menu_data.index=menu_data.node_id-menu_data.parent_node_id;
						menu_data.node_id++;
						
						
					
						ktext.wstr_len = MMIAPICOM_Wstrlen(g_dynaeng_subfunc_arr[j].name);
						ktext.wstr_ptr = g_dynaeng_subfunc_arr[j].name;
						//插入节点
                        node_item.item_text_ptr = &ktext;
						GUIMENU_InsertNode( menu_data.index,//顺序号
							menu_data.node_id, //节点编号
							menu_data.parent_node_id, //父节点编号
							&node_item, //文本
							MMIENG_DYNA_MENU_CTRL_ID);						
					}
				}		
			}	
			MMK_SetAtvCtrl(win_id, MMIENG_DYNA_MENU_CTRL_ID);
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
		}
		break;

	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
	
	case MSG_APP_OK:
  case MSG_CTL_OK:
  case MSG_APP_WEB:
  case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		{
			uint16 node_id = GUIMENU_GetCurNodeId(MMIENG_DYNA_MENU_CTRL_ID);
			FindSubArrIndexByNodeID((uint16 *)&selected_data.dynaeng_subfunc_arr_index, (uint32)node_id);
			MMK_CreateWin((uint32*) MMIENG_DYNA_SHOW_PARAM_VALUE_WIN_TAB, PNULL );
			
		}
		break;
		
		
	default:
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;
}

/********************************************************************************
NAME:			DynaEngItemListWinHandleMsg
DESCRIPTION:	
AUTHOR:	yongwei.he		
DATE:		2007-5-18	
********************************************************************************/
LOCAL MMI_RESULT_E DynaEngItemListWinHandleMsg(
											   MMI_WIN_ID_T win_id,
											   MMI_MESSAGE_ID_E msg_id,
											   DPARAM param
											   )
{
	uint16 i = 0;
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
	//uint16 cur_idx = 0;

	static GetEngItemCount getcountfunc = PNULL;
	static GetEngItem getfunc = PNULL;
	static SetEngItem setfunc = PNULL;
	uint16 subfunc_arr_index=selected_data.dynaeng_subfunc_arr_index;
	
	//SCI_TRACE_LOW:"DynaEngItemListWinHandleMsg msg_id = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_DYNAMIC_MAIN_889_112_2_18_2_18_31_0,(uint8*)"d", msg_id);
	
	switch(msg_id)
	{
	case MSG_FULL_PAINT:
		
		GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
	
		break;
	case MSG_OPEN_WINDOW:
		{
			uint8 count;
			//uint32 parent_node_id = 5;
			//uint32 node_id;
			MMI_STRING_T text_title = {0};
			ITEM_T item={0};
			GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
			GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
            uint16 item_buf[MAX_DYNAENG_STR_LENGTH]={0}; 
            MMI_STRING_T ktext = {0} ;	
            uint16 length = 0;
            //SCI_TRACE_LOW:"DynaEngItemListWinHandleMsg subfunc_arr_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_DYNAMIC_MAIN_910_112_2_18_2_18_31_1,(uint8*)"d", subfunc_arr_index);
 			FORM_MMI_STRING(text_title, g_dynaeng_subfunc_arr[subfunc_arr_index].name, FALSE);/*lint !e64*/
                    
                    
			GUIWIN_SetTitleText(win_id, text_title.wstr_ptr, text_title.wstr_len, FALSE);
			//get sub array index by menu node id
			//GetSubArrIndexByNodeID(&subfunc_arr_index, parent_node_id);
			getcountfunc = g_dynaeng_subfunc_arr[subfunc_arr_index].getcountfunc; //获得item 个数的函数 
			getfunc = g_dynaeng_subfunc_arr[subfunc_arr_index].getfunc; //获得指定index 的item 内容的函数 
			setfunc = g_dynaeng_subfunc_arr[subfunc_arr_index].setfunc;
			//SCI_TRACE_LOW:"DynaEngItemListWinHandleMsg getcountfunc = %d, getfunc = %d, setfunc = %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_DYNAMIC_MAIN_920_112_2_18_2_18_31_2,(uint8*)"ddd", getcountfunc, getfunc,setfunc);
			count = getcountfunc();
			GUILIST_SetMaxItem(MMIENG_DYNA_ITEM_LIST_LISTBOX_CTRL_ID , count, FALSE );
			//GUILIST_SetTitle(MMIENG_DYNA_ITEM_LIST_LISTBOX_CTRL_ID, GUI_TITLE_PARAM_T * title_ptr)
            //SCI_TRACE_LOW:"DynaEngItemListWinHandleMsg getcountfunc = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_DYNAMIC_MAIN_924_112_2_18_2_18_31_3,(uint8*)"d", count);
			for (i = 0; i <count; i++)
			{     			    
				item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
				item_t.item_data_ptr = &item_data;
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
                SCI_MEMSET(&item,0,sizeof(ITEM_T));
				SCI_MEMSET(item_buf,0,sizeof(uint16)*MAX_DYNAENG_STR_LENGTH);
				getfunc(&item,i);
                           
				MMI_STRNTOWSTR((wchar*)item_buf,MAX_DYNAENG_STR_LENGTH,(char*)item.item_name,sizeof(item.item_name),strlen(item.item_name));/*lint !e64 */
				FORM_MMI_STRING(ktext,item_buf, FALSE);/*lint !e64*/
                length = MMIAPICOM_Wstrlen((const wchar *)item_buf) ;
				FORM_GUILIST_ITEM(item_data, item_buf, length, FALSE, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, IMAGE_CHECK_UNSELECTED_ICON)/*lint !e64*/
				GUILIST_AppendItem( MMIENG_DYNA_ITEM_LIST_LISTBOX_CTRL_ID, &item_t );
			}
			//激活控件
			MMK_SetAtvCtrl(win_id, MMIENG_DYNA_ITEM_LIST_LISTBOX_CTRL_ID);
		}
		break;
		
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	case MSG_APP_OK:
	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
		{
			ITEM_T item = {0};
			uint16 pos = GUILIST_GetCurItemIndex(MMIENG_DYNA_ITEM_LIST_LISTBOX_CTRL_ID );
			//GUILIST_ChangeIconId(MMIENG_DYNA_ITEM_LIST_LISTBOX_CTRL_ID, pos, IMAGE_SELECT_TICK_ON_PIC, 0);
            getfunc(&item,pos);	
        	selected_data.param_index = pos;
            if(0 == pos)
            {
        		if(0 < item.arr_count)
        			MMK_CreateWin((uint32*) MMIENG_DYNA_VALUE_LIST_WIN_TAB, NULL);
        		else
        			MMK_CreateWin((uint32*) MMIENG_DYNA_EDIT_WIN_TAB,  NULL);
            }
		}
		break;
		
	default:
		recode = MMI_RESULT_FALSE;
		break;	
	}
	return recode;
}
/********************************************************************************
NAME:			GetSubMenuItemStr
DESCRIPTION:	
AUTHOR:	yongwei.he		
DATE:		2007-5-18	
********************************************************************************/
void GetSubMenuItemStr(MMI_STRING_T * text_ptr, uint16 subfunc_arr_index)
{
	//MMI_STRING_T * text = {0, FALSE, PNULL};
	//uint8 showbuf[2048] = {0};
	uint16 * showbuf = text_ptr->wstr_ptr;
       uint16 item_show_buf[64] = {0};
	static GetEngItemCount getcountfunc = PNULL;
	static GetEngItem getfunc = PNULL;
	static SetEngItem setfunc = PNULL;
	uint8 i = 0;
	uint32 ItemCount = 0;
	uint8  *item_show_buf_usc = (uint8 *)SCI_ALLOC_APP(3000);
	SCI_MEMSET(item_show_buf_usc, 0, 3000);	
	//get sub array index by menu node id
	//GetSubArrIndexByNodeID(&subfunc_arr_index, parent_node_id);
	getcountfunc = g_dynaeng_subfunc_arr[subfunc_arr_index].getcountfunc; //获得item 个数的函数 
	getfunc = g_dynaeng_subfunc_arr[subfunc_arr_index].getfunc; //获得指定index 的item 内容的函数 
	setfunc = g_dynaeng_subfunc_arr[subfunc_arr_index].setfunc;
	//SCI_TRACE_LOW("getcountfunc = %d, getfunc = %d, setfunc = %d", getcountfunc, getfunc,setfunc);        
	ItemCount = getcountfunc();
	//SCI_TRACE_LOW("count = %d", ItemCount);        
	for (i = 0; i < ItemCount; i++)
	{
		ITEM_T item={0};
				
		getfunc(&item,i);

		SCI_MEMSET(item_show_buf, 0 ,64*2);
		sprintf((char*)item_show_buf, "%s:  ", item.item_name);
		
		strcat((char*)item_show_buf_usc, (char*)item_show_buf);	
		if(1>= item.arr_count)
		{
                    /*george added 2010-02-25 */
                    //SCI_TRACE_LOW:"eng_dyn: GetEngItem %d, %s %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_DYNAMIC_MAIN_1010_112_2_18_2_18_31_4,(uint8*)"dsd", i,  item.item_name, item.arr_value[0]);
                    
			//sprintf((char*)item_show_buf, "%s = %d\n", item.item_name, (int16)item.arr_value[0]);
			sprintf((char*)item_show_buf, "%d\n", (int32)item.arr_value[0]); /* cr168311 */
			strcat((char*)item_show_buf_usc, (char*)item_show_buf);	
		
		}
		else //item.arr_count>0 
		{
			uint8 j = 0;
			for(;j < item.arr_count - 1;j++)
			{
				//sprintf((char*)item_show_buf, "%s[%d] = %d\n", item.item_name, (int16)j, (int16)item.arr_value[j]);
				sprintf((char*)item_show_buf, "%d, ", (int32)item.arr_value[j]);/* cr168311 */
			       strcat((char*)item_show_buf_usc, (char*)item_show_buf);	
			}
                    /*george added 2010-02-25 */
                    //SCI_TRACE_LOW:"eng_dyn: GetEngItem array>1: %d, %s %s %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_DYNAMIC_MAIN_1027_112_2_18_2_18_31_5,(uint8*)"dssd", i,  item.item_name,item_show_buf, item.arr_value[j]);
                    
			sprintf((char*)item_show_buf, "%d\n", (int32)item.arr_value[j]);/* cr168311 */
			strcat((char*)item_show_buf_usc, (char*)item_show_buf);
		}
        
               


	}
      MMI_STRNTOWSTR((wchar*)showbuf,3000/2,(char*)item_show_buf_usc,3000,strlen(item_show_buf_usc));/*lint !e64 */
	SCI_FREE(item_show_buf_usc);
	text_ptr->wstr_len = MMIAPICOM_Wstrlen(showbuf);
}
/********************************************************************************
NAME:			DynaEngShowParamValueWinHandleMsg
DESCRIPTION:	
AUTHOR:	yongwei.he		
DATE:		2007-5-18	
********************************************************************************/
LOCAL MMI_RESULT_E DynaEngShowParamValueWinHandleMsg(
                                                     MMI_WIN_ID_T win_id,
                                                     MMI_MESSAGE_ID_E msg_id,
                                                     DPARAM param
                                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16 subfunc_arr_index=selected_data.dynaeng_subfunc_arr_index;
    MMI_STRING_T text = {0};
    MMI_STRING_T text_title = {0};
    
    FORM_MMI_STRING(text_title, g_dynaeng_subfunc_arr[subfunc_arr_index].name, FALSE);/*lint !e64*/
    
    //SCI_TRACE_LOW("subfunc_arr_index = %d", subfunc_arr_index);
    
    /*george added 2010-02-25 */
    //SCI_TRACE_LOW:"eng_dyn: dynaeng_name %8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_DYNAMIC_MAIN_1062_112_2_18_2_18_31_6,(uint8*)"d", *(uint32*)g_dynaeng_subfunc_arr[subfunc_arr_index].name);

    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        text.wstr_ptr = (wchar *)SCI_ALLOC_APP(3000);
        SCI_MEMSET(text.wstr_ptr, 0, 3000);
        GetSubMenuItemStr(&text, subfunc_arr_index);
        GUITEXT_SetString(MMIENG_DYNA_PARAM_VALUE_TEXTBOX_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
        
        SCI_FREE(text.wstr_ptr);
        
        GUIWIN_SetTitleText(win_id, text_title.wstr_ptr, text_title.wstr_len, TRUE);
        MMK_SetAtvCtrl(win_id, MMIENG_DYNA_PARAM_VALUE_TEXTBOX_CTRL_ID);
        // create param value refresh timer
        if(g_dynaeng_subfunc_arr[subfunc_arr_index].parent_id==L1_INFO)
        {
            s_mmieng_dyna_timer_id = MMK_CreateWinTimer(win_id, 1000, FALSE);
        }
        break;
        
    case MSG_FULL_PAINT:
        if(!g_dynaeng_subfunc_arr[subfunc_arr_index].setfunc)
        {  
            
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_ENG_EDIT, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
        }
        
        text.wstr_ptr = (wchar *)SCI_ALLOC_APP(3000);
        SCI_MEMSET(text.wstr_ptr, 0, 3000);
        GetSubMenuItemStr(&text, subfunc_arr_index);
        GUITEXT_SetString(MMIENG_DYNA_PARAM_VALUE_TEXTBOX_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
        SCI_FREE(text.wstr_ptr);
        break;	
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
        if(g_dynaeng_subfunc_arr[subfunc_arr_index].setfunc)
        {
            MMK_CreateWin((uint32*) MMIENG_DYNA_ITEM_LIST_WIN_TAB,  PNULL);
        }
        break;
        
    case MSG_TIMER:
        if(g_dynaeng_subfunc_arr[subfunc_arr_index].parent_id==L1_INFO)
        { 
            if (*(uint8*)param == s_mmieng_dyna_timer_id) 
            {   
                MMK_StopTimer(s_mmieng_dyna_timer_id);
                text.wstr_ptr = (wchar *)SCI_ALLOC_APP(3000);
                SCI_MEMSET(text.wstr_ptr, 0, 3000);
                GetSubMenuItemStr(&text, subfunc_arr_index);
                GUITEXT_SetString(MMIENG_DYNA_PARAM_VALUE_TEXTBOX_CTRL_ID, text.wstr_ptr,text.wstr_len, TRUE);
                SCI_FREE(text.wstr_ptr);
                s_mmieng_dyna_timer_id = MMK_CreateWinTimer(win_id, 1000, FALSE);
            }
        }
        else
            recode = MMI_RESULT_FALSE;		// need additional process
        break;
        
    case MSG_LOSE_FOCUS:
        MMK_PauseTimer(s_mmieng_dyna_timer_id);
        break;
        
    case MSG_GET_FOCUS:
        MMK_ResumeTimer(s_mmieng_dyna_timer_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_mmieng_dyna_timer_id);
        s_mmieng_dyna_timer_id = 0;
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;	
    }
    
    return recode;
}
/********************************************************************************
NAME:			DynaEngValueListWinHandleMsg
DESCRIPTION:	
AUTHOR:	yongwei.he		
DATE:		2007-5-18	
********************************************************************************/
LOCAL MMI_RESULT_E DynaEngValueListWinHandleMsg(
												MMI_WIN_ID_T win_id,
												MMI_MESSAGE_ID_E msg_id,
												DPARAM param
												)
{
	
	MMI_CTRL_ID_T	ctrl_id=MMIENG_DYNA_VALUE_LIST_LISTBOX_CTRL_ID;
	uint16 i = 0;
	BOOLEAN recode = TRUE;
	MMI_STRING_T ktext;
	//static GetEngItemCount getcountfunc=NULL;
	static GetEngItem getfunc=NULL;
	//static SetEngItem setfunc=NULL;
	//uint16 subfunc_arr_index=selected_data.dynaeng_subfunc_arr_index;	
	//SCI_TRACE_LOW:"DynaEngValueListWinHandleMsg MSG_ID = %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_DYNAMIC_MAIN_1172_112_2_18_2_18_31_7,(uint8*)"d", msg_id);
	switch(msg_id)
	{		
	case MSG_OPEN_WINDOW:
		{
			uint8 count;
			ITEM_T item={0};
			//char value[30]={0};
			GUILIST_ITEM_T item_t;
			GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
			//MMI_STRING_T text_title = {0};/*lint !e64*/
			uint16 item_buf[MAX_DYNAENG_STR_LENGTH]={0}; 
			uint8  item_num[8]={0};
            uint16 length = 0;  
            
			//FORM_MMI_STRING(text_title, g_dynaeng_subfunc_arr[subfunc_arr_index].name, FALSE);
			//GUIWIN_SetTitleText(win_id, text_title.wstr_ptr, text_title.wstr_len, FALSE);
			//get sub array index by menu node id
			//getcountfunc=g_dynaeng_subfunc_arr[subfunc_arr_index].getcountfunc; //获得item 个数的函数 
			getfunc=g_dynaeng_subfunc_arr[selected_data.dynaeng_subfunc_arr_index].getfunc; //获得指定index 的item 内容的函数 
			//setfunc=g_dynaeng_subfunc_arr[subfunc_arr_index].setfunc;
			getfunc(&item,selected_data.param_index);	
			count=item.arr_count;
            //SCI_TRACE_LOW:"DynaEngValueListWinHandleMsg item.arr_count = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_DYNAMIC_MAIN_1195_112_2_18_2_18_31_8,(uint8*)"d", count);
			
			MMI_STRNTOWSTR((wchar*)item_buf,MAX_DYNAENG_STR_LENGTH,(char*)item.item_name,sizeof(item.item_name),strlen(item.item_name));/*lint !e64 */
			GUIWIN_SetTitleText(win_id, item_buf,MMIAPICOM_Wstrlen(item_buf), FALSE);
			GUILIST_SetMaxItem(ctrl_id , count, FALSE );
			//GUILIST_SetTitle(ctrl_id, GUI_TITLE_PARAM_T * title_ptr);
			for (i = 0; i <count; i++)
			{
				item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
				item_t.item_data_ptr = &item_data;
			
							
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
				//getfunc(&item,i);

				//增加这一句，buffer置0，否则对判断字符串长度有影响
				SCI_MEMSET(item_num, 0, 8);
                SCI_MEMSET((uint8*)item_buf,0,(MAX_DYNAENG_STR_LENGTH*sizeof(uint16)));
				
				sprintf((char*)item_num,"%d",item.arr_value[i]);
                //SCI_TRACE_LOW:"DynaEngValueListWinHandleMsg item.arr_value[%d] = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_DYNAMIC_MAIN_1215_112_2_18_2_18_31_9,(uint8*)"dd", i, item.arr_value[i]);

				//这个函数大量用错，第二个参数是蔮uffer可以存放wchar的个数，不是buffer大小
				MMI_STRNTOWSTR((wchar*)item_buf,MAX_DYNAENG_STR_LENGTH,(char*)item_num,sizeof(item_num),strlen(item_num));/*lint !e64 */
				
				FORM_MMI_STRING(ktext,item_buf, FALSE);/*lint !e64 !e733*/
                length = MMIAPICOM_Wstrlen((const wchar *)item_buf) ;
                
                //SCI_TRACE_LOW:"DynaEngValueListWinHandleMsg length = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_DYNAMIC_MAIN_1223_112_2_18_2_18_31_10,(uint8*)"d",  length);
//				FORM_GUILIST_ITEM(item_data, item_buf, MMIAPICOM_Wstrlen((char *)item_buf), FALSE, STXT_OK, TXT_NULL, STXT_RETURN, IMAGE_CHECK_UNSELECTED_ICON)/*lint !e64*/
                FORM_GUILIST_ITEM(item_data, item_buf, length, FALSE, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, IMAGE_CHECK_UNSELECTED_ICON)/*lint !e64*/
				GUILIST_AppendItem( ctrl_id, &item_t );
                
			}
			MMK_SetAtvCtrl(win_id, ctrl_id);
		}
		break;

	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
	case MSG_APP_OK:
	case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
		{
	        ITEM_T item={0};

			uint16 pos = GUILIST_GetCurItemIndex(ctrl_id );

    		selected_data.value_array_index=pos;
            
            GUILIST_SetSelectedItem( ctrl_id, pos, TRUE );

    		getfunc(&item,pos);	
            if(0 == pos)
            {
                MMK_CreateWin((uint32*) MMIENG_DYNA_EDIT_WIN_TAB, PNULL );	
            }
		}
		break;
		
	default:
		recode = FALSE;
		break;
	}
	return recode;
}

/********************************************************************************
NAME:			DynaEngEditWinHandleMsg
DESCRIPTION:	
AUTHOR:	yongwei.he		
DATE:		2007-5-18	
********************************************************************************/
LOCAL MMI_RESULT_E DynaEngEditWinHandleMsg(
										   MMI_WIN_ID_T win_id,
										   MMI_MESSAGE_ID_E msg_id,
										   DPARAM param
										   )
{
	//uint16 i,j = 0;
	MMI_CTRL_ID_T ctrl_id = MMIENG_DYNA_VALUE_EDITBOX_CTRL_ID;
	BOOLEAN recode = TRUE;
//	MMI_STRING_T ktext;
//	static GetEngItemCount getcountfunc=NULL;
	static GetEngItem getfunc=NULL;
	static SetEngItem setfunc=NULL;

	//GUI_RECT_T client_rect = MMITHEME_GetClientRect();
	
	switch(msg_id)
	{
		
	case MSG_OPEN_WINDOW:
		{
			uint8 value[30]={0};
			ITEM_T item={0};
			getfunc=g_dynaeng_subfunc_arr[selected_data.dynaeng_subfunc_arr_index].getfunc;
			getfunc(&item, selected_data.param_index);
			sprintf((char *)value, "%d",item.arr_value[selected_data.value_array_index]);
			//GUIEDIT_SetRect(MMK_ConvertIdToHandle(ctrl_id), &client_rect);
			MMK_SetAtvCtrl(win_id, ctrl_id);
		}
		break;
	case MSG_LCD_SWITCH:
		//GUIEDIT_SetRect(MMK_ConvertIdToHandle(ctrl_id), &client_rect);
		break;
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
	case MSG_APP_OK:
	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
		{
			ITEM_T item={0};
			MMI_STRING_T str_info={0};
			uint8 buf[32]={0};
			setfunc=g_dynaeng_subfunc_arr[selected_data.dynaeng_subfunc_arr_index].setfunc;
			getfunc(&item,selected_data.param_index);

			GUIEDIT_GetString(ctrl_id, &str_info);
			MMI_WSTRNTOSTR(buf,32,str_info.wstr_ptr,str_info.wstr_len,MIN(32,str_info.wstr_len));
			item.arr_value[selected_data.value_array_index]=atoi((char *)buf);
			
			if(CheckParam(&item))
			{    
				//SCI_PASSERT(0,("%d,%d,%s",item.arr_value[selected_data.value_array_index],selected_data.param_index,buf));
				setfunc(&item,selected_data.param_index);
				
				//MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
			}
		
			else
				MMIPUB_OpenAlertFailWin(TXT_ERROR);
			
			MMK_CloseWin(win_id);
			
		}
		break;
	default:
		recode = FALSE;
		break;
	}
	return recode;
}

/********************************************************************************
NAME:			FindSubArrIndexByNodeID
DESCRIPTION:	
AUTHOR:	sword		
DATE:			
********************************************************************************/
LOCAL void FindSubArrIndexByNodeID( uint16 * subfunc_arr_node_index,uint32 node_id)
{
	uint32 count=0;
	uint16 i,j;
	for (i = 0; i <DYNAENG_MAINFUNC_ARR_COUNT; i++)
	{
		count++;
		//now main menu node id is the parent node id of sub menu
		for (j = 0; j< DYNAENG_SUBFUNC_ARR_COUNT; j++)		
		{
			if(g_dynaeng_mainfunc_arr[i].id==g_dynaeng_subfunc_arr[j].parent_id)
			{
				count++;
				if(count==node_id)
				{
					//menu_data.name_ptr=g_dynaeng_subfunc_arr[j].name;
					*subfunc_arr_node_index=j;
					return;
					
				}
				//else
				//count++;
			}
		}
	}
}

/********************************************************************************
NAME:			CheckParam
DESCRIPTION:	
AUTHOR:	sword		
DATE:			
********************************************************************************/
LOCAL BOOLEAN CheckParam(ITEM_T *item)
{
	uint8 i;
	for(i=0;i<item->arr_count;i++)
		if((item->arr_value[i]>item->max_value)||(item->arr_value[i]<item->min_value))
		{
			return FALSE;
		}
		return TRUE;
}

/********************************************************************************
NAME:			get fta flag
DESCRIPTION:	
AUTHOR:	sword		
DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetFtaFlag(void)
{
/* +cr223863 */
#if 0
/* +cr221587 */
#if 0
#ifndef WIN32
    /* +CR200414 */
    ITEM_T item;

    //em_get_fta_opt_enable_flag(&item, 0);
    em_get_real_nw_opt_enable_flag(&item, 0);

    //return item.arr_value[0];
    return (item.arr_value[0] == 0) ? TRUE : FALSE;
    /* -CR200414 */
#else
    return FALSE;
#endif
#endif
    return FALSE;
/* -cr221587 */
#endif

#ifndef WIN32
    ITEM_T item;
    em_get_reserved_param(&item, 3);
    return (item.arr_value[0] != 0) ? TRUE : FALSE;
#else
    return FALSE;
#endif
/* -cr223863 */
}

#if 0
//MN 实现3 组GetCount/Get/Set 接口函数
/********************************************************************************
NAME:			GetEML1MesaCount
DESCRIPTION:	
AUTHOR:	yongwei.he		
DATE:		2007-5-18	
********************************************************************************/ 
uint32 GetEML1MesaCount(void) 
{
	
	return 7; 
	
} 
/********************************************************************************
NAME:			GetEML1Mesa
DESCRIPTION:	
AUTHOR:	yongwei.he		
DATE:		2007-5-18	
********************************************************************************/ 
void GetEML1Mesa(ITEM_T *item_ptr, uint32 index) 
{
	
	uint32 i = 0;
	#if 0
	
	switch (index) 
	{
	case 0: 
		sprintf((char*)item_ptr->item_name,"ta"); 
		item_ptr->arr_count = 0; 
		item_ptr->arr_value[0] = g_em_l1_meas.ta; 
		item_ptr->min_value = 0; 
		item_ptr->max_value = 0xffff; 
		break;
		
	case 1: 
		sprintf((char*)item_ptr->item_name,"cell_id"); 
		item_ptr->arr_count = 32; 
		for (i = 0; i < 32; i++) 
			item_ptr->arr_value[i] = g_em_l1_meas.cell_id[i]; 
		item_ptr->min_value = 0; 
		item_ptr->max_value = 0xffff; 
		break; 
		
	case 2: 
		sprintf((char*)item_ptr->item_name,"uarfcn"); 
		item_ptr->arr_count = 32; 
		for (i = 0; i < 32; i++) 
			item_ptr->arr_value[i] = g_em_l1_meas.uarfcn[i]; 
		item_ptr->min_value = 0; 
		item_ptr->max_value = 0xffff; 
		break;
		
	case 3: 
		sprintf((char*)item_ptr->item_name,"cell_param_id"); 
		item_ptr->arr_count = 32; 
		for (i = 0; i < 32; i++) 
			item_ptr->arr_value[i] = g_em_l1_meas.cell_param_id[i]; 
		item_ptr->min_value = 0; 
		item_ptr->max_value = 0xff; 
		break;
		
	case 4: 
		sprintf((char*)item_ptr->item_name,"rscp"); 
		item_ptr->arr_count = 32; 
		for (i = 0; i < 32; i++) 
			item_ptr->arr_value[i] = g_em_l1_meas.rscp[i]; 
		item_ptr->min_value = 0; 
		item_ptr->max_value = 0xffff; 
		break; 
		
	case 5: 
		sprintf((char*)item_ptr->item_name,"iscp"); 
		item_ptr->arr_count = 7; 
		for (i = 0; i < 7; i++) 
			item_ptr->arr_value[i] = g_em_l1_meas.iscp[i]; 
		item_ptr->min_value = 0; 
		item_ptr->max_value = 0xffff; 
		break;
		
	case 6: 
		sprintf((char*)item_ptr->item_name,"rssi"); 
		item_ptr->arr_count = 32; 
		for (i = 0; i < 32; i++) 
			item_ptr->arr_value[i] = g_em_l1_meas.rssi[i]; 
		item_ptr->min_value = 0; 
		item_ptr->max_value = 0xff; 
		break;
		
	default: 
		break; 
	}
	#endif 
} 
/********************************************************************************
NAME:			SetEML1Mesa
DESCRIPTION:	
AUTHOR:	yongwei.he		
DATE:		2007-5-18	
********************************************************************************/
void SetEML1Mesa(ITEM_T *item_ptr, uint32 index) 

{
	
	uint32 i = 0;
	#if 0
	
	switch (index) 
	{
	case 0: 
		g_em_l1_meas.ta = item_ptr->arr_value[0]; 
		break;
		
	case 1: 
		for (i = 0; i < 32; i++) 
			g_em_l1_meas.cell_id[i] = item_ptr->arr_value[i]; 
		break; 
		
	case 2: 
		for (i = 0; i < 32; i++) 
			g_em_l1_meas.uarfcn[i] = item_ptr->arr_value[i]; 
		break;
		
	case 3: 
		for (i = 0; i < 32; i++) 
			g_em_l1_meas.cell_param_id[i] = item_ptr->arr_value[i]; 
		break;
		
	case 4: 
		for (i = 0; i < 32; i++) 
			g_em_l1_meas.rscp[i] = item_ptr->arr_value[i]; 
		break; 
		
	case 5: 
		for (i = 0; i < 7; i++) 
			g_em_l1_meas.iscp[i] = item_ptr->arr_value[i]; 
		break;
		
	case 6: 
		for (i = 0; i < 32; i++) 
			g_em_l1_meas.rssi[i] = item_ptr->arr_value[i]; 
		break;
		
	default: 
		break; 
	}
	#endif
	
	
} 
//EM_CELL_REQ_T 
uint32 GetEMCellReqCount(void) 
{
	
	return 3; 
	
} 

void GetEMCellReq(ITEM_T *item_ptr, uint32 index) 
{
	
	uint32 i = 0;
	
	#if 0
	switch (index) 
	{
		
	case 0: //g_em_cell.em_cell_a 
		sprintf((char*)item_ptr->item_name,"em_cell_a"); 
		item_ptr->arr_count = 0; 
		item_ptr->arr_value[0] = g_em_cell.em_cell_a; 
		item_ptr->min_value = 0; 
		item_ptr->max_value = 0xffffffff; 
		break;
		
		
	case 1: //g_em_cell.em_cell_b 
		sprintf((char*)item_ptr->item_name,"em_cell_b"); 
		item_ptr->arr_count = 0; 
		item_ptr->arr_value[0] = g_em_cell.em_cell_b; 
		item_ptr->min_value = 0; 
		item_ptr->max_value = 0xff; 
		break; 
		
	case 2://g_em_cell.em_cell_c 
		sprintf((char*)item_ptr->item_name,"em_cell_c"); 
		item_ptr->arr_count = 0; 
		item_ptr->arr_value[0] = g_em_cell.em_cell_c; 
		item_ptr->min_value = 0; 
		item_ptr->max_value = 0xffff; 
		break;
		
	default: 
		break; 
	} 
	#endif
} 

/********************************************************************************
NAME:			SetEMCellReq
DESCRIPTION:	
AUTHOR:	sword		
DATE:			
********************************************************************************/
void SetEMCellReq(ITEM_T *item_ptr, uint32 index) 

{    
#if 0
	uint32 i = 0; 
	switch (index) 
	{
		
	case 0: //g_em_cell.em_cell_a 
		g_em_cell.em_cell_a = item_ptr->arr_value[0]; 
		break;
		
	case 1: //g_em_cell.em_cell_b 
		g_em_cell.em_cell_b = item_ptr->arr_value[0]; 
		break;
		
		
	case 2: //g_em_cell.em_cell_c 
		g_em_cell.em_cell_c = item_ptr->arr_value[0]; 
		break;
		
	default: 
		break; 
	} 
	#endif
} 

/********************************************************************************
NAME:			GetEMCellParamCount
DESCRIPTION:	
AUTHOR:	sword		
DATE:			
********************************************************************************/
uint32 GetEMCellParamCount(void) 
{
	return 2; 
} 

/********************************************************************************
NAME:			GetEMCellParam
DESCRIPTION:	
AUTHOR:	sword		
DATE:			
********************************************************************************/
void GetEMCellParam(ITEM_T *item_ptr, uint32 index) 

{ 
	uint32 i = 0;
	#if 0
	switch (index) 
	{
		
		
	case 0: //g_em_cell.em_cell_a 
		sprintf((char*)item_ptr->item_name,"em_param_a"); 
		item_ptr->arr_count = 0; 
		item_ptr->arr_value[0]=g_em_param.em_param_a;
		item_ptr->min_value = 0; 
		item_ptr->max_value = 0xffffffff; 
		
		
		break;
		
	case 1: //g_em_cell.em_cell_b 
		sprintf((char*)item_ptr->item_name,"em_param_b"); 
		item_ptr->arr_count = 15; 
		for (i = 0; i < 15; i++)
			
			item_ptr->arr_value[i] = g_em_param.em_param_b[i]; // ksword
		item_ptr->min_value = 0; 
		item_ptr->max_value = 0xff; 
		break;
		
	default://g_em_cell.em_cell_c 
		break; 
	} 
	#endif
} 

void SetEMCellParam(ITEM_T *item_ptr, uint32 index) 
{   
#if 0
	uint32 i = 0; 
	switch (index) 
	{ 
	case 0: //g_em_cell.em_cell_a 
		g_em_param.em_param_a = item_ptr->arr_value[0]; 
		break; 
	case 1: //g_em_cell.em_cell_b 
		for (i = 0; i < 15; i++) 
			g_em_param.em_param_b[i] = item_ptr->arr_value[i]; 
		break; 
	default: 
		break; 
	} 
#endif
} 

//FM_COAST_PARAM_T 
uint32 GetFMCoastParamCount(void) 
{
	
	return 4; 
} 

void GetFMCoastParam(ITEM_T *item_ptr, uint32 index) 
{ 
	
	
	uint32 i = 0;
	#if 0
	switch (index) 
	{
		
	case 0: //g_fm_coast_param.fm_coast_a 
		sprintf((char*)item_ptr->item_name,"g_fm_coast_param_a"); 
		item_ptr->arr_count = 0; 
		item_ptr->arr_value[0] = g_fm_coast_param.fm_coast_a; 
		item_ptr->min_value = 0; 
		item_ptr->max_value = 0xffffffff; 
		break;
		
	case 1: //g_fm_coast_param.fm_coast_b 
		sprintf((char*)item_ptr->item_name,"g_fm_coast_param_b"); 
		item_ptr->arr_count = 0;
		
		
		item_ptr->arr_value[0] = g_fm_coast_param.fm_coast_b; 
		item_ptr->min_value = 0; 
		item_ptr->max_value = 0xffffffff; 
		break;
		
		
	case 2: //g_fm_coast_param.fm_coast_c 
		sprintf((char*)item_ptr->item_name,"g_fm_coast_param_c"); 
		item_ptr->arr_count = 0; 
		item_ptr->arr_value[0] = g_fm_coast_param.fm_coast_c; 
		item_ptr->min_value = 0;
		
		item_ptr->max_value = 0xff; 
		break;
		
		
	case 3: //g_fm_coast_param.fm_coast_d 
		item_ptr->min_value = 0; 
		sprintf((char*)item_ptr->item_name,"g_fm_coast_param_d"); 
		item_ptr->arr_count = 0; 
		item_ptr->arr_value[0] = g_fm_coast_param.fm_coast_d;
		
		item_ptr->max_value = 0xffffffff; 
		break;
		
		
	default://g_em_cell.em_cell_c 
		break; 
	} 
	#endif
} 

void SetFMCoastParam(ITEM_T *item_ptr, uint32 index) 
{ 
	#if 0
	uint32 i = 0;
	
	switch (index) 
	{ 
	case 0: //g_fm_coast_param.em_cell_a 
		g_fm_coast_param.fm_coast_a = item_ptr->arr_value[0]; 
		break;
		
	case 1: //g_fm_coast_param.em_cell_b 
		g_fm_coast_param.fm_coast_b = item_ptr->arr_value[0]; 
		break;
		
	case 2: //g_fm_coast_param.em_cell_c 
		g_fm_coast_param.fm_coast_c = item_ptr->arr_value[0]; 
		break;
		
		
	case 3: //g_fm_coast_param.em_cell_d 
		g_fm_coast_param.fm_coast_d = item_ptr->arr_value[0]; 
		break;
		
	default: 
		break; 
	} 
	#endif
} 
#endif

#ifdef  _WIN32

PUBLIC uint32 rlc_em_get_stat_req_count(void){return 0;}
PUBLIC void rlc_em_get_stat_req(ITEM_T *item_t_ptr,uint32 index){}

PUBLIC uint32 pdcp_em_stat_get_item_count(void){return 0;}
PUBLIC void pdcp_em_stat_get_item(ITEM_T *item_t_ptr,uint32 index){}

PUBLIC uint32 mmi_get_em_lock_freq_count(void){return 0;}
PUBLIC void mmi_get_em_lock_freq(ITEM_T *item_ptr, uint32 index){}
PUBLIC void mmi_set_em_lock_freq(ITEM_T *item_ptr, uint32 index){}

PUBLIC uint32 mmi_get_em_lock_cell_count(void){return 0;}
PUBLIC void mmi_get_em_lock_cell(ITEM_T *item_ptr, uint32 index){}
PUBLIC void mmi_set_em_lock_cell(ITEM_T *item_ptr, uint32 index){}
//@yongwei.he add begin for cr96371	
PUBLIC uint32 mmi_get_em_unlock_freq_cell_count(void){return 0;}
PUBLIC void mmi_get_em_unlock_freq_cell(ITEM_T *item_ptr, uint32 index){}
PUBLIC void mmi_set_em_unlock_freq_cell(ITEM_T *item_ptr, uint32 index){}
//@yongwei.he add end for cr96371	

/* +cr96947 */
PUBLIC void em_get_integrity_enable_flag(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_integrity_enable_flag(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_integrity_enable_flag_count(void){return 0;}
/* +cr181390 */  
/*
PUBLIC void em_get_fta_opt_enable_flag(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_fta_opt_enable_flag(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_fta_opt_enable_flag_count(void){return 0;}
PUBLIC void em_get_real_nw_opt_enable_flag(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_real_nw_opt_enable_flag(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_real_nw_opt_enable_flag_count(void){return 0;}
*/
/* -cr181390 */  
/* -cr96947 */

/* +cr97057 */
/* +cr199401 delete AOC flag */
/* PUBLIC void em_get_aoc_enable_flag(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_aoc_enable_flag(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_aoc_enable_flag_count(void){return 0;} */
/* -cr199401 delete AOC flag */
/* -cr97057 */

/*+cr97909*/
PUBLIC void em_get_attach_type(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_execute_attach_with_specific_type(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_attach_type_count(void){return 0;}


PUBLIC void em_get_detach_type(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_execute_detach_with_specific_type(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_detach_type_count(void){return 0;}


PUBLIC void  em_get_pdp_activate_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_execute_pdp_activate(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_pdp_activate_param_count(void){return 0;}

PUBLIC void  em_get_pdp_deactivate_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_execute_pdp_deactivate(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_pdp_deactivate_param_count(void){return 0;}
/*-cr97909*/
	
PUBLIC uint32 mmi_get_em_serv_cell_info_count(void){return 0;}
PUBLIC void mmi_get_em_serv_cell_info(ITEM_T *item_ptr, uint32 index){}

PUBLIC uint32 mmi_get_em_neigbour_cell_count(void){return 0;}
PUBLIC void mmi_get_em_neigbour_cell_info(ITEM_T *item_ptr, uint32 index){}

PUBLIC uint32 mmi_get_em_cell_resel_num_count(void){return 0;}
PUBLIC void mmi_get_em_cell_resel_num(ITEM_T *item_ptr, uint32 index){}

PUBLIC void mm_em_get_identity_req(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 mm_em_get_identity_req_count(void){return 0;}

PUBLIC void mm_em_get_param_req(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 mm_em_get_param_req_count(void){return 0;}
/* +cr191140*/ 
PUBLIC void mm_em_get_abnormal_req(ITEM_T *item_ptr, uint32 index){} 
PUBLIC uint32 mm_em_get_abnormal_req_count(void){return 0;}
/* -cr191140 */
/* +cr215513*/ 
PUBLIC void mm_em_get_lu_rau_num_req(ITEM_T *item_ptr, uint32 index){} 
PUBLIC uint32 mm_em_get_lu_rau_num_req_count(void){return 0;}
/* -cr215513 */

PUBLIC uint32 SystemInfo_GetL1Meas_Count(void)
{
    return 5;
}
PUBLIC void SystemInfo_GetL1Meas_State_Req(ITEM_T *item_ptr, uint32 index)
{
    uint8 name_len=0;
    uint8  i=0;
	uint8 item_name[5][20]={"uarfcn","cell_param_id","rscp","iscp","rssi"};
	 
     if (5<=index)
     {
        //SME_ASSERT (KAL_FALSE, SME_NON_CRITICAL, "L1C: ERROR with index,it must < 5 ");
        return;  
      }        
 
   switch (index)
    {
       case 0:
         item_ptr->arr_count = 5;
          for(i=0;i<5;i++)
          {
             item_ptr->arr_value[i] =i;
           }
        break;
        
        case 1:
           item_ptr->arr_count = 5;
            for(i=0;i<5;i++)
            {
                item_ptr->arr_value[i] = i;
            }
          
        break;

        case 2:
         item_ptr->arr_count = 5;
         for(i=0;i<5;i++)
          {
            
                item_ptr->arr_value[i] =i;
           
           }

        break;

        case 3:
         item_ptr->arr_count = 5;
         for(i=0;i<5;i++)
          {
            item_ptr->arr_value[i] = i;
          }
        break;
        
        case 4:
          item_ptr->arr_count = 5;
          for(i=0;i<5;i++)
          {
             item_ptr->arr_value[i] =  i;
	      //SCI_TRACE_LOW("mmi rssi  %d", rrc2l1_meas.rssi[i]);		 
           }
        break;
        
        default:
        break;
    }
	name_len=strlen((char*)item_name[index]);
	SCI_MEMCPY(&(item_ptr->item_name),item_name[index],name_len);
	item_ptr->item_name[name_len]='\0';
	item_ptr->max_value=-1; /*lint !e570 */
	item_ptr->min_value=0;
	return ;
}

PUBLIC uint32 SystemInfo_GetL1MeasOther_Count(void){return 0;}
PUBLIC void SystemInfo_GetL1MeasOther_State_Req(ITEM_T *item_ptr, uint32 index){}

PUBLIC uint32 SystemInfo_GetL1Power_Count(void){return 0;}
PUBLIC void SystemInfo_GetL1Power_State_Req(ITEM_T *item_ptr, uint32 index){}

PUBLIC uint32 SystemInfo_GetL1Delta_Count(void){return 0;}
PUBLIC void SystemInfo_GetL1Delta_State_Req(ITEM_T *item_ptr, uint32 index){}

PUBLIC uint32 SystemInfo_GetL1Ma_Count(void){return 0;}
PUBLIC void SystemInfo_GetL1Ma_State_Req(ITEM_T *item_ptr, uint32 index){}

PUBLIC uint32 SystemInfo_GetL1Bler_Count(void){return 0;}
PUBLIC void SystemInfo_GetL1Bler_State_Req(ITEM_T *item_ptr, uint32 index){}

/*+cr233014*/
PUBLIC uint32 em_get_l1_dsp_para_count(void){return 0;}
PUBLIC void em_get_l1_dsp_para(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_l1_dsp_para(ITEM_T *item_ptr, uint32 index){}
/*-cr233014*/


PUBLIC uint32 em_get_debug_log_stat_count(void){return 0;}
PUBLIC void em_get_debug_log_stat(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_debug_log_stat(ITEM_T *item_ptr, uint32 index){}

PUBLIC uint32 em_get_mask_log_count(void){return 0;}
PUBLIC void em_get_mask_log(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_mask_log(ITEM_T *item_ptr, uint32 index){}

PUBLIC uint32 mac_em_get_stat_req_count(void){return 0;}
PUBLIC void mac_em_get_stat_req(ITEM_T *item, uint32  index){}

PUBLIC uint32 rrc_dcfe_em_get_ue_state_count(void){return 0;}
PUBLIC void rrc_dcfe_em_get_ue_state(ITEM_T *item_t_ptr, uint32 index){}
BOOLEAN	   auto_attach_power_on_flag = TRUE;	
PUBLIC void em_get_auto_attach_power_on_flag(ITEM_T *item_ptr, uint32 index)
	{
	uint16 name_len = 0;
	uint8 item_name[1][30]={"auto attach power on flag"};
	if (1<=index)
	{
		//SME_ASSERT (KAL_FALSE, SME_NON_CRITICAL, "ERROR with index,it must < 1 ");
          	return;  
 	}
	switch(index)
	{
		case 0:
		{
			
    
				
			//SCI_TRACE_LOW:"MN_AL_EM:  auto_attach_power_on_flag == %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENG_DYNAMIC_MAIN_2009_112_2_18_2_18_33_11,(uint8*)"d",auto_attach_power_on_flag);

			if (TRUE == auto_attach_power_on_flag)
				item_ptr->arr_value[0] = 1;
			else
				item_ptr->arr_value[0] = 0;						
			break;
		}		
		default:
			break;
		
	}
	name_len=strlen((char*)item_name[index]);
	SCI_MEMCPY(&(item_ptr->item_name),item_name[index],name_len);
	item_ptr->item_name[name_len] ='\0';
	item_ptr->max_value = 1;
	item_ptr->min_value  = 0;	

}
PUBLIC void em_set_auto_attach_power_on_flag(ITEM_T *item_ptr, uint32 index)

{
    auto_attach_power_on_flag=FALSE;
}
PUBLIC uint32 em_get_auto_attach_power_on_flag_count(void){return 1;}
/*+CR105931*/
PUBLIC uint32 em_get_qos_param_count(void){return 0;}
PUBLIC void em_get_browser_qos_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_browser_qos_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_get_mms_qos_param(ITEM_T *item_ptr, uint32 index){}

PUBLIC void em_set_mms_qos_param(ITEM_T *item_ptr, uint32 index){}	
PUBLIC void em_get_download_qos_param(ITEM_T *item_ptr, uint32 index){}

PUBLIC void em_set_download_qos_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_get_streaming_qos_param(ITEM_T *item_ptr, uint32 index){}

PUBLIC void em_set_streaming_qos_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_get_im_qos_param(ITEM_T *item_ptr, uint32 index){}

PUBLIC void em_set_im_qos_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_get_poc_qos_param(ITEM_T *item_ptr, uint32 index){}

PUBLIC void em_set_poc_qos_param(ITEM_T *item_ptr, uint32 index){}
/*-CR105931*/
/* +CR161092 */
PUBLIC uint32 em_get_dummy_nv_gsm_rr_param_count(void){return 0;}
PUBLIC uint32 em_get_dummy_nv_gsm_layer1_param_count(void){return 0;}
PUBLIC uint32 em_get_dummy_nv_td_bcfe_param_count(void){return 0;}
PUBLIC uint32 em_get_dummy_nv_td_layer1_param_count(void){return 0;}
/* -cr223863 */
PUBLIC uint32 em_get_dummy_nv_reserved_param_count(void){return 0;}
/* -cr223863 */
PUBLIC void em_get_gsm_rr_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_gsm_rr_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_get_gsm_layer1_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_gsm_layer1_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_get_td_bcfe_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_td_bcfe_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_get_td_layer1_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_td_layer1_param(ITEM_T *item_ptr, uint32 index){}
/* -cr223863 */
PUBLIC void em_get_reserved_param(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_reserved_param(ITEM_T *item_ptr, uint32 index){}
/* -cr223863 */

PUBLIC uint32 em_get_cell_resel_stat_count(void){return 0;}
PUBLIC void em_get_cell_resel_stat(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_cell_handover_stat_count(void){return 0;}
PUBLIC void em_get_cell_handover_stat(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_out_of_serving_area_stat_count(void){return 0;}
PUBLIC void em_get_out_of_serving_area_stat(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_cell_search_stat_count(void){return 0;}
PUBLIC void em_get_cell_search_stat(ITEM_T *item_ptr, uint32 index){}

PUBLIC uint32 mmi_get_em_neigbour_gsm_cell_count(void){return 0;}
PUBLIC void mmi_get_em_neigbour_gsm_cell_info(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_neigbour_td_cell_count(void){return 0;}
PUBLIC void em_get_neigbour_td_cell_info(ITEM_T *item_ptr, uint32 index){}

PUBLIC uint32 em_get_gsm_monitor_item_count(void){return 0;}
PUBLIC void em_get_gsm_monitor_item(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_td_monitor_item_count(void){return 0;}
PUBLIC void em_get_td_monitor_item(ITEM_T *item_ptr, uint32 index){}

/* -CR161092 */ 

/* +cr168406 */
PUBLIC uint32 em_get_2g_3g_time_count(void){return 0;}
PUBLIC void em_get_2g_3g_time(ITEM_T *item_ptr, uint32 index){}
/* -cr168406 */

/*+cr191140 */
PUBLIC uint32 em_get_rf_search_meas_count(void){return 0;}
PUBLIC void em_get_rf_search_meas(ITEM_T *item_ptr, uint32 index){}
/*-cr191140 */

/* +cr166898 */
PUBLIC void em_get_ps_test_mode(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_ps_test_mode_count(void){return 0;}
PUBLIC void em_set_ps_test_mode(ITEM_T *item_ptr, uint32 index){}
/* -cr166898 */
/* +cr169449 */
PUBLIC void em_get_stored_band_supp(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_stored_band_supp_count(void){ return 0;}
PUBLIC void em_set_stored_band_supp(ITEM_T *item_ptr, uint32 index){}
/* -cr169449 */
/* +cr166784 */
/* +cr181390 */  
/*
PUBLIC uint32 em_get_wakeup_condition_for_dsp_item_count(void){return 0;}
PUBLIC void em_get_wakeup_condition_for_dsp_stat(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_wakeup_condition_for_dsp_stat(ITEM_T *item_ptr, uint32 index){}
*/
/* -cr181390 */  
/* -cr166784 */
/* +CR168886 */
PUBLIC void em_get_mem_deinit_assert(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_mem_deinit_assert_count(void){return 0;}
PUBLIC void em_set_mem_deinit_assert(ITEM_T *item_ptr, uint32 index){}
/* -CR168886 */

/* +cr198069 */ 
extern PUBLIC void em_get_amr2_enable(ITEM_T *item_ptr, uint32 index){}
extern PUBLIC uint32 em_get_amr2_enable_count(void){return 0;}
extern PUBLIC void em_set_amr2_enable(ITEM_T *item_ptr, uint32 index){}
/* -cr198069 */

/* +CR180723 */
PUBLIC void em_get_armlog_saveinUdisk(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_armlog_saveinUdisk_count(void){return 0;}
PUBLIC void em_set_armlog_saveinUdisk(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_get_armlog_dynamic_saveinUdisk(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_armlog_dynamic_saveinUdisk_count(void){return 0;}
PUBLIC void em_set_armlog_dynamic_saveinUdisk(ITEM_T *item_ptr, uint32 index){}
/* -CR180723 */

/* +cr171120 */
PUBLIC void em_get_dsp_log_switch(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_dsp_log_switch_count(void){return 0;}
PUBLIC void em_set_dsp_log_switch(ITEM_T *item_ptr, uint32 index){}
/* -cr1171120 */

/* +cr173855 */
PUBLIC void em_get_usim_drv_log_switch(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_usim_drv_log_switch_count(void){return 0;}
PUBLIC void em_set_usim_drv_log_switch(ITEM_T *item_ptr, uint32 index){}
/* -cr173855 */

/* +cr172713 */
PUBLIC void em_get_fband_enable(ITEM_T *item_ptr, uint32 index){}
PUBLIC uint32 em_get_fband_enable_count(void){return 0;}
PUBLIC void em_set_fband_enable(ITEM_T *item_ptr, uint32 index){}
/* -cr172713 */

/* +cr201742 */  
PUBLIC uint32 em_get_tddo_enable_count(void){return 0;}
PUBLIC void em_get_tddo_enable(ITEM_T *item_ptr, uint32 index){}
PUBLIC void em_set_tddo_enable(ITEM_T *item_ptr, uint32 index){}
/* -cr201742 */

#endif

#endif// ENGTD_SUPPORT


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
