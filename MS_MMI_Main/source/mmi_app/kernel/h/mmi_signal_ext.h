/*******************************************************************************
** File Name:      mmi_signal_ext.h                                            *
** Author:         		                                               		   *
** Date:           01/2003                                                     *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.           *
** Description:    This file is used to describe the extern signal             *
**                        						       						   *
********************************************************************************
**                         Important Edit History                              *
** ----------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                 *
** 01/2003        Great.Tian       Create									   *
																			   *
*******************************************************************************/
																				
/*!
 *  \addtogroup mmk_signal_ext_group_label
 *  @{
 */
 
/*! 
 *  \file 	mmi_signal_ext.h
 *  \author	
 *  \date	Jan,2003
 *  \brief 	This file is used to describe the extern signal
 */
 
#ifndef MMI_SIGNAL_EXT_H
#define MMI_SIGNAL_EXT_H
    
#define MMI_SIGNAL_BASE  		0xF000
#include "mn.h"
#include "mn_type.h"
#include "simat_api.h"
#include "simat_callback.h"
#include "env_atc_signal.h"
#ifdef MMI_GPS_SUPPORT
#include "mn_gps_type.h"
#endif
#ifdef CMCC_VCARD_SUPPORT
#ifdef WIN32
//#include "mmipb_win32_simu.h"
#endif
 #endif
//add for dual sys
#if defined(MMI_MULTI_SIM_SYS_DUAL)
#define MMI_DUAL_SYS_MAX 	2
#elif defined(MMI_MULTI_SIM_SYS_TRI)
#define MMI_DUAL_SYS_MAX 	3
#elif defined(MMI_MULTI_SIM_SYS_QUAD)
#define MMI_DUAL_SYS_MAX 	4
#else
#define MMI_DUAL_SYS_MAX 	1
#endif


// add by allen 2004.09.28
typedef struct
{
    SIGNAL_VARS
    uint32	param1;
    uint32	param2;
} MMI_L1_MESSAGE;

//注意与MN中的MN_GMMREG_RAT_E同步更新
typedef enum MMI_GMMREG_RAT_E
{
    MMI_GMMREG_RAT_GPRS = 0,
    MMI_GMMREG_RAT_3G = 1,
    MMI_GMMREG_RAT_TD = MMI_GMMREG_RAT_3G,
    MMI_GMMREG_RAT_GPRS_AND_3G,
    MMI_GMMREG_RAT_UNKNOWN,
    MMI_GMMREG_RAT_VDS_GSM = 5,   
    MMI_GMMREG_RAT_LTE = 0x10,      
    MMI_GMMREG_RAT_GPRS_LTE = MMI_GMMREG_RAT_LTE + 1,  
    MMI_GMMREG_RAT_3G_LTE = MMI_GMMREG_RAT_LTE + 2,  
    MMI_GMMREG_RAT_GPRS_3G_LTE = MMI_GMMREG_RAT_LTE + 3,
    MMI_GMMREG_RAT_CDMA = 0x14
} MMI_GMMREG_RAT_E;


typedef enum
{
	/* MMI Signal */
	SIG_MMI_KEY_PRESS_IND,	

	/* MMI SIGNAL list */
	SIG_APP_MN_NETWORK_STATUS_IND,	
	SIG_APP_MN_PLMN_LIST_CNF,	
	SIG_APP_MN_PLMN_SELECT_CNF,	
	SIG_APP_MN_SIM_GET_PIN_IND,	
	SIG_APP_MN_SIM_READY_IND,		
	SIG_APP_MN_SIM_NOT_READY_IND,	
	SIG_APP_MN_SIM_PIN_FUNC_CNF,	
	SIG_APP_MN_SCELL_RSSI_IND,	
    SIG_APP_MN_MS_POWERING_OFF_IND, 
    SIG_APP_MN_MS_POWER_OFF_CNF,	
	SIG_APP_MN_NCELL_RSSI_CNF,	
    SIG_APP_MN_AOC_MODIFIED_IND,   
	SIG_APP_MN_FDN_SERVICE_IND,		
	SIG_APP_MN_GPRS_ATTACH_RESULT,	
	SIG_APP_MN_GPRS_DETACH_RESULT,	
	SIG_APP_MN_PUCT_INFO_CNF,		
	SIG_APP_MN_PS_READY_IND,
			
	/* be used for (mn_TASK,sim_module) */	
    SIG_APP_MN_MSISDN_UPDATE_CNF_F,
    SIG_APP_MN_ADN_UPDATE_CNF_F,
    SIG_APP_MN_FDN_UPDATE_CNF_F,

	/* be used for (mn_TASK,call_module) */	
    SIG_APP_MN_CALL_READY_IND,    
    SIG_APP_MN_CALL_START_IND,	
    SIG_APP_MN_ALERTING_IND,	
    SIG_APP_MN_SETUP_COMPLETE_IND,	
    SIG_APP_MN_SETUP_COMPLETE_CNF,    
    SIG_APP_MN_CALL_DISCONNECTED_IND,	
    SIG_APP_MN_CALL_ERR_IND,	
    SIG_APP_MN_SETUP_IND,	
    SIG_APP_MN_START_DTMF_CNF,	
    SIG_APP_MN_STOP_DTMF_CNF,     

    SIG_APP_MN_CALL_HELD_CNF,	
    SIG_APP_MN_CALL_HELD_IND,     
    SIG_APP_MN_CALL_HOLDING_IND,
    SIG_APP_MN_CALL_RETRIEVED_CNF,	
    SIG_APP_MN_CALL_RETRIEVED_IND, 
    SIG_APP_MN_CALL_RETRIEVING_IND,
    SIG_APP_MN_BUILDING_MPTY_IND,
    SIG_APP_MN_SPLITTING_MPTY_IND,
    SIG_APP_MN_CALL_SWAP_IND,	
    SIG_APP_MN_CALL_WAIT_IND,	

    SIG_APP_MN_CALL_NOTIFY_IND,	
    SIG_APP_MN_CALL_MODIFY_CNF,	
    SIG_APP_MN_CALL_MODIFY_IND,	
    SIG_APP_MN_CALL_BARRED_IND,	
    SIG_APP_MN_BUILD_MPTY_CNF,
    SIG_APP_MN_BUILD_MPTY_IND,
    SIG_APP_MN_SPLIT_MPTY_CNF,
    SIG_APP_MN_ACMMAX_IND,
    SIG_APP_MN_TMPTY_EXP_IND,
    SIG_APP_MN_SYNC_IND,
    SIG_APP_MN_PROGRESS_IND,
    SIG_APP_MN_USER_TO_USER_IND,
    SIG_APP_MN_DISCONNECTING_IND,


    SIG_APP_MN_CALL_FORWARD_IND,	
    SIG_APP_MN_FORWARD_STATUS_IND,
	SIG_APP_MN_CONNECTING_IND,   

    SIG_APP_MN_MTACT_RESPOND_MODE_CNF, 
    SIG_APP_MN_ACTIVATE_PDP_CONTEXT_CNF, 
    SIG_APP_MN_SET_QOS_CNF, 
    SIG_APP_MN_SET_MIN_QOS_CNF, 
    SIG_APP_MN_SET_PDP_CONTEXT_CNF, 
    SIG_APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,
    SIG_APP_MN_DELETE_PDP_CONTEXT_CNF,  
    SIG_APP_MN_DELETE_MIN_QOS_CNF,  
    SIG_APP_MN_DELETE_QOS_CNF, 

	/* be used for mnsms task */
	SIG_APP_MN_SMS_READY_IND, 
	SIG_APP_MN_SMS_MEM_FULL_IND_F,
    SIG_APP_MN_READ_SMS_CNF, 
    SIG_APP_MN_UPDATE_SMS_STATE_CNF, 
    SIG_APP_MN_READ_SMS_STATUS_REPORT_CNF, 
    SIG_APP_MN_SEND_SMS_CNF, 
    SIG_APP_MN_WRITE_SMS_CNF, 
    SIG_APP_MN_SMS_IND, 
    SIG_APP_MN_SMS_STATUS_REPORT_IND, 
    SIG_APP_MN_READ_SMS_PARAM_CNF, 
    SIG_APP_MN_WRITE_SMS_PARAM_CNF, 
	SIG_APP_MN_DELETE_ALL_SMS_CNF, 

    /* be used for mnsmscb task */ 
    SIG_APP_MN_SMSCB_MSG_IND, 
	SIG_APP_MN_SMSCB_MACRO_MSG_IND, 
	SIG_APP_MN_SMSCB_REJ_IND, 

	/* be used for mnss task */		
	SIG_APP_MN_GET_PASSWORD_IN, 
	SIG_APP_MN_USSD_IND, 
	SIG_APP_MN_USSD_NOTIFY_IND, 
	SIG_APP_MN_RELEASE_SS_IND, 
	SIG_APP_MN_BEGIN_REGISTER_SS, 
	SIG_APP_MN_BEGIN_ERASE_SS, 
	SIG_APP_MN_BEGIN_ACTIVATE_SS, 
	SIG_APP_MN_BEGIN_DEACTIVATE_SS, 
	SIG_APP_MN_BEGIN_INTERROGATE_SS, 
	SIG_APP_MN_BEGIN_REGISTER_PASSWORD, 
	SIG_APP_MN_BEGIN_USSD_SERVICE, 
	SIG_APP_MN_REGISTER_SS_CNF, 
	SIG_APP_MN_ERASE_SS_CNF, 
	SIG_APP_MN_ACTIVATE_SS_CNF, 
	SIG_APP_MN_DEACTIVATE_SS_CNF, 
	SIG_APP_MN_INTERROGATE_CNF, 
	SIG_APP_MN_REGISTER_PASSWORD_CNF, 
	SIG_APP_MN_SS_BUSYING_IND, 
	SIG_APP_MN_USSD_SERVICE_CNF, 

	/* LCD timer used */
	SIG_LCD_CURSOR_BLINK_TIMER,
	SIG_LCD_ICON_BLINK_TIMER,
	SIG_LCD_ANIMATION_TIMER,
	SIG_LCD_BATTERY_SCROLL_TIMER,

	
   	/* MMK timer */
	SIG_MMI_KBD_REP_TIMER,
    SIG_MMI_KBD_KBD_TIMER,
    SIG_MMI_SCREEN_TIMER,
    SIG_MMI_INPUT_ALPHA_TIMER,

    SIG_MMK_TEMP_CTXT_TIMER,
    SIG_MMK_TMP_SHOW_MENU_TIMER,
    
	/* MMI timer */
	// SIG_MMI_SYS_SHOW_TIMER,
    SIG_MMI_BACKLIGHT_TIMER,
    
	/* the customer timer */
	SIG_MMI_CUSTOMER_TIMER,
	
	/* cc timer */
	SIG_MMI_CC_DISPLAY_TIMER,
	
	/* T9 TIMER */
	SIG_MMI_T9_EXPL_TIMER,
    SIG_TIMER_EXPIRY,
    
	/* MISC event */
	SIG_MMI_MISC_TIME_IND,
	SIG_MMI_MISC_DATE_IND,
	SIG_MMI_MISC_ALARM_IND,

	/* be used for charge task */
	SIG_CHR_CAP_IND,
	SIG_CHR_CHARGE_START_IND,
	SIG_CHR_CHARGE_END_IND,
	SIG_CHR_WARNING_IND,
	SIG_CHR_SHUTDOWN_IND
}SignalId;

/*----------------------------------------------------------------------------*/
/*                         The signal structure                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         SS MODULE DEFINE                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         PHONE MODULE DEFINE                                */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         SIM MODULE DEFINE                                  */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                         CALL MODULE DEFINE                                 */
/*----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------*/
/*                         CB sigla list                                      */
/*----------------------------------------------------------------------------*/
typedef struct
{
    SIGNAL_VARS
    int32 data;
} APP_MN_COMMON_DATA_T;//FOR JAVA

typedef struct
{
    SIGNAL_VARS
    uint32 data;
} APP_MN_POINT_DATA_T;

typedef struct
{
    SIGNAL_VARS
    uint16 data;
} APP_MN_WCHAR_DATA_T;

typedef struct
{
    SIGNAL_VARS
    uint32 handle_data;
} APP_MN_HANDLE_DATA_T;//FOR JAVA

typedef struct
{
    SIGNAL_VARS
} APP_MN_SEARCH_NEXT_SMS_REQ_T;

#endif /* MMI_SIGNAL_EXT_H */
/*! @} */
