/******************************************************************************
 ** File Name:      sim_macro_switch.h                                        *
 ** Author:         Raislin.Kong                                              *
 ** Date:           11/27/2001                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    Use this h file to Define the macros that switch code in  *
 **				    on\off in SIMM as diffent implementation.				  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 11/27/2001     Raislin.Kong      create                                   *
 ******************************************************************************/

#ifndef _SIM_MACRO_SWITCH_H_
#define _SIM_MACRO_SWITCH_H_
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#ifdef STATIC
#undef STATIC
#endif

#ifdef SIM_CPPUNIT_TEST
#define STATIC
#else
#define STATIC static
#endif

//working in the sim simulator 
//#define SIM_TEST_IN_WIN32


//these macro define the sim layers capblity
#define SIM_MAX_VALIDATE_LANGUAGE_NUM			8
#define SIM_MAX_VALIDATE_PLN_SEL_NUM			8
#define SIM_MAX_GID_LEN							16
#define SIM_MAX_VALIDATE_CBMI_NUM				30
#define SIM_MAX_CBMID_CBMI_NUM					8
#define SIM_MAX_CBMIR_RANGE_NUM					8
#define SIM_MAX_CO_OPERATIVE_NET_LIST_NUM		8


#define SIM_MAX_DN_NUMBER_LEN                               20
#define SIM_MAX_DN_ALPHA_IDENTIFY_LEN                   60
#define SIM_MAX_ADN_NUM							510//255*2
#define SIM_MAX_FDN_NUM							20
#define SIM_MAX_LND_NUM							10
#define SIM_MAX_MSISDN_NUM						10
#define SIM_MAX_SDN_NUM							30
#define SIM_MAX_BDN_NUM							10
#define SIM_MAX_SMS_NUM							50
#define SIM_MAX_SMSP_NUM						10
#define SIM_MAX_SMSP_ALPHA_ID_LEN				100

#define SIM_MAX_PNN_ICON_NUM                    255
#define SIM_MAX_PNN_NUM                         255
#define SIM_MAX_OPL_NUM                         255

#define SIM_MAX_ECC_ALPHA_ID_LEN					25//Dubai DU is (0x1c-4)
#define SIM_PATH_LEVEL                          12
#define SIM_SEND_DATA_SIZE                      255

//the idle time that when pass the SIM card will stop the SIM clock to sleep
/*sim clock stop的时间，从原来的100ms变成2ms，根据ISO7816-3的5。3。4节，clock stop要在收到数据之后
的至少1860/f之后才能停掉。1860/1M=1.86ms。但是考虑到时钟的精确度，取成4ms.*/
#define SIM_IDLE_TIME_BEFORE_SLEEP              4

/********************************************************************
//    DESCRIPTION:Check if g_sim_card_state is SIM_INITIALIZE_STATE or
//                SIM_ACTIVE_STATE.
//    Global resource dependence :
//    Author:hanjun.liu
//    Note:
***********************************************************************/
uint8 SIM_IsSimInitialized(uint32 cur_sim_task);

/**********************************************************************
//    Description:
//      The entry function of SIM layer.the major function of SIM layer.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
extern void SIMCARD_Task(
                  uint32 agrc,  //the argument num
                  void* argv    //the argument array
                  );


#ifdef   __cplusplus
    }
#endif

#endif	//_SIM_MACRO_SWITCH_H_
