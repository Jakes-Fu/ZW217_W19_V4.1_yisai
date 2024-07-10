/****************************************************************************
** File Name:      mmimms_setting.c                                        *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_mms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mn_type.h"
#include "mmi_modu_main.h"
#include "mmi_nv.h"
#include "mmimms_setting.h"
#include "mmimms_control_main.h"
#include "mmimms_file.h"
#include "mmisms_export.h"
#include "mmiconnection_export.h"
#include "nvitem.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : update mms net setting when the index list item is deleted  
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void MMIMMS_Setting_ConnectionCallback(MMICONNECTION_NOTIFY_EVENT_E notify_event, uint32 param, MN_DUAL_SYS_E dual_sys);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Reset the mms setting to factory setting 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_NV_SETTING_T MMIMMS_ResetMMSFactorySetting(void)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    uint32 i =0;
    

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_ResetMMSFactorySetting"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_59_112_2_18_2_38_55_393,(uint8*)"");
    nv_setting.is_need_delivery_rpt_recv = FALSE;
    nv_setting.is_need_delivery_rpt_send = FALSE;
    nv_setting.is_need_read_rpt_send = FALSE;
    nv_setting.is_anonymous = FALSE;
    nv_setting.is_play_sound = TRUE;
    nv_setting.is_advertisingallowable = TRUE;
    nv_setting.priority = MMIMMS_SETTING_PRIORITY_NORMAL;
//////////////////////////////////////////////////////////////////////////
// modified by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
    nv_setting.retrieve_mode = MMIMMS_SETTING_RETRIEVE_MODE_IMMEDIATE;
	nv_setting.retrieve_mode_roam = MMIMMS_SETTING_RETRIEVE_MODE_DEFER;
#else
    nv_setting.retrieve_mode = MMIMMS_SETTING_RETRIEVE_AUTO;
#ifdef MMIMMS_SET_ROAMING_DOWNLOAD
    nv_setting.roaming_retrieve_mode = MMIMMS_SETTING_RETRIEVE_MANUAL;
#endif
#endif
    nv_setting.valid_period = MMIMMS_SETTING_VALID_PERIOD_LONGEST; 
    nv_setting.is_auto_sign = FALSE;
    nv_setting.is_extract_content = TRUE;
#ifdef NANDBOOT_SUPPORT
    nv_setting.device_type = MMI_DEVICE_UDISK;
#else
    nv_setting.device_type = MMIAPIFMM_GetDefaultDisk();
#endif
    //set sim index : setting_search_table中的第2条设置
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        nv_setting.sim_index[i] = MMIAPICONNECTION_GetAccountDefaultIndex((MN_DUAL_SYS_E)i, MMICONNECTION_ACCOUNT_MMS);
    }

    MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);

    MMIAPICONNECTION_AdaptingAccount(MMI_DUAL_SYS_MAX, MMICONNECTION_ACCOUNT_MMS);

    return nv_setting;
}

/*****************************************************************************/
//     Description : get NDT nv setting
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMIMMS_GetNDTNvSetting(
                                       uint16 item_id, void* setting
                                       )
{
    uint16 nv_len = 0;
    MN_RETURN_RESULT_E ret_value = MN_RETURN_FAILURE;
    NVITEM_ERROR_E nv_reslut = NVERR_NONE;
    nv_len = NVITEM_GetLength(item_id);
    nv_reslut = EFS_NvitemRead(item_id, nv_len, (uint8*)setting);
    if(NVERR_NONE == nv_reslut)
    {
        ret_value = MN_RETURN_SUCCESS;
    }
    return ret_value;
}

/*****************************************************************************/
//  Description : init mms setting 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_InitSetting(void)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;
    MMICONNECTION_NOTIFY_INFO_T notify_info = {0};

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_InitSetting"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_107_112_2_18_2_38_56_394,(uint8*)"");
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
       MMIMMS_ResetMMSFactorySetting();
    }
    notify_info.module_id = MMI_MODULE_MMS;
    notify_info.notify_func = MMIMMS_Setting_ConnectionCallback;
    MMIAPICONNECTION_RegisterNotifyInfo(&notify_info);
}

/*****************************************************************************/
//  Description : Get delivery report 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Setting_GetDeliveryReport(void)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_GetDeliveryReport deliver_report=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_133_112_2_18_2_38_56_395,(uint8*)"d",nv_setting.is_need_delivery_rpt_send);
    return nv_setting.is_need_delivery_rpt_send;
}

/*****************************************************************************/
//  Description : Set delivery report 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetDeliveryReport(BOOLEAN is_need_delivery_rpt_send)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_SetDeliveryReport is_need_delivery_rpt_send=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_146_112_2_18_2_38_56_396,(uint8*)"d",is_need_delivery_rpt_send);
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    nv_setting.is_need_delivery_rpt_send = is_need_delivery_rpt_send;
    MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
}

/*****************************************************************************/
//  Description : Get read report 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Setting_GetReadReport(void)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_GetReadReport read_rpt=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_171_112_2_18_2_38_56_397,(uint8*)"d",nv_setting.is_need_read_rpt_send);
    return nv_setting.is_need_read_rpt_send;
}

/*****************************************************************************/
//  Description : Set read report 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetReadReport(BOOLEAN is_need_read_rpt_send)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_SetReadReport is_need_read_rpt_send=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_184_112_2_18_2_38_56_398,(uint8*)"d",is_need_read_rpt_send);
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    nv_setting.is_need_read_rpt_send = is_need_read_rpt_send;
    MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
}

/*****************************************************************************/
//  Description : Get anonymous 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Setting_GetAnonymous(void)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_GetAnonymous is_anonymous=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_209_112_2_18_2_38_56_399,(uint8*)"d",nv_setting.is_anonymous);
    return nv_setting.is_anonymous;
}

/*****************************************************************************/
//  Description : Set anonymous 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetAnonymous(BOOLEAN is_anonymous)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_SetAnonymousReport is_anonymous=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_222_112_2_18_2_38_56_400,(uint8*)"d",is_anonymous);
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    nv_setting.is_anonymous = is_anonymous;
    MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
}

/*****************************************************************************/
//  Description : Get play sound status 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Setting_GetPlaysound(void)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_GetPlaysound is_play_sound=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_247_112_2_18_2_38_56_401,(uint8*)"d",nv_setting.is_play_sound);
    return nv_setting.is_play_sound;
}

/*****************************************************************************/
//  Description : Set delivery report 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetPlaysound(BOOLEAN is_play_sound)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_SetPlaysound is_play_sound=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_260_112_2_18_2_38_56_402,(uint8*)"d",is_play_sound);
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    nv_setting.is_play_sound = is_play_sound;
    MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
}

/*****************************************************************************/
//  Description : Get advertising setting
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Setting_GetAdvertising(void)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"MMIMMS_Setting_GetAdvertising"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_280_112_2_18_2_38_56_403,(uint8*)"");
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    return nv_setting.is_advertisingallowable;
}

/*****************************************************************************/
//  Description : Set advertising setting 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetAdvertising(BOOLEAN is_advertisingallowable)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"MMIMMS_Setting_SetAdvertising"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_299_112_2_18_2_38_56_404,(uint8*)"");
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    nv_setting.is_advertisingallowable = is_advertisingallowable;
    MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
}


/*****************************************************************************/
//  Description : Get extract content status 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Setting_GetExtractContent(void)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_GetExtractContent is_extract_content=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_325_112_2_18_2_38_56_405,(uint8*)"d",nv_setting.is_extract_content);
    return nv_setting.is_extract_content;
}

/*****************************************************************************/
//  Description : Set extract content status  
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetExtractContent(BOOLEAN is_extract_content)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_SetExtractContent is_extract_content=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_338_112_2_18_2_38_56_406,(uint8*)"d",is_extract_content);
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    nv_setting.is_extract_content = is_extract_content;
    MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
}

/*****************************************************************************/
//  Description : Get priority setting 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_SETTING_PRIORITY_E MMIMMS_Setting_GetPriority(void)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_GetPriority priority=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_363_112_2_18_2_38_56_407,(uint8*)"d",nv_setting.priority);
    return ((MMIMMS_SETTING_PRIORITY_E)nv_setting.priority);
}

/*****************************************************************************/
//  Description : Get priority setting 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetPriority(MMIMMS_SETTING_PRIORITY_E priority)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_SetPriority priority=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_376_112_2_18_2_38_56_408,(uint8*)"d",priority);
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    nv_setting.priority = priority;
    MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
}

/*****************************************************************************/
//  Description : Get retrieve mode 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
#ifndef DATA_ROAMING_SUPPORT
PUBLIC MMIMMS_SETTING_RETRIEVE_E MMIMMS_Setting_GetRetrieveMode(void)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_GetRetrieveMode retrieve_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_402_112_2_18_2_38_56_409,(uint8*)"d",nv_setting.retrieve_mode);
    return ((MMIMMS_SETTING_RETRIEVE_E)nv_setting.retrieve_mode);
}
#ifdef MMIMMS_SET_ROAMING_DOWNLOAD
/*****************************************************************************/
//  Description : Get roaming retrieve mode 
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_SETTING_RETRIEVE_E MMIMMS_Setting_GetRoamingRetrieveMode(void)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_GetRetrieveMode retrieve_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_402_112_2_18_2_38_56_409,(uint8*)"d",nv_setting.retrieve_mode);
    return ((MMIMMS_SETTING_RETRIEVE_E)nv_setting.roaming_retrieve_mode);
}
#endif

#endif

#ifndef DATA_ROAMING_SUPPORT
/*****************************************************************************/
//  Description : Set retrieve mode 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetRetrieveMode(MMIMMS_SETTING_RETRIEVE_E retrieve_mode)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_SetRetrieveMode retrieve_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_417_112_2_18_2_38_56_410,(uint8*)"d",retrieve_mode);
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    nv_setting.retrieve_mode = retrieve_mode;
    MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
}
#ifdef MMIMMS_SET_ROAMING_DOWNLOAD
/*****************************************************************************/
//  Description : Set roaming retrieve mode 
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetRoamingRetrieveMode(MMIMMS_SETTING_RETRIEVE_E retrieve_mode)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_SetRetrieveMode retrieve_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_417_112_2_18_2_38_56_410,(uint8*)"d",retrieve_mode);
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    nv_setting.roaming_retrieve_mode = retrieve_mode;
    MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
}
#endif
#endif

/*****************************************************************************/
//  Description : Get valid period 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_SETTING_VALID_PERIOD_E MMIMMS_Setting_GetValidPeriod(void)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_GetValidPeriod valid_period=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_443_112_2_18_2_38_56_411,(uint8*)"d",nv_setting.valid_period);
    return ((MMIMMS_SETTING_VALID_PERIOD_E)nv_setting.valid_period);
}

/*****************************************************************************/
//  Description : Set valid period 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetValidPeriod(MMIMMS_SETTING_VALID_PERIOD_E valid_period)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_SetValidPeriod valid_period=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_456_112_2_18_2_38_56_412,(uint8*)"d",valid_period);
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    nv_setting.valid_period = valid_period;
    MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
}

/*****************************************************************************/
//  Description : Get recv delivery report status
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Setting_GetRecvDeliveryReport(void)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_GetRecvDeliveryReport is_need_delivery_rpt_recv=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_481_112_2_18_2_38_56_413,(uint8*)"d",nv_setting.is_need_delivery_rpt_recv);
    return nv_setting.is_need_delivery_rpt_recv;
}

/*****************************************************************************/
//  Description : Set recv delivery report status 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetRecvDeliveryReport(BOOLEAN is_need_delivery_rpt_recv)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_SetRecvDeliveryReport is_need_delivery_rpt_recv=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_494_112_2_18_2_38_56_414,(uint8*)"d",is_need_delivery_rpt_recv);
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    nv_setting.is_need_delivery_rpt_recv = is_need_delivery_rpt_recv;
    MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
}

/*****************************************************************************/
//  Description : Get prefer disk
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIMMS_Setting_GetPreferDisk(void)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"MMIMMS_Setting_GetPreferDisk"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_514_112_2_18_2_38_56_415,(uint8*)"");
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_GetPreferDisk device_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_521_112_2_18_2_38_56_416,(uint8*)"d",nv_setting.device_type);
    return nv_setting.device_type;
}

/*****************************************************************************/
//  Description : Set prefer disk 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetPreferDisk(MMIFILE_DEVICE_E device_type)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_SetPreferDisk device_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_534_112_2_18_2_38_56_417,(uint8*)"d",device_type);
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    if(device_type < MMI_DEVICE_NUM)
    {
        nv_setting.device_type = device_type;
        MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
    }
}

/*****************************************************************************/
//  Description : Get mms net setting index 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMMS_Setting_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys)
{
    uint8               index = 0;
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;
 
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }
    
    index = nv_setting.sim_index[dual_sys];
    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_GetNetSettingIndex,dual_sys=%d,index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_564_112_2_18_2_38_57_418,(uint8*)"dd",dual_sys,index);
    return index;
}

/*****************************************************************************/
//  Description : Set mms net setting index  
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetNetSettingIndex(
                                               MN_DUAL_SYS_E dual_sys,
                                               uint8         index
                                               )
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_SetNetSettingIndex dual_sys=%d index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_580_112_2_18_2_38_57_419,(uint8*)"dd",dual_sys,index);
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }

    nv_setting.sim_index[dual_sys] = index;
    
    MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
}

/*****************************************************************************/
//  Description : update mms net setting when the index list item is deleted  
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void MMIMMS_Setting_ConnectionCallback(MMICONNECTION_NOTIFY_EVENT_E notify_event, uint32 param, MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN need_write = FALSE;
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;
    uint8 index = (uint8)param;

    //SCI_TRACE_LOW:"MMIMMS_Setting_ConnectionCallback,index=%d,dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_604_112_2_18_2_38_57_420,(uint8*)"dd",index, dual_sys);
    
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        need_write = TRUE;
        nv_setting = MMIMMS_ResetMMSFactorySetting();
        //return;
    }
    
    switch(notify_event)
    {
    case MMICONNECTION_NOTIFY_EVENT_DEL_ITEM:
        if (dual_sys < MMI_DUAL_SYS_MAX)
        {
            //小于的情况不用处理
            if(nv_setting.sim_index[dual_sys] == index)
            {
                need_write = TRUE;
                nv_setting.sim_index[dual_sys] = MMIMMS_SETTING_DEFAULT_INDEX;
            }
            else if(nv_setting.sim_index[dual_sys] > index)
            {
                need_write = TRUE;
                nv_setting.sim_index[dual_sys]--;
            }
        }
        
        if(need_write)
        {
            MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
            MMIAPICONNECTION_AdaptingAccount(dual_sys, MMICONNECTION_ACCOUNT_MMS);
        } 
        break;

    default:
        break;
    }
     
}

//////////////////////////////////////////////////////////////////////////
// added by feng.xiao
#ifdef DATA_ROAMING_SUPPORT
/*****************************************************************************/
//  Description : get retrieve mode from nv  
//  Global resource dependence : none
//  Author: feng.xiao
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_SETTING_RETRIEVE_MODE_E MMIMMS_Setting_GetRetrieveMode(MMIMMS_SETTING_TYPE_E type)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;
    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }

    if (MMIMMS_SETTING_TYPE_LOCAL == type)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_GetRetrieveModeLocal RetrieveModeLocal=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_664_112_2_18_2_38_57_421,(uint8*)"d",nv_setting.retrieve_mode);
        return ((MMIMMS_SETTING_RETRIEVE_MODE_E)nv_setting.retrieve_mode);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_GetRetrieveModeRoam RetrieveModeRoam=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_669_112_2_18_2_38_57_422,(uint8*)"d",nv_setting.retrieve_mode_roam);
        return ((MMIMMS_SETTING_RETRIEVE_MODE_E)nv_setting.retrieve_mode_roam);
    }
}

/*****************************************************************************/
//  Description : set retrieve mode 2 nv  
//  Global resource dependence : none
//  Author: feng.xiao
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_Setting_SetRetrieveMode(MMIMMS_SETTING_RETRIEVE_MODE_E retrieve_mode, 
                                           MMIMMS_SETTING_TYPE_E type)
{
    MMIMMS_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_MMS_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIMMS_ResetMMSFactorySetting();
    }

    if (MMIMMS_SETTING_TYPE_LOCAL == type)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_SetRetrieveModeLocal RetrieveModeLocal=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_692_112_2_18_2_38_57_423,(uint8*)"d", retrieve_mode);
        nv_setting.retrieve_mode = retrieve_mode;
    } 
    else
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_Setting_SetRetrieveModeRoam RetrieveModeRoam=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_SETTING_697_112_2_18_2_38_57_424,(uint8*)"d", retrieve_mode);
        nv_setting.retrieve_mode_roam = retrieve_mode;
    }
    MMINV_WRITE(MMINV_MMS_SETTING, &nv_setting);
}
/*****************************************************************************/
//  Description : get current retrieve mode
//  Global resource dependence : none
//  Author: feng.xiao
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_SETTING_RETRIEVE_MODE_E MMIMMS_Setting_GetCurRetrieveMode(MN_DUAL_SYS_E dual_sys)
{
    MMIPHONE_ROAMING_STATUS_E roam_status = MMIAPIPHONE_GetRoamingStatus(dual_sys);
    if (MMIPHONE_ROAMING_STATUS_NO_ROMING == roam_status ||
        MMIPHONE_ROAMING_STATUS_NATIVE_ROMING == roam_status)
    {
        return(MMIMMS_Setting_GetRetrieveMode(MMIMMS_SETTING_TYPE_LOCAL));
    }
    else if (MMIPHONE_ROAMING_STATUS_NATIONAL_ROMING == roam_status)
    {
        return(MMIMMS_Setting_GetRetrieveMode(MMIMMS_SETTING_TYPE_ROAM));
    }
    
    return MMIMMS_SETTING_RETRIEVE_MODE_DEFER;
}
#endif
