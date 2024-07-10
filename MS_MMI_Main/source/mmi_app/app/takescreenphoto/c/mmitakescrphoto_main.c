/****************************************************************************
** File Name:      mmiptscr_main.c                                               *
** Author:         bin.ji                                                  *
** Date:           04/14/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/14/2011        bin.ji            Create
** 
****************************************************************************/



#include "mmi_app_takescreenphoto.h"
#ifdef TAKESCREENPHOTO_SUPPORT  

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "ui_layer.h"
#include "mmifmm_export.h"
#include "dal_time.h"
#include "mmitakescrphoto_main.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiudisk_export.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
//文件名最大长度
#define MMITSCRPHOTO_FILENAME_MAX_LEN    100
//文件名前缀最大长度
#define MMITSCRPHOTO_MAX_PREFIX_FILENAME_LEN     30

//抓屏需要最小T卡空间（bytes）
#define MMITSCRPHOTO_MIN_NEEDED_SPACE         2048   



/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
              
/*****************************************************************************/
LOCAL BOOLEAN GenPrexfixName(
 						    wchar *prefixname_ptr,//out
                            uint16 prefixname_max_len,//in
                            uint16 *prefixname_len_ptr);//out

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : print动作之前的检查（SD卡是否存在，空间是否足够）
//    Global resource dependence : 
//  Author:   
//    Note:
/*****************************************************************************/
PUBLIC MMITSCRPHOTO_ERROR_STATUS_E MMITSCRPHOTO_PreCheck(void)
{
    
    MMITSCRPHOTO_ERROR_STATUS_E  sdcard_status = MMITSCRPHOTO_ERROR_NONE;

    if(MMIAPIUDISK_UdiskIsRun())
    {
        //u disk runing
        return  MMITSCRPHOTO_SDCARD_USING;
    } 

    
    //判断是否有TF卡
    if (MMI_DEVICE_NUM <= MMIAPIFMM_GetFirstValidSD())
    {
       //SCI_TRACE_LOW:"MMIPTSCR_PreCheck: sd not exist!"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITAKESCRPHOTO_MAIN_76_112_2_18_2_56_19_0,(uint8*)"");
       //判断是否有U盘
        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK)))
        {
            if(MMI_GetFreeSpace(MMI_DEVICE_UDISK) < (MMITSCRPHOTO_MIN_NEEDED_SPACE>>10))
            {
                sdcard_status = MMITSCRPHOTO_SDCARD_NO_SPACE; 
            }
        }
        else
        {
                sdcard_status = MMITSCRPHOTO_SDCARD_NO_EXIST; 
        }
    }
    //判断TF卡空间是否足够
    else if (MMI_GetFreeSpace(MMI_DEVICE_SDCARD) < (MMITSCRPHOTO_MIN_NEEDED_SPACE>>10))
    {
        //SCI_TRACE_LOW:"MMIPTSCR_PreCheck: sd no space!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITAKESCRPHOTO_MAIN_93_112_2_18_2_56_19_1,(uint8*)"");
          //判断是否有U盘
        if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK)))
        {
            if(MMI_GetFreeSpace(MMI_DEVICE_UDISK) < (MMITSCRPHOTO_MIN_NEEDED_SPACE>>10))
            {
                sdcard_status = MMITSCRPHOTO_SDCARD_NO_SPACE; 
            }
        }
        else
        {
            sdcard_status = MMITSCRPHOTO_SDCARD_NO_SPACE; 
        }
    }
    
    return sdcard_status;
}

/*****************************************************************************/
//     Description : 输出lcd_buffer以及图层数据
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITSCRPHOTO_TakePhoto(void)
{
    //const DISPLAY_BLEND_LAYER_T *layer_ptr = PNULL;
    //uint32 i = 0;
    wchar	    *file_dev_ptr = PNULL;
    uint16	    file_dev_len = 0;
    wchar       full_path_name[MMITSCRPHOTO_FILENAME_MAX_LEN] = {0};
    uint16      full_path_len = MMITSCRPHOTO_FILENAME_MAX_LEN;
    //uint32 ColorDepth = 0;
    const wchar print_dir[] = L"Photos\\Screenshots";
    const uint32 print_dir_len = MMIAPICOM_Wstrlen(print_dir);
    const wchar lcd_file_name[] = L"lcd.bmp";
    const uint32 lcd_file_name_len = MMIAPICOM_Wstrlen(lcd_file_name);
 
    UILAYER_PRINTSCREEN_INFO_T print_info = {0};
    wchar prefix_filename[MMITSCRPHOTO_FILENAME_MAX_LEN] = {0};
    uint16 prefix_filename_len = 0;
    wchar filename[MMITSCRPHOTO_FILENAME_MAX_LEN] = {0};
    uint32 filename_len = 0;    
    BOOLEAN ret = FALSE;
    ret = UILAYER_GetBltInfoForPrintScreen(&print_info);
    
    if(!ret)
    {
        //SCI_TRACE_LOW:"MMITSCRPHOTO_TakePhoto -- :GetBltInfoForPrintScreen result is failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITAKESCRPHOTO_MAIN_139_112_2_18_2_56_19_2,(uint8*)"");
        return FALSE;
    }
 if (MMI_DEVICE_NUM  > MMIAPIFMM_GetFirstValidSD())
    {
        file_dev_ptr = MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD);	/*lint !e605 */
        file_dev_len = MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD);
    }
 else
    {
        file_dev_ptr = MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK);	/*lint !e605 */
        file_dev_len = MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK);
    }

    //创建printscreen文件夹
    SCI_MEMSET(full_path_name, 0x00, (MMITSCRPHOTO_FILENAME_MAX_LEN)*sizeof(wchar));
    full_path_len = MMITSCRPHOTO_FILENAME_MAX_LEN;
    if (MMIAPIFMM_CombineFullPath( file_dev_ptr, file_dev_len,
        print_dir, print_dir_len, 
        NULL, 0, 
        full_path_name, &full_path_len))
    {
        if(!MMIFILE_IsFolderExist(full_path_name, full_path_len))
        {
            ret = MMIAPIFMM_CreateDir(full_path_name, full_path_len);
            
            if(!ret)
            {
                //SCI_TRACE_LOW:"MMITSCRPHOTO_TakePhoto -- :MMIFILE_CreateDir result is failed!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITAKESCRPHOTO_MAIN_167_112_2_18_2_56_19_3,(uint8*)"");
                return FALSE;
            }
        }
    }
    //生成文件名前缀
    ret = GenPrexfixName(prefix_filename, ARR_SIZE(prefix_filename), &prefix_filename_len);
    
    if(!ret)
    {
        //SCI_TRACE_LOW:"MMITSCRPHOTO_TakePhoto -- :GenPrexfixName result is failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITAKESCRPHOTO_MAIN_177_112_2_18_2_56_19_4,(uint8*)"");
        return FALSE;
    }
    
    //输出lcd buffer数据
    SCI_MEMSET(filename, 0x00, (MMITSCRPHOTO_FILENAME_MAX_LEN)*sizeof(wchar));
    MMIAPICOM_Wstrncat(filename, prefix_filename, prefix_filename_len);
    MMIAPICOM_Wstrncat(filename, lcd_file_name, lcd_file_name_len);
    filename_len = MMIAPICOM_Wstrlen(filename);
    SCI_MEMSET(full_path_name, 0x00, (MMITSCRPHOTO_FILENAME_MAX_LEN)*sizeof(wchar));
    full_path_len = MMITSCRPHOTO_FILENAME_MAX_LEN;
    if (MMIAPIFMM_CombineFullPath( file_dev_ptr, file_dev_len,
        print_dir, print_dir_len, 
        filename, filename_len, 
        full_path_name, &full_path_len)) 
    {
//        if (PNULL != print_info.lcd_layer_ptr)
        {
            ret = MMIAPICOM_WriteBMPFile(full_path_name, print_info.lcd_layer_blend.layer_info.width, print_info.lcd_layer_blend.layer_info.height, (uint8*)print_info.lcd_layer_blend.layer_info.buf_addr, 16);
            
            if(!ret)
            {
                //SCI_TRACE_LOW:"MMIPTSCR_Print -- :MMIAPIPTSCR_WriteBMPFile result is failed!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITAKESCRPHOTO_MAIN_199_112_2_18_2_56_19_5,(uint8*)"");
                return FALSE; 
            }
            
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMITSCRPHOTO_TakePhoto -- :MMIAPIFMM_CombineFullPath result is failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITAKESCRPHOTO_MAIN_207_112_2_18_2_56_19_6,(uint8*)"");
    }
     
    return TRUE;
}

/*****************************************************************************/
//     Description : 根据当前时间生成文件名前缀
//    Global resource dependence : 
//  Author:    
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GenPrexfixName(
 						    wchar *prefixname_ptr,//out
                            uint16 prefixname_max_len,//in
                            uint16 *prefixname_len_ptr//out
                            )
{
    uint8 u8filename[MMITSCRPHOTO_MAX_PREFIX_FILENAME_LEN] = {0};
    uint16 wstr_len = 0;
    BOOLEAN ret = FALSE;
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
    uint32      tick = 0;
    
    if (PNULL != prefixname_ptr)
    {
        tick = SCI_GetTickCount();
        TM_GetSysDate(&sys_date);
        TM_GetSysTime(&sys_time);

        sprintf((char*)u8filename, "%04d_%02d_%02d_%02d_%02d_%02d_%03d_", sys_date.year, sys_date.mon, sys_date.mday, sys_time.hour, sys_time.min, sys_time.sec, tick&0xff);
        wstr_len = strlen((char*)u8filename);
        wstr_len = MIN(wstr_len, MMITSCRPHOTO_MAX_PREFIX_FILENAME_LEN);
        MMI_STRNTOWSTR(prefixname_ptr, prefixname_max_len, u8filename, ARR_SIZE(u8filename), wstr_len);
        if (PNULL != prefixname_len_ptr)
        {
            *prefixname_len_ptr = MMIAPICOM_Wstrlen(prefixname_ptr);
        }        
        ret = TRUE;
    }
    return ret;
}


#endif  //PRINTSCREEN_SUPPORT
