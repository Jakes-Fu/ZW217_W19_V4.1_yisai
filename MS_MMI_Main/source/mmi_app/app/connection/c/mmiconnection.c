/*****************************************************************************
** File Name:      mmirecord.c                                               *
** Author:         ryan.xu                                                   *
** Date:           2007/1/31                                                 *
** Copyright:      2007/1/31 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file is used to describe recorder                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/1/31      ryan.xu	       Create                                    *
******************************************************************************/

#define _MMICONNECTION_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_connection_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "os_api.h"
//#include "sci_types.h" 
//#include "sig_code.h"
#include "mmk_type.h"
#include "mmk_app.h"
//#include "mmi_id.h"
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiidle_export.h"
#include "mn_api.h"
#include "mmiconnection_export.h"
#include "mmiconnection_id.h"
#include "app_tcp_if.h"
#include "mmiconnection_internal.h"
//////////////////////////////////////////////////////////////////////////
// added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
#include "mmi_nv.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  FUNCTION:      MMIAPICONNECTION_InitModule
// 	Description :   
//	Global resource dependence : 
//  Author:        ryan.xu
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_InitModule(void)
{
    MMIAPICONNECTION_RegMenuGroup();
    MMICONNECTION_RegWinIdNameArr();
}

/*****************************************************************************/
// 	Description : init connection
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_Init(void)
{
    MMICONNECTION_Init();
}

/*****************************************************************************/
//  Description : Reset Factory Settings
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_ResetFactorySetting(BOOLEAN is_clean_user_data)
{
#ifdef MMI_GPRS_SUPPORT
    MMICONNECTION_GPRS_SETTINGS_E gprs_settings = MMICONNECTION_GPRS_ATTACH_ONNEED;

    MMICONNECTION_ResetSettingList(is_clean_user_data);

#ifdef MMI_DUALMODE_ENABLE
#ifdef CMCC_UI_STYLE
    gprs_settings = MMICONNECTION_GPRS_ALWAYS;
#else
    gprs_settings = MMICONNECTION_GPRS_ALWAYS_LC;
#endif
#ifndef WIN32
    {
        uint32 i = 0;

        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            MNGPRS_SetEnergySavingMode(i, TRUE);
            SCI_TRACE_LOW("MNGPRS_SetEnergySavingMode TRUE by default");
        }
    }
#endif
#else
#ifdef CMCC_UI_STYLE
    gprs_settings = MMICONNECTION_GPRS_ALWAYS;
#else
    gprs_settings = MMICONNECTION_GPRS_ATTACH_ONNEED;
#endif
#endif

#ifdef MMI_GPRS_SWITCH_SUPPORT
    MMIAPICONNECTION_SetGPRSSwitchStatus(MMICONNECTION_SETTING_GPRS_SWITCH_ON);
#endif
    MMIAPICONNECTION_SetGPRSStatus(gprs_settings);

    if (!MMIAPIPDP_PsIsPdpLinkExist()
        && (MMIAPICONNECTION_isGPRSNeedAutoAttach()))//不存在PDP链路做Detach操作
    {
        MMIAPIPHONE_SetGPRSDetach();
    }
#endif
}
/*****************************************************************************/
// 	Description : change th ip address from uint32 type to string, 
//	Global resource dependence :
//  Author: bei.wu 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_IpNumToStr(uint32 ip_num, uint8* ip_str)
{
    if (PNULL != ip_str)
    {
        sprintf((char *)ip_str, "%d.%d.%d.%d", (ip_num&0xff000000)>>24, (ip_num&0x00ff0000)>>16, (ip_num&0x0000ff00)>>8, ip_num&0xff);
    }
}

/*****************************************************************************/
// 	Description : change th ip address string to uint32 type, 
//	Global resource dependence :
//  Author: bei.wu 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_IpStrToNum(uint8* ip_str, uint32* ip_num)
{
#ifdef MMI_GPRS_SUPPORT
    int ret =0;
    uint32 temp = 0;

    if (PNULL == ip_str || PNULL == ip_num)
    {
        return FALSE;
    }

    ret = inet_aton((char *)ip_str, &temp);

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
    *ip_num = temp;
#else
    *ip_num = (temp >> 24) + ((temp & 0xFF0000) >> 8) + ((temp & 0xFF00) << 8) + ((temp & 0xFF) << 24);
#endif

    if(ret)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else
    return FALSE;
#endif/*MMI_GPRS_SUPPORT*/
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    Start of Atest  Function                                                                */
/* Description:  Hanlde at cmd for connection auto test cases                                                   */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : input gateway and dns ip 
//	Global resource dependence :
//  Author: shuyan.hu 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN Atest_CONNECTIONGATEWAY_DNS_InsertStr(                                            
                                                                                                      uint8 *gateway_ptr                 //point to ip information buffer
                                                                                                                   )
{
      uint32 ipaddr = 0;
      BOOLEAN res = FALSE;
      //check if the gateway input interface is opend
      if(MMK_IsOpenWin(MMICONNECTION_INPUTGATEWAY_WIN_ID) && (PNULL != gateway_ptr))
      {
	     //transform ip address
	     MMIAPICONNECTION_IpStrToNum(gateway_ptr, &ipaddr);
	     //insert ip into editbox   
	     GUIEDIT_SetIP(MMICONNECTION_EDIT_INPUTGATEWAY_CTRL_ID, ipaddr);
	     //refresh the interface and show the input ip
	     MMK_SendMsg(MMICONNECTION_INPUTGATEWAY_WIN_ID, MSG_FULL_PAINT, PNULL);
	     res = TRUE;
      }
       //check if the dns input interface is opend
      else if(MMK_IsOpenWin(MMICONNECTION_SETTINGLINK_DNS_WIN_ID)&& (PNULL != gateway_ptr))
      {
	     //transform ip address
	     MMIAPICONNECTION_IpStrToNum(gateway_ptr, &ipaddr);
	     //insert ip into editbox    
	     GUIEDIT_SetIP(MMICONNECTION_EDIT_INPUT_DNS_CTRL_ID, ipaddr);
	     //refresh the interface and show the input ip
	     MMK_SendMsg(MMICONNECTION_SETTINGLINK_DNS_WIN_ID, MSG_FULL_PAINT, PNULL);
	     res = TRUE;
      }
      return res;
	 
}



/*****************************************************************************/
// 	Description : MMIAPICONNECTION_InsertStr
//	Global resource dependence :
//  Author: bei.wu 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_InsertStr(                                            //RETURN:
                             wchar *wstr_ptr,
				 uint16 wstr_len
                             )
{
    BOOLEAN result = FALSE;
    
    if(MMK_IsOpenWin(MMICONNECTION_INPUTUSERNAME_WIN_ID) && (PNULL != wstr_ptr) && (wstr_len > 0))
    {
        GUIEDIT_InsertString(
        	MMICONNECTION_EDIT_INPUTUSERNAME_CTRL_ID, 
        	wstr_ptr,
        	wstr_len
        	);
        MMK_SendMsg(MMICONNECTION_INPUTUSERNAME_WIN_ID, MSG_FULL_PAINT, PNULL);
        result = TRUE;
    }
    else if(MMK_IsOpenWin(MMICONNECTION_INPUTAPN_WIN_ID) && (PNULL != wstr_ptr) && (wstr_len > 0))
    {
        GUIEDIT_InsertString(
        	MMICONNECTION_EDIT_INPUTAPN_CTRL_ID, 
        	wstr_ptr,
        	wstr_len
        	);
        MMK_SendMsg(MMICONNECTION_INPUTAPN_WIN_ID, MSG_FULL_PAINT, PNULL);
        result = TRUE;
    }
    else if(MMK_IsOpenWin(MMICONNECTION_INPUTAGENTNAME_WIN_ID) && (PNULL != wstr_ptr) && (wstr_len > 0))
    {
        GUIEDIT_InsertString(
        	MMICONNECTION_EDIT_INPUTAGENTNAME_CTRL_ID, 
        	wstr_ptr,
        	wstr_len
        	);
        MMK_SendMsg(MMICONNECTION_INPUTAGENTNAME_WIN_ID, MSG_FULL_PAINT, PNULL);
        result = TRUE;
    }
    else if(MMK_IsOpenWin(MMICONNECTION_INPUTHOMEPAGE_WIN_ID) && (PNULL != wstr_ptr) && (wstr_len > 0))
    {
        GUIEDIT_InsertString(
        	MMICONNECTION_EDIT_INPUTHOMEPAGE_CTRL_ID, 
        	wstr_ptr,
        	wstr_len
        	);
        MMK_SendMsg(MMICONNECTION_INPUTHOMEPAGE_WIN_ID, MSG_FULL_PAINT, PNULL);
        result = TRUE;
    }
    //add by shuyan.hu at 2010.10 28
    //check if the password input interface is opend
    else if(MMK_IsOpenWin(MMICONNECTION_INPUTPASSWORD_WIN_ID) && (PNULL != wstr_ptr) && (wstr_len > 0))
    {
        //insert password into editbox 
        GUIEDIT_InsertString(
        	MMICONNECTION_EDIT_INPUTPASSWORD_CTRL_ID, 
        	wstr_ptr,
        	wstr_len
        	);
	//refresh the interface and show the input information	
        MMK_SendMsg(MMICONNECTION_INPUTPASSWORD_WIN_ID, MSG_FULL_PAINT, PNULL);
        result = TRUE;
    }
    //check if the port input interface is opend	
    else if(MMK_IsOpenWin(MMICONNECTION_EDITPORT_WIN_ID) && (PNULL != wstr_ptr) && (wstr_len > 0))
    {
         //insert port into editbox 
        GUIEDIT_InsertString(
        	MMICONNECTION_EDIT_INPUTPORT_CTRL_ID, 
        	wstr_ptr,
        	wstr_len
        	);
	//refresh the interface and show the input information	
        MMK_SendMsg(MMICONNECTION_EDITPORT_WIN_ID, MSG_FULL_PAINT, PNULL);
        result = TRUE;
    }	
    
    return result;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    End of Atest  Function                                                                   */
/* Description:  Hanlde at cmd for connection auto test cases                                                   */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
//  Description : Auto Adapting Network account
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_AutoAdapting(MN_DUAL_SYS_E dual_sys)
{
    MMICONNECTION_AutoAdapting(dual_sys);
}

/*****************************************************************************/
//  Description : Auto Adapting specified account
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_AdaptingAccount(MN_DUAL_SYS_E dual_sys, MMICONNECTION_ACCOUNT_TYPE_E account_type)
{
    MMICONNECTION_AdaptingAccount(dual_sys, account_type);
}

/*****************************************************************************/
// 	Description : create Linklist Win for mms,browser,dcd,java
//	Global resource dependence : none
//  Author: jixin.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_CreateLinklistWin(MMICONNECTION_ACCOUNT_TYPE_E account_type, uint8 index,
                                               MN_DUAL_SYS_E dual_sys, MMICONNECTION_CALLBACK_FUNC callback_func)
{
    uint32 add_data = 0;

    add_data = (((account_type & 0xff) << 16) | ((index & 0xff) << 8) | (dual_sys & 0xff));
    MMICONNECTION_CreateLinklistWin((ADD_DATA)add_data, callback_func);
}
//////////////////////////////////////////////////////////////////////////
// added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
/*****************************************************************************/
// 	Description : get data roaming setting status
//	Global resource dependence : none
//  Author: feng.xiao
//	Note:
/*****************************************************************************/
PUBLIC MMICONNECTION_SETTING_DATA_ROAMING_E MMAPIICONNECTION_Setting_GetDataRoamingStatus(void)
{
    MMICONNECTION_SETTING_DATA_ROAMING_E data_roaming_status = MMICONNECTION_SETTING_DATA_ROAMING_DISABLE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_CONNECTION_DATA_ROAMING, &data_roaming_status, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        data_roaming_status = MMICONNECTION_SETTING_DATA_ROAMING_DEFAULT;
        MMINV_WRITE(MMINV_CONNECTION_DATA_ROAMING, &data_roaming_status);
    }

    //SCI_TRACE_LOW:"MMICONNECTION_Setting_GetDataRoamingStatus data_roaming_status=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICONNECTION_347_112_2_18_2_12_21_27,(uint8*)"d", data_roaming_status);
    return data_roaming_status;
}

/*****************************************************************************/
// 	Description : set data roaming setting status
//	Global resource dependence : none
//  Author: feng.xiao
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICONNECTION_Setting_SetDataRoamingStatus(MMICONNECTION_SETTING_DATA_ROAMING_E data_raoming_status)
{
    if (data_raoming_status < MMICONNECTION_SETTING_DATA_ROAMING_MAX)
    {
        MMINV_WRITE(MMINV_CONNECTION_DATA_ROAMING, &data_raoming_status);
    }
}

/*****************************************************************************/
// 	Description : whether activated pdp link exist when data roaming
//	Global resource dependence : none
//  Author: feng.xiao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICONNECTION_IsRoamingActivedPdpLinkExist(void)
{
    int i = 0;
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (MMIPHONE_ROAMING_STATUS_NATIONAL_ROMING == MMIAPIPHONE_GetRoamingStatus((MN_DUAL_SYS_E)i) && 
            MMIAPIPDP_PsIsActivedPdpLinkExistEx((MN_DUAL_SYS_E)i))
        {
            return TRUE;
        }
    }
    return FALSE;
}
#endif
#endif
