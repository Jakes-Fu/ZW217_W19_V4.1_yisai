

/*****************************************************************************
** File Name:      mmi_livewallpaper_slideshow.c                           *
** Author:         cheney.wu                                                 *
** Date:           01/12/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011       Cheney Wu          Modify                                  *
******************************************************************************/
#ifndef _MMI_LIVEWALLPAPER_SLIDESHOW_C_
#define _MMI_LIVEWALLPAPER_SLIDESHOW_C_

#ifdef SLIDESHOW_WALLPAPER_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_livewallpaper_slideshow.h"
#include "mmi_livewallpaper_image.h"
#include "mmi_livewallpaper_text.h"

#include "mmiset_id.h"
#include "mmiset_menutable.h"
#include "mmiset_text.h"
#include "mmifilearray_export.h"
//#include "mmipicview_export.h"
#include "mmipicview_text.h"
#include "mmifmm_interface.h"
#include "mmipub.h"
#include "block_mem.h"
#include "img_dec_interface.h"
#include "mmiudisk_export.h"
#include "mmitheme_special.h"

#include "mmi_livewallpaper.h"
#include "mmi_nv.h" 
/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION    							  *
 **---------------------------------------------------------------------------*/

#define SCR_SIZE    ((MMI_MAINSCREEN_HEIGHT*MMI_MAINSCREEN_WIDTH)*sizeof(uint16))
#define SLIDESHOW_MAX_FAIL_NUM  500
//#define MMI_LIVEWALLPAPER_SLIDESHOW_SLIDE_TIME_NUM 6

#define BLOCK_MEM_POOL_SIZE_SLIDESHOW (0)

//slide play time
typedef enum
{
    SLIDESHOW_TIME_4S = 0,     //2s
    SLIDESHOW_TIME_6S,     //3s
    SLIDESHOW_TIME_8S,     //6s
    SLIDESHOW_TIME_10S,    //10s
    SLIDESHOW_TIME_16S,    //16s
    SLIDESHOW_TIME_20S,    //20s
    SLIDESHOW_TIME_TYPE_MAX
} SLIDESHOW_TIME_TYPE_E;


typedef struct 
{
    uint16 *target_buf_ptr;
    uint16 *source_buf_ptr;
    uint32 frame_num;   //刷新的帧数
    uint32 delay_time;  //每帧刷新延时时间
    uint16 *old_buf_ptr;
    uint32 dummy2;
}SLIDESHOW_COMMON_PARAM_T;


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL FILEARRAY s_file_array = PNULL;
LOCAL uint8                         s_slideshow_play_timer_id = 0;       //slide play timer
LOCAL uint16                        s_slideshow_file_total = 0;
LOCAL uint16		                s_slideshow_file_index = 0;
LOCAL MMI_CTRL_ID_T  s_ctrl_id = 0;
LOCAL uint16         s_slideshow_decode_fail_num = 0;
LOCAL uint8          s_slideshow_decode_fail_arr[SLIDESHOW_MAX_FAIL_NUM] = {0};

        
LOCAL GUI_LCD_DEV_INFO              s_slideshow_layer_handle = {0};

LOCAL WALLPAPER_SLIDESHOW_NV_T      s_slideshow_setting_info = {0};
// 视频墙纸文件路径
// 视频墙纸文件名
LOCAL const uint32    s_slideshow_slide_time[SLIDESHOW_TIME_TYPE_MAX] = {4000,6000,8000,10000,16000,20000}; //slide time
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : sideshow wallpaper initialize for the default value and the image list
//  Global resource dependence : 
//  Author:Cheney.Wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlideShowInit(void);
/*****************************************************************************/
//  Description : Load Photos from Photos Diretory
//  Global resource dependence : 
//  Author:xiaohua.liuu
//  Note:
/*****************************************************************************/
LOCAL void LoadPhotoFileFromDir();
/*****************************************************************************/
//  Description : instance construct for the slideshow layer
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlideShowConstruct(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : Neglect Msg In condition
//  Global resource dependence : 
//  Author:xiaohua.liuu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlideShowNeglectMsg(MMI_MESSAGE_ID_E msg_id/*, MMI_WIN_ID_T win_id, DPARAM param*/);
    
/*****************************************************************************/
// 	Description : Create the animation control for the slideshow wallpaper effect
//	Global resource dependence : 
//  Author:Cheney.Wu
//	Note:
/*****************************************************************************/
LOCAL void CreateSlideshowAnimCtrl(MMI_HANDLE_T win_id, MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : Prepare the background image buffer
//  Global resource dependence : 
//  Author:Cheney.Wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlideShowDrawBg(
                           MMI_WIN_ID_T win_id,
                           const GUI_LCD_DEV_INFO *dev_info_ptr
                           );

/*****************************************************************************/
//  Description : Slideshow image calculate function
//  Global resource dependence : 
//  Author:Cheney.Wu
//  Note:
/*****************************************************************************/
LOCAL void SlideShowCalculate(LW_CAL_PARAM *lw_cal_param_ptr);
/*****************************************************************************/
//  Description : get slide time index
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL uint32 GetSlideShowTime(void);

/*****************************************************************************/
//  Description : slideshow destruct
//  Global resource dependence : 
//  Author:Cheney.Wu
//  Note:
/*****************************************************************************/
LOCAL void SlideShowDestruct(void);
/*****************************************************************************/
//  Description : whether the file index have add to the s_slideshow_decode_fail_arr
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DecodeFailFileIndexWhetherInArray(uint32 file_index);
/*****************************************************************************/
//  Description : add the file index to the s_slideshow_decode_fail_arr
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void AddFailFileIndexToArray(uint32 file_index);

/*****************************************************************************/
//  Description : Slideshow event handler
//  Global resource dependence : 
//  Author:Cheney.Wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlideShowHandleEvent(
                                   MMI_WIN_ID_T win_id, 
                                   MMI_MESSAGE_ID_E msg_id, 
                                   DPARAM param
                                   );

/*****************************************************************************/
//  Description : get slide time index
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideTimeIndex(void);

/*****************************************************************************/
//  Description : set slide time index
//  Global resource dependence : none
//  Author: Cheney.Wu
//  Note:
/*****************************************************************************/
LOCAL void SetSlideTimeIndex(uint16 cur_index);

/*****************************************************************************/
// 	Description : set slide time
//	Global resource dependence : none
//  Author: Cheney.Wu
//	Note:
/*****************************************************************************/
LOCAL void SetSlideTimeNV(
                          SLIDESHOW_TIME_TYPE_E  slide_time
                          );
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void SlideShowGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);

/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 SlideShowGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *GetSlideShowOptionList(void);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


// Slideshow time period string table 

LOCAL MMI_TEXT_ID_T s_slideshow_time_text[SLIDESHOW_TIME_TYPE_MAX] = {
    TXT_PICVIEW_SLIDE_TIME_4S,
    TXT_PICVIEW_SLIDE_TIME_6S,
    TXT_PICVIEW_SLIDE_TIME_8S,
    TXT_PICVIEW_SLIDE_TIME_10S,
    TXT_PICVIEW_SLIDE_TIME_16S,
    TXT_PICVIEW_SLIDE_TIME_20S
};

LOCAL SelectList s_slideshow_option_time_sl  = {SLIDESHOW_TIME_TYPE_MAX, s_slideshow_time_text, GetSlideTimeIndex,SetSlideTimeIndex};

// Slideshow option main menu list 
LOCAL ListItem s_slideshow_option_items[]=
{
	{TXT_SET_SLIDESHOW_TIME_INTERVAL,      TYPE_SELECT_LIST, &s_slideshow_option_time_sl}
};

// Slideshow option menu list from live wallpaper setting screen */
LOCAL OPTIONLIST s_slideshow_option_list={TXT_SET_WALLPAPER_SLIDESHOW, 1, s_slideshow_option_items};

// LIVE wallpaper slideshow instance structure 
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_slideshow_Instance =
{
    SlideShowInit,
    SlideShowConstruct,
    SlideShowDrawBg,
    SlideShowCalculate,
    SlideShowDestruct,
    SlideShowHandleEvent,
    PNULL,
    SlideShowGetType,
    SlideShowGetThumbNailInfo,
    PNULL,
    GetSlideShowTime,
    SlideShowNeglectMsg,
    GetSlideShowOptionList,
    
};

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : reset setting info
//  Global resource dependence : 
//  Author: Cheney.Wu
//  Note: 
/*****************************************************************************/
LOCAL void ResetSettingInfo(void)
{
    SCI_MEMSET(&s_slideshow_setting_info,0,sizeof(s_slideshow_setting_info));
  //  s_slideshow_setting_info.slide_period  = SLIDESHOW_TIME_4S;
    MMINV_WRITE(MMINV_LIVE_WALLPAPER_SLIDESHOW, &s_slideshow_setting_info);
}
/*****************************************************************************/
// 	Description : set slide time
//	Global resource dependence : none
//  Author: Cheney.Wu
//	Note:
/*****************************************************************************/
LOCAL void SetSlideShowLastPlayNV(
                          uint16  last_play_index
                          )
{
    

    if (s_slideshow_setting_info.last_play_index != last_play_index)
    {
        s_slideshow_setting_info.last_play_index = last_play_index;

        //write slide effect
        MMINV_WRITE(MMINV_LIVE_WALLPAPER_SLIDESHOW, &s_slideshow_setting_info);
    }
}
/*****************************************************************************/
//  Description : sideshow wallpaper initialize for the default value and the image list
//  Global resource dependence : 
//  Author:Cheney.Wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlideShowInit(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_SUCCESS;
    
    
    SCI_MEMSET(s_slideshow_decode_fail_arr,0,sizeof(s_slideshow_decode_fail_arr));
    s_slideshow_file_index = 0;

    MMINV_READ(MMINV_LIVE_WALLPAPER_SLIDESHOW, &s_slideshow_setting_info, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {           
        ResetSettingInfo();
    }
     
    return TRUE;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSlideShowSearchPhotoWaitingWin(
                         MMI_WIN_ID_T       win_id,
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    MMIFMM_FILTER_T img_filter = {{"*.bmp *.png *.jpg *.jpeg *.wbmp"}};        
    uint16 array_size = 0;
    uint16 find_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar  find_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    { 
        case MSG_OPEN_WINDOW:
            MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
            if(PNULL == s_file_array)
            {
                s_file_array = MMIAPIFILEARRAY_Create();
            }
            if(PNULL == s_file_array)
            {
                MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
                result = MMI_RESULT_FALSE;
                return result;
            }
            if(MMIAPIFMM_CombineFullPath(
                (wchar*)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), 
                MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD),
                (wchar*)MMIMULTIM_DIR_PICTURE,MMIMULTIM_DIR_PIC_LEN,
                PNULL,0, find_path,&find_path_len))
            {
                MMIAPIFMM_SearchFileInPath(find_path,find_path_len,
                    &img_filter,FALSE,FUNC_FIND_FILE,
                s_file_array,
                MMILIVE_SLIDESHOW_FIND_PHOTO_WIN_ID,
                MSG_SET_SLIDESHOW_FIND_PHOTO
                );
            }
            break;
        case MSG_SET_SLIDESHOW_FIND_PHOTO:
            s_slideshow_file_total = MMIVIRTUALARRAY_GetArraySize(s_file_array);
    
        if(s_slideshow_file_total<1)
        {
            MMISET_WALLPAPER_SET_T wallpaper_info = {0};
            MMIAPISET_SetWallpaperSetInfo(wallpaper_info);
            MMIPUB_OpenAlertWarningWin(TXT_SET_SLIDESHOW_NO_FILE);
        }
        else
        {
            if(s_slideshow_setting_info.last_play_index<s_slideshow_file_total)
            {
                s_slideshow_file_index = s_slideshow_setting_info.last_play_index;
            }
        }
        MMK_CloseWin(win_id);
        
            break;
        case MSG_APP_CANCEL:
        MMIAPIFMM_SearchFileStop();
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_RED:
        MMIAPIFMM_SearchFileStop();
        MMK_CloseWin(win_id);
       
        break;

    case MSG_CLOSE_WINDOW:
		MMIAPIFMM_SearchFileStop();
               
        result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
        
        default:
            result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
            break;
    }

    return result;
}
/*****************************************************************************/
//  Description : Load Photos from Photos Diretory
//  Global resource dependence : 
//  Author:xiaohua.liuu
//  Note:
/*****************************************************************************/
LOCAL void LoadPhotoFileFromDir()
{
/*
    MMI_WIN_ID_T    waiting_win_id = MMISET_SLIDESHOW_FIND_PHOTO_WIN_ID;
    uint32          waiting_time_out = 0;

    MMIPUB_OpenWaitWin(3, TXT_COMMON_WAITING, TXT_NULL,TXT_NULL,
        waiting_win_id,IMAGE_PUBWIN_WAIT,0,PNULL,
        MMIPUB_SOFTKEY_NONE,HandleSlideShowSearchPhotoWaitingWin);
        */
  //  MMI_STRING_T str_t = {0};
    MMIPUB_WAITING_PARAM_T param = {0};
    param.win_id= MMILIVE_SLIDESHOW_FIND_PHOTO_WIN_ID;
 //   MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &str_t);
    param.wait_id = TXT_COMMON_WAITING;
    param.user_func = HandleSlideShowSearchPhotoWaitingWin;
    MMIPUB_OpenWaitingWin(&param);
}
/*****************************************************************************/
//  Description : Neglect Msg In condition
//  Global resource dependence : 
//  Author:xiaohua.liuu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlideShowNeglectMsg(MMI_MESSAGE_ID_E msg_id/*, MMI_WIN_ID_T win_id, DPARAM param*/)
{
    BOOLEAN result = FALSE;
    
    if(MSG_OPEN_WINDOW == msg_id||MSG_GET_FOCUS == msg_id)
    {
        if(!MMIDEFAULT_IsBacklightOn())
        {
            result = TRUE;
        }
    }
    else if(MSG_LOSE_FOCUS == msg_id)
    {
        if(MMILIVE_SLIDESHOW_FIND_PHOTO_WIN_ID == MMK_GetFocusWinId())
        {
            result = TRUE;
        }
    }
    else if(MSG_PRE_FULL_PAINT == msg_id)
    {
        result = TRUE;
    }
    return result;
}
/*****************************************************************************/
//  Description : instance construct for the slideshow layer
//  Global resource dependence : 
//  Author:Cheney.Wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlideShowConstruct(MMI_WIN_ID_T win_id)
{
    MMIFMM_FILTER_T img_filter = {{"*.bmp *.png *.jpg *.jpeg *.wbmp"}};        
    GUI_LCD_DEV_INFO*           source_dev_ptr = PNULL;
    uint16 find_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar  find_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    BOOLEAN result = FALSE;

    
    source_dev_ptr = MMILIVEWALLPAPER_GetSourceDevPtr();
    if(PNULL != source_dev_ptr)
    {
        s_slideshow_layer_handle.block_id = source_dev_ptr->block_id;
    }
    UILAYER_SetLayerColorKey(&s_slideshow_layer_handle, TRUE, MMI_BLACK_COLOR);

	/* clear the slideshow background image layer */
	UILAYER_Clear(&s_slideshow_layer_handle);
	
	if(MMIKL_CLOCK_DISP_WIN_ID == win_id)
    {
        s_ctrl_id = MMIKL_MAIN_ANIM_BG_ID;
	}
    else
    {
        s_ctrl_id = MMILIVEWALLPAPER_ANIM_CTRL_ID;
    }
    // Create the slideshow animation control
    CreateSlideshowAnimCtrl(win_id, s_ctrl_id);//MMI_LIVEWALLPAPER_SLIDESHOW_ANIM_CTRL_ID);

    MMK_SetCtrlLcdDevInfo(s_ctrl_id,&s_slideshow_layer_handle);
    LoadPhotoFileFromDir();  
    /*
  
   if(PNULL == s_file_array)
    {
        s_file_array = MMIAPIFILEARRAY_Create();
    }
    MMIAPIFMM_CombineFullPath(
                (wchar*)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), 
                MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD),
                (wchar*)MMIMULTIM_DIR_PICTURE,MMIMULTIM_DIR_PIC_LEN,
                PNULL,0, find_path,&find_path_len);

    MMIAPIFMM_SearchFileInPath(find_path,find_path_len,&img_filter,TRUE,
        FUNC_FIND_FILE,s_file_array,0,0);
        s_slideshow_file_total = MMIVIRTUALARRAY_GetArraySize(s_file_array);
    SCI_TRACE_LOW("SlideShowConstruct s_slideshow_file_total is %d",s_slideshow_file_total);
    if(s_slideshow_file_total>0) */
    {
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
// 	Description : Create the animation control for the slideshow wallpaper effect
//	Global resource dependence : 
//  Author:Cheney.Wu
//	Note:
/*****************************************************************************/
LOCAL void CreateSlideshowAnimCtrl(MMI_HANDLE_T win_id, MMI_CTRL_ID_T ctrl_id)
{
    GUIANIM_INIT_DATA_T     init_data       = {0};
    MMI_CONTROL_CREATE_T    create          = {0};
    GUI_BOTH_RECT_T         anim_ctrl_rect  = MMITHEME_GetFullScreenBothRect();
    
    /* Not decode the image if UDISK is running */
    if(MMIAPIUDISK_UdiskIsRun())
    {
        SCI_TRACE_LOW("[MMISETVWP]:CreateSlideshowAnimCtrl() exit. UdiskIsRun!!");
        return;
    }


    create.guid = SPRD_GUI_ANIM_ID;

    init_data.both_rect = anim_ctrl_rect;
    create.init_data_ptr     = &init_data;
    create.parent_win_handle = win_id;
    create.parent_ctrl_handle = 0;
    create.ctrl_id = ctrl_id;

    MMK_CreateControl( &create );

  //  GUIANIM_SetDefaultIcon(ctrl_id, img_id, img_id);
}


/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 SlideShowGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_SLIDESHOW;
}


/*****************************************************************************/
//  Description : Prepare the background image buffer
//  Global resource dependence : 
//  Author:Cheney.Wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlideShowDrawBg(
                           MMI_WIN_ID_T win_id,
                           const GUI_LCD_DEV_INFO *dev_info_ptr
                           )
{
        BOOLEAN                     effect_finish = FALSE;
    uint8                      *slideshow_layer_ptr = NULL;
    GUIANIM_RESULT_E            anim_result  = GUIANIM_RESULT_SUCC;
    GUIANIM_CTRL_INFO_T         control_info = {0};
    GUIANIM_FILE_INFO_T         anim_info    = {0};
    GUIANIM_DISPLAY_INFO_T      display_info = {0};
    BOOLEAN                     is_file_exist = FALSE;
    FILEARRAY_DATA_T            file_info = {0};
    MMI_CTRL_ID_T    ctrl_id = s_ctrl_id;
    GUI_LCD_DEV_INFO main_lcd = {0};
    GUI_RECT_T       main_rect = MMITHEME_GetFullScreenRect();
    
    
    MMIAPIFILEARRAY_Read(s_file_array,s_slideshow_file_index, &file_info);
    // Set the image file index to next one
    s_slideshow_file_index++;

    if(s_slideshow_file_total <= s_slideshow_file_index)
    {
        s_slideshow_file_index = 0;
    }
    SCI_TRACE_LOW("SlideShowDrawBg() s_slideshow_file_index %d",s_slideshow_file_index);
    SCI_TRACE_LOW("SlideShowDrawBg() file_info.filename0 %d",file_info.filename[0]);
    SCI_TRACE_LOW("SlideShowDrawBg() file_info.filename1 %d",file_info.filename[1]);
    SCI_TRACE_LOW("SlideShowDrawBg() file_info.filename2 %d",file_info.filename[2]);
    SCI_TRACE_LOW("SlideShowDrawBg() file_info.filename3 %d",file_info.filename[3]);
    SCI_TRACE_LOW("SlideShowDrawBg() file_info.filename4 %d",file_info.filename[4]);
    SCI_TRACE_LOW("SlideShowDrawBg() file_info.filename5 %d",file_info.filename[5]);
    SCI_TRACE_LOW("SlideShowDrawBg() file_info.filename6 %d",file_info.filename[6]);
    SCI_TRACE_LOW("SlideShowDrawBg() file_info.name_len %d",file_info.name_len);
    
    if(file_info.name_len>0)
    {
        is_file_exist = MMIFILE_IsFileExist(file_info.filename,file_info.name_len);
        
    }

    /* Not decode the image if UDISK is running */
    if(!is_file_exist)// || MMIAPIUDISK_UdiskIsRun() )
    {
        SCI_TRACE_LOW("SlideShowCalculate() is_file_exist is FALSE!");

    }

    if(is_file_exist)
    {
        //set anim min period
     //   GUIANIM_SetPeriod(MMITHEME_ANIM_MIN_PERIOD, ctrl_id);
        
        // Prepare to decode the image data
        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id = ctrl_id;
        
        anim_info.full_path_wstr_ptr = file_info.filename;
        anim_info.full_path_wstr_len = file_info.name_len;
        
        display_info.is_syn_decode  = TRUE;
        display_info.align_style    = GUIANIM_ALIGN_HVMIDDLE;
        display_info.bg.bg_type     = GUI_BG_COLOR;
        display_info.bg.color       = MMI_BLACK_COLOR;
        display_info.specify_width  = MMI_MAINSCREEN_WIDTH;
        display_info.specify_height = MMI_MAINSCREEN_HEIGHT;
        display_info.is_update      = TRUE;
        
        anim_result = GUIANIM_SetParam(&control_info,NULL,&anim_info,&display_info);
        SCI_TRACE_LOW("SlideShowCalculate() anim_result is %d",anim_result);
        if (GUIANIM_RESULT_SUCC == anim_result||GUIANIM_RESULT_WAIT == anim_result)
        {
            uint8 *main_buf_ptr = PNULL;
            uint8 *slideshow_buf_ptr = PNULL;
            
            main_buf_ptr = UILAYER_GetLayerBufferPtr(dev_info_ptr);
            slideshow_buf_ptr = (uint8*)UILAYER_GetLayerBufferPtr(&s_slideshow_layer_handle);
            if(PNULL!=main_buf_ptr&&PNULL!=slideshow_buf_ptr)
            {
                SCI_MEMCPY((void*)main_buf_ptr,slideshow_buf_ptr, SCR_SIZE);
            }
        }
    }
    return TRUE;
}


/*****************************************************************************/
//  Description : Slideshow image calculate function
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void SlideShowCalculate(LW_CAL_PARAM *lw_cal_param_ptr)
{
    BOOLEAN                     effect_finish = FALSE;
    uint8                      *slideshow_layer_ptr = NULL;
    GUIANIM_RESULT_E            anim_result  = GUIANIM_RESULT_SUCC;
    GUIANIM_CTRL_INFO_T         control_info = {0};
    GUIANIM_FILE_INFO_T         anim_info    = {0};
    GUIANIM_DISPLAY_INFO_T      display_info = {0};
    BOOLEAN                     is_file_exist = FALSE;
    FILEARRAY_DATA_T            file_info = {0};
    MMI_CTRL_ID_T    ctrl_id = s_ctrl_id;
    GUI_LCD_DEV_INFO main_lcd = {0};
    GUI_RECT_T       main_rect = MMITHEME_GetFullScreenRect();

    slideshow_layer_ptr = (uint8*)UILAYER_GetLayerBufferPtr(&s_slideshow_layer_handle);
    if(PNULL == lw_cal_param_ptr||PNULL == slideshow_layer_ptr)
    {
        return;
    }
    if(lw_cal_param_ptr->reserve1>0)
    {
        return;
    }
    
    if(DecodeFailFileIndexWhetherInArray(s_slideshow_file_index))
    {
        s_slideshow_file_index++;
        if(s_slideshow_file_total <= s_slideshow_file_index)
        {
            s_slideshow_file_index = 0;
        }
    }
    MMIAPIFILEARRAY_Read(s_file_array,s_slideshow_file_index, &file_info);
    // Set the image file index to next one
    s_slideshow_file_index++;

    if(s_slideshow_file_total <= s_slideshow_file_index)
    {
        s_slideshow_file_index = 0;
    }
    SCI_TRACE_LOW("SlideShowCalculate() s_slideshow_file_index %d",s_slideshow_file_index);
    SCI_TRACE_LOW("SlideShowCalculate() file_info.filename0 %d",file_info.filename[0]);
    SCI_TRACE_LOW("SlideShowCalculate() file_info.filename1 %d",file_info.filename[1]);
    SCI_TRACE_LOW("SlideShowCalculate() file_info.filename2 %d",file_info.filename[2]);
    SCI_TRACE_LOW("SlideShowCalculate() file_info.filename3 %d",file_info.filename[3]);
    SCI_TRACE_LOW("SlideShowCalculate() file_info.filename4 %d",file_info.filename[4]);
    SCI_TRACE_LOW("SlideShowCalculate() file_info.filename5 %d",file_info.filename[5]);
    SCI_TRACE_LOW("SlideShowCalculate() file_info.filename6 %d",file_info.filename[6]);
    SCI_TRACE_LOW("SlideShowCalculate() file_info.name_len %d",file_info.name_len);
    
    if(file_info.name_len>0)
    {
        is_file_exist = MMIFILE_IsFileExist(file_info.filename,file_info.name_len);
        
    }

    /* Not decode the image if UDISK is running */
    if(!is_file_exist)
    {
        SCI_TRACE_LOW("SlideShowCalculate() is_file_exist is FALSE!");

        return;
    }
    else
    {
        //set anim min period
     //   GUIANIM_SetPeriod(MMITHEME_ANIM_MIN_PERIOD, ctrl_id);
        
        // Prepare to decode the image data
        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id = ctrl_id;
        
        anim_info.full_path_wstr_ptr = file_info.filename;
        anim_info.full_path_wstr_len = file_info.name_len;
        
        display_info.is_syn_decode  = TRUE;
        display_info.align_style    = GUIANIM_ALIGN_HVMIDDLE;
        display_info.bg.bg_type     = GUI_BG_COLOR;
        display_info.bg.color       = MMI_BLACK_COLOR;
        display_info.specify_width  = MMI_MAINSCREEN_WIDTH;
        display_info.specify_height = MMI_MAINSCREEN_HEIGHT;
        display_info.is_update      = TRUE;
        
        anim_result = GUIANIM_SetParam(&control_info,NULL,&anim_info,&display_info);
        SCI_TRACE_LOW("SlideShowCalculate() anim_result is %d",anim_result);
    
        if (GUIANIM_RESULT_SUCC == anim_result||GUIANIM_RESULT_WAIT == anim_result)
        {
            SCI_MEMCPY((void*)lw_cal_param_ptr->dest_buf_info.buf_ptr,
                (void*)lw_cal_param_ptr->src_buf_info.buf_ptr, SCR_SIZE);
        }
        #ifdef UI_P3D_SUPPORT
        MMITHEME_StoreUpdateRect(&main_lcd,main_rect);
        MMILIEVEWALLPAPER_PlaySlideShowEffect(MMK_GetFocusWinId());
        #endif
    }  
    

}
#ifdef UI_P3D_SUPPORT
/*****************************************************************************/
//  Description :play  slideshow effect
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
PUBLIC void MMILIEVEWALLPAPER_PlaySlideShowEffect(MMI_WIN_ID_T win_id)
{
    MMI_WIN_ANIM_PARAM_T win_param = {0};
    MMIDEFAULT_SaveOldMoveBuf(SCR_EFFECT_BUF_TYPE_NONE);
    win_param.lcd_id = MAIN_LCD_ID;
    win_param.type = MMITHEME_Get3DScreenAnimType(win_id,MMI_WIN_DISP_STATE_PAINT);
        //SCR_EFFECT_BUF_TYPE_WIN_SWITCH);
    MMITHEME_PlayWinAnim(&win_param);
    MMI_BL_FREE((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_DESKTOP);
        
       // MMIDEFAULT_FreeSavedMove3DBuf(SCR_EFFECT_BUF_TYPE_NONE);
        //SCR_EFFECT_BUF_TYPE_WIN_SWITCH);
} 
#endif
/*****************************************************************************/
//  Description : slideshow destruct
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void SlideShowDestruct(void)
{	
    SetSlideShowLastPlayNV(s_slideshow_file_index);
    
    MMIAPIFILEARRAY_Destroy(&s_file_array);
  
   s_file_array = PNULL;
   
   s_slideshow_file_index = 0;
   s_slideshow_file_total = 0;
  
    s_slideshow_decode_fail_num = 0;
    SCI_MEMSET(s_slideshow_decode_fail_arr,0,sizeof(s_slideshow_decode_fail_arr));
    
    GUIANIM_PauseAnim(s_ctrl_id);//MMI_LIVEWALLPAPER_SLIDESHOW_ANIM_CTRL_ID);

    MMK_DestroyControl(s_ctrl_id);//MMI_LIVEWALLPAPER_SLIDESHOW_ANIM_CTRL_ID);
    UILAYER_ReleaseLayer(&s_slideshow_layer_handle);
}

/*****************************************************************************/
//  Description : Slideshow event handler
//  Global resource dependence : 
//  Author:Cheney.Wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SlideShowHandleEvent(
                                   MMI_WIN_ID_T win_id, 
                                   MMI_MESSAGE_ID_E msg_id, 
                                   DPARAM param
                                   )
{
    GUI_POINT_T   point       = {0};

    switch(msg_id)
    {
    case MSG_TIMER:
        
        break;

    case MSG_TP_PRESS_UP:
      
        break;
	
	default:
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : get slide time index
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideTimeIndex(void)
{
    return (s_slideshow_setting_info.slide_period);
}

/*****************************************************************************/
//  Description : get slide time index
//  Global resource dependence : none
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL uint32 GetSlideShowTime(void)
{
    uint32                  time_out = 0;
    SLIDESHOW_TIME_TYPE_E   slide_time;

    slide_time = (SLIDESHOW_TIME_TYPE_E)GetSlideTimeIndex();
    time_out = s_slideshow_slide_time[slide_time];

    return (time_out);
}


/*****************************************************************************/
//  Description : set slide time index
//  Global resource dependence : none
//  Author: Cheney.Wu
//  Note:
/*****************************************************************************/
LOCAL void SetSlideTimeIndex(uint16 cur_index)
{
    SLIDESHOW_TIME_TYPE_E  slide_time = SLIDESHOW_TIME_4S;

    if (SLIDESHOW_TIME_TYPE_MAX <= cur_index)
    {
        SCI_TRACE_LOW("[MMI_LIVEWALLPAPER]:SetSlideTimeIndex cur_index is overflow !!!");
        return ;
    }

    slide_time = (SLIDESHOW_TIME_TYPE_E)cur_index;

    //write slide time
    SetSlideTimeNV(slide_time);
}

/*****************************************************************************/
// 	Description : set slide time
//	Global resource dependence : none
//  Author: Cheney.Wu
//	Note:
/*****************************************************************************/
LOCAL void SetSlideTimeNV(
                          SLIDESHOW_TIME_TYPE_E  slide_time
                          )
{
    if ((SLIDESHOW_TIME_4S > slide_time)
       || (SLIDESHOW_TIME_TYPE_MAX <= slide_time)
       )
    {
        SCI_TRACE_LOW("[MMI_LIVEWALLPAPER]:SetSlideTimeNV slide_time=%d, overflow !!!", slide_time);
        return ;
    }

    if (s_slideshow_setting_info.slide_period != slide_time)
    {
        s_slideshow_setting_info.slide_period = slide_time;

        //write slide effect
        MMINV_WRITE(MMINV_LIVE_WALLPAPER_SLIDESHOW, &s_slideshow_setting_info);
    }
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void SlideShowGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{      
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_NULL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_SLIDESHOW;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_SLIDESHOW;
    }
}
/*****************************************************************************/
//  Description : add the file index to the s_slideshow_decode_fail_arr
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void AddFailFileIndexToArray(uint32 file_index)
{
    uint16 i = 0;
    while(i<SLIDESHOW_MAX_FAIL_NUM&&s_slideshow_decode_fail_arr[i]>0)
    {
        i++;
    }
    if(i<SLIDESHOW_MAX_FAIL_NUM)
    {
        s_slideshow_decode_fail_arr[i] = file_index;
       
    }
}

/*****************************************************************************/
//  Description : whether the file index have add to the s_slideshow_decode_fail_arr
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DecodeFailFileIndexWhetherInArray(uint32 file_index)
{
    uint16 i = 0;
          
    while(i<SLIDESHOW_MAX_FAIL_NUM&&s_slideshow_decode_fail_arr[i]>0)
    {
        if(file_index == s_slideshow_decode_fail_arr[i])
        {
            return TRUE;
        }
        else
        {
            i++;
        }
    }
    return FALSE;
}
/*****************************************************************************/
//  Description : when photo decode fail.whether reset wallpaper to default type
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMILiveWallpaper_NeedResetWallpaper(uint16 wallpaper_id)
{
    uint32 file_index = 0;
    BOOLEAN result = FALSE;
    if(MMI_LIVE_WALLPAPER_TYPE_SLIDESHOW == wallpaper_id)
    {
        if(s_slideshow_file_index>0)
        {
            file_index = s_slideshow_file_index-1;
        }
        else
        {
            file_index = s_slideshow_file_total-1;
        }
        if(!DecodeFailFileIndexWhetherInArray(file_index))
        {
            AddFailFileIndexToArray(file_index);
            s_slideshow_decode_fail_num++;
            SCI_TRACE_LOW("MMILiveWallpaper_NeedResetWallpaper %d %d",
                s_slideshow_decode_fail_num,s_slideshow_file_total);
            if(s_slideshow_decode_fail_num>=(s_slideshow_file_total-1))
            {
                result = TRUE;
            }
        }
    }
    else
    {
        result = TRUE;
    }
    SCI_TRACE_LOW("MMILiveWallpaper_NeedResetWallpaper return %d",result);
    return result;
}
/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *GetSlideShowOptionList(void)
{
    return &s_slideshow_option_list;
}


#endif// LIVE_WALLPAPER_SUPPORT_SLIDESHOW

#endif //_MMI_LIVEWALLPAPER_SLIDESHOW_C_
