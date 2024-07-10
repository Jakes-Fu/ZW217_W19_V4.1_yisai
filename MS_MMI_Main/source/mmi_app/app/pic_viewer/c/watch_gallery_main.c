/*****************************************************************************
** File Name:      watch_gallery_main.c                                      *
** Author:         longwei.qiao                                              *
** Date:           02/20/2020                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe gallery function            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2/2020      longwei.qiao          Create                                  *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef PIC_VIEWER_SUPPORT
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_type.h"
#include "guibutton.h"
#include "guictrl_api.h"
#include "mmipicview_id.h"

#include "mmidisplay_data.h"
#include "mmipicview_text.h"
#include "mmipicview_image.h"
#include "mmipub.h"
#include "mmk_tp.h"
#include "mmi_appmsg.h"
#include "mmicom_trace.h"
#include "watch_gallery_internal.h"
#include "watch_commonwin_export.h"
#include "guires.h"
#ifdef VIDEO_PLAYER_SUPPORT       
#include "mmivp_export.h"
#endif

#if defined(BAIDU_DRIVE_SUPPORT)
#include "mmibaidu_export.h"
#endif
#include "mmipicview_internal.h" 
#include "guilistbox.h"
#include "guiiconlist.h"
#include "mmipicview_position.h"
#include "guiiconlist.h"
#include "watch_common.h"
#include "zdt_app.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define GALLERY_SLIDE_VER_MOTION_THRES      5//the thres of vertion motion startup
#define GALLERY_OSD_DISPLAY_TIME            5000//ms
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
#define GALLERY_UP_INDICATOR_RECT         DP2PX_RECT(112,4,127,19)
#define GALLERY_DOWN_INDICATOR_RECT       DP2PX_RECT(112,220,127,235)
#define GALLERY_UP_INDICATOR_BG_RECT      DP2PX_RECT(0,0,239,35)
#define GALLERY_DOWN_INDICATOR_BG_RECT    DP2PX_RECT(0,204,239,239)
#define GALLERY_RIGHT_BG_RECT             DP2PX_RECT(204,0,239,239)
#define GALLERY_LEFT_BG_RECT              DP2PX_RECT(0,0,35,239)
#endif
#define   GALLERY_DELWIN_DEL_FOCUS_BORDER   {143-5,163-5,207+5,227+5}
#define   GALLERY_DELWIN_BACK_FOCUS_BORDER  {31-5,163-5,95+5,227+5}
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct
{
  MMIGALLERY_FILE_TYPE_E file_type;//文件类型
  uint16                 file_index;//文件索引
  BOOLEAN                is_dis_osd;//是否显示OSD
  uint8                  dis_osd_timer_id;//控制OSD显示定时器
}GALLERY_CUR_PANEL_INFO_T;
/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/
LOCAL  FILEARRAY                 s_watch_gallery_lib         = PNULL;
LOCAL  GUI_POINT_T               s_gallery_tp_down_point     = {0};//记录tp down point
LOCAL  GALLERY_CUR_PANEL_INFO_T  s_gallery_cur_panel_info    = {0};//当前预览界面的信息
LOCAL  BOOLEAN                   s_gallery_is_fling          = FALSE;//记录是否执行fling相关
LOCAL  wchar                     s_gallery_pic_suffix_str[]  = L"bmp wbmp gif png dm dcf jpg jpeg";
LOCAL  wchar                     s_gallery_video_suffix_str[]= L"avi 3gp mp4 flv rm rv rmvb";
#if defined( ADULT_WATCH_SUPPORT)
LOCAL  BOOLEAN                   s_gallery_stop_play       = FALSE;//是否被动停止播放
#endif

LOCAL  const uint8 s_gallery_all_suffix[MMIGALLERY_PIC_TYPE_MAX+MMIGALLERY_VIDEO_TYPE_MAX][GALLERY_FILE_SUFFIX_MAX_LEN]=
{

#ifdef BMP_DEC_SUPPORT
        "*.bmp ",
        "*.wbmp ",
#endif
#ifdef GIF_DEC_SUPPORT
        "*.gif ",
#endif
#ifdef PNG_DEC_SUPPORT
        "*.png ",
#endif
#ifdef DRM_SUPPORT
        "*.dm ",
        "*.dcf ",
#endif
        "*.jpg ",
        "*.jpeg ",

    //video file type
#ifdef AVI_DEC_SUPPORT
        "*.avi ",
#endif
#ifdef FLV_DEC_SUPPORT
        "*.flv ",
#endif
#ifdef MMI_RMVB_SUPPORT
        "*.rm ",
        "*.rv ",
        "*.rmvb "
#endif
        "*.3gp ",
        "*.mp4",
};

extern uint8 delete_button_click;
uint16 pic_del_remain_num = 0; //删除剩余文件个数
uint16 pic_del_num = 0; //删除文件个数
PUBLIC void WatchGallery_MainWin_Enter(void);
PUBLIC void GalleryPicListUnmarked(MMI_CTRL_ID_T list_ctrl_id);
LOCAL void GalleryPicList_TitleShow(MMI_WIN_ID_T win_id);
LOCAL void CreatePicListCtrl(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T  ctrl_id);
/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create gallery  file lib array
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL void Gallery_CreateLibArray(void)
{
    if(PNULL == s_watch_gallery_lib)
    {
        s_watch_gallery_lib = MMIAPIFILEARRAY_Create();
    }
}

/*****************************************************************************/
//  Description :destory gallery  file lib array
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL void Gallery_DestroyLibArray(void)
{
    MMIAPIFMM_SearchFileStop();
    MMIAPIFILEARRAY_Destroy(&s_watch_gallery_lib);
}

/*****************************************************************************/
//  Description : get gallery files array of total gallery lib
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:s_watch_gallery_lib
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL FILEARRAY Gallery_GetLibArray(void)
{
    return s_watch_gallery_lib;
}

/*****************************************************************************/
//  Description : get all gallery files number
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL uint32 Gallery_GetAllFileNum(void)
{
	uint32  file_num = 0, i = 0 , ret_val=0;
	FILEARRAY_DATA_T        file_info = {0};	
    file_num = MMIAPIFILEARRAY_GetArraySize(s_watch_gallery_lib);
    return (file_num);
}
/*****************************************************************************/
//  Description : get gallery file info by index
//  Paramter:
//          [In]:file_index,file_info_ptr
//          [Out]:file_info_ptr
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Gallery_GetFileInfo(
    uint32             file_index,      //in
    FILEARRAY_DATA_T   *file_info_ptr  //in/out
)
{
    BOOLEAN     result = FALSE;
    if(PNULL == file_info_ptr)
    {
        return FALSE;
    }

    result = MMIAPIFILEARRAY_Read(s_watch_gallery_lib, file_index, file_info_ptr);

    TRACE_APP_GALLERY(" file_info_ptr->size is %d,file_info_ptr->name_len is %d", file_info_ptr->size, file_info_ptr->name_len);
    if (0 == file_info_ptr->name_len)
    {
        result = FALSE;
    }
    return (result);
}

LOCAL void Gallery_GetAllSuffixFile(
    MMIFMM_FILTER_T  *filter_ptr
)
{
    uint32      i = 0;
    uint32      suffix_len = 0;

    if (PNULL == filter_ptr)
    {
        TRACE_APP_GALLERY("GalleryGetAllSuffixFile filter_ptr is PNULL");
        return ;
    }

    //memset
    SCI_MEMSET(filter_ptr, 0, sizeof(MMIFMM_FILTER_T));

    for (i = 0; i < MMIGALLERY_PIC_TYPE_MAX + MMIGALLERY_VIDEO_TYPE_MAX; i++)
    {
        MMI_MEMCPY((filter_ptr->filter_str + suffix_len), (sizeof(filter_ptr->filter_str) - suffix_len),
                   s_gallery_all_suffix[i], sizeof(s_gallery_all_suffix[i]),
                   sizeof(s_gallery_all_suffix[i]));

        suffix_len = suffix_len + strlen((char *)s_gallery_all_suffix[i]); /*lint !e64*/
    }

}

LOCAL MMI_RESULT_E Gallery_StartLoadFile()
{
    MMI_RESULT_E                result       = MMI_RESULT_TRUE;
    MMIFMM_FILTER_T             file_filter  = {0};
    wchar path_name[MMIFMM_PATHNAME_LEN + 1] = {0};

    //get all Gallery file suffix
    Gallery_GetAllSuffixFile(&file_filter);
    // 1389172: 在搜索时指定排序方式，且非递归搜索当前路径下的图片和音频文件
    if (MMISD_IsSDExistEx() && FALSE)
    {
        MMIAPIFMM_CombineFullPathEx(path_name, MMIFMM_PATHNAME_LEN, MMI_DEVICE_SDCARD, MMIMULTIM_DIR_MOVIE, PNULL);
    }
    else
    {
        MMIAPIFMM_CombineFullPathEx(path_name, MMIFMM_PATHNAME_LEN, MMI_DEVICE_UDISK, MMIMULTIM_DIR_MOVIE, PNULL);
    }
    MMIAPIFMM_SearchFileInPathAndSort(path_name,
                            MMIAPICOM_Wstrlen(path_name),
                            &file_filter,
#ifdef BAIDU_DRIVE_SUPPORT
                            TRUE ,
#else
                            FALSE,
#endif
                            FUNC_FIND_FILE,
                            Gallery_GetLibArray(),
                            WATCH_GALLERY_MAIN_WIN_ID,
                            MSG_PICVIEWER_LOAD_FINISH,
                            FILEARRAY_SORT_TIME_DESCEND);
    return (result);
}

PUBLIC void Gallery_Main_ReLoadFile(void)
{
    Gallery_DestroyLibArray();
    Gallery_CreateLibArray();
    Gallery_StartLoadFile();
}

LOCAL MMIGALLERY_FILE_TYPE_E Gallery_GetCurFileType( wchar *file_name_ptr)
{
    uint16                 filter_len = 0;
    uint16                 pic_len    = 0;
    uint16                 video_len  = 0;
    MMIGALLERY_FILE_TYPE_E class_type = MMIGALLERY_FILE_TYPE_ERROR;
    wchar*                 filter     = SCI_ALLOC_APP(GALLERY_FILE_SUFFIX_MAX_LEN * sizeof(wchar));
    if(filter==PNULL)
    {
        TRACE_APP_GALLERY("GalleryGetCurFileType:alloc mem is error");
        return class_type;
    }
    if(file_name_ptr==PNULL)
    {
      TRACE_APP_GALLERY("GalleryGetCurFileType:file_name_ptr==PNULL");
      return class_type;
    }
    MMIAPICOM_SplitSuffix(PNULL, 0, filter, GALLERY_FILE_SUFFIX_MAX_LEN,file_name_ptr);
    filter_len = MMIAPICOM_Wstrlen(filter);
    pic_len    = MMIAPICOM_Wstrlen(s_gallery_pic_suffix_str);
    video_len  = MMIAPICOM_Wstrlen(s_gallery_pic_suffix_str);
    if(MMIAPICOM_WstrnstrExt(s_gallery_pic_suffix_str,pic_len, filter, filter_len, FALSE))
    {
        class_type = MMIGALLERY_FILE_TYPE_PIC;
    }
    else if(MMIAPICOM_WstrnstrExt(s_gallery_video_suffix_str,video_len, filter, filter_len, FALSE))
    {
        class_type = MMIGALLERY_FILE_TYPE_VIDEO;
    }
    else
    {
        class_type = MMIGALLERY_FILE_TYPE_ERROR;
    }
    if(PNULL!=filter)
    {
        SCI_FREE(filter);
    }
    return class_type;
}
LOCAL void Gallery_SaveCurPreviewFileType(MMIGALLERY_FILE_TYPE_E file_type)
{
   s_gallery_cur_panel_info.file_type=file_type;
}

PUBLIC MMIGALLERY_FILE_TYPE_E Gallery_GetCurPreviewFileType(void)
{
   return s_gallery_cur_panel_info.file_type;
}

#if defined(BAIDU_DRIVE_SUPPORT)
LOCAL uint32	Gallery_remove_baidu_thumbnail_file(void)
{
	FILEARRAY   tmp_gallery_lib = MMIAPIFILEARRAY_Create();
	uint32      gallery_file_num = 0 , i=0 ;
	FILEARRAY_DATA_T       file_info    = {0};
	
	gallery_file_num = Gallery_GetAllFileNum();
	
	for(i=0 ; i<gallery_file_num ; i++)
	{
		if (Gallery_GetFileInfo(i,&file_info))
		{
			if(!MMISRV_BAIDUDRV_is_in_BaiduFolder (PICVIEW_SOURCE_CLOUD_THUMBNAIL , file_info.filename , file_info.name_len))
			{
				MMIAPIFILEARRAY_Add(tmp_gallery_lib , &file_info);
			}
		}
	}

	MMIAPIFILEARRAY_Destroy(&s_watch_gallery_lib);
	s_watch_gallery_lib = tmp_gallery_lib;

	return MMIAPIFILEARRAY_GetArraySize(s_watch_gallery_lib);
}
#endif

LOCAL void Gallery_ShowNoPhotosTip(MMI_WIN_ID_T win_id)
{
	GUI_LCD_DEV_INFO lcd_dev_info = {0,0};
	GUI_RECT_T win_rect = {0, 0, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT};
	GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
	GUISTR_STYLE_T text_style = {0};
	MMI_STRING_T text_string = {0};
	GUI_RECT_T img_rect = {80, 60, 180, 160};

	GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);

	GUIRES_DisplayImg(PNULL, &img_rect, PNULL, win_id, res_zte_gallery_none, &lcd_dev_info);
	
	text_style.align = ALIGN_HVMIDDLE;
	text_style.font = SONG_FONT_16;
	text_style.font_color = MMI_WHITE_COLOR;
	img_rect.left = 60;
	img_rect.top = 150;
	img_rect.bottom = 190;
	MMI_GetLabelTextByLang(TXT_GALLERY_NO_PHOTOS, &text_string);
	GUISTR_DrawTextToLCDInRect(
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&img_rect,
		&img_rect,
		&text_string,
		&text_style,
		text_state,
		GUISTR_TEXT_DIR_AUTO
		);
}

PUBLIC MMI_RESULT_E HandleGallerytIPWinMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                               )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
		case MSG_FULL_PAINT:
			{
				Gallery_ShowNoPhotosTip(win_id);
			}
			break;
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return result;
}

WINDOW_TABLE(WATCH_GALLERY_TIP_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleGallerytIPWinMsg),
    WIN_ID(WATCH_GALLERY_NOTE_WIN_ID),
    WIN_HIDE_STATUS,
    END_WIN
};

PUBLIC void WatchGallery_NoPhotosTipWin(void)
{
	MMK_CreateWin((uint32 *)WATCH_GALLERY_TIP_WIN_TAB,PNULL);
}

LOCAL BOOLEAN Gallery_FinishLoadFile(void)
{
    uint32      gallery_file_num = 0;
    BOOLEAN     result = FALSE;
#if defined( ADULT_WATCH_SUPPORT)
    GUI_COLOR_T fontColor   = AUDULT_GRAY2_COLOR;
#elif defined(SCREEN_SHAPE_CIRCULAR)
    GUI_COLOR_T fontColor   = GRAY2_COLOR;
#endif
#if defined(BAIDU_DRIVE_SUPPORT)
    gallery_file_num =Gallery_remove_baidu_thumbnail_file();
#else
    gallery_file_num = Gallery_GetAllFileNum();
#endif
    TRACE_APP_GALLERY("GalleryFinishLoadFile, and gallery_file_num is %d", gallery_file_num);
    if(gallery_file_num == 0)
    {
        //go display no file tip
        //edit by fys 09/26
        /*{ 
           MMI_STRING_T     tip_content      = {0};
           WATCH_SOFTKEY_TEXT_ID_T   softket_test = {TXT_NULL,TXT_NULL,TXT_NULL};
           MMI_GetLabelTextByLang(TXT_GALLERY_NO_FILE, &tip_content);
#ifndef ADULT_WATCH_SUPPORT
           WatchCOM_NoteWin_FullScreen_Enter(WATCH_GALLERY_NOTE_WIN_ID,PNULL,&tip_content,res_gallery_blank,0,softket_test,PNULL);
            WatchCOM_NoteWin_Set_Font(PNULL,&fontColor);

#else
           Adult_WatchCOM_NoteWin_1Icon_1Str_Enter(WATCH_GALLERY_NOTE_WIN_ID,&tip_content,res_gallery_blank,PNULL);
           Adult_WatchCOM_NoteWin_Set_Font(PNULL,&fontColor);

#endif
        }*/
        MMK_CloseWin(WATCH_GALLERY_MAIN_WIN_ID);
        WatchGallery_NoPhotosTipWin();
    }
    else
    {
        // 1389172: no need to sort
#if !defined(MMIFMM_XFIND_SUPPORT) //bug 2092288
       MMIAPIFILEARRAY_Sort(s_watch_gallery_lib,FILEARRAY_SORT_TIME_DESCEND);//将图和视频文件按时间混排
#endif
       result = TRUE;
    }
    MMK_CloseWin(WATCH_GALLERY_WAITING_WIN_ID);
    return result;
}
LOCAL MMI_RESULT_E HandleLoadGalleryFileWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
             TRACE_APP_GALLERY("GalleryStartLoadFile");
             Gallery_StartLoadFile();
        }
        break;
        case MSG_CLOSE_WINDOW:
        {
            MMIAPIFMM_SearchFileStop();
            TRACE_APP_GALLERY("GalleryFinishLoadFile");
        }
        break;
        default:
        {
            result = MMI_RESULT_FALSE;
        }
        break;
    }
    return (result);
}
LOCAL MMI_RESULT_E HandleGalleryFileDelQueryWin(
                                               MMI_WIN_ID_T     win_id,
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               )
{
    MMI_RESULT_E        result         = MMI_RESULT_TRUE;
    FILEARRAY_DATA_T    file_info      = {0};
    switch (msg_id)
    {
        case MSG_NOTIFY_CANCEL:
        {
            uint16      mark_num = 0;
            if(MMIGALLERY_FILE_TYPE_VIDEO==Gallery_GetCurPreviewFileType())
            {
                  Gallery_Vp_StopPlayAndClearDis();//stop play
            }
            //delete gallery file
            TRACE_APP_GALLERY("DeleteSelectedfile start_time = %d", SCI_GetTickCount());
            
            mark_num = GUIICONLIST_GetSelectedItemIndex(MMIPICVIEW_LIST_CTRL_ID,PNULL,0);
            if (0 == mark_num)
            {
            }
            else
            {
                //delete all mark items
                MMIPICVIEW_GetSelectItemIndex(MMIPICVIEW_LIST_CTRL_ID,&s_gallery_cur_panel_info.file_index,1);
            }
            //get file info
            if (Gallery_GetFileInfo(s_gallery_cur_panel_info.file_index,&file_info))
            {
                if( MMIAPIFMM_DeleteFileAsyn(file_info.filename,file_info.name_len,WATCH_GALLERY_MAIN_WIN_ID,MSG_PICVIEWER_DELETE_CNF) )
                {
                    result = TRUE;
                }
            }
             TRACE_APP_GALLERY("DeleteSelectedfile end_time = %d", SCI_GetTickCount());
            if ( !result )
            {
                //prompt fail
                {
                   MMI_STRING_T              tip_content  = {0};
                   WATCH_SOFTKEY_TEXT_ID_T   softket_test = {TXT_NULL,TXT_NULL,TXT_NULL};
                   MMI_GetLabelTextByLang(TXT_ERROR, &tip_content);
                   WatchCOM_NoteWin_1Line_Enter(WATCH_GALLERY_NOTE_WIN_ID,&tip_content,0,softket_test,PNULL);
                }
            }
        }
        break;
        case MSG_NOTIFY_OK:
        {
            MMK_CloseWin(win_id);
        }
        break;
        default:
        {
            result = FALSE;
        }
        break;
    }

    return (result);
}
/*****************************************************************************/
//  Description : open gallery del file query win
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void Gallery_Main_OpenDelQueryWin(void)
{
#ifdef ADULT_WATCH_SUPPORT
    Gallery_Main_DeleteCurPreviewFile();
#else
    {
        MMI_STRING_T        tip_content = {0};
        MMI_WIN_ID_T        win_id      = WATCH_GALLERY_QUERY_WIN_ID;
        WATCH_SOFTKEY_TEXT_ID_T   softket_test = {STXT_RETURN,TXT_NULL,TXT_GALLERY_DEL_BUTTON};
#ifdef SCREEN_SHAPE_CIRCULAR
        MMI_GetLabelTextByLang(TXT_GALLERY_DEL_BUTTON, &tip_content);
#else
        MMI_GetLabelTextByLang(TXT_GALLERY_DELWIN_TITLE, &tip_content);
#endif
        WatchCOM_QueryWin_2Btn_Enter(win_id, 
                                                                &tip_content, 
                                                                PNULL, 
                                                                PNULL, 
                                                                res_gallery_ic_cancel, 
                                                                res_gallery_ic_confirm,
                                                                softket_test,
                                                                HandleGalleryFileDelQueryWin);
    }
#endif
}

LOCAL void Gallery_DisplayFilePreview(MMI_WIN_ID_T win_id, uint32 file_index)
{
    FILEARRAY_DATA_T       file_info    = {0};
    MMIGALLERY_FILE_TYPE_E file_type    = 0;
    if(Gallery_GetFileInfo(file_index,&file_info))
    {
        file_type = Gallery_GetCurFileType(file_info.filename);
        switch(file_type)
        {
            case MMIGALLERY_FILE_TYPE_PIC:
            {
                 Gallery_Pic_PictureFilePreview(win_id,&file_info);
                 s_gallery_cur_panel_info.file_index = file_index;
            }
            break;
            case MMIGALLERY_FILE_TYPE_VIDEO:
            {
                 Gallery_Vp_VideoFilePreview(win_id,&file_info);
            }
            break;
            default:
            {
                 TRACE_APP_GALLERY("gallery cur file type is error");
            }
            break;
        }
        Gallery_SaveCurPreviewFileType(file_type);
        Gallery_Main_SetDisplayOsd(win_id,TRUE);
    }
    TRACE_APP_GALLERY("gallery cur file type is %d,cur file index is %d",file_type,file_index);
}

PUBLIC void Gallery_NextFilePreview(MMI_WIN_ID_T win_id)
{
    uint16 file_nume     = 0;
    uint16 cur_file_index= s_gallery_cur_panel_info.file_index;
    if(MMIGALLERY_FILE_TYPE_VIDEO==Gallery_GetCurPreviewFileType())
    {
        Gallery_Vp_StopPlayAndClearDis();
    }
    file_nume = Gallery_GetAllFileNum();
    if(file_nume==cur_file_index+1)
    {
       cur_file_index=0;//当前是最后一个文件则显示第一个文件,循环显示
       Gallery_DisplayFilePreview(win_id,cur_file_index);
    }
    else
    {
       Gallery_DisplayFilePreview(win_id,cur_file_index+1);
       cur_file_index=cur_file_index+1;
    }
    s_gallery_cur_panel_info.file_index=cur_file_index;
}

LOCAL void Gallery_PreFilePreview(MMI_WIN_ID_T win_id)
{
    uint16 file_nume      = 0;
    uint16 cur_file_index = s_gallery_cur_panel_info.file_index;
    if(MMIGALLERY_FILE_TYPE_VIDEO==Gallery_GetCurPreviewFileType())
    {
        Gallery_Vp_StopPlayAndClearDis();
    }
    file_nume = Gallery_GetAllFileNum();
    if(0==cur_file_index)
    {
        //cur_file_index = file_nume-1;//当前是第一个文件则显示最后一个文件
        Gallery_DisplayFilePreview(win_id,cur_file_index);
    }
    else
    {
        Gallery_DisplayFilePreview(win_id,cur_file_index);
        //cur_file_index = cur_file_index-1;
    }
    s_gallery_cur_panel_info.file_index=cur_file_index;
}

LOCAL void Gallery_StopDisplayTimer(void)
{
    uint8 timer_id=s_gallery_cur_panel_info.dis_osd_timer_id;
    if (0 < timer_id)
    {
        MMK_StopTimer(timer_id);
        timer_id = 0;
    }
    s_gallery_cur_panel_info.dis_osd_timer_id=timer_id;
}

LOCAL void Gallery_StartDisplayTimer(
    MMI_WIN_ID_T   win_id
)
{
    if (0 == s_gallery_cur_panel_info.dis_osd_timer_id)
    {
        s_gallery_cur_panel_info.dis_osd_timer_id = MMK_CreateWinTimer(win_id, GALLERY_OSD_DISPLAY_TIME, FALSE);
    }
}

LOCAL void Gallery_ResumeDisplayTimer( MMI_WIN_ID_T   win_id)
{
    if (0 < s_gallery_cur_panel_info.dis_osd_timer_id)
    {
        MMK_StopTimer(s_gallery_cur_panel_info.dis_osd_timer_id);
        s_gallery_cur_panel_info.dis_osd_timer_id = 0;
    }
    Gallery_StartDisplayTimer(win_id);
}
/*****************************************************************************/
//  Description : Get is dispaly preview osd flag
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:s_gallery_cur_panel_info.is_dis_osd
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  Gallery_Main_IsDisplayOsd(void)
{
    return s_gallery_cur_panel_info.is_dis_osd;
}
/*****************************************************************************/
//  Description : set is dispaly gallery osd
//  Paramter:
//          [In]:win_id,is_display
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void  Gallery_Main_SetDisplayOsd(MMI_WIN_ID_T win_id,BOOLEAN is_display)
{
   s_gallery_cur_panel_info.is_dis_osd=is_display;
   if(is_display)
   {
      Gallery_ResumeDisplayTimer(win_id);
   }
   else
   {
      Gallery_StopDisplayTimer();
   }
}
LOCAL BOOLEAN Gallery_ClickPointInCtrlRect(GUI_POINT_T up_point)
{
    GUI_RECT_T       play_button_rect   = {0};
    GUI_RECT_T       del_button_rect    = {0};
    if(Gallery_Main_IsDisplayOsd())
    {
        if(MMIGALLERY_FILE_TYPE_PIC==Gallery_GetCurPreviewFileType())
        {
           GUIAPICTRL_GetRect(MMIGALLERY_PIC_DEL_BUTTON_CTRL_ID,&del_button_rect);
           return GUI_PointIsInRect(up_point,del_button_rect);
        }
        else if(MMIGALLERY_FILE_TYPE_VIDEO==Gallery_GetCurPreviewFileType())
        {
           GUIAPICTRL_GetRect(MMIGALLERY_VP_PLAY_BUTTON_CTRL_ID,&play_button_rect);
           GUIAPICTRL_GetRect(MMIGALLERY_VP_DEL_BUTTON_CTRL_ID,&del_button_rect);
           return GUI_PointIsInRect(up_point,del_button_rect)||GUI_PointIsInRect(up_point,play_button_rect);
        }
    }
    return FALSE;
}

LOCAL void Gallery_SetCtrlPanelVisible(MMI_WIN_ID_T win_id,BOOLEAN is_visible,BOOLEAN is_update)
{
    MMIGALLERY_FILE_TYPE_E  cur_file_type   = Gallery_GetCurPreviewFileType();
    switch(cur_file_type)
    {
      case MMIGALLERY_FILE_TYPE_PIC:
           Gallery_Pic_SetBtnVisible(win_id,is_visible,is_update);
           break;
      case MMIGALLERY_FILE_TYPE_VIDEO:
           Gallery_Vp_SetPanelVisible(is_visible);
           break;
      default:
           break;
    }
}

LOCAL void GalleryScreenAreaClickEvent(MMI_WIN_ID_T win_id)
{
     if(!Gallery_Main_IsDisplayOsd())
     {
        Gallery_Main_SetDisplayOsd(win_id,TRUE);
        Gallery_SetCtrlPanelVisible(win_id,TRUE,TRUE);
     }
     else
     {
        Gallery_Main_SetDisplayOsd(win_id,FALSE);
        Gallery_SetCtrlPanelVisible(win_id,FALSE,FALSE);
     }
}

PUBLIC void Gallery_main_ClearScreen(void)
{
    GUI_RECT_T       bg_rect   = DP2PX_RECT(0, 0, 239, 239);
    GUI_LCD_DEV_INFO lcd_dev_info = {0,0};

    MMK_GetWinLcdDevInfo(WATCH_GALLERY_MAIN_WIN_ID, &lcd_dev_info);
    LCD_FillRect(&lcd_dev_info, bg_rect,    MMI_BLACK_COLOR);
}

#if defined( ADULT_WATCH_SUPPORT)
LOCAL void GalleryDrawBackground(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T       up_bg_rect        = GALLERY_UP_INDICATOR_BG_RECT;
    GUI_RECT_T       down_bg_rect      = GALLERY_DOWN_INDICATOR_BG_RECT;
    GUI_RECT_T       left_bg_rect      = GALLERY_LEFT_BG_RECT;
    GUI_RECT_T       right_bg_rect     = GALLERY_RIGHT_BG_RECT;
    GUI_LCD_DEV_INFO lcd_dev_info = {0,0};
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    LCD_FillRect(&lcd_dev_info, up_bg_rect,    MMI_BLACK_COLOR);
    LCD_FillRect(&lcd_dev_info, down_bg_rect,  MMI_BLACK_COLOR);
    LCD_FillRect(&lcd_dev_info, left_bg_rect,  MMI_BLACK_COLOR);
    LCD_FillRect(&lcd_dev_info, right_bg_rect, MMI_BLACK_COLOR);
}
LOCAL void GalleryDrawIndicator(MMI_WIN_ID_T win_id)
{
    uint16           gallery_file_num    = 0;
    GUI_RECT_T       down_indicator_rect = GALLERY_DOWN_INDICATOR_RECT;
    GUI_RECT_T       up_indicator_rect   = GALLERY_UP_INDICATOR_RECT;
    MMI_IMAGE_ID_T   down_image_id       = res_aw_gallery_ic_down;
    MMI_IMAGE_ID_T   up_image_id         = res_aw_gallery_ic_up;
    GUI_LCD_DEV_INFO lcd_dev_info        = {0,0};

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    gallery_file_num = Gallery_GetAllFileNum();
    if(gallery_file_num > 1)//总数大于1时才显示指示器图标
    {
       if(s_gallery_cur_panel_info.file_index != 0)//第一个资源不显示上指示器
       {
           //draw up indicator icon
           GUIRES_DisplayImg(
                             PNULL,
                             &up_indicator_rect,
                             PNULL,
                             win_id,
                             up_image_id,
                             &lcd_dev_info
                             );
       }
       //if(s_gallery_cur_panel_info.file_index != (gallery_file_num - 1))//最后一个不显示下指示器
       {
            //draw down indicator icon
            GUIRES_DisplayImg(
                              PNULL,
                              &down_indicator_rect,
                              PNULL,
                              win_id,
                              down_image_id,
                              &lcd_dev_info
                              );
       }
    }
}
/*****************************************************************************/
//  Description : DrawIndicator
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:TRUE or FALSE
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Gallery_Main_DrawIndicator(BOOLEAN is_draw)
{
    MMI_WIN_ID_T win_id = WATCH_GALLERY_MAIN_WIN_ID;

     GalleryDrawBackground(win_id);
     if(is_draw)
     {
         GalleryDrawIndicator(win_id);
     }
     return TRUE;
}
/*****************************************************************************/
//  Description : Delete Cur Preview File
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:TRUE or FALSE
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Gallery_Main_DeleteCurPreviewFile(void)
{
    BOOLEAN result = FALSE;
    FILEARRAY_DATA_T    file_info      = {0};

    if(MMIGALLERY_FILE_TYPE_VIDEO == Gallery_GetCurPreviewFileType())//bug1639807
    {
          s_gallery_stop_play = Gallery_Vp_StopPlayAndClearDis();//stop play
    }
    //delete gallery file
    TRACE_APP_GALLERY("DeleteSelectedfile start_time = %d", SCI_GetTickCount());
    //get file info
    if (Gallery_GetFileInfo(s_gallery_cur_panel_info.file_index,&file_info))
    {
        if( MMIAPIFMM_DeleteFileAsyn(file_info.filename,file_info.name_len,WATCH_GALLERY_MAIN_WIN_ID,MSG_PICVIEWER_DELETE_CNF) )
        {
            result = TRUE;
        }
    }
    TRACE_APP_GALLERY("DeleteSelectedfile end_time = %d", SCI_GetTickCount());
    if ( !result )
    {
        //prompt fail
        {
           MMI_STRING_T              tip_content  = {0};
           MMI_GetLabelTextByLang(TXT_ERROR, &tip_content);
           Adult_WatchCOM_NoteWin_1Line_Toast_Enter(WATCH_GALLERY_NOTE_WIN_ID,&tip_content,0,PNULL);
        }
    }
    return result;
}
#endif

LOCAL void HandleFileDeleteCNF(MMI_WIN_ID_T    win_id, DPARAM    param)
{
#if defined( ADULT_WATCH_SUPPORT)
     GUI_COLOR_T fontColor  = AUDULT_GRAY2_COLOR;
#elif  defined(SCREEN_SHAPE_CIRCULAR)
    GUI_COLOR_T fontColor  = GRAY2_COLOR;
#endif
     BOOLEAN     is_success = FALSE;
     uint32      remain_file_num = 0;
     LOCAL uint16 delete_num = 0;
     is_success=*((BOOLEAN*)param);
     delete_num ++;
     if(is_success)
     {
        //update gallery array
        MMIAPIFILEARRAY_Delete(s_watch_gallery_lib, s_gallery_cur_panel_info.file_index);
        if(delete_num == pic_del_num)
        {
            delete_num = 0;
            if(pic_del_remain_num == 0)//删除的是最后一个文件
            {
			    if(MMK_IsOpenWin(MMIPICVIEW_SHOW_PREVIEW_WIN_ID))
                {
				    MMK_CloseWin(MMIPICVIEW_SHOW_PREVIEW_WIN_ID);
			    }
			    //add end
                WatchGallery_NoPhotosTipWin();
                MMK_CloseWin(win_id);
                return;
            }
    #ifdef ADULT_WATCH_SUPPORT
            if(TRUE == s_gallery_stop_play)//删除文件时，如果视频不是初始状态，不弹出删除成功窗口，如果有提示窗，因为文件已经被删除了，上层无法获取视频层的显示信息，会有黑色背景
            {
                s_gallery_stop_play = FALSE;
            }
            else
            {
                //prompt success
                {
                   MMI_STRING_T              tip_content  = {0};
                   MMI_GetLabelTextByLang(TXT_GALLERY_DELETED, &tip_content);
                   Adult_WatchCOM_NoteWin_1Line_Toast_Enter(WATCH_GALLERY_NOTE_WIN_ID,&tip_content,0,PNULL);
                }
            }
    #endif
    #ifndef ADULT_WATCH_SUPPORT
            if(MMIGALLERY_FILE_TYPE_VIDEO == Gallery_GetCurPreviewFileType())
            {
                Gallery_Vp_StopPlayAndClearDis();//stop play
            }
    #endif

		    //add by fys for bug 10/27
		    if(delete_button_click == 2)
            {
                delete_button_click = 0;
                MMK_SendMsg(win_id, MSG_PICVIEWER_RELOAD_FILE, PNULL);
		    }
            else
            {			
			    delete_button_click = 0;			
		    }
        }
     }
     else
     {
        //prompt fail
        {
           MMI_STRING_T              tip_content  = {0};
           WATCH_SOFTKEY_TEXT_ID_T   softket_test = {TXT_NULL,TXT_NULL,TXT_NULL};
           MMI_GetLabelTextByLang(TXT_ERROR, &tip_content);
           #ifndef ADULT_WATCH_SUPPORT
           WatchCOM_NoteWin_1Line_Enter(WATCH_GALLERY_NOTE_WIN_ID,&tip_content,0,softket_test,PNULL);
           #else
           Adult_WatchCOM_NoteWin_1Line_Toast_Enter(WATCH_GALLERY_NOTE_WIN_ID,&tip_content,0,PNULL);
           #endif
        }
     }
}
LOCAL void Gallery_HandleTpUpMsg(MMI_WIN_ID_T  win_id, DPARAM    param)
{
    GUI_POINT_T  up_point = {0};
    GUI_RECT_T cancel_rect = GALLERY_PICK_CANEL_RECT;
	GUI_RECT_T del_rect = GALLERY_PICK_DELETE_RECT;
    MMI_TP_FLING_DIRECTION_E fling_direction=((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->tp_fling_direction;
    up_point.x  = MMK_GET_TP_X(param);
    up_point.y  = MMK_GET_TP_Y(param);
    if(fling_direction == TP_FLING_LEFT)
    {
         Gallery_PreFilePreview(win_id);
         s_gallery_is_fling = TRUE;
    }
    else if(fling_direction == TP_FLING_RIGHT)
    {
         Gallery_NextFilePreview(win_id);
         s_gallery_is_fling = TRUE;
    }
    if(!s_gallery_is_fling)
    {
        if(FALSE==Gallery_ClickPointInCtrlRect(up_point))
        {
            GalleryScreenAreaClickEvent(win_id);
        }
    }
	
	if(delete_button_click && GUI_PointIsInRect(up_point,cancel_rect))
	{
        GUI_RECT_T    icon_rect = GALLERY_ICONLIST_RECT;
		delete_button_click = 0;
        GUIAPICTRL_SetRect(MMIPICVIEW_LIST_CTRL_ID, &icon_rect);
        GUIICONLIST_SetAllSelected(MMIPICVIEW_LIST_CTRL_ID, FALSE);
        GUIICONLIST_SetMarkable(MMIPICVIEW_LIST_CTRL_ID, FALSE);
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
	}
	else if(delete_button_click && GUI_PointIsInRect(up_point,del_rect))
	{
		FILEARRAY_DATA_T file_info = {0};
		BOOLEAN result = FALSE;
		uint16 sel_index[100] = {0};
		uint32 index = 0;
		uint32 pic_num = Gallery_GetAllFileNum();
    	uint32 mark_num = GUIICONLIST_GetSelectedItemIndex(MMIPICVIEW_LIST_CTRL_ID, &sel_index, pic_num);
		SCI_TRACE_LOW("%s: pic_num = %d", __FUNCTION__, pic_num);
		SCI_TRACE_LOW("%s: mark_num = %d", __FUNCTION__, mark_num);
		pic_del_remain_num = pic_num - mark_num;
        pic_del_num = mark_num;
		if(mark_num == 0 || pic_del_remain_num < 0)
        {
			return;
		}
		for(index = 0;index < mark_num && index < pic_num;index++)
        {
			SCI_TRACE_LOW("%s: sel_index[%d] = %d", __FUNCTION__, index, sel_index[index]);
			if (Gallery_GetFileInfo(sel_index[index],&file_info))
			{
				if( MMIAPIFMM_DeleteFileAsyn(file_info.filename,file_info.name_len,win_id,MSG_PICVIEWER_DELETE_CNF) )
				{
					result = TRUE;
				}
			}
		}
	}
}


LOCAL GUI_BOTH_RECT_T MMIPICVIEW_GetClientWinRect(MMI_WIN_ID_T  win_id)
{
    GUI_BOTH_RECT_T     client_rect = MMITHEME_GetWinClientBothRect(win_id);
    return client_rect;
}

LOCAL BOOLEAN AppendPicListItem(
    BOOLEAN         is_text,        //in:是否添加text data
    uint16          item_index,     //in:item索引
    uint16          content_index,  //in:item内容索引,text or anim etc.
    MMI_CTRL_ID_T   ctrl_id         //in:control id
)
{
    BOOLEAN             result = FALSE;
    wchar               temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    wchar               *file_name_ptr = PNULL;
    uint16              file_name_len = 0;
    FILEARRAY_DATA_T    file_info = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    GUIANIM_FILE_INFO_T anim_path = {0};

    //get picture info
    if (Gallery_GetFileInfo(content_index,&file_info))
    {

        //icon
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
            item_data.item_content[0].item_data.anim_path_ptr = &anim_path;
            anim_path.full_path_wstr_ptr = file_info.filename;
            anim_path.full_path_wstr_len = file_info.name_len;
        }

        //alloc file name mem
        file_name_ptr = SCI_ALLOC_APP((MMIFILE_FILE_NAME_MAX_LEN + 1) * sizeof(wchar));
        SCI_MEMSET(file_name_ptr, 0, ((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar)));

        //split file name
        MMIAPIFMM_SplitFullPath(file_info.filename,
                              file_info.name_len,
                              PNULL,
                              PNULL,
                              PNULL,
                              PNULL,
                              file_name_ptr,
                              &file_name_len);

        //text1,file name
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = file_name_len;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = file_name_ptr;

        //text2,file size
        item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
        MMIAPIFMM_GetFileSizeString(file_info.size, temp_wstr, MMIFMM_SIZE_STR_LEN, TRUE);
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16)MMIAPICOM_Wstrlen(item_data.item_content[2].item_data.text_buffer.wstr_ptr);

        if (is_text)
        {
            //add item text etc.
            result = GUILIST_SetItemData(ctrl_id,
                                         &item_data,
                                         item_index);
        }
        else
        {
            //add item anim
            result = GUILIST_SetItemContent(ctrl_id,
                                            &item_data.item_content[content_index],
                                            item_index,
                                            content_index);
        }

        //free file name mem
        SCI_FREE(file_name_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : append picture icon list one anim
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AppendPicIconListIcon(
    uint16          index,
    MMI_CTRL_ID_T   ctrl_id
)
{
    BOOLEAN                 result = FALSE;
    FILEARRAY_DATA_T        file_info = {0};
    GUIANIM_FILE_INFO_T     icon_file = {0};

    //get picture info
    if (Gallery_GetFileInfo(index, &file_info))
    {
        {
            //set file name
            icon_file.full_path_wstr_ptr = file_info.filename;
            icon_file.full_path_wstr_len = file_info.name_len;

            //append icon
            GUIICONLIST_AppendIcon(index, ctrl_id, PNULL, &icon_file);
        }

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : clear pictures list
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ClearPicList(MMI_WIN_ID_T   win_id,   MMI_CTRL_ID_T  ctrl_id )
{
    GUIICONLIST_SetMarkable(ctrl_id, FALSE);
    GUIICONLIST_SetTotalIcon(ctrl_id, 0);

    //set title
    //GUIWIN_SetTitleTextId(win_id, TXT_PICVIEW, FALSE);

    //set softkey
    //GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
}

LOCAL void SetIconListParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, BOOLEAN is_folder_list)
{
    GUI_BG_T            ctrl_bg     = {0};
    GUI_BOTH_RECT_T         icon_rect = GALLERY_ICONLIST_RECT;
    if(GUIICONLIST_GetMarkable(ctrl_id))
    {
        icon_rect.v_rect.bottom = icon_rect.v_rect.bottom - GALLERY_PICK_LIST_MARGIN_BOTTOM;
    }
    else
    {
        icon_rect.v_rect.bottom = icon_rect.v_rect.bottom;
    }
		
    //设置iconlist的范围
    GUIAPICTRL_SetBothRect(ctrl_id, &icon_rect);
        
    //设置背景
    ctrl_bg.bg_type         = GUI_BG_COLOR;
    ctrl_bg.is_screen_img   = TRUE;
    ctrl_bg.color       = MMI_BLACK_COLOR;
    GUIICONLIST_SetBg(ctrl_id, &ctrl_bg);

    if(!is_folder_list)
    {
        //设置每个icon的高、宽距
        GUIICONLIST_MARGINSPACE_INFO_T  icon_space = GALLERY_LIST_ITEM_MARGIN_SPACE;
        GUIICONLIST_SetIconItemSpace(ctrl_id, icon_space);
        GUIICONLIST_SetIconWidthHeight(ctrl_id, GALLERY_LIST_ITEM_ICON_WIDTH, GALLERY_LIST_ITEM_ICON_HEIGHT);
		GUIICONLIST_SetHideSlideBar(ctrl_id);
	}
    MMK_SetAtvCtrl(win_id, ctrl_id);
}

/*****************************************************************************/
//     Description : set spftkey param for pic list
//    Global resource dependence :
//  Author: yaye.jiang
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetButtonFormParam(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T* ctrl_id, MMI_IMAGE_ID_T* img_id, uint16 button_num)
{
    BOOLEAN             result      = TRUE;
    GUI_BG_T            bg_info     = {0};
    GUI_BG_T            fg_info     = {0};
    GUI_BOTH_RECT_T     form_rect   = MMITHEME_GetWinClientBothRect(win_id);
    GUI_BOTH_RECT_T     button_rect = {0};
    uint16              button_height   = MMIPICPREVIEW_BOTTOM_MENU_HEIGHT;
    uint16              button_width_v  = form_rect.v_rect.right - form_rect.v_rect.left + 1;
    uint16              button_width_h  = form_rect.h_rect.right - form_rect.h_rect.left + 1;
    int                 i = 0;
    
    if(button_num > 0)
    {
        form_rect.v_rect.top = form_rect.v_rect.bottom - button_height + 1;
        form_rect.h_rect.top = form_rect.h_rect.bottom - button_height + 1;
        bg_info.bg_type = GUI_BG_NONE;
        
        button_width_v    = button_width_v / button_num;
        button_width_h    = button_width_h / button_num;
        button_rect       = form_rect;

        fg_info.bg_type = GUI_BG_IMG;
        for(i = 0; i < button_num; i++)
        {
            button_rect.v_rect.left    = form_rect.v_rect.left + i * button_width_v;
            button_rect.v_rect.right   = button_rect.v_rect.left + button_width_v - 1;
            button_rect.h_rect.left    = form_rect.h_rect.left + i * button_width_h;
            button_rect.h_rect.right   = button_rect.h_rect.left + button_width_h - 1;
            GUIAPICTRL_SetBothRect(ctrl_id[i], &button_rect);

            fg_info.img_id = img_id[i];
            GUIBUTTON_SetBg(ctrl_id[i], &bg_info);
            GUIBUTTON_SetFg(ctrl_id[i], &fg_info);
            GUIBUTTON_PermitBorder(ctrl_id[i], FALSE);
        }
    }
    else
    {
        result = FALSE;
    }
    return FALSE;
}

/*****************************************************************************/
//     Description : set pic list buttons visible
//    Global resource dependence :
//  Author: yaye.jiang
//    Note: 
/*****************************************************************************/
LOCAL void Gallery_SetButtonsVisible(BOOLEAN is_visible)
{
//    GUIBUTTON_SetVisible(MMIPICVIEW_LIST_SLIDE_PREVIEW, is_visible, FALSE);
//    GUIBUTTON_SetVisible(MMIPICVIEW_LIST_MARKED, is_visible, FALSE);    
}

/*****************************************************************************/
//     Description : handle after point the mark menu
//    Global resource dependence :
//  Author: yaye.jiang
//    Note: 
/*****************************************************************************/
LOCAL void InitMarkMenuButtons(MMI_WIN_ID_T win_id)
{
    MMI_CONTROL_CREATE_T    create_ptr  = {0};
    GUIBUTTON_INIT_DATA_T   init_data   = {0};
    GUI_BOTH_RECT_T         form_rect   = MMITHEME_GetWinClientBothRect(win_id);
    GUI_BOTH_RECT_T         button_rect = form_rect;
    uint16                  button_width_v  = form_rect.v_rect.right - form_rect.v_rect.left + 1;
    uint16                  button_width_h  = form_rect.h_rect.right - form_rect.h_rect.left + 1;
//    GUI_BG_T                button_bg      = {0};
    GUI_FONT_ALL_T          button_font_all = {0};//button的字体与颜色
    MMI_CTRL_ID_T           button_id[] = {MMIGALLERY_MARK_ALLMARK_CTRL_ID,MMIGALLERY_UNMARK_ALL_CTRL_ID,MMIGALLERY_MARK_DELETE_CTRL_ID};//
    MMI_TEXT_ID_T           txt_id[]    = {TXT_MARK_ALL, TXT_DELETE, STXT_CANCEL};// 
    uint8                   button_num  = 3;
    uint8                   i;

    {
        create_ptr.guid    = SPRD_GUI_BUTTON_ID;
        create_ptr.parent_win_handle = win_id;
        create_ptr.init_data_ptr = &init_data;

        button_rect.v_rect.bottom = MMIPICPREVIEW_BOTTOM_MENU_HEIGHT + 1;
        button_rect.h_rect.bottom = MMIPICPREVIEW_BOTTOM_MENU_HEIGHT + 1;
                 
        button_width_v    = button_width_v / button_num;
        button_width_h    = button_width_h / button_num;
                
        init_data.bg.bg_type    = GUI_BG_NONE;
        button_font_all.font    = MMI_DEFAULT_NORMAL_FONT;
        button_font_all.color   = MMI_WHITE_COLOR;
 
        button_rect.v_rect.left    = form_rect.v_rect.left + button_width_v;
        button_rect.v_rect.right   = button_rect.v_rect.left + button_width_v - 1;
        button_rect.h_rect.left    = form_rect.h_rect.left + button_width_h;
        button_rect.h_rect.right   = button_rect.h_rect.left + button_width_h - 1;

        init_data.both_rect = button_rect;
        create_ptr.ctrl_id = MMIGALLERY_MARK_ALLMARK_CTRL_ID;
        MMK_CreateControl(&create_ptr);
            
        GUIBUTTON_SetFont(create_ptr.ctrl_id, &button_font_all);
        GUIBUTTON_SetTextId(create_ptr.ctrl_id, TXT_MARK_ALL);
        GUIBUTTON_PermitBorder(create_ptr.ctrl_id, FALSE);
        GUIBUTTON_SetVisible(create_ptr.ctrl_id, FALSE, FALSE); 
        
    }

    {
        create_ptr.guid    = SPRD_GUI_BUTTON_ID;
        create_ptr.parent_win_handle = win_id;
        create_ptr.init_data_ptr = &init_data;

        button_rect.v_rect.bottom = MMIPICPREVIEW_BOTTOM_MENU_HEIGHT + 1;
        button_rect.h_rect.bottom = MMIPICPREVIEW_BOTTOM_MENU_HEIGHT + 1;
                
        init_data.bg.bg_type    = GUI_BG_NONE;
        button_font_all.font    = MMI_DEFAULT_NORMAL_FONT;
        button_font_all.color   = MMI_WHITE_COLOR;
 
        button_rect.v_rect.left    = form_rect.v_rect.left + button_width_v;
        button_rect.v_rect.right   = button_rect.v_rect.left + button_width_v - 1;
        button_rect.h_rect.left    = form_rect.h_rect.left + button_width_h;
        button_rect.h_rect.right   = button_rect.h_rect.left + button_width_h - 1;

        init_data.both_rect = button_rect;
        create_ptr.ctrl_id = MMIGALLERY_UNMARK_ALL_CTRL_ID;
        MMK_CreateControl(&create_ptr);
            
        GUIBUTTON_SetFont(create_ptr.ctrl_id, &button_font_all);
        GUIBUTTON_SetTextId(create_ptr.ctrl_id, TXT_CANCEL_SELECT_ALL);
        GUIBUTTON_PermitBorder(create_ptr.ctrl_id, FALSE);
        GUIBUTTON_SetVisible(create_ptr.ctrl_id, FALSE, FALSE); 
        
    }

    {
        create_ptr.guid    = SPRD_GUI_BUTTON_ID;
        create_ptr.parent_win_handle = win_id;
        create_ptr.init_data_ptr = &init_data;

        button_rect.v_rect.left    = 10;
        button_rect.v_rect.right   = 26;
        button_rect.h_rect.left    = 10;
        button_rect.h_rect.right   = 26;
                 
                
        init_data.bg.bg_type    = GUI_BG_IMG;
        init_data.bg.img_id     = res_common_title_back;
        init_data.both_rect = button_rect;
        create_ptr.ctrl_id = MMIGALLERY_RETURN_BUTTON_CTRL_ID;
        MMK_CreateControl(&create_ptr);
        GUIBUTTON_PermitBorder(create_ptr.ctrl_id, FALSE);
        GUIBUTTON_SetVisible(create_ptr.ctrl_id, FALSE, FALSE);
    }
    
    {
        create_ptr.guid    = SPRD_GUI_BUTTON_ID;
        create_ptr.parent_win_handle = win_id;
        create_ptr.init_data_ptr = &init_data;
        button_rect.h_rect.left = button_rect.v_rect.left = form_rect.v_rect.right - 20 -28;
        button_rect.h_rect.right = button_rect.v_rect.right = form_rect.v_rect.right - 20;
        button_rect.v_rect.bottom = 28 + 1;
        button_rect.h_rect.bottom = 28 + 1;
                 
                
        init_data.bg.bg_type    = GUI_BG_IMG;
        init_data.bg.img_id     = res_gallery_mark_delete;
        init_data.both_rect = button_rect;
        create_ptr.ctrl_id = MMIGALLERY_MARK_DELETE_CTRL_ID;
        MMK_CreateControl(&create_ptr);
        GUIBUTTON_PermitBorder(create_ptr.ctrl_id, FALSE);
        GUIBUTTON_SetVisible(create_ptr.ctrl_id, FALSE, FALSE);
    }
}

LOCAL void InitOptionMenuButtons(MMI_WIN_ID_T win_id)
{
    MMI_CONTROL_CREATE_T    create_ptr  = {0};
    GUIBUTTON_INIT_DATA_T   init_data   = {0};
    GUI_BOTH_RECT_T         form_rect   = MMITHEME_GetWinClientBothRect(win_id);
    GUI_BOTH_RECT_T         button_rect = form_rect;

    {
        create_ptr.guid    = SPRD_GUI_BUTTON_ID;
        create_ptr.parent_win_handle = win_id;
        create_ptr.init_data_ptr = &init_data;
        button_rect.h_rect.left = button_rect.v_rect.left = form_rect.v_rect.right - 20 -28;
        button_rect.h_rect.right = button_rect.v_rect.right = form_rect.v_rect.right - 20;
        button_rect.v_rect.bottom = 28 + 1;
        button_rect.h_rect.bottom = 28 + 1;
                 
                
        init_data.bg.bg_type    = GUI_BG_IMG;
        init_data.bg.img_id     = res_gallery_ic_option;
        init_data.both_rect = button_rect;
        create_ptr.ctrl_id = MMIGALLERY_OPTION_BUTTON_CTRL_ID;
        MMK_CreateControl(&create_ptr);
        GUIBUTTON_PermitBorder(MMIGALLERY_OPTION_BUTTON_CTRL_ID, FALSE);
        GUIBUTTON_SetVisible(create_ptr.ctrl_id, FALSE, FALSE);
    }
}

/*****************************************************************************/
//     Description : set pic list buttons visible
//    Global resource dependence :
//  Author: yaye.jiang
//    Note: 
/*****************************************************************************/
LOCAL void Gallery_DestroyMarkMenuButtons(MMI_WIN_ID_T win_id)
{
    if(PNULL != MMK_GetCtrlPtrByWin(win_id, MMIGALLERY_MARK_ALLMARK_CTRL_ID))
    {
        MMK_DestroyControl(MMIGALLERY_MARK_ALLMARK_CTRL_ID);
        MMK_DestroyControl(MMIGALLERY_MARK_DELETE_CTRL_ID);
        MMK_DestroyControl(MMIGALLERY_UNMARK_ALL_CTRL_ID);
    }
}

/*****************************************************************************/
//  Description : create pictures list control
//  Global resource dependence :
//  Author: Jassmine
//  Note:yaye.jiang
/*****************************************************************************/
LOCAL void CreatePicListCtrl(
                             MMI_WIN_ID_T   win_id, //in:
                             MMI_CTRL_ID_T  ctrl_id //in:
                             )
{
    GUILIST_INIT_DATA_T     list_init_data = {0};
    MMI_CONTROL_CREATE_T    create = {0};
    GUIICONLIST_INIT_DATA_T icon_init_data = {0};
#ifndef BAIDU_DRIVE_SUPPORT
    GUI_BOTH_RECT_T         both_rect = MMITHEME_GetFullScreenBothRect();
#else
    GUI_BOTH_RECT_T         both_rect = MMITHEME_GetFullScreenBothRect();
#endif
    GUI_BORDER_T            boder_type  = {0};
    

    icon_init_data.both_rect = both_rect;

    create.guid          = SPRD_GUI_ICONLIST_ID;
    create.init_data_ptr = &icon_init_data;

    //creat control
    create.ctrl_id           = ctrl_id;
    create.parent_win_handle = win_id;

    MMK_CreateControl(&create);

    //set active
    MMK_SetAtvCtrl(win_id, ctrl_id);
    GUIICONLIST_SetStyle(ctrl_id, GUIICONLIST_STYLE_ICON_UIDT);
    boder_type.color    = MMITHEME_UNSELECT_BORDER_COLOR;
    boder_type.width    = 0;
    boder_type.type     = GUI_BORDER_SOLID;
    GUIICONLIST_SetItemBorderStyle(ctrl_id, FALSE, &boder_type);
	/*edit by fys 09/26 */
    boder_type.width    = 0;
	boder_type.color    = MMI_BLACK_COLOR;
    GUIICONLIST_SetItemBorderStyle(ctrl_id, TRUE, &boder_type);
#ifdef MMI_GUI_STYLE_MINISCREEN
    /*MINI UI 需求:不显示list_title*/
    CTRLICONLIST_SetIsDispTitle(ctrl_id, FALSE);
#endif
    SetIconListParam(win_id, ctrl_id, FALSE);
    GUIICONLIST_SetDefaultIconByPtr(ctrl_id, IMAGE_PICVIEW_FOLDER_DEFAULT_ICON, IMAGE_PICVIEW_PIC_ERROR_ICON);
    GUIICONLIST_SetStyle(ctrl_id, GUIICONLIST_STYLE_ICON);
    GUIICONLIST_SetLoadType(ctrl_id, GUIICONLIST_LOAD_CUR_PAGE);//GUIICONLIST_LOAD_ALL 方式占用内存大
    GUIICONLIST_SetTotalIcon(ctrl_id, Gallery_GetAllFileNum());
	SCI_TRACE_LOW("%s；Gallery_GetAllFileNum() = %d", __FUNCTION__, Gallery_GetAllFileNum());
    GUIICONLIST_SetCurIconIndex(0, ctrl_id);
}

LOCAL void UpdatePicList(MMI_WIN_ID_T   win_id,   MMI_CTRL_ID_T  ctrl_id,   uint16  cur_index)
{
    uint16                  i = 0;
    uint16                  total_num = (uint16)Gallery_GetAllFileNum();
    GUILIST_ITEM_T          item = {0};
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;
    //set max icon
    GUIICONLIST_SetTotalIcon(ctrl_id, total_num);
    //set title
    if (0 == total_num)
    {
#ifndef MMI_GUI_STYLE_MINISCREEN
        //GUIWIN_SetTitleTextId(win_id, TXT_COMMON_PIC, FALSE);
#else
        /*MINI UI 需求*/
        //GUIWIN_SetTitleTextId(win_id, TXT_PICVIEW_TITLE, FALSE);
#endif
    }

    //set current index
    GUIICONLIST_SetCurIconIndex(cur_index, ctrl_id);
}


/*****************************************************************************/
//  Description : modify pictures list view
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ModifyPicListView(MMI_WIN_ID_T   win_id, MMI_CTRL_ID_T  ctrl_id)
{
    uint16                  cur_index = 0;

    //get current index
    cur_index = GUIICONLIST_GetCurIconIndex(ctrl_id);

    //destroy control
    MMK_DestroyControl(ctrl_id);

    //creat control
    CreatePicListCtrl(win_id, ctrl_id);

	//creat title
	GalleryPicList_TitleShow(win_id);
	 
    //need update list
    MMK_PostMsg(win_id, MSG_PICVIEWER_UPDATE_LIST, &cur_index, sizeof(uint16));
}


PUBLIC void GalleryPicListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               )
{
    uint16      mark_num = 0;
    //uint16      cur_index = 0;
    uint32      pic_num = Gallery_GetAllFileNum();

    //get mark num
    mark_num = GUIICONLIST_GetSelectedItemIndex(list_ctrl_id, PNULL, pic_num);
    if (0 == mark_num)
    {
        //set list enable mark
        GUIICONLIST_SetMarkable(list_ctrl_id,TRUE);

        //set mark max number
        GUIICONLIST_SetMaxSelectedItem(list_ctrl_id,(uint16)pic_num);
    }
}

/*****************************************************************************/
//  Description : handle picture list mark cancel option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void GalleryPicListUnmarked(
                                 MMI_CTRL_ID_T    list_ctrl_id
                                 )
{
    uint16      cur_index = 0;
    uint16      mark_num = 0;

    //get item index
    cur_index = GUIICONLIST_GetCurIconIndex(list_ctrl_id);

    //cancel mark item
    MMIPICVIEW_SetSelectedItem(list_ctrl_id,cur_index,FALSE);

    //get mark num
    mark_num = GUIICONLIST_GetSelectedItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //set list enable mark
        //GUIICONLIST_SetMarkable(list_ctrl_id, FALSE);
    }
	GUIICONLIST_SetMarkable(list_ctrl_id, FALSE);
}

/*****************************************************************************/
//  Description : handle picture list mark all option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void GalleryPicListMarkedAll(
                                  MMI_CTRL_ID_T   list_ctrl_id
                                  )
{
    uint16      mark_num = 0;
    uint32      pic_num = Gallery_GetAllFileNum();

    //get mark num
    mark_num = GUIICONLIST_GetSelectedItemIndex(list_ctrl_id, PNULL, pic_num);
    if (0 == mark_num)
    {
        //set list enable mark
        GUIICONLIST_SetMarkable(list_ctrl_id, TRUE);

        //set mark max number
        GUIICONLIST_SetMaxSelectedItem(list_ctrl_id,(uint16)pic_num);
    }

    //mark all item
    GUIICONLIST_SetAllSelected(list_ctrl_id, TRUE);
}

/*****************************************************************************/
//  Description : handle picture list mark all cancel option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void GalleryPicListUnmarkedAll(
                                    MMI_CTRL_ID_T list_ctrl_id
                                    )
{
    GUIICONLIST_SetAllSelected(list_ctrl_id, FALSE);

    //set list enable mark
//    MMIPICVIEW_SetListMarkable(list_ctrl_id,FALSE);
}

/*add by fys 09/26 */
LOCAL void GalleryPicListMarkEnable(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO lcd_dev_info)
{
	GUI_RECT_T cancel_rect = GALLERY_PICK_CANEL_RECT;
	GUI_RECT_T del_rect = GALLERY_PICK_DELETE_RECT;
	GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
	GUISTR_STYLE_T text_style = {0};
	MMI_STRING_T text_string = {0};
    GUI_BOTH_RECT_T    icon_rect = GALLERY_ICONLIST_MARK_RECT;

	//icon_rect.v_rect.bottom = icon_rect.v_rect.bottom - GALLERY_PICK_LIST_MARGIN_BOTTOM;
	//设置iconlist的范围
    GUIAPICTRL_SetBothRect(MMIPICVIEW_LIST_CTRL_ID, &icon_rect);

	GUIRES_DisplayImg(PNULL, &cancel_rect, PNULL, win_id, res_zte_gallery_can, &lcd_dev_info);
	GUIRES_DisplayImg(PNULL, &del_rect, PNULL, win_id, res_zte_gallery_del, &lcd_dev_info);
}
/*add end */

/*add by fys 10/27 */
LOCAL MMI_RESULT_E GalleryPicList_TitleBackCallbackFunc(void)
{
    if(GUIICONLIST_GetMarkable(MMIPICVIEW_LIST_CTRL_ID))//多选操作返回
    {
        GUI_RECT_T    icon_rect = GALLERY_ICONLIST_RECT;
        GUIAPICTRL_SetRect(MMIPICVIEW_LIST_CTRL_ID, &icon_rect);
        GUIICONLIST_SetAllSelected(MMIPICVIEW_LIST_CTRL_ID, FALSE);
        GUIICONLIST_SetMarkable(MMIPICVIEW_LIST_CTRL_ID, FALSE);
        MMK_SendMsg(WATCH_GALLERY_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL);
        delete_button_click = 0;
    }
    else
    {
	    if(MMK_IsOpenWin(WATCH_GALLERY_MAIN_WIN_ID)){
		    MMK_CloseWin(WATCH_GALLERY_MAIN_WIN_ID);
	    }
	    if(MMK_IsOpenWin(MMIPICVIEW_SHOW_PREVIEW_WIN_ID)){
		    MMK_CloseWin(MMIPICVIEW_SHOW_PREVIEW_WIN_ID);
	    }
    }
	return MMI_RESULT_TRUE;
}

LOCAL MMI_RESULT_E GalleryPicList_TitleDelCallbackFunc(void)
{
	if(!delete_button_click)
    {
		//GalleryPicList_TitleBackCallbackFunc();
        if(MMK_IsOpenWin(MMIPICVIEW_SHOW_PREVIEW_WIN_ID))
        {
		    MMK_CloseWin(MMIPICVIEW_SHOW_PREVIEW_WIN_ID);
	    }
		delete_button_click = 2;
		//WatchGallery_MainWin_Enter();
        //MMK_SendMsg(WATCH_GALLERY_MAIN_WIN_ID, MSG_PICVIEWER_RELOAD_FILE, PNULL);
        GalleryPicListMarked(MMIPICVIEW_LIST_CTRL_ID);
	    MMK_SendMsg(WATCH_GALLERY_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL);
	}
	return MMI_RESULT_TRUE;
}

LOCAL void GalleryPicList_TitleButtonCallbackInit(void)
{
	MMI_CTRL_ID_T back_button_ctrlid = MMIGALLERY_MAIN_BACK_BUTTON_CTRL_ID;
	MMI_CTRL_ID_T del_button_ctrlid = MMIGALLERY_MAIN_DEL_BUTTON_CTRL_ID;

	GUIBUTTON_SetCallBackFunc(back_button_ctrlid, GalleryPicList_TitleBackCallbackFunc);
	GUIBUTTON_SetCallBackFunc(del_button_ctrlid, GalleryPicList_TitleDelCallbackFunc);
}

LOCAL void GalleryPicList_TitleShow(MMI_WIN_ID_T win_id)
{
	GUI_LCD_DEV_INFO lcd_dev_info = {0,0};
	GUI_RECT_T text_rect = {50, 12, 120, 40};
	GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN;
	GUISTR_STYLE_T text_style = {0};
	MMI_STRING_T text_string = {0};
	GUI_RECT_T back_rect = {20, 12, 50, 36};
    MMI_CTRL_ID_T back_button_ctrlid = MMIGALLERY_MAIN_BACK_BUTTON_CTRL_ID;
    GUI_RECT_T del_rect = {192, 12, 220, 36};
    MMI_CTRL_ID_T del_button_ctrlid = MMIGALLERY_MAIN_DEL_BUTTON_CTRL_ID;

	//GUIRES_DisplayImg(PNULL, &back_rect, PNULL, win_id, res_zte_gallery_back, &lcd_dev_info);
	//GUIRES_DisplayImg(PNULL, &del_rect, PNULL, win_id, res_zte_gallery_set, &lcd_dev_info);

	text_style.align = ALIGN_HVMIDDLE;
	text_style.font = SONG_FONT_22;
	text_style.font_color = MMI_WHITE_COLOR;
	MMIRES_GetText(TXT_GALLERY,win_id,&text_string);
	GUISTR_DrawTextToLCDInRect(
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&text_rect,
		&text_rect,
		&text_string,
		&text_style,
		text_state,
		GUISTR_TEXT_DIR_AUTO
		);

    GUIBUTTON_SetRect(back_button_ctrlid, &back_rect);	
	GUIBUTTON_SetRunSheen(back_button_ctrlid, FALSE);
	
	GUIBUTTON_SetRect(del_button_ctrlid, &del_rect);
	GUIBUTTON_SetRunSheen(del_button_ctrlid, FALSE);
	
	//窗口full paint时，禁止刷新，防止重绘
	GUIBUTTON_SetForbidPaint(back_button_ctrlid, TRUE);
    GUIBUTTON_SetForbidPaint(del_button_ctrlid, TRUE);

	GUIBUTTON_SetVisible(back_button_ctrlid, TRUE, TRUE);
	GUIBUTTON_SetVisible(del_button_ctrlid, TRUE, TRUE);
}
/*add end */

LOCAL MMI_RESULT_E HandleGalleryMainWinMsg(
    MMI_WIN_ID_T        win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM              param
)
{
    MMI_RESULT_E     result      = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    GUILIST_NEED_ITEM_DATA_T    *item_data_ptr = PNULL;
    GUILIST_NEED_ITEM_CONTENT_T *item_content_ptr = PNULL;
    uint32              cur_index;
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
             MMIDEFAULT_EnableKeyRing(GALLERY_MODULE_NAME,FALSE);//1352322
             //create gallery lib
             Gallery_DestroyLibArray();
             Gallery_CreateLibArray();
			 GalleryPicList_TitleButtonCallbackInit();
             //open waiting win begin load file
             {
                 MMI_WIN_ID_T     wait_win_id = WATCH_GALLERY_WAITING_WIN_ID;
#ifndef ADULT_WATCH_SUPPORT
                 WATCH_SOFTKEY_TEXT_ID_T   softket_test = {TXT_NULL,TXT_NULL,TXT_NULL};
                 WatchCOM_WaitingWin_FullScreen_Enter(wait_win_id,PNULL,image_watch_waitingwin_fullscreen_pic,softket_test,HandleLoadGalleryFileWinMsg);
#else
                 AdultWatchCOM_WaitingWin_1str_WaveIcon_Enter(wait_win_id, PNULL, HandleLoadGalleryFileWinMsg);
#endif
             }
            InitMarkMenuButtons(win_id);
            GalleryPicList_TitleShow(win_id);
        }
        break;
        case MSG_PICVIEWER_LOAD_FINISH://load file end
        {
             if(TRUE == Gallery_FinishLoadFile())
             {
                 //Gallery_Vp_CreateFsmAndInit();       //BUG重复申请图层未释放, 无视频功能 不需要开图层播放视频
                 ////Gallery_DisplayFilePreview(win_id,0);//进入gallery会显示最新的文件
                 CreatePicListCtrl(win_id, ctrl_id);
                 //InitMarkMenuButtons(win_id);
                 ////InitOptionMenuButtons(win_id);
                 //GalleryPicList_TitleShow(win_id);
                 
             }
			 //add by fys for bug 10/24
			 if(delete_button_click){
			 	GalleryPicListMarked(MMIPICVIEW_LIST_CTRL_ID);
	        	MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
			 }
			 //add end
        }
        break;
        case MSG_PICVIEWER_DELETE_CNF:
        {
             HandleFileDeleteCNF(win_id, param);
#ifndef ADULT_WATCH_SUPPORT
             MMK_CloseWin(WATCH_GALLERY_QUERY_WIN_ID);//Bug 2199498
#endif
        }
        break;
        case MSG_APP_DOWN:
        {
             Gallery_NextFilePreview(win_id);
        }
        break;
        case MSG_APP_UP:
        {
             Gallery_PreFilePreview(win_id);
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_UP:
        {
             Gallery_HandleTpUpMsg(win_id,param);
        }
        break;
        case MSG_TP_PRESS_DOWN:
        {
           s_gallery_is_fling=FALSE;
        }
        break;
#endif
        case MSG_TIMER:
        {
            if(s_gallery_cur_panel_info.dis_osd_timer_id == *(uint8*)param)
            {
               Gallery_Main_SetDisplayOsd(win_id,FALSE);
               Gallery_SetCtrlPanelVisible(win_id,FALSE,FALSE);
            }
        }
        break;
        case MSG_CLOSE_WINDOW:
        {
             MMIDEFAULT_EnableKeyRing(GALLERY_MODULE_NAME,TRUE);//1352322
             Gallery_DestroyLibArray();
             Gallery_Vp_FSMRelease();
#ifdef UI_MULTILAYER_SUPPORT
             Gallery_Vp_DestoryLayer();
#endif
#ifdef VIDEOTHUMBNAIL_SUPPORT
             MMIAPIVP_ThumbnailClose();//bug1348408
#endif
             MMIDEFAULT_AllowTurnOffBackLight(TRUE);
             s_gallery_cur_panel_info.file_index=0;
             Gallery_SaveCurPreviewFileType(MMIGALLERY_FILE_TYPE_ERROR);
			 //add by fys for bug 10/24
			 delete_button_click = 0;
			 //add end
			 if(MMK_IsOpenWin(MMIPICVIEW_SHOW_PREVIEW_WIN_ID))
			 {
			 	MMK_CloseWin(MMIPICVIEW_SHOW_PREVIEW_WIN_ID);
			 }
        }
        break;
        case MSG_APP_0:
        case MSG_APP_1:
        case MSG_APP_2:
        case MSG_APP_3:
        case MSG_APP_4:
        case MSG_APP_5:
        case MSG_APP_6:
        case MSG_APP_7:
        case MSG_APP_8:
        case MSG_APP_9:
        case MSG_APP_HASH:
        case MSG_APP_STAR:
        case MSG_APP_GREEN:
        {
             if(!Gallery_Main_IsDisplayOsd())
             {
                Gallery_Main_SetDisplayOsd(win_id,TRUE);
                Gallery_SetCtrlPanelVisible(win_id,TRUE,TRUE);
             }
        }
        break;
        case MSG_PICVIEWER_RELOAD_FILE:
        //load file
        Gallery_DestroyLibArray();
        Gallery_Vp_FSMRelease();
        Gallery_CreateLibArray();

#ifdef UI_MULTILAYER_SUPPORT
        Gallery_Vp_DestoryLayer();
#endif
        //clear pic list
        ClearPicList(win_id, ctrl_id);
        MMK_DestroyControl(ctrl_id);
        {
            MMI_WIN_ID_T     wait_win_id = WATCH_GALLERY_WAITING_WIN_ID;
            WATCH_SOFTKEY_TEXT_ID_T   softket_test = {TXT_NULL,TXT_NULL,TXT_NULL};
            WatchCOM_WaitingWin_FullScreen_Enter(wait_win_id,PNULL,image_watch_waitingwin_fullscreen_pic,softket_test,HandleLoadGalleryFileWinMsg);
        }
        break;

    case MSG_PICVIEWER_UPDATE_LIST:
        //update file
        UpdatePicList(win_id, ctrl_id, *(uint16*)param);
        break;

    case MSG_CTL_LIST_NEED_ITEM_DATA:
        //add item text etc.
        item_data_ptr = (GUILIST_NEED_ITEM_DATA_T *)param;
        if (PNULL == item_data_ptr)
        {
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePicListWinMsg item_data_ptr is PNULL !!!"
            break;
        }

        AppendPicListItem(TRUE, item_data_ptr->item_index, 0, ctrl_id);
        break;

    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        //add item anim
        item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T *)param;
        if (PNULL == item_content_ptr)
        {
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePicListWinMsg item_content_ptr is PNULL !!!"
            break;
        }

        AppendPicListItem(FALSE, item_content_ptr->item_index, item_content_ptr->item_content_index, ctrl_id);
        break;

    case MSG_CTL_ICONLIST_MARK_ITEM:
        {
            //get mark num
            uint16 mark_num = GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0);
			/*edit by fys 09/26 */
            /*if (0 == mark_num)
            {
                //set list enable mark
                //GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                GUIBUTTON_SetVisible(MMIGALLERY_MARK_ALLMARK_CTRL_ID, TRUE, FALSE); 
                GUIBUTTON_SetVisible(MMIGALLERY_UNMARK_ALL_CTRL_ID, FALSE, FALSE);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
            }
            else if(mark_num == Gallery_GetAllFileNum())
            {
                GUIBUTTON_SetVisible(MMIGALLERY_MARK_ALLMARK_CTRL_ID, FALSE, FALSE); 
                GUIBUTTON_SetVisible(MMIGALLERY_UNMARK_ALL_CTRL_ID, TRUE, FALSE);
                MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
            }*/
        }
        break;

    case MSG_PICVIEWER_MODIFY_VIEW:
        ModifyPicListView(win_id, ctrl_id);
        break;

    case MSG_CTL_ICONLIST_APPEND_TEXT:
        {
            GUIICONLIST_DISP_T display_ptr      = {0};

            display_ptr.softkey_id[0] = STXT_OPTION;
            display_ptr.softkey_id[2] = STXT_RETURN;
            GUIICONLIST_AppendText(*(uint16 *)param, ctrl_id, &display_ptr);
        }
        break;

    case MSG_CTL_ICONLIST_APPEND_ICON:
        AppendPicIconListIcon(*(uint16 *)param, ctrl_id);
        break;
    case MSG_CTL_PENOK:
        if(PNULL != param)
        {   
            uint16  marked_num = 0;
            cur_index = GUIICONLIST_GetCurIconIndex(ctrl_id);
            switch(((MMI_NOTIFY_T*)param)->src_id)
            {
                case MMIGALLERY_LIST_MARKED:
                {
                    cur_index = GUIICONLIST_GetCurIconIndex(ctrl_id);
                    GalleryPicListMarked(ctrl_id);
                    GUIICONLIST_SetSelectedItem(ctrl_id,cur_index,TRUE);
                }
                break;

                case MMIGALLERY_RETURN_BUTTON_CTRL_ID:
                    GalleryPicListUnmarked(ctrl_id);
                    GUIBUTTON_SetVisible(MMIGALLERY_OPTION_BUTTON_CTRL_ID, TRUE, FALSE); 
                    GUIBUTTON_SetVisible(MMIGALLERY_MARK_ALLMARK_CTRL_ID, FALSE, FALSE); 
                    GUIBUTTON_SetVisible(MMIGALLERY_RETURN_BUTTON_CTRL_ID, FALSE, FALSE);
                    GUIBUTTON_SetVisible(MMIGALLERY_MARK_DELETE_CTRL_ID, FALSE, FALSE);
                    GUIBUTTON_SetVisible(MMIGALLERY_UNMARK_ALL_CTRL_ID, FALSE, FALSE);
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
                    break;

                case MMIGALLERY_MARK_ALLMARK_CTRL_ID:
                    GalleryPicListMarkedAll(ctrl_id);
                    GUIBUTTON_SetVisible(MMIGALLERY_MARK_ALLMARK_CTRL_ID, FALSE, FALSE); 
                    GUIBUTTON_SetVisible(MMIGALLERY_UNMARK_ALL_CTRL_ID, TRUE, FALSE);
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
                    break;

                case MMIGALLERY_UNMARK_ALL_CTRL_ID:
                    GUIBUTTON_SetVisible(MMIGALLERY_MARK_ALLMARK_CTRL_ID, TRUE, FALSE); 
                    GUIBUTTON_SetVisible(MMIGALLERY_UNMARK_ALL_CTRL_ID, FALSE, FALSE);
                    GalleryPicListUnmarkedAll(ctrl_id);
                    break;
                case MMIGALLERY_MARK_DELETE_CTRL_ID:
                    Gallery_Main_OpenDelQueryWin();
                    break;
                case MMIGALLERY_OPTION_BUTTON_CTRL_ID:
                    GUIBUTTON_SetVisible(MMIGALLERY_OPTION_BUTTON_CTRL_ID, FALSE, FALSE); 
                    GUIBUTTON_SetVisible(MMIGALLERY_MARK_ALLMARK_CTRL_ID, TRUE, FALSE); 
                    GUIBUTTON_SetVisible(MMIGALLERY_RETURN_BUTTON_CTRL_ID, TRUE, FALSE);
                    GUIBUTTON_SetVisible(MMIGALLERY_MARK_DELETE_CTRL_ID, TRUE, FALSE); 
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, NULL);
                    GalleryPicListMarked(ctrl_id);
                    break;
                case MMIPICVIEW_LIST_CTRL_ID:
                    cur_index = GUIICONLIST_GetCurIconIndex(ctrl_id);
                     s_gallery_cur_panel_info.file_index=cur_index;
                    Gallery_PreFilePreview(win_id);
                    break;

            }
        }
        break;
        case MSG_FULL_PAINT:
        {
			GUI_RECT_T       bg_rect     = {0, 0, MMI_MAINSCREEN_WIDTH,MMI_MAINSCREEN_HEIGHT};
            GUI_LCD_DEV_INFO lcd_dev_info = {0,0};
            LCD_FillRect(&lcd_dev_info, bg_rect,    MMI_BLACK_COLOR);
			/*edit by fys 09/26 */
            //WATCHCOM_DisplayTitle(win_id, TXT_GALLERY);
            if(GUIICONLIST_GetMarkable(ctrl_id)){ 
				GalleryPicListMarkEnable(win_id, lcd_dev_info);
			}
			/*edit end*/
			GalleryPicList_TitleShow(win_id);
        }
        break;
        case MSG_KEYDOWN_RED:
            break;
        case MSG_KEYUP_RED:
            MMK_CloseWin(win_id);
            break;
        
        default:
        {
             if(MMIGALLERY_FILE_TYPE_VIDEO == Gallery_GetCurPreviewFileType())
             {
                 result = Gallery_Vp_HandleMsg(win_id,msg_id,param);
             }
             else if(MMIGALLERY_FILE_TYPE_PIC == Gallery_GetCurPreviewFileType())
             {
                 //result = Gallery_Pic_HandleMsg(win_id,msg_id,param);
                 result = MMI_RESULT_FALSE;
             }
             else
             {
                 result = MMI_RESULT_FALSE;
             }
        }
        break;
    }
    return result;
}

// window table of gallery main win
WINDOW_TABLE(WATCH_GALLERY_MAIN_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleGalleryMainWinMsg),
    WIN_ID(WATCH_GALLERY_MAIN_WIN_ID),
    CREATE_BUTTON_CTRL(res_zte_gallery_set, MMIGALLERY_MAIN_DEL_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(res_zte_gallery_back, MMIGALLERY_MAIN_BACK_BUTTON_CTRL_ID),
    WIN_HIDE_STATUS,
    END_WIN
};
/*****************************************************************************/
//  Description : enter gallery main win
//  Paramter:
//          [In]:None
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void WatchGallery_MainWin_Enter(void)
{
#ifdef BAIDU_DRIVE_SUPPORT
	MMIPICVIEW_OpenPicViewerWin();
#else
    if (!MMIAPIUDISK_TipWin_Judge())
    {
        MMK_CreateWin((uint32 *)WATCH_GALLERY_MAIN_WIN_TAB,PNULL);
    }
#endif
}
#endif

PUBLIC void WatchGallery_Exit(void)
{
    if(MMK_IsOpenWin(MMIPICVIEW_SHOW_PREVIEW_WIN_ID))
    {
	    MMK_CloseWin(MMIPICVIEW_SHOW_PREVIEW_WIN_ID);
    }
    if(MMK_IsOpenWin(WATCH_GALLERY_MAIN_WIN_ID))
    {
	    MMK_CloseWin(WATCH_GALLERY_MAIN_WIN_ID);
    }
}
