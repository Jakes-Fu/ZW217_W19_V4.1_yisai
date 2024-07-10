/*****************************************************************************
** File Name:      mmiudisk.c                                                *
** Author:                                                                   *
** Date:           24/03/2004                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       Jassmine.Meng       Create
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_udisk_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#include "dal_power.h"
#ifdef WIN32
#include "mmiudisk_simu.h"
#include "os_param.h"
#else
#include "os_param.h"
#include "usbservice_api.h"
//#include "UPM_api.h"
#include "power.h"
#include "ref_param.h"
#include "mmi_pccamera.h"
#include "ucom_api.h"
#ifdef USB_WEBCAMERA_SUPPORT
#include "upcc_api.h"
#include "sensor_drv.h"
#endif
#endif
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#ifdef BT_DUN_SUPPORT
#include "sio.h"
#endif
#include "mmibt_export.h"
#include "mmi_id.h"
#include "mmi_anim.h"
#include "mmiacc_id.h"
#include "mmiacc_text.h"
//#include "mmiacc_nv.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmicc_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmialarm_export.h"
#include "mmicountedtime_export.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmiacc.h"
#include "mmi_default.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmicc_export.h"
//#include "mmiebook.h"
#ifdef EBOOK_SUPPORT
#include "mmiebook_export.h"
#endif
#include "mmimms_export.h"
#include "mmiota_export.h"
#include "mmifmm_export.h"
#ifdef CAMERA_SUPPORT
#include "mmidc_export.h"
#endif
//#include "mmiset_wintab.h"
#include "mmiudisk_internal.h"
#include "mmiudisk_export.h"
#include "mmivcard_export.h"
#include "mmisms_export.h"
#include "mmi_wallpaper_export.h"
#include "guilistbox.h"
#include "mmisd_export.h"
#include "guitext.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmk_app.h"
//#include "mmk_modinfo.h"
#include "guirichtext.h"
//#include "mmifmm_text.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmimp3_export.h"
#include "mmidm_export.h"
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#endif
//#include "mmiebook.h"
#ifdef EBOOK_SUPPORT
#include "mmiebook_export.h"
#endif
#ifdef DCD_SUPPORT
#include "mmidcd_export.h"
#endif

#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif

#include "mmifmm_id.h"
#include "guilabel.h"
#include "guires.h"
#ifdef PUSH_EMAIL_SUPPORT//�ֻ�����
#include "mail_export.h"
#endif 
#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
#endif

#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif
#include "scm_api.h"
#ifdef FM_SUPPORT
#include "mmifm_export.h"
#endif
#ifdef MSDC_CARD_SUPPORT
//#include "mmisd_internal.h"/*lint !e766*/
#endif
#ifdef MMI_VCALENDAR_SUPPORT
#include "Mmivcalendar.h"
#endif
#ifdef MCARE_V31_SUPPORT
#include "McfInterface.h"
#endif
#ifdef MMIEMAIL_SUPPORT
#include "mmiemail_export.h"
#endif
#ifdef FLASH_SUPPORT
#ifndef WIN32
#include "Ubot_config.h"
#endif
#include "xsr_partition.h"
#endif
#ifdef DSP_USB_LOG
#include "dsp_log.h"      //xuefang.jiang 20110804
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h"
#endif
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif /* MET_MEX_SUPPORT */
#ifdef WRE_SUPPORT
#include "mmiwre_other.h"
#endif
#ifdef ASP_SUPPORT
#include "mmiidle_statusbar.h"
#endif
#ifdef ENGTD_SUPPORT
#include "mmiengtd_export.h"
#endif

#include "mmidata_share.h"

#if (!defined ADULT_WATCH_SUPPORT) && (defined SCREEN_SHAPE_CIRCULAR)
#include "watch_commonwin_export.h"
#include "mmi_text.h"
#include "mmi_image.h"
#endif

#include "ppp_mp_api.h"
#include "mmicc_internal.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define MMIUDISK_SIZE_STR_LEN             14  //u��/sd���ռ��С�ִ�����󳤶�
//@juan:add the default service the usb service should 
//restore to the default service after pluging out
#define MMIUDISK_DEFAULT_SERVICE    UPM_NO_SERVICE
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN s_is_power_on = TRUE; // whether udisk start in power_on state
LOCAL BOOLEAN s_udisk_is_run = FALSE;             //Udisk�Ƿ�������
//LOCAL BOOLEAN s_is_upm_ready = FALSE;   //whether UPM is ready
//LOCAL BOOLEAN s_is_usb_delay = FALSE;   //whether USB is delay
//LOCAL BOOLEAN s_is_usb_ps_ready = FALSE;   //whether ps is ready
//LOCAL BOOLEAN s_is_vuart_on = FALSE;
//LOCAL BOOLEAN s_is_open_pc_camera=FALSE;//@shangke whether pc camera is opened
//LOCAL BOOLEAN s_is_scm_reg_udisk = FALSE;  //scm�Ƿ�ע����u��

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern BOOLEAN SCM_BSD_REG(SCM_SLOT_NAME_E slotNO);

extern BOOLEAN SCM_BSD_UNREG(SCM_SLOT_NAME_E slotNO);

//USB_SERVICE_E usbsrv_inuse=0 ;//added 20100823

#ifdef MMI_UDISK_MEM_ENABLE
#ifndef WIN32
extern void Umem_CtrlSecInfoReset(void);
#endif
#endif
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : UnRegister All File System
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void UnRegisterAllStorageFileSystem(void);

/*****************************************************************************/
//  Description : UnRegister All File System
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
//LOCAL RegisterAllStorageFileSystem(void);

/*****************************************************************************/
//  Description : UnRegister All sd to pc
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void UnRegisterSD(void);

/*****************************************************************************/
//  Description : UnRegister All sd to pc
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void RegisterSD(void);
#ifdef ENGTD_SUPPORT
/********************************************************************************
 NAME:			HandleDSPParamLogWinMsg
 DESCRIPTION:	
 AUTHOR:		jinju.ma
 DATE:			
********************************************************************************/
extern void L1_SET_DSPPara(uint16 para0,
                            uint16 para1,
                            uint16 para2, 
                            uint16 para3);
#endif

/*****************************************************************************/
//  Description : is need  open lowest level usb select window
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
//LOCAL BOOLEAN IsNeedOpenLowestLevelUSB(void);




/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:   init hidden partion on nand flash.
//  Author     :   Arthur.Peng 2006/12
//  Param
//           void
//  Return:
//         TRUE  : INIT COMPLETE
//         FALSE : INIT FAILED.
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIUDISK_SysDeviceinit(void)
{
#ifndef WIN32
    if (SFS_NO_ERROR == MMIAPIFMM_RegisterDevice((uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SYSTEM)))
    {
        //SCI_TRACE_LOW:"mmisd:MMIAPISD_SysDeviceinit: register sys device success"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_240_112_2_18_3_4_4_31,(uint8*)"");
    }
    else
    {
        //SCI_TRACE_LOW:"mmisd:MMIAPISD_SysDeviceinit: register sys device fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_244_112_2_18_3_4_4_32,(uint8*)"");
        MMIAPIFMM_FormatDevice(
                (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM),
                MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SYSTEM),
                MMIFILE_FORMAT_AUTO
             );
    }
#endif
        return TRUE;

}
/*****************************************************************************/
//  Description : deal with signal of UDISK_PLUGIN
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIUdisk_HandleUsbCablePlugIn(void)
{
    MMI_RESULT_E                result              =   MMI_RESULT_TRUE;
    POWER_RESTART_CONDITION_E   restart_condition   =   RESTART_BY_NONE;
#ifndef WIN32
    MCU_MODE_E iRet = 0; 

    iRet = POWER_GetResetMode(); 

    if (CALIBRATION_POST_MODE == iRet
#if defined (PLATFORM_ANTISW3)
        || WCDMA_CALIBRATION_POST_MODE == iRet
#endif
        )
    {
        return result;
    }
#endif

#if 1
    if (MMIAPIENG_IsUSBLOGON()||POWER_IsAutoTestMode() || 
        REFPARAM_GetUsbAutoAssertRelModeFlag())
    {   uint32       alert_time_period = 0;
        if(REFPARAM_GetUsbAutoAssertRelModeFlag())
        {
            MMIPUB_OpenAlertWinByTextId(&alert_time_period, TXT_UDISK_VUART, TXT_NULL,
                                    IMAGE_PUBWIN_WARNING, PNULL, PNULL,
                                    MMIPUB_SOFTKEY_ONE, PNULL);
        }
                                    
        MMIUDISK_StartUsbLog();
        return result;
    }
#endif
    restart_condition = POWER_GetRestartCondition();
    //SCI_TRACE_LOW:"mmiudisk:MMIAPIUdisk_HandlePlugIn:restart_condition=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_279_112_2_18_3_4_4_33,(uint8*)"d", restart_condition);
    
    if (RESTART_BY_CHARGE == restart_condition) //Judge whether is restart by charge
    {//�ֻ��ػ�������U��
#ifdef USB_LOG
        uint8 assert_mode = 0;
        OSPARAM_GetAssertMode(&assert_mode);
        if (SCI_ASSERT_DEBUG_MODE == assert_mode)
        {
            result = MMIAPIUdisk_OpenUsbOperWindow();        //Popup usb operation select window
        }
        else
#endif
        {
			MMIAPIUdisk_OpenUsbOperWindow();
        }
    }
    else//�ֻ���������������U��
    {
        if (MMIAPIPHONE_IsNeedDelayEvent(MMI_USB_DELAY))// �Ƿ���Ҫ�ӳٵ���U��ѡ��˵�
        {
            MMIAPIPHONE_SetEventDelaied(MMI_USB_DELAY);
        }
        else
        {
//            if(IsNeedOpenLowestLevelUSB())
//            {
//                if (MMIUDISK_UsbExpiredInCallWindow())
//                {
//                    result = MMI_RESULT_TRUE;
//                }
//                else
//                {
//                    result = MMI_RESULT_FALSE;
//                }
//                // 
//                if(result == MMI_RESULT_TRUE)
//                {
//                    MMIAPIUdisk_SetUSBDelay(FALSE);
//                }
//                else
//                {
//                    MMIAPIUdisk_SetUSBDelay(TRUE);
//                }
//            }
//             else
            {               
                if (MMIAPIUdisk_OpenUsbOperWindow())//Popup usb operation select window
                {
                    result = MMI_RESULT_TRUE;
                }
                else
                {
                    result = MMI_RESULT_FALSE;
                }
                //
//                if(result == MMI_RESULT_TRUE)
//                {
//                    MMIAPIUdisk_SetUSBDelay(FALSE);
//                }
//                else
//                {   
//                    MMIAPIUdisk_SetUSBDelay(TRUE);                  
//                 }
            }           
        }       
    }
    
    return result;
}

/*****************************************************************************/
//  Description : current operation can be abort 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIUDISK_CurrentAppCanbeAbort(void)
{
    BOOLEAN     bret    =   TRUE;
    
    if(MMIAPICC_IsInState(CC_IN_CALL_STATE)
        || MMK_IsOpenWin(MMIUDISK_STOP_WAIT_WIN_ID)
        || MMK_IsOpenWin(MMIUDISK_IS_USING_WIN_ID)
#ifdef BROWSER_SUPPORT
        || MMIAPIBROWSER_IsRunning()
#endif
#ifdef MMIEMAIL_SUPPORT
        || MMIAPIEMAIL_IsRunning()
#endif
        || MMIAPIMMS_IsSendingOrRecving()
#ifdef BLUETOOTH_SUPPORT
        ||  MMIAPIBT_IsOpenFileWin()
#endif
#ifdef MOBILE_VIDEO_SUPPORT
        || MMIAPIMV_IsOpenMobileVideo()
#endif
#ifdef DL_SUPPORT
        || MMIAPIDL_GetDownloadStatus()
#endif
#ifdef PUSH_EMAIL_SUPPORT//�ֻ�����
        || mail_isEnableUdisk()
#endif /* PUSH_EMAIL_SUPPORT */ 
#ifdef WRE_SUPPORT
        || MMIWRE_isUsingUdisk()
#endif
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
        || js_is_audio_exist() //fix bug1973387
#endif
        )
    {
        bret = FALSE;
    }

    return bret;
}

/*****************************************************************************/
//  Description : Start udisk in power off
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIUDISK_StartUdiskInPowerOff(void)
{  
    //�첽�ر��ļ�ϵͳ
#ifndef WIN32
    //BOOLEAN     is_sd_exist     =   FALSE; //���sd�����ڣ�����Ҫ����SCM

    s_is_power_on   = FALSE;   
#ifdef KURO_SUPPORT
    MMIAPIKUR_OnUdiskStartup();
#endif
	UnRegisterAllStorageFileSystem();
    MMIAPIUdisk_SetRunStatus(TRUE);
	USB_StartService(USB_SERVICE_UDISK);
#endif
}

/*****************************************************************************/
//  Description : UnRegister All File System
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void UnRegisterAllStorageFileSystem(void)
{
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
    
	for(device_type = MMI_DEVICE_SYSTEM; device_type < MMI_DEVICE_NUM; device_type++)
	{
		if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type)))
		{
			MMIAPIFMM_UnRegisterDevice(MMIAPIFMM_GetDevicePath(device_type),  MMIAPIFMM_GetDevicePathLen(device_type));
		}
	}
    RegisterSD();
//    MMIAPIFMM_UnRegisterDevice((uint16 *)MMIFILE_DEVICE_UDISK, 
//        MMIFILE_DEVICE_UDISK_LEN);
//    
//    MMIAPIFMM_UnRegisterDevice((uint16 *)MMIFILE_DEVICE_SYS_NAME, 
//         MMIFILE_DEVICE_SYS_NAME_LEN);
}

/*****************************************************************************/
//  Description : UnRegister All File System
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
// LOCAL RegisterAllStorageFileSystem(void)
// {
//     MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
// 	for(device_type = MMI_DEVICE_SDCARD; device_type < MMI_DEVICE_NUM; device_type++)
// 	{
// 		if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type)))
// 		{
// 			MMIAPIFMM_RegisterDevice(MMIAPIFMM_GetDevicePath(device_type),  MMIAPIFMM_GetDevicePathLen(device_type));
// 		}
// 	}
//     //RegisterSD();
//     MMIAPIFMM_RegisterDevice((uint16 *)MMIFILE_DEVICE_UDISK, 
//         MMIFILE_DEVICE_UDISK_LEN);
//     
//     MMIAPIFMM_RegisterDevice((uint16 *)MMIFILE_DEVICE_SYS_NAME, 
//         MMIFILE_DEVICE_SYS_NAME_LEN);
// }

/*****************************************************************************/
//  Description : Register All sd 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void RegisterSD(void)
{
#ifdef MSDC_CARD_SUPPORT	
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
	SCM_SLOT_NAME_E slot_name = SCM_SLOT_0; 

	for(device_type = MMI_DEVICE_SDCARD; device_type < MMI_DEVICE_NUM; device_type++)
	{
		if(MMIAPISD_IsSDPlugIn(device_type))
		{
			slot_name = MMISD_GetSLOTName(device_type);/*lint !e718*/
			if(slot_name < SCM_MAX_SLOT_NO)
			{
                  SCM_BSD_REG(slot_name);
			}
		}
	}
#endif
}

/*****************************************************************************/
//  Description : UnRegister All sd 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void UnRegisterSD(void)
{
#ifdef MSDC_CARD_SUPPORT
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
	SCM_SLOT_NAME_E slot_name = SCM_SLOT_0; 

	for(device_type = MMI_DEVICE_SDCARD; device_type < MMI_DEVICE_NUM; device_type++)
	{
		//if(MMIAPISD_IsSDPlugIn(device_type))
		{
			slot_name = MMISD_GetSLOTName(device_type);
			if(slot_name < SCM_MAX_SLOT_NO)
			{
#ifndef WIN32
                  SCM_BSD_UNREG(slot_name);
#endif
			}
		}
	}
#endif	
}


/*****************************************************************************/
//  Description : deal with signal of UDISK_PLUGOUT
//  Global resource dependence : 
//  Author: Jassmine
//  Note: // @wancan.you udisk modify
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIUdisk_HandleUsbCablePlugOut(void)
{
    MMI_RESULT_E        result  =   MMI_RESULT_TRUE;
    //SCI_TRACE_LOW:"MMIAPIUdisk_HandlePlugOut s_is_power_on = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_511_112_2_18_3_4_5_34,(uint8*)"d", s_is_power_on);
   
#ifdef MCARE_V31_SUPPORT
    MCareV31_Close_ExitConfirm_PubWin();
#endif  
  
	MMK_CloseWin(MMIUDISK_USB_OPER_SELECT_WIN_ID);
    MMK_CloseWin(MMIUDISK_UPCC_SENSOR_WIN_ID);
    
    if (!s_is_power_on)//�Ƿ����ڿ���״̬
    {
        //�ػ�
        if(USB_SERVICE_NULL !=  USB_GetCurService())
	{
		USB_StopService(USB_GetCurService());
	}
        //MMIDEFAULT_TurnOffBackLight();
        //POWER_PowerOff();
    }
    else
    {
        MMIPUB_CloseAlertWin(); 
		if(USB_SERVICE_NULL !=  USB_GetCurService())
		{
			USB_StopService(USB_GetCurService());
		}
		//if (MMIUSBSHARE_IsRunning())
		{
			//MMIUSBSHARE_Disable();
			MMIUSBSHARE_Disconnect();
		}
		//MMIAPIUdisk_USBServiceStopInd();
	    //s_is_vuart_on = FALSE;
	    PPP_Terminate(0, TERMINATE_LINK_DISCONNECT);
	}

    return(result);
}

/*****************************************************************************/
//  Description : u���˳��Ժ����³�ʼ���й��ļ�ϵͳMMI����Ĳ���
//  Global resource dependence : 
//  Author: wancan.you
//  Note: @wancan.you udisk
/*****************************************************************************/
PUBLIC void MMIUDISK_InitFFS_MMI(void)
{
#ifndef WIN32
    if(!MMIAPIFMM_GetDeviceStatus((uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        if(MMIAPIFMM_FormatDevice((uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK), MMIFILE_FORMAT_AUTO))
        {
            //SCI_TRACE_LOW:"mmiudisk:MMIUDISK_InitFFS_MMI: format udisk success"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_555_112_2_18_3_4_5_35,(uint8*)"");
        }
        else
        {
            //SCI_TRACE_LOW:"mmiudisk:MMIUDISK_InitFFS_MMI: format udisk fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_559_112_2_18_3_4_5_36,(uint8*)"");
        }
    }
#endif
#ifdef MULTI_THEME_SUPPORT
    if(MMISET_THEME_FILE_TYPE == MMIAPISET_GetCurrentThemeType())
    {
        MMIAPISET_IniCurrentTheme();
    }
#endif
#ifdef TCARD_PB_SUPPORT

    if(MMIAPIPB_IsTCardContactExist())
    {

        MMIAPIPB_ResortPB();
    }


#endif
    // free block mem, cr75734
    MMIAPIDC_Exit();
    
    //init ring and setting
    //MMIAPISET_AllInit();
    
    // accessory init for alarm EFS data   -- add by taul
    MMIAPIACC_Init();  
#ifdef VIDEO_PLAYER_SUPPORT
    MMIAPIVP_Init();
#endif
    MMIAPIFMM_InitFolders();
    
    //create ebook dir
#ifdef EBOOK_SUPPORT
    MMIAPIEBOOK_CreateAllDevEbookDir();
#endif
	#ifdef  JAVA_SUPPORT_SUN
    MMIAPIJAVA_CreateJavaDirectory(MMI_DEVICE_UDISK);
    MMIAPIJAVA_CreateJavaDirectory(MMI_DEVICE_SDCARD);
    MMIAPIJAVA_UpdatePreloadMidlet();
	#endif
#ifdef KURO_SUPPORT
    MMIAPIKUR_InitKurFolder();
#endif
    if(MMIAPISMS_IsOrderOk())
    {
        MMIAPIMMS_ListInit();
#ifdef MMI_OTA_SUPPORT
        MMIAPIOTA_InitOTAPUSH();
#endif
    }  
#if defined MMI_VCARD_SUPPORT
    MMIAPIVCARD_Init();
#endif
#ifdef MMI_VCALENDAR_SUPPORT
	MMIAPIVCAL_Init();
#endif
    //MMIAPIIDLE_SetWallpaper(FALSE);

#ifdef ASP_SUPPORT
    // ����idle�ϵ�ͼ��ָʾ.....
    MMIAPIIDLE_UpdateAspState();
#endif
}

/*****************************************************************************/
//  Description : set ucom mode
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIUDISK_SetUcomMode(uint32 ucom_type)
{
    //USB_LOG���ʱ��USB "Log" ѡ���Ӧ UCOM_CREATE_COM_DEBUG
    //                USB "���⴮��" ѡ���Ӧ UCOM_CREATE_COM_DEBUGDATA
    //USB_LOG��ر�ʱ��USB "���⴮��" ѡ���Ӧ UCOM_CREATE_COM_DATA
    //                ����û��USB "Log" ѡ��
    
#ifdef USB_LOG
    uint8   assert_mode  =  0;
    //SCI_TRACE_LOW:"mmiudisk:mmiudisk.c:MMIUDISK_SetUcomMode type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_620_112_2_18_3_4_5_37,(uint8*)"d", ucom_type);
    OSPARAM_GetAssertMode(&assert_mode);
    if (SCI_ASSERT_DEBUG_MODE == assert_mode) //debug mode
    {
        
        if (UCOM_CREATE_COM_DEBUG == ucom_type) //log
        {
            UCOM_CreatForCOM(UCOM_CREATE_COM_DEBUG);
        }
        else //vuart
        {
    #ifdef USB_VCOM_AUTORUN_SUPPORT 
            UCOM_CreatForCOM(UCOM_CREATE_COM_DATADISK);
    #else
            UCOM_CreatForCOM(UCOM_CREATE_COM_DEBUGDATA);
    #endif
        }
    }
    else if (SCI_ASSERT_RELEASE_MODE == assert_mode) //release mode
    {
    #ifdef USB_VCOM_AUTORUN_SUPPORT 
        UCOM_CreatForCOM(UCOM_CREATE_COM_DATADISK);
    #else
        UCOM_CreatForCOM(UCOM_CREATE_COM_DEBUGDATA);
    #endif
    }
#else
    if(UCOM_CREATE_COM_DATA == ucom_type)
    {
    #ifdef USB_VCOM_AUTORUN_SUPPORT 
        UCOM_CreatForCOM(UCOM_CREATE_COM_DATADISK);
    #else
        UCOM_CreatForCOM(UCOM_CREATE_COM_DATA);
    #endif
    }
    else
    {
        //SCI_TRACE_LOW:"MMIUDISK_SetUcomMode ucom_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_657_112_2_18_3_4_5_38,(uint8*)"d", ucom_type);
    }
#endif

    
}

/*****************************************************************************/
//  Description : ����U�̹���
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIUDISK_StartUdisk(void)
{
    BOOLEAN             result      =   TRUE;
    // MMI_STRING_T        wait_text   =   {0};
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
#ifdef MSDC_CARD_SUPPORT
    SCM_SLOT_NAME_E slot_name = SCM_SLOT_0; 
#endif    
#if (!defined ADULT_WATCH_SUPPORT) && (defined SCREEN_SHAPE_CIRCULAR)
    MMI_STRING_T txt_wait = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_NULL,TXT_NULL};
#endif
    s_is_power_on = TRUE;
	//stop search file
	MMIAPIFMM_SearchFileStop();
#ifdef TCARD_PB_SUPPORT
    if(MMIAPIPB_IsTCardContactExist())
    {

        MMIAPIPB_ResortPB();
    }

#endif
#ifdef DSP_USB_LOG
#ifndef WIN32
	SIO_DSPCardLogClose();  //xuefang.jiang 20110802
#endif
#endif	
    //stop mpeg4player
#ifdef VIDEO_PLAYER_SUPPORT
#ifdef MMI_VIDEOPLAYER_MINI_FUNCTION
            MMIAPIVP_MiniFunction_ExitVideoPlayer();
#else
    MMIAPIVP_ExitVideoPlayer();
#endif
#endif
#ifdef MOILE_VIDEO_SUPPORT
        MMIAPIMV_Exit();
#endif
    //ATV
#ifdef ATV_SUPPORT
    MMIAPIATV_Exit();
#endif
#ifdef WRE_SUPPORT
    MMIWRE_CloseWin();
#endif

    //Notify MN to save mms Push.
    MMIAPISMS_SetSavePush(TRUE);
#ifdef MMI_AUDIO_PLAYER_SUPPORT  
    MMIAPIMP3_ResetAllMp3();
#endif
#ifdef FM_SUPPORT    
//    MMIAPIFM_Exit();
#endif
#ifdef SEARCH_SUPPORT
    MMIAPISEARCH_ExitSearching(MMISEARCH_TYPE_FILE);
#endif
#ifdef MMI_RECORD_SUPPORT
    MMIENG_StopENGRecord();
#ifdef OLD_CC_SUPPORT //for 217 ram cutdown:remove old cc
    MMIAPICC_StopCCRecord();
#else
    MMICC_StopRecordInCall();
#endif
#endif

#ifdef MMI_RECORD_SUPPORT
    MMIAPIRECORD_Exit();    
#endif
#ifdef MET_MEX_SUPPORT
    MMIMEX_RecordStop();
#endif
#ifdef JAVA_SUPPORT_IA
    MMIAPIJAVA_StopOperaStartUDisk();
#elif defined(JAVA_SUPPORT_MYRIAD)
    if(MMIAPIJAVA_IsJavaRuning())
    {
        MMIAPIJAVA_UdiskNotifyJavaRunning();
        return FALSE;
    }
#endif


#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
    MMIAPISET_SetFont( PNULL, TRUE, FALSE );
#endif

#ifdef BLUETOOTH_SUPPORT
    MMIAPIBT_SetFtpRootFolder(MMI_DEVICE_NUM);
#endif
#ifdef KURO_SUPPORT
    MMIAPIKUR_OnUdiskStartup();
#endif
#ifdef HERO_ENGINE_SUPPORT 
    MMIHEROAPP_closeHeroApp(); /*lint !e718 !e18*/
#endif

#ifdef MRAPP_SUPPORT
    MMIMRAPP_closeMrapp(); /*lint !e718 !e18*/
#endif

#ifdef CAMERA_SUPPORT
    MMIAPIDC_Exit();
#endif
#ifdef DYNAMIC_MODULE_SUPPORT
    /* begin ����Ҫͣ�����й���applet����Ϣ��*/
    MMK_CloseAllDynamicApplet();
    MMK_UnLoadAllDynModInfo();
    /* end*/
#endif
	 //back to idle
#ifndef ADULT_WATCH_SUPPORT
    MMK_ReturnIdleWin(); //1656763 watch ui, this cause flash screen. 
#endif
	//�������һ��Ҫ��MMK_ReturnIdleWin�����
#ifdef EBOOK_SUPPORT
	MMIAPIEBOOK_WaitExit();
#endif
#ifdef MULTI_THEME_SUPPORT
    if(MMISET_THEME_FILE_TYPE == MMIAPISET_GetCurrentThemeType())
    {
        MMIAPISET_IniCurrentTheme();
    }
#endif
     //����ȴ�����
#if (!defined ADULT_WATCH_SUPPORT) && (defined SCREEN_SHAPE_CIRCULAR)
    MMI_GetLabelTextByLang(TXT_UDISK_WAIT_START_UDISK, &txt_wait);
    WatchCOM_NoteWin_1Line_Enter(MMIUDISK_USB_SERVICE_START_WIN_ID,&txt_wait,res_common_toast_bg_1,softkey,PNULL);
#else
    MMIUDISK_OpenWaitWin(USB_SERVICE_UDISK, MMIUDISK_USB_SERVICE_START_WIN_ID, TXT_UDISK_WAIT_START_UDISK);
#endif
#ifndef ADULT_WATCH_SUPPORT
     MMIPUB_SetWaitWinIsupdatedelay(MMIUDISK_USB_SERVICE_START_WIN_ID, FALSE);
#endif

#ifndef WIN32
         //start udisk service
#ifdef FLASH_SUPPORT
		//if flash ui is exist,we can never suspend or unregister MMI_DEVICE_SYSTEM
		if(UBOT_GetDiskHiddenState((uint8*)STL_UDISK_HIDDEN_FS_PART))
		{
		    device_type = MMI_DEVICE_UDISK;
		}
		else
		{
			device_type = MMI_DEVICE_SYSTEM;
		}
		for(; device_type < MMI_DEVICE_NUM; device_type++)
#else
		for(device_type = MMI_DEVICE_SYSTEM; device_type < MMI_DEVICE_NUM; device_type++)
#endif
    	{
            MMIAPIFMM_UnRegisterDevice(MMIAPIFMM_GetDevicePath(device_type),  MMIAPIFMM_GetDevicePathLen(device_type));
#ifdef MSDC_CARD_SUPPORT
    		if(MMIAPISD_IsSDPlugIn(device_type))
    		{
    			slot_name = MMISD_GetSLOTName(device_type);
    			if(slot_name < SCM_MAX_SLOT_NO)
    			{
#ifndef WIN32
                      SCM_BSD_REG(slot_name);
#endif
    			}
    		} 
#endif			
    	}
#endif 		
		if (USB_StartService(USB_SERVICE_UDISK))                 
		{   
//#ifdef PDA_UI_DROPDOWN_WIN
//            MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_RUNNING_USB_USING);
//#endif  
            MMIAPIUdisk_SetRunStatus(TRUE);
            //MMK_CloseWin(MMIUDISK_USB_SERVICE_START_WIN_ID);        
            MMK_CloseWin(MMIUDISK_USB_OPER_SELECT_WIN_ID);
        }
		else
		{
		    //SCI_ASSERT(0);
		    //MMIAPIUdisk_USBServiceStopInd();
#ifdef ADULT_WATCH_SUPPORT
            MMIUDISK_WatchClosePrompt();
            MMIUDISK_WatchOpenErrPrompt(TXT_ERROR, usb_storage_connect_fail);
#else
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
            MMK_CloseWin(MMIUDISK_USB_SERVICE_START_WIN_ID); 
#endif
        }

#ifdef ASP_SUPPORT
    // ����idle�ϵ�ͼ��ָʾ.....
    MMIAPIIDLE_UpdateAspState();
#endif
            
    return(result);
}                      
                                             
/*****************************************************************************/
//  Description : ��ʽ��U��
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIUDISK_FormatUdisk(void)
{
    BOOLEAN         result      =   FALSE;

#ifdef MMI_AUDIO_PLAYER_SUPPORT    
    MMIAPIMP3_StopAudioPlayer();
#endif
#ifdef JAVA_SUPPORT_IA    
    MMIAPIJAVA_Finalize_Special();
#endif	
#ifdef KURO_SUPPORT    
    MMIAPIKUR_StopKurPlayer();
#endif
#ifdef MMI_UDISK_MEM_ENABLE
#ifndef WIN32
    Umem_CtrlSecInfoReset();
#endif
#endif
    //��ʽ��U��,��Ҫ�ĳ��첽��ʽ
    result = MMIAPIFMM_FormatDevice((uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK), MMIFILE_FORMAT_AUTO);
    
    //���������ļ�ϵͳ��ص�ȫ�ֱ���
    if (result)
    {
#ifdef MMI_AUDIO_PLAYER_SUPPORT          
        MMIAPIMP3_ResetAllMp3();
#endif
#ifdef KURO_SUPPORT
        MMIAPIKUR_ResetAllKur();
        MMIAPIKUR_InitKurFolder();
#endif

        MMIAPIFMM_InitFolders();
#ifdef EBOOK_SUPPORT
        MMIAPIEBOOK_CreateAllDevEbookDir();
#endif
		#ifdef  JAVA_SUPPORT_SUN
        MMIAPIJAVA_CreateJavaDirectory(MMI_DEVICE_UDISK);
        MMIAPIJAVA_CreateJavaDirectory(MMI_DEVICE_SDCARD);
        MMIAPIJAVA_PreloadInstall();
		#endif
#if defined MMI_VCARD_SUPPORT
        MMIAPIVCARD_Init();
#endif
#ifdef MMI_VCALENDAR_SUPPORT
	MMIAPIVCAL_Init();
#endif
        if(MMIAPISMS_IsOrderOk())
        {
            MMIAPIMMS_ListInit();
#ifdef MMI_OTA_SUPPORT
            MMIAPIOTA_InitOTAPUSH();
#endif
        }
        
    }

#ifdef ASP_SUPPORT
    // ����idle�ϵ�ͼ��ָʾ.....
    MMIAPIIDLE_UpdateAspState();
#endif
        
    return (result);
}


/*****************************************************************************/
//  Description :get udisk run status 
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUDISK_UdiskIsRun(void)
{
    //SCI_TRACE_LOW:"mmiudisk:MMIAPIUDISK_UdiskIsRun s_udisk_is_run=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_863_112_2_18_3_4_5_39,(uint8*)"d", s_udisk_is_run);
    return s_udisk_is_run;
}

/*****************************************************************************/
//  Description : udisk run status
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUdisk_SetRunStatus(BOOLEAN is_run)
{
    s_udisk_is_run = is_run;
}

/*****************************************************************************/
//  Description :get vuart run status 
//  Global resource dependence : 
//  Author:jassmine.meng
// te: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUdisk_VUARTIsRun(void)
{
	if(USB_SERVICE_UCOM == USB_GetCurService())
	{
		return TRUE;
	}
	return FALSE;
    //return s_is_vuart_on;
}

/*****************************************************************************/
//  Description : start Virtual uart
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIUDISK_StartVirtualUart(void)
{    
    // MMI_STRING_T        wait_text   =   {0}; 
	
    USB_StartService(USB_SERVICE_UCOM);
#ifdef BLUETOOTH_SUPPORT
    MMIAPIBT_CloseDUNWin();
#endif
    //����ȴ�����
	//MMIUDISK_OpenWaitWin(USB_SERVICE_UCOM, MMIUDISK_USB_SERVICE_START_WIN_ID, TXT_COMMON_WAITING);
    //MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
    
    //MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIUDISK_VUART_WAITING_WIN_ID,IMAGE_NULL,
     //   ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,(MMIPUB_HANDLE_FUNC)HandleVUARTWaitWindow);       
            
        
}

/*****************************************************************************/
//  Description : start usb log
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIUDISK_StartUsbLog(void)
{    
#ifdef USB_LOG
#ifdef ENGTD_SUPPORT
	uint16 dsp_param[4] = {0};
#endif
//#ifdef PDA_UI_DROPDOWN_WIN
//     MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_USB_DEBUG);
//#endif     
     USB_StartService(USB_SERVICE_LOG);
	 //MMIAPIUdisk_USBServiceStopInd();
    //MMK_CloseWin(MMIUDISK_USB_OPER_SELECT_WIN_ID);
#ifdef ENGTD_SUPPORT
	if(MMIENGTD_GetIsDspParaLogOn())
	{
		dsp_param[0] = 65535;
		dsp_param[1] = 0;
		dsp_param[2] = 0;
		dsp_param[3] = 4096;
	}
	else
	{
		dsp_param[0] = 65535;
		dsp_param[1] = 0;
		dsp_param[2] = 0;
		dsp_param[3] = 0;
	}
		
#ifndef _WIN32         
		L1_SET_DSPPara( dsp_param[0],dsp_param[1],dsp_param[2],  dsp_param[3]);/*lint !e628 !e718 !e748 !e746*/
#endif 
#endif
#endif
}

/*****************************************************************************/
//  Description :is in PC Camera 
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUDISK_IsInPCCamera(void)
{
	if(USB_SERVICE_UPCC == USB_GetCurService())
	{
		return TRUE;
	}
	return FALSE;
    //return s_is_open_pc_camera;
}

/*****************************************************************************/
//  Description :get udisk reday status 
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN MMIUDISK_UPMIsReady(void)
//{
//    return s_is_upm_ready;   
// }

/*****************************************************************************/
//  Description : set udisk ready status
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
//PUBLIC void MMIAPIUdisk_SetUPMIsReady(BOOLEAN is_ready)
//{
//    s_is_upm_ready = is_ready;
// }

/*****************************************************************************/
//  Description :get usb delay handle state
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUdisk_IsUSBDelay(void)
{
	return MMIAPIPHONE_GetEventDelaied(MMI_USB_DELAY);
    //SCI_TRACE_LOW("MMIAPIUdisk_IsUSBDelay s_is_usb_delay = %d",s_is_usb_delay);
    //return s_is_usb_delay;
}

/*****************************************************************************/
//  Description : set usb delay handle state
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUdisk_SetUSBDelay(BOOLEAN is_ready)
{    
	if(is_ready)
	{
		MMIAPIPHONE_SetEventDelaied(MMI_USB_DELAY);
	}
	else
	{
		MMIAPIPHONE_CleanEventDelaied(MMI_USB_DELAY);
	}
    //s_is_usb_delay = is_ready;
    //SCI_TRACE_LOW("MMIAPIUdisk_SetUSBDelay s_is_usb_delay = %d",s_is_usb_delay);
}



/*****************************************************************************/
//  Description : �첽��ʽ���ļ�ϵͳ����Ϣ������
//  Global resource dependence :
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIUdisk_HandleFormatCnf(BOOLEAN result)
{
	//CR233442
#ifdef MMI_GUI_STYLE_MINISCREEN
	MMI_WIN_ID_T alert_id = MMIFMM_FORMAT_ALERT_WIN;
#endif
    //SCI_TRACE_LOW:"mmiudisk:MMIAPIUdisk_HandleFormatCnf:result is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_996_112_2_18_3_4_6_40,(uint8*)"d",result);
    
    if (result)
    {
        MMIAPIFMM_InitFolders();
#ifdef VIDEO_PLAYER_SUPPORT
        MMIAPIVP_Init();
#endif
#ifdef KURO_SUPPORT
        MMIAPIKUR_InitKurFolder();
#endif
#ifdef EBOOK_SUPPORT
        MMIAPIEBOOK_CreateAllDevEbookDir();
#endif
#if defined MMI_VCARD_SUPPORT
        MMIAPIVCARD_Init();
#endif
#ifdef MMI_VCALENDAR_SUPPORT
	MMIAPIVCAL_Init();
#endif

        if(MMIAPISMS_IsOrderOk())
        {
            MMIAPIMMS_ListInit();
#ifdef MMI_OTA_SUPPORT
            MMIAPIOTA_InitOTAPUSH();
#endif
        }
        //MMIMMS_ReInit4DevMount(TRUE, FS_MMC);
#ifdef MMI_AUDIO_PLAYER_SUPPORT          
        //ֹͣMp3��������
        MMIAPIMP3_ResetAllMp3();
        //�˳�Mp3 player
        MMIAPIMP3_StopOperaSDPlug();      
#endif
#ifdef KURO_SUPPORT
        MMIAPIKUR_ResetAllKur();
#endif

        
#ifdef BROWSER_SUPPORT_DORADO
        MMIAPIBROWSER_HandleStorageFormat();//NEWMS00119929
#endif        

        MMK_CloseWin(MMISDCARD_FORMATTING_WIN_ID);
        
        //��ʾ�ɹ�
        MMK_CloseWin(MMIUDISK_WAIT_WIN_ID);
			//CR233442
#ifndef MMI_GUI_STYLE_MINISCREEN
        MMIPUB_DisplayTipsIDEx(PNULL, TXT_SUCCESS, MMIPUB_TIPS_DIS_BOTTOM, PNULL);//194364
#else
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,MMIUDISK_HandleUdiskAlertClosedMsg);
#endif
   }
    else
    {
        MMK_CloseWin(MMISDCARD_FORMATTING_WIN_ID);
        //��ʾʧ��
        MMK_CloseWin(MMIUDISK_WAIT_WIN_ID);
			//CR233442
#ifndef MMI_GUI_STYLE_MINISCREEN
        MMIPUB_DisplayTipsIDEx(PNULL, TXT_ERROR, MMIPUB_TIPS_DIS_BOTTOM, PNULL);//194364
#else
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,MMIUDISK_HandleUdiskAlertClosedMsg);
#endif   
    }
}

/*****************************************************************************/
//  Description : if  pc camera is opened
//  Global resource dependence : 
//  Author:shangke
//  Note:2006 12 29
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUdisk_IsOpenPcCamera(void)
{
	return MMIAPIUDISK_IsInPCCamera();   
}

#ifdef USB_WEBCAMERA_SUPPORT 
/*****************************************************************************/
//  Description :set pc camera sensor id 
//  Global resource dependence : 
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUDISK_SetPCCameraSensorID(SENSOR_ID_E sensor_id)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    uint32      sensor_param = 0;

    sensor_param = sensor_id;
    ret = DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_IMG_SENSOR_MODE, sensor_param, PNULL);
    //SCI_TRACE_LOW:"[mmiudisk MMIUDISK_SetPCCameraSensorID ret=%d, sensor_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_1068_112_2_18_3_4_6_41,(uint8*)"dd", ret, sensor_id);
    if(DCAMERA_OP_SUCCESS != ret)
    {
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description :get sensor number
//  Global resource dependence : 
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int16 MMIUDISK_GetSensorNumber(void)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    uint32 sensor_number = 0;    
#ifdef  CAMERA_SUPPORT
    ret = DCAMERA_GetInfo((DCAMERA_INFO_CMD_E)DCAMERA_INFO_IMG_SENSOR_NUM, &sensor_number);
    if (DCAMERA_OP_SUCCESS != ret)
    {
        sensor_number = 0;
    }
#endif
    //SCI_TRACE_LOW:"mmiudisk MMIUDISK_GetSensorNumber ret=%d, sensor_number=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_1093_112_2_18_3_4_6_42,(uint8*)"dd",ret, sensor_number);

    return sensor_number;
}

/*****************************************************************************/
//  Description :get current sensor id
//  Global resource dependence : 
//  Author:robert.wang
//  Note:
/*****************************************************************************/
LOCAL SENSOR_ID_E MMIUDISK_GetCurSensorID(void)
{
    DCAMERA_RETURN_VALUE_E ret = DCAMERA_OP_SUCCESS;
    uint32 sensor_id = 0;    
#ifdef  CAMERA_SUPPORT
    ret = DCAMERA_GetInfo((DCAMERA_INFO_CMD_E)DCAMERA_INFO_CUR_IMG_SENSOR, &sensor_id);
    if (DCAMERA_OP_SUCCESS != ret)
    {
        sensor_id = 0;
    }
#endif
    //SCI_TRACE_LOW:"mmiudisk MMIUDISK_GetCurSensorID ret=%d, sensor_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_1114_112_2_18_3_4_6_43,(uint8*)"dd",ret, sensor_id);

    return (SENSOR_ID_E)sensor_id;
}

#endif

/*****************************************************************************/
//  Description :MMI open  pc camera 
//  Global resource dependence : 
//  Author:shangke
//  Note:2006 12 31
/*****************************************************************************/
PUBLIC BOOLEAN MMIUDISK_OpenPCCamera(void)
{
#ifdef USB_WEBCAMERA_SUPPORT 

    MMIAPIDC_Exit();
    //SCI_TRACE_LOW:"mmiudisk:UPCC_OpenPCCamera"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_1130_112_2_18_3_4_6_44,(uint8*)"");
    //MMI_Enable3DMMI(FALSE);
    
    if (UPCC_OpenPCCamera())//image_format))
    {
        return TRUE;
    }
    
    return FALSE;
#else    
    return TRUE;
#endif
}
/*****************************************************************************/
//  Description :MMI close pc camera 
//  Global resource dependence : 
//  Author:shangke
//  Note:2006 12 31
/*****************************************************************************/
PUBLIC void MMIAPIUDISK_ClosePCCamera(void)
{
#ifdef USB_WEBCAMERA_SUPPORT
    UPCC_ClosePCCamera();
#endif
}

/*****************************************************************************/
//  Description : to handle alert window closed message
//  Global resource dependence : 
//  Author:vincent.shang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E  MMIUDISK_HandleUdiskAlertClosedMsg(
                                                        MMI_WIN_ID_T win_id, 
                                                        MMI_MESSAGE_ID_E msg_id, 
                                                        DPARAM param
                                                        )
{
    MMI_RESULT_E    recode  =   MMI_RESULT_TRUE;
    
    recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
    
    if (MSG_CLOSE_WINDOW == msg_id)
    {      
        // ���Alarm�����
        if (!MMIAPIALM_CheckAndStartDelayedAlarm())
        {//@zhaohui,counted time
            MMIAPICT_CheckandStartDelayedCoutimeEvent();//�����ӳٵĵ���ʱ����ʾ
        }
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : get device  status
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC DEVICE_STATUS_E MMIAPIUDISK_GetDeviceStatus( 
                                                   const wchar *device_ptr, /*lint !e18*/ 
                                                   uint16 device_len  )
{
    SFS_ERROR_E     error   =   SFS_NO_ERROR;
    if(MMIAPIFMM_GetDeviceStatus( device_ptr, device_len))
    {
        return DEVICE_STATUS_OK;
    }
    else
    {        
        error = MMIAPIFMM_RegisterDevice(device_ptr, device_len);
        //SCI_TRACE_LOW:"MMIAPISD_GetDeviceStatus sd  exist %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_1194_112_2_18_3_4_6_45,(uint8*)"d", error);
        if (SFS_ERROR_INVALID_FORMAT == error)
        {
            return DEVICE_STATUS_FORMAT;           
        }
        else
        {                
           return DEVICE_STATUS_ERROR;
        }
    }
}

/*****************************************************************************/
//  Description : if there is a device for using
//  Global resource dependence : 
//  Author:apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUDISK_IsDeviceReady(void)
{
    BOOLEAN result = FALSE;
    BOOLEAN is_need_format  = FALSE;
    MMIFILE_DEVICE_E device = MMI_DEVICE_UDISK;
  
    for(; device < MMI_DEVICE_NUM; device++)
    {
        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device)))
        {
            result = TRUE;
            break;
        }
#ifdef MSDC_CARD_SUPPORT
        else if(device > MMI_DEVICE_UDISK)
        {
            is_need_format = MMIAPISD_IsSDNeedFormat(device);
        }
#endif
    }

    if(!result)
    {
        //all device not exist, format errror
        if(is_need_format)
        {
            MMIPUB_OpenAlertWarningWin(TXT_SD_NOT_SUPPORT);
        }
        else
        {

            MMIPUB_OpenAlertWinByTextId(PNULL,
                TXT_SD_NO_EXIST, //norֻ��ʾsd��������
                TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
    }
    
    return result;

}

/*****************************************************************************/
//  Description : UDISKMemDetailGetRate
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void UDISKMemDetailGetRate(MMI_STRING_T *label_str,uint32 *used_num,uint32 *total_num)
{
	int i=0;
	int j=0;
	int k=0;
	wchar used[10]={0};
	wchar total[10]={0};
	wchar unit[5]={'B','K','M','G','T'};
	BOOLEAN is_total=FALSE;
	uint32 temp_u=0;
	uint32 temp_t=0; 
	wchar used_unit=0;
	wchar total_unit=0;
	uint32 num=1;
	BOOLEAN is_used_have_dot=FALSE;
	BOOLEAN is_total_have_dot=FALSE;
	int16 used_index=-1;
	int16 total_index=-1;
	int16 index_interval=0;

	for(i=0;i<label_str->wstr_len;i++)
	{
		if(is_total)
		{
			total[j]=label_str->wstr_ptr[i];
			j++;
		}
		else
		{
			if(label_str->wstr_ptr[i]!='/')
			{
				used[k]=label_str->wstr_ptr[i];
				k++;
			}
			else
			{
				is_total=TRUE;
			}
		}
	}

    if (1 > k || 1 > j)
    {
        return;
    }
	used_unit=used[k-1];/*lint !e676*/
	used[k-1]=0; /*lint !e676 !e831*/
	k--;

	total_unit=total[j-1];
	total[j-1]=0;
	j--;
	
	//�ַ���ת��Ϊ����
	for(i=k-1;i>=0;i--)
	{
		if(used[i]>='0'&&used[i]<='9')
		{
			temp_u+=(used[i]-'0')*num;
			num*=10;
		}
		else
		{
			is_used_have_dot=TRUE;
		}
	}
	
	//�ַ���ת��Ϊ����
	num=1;
	for(i=j-1;i>=0;i--)
	{
		if(total[i]>='0'&&total[i]<='9')
		{
			temp_t+=(total[i]-'0')*num;
			num*=10;
		}
		else
		{
			is_total_have_dot=TRUE;
		}
	}

	if(is_used_have_dot)//��Ϊ������1λС������������ʱ��ȷ��
	{
		temp_t*=10;
	}
	if(is_total_have_dot)
	{
		temp_u*=10;
	}

	for(i=0;i<5;i++)
	{
		if(used_unit==unit[i])
		{
			used_index=i;
		}
		if(total_unit==unit[i])
		{
			total_index=i;
		}
	}

	if(used_index==-1||total_index==-1)//������
	{
		*used_num=0;
		*total_num=100;
		return;
	}
	
	index_interval=abs((total_index-used_index));
	if(index_interval==0)
	{
		*used_num=temp_u;
		*total_num=temp_t;
	}
	else if(index_interval==1)
	{
		*used_num=temp_u;
		*total_num=temp_t*1024;
	}
	else if(index_interval==2)
	{
		*used_num=temp_u;
		*total_num=temp_t*1024*1024;
	}
	else if(index_interval>2)//���̫�󣬽���һ��
	{
		if(temp_u>0)
		{
			*used_num=1;
			*total_num=100;
		}
		else
		{
			*used_num=0;
			*total_num=100;
		}
	}

	temp_u=*used_num;
	temp_t=*total_num;

	if(temp_u!=0)//�Ż�һ����ʾ
	{
		if(temp_t/temp_u>=50)
		{
			*used_num=2;
			*total_num=100;
		}
	}

}

/*****************************************************************************/
//  Description : UDISKMemDetailCtrlCallbackFun
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void UDISKMemDetailCtrlCallbackFun( GUIANIM_OWNER_DRAW_T    *draw_ptr)
{
    uint16              width = 0;
    uint16              height = 0;
    uint32              used_num = 0;
    uint32              total_num = 1;
    GUI_RECT_T          lcd_rect = {0};
    GUI_RECT_T          img_rect = {0};
    MMI_STRING_T        label_str = {0};

    GUILABEL_GetText(MMIUDISK_MEM_DETAIL_LABEL_CTRL_ID,&label_str);
	if (label_str.wstr_ptr!=PNULL && label_str.wstr_len!=0)
	{
		UDISKMemDetailGetRate(&label_str,&used_num,&total_num);
	}

    //������
	if (0 == total_num)
	{
		used_num = 0;
    	total_num = 1;
	}

	if (used_num>total_num)
	{
		used_num=total_num;
	}

    //display battery image bar
    GUIRES_GetImgWidthHeight(&width,&height, IMAGE_BATTERY_CAPACIT_FG,draw_ptr->win_handle);

    //set lcd rect
    lcd_rect.left   = draw_ptr->display_x;
    lcd_rect.top    = draw_ptr->display_y;
    lcd_rect.right  = (int16)(draw_ptr->display_x + used_num*width/total_num);/*lint !e737*/
    lcd_rect.bottom = (int16)(draw_ptr->display_y + height - 1);

    //get crossed rect is lcd rect
    if (GUI_IntersectRect(&lcd_rect,lcd_rect,draw_ptr->display_rect))
    {
        //set img rect
        img_rect.left   = (int16)(lcd_rect.left - draw_ptr->display_x);
        img_rect.top    = (int16)(lcd_rect.top - draw_ptr->display_y);
        img_rect.right  = (int16)(lcd_rect.right - draw_ptr->display_x);
        img_rect.bottom = (int16)(lcd_rect.bottom - draw_ptr->display_y);

        GUIRES_DisplayImg(PNULL,
            &lcd_rect,
            &img_rect,
            draw_ptr->win_handle,
            IMAGE_BATTERY_CAPACIT_FG,
            &draw_ptr->lcd_dev);
    }
}


/*****************************************************************************/
//  Description : setting sd/udisk memory detail info
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIUDISK_SetMemoryDetail(uint32 device)
{
    uint32          free_space_low                              =   0;
    uint32          free_space_high                             =   0;
    uint32          used_space_low                              =   0;
    uint32          used_space_high                             =   0;
    uint32          total_space_high                            =   0;
    uint32          total_space_low                             =   0;
    // uint16          nIndex                                      =   0;      
    wchar           used_wstr [GUILIST_STRING_MAX_NUM + 1]      =   {0};
    // wchar           unused_wstr [GUILIST_STRING_MAX_NUM + 1]    =   {0};
    wchar           total_wstr [GUILIST_STRING_MAX_NUM + 1]     =   {0};

	MMI_STRING_T       label_str = {0};
	wchar  separator[2] = {0};
	wchar  text[64]={0};
	GUIANIM_CTRL_INFO_T         control_info = {0};
    GUIANIM_DATA_INFO_T         data_info =    {0};
    GUIANIM_DISPLAY_INFO_T      display_info = {0};
	
	label_str.wstr_ptr=text;
	separator[0]='/';
    
	if (MMIAPIFMM_GetDeviceStatus((uint16 *)MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device)))
	{                    
		MMIAPIFMM_GetDeviceUsedSpace(MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device),&used_space_high, &used_space_low);
		MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device),&free_space_high, &free_space_low);   
	}
	else
	{   
		free_space_low  = 0;
		free_space_high = 0;
		used_space_low  = 0;
		used_space_high = 0;
	}
    total_space_low  =  used_space_low + free_space_low;  
    if ((total_space_low < used_space_low) || (total_space_low < free_space_low))
    {
        total_space_high =  used_space_high + free_space_high +1;
    }
    else
    {
        total_space_high =  used_space_high + free_space_high;
    }
    
    //SCI_TRACE_LOW:"MMI device total_low =%d,used_low =%d, free_low =%d total_space_high=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_1518_112_2_18_3_4_7_46,(uint8*)"dddd",total_space_low,used_space_low,free_space_low,total_space_high);

    //set total memory
    MMIUDISK_GetFileSizeString(total_space_low,total_space_high,total_wstr,GUILIST_STRING_MAX_NUM);
    //MMIUDISK_AddOneDetailItem(MMIUDISK_MEM_DETAIL_TEXT_CTRL_ID,TXT_FMM_DETAIL_TOTAL_SPACE,PNULL,&nIndex,total_wstr,MMIAPICOM_Wstrlen(total_wstr));
    
    //set used memory
    MMIUDISK_GetFileSizeString(used_space_low,used_space_high,used_wstr,GUILIST_STRING_MAX_NUM);
    //MMIUDISK_AddOneDetailItem(MMIUDISK_MEM_DETAIL_TEXT_CTRL_ID,TXT_UDISK_USED_MEMORY,PNULL,&nIndex,used_wstr,MMIAPICOM_Wstrlen(used_wstr));

    //set unused memory
    //MMIUDISK_GetFileSizeString(free_space_low,free_space_high,unused_wstr,GUILIST_STRING_MAX_NUM);
    //MMIUDISK_AddOneDetailItem(MMIUDISK_MEM_DETAIL_TEXT_CTRL_ID,TXT_UDISK_UNUSED_MEMORY,PNULL,&nIndex,unused_wstr,MMIAPICOM_Wstrlen(unused_wstr));

	MMI_WSTRNCPY(label_str.wstr_ptr,64,used_wstr,MMIAPICOM_Wstrlen(used_wstr),MMIAPICOM_Wstrlen(used_wstr));
	label_str.wstr_len=MMIAPICOM_Wstrlen(used_wstr);
	MMIAPICOM_Wstrcat(label_str.wstr_ptr,separator);
	label_str.wstr_len+=1;
	MMIAPICOM_Wstrcat(label_str.wstr_ptr,total_wstr);
	label_str.wstr_len+=MMIAPICOM_Wstrlen(total_wstr);

	GUILABEL_SetText(MMIUDISK_MEM_DETAIL_LABEL_CTRL_ID, &label_str, FALSE);

	data_info.img_id = IMAGE_BATTERY_CAPACIT_BG;
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMIUDISK_MEM_DETAIL_ANIM_CTRL_ID;
    control_info.ctrl_ptr = PNULL;
    
    display_info.is_syn_decode = TRUE;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    GUIANIM_SetOwnerDraw(MMIUDISK_MEM_DETAIL_ANIM_CTRL_ID,UDISKMemDetailCtrlCallbackFun);
}


/*****************************************************************************/
//  Description : get file size string
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIUDISK_GetFileSizeString(
                             uint32  low_size,
                             uint32  hight_size,
                             wchar*  str_ptr,
                             uint16  wstr_len
                             )
{    
    uint32      gb_size     =   1 << 30;
    uint32      tb_size     =   1 << 8;

    char temp_str[MMIUDISK_SIZE_STR_LEN] = {0};
    
    if((PNULL == str_ptr) || (MMIUDISK_SIZE_STR_LEN > wstr_len))
    {
        //SCI_TRACE_LOW:"[MMIUDISK]MMIUDISK_GetFileSizeString str_ptr 0x%x wstr_len %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_1571_112_2_18_3_4_7_47,(uint8*)"dd", str_ptr, wstr_len);
        return;
    }
    //SCI_ASSERT( PNULL != str_ptr );
    //SCI_ASSERT( MMIUDISK_SIZE_STR_LEN <= wstr_len );

    //SCI_TRACE_LOW:"MMIUDISK_GetFileSizeString low_size =%d,hight_size =%d,tb_size =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_1577_112_2_18_3_4_7_48,(uint8*)"ddd",low_size,hight_size,tb_size);

    if (hight_size > tb_size)
    {
        sprintf( temp_str, "%ld.%ld T", (hight_size/tb_size),(hight_size % tb_size)/(tb_size/10));/*lint !e737 */
        MMI_STRNTOWSTR( str_ptr, wstr_len, (uint8*)temp_str, MMIUDISK_SIZE_STR_LEN, strlen(temp_str) );
    }
    else if (hight_size > 0)
    {
        sprintf( temp_str, "%ld.%ld G", ((hight_size<<2) + low_size/gb_size),(low_size % gb_size)/(gb_size/10));/*lint !e737 */
        MMI_STRNTOWSTR( str_ptr, wstr_len, (uint8*)temp_str, MMIUDISK_SIZE_STR_LEN, strlen(temp_str) );
    }
    else
    {
        MMIAPIFMM_GetFileSizeString(low_size,str_ptr,wstr_len, FALSE);
    }   
}

 
/*****************************************************************************/
//  Description : restore file system
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIUDISK_RestoreFS(void)
{
//#ifndef WIN32
     MMIFILE_DEVICE_E dev = MMI_DEVICE_SYSTEM;
     MMIAPIUdisk_SetRunStatus(FALSE);
     UnRegisterSD();
     //RegisterAllStorageFileSystem();
	//register file system
    do
    {	
        if (MMIAPISD_IsSDPlugIn(dev) || (dev < MMI_DEVICE_SDCARD))
        {
            if(SFS_NO_ERROR == MMIAPIFMM_RegisterDevice(MMIAPIFMM_GetDevicePath(dev), 
                MMIAPIFMM_GetDevicePathLen(dev)))
            {
                //SCI_TRACE_LOW:"mmiudisk:StopUdisk: register SD success"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_1615_112_2_18_3_4_7_49,(uint8*)"");
            }
            else if(dev < MMI_DEVICE_SDCARD)
            {
                MMIAPIFMM_FormatDevice((uint16 *)MMIAPIFMM_GetDevicePath(dev),
                    MMIAPIFMM_GetDevicePathLen(dev),
                    MMIFILE_FORMAT_AUTO);            
            }
            else
            {
               
                //SCI_TRACE_LOW:"mmiudisk:StopUdisk: register SD fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUDISK_1626_112_2_18_3_4_7_50,(uint8*)"");
            }
        }
    }while(++dev < MMI_DEVICE_NUM);
    
//#endif       
//	MMIAPIUdisk_SetRunStatus(FALSE);
	
	//��ʼ��FFS�йص�MMI
	MMIUDISK_InitFFS_MMI();      
	
	MMIAPISMS_SetSavePush(FALSE);
	
	MMIAPISMS_ReadMmsPush(); //read mms push          
	
	if (!MMIAPIALM_CheckAndStartDelayedAlarm())
	{//@zhaohui,counted time
		MMIAPICT_CheckandStartDelayedCoutimeEvent();//�����ӳٵĵ���ʱ����ʾ
	}
	
#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
	MMIAPISET_SetFont( PNULL, FALSE, FALSE );
#endif
	//recovery ftp folder
#ifdef BLUETOOTH_SUPPORT
	MMIAPIBT_SetFtpRootFolder(MMIAPIBT_GetFileLocation());
#endif	
#ifndef ADULT_WATCH_SUPPORT
	//bug-1656763 . On W317, it should be no need to do return idle screen. this cause flash screen.
	if (!MMIAPICC_IsInState(CC_IN_CALL_STATE) && !MMIAPIPHONE_IsNeedDelayEvent(MMI_USB_DELAY)) 
	{
        //MMK_ReturnIdleWin();
    }
#endif
#ifdef KURO_SUPPORT    
     MMIAPIKUR_OnUdiskShutDown();
#endif

#ifdef DCD_SUPPORT
    MMIAPIDCD_StopUdiskUpdateIdleWin();
#endif
}

/*****************************************************************************/
//  Description :is in PC Camera 
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUDISK_GetIsPowerOn(void)
{
	
	return s_is_power_on;
    //return s_is_open_pc_camera;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUDISK_TipWin_Judge(void)
{
    MMI_STRING_T commentString = {0};
    MMI_STRING_T buttonString = {0};
    WATCH_SOFTKEY_TEXT_ID_T   softket_test = {TXT_NULL,STXT_RETURN,TXT_NULL};

    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMI_GetLabelTextByLang(TXT_COMMON_UDISK_USING, &commentString);
        MMI_GetLabelTextByLang(TXT_COMMON_OK, &buttonString);
#ifdef SCREEN_SHAPE_CIRCULAR
        WatchCOM_QueryWin_1Btn_Enter(MMIUDISK_TIP_WIN_ID, &commentString, PNULL, res_common_ic_confirm_green, softket_test, PNULL );
#else
        WatchCOM_QueryWin_1Btn_Enter(MMIUDISK_TIP_WIN_ID, &commentString, &buttonString,PNULL, softket_test, PNULL );
#endif
        return TRUE;
    }
    return FALSE;
}

