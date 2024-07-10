/****************************************************************************
** File Name:      mmicc_audio.c                                             *
** Author:                                                                 *
** Date:           24/11/2008                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2008        samboo.shen      Create
****************************************************************************/
#define _MMICC_AUDIO_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_cc_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmibt_export.h"
#include "mmicc_internal.h"
#include "mmicc_export.h"
#include "mmicc_audio.h"

#include "gpio_prod_api.h"
#include "mmi_default.h"

//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "audio_api.h"
//#include "mmiset_call.h"
#include "mmiset_call_export.h"
#include "mmisrvaud_api.h"
#include "mmisrvaud.h"

#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif
#ifdef TTS_SUPPORT
#include "mmitts_export.h"
#endif
#include "mmiudisk_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mmimp3_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmk_timer.h"
#ifdef TTS_SUPPORT
#include "mmitts_export.h"
#endif

#ifdef BLUETOOTH_SUPPORT
#include "mmibt_export.h"
#endif

#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmieng_export.h"

#include "mmipub.h"
#include "mmi_ring.h"
#include "mmi_modu_main.h"
#include "ual_tele_common.h"
#include "ual_tele_call.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define VOLUME_MAX  MMISET_VOL_MAX
#define CC_VIBRATE_TIME   2

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef enum
{
    CC_AUDIO_SWITCH_NONE,    //没有异步设备进行切换
    CC_AUDIO_SWITCH_BT,     //蓝牙设备处于切换中
    CC_AUDIO_SWITCH_MAX 
}CC_AUDIO_SWITCH_E;     //异步设备切换过程中的状态，比如用户选择蓝牙切换免提，此时蓝牙设备仍在切换中的状态。

typedef struct
{    
    //uint8 volume;   //当前音量//MMISRVAUD_GetVolume
    //MMICC_AUDIO_DEV_E cur_dev;        //当前设备//MMISRVAUD_GetRoute
    MMISRVAUD_ROUTE_T pre_route;        //之前设备
    MMISRVAUD_ROUTE_T target_route;   //切换的目标设备
    MMISRVAUD_ROUTE_T default_route;    //默认设备//f
    MMISRVAUD_ROUTE_T mt_preset_codec_route; //set codec route before mt accept, 0 means no set, according to default handle
    //CC_AUDIO_SWITCH_E switch_mode;  //异步设备切换状态//f
    BOOLEAN    is_mute;                //是否静音
    BOOLEAN is_remote_mute;         //是否远端静音
    BOOLEAN    is_codec_on;            //voice codec 是否打开
    //BOOLEAN  is_running;            //audio功能是否处于工作中，如果该值为TRUE,INIT操作将不进行
    MMISRV_HANDLE_T codec_handle;   //voice call codec handle
    MMISRV_HANDLE_T mt_ring_handle; //ring handle for incomming call alerting
    MMISRV_HANDLE_T audio_handle; //虚拟audio handle，键出来，负责赞停其他声音的播放
}CC_AUDIO_STATUS_T;

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//纪录声音设备相关信息

LOCAL CC_AUDIO_STATUS_T s_audio_status = {  // 1,
                                            MMICC_AUDIO_DEV_HH, 
                                            MMICC_AUDIO_DEV_HH,
                                            0,
                                            0,
                                            //MMICC_AUDIO_DEV_HH,
                                            //CC_AUDIO_SWITCH_NONE,
                                            FALSE,
                                            FALSE,
                                            FALSE,
                                            //FALSE,
                                            0,
                                            0,
                                            0
};

LOCAL uint8 s_mmicc_vibrate_timer = 0; //来电提醒设为先振动再播放铃声，此为振动计时器，定时后播放铃声
 LOCAL uint8 s_g_vibrate_times = 0;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get audio handle :codec handle or mt ring handle
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T GetAudioHandle ( void );

/*****************************************************************************/
//  Description : 在用户发起切换听筒请求之后，设置切换状态s_audio_status
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN    ChangeStatusToHH ( MMICC_AUDIO_OPER_E oper );

/*****************************************************************************/
//  Description : 在用户发起切换耳机请求之后，设置切换状态s_audio_status
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN    ChangeStatusToEP ( MMICC_AUDIO_OPER_E oper );

/*****************************************************************************/
//  Description : 在用户发起切换免提请求之后，设置切换状态s_audio_status
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN    ChangeStatusToHF ( MMICC_AUDIO_OPER_E oper );

/*****************************************************************************/
//  Description : 在用户发起切换蓝牙请求之后，设置切换状态s_audio_status
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN    ChangeStatusToBT ( MMICC_AUDIO_OPER_E oper );


/*****************************************************************************/
//  Description : 根据切换状态中，调用下层接口发起设备模式切换
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN    SwitchMode ( MMISRVAUD_ROUTE_T audio_route );

/*****************************************************************************/
//  Description : 切换设备模式。
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDevMode ( MMISRVAUD_ROUTE_T audio_route);

/*****************************************************************************/
//  Description : 根据PB, SETTTING等模块的设置,播放自定义铃声
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL void PlayCustomRing(MN_DUAL_SYS_E dual_sys, MMIPB_BCD_NUMBER_T *bcd_number_ptr);

/*****************************************************************************/
//  Description : construct codec
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN VoiceCodecPlayCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);



#ifdef TTS_SUPPORT
#if 0
/*****************************************************************************/
//  Description : 通话前tts是否要同步退出
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  IsTTSShouldSynchStopBeforCCing(void);
#endif
/*****************************************************************************/
//  Description :通话前停止TTS 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void  StopTTSBeforeCCing(void);
#endif
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get audio handle :codec handle or mt ring handle
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T GetAudioHandle ( void )
{
    if(0 != s_audio_status.mt_ring_handle)
    {
        return s_audio_status.mt_ring_handle;
    }
    else if(0 != s_audio_status.codec_handle)
    {
        return s_audio_status.codec_handle;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : 在通话过程中，对声音设备进行操作
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN    MMIAPICC_SwitchAudioDevice( MMICC_AUDIO_OPER_E oper, MMICC_AUDIO_DEV_E dev)
{
    
    BOOLEAN result = TRUE;
    
    if(!((oper < MMICC_AUDIO_OPER_MAX) && (dev <= MMICC_AUDIO_DEV_MAX)))
    {
        //SCI_TRACE_LOW:"ccapp_a SwitchAudioDevice err oper:%d, dev:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_221_112_2_18_2_10_30_257,(uint8*)"dd", oper, dev);
        return FALSE;
    }
    //SCI_TRACE_LOW:"ccapp_a SwitchAudioDevice oper:%d, dev:%d, status:%d, %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_225_112_2_18_2_10_30_258,(uint8*)"ddddd", oper, dev, MMISRVAUD_GetRoute(GetAudioHandle()), s_audio_status.pre_route, s_audio_status.default_route);
    /*检查参数有效性*/
    //该函数只在通话过程中有效，CLOSE操作和对异步设备DISABLE_ASYNC操作除外
    if (!MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        //SCI_TRACE_LOW:"ccapp_a SwitchAudioDevice: no call is being"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_231_112_2_18_2_10_30_260,(uint8*)"");
        return FALSE;
    }   

#ifdef MV_SUPPORT
        if(MMIAPICC_IsMagicVoiceWorking())
        {
            //SCI_TRACE_LOW:"ccapp_a SwitchAudioDevice bt is invalid, mv is running"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_238_112_2_18_2_10_30_261,(uint8*)"");
            return FALSE;
        }
#endif
    
    //确定临时切换状态
    switch(dev) 
    {
    case MMICC_AUDIO_DEV_BT:
        result = ChangeStatusToBT(oper);
        break;
        
    case MMICC_AUDIO_DEV_HH:
        result = ChangeStatusToHH(oper);
        break;
        
    case MMICC_AUDIO_DEV_EP:
        result = ChangeStatusToEP(oper);
        break;
        
    case MMICC_AUDIO_DEV_HF:
        result = ChangeStatusToHF(oper);
        break;
        
    default:
        //SCI_TRACE_LOW:"ccapp_a AudioOperation: invalid oper %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_263_112_2_18_2_10_30_262,(uint8*)"d", oper);
        return FALSE;
    }
    
    if (!result)
    {
        //SCI_TRACE_LOW:"ccapp_a SwitchAudioDevice not need switch"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_269_112_2_18_2_10_30_263,(uint8*)"");
    }
    //否则调用底层函数发起设备模式切换
    else
    {
        s_audio_status.pre_route = MMISRVAUD_GetRoute(GetAudioHandle());
#ifdef BLUETOOTH_SUPPORT
		if((MMISRVAUD_ROUTE_BLUETOOTH == s_audio_status.pre_route) && (MMISRVAUD_ROUTE_BLUETOOTH == s_audio_status.target_route))
		{
			SCI_TRACE_LOW("ccapp_a SwitchAudioDevice, pre_route and target_route both bluetooth, so directly return");
			return result;
		}
		else if(MMISRVAUD_ROUTE_BLUETOOTH == s_audio_status.pre_route)
		{
			BT_TransferAudioConn(BT_AUDIO_CONN_HF_TO_AG);
			SCI_TRACE_LOW("ccapp_a SwitchAudioDevice, BT_TransferAudioConn(BT_AUDIO_CONN_HF_TO_AG)");
		}
        else if(MMISRVAUD_ROUTE_BLUETOOTH == s_audio_status.target_route)
		{
			BT_TransferAudioConn(BT_AUDIO_CONN_AG_TO_HF);
			SCI_TRACE_LOW("ccapp_a SwitchAudioDevice, BT_TransferAudioConn(BT_AUDIO_CONN_AG_TO_HF)");
		}
#endif		
        //SCI_TRACE_LOW:"ccapp_a target dev: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_275_112_2_18_2_10_30_264,(uint8*)"d", s_audio_status.target_route);
        result = SwitchMode(s_audio_status.target_route);
        s_audio_status.target_route = 0;
    }
    //SCI_TRACE_LOW:"ccapp_a SwitchAudioDevice end, status:%d, %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_280_112_2_18_2_10_30_265,(uint8*)"ddd", MMISRVAUD_GetRoute(GetAudioHandle()), s_audio_status.pre_route, s_audio_status.target_route);
    //异步设备状态更新，刷新通话界面，更新声音设备状态的显示
    if (result)
    {
#ifdef OLD_CC_SUPPORT //for 217 ram cutdown:remove old cc
        MMICC_UpdateStatusWin();
#endif
    }
    return result;
    
}

/*****************************************************************************/
//  Description : 在通话过程中，调节音量
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_SetCallVolume(uint8 volume)
{
    BOOLEAN result = FALSE;
    MMISRV_HANDLE_T handle = GetAudioHandle();
    
    result = MMISRVAUD_SetVolume(handle, (uint32)volume);

    //在通话未建立(无回铃音)时，允许用户调节通话音量，此时，对NV进行读写操作。
    if (!s_audio_status.is_codec_on && (MMISET_VOL_MAX > volume && 0 < volume))
    {
        MMIAPISET_SetCallVolume(volume);
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 在通话过程中，调节音量
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetCallVolume(void)
{
    uint8 volume = 0;
    MMISRV_HANDLE_T handle = GetAudioHandle();
    
    if (handle > 0 )
    {
        volume = MMISRVAUD_GetVolume(handle);
    }
    else
    {
        volume = MMIAPISET_GetCallVolume();
    }
    return volume;
}
/*****************************************************************************/
//  Description : 在用户发起切换听筒请求之后，设置切换状态s_audio_status
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN    ChangeStatusToHH ( MMICC_AUDIO_OPER_E oper )
{
    BOOLEAN result = TRUE;
    MMISRVAUD_ROUTE_T cur_dev = MMISRVAUD_GetRoute(GetAudioHandle());
    
    switch(oper) 
    {
    case MMICC_AUDIO_OPER_ENABLE:
        //如果当前设备是耳机设备，不会切换到听筒
        if (MMISRVAUD_ROUTE_HANDHOLD == cur_dev)
        {           
            result = FALSE;
        }
        //如果当前有耳机插入，且当前设备不是蓝牙，则将切换到耳机
        else if (GPIO_CheckHeadsetStatus())
        {
            s_audio_status.target_route = MMISRVAUD_ROUTE_EARPHONE;
        }
        else
        {
            s_audio_status.target_route = MMISRVAUD_ROUTE_HANDHOLD;            
        }
        break;
    default:
        result = FALSE;
        break;
    }
    //SCI_TRACE_LOW:"ccapp_a ChangeStatusToHH result:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_360_112_2_18_2_10_30_266,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description : 在用户发起切换耳机请求之后，设置切换状态s_audio_status
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN    ChangeStatusToEP ( MMICC_AUDIO_OPER_E oper )
{
    BOOLEAN result = TRUE;
    MMISRVAUD_ROUTE_T  cur_dev = MMISRVAUD_GetRoute(GetAudioHandle());
    
    switch (oper)
    {
    case MMICC_AUDIO_OPER_ENABLE: 
        if(s_audio_status.mt_ring_handle > 0)
        {
            if(MMISRVAUD_ROUTE_BLUETOOTH != cur_dev)
            {
                s_audio_status.target_route = MMISRVAUD_ROUTE_EARFREE;
            }
            else
            {
                result = FALSE;
            }
        }
        else
        {
            //蓝牙优先级高于耳机,所以如果蓝牙正在工作，耳机不开启
            if(MMISRVAUD_ROUTE_BLUETOOTH == cur_dev)
            { 
                s_audio_status.pre_route = MMISRVAUD_ROUTE_EARPHONE;
                result = FALSE;
            }
            else
            { 
                s_audio_status.target_route = MMISRVAUD_ROUTE_EARPHONE;
            }
        }
        break;
        
    case MMICC_AUDIO_OPER_DISABLE:
        if(s_audio_status.mt_ring_handle > 0)
        {
            s_audio_status.target_route = MMISRVAUD_ROUTE_SPEAKER;
        }
        else
        {
            //如果当前设备就是耳机设备，则转换目标设备应该是默认的设备
            if(MMISRVAUD_ROUTE_EARPHONE == cur_dev)
            { 
                s_audio_status.target_route = s_audio_status.default_route;
            }
            //如果当前设备是蓝牙，之前的设备是耳机，则当前设备保持不变，之前设备变为默认
            else if(MMISRVAUD_ROUTE_BLUETOOTH == cur_dev
                && MMISRVAUD_ROUTE_EARPHONE == s_audio_status.pre_route)
            {
                s_audio_status.pre_route = s_audio_status.default_route;
                result = FALSE;
            }
            else
            {
                result = FALSE;
            }
        }        
        break;
        
    default:       
        result = FALSE;
        break;
    }

    //SCI_TRACE_LOW:"ccapp_a: ChangeStatusToEP result:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_433_112_2_18_2_10_30_267,(uint8*)"d",result);
    return result;
    
}

/*****************************************************************************/
//  Description : 在用户发起切换为免提模式请求之后，设置切换状态s_audio_switch_status
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN    ChangeStatusToHF ( MMICC_AUDIO_OPER_E oper )
{
    BOOLEAN result = TRUE;
    MMISRVAUD_ROUTE_T cur_dev = MMISRVAUD_GetRoute(GetAudioHandle());
    switch (oper)
    {
    case MMICC_AUDIO_OPER_ENABLE: 
        s_audio_status.target_route = MMISRVAUD_ROUTE_SPEAKER;       
        break;
    case MMICC_AUDIO_OPER_DISABLE:
        if (MMISRVAUD_ROUTE_SPEAKER == cur_dev)
        {
            s_audio_status.target_route = s_audio_status.pre_route; //MMICC_AUDIO_DEV_HH;
        }
        else
        {
            result = FALSE;
        }
        break;
    default:
        result = FALSE;
        break;
    }
    //SCI_TRACE_LOW:"ccapp_a: ChangeStatusToHF result:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_465_112_2_18_2_10_30_268,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : 在用户发起切换蓝牙请求之后，设置切换状态s_audio_status
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN    ChangeStatusToBT ( MMICC_AUDIO_OPER_E oper )
{
    BOOLEAN result = TRUE;
    MMISRVAUD_ROUTE_T cur_dev = MMISRVAUD_GetRoute(GetAudioHandle());
    
    switch (oper)
    {
    case MMICC_AUDIO_OPER_ENABLE: 
        s_audio_status.target_route = MMISRVAUD_ROUTE_BLUETOOTH;
        break;
    case MMICC_AUDIO_OPER_DISABLE:
        if(MMISRVAUD_ROUTE_BLUETOOTH == cur_dev)
        { 
            if(MMISRVAUD_ROUTE_BLUETOOTH != s_audio_status.pre_route
                && MMISRVAUD_ROUTE_AUTO != s_audio_status.pre_route)
            {
                s_audio_status.target_route = s_audio_status.pre_route;
            }
            else
            {
                s_audio_status.target_route =  s_audio_status.default_route;
            }
        }
        else 
        {
            result = FALSE;
        }
        break;
    default:
        result = FALSE;
        break;
    }
    //SCI_TRACE_LOW:"ccapp_a: ChangeStatusToBT result:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_497_112_2_18_2_10_30_269,(uint8*)"dd",result, s_audio_status.target_route);
    return result;
    
}


/*****************************************************************************/
//  Description : 根据切换状态中，调用下层接口发起设备模式切换
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN    SwitchMode ( MMISRVAUD_ROUTE_T audio_route )
{
    //SCI_TRACE_LOW:"ccapp_a SwitchMode dev:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_509_112_2_18_2_10_30_270,(uint8*)"d",audio_route);
     return SetDevMode(audio_route);    
}

/*****************************************************************************/
//  Description : 切换设备模式。
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDevMode ( MMISRVAUD_ROUTE_T audio_route)
{
    BOOLEAN result = FALSE;
    result = MMISRVAUD_SetRoute(GetAudioHandle(), audio_route);
#if 0    
    //设置模式会造成DSP参数清空，需要重新设置静音

    MMISRVAUD_SetUpLinkMute(s_audio_status.is_mute);

    MMISRVAUD_SetMuteNohandle(s_audio_status.is_remote_mute);
#endif
    return result;
}

/*****************************************************************************/
//  Description : 获得当前的声音模式
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC MMICC_AUDIO_DEV_E    MMICC_GetCurrentDev ( void )
{
    MMICC_AUDIO_DEV_E dev = MMICC_AUDIO_DEV_HH;

    switch(MMISRVAUD_GetRoute(GetAudioHandle()))
    {
        case MMISRVAUD_ROUTE_BLUETOOTH:
            dev = MMICC_AUDIO_DEV_BT;
            break;

        case MMISRVAUD_ROUTE_EARPHONE:
            dev = MMICC_AUDIO_DEV_EP;
            break;

        case MMISRVAUD_ROUTE_SPEAKER:
            dev = MMICC_AUDIO_DEV_HF;
            break;

        case MMISRVAUD_ROUTE_HANDHOLD:
            dev = MMICC_AUDIO_DEV_HH;
            break;

        default:
            dev = MMICC_AUDIO_DEV_MAX;
            break;
    }
    return dev;
}

/*****************************************************************************/
//  Description : construct 
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_ConstructAudioHandle (void)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN result = FALSE;
        
    if(s_audio_status.audio_handle != 0)
    {
        //SCI_TRACE_LOW:"ccapp_a ConstructAudioHandle error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_576_112_2_18_2_10_31_271,(uint8*)"");
        return FALSE;
    }
    //SCI_TRACE_LOW:"ccapp_a ConstructAudioHandle"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_579_112_2_18_2_10_31_272,(uint8*)"");

    req.ind_data = 0;
    req.is_async = FALSE;
    req.is_auto_resume_off = FALSE;
    req.notify = PNULL;
    req.pri = MMISRVAUD_PRI_HIGH;

    audio_srv.info.type = MMISRVAUD_TYPE_VIRTUAL;
    audio_srv.all_support_route = MMISRVAUD_ROUTE_HANDHOLD|MMISRVAUD_ROUTE_SPEAKER|MMISRVAUD_ROUTE_EARPHONE|MMISRVAUD_ROUTE_BLUETOOTH;
    audio_srv.play_times = 1;
    
    SCI_TRACE_LOW("ccapp_a ConstructAudioHandle audio_srv.info.type:%d,audio_srv.default_route:%d,audio_srv.info.type:%d,audio_srv.all_support_route:%d,audio_srv.volume:%d",
    audio_srv.info.type,audio_srv.default_route,audio_srv.info.type,audio_srv.all_support_route,audio_srv.volume);

    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

    if(handle != 0)
    {
        s_audio_status.audio_handle = handle;
        result = TRUE;
    }
    return result;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC void MMICC_DestroyAudioHandle (void)
{
    if(s_audio_status.audio_handle == 0)
    {
        //SCI_TRACE_LOW:"ccapp_a DestroyAudioHandle handle not being"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_608_112_2_18_2_10_31_273,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"ccapp_a DestroyAudioHandle"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_611_112_2_18_2_10_31_274,(uint8*)"");
    MMISRVMGR_Free(s_audio_status.audio_handle);
    s_audio_status.audio_handle = 0;

	//bt dialer ok bug478014
    /* This function is used to make the current windows get a MSG_GET_MSG msg. */
#ifndef MMI_MULTI_SIM_SYS_NONE	
    MMIPUB_OpenAndCloseTempWin();
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsAudioHandleEnable (void)
{
#ifdef UAL_TELE_SUPPORT
    return call_audio_get_virtual_handle_enable();
#else
    return (s_audio_status.audio_handle != 0);
#endif
}


/*****************************************************************************/
//  Description : construct codec
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_ConstructVoiceCodec (BOOLEAN is_vt_codec)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;
    BOOLEAN result = FALSE;
    
#ifdef  BLUETOOTH_SUPPORT    
    BT_ADDRESS addr = {0};
#endif
    
    if(s_audio_status.is_codec_on || s_audio_status.codec_handle != 0)
    {
        //SCI_TRACE_LOW:"ccapp_a ConstructVoiceCodec codec has been enable"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_633_112_2_18_2_10_31_275,(uint8*)"");
        return FALSE;
    }
    //SCI_TRACE_LOW:"ccapp_a ConstructVoiceCodec"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_636_112_2_18_2_10_31_276,(uint8*)"");
#ifdef BT_DIALER_SUPPORT
	if((MMICC_GetBtCallFlag() && (s_audio_status.mt_ring_handle > 0)) || (0 == s_audio_status.audio_handle))
	{
		SCI_TRACE_LOW("ccapp_a VoiceCodec s_audio_status.mt_ring_handle: %x",s_audio_status.mt_ring_handle);
		return FALSE;
	}
#endif

    if(is_vt_codec)
    {
        //audio_srv.default_route = MMISRVAUD_ROUTE_SPEAKER;
        s_audio_status.pre_route= s_audio_status.default_route= MMISRVAUD_ROUTE_SPEAKER;
        audio_srv.info.type = MMISRVAUD_TYPE_VIRTUAL;
    }
    else
    {
    //default system audio route is  speaker, need set handlehold for voice call
        if(!GPIO_CheckHeadsetStatus()
#ifdef  BLUETOOTH_SUPPORT        
            && !MMIAPIBT_GetActivatedHS(&addr)
#endif
            )
        {
             audio_srv.default_route = MMISRVAUD_ROUTE_SPEAKER;  //bug 1363533, 针对audio目前HH模式存在问题work around
        }
        s_audio_status.pre_route = s_audio_status.default_route =  MMISRVAUD_ROUTE_SPEAKER; //bug 1363533
#ifdef BT_DIALER_SUPPORT
        if(MMICC_GetBtCallFlag())
        {
            audio_srv.info.type = MMISRVAUD_TYPE_VOICE_BTCALL;   
        }
        else
#endif
        {
            audio_srv.info.type = MMISRVAUD_TYPE_VOICE;
        }
    }
    req.ind_data = 0;
    req.is_async = FALSE;
    req.is_auto_resume_off = FALSE;
    req.notify = VoiceCodecPlayCallBack;
    req.pri = MMISRVAUD_PRI_HIGH;

    audio_srv.all_support_route = MMISRVAUD_ROUTE_HANDHOLD|MMISRVAUD_ROUTE_SPEAKER|MMISRVAUD_ROUTE_EARPHONE|MMISRVAUD_ROUTE_BLUETOOTH;
    audio_srv.play_times = 1;
    audio_srv.volume = MMIAPISET_GetCallVolume();
    
    if(s_audio_status.mt_preset_codec_route > 0)
    {
        audio_srv.default_route = s_audio_status.mt_preset_codec_route;
        s_audio_status.mt_preset_codec_route = 0;
    }

    SCI_TRACE_LOW("ccapp_a ConstructVoiceCodec audio_srv.info.type:%d,audio_srv.default_route:%d,audio_srv.info.type:%d,audio_srv.all_support_route:%d,audio_srv.volume:%d",
    audio_srv.info.type,audio_srv.default_route,audio_srv.info.type,audio_srv.all_support_route,audio_srv.volume);
    
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

    if(handle != 0)
    {
        s_audio_status.codec_handle = handle;
        result = TRUE;
    }
    return result;
}
/*****************************************************************************/
//  Description : construct codec
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC void MMICC_DestroyVoiceCodec (void)
{
    s_audio_status.mt_preset_codec_route = 0;
    if(s_audio_status.codec_handle == 0)
    {
        //SCI_TRACE_LOW:"ccapp_a DestroyVoiceCodec handle not being"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_692_112_2_18_2_10_31_277,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"ccapp_a DestroyVoiceCodec"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_695_112_2_18_2_10_31_278,(uint8*)"");
    MMISRVMGR_Free(s_audio_status.codec_handle);
    s_audio_status.codec_handle = 0;
}

/*****************************************************************************/
//  Description : construct codec
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN VoiceCodecPlayCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    if(PNULL != param)
    {
        switch(param->event)
        {
        case MMISRVMGR_NOTIFY_EXT:
            report_ptr = (MMISRVAUD_REPORT_T *)param->data;
            if(PNULL != report_ptr)
            {
                switch(report_ptr->report)
                {
                    case MMISRVAUD_REPORT_END:
                        break;

                    case MMISRVAUD_REPORT_PLAY:
                        break;

#if defined (BT_HFG_SUPPORT)
                    case MMISRVAUD_REPORT_BT_EVENT:
                        switch(report_ptr->data1) 
                        {
                            case MMISRVAUD_BT_EVENT_KEY_VOL_UP:
                                SCI_TRACE_LOW("ccapp MMISRVAUD_BT_EVENT_KEY_VOL_UP %d", MMISRVAUD_BT_EVENT_KEY_VOL_UP) ;
                                MMIAPICC_AdjustCallVolume(TRUE);//MMIAPICC_SetCallVolume(MMIAPICC_GetCallVolume() + 1);
                                break;
                            case MMISRVAUD_BT_EVENT_KEY_VOL_DOWN:
                                SCI_TRACE_LOW("ccapp MMISRVAUD_BT_EVENT_KEY_VOL_DOWN %d", MMISRVAUD_BT_EVENT_KEY_VOL_DOWN) ;
                                MMIAPICC_AdjustCallVolume(FALSE);//MMIAPICC_SetCallVolume(MMIAPICC_GetCallVolume() - 1);
                                break;
                            case MMISRVAUD_BT_EVENT_ACTIVE:
                                SCI_TRACE_LOW("ccapp_a MMISRVAUD_BT_EVENT_ACTIVE %d", MMISRVAUD_BT_EVENT_ACTIVE) ;
                                MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_ENABLE, MMICC_AUDIO_DEV_BT);
                                break;
                            case MMISRVAUD_BT_EVENT_DEACTIVE:
                                SCI_TRACE_LOW("ccapp_a MMISRVAUD_BT_EVENT_DEACTIVE %d", MMISRVAUD_BT_EVENT_DEACTIVE) ;
                                MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_DISABLE, MMICC_AUDIO_DEV_BT);
                                break;    
                            case MMISRVAUD_BT_EVENT_OPENED:
                                SCI_TRACE_LOW("ccapp_a MMISRVAUD_BT_EVENT_OPENED %d", MMISRVAUD_BT_EVENT_OPENED) ;
                                if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
                                {
                                    MMISRVAUD_SetManualRoute(GetAudioHandle(), MMISRVAUD_ROUTE_BLUETOOTH);
                                }
#ifdef OLD_CC_SUPPORT //for 217 ram cutdown:remove old cc
                                MMICC_UpdateStatusWin();
#endif
                                break;
                            case MMISRVAUD_BT_EVENT_CLOSED:
                                SCI_TRACE_LOW("ccapp_a MMISRVAUD_BT_EVENT_CLOSED %d", MMISRVAUD_BT_EVENT_CLOSED) ;
                                MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_DISABLE, MMICC_AUDIO_DEV_BT);
#ifdef OLD_CC_SUPPORT //for 217 ram cutdown:remove old cc
                                MMICC_UpdateStatusWin();
#endif
                                break;                                
                            default:
                                break;
                        }
                        break;
#endif
                    default:
                        break;
                }
            }
            break;
        default:
            break;
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : 打开/关闭voice codec
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC void MMICC_EnableVoiceCodec ( BOOLEAN is_enable )
{
    MMISRV_HANDLE_T handle = s_audio_status.codec_handle;

#ifdef  BLUETOOTH_SUPPORT    
    BT_ADDRESS addr = {0};
#endif
    
    if(0 == handle || is_enable == s_audio_status.is_codec_on)
    {
        //SCI_TRACE_LOW:"ccapp_a VoiceCodec is same state"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_711_112_2_18_2_10_31_279,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"ccapp_a VoiceCodec enable: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_714_112_2_18_2_10_31_280,(uint8*)"d", is_enable);
#if 0
    if(is_enable)
    {
        //关闭所有铃声
        MMIAPISET_StopAllRing(TRUE);    
    }
    if(MMIAUDIO_EnableVoiceCodec(is_enable))
    {
        s_audio_status.is_codec_on = is_enable;
    }
#else
#ifdef BT_DIALER_SUPPORT
	if(MMICC_GetBtCallFlag() && (s_audio_status.mt_ring_handle > 0))
	{
		SCI_TRACE_LOW("ccapp_a VoiceCodec s_audio_status.mt_ring_handle: %d",s_audio_status.mt_ring_handle);
		return;
	}
#endif

    if(is_enable)
    {
		if(!GPIO_CheckHeadsetStatus()
#ifdef  BLUETOOTH_SUPPORT        
            && !MMIAPIBT_GetActivatedHS(&addr)
#endif
            )
        {
            MMISRVAUD_SetDefaultRouteByHandle(handle, MMISRVAUD_ROUTE_SPEAKER); //bug 1363533, 针对audio目前HH模式存在问题work around
        }
		
		if(MMISRVAUD_Play(handle, 0))
        {
            s_audio_status.is_codec_on = TRUE;
        }
        else
        {
            //do nothing
        }
    }
    else
    {
        if(s_audio_status.codec_handle > 0)
        {
            MMISRVAUD_Stop(s_audio_status.codec_handle);
            //codec stop need save call volume into nv
            MMIAPISET_SetCallVolume(MMISRVAUD_GetVolume(GetAudioHandle()));
        }
        s_audio_status.is_codec_on = FALSE;
    }
    //SCI_TRACE_LOW:"ccapp_a VoiceCodec status: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_747_112_2_18_2_10_31_281,(uint8*)"d", s_audio_status.is_codec_on);
#endif
}

/*****************************************************************************/
//  Description : voice codec是否已经打开
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsVoiceCodecEnable ( void )
{
    return s_audio_status.is_codec_on;
}


/*****************************************************************************/
//  Description : 打开/关闭 本端静音
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_EnableMute ( BOOLEAN is_mute )
{
    BOOLEAN result = FALSE;
    //SCI_TRACE_LOW:"ccapp_a: MMICC_EnableMute is_mute:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_768_112_2_18_2_10_31_282,(uint8*)"d",is_mute);
    if (is_mute == s_audio_status.is_mute)
    {
        //SCI_TRACE_LOW:"ccapp_a mute is same state"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_771_112_2_18_2_10_31_283,(uint8*)"");
        return FALSE;
    }
    result = MMISRVAUD_SetUpLinkMute(is_mute);
    if(result)
    {
        s_audio_status.is_mute = is_mute;
    } 
    return result;
    
}

/*****************************************************************************/
//  Description : 判断本端静音本端静音是否打开
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsMute ( void )
{
    return s_audio_status.is_mute;
}

/*****************************************************************************/
//  Description : 打开/关闭 远端静音
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_EnableRemoteMute ( BOOLEAN is_enable )
{
    BOOLEAN result = FALSE;
    //SCI_TRACE_LOW:"ccapp_a: MMICC_EnableRemoteMute is_enable:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_799_112_2_18_2_10_31_284,(uint8*)"d",is_enable);
    /*if (is_enable == s_audio_status.is_remote_mute)
    {
        //SCI_TRACE_LOW:"ccapp_a remote mute is same state"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_802_112_2_18_2_10_31_285,(uint8*)"");
        return FALSE;
    }*/ 
	//modify for cr 135730, if other module modify audio mute,s_audio_status.is_remote_mute won't be modified
    result = MMISRVAUD_SetMuteNohandle(is_enable);
    if(result)
    {
        s_audio_status.is_remote_mute = is_enable;
    } 
    return result;    
}

/*****************************************************************************/
//  Description : 判断远端静音是否打开
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN    MMICC_IsRemoteMute ( void )
{
    return s_audio_status.is_remote_mute;
}


/****************************incoming reminder design*************************/

/*****************************************************************************/
// 	Description : start ring or vibrate
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.9.14
/*****************************************************************************/
PUBLIC MMISET_CALL_RING_TYPE_E MMIAPICC_StartRingOrVibrate(BOOLEAN is_vt_call)
{
    MMISET_CALL_RING_TYPE_E     call_ring_type = MMISET_CALL_RING;
    MMIPB_BCD_NUMBER_T		    pb_num = {0};
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

#ifdef VT_SUPPORT

    if (MMIAPIVT_IsVtCalling())
    {
        dual_sys = MN_DUAL_SYS_1;
        //处理pb_num
        MMIAPIVT_GetCallPbBCDNumber(&pb_num);
    }
    else
#endif
    {
        dual_sys = MMIAPICC_GetCallDualSys();
        MMICC_GetCurrentCallNumber(&pb_num, FALSE, FALSE);
    }    
    
    //根据不同设置，播放铃声或振动
    call_ring_type = (MMISET_CALL_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(dual_sys, CALL_RING_TYPE);

    if(MMIAPIENG_GetIQModeStatus())
    {
        call_ring_type = MMISET_CALL_VIBRA;
    }
#if defined(BROWSER_SUPPORT) && defined(LOW_MEMORY_SUPPORT_32X16)  //进入浏览器强制铃声为震动小屏项目内存需要
    if(MMIAPIBROWSER_IsRunning())
    {
		if((call_ring_type == MMISET_CALL_RING) ||
		   (call_ring_type == MMISET_CALL_VIBRA_BEFORE_RING)||
		   (call_ring_type == MMISET_CALL_VIBRA_AND_RING))
        
		{	
			call_ring_type = MMISET_CALL_VIBRA;
		}
    }
#endif
    //SCI_TRACE_LOW:"ccapp: StartRingOrVibrate type: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_851_112_2_18_2_10_31_286,(uint8*)"d", call_ring_type);
    
    switch(call_ring_type)
    {
    case MMISET_CALL_RING:
#ifdef TTS_SUPPORT		
        if(!MMICC_IncomingCallName_To_Speech())
         {
              PlayCustomRing(dual_sys, &pb_num);
         }
#else
        PlayCustomRing(dual_sys, &pb_num);
#endif

        break;
        
    case MMISET_CALL_VIBRA:
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_CALL))
        {
            MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_CALL,TRUE,CC_VIBRATE_TIMER_OUT); 
        }
        break;
        
    case MMISET_CALL_SILENT:
        MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_CALL);
        break;
        
    case MMISET_CALL_VIBRA_BEFORE_RING:
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_CALL))
        {
            MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_CALL);
            
            //start timer
            
            if (s_mmicc_vibrate_timer != 0)
            {
                MMK_StopTimer(s_mmicc_vibrate_timer);
                s_mmicc_vibrate_timer = 0;
            }

            s_mmicc_vibrate_timer = MMK_CreateTimerCallback(CC_VIBRATE_TIMER_OUT, MMICC_HandleVibrateTimer, NULL, FALSE);
            MMIAPIENVSET_SetVibStatus(TRUE); 
        }
        break;
        
    case MMISET_CALL_VIBRA_AND_RING:
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_CALL))
        {  
            // 播放铃声
            //GetCurrentCallNumber(&pb_num);
#ifdef TTS_SUPPORT		
        if(!MMICC_IncomingCallName_To_Speech())
#endif            
         {
              PlayCustomRing(dual_sys, &pb_num);
         }
            
            // 开启震动
            MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_CALL,TRUE,CC_VIBRATE_TIMER_OUT);
        }
        break;
        
    default:
        break;
    }
    
    return call_ring_type;
}

/*****************************************************************************/
//  Description : stop ring or vibrate
//  Global resource dependence : none
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_StopRingOrVibrate(void)
{
#ifdef TTS_SUPPORT
    MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_CALL);
#endif

#if 0
    if (MMIAPIPB_StopCustomRing())
    {

    }
    else
    {
        MMIAPISET_StopRing(MMISET_RING_TYPE_CALL);
    }
#endif
    if(s_audio_status.mt_ring_handle > 0)
    {
        MMISRVAUD_Stop(s_audio_status.mt_ring_handle);
        MMISRVMGR_Free(s_audio_status.mt_ring_handle);
        s_audio_status.mt_ring_handle = 0;        
    }

    MMIAPISET_StopRing(MMISET_RING_TYPE_CALL_IN_CALL);
    
    //stop vibrate
    MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_CALL,FALSE,0);
    
    //MMIAPISET_StopAllVibrator();
    
    //stop vibrate timer
    MMK_StopTimer(s_mmicc_vibrate_timer);
    s_mmicc_vibrate_timer = 0;
    
}

/*****************************************************************************/
//  Description :handle vibrate timer
//  Global resource dependence : none
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_HandleVibrateTimer(uint8 timer_id, uint32 param)
{
    MMISET_CALL_RING_TYPE_E     call_ring_type = MMISET_CALL_RING;
    MMIPB_BCD_NUMBER_T          pb_num = {0};

    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    if(s_mmicc_vibrate_timer == timer_id)
    {
#ifdef VT_SUPPORT
        if (MMIAPIVT_IsVtCalling())
        {
            dual_sys = MN_DUAL_SYS_1;
            //处理pb_num
            MMIAPIVT_GetCallPbBCDNumber(&pb_num);
        }
        else
#endif
        {
            dual_sys = MMIAPICC_GetCallDualSys();
            MMICC_GetCurrentCallNumber(&pb_num, FALSE, FALSE);
        }
    
        call_ring_type = (MMISET_CALL_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(dual_sys, CALL_RING_TYPE);
        //SCI_TRACE_LOW:"HandleVibrateTimer: call_ring_type is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_988_112_2_18_2_10_31_287,(uint8*)"d",call_ring_type);
    
        switch(call_ring_type)
        {
        case MMISET_CALL_RING:
        case MMISET_CALL_SILENT:
            break;
        
        case MMISET_CALL_VIBRA:
            if (MMIAPIENVSET_GetVibStatus())
            {
                //stop vibrate
                MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_CALL);
                s_mmicc_vibrate_timer = MMK_CreateTimerCallback(CC_VIBRATE_TIMER_OUT, MMICC_HandleVibrateTimer, NULL, FALSE);
                 MMIAPIENVSET_SetVibStatus(FALSE); 
                 
            }
            else
            {
                 MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_CALL);
                 s_mmicc_vibrate_timer = MMK_CreateTimerCallback(CC_VIBRATE_TIMER_OUT, MMICC_HandleVibrateTimer, NULL, FALSE);
                  MMIAPIENVSET_SetVibStatus(TRUE); 
            }            
            break;
        case MMISET_CALL_VIBRA_AND_RING:
            break;
        
        case MMISET_CALL_VIBRA_BEFORE_RING:
            if (MMIAPIENVSET_GetVibStatus())
            {
                //stop vibrate
                MMIAPISET_SetVibrator(FALSE,MMISET_RING_TYPE_CALL);
                 MMIAPIENVSET_SetVibStatus(FALSE); 
                
                if(s_g_vibrate_times<CC_VIBRATE_TIME)
                {
                    s_g_vibrate_times++;
                    s_mmicc_vibrate_timer = MMK_CreateTimerCallback(CC_VIBRATE_TIMER_OUT, MMICC_HandleVibrateTimer, NULL, FALSE);
                }
                else
                {
                    s_g_vibrate_times = 0;
                    #ifdef TTS_SUPPORT		
                            if(!MMICC_IncomingCallName_To_Speech())
                             {
                                  PlayCustomRing(dual_sys, &pb_num);
                             }
                    #else
                            PlayCustomRing(dual_sys, &pb_num);
                    #endif
                            
                }    
            }
            else
            {
                 MMIAPISET_SetVibrator(TRUE,MMISET_RING_TYPE_CALL);
                 s_mmicc_vibrate_timer = MMK_CreateTimerCallback(CC_VIBRATE_TIMER_OUT, MMICC_HandleVibrateTimer, NULL, FALSE);
                  MMIAPIENVSET_SetVibStatus(TRUE); 
            }
            //start play ring
            //GetCurrentCallNumber(&pb_num);
            
            break;
        
        default:
            break;
        }
    }
}

/*****************************************************************************/
//  Description : 根据PB, SETTTING等模块的设置,播放自定义铃声
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL void PlayCustomRing(MN_DUAL_SYS_E dual_sys, MMIPB_BCD_NUMBER_T *bcd_number_ptr)
{
#if 0
    if(!MMIAPIPB_PlayCustomRingByNumber(bcd_number_ptr))
    {
        MMIAPISET_PlayRing(dual_sys, FALSE, 1, AUD_PLAY_FOREVER,MMISET_RING_TYPE_CALL,PNULL);
    }
#endif    
    MMIAUD_RING_DATA_INFO_T     ring_data = {MMISRVAUD_RING_FMT_MIDI, 0, NULL};/*lint !e64*/
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    MMISRV_HANDLE_T handle = 0;

    uint16              file_name[MMIFILE_FILE_NAME_MAX_LEN+2] = {0};
    uint16              file_name_len = 0;

    wchar  file_name_wstr[MMISET_CALL_RING_NAME_MAX_LEN + 1] = {0};
    uint16 file_name_wstr_len = 0;
    
    MMISRVAUD_RING_FMT_E     music_type = 0;

#ifdef DRM_SUPPORT
    BOOLEAN                 is_audio_drm_file = FALSE;
    BOOLEAN                 is_drm_valid = FALSE;
    MMISRVAUD_RING_FMT_E         drm_audio_type = MMISRVAUD_RING_FMT_MAX;
#endif
#ifdef  BLUETOOTH_SUPPORT   
    BT_ADDRESS addr = {0};
#endif
    BOOLEAN is_ring_info_valid = FALSE;
    MMISET_CALL_RING_T call_ring_info = {MMISET_CALL_RING_FIXED, 0, 0};        
    uint8 ring_vol = MMIAPIENVSET_GetActiveModeRingVol( MMISET_RING_TYPE_CALL);
        
    //init handle param
    
    req.is_auto_free = TRUE;
    req.notify = PNULL;
    req.pri = MMISRVAUD_PRI_HIGH;
    req.is_async = FALSE;
    req.is_auto_resume_off = FALSE;

    audio_srv.volume = ring_vol;
    audio_srv.play_times = 0xffff;
    audio_srv.all_support_route = MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE | MMISRVAUD_ROUTE_EARFREE;

    //插耳机无蓝牙的时候，来电铃声要从免提和耳机出
    if(GPIO_CheckHeadsetStatus()

        )
    {
#ifdef  BLUETOOTH_SUPPORT        
        if(MMIAPIBT_GetActivatedHS(&addr))
        {
            audio_srv.default_route = MMISRVAUD_ROUTE_SPEAKER;
        }
        else
#endif
        {
            audio_srv.default_route = MMISRVAUD_ROUTE_EARFREE;
        }
    }

    /* first play pb ring*/    
    {
        BOOLEAN is_fixed_ring = FALSE;
        uint32 fixed_ring_id = 0;
        BOOLEAN is_exist_in_pb = FALSE;
        //get pb ring info
        is_exist_in_pb = MMIAPIPB_GetCustomRingPathByNumber(bcd_number_ptr, &is_fixed_ring, file_name_wstr, (uint32*)&file_name_wstr_len, &fixed_ring_id);
        if(is_exist_in_pb)
        {
            is_ring_info_valid = ((is_fixed_ring) && (fixed_ring_id <= MMISET_RING_FIXED_DEFAUT_NUM))
                                ||((!is_fixed_ring) 
                                    //&& (PNULL != call_ring_info.more_ring_info.name_wstr) 
                                    && (0 < file_name_wstr_len) 
                                    && MMIAPIFMM_IsFileExist(file_name_wstr, file_name_wstr_len));
        }
        //if pb ring info is valid
        if(is_ring_info_valid)
        {
            //set pb ring info
            if(is_fixed_ring)
            {
                audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
                MMI_GetRingInfo(
                    (fixed_ring_id + MMI_MIDI_RING_START_ID - 1),
                    &ring_data);
                audio_srv.info.ring_buf.fmt = ring_data.type;
                audio_srv.info.ring_buf.data = ring_data.data_ptr;
                audio_srv.info.ring_buf.data_len = ring_data.data_len;
            }
            else
            {
                audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_FILE;
                audio_srv.info.ring_file.name = file_name_wstr;
                audio_srv.info.ring_file.name_len = file_name_wstr_len;
                MMIAPIFMM_SplitFullPath(file_name_wstr, file_name_wstr_len, 
                                      PNULL, PNULL, PNULL, PNULL,
                                      file_name, &file_name_len);
                
                music_type = MMIAPICOM_GetMusicType( file_name, file_name_len );

#ifdef DRM_SUPPORT
                if(MMIAPIMP3_IsDrmFile(
                        file_name_wstr, file_name_wstr_len,
                        &drm_audio_type,&is_audio_drm_file,&is_drm_valid
                        ))
                {
                    if(is_audio_drm_file && is_drm_valid)
                    {
                        music_type = drm_audio_type;
                    }
                }    
#endif
                audio_srv.info.ring_file.fmt = music_type;            
            }

            //construct handle
            handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
            //if handle > 0 && play success
            if((handle > 0) && (MMISRVAUD_RET_OK == MMISRVAUD_Play(handle, 0)))
            {
#ifdef DRM_SUPPORT
                if(is_audio_drm_file)
                {
                    MMIAPIDRM_ConsumeRights(SFS_INVALID_HANDLE, call_ring_info.more_ring_info.name_wstr);
                }
#endif
                s_audio_status.mt_ring_handle = handle;
                return;
            }
            else
            {
                //destroy handle
    			if(handle > 0)
    		    {
                    MMISRVMGR_Free(handle);
    		    }
                else if(s_audio_status.mt_ring_handle >0)
                {
                    MMISRVMGR_Free(s_audio_status.mt_ring_handle);
                }

                s_audio_status.mt_ring_handle = 0;
                //reset pb ring info
//                 if(!MMIAPIUDISK_UdiskIsRun() && !is_fixed_ring)
//                 {
//                     MMIAPIPB_ClearContactRing(bcd_number_ptr);
//                 }
            }
        }
        else
        {
            //reset pb ring info
//             if(!MMIAPIUDISK_UdiskIsRun() && !is_fixed_ring)
//             {
//                 MMIAPIPB_ClearContactRing(bcd_number_ptr);
//             }
        }
    }

    /* then play envr ring*/
    //get envr ring info
    call_ring_info = MMIAPISET_GetCallRingInfo(dual_sys,MMIAPIENVSET_GetActiveModeId());
        
    //if envr ring info is valid
    is_ring_info_valid = ((call_ring_info.call_ring_type == MMISET_CALL_RING_FIXED) && (call_ring_info.fix_ring_index <= MMISET_RING_FIXED_DEFAUT_NUM))
    ||((call_ring_info.call_ring_type == MMISET_CALL_RING_MORE_RING) 
        //&& (PNULL != call_ring_info.more_ring_info.name_wstr) 
        && (0 < call_ring_info.more_ring_info.name_wstr_len) 
        && MMIAPIFMM_IsFileExist(call_ring_info.more_ring_info.name_wstr, call_ring_info.more_ring_info.name_wstr_len));
    //SCI_TRACE_LOW:"ccapp PlayCustomRing valid: %d, type: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_1219_112_2_18_2_10_32_288,(uint8*)"dd", is_ring_info_valid, call_ring_info.call_ring_type);
    if(is_ring_info_valid)
    {
        //set envr ring info
        if(call_ring_info.call_ring_type == MMISET_CALL_RING_FIXED)
        {
            audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
            MMI_GetRingInfo(
                (call_ring_info.fix_ring_index + MMI_MIDI_RING_START_ID - 1),
                &ring_data);
            audio_srv.info.ring_buf.fmt = ring_data.type;
            audio_srv.info.ring_buf.data = ring_data.data_ptr;
            audio_srv.info.ring_buf.data_len = ring_data.data_len;
        }
        else if(call_ring_info.call_ring_type == MMISET_CALL_RING_MORE_RING)
        {
        
            audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_FILE;
            audio_srv.info.ring_file.name = call_ring_info.more_ring_info.name_wstr;
            audio_srv.info.ring_file.name_len = call_ring_info.more_ring_info.name_wstr_len;
            MMIAPIFMM_SplitFullPath(call_ring_info.more_ring_info.name_wstr, call_ring_info.more_ring_info.name_wstr_len, 
                                  PNULL, PNULL, PNULL, PNULL,
                                  file_name, &file_name_len);
            
            music_type = MMIAPICOM_GetMusicType( file_name, file_name_len );

#ifdef DRM_SUPPORT
            if(MMIAPIMP3_IsDrmFile(
                    call_ring_info.more_ring_info.name_wstr, call_ring_info.more_ring_info.name_wstr_len,
                    &drm_audio_type,&is_audio_drm_file,&is_drm_valid
                    ))
            {
                if(is_audio_drm_file && is_drm_valid)
                {
                    music_type = drm_audio_type;
                }
            }    
#endif
            audio_srv.info.ring_file.fmt = music_type;

        }
        
        //construct handle
        handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
        //if handle > 0 && play success
        if((handle > 0) && (MMISRVAUD_RET_OK == MMISRVAUD_Play(handle, 0)))
        {
#ifdef DRM_SUPPORT
            if(is_audio_drm_file)
            {
                MMIAPIDRM_ConsumeRights(SFS_INVALID_HANDLE, call_ring_info.more_ring_info.name_wstr);
            }
#endif
            s_audio_status.mt_ring_handle = handle;
            return;
        }
        else
        {
            //destroy handle
			if(handle > 0)
		    {
                MMISRVMGR_Free(handle);
		    }
            else if(s_audio_status.mt_ring_handle > 0)
            {
                MMISRVMGR_Free(s_audio_status.mt_ring_handle);
            }
            s_audio_status.mt_ring_handle = 0;
            //reset envr ring info
        }
    }
    //else
    {
        //reset envr ring info
    }

    /* finaly play default ring*/
    //SCI_TRACE_LOW:"ccapp PlayCustomRing default"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_AUDIO_1289_112_2_18_2_10_32_289,(uint8*)"");
    //set default ring info
    audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
    MMI_GetRingInfo(
        (1 + MMI_MIDI_RING_START_ID - 1),
        &ring_data);
    audio_srv.info.ring_buf.fmt = ring_data.type;
    audio_srv.info.ring_buf.data = ring_data.data_ptr;
    audio_srv.info.ring_buf.data_len = ring_data.data_len;
    
    //construct handle
    handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

    //if handle > 0 && play success
    if((handle > 0) && (MMISRVAUD_RET_OK == MMISRVAUD_Play(handle, 0)))
    {
        s_audio_status.mt_ring_handle = handle;
        return;
    }
    //else
    {
        //destroy handle
    	if(handle > 0)
    	{
            MMISRVMGR_Free(handle);
    	}
        else if(s_audio_status.mt_ring_handle > 0)
        {
            MMISRVMGR_Free(s_audio_status.mt_ring_handle);
        }

        s_audio_status.mt_ring_handle = 0;        
    }
            
}

/*******************************************************************************/
//  Description : 通话前停止所有的铃声和震动
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 从 MMICC_StopRingOrVibrate改造而来，所不同的是对TTS的处理采用异步调用的方式
/********************************************************************************/
PUBLIC void MMICC_StopRingOrVibrateBeforeCCing(void)
{
#if 0
    if (MMIAPIPB_StopCustomRing())
    {

    }
    else
    {
        MMIAPISET_StopRing(MMISET_RING_TYPE_CALL);
    }
#endif
    if(s_audio_status.mt_ring_handle > 0)
    {
        MMISRVAUD_Stop(s_audio_status.mt_ring_handle);
        MMISRVMGR_Free(s_audio_status.mt_ring_handle);
        s_audio_status.mt_ring_handle = 0;        
    }

    MMIAPISET_StopRing(MMISET_RING_TYPE_CALL_IN_CALL);
    
    //stop vibrate
    MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_CALL,FALSE,0);
    
    //stop vibrate timer
    MMK_StopTimer(s_mmicc_vibrate_timer);
    s_mmicc_vibrate_timer = 0;

#ifdef TTS_SUPPORT
    StopTTSBeforeCCing();
#endif

}

/*****************************************************************************/
// 	Description : if it is playing ring at incoming call(ring,tts or vibrate)
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPICC_IsMtCallPlayingRing(void)
{
    if (!MMICC_IsExistIncommingCall())
    {
        return FALSE;
    }
    
    if (((MMISRVAUD_IsAudPlaying(s_audio_status.mt_ring_handle)
#ifdef TTS_SUPPORT
        || MMIAPITTS_IsPlayRing(TTS_RING_TYPE_CALL)
#endif
        )
        && 0 != MMIAPICC_GetCallVolume()) //if mt ring volumn is 0, it's not play mt ring for user.
        || MMIAPISET_IsVibratorNow()
        )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : enable /disable remote mute from net, but the local tone still to be played
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: just only used in mo calling state
/*****************************************************************************/
PUBLIC void MMICC_EnableNetRemoteMute ( BOOLEAN is_enable )
{
#ifndef WIN32
    AUDIO_SetDownLinkLocalNetMute(FALSE, is_enable);
#endif
}

/*****************************************************************************/
//  Description :set mt_preset_codec_route
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: just only use in incomming call state
/*****************************************************************************/
PUBLIC void MMICC_SetMtPresetCodecRoute ( MMISRVAUD_ROUTE_T  route )
{
    s_audio_status.mt_preset_codec_route = route;
}

/*****************************************************************************/
//  Description :get mt_preset_codec_route
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: just only use in incomming call state
/*****************************************************************************/
PUBLIC MMISRVAUD_ROUTE_T MMICC_GetMtPresetCodecRoute ( void )
{
    return s_audio_status.mt_preset_codec_route;
}


#ifdef TTS_SUPPORT
#if 0
/*****************************************************************************/
//  Description : 通话前tts是否要同步退出
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  IsTTSShouldSynchStopBeforCCing(void)
{
    BOOLEAN ret_value = FALSE;
    
#ifdef MV_SUPPORT
    // 如果打开了魔音，则需要同步停TTS, for cr177258
    if (MMIAPISET_GetIsMagicVoice())
    {
        ret_value = TRUE;
    }
#endif
    
    return ret_value;
}
#endif
/*****************************************************************************/
//  Description :通话前停止TTS 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void  StopTTSBeforeCCing(void)
{
    MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_CALL);
}
#endif

//+++ mdy by qing.yu@for cr238867
#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Description : 获取蓝牙设置的音量
//  Global resource dependence : 
//  Author: Qing.Yu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetBtSpeakerVol(void)
{
    return MMIAPIBT_GetVolume(BT_CHANNEL_SPK);
}
//--- mdy by qing.yu@for cr238867
#endif
/****************************END incoming reminder design*************************/
