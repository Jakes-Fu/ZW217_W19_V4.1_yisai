/*****************************************************************************
** File Name:                    mivp_setting.c                                  *
** Author:                                                                   *
** Date:           2010/3/11                                                 *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe vp state machine            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/3/11      apple.zhang      Create
******************************************************************************/
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
#define MMIVP_SETTING_C
/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#include "mmi_app_video_player_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mn_type.h"
#include "mmivp_export.h"
#include "mmivp_nv.h"
#include "mmivp_internal.h"
#include "mmiconnection_export.h"

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 用于将设置信息保存到nv中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetNVSettingInfo(VP_NV_SETTING_T nv_setting_info);

/*****************************************************************************/
//  Description : 从nv中获取设置信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_NV_SETTING_T GetNVSettingInfo(void);

/*****************************************************************************/
//  Description : 获取默认的设置信息。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_NV_SETTING_T GetDefaultSettingInfo(void);

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 用于将设置信息保存到nv中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL void SetNVSettingInfo(VP_NV_SETTING_T nv_setting_info)
{
    MMINV_WRITE(VP_SETTING_INFO, &nv_setting_info);    
}

/*****************************************************************************/
//  Description : 从nv中获取设置信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_NV_SETTING_T GetNVSettingInfo(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    VP_NV_SETTING_T  setting_info = {0};   
    
    MMINV_READ(VP_SETTING_INFO, &setting_info, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        //获取默认设置信息
        setting_info  =  GetDefaultSettingInfo();
        MMINV_WRITE(VP_SETTING_INFO, &setting_info);    
    }   
    return setting_info;
}

/*****************************************************************************/
//  Description : 获取默认的设置信息。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
LOCAL VP_NV_SETTING_T GetDefaultSettingInfo(void)
{
    VP_NV_SETTING_T setting_info = {0};
    uint32 i = MN_DUAL_SYS_1;

    setting_info.brightness = VP_DEFAULT_BRIGHTNESS;
    setting_info.mode = VP_DEFAULT_REPEAT_MODE;
    setting_info.is_random_play = VP_DEFAULT_RANDOM_PLAY;
	setting_info.playback_mode = VP_PLAYBACK_MODE_SCREEN_FLOW;
   
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
         setting_info.net_index[i] = MMIAPICONNECTION_GetAccountDefaultIndex((MN_DUAL_SYS_E)i, MMICONNECTION_ACCOUNT_BROWSER);
    }
  
    setting_info.lowest_port = 0;
    setting_info.highest_port = 0;   
    setting_info.is_use_agent = FALSE;
    SCI_MEMSET(setting_info.agent_ip,0,(VP_MAX_IP_LENGTH+1));  
    setting_info.agent_ip_length = 0;
    setting_info.agent_port = 0;   
    return setting_info;
}

/*****************************************************************************/
//  Description : 恢复初始设置
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_ResetSettingInfo(void)
{
    VP_NV_SETTING_T setting_info = {0};
    
    //获取默认设置信息
    setting_info = GetDefaultSettingInfo();
    
    //保存到nv中
    SetNVSettingInfo(setting_info);
}

/*****************************************************************************/
//  Description : 获取设置信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_NV_SETTING_T VP_Setting_GetSettingInfo(void)
{
    VP_NV_SETTING_T nv_setting_info = {0};
    nv_setting_info = GetNVSettingInfo();
    return nv_setting_info;
}

/*****************************************************************************/
//  Description : 获取亮度大小
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 VP_Setting_GetBrightness(void)
{
    VP_NV_SETTING_T nv_setting_info = {0};
    nv_setting_info = GetNVSettingInfo();
    //VP_STRACE_LOW:"[VP] VP_Setting_GetBrightness nv_setting_info.brightness = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_136_112_2_18_3_5_24_353,(uint8*)"d",nv_setting_info.brightness);
    return nv_setting_info.brightness;
}

/*****************************************************************************/
//  Description : 设置亮度
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetBrightness(uint8 brightness)
{
    VP_NV_SETTING_T nv_setting_info = {0};
    //VP_STRACE_LOW:"[VP] VP_Setting_SetBrightness brightness = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_147_112_2_18_3_5_24_354,(uint8*)"d",brightness);
    nv_setting_info = GetNVSettingInfo();
    nv_setting_info.brightness = brightness;
    SetNVSettingInfo(nv_setting_info);
}

/*****************************************************************************/
//  Description : 获取本地文件播放方式
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_PLAYBACK_MODE_E VP_Setting_GetPlaybackMode(void)
{
	VP_NV_SETTING_T nv_setting_info = {0};
    nv_setting_info = GetNVSettingInfo();
    //VP_STRACE_LOW:"[VP] VP_Setting_GetPlaybackMode nv_setting_info.playback_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_161_112_2_18_3_5_24_355,(uint8*)"d",nv_setting_info.playback_mode);
    return nv_setting_info.playback_mode;
}

/*****************************************************************************/
//  Description : 设置本地文件播放方式
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetPlaybackMode(VP_PLAYBACK_MODE_E playback_mode)
{
	VP_NV_SETTING_T nv_setting_info = {0};
    //VP_STRACE_LOW:"[VP] VP_Setting_SetRepeatPlayMode playback_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_172_112_2_18_3_5_24_356,(uint8*)"d",playback_mode);
    nv_setting_info = GetNVSettingInfo();
    nv_setting_info.playback_mode = playback_mode;
    SetNVSettingInfo(nv_setting_info);
}

/*****************************************************************************/
//  Description : 获取循环播放模式类型
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_REPEAT_PLAY_MODE_E VP_Setting_GetRepeatPlayMode(void)
{
    VP_NV_SETTING_T nv_setting_info = {0};
    nv_setting_info = GetNVSettingInfo();
    //VP_STRACE_LOW:"[VP] VP_Setting_GetRepeatPlayMode nv_setting_info.mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_186_112_2_18_3_5_24_357,(uint8*)"d",nv_setting_info.mode);
    return nv_setting_info.mode;
}

/*****************************************************************************/
//  Description : 设置循环播放模式类型
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetRepeatPlayMode(VP_REPEAT_PLAY_MODE_E mode)
{
    VP_NV_SETTING_T nv_setting_info = {0};
    //VP_STRACE_LOW:"[VP] VP_Setting_SetRepeatPlayMode mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_197_112_2_18_3_5_24_358,(uint8*)"d",mode);
    nv_setting_info = GetNVSettingInfo();
    nv_setting_info.mode = mode;
    SetNVSettingInfo(nv_setting_info);
}

/*****************************************************************************/
//  Description : 获取是否开启随机播放模式
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN VP_Setting_GetIsRandomPlayMode(void)
{
    VP_NV_SETTING_T nv_setting_info = {0};
    nv_setting_info = GetNVSettingInfo();
    //VP_STRACE_LOW:"[VP] VP_Setting_GetIsRandomPlayMode nv_setting_info.is_random_play = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_211_112_2_18_3_5_25_359,(uint8*)"d",nv_setting_info.is_random_play);
    return nv_setting_info.is_random_play;
}

/*****************************************************************************/
//  Description : 设置是否开启随机播放模式
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetIsRandomPlayMode(BOOLEAN is_open)
{
    VP_NV_SETTING_T nv_setting_info = {0};
    //VP_STRACE_LOW:"[VP] VP_Setting_SetIsRandomPlayMode is_open = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_222_112_2_18_3_5_25_360,(uint8*)"d",is_open);
    nv_setting_info = GetNVSettingInfo();
    nv_setting_info.is_random_play = is_open;
    SetNVSettingInfo(nv_setting_info);
}

/*****************************************************************************/
//  Description : 获取网络连接设置的index
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 VP_Setting_GetNetIndex(MN_DUAL_SYS_E dual_sys)
{
    VP_NV_SETTING_T nv_setting_info = {0};

    if (MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //VP_STRACE_LOW:"[VP] VP_Setting_GetNetIndex dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_238_112_2_18_3_5_25_361,(uint8*)"d",dual_sys);
        return 0;
    }
    
    nv_setting_info = GetNVSettingInfo();
    //VP_STRACE_LOW:"[VP] VP_Setting_GetNetIndex dual_sys = %d, index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_243_112_2_18_3_5_25_362,(uint8*)"dd",dual_sys,nv_setting_info.net_index[dual_sys]);
    return nv_setting_info.net_index[dual_sys];
}

/*****************************************************************************/
//  Description : 设置网络连接设置的index。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetNetIndex(MN_DUAL_SYS_E dual_sys,
                                   uint8         index)
{
    VP_NV_SETTING_T nv_setting_info = {0};
    //VP_STRACE_LOW:"[VP] VP_Setting_SetNetIndex dual_sys = %d, index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_255_112_2_18_3_5_25_363,(uint8*)"dd",dual_sys,index);

    if (MMI_DUAL_SYS_MAX <= dual_sys)
    {
        return;
    }

    nv_setting_info = GetNVSettingInfo();
    nv_setting_info.net_index[dual_sys] = index;
    SetNVSettingInfo(nv_setting_info);
}

/*****************************************************************************/
//  Description : 设置RTP/RTCP最低最高端口。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetRTPPort(uint16 lowest_port,
                                  uint16 highest_port)
{
    VP_NV_SETTING_T nv_setting_info = {0};
    
    nv_setting_info = GetNVSettingInfo();
    //VP_STRACE_LOW:"[VP] VP_Setting_SetRTPPort lowest_port = %d, highest_port = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_277_112_2_18_3_5_25_364,(uint8*)"dd",lowest_port,highest_port);

    nv_setting_info.lowest_port = lowest_port;
    nv_setting_info.highest_port = highest_port;
          
    SetNVSettingInfo(nv_setting_info);
}

/*****************************************************************************/
//  Description : 获得RTP/RTCP最低最高端口。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_GetRTPPort(uint16 *lowest_port_ptr,
                                  uint16 *highest_port_ptr)
{
    VP_NV_SETTING_T nv_setting_info = {0};

    if ((PNULL == lowest_port_ptr) || (PNULL == highest_port_ptr) )
    {
        //VP_STRACE_LOW:"[VP] VP_Setting_GetRTPPort param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_296_112_2_18_3_5_25_365,(uint8*)"");
        return;
    } 
    
    nv_setting_info = GetNVSettingInfo();

    *lowest_port_ptr = nv_setting_info.lowest_port;
    *highest_port_ptr = nv_setting_info.highest_port;
}

/*****************************************************************************/
//  Description : 设置代理参数。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_SetAgent(BOOLEAN  is_use_agent,
                                uint8 *ip_ptr,
                                uint8  ip_length,
                                uint16 port)
{
    VP_NV_SETTING_T nv_setting_info = {0};

    if (PNULL == ip_ptr && 0 != ip_length)
    {
        //VP_STRACE_LOW:"[VP] VP_Setting_SetAgent param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_319_112_2_18_3_5_25_366,(uint8*)"");
        return;
    }    
    
    nv_setting_info = GetNVSettingInfo();
    nv_setting_info.is_use_agent = is_use_agent;
    SCI_MEMSET(nv_setting_info.agent_ip,0,(VP_MAX_IP_LENGTH+1));
    nv_setting_info.agent_ip_length = 0;

    if (0 != ip_length && PNULL != ip_ptr)
    {
        MMI_MEMCPY(nv_setting_info.agent_ip,VP_MAX_IP_LENGTH,ip_ptr,ip_length,ip_length);      
        nv_setting_info.agent_ip_length = ip_length;
    }
    
    nv_setting_info.agent_port = port;
    
    SetNVSettingInfo(nv_setting_info);
}

/*****************************************************************************/
//  Description : 获得代理参数。
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void VP_Setting_GetAgent(BOOLEAN  *is_use_agent_ptr,
                                uint8 *ip_ptr,
                                uint8 ip_max_length,
                                uint16 *port_ptr)
{
    VP_NV_SETTING_T nv_setting_info = {0};
    if ((PNULL == is_use_agent_ptr)||(PNULL == ip_ptr) || (PNULL == port_ptr))
    {
        //VP_STRACE_LOW:"[VP] VP_Setting_GetAgent param is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_SETTING_351_112_2_18_3_5_25_367,(uint8*)"");
        return;
    }  
    
    nv_setting_info = GetNVSettingInfo();
    *is_use_agent_ptr = nv_setting_info.is_use_agent;    
    MMI_MEMCPY(ip_ptr,ip_max_length,nv_setting_info.agent_ip,nv_setting_info.agent_ip_length,nv_setting_info.agent_ip_length);
    *port_ptr = nv_setting_info.agent_port;     
    
}
#ifdef MMI_STREAMING_PLAYER_SUPPORT//norflashcut
/*****************************************************************************/
//  Description : 用于将设置信息保存到nv中
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVP_SetNVSettingInfo(VP_NV_SETTING_T nv_setting_info)
{
    SetNVSettingInfo(nv_setting_info);
}

/*****************************************************************************/
//  Description : 从nv中获取设置信息
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC VP_NV_SETTING_T MMIAPIVP_GetNVSettingInfo(void)
{
    return VP_Setting_GetSettingInfo();
}

/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIvp_RequestHandle(void)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};

    req.pri = MMISRVAUD_PRI_NORMAL;
    audio_srv.volume =  MMIAPISET_GetMultimVolume();
    audio_srv.info.type = MMISRVAUD_TYPE_VIRTUAL;
    
    if(audio_srv.volume > MMISET_VOL_MAX)
    {
        audio_srv.volume = MMISET_VOL_MAX;
    }

    return MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
}

#endif
#endif

