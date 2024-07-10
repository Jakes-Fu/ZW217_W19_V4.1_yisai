/*************************************************************************
 ** File Name:      mmieng_nv.h                                          *
 ** Author:         songbin.zeng                                           *
 ** Date:           2006/10/09                                          *
 ** Copyright:      2006 Spreadrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/10/09  songbin.zeng  Create.                             *
*************************************************************************/

#ifndef _MMIENG_NV_H_
#define _MMIENG_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#ifdef ENG_SUPPORT
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMIENG_NV_SENSOR_MODE_ID = MMI_MODULE_ENG<< 16,
    MMIENG_NV_DV_FRAME_FRE_ID,
    MMIENG_NV_CHIP_TEST_MODE_ID,
    MMIENG_NV_CALLFORWARD_QUERY_ID,
    MMIENG_NV_IS_CARDLOG_ON,
    MMIENG_NV_IQ_MODE,
    MMINV_ENG_USBLOG,
    MMIENG_NV_CAP_OUTPUT,
#ifdef PCLINK_SUPPORT
    MMIENG_NV_PCLINK_TEST_ADDR_ID,
#endif
#ifdef CMMB_SUPPORT
    MMIENG_NV_MBBMS_AUDIO_ID,
#endif
    MMIENG_NV_AUTO_ANSWER_CALL_SET_ID,
    MMIENG_NV_WAPMMS_SAS_TEST_MODE_ID,
    MMIENG_NV_LAST_DATA_ID,
    MMIENG_NV_LAST_TIME_ID,
    MMIENG_NV_UA,
    MMIENG_NV_IS_USER_AGENT_USE,
    MMIENG_NV_IS_UA_PROFILE_USE,
    MMIENG_NV_IS_USER_AGENT_USE_LIST,
    MMIENG_NV_IS_UA_PROFILE_USE_LIST,
    MMIENG_NV_USER_AGENT_NUM,
    MMIENG_NV_UA_PROFILE_NUM,
    MMIENG_NV_IS_CALL_BARRING,
    MMIENG_NV_TEST_ITEM,        //phone test item,MS00192606
    MMIENG_NV_TEST_RESULT,      //phone test result
    MMIENG_NV_CMCC_TEST_FLAG, //CMCC TEST FLAG,MS00216980
    MMIENG_NV_POWERON_TIME_ID,   
    MMIENG_NV_STANDBY_TIME_ID,
    #ifdef MMI_AUTOTEST_SUPPORT
    MMIENG_NV_AUTOTEST,     // @CR250530 renwei add
    #endif
    MMIENG_NV_POWER_KEY_SETTING,
    MMIENG_NV_IS_REGISTER_PS_SERVICE,
    MMIENG_NV_IS_MONKEY_MODE,//monkey mode
    MMIENG_NV_IS_UART_LOG_OPEN, //is uart log open
    MMIENG_NV_LTE_OPERATION_MODE_SIM1,
    MMIENG_NV_LTE_OPERATION_MODE_SIM2,
    MMIENG_NV_FORBID_ALL_GPRS_DATA,
#if defined ENG_BAND_FREQ_SUPPORT
    MMIENG_NV_BAND_FREQ_SIM1,
    MMIENG_NV_BAND_FREQ_SIM2,
#endif
    MMIENG_NV_MAX_ITEM_ID,
}MMIENG_NV_ITEM_ID_E;

#endif//ENG_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIENG_NV_H_

