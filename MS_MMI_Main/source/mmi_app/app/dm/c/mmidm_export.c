/*************************************************************************
 ** File Name:      mmidm_export.c                                       *
 ** Author:         jian.ma                                              *
 ** Date:           2009/07/02                                           *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about dm function     *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/07/02     jian.ma          Create.                              *
*************************************************************************/
#define _MMIDM_EXPORT_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
//#include "std_header.h"
//#include "sci_types.h"
#include "mmi_app_dm_trc.h"
#include "mn_type.h"
#include "mmidm_export.h"
#ifdef DM_SUPPORT
#include "mmidm_id.h"
#include "mmidm_app.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
#ifdef DM_SUPPORT_REDBENCH
extern char* VDM_getVersion(void);
#endif
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
#ifdef DM_SUPPORT_DFRESH
LOCAL  char s_dfresh_version[] = {"Dfresh"};
#endif

/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : set the simdm will use
//	Global resource dependence : none
//  Author: maryxiao
//	Note
/*****************************************************************************/ 
PUBLIC void MMIAPIDM_SetMainSim(MN_DUAL_SYS_E sim)
{
    MMIDM_SetMainSim(sim);
}

/*****************************************************************************/
// 	Description : get the sim dm will use
//	Global resource dependence : none
//  Author: maryxiao
//	Note
/*****************************************************************************/  
PUBLIC MN_DUAL_SYS_E MMIAPIDM_GetMainSim(void)
{
    return MMIDM_GetMainSim();
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: jian.ma
//	Note
/*****************************************************************************/
PUBLIC void MMIAPIDM_Init(void)
{
    MMIDM_Init();
}

/*****************************************************************************/
// 	Description : intial dm module  
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_InitModule(void)
{
    MMIDM_RegMenuGroup();
    MMIDM_RegDmNv();
    MMIDM_RegWinIdNameArr();
}


/*****************************************************************************/
//  Discription: This function is used to open the DM main menu win
//  Global resource dependence: None
//  Author: jian.ma 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIDM_OpenMainMenuWin(void)
{
   MMIDM_OpenDmMainMenu();
}
/*****************************************************************************/
//  Discription: handle sms MT ind
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/	
PUBLIC  BOOLEAN MMIAPIDM_HandleSmsMtInd(
                                                                    MN_DUAL_SYS_E dual_sys,
                                       uint8   *number_ptr,
                                       uint16   num_len,
                                       APP_SMS_USER_DATA_T	sms_user_data
                                       )
{
    return MMIDM_HandleSmsMtInd(dual_sys, number_ptr,num_len,sms_user_data);
}

/*****************************************************************************/
// 	Description : 判断当前SIM卡跟IMEI号是否已经注册，若是，那么就不用再次发送自注册短信，若否，
//			则发送如下格式的自注册短信:
//			IMEI:IMEI number/manufacture/mode/sw version
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_SendSelfRegisterSMS(MN_DUAL_SYS_E		dual_sys)
{
    if (MMIDM_GetRegTimes()<1 &&DM_SELF_REG_NONE == MMIDM_GetIsSrReady())
    {
        MMIDM_SetSmsReady(TRUE);
        if (dual_sys < MN_DUAL_SYS_MAX)
        {
            MMIDM_SendSelfRegisterSMS(dual_sys);
        }
        else
        {
            MN_DUAL_SYS_E i = MN_DUAL_SYS_1;
            
            for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
            {
                if (MMIDM_SendSelfRegisterSMS(i))
                {
                    break;
                }
            }
        }        
    }
}

/*****************************************************************************/
// 	Description : handle dm message
//	Global resource dependence : none
//  Author:jian.ma 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDM_HandleDmMessage(DPARAM     sig_data_ptr)
{
#ifdef WIN32
    MMIDM_Test();
#else
    return MMIDM_HandleDmMessage(sig_data_ptr);
#endif
}

/*****************************************************************************/
// 	Description : reset dm factory setting
//	Global resource dependence : none
//  Author:jianhui.luo 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_ResetFactorySetting(void)
{
     MMIDM_ResetFactorySetting();
}
/*****************************************************************************/
// 	Description : open dm debug win
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_OpenDebugMenuWin(void)
{
    MMIDM_OpenDebugMenuWin();
}

/*****************************************************************************/
// 	Description : get fumo update timer id
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDM_IsDmTimerId(uint32 timer_id)
{
    return MMIDM_IsDmTimerId(timer_id);
}

/*****************************************************************************/
// 	Description : handle fumo update timer 
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandleDmTimer(uint8 timer_id, uint32 param)
{
    MMIDM_HandleDmTimer(timer_id, param);
}

/*****************************************************************************/
//  Description : handle socmo callback
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandleScomoCallback(int suiteID,
                                         uint16*version,
                                         uint16 version_len,
                                         uint16* name,
                                         uint16 name_len)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIAPIDM_HandleScomoCallback suiteID=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_EXPORT_180_112_2_18_2_13_53_176,(uint8*)"d",suiteID);
    if (0<suiteID)//install
    {
        MMIDM_ScomoInstallCallback(suiteID,version,version_len,name,name_len);
    }
    else if (0 >suiteID) //remove
    {
        MMIDM_ScomoRemoveCallback(suiteID);
    }
}

/*****************************************************************************/
//  Description : get pdp status
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMIDM_PDP_STATUS_E MMIAPIDM_GetPdpActiveStatus(void)
{
    return MMIDM_GetPdpActiveStatus();
}

/*****************************************************************************/
//  Description :handle dm rx change
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandDmRxChange(uint8 rx_level)
{
    MMIDM_HandDmRxChange(rx_level);
}

/*****************************************************************************/
//  Description :handle cc close 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandDmCallClose(void)
{
    MMIDM_HandDmCallClose();
}

/*****************************************************************************/
//  Description :handle cc close 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandWapClose(void)
{
    MMIDM_HandWapClose();
}

/*****************************************************************************/
//  Description :handle network change
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandleNetChange(MN_DUAL_SYS_E dual_sys)
{
    MMIDM_HandleNetChange(dual_sys);
}

/*****************************************************************************/
//  Description :hand set lock
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDM_GetHandsetLock(void)
{
    return MMIDM_GetHandsetLock();
}

/*****************************************************************************/
// 	Description : to handle dm idle option
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandleDmIdleOption(void)
{
    MMIDM_HandleDmIdleOption();
}
/*****************************************************************************/
// 	Description : to handle dm idle option
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDM_SetReg(BOOLEAN enable)
{
    MMIDM_SetDmRegSwitchState(enable);
}

/*****************************************************************************/
// 	Description : get dm version
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC char* MMIAPIDM_GetVersion(void)
{
#ifdef DM_SUPPORT_REDBENCH    
    return (char*)VDM_getVersion();   
#elif DM_SUPPORT_DFRESH
    return s_dfresh_version;
#else  
    return PNULL;
#endif
}
#else
#ifdef DM_ONLY_SELFREG_SUPPORT
#include "mmidm_app.h"
#endif
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: jian.ma
//	Note
/*****************************************************************************/
PUBLIC void MMIAPIDM_AppInit(void)
{

}

/*****************************************************************************/
// 	Description : intial dm module  
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_InitModule(void)
{
#ifdef DM_ONLY_SELFREG_SUPPORT
    MMIDM_RegMenuGroup();
    MMIDM_RegDmNv();
    MMIDM_RegWinIdNameArr();
#else
	return;
#endif
}
/*****************************************************************************/
//  Discription: This function is used to open the DM main menu win
//  Global resource dependence: None
//  Author: jian.ma 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIDM_OpenMainMenuWin(void)
{

}
/*****************************************************************************/
// 	Description : set the simdm will use
//	Global resource dependence : none
//  Author: maryxiao
//	Note
/*****************************************************************************/ 
PUBLIC void MMIAPIDM_SetMainSim(MN_DUAL_SYS_E sim)
{

}

/*****************************************************************************/
// 	Description : get the sim dm will use
//	Global resource dependence : none
//  Author: maryxiao
//	Note
/*****************************************************************************/  
PUBLIC MN_DUAL_SYS_E MMIAPIDM_GetMainSim(void)
{
    return MN_DUAL_SYS_1;
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: jian.ma
//	Note
/*****************************************************************************/
PUBLIC void MMIAPIDM_Init(void)
{
#ifdef DM_ONLY_SELFREG_SUPPORT
    MMIDM_Init();
#else
	return;
#endif
}

/*****************************************************************************/
//  Discription: handle sms MT ind
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/	
PUBLIC  BOOLEAN MMIAPIDM_HandleSmsMtInd( 
                                                                    MN_DUAL_SYS_E dual_sys,
                                                                    uint8   *number_ptr,
                                                                    uint16   num_len,
                                                                    APP_SMS_USER_DATA_T	sms_user_data)
{
#ifdef DM_ONLY_SELFREG_SUPPORT
    return MMIDM_HandleSmsMtInd(dual_sys, number_ptr,num_len,sms_user_data);
#else
    return FALSE;
#endif
}

/*****************************************************************************/
// 	Description : 判断当前SIM卡跟IMEI号是否已经注册，若是，那么就不用再次发送自注册短信，若否，
//			则发送如下格式的自注册短信:
//			IMEI:IMEI number/manufacture/mode/sw version
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_SendSelfRegisterSMS(MN_DUAL_SYS_E		dual_sys)
{
#ifdef DM_ONLY_SELFREG_SUPPORT
    if (MMIDM_GetRegTimes()<1 &&DM_SELF_REG_NONE == MMIDM_GetIsSrReady())
    {
        MMIDM_SetSmsReady(TRUE);
        if (dual_sys < MN_DUAL_SYS_MAX)
        {
            MMIDM_SendSelfRegisterSMS(dual_sys);
        }
        else
        {
            MN_DUAL_SYS_E i = MN_DUAL_SYS_1;
            
            for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
            {
                if (MMIDM_SendSelfRegisterSMS(i))
                {
                    break;
                }
            }
        }
    }
#else
	return;
#endif
}

/*****************************************************************************/
// 	Description : handle dm message
//	Global resource dependence : none
//  Author:jian.ma 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDM_HandleDmMessage(DPARAM     sig_data_ptr)
{

    return FALSE;
}

/*****************************************************************************/
// 	Description : reset dm factory setting
//	Global resource dependence : none
//  Author:jianhui.luo 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_ResetFactorySetting(void)
{

}
/*****************************************************************************/
// 	Description : open dm debug win
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_OpenDebugMenuWin(void)
{
#ifdef DM_ONLY_SELFREG_SUPPORT
    MMIDM_OpenDebugMenuWin();
#else
	return;
#endif
}

/*****************************************************************************/
// 	Description : get fumo update timer id
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDM_IsDmTimerId(uint32 timer_id)
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : handle fumo update timer 
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandleDmTimer(uint8 timer_id, uint32 param)
{

}

/*****************************************************************************/
//  Description : open lock win
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_OpenLockWin(void)
{

}
/*****************************************************************************/
//  Description : handle socmo callback
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandleScomoCallback(int suiteID,
                                         uint16*version,
                                         uint16 version_len,
                                         uint16* name,
                                         uint16 name_len)
{
}

/*****************************************************************************/
//  Description : get pdp status
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMIDM_PDP_STATUS_E MMIAPIDM_GetPdpActiveStatus(void)
{
    return MMIDM_PDP_STATUS_NONE;
}

/*****************************************************************************/
//  Description :handle dm rx change
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandDmRxChange(uint8 rx_level)
{

}
/*****************************************************************************/
//  Description :handle cc close 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandDmCallClose(void)
{

}

/*****************************************************************************/
//  Description :handle cc close 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandWapClose(void)
{

}

/*****************************************************************************/
//  Description :handle network change
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandleNetChange(MN_DUAL_SYS_E dual_sys)
{
#ifdef DM_ONLY_SELFREG_SUPPORT
    MMIDM_HandleNetChange(dual_sys);
#else
	return;
#endif
}

/*****************************************************************************/
//  Description :hand set lock
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDM_GetHandsetLock(void)
{
    return FALSE;
}
/*****************************************************************************/
// 	Description : to handle dm idle option
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDM_HandleDmIdleOption(void)
{

}
/*****************************************************************************/
// 	Description : to handle dm idle option
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDM_SetReg(BOOLEAN enable)
{

}
/*****************************************************************************/
// 	Description : get dm version
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC char* MMIAPIDM_GetVersion(void)
{   
    return PNULL;
}
#endif

