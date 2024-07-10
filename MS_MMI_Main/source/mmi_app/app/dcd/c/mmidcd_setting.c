/****************************************************************************
** File Name:      mmidcd_setting.c                                        *
** Author:         jun.hu                                                  *
** Date:           03/02/2010                                              *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/02/2010     jun.hu            create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_dcd_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mn_type.h"
#include "mmi_nv.h"
#include "mmidcd_export.h"
#include "mmidcd_setting.h"
#include "mmidcd_file.h"
#include "mmidcd_nv.h"
#include "mmiconnection_export.h"
#include "Mmi_modu_main.h"

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
static MMIDCD_NV_SETTING_T   s_mmidcd_setting_info = {0};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : update dcd net setting when the index list item is deleted  
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void MMIDCD_Setting_ConnectionCallback(
                                             MMICONNECTION_NOTIFY_EVENT_E notify_event,
                                             uint32 param,
                                             MN_DUAL_SYS_E dual_sys
                                             );
/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
PUBLIC MMIDCD_NV_SETTING_T*  MMIAPIDCD_GeSettingInfo(void)
{
    return &s_mmidcd_setting_info;
}
/*****************************************************************************/
// 	Description : write setting info to nv
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_WriteSettingToNv(void)
{
    MMINV_WRITE(MMINV_DCD_SETTING, MMIAPIDCD_GeSettingInfo());
    //SCI_TRACE_LOW:"[MMIDCD] MMIAPIDCD_WriteSettingToNv is_dcd_on = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_SETTING_67_112_2_18_2_12_53_241,(uint8*)"d", MMIAPIDCD_GeSettingInfo()->is_dcd_open);
}
/*****************************************************************************/
// 	Description : read setting info from nv
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMIDCD_NV_SETTING_T MMIDCD_SetSettingDefault(void)
{
    int i=0;
    //SCI_TRACE_LOW:"[MMIDCD]:: MMIDCD_SetSettingDefault enter! "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_SETTING_76_112_2_18_2_12_54_242,(uint8*)"");
    
    s_mmidcd_setting_info.is_dcd_open    = TRUE; //DCDÄ¬ÈÏÊÇ¿ªÆô×´Ì¬....
    s_mmidcd_setting_info.is_auto_update = TRUE;	
    s_mmidcd_setting_info.is_show_readed = TRUE;
    s_mmidcd_setting_info.memory_type    = MMIDCD_MEMORY_ON_PHONE;	
    s_mmidcd_setting_info.connect_mode   = MMIDCD_CONNECT_MODE_OFF_WHEN_CRUISE;
    s_mmidcd_setting_info.scroll_time    = MMIDCD_SCROLL_TIME_MIDDLE;

    for(;i<MMI_DUAL_SYS_MAX;i++)
    {
          s_mmidcd_setting_info.simindex[i] = MMIAPICONNECTION_GetAccountDefaultIndex(i, MMICONNECTION_ACCOUNT_DCD);;
    }
    
    s_mmidcd_setting_info.is_dcd_update_log_on = FALSE;
    
    SCI_MEMSET(&s_mmidcd_setting_info.dcd_update_time, 0, sizeof(s_mmidcd_setting_info.dcd_update_time));
    
    MMINV_WRITE(MMINV_DCD_SETTING, &s_mmidcd_setting_info);
    
    MMIDCDFILE_InitLocalFileHeaderInfo();
    
    return s_mmidcd_setting_info;
    
}
/*****************************************************************************/
//  Description : init dcd setting 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_InitSetting(void)
{
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;
    MMICONNECTION_NOTIFY_INFO_T notify_info = {0};

    //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_InitSetting"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_SETTING_110_112_2_18_2_12_54_243,(uint8*)"");
    
    MMINV_READ(MMINV_DCD_SETTING, MMIAPIDCD_GeSettingInfo(), nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
       MMIDCD_SetSettingDefault();
    }

    notify_info.module_id = MMI_MODULE_DCD;
    notify_info.notify_func = MMIDCD_Setting_ConnectionCallback;
    MMIAPICONNECTION_RegisterNotifyInfo(&notify_info);
}
/*****************************************************************************/
//  Description : Get mms net setting index 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIDCD_Setting_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys)
{
    uint8                 index = 0;
    MMIDCD_NV_SETTING_T   nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;
 
    MMINV_READ(MMINV_DCD_SETTING, &nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        nv_setting = MMIDCD_SetSettingDefault();
    }

    index = nv_setting.simindex[dual_sys] ;


    //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_Setting_GetNetSettingIndex,dual_sys=%d,index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_SETTING_141_112_2_18_2_12_54_244,(uint8*)"dd",dual_sys,index);
    return index;
}

/*****************************************************************************/
//  Description : Set mms net setting index  
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_Setting_SetNetSettingIndex(
                                              MN_DUAL_SYS_E dual_sys,
                                              uint8         index
                                              )
{
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;

    //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_Setting_SetNetSettingIndex dual_sys=%d index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_SETTING_156_112_2_18_2_12_54_245,(uint8*)"dd",dual_sys,index);
    
    MMINV_READ(MMINV_DCD_SETTING, MMIAPIDCD_GeSettingInfo(), nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
       MMIDCD_SetSettingDefault();
    }

    s_mmidcd_setting_info.simindex[dual_sys]= index;
    
    MMINV_WRITE(MMINV_DCD_SETTING, MMIAPIDCD_GeSettingInfo());
}

/*****************************************************************************/
//  Description : update mms net setting when the index list item is deleted  
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void MMIDCD_Setting_ConnectionCallback(
                                             MMICONNECTION_NOTIFY_EVENT_E notify_event,
                                             uint32 param,
                                             MN_DUAL_SYS_E dual_sys
                                             )
{
    BOOLEAN need_write = FALSE;
    // MMIDCD_NV_SETTING_T nv_setting = {0};
    MN_RETURN_RESULT_E    nv_result = MN_RETURN_FAILURE;
    uint8 index = (uint8)param;

    //SCI_TRACE_LOW:"MMIDCD_Setting_ConnectionCallback,index=%d, dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_SETTING_184_112_2_18_2_12_54_246,(uint8*)"dd",index, dual_sys);
    
    MMINV_READ(MMINV_DCD_SETTING, MMIAPIDCD_GeSettingInfo(), nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        need_write = TRUE;
        MMIDCD_SetSettingDefault();
        //return;
    }
    
    switch(notify_event)
    {
    case MMICONNECTION_NOTIFY_EVENT_DEL_ITEM:
        {	
            int i=0;
            for(;i<MMI_DUAL_SYS_MAX;i++)
            {
                if(s_mmidcd_setting_info.simindex[i] == index)
                {
                    need_write = TRUE;
                    s_mmidcd_setting_info.simindex[i] = MMIDCD_SETTING_DEFAULT_INDEX;
                }
                else if(s_mmidcd_setting_info.simindex[i] > index)
                {
                    need_write = TRUE;
                    s_mmidcd_setting_info.simindex[i]--;
                }
            }

            if(need_write)
            {
                MMINV_WRITE(MMINV_DCD_SETTING, MMIAPIDCD_GeSettingInfo());
            } 
        }
        break;

    default:
        break;
    }
     
}
