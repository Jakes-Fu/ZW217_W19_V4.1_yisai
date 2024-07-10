/*****************************************************************************
** File Name:      mmi_sd.c                                                  *
** Author:                                                                   *
** Date:           07/2006                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe sd card                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2006       Bruce.Chi        Creat                                      *
** 08/2006       haiyang.hu       Modify                                     *
******************************************************************************/
#define _MMI_SD_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_udisk_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
//#ifdef MSDC_CARD_SUPPORT
//#include "sci_types.h"
#include "scm_api.h"
#include "tb_comm.h"
#include "window_parse.h"
#include "mmiacc_id.h"
//#include "mmiacc_menutable.h"
//#include "mmiacc_text.h"
#include "mmidisplay_data.h"
#include "mmifmm_export.h"
#include "mmi_appmsg.h"
#include "gui_ucs2b_converter.h"
//#include "mmiebook_file.h"
#include "mmimp3_export.h"
#include "mmipub.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_default.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmicom_string.h"
#include "mmiudisk_export.h"
#ifndef WIN32
#include "prod_param.h"
#include "sio.h"
#include "ref_param.h"
#endif
#include "mmisd_export.h"
#include "mmisd_internal.h"
#include "guimenu.h"
#include "guicommon.h"
#include "guisoftkey.h"
#include "guitext.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif

//#include "mmk_modinfo.h"
#include "guirichtext.h"
#include "mmifmm_id.h"

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

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN       s_plug_in_devices[MMI_SDCARD_MAX_NUM] = {FALSE};    //SD卡是否插入  
#ifdef WIN32
LOCAL BOOLEAN       s_is_cardlog_com_on = FALSE;
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : when startup reset cardlog debug com
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
#ifndef WIN32
LOCAL void ResetCardLogDebugCom(void);
#endif
/*****************************************************************************/
//  Description : int sd card and register sd device to ffs
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitSDAndRegDevice(MMIFILE_DEVICE_E slot_type);

/*****************************************************************************/
//  Description : handle SD msg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
#ifndef WIN32
LOCAL void HandleSDMsg(SCM_SLOT_NAME_E slot_name,SCM_EVENT_E event);/*lint !e113*/
#endif
/*****************************************************************************/
//  Description : create fold
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL void InitFolder(
                      const wchar *device_ptr,
                      uint16  device_len,
                      const wchar *dir_ptr,
                      uint16  dir_len
                      );

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Init SD card
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISD_Init(void)
{

    SFS_ERROR_E     error   =   SFS_ERROR_NONE;
    SCM_STATUS      scmstatus   =   SCM_STATUS_NORMAL;
#ifdef BOARD_CONFIG_AntisW3 //1924362
#ifndef MAINLCD_SIZE_128X64 
    SCM_SLOT_NAME_E slot_name = SCM_SLOT_0;
#endif
#endif

    //SCI_TRACE_LOW:"mmisd:MMIAPISD_Init: register Udisk enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_128_112_2_18_3_3_59_0,(uint8*)"");

    error = MMIAPIFMM_RegisterDevice(MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK));
   // if (SFS_NO_ERROR == error || SFS_ERROR_INVALID_PARAM == error)
    if (SFS_NO_ERROR == error )/*revised by yongwei for8801h format usdik*/
    {
        //SCI_TRACE_LOW:"mmisd:MMIAPISD_Init: register Udisk success"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_134_112_2_18_3_3_59_1,(uint8*)"");
    }
    else
    {
        //SCI_TRACE_LOW:"mmisd:MMIAPISD_Init: register Udisk fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_138_112_2_18_3_3_59_2,(uint8*)"");
        MMIAPIFMM_FormatDevice(
            MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK),
            MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK),
            MMIFILE_FORMAT_AUTO
        );
    }

#ifdef WIN32
    error = MMIAPIFMM_RegisterDevice(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SYSTEM));
    if (SFS_NO_ERROR != error )
    {
        MMIAPIFMM_FormatDevice(
			MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM),
			MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SYSTEM),
			MMIFILE_FORMAT_AUTO
		);
	}
    else
    {
        SCI_TRACE_LOW("MMIAPIFMM_RegisterDevice fail, error = %d", error);
    }
#endif
#ifdef BOARD_CONFIG_AntisW3 //1924362
#ifndef MAINLCD_SIZE_128X64 
//#ifndef WIN32
#ifdef MSDC_CARD_SUPPORT
    //init all sd card 	
	for(slot_name = SCM_SLOT_0; slot_name < SCM_SLOT_0 + MMI_SDCARD_MAX_NUM; slot_name++)/*lint !e520*/
	{///coverity10743
#ifdef WIN32
		MMIAPISD_SetSDPlugIn(MMISD_GetSDCardType(slot_name) , TRUE);
		MMIAPIFMM_RegisterDevice(MMIAPIFMM_GetDevicePath(MMISD_GetSDCardType(slot_name)), MMIAPIFMM_GetDevicePathLen(MMISD_GetSDCardType(slot_name)));
#else
		if(!SCM_RegEvent(slot_name,SCI_IdentifyThread(),SCM_ALL_EVT,(SCM_EVENT_CALLBACK)HandleSDMsg))
		{
			//SCI_PASSERT(0, ("slot_name %d", slot_name));
			//SCI_TRACE_LOW:"mmisd:MMIAPISD_Init: register sd callback func failed"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_157_112_2_18_3_3_59_3,(uint8*)"");
			scmstatus = FALSE;
		}
		else
		{

			scmstatus = SCM_GetSlotStatus(slot_name);
			
			switch(scmstatus)
			{
			case SCM_STATUS_NORMAL:
				MMIAPISD_SetSDPlugIn(MMISD_GetSDCardType(slot_name) , TRUE);
				//s_is_sd_plug_in = TRUE;                //记录SCM状态是否oK
				if (MMIAPIENG_GetIsCardLogOn())
				{
					//开启cardlog
					MMIAPIENG_StartupAdjustCardLogOn();
				}
				else
				{
					//回复cardlog debug com
					ResetCardLogDebugCom();
					//在MMI初始化时初始化SDCard硬件接口
					InitSDAndRegDevice(MMISD_GetSDCardType(slot_name));
				}
				MMISD_SDPlugInit(TRUE, slot_name);        //记录sd卡插入还是拔出           
				break;
			case SCM_STATUS_ERROR:                      //SD卡插入，但是出现错误
				MMIAPISD_SetSDPlugIn(MMISD_GetSDCardType(slot_name) , FALSE);
				ResetCardLogDebugCom();
				MMISD_SDPlugInit(TRUE, slot_name);         
				break;
			case SCM_NOT_EXIST:   //SD卡不存在
				MMIAPISD_SetSDPlugIn(MMISD_GetSDCardType(slot_name), FALSE);
				ResetCardLogDebugCom();
				MMISD_SDPlugInit(FALSE, slot_name);
				break;
			default:
				//SCI_PASSERT(0, ("slot_name %d", slot_name));
				//SCI_TRACE_LOW:"MMIAPISD_Init scmstatus = %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_196_112_2_18_3_4_0_4,(uint8*)"d",scmstatus);
				break;	
			} 
		} 
#endif
	} 
#endif
//#endif
#endif
#endif
    return TRUE;
}

#ifdef MSDC_CARD_SUPPORT
/*****************************************************************************/
//  Description : init sdcard
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISD_InitSDCard(SCM_SLOT_NAME_E scm_name)
{
    BOOLEAN     status  =   FALSE;
    
    status = InitSDAndRegDevice(MMISD_GetSDCardType(scm_name));
    
    return (status);
}

/*****************************************************************************/
//  Description : open SD room infor window
//  Global resource dependence :
//  Author:jassmine.meng
//  Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMIAPISD_OpenMemoryInfoWin(void)
//{
//    if (MMK_CreateWin((uint32 *)MMISD_MEMORY_WIN_TAB, PNULL))
//    {
//        return TRUE;
//    }
//    else
//    {
//        return FALSE;
//    }    
// }

///*****************************************************************************/
////  Description : to handle the message of  SDcard management window   
////  Global resource dependence : 
////  Author:Rui.Zhang
////  Note: 
///*****************************************************************************/
//LOCAL MMI_RESULT_E  HandleSDcardManageWinMsg(
//                                             MMI_WIN_ID_T win_id, 
//                                             MMI_MESSAGE_ID_E msg_id, 
//                                             DPARAM param
//                                             )
//{
//    uint32                 time_out = 0;
//    MMI_MENU_GROUP_ID_T    group_id = 0;
//    MMI_MENU_ID_T          menu_id  = 0;
//    MMI_RESULT_E           result   = MMI_RESULT_TRUE;
//    
//    switch (msg_id)
//    {
//    case MSG_OPEN_WINDOW:
//        MMIAUDIO_PauseBgPlay(MMIBGPLAY_MODULE_SD);
//        MMK_SetAtvCtrl(win_id, MMI_SDCARD_MANAGE_CTRL_ID);
//        GUIWIN_SetSoftkeyTextId(win_id,  STXT_OK, TXT_NULL, STXT_RETURN, FALSE);
//        break;
//        
//    case MSG_CTL_PENOK:
//    case MSG_CTL_MIDSK:
//    case MSG_CTL_OK:
//    case MSG_APP_OK:
//    case MSG_APP_WEB:
//        GUIMENU_GetId(MMI_SDCARD_MANAGE_CTRL_ID,&group_id,&menu_id);
//        switch (menu_id)
//        {
//        case ID_ACC_SDCARD_MANAGE_FORMAT:
//            MMIPUB_OpenQueryWinByTextId(TXT_MMIMULTIM_SDCARD_FORMAT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
//            break;
//            
//        case ID_ACC_SDCARD_MANAGE_STATUS:
//            MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
//            MMK_PostMsg(win_id, MSG_UDISK_START_OPERATION, PNULL, 0); 
//            break;
//            
//        default:
//            break;
//        }
//        break;
//        
//    case MSG_UDISK_START_OPERATION:
//        GUIMENU_GetId(MMI_SDCARD_MANAGE_CTRL_ID, &group_id, &menu_id);
//        switch (menu_id)
//        {
//        case ID_ACC_SDCARD_MANAGE_FORMAT:
//            if (MMIAPISD_IsSDPlugIn())
//            {
//                MMIAPISD_HandleBeforeSDCardFormat();
//                MMIAPIFMM_FormatDeviceAsyn(
//                    MMIFILE_DEVICE_SDCARD, 
//                    MMIFILE_DEVICE_SDCARD_LEN, 
//                    MMIFILE_FORMAT_AUTO,
//                    win_id, 
//                    MSG_UDISK_FORMAT_CNF
//                    );
//            }
//            else
//            {
//                MMIPUB_OpenAlertWinByTextId(
//                    PNULL,  
//                    TXT_COMMON_SD_FAIL,
//                    TXT_NULL,
//                    IMAGE_PUBWIN_WARNING,
//                    PNULL,
//                    PNULL,
//                    MMIPUB_SOFTKEY_ONE,
//                    PNULL
//                    );
//            }
//            break;
//                
//        case ID_ACC_SDCARD_MANAGE_STATUS:  
//            if(MMIAPIFMM_GetDeviceStatus( MMIFILE_DEVICE_SDCARD, MMIFILE_DEVICE_SDCARD_LEN))
//            {
//                MMIAPISD_OpenMemoryInfoWin();
//            }
//            else
//            {
//                SFS_ERROR_E error = SFS_NO_ERROR;
//                error = MMIAPIFMM_RegisterDevice((uint16 *)MMIFILE_DEVICE_SDCARD, MMIFILE_DEVICE_SDCARD_LEN);
//                SCI_TRACE_LOW("HandleSDcardManageWinMsg sd  exist %d", error);
//                MMIPUB_CloseAlertWin();
//                if (SFS_ERROR_INVALID_PARAM == error)
//                {
//                    MMIPUB_OpenAlertWinByTextId(
//                        PNULL,
//                        TXT_SD_NOT_SUPPORT,
//                        TXT_NULL,
//                        IMAGE_PUBWIN_WARNING,
//                        PNULL,
//                        PNULL,
//                        MMIPUB_SOFTKEY_ONE,
//                        PNULL
//                        );
//                }
//                else
//                {                
//                    MMIPUB_OpenAlertWinByTextId(
//                        PNULL,
//                        TXT_COMMON_SD_FAIL,
//                        TXT_NULL,
//                        IMAGE_PUBWIN_WARNING,
//                        PNULL,
//                        PNULL,
//                        MMIPUB_SOFTKEY_ONE,
//                        PNULL
//                        );
//                }
//            }
//            break;
//                
//        default:
//            break;
//        }
//        break;
//        
//    case MSG_UDISK_FORMAT_CNF:
//        MMIAPIUdisk_HandleFormatCnf(*(BOOLEAN *)param);
//        break;
//    
//    case MSG_APP_CANCEL:
//    case MSG_CTL_CANCEL:
//        MMK_CloseWin(win_id);
//        break;
//                
//    case MSG_PROMPTWIN_OK:
//        MMIPUB_CloseQuerytWin(PNULL);
//        GUIMENU_GetId(MMI_SDCARD_MANAGE_CTRL_ID, &group_id, &menu_id);
//        switch (menu_id)
//        {
//        case ID_ACC_SDCARD_MANAGE_FORMAT:
//            if (MMIAPISD_IsSDPlugIn())
//            {
//                MMI_WIN_ID_T    alert_win_id = MMISDCARD_FORMATTING_WIN_ID;
//                
//                MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&alert_win_id,PNULL,MMIPUB_SOFTKEY_NONE,MMIAPISD_NotInterruptFfs);
//                MMK_PostMsg(win_id, MSG_UDISK_START_OPERATION, PNULL, 0); 
//            }
//            else
//            {
//                MMIPUB_OpenAlertWinByTextId(
//                    PNULL,  
//                    TXT_COMMON_SD_FAIL,
//                    TXT_NULL,
//                    IMAGE_PUBWIN_WARNING,
//                    PNULL,
//                    PNULL,
//                    MMIPUB_SOFTKEY_ONE,
//                    PNULL
//                    );
//            }
//            break;
//            
//        default:
//            break;
//        }
//        break;
//                
//    case MSG_PROMPTWIN_CANCEL:
//        MMIPUB_CloseQuerytWin(PNULL);
//        break;
//        
//    case MSG_CLOSE_WINDOW:
//        //退出Udisk使用情况界面，则恢复mp3的播放
//        MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_SD);
//        break;
//        
//    default:
//        result = MMI_RESULT_FALSE;
//        break;
//    }
//    
//    return (result);
// }

/*****************************************************************************/
//  Description : before sd card format, we have something to do.
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISD_HandleBeforeSDCardFormat(void)
{
#ifdef JAVA_SUPPORT_IA    
    MMIAPIJAVA_Finalize_Special();
#endif
#ifdef MMI_AUDIO_PLAYER_SUPPORT      
    MMIAPIMP3_ResetAllMp3();
#endif
#ifdef KURO_SUPPORT
    MMIAPIKUR_ResetAllKur();
#endif
    
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISD_NotInterruptFfs(
                                      MMI_WIN_ID_T     win_id,
                                      MMI_MESSAGE_ID_E  msg_id,
                                      DPARAM              param
                                      )
{
    MMI_RESULT_E    result  =   MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pub_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);//cr188283
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
		if (PNULL != pub_info_ptr)//coverity18766
		{
			MMK_PostMsg((MMI_WIN_ID_T)pub_info_ptr->user_data,MSG_FMM_FORMAT,PNULL,0);//cr188283
		}
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
        
    case MSG_APP_RED:
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_TurnOnBackLight();
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : is SD card plug in
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISD_IsSDPlugIn(MMIFILE_DEVICE_E device_type)
{
	uint16 base_index = MMI_DEVICE_SDCARD;
	if(device_type >= MMI_DEVICE_SDCARD && device_type < MMI_DEVICE_SDCARD + MMI_SDCARD_MAX_NUM)
	{
		return s_plug_in_devices[(uint16)(device_type - base_index)];
	}
    return FALSE;
}


/*****************************************************************************/
//  Description : is SD card plug in
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISD_SetSDPlugIn(MMIFILE_DEVICE_E device_type , BOOLEAN is_sd_plug_in)
{    
	uint16 base_index = MMI_DEVICE_SDCARD;
    if(device_type >= MMI_DEVICE_SDCARD && device_type < MMI_DEVICE_SDCARD + MMI_SDCARD_MAX_NUM)
	{
		s_plug_in_devices[(uint16)(device_type - base_index)] = is_sd_plug_in;
	}
}
/*****************************************************************************/
//  Description : int sd card and register sd device to ffs
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitSDAndRegDevice(MMIFILE_DEVICE_E device_type)
{
    if (SFS_NO_ERROR == MMIAPIFMM_RegisterDevice(MMIAPIFMM_GetDevicePath(device_type),  MMIAPIFMM_GetDevicePathLen(device_type)))
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : stop SD card using
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISD_StopSDCard(MMIFILE_DEVICE_E device_type)
{
    BOOLEAN     result      =  FALSE;

    //SCI_TRACE_LOW:"MMIAPISD_StopSDCard, enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_521_112_2_18_3_4_0_5,(uint8*)"");
    
    __MMI_PRINT_DEADLOOP_CHECK__();
    if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type)))
    {
        __MMI_PRINT_DEADLOOP_CHECK__();
        if (MMIAPIFMM_UnRegisterDevice( MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type)))
        {
            result = TRUE;
        }
        __MMI_PRINT_DEADLOOP_CHECK__();
    }    
    __MMI_PRINT_DEADLOOP_CHECK__();
    
    return (result);
}

/*****************************************************************************/
//  Description : stop current operation when sd plug
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISD_StopOperaSDPlug(void)
{
    if(MMK_IsOpenWin(MMIFMM_MEMORY_DETAIL_WIN_ID))
    {
        //SCI_TRACE_LOW:"MMIAPISD_StopOperaSDPlug"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_546_112_2_18_3_4_0_6,(uint8*)"");
        MMIPUB_CloseAlertWin();
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(MMISDCARD_FORMATTING_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : create fold
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL void InitFolder(
                      const wchar *device_ptr,
                      uint16  device_len,
                      const wchar *dir_ptr,
                      uint16  dir_len
                      )
{
    uint16  full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] =   {0};
    uint16  full_path_len                               =   MMIFILE_FULL_PATH_MAX_LEN+2;
    
    if((NULL == device_ptr) || (0 == device_len) || (NULL == dir_ptr) || (0 == dir_len))
    {
        //SCI_TRACE_LOW:"[MMIUDISK]InitFolder invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_569_112_2_18_3_4_0_7,(uint8*)"");
        return;
    }
    if( MMIAPIFMM_CombineFullPath( device_ptr, device_len,
        dir_ptr, dir_len, 
        NULL, 0, 
        full_path_name, &full_path_len))
    {
        if(!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
        {
            MMIAPIFMM_CreateDir(full_path_name, full_path_len);
        }
    }
}

/*****************************************************************************/
//  Description : create default fold in SD card
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMISD_CreateDefaultFold(MMIFILE_DEVICE_E device_type)
{
    if(MMIAPIFMM_GetDeviceTypeStatus(device_type))
    {
        MMIAPIFMM_InitSystemFolder(MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type),
                                MMIMULTIM_DIR_SYSTEM, MMIMULTIM_DIR_SYSTEM_LEN);
        // Picture
        InitFolder(MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type),
            MMIMULTIM_DIR_PICTURE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE));
        
        // Music
        InitFolder(MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type),
            MMIMULTIM_DIR_MUSIC, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MUSIC));
        
        // Movie
        InitFolder(MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type),
            MMIMULTIM_DIR_MOVIE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_MOVIE));
#ifdef MMI_KING_MOVIE_SUPPORT
         // KingMovie
        InitFolder(MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type),
            MMIMULTIM_DIR_KINGMOVIE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_KINGMOVIE));
#endif
#ifdef EBOOK_SUPPORT        

        // Ebook
        InitFolder(MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type),
            MMIMULTIM_DIR_EBOOK, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_EBOOK));
#endif
#ifdef  JAVA_SUPPORT_SUN
        MMIAPIJAVA_CreateJavaDirectory(device_type);
#endif
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:vincent.shang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISD_IsFormatWaitWinOpened(void)
{
    return MMK_IsOpenWin(MMISDCARD_FORMATTING_WIN_ID);
}
/*****************************************************************************/
//  Description:   init for cardlog
//  Author     :   bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISD_InitForFAT(MMIFILE_DEVICE_E device_type)
{
    BOOLEAN     is_return_success   =   FALSE;
    
    //SCI_TRACE_LOW:"mmi_sd.c enter MMIAPISD_InitForFAT()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_639_112_2_18_3_4_0_8,(uint8*)"");
    is_return_success = InitSDAndRegDevice(device_type);
    return is_return_success;
}

/*****************************************************************************/
//  Description:   open cardlog
//  Author     :  bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISD_OpenCardLog(void)
{
#ifndef WIN32   
    SIO_CONTROL_S   m_dcb = {0};        
    m_dcb.flow_control   = 0;
    __MMI_PRINT_DEADLOOP_CHECK__();
    m_dcb.baud_rate      = SIO_GetBaudRate(COM_DEBUG);
    __MMI_PRINT_DEADLOOP_CHECK__();
    

    //关闭当前端口
    SIO_Close(COM_DEBUG); 
    __MMI_PRINT_DEADLOOP_CHECK__();
    //    SCI_ASSERT(0);           
    //打开cardlog端口
    if (PNULL == SIO_Create(COM_DEBUG, DSK_COM0, &m_dcb))
    {
        __MMI_PRINT_DEADLOOP_CHECK__();
        MMIAPISD_CloseCardLog();
        __MMI_PRINT_DEADLOOP_CHECK__();
        return FALSE;
    }
#else
    s_is_cardlog_com_on = TRUE;
#endif
    __MMI_PRINT_DEADLOOP_CHECK__();
    return TRUE;
}

/*****************************************************************************/
//  Description:   close cardlog
//  Author     :   bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISD_CloseCardLog(void)
{
#ifndef WIN32
    SIO_CONTROL_S   m_dcb           =   {0};
    uint8           debug_phy_port  =   0;
    

    m_dcb.flow_control   = 0;
    __MMI_PRINT_DEADLOOP_CHECK__();
    m_dcb.baud_rate      = SIO_GetBaudRate(COM_DEBUG);
    __MMI_PRINT_DEADLOOP_CHECK__();
    
    debug_phy_port = REFPARAM_GetDebugPortPhyNo();
    __MMI_PRINT_DEADLOOP_CHECK__();
    if ((DSK_COM0 == debug_phy_port)
        || (DSK_COM1 == debug_phy_port))    
    {
        debug_phy_port = 0xff;
    }
    
    //关闭cardlog端口
    __MMI_PRINT_DEADLOOP_CHECK__();
    SIO_Close(COM_DEBUG);
    //回复正常端口    
    __MMI_PRINT_DEADLOOP_CHECK__();
    SIO_Create(COM_DEBUG, debug_phy_port, &m_dcb);
    __MMI_PRINT_DEADLOOP_CHECK__();
    
#else
    s_is_cardlog_com_on = FALSE;
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : when startup reset cardlog debug com
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
#ifndef WIN32
LOCAL void ResetCardLogDebugCom(void)
{

    SIO_CONTROL_S   m_dcb   =   {0};
    uint32          PortNo  =   0;
    

    m_dcb.flow_control   = 0;
    m_dcb.baud_rate      = SIO_GetBaudRate(COM_DEBUG);
    
    PortNo = SIO_GetPhyPortNo(COM_DEBUG);   //获得当前的物理口
    //恢复原端口
    if ((DSK_COM0 == PortNo)
        || (DSK_COM1 == PortNo))
    {
        //关闭cardlog端口
        SIO_Close(COM_DEBUG);    
        //回复正常端口
        SIO_Create(COM_DEBUG, 0xff, &m_dcb);
    }


}
#endif
/*****************************************************************************/
//  Description:   is cardlog on
//  Author     :  bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISD_IsCardLogOn(void)
{
#ifndef WIN32
    uint32      PortNo   =  0;
    

    PortNo = SIO_GetPhyPortNo(COM_DEBUG);   //获得当前的物理口
    if ((DSK_COM0 == PortNo)
        || (DSK_COM1 == PortNo))
    {
        return TRUE;
    }
    return FALSE;
#else
    return s_is_cardlog_com_on;
#endif
}

/*****************************************************************************/
//  Description:   is usb log on
//  Author     :  bin.ji
//  Param
//           void
//  Return:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPISD_IsUsbLogOn(void)
{
#ifndef WIN32
    uint32      PortNo   =  0;
    

    PortNo = SIO_GetPhyPortNo(COM_DEBUG);   //获得当前的物理口
    SCI_TRACE_LOW("MMIAPISD_IsUsbLogOn PortNo = %d", PortNo);// @cr239352 renwei add
    
    if ((VIR_COM0 == PortNo)
        || (VIR_COM1 == PortNo))
    {
        return TRUE;
    }
#endif
    return FALSE;
}

/*****************************************************************************/
//  Description : handle SD error
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
#ifndef WIN32
LOCAL void HandleSDMsg(SCM_SLOT_NAME_E slot_name,SCM_EVENT_E event)
{	
    if(SCM_MAX_SLOT_NO <= slot_name)
    {
        //SCI_TRACE_LOW:"[MMIUDISK] HandleSDMsg slot_name %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_804_112_2_18_3_4_1_9,(uint8*)"d", slot_name);
        return;
    }

    //SCI_TRACE_LOW:"HandleSDMsg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_808_112_2_18_3_4_1_10,(uint8*)"");
	
	switch(event)
	{
	case SCM_PLUG_IN_EVT:   //cr125575
		MMIAPICOM_OtherTaskToMMI(VIRTUAL_WIN_ID,MSG_KEYDOWN_SDCARD_DETECT,&slot_name,sizeof(SCM_SLOT_NAME_E));
		//MMK_PostMsg(VIRTUAL_WIN_ID,MSG_KEYDOWN_SDCARD_DETECT ,PNULL,0);
		//SCI_TRACE_LOW:"SCM plug in"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_815_112_2_18_3_4_1_11,(uint8*)"");
		break;
		
	case SCM_PLUG_OUT_EVT:
		MMIAPICOM_OtherTaskToMMI(VIRTUAL_WIN_ID,MSG_KEYUP_SDCARD_DETECT,&slot_name,sizeof(SCM_SLOT_NAME_E));
		//MMK_PostMsg(VIRTUAL_WIN_ID,MSG_KEYUP_SDCARD_DETECT ,PNULL,0);
		//SCI_TRACE_LOW:"SCM plug out"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_821_112_2_18_3_4_1_12,(uint8*)"");
		break;
		
	case SCM_ERROR_EVT:   
		MMIAPICOM_OtherTaskToMMI(VIRTUAL_WIN_ID,MSG_HANDLE_SD_ERR,&slot_name,sizeof(SCM_SLOT_NAME_E));
		//MMK_PostMsg(VIRTUAL_WIN_ID,MSG_HANDLE_SD_ERR ,PNULL,0);
		//SCI_TRACE_LOW:"SCM SLOT Error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SD_827_112_2_18_3_4_1_13,(uint8*)"");
		break;
		
	default:        
		//SCI_ASSERT(0);      
		break;
	}
}
#endif

/*****************************************************************************/
//  Description : get sdcard type by scm slot type
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMISD_GetSDCardType(SCM_SLOT_NAME_E slot_name)
{
	MMIFILE_DEVICE_E sd_type = MMI_DEVICE_SDCARD;

	switch(slot_name)
	{
	case SCM_SLOT_0:
        sd_type = MMI_DEVICE_SDCARD;
		break;
	case SCM_SLOT_1:
        sd_type = MMI_DEVICE_SDCARD_1;
		break;
	default:
		break;
	}

	return sd_type;
}

/*****************************************************************************/
//  Description : get scm slot type by sdcard type  
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC SCM_SLOT_NAME_E MMISD_GetSLOTName(MMIFILE_DEVICE_E device_type)
{
	SCM_SLOT_NAME_E slot_id = SCM_MAX_SLOT_NO;

	switch(device_type)
	{
	case MMI_DEVICE_SDCARD:
        slot_id = SCM_SLOT_0;
		break;
	case MMI_DEVICE_SDCARD_1:
        slot_id = SCM_SLOT_1;
		break;
	default:
		break;
	}

	return slot_id;
}

/*****************************************************************************/
//  Description : check if there is a SD card existed
//  Global resource dependence : 
//  Author:charlie.wang
//  Note:若已选SD卡存在则返回TRUE，若不存在则遍历，若存在另外一张，则赋值p_device，若没有SD卡，则返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISD_IsSDExist(MMIFILE_DEVICE_E *p_device)
{
    uint32          free_space_high     = 0;
    uint32          free_space_low      = 0;
    uint32          cur_free_space_high = 0;
    uint32          cur_free_space_low  = 0;
    uint32          i                   = MMI_DEVICE_SDCARD;
    MMIFILE_ERROR_E file_ret            = SFS_ERROR_NONE;
    BOOLEAN         b_sd_exist          = FALSE;

    do
    {
        if(!MMIAPIFMM_GetDeviceTypeStatus((MMIFILE_DEVICE_E)i))
        {
            continue;
        }

		file_ret = SFS_GetDeviceFreeSpace((uint16 *)MMIAPIFMM_GetDevicePath((MMIFILE_DEVICE_E)i),
								&cur_free_space_high, 
								&cur_free_space_low);
		if(file_ret != SFS_ERROR_NONE)
		{
		    continue;
		}
		
        if(cur_free_space_high > free_space_high //高位大于肯定大于
    		||((cur_free_space_high == free_space_high) //高位相等，低位大于
    		&&(cur_free_space_low > free_space_low))
    		)
		{
			free_space_high = cur_free_space_high;
			free_space_low = cur_free_space_low;

            if(PNULL != p_device)
            {
			    *p_device  = (MMIFILE_DEVICE_E)i;
            }
		}
		
		b_sd_exist = TRUE;
    }while(++i < MMI_DEVICE_NUM);
    
    return b_sd_exist;
}

/*****************************************************************************/
//  Description : check if there is a sd card whose left space is bigger than the min space
//  Global resource dependence : 
//  Author:charlie
//  Note:
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIAPISD_CheckSDSpace(uint32 min_space, MMIFILE_DEVICE_E *p_device)
{
    uint32      i              = MMI_DEVICE_SDCARD;
    uint32      free_high_word = 0;
    uint32      free_low_word  = 0;
    SFS_ERROR_E sfs_result     = SFS_ERROR_NO_SPACE;
    //BOOLEAN     sd_space_ok    = FALSE;
    
    do
    {
        if(!MMIAPIFMM_GetDeviceTypeStatus((MMIFILE_DEVICE_E)i))
        {
            sfs_result = SFS_ERROR_DEVICE;
            continue;
        }
        
        sfs_result = SFS_GetDeviceFreeSpace((uint16 *)MMIAPIFMM_GetDevicePath((MMIFILE_DEVICE_E)i), &free_high_word, &free_low_word);
        if(SFS_ERROR_NONE == sfs_result)
        {
             if(0 == free_high_word && (free_low_word < min_space))
             {
                sfs_result = SFS_ERROR_NO_SPACE;
                continue;
             }
             else
             {
                if(PNULL != p_device)
                {
                    *p_device = (MMIFILE_DEVICE_E)i;
                }
                sfs_result = SFS_ERROR_NONE;
                break;
             }
        }
    }while(++i < MMI_DEVICE_NUM);    
    
    return sfs_result;
}

/*****************************************************************************/
//  Description : check if the fmm_device(SD Card) needs format
//  Global resource dependence : 
//  Author:charlie.wang
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPISD_IsSDNeedFormat(MMIFILE_DEVICE_E fmm_device)
{
    //SFS_ERROR_E error          =   SFS_NO_ERROR;
    BOOLEAN     sd_need_format = FALSE;

    if(MMIAPISD_IsCardLogOn())
    {
        sd_need_format = FALSE;
    }
    else if(MMIAPIFMM_GetDeviceTypeStatus(fmm_device))
    {
        sd_need_format = FALSE;
    }
    else if(SFS_ERROR_INVALID_FORMAT == MMIAPIFMM_RegisterDevice((uint16 *)MMIAPIFMM_GetDevicePath(fmm_device),
              MMIAPIFMM_GetDevicePathLen(fmm_device)))
    {
		/*if(fmm_device == MMI_DEVICE_SDCARD_1)
		{
			SCI_ASSERT(0);
		}*/
        sd_need_format = TRUE;
    }
    else
    {
        sd_need_format = FALSE;
    }
    
    return sd_need_format;
}

/*****************************************************************************/
//  Description : check if there is one sd card existed
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMISD_IsSDExistEx(void)
{
    MMIFILE_DEVICE_E i    = MMI_DEVICE_SDCARD;
    BOOLEAN         SD_OK = FALSE;
    
    do
    {
        if(MMIAPIFMM_GetDeviceTypeStatus((MMIFILE_DEVICE_E)i))
        {
            SD_OK = TRUE;
            break;
        }
    }while(++i < MMI_DEVICE_NUM);    

   return SD_OK;
}

/*****************************************************************************/
//  Description : return existed SD card type
//  Global resource dependence : 
//  Author:
//  Note:返回第一张存在的SD卡，若不存在则返回UDISK
/*****************************************************************************/ 
PUBLIC MMIFILE_DEVICE_E MMISD_GetExistSDCardType(void)
{
    MMIFILE_DEVICE_E  i     = MMI_DEVICE_SDCARD;
    BOOLEAN          SD_OK = FALSE;
    
    do
    {
        if(MMIAPIFMM_GetDeviceTypeStatus((MMIFILE_DEVICE_E)i))
        {
            SD_OK = TRUE;
            break;
        }
    }while(++i < MMI_DEVICE_NUM);    

    if(!SD_OK)
    {
        i = MMI_DEVICE_NUM;
    }

    return i;
}
#else
PUBLIC BOOLEAN MMISD_IsSDExistEx(void)
{
    return FALSE;
}


#endif
