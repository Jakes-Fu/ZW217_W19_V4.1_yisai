
/*****************************************************************************
** File Name:      mmidcd.c                                                  *
** Author:                                                                   *
** Date:           09/2007                                                   *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    											                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2007        violent.wei      Create
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_cs_trc.h"
#include "std_header.h"
#include "sig_code.h"
#include "window_parse.h"
#include "mmifmm_export.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"

#ifdef CMCC_UI_STYLE	//!宏定义开头

#include "mmics_main.h"
#include "mmics_export.h"
#include "mmics_id.h"
#include "mmics_nv.h"
#include "mmics_wintab.h"
#include "mmipb_export.h"
#include "mmiidle_export.h"
#include "mmiudisk_export.h"
#include "mmipub.h"
#include "mmiphone_export.h"
#include "mmics_text.h"
#include "mmics_image.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h" 
#include "mmics_export.h"
#include "mmi_appmsg.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/  

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

static MMICS_SETTING_INFO_T   s_mmics_setting_info = {0};

static CS_SigDispatch s_SigDispatch = PNULL;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description :get current mcc
//  Global resource dependence :
//  Author:
//  Note: app interface
/*****************************************************************************/
LOCAL void CS_SigDispatchReg(CS_SigDispatch SigDispatch);
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author: z.f.for dcd_sim
//	Note:
/*****************************************************************************/
LOCAL void MMICS_SigSend(uint16 sig_code,uint16 para);

/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/

PUBLIC MMICS_SETTING_INFO_T*  MMICS_GeSettingInfo(void)
{
    return &s_mmics_setting_info;
}

/****************************************************************************/
//  Description: 设置信号分发函数
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/26
/****************************************************************************/
PUBLIC void  MMICS_SetSigDispatchFun(CS_SigDispatch cs_sig)
{
    s_SigDispatch = cs_sig;
}
/****************************************************************************/
//  Description: 获取信号分发函数
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/26
/****************************************************************************/
PUBLIC CS_SigDispatch  MMICS_GetSigDispatchFun(void)
{
    return s_SigDispatch;
}


/*****************************************************************************/
//  Description :  dcd send sig to mmi
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMICS_Sendsig(uint16 signal_code,uint16 para)
{
    CS_SigDispatch cs_sigfunc = MMICS_GetSigDispatchFun();
    if(PNULL != cs_sigfunc)
    {
        cs_sigfunc(signal_code,para);
    }

} 

/*****************************************************************************/
// Description :  dcd handle http msg function!
// Global resource dependence : 
// Author:Zhuo Wang
// Note: modified by jun.hu 2009/11/12
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleCsProcessMsg(
                                        PWND app_ptr,
                                        uint16 msg_id,
                                        DPARAM param
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    DPARAM para = PNULL;

    //SCI_TRACE_LOW:"HandleDCDProcessMsg() enter msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICS_MAIN_137_112_2_18_2_12_25_1,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    default:
		break;
    }


    //SCI_TRACE_LOW:"HandleDCDProcessMsg()  exit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICS_MAIN_146_112_2_18_2_12_25_2,(uint8*)"");

    return (result);
	
}


/*****************************************************************************/
// 	Description : write setting info to nv
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC void MMICS_WriteSettingToNv(void)
{
    MMINV_WRITE(MMINV_CS_SET_INFO, MMICS_GeSettingInfo());
    //SCI_TRACE_LOW:"[MMIDCD] MMICS_WriteSettingToNv !! "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICS_MAIN_160_112_2_18_2_12_25_3,(uint8*)"");
}

/*****************************************************************************/
// 	Description : read setting info from nv
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC void MMICS_SetSettingDefault(void)
{
	const wchar     test_str[] = {L"10086"};
	//SCI_TRACE_LOW:"[MMIDCD]:: MMICS_SetSettingDefault enter! "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICS_MAIN_170_112_2_18_2_12_25_4,(uint8*)"");
       SCI_MEMSET(&s_mmics_setting_info, 0, sizeof(MMICS_SETTING_INFO_T));
       MMIAPICOM_Wstrcpy(MMICS_GeSettingInfo()->hotline_string, test_str);
	MMINV_WRITE(MMINV_CS_SET_INFO, &s_mmics_setting_info);
	

}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author: z.f.for dcd_sim
//	Note:
/*****************************************************************************/
LOCAL void MMICS_SigSend(uint16 sig_code,uint16 para)
{
    MmiAppCopyMsgS   *sendSignal = PNULL;
    MmiCreateSignal(sig_code,sizeof(MmiAppCopyMsgS),(MmiSignalS**)&sendSignal);
    sendSignal ->data =para;
    MmiSendSignal(P_APP,(MmiSignalS*)sendSignal);
}


/*****************************************************************************/
//  Description :get current mcc
//  Global resource dependence :
//  Author:
//  Note: app interface
/*****************************************************************************/
LOCAL void CS_SigDispatchReg(CS_SigDispatch SigDispatch)
{
    if(PNULL != SigDispatch)
    {
        MMICS_SetSigDispatchFun(SigDispatch);
    }
}
/****************************************************************************
   Description : DCD task init handler
   Global resource dependence : 
   Author:Zhuo.Wang
   Note: 
            first set some param
            parser default xml file in fs

 ****************************************************************************/
PUBLIC void MMICS_InitStart(void)
{

    CS_SigDispatchReg(MMICS_SigSend);
    
    
}




/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif /*#ifdef MMI_ENABLE_DCD*/    


