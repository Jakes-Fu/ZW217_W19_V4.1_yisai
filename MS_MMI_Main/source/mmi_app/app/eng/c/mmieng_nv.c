/*************************************************************************
 ** File Name:      mmimp3_nv.c                                          *
 ** Author:         haiyang.hu                                           *
 ** Date:           2006/09/18                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/10/09 songbin.zeng   Create.                              *
*************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef ENG_SUPPORT
//#include "sci_types.h"
#include "mmieng_nv.h"
#include "mmi_modu_main.h"
//#include "mmieng.h"
#include "mmieng_internal.h"

#ifdef MMI_AUTOTEST_SUPPORT
#include "mmi_autotest.h"// @CR250530 renwei add
#endif

/*the length of eng nv*/
LOCAL const uint16 mmieng_nv_len_tab[] =
{
    sizeof(MMIENG_SENSOR_MODE_E),       //MMIENG_NV_SENSOR_MODE_ID
    sizeof(MMIENG_DV_FRAME_FRE_E),      //MMIENG_NV_DV_FRAME_FRE_ID
    sizeof(MMIENG_CHIP_TEST_SETTING_E), 	//MMIENG_NV_CALLFORWARD_QUERY_ID
    sizeof(BOOLEAN),   	 //MMIENG_NV_CALLFORWARD_QUERY_ID
    sizeof(BOOLEAN),    	 //MMIENG_NV_IS_CARDLOG_ON
    sizeof(MMIENG_IQ_MODE_T), 	//MMIENG_NV_IQ_MODE
    sizeof(BOOLEAN),
    sizeof(BOOLEAN),	//MMIENG_NV_CAP_OUTPUT
#ifdef PCLINK_SUPPORT
    sizeof(uint32),	//MMIENG_NV_PCLINK_TEST_ADDR_ID
#endif
#ifdef CMMB_SUPPORT
    sizeof(BOOLEAN),  //mbbms audio service support
#endif
    sizeof(BOOLEAN),    //MMIENG_NV_AUTO_ANSWER_CALL_SET_ID
    sizeof(BOOLEAN),     //MMIENG_NV_WAPMMS_SAS_TEST_MODE_ID
    sizeof(SCI_DATE_T),		// MMIENG_NV_LAST_DATA_ID,
    sizeof(SCI_TIME_T),		//MMIENG_NV_LAST_TIME_ID,
    sizeof(MMIENG_UA_T),	//ua
    sizeof(BOOLEAN),	//is user agent use
    sizeof(BOOLEAN),	//is user profile use
    sizeof(BOOLEAN),	//is user agent use list
    sizeof(BOOLEAN),	//is ua profile use list
    sizeof(uint32),	//user agent list num
    sizeof(uint32),	//ua profile list num
    sizeof(BOOLEAN),	//MMIENG_NV_IS_CALL_BARRING
    sizeof(uint32),     	 //phone test item   0:no test 1:test
    sizeof(uint32),     	 //phone test result  0:fail 1:pass
    sizeof(BOOLEAN),      ////CMCC TEST FLAG, MS00216980
    sizeof(STANDBY_RECORD_T),  //power on time
    sizeof(uint32),     //standby time
    #ifdef MMI_AUTOTEST_SUPPORT
    sizeof(BOOLEAN)*MMI_AUTO_TEST_MAX, // @CR250530 renwei add  MMIENG_NV_AUTOTEST
    #endif
    sizeof(BOOLEAN),  //MMIENG_NV_POWER_KEY_SETTING 
    sizeof(BOOLEAN),  //MMIENG_NV_IS_REGISTER_PS_SERVICE
    sizeof(BOOLEAN),  //MMIENG_NV_IS_MONKEY_MODE
    sizeof(BOOLEAN),  //MMIENG_NV_IS_UART_LOG_OPEN
    sizeof(MMIENG_LTE_MODE_E),
    sizeof(MMIENG_LTE_MODE_E),
    sizeof(BOOLEAN),//MMIENG_NV_FORBID_ALL_GPRS_DATA
#ifdef ENG_BAND_FREQ_SUPPORT
    sizeof(MMIENG_BAND_FREQ_T),
    sizeof(MMIENG_BAND_FREQ_T)
#endif
};

/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_ENG, mmieng_nv_len_tab, ARR_SIZE(mmieng_nv_len_tab));
}
#endif//ENG_SUPPORT
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

