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

#define MMIPTSCR_MAIN_C


#include "mmi_app_printscreen_trc.h"
#ifdef PRINTSCREEN_SUPPORT

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
#include "mmiptscr_export.h"
#include "mmiptscr_save.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_common.h"
#include "mmi_nv.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
//文件名最大长度
#define MMIPTSCR_FILENAME_MAX_LEN    100
//文件名前缀最大长度
#define MMIPTSCR_MAX_PREFIX_FILENAME_LEN     30
//自动抓屏模式的定时器
#define MMIPTSCR_AUTORUN_TIMEOUT     500
//自动抓屏模式的定时器次数
#define MMIPTSCR_AUTORUN_TIMER_MAX_COUNT     800
//抓屏需要最小T卡空间（bytes）
#define MMIPTSCR_MIN_NEEDED_SPACE         2048   


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL MMIPTSCR_RUNMODE_E s_ptscr_runmode = MMIPTSCR_RUNMODE_OFF;
LOCAL uint8   s_ptscr_autorun_timer_id           = 0;          /*自动模式抓屏的Timer id*/
LOCAL uint32  s_ptscr_autorun_timer_count      = 0;          /*自动模式抓屏的Timer记数*/

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
// 	Description : 处理autorun的Timer事件
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void HandleAutoRunTimeTimerEvent(uint8 timerid, uint32 param);

/*****************************************************************************/
// 	Description : 启动或者停止autorun Timer
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void StartOrStopAutoRunTimer(
                                   BOOLEAN is_start
                                   );
      
/*****************************************************************************/
//     Description : 设置抓屏的运行模式
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void SetRunMode(
                      MMIPTSCR_RUNMODE_E runmode
                      );

/*****************************************************************************/
//     Description : 根据当前时间生成文件名前缀
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GenPrexfixName(
 						    wchar *prefixname_ptr,//out
                            uint16 prefixname_max_len,//in
                            uint16 *prefixname_len_ptr//out
                            );


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*ms00239652 renwei modify begin*/ 
/*****************************************************************************/
//     Description : print动作之前的检查（SD卡是否存在，空间是否足够）
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC MMIPTSCR_ERROR_STATUS_E MMIAPIPTSCR_PreCheck(void)
{
    
    MMIPTSCR_ERROR_STATUS_E sdcard_status = MMIPTSCR_ERROR_NONE;
    
    //判断是否有TF卡
    if (MMI_DEVICE_NUM <= MMIAPIFMM_GetFirstValidSD())
    {
        //SCI_TRACE_LOW:"MMIAPIPTSCR_PreCheck: sd not exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_MAIN_113_112_2_18_2_47_34_0,(uint8*)"");
        
        sdcard_status = MMIPTSCR_SDCARD_NO_EXIST;
    }
    //判断TF卡空间是否足够
    else if (MMI_GetFreeSpace(MMI_DEVICE_SDCARD) < (MMIPTSCR_MIN_NEEDED_SPACE>>10))
    {
        //SCI_TRACE_LOW:"MMIAPIPTSCR_PreCheck: sd no space!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_MAIN_120_112_2_18_2_47_34_1,(uint8*)"");
        
        sdcard_status = MMIPTSCR_SDCARD_NO_SPACE;
    }
    
    return sdcard_status;
}



/*****************************************************************************/
//     Description : extract lcd buffer
//    Global resource dependence : 
//  Author:   sheng.zhu
//    Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPTSCR_GetLCDBuffer(unsigned char *lcd_buffer_ptr, \
													uint32 lcd_buffer_size_max, \
													uint32 *lcd_buffer_size_ptr, \
													uint32 *lcd_width_ptr, \
													uint32 *lcd_height_ptr)
{
	UILAYER_PRINTSCREEN_INFO_T print_info = {0};

    BOOLEAN ret = FALSE;
	uint8* pix = NULL;
	uint16   w;
	uint16   h;

	if((NULL == lcd_buffer_ptr) || (NULL == lcd_buffer_size_ptr)|| \
		(NULL == lcd_width_ptr) || (NULL == lcd_height_ptr) )
	{
		return FALSE;
	}
	
    ret = UILAYER_GetBltInfoForPrintScreen(&print_info);
    if(!ret)
    {        
        return FALSE;
    }	
	
	w = print_info.lcd_layer_blend.layer_info.width;
	h = print_info.lcd_layer_blend.layer_info.height;
	pix = (uint8*)print_info.lcd_layer_blend.layer_info.buf_addr;
	
	if(w*h > lcd_buffer_size_max)
	{
		return FALSE;
	}

	SCI_MEMCPY(lcd_buffer_ptr, pix, w*h*2);
	*lcd_buffer_size_ptr = w*h*2;
	*lcd_width_ptr = w;
	*lcd_height_ptr = h;

	return TRUE;	
}

/*ms00239652 renwei modify end*/ 
/*****************************************************************************/
//     Description : 输出lcd_buffer以及图层数据
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPTSCR_Print(BOOLEAN is_includelayer)
{
    const DISPLAY_BLEND_LAYER_T *layer_ptr = PNULL;
    uint32 i = 0;
    wchar	    *file_dev_ptr = PNULL;
    uint16	    file_dev_len = 0;
    wchar       full_path_name[MMIPTSCR_FILENAME_MAX_LEN] = {0};
    uint16      full_path_len = MMIPTSCR_FILENAME_MAX_LEN;
    uint32 ColorDepth = 0;
    const wchar print_dir[] = L"Screenshots";
    const uint32 print_dir_len = MMIAPICOM_Wstrlen(print_dir);
    const wchar lcd_file_name[] = L"lcd.bmp";
    const uint32 lcd_file_name_len = MMIAPICOM_Wstrlen(lcd_file_name);
#ifdef SUBLCD_SIZE
    const wchar sublcd_file_name[] = L"sublcd.bmp";
    const uint32 sublcd_file_name_len = MMIAPICOM_Wstrlen(sublcd_file_name);
#endif
    const wchar *layer_file_name[UILAYER_TOTAL_BLT_COUNT] = {L"layer0.bmp", L"layer1.bmp", L"layer2.bmp", L"layer3.bmp",
        L"layer4.bmp", L"layer5.bmp", L"layer6.bmp", L"layer7.bmp"};
    const uint32 layer_file_name_len = MMIAPICOM_Wstrlen(layer_file_name[0]);
    const wchar layerinfo_file_name[] = L"layerinfo.txt";
    const uint32 layerinfo_file_name_len = MMIAPICOM_Wstrlen(layerinfo_file_name);    
    UILAYER_PRINTSCREEN_INFO_T print_info = {0};
    wchar prefix_filename[MMIPTSCR_FILENAME_MAX_LEN] = {0};
    uint16 prefix_filename_len = 0;
    wchar filename[MMIPTSCR_FILENAME_MAX_LEN] = {0};
    uint32 filename_len = 0;    
    BOOLEAN ret = FALSE;
    
    ret = UILAYER_GetBltInfoForPrintScreen(&print_info);
    
    if(!ret)
    {
        //SCI_TRACE_LOW:"MMIAPIPTSCR_Print -- :GetBltInfoForPrintScreen result is failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_MAIN_162_112_2_18_2_47_34_2,(uint8*)"");
        return FALSE;
    }
    
    file_dev_ptr = MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD);	/*lint !e605 */
    file_dev_len = MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD);
    
    //创建printscreen文件夹
    SCI_MEMSET(full_path_name, 0x00, (MMIPTSCR_FILENAME_MAX_LEN)*sizeof(wchar));
    full_path_len = MMIPTSCR_FILENAME_MAX_LEN;
    if (MMIAPIFMM_CombineFullPath( file_dev_ptr, file_dev_len,
        print_dir, print_dir_len, 
        NULL, 0, 
        full_path_name, &full_path_len))
    {
        if(!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
        {
            ret = MMIAPIFMM_CreateDir(full_path_name, full_path_len);
            
            if(!ret)
            {
                //SCI_TRACE_LOW:"MMIAPIPTSCR_Print -- :MMIAPIFMM_CreateDir result is failed!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_MAIN_183_112_2_18_2_47_34_3,(uint8*)"");
                return FALSE;
            }
        }
    }
    //生成文件名前缀
    ret = GenPrexfixName(prefix_filename, ARR_SIZE(prefix_filename), &prefix_filename_len);
    
    if(!ret)
    {
        //SCI_TRACE_LOW:"MMIAPIPTSCR_Print -- :GenPrexfixName result is failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_MAIN_193_112_2_18_2_47_34_4,(uint8*)"");
        return FALSE;
    }
    
    //输出lcd buffer数据
    SCI_MEMSET(filename, 0x00, (MMIPTSCR_FILENAME_MAX_LEN)*sizeof(wchar));
    MMIAPICOM_Wstrncat(filename, prefix_filename, prefix_filename_len);
    MMIAPICOM_Wstrncat(filename, lcd_file_name, lcd_file_name_len);
    filename_len = MMIAPICOM_Wstrlen(filename);
    SCI_MEMSET(full_path_name, 0x00, (MMIPTSCR_FILENAME_MAX_LEN)*sizeof(wchar));
    full_path_len = MMIPTSCR_FILENAME_MAX_LEN;
    if (MMIAPIFMM_CombineFullPath( file_dev_ptr, file_dev_len,
        print_dir, print_dir_len, 
        filename, filename_len, 
        full_path_name, &full_path_len)) 
    {
//        if (PNULL != print_info.lcd_layer_ptr)
        {
            ret = MMIAPIPTSCR_WriteBMPFile(full_path_name, print_info.lcd_layer_blend.layer_info.width, print_info.lcd_layer_blend.layer_info.height, (uint8*)print_info.lcd_layer_blend.layer_info.buf_addr, 16);
            
            if(!ret)
            {
                //SCI_TRACE_LOW:"MMIAPIPTSCR_Print -- :MMIAPIPTSCR_WriteBMPFile result is failed!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_MAIN_215_112_2_18_2_47_34_5,(uint8*)"");
                return FALSE; 
            }
            
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIPTSCR_Print -- :MMIAPIFMM_CombineFullPath result is failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_MAIN_223_112_2_18_2_47_34_6,(uint8*)"");
    }

#ifdef SUBLCD_SIZE
    //输出lcd buffer数据
    SCI_MEMSET(filename, 0x00, (MMIPTSCR_FILENAME_MAX_LEN)*sizeof(wchar));
    MMIAPICOM_Wstrncat(filename, prefix_filename, prefix_filename_len);
    MMIAPICOM_Wstrncat(filename, sublcd_file_name, sublcd_file_name_len);
    filename_len = MMIAPICOM_Wstrlen(filename);
    SCI_MEMSET(full_path_name, 0x00, (MMIPTSCR_FILENAME_MAX_LEN)*sizeof(wchar));
    full_path_len = MMIPTSCR_FILENAME_MAX_LEN;
    if (MMIAPIFMM_CombineFullPath( file_dev_ptr, file_dev_len,
        print_dir, print_dir_len, 
        filename, filename_len, 
        full_path_name, &full_path_len)) 
    {
        //        if (PNULL != print_info.lcd_layer_ptr)
        {
            ret = MMIAPIPTSCR_WriteBMPFile(full_path_name, print_info.sublcd_layer_blend.layer_info.width, print_info.sublcd_layer_blend.layer_info.height, (uint8*)print_info.sublcd_layer_blend.layer_info.buf_addr, 16);
            
            if(!ret)
            {
                //SCI_TRACE_LOW:"MMIAPIPTSCR_Print -- :MMIAPIPTSCR_WriteBMPFile result is failed!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_MAIN_215_112_2_18_2_47_34_5,(uint8*)"");
                return FALSE; 
            }
            
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIPTSCR_Print -- :MMIAPIFMM_CombineFullPath result is failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_MAIN_223_112_2_18_2_47_34_6,(uint8*)"");
    }
#endif

    if (is_includelayer)
    {
        //输出图层信息
        SCI_MEMSET(filename, 0x00, (MMIPTSCR_FILENAME_MAX_LEN)*sizeof(wchar));
        MMIAPICOM_Wstrncat(filename, prefix_filename, prefix_filename_len);
        MMIAPICOM_Wstrncat(filename, layerinfo_file_name, layerinfo_file_name_len);
        filename_len = MMIAPICOM_Wstrlen(filename);
        SCI_MEMSET(full_path_name, 0x00, (MMIPTSCR_FILENAME_MAX_LEN)*sizeof(wchar));
        full_path_len = MMIPTSCR_FILENAME_MAX_LEN;
        if (MMIAPIFMM_CombineFullPath( file_dev_ptr, file_dev_len,
            print_dir, print_dir_len, 
            filename, filename_len, 
            full_path_name, &full_path_len))
        {
            MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
            MMIFILE_HANDLE fp = 0;
            char layer_str[100] = {0};
            uint32 write_size = 0;
            
            fp = MMIAPIFMM_CreateFile(full_path_name, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
            
            if(SFS_INVALID_HANDLE != fp)
            {
                for (i = 0; i < print_info.arr_size; i++)
                {
                    layer_ptr = &print_info.layer_stuct_arr[i];
//                    if (PNULL != layer_ptr)
                    {
                        sprintf(layer_str, "layer%d: width-%d height-%d type-%d\n", i, layer_ptr->layer_info.width, layer_ptr->layer_info.height, layer_ptr->layer_info.layer_data.type);
                        file_ret = MMIAPIFMM_WriteFile(fp, layer_str, strlen(layer_str), &write_size, PNULL);
                        if(SFS_ERROR_NONE != file_ret)
                        {
                            break;
                        }
                    }
                }
                MMIAPIFMM_CloseFile(fp);
            }
        }
        //输出图层数据
        for (i = 0; i < print_info.arr_size; i++)
        {
            SCI_MEMSET(filename, 0x00, (MMIPTSCR_FILENAME_MAX_LEN)*sizeof(wchar));
            MMIAPICOM_Wstrncat(filename, prefix_filename, prefix_filename_len);
            MMIAPICOM_Wstrncat(filename, layer_file_name[i], layer_file_name_len);
            filename_len = MMIAPICOM_Wstrlen(filename);
            SCI_MEMSET(full_path_name, 0x00, (MMIPTSCR_FILENAME_MAX_LEN)*sizeof(wchar));
            full_path_len = MMIPTSCR_FILENAME_MAX_LEN;
            if (MMIAPIFMM_CombineFullPath( file_dev_ptr, file_dev_len,
                print_dir, print_dir_len, 
                filename, filename_len, 
                full_path_name, &full_path_len)) 
            {
                layer_ptr = &print_info.layer_stuct_arr[i];
//                if (PNULL != layer_ptr)
                if (PNULL != layer_ptr->layer_info.buf_addr)
                {
                    ColorDepth = (DATA_TYPE_ARGB888 == layer_ptr->layer_info.layer_data.type) ? 32:16;
                    ret = MMIAPIPTSCR_WriteBMPFile(full_path_name, layer_ptr->layer_info.width, layer_ptr->layer_info.height, (uint8*)layer_ptr->layer_info.buf_addr, ColorDepth);
                    
                    if(!ret)
                    {
                        //SCI_TRACE_LOW:"MMIAPIPTSCR_Print -- :WriteBMPFile result is failed!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_MAIN_288_112_2_18_2_47_34_7,(uint8*)"");
                        return FALSE;
                    }
                }
            }
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
//     Description : 根据当前时间生成文件名前缀
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN GenPrexfixName(
 						    wchar *prefixname_ptr,//out
                            uint16 prefixname_max_len,//in
                            uint16 *prefixname_len_ptr//out
                            )
{
    uint8 u8filename[MMIPTSCR_MAX_PREFIX_FILENAME_LEN] = {0};
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
        wstr_len = MIN(wstr_len, MMIPTSCR_MAX_PREFIX_FILENAME_LEN);
        MMI_STRNTOWSTR(prefixname_ptr, prefixname_max_len, u8filename, ARR_SIZE(u8filename), wstr_len);
        if (PNULL != prefixname_len_ptr)
        {
            *prefixname_len_ptr = MMIAPICOM_Wstrlen(prefixname_ptr);
        }        
        ret = TRUE;
    }
    return ret;
}

/*****************************************************************************/
//     Description : 得到抓屏的运行模式
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC MMIPTSCR_RUNMODE_E MMIAPIPTSCR_GetRunMode(void)
{
    return s_ptscr_runmode;
}

/*****************************************************************************/
//     Description : 设置抓屏的运行模式
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void SetRunMode(
                      MMIPTSCR_RUNMODE_E runmode
                      )
{
    s_ptscr_runmode = runmode;
}

/*****************************************************************************/
//     Description : 改变抓屏的运行模式
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPIPTSCR_ChangeRunMode(
                                MMIPTSCR_RUNMODE_E runmode
                                )
{
    if (runmode != s_ptscr_runmode)
    {
        if ((MMIPTSCR_RUNMODE_OFF != runmode)
            //判断是否有TF卡
            && (MMI_DEVICE_NUM <= MMIAPIFMM_GetFirstValidSD()))
        {
            //SCI_TRACE_LOW:"MMIAPIPTSCR_ChangeRunMode: sd not exist!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_MAIN_369_112_2_18_2_47_34_8,(uint8*)"");
            MMIPUB_OpenAlertWarningWin(TXT_NO_SD_CARD_ALERT);
            return;
        }
        if (MMIPTSCR_RUNMODE_AUTO == s_ptscr_runmode)
        {
            s_ptscr_autorun_timer_count = 0;
            StartOrStopAutoRunTimer(FALSE);
        }
        if (MMIPTSCR_RUNMODE_AUTO == runmode)
        {
            s_ptscr_autorun_timer_count = 0;
            StartOrStopAutoRunTimer(TRUE);
        }
        SetRunMode(runmode);
    }
}

/*****************************************************************************/
// 	Description : 启动或者停止autorun Timer
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void StartOrStopAutoRunTimer(
                                   BOOLEAN is_start
                                   )
{
    if (is_start)
    {
        //启动autorun Timer
        if (0 != s_ptscr_autorun_timer_id)
        {
            MMK_StopTimer(s_ptscr_autorun_timer_id);
            s_ptscr_autorun_timer_id = 0;
        }
        //SCI_TRACE_LOW:"StartOrStopAutoRunTimer, MMK_CreateTimerCallback MMIPTSCR_AUTORUN_TIMEOUT = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_MAIN_403_112_2_18_2_47_34_9,(uint8*)"d",MMIPTSCR_AUTORUN_TIMEOUT);
        s_ptscr_autorun_timer_id = MMK_CreateTimerCallback(MMIPTSCR_AUTORUN_TIMEOUT, HandleAutoRunTimeTimerEvent, NULL, FALSE);
    }
    else
    {
        //停止autorun Timer
        if (0 != s_ptscr_autorun_timer_id)
        {
            MMK_StopTimer(s_ptscr_autorun_timer_id);
            s_ptscr_autorun_timer_id = 0;
        }
    }
}

/*****************************************************************************/
// 	Description : 处理autorun的Timer事件
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void HandleAutoRunTimeTimerEvent(uint8 timerid, uint32 param)
{
    BOOLEAN                  ret             = FALSE;
    MMIPTSCR_ERROR_STATUS_E sdcard_status = MMIPTSCR_ERROR_NONE;
    
    if (timerid == s_ptscr_autorun_timer_id)
    {
        sdcard_status = MMIAPIPTSCR_PreCheck();
        
        /*,@CRMS00239652 renwei modify begin*/ 
        switch(sdcard_status)
        {
        case MMIPTSCR_SDCARD_NO_EXIST:
            {
                MMIAPIPTSCR_ChangeRunMode(MMIPTSCR_RUNMODE_OFF);
                MMIPUB_OpenAlertWarningWin(TXT_SD_NO_EXIST);
                return;
            }
        case MMIPTSCR_SDCARD_NO_SPACE:
            {
                MMIAPIPTSCR_ChangeRunMode(MMIPTSCR_RUNMODE_OFF);
                MMIPUB_OpenAlertWarningWin(TXT_DC_NOEMPTYSPACE);
                return;
            }
        default:
            break;
        }
        
        ret = MMIAPIPTSCR_Print(FALSE);
        
        if(ret)/*save success */
        {
            if(s_ptscr_autorun_timer_count < MMIPTSCR_AUTORUN_TIMER_MAX_COUNT)
            {
                s_ptscr_autorun_timer_count++;
                StartOrStopAutoRunTimer(TRUE);
            }
            else if(s_ptscr_autorun_timer_count == MMIPTSCR_AUTORUN_TIMER_MAX_COUNT)
            {
                MMIAPIPTSCR_ChangeRunMode(MMIPTSCR_RUNMODE_OFF);
                MMIPUB_OpenAlertSuccessWin(TXT_PRINTSCREEN_COMPLETE);
            }
            else
            {
                //SCI_TRACE_LOW:"HandleAutoRunTimeTimerEvent -- :PTSCR_Print result is failed!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_MAIN_465_112_2_18_2_47_34_10,(uint8*)"");
            }
        }
        else/*save failed*/
        {
            MMIAPIPTSCR_ChangeRunMode(MMIPTSCR_RUNMODE_OFF);
            MMIPUB_OpenAlertWarningWin(TXT_MMI_SAVEFAILED);
        }
        /*,@CRMS00239652 renwei modify end*/
    }
}

#endif  //PRINTSCREEN_SUPPORT
