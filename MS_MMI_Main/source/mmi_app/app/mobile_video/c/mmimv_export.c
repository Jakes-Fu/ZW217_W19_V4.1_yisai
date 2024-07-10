/*****************************************************************************
** File Name:      mmimv_export.c                                           *
** Author:                                                                   *
** Date:           05/2010                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       		     Creat
******************************************************************************/

#define _MMIMV_EXPORT_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmimv_export.h"
//#include "mv_timer_api.h"
#include "mmimv_wintab.h"
#include "mmimv_id.h"
#include "mmipub.h"
#include "mmi_default.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmieng.h"
#include "mmicc_export.h"
#include "mv_base_api.h"
#include "mv_player_api.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mv_debug.h"
#include "mv_mm_api.h"
#include "mv_timer_api.h"
#include "Mmivp_text.h"
#include "mmifmm_export.h"
#include "mmi_text.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
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

                                                 
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

#define     MOBILEVIDEO_DIR                        L"MobileVideo"
#define     MOBILEVIDEO_DIR_LEN                    (11)

#define     MOBILEVIDEO_MODULE                        L"module.zip"
#define     MOBILEVIDEO_MODULE_LEN                    (10)

#define     MOBILEVIDEO_IMAGE                        L"image.zip"
#define     MOBILEVIDEO_IMAGE_LEN                    (9)
/*****************************************************************************/
//  Description : initialize mobile video module
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMV_InitModule(void)
{
    //register menu here

    //register win id
    MMIMV_RegWinIdNameArr();
}

/*****************************************************************************/
//  Description : Init mv app 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMV_Init(void)
{
    MMIMV_Init();
}


///*****************************************************************************/
////  Description:  display text string
////	Global resource dependence: 
////  Author: 
////	Note:
///*****************************************************************************/
//_INT try_text_out(void
//      )									//当前字体颜色
//{
//
//    GUI_RECT_T  display_rect ={0};
//    GUI_RECT_T  clip_rect ={0};
//    GUISTR_STYLE_T text_style ={0};
//    MMI_STRING_T  str_data={0};
//   
//    GUI_COLOR_T txt_color =0;
//  
//    GUISTR_STATE_T  state =  GUISTR_STATE_SINGLE_LINE;
//   
//    GUISTR_INFO_T str_info = {0};
//    GUISTR_BUF_T dest_buf = {0};
//
//    GUI_RECT_T   result_rect = {0};
//    wchar        wChar = 0x767b;
//    uint8        pBuffer[240*320] = {0};
//
//    clip_rect.left = 0;
//    clip_rect.top = 0;        
//    clip_rect.right = 239;
//    clip_rect.bottom = 319;      
//
//    str_data.wstr_ptr = &wChar;
//    str_data.wstr_len = (uint16)MMIAPICOM_Wstrlen(&wChar);   
//
//
//    //font size
//    text_style.font = 14;     
//
//    //should handle font statle here
//
//    text_style.font_color = 24582;
//    text_style.angle = ANGLE_0;
//
//    //text_style.font_color = 0x0000;   //red color
//
//    GUISTR_GetStringInfo(&text_style,&str_data, state, &str_info );
//
//
//    display_rect.left = 103;
//    display_rect.top= (int16)241;
//    display_rect.right = (int16)(103+ str_info.width);
//	display_rect.bottom = (int16)(241+str_info.height);
//
//
//    dest_buf.buffer = pBuffer;
//    dest_buf.mem_width = MMI_MAINSCREEN_WIDTH;
//    dest_buf.height = MMI_MAINSCREEN_HEIGHT;
//    dest_buf.width  = MMI_MAINSCREEN_WIDTH;   
//
//
//    result_rect = GUISTR_DrawTextInRect( 
//                        (GUISTR_BUF_T *)&dest_buf,
//                        (const GUI_RECT_T *)&display_rect,       //the fixed display area
//                        (const GUI_RECT_T *)&clip_rect,       //用户要剪切的实际区域
//                        (const MMI_STRING_T *)&str_data,
//                        &text_style,
//                        state,
//			            GUISTR_TEXT_DIR_AUTO
//                        );   
//    
//            return 1;
// }

/*****************************************************************************/
//  Description : mobile video entry
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMV_Entry(void)
{
    wchar           full_path_module[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16          full_path_module_len  = MMIFILE_FULL_PATH_MAX_LEN+2;
    wchar           full_path_image[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16          full_path_image_len  = MMIFILE_FULL_PATH_MAX_LEN+2;
    const wchar     * dir  = MOBILEVIDEO_DIR;
    uint16          dir_len = MOBILEVIDEO_DIR_LEN;
    const wchar     * file_module  = MOBILEVIDEO_MODULE;
    uint16          file_module_len = MOBILEVIDEO_MODULE_LEN;
    const wchar     * file_iamge  = MOBILEVIDEO_IMAGE;
    uint16          file_iamge_len = MOBILEVIDEO_IMAGE_LEN;
    BOOLEAN         result = FALSE;
    MMIFILE_DEVICE_E device_type = MMI_DEVICE_UDISK;
    const wchar *   dev_name = NULL;
    uint16          dev_name_len = 0;
    
    //MV_TRACE_LOW:"[MV]--WD[MMIAPIMV_Entry]:enter mv"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMV_EXPORT_196_112_2_18_2_39_7_1,(uint8*)"");

    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else if(MMIAPISD_IsCardLogOn())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_CARDLOG_USING);
       
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
        
    }
    else if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
       
    } 
        //JAVA 后台运行,内存不足，提示退出
#ifdef JAVA_SUPPORT
    else if (MMIAPIJAVA_IsJavaRuning())
    {
        MMIPUB_OpenAlertWarningWin( TXT_EXIT_BACKGROUND_JAVA);
    }
#endif
#ifdef BROWSER_SUPPORT
    else if (MMIAPIBROWSER_IsRunning())
    {
        MMIPUB_OpenAlertWarningWin( TXT_SET_MANUAL_UPDATE_TIME_PROMPT);
    }
#endif
    else
    {  
        device_type = MMIAPIFMM_GetFirstValidDevice();
        if(device_type >= MMI_DEVICE_NUM)
        {
            device_type = MMI_DEVICE_SYSTEM;
        }
        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type)))
        {
            dev_name = MMIAPIFMM_GetDevicePath(device_type);
            dev_name_len = MMIAPIFMM_GetDevicePathLen(device_type);
        }
        
    
        if(!MMIAPIFMM_CombineFullPath(dev_name, dev_name_len, dir, dir_len, file_module, file_module_len, full_path_module, &full_path_module_len)
           || !MMIAPIFMM_CombineFullPath(dev_name, dev_name_len, dir, dir_len, file_iamge, file_iamge_len, full_path_image, &full_path_image_len)
           || !MMIAPIFMM_IsFileExist((const wchar *) full_path_module,  full_path_module_len)
           || !MMIAPIFMM_IsFileExist((const wchar *) full_path_image,  full_path_image_len)
           )
        {
            MMIPUB_OpenAlertWarningWin( TXT_COMMON_RESOURCE_NO_EXIST);
            return FALSE;
        }

        //try_text_out();
    	//open main window
        MMIMV_OpenMainWin();

#ifndef _WIN32
        MV_CreatMemHandle();
	    MV_init(80);
	    CM_MobileTV_Init();       
#endif
        result = TRUE;
    }    
    return result;
}


/*****************************************************************************/
//  Description : mobile video exit
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMV_Exit(void)
{
    if (MMK_IsOpenWin(MMIMV_MAIN_WIN_ID))
    {
#ifndef WIN32    
		//视频通话释放资源占用,此处不用释放手机视屏
//      CM_MobileTV_Terminate();	//此函数调用的结果是:退出手机视频
	#if 0
        MV_Exit(); // 调用次函数会注销Fsm导致并发结束后播放失败
	#else
		MV_Stop();
	#endif
#endif
    }
}

/*****************************************************************************/
//  Description : mobile video exit
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMV_Pause(void)
{
    if (MMK_IsOpenWin(MMIMV_MAIN_WIN_ID))
    {
		MV_Pause();
    }
}

/*****************************************************************************/
//  Description : is living
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMV_IsLiving(void)
{
    return MV_IsLiving();
}

/*****************************************************************************/
//  Description : is open mobile video
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMV_IsOpenMobileVideo(void)
{
    return MMK_IsOpenWin(MMIMV_MAIN_WIN_ID);
}
