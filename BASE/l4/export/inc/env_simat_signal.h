/******************************************************************************
 ** File Name:      env_simat_signal.h                                        *
 ** Author:         Raistlin.kong                                             *
 ** Date:           10/15/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file define SIM Application  signals          *                                                      
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 10/15/2002     Raistlin.kong      Create.                                 *
 ******************************************************************************/
#ifndef _ENV_SIMAT_SIGNAL_H
#define _ENV_SIMAT_SIGNAL_H

#include "tasks_id.h"

#define ENV_SIMAT_SIGNAL_LIST   SIMAT_POLLING_IND = ((P_SIMAT << 8) | 1),\
                                SIMAT_PC_IND,\
                                SIMAT_ENVELOPE_CNF,\
                                SIMAT_SETUP_EVENT_LIST_CNF,\
                                SIMAT_DISPLAY_TEXT_CNF,\
                                SIMAT_GET_INKEY_CNF,\
                                SIMAT_GET_INPUT_CNF,\
                                SIMAT_PLAY_TONE_CNF,\
                                SIMAT_SETUP_MENU_CNF,\
                                SIMAT_SELECT_ITEM_CNF,\
                                SIMAT_GET_DT_TZ_CNF,\
                                SIMAT_GET_LANG_CNF,\
                                SIMAT_SETUP_IDLE_TEXT_CNF,\
                                SIMAT_MENU_SELECTION_IND,\
                                SIMAT_USER_ACTIVITY_IND,\
                                SIMAT_IDLE_SCREEN_AVA_IND,\
                                SIMAT_LANG_SELECTION_IND,\
                                SIMAT_SEND_SMS_CNF,\
                                SIMAT_SMS_PP_IND,\
                                SIMAT_SMS_CB_IND,\
                                SIMAT_MO_SMS_CONTROL_IND,\
                                SIMAT_SEND_SS_CNF,\
                                SIMAT_SEND_USSD_CNF,\
                                SIMAT_MO_SS_CONTROL_IND,\
                                SIMAT_MO_USSD_CONTROL_IND,\
                                SIMAT_SETUP_CALL_CNF,\
                                SIMAT_SEND_DTMF_CNF,\
                                SIMAT_MO_CC_IND,\
                                SIMAT_MT_CALL_IND,\
                                SIMAT_CALL_CON_IND,\
                                SIMAT_CALL_DISCON_IND,\
                                SIMAT_GET_LAI_CNF,\
                                SIMAT_LOCATION_STATUS_IND,\
                                SIMAT_GET_IMEI_CNF,\
                                SIMAT_GET_NMR_BA_CNF,\
                                SIMAT_GET_TIMING_AD_CNF,\
                                SIMAT_TERMINAL_PROFILE_IND,\
                                SIMAT_TIMER_1_IND,\
                                SIMAT_TIMER_2_IND,\
                                SIMAT_TIMER_3_IND,\
                                SIMAT_TIMER_4_IND,\
                                SIMAT_TIMER_5_IND,\
                                SIMAT_TIMER_6_IND,\
                                SIMAT_TIMER_7_IND,\
                                SIMAT_TIMER_8_IND,\
                                SIMAT_BC_SMS_IND,\
                                SIMAT_PP_SMS_IND,\
                                SIMAT_POWER_OFF_IND,\
                                SIMAT_TERMINAL_PROFILE_CNF,\
                                SIMAT_REFRESH_CNF,\
								SIMAT_LAUNCH_BROWSER_CNF,\
/* +cr110036 , 2.5g(dual sim)/3g integration */\
                                SIMAT_DATA_AVAILABLE_IND,\
                                SIMAT_CHANNEL_STATUS_IND,\
                                SIMAT_BROWSER_TERMINATION_IND,\
                                SIMAT_BROWSER_STATUS_IND,\
                                SIMAT_CARD_READER_STATUS_IND,\
                                SIMAT_LOCAL_CONNECTION_IND,\
                                SIMAT_MMS_TRANS_STATUS_IND,\
                                SIMAT_MMS_NOTIFICATION_DOWNLOAD_IND,\
/* -cr110036 */\
                                SIMAT_OPEN_CHANNEL_CNF,\
                                SIMAT_CLOSE_CHANNEL_CNF,\
                                SIMAT_GET_CHANNEL_STATUS_CNF,\
                                SIMAT_SEND_DATA_CNF,\
                                SIMAT_RECEIVE_DATA_CNF,\
                                SIMAT_AT_CMD_ENVELOPE_IND,\
                                SIMAT_AT_CPDU_ENVELOPE_CNF,\
                                SIMAT_AT_CMD_TERMINAL_RES,\
                                SIMAT_TERMINAL_RESPONSE_CNF,\
								END_SIMAT_SIGNAL
                                
typedef enum
{
    ENV_SIMAT_SIGNAL_LIST
} simat_signal_id_t;


#endif
