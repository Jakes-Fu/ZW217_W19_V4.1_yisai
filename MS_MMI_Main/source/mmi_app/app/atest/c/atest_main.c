/***************************************************************************************
** FileName    :  atest_main.c                                                        **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  01/06/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  The implementaion of the message dispatch function in ATEST module. **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  01/06/2010     Mingxuan.Zhu         Created                                       **
**  29/09/2010     Mingxuan.Zhu         Updated (delete ota function)                 **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#include "mmi_app_atest_trc.h"
#include "sci_api.h"
#include "atest_location.h"
#include "atest_ota.h"
#include "atest_signal.h"
#include "atest_export.h"
#include "atest_pb.h"
#include "atest_r8.h"
#include "atest_inputmethod.h"
#include "atest_calendar.h"
#include "atest_plmn.h"

/**-----------------------------------------------------------------------------------*
 **                               Constants and  Variables                            *
 **-----------------------------------------------------------------------------------*/


/**-----------------------------------------------------------------------------------*
 **                                Function Prototypes                                *
 **-----------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description      : create req signal.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1.ATEST_SIGNAL_E  e_sigid
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: shuyan.hu
// Note:
/**************************************************************************************/

LOCAL void ATEST_HandleREQMsg(ATEST_SIGNAL_E e_sigid);


/**-----------------------------------------------------------------------------------*
 **                                Function Implementation                            *
 **-----------------------------------------------------------------------------------*/

/**************************************************************************************/
// Description      : create req signal.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1.ATEST_SIGNAL_E  e_sigid
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: shuyan.hu
// Note:
/**************************************************************************************/

LOCAL void ATEST_HandleREQMsg(ATEST_SIGNAL_E e_sigid)
{
	ATEST_APP_SIG_T *signal_ptr = PNULL; 
    
    // create signal
    SCI_CREATE_SIGNAL( signal_ptr,
		CPANEL_ATEST_REQ,
		sizeof(ATEST_APP_SIG_T),
		SCI_IDENTIFY_THREAD() );
	
    // set the sig_ptr's param
     signal_ptr->signal_id = e_sigid;
    
    // send signal to control panel.
    SCI_SEND_SIGNAL((xSignalHeaderRec*)signal_ptr, P_CPANEL);
}



/**************************************************************************************/
// Description      : process the signal from control panel.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1.DPARAM  param
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC MMI_RESULT_E MMIAPI_ATESTMAINHandleAtestMsg(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ATEST_APP_DATA_T *signal_ptr = (ATEST_APP_DATA_T*)param;
    
    switch(signal_ptr->signal_id)
    {
    case ATEST_ENTER_IDLE_IND:  // start OTA testing.
    case ATEST_SEND_OTA_IND  :  
    case ATEST_END_OTA_CNF   :  
        ATESTOTA_TestOta(signal_ptr->signal_id);
        break;
	case ATEST_PB_RUN_IND:
		Atest_PbGetPathandNum(signal_ptr);
		atest_generatecases();
		atest_mmipbcode();
		ATEST_HandleREQMsg(ATEST_MC_PB_RUN_REQ);
		break;
	case ATEST_PB_SELF_IND:
	    Atest_PbGetConfigPath(signal_ptr);
		atest_mmipb_fl();
		ATEST_HandleREQMsg(ATEST_MC_PB_SELF_REQ);
		break;
	case ATEST_R8_RUN_IND:
		Atest_R8GetConfigPath(signal_ptr);
		 atest_r8();
         ATEST_HandleREQMsg(ATEST_MC_R8_RUN_REQ);
		break;
    case ATEST_LOCATION_IND:    // start location testing.
        //SCI_TRACE_LOW:"ATEST_TRACE====Enter LOCATION MODULE!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATEST_MAIN_113_112_2_18_2_3_0_1,(uint8*)"");
#ifdef WIN32
        ATESTLOC_TestPhoneLocation(signal_ptr);
#endif//WIN32
        break;
	case ATEST_INPUTMETHOD_RUN_IND:// start imputmethod testing
	    Atest_InputMethodTesting(signal_ptr->check_type,signal_ptr->result_file_path);
        break;
	case ATEST_CALENDAR_RUN_IND:// start imputmethod testing
	    Atest_GetLunarDataFromMocor(signal_ptr->result_file_path);
        break;
	case ATEST_PLMN_RUN_IND:
		Atest_GetPLMNDataFromMocor(signal_ptr->result_file_path);
        break;
    default:
        break;
    }
    return result;
}