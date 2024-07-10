/******************************************************************************
** File Name:      sim_main.c                                                *
** Author:         Raislin.Kong                                              *
** Date:           2002.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the C file of sim task's  entry funciton          *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2002.2         Raislin.Kong     Create                                    *
******************************************************************************/

#include "base_trc.h"
#include "sim_include.h"
#include "deep_sleep.h"//need by DISABLE_DEEP_SLEEP
#include "nvitem.h"//NVITEM_ERROR_E
#include "ps_nvitem_id.h"//NV_SIM_CFG
//#ifdef MULTI_SIM_SYS_QUAD
//#include "env_rra_signal.h"//yPDef_RRA_SIM_NUM_SET_REQ
//#endif
#ifdef SIM_DROP_RESTORE_SUPPORT
#include "layer1_engineering.h"
#endif

#ifdef   __cplusplus
extern   "C"
{
#endif

STATIC SCI_MUTEX_PTR s_sim_task_mutex[2] = {NULL, NULL};
#define SIM_MUTEX0_SEMTAKE (SCI_GetMutex(s_sim_task_mutex[0], SCI_WAIT_FOREVER))
#define SIM_MUTEX0_SEMGIVE (SCI_PutMutex(s_sim_task_mutex[0]))
#define SIM_MUTEX1_SEMTAKE (SCI_GetMutex(s_sim_task_mutex[1], SCI_WAIT_FOREVER))
#define SIM_MUTEX1_SEMGIVE (SCI_PutMutex(s_sim_task_mutex[1]))

/*@yongxia.zhang,2003-11-03,begin*/
#define SIM_NEXTSTATE(_CARD_ID,_STATE, _STATE_VALUE, _STATE_NAME_STRING, _SELF_ID)\
s_sim_keep_state[_CARD_ID] = FALSE;\
SCI_NEXTSTATE(_STATE, _STATE_VALUE, _STATE_NAME_STRING, _SELF_ID);

#define SIM_CARD_PRESENT_CHECK_FREQUENCY_CALL        25000   //the frequency of the card presentation check during call
#ifdef SIM_SELF_TEST_CONTINUOUS_READ_WRITE
#define SIM_CARD_PRESENT_CHECK_FREQUENCY_IDLE        65000//only for SIM dveloper's self-test.
#else
#define SIM_CARD_PRESENT_CHECK_FREQUENCY_IDLE        25000   //the frequency of the card presentation check in Idle
#endif
//In order to pass case 27.17.1.5.1,If a 3V only ME cannot complete the SIM type recognition procedure
//The 3V only ME shall deactivate the SIM/ME interface within 5 seconds and reject the SIM.
//cr36632,modify for the m-zone2.0 card
#define SIM_FIRST_GSM_INFO_TIME_LIMIT           3000    //the time limit to get the GSM information after the ATR
#define SIM_CARD_PRESENT_TIME_LIMIT             4500    //the time limit when card present to pass the card present check
//In order to pass case 27.11.1.3(step c),the two timer will adjust to 60s,for the work-waiting-time of 9600 etu
#define SIM_WORK_WAIT_TIME_LIMIT             100000    //the time for work-wait-time is 9600 etu
#define SIM_PIN1PUK1_WAIT_TIME_LIMIT         450000  //the time to wait user input pin1/puk1, if timeout, arm will deep sleep.
#define SIM_RESTORE_TIME_LIMIT               5000  //the time to restore sim card limit
#ifdef SIM_DROP_RESTORE_SUPPORT
#define SIM_MAX_RESTORE_COUNT  				 10        //the max restore count
#else
#define SIM_MAX_RESTORE_COUNT  				 6 		   //the max restore count 
#endif

#ifdef MULTI_SIM_SYS_QUAD
#define SIM_QUADSIM_SELF_DETECT_CARD_NUM     4
#endif

//-----------------------global varialble define-----------------------
//this global variavle used to get the signal that send by sim or simat layer and
//then to do the unit test for the signal send function
#ifdef SIM_CPPUNIT_TEST
xSignalHeaderRec *g_test_sig = NULL;
#endif

#ifdef SIM_SIMULATOR
SIMDRV_INSERT_STATUS_E g_sim_insert_status;
#endif

#ifdef MULTI_SIM_SYS_QUAD
#ifdef SIM_TEST_IN_WIN32
BOOLEAN g_is_mobile_power_on[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
BOOLEAN g_is_sim_card_inserted[SIM_CARD_NUM] = {TRUE, TRUE, TRUE, TRUE};
#endif

#ifdef SIM_SIMULATOR
uint32  g_sim_info_task_id[SIM_CARD_NUM] = {P_SIM_WIN_DUMMY, P_SIM_WIN_DUMMY, P_SIM_WIN_DUMMY, P_SIM_WIN_DUMMY};    //the task id that the sim layer information to
#else
uint32  g_sim_info_task_id[SIM_CARD_NUM] = {P_MN, P_MN, P_MN, P_MN};
#endif

#ifdef GSM_DPHONE_VER2_SUPPORT
static BOOLEAN s_sim_is_dphone_ver2_need_pending[SIM_CARD_NUM] = {TRUE, TRUE, TRUE, TRUE};
static BOOLEAN s_sim_is_initialize_skip_refresh[SIM_CARD_NUM] = {TRUE, TRUE, TRUE, TRUE};
#endif

uint32  g_sim_simat_task_id[SIM_CARD_NUM] = {P_MN, P_MN, P_MN, P_MN};

static BOOLEAN s_sim_is_in_refresh[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};

uint32  g_sim_task_id[SIM_CARD_NUM] = {P_SIM, P_SIM, P_SIM, P_SIM};             //the sim layer's taks id

SIM_CARD_SETTING_T  g_sim_card_setting[SIM_CARD_NUM] = {0};             //the setting of SIM card
SIM_CARD_STATE_E g_sim_card_state[SIM_CARD_NUM] = {SIM_NO_POWER_STATE, SIM_NO_POWER_STATE, SIM_NO_POWER_STATE, SIM_NO_POWER_STATE}; //the sim card's state
SIM_CARD_CPHSINFO_T g_sim_cphsinfo[SIM_CARD_NUM] = {0}; //To save the CPHS service table

//------------------------local variable define-------------------------
STATIC SCI_TMID s_sim_idle_timer[SIM_CARD_NUM] = {NULL, NULL, NULL, NULL}; //the timer used to idle the SIM card;
STATIC SCI_TMID s_sim_check_period_timer[SIM_CARD_NUM] = {NULL, NULL, NULL, NULL}; //the timer used to check the SIM card presentation period
STATIC SCI_TMID s_sim_present_timer[SIM_CARD_NUM] = {NULL, NULL, NULL, NULL}; //the timer used to confirm the SIM card exist or not by
//the status fuc

STATIC SCI_TMID s_sim_pin1puk1_wait_timer[SIM_CARD_NUM] = {NULL, NULL, NULL, NULL}; //this timer is used to wait users to input PIN1/PUK1 when power on. if time out, enable arm deep sleep.

STATIC BOOLEAN  s_b_sim_present_timer[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};

STATIC SCI_TMID s_sim_restore_waiting_timer[SIM_CARD_NUM] = {NULL, NULL, NULL, NULL}; //this timer is used to waiting for the restore of the sim,
//if the sim restore is failed,the timer expired and the sim task assert

STATIC BOOLEAN  s_sim_keep_state[SIM_CARD_NUM];                 //set this value to false to let the sim layer
//change to the current state process function
STATIC uint8 s_sim_restore_count[SIM_CARD_NUM] = {0, 0, 0, 0};
STATIC uint8 s_total_sim_reset_times[SIM_CARD_NUM] = {0, 0, 0, 0}; /*new adds by king.li to make the card reset no more than 3 times*/
STATIC uint8 s_total_sim_change_reset_times[SIM_CARD_NUM] = {0, 0, 0, 0}; //

STATIC BOOLEAN s_sim_arm_sleep[SIM_CARD_NUM] = {TRUE, TRUE, TRUE, TRUE};
STATIC BOOLEAN s_sim_restore_with_3v[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
STATIC uint32 s_sim_cardout_reset_times[SIM_CARD_NUM] = {0, 0, 0, 0};

BOOLEAN s_sim_is_ext_dualsim = FALSE;

#ifdef SIM_3G_USIM_SUPPORT
SIM_APPLICATION_E s_sim_is_usim[SIM_CARD_NUM] = {SIM_2G_APPLICATION, SIM_2G_APPLICATION, SIM_2G_APPLICATION, SIM_2G_APPLICATION};
SIM_AID_S s_sim_usim_aid[SIM_CARD_NUM];
STATIC uint8 s_sim_usim_is_checked[SIM_CARD_NUM] = {0, 0, 0, 0};
uint8   g_sim_netpar_file_size[SIM_CARD_NUM] = {0, 0, 0, 0};
#endif

BOOLEAN s_sim_cdma_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
#ifdef SIM_CDMA_CARD_SUPPORT
BOOLEAN s_sim_cdma_only_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
#endif

uint32 s_sim_sim_poll_time[SIM_CARD_NUM] = {0, 0, 0, 0};
uint32 s_sim_call_poll_time[SIM_CARD_NUM] = {0, 0, 0, 0};
uint32 s_sim_stk_poll_time[SIM_CARD_NUM] = {0, 0, 0, 0};

#ifdef SIM_PLUG_IN_SUPPORT
static BOOLEAN s_sim_plug_in_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
#ifdef GSM_CUSTOMER_AFP_SUPPORT
static BOOLEAN s_sim_after_plugin_wait_app_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
#endif
#endif

uint8 g_sim_cphs_vm_record_len[SIM_CARD_NUM] = {0, 0, 0, 0};
uint8 g_sim_r4_vm_record_len[SIM_CARD_NUM] = {0, 0, 0, 0};
uint8 g_sim_r4_vm_mbi_record_len[SIM_CARD_NUM] = {0, 0, 0, 0};
uint8 g_sim_r4_vm_mwis_record_len[SIM_CARD_NUM] = {0, 0, 0, 0};
uint8 g_sim_r4_vm_mwis_record_num[SIM_CARD_NUM] = {0, 0, 0, 0};
uint8 g_sim_r4_vm_vmwflag_len[SIM_CARD_NUM] = {0, 0, 0, 0};

#elif defined (MULTI_SIM_SYS_TRI)
#ifdef SIM_TEST_IN_WIN32
BOOLEAN g_is_mobile_power_on[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
BOOLEAN g_is_sim_card_inserted[SIM_CARD_NUM] = {TRUE, TRUE, TRUE};
#endif

#ifdef SIM_SIMULATOR
uint32  g_sim_info_task_id[SIM_CARD_NUM] = {P_SIM_WIN_DUMMY, P_SIM_WIN_DUMMY, P_SIM_WIN_DUMMY}; //the task id that the sim layer information to
#else
uint32  g_sim_info_task_id[SIM_CARD_NUM] = {P_MN, P_MN, P_MN};
#endif

#ifdef GSM_DPHONE_VER2_SUPPORT
static BOOLEAN s_sim_is_dphone_ver2_need_pending[SIM_CARD_NUM] = {TRUE, TRUE, TRUE};
static BOOLEAN s_sim_is_initialize_skip_refresh[SIM_CARD_NUM] = {TRUE, TRUE, TRUE};
#endif

uint32  g_sim_simat_task_id[SIM_CARD_NUM] = {P_MN, P_MN, P_MN};

static BOOLEAN s_sim_is_in_refresh[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};

uint32  g_sim_task_id[SIM_CARD_NUM] = {P_SIM, P_SIM, P_SIM};            //the sim layer's taks id

SIM_CARD_SETTING_T  g_sim_card_setting[SIM_CARD_NUM] = {0};             //the setting of SIM card
SIM_CARD_STATE_E g_sim_card_state[SIM_CARD_NUM] = {SIM_NO_POWER_STATE, SIM_NO_POWER_STATE, SIM_NO_POWER_STATE}; //the sim card's state
SIM_CARD_CPHSINFO_T g_sim_cphsinfo[SIM_CARD_NUM] = {0}; //To save the CPHS service table

//------------------------local variable define-------------------------
STATIC SCI_TMID s_sim_idle_timer[SIM_CARD_NUM] = {NULL, NULL, NULL}; //the timer used to idle the SIM card;
STATIC SCI_TMID s_sim_check_period_timer[SIM_CARD_NUM] = {NULL, NULL, NULL}; //the timer used to check the SIM card presentation period
STATIC SCI_TMID s_sim_present_timer[SIM_CARD_NUM] = {NULL, NULL, NULL}; //the timer used to confirm the SIM card exist or not by
//the status fuc

STATIC SCI_TMID s_sim_pin1puk1_wait_timer[SIM_CARD_NUM] = {NULL, NULL, NULL}; //this timer is used to wait users to input PIN1/PUK1 when power on. if time out, enable arm deep sleep.

STATIC BOOLEAN  s_b_sim_present_timer[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};

STATIC SCI_TMID s_sim_restore_waiting_timer[SIM_CARD_NUM] = {NULL, NULL, NULL}; //this timer is used to waiting for the restore of the sim,
//if the sim restore is failed,the timer expired and the sim task assert

STATIC BOOLEAN  s_sim_keep_state[SIM_CARD_NUM];                 //set this value to false to let the sim layer
//change to the current state process function
STATIC uint8 s_sim_restore_count[SIM_CARD_NUM] = {0, 0, 0};
STATIC uint8 s_total_sim_reset_times[SIM_CARD_NUM] = {0, 0, 0}; /*new adds by king.li to make the card reset no more than 3 times*/
STATIC uint8 s_total_sim_change_reset_times[SIM_CARD_NUM] = {0, 0, 0}; //

STATIC BOOLEAN s_sim_arm_sleep[SIM_CARD_NUM] = {TRUE, TRUE, TRUE};
STATIC BOOLEAN s_sim_restore_with_3v[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
STATIC uint32 s_sim_cardout_reset_times[SIM_CARD_NUM] = {0, 0, 0};

BOOLEAN s_sim_is_ext_dualsim = FALSE;

#ifdef SIM_3G_USIM_SUPPORT
SIM_APPLICATION_E s_sim_is_usim[SIM_CARD_NUM] = {SIM_2G_APPLICATION, SIM_2G_APPLICATION, SIM_2G_APPLICATION};
SIM_AID_S s_sim_usim_aid[SIM_CARD_NUM];
STATIC uint8 s_sim_usim_is_checked[SIM_CARD_NUM] = {0, 0, 0};
uint8   g_sim_netpar_file_size[SIM_CARD_NUM] = {0, 0, 0};
#endif

BOOLEAN s_sim_cdma_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
#ifdef SIM_CDMA_CARD_SUPPORT
BOOLEAN s_sim_cdma_only_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
#endif

uint32 s_sim_sim_poll_time[SIM_CARD_NUM] = {0, 0, 0};
uint32 s_sim_call_poll_time[SIM_CARD_NUM] = {0, 0, 0};
uint32 s_sim_stk_poll_time[SIM_CARD_NUM] = {0, 0, 0};

#ifdef SIM_PLUG_IN_SUPPORT
static BOOLEAN s_sim_plug_in_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
#ifdef GSM_CUSTOMER_AFP_SUPPORT
static BOOLEAN s_sim_after_plugin_wait_app_flag[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
#endif
#endif

uint8 g_sim_cphs_vm_record_len[SIM_CARD_NUM] = {0, 0, 0};
uint8 g_sim_r4_vm_record_len[SIM_CARD_NUM] = {0, 0, 0};
uint8 g_sim_r4_vm_mbi_record_len[SIM_CARD_NUM] = {0, 0, 0};
uint8 g_sim_r4_vm_mwis_record_len[SIM_CARD_NUM] = {0, 0, 0};
uint8 g_sim_r4_vm_mwis_record_num[SIM_CARD_NUM] = {0, 0, 0};
uint8 g_sim_r4_vm_vmwflag_len[SIM_CARD_NUM] = {0, 0, 0};

#else//MULTI_SIM_SYS_TRI

#ifdef SIM_TEST_IN_WIN32
BOOLEAN g_is_mobile_power_on[SIM_CARD_NUM] = {FALSE, FALSE};
BOOLEAN g_is_sim_card_inserted[SIM_CARD_NUM] = {TRUE, TRUE};
#endif

#ifdef SIM_SIMULATOR
uint32 g_sim_info_task_id[SIM_CARD_NUM] = {P_SIM_WIN_DUMMY, P_SIM_WIN_DUMMY};   //the task id that the sim layer information to
#else
uint32  g_sim_info_task_id[SIM_CARD_NUM] = {P_MN, P_MN};
#endif

#ifdef GSM_DPHONE_VER2_SUPPORT
static BOOLEAN s_sim_is_dphone_ver2_need_pending[SIM_CARD_NUM] = {TRUE, TRUE};
static BOOLEAN s_sim_is_initialize_skip_refresh[SIM_CARD_NUM] = {TRUE, TRUE};
#endif

uint32  g_sim_simat_task_id[SIM_CARD_NUM] = {P_MN, P_MN};

static BOOLEAN s_sim_is_in_refresh[SIM_CARD_NUM] = {FALSE, FALSE};

uint32  g_sim_task_id[SIM_CARD_NUM] = {P_SIM, P_SIM};               //the sim layer's taks id

SIM_CARD_SETTING_T  g_sim_card_setting[SIM_CARD_NUM] = {0};             //the setting of SIM card
SIM_CARD_STATE_E g_sim_card_state[SIM_CARD_NUM] = {SIM_NO_POWER_STATE, SIM_NO_POWER_STATE}; //the sim card's state
SIM_CARD_CPHSINFO_T g_sim_cphsinfo[SIM_CARD_NUM] = {0}; //To save the CPHS service table

//------------------------local variable define-------------------------
STATIC SCI_TMID s_sim_idle_timer[SIM_CARD_NUM] = {NULL, NULL}; //the timer used to idle the SIM card;
STATIC SCI_TMID s_sim_check_period_timer[SIM_CARD_NUM] = {NULL, NULL}; //the timer used to check the SIM card presentation period
STATIC SCI_TMID s_sim_present_timer[SIM_CARD_NUM] = {NULL, NULL}; //the timer used to confirm the SIM card exist or not by
//the status fuc

STATIC SCI_TMID s_sim_pin1puk1_wait_timer[SIM_CARD_NUM] = {NULL, NULL}; //this timer is used to wait users to input PIN1/PUK1 when power on. if time out, enable arm deep sleep.

STATIC BOOLEAN  s_b_sim_present_timer[SIM_CARD_NUM] = {FALSE, FALSE};

STATIC SCI_TMID s_sim_restore_waiting_timer[SIM_CARD_NUM] = {NULL, NULL}; //this timer is used to waiting for the restore of the sim,
//if the sim restore is failed,the timer expired and the sim task assert

STATIC BOOLEAN  s_sim_keep_state[SIM_CARD_NUM];                 //set this value to false to let the sim layer
//change to the current state process function
STATIC uint32 s_sim_restore_count[SIM_CARD_NUM] = {0};
STATIC uint8 s_total_sim_reset_times[SIM_CARD_NUM] = {0, 0}; /*new adds by king.li to make the card reset no more than 3 times*/
STATIC uint8 s_total_sim_change_reset_times[SIM_CARD_NUM] = {0, 0}; //

STATIC BOOLEAN s_sim_arm_sleep[SIM_CARD_NUM] = {TRUE, TRUE};
STATIC BOOLEAN s_sim_restore_with_3v[SIM_CARD_NUM] = {FALSE, FALSE};
STATIC uint32 s_sim_cardout_reset_times[SIM_CARD_NUM] = {0, 0};

BOOLEAN s_sim_is_ext_dualsim = FALSE;

#ifdef SIM_3G_USIM_SUPPORT
SIM_APPLICATION_E s_sim_is_usim[SIM_CARD_NUM] = {SIM_2G_APPLICATION, SIM_2G_APPLICATION};
SIM_AID_S s_sim_usim_aid[SIM_CARD_NUM];
STATIC uint8 s_sim_usim_is_checked[SIM_CARD_NUM] = {0, 0};
uint8   g_sim_netpar_file_size[SIM_CARD_NUM] = {0, 0};
#endif

BOOLEAN s_sim_cdma_flag[SIM_CARD_NUM] = {FALSE, FALSE};
#ifdef SIM_CDMA_CARD_SUPPORT
BOOLEAN s_sim_cdma_only_flag[SIM_CARD_NUM] = {FALSE, FALSE};
#endif

uint32 s_sim_sim_poll_time[SIM_CARD_NUM] = {0, 0};
uint32 s_sim_call_poll_time[SIM_CARD_NUM] = {0, 0};
uint32 s_sim_stk_poll_time[SIM_CARD_NUM] = {0, 0};

#ifdef SIM_PLUG_IN_SUPPORT
static BOOLEAN s_sim_plug_in_flag[SIM_CARD_NUM] = {FALSE, FALSE};
#ifdef GSM_CUSTOMER_AFP_SUPPORT
static BOOLEAN s_sim_after_plugin_wait_app_flag[SIM_CARD_NUM] = {FALSE, FALSE};
#endif
#endif

uint8 g_sim_cphs_vm_record_len[SIM_CARD_NUM] = {0, 0};
uint8 g_sim_r4_vm_record_len[SIM_CARD_NUM] = {0, 0};
uint8 g_sim_r4_vm_mbi_record_len[SIM_CARD_NUM] = {0, 0};
uint8 g_sim_r4_vm_mwis_record_len[SIM_CARD_NUM] = {0, 0};
uint8 g_sim_r4_vm_mwis_record_num[SIM_CARD_NUM] = {0, 0};
uint8 g_sim_r4_vm_vmwflag_len[SIM_CARD_NUM] = {0, 0};
#endif//MULTI_SIM_SYS_TRI

//for the SIM SLOT mapping.
#if defined (MULTI_SIM_SYS_SINGLE) || defined (MULTI_SIM_SYS_DUAL) || defined (MULTI_SIM_SYS_TRI) || defined (MULTI_SIM_SYS_QUAD)
static BOOLEAN s_sim_slot_0123_to_1023_flag = FALSE;
#if defined (MULTI_SIM_SYS_TRI) || defined (MULTI_SIM_SYS_QUAD)
static BOOLEAN s_sim_slot_0123_to_0213_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_2103_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_1203_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_2013_flag = FALSE;
#endif
#ifdef MULTI_SIM_SYS_QUAD
static BOOLEAN s_sim_slot_0123_to_0132_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_0231_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_0312_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_0321_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_1032_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_1230_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_1302_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_1320_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_2031_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_2130_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_2301_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_2310_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_3012_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_3021_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_3120_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_3102_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_3201_flag = FALSE;
static BOOLEAN s_sim_slot_0123_to_3210_flag = FALSE;
#endif
#endif

#ifdef SIM_3G_USIM_SUPPORT
static BOOLEAN s_sim_case_test_flag = FALSE;
#endif

static uint8 s_sim_reset_total_times[SIM_CARD_NUM] = {0};
#if defined SIM_DROP_RESTORE_SUPPORT && defined SIM_DROP_RESTORE_DELAY_PBSMS_READ
STATIC SCI_TMID s_sim_delay_smspb_timer[4]={NULL,NULL,NULL,NULL};
#endif

#ifdef SIM_FILL_PHONEBOOK_FULL//test func when do phonebook debug
extern void SIM_WritePhonebookFull(MN_DUAL_SYS_E  dual_sys);
extern BOOLEAN SIM_GetWritePhonebookFull(MN_DUAL_SYS_E  dual_sys);
#endif

#ifdef SIM_PLUG_IN_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK
#define SIM_PLUG_IN_BY_CHECK_MIN_TIME 8000
static BOOLEAN s_sim_plug_in_by_check_card_id = 0;// 0-->SIM1, 1-->SIM2
static uint32 s_sim_plug_in_by_check_interval = SIM_PLUG_IN_BY_CHECK_MIN_TIME;//every 8s check SIM
STATIC SCI_TMID s_sim_plug_in_check_timer[3] = {NULL,NULL,NULL};
#endif
#endif

//-----------------------local function declare----------------------------
/**********************************************************************
//    Description:
//      Check the Macro define for SIM layer to ensure it's in the
//    correct value
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void CheckSIMMacro(void);

/**********************************************************************
//    Description:
//      when still not power on the SIM card,the SIM layer can not supply
//    any service but the start up SIM request to power on the SIM.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void NoPowerStateProcessor(uint32 agrc);

/********************************************************************
//    DESCRIPTION
//      the sim present check period tiemr expiration function,send a
//    card present check indication to the sim layer itself
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SendCardPresentCheckInd(
    uint32 lParam   //the parameter of the timer function
);

/********************************************************************
//    DESCRIPTION
//      the function of NULL state.the NULL state is mean when power
//    on the SIM and there are still no indication that whether the
//    SIM card is present in the SIM slot,so just process the two signal
//    from the SIM drive:card out and card in.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void NullStateProcessor(uint32 agrc);

/********************************************************************
//    DESCRIPTION
//      When there are SIM card in the SIM slot,the next step is receive
//    the ATR  data from the SIM card.and decode the ATR data to make
//    sure the SIM card is inserted ok,the get the SIM cad specific data
//    for initialize using.at this time also no service request will be
//    suppled.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void ATRStateProcessor(uint32 agrc);

/********************************************************************
//    DESCRIPTION
//      After the successful ATR proccess,the other layer can send
//    INITIALIZE request service to INITIALIZE the SIM card.and at
//    this state,just only INITIALIZE request service is accepted.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void InitializeStateProcessor(uint32 agrc);

/********************************************************************
//    DESCRIPTION
//      after the successful Initialize,the SIM card can now supply
//    kinds of service,and this state is the SIM working state.the
//    other layer now can request all kinds service.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void ActiveStateProcessor(uint32 agrc);

/********************************************************************
//    DESCRIPTION
//      the sim card not presnet,the sim card present judge timer expiration
//    function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void CardNotPresent(
    uint32 lParam   //the parameter of the timer function
);

/********************************************************************
//    DESCRIPTION
//      get the gsm data time out
//    function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void GetGSMDataTimeOut(
    uint32 lParam   //the parameter of the timer function
);

/********************************************************************
//    DESCRIPTION
//      start to check card present present periodly
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void StartCardPresentCheck(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//      start or stop checking the card present according to the ME call state.
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void SwitchCheckCardPresent(uint32 cur_sim_task, SIM_SIG_SWITCH_CHECK_CARD_PRES_REQ_T *psig);

/********************************************************************
//    DESCRIPTION
//      check the card present by the STATUS command
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void CheckCardPresent(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//      Process the startup request from the MN layer
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void StartupSIM(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      there are only one service can SIM layer supply at the seem time.
//    so when there are a service accept by the SIM layer,SIM task should
//    do some operation to make sure will not accept the second service
//    request and save the information of the accepted request.if the idle
//    timer is active,the timer should be deactived here also.
//    correct value
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void EnterService(
    uint32 cur_sim_task,
    xSignalHeaderRec *psig  //the service request signal
);

/**********************************************************************
//    Description:
//      there are only one service can SIM layer supply at the seem time.
//    so when a service is finisehed should call this function to ready
//    for next service request.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void FinishService(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//      the function that process some of the SIM card Interrupt
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void IntProcess(
    uint32 cur_sim_task,
    SIM_SIG_INT_IND_T *psig //the SIM card interrupt signal
);

/********************************************************************
//    DESCRIPTION
//      the function that Deal with the Card out Interrupt from SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void DealCardOut(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//      the function that Deal with the Card in Interrupt from SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void DealCardIn(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//      the function that Deal with the RX Parity Error exception from
//    the SIM driver
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void DealRxParityError(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//      the function that Deal with the TX Parity Error exception from
//    the SIM driver
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void DealTxParityError(uint32 cur_sim_task);

#ifdef SIM_PLUG_IN_SUPPORT
/********************************************************************
//    DESCRIPTION
//      the function that Deal with the Card in Interrupt from driver
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC void DealSIMPlugIn(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//      the function that Deal with the Card in Interrupt from driver
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC void DealSIMPullOut(uint32 cur_sim_task);
#endif

/********************************************************************
//    DESCRIPTION
//      A common part of CheckCardStatus, which will re-start sim.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC void CheckCardStatus_ReStartSIM(
    uint32 cur_sim_task,
    BOOLEAN is_sim_fail,
    BOOLEAN is_volt_rise);

/********************************************************************
//    DESCRIPTION
//      This Function is to get the SIM card Insert Status by the ATR data from
//    the SIM card,and then do the coressponding operation of different insert
//    status.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void CheckCardStatus(
    uint32 current_sim_card,
    SIM_SIG_CARD_INSERTED_IND_T *psig //the atr data signal
);

#ifdef SIM_PLUG_IN_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK
/********************************************************************
//    DESCRIPTION
//      this fuction do the service of the query on the card status
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void QueryCardStatus(uint32 cur_sim_task);
#endif
#endif

/********************************************************************
//    DESCRIPTION
//      the SIM card have two serivce state and the process of service
//    request signal is different:
//    1.can not acctep new service request from other layer:only signal
//    from the SIM drive(SIM layer self) can be process and service
//    request  should be save to temporary queue for laterly process.
//    2.can accept new service request from other layer
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC xSignalHeaderRec *RecevieSignal(uint32 cur_sim_task);   //return value:the singal received

/********************************************************************
//    DESCRIPTION
//     Enable deep sleep
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void EnableDeepSleep(uint32 lParam);

#ifndef SIM_TEST_IN_WIN32
/**********************************************************************
//    Description:
//     Reject the SIM card, deactive the SIM card and Power off the SIM card.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void Dualsim_RejectSimCard(uint32 cur_sim_task);
#endif

/********************************************************************
//    DESCRIPTION
//      the sim idle timer expiration function,send a clock stop
//    indication to the sim layer itself
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void IdleCard(
    uint32 lParam   //the parameter of the timer function
);

/********************************************************************
//    DESCRIPTION
//    dispatch the service requst signal to it's process function in
//    active state.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void ServiceRequestDispatch(
    uint32 cur_sim_task,
    xSignalHeaderRec *psig  //the servuce request signal
);

/********************************************************************
//    DESCRIPTION
//      the sim restore failed,the sim driver has no response, the restore waiting timer
//    expired
//    function
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void RestoreExpired(
    uint32 lParam   //the parameter of the timer function
);

#ifdef SIM_3G_USIM_SUPPORT
/********************************************************************
//    DESCRIPTION
//     To judge the SIM is 2G or 3G sim card after power on.
//    function
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E SIM_IsSIMorUSIM(uint32 cur_sim_task);
#endif

/********************************************************************
//    DESCRIPTION
//    get the SIM card's volt info in ATR data
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static BOOLEAN SIM_IsGetATRVoltInfo(uint32 cur_sim_task, uint8 *atr_volt_byte);

#ifdef SIM_PLUG_IN_SUPPORT
/********************************************************************
//    DESCRIPTION
//
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMMAIN_IsSIMPlugIn(uint32 cur_sim_task, BOOLEAN is_plug_in);
#endif

/********************************************************************
//    DESCRIPTION
//    Read NV to get the SIM Slot mapping configure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIM_Exchange_Init_Card_Slot_Label(void);

/********************************************************************
//    DESCRIPTION
//    Remap sim card id according to the drv information
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static uint8 SIM_Exchange_Remap_SimId_From_Drv(uint8 exchange_card_id);

/********************************************************************
//    DESCRIPTION
//   Judge the corresponding Idle Timer is active or not, then stop sim clk stop.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMMAIN_ClockStop(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//   create the mutex for the multi sim task with few sim controller
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMMAIN_InitMutex(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//   several sim tasks wait and get the mutex to controll the sim hardware interface
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMMAIN_GetMutex(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//   put the mutex to give up the sim hardware interface
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMMAIN_PutMutex(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//      Warm reset the sim card
//    Global resource dependence :
//    Author:Jacky.Ying
//    Note:
***********************************************************************/
static void SIMMAIN_RefreshWarmReset(uint32 cur_sim_task);

/********************************************************************
//    DESCRIPTION
//      send refresh reset result to MN
//    Global resource dependence :
//    Author: Jacky.Ying
//    Note:
***********************************************************************/
static void SIMMAIN_SendRefreshResetCnf(
    uint32 cur_sim_task,
    BOOLEAN reset_result     //TRUE,reset ok,FALSE,reset fail
);

#if defined SIM_DROP_RESTORE_SUPPORT && defined SIM_DROP_RESTORE_DELAY_PBSMS_READ
/********************************************************************
//    DESCRIPTION
//      
//    Global resource dependence :
//    Author: wuding.yang
//    Note:
***********************************************************************/
static void SIM_Delay_Read(uint32 lParam);
#endif
#ifdef SIM_PLUG_IN_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK
/********************************************************************
//    DESCRIPTION
//      
//
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMMAIN_PLUG_IN_By_Check(uint32 lParam);
#endif
#endif

//-----------------------the function define-------------------------
/********************************************************************
//    DESCRIPTION
//      this is the call back function for our SIM driver,to make the
//    SIM driver can notify the SIM layer with kind of signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void DriverNotifySIM(
    SIMHAL_MSG *msg //the message parameter of the function
)
{
    xSignalHeaderRec *psig = NULL;
    uint8 card_id = (uint8)msg->wparam;

    card_id = SIM_Exchange_Remap_SimId_From_Drv((uint8)msg->wparam);

    //cehck the meesgae type
    switch(msg->message)
    {
            //the message is a card inserted one
        case SIM_CARD_INSERT:
            SCI_CREATE_SIGNAL(psig, SIM_CARD_INSERTED_IND, sizeof(SIM_SIG_CARD_INSERTED_IND_T), g_sim_task_id[card_id]);
            ((SIM_SIG_CARD_INSERTED_IND_T *)psig)->insert_status = ((SIMDRV_CARD_INSERT_T *)msg->lparam)->insert_status;
            ((SIM_SIG_CARD_INSERTED_IND_T *)psig)->t_protocol = ((SIMDRV_CARD_INSERT_T *)msg->lparam)->t_protocol;
            ((SIM_SIG_CARD_INSERTED_IND_T *)psig)->is_3v = (SIMDRV_WORKING_VOLT_3V == ((SIMDRV_CARD_INSERT_T *)msg->lparam)->working_volt);
            SCI_SEND_SIGNAL(psig, g_sim_task_id[card_id]);
            break;
            //the message is SIM data response one
        case SIM_DATA_RESPONSE:

            if((0x60 == (0xF0 & (((SIMDRV_DATA_RESPONSE_T *)msg->lparam)->sw1)))
                || (0x90 == (0xF0 & (((SIMDRV_DATA_RESPONSE_T *)msg->lparam)->sw1))))
            {
                SCI_CREATE_SIGNAL(psig, SIM_RESPONSE_IND, sizeof(SIM_SIG_RESPONSE_IND_T), g_sim_task_id[card_id]);
                ((SIM_SIG_RESPONSE_IND_T *)psig)->data = ((SIMDRV_DATA_RESPONSE_T *)msg->lparam)->data;
                ((SIM_SIG_RESPONSE_IND_T *)psig)->data_len = ((SIMDRV_DATA_RESPONSE_T *)msg->lparam)->data_len;
                ((SIM_SIG_RESPONSE_IND_T *)psig)->sw1 = ((SIMDRV_DATA_RESPONSE_T *)msg->lparam)->sw1;
                ((SIM_SIG_RESPONSE_IND_T *)psig)->sw2 = ((SIMDRV_DATA_RESPONSE_T *)msg->lparam)->sw2;
            }
            else
            {
                SCI_CREATE_SIGNAL(psig, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), g_sim_task_id[card_id]);
                ((SIM_SIG_INT_IND_T *)psig)->int_type = TB_SIMINT_CARD_UNRESPONSE;
            }

            SCI_SEND_SIGNAL(psig, g_sim_task_id[card_id]);
            break;
            //the message is a SIM exception one
        case SIM_EXCEPTION_INT:
            SCI_CREATE_SIGNAL(psig, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), g_sim_task_id[card_id]);
            ((SIM_SIG_INT_IND_T *)psig)->int_type = ((TB_SIM_INT_T *)msg->lparam)->int_type;
            SCI_SEND_SIGNAL(psig, g_sim_task_id[card_id]);
            break;
        default:
            //SCI_TRACE_LOW:"SIM::DriverNotifySIM, msg->message= %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_772_112_2_17_22_59_34_4532, (uint8 *)"d", msg->message);
            break;
    }
}

/**********************************************************************
//    Description:
//      The entry function of SIM layer.the major function of SIM layer.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMCARD_Task(
    uint32 agrc,  //the argument num
    void *argv    //the argument array
)
{
    //the different signal process function of different state
    void (*sim_state_process_funtion[])(uint32) =
    {
        NoPowerStateProcessor,
        NullStateProcessor,
        ATRStateProcessor,
        InitializeStateProcessor,
        ActiveStateProcessor
    };
    uint32 cur_sim_task = agrc;

    //check the SIM macro define
    CheckSIMMacro();
    //Initialize the SIM layer
    SIMMAIN_InitSIMTask(cur_sim_task);

    //SCI_TRACE_LOW:"\n\rSIM::SIM Layer Task %d StartUp! %d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_807_112_2_17_22_59_34_4533, (uint8 *)"dd", cur_sim_task, s_sim_is_ext_dualsim);
    //when in this state, can enable deep sleep
    EnableDeepSleep(cur_sim_task);

#ifdef MULTI_SIM_SYS_QUAD
#if defined (MULTI_SIM_SYS_QUAD_TO_DUAL_WITHOUT6302)

    if((0 == cur_sim_task) || (1 == cur_sim_task))
#elif defined (MULTI_SIM_SYS_QUAD_TO_TRI_SINGLE6302_BBSIM0)
    if((0 == cur_sim_task) || (1 == cur_sim_task) || (2 == cur_sim_task))
#elif defined (MULTI_SIM_SYS_QUAD_TO_TRI_SINGLE6302_BBSIM1)
    if((0 == cur_sim_task) || (1 == cur_sim_task) || (2 == cur_sim_task))
#endif
    {
        if(1 == cur_sim_task)
        {
            SCI_Sleep(5);
        }
        else if(2 == cur_sim_task)
        {
            SCI_Sleep(10);
        }
        else if(3 == cur_sim_task)
        {
            SCI_Sleep(15);
        }
    }

#endif

    //the signal process endless loop
    for(;;)
    {
        //call the process function depending the cur sim card state
        sim_state_process_funtion[g_sim_card_state[cur_sim_task]](cur_sim_task);
    }
}

/**********************************************************************
//    Description:
//      Check the Macro define for SIM layer to ensure it's in the
//    correct value
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void CheckSIMMacro(void)
{
    //the valide language valide extent is [1:20]
#if( !((SIM_MAX_VALIDATE_LANGUAGE_NUM > 0) && (SIM_MAX_VALIDATE_LANGUAGE_NUM <= 20)) )

#error SIM_MAX_VALIDATE_LANGUAGE_NUM

#endif

    //the gid length exten is [16:255]
#if( !((SIM_MAX_GID_LEN >= 16)&&(SIM_MAX_GID_LEN < SIM_MAX_RECORD_LEN)) )

#error SIM_MAX_GID_LEN

#endif

    //the valide cbmid number extent is [8:30]
#if( !((SIM_MAX_VALIDATE_CBMI_NUM >= 8) && (SIM_MAX_VALIDATE_CBMI_NUM <= 30)) )

#error SIM_MAX_VALIDATE_CBMI_NUM

#endif

    //the cbmi number in EFcbmid exten is [8:20]
#if( !((SIM_MAX_CBMID_CBMI_NUM >= 8) && (SIM_MAX_CBMID_CBMI_NUM <= 20 )) )

#error SIM_MAX_CBMID_CBMI_NUM

#endif

    //the cbmir number extent is [8:20]
#if( !((SIM_MAX_CBMIR_RANGE_NUM >= 8 )&&(SIM_MAX_CBMIR_RANGE_NUM <= 20)) )

#error SIM_MAX_CBMIR_RANGE_NUM

#endif

    //the co_operate network number extent is [8:20]
#if( !((SIM_MAX_CO_OPERATIVE_NET_LIST_NUM >= 8) && (SIM_MAX_CO_OPERATIVE_NET_LIST_NUM <= 20)) )

#error SIM_MAX_CO_OPERATIVE_NET_LIST_NUM

#endif

    //the max number in dialling number exten is [20:42]
#if( !((SIM_MAX_DN_NUMBER_LEN >= 20) &&(SIM_MAX_DN_NUMBER_LEN <= 42)) )

#error SIM_MAX_DN_NUMBER_LEN

#endif

    //the alpha id number in dialling number extent is [0:241]
#if( !((SIM_MAX_DN_ALPHA_IDENTIFY_LEN >= 0) && (SIM_MAX_DN_ALPHA_IDENTIFY_LEN <= 241)) )

#error SIM_MAX_DN_ALPHA_IDENTIFY_LEN

#endif

    //the max Abreviated dialling number in the sim card extent is [20:255]
#if( !((SIM_MAX_ADN_NUM >= 20) && (SIM_MAX_ADN_NUM <= SIM_MAX_RECORD_NUM)) )

#error SIM_MAX_ADN_NUM

#endif

    //the max fixed dialling number in the SIM card extent is [5:20]
#if( !((SIM_MAX_FDN_NUM >= 5) && (SIM_MAX_FDN_NUM <= 20)) )

#error SIM_MAX_FDN_NUM

#endif

    //the max last number dialed extent is [5:20]
#if( !((SIM_MAX_LND_NUM >= 5) && (SIM_MAX_LND_NUM <= 20)) )

#error SIM_MAX_LND_NUM

#endif

    //the max MSISDN dialling number extent is [5:20]
#if( !((SIM_MAX_MSISDN_NUM >= 5) && (SIM_MAX_MSISDN_NUM <= 20)) )

#error SIM_MAX_MSISDN_NUM

#endif

    //the max service dialling number extent is [5:30]
#if( !((SIM_MAX_SDN_NUM >= 5) && (SIM_MAX_SDN_NUM <= 30)) )

#error SIM_MAX_SDN_NUM

#endif

    //the max barred dialling number extent is [5:20]
#if( !((SIM_MAX_BDN_NUM >= 5) && (SIM_MAX_BDN_NUM <= 20)) )

#error SIM_MAX_BDN_NUM

#endif

    //the max Short message number extent is [10:255]
#if( !((SIM_MAX_SMS_NUM >= 10) && (SIM_MAX_SMS_NUM <= SIM_MAX_RECORD_NUM)) )

#error SIM_MAX_SMS_NUM

#endif

    //the max short message parameter number extent is [5:255]
#if( !((SIM_MAX_SMSP_NUM >= 5) && (SIM_MAX_SMSP_NUM <= SIM_MAX_RECORD_NUM)) )

#error SIM_MAX_SMSP_NUM

#endif

    //the max short message parameter alpha id len extent is [0:20]
#if( !((SIM_MAX_SMSP_ALPHA_ID_LEN >= 0) && (SIM_MAX_SMSP_ALPHA_ID_LEN <= 100)) )

#error SIM_MAX_SMSP_ALPHA_ID_LEN

#endif
}

/**********************************************************************
//    Description:
//      there are only one service can SIM layer supply at the seem time.
//    so when there are a service accept by the SIM layer,SIM task should
//    do some operation to make sure will not accept the second service
//    request and save the information of the accepted request.if the idle
//    timer is active,the timer should be deactivated here also.
//    correct value
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void EnterService(
    uint32 cur_sim_task,
    xSignalHeaderRec *psig  //the service request signal
)
{
    //check is the idle timer is avtived
    if(SCI_IsTimerActive(s_sim_idle_timer[cur_sim_task]))
    {
        SCI_DeactiveTimer(s_sim_idle_timer[cur_sim_task]);
    }

#ifndef WIN32
    DisableDeepSleep(cur_sim_task);//raistlin.kong 2003-10-16 prevent the arm go deep sleep
#endif  // WIN32
}

/**********************************************************************
//    Description:
//      there are only one service can SIM layer supply at the seem time.
//    so when a service is finisehed should call this function to ready
//    for next service request.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void FinishService(uint32 cur_sim_task)
{
    uint32 other_task = 1;
#ifdef MULTI_SIM_SYS_TRI
    uint32 third_task = 2;
#endif
#ifdef MULTI_SIM_SYS_QUAD
    uint32 third_task = 2;
    uint32 fourth_task = 3;
#endif

    //start the Idle timer to make sure the timer of SIM will be idled
    if(s_sim_is_ext_dualsim)
    {
        if((SIM_ACTIVE_STATE == g_sim_card_state[cur_sim_task]) 
#ifndef _SUPPORT_IPC_
            && (SIM_CLOCK_S_FORBIDEN != g_sim_card_setting[cur_sim_task].gsm_data.clock_stop_type)
#endif
            )
        {
            if(1 == cur_sim_task)
            {
                other_task = 0;
#ifdef MULTI_SIM_SYS_TRI
                third_task = 2;
#endif
#ifdef MULTI_SIM_SYS_QUAD
                fourth_task = 3;
#endif
            }

#ifdef MULTI_SIM_SYS_TRI
            else if(2 == cur_sim_task)
            {
                other_task = 0;
                third_task = 1;
            }

#elif defined (MULTI_SIM_SYS_QUAD)
            else if(2 == cur_sim_task)
            {
                other_task = 0;
                third_task = 1;
                fourth_task = 3;
            }
            else if(3 == cur_sim_task)
            {
                other_task = 0;
                third_task = 1;
                fourth_task = 2;

            }

#endif

            //if the other card is in no power state or null state,can the clock stop
            //if the other card enable clock stop,can the clock stop
#ifdef MULTI_SIM_SYS_TRI

            if(((SIM_NO_POWER_STATE == g_sim_card_state[other_task]) ||
                (SIM_NULL_STATE == g_sim_card_state[other_task]) ||
                (SIM_CLOCK_S_FORBIDEN != g_sim_card_setting[other_task].gsm_data.clock_stop_type))
                && ((SIM_NO_POWER_STATE == g_sim_card_state[third_task]) ||
                    (SIM_NULL_STATE == g_sim_card_state[third_task]) ||
                    (SIM_CLOCK_S_FORBIDEN != g_sim_card_setting[third_task].gsm_data.clock_stop_type))
              )
#elif defined (MULTI_SIM_SYS_QUAD)
            if(((SIM_NO_POWER_STATE == g_sim_card_state[other_task]) ||
                (SIM_NULL_STATE == g_sim_card_state[other_task]) ||
                (SIM_CLOCK_S_FORBIDEN != g_sim_card_setting[other_task].gsm_data.clock_stop_type))
                && ((SIM_NO_POWER_STATE == g_sim_card_state[third_task]) ||
                    (SIM_NULL_STATE == g_sim_card_state[third_task]) ||
                    (SIM_CLOCK_S_FORBIDEN != g_sim_card_setting[third_task].gsm_data.clock_stop_type))
                && ((SIM_NO_POWER_STATE == g_sim_card_state[fourth_task]) ||
                    (SIM_NULL_STATE == g_sim_card_state[fourth_task]) ||
                    (SIM_CLOCK_S_FORBIDEN != g_sim_card_setting[fourth_task].gsm_data.clock_stop_type))
              )
#else
            if(((SIM_NO_POWER_STATE == g_sim_card_state[other_task]) ||
                (SIM_NULL_STATE == g_sim_card_state[other_task]) ||
                (SIM_CLOCK_S_FORBIDEN != g_sim_card_setting[other_task].gsm_data.clock_stop_type)))
#endif
            {
                //if the other sim task idle timer is running, stop it and use current sim task idle timer.
                if((NULL != s_sim_idle_timer[other_task]) && (SCI_IsTimerActive(s_sim_idle_timer[other_task])))
                {
                    SCI_DeactiveTimer(s_sim_idle_timer[other_task]);
                    s_sim_arm_sleep[other_task] = TRUE;
                }

#ifdef MULTI_SIM_SYS_TRI

                if((NULL != s_sim_idle_timer[third_task]) && (SCI_IsTimerActive(s_sim_idle_timer[third_task])))
                {
                    SCI_DeactiveTimer(s_sim_idle_timer[third_task]);
                    s_sim_arm_sleep[third_task] = TRUE;
                }

#elif defined (MULTI_SIM_SYS_QUAD)
#ifndef WIN32

                if(SIMHAL_IsParallelDualSIM())
                {
                    //SC6800H quadsim. 2 sim controller can work at the same time.
                    if(SIMHAL_MultiSim_Is6302())
                    {
                        if(SIMHAL_MultiSim_IsParallelDual6302())
                        {
                            if(0 == cur_sim_task)
                            {
                                if((NULL != s_sim_idle_timer[3]) && (SCI_IsTimerActive(s_sim_idle_timer[3])))
                                {
                                    SCI_DeactiveTimer(s_sim_idle_timer[3]);
                                    s_sim_arm_sleep[3] = TRUE;
                                }
                            }
                            else if(1 == cur_sim_task)
                            {
                                if((NULL != s_sim_idle_timer[2]) && (SCI_IsTimerActive(s_sim_idle_timer[2])))
                                {
                                    SCI_DeactiveTimer(s_sim_idle_timer[2]);
                                    s_sim_arm_sleep[2] = TRUE;
                                }
                            }
                            else if(2 == cur_sim_task)
                            {
                                if((NULL != s_sim_idle_timer[1]) && (SCI_IsTimerActive(s_sim_idle_timer[1])))
                                {
                                    SCI_DeactiveTimer(s_sim_idle_timer[1]);
                                    s_sim_arm_sleep[1] = TRUE;
                                }
                            }
                            else// if (3 == cur_sim_task)
                            {
                                if((NULL != s_sim_idle_timer[0]) && (SCI_IsTimerActive(s_sim_idle_timer[0])))
                                {
                                    SCI_DeactiveTimer(s_sim_idle_timer[0]);
                                    s_sim_arm_sleep[0] = TRUE;
                                }
                            }
                        }
                        else
                        {
                            //SC6800H quadsim, only 1 spi channel
                            if((NULL != s_sim_idle_timer[third_task]) && (SCI_IsTimerActive(s_sim_idle_timer[third_task])))
                            {
                                SCI_DeactiveTimer(s_sim_idle_timer[third_task]);
                                s_sim_arm_sleep[third_task] = TRUE;
                            }

                            if((NULL != s_sim_idle_timer[fourth_task]) && (SCI_IsTimerActive(s_sim_idle_timer[fourth_task])))
                            {
                                SCI_DeactiveTimer(s_sim_idle_timer[fourth_task]);
                                s_sim_arm_sleep[fourth_task] = TRUE;
                            }
                        }
                    }
                    else//SC6800H + 6303
                    {
                        if(1 == cur_sim_task)
                        {
                            if((NULL != s_sim_idle_timer[2]) && (SCI_IsTimerActive(s_sim_idle_timer[2])))
                            {
                                SCI_DeactiveTimer(s_sim_idle_timer[2]);
                                s_sim_arm_sleep[2] = TRUE;
                            }

                            if((NULL != s_sim_idle_timer[3]) && (SCI_IsTimerActive(s_sim_idle_timer[3])))
                            {
                                SCI_DeactiveTimer(s_sim_idle_timer[3]);
                                s_sim_arm_sleep[3] = TRUE;
                            }
                        }
                        else if(2 == cur_sim_task)
                        {
                            if((NULL != s_sim_idle_timer[1]) && (SCI_IsTimerActive(s_sim_idle_timer[1])))
                            {
                                SCI_DeactiveTimer(s_sim_idle_timer[1]);
                                s_sim_arm_sleep[1] = TRUE;
                            }

                            if((NULL != s_sim_idle_timer[3]) && (SCI_IsTimerActive(s_sim_idle_timer[3])))
                            {
                                SCI_DeactiveTimer(s_sim_idle_timer[3]);
                                s_sim_arm_sleep[3] = TRUE;
                            }
                        }
                        else// if (3 == cur_sim_task)
                        {
                            if((NULL != s_sim_idle_timer[1]) && (SCI_IsTimerActive(s_sim_idle_timer[1])))
                            {
                                SCI_DeactiveTimer(s_sim_idle_timer[1]);
                                s_sim_arm_sleep[1] = TRUE;
                            }

                            if((NULL != s_sim_idle_timer[2]) && (SCI_IsTimerActive(s_sim_idle_timer[2])))
                            {
                                SCI_DeactiveTimer(s_sim_idle_timer[2]);
                                s_sim_arm_sleep[2] = TRUE;
                            }
                        }
                    }
                }
                else
                {
                    // SC6600L quadsim, only 1 sim controller
                    if((NULL != s_sim_idle_timer[third_task]) && (SCI_IsTimerActive(s_sim_idle_timer[third_task])))
                    {
                        SCI_DeactiveTimer(s_sim_idle_timer[third_task]);
                        s_sim_arm_sleep[third_task] = TRUE;
                    }

                    if((NULL != s_sim_idle_timer[fourth_task]) && (SCI_IsTimerActive(s_sim_idle_timer[fourth_task])))
                    {
                        SCI_DeactiveTimer(s_sim_idle_timer[fourth_task]);
                        s_sim_arm_sleep[fourth_task] = TRUE;
                    }
                }

#endif
#endif

                if(SCI_IsTimerActive(s_sim_idle_timer[cur_sim_task]))
                {
                    SCI_DeactiveTimer(s_sim_idle_timer[cur_sim_task]);
                }

                SCI_ChangeTimer(s_sim_idle_timer[cur_sim_task], IdleCard, SIM_IDLE_TIME_BEFORE_SLEEP);
                SCI_ActiveTimer(s_sim_idle_timer[cur_sim_task]);
            }
        }
    }
    else
    {
        if ((SIM_ACTIVE_STATE == g_sim_card_state[cur_sim_task]) 
#ifndef _SUPPORT_IPC_
            && (SIM_CLOCK_S_FORBIDEN != g_sim_card_setting[cur_sim_task].gsm_data.clock_stop_type)
#endif
            )
        {
            if(SCI_IsTimerActive(s_sim_idle_timer[cur_sim_task]))
            {
                SCI_DeactiveTimer(s_sim_idle_timer[cur_sim_task]);
            }

            SCI_ChangeTimer(s_sim_idle_timer[cur_sim_task], IdleCard, SIM_IDLE_TIME_BEFORE_SLEEP);
            SCI_ActiveTimer(s_sim_idle_timer[cur_sim_task]);
        }
    }

#ifdef SIM_DROP_RESTORE_SUPPORT
    SIMINSTR_SetAuthInAuthFlag(cur_sim_task,FALSE);//
    SIMINSTR_SetGMMAuthInAuthFlag(cur_sim_task,FALSE);//
#endif
}

/********************************************************************
//    DESCRIPTION
//      Process the startup request from the MN layer
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void StartupSIM(uint32 cur_sim_task)
{
#ifndef SIM_TEST_IN_WIN32
    uint32 exchange_card_id = 0;
#endif

#ifdef SIM_TEST_IN_WIN32
    xSignalHeaderRec *pcnf = NULL;
    SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);
#ifdef SIM_SIMULATOR

    if(TB_SIM_INSERTED_NO_SIM != g_sim_insert_status)
    {
        //send self a card in int to go on the sim task in windows test
        SCI_CREATE_SIGNAL(pcnf, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), g_sim_task_id[cur_sim_task]);
        ((SIM_SIG_INT_IND_T *)pcnf)->int_type = TB_SIMINT_CARD_IN;
        SCI_SEND_SIGNAL(pcnf, g_sim_task_id[cur_sim_task]);
    }

#else

    if(g_is_sim_card_inserted[cur_sim_task])
    {
        SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_ATR_STATE, "SIM ATR State", g_sim_task_id[cur_sim_task]);
        SCI_CREATE_SIGNAL(pcnf, SIM_CARD_INSERTED_IND, sizeof(SIM_SIG_CARD_INSERTED_IND_T), g_sim_task_id[cur_sim_task]);
        ((SIM_SIG_CARD_INSERTED_IND_T *)pcnf)->insert_status = TB_SIM_INSERTED_OK;
        ((SIM_SIG_CARD_INSERTED_IND_T *)pcnf)->t_protocol = TB_SIM_T_0_PROTOCOL;

        SCI_SEND_SIGNAL(pcnf, g_sim_task_id[cur_sim_task]);
    }
    else
    {
        SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);
        SCI_CREATE_SIGNAL(pcnf, SIM_CARD_PRESENT_IND, sizeof(SIM_SIG_CARD_PRESENT_IND_T), g_sim_task_id[cur_sim_task]);
        ((SIM_SIG_CARD_PRESENT_IND_T *)pcnf)->insert_status = SIM_INSERTED_WRONG_SIM;
        SCI_SEND_SIGNAL(pcnf, g_sim_info_task_id[cur_sim_task]);
    }

#endif
#else
    exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);

    //MOCOR Maintrunk support R and L chip, donot need 1.8V detect any more.
    //If want to support H chip(or other chip with 1 SIM LDO), we re-open here. wuding.
    {
        SIMHAL_ResetDriver(FALSE, exchange_card_id);
        //<<CR26907
        SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_ATR_STATE, "SIM ATR State", g_sim_task_id[cur_sim_task]);
        SCI_SLEEP((s_sim_ldo_stable_time + 2));
        SIMHAL_ResetSIMCard(FALSE, exchange_card_id, FALSE);
    }
    g_sim_card_setting[cur_sim_task].card_present = TRUE;
#endif
}

/**********************************************************************
//    Description:
//      when still not power on the SIM card,the SIM layer can not supply
//    any service but the start up SIM request to power on the SIM.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void NoPowerStateProcessor(uint32 cur_sim_task)
{
    xSignalHeaderRec *psig = NULL;
#ifdef SIM_TEST_IN_WIN32
    xSignalHeaderRec *pcnf = NULL;
#endif

    s_sim_keep_state[cur_sim_task] = TRUE;

    //when in no power state,first enable the deepsleep
    //SCI_TRACE_LOW:"SIM%d::NoPowerState enable deepsleep."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_1331_112_2_17_22_59_35_4534, (uint8 *)"d", cur_sim_task);
    EnableDeepSleep(cur_sim_task);

    while(s_sim_keep_state[cur_sim_task])
    {
        psig = RecevieSignal(cur_sim_task);
        SIMMAIN_GetMutex(cur_sim_task);

        switch(psig->SignalCode)
        {
#ifndef SIM_CODE_COMPACT
            case SIM_GET_CARD_TYPE_REQ:
                SIMSIGNAL_GetCardType(cur_sim_task, (SIM_SIG_GET_CARD_TYPE_REQ_T *) psig);
                break;
            case SIM_GET_CDMA_CARD_IMSI_REQ:
                SIMSIGNAL_GetCDMACardIMSI(cur_sim_task, (SIM_SIG_GET_CDMA_CARD_IMSI_REQ_T *) psig);
                break;
#endif
#ifdef SIM_PLUG_IN_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK
        case SIM_QUERY_CARD_STATUS_REQ:
            QueryCardStatus(cur_sim_task);
            break;
#endif
            case SIM_INT_IND:
                IntProcess(cur_sim_task, (SIM_SIG_INT_IND_T *)psig);
                break;
#endif
            case SIM_START_UP_REQ:
                {
                    DisableDeepSleep(cur_sim_task);
                    StartupSIM(cur_sim_task);

                    if(s_sim_is_ext_dualsim)
                    {
                        //ext dual sim. we will change into ATR state directly.
                        //Not give up the semaphore. Yang Wuding.2007.6.20
                        SCI_FREE_SIGNAL(psig);
                        return;
                    }
                }
                break;
            case SIM_POWER_OFF_REQ:
                PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF);
                break;
#ifdef GSM_DPHONE_SUPPORT
            case SIMAT_TERMINAL_RESPONSE_REQ:

                if(s_sim_gsm_dphone_support[cur_sim_task])
                {
                    SIMSIGNAL_TerminalResponse(cur_sim_task, (SIMAT_SIG_TERMINAL_RESPONSE_REQ_T *)psig);
                }

                break;
#elif defined GSM_DPHONE_VER2_SUPPORT
            case SIMAT_TERMINAL_RESPONSE_REQ:
                SIMSIGNAL_TerminalResponse(cur_sim_task, (SIMAT_SIG_TERMINAL_RESPONSE_REQ_T *)psig);
                break;
#endif
            case SIMAT_TERMINAL_PROFILE_REQ:
                SIMSIGNAL_TerminalProfile(cur_sim_task, (SIMAT_SIG_TERMINAL_PROFILE_REQ_T *)psig);
                break;
#ifdef MODEM_PLATFORM
            case SIM_SIG_CRSM_REQ:
            {
                SIM_SIG_CRSM_CNF_T *pcnf = NULL;
                SCI_CREATE_SIGNAL(pcnf,SIM_SIG_CRSM_CNF,sizeof(SIM_SIG_CRSM_CNF_T),g_sim_task_id[cur_sim_task]);
                pcnf->file_id = ((SIM_SIG_CRSM_REQ_T*)psig)->file_id;
                pcnf->sw1 = 0x94;
                pcnf->sw2 = 0x4;
                pcnf->data_len = 0;
                pcnf->result = FALSE;
                SCI_TRACE_LOW("SIM%d: Get CRSM in No Power State",cur_sim_task,pcnf->file_id);
                SCI_SEND_SIGNAL((xSignalHeaderRec*)pcnf,g_sim_info_task_id[cur_sim_task]);
            }
            break;
#endif
            default:
                //SCI_TRACE_LOW:"SIM::No power state received abnormal signal:signal_code =0x%x,sender =0x%x\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_1417_112_2_17_22_59_35_4535, (uint8 *)"dd", psig->SignalCode, psig->Sender);
                break;
        }

        SCI_FREE_SIGNAL(psig);
        SIMMAIN_PutMutex(cur_sim_task);
    }
}

/********************************************************************
//    DESCRIPTION
//    Initialize the SIM layer when the SIM layer just been start up
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMAIN_InitSIMTask(uint32 cur_sim_task)
{
    uint32 exchange_card_id = 0;

    //the sim card state
    SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NO_POWER_STATE, "SIM No Power State", g_sim_task_id[cur_sim_task]);
    SIMINSTR_InitTempSignalQueues(cur_sim_task);//Initialize the Temp signal queue
    SIMMAIN_ResetSIMVariable(cur_sim_task);//reset the SIM variable
    SIM_Exchange_Init_Card_Slot_Label();
#ifndef SIM_TEST_IN_WIN32
    exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);
    SIMHAL_Init(exchange_card_id);
#ifdef MULTI_SIM_SYS_QUAD

    if(s_sim_slot_0123_to_1023_flag
        || s_sim_slot_0123_to_2103_flag
        || s_sim_slot_0123_to_1203_flag
        || s_sim_slot_0123_to_2013_flag
        || s_sim_slot_0123_to_1230_flag
        || s_sim_slot_0123_to_0132_flag
        || s_sim_slot_0123_to_0231_flag
        || s_sim_slot_0123_to_0312_flag
        || s_sim_slot_0123_to_0321_flag
        || s_sim_slot_0123_to_1320_flag
        || s_sim_slot_0123_to_2130_flag
        || s_sim_slot_0123_to_2310_flag
        || s_sim_slot_0123_to_3012_flag
        || s_sim_slot_0123_to_3021_flag
        || s_sim_slot_0123_to_3102_flag
        || s_sim_slot_0123_to_3201_flag)
    {
        SIMHAL_MultiSim_Set_ParallelDual6302(FALSE);
    }
    else
    {
        SIMHAL_MultiSim_Set_ParallelDual6302(TRUE);
    }

#endif
#endif
    SIMMAIN_InitMutex(cur_sim_task);

    //the initialize the sim card insert status
    g_sim_card_setting[cur_sim_task].card_insert_status = TB_SIM_INSERTED_OK;

    //create the sim present check period timer
    s_sim_check_period_timer[cur_sim_task] = SCI_CreateTimer("Present Check Frequency Timer", SendCardPresentCheckInd,
                                                             cur_sim_task, SIM_CARD_PRESENT_CHECK_FREQUENCY_IDLE, SCI_NO_ACTIVATE);
    s_sim_present_timer[cur_sim_task] = SCI_CreateTimer("Present Check Timer", CardNotPresent,
                                                        cur_sim_task, SIM_CARD_PRESENT_TIME_LIMIT, SCI_NO_ACTIVATE);
    s_sim_idle_timer[cur_sim_task] = SCI_CreateTimer("Idle Timer", IdleCard,
                                                     cur_sim_task, SIM_IDLE_TIME_BEFORE_SLEEP, SCI_NO_ACTIVATE);
    s_sim_pin1puk1_wait_timer[cur_sim_task] = SCI_CreateTimer("PIN1PUK1 Wait Timer", EnableDeepSleep,
                                                              cur_sim_task, SIM_PIN1PUK1_WAIT_TIME_LIMIT, SCI_NO_ACTIVATE);
    s_sim_restore_waiting_timer[cur_sim_task] = SCI_CreateTimer("SIM Restore Wait Timer", RestoreExpired,
                                                                cur_sim_task, SIM_RESTORE_TIME_LIMIT, SCI_NO_ACTIVATE);
#if defined SIM_DROP_RESTORE_SUPPORT && defined SIM_DROP_RESTORE_DELAY_PBSMS_READ
    s_sim_delay_smspb_timer[cur_sim_task] = SCI_CreateTimer("SIM Delay Timer",SIM_Delay_Read,
											cur_sim_task,45000,SCI_NO_ACTIVATE);//delay 45s
#endif

#ifdef SIM_PLUG_IN_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK
	s_sim_plug_in_check_timer[cur_sim_task] = SCI_CreateTimer("SIM Check Timer",SIMMAIN_PLUG_IN_By_Check,
											cur_sim_task,s_sim_plug_in_by_check_interval,SCI_NO_ACTIVATE);
#endif
#endif
}

/********************************************************************
//    DESCRIPTION
//    reset the SIM layer STATIC and global variable at the beignning of SIM
//    layer or ever reset and reject the SIM card.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMMAIN_ResetSIMVariable(uint32 cur_sim_task)
{
    SIMFS_SetDSC1800Flag(cur_sim_task, FALSE);

    s_b_sim_present_timer[cur_sim_task] = FALSE;
    //clear the Temp signal queue
    SIMINSTR_ClearTempSignalQueue(cur_sim_task);
    SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);
    //the card is not present or it still not detect the sim card is present
    g_sim_card_setting[cur_sim_task].card_present = FALSE;
    //at the sim layer beginning,the sim card is in the active
    //service,can not supply service for other layer

    s_sim_sim_poll_time[cur_sim_task] = 0;
    s_sim_call_poll_time[cur_sim_task] = 0;
    s_sim_stk_poll_time[cur_sim_task] = 0;

    s_sim_is_in_refresh[cur_sim_task] = FALSE;

#ifdef SIM_3G_USIM_SUPPORT
    s_sim_usim_adf_file_id_flag[cur_sim_task] = FALSE;
    s_sim_gain_chv_remain_times_flag[cur_sim_task] = FALSE;
#endif

    //set the default value to clock stop
    g_sim_card_setting[cur_sim_task].gsm_data.clock_stop_type   = SIM_CLOCK_S_FORBIDEN;
    //initialize the default SIM card setting
    g_sim_card_setting[cur_sim_task].adn_info.other_content_len = SIM_EF_DN_MIN_RECORD_LEN;
    g_sim_card_setting[cur_sim_task].adn_info.alpha_id_len = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].adn_info.max_alpha_id_len = SIM_MAX_DN_ALPHA_IDENTIFY_LEN;
    g_sim_card_setting[cur_sim_task].adn_info.record_num = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].adn_info.max_record_num = SIM_MAX_ADN_NUM;

    g_sim_card_setting[cur_sim_task].bdn_info.other_content_len = SIM_EF_DN_MIN_RECORD_LEN;
    g_sim_card_setting[cur_sim_task].bdn_info.alpha_id_len = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].bdn_info.max_alpha_id_len = SIM_MAX_DN_ALPHA_IDENTIFY_LEN;
    g_sim_card_setting[cur_sim_task].bdn_info.record_num = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].bdn_info.max_record_num = SIM_MAX_BDN_NUM;

    g_sim_card_setting[cur_sim_task].fdn_info.other_content_len = SIM_EF_DN_MIN_RECORD_LEN;
    g_sim_card_setting[cur_sim_task].fdn_info.alpha_id_len = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].fdn_info.max_alpha_id_len = SIM_MAX_DN_ALPHA_IDENTIFY_LEN;
    g_sim_card_setting[cur_sim_task].fdn_info.record_num = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].fdn_info.max_record_num = SIM_MAX_FDN_NUM;

    g_sim_card_setting[cur_sim_task].lnd_info.other_content_len = SIM_EF_DN_MIN_RECORD_LEN;
    g_sim_card_setting[cur_sim_task].lnd_info.alpha_id_len = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].lnd_info.max_alpha_id_len = SIM_MAX_DN_ALPHA_IDENTIFY_LEN;
    g_sim_card_setting[cur_sim_task].lnd_info.record_num = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].lnd_info.max_record_num = SIM_MAX_LND_NUM;

    g_sim_card_setting[cur_sim_task].msisdn_info.other_content_len = SIM_EF_DN_MIN_RECORD_LEN;
    g_sim_card_setting[cur_sim_task].msisdn_info.alpha_id_len = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].msisdn_info.max_alpha_id_len = SIM_MAX_DN_ALPHA_IDENTIFY_LEN;
    g_sim_card_setting[cur_sim_task].msisdn_info.record_num = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].msisdn_info.max_record_num = SIM_MAX_MSISDN_NUM;

    g_sim_card_setting[cur_sim_task].sdn_info.other_content_len = SIM_EF_DN_MIN_RECORD_LEN;
    g_sim_card_setting[cur_sim_task].sdn_info.alpha_id_len = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].sdn_info.max_alpha_id_len = SIM_MAX_DN_ALPHA_IDENTIFY_LEN;
    g_sim_card_setting[cur_sim_task].sdn_info.record_num = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].sdn_info.max_record_num = SIM_MAX_SDN_NUM;

    g_sim_card_setting[cur_sim_task].smsp_info.other_content_len = SIM_EF_SMSP_MIN_RECORD_LEN;
    g_sim_card_setting[cur_sim_task].smsp_info.alpha_id_len = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].smsp_info.max_alpha_id_len = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].smsp_info.record_num = UINT8_INVALIDE;
    g_sim_card_setting[cur_sim_task].smsp_info.max_record_num = SIM_MAX_SMSP_NUM;

    //g_sim_card_setting.sms_info.me_sms_num = MN_SMS_IN_SIM_MAX_NUM;
    //g_sim_card_setting.sms_info.sim_sms_num = UINT8_INVALIDE;
    //SCI_MEMSET(g_sim_card_setting.sms_info.states_got_ptr,(uint8)FALSE,MN_SMS_IN_SIM_MAX_NUM * sizeof(uint8));
    if(PNULL != g_sim_card_setting[cur_sim_task].sms_info.states_got_ptr)     // CR36741
    {
        SCI_FREE(g_sim_card_setting[cur_sim_task].sms_info.states_got_ptr);
        g_sim_card_setting[cur_sim_task].sms_info.states_got_ptr = PNULL;
    }

    if(PNULL != g_sim_card_setting[cur_sim_task].sms_info.states_ptr)         // CR36741
    {
        SCI_FREE(g_sim_card_setting[cur_sim_task].sms_info.states_ptr);
        g_sim_card_setting[cur_sim_task].sms_info.states_ptr = PNULL;
    }

    //insitilize the gsm data
    (g_sim_card_setting[cur_sim_task].gsm_data).chv1_status.chv_verified = FALSE;
    (g_sim_card_setting[cur_sim_task].gsm_data).chv2_status.chv_verified = FALSE;

    //initialize the cphs data
    SCI_MEMSET(&(g_sim_cphsinfo[cur_sim_task]), 0, sizeof(SIM_CARD_CPHSINFO_T));
    g_sim_cphsinfo[cur_sim_task].cphsoptionalinfo.cphs_phase = PHASE1;

//#ifdef SIM_PLUG_IN_SUPPORT
    //if sim hot swap support, another sim card has different feature.
    s_total_sim_reset_times[cur_sim_task] = 0;
    s_total_sim_change_reset_times[cur_sim_task] = 0;
    s_sim_restore_with_3v[cur_sim_task] = FALSE;
    s_sim_cardout_reset_times[cur_sim_task] = 0;
    s_sim_cdma_flag[cur_sim_task] = FALSE;
#ifdef SIM_CDMA_CARD_SUPPORT
    s_sim_cdma_only_flag[cur_sim_task] = FALSE;
#endif

#ifdef SIM_3G_USIM_SUPPORT
    s_sim_usim_is_checked[cur_sim_task] = 0;
    g_sim_netpar_file_size[cur_sim_task] = 0;
#endif
    g_sim_cphs_vm_record_len[cur_sim_task] = 0;
    g_sim_r4_vm_record_len[cur_sim_task] = 0;
    g_sim_r4_vm_mbi_record_len[cur_sim_task] = 0;
    g_sim_r4_vm_mwis_record_len[cur_sim_task] = 0;
    g_sim_r4_vm_mwis_record_num[cur_sim_task] = 0;
    g_sim_r4_vm_vmwflag_len[cur_sim_task] = 0;

#ifndef SIM_TEST_IN_WIN32
    SIMHAL_HotSwap_ClearStaticVariable(cur_sim_task);
#endif
    SIMINSTR_HotSwap_ClearStaticVariable(cur_sim_task);
    SIMDN_HotSwap_ClearStaticVariable(cur_sim_task);
    SIMSIGNAL_HotSwap_ClearStaticVariable(cur_sim_task);
    SIMMMSIG_HotSwap_ClearStaticVariable(cur_sim_task);
    SIMSMS_SetSMSLocationDFtel(cur_sim_task, TRUE); //clear
//#endif
}

/********************************************************************
//    DESCRIPTION
//      the function of NULL state.the NULL state is mean when power
//    on the SIM and there are still no indication that whether the
//    SIM card is present in the SIM slot,so just process the two signal
//    from the SIM drive:card out and card in.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void NullStateProcessor(uint32 cur_sim_task)
{
    xSignalHeaderRec *psig = NULL;

    s_sim_keep_state[cur_sim_task] = TRUE;
    //when in null state,first enable the deepsleep
    //SCI_TRACE_LOW:"SIM%d::NullState enable deepsleep."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_1624_112_2_17_22_59_36_4536, (uint8 *)"d", cur_sim_task);
    EnableDeepSleep(cur_sim_task);

    while(s_sim_keep_state[cur_sim_task])
    {
        psig = RecevieSignal(cur_sim_task);
        SIMMAIN_GetMutex(cur_sim_task);

        switch(psig->SignalCode)
        {
#ifdef SIM_PLUG_IN_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK
        case SIM_QUERY_CARD_STATUS_REQ:
            QueryCardStatus(cur_sim_task);
            break;
#endif
#endif
#ifndef SIM_CODE_COMPACT
            case SIM_GET_CARD_TYPE_REQ:
                SIMSIGNAL_GetCardType(cur_sim_task, (SIM_SIG_GET_CARD_TYPE_REQ_T *) psig);
                break;
            case SIM_GET_CDMA_CARD_IMSI_REQ:
                SIMSIGNAL_GetCDMACardIMSI(cur_sim_task, (SIM_SIG_GET_CDMA_CARD_IMSI_REQ_T *) psig);
                break;
#endif
            case SIM_INT_IND:
                IntProcess(cur_sim_task, (SIM_SIG_INT_IND_T *)psig);
                break;
            case SIM_START_UP_REQ:
                {
                    DisableDeepSleep(cur_sim_task);
                    PowerOffSIM(cur_sim_task, SIM_ABNORMAL_POWER_OFF);
                    StartupSIM(cur_sim_task);

                    if(s_sim_is_ext_dualsim)
                    {
                        //ext dual sim. we will change into ATR state directly.
                        //Not give up the semaphore. Yang Wuding.2007.6.20
                        SCI_FREE_SIGNAL(psig);
                        return;
                    }
                }
                break;
            case SIM_POWER_OFF_REQ:
                PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF);
                break;
#ifdef GSM_DPHONE_SUPPORT
            case SIMAT_TERMINAL_RESPONSE_REQ:

                if(s_sim_gsm_dphone_support[cur_sim_task])
                {
                    SIMSIGNAL_TerminalResponse(cur_sim_task, (SIMAT_SIG_TERMINAL_RESPONSE_REQ_T *)psig);
                }

                break;
#elif defined GSM_DPHONE_VER2_SUPPORT
            case SIMAT_TERMINAL_RESPONSE_REQ:
                SIMSIGNAL_TerminalResponse(cur_sim_task, (SIMAT_SIG_TERMINAL_RESPONSE_REQ_T *)psig);
                break;
#endif
            case SIMAT_TERMINAL_PROFILE_REQ:
                SIMSIGNAL_TerminalProfile(cur_sim_task, (SIMAT_SIG_TERMINAL_PROFILE_REQ_T *)psig);
                break;
            default:
                //SCI_TRACE_LOW:"SIM::Null state received abnormal signal:signal_code =0x%x,sender =0x%x\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_1693_112_2_17_22_59_36_4537, (uint8 *)"dd", psig->SignalCode, psig->Sender);
                //SCI_ASSERT(FALSE);
                break;
        }

        SCI_FREE_SIGNAL(psig);
        SIMMAIN_PutMutex(cur_sim_task);
    }
}

/********************************************************************
//    DESCRIPTION
//      A common part of CheckCardStatus, which will re-start sim.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC void CheckCardStatus_ReStartSIM(
    uint32 cur_sim_task,
    BOOLEAN is_sim_fail,
    BOOLEAN is_volt_rise)
{
    xSignalHeaderRec *sending_sig = NULL;
    BOOLEAN sim_fail_flag = is_sim_fail;
    uint32 exchange_card_id = 0;

    if(!sim_fail_flag) //SIM is not fail, continue.
    {
        exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);
        SIMHAL_PowerOffSIM(exchange_card_id);
        //If the Vcc can be power off, we should keep the deactivation at least 10 ms.
        //our Vcc will be a long slope, so we should wait more than 10ms
        SCI_Sleep(40);

        SIMHAL_ResetDriver(FALSE, exchange_card_id);
        SCI_Sleep((s_sim_ldo_stable_time + 2)); //wait the LDO stable

        if ((s_total_sim_change_reset_times[cur_sim_task] >= 3) 
            && SIMHAL_GetParityResetFlag(exchange_card_id))
        {
            SIMHAL_ChangeConvetion(exchange_card_id);
        }
        SIMHAL_SetParityResetFlag(exchange_card_id,FALSE);//clear

        if(!SIMHAL_ResetSIMCard(FALSE, exchange_card_id, is_volt_rise))
        {
            sim_fail_flag = TRUE;
        }
    }

    if(sim_fail_flag)
    {
#ifdef GSM_DPHONE_SUPPORT

        if(s_sim_gsm_dphone_support[cur_sim_task])
        {
            //after Dphone warm reset, if card_insert_status is not ok, we should notify MN Initialization is failed
            if(s_dphone_reset_times[cur_sim_task] != 0)
            {
                SIMSIGNAL_SIMInitNotOK(cur_sim_task);
            }
        }

#endif
        //get the GSM specific data,maybe insert the wrong sim card reject the sim card
        g_sim_card_setting[cur_sim_task].card_insert_status = TB_SIM_INSERTED_WRONG_SIM;
#ifndef SIM_TEST_IN_WIN32
        Dualsim_RejectSimCard(cur_sim_task);
#endif
        //current enter to the NULL state
        SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);
        //reset the SIM variable
        SIMMAIN_ResetSIMVariable(cur_sim_task);

        //if now in refresh reset, we should notify MN SIM_REFRESH_RESET_CNF
        SIMMAIN_SendRefreshResetCnf(cur_sim_task, FALSE);

#ifdef GSM_DPHONE_SUPPORT

        //after Dphone warm reset, if card_insert_status is not ok, we will power off sim
        if(s_sim_gsm_dphone_support[cur_sim_task]
            && s_dphone_reset_times[cur_sim_task] != 0)
        {
            PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF);
            EnableDeepSleep(cur_sim_task);
            g_sim_card_setting[cur_sim_task].card_present = FALSE;
        }
        else
#endif
        {
            SCI_CREATE_SIGNAL(sending_sig, SIM_CARD_PRESENT_IND, sizeof(SIM_SIG_CARD_PRESENT_IND_T), g_sim_task_id[cur_sim_task]);
#ifdef GSM_CUSTOMER_AFP_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT

            if(s_sim_after_plugin_wait_app_flag[cur_sim_task])
            {
                s_sim_after_plugin_wait_app_flag[cur_sim_task] = FALSE;
                ((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->insert_status = SIM_INSERTED_CARD_OUT;
            }
            else
#endif
#endif
            {
                ((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->insert_status = SIM_INSERTED_WRONG_SIM;
            }

            SCI_SEND_SIGNAL(sending_sig, g_sim_info_task_id[cur_sim_task]);
        }
    }

    return;
}

/********************************************************************
FUNCTION CheckCardStatus

DESCRIPTION
This Function is to get the SIM card Insert Status by the ATR data from
the SIM card,and then do the coressponding operation of different insert
status.

RETURN      void
***********************************************************************/
STATIC void CheckCardStatus(
    uint32 cur_sim_task,
    SIM_SIG_CARD_INSERTED_IND_T *psig //the atr data signal
)
{
    xSignalHeaderRec *sending_sig = NULL;
    SIM_SERVICE_RESULT_E ser_result;
    BOOLEAN sim_fail_flag = FALSE;
    uint8 atr_volt_info = 0;
    uint32 exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);
    SIM_RESPONSE_DATA_OF_SELECT_T temp_select_response;
    uint8 temp_volt = 0;
    BOOLEAN is_volt_in_atr = FALSE;
    BOOLEAN is_rise_volt_class = FALSE;
    SIMHAL_VOLT_CLASS_E volt_in_atr = SIMHAL_VOLT_CLASS_BC;
    SIMHAL_VOLT_CLASS_E lowest_volt_in_atr = SIMHAL_VOLT_CLASS_C_ONLY;
    SIMHAL_VOLT_CLASS_E volt_in_dfgsm = SIMHAL_VOLT_CLASS_BC;
    SIMHAL_VOLT_CLASS_E lowest_volt_in_dfgsm = SIMHAL_VOLT_CLASS_C_ONLY;
#ifdef SIM_PLUG_IN_SUPPORT
    BOOLEAN is_plug_in_flag[SIM_CARD_NUM] = {FALSE};
    xSignalHeaderRec *pintind = NULL;
#endif
#ifdef SIM_CDMA_CARD_SUPPORT
    SIM_RESPONSE_DATA_OF_SELECT_T cdma_select_data;
    SIM_SERVICE_RESULT_E cdma_ser_result;
#endif

#ifdef SIM_TEST_IN_WIN32
#ifdef SIM_SIMULATOR
    g_sim_card_setting[cur_sim_task].card_insert_status = g_sim_insert_status;
#else
    g_sim_card_setting[cur_sim_task].card_insert_status = TB_SIM_INSERTED_OK;
#endif
#else
    g_sim_card_setting[cur_sim_task].card_insert_status = psig->insert_status;
#endif

    //cr62704, to ensure the timing between ATR and our command is enough.
    SCI_Sleep(1);

    //the card insert ok
    if(TB_SIM_INSERTED_OK == g_sim_card_setting[cur_sim_task].card_insert_status)
    {
#ifdef SIM_PLUG_IN_SUPPORT

        if(s_sim_plug_in_flag[cur_sim_task])
        {
            SIMMAIN_IsSIMPlugIn(cur_sim_task, TRUE);
            return;
        }

#endif

        //save down the SIM card transmit protocol
        g_sim_card_setting[cur_sim_task].t_protocol = psig->t_protocol;
        //the default current selected dirctory file is MF after the atr
        SIMFS_SetCurDirectoryFile(cur_sim_task, SIM_FILE_MF);
        SIMFS_SetCurFileName(cur_sim_task, SIM_FILE_MF);

#ifndef SIM_TEST_IN_WIN32
        if (SIM_IsGetATRVoltInfo(cur_sim_task, &atr_volt_info))
        {
            is_volt_in_atr = TRUE;
            atr_volt_info &= (BIT_5 | BIT_4 | BIT_3 | BIT_2 | BIT_1 | BIT_0);
        }
        else
        {
            is_volt_in_atr = FALSE;
        }

        if(!(psig->is_3v)) //current is 1.8V
        {
            if (is_volt_in_atr)
            {
                SCI_TRACE_LOW("SIM%d:CheckCardStatus,atr_volt_info=%d.",cur_sim_task,atr_volt_info);
                switch(atr_volt_info)
                {
                    case 1:
                        volt_in_atr = SIMHAL_VOLT_CLASS_A_ONLY;
                        lowest_volt_in_atr = SIMHAL_VOLT_CLASS_A_ONLY;
                        break;
                    case 2:
                        volt_in_atr = SIMHAL_VOLT_CLASS_B_ONLY;
                        lowest_volt_in_atr = SIMHAL_VOLT_CLASS_B_ONLY;
                        break;
                    case 3:
                        volt_in_atr = SIMHAL_VOLT_CLASS_AB;
                        lowest_volt_in_atr = SIMHAL_VOLT_CLASS_B_ONLY;
                        break;
                    case 4:
                        volt_in_atr = SIMHAL_VOLT_CLASS_C_ONLY;
                        lowest_volt_in_atr = SIMHAL_VOLT_CLASS_C_ONLY;
                        break;
                    case 6:
                        volt_in_atr = SIMHAL_VOLT_CLASS_BC;
                        lowest_volt_in_atr = SIMHAL_VOLT_CLASS_C_ONLY;
                        break;
                    case 7:
                        volt_in_atr = SIMHAL_VOLT_CLASS_ABC;
                        lowest_volt_in_atr = SIMHAL_VOLT_CLASS_C_ONLY;
                        break;
                    default:
                        volt_in_atr = SIMHAL_VOLT_CLASS_RFU;
                        lowest_volt_in_atr = SIMHAL_VOLT_CLASS_C_ONLY;
                        break;
                }
            }
            else
            {
                SCI_TRACE_LOW("SIM%d:CheckCardStatus,no atr voltage info.",cur_sim_task);
                //No ATR voltage info, we wish to work in 3V
                volt_in_atr = SIMHAL_VOLT_CLASS_B_ONLY;
                lowest_volt_in_atr = SIMHAL_VOLT_CLASS_B_ONLY;
            }

#ifdef _SUPPORT_IPC_//Delos should poweroff
            if (SIMHAL_VOLT_CLASS_A_ONLY == volt_in_atr)
            {
                SCI_TRACE_LOW("SIM%d::DF 5V SIM. power off");
#ifndef SIM_TEST_IN_WIN32
                Dualsim_RejectSimCard(cur_sim_task);
#endif
                //current enter to the NULL state
                SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);
                g_sim_card_setting[cur_sim_task].card_insert_status  = TB_SIM_INSERTED_WRONG_SIM;
                SCI_CREATE_SIGNAL(sending_sig, SIM_CARD_PRESENT_IND, sizeof(SIM_SIG_CARD_PRESENT_IND_T), g_sim_task_id[cur_sim_task]);
                ((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->insert_status = SIM_INSERTED_CARD_OUT;
                SCI_SEND_SIGNAL(sending_sig, g_sim_info_task_id[cur_sim_task]);

                //reset the SIM variable
                SIMMAIN_ResetSIMVariable(cur_sim_task);
                return;
            }
#endif

#ifdef SIM_3G_USIM_SUPPORT

            //UICC case 5.1.5.4. ME should rise to 3V asap if ATR indicate 3V SIM.
            if(is_volt_in_atr
                && (SIMHAL_VOLT_CLASS_C_ONLY != lowest_volt_in_atr))
            {
                if(s_total_sim_change_reset_times[cur_sim_task] < 3)
                {
                    s_total_sim_change_reset_times[cur_sim_task]++;
                    sim_fail_flag = FALSE;
                }
                else
                {
                    s_total_sim_change_reset_times[cur_sim_task] = 0;
                    sim_fail_flag = TRUE;
#ifdef SIM_PLUG_IN_SUPPORT

                    if(s_sim_plug_in_flag[cur_sim_task])
                    {
                        SIMMAIN_IsSIMPlugIn(cur_sim_task, FALSE);
                        return;
                    }

#endif
                }

#ifndef SIM_TEST_IN_WIN32
                SIMHAL_SetVoltDetectComplete(exchange_card_id, TRUE);
                SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);
#ifdef SIM_3G_USIM_SUPPORT
                s_sim_usim_adf_file_id_flag[cur_sim_task] = FALSE;
                s_sim_gain_chv_remain_times_flag[cur_sim_task] = FALSE;
#endif
                CheckCardStatus_ReStartSIM(cur_sim_task, FALSE, TRUE);
#endif
                return;
            }

#endif
        }

#endif

        s_b_sim_present_timer[cur_sim_task] = TRUE;

#ifdef SIM_3G_USIM_SUPPORT

        if(!s_sim_case_test_flag)
        {
            //Support USIM card. And dnnot need PASS the GSM 2 cases.
#ifndef SIM_TEST_IN_WIN32
            SCI_MEMSET(&temp_select_response,0,sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
            ser_result = SIM_IsSIMorUSIM(cur_sim_task);
            if ((SIM_SERVICE_RX_PARITY_ERROR == ser_result) 
                || (SIM_SERVICE_TX_PARITY_ERROR == ser_result) 
                || (SIM_SERVICE_CARD_OUT == ser_result) 
                || (SIM_SERVICE_CARD_UNRESPONSE == ser_result))
            {
                SCI_TRACE_LOW("SIM%d: fatal error is USIM detect %d",cur_sim_task,ser_result);
                //we should keep in ATR state, but we'd clear the int queue.
                SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);
                s_sim_usim_adf_file_id_flag[cur_sim_task] = FALSE;
                s_sim_gain_chv_remain_times_flag[cur_sim_task] = FALSE;

                s_sim_reset_total_times[cur_sim_task]++;
                if (s_sim_reset_total_times[cur_sim_task] < 10)
                {
#ifndef SIM_TEST_IN_WIN32
                    CheckCardStatus_ReStartSIM(cur_sim_task, FALSE, FALSE);
#endif
                }
                else
                {
                    //Damaged sim card
                    SCI_TRACE_LOW("SIM%d::Damaged sim card. power off");
#ifndef SIM_TEST_IN_WIN32
                    Dualsim_RejectSimCard(cur_sim_task);
#endif
                    //current enter to the NULL state
                    SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);
                    g_sim_card_setting[cur_sim_task].card_insert_status  = TB_SIM_INSERTED_WRONG_SIM;
                    SCI_CREATE_SIGNAL(sending_sig, SIM_CARD_PRESENT_IND, sizeof(SIM_SIG_CARD_PRESENT_IND_T), g_sim_task_id[cur_sim_task]);
                    ((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->insert_status = SIM_INSERTED_CARD_OUT;
                    SCI_SEND_SIGNAL(sending_sig, g_sim_info_task_id[cur_sim_task]);
                    //reset the SIM variable
                    SIMMAIN_ResetSIMVariable(cur_sim_task);
                }
                return;
            }

            if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
            {
                ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_ADF_USIM, &temp_select_response);

#ifdef GSM_CUSTOMER_AFP_SUPPORT
                //Special case
                SCI_TRACE_LOW("SIM special case %d",ser_result);
                if (SIM_SERVICE_REF_DATA_NOT_FOUND == ser_result)
                {
                    s_sim_is_usim[cur_sim_task] = SIM_2G_APPLICATION;
                    
                    {
                    SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);
                    s_sim_usim_adf_file_id_flag[cur_sim_task] = FALSE;
                    s_sim_gain_chv_remain_times_flag[cur_sim_task] = FALSE;

                    s_sim_reset_total_times[cur_sim_task]++;
                    if (s_sim_reset_total_times[cur_sim_task] < 10)
                    {
#ifndef SIM_TEST_IN_WIN32
                        CheckCardStatus_ReStartSIM(cur_sim_task, FALSE, FALSE);
#endif
                    }
                    else
                    {
                        //Damaged sim card
                        SCI_TRACE_LOW("SIM%d::Damaged sim card. power off");
#ifndef SIM_TEST_IN_WIN32
                        Dualsim_RejectSimCard(cur_sim_task);
#endif
                        //current enter to the NULL state
                        SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);
                        g_sim_card_setting[cur_sim_task].card_insert_status  = TB_SIM_INSERTED_WRONG_SIM;
                        SCI_CREATE_SIGNAL(sending_sig, SIM_CARD_PRESENT_IND, sizeof(SIM_SIG_CARD_PRESENT_IND_T), g_sim_task_id[cur_sim_task]);
                        ((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->insert_status = SIM_INSERTED_CARD_OUT;
                        SCI_SEND_SIGNAL(sending_sig, g_sim_info_task_id[cur_sim_task]);
                        //reset the SIM variable
                        SIMMAIN_ResetSIMVariable(cur_sim_task);
                    }
                    return;
                }

                }
                //Special case
#endif
            }
            else
#endif//SIM_TEST_IN_WIN32
            {
                ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_GSM, &temp_select_response);
            }
#ifdef SIM_CDMA_CARD_SUPPORT
            if (SIM_SERVICE_OK != ser_result) //when SELECT DFgsm FAIL, we will check wether CDMA only card.
            {
                cdma_ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_CDMA, &cdma_select_data);
                if(SIM_SERVICE_OK == cdma_ser_result)
                {
                    ser_result = SIM_SERVICE_OK;
                    s_sim_cdma_flag[cur_sim_task] = TRUE;
                    s_sim_cdma_only_flag[cur_sim_task] = TRUE;
                    SCI_MEMCPY(&temp_select_response, &cdma_select_data, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
                }
            }
#endif
        }
        else
#endif//SIM_3G_USIM_SUPPORT
        {
            // 1, Support USIM card. But need PASS the GSM 2 cases. It will lead some special Usim card cannot auth OK(it work like sim for 1st ins is 0xA0).
            // 2, Donnot support USIM card. SELCET DFgsm firstly.
            ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_GSM, &temp_select_response);
#ifdef _SUPPORT_IPC_
#ifdef SIM_CDMA_CARD_SUPPORT
            if (SIM_SERVICE_OK != ser_result) //when SELECT DFgsm FAIL, we will check wether CDMA only card.
            {
                cdma_ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_CDMA, &cdma_select_data);
                if(SIM_SERVICE_OK == cdma_ser_result)
                {
                    ser_result = SIM_SERVICE_OK;
                    s_sim_cdma_flag[cur_sim_task] = TRUE;
                    s_sim_cdma_only_flag[cur_sim_task] = TRUE;
                    SCI_MEMCPY(&temp_select_response, &cdma_select_data, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T));
                }
            }
#endif
#endif
        }

        if(SIM_SERVICE_OK == ser_result)
        {
            temp_volt = (uint8)((temp_select_response.filecharacter_efstructure & SIM_GSM_SPECIFIC_CARD_VOLT) >> 4);
            SCI_TRACE_LOW("SIM%d:CheckCardStatus,dfgsm indicator temp_volt=%d.",cur_sim_task,temp_volt);

            switch(temp_volt)
            {
                case 0://5V ONLY
                    volt_in_dfgsm = SIMHAL_VOLT_CLASS_A_ONLY;
                    lowest_volt_in_dfgsm = SIMHAL_VOLT_CLASS_A_ONLY;
                    break;
                case 1://3V TECH
                    volt_in_dfgsm = SIMHAL_VOLT_CLASS_AB;
                    lowest_volt_in_dfgsm = SIMHAL_VOLT_CLASS_B_ONLY;
                    break;
                case 3://1.8V TECH
                    volt_in_dfgsm = SIMHAL_VOLT_CLASS_BC;
                    lowest_volt_in_dfgsm = SIMHAL_VOLT_CLASS_C_ONLY;
                    break;
                case 7://FUTRUE
                default:
                    volt_in_dfgsm = SIMHAL_VOLT_CLASS_C_ONLY;
                    lowest_volt_in_dfgsm = SIMHAL_VOLT_CLASS_C_ONLY;
                    break;
            }

#ifdef _SUPPORT_IPC_//Delos should poweroff
            if (SIMHAL_VOLT_CLASS_A_ONLY == volt_in_dfgsm)//SSTC
            {
                BOOLEAN power_off = FALSE;

                if (is_volt_in_atr)
                {
                    if ((0x2 == atr_volt_info) 
                        || (0x3 == atr_volt_info)
                        || (0x4 == atr_volt_info)
                        || (0x6 == atr_volt_info))//ATR_BC File_A; ATR_AB File_A; ATR_B File_A; ATR_C File_A;
                    {
                        power_off = FALSE;
                    }
                    else
                    {
                        power_off = TRUE;
                    }
                }
                else
                {
                    power_off = TRUE;//ATR_nothing, File_A
                }

                if (power_off)
                {
                    SCI_TRACE_LOW("SIM%d::DF 5V SIM. power off");
#ifndef SIM_TEST_IN_WIN32
                    Dualsim_RejectSimCard(cur_sim_task);
#endif
                    //current enter to the NULL state
                    SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);
                    g_sim_card_setting[cur_sim_task].card_insert_status  = TB_SIM_INSERTED_WRONG_SIM;
                    SCI_CREATE_SIGNAL(sending_sig, SIM_CARD_PRESENT_IND, sizeof(SIM_SIG_CARD_PRESENT_IND_T), g_sim_task_id[cur_sim_task]);
                    ((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->insert_status = SIM_INSERTED_CARD_OUT;
                    SCI_SEND_SIGNAL(sending_sig, g_sim_info_task_id[cur_sim_task]);

                    //reset the SIM variable
                    SIMMAIN_ResetSIMVariable(cur_sim_task);
                    return;
                }
            }
#endif

        }
        else
        {
#ifdef SIM_PLUG_IN_SUPPORT
#ifdef GSM_CUSTOMER_AFP_SUPPORT

            if(SIM_SERVICE_CARD_OUT == ser_result)
            {
                g_sim_card_setting[cur_sim_task].card_insert_status = TB_SIM_INSERTED_WRONG_SIM;
#ifndef SIM_TEST_IN_WIN32
                Dualsim_RejectSimCard(cur_sim_task);
#endif
                //current enter to the NULL state
                SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);
                //reset the SIM variable
                SIMMAIN_ResetSIMVariable(cur_sim_task);

                SCI_CREATE_SIGNAL(sending_sig, SIM_CARD_PRESENT_IND, sizeof(SIM_SIG_CARD_PRESENT_IND_T), g_sim_task_id[cur_sim_task]);
                s_sim_after_plugin_wait_app_flag[cur_sim_task] = FALSE;
                ((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->insert_status = SIM_INSERTED_CARD_OUT;
                SCI_SEND_SIGNAL(sending_sig, g_sim_info_task_id[cur_sim_task]);
                return;
            }

#endif

            if(TB_SIMINT_PLUG_IN == SIMINSTR_LatestHotSwapMessageInTempIntSignalQueue(cur_sim_task))
            {
                SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);
                SCI_CREATE_SIGNAL(pintind, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), g_sim_task_id[cur_sim_task]);
                ((SIM_SIG_INT_IND_T *)pintind)->int_type = TB_SIMINT_PLUG_IN;
                SIMINSTR_AddThisSIGNALInTempIntSignalQueue(cur_sim_task, pintind);
                return;
            }

#endif

#ifdef SIM_3G_USIM_SUPPORT
            if(((1 == s_sim_usim_is_checked[cur_sim_task]) 
                && (SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task])) 
                || (((SIM_SERVICE_RX_PARITY_ERROR == ser_result) 
                || (SIM_SERVICE_TX_PARITY_ERROR == ser_result) 
                || (SIM_SERVICE_CARD_OUT == ser_result) 
                || (SIM_SERVICE_CARD_UNRESPONSE == ser_result))))
            {
                //we should keep in ATR state, but we'd clear the int queue.
                SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);
                s_sim_usim_adf_file_id_flag[cur_sim_task] = FALSE;
                s_sim_gain_chv_remain_times_flag[cur_sim_task] = FALSE;

                s_sim_reset_total_times[cur_sim_task]++;
                if (s_sim_reset_total_times[cur_sim_task] < 10)
                {
#ifndef SIM_TEST_IN_WIN32
                    CheckCardStatus_ReStartSIM(cur_sim_task, FALSE, FALSE);
#endif
                }
                else
                {
                    //Damaged sim card
                    SCI_TRACE_LOW("SIM%d::Damaged sim card. power off");
#ifndef SIM_TEST_IN_WIN32
                    Dualsim_RejectSimCard(cur_sim_task);
#endif
                    //current enter to the NULL state
                    SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);
                    g_sim_card_setting[cur_sim_task].card_insert_status  = TB_SIM_INSERTED_WRONG_SIM;
                    SCI_CREATE_SIGNAL(sending_sig, SIM_CARD_PRESENT_IND, sizeof(SIM_SIG_CARD_PRESENT_IND_T), g_sim_task_id[cur_sim_task]);
                    ((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->insert_status = SIM_INSERTED_CARD_OUT;
                    SCI_SEND_SIGNAL(sending_sig, g_sim_info_task_id[cur_sim_task]);
                    //reset the SIM variable
                    SIMMAIN_ResetSIMVariable(cur_sim_task);
                }
                return;
            }

#endif
        }
        s_sim_reset_total_times[cur_sim_task] = 0;//If we PASS to here, that's RESET OK. so clear it.

        if(!psig->is_3v)
        {
            if(is_volt_in_atr) //ATR has the voltage info
            {
                if(volt_in_atr == volt_in_dfgsm)
                {
                    if(SIMHAL_VOLT_CLASS_C_ONLY == lowest_volt_in_dfgsm)
                    {
                        is_rise_volt_class = FALSE;
#ifndef SIM_TEST_IN_WIN32
                        SIMHAL_SetVoltLimit(exchange_card_id, TRUE);
#endif
                        SCI_TRACE_LOW("SIM%d:1.8V only in ATR and DFgsm");
                    }
                    else
                    {
                        is_rise_volt_class = TRUE;
                    }
                }
                else
                {
                    SIMHAL_VOLT_CLASS_E temp_lowest_volt;

                    //Get the lowest voltage
                    if(lowest_volt_in_dfgsm > lowest_volt_in_atr) //lowest is bigger in enum
                    {
                        temp_lowest_volt = lowest_volt_in_dfgsm;
                    }
                    else
                    {
                        temp_lowest_volt = lowest_volt_in_atr;
                    }

                    if(SIMHAL_VOLT_CLASS_C_ONLY == temp_lowest_volt)
                    {
                        is_rise_volt_class = FALSE;
#ifndef SIM_TEST_IN_WIN32
                        SIMHAL_SetVoltLimit(exchange_card_id, TRUE);
#endif
                        SCI_TRACE_LOW("SIM%d:1.8V only in one of the ATR and DFgsm lowest");
                    }
                    else
                    {
                        is_rise_volt_class = TRUE;
                    }
                }
            }
            else
            {
                if(SIMHAL_VOLT_CLASS_C_ONLY == lowest_volt_in_dfgsm)
                {
                    is_rise_volt_class = FALSE;
#ifndef SIM_TEST_IN_WIN32
                    SIMHAL_SetVoltLimit(exchange_card_id, TRUE);
#endif
                    SCI_TRACE_LOW("SIM%d:1.8V only in DFgsm");
                }
                else
                {
                    is_rise_volt_class = TRUE;
                }
            }

            if(is_rise_volt_class)
            {
                if(s_total_sim_change_reset_times[cur_sim_task] < 3)
                {
                    s_total_sim_change_reset_times[cur_sim_task]++;
                    sim_fail_flag = FALSE;
                }
                else
                {
                    s_total_sim_change_reset_times[cur_sim_task] = 0;
                    sim_fail_flag = TRUE;
#ifdef SIM_PLUG_IN_SUPPORT

                    if(s_sim_plug_in_flag[cur_sim_task])
                    {
                        SIMMAIN_IsSIMPlugIn(cur_sim_task, FALSE);
                        return;
                    }

#endif
                }

#ifndef SIM_TEST_IN_WIN32
                SIMHAL_SetVoltDetectComplete(exchange_card_id, TRUE);
                SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);
#ifdef SIM_3G_USIM_SUPPORT
                s_sim_usim_adf_file_id_flag[cur_sim_task] = FALSE;
                s_sim_gain_chv_remain_times_flag[cur_sim_task] = FALSE;
#endif
                CheckCardStatus_ReStartSIM(cur_sim_task, FALSE, TRUE);
#endif
                return;
            }
            else
            {
#ifndef SIM_TEST_IN_WIN32
                SIMHAL_SetVoltLimit(exchange_card_id, TRUE);
                SCI_TRACE_LOW("SIM%d:SET as 1.8V only in SS", cur_sim_task);
#endif
            }
        }

#ifndef SIM_TEST_IN_WIN32
        SIMHAL_SetVoltDetectComplete(exchange_card_id, TRUE);
#endif

#ifdef SIM_3G_USIM_SUPPORT
#ifndef SIM_TEST_IN_WIN32

        if(s_sim_case_test_flag)
        {
            //check for 2/3G here.
            //Support USIM card, but need PASS the GSM 2 cases.
            //It will lead some special Usim card cannot auth OK(it work like sim becasue of 1st ins is 0xA0).
            SIM_IsSIMorUSIM(cur_sim_task);
        }

#endif
#endif//SIM_3G_USIM_SUPPORT

        SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);//we'd clear the int queue.
        SIMFS_SetCurDirectoryFile(cur_sim_task, SIM_FILE_MF);
        SIMFS_SetCurFileName(cur_sim_task, SIM_FILE_MF);

        //get the GSM information
        ser_result = SIMINSTR_GetGSMSpecificData(cur_sim_task, &(g_sim_card_setting[cur_sim_task].gsm_data));

        //get the GSM Sepcific data
        if(SIM_SERVICE_OK != ser_result)
        {
#ifdef SIM_PLUG_IN_SUPPORT

            if(TB_SIMINT_PLUG_IN == SIMINSTR_LatestHotSwapMessageInTempIntSignalQueue(cur_sim_task))
            {
                SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);
                SCI_CREATE_SIGNAL(pintind, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), g_sim_task_id[cur_sim_task]);
                ((SIM_SIG_INT_IND_T *)pintind)->int_type = TB_SIMINT_PLUG_IN;
                SIMINSTR_AddThisSIGNALInTempIntSignalQueue(cur_sim_task, pintind);
                return;
            }

#endif

#ifdef SIM_3G_USIM_SUPPORT

            if((1 == s_sim_usim_is_checked[cur_sim_task])
                && (SIM_2G_APPLICATION == s_sim_is_usim[cur_sim_task]))
            {
                //we should keep in ATR state, but we'd clear the int queue.
                SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);
                s_sim_usim_adf_file_id_flag[cur_sim_task] = FALSE;
                s_sim_gain_chv_remain_times_flag[cur_sim_task] = FALSE;
#ifndef SIM_TEST_IN_WIN32
                CheckCardStatus_ReStartSIM(cur_sim_task, FALSE, FALSE);
#endif
                return;
            }

#endif//SIM_TEST_IN_WIN32

            //get the GSM specific data,maybe insert the wrong sim card reject the sim card
            g_sim_card_setting[cur_sim_task].card_insert_status = TB_SIM_INSERTED_WRONG_SIM;

#ifndef SIM_TEST_IN_WIN32
            Dualsim_RejectSimCard(cur_sim_task);
#endif

            //if now in refresh reset, we should notify MN SIM_REFRESH_RESET_CNF
            SIMMAIN_SendRefreshResetCnf(cur_sim_task, FALSE);

            //current enter to the NULL state
            SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);

#ifdef GSM_DPHONE_SUPPORT

            if(s_sim_gsm_dphone_support[cur_sim_task]
                && s_dphone_reset_times[cur_sim_task] != 0)
            {
                SIMSIGNAL_SIMInitNotOK(cur_sim_task);
                g_sim_card_setting[cur_sim_task].card_present = FALSE;
                SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);

                //reset the SIM variable
                SIMMAIN_ResetSIMVariable(cur_sim_task);

                PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF);
                EnableDeepSleep(cur_sim_task);
            }
            else
#endif
            {
                //tell other layer wrong sim card inserted
                SCI_CREATE_SIGNAL(sending_sig, SIM_CARD_PRESENT_IND, sizeof(SIM_SIG_CARD_PRESENT_IND_T), g_sim_task_id[cur_sim_task]);
#ifdef GSM_CUSTOMER_AFP_SUPPORT

                if((SIM_SERVICE_RX_PARITY_ERROR == ser_result)
                    || (SIM_SERVICE_TX_PARITY_ERROR == ser_result)
                    || (SIM_SERVICE_CARD_OUT == ser_result)
                    || (SIM_SERVICE_CARD_UNRESPONSE == ser_result))
                {
                    ((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->insert_status = SIM_INSERTED_CARD_OUT;
                }
                else
#endif
                {
                    ((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->insert_status = SIM_INSERTED_WRONG_SIM;
                }

#ifdef GSM_CUSTOMER_AFP_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT
                s_sim_after_plugin_wait_app_flag[cur_sim_task] = FALSE;
#endif
#endif
                SCI_SEND_SIGNAL(sending_sig, g_sim_info_task_id[cur_sim_task]);
            }
        }
        else
        {
#ifndef SIM_TEST_IN_WIN32
            SIMHAL_SetVoltDetectComplete(exchange_card_id, TRUE);
            //SCI_TRACE_LOW:"SIM%d::card supply volt is:%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_1944_112_2_17_22_59_36_4540, (uint8 *)"dd", cur_sim_task, g_sim_card_setting[cur_sim_task].gsm_data.card_supply_volt);

            //after the GSM information gotten,enable the SIM watch dog to make sure the command to
            //the SIM card be excuted or there will be card unreponse interrupter instead.
            if((SIM_CLOCK_S_ALLOWED_2 == g_sim_card_setting[cur_sim_task].gsm_data.clock_stop_type)
                || (SIM_CLOCK_S_ALLOWED_4 == g_sim_card_setting[cur_sim_task].gsm_data.clock_stop_type))
            {
                SIMHAL_SetClockStopLevel(exchange_card_id, TRUE);
            }
            else
            {
                SIMHAL_SetClockStopLevel(exchange_card_id, FALSE);
            }

#endif

#ifdef GSM_DPHONE_SUPPORT

            if(s_sim_gsm_dphone_support[cur_sim_task]
                && s_dphone_reset_times[cur_sim_task] != 0)
            {
                SIMSIGNAL_SIMInitReq(cur_sim_task);
                s_b_sim_present_timer[cur_sim_task] = TRUE;
#ifndef WIN32
                SIMHAL_PrintATRDataBuf(exchange_card_id);
#endif
                //current enter to the INITIALIZE state
                SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_INITIALIZE_STATE, "SIM Initialize State", g_sim_task_id[cur_sim_task]);
            }
            else
#endif
            {
                //SCI_TRACE_LOW:"SIM%d::the current working voltage is matched with the card supply voltage."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_2051_112_2_17_22_59_37_4543, (uint8 *)"d", cur_sim_task);
                //tell other layer sim card inserted
                SCI_CREATE_SIGNAL(sending_sig, SIM_CARD_PRESENT_IND, sizeof(SIM_SIG_CARD_PRESENT_IND_T), g_sim_task_id[cur_sim_task]);

                ((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->insert_status = SIM_INSERTED_OK;

                //prepare sim card presented ind failed,reject the card
                if(!SIMSIGNAL_PrepareCardPresentInd(cur_sim_task, (SIM_SIG_CARD_PRESENT_IND_T *)sending_sig))
                {
                    //SCI_TRACE_LOW:"\n\rSIM%d::SIM prepare sim card present failed."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_2060_112_2_17_22_59_37_4544, (uint8 *)"d", cur_sim_task);
#ifndef SIM_TEST_IN_WIN32
                    Dualsim_RejectSimCard(cur_sim_task);
#endif

                    //if now in refresh reset, we should notify MN SIM_REFRESH_RESET_CNF
                    SIMMAIN_SendRefreshResetCnf(cur_sim_task, FALSE);

                    //current enter to the NULL state
                    SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);
                    g_sim_card_setting[cur_sim_task].card_insert_status  = TB_SIM_INSERTED_WRONG_SIM;
#ifdef GSM_CUSTOMER_AFP_SUPPORT
                    ((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->insert_status = SIM_INSERTED_CARD_OUT;
#ifdef SIM_PLUG_IN_SUPPORT
                    s_sim_after_plugin_wait_app_flag[cur_sim_task] = FALSE;
#endif
#else
                    ((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->insert_status = SIM_INSERTED_WRONG_SIM;
#endif
                    SCI_SEND_SIGNAL(sending_sig, g_sim_info_task_id[cur_sim_task]);

                    //To check the Int queue, to find out the "PLUG IN"
#ifdef SIM_PLUG_IN_SUPPORT

                    if(TB_SIMINT_PLUG_IN == SIMINSTR_LatestHotSwapMessageInTempIntSignalQueue(cur_sim_task))
                    {
                        is_plug_in_flag[cur_sim_task] = TRUE;
                    }

#endif

                    //reset the SIM variable
                    SIMMAIN_ResetSIMVariable(cur_sim_task);

#ifdef SIM_PLUG_IN_SUPPORT

                    if(is_plug_in_flag[cur_sim_task])
                    {
                        SCI_CREATE_SIGNAL(pintind, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), g_sim_task_id[cur_sim_task]);
                        ((SIM_SIG_INT_IND_T *)pintind)->int_type = TB_SIMINT_PLUG_IN;
                        SIMINSTR_AddThisSIGNALInTempIntSignalQueue(cur_sim_task, pintind);
                    }

#endif

                }
                else
                {
                    //SCI_TRACE_LOW:"\n\rSIM%d::SIM Inserted OK!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_2074_112_2_17_22_59_37_4545, (uint8 *)"d", cur_sim_task);
#ifdef GSM_CUSTOMER_AFP_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT
                    s_sim_after_plugin_wait_app_flag[cur_sim_task] = FALSE;
#endif
#endif
                    SCI_SEND_SIGNAL(sending_sig, g_sim_info_task_id[cur_sim_task]);
#ifndef WIN32
                    SIMHAL_PrintATRDataBuf(exchange_card_id);
#endif
                    //current enter to the INITIALIZE state
                    SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_INITIALIZE_STATE, "SIM Initialize State", g_sim_task_id[cur_sim_task]);

                    if(psig->is_3v)
                    {
                        s_sim_restore_with_3v[cur_sim_task]  = TRUE;
                    }
                    else
                    {
                        s_sim_restore_with_3v[cur_sim_task]  = FALSE;
                    }

                    //Start the PIN1/PUK1 wait timer if needed
                    if((((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->present_info.chv1_enable)
                        || (((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->present_info.unblock_chv1_status.chv_blocked))
                    {
                        if(SCI_IsTimerActive(s_sim_pin1puk1_wait_timer[cur_sim_task]))
                        {
                            SCI_DeactiveTimer(s_sim_pin1puk1_wait_timer[cur_sim_task]);
                        }

                        SCI_ChangeTimer(s_sim_pin1puk1_wait_timer[cur_sim_task], EnableDeepSleep, SIM_PIN1PUK1_WAIT_TIME_LIMIT);
                        SCI_ActiveTimer(s_sim_pin1puk1_wait_timer[cur_sim_task]);
                    }

                    s_sim_cardout_reset_times[cur_sim_task] = 0;
                }
            }
        }
    }

#ifndef SIM_TEST_IN_WIN32
    else if(TB_SIM_INSERTED_CLASS_CHANGE == g_sim_card_setting[cur_sim_task].card_insert_status)
    {
        //SCI_TRACE_LOW:"SIM%d: class change %d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_2112_112_2_17_22_59_37_4546, (uint8 *)"dd", cur_sim_task, s_total_sim_change_reset_times[cur_sim_task]);
        SIMHAL_SetVoltDetectComplete(exchange_card_id, TRUE);

        if(s_total_sim_change_reset_times[cur_sim_task] < 3)
        {
            s_total_sim_change_reset_times[cur_sim_task]++;
            sim_fail_flag = FALSE;
        }
        else
        {
            //Huawei try again
            if (SIMHAL_GetParityResetFlag(exchange_card_id))
            {
                s_total_sim_change_reset_times[cur_sim_task]++;
                sim_fail_flag = FALSE;          
            }
            else
            //Huawei try again
            {

            s_total_sim_change_reset_times[cur_sim_task] = 0;
            sim_fail_flag = TRUE;

#ifdef SIM_PLUG_IN_SUPPORT

            if(s_sim_plug_in_flag[cur_sim_task])
            {
                SIMMAIN_IsSIMPlugIn(cur_sim_task, FALSE);
                return;
            }

#endif

            }
        }

#ifdef SIM_3G_USIM_SUPPORT
        s_sim_usim_adf_file_id_flag[cur_sim_task] = FALSE;
        s_sim_gain_chv_remain_times_flag[cur_sim_task] = FALSE;
#endif
        CheckCardStatus_ReStartSIM(cur_sim_task, sim_fail_flag, TRUE);
    }

#endif//SIM_TEST_IN_WIN32
    //insert the worng SIM card,shoud reject the card
    //TB_SIM_INSERTED_WRONG_SIM
    //TB_SIM_INSERTED_ERROR
    else
    {
#ifdef SIM_3G_USIM_SUPPORT
        s_sim_usim_adf_file_id_flag[cur_sim_task] = FALSE;
        s_sim_gain_chv_remain_times_flag[cur_sim_task] = FALSE;
#endif

#ifdef SIM_PLUG_IN_SUPPORT

        if(s_sim_plug_in_flag[cur_sim_task])
        {
            SIMMAIN_IsSIMPlugIn(cur_sim_task, FALSE);
            return;
        }

#endif

        //SCI_TRACE_LOW:"\n\rSIM%d::Wrong SIM inserted!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_2171_112_2_17_22_59_37_4547, (uint8 *)"d", cur_sim_task);
#ifdef GSM_DPHONE_SUPPORT

        if(s_sim_gsm_dphone_support[cur_sim_task])
        {
            //after Dphone warm reset, if card_insert_status is not ok, we should notify MN Initialization is failed
            //and do not to restart sim card
            if(s_dphone_reset_times[cur_sim_task] != 0)
            {
                SIMSIGNAL_SIMInitNotOK(cur_sim_task);
                g_sim_card_setting[cur_sim_task].card_present = FALSE;

                PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF);
                EnableDeepSleep(cur_sim_task);
                return;
            }
        }

#endif

#ifndef SIM_TEST_IN_WIN32

        //reset sim card time not exceed,then reset the sim card
        if(s_total_sim_reset_times[cur_sim_task] < 3)
        {
            s_total_sim_reset_times[cur_sim_task]++;
            sim_fail_flag = FALSE;
        }
        else
        {
            s_total_sim_reset_times[cur_sim_task] = 0;
            sim_fail_flag = TRUE;
        }

        CheckCardStatus_ReStartSIM(cur_sim_task, sim_fail_flag, FALSE);
#else
        //if now in refresh reset, we should notify MN SIM_REFRESH_RESET_CNF
        SIMMAIN_SendRefreshResetCnf(cur_sim_task, FALSE);

        SCI_CREATE_SIGNAL(sending_sig, SIM_CARD_PRESENT_IND, sizeof(SIM_SIG_CARD_PRESENT_IND_T), g_sim_task_id[cur_sim_task]);
        ((SIM_SIG_CARD_PRESENT_IND_T *)sending_sig)->insert_status = g_sim_card_setting[cur_sim_task].card_insert_status;
        SCI_SEND_SIGNAL(sending_sig, g_sim_info_task_id[cur_sim_task]);
#endif
    }
}

/********************************************************************
//    DESCRIPTION
//      When there are SIM card in the SIM slot,the next step is receive
//    the ATR  data from the SIM card.and decode the ATR data to make
//    sure the SIM card is inserted ok,the get the SIM cad specific data
//    for initialize using.at this time also no service request will be
//    suppled.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void ATRStateProcessor(uint32 cur_sim_task)
{
    xSignalHeaderRec *psig = NULL;

    s_sim_keep_state[cur_sim_task] = TRUE;

    //when in atr state,first disable the deepsleep
    //SCI_TRACE_LOW:"SIM%d::ATRState disable deepsleep."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_2237_112_2_17_22_59_37_4548, (uint8 *)"d", cur_sim_task);
    DisableDeepSleep(cur_sim_task);

    while(s_sim_keep_state[cur_sim_task])
    {
        psig = RecevieSignal(cur_sim_task);

        switch(psig->SignalCode)
        {
#ifdef SIM_PLUG_IN_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK
        case SIM_QUERY_CARD_STATUS_REQ:
            QueryCardStatus(cur_sim_task);
            break;
#endif
#endif
#ifndef SIM_CODE_COMPACT		
            case SIM_GET_CARD_TYPE_REQ:
                SIMSIGNAL_GetCardType(cur_sim_task, (SIM_SIG_GET_CARD_TYPE_REQ_T *) psig);
                break;
            case SIM_GET_CDMA_CARD_IMSI_REQ:
                SIMSIGNAL_GetCDMACardIMSI(cur_sim_task, (SIM_SIG_GET_CDMA_CARD_IMSI_REQ_T *) psig);
                break;
#endif
            case SIM_CARD_INSERTED_IND:
#ifdef SIM_PLUG_IN_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK
			if (cur_sim_task == s_sim_plug_in_by_check_card_id)
			{
				if (SCI_IsTimerActive(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id]))
				{
					SCI_DeactiveTimer(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id]);
				}
				SCI_ChangeTimer(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id],SIMMAIN_PLUG_IN_By_Check,s_sim_plug_in_by_check_interval);
				SCI_ActiveTimer(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id]);
			}
#endif
#endif            
                //process the ATR and prepare Card
                CheckCardStatus(cur_sim_task, (SIM_SIG_CARD_INSERTED_IND_T *)psig);
                break;
            case SIM_INT_IND:
                IntProcess(cur_sim_task, (SIM_SIG_INT_IND_T *)psig);
                break;
            case SIM_START_UP_REQ:
                {
                    PowerOffSIM(cur_sim_task, SIM_ABNORMAL_POWER_OFF);
                    SCI_SLEEP(10);//delay for voltage stable
                    StartupSIM(cur_sim_task);
                    s_sim_keep_state[cur_sim_task] = TRUE;//we should keep in ATR state!
                }
                break;
            case SIM_POWER_OFF_REQ:
                PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF);
                break;
            case SIMAT_TERMINAL_PROFILE_REQ:
                SIMSIGNAL_TerminalProfile(cur_sim_task, (SIMAT_SIG_TERMINAL_PROFILE_REQ_T *)psig);
                break;
            default:
                //SCI_TRACE_LOW:"SIM::ATR state received abnormal signal:signal_code =0x%x,sender =0x%x\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_2290_112_2_17_22_59_37_4549, (uint8 *)"dd", psig->SignalCode, psig->Sender);
                //SCI_ASSERT(FALSE);
                break;
        }

        SCI_FREE_SIGNAL(psig);

        //ATRStateProcessor do not take the mutex, so we should not give up mutex when next state is still ATR
        if(SIM_ATR_STATE == g_sim_card_state[cur_sim_task])
        {
            s_sim_keep_state[cur_sim_task] = TRUE;//we should keep in ATR state!
        }
    }

    if(s_sim_is_ext_dualsim)
    {
        if(0 != s_sim_cardout_reset_times[cur_sim_task])
        {
            //s_sim_cardout_reset_times is 2, there is no SIM.
            if((s_sim_cardout_reset_times[cur_sim_task] > 1)
                || (SIM_NULL_STATE == g_sim_card_state[cur_sim_task])
                || (SIM_NO_POWER_STATE == g_sim_card_state[cur_sim_task]))
            {
                SIMMAIN_PutMutex(cur_sim_task);
            }
        }
        else
        {
            SIMMAIN_PutMutex(cur_sim_task);
        }
    }
}

/********************************************************************
//    DESCRIPTION
//      After the successful ATR proccess,the other layer can send
//    INITIALIZE request service to INITIALIZE the SIM card.and at
//    this state,just only INITIALIZE request service is accepted.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void InitializeStateProcessor(uint32 cur_sim_task)
{
    xSignalHeaderRec *psig = NULL;

    s_sim_keep_state[cur_sim_task] = TRUE;
    //SCI_TRACE_LOW:"SIM%d::InitilaizeState disable deepsleep."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_2333_112_2_17_22_59_37_4550, (uint8 *)"d", cur_sim_task);
    EnableDeepSleep(cur_sim_task);//Maybe the user forget to input PIN1/PUK1.

    while(s_sim_keep_state[cur_sim_task])
    {
        psig = RecevieSignal(cur_sim_task);
        DisableDeepSleep(cur_sim_task);
        SIMMAIN_GetMutex(cur_sim_task);

        switch(psig->SignalCode)
        {
#ifdef SIM_PLUG_IN_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK
        case SIM_QUERY_CARD_STATUS_REQ:
            QueryCardStatus(cur_sim_task);
            break;
#endif
#endif
#ifndef SIM_CODE_COMPACT
            case SIM_INIT_ENCRYPT_REQ:
                EnterService(cur_sim_task, psig);

                if(SIMSIGNAL_EncryptReadIMSI(cur_sim_task, (SIM_SIG_INIT_ENCRYPT_REQ_T *)psig))
                {
                    SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_ACTIVE_STATE, "SIM Active State", g_sim_task_id[cur_sim_task]);
                    StartCardPresentCheck(cur_sim_task);
                }

                //the service is finished,now clear the service info and enable new service request
                FinishService(cur_sim_task);
                break;
            case SIM_GET_CDMA_CARD_IMSI_REQ:
                SIMSIGNAL_GetCDMACardIMSI(cur_sim_task, (SIM_SIG_GET_CDMA_CARD_IMSI_REQ_T *) psig);
                break;
#endif
#ifdef MODEM_PLATFORM
            case SIM_GET_CARD_TYPE_REQ:
                SIMSIGNAL_GetCardType(cur_sim_task, (SIM_SIG_GET_CARD_TYPE_REQ_T *) psig);
                break;
#endif
            case SIM_INT_IND:
                IntProcess(cur_sim_task, (SIM_SIG_INT_IND_T *)psig);
                break;
            case SIM_INITIALIZE_REQ:

                //stop the pin1/puk1 wait timer if the user input the pin1/puk1 code
                if(SCI_IsTimerActive(s_sim_pin1puk1_wait_timer[cur_sim_task]))
                {
                    SCI_DeactiveTimer(s_sim_pin1puk1_wait_timer[cur_sim_task]);
                }

#ifdef GSM_DPHONE_VER2_SUPPORT
                SCI_TRACE_LOW("SIM%d: SIM_INITIALIZE_REQ is received in Initialize state", cur_sim_task);

                if(s_sim_is_dphone_ver2_need_pending[cur_sim_task])
                {
                    s_sim_is_dphone_ver2_need_pending[cur_sim_task] = FALSE;

                    if(!SIMSIGNAL_SIMDphonePinOperation(cur_sim_task, (SIM_SIG_INITIALIZE_REQ_T *)psig))
                    {
                        //PIN check is failed, keep waiting
                        s_sim_is_dphone_ver2_need_pending[cur_sim_task] = TRUE;
                    }

                    //PIN CHECK & terminal profile have been done, skip next time
                    s_sim_is_dphone_operation_skip[cur_sim_task] = TRUE;
                    s_sim_is_initialize_skip_refresh[cur_sim_task] = TRUE;
                    StartCardPresentCheck(cur_sim_task);
                }
                else
#endif
                {
#ifdef GSM_DPHONE_VER2_SUPPORT
                    s_sim_is_initialize_skip_refresh[cur_sim_task] = FALSE;
#endif

                    //if Initialize success,go to the Actived state
                    if(SIMSIGNAL_SIMInitialization(cur_sim_task, (SIM_SIG_INITIALIZE_REQ_T *)psig))
                    {
                        SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_ACTIVE_STATE, "SIM Active State", g_sim_task_id[cur_sim_task]);
#if defined (GSM_DPHONE_VER2_SUPPORT) || defined (MODEM_PLATFORM_PS_SIM_SAVE_POWER)
						//Do not self-check
#else
                        StartCardPresentCheck(cur_sim_task);
#endif

                        //if now in refresh reset, we should notify MN SIM_REFRESH_RESET_CNF
                        SIMMAIN_SendRefreshResetCnf(cur_sim_task, TRUE);
                    }
                    else
                    {
                        //if now in refresh reset, we should notify MN SIM_REFRESH_RESET_CNF
                        SIMMAIN_SendRefreshResetCnf(cur_sim_task, FALSE);
                    }

#ifdef GSM_DPHONE_VER2_SUPPORT
                    //Skip aready, now open!
                    s_sim_is_dphone_operation_skip[cur_sim_task] = FALSE;
#endif
                }

                break;
#ifdef GSM_DPHONE_VER2_SUPPORT
            case SIM_CHV_REQ:
                //now inservice disable other request service and save cur service info
                EnterService(cur_sim_task, psig);

                SIMSIGNAL_CHVRequest(cur_sim_task, (SIM_SIG_CHV_REQ_T *)psig);

                //the service is finished,now clear the service info and enable new service request
                FinishService(cur_sim_task);
                break;
#endif
            case SIM_START_UP_REQ:
                {
                    PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF);
                    StartupSIM(cur_sim_task);

                    if(s_sim_is_ext_dualsim)
                    {
                        //ext dual sim. we will change into ATR state directly.
                        //Not give up the semaphore. Yang Wuding.2007.6.20
                        SCI_FREE_SIGNAL(psig);
                        return;
                    }
                }
                break;
            case SIM_POWER_OFF_REQ:
                PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF);
                break;
#ifdef GSM_DPHONE_SUPPORT
            case SIM_DPHONE_REFRESH_REQ:

                if(s_sim_gsm_dphone_support[cur_sim_task])
                {
                    s_dphone_refresh_type[cur_sim_task] = ((SIMAT_DPHONE_REFRESH_REQ_T *)psig)->refresh_type;

                    if(s_dphone_refresh_type[cur_sim_task] != SIMAT_SIG_RF_RESET)
                    {
                        SIMSIGNAL_SIMInitReq(cur_sim_task);
                    }
                    else
                    {
                        SIMMAIN_RefreshWarmReset(cur_sim_task);

                        if(s_sim_is_ext_dualsim)
                        {
                            //ext dual sim. we will change into ATR state directly.
                            //Not give up the semaphore. Yang Wuding.2007.6.20
                            SCI_FREE_SIGNAL(psig);
                            return;
                        }
                    }
                }

                break;
            case SIM_DPHOEN_REFRESH_TIMEOUT_IND:
                SIMSIGNAL_SIMInitNotOK(cur_sim_task);
#if 0
                g_sim_card_setting[cur_sim_task].card_present = FALSE;
                SIM_NEXTSTATE(g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);

                //reset the SIM variable
                SIMMAIN_ResetSIMVariable();

                PowerOffSIM(SIM_NORMAL_POWER_OFF);
#endif
                EnableDeepSleep(cur_sim_task);
                break;
#endif

#ifdef GSM_DPHONE_VER2_SUPPORT
            case SIM_READ_FILE_BUF_BY_PATH_REQ:
                SIMSIGNAL_ReadFileBufByPath(cur_sim_task, (SIM_SIG_READ_FILE_BUF_BY_PATH_REQ_T *)psig);
                break;
#endif
#ifdef MODEM_PLATFORM
            case SIM_SIG_CRSM_REQ:
                //now inservice disable other request service and save cur service info
                EnterService(cur_sim_task, psig);

                SIMSIGNAL_CRSM(cur_sim_task, (SIM_SIG_CRSM_REQ_T *)psig);

                //the service is finished,now clear the service info and enable new service request
                FinishService(cur_sim_task);
                break;
#endif
            case SIM_PC_EXIST_IND:
                SIMSIGNAL_GetPC(cur_sim_task, (SIM_SIG_PC_EXIST_IND_T *)psig);
                break;
            case SIMAT_TERMINAL_RESPONSE_REQ:
                SIMSIGNAL_TerminalResponse(cur_sim_task, (SIMAT_SIG_TERMINAL_RESPONSE_REQ_T *)psig);
                break;
            case SIMAT_TERMINAL_PROFILE_REQ:
                SIMSIGNAL_TerminalProfile(cur_sim_task, (SIMAT_SIG_TERMINAL_PROFILE_REQ_T *)psig);
                break;
            case SIMAT_POLLING_IND:
                SIMSIGNAL_Polling(cur_sim_task, (SIMAT_SIG_POLLING_IND_T *)psig);
                break;
            case SIM_REFRESH_INI_REQ:
                //now inservice disable other request service and save cur service info
                EnterService(cur_sim_task, psig);

#ifdef GSM_DPHONE_VER2_SUPPORT

                if(s_sim_is_initialize_skip_refresh[cur_sim_task])
                {
                    SIMSIGNAL_DphoneRefreshskip(cur_sim_task, psig->Sender);
                }
                else
#endif
                {
                    SIMSIGNAL_RefreshIni(cur_sim_task, (SIM_SIG_REFRESH_INI_REQ_T *)psig);
                }

                //the service is finished,now clear the service info and enable new service request
                FinishService(cur_sim_task);
                break;
            case SIM_REFRESH_FILE_CHG_REQ:
                //now inservice disable other request service and save cur service info
                EnterService(cur_sim_task, psig);

#ifdef GSM_DPHONE_VER2_SUPPORT

                if(s_sim_is_initialize_skip_refresh[cur_sim_task])
                {
                    SIMSIGNAL_DphoneRefreshskip(cur_sim_task, psig->Sender);
                }
                else
#endif
                {
                    SIMSIGNAL_RefreshFileChg(cur_sim_task, (SIM_SIG_REFRESH_FILE_CHG_REQ_T *)psig);
                }

                //the service is finished,now clear the service info and enable new service request
                FinishService(cur_sim_task);
                break;
            case SIM_REFRESH_INI_FILE_CHG_REQ:
                //now inservice disable other request service and save cur service info
                EnterService(cur_sim_task, psig);

#ifdef GSM_DPHONE_VER2_SUPPORT

                if(s_sim_is_initialize_skip_refresh[cur_sim_task])
                {
                    SIMSIGNAL_DphoneRefreshskip(cur_sim_task, psig->Sender);
                }
                else
#endif
                {
                    SIMSIGNAL_RefreshIniFileChg(cur_sim_task, (SIM_SIG_REFRESH_INI_FILE_CHG_REQ_T *)psig);
                }

                //the service is finished,now clear the service info and enable new service request
                FinishService(cur_sim_task);
                break;

            case SIM_REFRESH_INI_FULL_REQ:
                //now inservice disable other request service and save cur service info
                EnterService(cur_sim_task, psig);

#ifdef GSM_DPHONE_VER2_SUPPORT

                if(s_sim_is_initialize_skip_refresh[cur_sim_task])
                {
                    SIMSIGNAL_DphoneRefreshskip(cur_sim_task, psig->Sender);
                }
                else
#endif
                {
                    SIMSIGNAL_RefreshIniFull(cur_sim_task, (SIM_SIG_REFRESH_INI_FULL_REQ_T *)psig);
                }

                //the service is finished,now clear the service info and enable new service request
                FinishService(cur_sim_task);
                break;
            case SIM_REFRESH_READ_FILE_REQ:
                //now inservice disable other request service and save cur service info
                EnterService(cur_sim_task, psig);

                SIMSIGNAL_RefreshReadFile(cur_sim_task, (SIM_SIG_REFRESH_READ_FILE_REQ_T *)psig);

                //the service is finished,now clear the service info and enable new service request
                FinishService(cur_sim_task);
                break;
            case SIM_REFRESH_RESET_REQ:
                //now inservice disable other request service and save cur service info
                EnterService(cur_sim_task, psig);

                SIMMAIN_RefreshWarmReset(cur_sim_task);

                //the service is finished,now clear the service info and enable new service request
                FinishService(cur_sim_task);

                if(s_sim_is_ext_dualsim)
                {
                    //ext dual sim. we will change into ATR state directly.
                    //Not give up the semaphore. Yang Wuding.2007.6.20
                    SCI_FREE_SIGNAL(psig);
                    return;
                }

                break;

            default:
                //SCI_TRACE_LOW:"SIM::Initilaize state received abnormal signal:signal_code =0x%x,sender =0x%x\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_2521_112_2_17_22_59_38_4551, (uint8 *)"dd", psig->SignalCode, psig->Sender);
                break;
        }

        SCI_FREE_SIGNAL(psig);
        SIMMAIN_PutMutex(cur_sim_task);
    }
}

/********************************************************************
//    DESCRIPTION
//    dispatch the service requst signal to it's process function in
//    active state.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void ServiceRequestDispatch(
    uint32 cur_sim_task,
    xSignalHeaderRec *psig  //the servuce request signal
)
{
#if defined SIM_DROP_RESTORE_SUPPORT && defined SIM_DROP_RESTORE_DELAY_PBSMS_READ
    static BOOLEAN s_sim_first_enter_flag[4] = {TRUE,TRUE,TRUE,TRUE};
#endif
#ifdef MULTI_SIM_SYS_TRI
    SCI_ASSERT((0 == cur_sim_task) || (1 == cur_sim_task) || (2 == cur_sim_task)); /*assert verified:Check_value */
#elif defined (MULTI_SIM_SYS_QUAD)
    SCI_ASSERT((0 == cur_sim_task) || (1 == cur_sim_task) || (2 == cur_sim_task) || (3 == cur_sim_task)); /*assert verified:Check_value */
#endif

    SCI_ASSERT(NULL != psig); /*assert verified: Null_pointer*/

    switch(psig->SignalCode)
    {
        case SIM_CLOCK_STOP_IND:
            SIMMAIN_ClockStop(cur_sim_task);
            break;
        case SIM_START_UP_REQ:
            PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF);
            StartupSIM(cur_sim_task);
            break;
        case SIM_POWER_OFF_REQ:
            PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF);
            break;
        case SIM_SWITCH_CHECK_CARD_PRES_REQ:
            //now inservice disable other request service
            EnterService(cur_sim_task, psig);

            SwitchCheckCardPresent(cur_sim_task, (SIM_SIG_SWITCH_CHECK_CARD_PRES_REQ_T *)psig);

            FinishService(cur_sim_task);
            break;
        case SIM_PRESENT_CHECK_IND:
            //now inservice disable other request service
            EnterService(cur_sim_task, psig);

            CheckCardPresent(cur_sim_task);

            FinishService(cur_sim_task);
            break;
        case SIM_CHV_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_CHVRequest(cur_sim_task, (SIM_SIG_CHV_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_T_FILE_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            //read the Transparent File
            SIMSIGNAL_ReadTransparentEF(cur_sim_task, (SIM_SIG_READ_T_FILE_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_UPDATE_T_FILE_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            //read record file
            SIMSIGNAL_UpdateTransparentEF(cur_sim_task, (SIM_SIG_UPDATE_T_FILE_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);

            break;
        case SIM_READ_R_FILE_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            //update record file
            SIMSIGNAL_ReadRecordEF(cur_sim_task, (SIM_SIG_READ_R_FILE_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;

        case SIM_UPDATE_R_FILE_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            //update record file
            SIMSIGNAL_UpdateRecordEF(cur_sim_task, (SIM_SIG_UPDATE_R_FILE_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#ifdef GSM_CUSTOMER_AFP_SUPPORT
        case SIM_UPDATE_LANGUAGE_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            //update record file
            SIMSIGNAL_UpdateLanguageReq(cur_sim_task, (SIM_SIG_UPDATE_LANGUAGE_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#endif
        case MM_SIM_DATA_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMMMSIG_MMDataReq(cur_sim_task, psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case MM_SIM_AUTH_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMMMSIG_AuthReq(cur_sim_task, (yPDef_MM_SIM_AUTH_REQ *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case MM_SIM_FPLMN_UPD_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMMMSIG_FPLMNUpdateReq(cur_sim_task, (yPDef_MM_SIM_FPLMN_UPD_REQ *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case MM_SIM_CIPHER_UPD_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMMMSIG_CipherUpdateReq(cur_sim_task, (yPDef_MM_SIM_CIPHER_UPD_REQ *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case MM_SIM_STATUS_UPD_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMMMSIG_SIMStatusUpdateReq(cur_sim_task, (yPDef_MM_SIM_STATUS_UPD_REQ *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case MM_SIM_BALIST_UPD_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMMMSIG_BAListUpdateReq(cur_sim_task, (yPDef_MM_SIM_BALIST_UPD_REQ *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_SMS_READ_PARAM_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSMS_ReadSMSParameter(cur_sim_task, (SIM_SMS_READ_PARAM_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_SMS_GET_SMS_NUM_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSMS_GetSMSNum(cur_sim_task, (SIM_SMS_GET_SMS_NUM_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_SMS_READ_SMS_REQ:
            {
                //now inservice disable other request service and save cur service info
                EnterService(cur_sim_task,psig);

#if defined SIM_DROP_RESTORE_SUPPORT && defined SIM_DROP_RESTORE_DELAY_PBSMS_READ
////////////////////////////////////////////////////
                if (s_sim_first_enter_flag[cur_sim_task])
                {
                    s_sim_first_enter_flag[cur_sim_task] = FALSE;

                    if (NULL != s_sim_delay_smspb_timer[cur_sim_task])
                	{
                	    SCI_TRACE_LOW("SIM%d: start delay timer",cur_sim_task);

                		if (SCI_IsTimerActive(s_sim_delay_smspb_timer[cur_sim_task]))
                		{
                			SCI_DeactiveTimer(s_sim_delay_smspb_timer[cur_sim_task]);
                		}

                		SCI_ChangeTimer(s_sim_delay_smspb_timer[cur_sim_task],SIM_Delay_Read,25000);//Delay 45s
                		SCI_ActiveTimer(s_sim_delay_smspb_timer[cur_sim_task]);
                	}
                }
                else
///////////////////////////////////////////////////////
#endif
                {
        			SIMSMS_ReadSM(cur_sim_task,(SIM_SMS_READ_SMS_REQ_T*)psig);
                }

                //the service is finished,now clear the service info and enable new service request
                FinishService(cur_sim_task);
		    }
            break;
        case SIM_SMS_WRITE_SMS_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSMS_SaveSM(cur_sim_task, (SIM_SMS_WRITE_SMS_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#ifdef MNSMS_REPLACE_SMS
        case SIM_SMS_REPLACE_SMS_REQ: // for replace sms sim
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSMS_ReplaceSM(cur_sim_task, (SIM_SMS_REPLACE_SMS_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#endif
        case SIM_SMS_EDIT_SMS_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSMS_EditSM(cur_sim_task, (SIM_SMS_EDIT_SMS_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_SMS_UPDATE_SMS_STATUS_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSMS_UpdateSMState(cur_sim_task, (SIM_SMS_UPDATE_SMS_STATUS_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_SMS_READ_STATUS_REPORT_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSMS_ReadSMStatusReport(cur_sim_task, (SIM_SMS_READ_STATUS_REPORT_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_SMS_READ_SMSS_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSMS_GetTPMessageReference(cur_sim_task, (SIM_SMS_READ_SMSS_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_SMS_WRITE_SMSS_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSMS_SaveTPMessageReference(cur_sim_task, (SIM_SMS_WRITE_SMSS_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_SMS_WRITE_PARAM_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSMS_UpdateSMSParameter(cur_sim_task, (SIM_SMS_WRITE_PARAM_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;

#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK
#ifdef SIM_PLUG_IN_SUPPORT
        case SIM_QUERY_CARD_STATUS_REQ:
            //now inservice disable other request service
            EnterService(cur_sim_task,psig);

            QueryCardStatus(cur_sim_task);

            FinishService(cur_sim_task);
            break;
#endif
#endif

        case SIM_GET_FILE_INFO_REQ:
            //now inservice disable other request service
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_GetFileInfo(cur_sim_task, (SIM_SIG_GET_FILE_INFO_REQ_T *)psig);

            FinishService(cur_sim_task);
            break;
#if !defined (SIM_CODE_COMPACT) || defined (MODEM_PLATFORM)
        case SIM_GET_CARD_TYPE_REQ:
            //now inservice disable other request service
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_GetCardType(cur_sim_task, (SIM_SIG_GET_CARD_TYPE_REQ_T *) psig);

            FinishService(cur_sim_task);
            break;
#endif
#ifndef SIM_CODE_COMPACT
        case SIM_INCREASE_FILE_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_IncreaseEF(cur_sim_task, (SIM_SIG_INCREASE_FILE_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;

        case SIM_GET_CDMA_CARD_IMSI_REQ:
            //now inservice disable other request service
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_GetCDMACardIMSI(cur_sim_task, (SIM_SIG_GET_CDMA_CARD_IMSI_REQ_T *) psig);

            FinishService(cur_sim_task);
            break;
        case SIM_GET_SPN_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_GetSPN(cur_sim_task, (SIM_SIG_GET_SPN_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_ECC_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadECC(cur_sim_task, (SIM_SIG_READ_ECC_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_UPDATE_CCP_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMDN_UpdateCCP(cur_sim_task, (SIM_SIG_UPDATE_CCP_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_CCP_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMDN_ReadCCP(cur_sim_task, (SIM_SIG_READ_CCP_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_ERASE_CCP_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMDN_EraseCCP(cur_sim_task, (SIM_SIG_ERASE_CCP_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_GET_FREE_EXT_INFO_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMDN_GetFreeExtInfo(cur_sim_task, (SIM_SIG_GET_FREE_EXT_INFO_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_PURGE_DN_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMDN_PurgeDN(cur_sim_task, (SIM_SIG_PURGE_DN_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;

        case SIM_READ_LP_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadLP(cur_sim_task, (SIM_SIG_READ_LP_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_UPDATE_LP_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdateLP(cur_sim_task, (SIM_SIG_UPDATE_LP_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_AD_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadAD(cur_sim_task, (SIM_SIG_READ_AD_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_SST_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadSST(cur_sim_task, (SIM_SIG_READ_SST_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_PHASE_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadPhase(cur_sim_task, (SIM_SIG_READ_PHASE_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_ELP_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadELP(cur_sim_task, (SIM_SIG_READ_ELP_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_UPDATE_ELP_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdateELP(cur_sim_task, (SIM_SIG_UPDATE_ELP_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_IMSI_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadIMSI(cur_sim_task, (SIM_SIG_READ_IMSI_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_ACC_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadACC(cur_sim_task, (SIM_SIG_READ_ACC_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_HPLMN_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadHPLMN(cur_sim_task, (SIM_SIG_READ_HPLMN_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_LOCI_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadLOCI(cur_sim_task, (SIM_SIG_READ_LOCI_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_UPDATE_LOCI_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdateLOCI(cur_sim_task, (SIM_SIG_UPDATE_LOCI_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_KC_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadKC(cur_sim_task, (SIM_SIG_READ_KC_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_UPDATE_KC_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdateKC(cur_sim_task, (SIM_SIG_UPDATE_KC_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_BCCH_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadBCCH(cur_sim_task, (SIM_SIG_READ_BCCH_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_UPDATE_BCCH_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdateBCCH(cur_sim_task, (SIM_SIG_UPDATE_BCCH_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_FPLMN_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadFPLMN(cur_sim_task, (SIM_SIG_READ_FPLMN_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_UPDATE_FPLMN_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdateFPLMN(cur_sim_task, (SIM_SIG_UPDATE_FPLMN_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_PLMNSEL_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadPLMNSEL(cur_sim_task, (SIM_SIG_READ_PLMNSEL_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_UPDATE_PLMNSEL_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdatePLMNSEL(cur_sim_task, (SIM_SIG_UPDATE_PLMNSEL_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_SESSION_TERMINATION_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_SessionTerminate(cur_sim_task, (SIM_SIG_SESSION_TERMINATION_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_CBMI_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadCBMI(cur_sim_task, (SIM_SIG_READ_CBMI_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_UPDATE_CBMI_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdateCBMI(cur_sim_task, (SIM_SIG_UPDATE_CBMI_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_VGCS_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadVGCS(cur_sim_task, (SIM_SIG_READ_VGCS_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_VGCSS_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadVGCSS(cur_sim_task, (SIM_SIG_READ_VGCSS_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_VBS_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadVBS(cur_sim_task, (SIM_SIG_READ_VBS_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_VBSS_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadVBSS(cur_sim_task, (SIM_SIG_READ_VBSS_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_EMLPP_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadEMLPP(cur_sim_task, (SIM_SIG_READ_EMLPP_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_AAEM_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadAAEM(cur_sim_task, (SIM_SIG_READ_AAEM_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_UPDATE_AAEM_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdateAAEM(cur_sim_task, (SIM_SIG_UPDATE_AAEM_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_CBMIR_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadCBMIR(cur_sim_task, (SIM_SIG_READ_CBMIR_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_UPDATE_CBMIR_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdateCBMIR(cur_sim_task, (SIM_SIG_UPDATE_CBMIR_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_DCK_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadDCK(cur_sim_task, (SIM_SIG_READ_DCK_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_NIA_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadNIA(cur_sim_task, (SIM_SIG_READ_NIA_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#endif

#ifdef SIM_AOC_SUPPORT
        case SIM_AOC_READ_ACM_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadACM(cur_sim_task, (SIM_SIG_AOC_READ_ACM_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_AOC_READ_ACMMAX_REQ://now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadACMMAX(cur_sim_task, (SIM_SIG_AOC_READ_ACMMAX_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#endif

        case SIM_READ_DN_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMDN_ReadDN(cur_sim_task, (SIM_SIG_READ_DN_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_UPDATE_DN_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMDN_UpdateDN(cur_sim_task, (SIM_SIG_UPDATE_DN_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_ERASE_DN_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMDN_EraseDN(cur_sim_task, (SIM_SIG_ERASE_DN_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_ERASE_LND_ALL_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMDN_EraseLNDAll(cur_sim_task, (SIM_SIG_ERASE_LND_ALL_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_FDN_SERVICE_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMDN_FDNService(cur_sim_task, (SIM_SIG_FDN_SERVICE_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_BDN_SERVICE_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMDN_BDNService(cur_sim_task, (SIM_SIG_BDN_SERVICE_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#ifdef _SUPPORT_GPRS_
        case GMM_SIM_AUTH_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMGMMSIG_AuthReq(cur_sim_task, (yPDef_GMM_SIM_AUTH_REQ *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case GMM_SIM_SAVE_CIPHER_INFO:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMGMMSIG_SaveCipherInfo(cur_sim_task, (yPDef_GMM_SIM_SAVE_CIPHER_INFO *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case GMM_SIM_SAVE_RAI_INFO:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMGMMSIG_SaveRAIInfo(cur_sim_task, (yPDef_GMM_SIM_SAVE_RAI_INFO *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#endif
#ifndef SIM_CODE_COMPACT
        case SIM_READ_GID1_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadGID1(cur_sim_task, (SIM_SIG_READ_GID_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_READ_GID2_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadGID2(cur_sim_task, (SIM_SIG_READ_GID_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#endif

#ifdef SIM_AOC_SUPPORT
        case SIM_AOC_INIT_ACM_REQ://now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_InitACM(cur_sim_task, (SIM_SIG_AOC_INIT_ACM_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_AOC_INCREASE_ACM_REQ://now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_IncreaseACM(cur_sim_task, (SIM_SIG_AOC_INCREASE_ACM_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_AOC_UPDATE_ACMMAX_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdateACMMAX(cur_sim_task, (SIM_SIG_AOC_UPDATE_ACMMAX_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_AOC_READ_PUCT_REQ://now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadPUCT(cur_sim_task, (SIM_SIG_AOC_READ_PUCT_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;            
        case SIM_AOC_UPDATE_PUCT_REQ://now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdatePUCT(cur_sim_task, (SIM_SIG_AOC_UPDATE_PUCT_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#endif

        case SIM_GET_RECORD_EF_INFO_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_GetRecordEFInfo(cur_sim_task, (SIM_SIG_GET_RECORD_EF_INFO_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;

#ifdef GSM_DPHONE_VER2_SUPPORT
        case SIM_READ_FILE_BUF_BY_PATH_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadFileBufByPath(cur_sim_task, (SIM_SIG_READ_FILE_BUF_BY_PATH_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#endif
#ifdef MODEM_PLATFORM
        case SIM_SIG_CRSM_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_CRSM(cur_sim_task, (SIM_SIG_CRSM_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#endif
        case SIM_PC_EXIST_IND:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_GetPC(cur_sim_task, (SIM_SIG_PC_EXIST_IND_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIMAT_POLLING_IND:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_Polling(cur_sim_task, (SIMAT_SIG_POLLING_IND_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIMAT_TERMINAL_RESPONSE_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);
            SIMSIGNAL_TerminalResponse(cur_sim_task, (SIMAT_SIG_TERMINAL_RESPONSE_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIMAT_ENVELOPE_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_Envelope(cur_sim_task, (SIMAT_SIG_ENVELOPE_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIMAT_TERMINAL_PROFILE_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_TerminalProfile(cur_sim_task, (SIMAT_SIG_TERMINAL_PROFILE_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_REFRESH_INI_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_RefreshIni(cur_sim_task, (SIM_SIG_REFRESH_INI_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_REFRESH_FILE_CHG_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_RefreshFileChg(cur_sim_task, (SIM_SIG_REFRESH_FILE_CHG_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_REFRESH_INI_FILE_CHG_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_RefreshIniFileChg(cur_sim_task, (SIM_SIG_REFRESH_INI_FILE_CHG_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_REFRESH_INI_FULL_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_RefreshIniFull(cur_sim_task, (SIM_SIG_REFRESH_INI_FULL_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
        case SIM_REFRESH_READ_FILE_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_RefreshReadFile(cur_sim_task, (SIM_SIG_REFRESH_READ_FILE_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;

        case SIM_REFRESH_RESET_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMMAIN_RefreshWarmReset(cur_sim_task);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;

#ifdef SIMAT_ICON_SUPPORT
        case SIM_IMG_READ_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadImg(cur_sim_task, (SIM_SIG_IMG_READ_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#endif

#ifndef SIM_CODE_COMPACT
        case SIM_COMMON_AUTH_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_CommonAuth(cur_sim_task, (SIM_SIG_COMMON_AUTH_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#endif
        case SIM_READ_VOICE_MAIL_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadVoiceMail(cur_sim_task, (SIM_SIG_READ_VOICE_MAIL_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;

        case SIM_UPDATE_VOICE_MAIL_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdateVoiceMail(cur_sim_task, (SIM_SIG_UPDATE_VOICE_MAIL_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
			
#ifdef MODEM_PLATFORM
        case SIMAT_AT_CPDU_ENVELOPE_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ATCpduEnvelope(cur_sim_task, (SIMAT_SIG_AT_CPDU_ENVELOPE_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#endif

#ifdef MODEM_PLATFORM
#ifdef _SUPPORT_IPC_
        case SIM_READ_RAW_T_FILE_REQ:
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadTransparentEFInRawData(cur_sim_task, (SIM_SIG_READ_RAW_T_FILE_REQ_T *)psig);

            FinishService(cur_sim_task);
            break;

        case SIM_UPDATE_RAW_T_FILE_REQ:
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdateTransparentEFInRawData(cur_sim_task, (SIM_SIG_UPDATE_RAW_T_FILE_REQ_T *)psig);

            FinishService(cur_sim_task);
            break;

        case SIM_READ_RAW_R_FILE_REQ:
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_ReadRecordEFInRawData(cur_sim_task, (SIM_SIG_READ_RAW_R_FILE_REQ_T *)psig);

            FinishService(cur_sim_task);
            break;

        case SIM_UPDATE_RAW_R_FILE_REQ:
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_UpdateRecordEFInRawData(cur_sim_task, (SIM_SIG_UPDATE_RAW_R_FILE_REQ_T *)psig);

            FinishService(cur_sim_task);
            break;

        case SIM_SEARCH_R_FILE_REQ:
            EnterService(cur_sim_task, psig);

            SIMSIGNAL_Search(cur_sim_task, (SIM_SIG_SEARCH_R_FILE_REQ_T *)psig);

            FinishService(cur_sim_task);
            break;
#endif
        case SIM_SIG_CSIM_REQ:
            //now inservice disable other request service and save cur service info
            EnterService(cur_sim_task, psig);
			
			SIMSIGNAL_CSIM(cur_sim_task, (SIM_SIG_CSIM_REQ_T *)psig);

            //the service is finished,now clear the service info and enable new service request
            FinishService(cur_sim_task);
            break;
#endif
        default:
            //SCI_TRACE_LOW:"SIM::Active state received abnormal signal:signal_code =0x%x,sender =0x%x\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_3492_112_2_17_22_59_40_4552, (uint8 *)"dd", psig->SignalCode, psig->Sender);
            break;
    }
}

/********************************************************************
//    DESCRIPTION
//      after the successful Initialize,the SIM card can now supply
//    kinds of service,and this state is the SIM working state.the
//    other layer now can request all kinds service.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void ActiveStateProcessor(uint32 cur_sim_task)
{
    xSignalHeaderRec *psig = NULL;
    xSignalHeaderRec *psig_int = NULL;
#ifdef SIM_PLUG_IN_SUPPORT
    BOOLEAN is_plug_in_flag[SIM_CARD_NUM] = {FALSE};
    xSignalHeaderRec *pintind = NULL;
#endif

    s_sim_keep_state[cur_sim_task] = TRUE;

    //when enter active state,if card allow clock stop,enable deepsleep
    if(SIM_CLOCK_S_FORBIDEN != g_sim_card_setting[cur_sim_task].gsm_data.clock_stop_type)
    {
        //SCI_TRACE_LOW:"SIM%d::ActiveState enable deepsleep."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_3514_112_2_17_22_59_40_4553, (uint8 *)"d", cur_sim_task);
        EnableDeepSleep(cur_sim_task);
    }

    while(s_sim_keep_state[cur_sim_task])
    {
        psig = RecevieSignal(cur_sim_task);
        SIMMAIN_GetMutex(cur_sim_task);

        if(SIM_INT_IND == psig->SignalCode)
        {
            IntProcess(cur_sim_task, (SIM_SIG_INT_IND_T *)psig);
        }
        else
        {
            ServiceRequestDispatch(cur_sim_task, psig);
        }

        if(s_sim_is_ext_dualsim)
        {
            if((SIM_START_UP_REQ == psig->SignalCode) || (SIM_REFRESH_RESET_REQ == psig->SignalCode))
            {
                SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);
                SCI_FREE_SIGNAL(psig);
                return;
                //ext dual sim. we will change into ATR state directly when SIM_START_UP_REQ.
                //Not give up the semaphore. wuding.
            }
            else
            {
                //Ext-dualsim. If there is a INT during a operation, we should
                //restore the current sim before give up the MUTEX. Or the
                //other sim drv will have problem.
                psig_int = SIMINSTR_CheckTempIntSignalQueue(cur_sim_task);

                if(NULL != psig_int)
                {
                    IntProcess(cur_sim_task, (SIM_SIG_INT_IND_T *)psig_int);
                    SCI_FREE_SIGNAL(psig_int);

#ifdef SIM_PLUG_IN_SUPPORT

                    if(TB_SIMINT_PLUG_IN == SIMINSTR_LatestHotSwapMessageInTempIntSignalQueue(cur_sim_task))
                    {
                        is_plug_in_flag[cur_sim_task] = TRUE;
                    }

#endif

                    SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);

#ifdef SIM_PLUG_IN_SUPPORT

                    if(is_plug_in_flag[cur_sim_task])
                    {
                        SCI_CREATE_SIGNAL(pintind, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), g_sim_task_id[cur_sim_task]);
                        ((SIM_SIG_INT_IND_T *)pintind)->int_type = TB_SIMINT_PLUG_IN;
                        SIMINSTR_AddThisSIGNALInTempIntSignalQueue(cur_sim_task, pintind);
                    }

#endif
                }
            }
        }

        SCI_FREE_SIGNAL(psig);
        SIMMAIN_PutMutex(cur_sim_task);
    }
}

/********************************************************************
//    DESCRIPTION
//      the SIM card have two serivce state and the process of service
//    request signal is different:
//    1.can not acctep new service request from other layer:only signal
//    from the SIM drive(SIM layer self) can be process and service
//    request  should be save to temporary queue for laterly process.
//    2.can accept new service request from other layer
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC xSignalHeaderRec *RecevieSignal(uint32 cur_sim_task)   //return value:the singal received
{
    xSignalHeaderRec *psig = NULL;

    //first check the sim card temp signal queue
    psig = SIMINSTR_CheckTempSignalQueue(cur_sim_task);

    if(NULL == psig)
    {
        //if there no reqest in the temp signal queue
        //the receive signal from the sim task
        SCI_RECEIVE_SIGNAL(psig, g_sim_task_id[cur_sim_task]);
    }

    return psig;
}

/********************************************************************
//    DESCRIPTION
//      the function that process some of the SIM card Interrupt
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void IntProcess(
    uint32 cur_sim_task,
    SIM_SIG_INT_IND_T *psig //the SIM card interrupt signal
)
{
    if((SIM_NO_POWER_STATE == g_sim_card_state[cur_sim_task])
        || (SIM_NULL_STATE == g_sim_card_state[cur_sim_task]))
    {
#ifdef SIM_PLUG_IN_SUPPORT

        if((TB_SIMINT_PLUG_IN == psig->int_type)
            || (TB_SIMINT_PULL_OUT == psig->int_type))
        {
            //continue
        }
        else
#endif
        {
            return;//do not deal these signal in no_power and null state.
        }
    }

    switch(psig->int_type)
    {
        case TB_SIMINT_TX_PARITY_ERROR:
            DealTxParityError(cur_sim_task);
            break;
        case TB_SIMINT_RX_PARITY_ERROR:
        case TB_SIMINT_CARD_UNRESPONSE:
        case TB_SIMINT_NEED_WAITTING://To many null bytes, we restore it.
            DealCardUnResponse(cur_sim_task);
            break;
        case TB_SIMINT_CARD_IN:
            DealCardIn(cur_sim_task);
            break;
        case TB_SIMINT_CARD_OUT:
            DealCardOut(cur_sim_task);
            break;
#ifdef SIM_PLUG_IN_SUPPORT
        case TB_SIMINT_PLUG_IN:
            DealSIMPlugIn(cur_sim_task);
            break;
        case TB_SIMINT_PULL_OUT:
            DealSIMPullOut(cur_sim_task);
            break;
#endif
        default:
            //SCI_TRACE_LOW:"SIM::Wrong SIM Interrupt Type %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_3643_112_2_17_22_59_40_4554, (uint8 *)"d", psig->int_type);
            DealCardUnResponse(cur_sim_task);
            break;
    }
}

/********************************************************************
//    DESCRIPTION
//      the function that Deal with the Card Un Response Interrupt from SIM card
//    Global resource dependence :
//    retrun: TRUE,the card restore is successful,else restore failed
//    Author:Raislin.kong
//    Note:
***********************************************************************/
BOOLEAN DealCardUnResponse(uint32 cur_sim_task)
{
    xSignalHeaderRec *psig1 = NULL;
#ifndef WIN32
    xSignalHeaderRec *psig = NULL;
    SIM_CARD_STATE_E current_sim_state;
#endif
    uint32 redemption_period = 400;//400ms //2000;// 2s
    uint32 exchange_card_id = 0;

#ifdef GSM_DPHONE_SUPPORT

    if(s_sim_gsm_dphone_support[cur_sim_task])
    {
        SCI_CREATE_SIGNAL(psig1, SIM_CARD_REMOVE_IND, sizeof(SIM_SIG_CARD_REMOVE_IND_T), g_sim_task_id[cur_sim_task]);
        SCI_SEND_SIGNAL(psig1, g_sim_info_task_id[cur_sim_task]);
        SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);//Clear the temp int queue because the reset is coming.
        PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF); //power off the sim card
        EnableDeepSleep(cur_sim_task);//enable arm deep sleep
        return FALSE;
    }

#elif defined GSM_DPHONE_VER2_SUPPORT
    SCI_CREATE_SIGNAL(psig1, SIM_CARD_REMOVE_IND, sizeof(SIM_SIG_CARD_REMOVE_IND_T), g_sim_task_id[cur_sim_task]);
    SCI_SEND_SIGNAL(psig1, g_sim_info_task_id[cur_sim_task]);
    SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);//Clear the temp int queue because the reset is coming.
    PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF); //power off the sim card
    EnableDeepSleep(cur_sim_task);//enable arm deep sleep
    return FALSE;
#endif

#ifndef WIN32
    SIM_PrintTxBuf(cur_sim_task);
    SIM_PrintRxBuf(cur_sim_task);
    SIM_PrintRegisterBuf(cur_sim_task);

#ifdef SIM_PLUG_IN_SUPPORT

    if(s_sim_plug_in_flag[cur_sim_task])
    {
        SIMMAIN_IsSIMPlugIn(cur_sim_task, FALSE);
        return FALSE;
    }

#endif

    exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);

#ifdef SIM_3G_USIM_SUPPORT
    s_sim_usim_adf_file_id_flag[cur_sim_task] = FALSE;
    s_sim_gain_chv_remain_times_flag[cur_sim_task] = FALSE;
#endif

    //if the card is before initialization state,do not excute the restore
    if(g_sim_card_state[cur_sim_task] < SIM_INITIALIZE_STATE)
    {
        //SCI_TRACE_LOW:"SIM%d::card unresponse before initialization."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_3700_112_2_17_22_59_40_4555, (uint8 *)"d", cur_sim_task);
    }
    else
    {
        //if the restore count is less than the max restore num,then to excute restore action,else send card_removed_ind to the MN
        //if(s_sim_restore_count[cur_sim_task] > SIM_MAX_RESTORE_COUNT)
        {
            //SCI_TRACE_LOW:"SIM%d::sim restore count is out of the range."
            //SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_3707_112_2_17_22_59_40_4556, (uint8 *)"d", cur_sim_task);
        }
        //else
        {
            current_sim_state = g_sim_card_state[cur_sim_task];
            //power off the sim card if not the ext-dualsim
            SIMHAL_PowerOffSIM(exchange_card_id);
            //we will begin re-store, the SIM_PC_EXIST_IND in queue now
            //should be deleted. it shold not be sent to L4.
            SIMINSTR_ClearPCExistSignalInQueue(cur_sim_task);
#ifdef SIM_DROP_RESTORE_SUPPORT
			SCI_Sleep(1000);
#else
			SCI_Sleep(40);
#endif

#ifdef SIM_DROP_RESTORE_SUPPORT
            //Decrease the RF TX power in dBm.
            //The 30,23,30,23 is a test-experienced value. It can be adjusted.
            L1API_SetGsmMaxTXPwr_Custom(TRUE,30,23,30,23);//Traffic. GSM DCS GSM850 PCS
            L1API_SetGsmMaxTXPwr_Custom_Rach(TRUE,30,23,30,23);//RACH.
#endif

SIM_RESTORE:
            s_sim_restore_count[cur_sim_task]++;
#ifdef SIM_3G_USIM_SUPPORT
            s_sim_usim_adf_file_id_flag[cur_sim_task] = FALSE;
            s_sim_gain_chv_remain_times_flag[cur_sim_task] = FALSE;
#endif
            //stop the clock stop timer
            if(SCI_IsTimerActive(s_sim_idle_timer[cur_sim_task]))
            {
                SCI_DeactiveTimer(s_sim_idle_timer[cur_sim_task]);
            }

            //stop the check card present timer
            if(SCI_IsTimerActive(s_sim_check_period_timer[cur_sim_task]))
            {
                SCI_DeactiveTimer(s_sim_check_period_timer[cur_sim_task]);
                s_sim_call_poll_time[cur_sim_task] = 0;
                s_sim_stk_poll_time[cur_sim_task] = 0;
                s_sim_sim_poll_time[cur_sim_task] = 0;
            }

            SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);//Clear the temp int queue because the reset is coming.
            DisableDeepSleep(cur_sim_task);//disable the deep sleep
            //open the restore timer
            if(SCI_IsTimerActive(s_sim_restore_waiting_timer[cur_sim_task]))
            {
                SCI_DeactiveTimer(s_sim_restore_waiting_timer[cur_sim_task]);
            }

            SCI_ChangeTimer(s_sim_restore_waiting_timer[cur_sim_task], RestoreExpired, SIM_RESTORE_TIME_LIMIT);
            SCI_ActiveTimer(s_sim_restore_waiting_timer[cur_sim_task]);

#ifdef SIM_DROP_RESTORE_SUPPORT
			SIM_DisablePPSFeature(cur_sim_task);//wuding
			SCI_TRACE_LOW("SIM%d::the current s_sim_restore_count is %d",cur_sim_task,s_sim_restore_count[cur_sim_task]);
#endif

            //reset the sim card to check whether the card is in the seat
            //SCI_TRACE_LOW:"SIM%d::begin to restore the sim card."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_3748_112_2_17_22_59_40_4557, (uint8 *)"d", cur_sim_task);
            SIMHAL_ResetDriver(FALSE, exchange_card_id);
            SCI_Sleep((s_sim_ldo_stable_time + 2));

            SIMHAL_ResetSIMCard(FALSE, exchange_card_id, s_sim_restore_with_3v[cur_sim_task]);//dualsim(6600r) or ext-dualsim support 2 different Vsim

            for(;;)
            {
                SCI_RECEIVE_SIGNAL(psig, g_sim_task_id[cur_sim_task]);

                if(psig->SignalCode == SIM_CARD_INSERTED_IND)
                {
                    //wuding add here 
                    SCI_TRACE_LOW("SIM%d::the card status is %d %d %d",cur_sim_task,\
                        ((SIM_SIG_CARD_INSERTED_IND_T*)psig)->t_protocol,\
                         ((SIM_SIG_CARD_INSERTED_IND_T*)psig)->is_3v,\
                        ((SIM_SIG_CARD_INSERTED_IND_T*)psig)->insert_status);

                    //deactive the restore timer
                    if(SCI_IsTimerActive(s_sim_restore_waiting_timer[cur_sim_task]))
                    {
                        SCI_DeactiveTimer(s_sim_restore_waiting_timer[cur_sim_task]);
                    }

                    if (TB_SIM_INSERTED_OK != ((SIM_SIG_CARD_INSERTED_IND_T*)psig)->insert_status)
                    {
                        SCI_FREE_SIGNAL(psig);//free the signal
#if 1//If the hardware has too many problems, then you can use this code, or never. 
                        s_sim_restore_count[cur_sim_task]++;
                        if (s_sim_restore_count[cur_sim_task] < SIM_MAX_RESTORE_COUNT)
                        {
                            //SCI_TRACE_LOW:"SIM%d:restore fail2. %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_MAIN_3828_112_2_17_22_59_40_4560,(uint8*)"dd",cur_sim_task,s_sim_restore_count[cur_sim_task]);
                            SIMHAL_PowerOffSIM(exchange_card_id);
                            SCI_Sleep(redemption_period);
                            goto SIM_RESTORE;
                        }
                        else
#endif
                        {
                            //SCI_TRACE_LOW:"SIM%d::the card is not present."
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIM_MAIN_3836_112_2_17_22_59_40_4561,(uint8*)"d",cur_sim_task);
                            //report to the upplayer that the SIM card have serious error
                            break;//quit the loop
                        }
                    }
                    //wuding add here 

                    //then begin to re-init the sim card
                    if(SIM_ReInitialization(cur_sim_task))
                    {
                        if(SIM_INITIALIZE_STATE == current_sim_state)
                        {
                            SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_INITIALIZE_STATE, "SIM Initialize State", g_sim_task_id[cur_sim_task]);
                        }
                        else
                        {
                            SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_ACTIVE_STATE, "SIM Active State", g_sim_task_id[cur_sim_task]);
                        }

                        s_sim_restore_count[cur_sim_task] = 0;//then clear the restore count

                        //re-start the Idle timer
                        if(SIM_CLOCK_S_FORBIDEN != g_sim_card_setting[cur_sim_task].gsm_data.clock_stop_type)
                        {
                            if(SCI_IsTimerActive(s_sim_idle_timer[cur_sim_task]))
                            {
                                SCI_DeactiveTimer(s_sim_idle_timer[cur_sim_task]);
                            }

                            SCI_ChangeTimer(s_sim_idle_timer[cur_sim_task], IdleCard, SIM_IDLE_TIME_BEFORE_SLEEP);
                            SCI_ActiveTimer(s_sim_idle_timer[cur_sim_task]);
                        }

                        //re-start the check card present timer
                        StartCardPresentCheck(cur_sim_task);

                        SCI_FREE_SIGNAL(psig);//free the signal
                        EnableDeepSleep(cur_sim_task);//enable the deep sleep
                        //If this funciton return at here, means sim-restore successfully.

#ifdef SIM_DROP_RESTORE_SUPPORT
                        L1API_SetGsmMaxTXPwr_Custom(FALSE,30,23,30,23);//Traffic. GSM DCS GSM850 PCS
                        L1API_SetGsmMaxTXPwr_Custom_Rach(FALSE,30,23,30,23);//RACH.
#endif

                        return TRUE;
                    }
                    else
                    {
                        SCI_FREE_SIGNAL(psig);//free the signal
#if 1//If the hardware has too many problems, then you can use this code, or never. 
                        s_sim_restore_count[cur_sim_task]++;
                        if (s_sim_restore_count[cur_sim_task] < SIM_MAX_RESTORE_COUNT)
                        {
                            //SCI_TRACE_LOW:"SIM%d:restore fail1. %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_3803_112_2_17_22_59_40_4559, (uint8 *)"dd", cur_sim_task, s_sim_restore_count[cur_sim_task]);
                            SIMHAL_PowerOffSIM(exchange_card_id);
                            SCI_Sleep(redemption_period);
                            goto SIM_RESTORE;
                        }
                        else
#endif
                        {
                            //if the initialization is failed, report to the upplayer that the SIM card have serious error
                            break;//quit the loop
                        }
                    }
                }
                else if(psig->SignalCode == SIM_INT_IND)
                {

                    //deactive the restore timer
                    if(SCI_IsTimerActive(s_sim_restore_waiting_timer[cur_sim_task]))
                    {
                        SCI_DeactiveTimer(s_sim_restore_waiting_timer[cur_sim_task]);
                    }

#ifdef SIM_PLUG_IN_SUPPORT
					SCI_TRACE_LOW("SIM:Got INT_TYPE=0x%x in Restore",((SIM_SIG_INT_IND_T*)psig)->int_type);
					if (TB_SIMINT_PULL_OUT == ((SIM_SIG_INT_IND_T*)psig)->int_type)
					{
						SCI_FREE_SIGNAL(psig);//free the signal
						break;//quit the loop
					}
#endif

					SCI_FREE_SIGNAL(psig);//free the signal
					
#if 1//If the hardware has too many problems, then you can use this code, or never. 
                    s_sim_restore_count[cur_sim_task]++;
                    if (s_sim_restore_count[cur_sim_task] < SIM_MAX_RESTORE_COUNT)
                    {
                        //SCI_TRACE_LOW:"SIM%d:restore fail2. %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_3828_112_2_17_22_59_40_4560, (uint8 *)"dd", cur_sim_task, s_sim_restore_count[cur_sim_task]);
                        SIMHAL_PowerOffSIM(exchange_card_id);
                        SCI_Sleep(redemption_period);
                        goto SIM_RESTORE;
                    }
                    else
#endif
                    {
                        //SCI_TRACE_LOW:"SIM%d::the card is not present."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_3836_112_2_17_22_59_40_4561, (uint8 *)"d", cur_sim_task);
                        //report to the upplayer that the SIM card have serious error
                        break;//quit the loop
                    }
                }
                else if(psig->SignalCode == SIM_RESTORE_EXP_IND)
                {
                    SCI_FREE_SIGNAL(psig);//free the signal
#if 1//If the hardware has too many problems, then you can use this code, or never. 
                    s_sim_restore_count[cur_sim_task]++;
                    if (s_sim_restore_count[cur_sim_task] < SIM_MAX_RESTORE_COUNT)
                    {
                        //SCI_TRACE_LOW:"SIM%d:restore failed T.O. %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_3848_112_2_17_22_59_40_4562, (uint8 *)"dd", cur_sim_task, s_sim_restore_count[cur_sim_task]);
                        SIMHAL_PowerOffSIM(exchange_card_id);
                        SCI_Sleep(redemption_period);
                        goto SIM_RESTORE;
                    }
                    else
#endif
                    {
                        //SCI_TRACE_LOW:"SIM%d::the card no response when restore timeout."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_3856_112_2_17_22_59_41_4563, (uint8 *)"d", cur_sim_task);
                        break;//quit the loop
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"SIM%d::receive signal 0x%x during restore."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_3862_112_2_17_22_59_41_4564, (uint8 *)"dd", cur_sim_task, psig->SignalCode);
                    TempServiceReq(cur_sim_task, psig); //add the signal to the temp queque
                }
            }
        }
    }

#endif

#if 0//If the hardware has too many problems, then you can use this code, or NEVER.
{
    uint8 assert_val;
    //in other platform, deal relative case here ...
    OSPARAM_GetAssertMode(&assert_val);
    if (SCI_ASSERT_RELEASE_MODE == assert_val)
    {
        SCI_ASSERT(FALSE);/*assert verified */
    }
    //Reset the mobile phone
}
#endif

#ifdef MODEM_PLATFORM
#ifdef SIM_PLUG_IN_SUPPORT
	{
		xSignalHeaderRec *preq = NULL;
		SIM_SIG_CRSM_CNF_T *pcnf = NULL;

		preq = SIMINSTR_IsThisSIGNALInTempSignalQueue(cur_sim_task,SIM_SIG_CRSM_REQ);
		if (preq)
		{
			SCI_CREATE_SIGNAL(pcnf,SIM_SIG_CRSM_CNF,sizeof(SIM_SIG_CRSM_CNF_T),g_sim_task_id[cur_sim_task]);
			pcnf->file_id = ((SIM_SIG_CRSM_REQ_T*)preq)->file_id;
			pcnf->sw1 = 0x94;
			pcnf->sw2 = 0x4;
			pcnf->data_len = 0;
			pcnf->result = FALSE;
			SCI_TRACE_LOW("SIM%d: Get CRSM from temp queue. file_id=0x%x",cur_sim_task,pcnf->file_id);
			SCI_SEND_SIGNAL((xSignalHeaderRec*)pcnf,g_sim_info_task_id[cur_sim_task]);

			SCI_FREE_SIGNAL(preq);
		}
	}
	
#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK
	if (s_sim_plug_in_by_check_card_id == cur_sim_task)
	{
		if (SCI_IsTimerActive(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id]))
		{
			SCI_DeactiveTimer(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id]);
		}
		SCI_ChangeTimer(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id],SIMMAIN_PLUG_IN_By_Check,s_sim_plug_in_by_check_interval);
		SCI_ActiveTimer(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id]);
	}
#endif
#endif
#endif

#if defined SIM_DROP_RESTORE_SUPPORT && defined SIM_DROP_RESTORE_DELAY_PBSMS_READ
    if (SCI_IsTimerActive(s_sim_delay_smspb_timer[cur_sim_task]))
    {
        SCI_DeactiveTimer(s_sim_delay_smspb_timer[cur_sim_task]);
    }

    L1API_SetGsmMaxTXPwr_Custom(FALSE,30,23,30,23);//FALSE --- Recover the MAX pwr.
    L1API_SetGsmMaxTXPwr_Custom_Rach(FALSE,30,23,30,23);//FALSE --- Recover the MAX pwr.
#endif

    SCI_CREATE_SIGNAL(psig1, SIM_CARD_REMOVE_IND, sizeof(SIM_SIG_CARD_REMOVE_IND_T), g_sim_task_id[cur_sim_task]);
    SCI_SEND_SIGNAL(psig1, g_sim_info_task_id[cur_sim_task]);
    SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);//Clear the temp int queue because the reset is coming.
    PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF); //power off the sim card
    EnableDeepSleep(cur_sim_task);//enable arm deep sleep
    return FALSE;
}

/********************************************************************
//    DESCRIPTION
//      the function that Deal with the Card in Interrupt from SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void DealCardIn(uint32 cur_sim_task)
{
#ifdef SIM_TEST_IN_WIN32
    xSignalHeaderRec *ind = NULL;
#endif
    uint32 exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);

    if(SIM_NULL_STATE == g_sim_card_state[cur_sim_task])
    {
        SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_ATR_STATE, "SIM ATR State", g_sim_task_id[cur_sim_task]);
        g_sim_card_setting[cur_sim_task].card_present = TRUE;
#ifdef SIM_TEST_IN_WIN32
#ifdef SIM_SIMULATOR
        SCI_CREATE_SIGNAL(ind, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), g_sim_task_id[cur_sim_task]);
        ((SIM_SIG_INT_IND_T *)ind)->int_type = TB_SIMINT_CARD_IN;
        SCI_SEND_SIGNAL(ind, g_sim_info_task_id[cur_sim_task]);
#else
        SCI_CREATE_SIGNAL(ind, SIM_CARD_INSERTED_IND, sizeof(SIM_SIG_CARD_INSERTED_IND_T), g_sim_task_id[cur_sim_task]);
        ((SIM_SIG_CARD_INSERTED_IND_T *)ind)->insert_status = TB_SIM_INSERTED_OK;
        ((SIM_SIG_CARD_INSERTED_IND_T *)ind)->t_protocol =  TB_SIM_T_0_PROTOCOL;

        SCI_SEND_SIGNAL(ind, g_sim_task_id[cur_sim_task]);
#endif
#else
        SIMHAL_ResetSIMCard(FALSE, exchange_card_id, FALSE);
#endif
    }
    else
    {
        SCI_TRACE_HIGH("SIM::Error SIM state");/*assert verified */
    }
}

/********************************************************************
//    DESCRIPTION
//      the function that Deal with the Card out Interrupt from SIM card
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void DealCardOut(uint32 cur_sim_task)
{
    xSignalHeaderRec *ind = NULL;
    uint32 exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);

#ifdef SIM_PLUG_IN_SUPPORT

    if(s_sim_plug_in_flag[cur_sim_task])
    {
        SIMMAIN_IsSIMPlugIn(cur_sim_task, FALSE);
        return;
    }

#endif

#ifndef SIM_TEST_IN_WIN32

    //if ((s_sim_cardout_reset_times[current_sim_card] < 2) && (!s_sim_is_ext_dualsim))//rocky power on will reset 2 times
    if(s_sim_cardout_reset_times[cur_sim_task] < 1)
    {
        //This is a special operation. We get a Liantong SIM card, it will send ATR data out after poweron and clk enable,
        //do not wait the RST high. Our rocky and 68 can use the reset make it work well.
        s_sim_cardout_reset_times[cur_sim_task]++;
        //SCI_TRACE_LOW:"SIM%d:cardout %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_3956_112_2_17_22_59_41_4565, (uint8 *)"dd", cur_sim_task, s_sim_cardout_reset_times[cur_sim_task]);
#ifdef MODEM_PLATFORM
        SIMHAL_PowerOffSIM(exchange_card_id); //power off the sim card
#else
        PowerOffSIM(cur_sim_task, SIM_ABNORMAL_POWER_OFF); //power off the sim card
#endif
        SCI_Sleep(60);
        SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);//Clear the temp int queue because the reset is coming.
        SIMHAL_ResetDriver(FALSE, exchange_card_id);
        SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_ATR_STATE, "SIM ATR State", g_sim_task_id[cur_sim_task]);
        SCI_Sleep((s_sim_ldo_stable_time + 2)); //wait the SIM LDO stable
        SIMHAL_ResetSIMCard(FALSE, exchange_card_id, FALSE);
        g_sim_card_setting[cur_sim_task].card_present = TRUE;
        return;
    }
    else
#endif
    {
        SCI_CREATE_SIGNAL(ind, SIM_CARD_REMOVE_IND, sizeof(SIM_SIG_CARD_REMOVE_IND_T), g_sim_task_id[cur_sim_task]);
        SCI_SEND_SIGNAL(ind, g_sim_info_task_id[cur_sim_task]);

        g_sim_card_setting[cur_sim_task].card_present = FALSE;
        SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);

        //reset the SIM variable
        SIMMAIN_ResetSIMVariable(cur_sim_task);

        PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF);
        EnableDeepSleep(cur_sim_task);
    }
}

/********************************************************************
//    DESCRIPTION
//      the function that Deal with the RX Parity Error exception from
//    the SIM driver
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void DealRxParityError(uint32 cur_sim_task)
{
    xSignalHeaderRec *psig = NULL;

#ifndef WIN32
    SIM_PrintTxBuf(cur_sim_task);
    SIM_PrintRxBuf(cur_sim_task);
#endif

    //SCI_TRACE_LOW:"SIM%d:RX Parity Error"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_4000_112_2_17_22_59_41_4566, (uint8 *)"d", cur_sim_task);

#ifdef SIM_PLUG_IN_SUPPORT

    if(s_sim_plug_in_flag[cur_sim_task])
    {
        SIMMAIN_IsSIMPlugIn(cur_sim_task, FALSE);
        return;
    }

#endif

    if((SIM_NO_POWER_STATE == g_sim_card_state[cur_sim_task])
        || (SIM_NULL_STATE == g_sim_card_state[cur_sim_task]))
    {
        PowerOffSIM(cur_sim_task, SIM_ABNORMAL_POWER_OFF);
        return;
    }

    //creat the serious error indicator signal to indicate the upper layer the serious error
    SCI_CREATE_SIGNAL(psig, SIM_SERIOUS_ERROR_IND, sizeof(SIM_SIG_SERIOUS_ERROR_IND_T), g_sim_task_id[cur_sim_task]);
    SCI_SEND_SIGNAL(psig, g_sim_info_task_id[cur_sim_task]);
    //power off the SIM card
    PowerOffSIM(cur_sim_task, SIM_ABNORMAL_POWER_OFF);
}

/********************************************************************
//    DESCRIPTION
//      the function that Deal with the TX Parity Error exception from
//    the SIM driver
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void DealTxParityError(uint32 cur_sim_task)
{
    xSignalHeaderRec *psig = NULL;

#ifndef WIN32
    SIM_PrintTxBuf(cur_sim_task);
    SIM_PrintRxBuf(cur_sim_task);
#endif

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM%d:TX Parity Error"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_4041_112_2_17_22_59_41_4567, (uint8 *)"d", cur_sim_task);
#endif

#ifdef SIM_PLUG_IN_SUPPORT

    if(s_sim_plug_in_flag[cur_sim_task])
    {
        SIMMAIN_IsSIMPlugIn(cur_sim_task, FALSE);
        return;
    }

#endif

    if((SIM_NO_POWER_STATE == g_sim_card_state[cur_sim_task])
        || (SIM_NULL_STATE == g_sim_card_state[cur_sim_task]))
    {
        PowerOffSIM(cur_sim_task, SIM_ABNORMAL_POWER_OFF);
        return;
    }

    //creat the serious error indicator signal to indicate the upper layer the serious error
    SCI_CREATE_SIGNAL(psig, SIM_SERIOUS_ERROR_IND, sizeof(SIM_SIG_SERIOUS_ERROR_IND_T), g_sim_task_id[cur_sim_task]);
    SCI_SEND_SIGNAL(psig, g_sim_info_task_id[cur_sim_task]);
    //power off the SIM card
    PowerOffSIM(cur_sim_task, SIM_ABNORMAL_POWER_OFF);
}

#ifdef SIM_PLUG_IN_SUPPORT
/********************************************************************
//    DESCRIPTION
//      the function that Deal with the Card in Interrupt from driver
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC void DealSIMPlugIn(uint32 cur_sim_task)
{
    xSignalHeaderRec *psig = NULL;

    if(SIM_ACTIVE_STATE == g_sim_card_state[cur_sim_task])
    {
        //If we get a plug in in Active State, this is a wrong signal.
        return;
    }

#if 0 //MODEM_PLATFORM//Notify AP directly.
    if (SIM_ATR_STATE == g_sim_card_state[cur_sim_task])
    {
        //We should response to APP layer(SFUN=2) before a HOT_PLUG_IN
        SCI_TRACE_LOW("SIM%d: ATR_State Got Plug-In",cur_sim_task);
        SCI_CREATE_SIGNAL(psig, SIM_CARD_PRESENT_IND, sizeof(SIM_SIG_CARD_PRESENT_IND_T), g_sim_task_id[cur_sim_task]);
        ((SIM_SIG_CARD_PRESENT_IND_T *)psig)->insert_status = SIM_INSERTED_WRONG_SIM;
        SCI_SEND_SIGNAL(psig, g_sim_info_task_id[cur_sim_task]);
    }
    SCI_CREATE_SIGNAL(psig, SIM_HOT_PLUG_IN_IND, sizeof(SIM_SIG_HOT_PLUG_IN_IND_T), g_sim_task_id[cur_sim_task]);
    SCI_SEND_SIGNAL(psig, g_sim_info_task_id[cur_sim_task]);
#else
    if(s_sim_plug_in_flag[cur_sim_task])
    {
        return;
    }

    s_sim_plug_in_flag[cur_sim_task] = TRUE;
    SCI_CREATE_SIGNAL(psig, SIM_START_UP_REQ, sizeof(xSignalHeaderRec), g_sim_task_id[cur_sim_task]);
    SCI_SEND_SIGNAL(psig, g_sim_task_id[cur_sim_task]);
#endif

    return;
}

/********************************************************************
//    DESCRIPTION
//      the function that Deal with the Card in Interrupt from driver
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC void DealSIMPullOut(uint32 cur_sim_task)
{
    xSignalHeaderRec *ind = NULL;

    if(s_sim_plug_in_flag[cur_sim_task])
    {
        SIMMAIN_IsSIMPlugIn(cur_sim_task, FALSE);
        return;
    }

#ifdef MODEM_PLATFORM
	{
		xSignalHeaderRec *preq = NULL;
		SIM_SIG_CRSM_CNF_T *pcnf = NULL;

		preq = SIMINSTR_IsThisSIGNALInTempSignalQueue(cur_sim_task,SIM_SIG_CRSM_REQ);
		if (preq)
		{
			SCI_CREATE_SIGNAL(pcnf,SIM_SIG_CRSM_CNF,sizeof(SIM_SIG_CRSM_CNF_T),g_sim_task_id[cur_sim_task]);
			pcnf->file_id = ((SIM_SIG_CRSM_REQ_T*)preq)->file_id;
			pcnf->sw1 = 0x94;
			pcnf->sw2 = 0x4;
			pcnf->data_len = 0;
			pcnf->result = FALSE;
			SCI_TRACE_LOW("SIM%d: Get CRSM from temp queue. file_id=0x%x",cur_sim_task,pcnf->file_id);
			SCI_SEND_SIGNAL((xSignalHeaderRec*)pcnf,g_sim_info_task_id[cur_sim_task]);

			SCI_FREE_SIGNAL(preq);
		}
	}
#endif

    SCI_CREATE_SIGNAL(ind, SIM_CARD_REMOVE_IND, sizeof(SIM_SIG_CARD_REMOVE_IND_T), g_sim_task_id[cur_sim_task]);
    SCI_SEND_SIGNAL(ind, g_sim_info_task_id[cur_sim_task]);

    g_sim_card_setting[cur_sim_task].card_present = FALSE;
    SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);

    //reset the SIM variable
    SIMMAIN_ResetSIMVariable(cur_sim_task);

    PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF);
    EnableDeepSleep(cur_sim_task);

    return;
}
#endif//SIM_PLUG_IN_SUPPORT

#ifdef SIM_PLUG_IN_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK
/********************************************************************
//    DESCRIPTION
//      this fuction do the service of the query on the card status
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC void QueryCardStatus(uint32 cur_sim_task)
{
	static BOOLEAN select_flag = TRUE;
	SIM_RESPONSE_DATA_OF_SELECT_T select_response = {0};
	SIM_FILE_NAME_E file_name = SIM_FILE_MF;
	SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_ERROR_UNKNOW;

	SCI_TRACE_LOW("SIM%d:QueryCardStatus %d",cur_sim_task,g_sim_card_state[cur_sim_task]);

	if (SIM_ACTIVE_STATE == g_sim_card_state[s_sim_plug_in_by_check_card_id])
	{
		if (select_flag)
		{
			file_name = SIM_FILE_MF;
			select_flag = FALSE;
		}
		else
		{
			file_name = SIM_FILE_DF_GSM;
			select_flag = TRUE;
		}
		ser_result = SIMSER_SelectFile(cur_sim_task, file_name, &select_response);

        if((SIM_SERVICE_RX_PARITY_ERROR == ser_result)|| 
            (SIM_SERVICE_TX_PARITY_ERROR == ser_result)||
            (SIM_SERVICE_CARD_OUT == ser_result)||
            (SIM_SERVICE_CARD_UNRESPONSE == ser_result))
		{
		    xSignalHeaderRec *psig = NULL;
		    SCI_CREATE_SIGNAL(psig, SIM_CARD_REMOVE_IND, sizeof(SIM_SIG_CARD_REMOVE_IND_T), g_sim_task_id[cur_sim_task]);
            SCI_SEND_SIGNAL(psig, g_sim_info_task_id[cur_sim_task]);
            SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);//Clear the temp int queue because the reset is coming.
            PowerOffSIM(cur_sim_task, SIM_NORMAL_POWER_OFF); //power off the sim card
            EnableDeepSleep(cur_sim_task);//enable arm deep sleep
		}
		
		if (SCI_IsTimerActive(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id]))
		{
			SCI_DeactiveTimer(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id]);
		}
		SCI_ChangeTimer(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id],SIMMAIN_PLUG_IN_By_Check,s_sim_plug_in_by_check_interval);
		SCI_ActiveTimer(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id]);
	}
	else
	{
		DealSIMPlugIn(s_sim_plug_in_by_check_card_id);
	}
}
#endif
#endif

/********************************************************************
//    DESCRIPTION
//      soft power off the sim card when upper indicate that the mobile
//    has been power off
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void PowerOffSIM(uint32 cur_sim_task, SIM_POWER_OFF_TYPE_E type)
{
    xSignalHeaderRec *psig = NULL;
	xSignalHeaderRec *psig1 = NULL;
    xSignalHeaderRec *pstartup = NULL;
	xSignalHeaderRec *preq = NULL;
    BOOLEAN is_start_in_queue = FALSE;

#ifndef SIM_TEST_IN_WIN32
    uint32 exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);
#endif

#ifndef SIM_TEST_IN_WIN32
    SIMHAL_PowerOffSIM(exchange_card_id);
#endif

    //If there has a SIM_START_UP_REQ in queue, we should do it after SIM_POWER_OFF_REQ
	preq = SIMINSTR_IsThisSIGNALInTempSignalQueue(cur_sim_task, SIM_START_UP_REQ);
    if(preq)
    {
        is_start_in_queue = TRUE;
		SCI_FREE_SIGNAL(preq);
    }

    SIMMAIN_ResetSIMVariable(cur_sim_task);
    SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NO_POWER_STATE, "SIM NO POWER State", g_sim_task_id[cur_sim_task]);

    SCI_CREATE_SIGNAL(psig, SIM_POWER_OFF_CNF, sizeof(SIM_SIG_POWER_OFF_CNF_T), g_sim_task_id[cur_sim_task]);
    SCI_SEND_SIGNAL((xSignalHeaderRec *)psig, g_sim_simat_task_id[cur_sim_task]);

    //send signal to notify STK layer stopping the polling timer
    SCI_CREATE_SIGNAL(psig1, SIMAT_POWER_OFF_IND, sizeof(SIM_SIG_SIMAT_POWER_OFF_IND_T), g_sim_task_id[cur_sim_task]);
    SCI_SEND_SIGNAL((xSignalHeaderRec *)psig1, g_sim_simat_task_id[cur_sim_task]);

    if(is_start_in_queue)
    {
        SCI_CREATE_SIGNAL(pstartup, SIM_START_UP_REQ, sizeof(SIM_SIG_SIMAT_POWER_OFF_IND_T), g_sim_task_id[cur_sim_task]);
        SIMINSTR_AddThisSIGNALInTempSignalQueue(cur_sim_task, pstartup);
    }
}

/********************************************************************
//    DESCRIPTION
//      start to check card present present periodly
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void StartCardPresentCheck(uint32 cur_sim_task)
{
    //We should make the 1st STATUS quickly.
    s_sim_sim_poll_time[cur_sim_task] = SIM_CARD_PRESENT_CHECK_FREQUENCY_CALL - 15000;

    if((s_sim_call_poll_time[cur_sim_task] == 0) &&
        (s_sim_stk_poll_time[cur_sim_task] == 0))
    {
        SIMMAIN_StartCardPresentTimer(cur_sim_task);
    }

    //Then the consecutive STATUS time interval can be more slowly
    s_sim_sim_poll_time[cur_sim_task] = SIM_CARD_PRESENT_CHECK_FREQUENCY_IDLE;
}

/********************************************************************
//    DESCRIPTION
//      start or stop checking the card present according to the ME call state.
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void SwitchCheckCardPresent(uint32 cur_sim_task, SIM_SIG_SWITCH_CHECK_CARD_PRES_REQ_T *psig)
{
    //if is ME is in call status,begin to check the SIM Card every 25s
    if(psig->call_state)
    {
        s_sim_call_poll_time[cur_sim_task] = SIM_CARD_PRESENT_CHECK_FREQUENCY_CALL;
        //SCI_TRACE_LOW:"SIM%d::ME is in call,begin to check SIM card every %d ms."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_4252_112_2_17_22_59_41_4568, (uint8 *)"dd", cur_sim_task, s_sim_call_poll_time[cur_sim_task]);

#if defined SIM_DROP_RESTORE_SUPPORT && defined SIM_DROP_RESTORE_NO_SELF_CHECK
        SCI_TRACE_LOW("SIM No check in call");
#else
        //start to check the card present
        CheckCardPresent(cur_sim_task);//wuding 2013-09-26 for DSWY
#endif
    }
    else  //if the call is terminated,then adjust the check timer to Idle check time
    {
        s_sim_call_poll_time[cur_sim_task] = 0;

        SIMMAIN_StartCardPresentTimer(cur_sim_task);
    }
}

/********************************************************************
//    DESCRIPTION
//      the sim present check period tiemr expiration function,send a
//    card present check indication to the sim layer itself
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void SendCardPresentCheckInd(
    uint32 lParam   //the parameter of the timer function
)
{
    xSignalHeaderRec *psig = NULL;

    if(SIM_ACTIVE_STATE == g_sim_card_state[lParam])
    {
        SCI_CREATE_SIGNAL(psig, SIM_PRESENT_CHECK_IND, sizeof(SIM_SIG_PRESENT_CHECK_IND_T), g_sim_task_id[lParam]);
        SCI_SEND_SIGNAL(psig, g_sim_task_id[lParam]);
    }
}

/********************************************************************
//    DESCRIPTION
//      check the card present by the STATUS command
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void CheckCardPresent(uint32 cur_sim_task)
{
    uint16 file_id;
    xSignalHeaderRec *psig;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    BOOLEAN fetch_flag = FALSE;
#ifndef SIM_TEST_IN_WIN32
#ifdef SIM_SELF_TEST_CONTINUOUS_READ_WRITE
    RAND_T alg_rand = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
    SIM_GSM_ALGORITHM_RESULT_T alg_result;
    SIM_EF_MAPPING_KC_T kc_mapping;
#endif
#endif

    //SCI_TRACE_LOW:"SIM%d::check card present."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_4303_112_2_17_22_59_41_4569, (uint8 *)"d", cur_sim_task);

#ifdef SIM_FILL_PHONEBOOK_FULL//test func when do phonebook debug
    if(SIM_GetWritePhonebookFull(cur_sim_task))
    {
        SIM_WritePhonebookFull(cur_sim_task);
        
        //Next STATUS check
		SIMMAIN_StartCardPresentTimer(cur_sim_task);
        return;
    }
#endif
    
    //get the current direcotry file before the new status command
    file_id = SIMFS_GetFileID(cur_sim_task, SIMFS_GetCurDirectoryFile(cur_sim_task));

    //get the current DF information
    ser_result = SIMINSTR_Status(cur_sim_task, &select_data, SIM_SELECT_FILE_TYPE_POS,&fetch_flag);

	if(ser_result == SIM_SERVICE_SELECT_FILE_ERROR)
    {
        if (TRUE == fetch_flag)
        {
            SCI_TRACE_LOW("SIM%d::check card failed for card return only 91 xx, without any data!",cur_sim_task);
            SIMMAIN_StartCardPresentTimer(cur_sim_task);
            return;
        }
    }
	
    if(SIM_SERVICE_OK == ser_result)
    {
        //if the status return information is not the GSM file's s
        //see the fta case 27.20,the ME terminates a call within 5 seconds at the latest after having received a wrong response to
        //the STATUS command.
        if(select_data.file_id != file_id)
        {
			SCI_TRACE_LOW("SIM%d::Card not present because the current directory is 0x%x! 0x%x",cur_sim_task,select_data.file_id,file_id);

            if (TRUE == fetch_flag)
            {
                SIMMAIN_StartCardPresentTimer(cur_sim_task);
                return;
            }

#ifdef SIM_3G_USIM_SUPPORT
            if ((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task]) 
				&& (0x7F10 == select_data.file_id) 
				&& (0x5F3A == file_id))
            {
            	//Special Globe SIM ...
            	//We go back to MF.
				SIM_RESPONSE_DATA_OF_SELECT_T select_response;
				SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &select_response);

				//Next STATUS check
				SIMMAIN_StartCardPresentTimer(cur_sim_task);
            }
			else
#endif
			{
            //report to the upplayer that the SIM card have serious error
            SCI_CREATE_SIGNAL(psig, SIM_CARD_REMOVE_IND, sizeof(SIM_SIG_CARD_REMOVE_IND_T), g_sim_task_id[cur_sim_task]);
            SCI_SEND_SIGNAL(psig, g_sim_info_task_id[cur_sim_task]);
#ifndef WIN32
            SIM_PrintTxBuf(cur_sim_task);
            SIM_PrintRxBuf(cur_sim_task);
#endif
            //power off the sim card
            PowerOffSIM(cur_sim_task, SIM_ABNORMAL_POWER_OFF);
            EnableDeepSleep(cur_sim_task);
            }
			
        }
        else
        {
            //This is only for SIM developer's self-test, shoule NEVER be used in release code.
#ifndef SIM_TEST_IN_WIN32
#ifdef SIM_SELF_TEST_CONTINUOUS_READ_WRITE
            SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_KC, &kc_mapping);
            SIMINSTR_RunGSMAlgorithm(cur_sim_task, alg_rand, &alg_result);
            s_sim_sim_poll_time[cur_sim_task] = 5000;// 1s is the interval between 2 operation of a sim card
#endif
#endif
            //This is only for SIM developer's self-test, shoule NEVER be used in release code.
            //reactive the card present check period timer to make the next card present check
            SIMMAIN_StartCardPresentTimer(cur_sim_task);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"SIM%d::CheckCardPresent,Card not present because the Status instruction is not ok\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_4346_112_2_17_22_59_41_4571, (uint8 *)"d", cur_sim_task);
#ifndef WIN32
        SIM_PrintTxBuf(cur_sim_task);
        SIM_PrintRxBuf(cur_sim_task);
#endif
    }
}

/********************************************************************
//    DESCRIPTION
//      the sim card not presnet,the sim card present judge timer expiration
//    function
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void CardNotPresent(
    uint32 lParam   //the parameter of the timer function
)
{
    xSignalHeaderRec *psig = NULL;

    //tell the sim layer that the sim card is not present in the same way that
    //sim driver do
    SCI_CREATE_SIGNAL(psig, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), g_sim_task_id[lParam]);
    ((SIM_SIG_INT_IND_T *)psig)->int_type = TB_SIMINT_CARD_UNRESPONSE;
    SCI_SEND_SIGNAL(psig, g_sim_task_id[lParam]);
    //SCI_TRACE_LOW:"SIM%d:Card unresponse!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_4372_112_2_17_22_59_42_4572, (uint8 *)"d", lParam);
}

/********************************************************************
//    DESCRIPTION
//      get the gsm data time out
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void GetGSMDataTimeOut(
    uint32 lParam   //the parameter of the timer function
)
{
    xSignalHeaderRec *psig = NULL;
    uint32 exchange_card_id = 0;

#ifndef WIN32
    //if the 5s timer out,judge wether the card has send some response,if the card sent
    //response,may be the card is timed-out for the 9600etu,the ME should wait until the
    //work-waiting-time timer expired,else the ME should not wait,and should reject the
    //card whithin 5s.see detail case 27.11.1.3c and 27.17.1.5
    exchange_card_id = SIM_Exchange_Card_Slot_Label(lParam);

    if(SIMHAL_IsCardNotPresent(exchange_card_id))
    {
        //tell the sim layer that the sim card is not present in the same way that
        //sim driver do
        SCI_CREATE_SIGNAL(psig, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), g_sim_task_id[lParam]);
        ((SIM_SIG_INT_IND_T *)psig)->int_type = TB_SIMINT_CARD_UNRESPONSE;
        SCI_SEND_SIGNAL(psig, g_sim_task_id[lParam]);

        //SCI_TRACE_LOW:"SIM%d:Card give no any response during get the gsm data in 3s!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_4402_112_2_17_22_59_42_4573, (uint8 *)"d", lParam);
    }
    else
    {
        //SCI_TRACE_LOW:"SIM%d:Card has give part response,wait until 100s timer expired!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_4406_112_2_17_22_59_42_4574, (uint8 *)"d", lParam);
    }

#else
    //tell the sim layer that the sim card is not present in the same way that
    //sim driver do
    SCI_CREATE_SIGNAL(psig, SIM_INT_IND, sizeof(SIM_SIG_INT_IND_T), g_sim_task_id[lParam]);
    ((SIM_SIG_INT_IND_T *)psig)->int_type = TB_SIMINT_CARD_UNRESPONSE;
    SCI_SEND_SIGNAL(psig, g_sim_task_id[lParam]);

    //SCI_TRACE_LOW:"SIM:Card give no any response during get the gsm data in 3s!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_4415_112_2_17_22_59_42_4575, (uint8 *)"");
#endif
}

/********************************************************************
//    DESCRIPTION:Check if g_sim_card_state is SIM_INITIALIZE_STATE or
//                SIM_ACTIVE_STATE.
//    Global resource dependence :
//    Author:hanjun.liu
//    Note:
***********************************************************************/
uint8 SIM_IsSimInitialized(uint32 cur_sim_task)
{
    uint8 return_value = 0;

    return_value = ((SIM_INITIALIZE_STATE == g_sim_card_state[cur_sim_task])
                    || (SIM_ACTIVE_STATE == g_sim_card_state[cur_sim_task]))
                   ?  1 : 0;
    return return_value;
}

/**********************************************************************************************
//    DESCRIPTION
//      Disable the deep sleep.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************************************/
void DisableDeepSleep(uint32 cur_sim_task)
{
#ifndef WIN32
    SCI_SIM_EnableDeepSleep(DISABLE_DEEP_SLEEP);
    //SCI_TRACE_LOW("SIM::disable deep sleep.");
#endif
    s_sim_arm_sleep[cur_sim_task] = FALSE;
}

/**********************************************************************************************
//    DESCRIPTION
//      Enable the deep sleep.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************************************/
STATIC void EnableDeepSleep(uint32 lParam)
{
    uint32 other_sim_task = 1;
#ifdef MULTI_SIM_SYS_TRI
    uint32 third_sim_task = 2;
#elif defined (MULTI_SIM_SYS_QUAD)
    uint32 third_sim_task = 2;
    uint32 fourth_sim_task = 3;
#endif
    uint32 cur_sim_task = lParam;

    if(1 == cur_sim_task)
    {
        other_sim_task = 0;
#ifdef MULTI_SIM_SYS_TRI
        third_sim_task = 2;
#endif
#ifdef MULTI_SIM_SYS_QUAD
        fourth_sim_task = 3;
#endif
    }

#ifdef MULTI_SIM_SYS_TRI
    else if(2 == cur_sim_task)
    {
        other_sim_task = 0;
        third_sim_task = 1;
    }

#elif defined (MULTI_SIM_SYS_QUAD)
    else if(2 == cur_sim_task)
    {
        other_sim_task = 0;
        third_sim_task = 1;
        fourth_sim_task = 3;
    }
    else if(3 == cur_sim_task)
    {
        other_sim_task = 0;
        third_sim_task = 1;
        fourth_sim_task = 2;
    }

#endif

#ifndef WIN32
#ifdef MULTI_SIM_SYS_TRI

    if(s_sim_arm_sleep[other_sim_task]
        && s_sim_arm_sleep[third_sim_task])
#elif defined (MULTI_SIM_SYS_QUAD)
    if(s_sim_arm_sleep[other_sim_task]
        && s_sim_arm_sleep[third_sim_task]
        && s_sim_arm_sleep[fourth_sim_task])
#else
    if(s_sim_arm_sleep[other_sim_task])
#endif
    {
        //SCI_TRACE_LOW("SIM::enable deep sleeep.");
        SCI_SIM_EnableDeepSleep(ENABLE_DEEP_SLEEP);
    }

#endif
    s_sim_arm_sleep[cur_sim_task] = TRUE;
}

/**********************************************************************************************
//    DESCRIPTION
//      Active the instruction timer,while sending the command to the card,the timer
//    is actived at the same time.the timer is used to prevent the SIM card has no any response
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************************************/
void ActiveInstructionTimer(uint32 cur_sim_task, uint32 total_time_value)
{
    uint32 temp_total_time_value = total_time_value;

    if(s_b_sim_present_timer[cur_sim_task])
    {
        if(SCI_IsTimerActive(s_sim_present_timer[cur_sim_task]))
        {
            SCI_DeactiveTimer(s_sim_present_timer[cur_sim_task]);
        }

        if(0 == temp_total_time_value) //double check
        {
            temp_total_time_value = SIM_CARD_PRESENT_TIME_LIMIT;
        }

        SCI_ChangeTimer(s_sim_present_timer[cur_sim_task], CardNotPresent, temp_total_time_value);
        SCI_ActiveTimer(s_sim_present_timer[cur_sim_task]);

        SCI_TRACE_LOW("SIM%d:ActiveInstructionTimer %d ms", cur_sim_task, temp_total_time_value);
    }
}

/**********************************************************************************************
//    DESCRIPTION
//      Dective the instruction timer,while has received the response data from the SIM card,
//    the timer is deactived at the same time.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************************************/
void DeactiveInstructionTimer(uint32 cur_sim_task)
{
    if(SCI_IsTimerActive(s_sim_present_timer[cur_sim_task]))
    {
        SCI_DeactiveTimer(s_sim_present_timer[cur_sim_task]);
    }
}

/**********************************************************************************************
//    DESCRIPTION
//    To judge the Present Timer is Active or not
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************************************/
BOOLEAN IsInstructionTimerActive(uint32 cur_sim_task)
{
    if(SCI_IsTimerActive(s_sim_present_timer[cur_sim_task]))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************************************
//    DESCRIPTION
//      Dective the instruction timer,while has received the response data from the SIM card,
//    the timer is deactived at the same time.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************************************/
BOOLEAN SIM_IsCallControlSupport(uint8 cur_sim_task)
{
    BOOLEAN result = TRUE;

    //if the card is stk card and the call control is allocated and actived.
    if((SIM_PH_2_PD == g_sim_card_setting[cur_sim_task].card_phase)
        && (SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CALL_CON)))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM_IsCallControlSupport result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_4592_112_2_17_22_59_42_4576, (uint8 *)"d", result);
#endif
    return (result);
}

/********************************************************************
//    DESCRIPTION
//      the sim idle timer expiration function,send a clock stop
//    indication to the sim layer itself
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void IdleCard(
    uint32 lParam   //the parameter of the timer function
)
{
    xSignalHeaderRec *psig = NULL;

    if(SIM_ACTIVE_STATE == g_sim_card_state[lParam])
    {
        SCI_CREATE_SIGNAL(psig, SIM_CLOCK_STOP_IND, sizeof(SIM_SIG_CLOCK_STOP_IND_T), g_sim_task_id[lParam]);
        SCI_SEND_SIGNAL(psig, g_sim_task_id[lParam]);
    }
}

/********************************************************************
//    DESCRIPTION
//      To check a CPHS optional service is actived and allocated.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIM_IsCPHSOptionalSupport(uint32 cur_sim_task, SIM_FILE_NAME_E cphs_file_name)
{
    BOOLEAN ret = FALSE;

    if((SIM_FILE_EF_ONSTRING == cphs_file_name) || (SIM_FILE_EF_ONSTHORT == cphs_file_name) ||
        (SIM_FILE_EF_CPHSINFO == cphs_file_name) || (SIM_FILE_EF_VMWFLAG == cphs_file_name) ||
        (SIM_FILE_EF_CFFLAG == cphs_file_name) || (SIM_FILE_EF_CSP == cphs_file_name) ||
        (SIM_FILE_EF_MBNUM == cphs_file_name)
#ifdef _CPHS_SUPPORT
        || (SIM_FILE_EF_INFONUM == cphs_file_name) || (SIM_FILE_EF_CPHS_SST == cphs_file_name)
        || (SIM_FILE_EF_ORANGE_DYN2_FLAG == cphs_file_name) || (SIM_FILE_EF_ORANGE_CSP_LINE2 == cphs_file_name)
        || (SIM_FILE_EF_ORANGE_ACM2 == cphs_file_name) || (SIM_FILE_EF_ORANGE_DYN_FLAG == cphs_file_name)
#endif
      )
    {
        // There is no CPHS on this SIM, return FALSE.
        if(g_sim_cphsinfo[cur_sim_task].cphs_optional_flag)
        {
            //There has CPHS on this SIM, check wether it have the optional EFs.
            switch(cphs_file_name)
            {
                case SIM_FILE_EF_ONSTHORT:
                    {
                        if((3 == g_sim_cphsinfo[cur_sim_task].cphsoptionalinfo.cphs_service_table.opnameshort)
                            && (PHASE2 == g_sim_cphsinfo[cur_sim_task].cphsoptionalinfo.cphs_phase))
                        {
                            ret = TRUE;
                        }
                        else
                        {
                            ret = FALSE;
                        }
                    }
                    break;
                case SIM_FILE_EF_MBNUM:
#ifdef SIM_3G_USIM_SUPPORT

                    if((g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_MDN].allocated)
                        && (g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_MDN].activated))
                    {
                        ret = TRUE;
                    }
                    else
#endif
                    {
                        if(3 == g_sim_cphsinfo[cur_sim_task].cphsoptionalinfo.cphs_service_table.mailboxnum)
                        {
                            ret = TRUE;
                        }
                        else
                        {
                            ret = FALSE;
                        }

                    }

                    break;
                case SIM_FILE_EF_CSP:
                    {
                        if(3 == g_sim_cphsinfo[cur_sim_task].cphsoptionalinfo.cphs_service_table.csp)
                        {
                            ret = TRUE;
                        }
                        else
                        {
                            ret = FALSE;
                        }
                    }
                    break;

#ifdef _CPHS_SUPPORT
                case SIM_FILE_EF_CPHS_SST://phase1
                    {
                        if(3 == g_sim_cphsinfo[cur_sim_task].cphsoptionalinfo.cphs_service_table.sst)
                        {
                            ret = TRUE;
                        }
                        else
                        {
                            ret = FALSE;
                        }
                    }
                    break;
                case SIM_FILE_EF_INFONUM:
                    {
                        if(3 == g_sim_cphsinfo[cur_sim_task].cphsoptionalinfo.cphs_service_table.infonum)
                        {
                            ret = TRUE;
                        }
                        else
                        {
                            ret = FALSE;
                        }
                    }
                    break;
                case SIM_FILE_EF_ORANGE_DYN2_FLAG:
                case SIM_FILE_EF_ORANGE_CSP_LINE2:
                case SIM_FILE_EF_ORANGE_ACM2:
                case SIM_FILE_EF_ORANGE_DYN_FLAG:

                    if(s_sim_df_orange_exist[cur_sim_task])
                    {
                        ret = TRUE;
                    }
                    else
                    {
                        ret = FALSE;
                    }

                    break;
#endif
                default:
                    ret = TRUE;
                    break;
            }
        }

#ifdef _CPHS_SUPPORT
        else if(s_sim_df_orange_exist[cur_sim_task])
        {
            switch(cphs_file_name)
            {
                case SIM_FILE_EF_ORANGE_DYN2_FLAG:
                case SIM_FILE_EF_ORANGE_CSP_LINE2:
                case SIM_FILE_EF_ORANGE_ACM2:
                case SIM_FILE_EF_ORANGE_DYN_FLAG:
                    ret = TRUE;
                    break;
                default:
                    ret = FALSE;
                    break;
            }
        }

#endif
        else
        {
            ret = FALSE;
        }
    }
    else
    {
        //other norrmal sim EFs return TRUE
        ret = TRUE;
    }

    return ret;
}

#ifndef SIM_TEST_IN_WIN32
/**********************************************************************
//    Description:
//     Reject the SIM card, deactive the SIM card and Power off the SIM card.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void Dualsim_RejectSimCard(uint32 cur_sim_task)
{
    uint32 exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);

    SIMHAL_RejectSIMCard(exchange_card_id);

    return;
}
#endif

/********************************************************************
//    DESCRIPTION
//      the sim restore failed,the sim driver has no response, the restore waiting timer
//    expired
//    function
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
static void RestoreExpired(
    uint32 lParam   //the parameter of the timer function
)
{
    xSignalHeaderRec *psig = NULL;

    //tell the sim layer that the sim restore is failed
    SCI_CREATE_SIGNAL(psig, SIM_RESTORE_EXP_IND, sizeof(SIM_SIG_RESTORE_EXP_IND_T), g_sim_task_id[lParam]);
    SCI_SEND_SIGNAL(psig, g_sim_task_id[lParam]);
}

#ifdef SIM_3G_USIM_SUPPORT
/********************************************************************
//    DESCRIPTION
//     To judge the SIM is 2G or 3G sim card after power on. Return FALSE, means
//      this card cannot init as 3G mode, we should COLD RESET and change into
//      another mode. TRUE means init OK,we can continue.
//    function
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static SIM_SERVICE_RESULT_E SIM_IsSIMorUSIM(uint32 cur_sim_task)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_EF_MAPPING_DIR_T dir_data;
    uint8 dir_record_num = 0;
    uint8 i = 0;

    if(0 != s_sim_usim_is_checked[cur_sim_task])
    {
        s_sim_usim_is_checked[cur_sim_task] = 2;
        //this function shall be called only once
    }
    else
    {
        //the rules:
        // 1, send the instructions as 3G UICC.
        // 2, if we can select and read the EFdir successfully, we regard this card as 3g UICC.
        // 3, if fail, we will cold reset again(by return FALSE), and regard this as 2g sim.
        s_sim_usim_is_checked[cur_sim_task] = 1;
        s_sim_is_usim[cur_sim_task] = USIM_3G_APPLICATION;
        // SELECT like as an 3G USIM card firstly.
        ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &select_data);

        if(SIM_SERVICE_OK == ser_result)
        {
            ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_DIR, &select_data);

            if(SIM_SERVICE_OK == ser_result)
            {
                if(0 != select_data.dfnumber_recordlen)
                {
                    dir_record_num = (uint8)(select_data.freespace_filesize / select_data.dfnumber_recordlen);

                    if(dir_record_num > 3)
                    {
                        dir_record_num = 3;//Set a max limit
                    }
                }

                ser_result = SIM_SERVICE_ERROR_UNKNOW;//clear it

                for(i = 0; i < dir_record_num; i++)
                {
                    //To get the ADFusim ID
                    ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_DIR, (i + 1), SIM_RECORD_ABSOLUTE, &dir_data);

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        //store the AID
                        s_sim_usim_aid[cur_sim_task].len = dir_data.aid_len;
                        SCI_MEMCPY(s_sim_usim_aid[cur_sim_task].value, dir_data.aid_value, dir_data.aid_len);
                        //SCI_TRACE_LOW:"SIM%d: this is a USIM. AID len%d, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_4853_112_2_17_22_59_42_4577, (uint8 *)"dddddddddddddd", cur_sim_task, s_sim_usim_aid[cur_sim_task].len, \
                                     s_sim_usim_aid[cur_sim_task].value[0], s_sim_usim_aid[cur_sim_task].value[1], s_sim_usim_aid[cur_sim_task].value[2], s_sim_usim_aid[cur_sim_task].value[3], \
                                     s_sim_usim_aid[cur_sim_task].value[4], s_sim_usim_aid[cur_sim_task].value[5], s_sim_usim_aid[cur_sim_task].value[6], s_sim_usim_aid[cur_sim_task].value[7], \
                                     s_sim_usim_aid[cur_sim_task].value[8], s_sim_usim_aid[cur_sim_task].value[9], s_sim_usim_aid[cur_sim_task].value[10], s_sim_usim_aid[cur_sim_task].value[11], \
                                     s_sim_usim_aid[cur_sim_task].value[12], s_sim_usim_aid[cur_sim_task].value[13], s_sim_usim_aid[cur_sim_task].value[14], s_sim_usim_aid[cur_sim_task].value[15]);

                        ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_ADF_USIM, &select_data);
                        break;//OK. quit the loop.
                    }
                    else if ((SIM_SERVICE_DECODE_EMPTY_RECORD == ser_result) 
						|| (SIM_SERVICE_DECODE_EF_ERROR == ser_result))
                    {
                        continue;//An USIM has 3 EFdir records, only the last one is OK.
                    }
                    else
                    {
                        break;//Other errors. quit the loop.
                    }
                }
            }
        }

        if(SIM_SERVICE_OK == ser_result)
        {
            //SCI_TRACE_LOW:"SIM%d: this is a USIM card"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_4861_112_2_17_22_59_42_4578, (uint8 *)"d", cur_sim_task);
        }
        else
        {
            //SCI_TRACE_LOW:"SIM%d: this is a SIM card"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_4866_112_2_17_22_59_42_4579, (uint8 *)"d", cur_sim_task);
            s_sim_is_usim[cur_sim_task] = SIM_2G_APPLICATION;//this is still a 2G SIM card.
        }
    }

    return ser_result;
}
#endif

/********************************************************************
//    DESCRIPTION
//    get the SIM card's volt info in ATR data
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static BOOLEAN SIM_IsGetATRVoltInfo(uint32 cur_sim_task, uint8 *atr_volt_byte)
{
    BOOLEAN ret_value = FALSE;
    uint32 exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);

    ret_value = SIMHAL_GetATRVoltInfo(exchange_card_id, atr_volt_byte);

    SCI_TRACE_LOW("SIM%d:IsGetATRVoltInfo %d. 0x%x",cur_sim_task,ret_value,*atr_volt_byte);

    return ret_value;
}

#ifdef SIM_TEST_IN_WIN32
/********************************************************************
//    DESCRIPTION
//    get the current SIM task's index, 0,1 or 2.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIM_GetCurrentTaskIndex(void)
{
    uint32 current_sim_thread_id = SCI_IDENTIFY_THREAD();

    if(P_SIM == current_sim_thread_id)
    {
        return 0;//SIM Card 1
    }
    else if(g_sim_task_id[1] == current_sim_thread_id)
    {
        return 1;//SIM Card 2
    }
    else if(g_sim_task_id[2] == current_sim_thread_id)
    {
        return 2;//SIM Card 3
    }
    else
    {
        SCI_ASSERT(0);/*assert verified */
        //return 0;//compile warning
    }

    //return 0;
}
#endif

/********************************************************************
//    DESCRIPTION
//    active the sim check timer
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMMAIN_StartCardPresentTimer(uint32 cur_sim_task)
{
    uint32 sim_check_card_time = 0;
#if defined SIM_DROP_RESTORE_SUPPORT && defined SIM_DROP_RESTORE_NO_SELF_CHECK
    static uint32 enter_times[4] = {0,0,0,0};
#endif

    if(s_sim_call_poll_time[cur_sim_task] != 0)
    {
        sim_check_card_time = s_sim_call_poll_time[cur_sim_task];
    }
    else if(s_sim_stk_poll_time[cur_sim_task] != 0)
    {
        sim_check_card_time = s_sim_stk_poll_time[cur_sim_task];
    }
    else if(s_sim_sim_poll_time[cur_sim_task] != 0)
    {
        sim_check_card_time = s_sim_sim_poll_time[cur_sim_task];
    }
    else
    {
        sim_check_card_time = SIM_CARD_PRESENT_CHECK_FREQUENCY_IDLE;
    }

#if defined SIM_DROP_RESTORE_SUPPORT && defined SIM_DROP_RESTORE_NO_SELF_CHECK
    if (0 == s_sim_stk_poll_time[cur_sim_task])
    {
        if (0 == enter_times[cur_sim_task])
        {
            enter_times[cur_sim_task]++;
            if (NULL != s_sim_check_period_timer[cur_sim_task])
        	{
        		if (SCI_IsTimerActive(s_sim_check_period_timer[cur_sim_task]))
        		{
        			SCI_DeactiveTimer(s_sim_check_period_timer[cur_sim_task]);
        		}

        		SCI_ChangeTimer(s_sim_check_period_timer[cur_sim_task],SendCardPresentCheckInd,sim_check_card_time);
        		SCI_ActiveTimer(s_sim_check_period_timer[cur_sim_task]);
        	}
        }
    }
    else
    {
        enter_times[cur_sim_task] = 0;//clear it
    	if (NULL != s_sim_check_period_timer[cur_sim_task])
    	{
    		if (SCI_IsTimerActive(s_sim_check_period_timer[cur_sim_task]))
    		{
    			SCI_DeactiveTimer(s_sim_check_period_timer[cur_sim_task]);
    		}

    		SCI_ChangeTimer(s_sim_check_period_timer[cur_sim_task],SendCardPresentCheckInd,sim_check_card_time);
    		SCI_ActiveTimer(s_sim_check_period_timer[cur_sim_task]);
    	}
    }
#else
    {
    	if (NULL != s_sim_check_period_timer[cur_sim_task])
    	{
    		if (SCI_IsTimerActive(s_sim_check_period_timer[cur_sim_task]))
    		{
    			SCI_DeactiveTimer(s_sim_check_period_timer[cur_sim_task]);
    		}

    		SCI_ChangeTimer(s_sim_check_period_timer[cur_sim_task],SendCardPresentCheckInd,sim_check_card_time);
    		SCI_ActiveTimer(s_sim_check_period_timer[cur_sim_task]);
    	}
    }
#endif
}

/********************************************************************
//    DESCRIPTION
//    Read NV to get the SIM Slot mapping configure.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIM_Exchange_Init_Card_Slot_Label(void)
{
#if defined (MULTI_SIM_SYS_SINGLE) || defined (MULTI_SIM_SYS_DUAL) || defined (MULTI_SIM_SYS_TRI) || defined (MULTI_SIM_SYS_QUAD)
    static BOOLEAN enter_1st_flag = TRUE;
    uint8 sim_nv_slot_map[4] = {0, 1, 2, 3};
    NVITEM_ERROR_E nv_result = NVERR_NONE;
    MULTIPS_CREATE_PARAM_T multips_param;

    nv_result = EFS_NvitemRead(NV_SIM_CFG, (uint16)sizeof(MULTIPS_CREATE_PARAM_T), (uint8 *)&multips_param); //read nv maybe will last a long time

    //We only call this function only once
    if(enter_1st_flag)
    {
        enter_1st_flag = FALSE;
    }
    else
    {
        return;
    }

    if(NVERR_NONE == nv_result)
    {
        sim_nv_slot_map[3] = (uint8)(multips_param.sim_slot_map & 0xFF);
        sim_nv_slot_map[2] = (uint8)((multips_param.sim_slot_map >> 8)  & 0xFF);
        sim_nv_slot_map[1] = (uint8)((multips_param.sim_slot_map >> 16) & 0xFF);
        sim_nv_slot_map[0] = (uint8)((multips_param.sim_slot_map >> 24) & 0xFF);

#ifdef SIM_3G_USIM_SUPPORT

        if(1 == multips_param.reserve0)
        {
            s_sim_case_test_flag = TRUE;
        }
        else
        {
            s_sim_case_test_flag = FALSE;
        }

#endif

#if defined (MULTI_SIM_SYS_SINGLE)

        if(1 == sim_nv_slot_map[0])
        {
            s_sim_slot_0123_to_1023_flag = TRUE;
        }

#elif defined (MULTI_SIM_SYS_DUAL)

        if((0 == sim_nv_slot_map[0])
            && (1 == sim_nv_slot_map[1]))
        {
            s_sim_slot_0123_to_1023_flag = FALSE;
        }
        else if((1 == sim_nv_slot_map[0])
                && (0 == sim_nv_slot_map[1]))
        {
            s_sim_slot_0123_to_1023_flag = TRUE;
        }

#elif defined (MULTI_SIM_SYS_TRI)

        if((0 == sim_nv_slot_map[0])
            && (1 == sim_nv_slot_map[1]))
        {
            s_sim_slot_0123_to_1023_flag = FALSE;
        }
        else if((1 == sim_nv_slot_map[0])
                && (0 == sim_nv_slot_map[1]))
        {
            s_sim_slot_0123_to_1023_flag = TRUE;
        }
        else if((0 == sim_nv_slot_map[0])
                && (2 == sim_nv_slot_map[1]))
        {
            s_sim_slot_0123_to_0213_flag = TRUE;
        }
        else if((2 == sim_nv_slot_map[0])
                && (1 == sim_nv_slot_map[1]))
        {
            s_sim_slot_0123_to_2103_flag = TRUE;
        }
        else if((1 == sim_nv_slot_map[0])
                && (2 == sim_nv_slot_map[1]))
        {
            s_sim_slot_0123_to_1203_flag = TRUE;
        }
        else if((2 == sim_nv_slot_map[0])
                && (0 == sim_nv_slot_map[1]))
        {
            s_sim_slot_0123_to_2013_flag = TRUE;
        }

#elif defined (MULTI_SIM_SYS_QUAD)

        if(4 == multips_param.multips_create_set) //this means the user want 4 sets PS.
        {
            if((0 == sim_nv_slot_map[0])
                && (1 == sim_nv_slot_map[1])
                && (2 == sim_nv_slot_map[2])
                && (3 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_1023_flag = FALSE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1])
                    && (2 == sim_nv_slot_map[2])
                    && (3 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_1023_flag = TRUE;
            }
            else if((0 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1])
                    && (1 == sim_nv_slot_map[2])
                    && (3 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_0213_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (1 == sim_nv_slot_map[1])
                    && (0 == sim_nv_slot_map[2])
                    && (3 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_2103_flag = TRUE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1])
                    && (0 == sim_nv_slot_map[2])
                    && (3 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_1203_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1])
                    && (1 == sim_nv_slot_map[2])
                    && (3 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_2013_flag = TRUE;
            }
            else if((0 == sim_nv_slot_map[0])
                    && (1 == sim_nv_slot_map[1])
                    && (3 == sim_nv_slot_map[2])
                    && (2 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_0132_flag = TRUE;
            }
            else if((0 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1])
                    && (3 == sim_nv_slot_map[2])
                    && (1 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_0231_flag = TRUE;
            }
            else if((0 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1])
                    && (1 == sim_nv_slot_map[2])
                    && (2 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_0312_flag = TRUE;
            }
            else if((0 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1])
                    && (2 == sim_nv_slot_map[2])
                    && (1 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_0321_flag = TRUE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1])
                    && (3 == sim_nv_slot_map[2])
                    && (2 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_1032_flag = TRUE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1])
                    && (3 == sim_nv_slot_map[2])
                    && (0 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_1230_flag = TRUE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1])
                    && (0 == sim_nv_slot_map[2])
                    && (2 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_1302_flag = TRUE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1])
                    && (2 == sim_nv_slot_map[2])
                    && (0 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_1320_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1])
                    && (3 == sim_nv_slot_map[2])
                    && (1 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_2031_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (1 == sim_nv_slot_map[1])
                    && (3 == sim_nv_slot_map[2])
                    && (0 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_2130_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1])
                    && (0 == sim_nv_slot_map[2])
                    && (1 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_2301_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1])
                    && (1 == sim_nv_slot_map[2])
                    && (0 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_2310_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1])
                    && (1 == sim_nv_slot_map[2])
                    && (2 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_3012_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1])
                    && (2 == sim_nv_slot_map[2])
                    && (1 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_3021_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (1 == sim_nv_slot_map[1])
                    && (2 == sim_nv_slot_map[2])
                    && (0 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_3120_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (1 == sim_nv_slot_map[1])
                    && (0 == sim_nv_slot_map[2])
                    && (2 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_3102_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1])
                    && (0 == sim_nv_slot_map[2])
                    && (1 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_3201_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1])
                    && (1 == sim_nv_slot_map[2])
                    && (0 == sim_nv_slot_map[3]))
            {
                s_sim_slot_0123_to_3210_flag = TRUE;
            }
        }
        else if(3 == multips_param.multips_create_set) //the user only need 3 sets PS.
        {
#if defined (MULTI_SIM_SYS_QUAD_TO_TRI_SINGLE6302_BBSIM0)
            //In fact, you can modify the NV value as 0x00010302.
            s_sim_slot_0123_to_0132_flag = TRUE;
            return;
#endif

            if((0 == sim_nv_slot_map[0])
                && (1 == sim_nv_slot_map[1])
                && (2 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_1023_flag = FALSE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1])
                    && (2 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_1023_flag = TRUE;
            }
            else if((0 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1])
                    && (1 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_0213_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (1 == sim_nv_slot_map[1])
                    && (0 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_2103_flag = TRUE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1])
                    && (0 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_1203_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1])
                    && (1 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_2013_flag = TRUE;
            }
            else if((0 == sim_nv_slot_map[0])
                    && (1 == sim_nv_slot_map[1])
                    && (3 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_0132_flag = TRUE;
            }
            else if((0 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1])
                    && (3 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_0231_flag = TRUE;
            }
            else if((0 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1])
                    && (1 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_0312_flag = TRUE;
            }
            else if((0 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1])
                    && (2 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_0321_flag = TRUE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1])
                    && (3 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_1032_flag = TRUE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1])
                    && (3 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_1230_flag = TRUE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1])
                    && (0 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_1302_flag = TRUE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1])
                    && (2 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_1320_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1])
                    && (3 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_2031_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (1 == sim_nv_slot_map[1])
                    && (3 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_2130_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1])
                    && (0 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_2301_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1])
                    && (1 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_2310_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1])
                    && (1 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_3012_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1])
                    && (2 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_3021_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (1 == sim_nv_slot_map[1])
                    && (2 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_3120_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (1 == sim_nv_slot_map[1])
                    && (0 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_3102_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1])
                    && (0 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_3201_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1])
                    && (1 == sim_nv_slot_map[2]))
            {
                s_sim_slot_0123_to_3210_flag = TRUE;
            }
        }
        else if(2 == multips_param.multips_create_set) // they want only 2 sets of PS.
        {
            if((0 == sim_nv_slot_map[0])
                && (1 == sim_nv_slot_map[1]))
            {
                s_sim_slot_0123_to_1023_flag = FALSE;
                //s_sim_slot_0123_to_0132_flag = TRUE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1]))
            {
                s_sim_slot_0123_to_1023_flag = TRUE;
                //s_sim_slot_0123_to_1032_flag = TRUE;
            }
            else if((0 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1]))
            {
                s_sim_slot_0123_to_0213_flag = TRUE;
                //s_sim_slot_0123_to_0231_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (1 == sim_nv_slot_map[1]))
            {
                s_sim_slot_0123_to_2103_flag = TRUE;
                //s_sim_slot_0123_to_2130_flag = TRUE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1]))
            {
                s_sim_slot_0123_to_1203_flag = TRUE;
                //s_sim_slot_0123_to_1230_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1]))
            {
                s_sim_slot_0123_to_2013_flag = TRUE;
                //s_sim_slot_0123_to_2031_flag = TRUE;
            }
            else if((0 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1]))
            {
                s_sim_slot_0123_to_0312_flag = TRUE;
                //s_sim_slot_0123_to_0321_flag = TRUE;
            }
            else if((1 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1]))
            {
                s_sim_slot_0123_to_1302_flag = TRUE;
                //s_sim_slot_0123_to_1320_flag = TRUE;
            }
            else if((2 == sim_nv_slot_map[0])
                    && (3 == sim_nv_slot_map[1]))
            {
                s_sim_slot_0123_to_2301_flag = TRUE;
                //s_sim_slot_0123_to_2310_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (0 == sim_nv_slot_map[1]))
            {
                s_sim_slot_0123_to_3012_flag = TRUE;
                //s_sim_slot_0123_to_3021_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (1 == sim_nv_slot_map[1]))
            {
                s_sim_slot_0123_to_3120_flag = TRUE;
                //s_sim_slot_0123_to_3102_flag = TRUE;
            }
            else if((3 == sim_nv_slot_map[0])
                    && (2 == sim_nv_slot_map[1]))
            {
                s_sim_slot_0123_to_3201_flag = TRUE;
                //s_sim_slot_0123_to_3210_flag = TRUE;
            }
        }
        else//this means the user want 1 PS.
        {
            if(1 == sim_nv_slot_map[0])
            {
                s_sim_slot_0123_to_1023_flag = TRUE;
            }
            else if(2 == sim_nv_slot_map[0])
            {
                s_sim_slot_0123_to_2103_flag = TRUE;
            }
            else if(3 == sim_nv_slot_map[0])
            {
                s_sim_slot_0123_to_3120_flag = TRUE;
            }
        }

#endif
    }

#endif
    return;
}

/********************************************************************
//    DESCRIPTION
//    To exchange the SIM slot label in mutli-sim platform.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
uint32 SIM_Exchange_Card_Slot_Label(uint32 cur_sim_task)
{
    uint32 exchange_card_id = cur_sim_task;

#if defined (MULTI_SIM_SYS_SINGLE)

    if(s_sim_slot_0123_to_1023_flag)
    {
        if(0 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
    }

#elif defined (MULTI_SIM_SYS_DUAL) || defined (MULTI_SIM_SYS_TRI) || defined (MULTI_SIM_SYS_QUAD)

    if(s_sim_slot_0123_to_1023_flag)
    {
        if(0 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
    }

#if defined (MULTI_SIM_SYS_TRI) || defined (MULTI_SIM_SYS_QUAD)
    else if(s_sim_slot_0123_to_0213_flag)
    {
        if(1 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(2 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
    }
    else if(s_sim_slot_0123_to_2103_flag)
    {
        if(2 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
    }
    else if(s_sim_slot_0123_to_1203_flag)
    {
        if(0 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(2 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
    }
    else if(s_sim_slot_0123_to_2013_flag)
    {
        if(0 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(2 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
    }

#endif
#ifdef MULTI_SIM_SYS_QUAD
    else if(s_sim_slot_0123_to_0132_flag)
    {
        if(3 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(2 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_0231_flag)
    {
        if(2 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(3 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_0312_flag)
    {
        if(3 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(2 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_0321_flag)
    {
        if(3 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_1032_flag)
    {
        if(1 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(3 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(2 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_1230_flag)
    {
        if(1 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(2 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(3 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_1302_flag)
    {
        if(1 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(3 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(2 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_1320_flag)
    {
        if(1 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(3 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_2031_flag)
    {
        if(2 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(3 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_2130_flag)
    {
        if(2 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(3 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_2301_flag)
    {
        if(2 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(3 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_2310_flag)
    {
        if(2 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(3 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_3012_flag)
    {
        if(3 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(2 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_3021_flag)
    {
        if(3 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_3120_flag)
    {
        if(3 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_3102_flag)
    {
        if(3 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(2 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_3201_flag)
    {
        if(3 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(2 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }
    else if(s_sim_slot_0123_to_3210_flag)
    {
        if(3 == cur_sim_task)
        {
            exchange_card_id = 0;
        }
        else if(2 == cur_sim_task)
        {
            exchange_card_id = 1;
        }
        else if(1 == cur_sim_task)
        {
            exchange_card_id = 2;
        }
        else if(0 == cur_sim_task)
        {
            exchange_card_id = 3;
        }
    }

#endif
#endif
    return exchange_card_id;
}

/********************************************************************
//    DESCRIPTION
//    Remap sim card id according to the drv information
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static uint8 SIM_Exchange_Remap_SimId_From_Drv(uint8 exchange_card_id)
{
    uint8 card_id = exchange_card_id;

#if defined (MULTI_SIM_SYS_SINGLE)

    if(s_sim_slot_0123_to_1023_flag)
    {
        if(0 == exchange_card_id)
        {
            exchange_card_id = 1;
        }
        else
        {
            exchange_card_id = 0;
        }
    }

#elif defined (MULTI_SIM_SYS_DUAL) || defined (MULTI_SIM_SYS_TRI) || defined (MULTI_SIM_SYS_QUAD)

    if(s_sim_slot_0123_to_1023_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 1;
        }
        else if(1 == exchange_card_id)
        {
            card_id = 0;
        }
    }

#if defined (MULTI_SIM_SYS_TRI) || defined (MULTI_SIM_SYS_QUAD)
    else if(s_sim_slot_0123_to_0213_flag)
    {
        if(1 == exchange_card_id)
        {
            card_id = 2;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 1;
        }
    }
    else if(s_sim_slot_0123_to_2103_flag)
    {
        if(2 == exchange_card_id)
        {
            card_id = 0;
        }
        else if(0 == exchange_card_id)
        {
            card_id = 2;
        }
    }
    else if(s_sim_slot_0123_to_1203_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 1;
        }
        else if(1 == exchange_card_id)
        {
            card_id = 2;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 0;
        }
    }
    else if(s_sim_slot_0123_to_2013_flag)
    {
        if(1 == exchange_card_id)
        {
            card_id = 0;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 1;
        }
        else if(0 == exchange_card_id)
        {
            card_id = 2;
        }
    }

#endif
#ifdef MULTI_SIM_SYS_QUAD
    else if(s_sim_slot_0123_to_0132_flag)
    {
        if(2 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 2;
        }
    }
    else if(s_sim_slot_0123_to_0231_flag)
    {
        if(1 == exchange_card_id)
        {
            card_id = 2;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 1;
        }
    }
    else if(s_sim_slot_0123_to_0312_flag)
    {
        if(1 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 1;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 2;
        }
    }
    else if(s_sim_slot_0123_to_0321_flag)
    {
        if(1 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 1;
        }
    }
    else if(s_sim_slot_0123_to_1032_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 1;
        }

        if(1 == exchange_card_id)
        {
            card_id = 0;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 2;
        }
    }
    else if(s_sim_slot_0123_to_1230_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 1;
        }
        else if(1 == exchange_card_id)
        {
            card_id = 2;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 0;
        }
    }
    else if(s_sim_slot_0123_to_1302_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 1;
        }
        else if(1 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 0;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 2;
        }
    }
    else if(s_sim_slot_0123_to_1320_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 1;
        }
        else if(1 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 0;
        }
    }
    else if(s_sim_slot_0123_to_2031_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 2;
        }
        else if(1 == exchange_card_id)
        {
            card_id = 0;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 1;
        }
    }
    else if(s_sim_slot_0123_to_2130_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 2;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 0;
        }
    }
    else if(s_sim_slot_0123_to_2301_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 2;
        }
        else if(1 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 0;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 1;
        }
    }
    else if(s_sim_slot_0123_to_2310_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 2;
        }
        else if(1 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 1;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 0;
        }
    }
    else if(s_sim_slot_0123_to_3012_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(1 == exchange_card_id)
        {
            card_id = 0;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 1;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 2;
        }
    }
    else if(s_sim_slot_0123_to_3021_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(1 == exchange_card_id)
        {
            card_id = 0;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 1;
        }
    }
    else if(s_sim_slot_0123_to_3120_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 0;
        }
    }
    else if(s_sim_slot_0123_to_3102_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 0;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 2;
        }
    }
    else if(s_sim_slot_0123_to_3201_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(1 == exchange_card_id)
        {
            card_id = 2;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 0;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 1;
        }
    }
    else if(s_sim_slot_0123_to_3210_flag)
    {
        if(0 == exchange_card_id)
        {
            card_id = 3;
        }
        else if(1 == exchange_card_id)
        {
            card_id = 2;
        }
        else if(2 == exchange_card_id)
        {
            card_id = 1;
        }
        else if(3 == exchange_card_id)
        {
            card_id = 0;
        }
    }

#endif
#endif
    return card_id;
}

#ifdef SIM_PLUG_IN_SUPPORT
/********************************************************************
//    DESCRIPTION
//
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMMAIN_IsSIMPlugIn(uint32 cur_sim_task, BOOLEAN is_plug_in)
{
    xSignalHeaderRec *psig = NULL;
#ifndef SIM_TEST_IN_WIN32
    uint32 exchange_card_id = 0;
#endif

    s_sim_plug_in_flag[cur_sim_task] = FALSE;

#ifndef SIM_TEST_IN_WIN32
    exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);
    SIMHAL_PowerOffSIM(exchange_card_id);
#endif
    //current enter to the NULL state
    SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_NULL_STATE, "SIM NULL State", g_sim_task_id[cur_sim_task]);
    //reset the SIM variable
    SIMMAIN_ResetSIMVariable(cur_sim_task);
    EnableDeepSleep(cur_sim_task);

    if(is_plug_in)
    {
        SCI_CREATE_SIGNAL(psig, SIM_HOT_PLUG_IN_IND, sizeof(SIM_SIG_HOT_PLUG_IN_IND_T), g_sim_task_id[cur_sim_task]);
#ifdef GSM_CUSTOMER_AFP_SUPPORT
        s_sim_after_plugin_wait_app_flag[cur_sim_task] = TRUE;
#endif
        SCI_SEND_SIGNAL(psig, g_sim_info_task_id[cur_sim_task]);

#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK
        if (SCI_IsTimerActive(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id]))
		{
			SCI_DeactiveTimer(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id]);
		}
#endif
    }
#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK    
    else
    {
        if (SCI_IsTimerActive(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id]))
		{
			SCI_DeactiveTimer(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id]);
		}
		SCI_ChangeTimer(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id],SIMMAIN_PLUG_IN_By_Check,s_sim_plug_in_by_check_interval);
		SCI_ActiveTimer(s_sim_plug_in_check_timer[s_sim_plug_in_by_check_card_id]);
    }
#endif

    return;
}
#endif

/********************************************************************
//    DESCRIPTION
//   Judge the corresponding Idle Timer is active or not, then stop sim clock.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMMAIN_ClockStop(uint32 cur_sim_task)
{
#ifndef SIM_TEST_IN_WIN32
    BOOLEAN result = FALSE;
    uint32 exchange_card_id = cur_sim_task;
    uint32 other_sim_card = 1;
#ifdef MULTI_SIM_SYS_TRI
    uint32 third_sim_card = 2;
#endif
#ifdef MULTI_SIM_SYS_QUAD
#ifndef MULTI_SIM_SYS_QUAD_2SIM_INTERACE
    uint32 third_sim_card = 2;
    uint32 fourth_sim_card = 3;
#endif
#endif

    if(s_sim_is_ext_dualsim)
    {
#ifdef MULTI_SIM_SYS_QUAD

        if(SIMHAL_IsParallelDualSIM())
        {
            //SC6800H quadsim. 2 sim controller can work at the same time.
            if(SIMHAL_MultiSim_Is6302())
            {
                if(SIMHAL_MultiSim_IsParallelDual6302())
                {
                    //SC6800H with  both 6302 can work at the same time because of two separated spi
                    if(0 == cur_sim_task)
                    {
                        other_sim_card = 3;
                    }
                    else if(1 == cur_sim_task)
                    {
                        other_sim_card = 2;
                    }
                    else if(2 == cur_sim_task)
                    {
                        other_sim_card = 1;
                    }
                    else
                    {
                        other_sim_card = 0;
                    }

                    if((NULL != s_sim_idle_timer[other_sim_card]) && (SCI_IsTimerActive(s_sim_idle_timer[other_sim_card])))
                    {
                        result = TRUE;
                    }
                }
                else
                {
                    //SC6800H quadsim, only single spi channel
                    if(0 == cur_sim_task)
                    {
                        other_sim_card = 1;
                        third_sim_card = 2;
                        fourth_sim_card = 3;
                    }
                    else if(1 == cur_sim_task)
                    {
                        other_sim_card = 0;
                        third_sim_card = 2;
                        fourth_sim_card = 3;
                    }
                    else if(2 == cur_sim_task)
                    {
                        other_sim_card = 0;
                        third_sim_card = 1;
                        fourth_sim_card = 3;
                    }
                    else
                    {
                        other_sim_card = 0;
                        third_sim_card = 1;
                        fourth_sim_card = 2;
                    }

                    if(((NULL != s_sim_idle_timer[other_sim_card]) && (SCI_IsTimerActive(s_sim_idle_timer[other_sim_card])))
                        || ((NULL != s_sim_idle_timer[third_sim_card]) && (SCI_IsTimerActive(s_sim_idle_timer[third_sim_card])))
                        || ((NULL != s_sim_idle_timer[fourth_sim_card]) && (SCI_IsTimerActive(s_sim_idle_timer[fourth_sim_card]))))
                    {
                        result = TRUE;
                    }
                }
            }
            else//SC6800H+6303
            {
                if((1 == cur_sim_task) || (2 == cur_sim_task) || (3 == cur_sim_task))
                {
                    if(1 == cur_sim_task)
                    {
                        other_sim_card = 2;
                        third_sim_card = 3;
                    }
                    else if(2 == cur_sim_task)
                    {
                        other_sim_card = 1;
                        third_sim_card = 3;
                    }
                    else
                    {
                        other_sim_card = 1;
                        third_sim_card = 2;
                    }

                    if(((NULL != s_sim_idle_timer[other_sim_card]) && (SCI_IsTimerActive(s_sim_idle_timer[other_sim_card])))
                        || ((NULL != s_sim_idle_timer[third_sim_card]) && (SCI_IsTimerActive(s_sim_idle_timer[third_sim_card]))))
                    {
                        result = TRUE;
                    }
                }
            }
        }
        else
        {
            //SC6600L quadsim, only 1 sim controller
            if(0 == cur_sim_task)
            {
                other_sim_card = 1;
                third_sim_card = 2;
                fourth_sim_card = 3;
            }
            else if(1 == cur_sim_task)
            {
                other_sim_card = 0;
                third_sim_card = 2;
                fourth_sim_card = 3;
            }
            else if(2 == cur_sim_task)
            {
                other_sim_card = 0;
                third_sim_card = 1;
                fourth_sim_card = 3;
            }
            else
            {
                other_sim_card = 0;
                third_sim_card = 1;
                fourth_sim_card = 2;
            }

            if(((NULL != s_sim_idle_timer[other_sim_card]) && (SCI_IsTimerActive(s_sim_idle_timer[other_sim_card])))
                || ((NULL != s_sim_idle_timer[third_sim_card]) && (SCI_IsTimerActive(s_sim_idle_timer[third_sim_card])))
                || ((NULL != s_sim_idle_timer[fourth_sim_card]) && (SCI_IsTimerActive(s_sim_idle_timer[fourth_sim_card]))))
            {
                result = TRUE;
            }
        }

#elif defined (MULTI_SIM_SYS_TRI)

        if(0 == cur_sim_task)
        {
            other_sim_card = 1;
            third_sim_card = 2;
        }
        else if(1 == cur_sim_task)
        {
            other_sim_card = 0;
            third_sim_card = 2;
        }
        else
        {
            other_sim_card = 0;
            third_sim_card = 1;
        }

        if(((NULL != s_sim_idle_timer[other_sim_card]) && (SCI_IsTimerActive(s_sim_idle_timer[other_sim_card])))
            || ((NULL != s_sim_idle_timer[third_sim_card]) && (SCI_IsTimerActive(s_sim_idle_timer[third_sim_card]))))
        {
            result = TRUE;
        }

#else

        if(0 == cur_sim_task)
        {
            other_sim_card = 1;
        }
        else
        {
            other_sim_card = 0;
        }

        if((NULL != s_sim_idle_timer[other_sim_card]) && (SCI_IsTimerActive(s_sim_idle_timer[other_sim_card])))
        {
            result = TRUE;
        }

#endif

        if(result)
        {
            s_sim_arm_sleep[cur_sim_task] = TRUE;
            return;
        }
    }

    exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);
    SIMHAL_IdleSIMCard(exchange_card_id);
    EnableDeepSleep(cur_sim_task);
#endif//SIM_TEST_IN_WIN32
}

/********************************************************************
//    DESCRIPTION
//   create the mutex for the multi sim task with few sim controller
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMMAIN_InitMutex(uint32 cur_sim_task)
{
    if(s_sim_is_ext_dualsim)
    {
        if(NULL == s_sim_task_mutex[0])
        {
            s_sim_task_mutex[0] = SCI_CreateMutex("sim_mutex_1", SCI_INHERIT);
            SCI_ASSERT(SCI_NULL != s_sim_task_mutex[0]);/*assert verified:SCI_NULL */
        }

        if(NULL == s_sim_task_mutex[1])
        {
            s_sim_task_mutex[1] = SCI_CreateMutex("sim_mutex_2", SCI_INHERIT);
            SCI_ASSERT(SCI_NULL != s_sim_task_mutex[1]);/*assert verified:SCI_NULL */
        }
    }
}

/********************************************************************
//    DESCRIPTION
//   sim task wait and get the mutex to controll the sim hardware interface
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMMAIN_GetMutex(uint32 cur_sim_task)
{
#ifndef SIM_TEST_IN_WIN32
    uint32 mutex_result = SCI_SUCCESS;
#ifdef MULTI_SIM_SYS_QUAD
    BOOLEAN fisrt_mutex_flag = FALSE;
#endif

    if(s_sim_is_ext_dualsim)
    {
#ifdef MULTI_SIM_SYS_QUAD

        if(SIMHAL_IsParallelDualSIM())
        {
            //SC6800H quadsim. 2 sim controller can work at the same time.
            if(SIMHAL_MultiSim_Is6302())
            {
                //SC6800H + dual 6302
                if(SIMHAL_MultiSim_IsParallelDual6302())
                {
                    //SC6800H + dual 6302 with 2 separated spi
                    if((0 == cur_sim_task) || (3 == cur_sim_task))
                    {
                        fisrt_mutex_flag = TRUE;
                    }
                    else
                    {
                        fisrt_mutex_flag = FALSE;
                    }
                }
                else
                {
                    //SC6800H + dual 6302 with 1 spi
                    fisrt_mutex_flag = TRUE;
                }
            }
            else
            {
                //SC6800H + 6303
                if(0 == cur_sim_task)
                {
                    fisrt_mutex_flag = TRUE;
                }
                else
                {
                    fisrt_mutex_flag = FALSE;
                }
            }
        }
        else
        {
            //SC6600L
            fisrt_mutex_flag = TRUE;
        }

        if(fisrt_mutex_flag)
        {
            mutex_result = SIM_MUTEX0_SEMTAKE;
        }
        else
        {
            mutex_result = SIM_MUTEX1_SEMTAKE;
        }

#elif defined (MULTI_SIM_SYS_TRI)

        if(SIMHAL_IsParallelDualSIM())
        {
            mutex_result = SCI_SUCCESS;
        }
        else
        {
            mutex_result = SIM_MUTEX0_SEMTAKE;
        }

#else

        if(SIMHAL_IsParallelDualSIM())
        {
            //SC6800H dual sim
            mutex_result = SCI_SUCCESS;
        }
        else
        {
            //SC6600L dual sim
            mutex_result = SIM_MUTEX0_SEMTAKE;
        }

#endif

        if(SCI_SUCCESS != mutex_result)
        {
            //SCI_TRACE_LOW:"SIM::mutex operate error."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_6567_112_2_17_22_59_46_4580, (uint8 *)"");
            SCI_ASSERT(FALSE);/*assert verified */
        }
        else
        {
            //SCI_TRACE_LOW:"SIM::task %d take the sem."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_6572_112_2_17_22_59_46_4581, (uint8 *)"d", cur_sim_task);
        }
    }

#endif
}

/********************************************************************
//    DESCRIPTION
//
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMMAIN_PutMutex(uint32 cur_sim_task)
{
#ifndef SIM_TEST_IN_WIN32
#ifdef MULTI_SIM_SYS_QUAD
    BOOLEAN fisrt_mutex_flag = FALSE;
#endif

    if(s_sim_is_ext_dualsim)
    {
#ifdef MULTI_SIM_SYS_QUAD

        if(SIMHAL_IsParallelDualSIM())
        {
            //SC6800H quadsim. 2 sim controller can work at the same time.
            if(SIMHAL_MultiSim_Is6302())
            {
                //SC6800H + dual 6302
                if(SIMHAL_MultiSim_IsParallelDual6302())
                {
                    //SC6800H + dual 6302 with 2 separated spi
                    if((0 == cur_sim_task) || (3 == cur_sim_task))
                    {
                        fisrt_mutex_flag = TRUE;
                    }
                    else
                    {
                        fisrt_mutex_flag = FALSE;
                    }
                }
                else
                {
                    //SC6800H + dual 6302 with 1 spi
                    fisrt_mutex_flag = TRUE;
                }
            }
            else
            {
                //SC6800H + 6303
                if(0 == cur_sim_task)
                {
                    fisrt_mutex_flag = TRUE;
                }
                else
                {
                    fisrt_mutex_flag = FALSE;
                }
            }
        }
        else
        {
            //SC6600L
            fisrt_mutex_flag = TRUE;
        }

        if(fisrt_mutex_flag)
        {
            SIM_MUTEX0_SEMGIVE;
        }
        else
        {
            SIM_MUTEX1_SEMGIVE;
        }

        //SCI_TRACE_LOW:"SIM::task %d put the sem."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_6639_112_2_17_22_59_46_4582, (uint8 *)"d", cur_sim_task);
#elif defined (MULTI_SIM_SYS_TRI)
        SIM_MUTEX0_SEMGIVE;
        //SCI_TRACE_LOW:"SIM::task %d put the sem."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_6642_112_2_17_22_59_46_4583, (uint8 *)"d", cur_sim_task);
#else

        if(SIMHAL_IsParallelDualSIM())
        {
            //SC6800H dual sim
            //do nothing
        }
        else
        {
            //SC6600L dual sim
            SIM_MUTEX0_SEMGIVE;
            //SCI_TRACE_LOW:"SIM::task %d put the sem."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_MAIN_6651_112_2_17_22_59_46_4584, (uint8 *)"d", cur_sim_task);
        }

#endif
    }

#endif
}

/********************************************************************
//    DESCRIPTION
//      Warm reset the sim card
//    Global resource dependence :
//    Author:Jacky.Ying
//    Note:
***********************************************************************/
static void SIMMAIN_RefreshWarmReset(uint32 cur_sim_task)
{
#ifndef WIN32
#ifdef SIM_APPLICATION_TOOLKIT
    xSignalHeaderRec *pind = NULL;
#endif
    uint8 exchange_card_id = 0;

    SCI_TRACE_LOW("SIM%d:Refresh WarmReset SIM !", cur_sim_task);

    exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);

    SIMMAIN_ResetSIMVariable(cur_sim_task);

    //now in refresh warm reset procedure
    s_sim_is_in_refresh[cur_sim_task] = TRUE;

    SIMHAL_WARM_ResetDriver((SIM_CARD_INDEX_E)exchange_card_id);

    SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_ATR_STATE, "SIM ATR State", g_sim_task_id[cur_sim_task]);

    SIMHAL_WARM_ResetSIMCard((SIM_CARD_INDEX_E)exchange_card_id);

    g_sim_card_setting[cur_sim_task].card_present = TRUE;

#ifdef SIM_APPLICATION_TOOLKIT
    //send signal to notify STK layer stopping the polling timer
    SCI_CREATE_SIGNAL(pind, SIMAT_POWER_OFF_IND, sizeof(SIM_SIG_SIMAT_POWER_OFF_IND_T), g_sim_task_id[cur_sim_task]);
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pind, g_sim_simat_task_id[cur_sim_task]);
#endif
#else//ifdef WIN32
    xSignalHeaderRec *pind = NULL;

    //now in refresh warm reset procedure
    s_sim_is_in_refresh[cur_sim_task] = TRUE;

    SIM_NEXTSTATE(cur_sim_task, g_sim_card_state[cur_sim_task], SIM_ATR_STATE, "SIM ATR State", g_sim_task_id[cur_sim_task]);
    SCI_CREATE_SIGNAL(pind, SIM_CARD_INSERTED_IND, sizeof(SIM_SIG_CARD_INSERTED_IND_T), g_sim_task_id[cur_sim_task]);
    ((SIM_SIG_CARD_INSERTED_IND_T *)pind)->insert_status = TB_SIM_INSERTED_OK;
    ((SIM_SIG_CARD_INSERTED_IND_T *)pind)->t_protocol = TB_SIM_T_0_PROTOCOL;

    SCI_SEND_SIGNAL(pind, g_sim_task_id[cur_sim_task]);
#endif
}

/********************************************************************
//    DESCRIPTION
//      send refresh reset result to MN
//    Global resource dependence :
//    Author:Jacky.Ying
//    Note:
***********************************************************************/
static void SIMMAIN_SendRefreshResetCnf(
    uint32 cur_sim_task,
    BOOLEAN reset_result     //TRUE,reset ok,FALSE,reset fail
)
{
    SIM_SIG_REFRESH_RESET_CNF_T *pcnf;

    //if now in refresh reset, we should notify MN SIM_REFRESH_RESET_CNF
    if(s_sim_is_in_refresh[cur_sim_task])
    {
        s_sim_is_in_refresh[cur_sim_task] = FALSE;

        //create the confirm signal
        SCI_CREATE_SIGNAL(pcnf, SIM_REFRESH_RESET_CNF, sizeof(SIM_SIG_REFRESH_RESET_CNF_T), g_sim_task_id[cur_sim_task]);
        //get info for the confirm signal from request signal
        pcnf->command_ref = 0;
        pcnf->result = reset_result;

        //send the confirm signal of the request signal
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, g_sim_info_task_id[cur_sim_task]);
    }
}

/********************************************************************
//    DESCRIPTION
//    Disable low layer PPS procedure
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIM_DisablePPSFeature(uint32 cur_sim_task)
{
#ifndef SIM_TEST_IN_WIN32
    uint32 exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);
    SCI_TRACE_LOW("SIM%d: disable PPS by high layer",cur_sim_task);
    SIMHAL_DisablePPSFeature(exchange_card_id);
#endif
}

#ifdef SIM_DROP_RESTORE_SUPPORT 
#ifdef SIM_DROP_RESTORE_DELAY_PBSMS_READ
/********************************************************************
//    DESCRIPTION
//      send refresh reset result to MN
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIM_Delay_Read(uint32 lParam)
{
	xSignalHeaderRec* psig = NULL;

	SCI_CREATE_SIGNAL(psig,SIM_SMS_READ_SMS_REQ,sizeof(SIM_SMS_READ_SMS_REQ_T),g_sim_info_task_id[lParam]);
	((SIM_SMS_READ_SMS_REQ_T*)psig)->record_id = 1;
	SCI_SEND_SIGNAL(psig,g_sim_task_id[lParam]);
	SCI_TRACE_LOW("SIM%d: delay timer start to read",lParam);
}
#endif
#endif

#ifdef SIM_PLUG_IN_SUPPORT
#ifdef SIM_PLUG_IN_SUPPORT_BY_CHECK
/********************************************************************
//    DESCRIPTION
//      
//
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
static void SIMMAIN_PLUG_IN_By_Check(uint32 lParam)
{
	xSignalHeaderRec* psig = NULL;

	SCI_TRACE_LOW("SIMMAIN_PLUG_IN_By_Check %d %d",lParam,s_sim_plug_in_by_check_card_id);

	//if ((SIM_NULL_STATE == g_sim_card_state[s_sim_plug_in_by_check_card_id]) 
		//|| (SIM_NO_POWER_STATE == g_sim_card_state[s_sim_plug_in_by_check_card_id]))
	{
		SCI_CREATE_SIGNAL(psig,SIM_QUERY_CARD_STATUS_REQ,sizeof(SIM_SIG_QUERY_CARD_STATUS_REQ_T),g_sim_task_id[s_sim_plug_in_by_check_card_id]);
		SCI_SEND_SIGNAL(psig,g_sim_task_id[s_sim_plug_in_by_check_card_id]);
	}
}

/********************************************************************
//    DESCRIPTION
//      to set the SIM PLug card_id 
//      tp set the SIM poll time interval
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMMAIN_PLUG_IN_By_Check_Setting(uint32 card_id, uint32 time_interval)
{
	if ((0 == card_id) || (1 == card_id) || (2 == card_id))
	{
		s_sim_plug_in_by_check_card_id = card_id;
	}
	else
	{
		SCI_ASSERT(0);
	}

	s_sim_plug_in_by_check_interval = time_interval;
	if (s_sim_plug_in_by_check_interval < SIM_PLUG_IN_BY_CHECK_MIN_TIME)
	{
		s_sim_plug_in_by_check_interval = SIM_PLUG_IN_BY_CHECK_MIN_TIME;//I think 8s is the smallest interval
	}

	SCI_TRACE_LOW("SIMMAIN_PLUG_IN_By_Check_Setting card_id=%d, time=%d",\
		s_sim_plug_in_by_check_card_id,s_sim_plug_in_by_check_interval);
}
#endif
#endif

/**********************************************************************
//      Trace the data in the sim rx buffer
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIM_PrintRxBuf(uint32 cur_sim_task)
{
#ifndef SIM_TEST_IN_WIN32
    uint32 exchange_card_id = 0;
    exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);

    SIMHAL_PrintRxBuf(exchange_card_id);
#endif    
    return;
}

/**********************************************************************
//    Description:
//      Trace the data in the sim tx buffer
//    Global resource dependence :
//    Author:Yongxia.zhang
//    Note:
***********************************************************************/
void SIM_PrintTxBuf(uint32 cur_sim_task)
{
#ifndef SIM_TEST_IN_WIN32
    uint32 exchange_card_id = 0;
    exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);

    SIMHAL_PrintTxBuf(exchange_card_id);
#endif      
    return;
}

/**********************************************************************
//    Description:
//      trace the register value when exception occurs
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIM_PrintRegisterBuf(uint32 cur_sim_task)
{
#ifndef SIM_TEST_IN_WIN32

    uint32 exchange_card_id = 0;
    exchange_card_id = SIM_Exchange_Card_Slot_Label(cur_sim_task);

    SIMHAL_PrintRegisterBuf(exchange_card_id);
#endif    
    return;
}


#ifdef   __cplusplus
}
#endif

