/****************************************************************************
** File Name:      mmivt_main.c                                  *
** Author:                                                                 *
** Date:           12/18/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/2009        samboo.shen         Create
****************************************************************************/
//#ifndef _MMIVT_MAIN_C_
#define _MMIVT_MAIN_C_

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_vt_trc.h"
#ifdef __cplusplus
extern   "C"
{
#endif
    
    //#include "mmivt_export.h"
#ifdef VT_SUPPORT
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_applet_table.h"
#include "mmk_app.h"
#include "mmivt_ui.h"
#include "mmivt_kernel.h"
#include "mmivt_nv.h"
#include "mmiaudio_ctrl.h"

#include "mmi_default.h"
//#include "mmiset_display.h"

#include "mmifmm_export.h"
#include "mmidc_export.h"
#include "mmimtv_export.h"
#include "mmipicview_export.h"
#include "mmialarm_export.h"
#include "mmicountedtime_export.h"

#ifdef STREAMING_HS_SUPPORT
#include "mmismp_export.h"
#endif
#include "mmiudisk_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"        
#endif
#include "guiblock.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
MMI_APPLICATION_T g_mmivt_app;  /*lint !e552*/       //vt的实体


/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register vt module applet info
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void RegAppletInfo(void);

/*****************************************************************************/
//  Description : the process message function of the VT
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  VTApplet_HandleEvent(    
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    );

/*****************************************************************************/
//  Description : reg VT MENU group
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN VT_Initialize(MMIVT_INSTANCE_T *self, DPARAM param);

/*****************************************************************************/
//  Description : reg VT MENU group
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN VT_Finalize(MMIVT_INSTANCE_T *self, DPARAM param);

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                               *
**--------------------------------------------------------------------------*/

LOCAL MMIVT_APPLET_T *s_vt_applet_instance = PNULL; /* Temp*/

//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmivt_applet_array[] = 
{
    { GUID_NAME_DEF(SPRD_VT_APPLET_ID), sizeof(MMIVT_APPLET_T), VTApplet_HandleEvent , IMAGE_NULL, TXT_NULL},
};

//applet的信息, 被mmi_applet_table.c extern
LOCAL const CAF_STATIC_APPLET_INFO_T g_mmivt_applet_info = 
{
    s_mmivt_applet_array,
    ARR_SIZE(s_mmivt_applet_array)
};

/*****************************************************************************/
// 	Description : register www module applet info
//	Global resource dependence : none
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void RegAppletInfo(void)
{
    MMI_RegAppletInfo( MMI_MODULE_VT, &g_mmivt_applet_info );
}

/*****************************************************************************/
//  Description : the process message function of the VT
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  VTApplet_HandleEvent(    
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIVT_APPLET_T* vt_applet_ptr = (MMIVT_APPLET_T*)iapplet_ptr;
    
    //SCI_TRACE_LOW:"mmi vtApplet_HandleEvent msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_MAIN_150_112_2_18_3_6_9_85,(uint8*)"d",msg_id);
    
    switch (msg_id)
    {
    case MSG_APPLET_START:
        s_vt_applet_instance = vt_applet_ptr;
        VT_Initialize((&(vt_applet_ptr->vt_instance)), param);
        MMK_SetAppletState(vt_applet_ptr->caf_applet.app_handle, MMI_APPLET_STATE_DISABLE_RETURN_IDLE, TRUE);

        {
            //MMIAUDIO_HandleNewMsgKey();
    
#ifdef ANALOG_TV_SUPPORT 
	        MMIAPITV_Exit();
#endif

            //suspend Mp3 or FM background play
            //MMIAUDIO_PauseBgPlay(MMIBGPLAY_MODULE_CC);
    
            MMIAPIFMM_ExitFileOption();
    
            MMIAPIDC_Interrupt();
            
            MMIAPIUDISK_ClosePCCamera();

            //MMIAPIRECORD_Stop();

            //exit mobiletv immediately
            MMIAPIMTV_VTExit();


#ifdef VIDEO_PLAYER_SUPPORT
            MMIAPIVP_NotifyVPStop();
#endif
#ifdef MOBILE_VIDEO_SUPPORT
            if(MMIAPIMV_IsLiving())
            {
                MMIAPIMV_Exit();
            }
	     else
	     	{
	        	MMIAPIMV_Pause();
	     	}
#endif
            MMIAPIPB_AbortPB();  
            MMIAPIPICVIEW_StopDeleteFiles();       

#ifdef JAVA_SUPPORT_SUN //ia java 暂时没有这个接口
            //并发保护处理,浏览器中下载安装java来VT,有mem不够导致assert问题,此时需要先将jvm同步方式关闭
            if (MMIAPIBROWSER_IsRunning()&&MMIAPIJAVA_IsJavaRuning())
            {
                MMIAPIJAVA_ShutdownSync();
            }
#endif

            MMIDEFAULT_SetBackLight(TRUE);		//点亮背光
		    MMIDEFAULT_AllowTurnOffBackLight(FALSE);	//背光常开
            //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_CC, FALSE);    

#ifdef UI_P3D_SUPPORT
            MMI_Disable3DEffect(MMI_3D_EFFECT_VT);
#endif                    
            //关闭3D效果
            //MMI_Enable3DMMI(FALSE);

 

#ifdef STREAMING_HS_SUPPORT // chenxiang 20100220
            MMIAPISMP_CloseSMP(TRUE);
#endif // STREAMING_HS_SUPPORT
        }
        if(MMIVT_CALL_TYPE_MO_CALL == MMIVT_GetCallType())
        {
            MMICC_ConstructVoiceCodec(TRUE);
            MMICC_EnableVoiceCodec(TRUE);
        }
        else
        {
            //mt 启动play ring handle
        }
        break;
        
    case MSG_APPLET_STOP: 
        VT_Finalize((&(vt_applet_ptr->vt_instance)), param);
        MMICC_EnableVoiceCodec(FALSE);
        MMICC_DestroyVoiceCodec();
		if(MMIAPIVT_GetAudioPlayResumeStatus())
		{
	 	      MMIAPIMP3_ResumeMp3(FALSE);
		}
	
        {
            if (!MMIAPICC_IsInState(CC_IN_CALL_STATE))
            {
               // MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_CC);
//                 MMIAPIBT_RecoveryBtStatus(MMIBT_MODULE_CC);
#ifdef STREAMING_HS_SUPPORT // chenxiang 20100310 
                MMIAPISMP_StartUp(SMP_STARTUP_EXTERN, NULL);
#endif // STREAMING_HS_SUPPORT

            }
                  
#if 0
            //mbbms is still exiting
            if(MMIAPIMTV_IsMTVRunning())
            {
                MMIAPIMTV_PreExitCallback();
            }
#endif
            
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);	//关闭背光常开
            MMIAPISET_SetMainLCDContrast();
            //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_CC, TRUE);
            //显示3D效果
#ifdef UI_P3D_SUPPORT
            MMI_Enable3DEffect(MMI_3D_EFFECT_VT);
#endif             
            //MMI_Enable3DMMI(TRUE);
            //启动延迟的闹铃，自动关机
            if (!MMIAPIALM_CheckAndStartDelayedAlarm())
            {
                MMIAPICT_CheckandStartDelayedCoutimeEvent();
            }

            MMIAPICC_ShowMCHint(); //显示未接来电
#ifdef STREAMING_HS_SUPPORT // chenxiang 20100310 
            MMIAPISMP_StartUp(SMP_STARTUP_EXTERN, NULL);
#endif // STREAMING_HS_SUPPORT
        }
        s_vt_applet_instance = PNULL;
        break;

    case MSG_APPLET_RESUME:
        MMIDEFAULT_SetBackLight(TRUE);		//点亮背光
		MMIDEFAULT_AllowTurnOffBackLight(FALSE);	//背光常开        
        break;

    case MSG_KEYDOWN_HEADSET_DETECT:
    case MSG_KEYUP_HEADSET_DETECT:
        //处理耳机插拔消息
        if (MMIVT_KERNEL_STATE_IDLE != MMIVT_GetCallState())
        {
            MMIAPICC_HandleHeadsetInCall(msg_id);
            MMIVT_HandleUIMsg(MMIVT_UI_MSG_UPDATE_AUDIO_DEVICE, PNULL);
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description : init msg handle func & local variable
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVT_Init(void)
{
    //定义回调函数
    g_mmivt_app.ProcessMsg = MMIVT_HandleExternalMsg;

    //初始化内部变量
}


/*****************************************************************************/
//  Description : reg VT MENU group
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVT_InitModule(void)
{
    MMIVT_RegWinIdNameArr();
    MMIVT_RegNv();
    //MMIVT_RegMenuGroup();
    RegAppletInfo();
    
}

/*****************************************************************************/
// 	Description : request a videomode call
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIVT_ReqVideoModeCall(MN_DUAL_SYS_E dual_sys, uint8 *tele_num, uint8 tele_len)
{
    MMIVT_UI_INFO_DIAL_T dial_info = {0};
    if (MMIAPIVT_IsVtCalling())
    {
        //SCI_TRACE_LOW:"mmivt : is vt call being"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_MAIN_334_112_2_18_3_6_10_86,(uint8*)"");
        return;
    }
    dial_info.dual_sys = dual_sys;
    dial_info.tele_num = tele_num;
    dial_info.tele_len = tele_len;
    MMIVT_SetAddressInfoByNormalNum(tele_num, tele_len);
    MMIVT_HandleUIMsg(MMIVT_UI_MSG_DIAL, (void*)&dial_info);
}

/*****************************************************************************/
// 	Description : request a videomode call
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIVT_Redial(void)
{
    MMIVT_UI_INFO_DIAL_T dial_info = {0};
    uint8 tele_num [MMIVT_PHONE_NUM_MAX_LEN + 2] = {0};
    MMIVT_APPLET_T *vt_applet_ptr = MMIVT_GetAppletInstance();

    dial_info.tele_num = tele_num;
    if (MMIAPIVT_IsVtCalling())
    {
        //SCI_TRACE_LOW:"mmivt : is vt call being"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_MAIN_357_112_2_18_3_6_10_87,(uint8*)"");
        return;
    }
    if (PNULL != vt_applet_ptr)
    {
        vt_applet_ptr->vt_instance.redial_info.redialed_times++;
    }
    else
    {
        //SCI_TRACE_LOW:"mmivt: null applet!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_MAIN_366_112_2_18_3_6_10_88,(uint8*)"");
    }
    dial_info.dual_sys = MMIVT_GetDualSys();
    MMIVT_GetCallNormalNumber(dial_info.tele_num, &dial_info.tele_len);
    MMIVT_SetAddressInfoByNormalNum(dial_info.tele_num, dial_info.tele_len);
    MMIVT_HandleUIMsg(MMIVT_UI_MSG_REDIAL, (void*)&dial_info);    
}

/*****************************************************************************/
//  Description : VT_Initialize
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN VT_Initialize(MMIVT_INSTANCE_T *self, DPARAM param)
{
    //SCI_TRACE_LOW:"MMIVT_Initialize"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_MAIN_380_112_2_18_3_6_10_89,(uint8*)"");

    self->ui_state_info.msg = MMIVT_UI_MSG_MAX;
    self->ui_state_info.cur_state = MMIVT_UI_STATE_IDLE;
    self->ui_state_info.last_state = MMIVT_UI_STATE_IDLE;

    self->ui_call_time_count = 0;

    self->ui_display_info.option_bank_mode = MMIVT_OPTION_BANK_MODE_NORMAL;
    self->ui_display_info.option_focus = MMIVT_1LEVEL_AUDIO_OPTION;
    self->ui_display_info.video_sub_option_focus = MMIVT_2LEVEL_VIDEO_LCD_BRIGHT_OPTION;
    self->ui_display_info.image_rotate_mode = VT_IMAGE_ROTATE_90_DEGREE;
    self->ui_display_info.image_local_rotate_status = VT_IMAGE_STATUS_ROTATE_0_DEGREE;
//    self->ui_display_info.is_option_active = FALSE;
    self->ui_display_info.is_video_sub_option_active = FALSE;

    self->ui_display_info.is_handfree_show = FALSE;
    self->ui_display_info.camera_id = MMIVT_CAMERA_ID_SECONDARY;
    self->ui_display_info.display_mode = MMIVT_DISP_MODE_REMOTE_LOCAL;
    self->ui_display_info.brightness_level = MMIVT_VIDEO_LEVEL_FOUR;
    self->ui_display_info.contrast_level = MMIVT_VIDEO_LEVEL_FOUR;
    self->ui_display_info.lcd_bright_level = MMIVT_VIDEO_LEVEL_FOUR;
    
    self->ui_display_info.is_recording = FALSE;
    self->ui_display_info.is_replacing = FALSE;
    self->ui_display_info.is_remote_replacing = FALSE;
    self->ui_display_info.is_camera_on = TRUE;
    self->ui_display_info.is_hangup_by_user = FALSE;
    
    self->redial_info.redialed_times = 0;
    self->redial_info.redial_times_max = 0;

    

    //设置mmi block
    {
        GUI_LCD_DEV_INFO lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        GUIBLOCK_SetType(255, MMI_TRANSPARENCE_COLOR, GUIBLOCK_TYPE_OSD, &lcd_dev);
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : VT_Finalize
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN VT_Finalize(MMIVT_INSTANCE_T *self, DPARAM param)
{
    //SCI_TRACE_LOW:"MMIVT_Finalize"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_MAIN_428_112_2_18_3_6_10_90,(uint8*)"");
    GUIBLOCK_ResetMainType();
    //注销 mmi block
     /*{
        GUI_LCD_DEV_INFO lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_1};
        GUIBLOCK_Destroy(&lcd_dev);
        GUIBLOCK_Disable(&lcd_dev);
    }*/
   
    return TRUE;
}

/*****************************************************************************/
//  Description : Get VT Applet Instance
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:可以返回PNULL，调用函数需要做判断
/*****************************************************************************/
PUBLIC MMIVT_APPLET_T *MMIVT_GetAppletInstance(void)
{
    return s_vt_applet_instance;
}

/*****************************************************************************/
//  Description : Stop VT 
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIVT_StartVTApplet(void)
{
    MMI_APPLET_START_T start ={0};
    uint32 user_data = 0;
    //SCI_TRACE_LOW:"mmivt StartVTApplet"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_MAIN_457_112_2_18_3_6_10_91,(uint8*)"");
    start.guid = SPRD_VT_APPLET_ID;
    start.param_ptr = &user_data;
    start.size_of_param = sizeof(uint32);
    MMK_StartApplet( &start );
}

/*****************************************************************************/
//  Description : Stop VT 
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIVT_StopVTApplet(void)
{

    MMIVT_APPLET_T* vt_applet_ptr = MMIVT_GetAppletInstance();
    
    //SCI_TRACE_LOW:"mmivt StopVT"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_MAIN_473_112_2_18_3_6_10_92,(uint8*)"");


    if (PNULL != vt_applet_ptr)
    {
        MMK_CloseApplet(vt_applet_ptr->caf_applet.app_handle);

    }
}

/*****************************************************************************/
//  Description : MMIAPIVT_SendMsgToApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 该接口目前用于接收耳机插拔消息
/*****************************************************************************/
PUBLIC void MMIAPIVT_SendMsgToApplet(MMI_MESSAGE_ID_E msg_id)
{

    MMIVT_APPLET_T* vt_applet_ptr = MMIVT_GetAppletInstance();
    MMI_HANDLE_T vt_handle = PNULL;
    
    //SCI_TRACE_LOW:"MMIAPIVT_SendMsgToApplet msg:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_MAIN_493_112_2_18_3_6_10_93,(uint8*)"d", msg_id);

    if (PNULL != vt_applet_ptr)
    {
        vt_handle = vt_applet_ptr->caf_applet.app_handle;
        MMK_SendMsg(vt_handle, msg_id, PNULL);
    }
}

/*****************************************************************************/
//  Description : MMIAPIVT_ActiveVT
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 让VT applet获得焦点，返回值TRUE：代表VT applet存在，FALSE：表示applet不存在
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_ActiveVT(void)
{
    MMIVT_APPLET_T* vt_applet_ptr = MMIVT_GetAppletInstance();
    MMI_HANDLE_T vt_handle = PNULL;
    BOOLEAN result = FALSE;

    if (PNULL != vt_applet_ptr)
    {
        vt_handle = vt_applet_ptr->caf_applet.app_handle;
        if (PNULL != vt_handle)
        {
            MMK_GrabWinFocusByApplet(vt_handle);
            result = TRUE;
        }
    }   
    //SCI_TRACE_LOW:"MMIAPIVT_ActiveVT result:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVT_MAIN_521_112_2_18_3_6_10_94,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description : MMIAPIVT_SetIsEPDefault
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 工程模式下设置默认声音模式是否为耳机
/*****************************************************************************/
PUBLIC void MMIAPIVT_SetIsEPDefault(BOOLEAN is_default_earphone)
{
    MMINV_WRITE(MMINV_VT_IS_EP_DEFAULT,&is_default_earphone);
}

/*****************************************************************************/
//  Description : MMIAPIVT_GetIsEPDefault
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 工程模式下获得默认声音模式是否为耳机
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_GetIsEPDefault(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    BOOLEAN is_ep_default = FALSE;
    MMINV_READ(MMINV_VT_IS_EP_DEFAULT,&is_ep_default,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_ep_default = FALSE;
        MMINV_WRITE(MMINV_VT_IS_EP_DEFAULT,&is_ep_default);
    }
    return is_ep_default;
}
    
#endif //VT_SUPPORT
    
    
#ifdef   __cplusplus
    }
#endif
    
    //#endif //_MMIVT_MAIN_C_
