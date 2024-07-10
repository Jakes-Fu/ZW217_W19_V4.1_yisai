/*****************************************************************************
** File Name:      mmiwidget_calllog.c                                        *
** Author:                                                                   *
** Date:           10/23/2009                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2011/08/01     hongbo.lan          Creat
******************************************************************************/


#define _MMIWIDGET_IMGVIEWER_C

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"

//#ifdef MMI_GRID_IDLE_SUPPORT
#include "mmi_app_widget_trc.h"
#if defined MMI_GRID_IMG_VIEWER
#include "mmi_nv.h"
#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guires.h"
#include "guianim.h"
#include "mmidisplay_data.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
#include "mmiwidget_position.h"
#include "mmifilearray_export.h"
#include "mmifmm_export.h"
#include "mmiwidget_nv.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmk_tp.h"
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_idlewin.h"
#endif
#include "mmipicview_export.h"
#include "mmifmm_interface.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#if defined MAINLCD_SIZE_320X480
#define MMIGRID_IMGVIEWER_ANIM_MAX     3
#else
#define MMIGRID_IMGVIEWER_ANIM_MAX     3
#endif
#define MMIGRID_IMGVIEWER_ANIM_SETP_OFFSET     (8)

#define MMI_IMGVIEWER_FRW_MARGIN   20 

#ifdef MMI_SAM_THEME 
#define MMI_SAM_IMGVIEWER_FRW_MARGIN   2 
#endif
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

typedef struct
{
    BOOLEAN                     is_first_create;   //the first creation of img viewer.

    BOOLEAN                     is_set;            //the selected folder has been set.
    MMIGRID_IMGVIEWWER_FOLDER_T img_folder;        //selected image folder.
    
    uint32                      files_num;         //file num of selected image folder.
    FILEARRAY                   s_files;           //file path info
    uint32                      cur_idx;           //the current displayed image index.
    
    GUI_POINT_T                 cur_point;         //save current point.
    GUI_POINT_T                 old_point;         //save tp point.

    BOOLEAN                     is_v_slide;        //vertical slide.
    BOOLEAN                     is_up;             //slide up.
    
    uint32                      cur_anim_idx;      //current animation control index.
    uint32                      bg_anim_idx;       //bg animation ctrl index.
    uint32                      mov_anim_idx;      //moving animation ctrl index.
    MMI_IMAGE_ID_T              bg_img_id;         //background image id.
}MMIGRID_IMGVIEWER_INFO_T;

LOCAL MMIGRID_IMGVIEWER_INFO_T s_imgviewer_info                      = {0};
LOCAL GUIIMG_BITMAP_T          s_img_bmp[MMIGRID_IMGVIEWER_ANIM_MAX] = {0};
LOCAL int32            s_img_bmg_state[MMIGRID_IMGVIEWER_ANIM_MAX]   = {0};    //0:not init ;1 ready; -1 decoding   
LOCAL uint8	                   s_tp_move_timer_id                    = 0;   //tp move timer.
#ifdef BAR_PHONE_WIDGET
LOCAL const char img_view_default_folder_name[MMIFILE_FULL_PATH_MAX_LEN+1] = "E:\\Photos";
#endif

LOCAL GUI_LCD_DEV_INFO  s_widget_application_layer = {0, UILAYER_NULL_HANDLE};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetImgViewerWinMsg(
										   MMI_WIN_ID_T        win_id,        //IN:
										   MMI_MESSAGE_ID_E    msg_id,        //IN:
										   DPARAM            param        //IN:
										   );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayImageViewer(MMI_WIN_ID_T     win_id);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImgViewerTpUp(MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleImgViewerTpDown(MMI_WIN_ID_T  win_id, DPARAM  param);

/*****************************************************************************/
//  Description : reset call log button status.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void  ResetWidgetImgViewer();

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void SetImgAnimData(uint32 ctrl_idx, uint32 idx);
/*****************************************************************************/
// Description :   清除anim控件的buffer
// Global resource dependence :
// Author: guodong.liu
// Note: 
/*****************************************************************************/
LOCAL void ClearImgAnimData(void);

/*****************************************************************************/
//  Description :  update folder photo.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UpdateFolderPhoto(void);

/*****************************************************************************/
//  Description : handle image viewer tp move action.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImgViewerTpMove(MMI_WIN_ID_T win_id, DPARAM param);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayMovingImg(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
//  Description : get next photo index.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 GetNextPhotoIdx();

/*****************************************************************************/
//  Description : get previous photo index.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 GetPrevPhotoIdx();

/*****************************************************************************/
//  Description : set current photo index.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetCurPhotoIdx(uint32 index);

/*****************************************************************************/
//  Description : go to previous photo.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 GetCurAnimCtrlIdx(void);

/*****************************************************************************/
//  Description : go next animation control index.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 GetNextAnimCtrlIdx(uint32 cur_idx);

/*****************************************************************************/
//  Description : go previous animation control index.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 GetPrevAnimCtrlIdx(uint32 cur_idx);

/*****************************************************************************/
//  Description : get previous photo index.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 GetPrevPhotoIndex(uint32 idx);

/*****************************************************************************/
//  Description : go to next photo.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 GetNextPhotoIndex(uint32 idx);
LOCAL void OpenPhotoFolderCallback(BOOLEAN is_success, FILEARRAY file_array);
LOCAL void DisplayImage(MMI_WIN_ID_T  win_id, int32 ctrl_index, GUI_LCD_DEV_INFO *lcd_dev_info);


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImgViewer(MMI_WIN_ID_T win_id,  BOOLEAN is_up);

/*****************************************************************************/
//  Description : get dual aclock widget image.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void GetImageViewerWidgetImage(MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img);

/**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_imgviewer_widget =
{
    MMIWIDGET_IMG_VIEWER_ID, 
	HandleWidgetImgViewerWinMsg, 
  	PNULL,
   	ResetWidgetImgViewer,
   	GetImageViewerWidgetImage,
	MMI_GRID_IMGVIEWER_WIN_ID, 
	IMAGE_GRID_IMGVIEWER_SHORTCUT, 
	IMAGE_GRID_IMGVIEWER_BG,
    TXT_COMMON_PIC,		
	0,//    75, 
	0,//    50
    TRUE,
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get dual aclock widget image.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void GetImageViewerWidgetImage(MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img)
{
    if (PNULL != bg_img)
    {
        *bg_img = IMAGE_GRID_IMGVIEWER_BG;
    }

    if (PNULL != sc_img)    
    {
        *sc_img = IMAGE_GRID_IMGVIEWER_SHORTCUT;
    }
    
#ifdef MMI_SAM_THEME	
    if ((MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM) && (PNULL != bg_img))
    {
        *bg_img = IMAGE_GRID_SAM_IMGVIEWER_BG;
    }
#endif    
}

/*****************************************************************************/
//  Description : create widget应用层
//  Global resource dependence : 
//  Author:hongbo.lan
//  Note: 
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO CreateApplicationLayer(uint16 width, uint16 height)
{
    UILAYER_CREATE_T create_info = {0};
    UILAYER_APPEND_BLT_T append_layer = {0};
    
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_widget_application_layer))    
    {
        create_info.lcd_id = LCD_ID_0;
        create_info.owner_handle = MAIN_IDLE_WIN_ID;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = width;
        create_info.height = height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;        
        UILAYER_CreateLayer(&create_info, 
            &s_widget_application_layer);
        
    }
    if (UILAYER_IsLayerActive(&s_widget_application_layer))
    {
        append_layer.lcd_dev_info = s_widget_application_layer;
        append_layer.layer_level = UILAYER_LEVEL_HIGH;
        UILAYER_AppendBltLayer(&append_layer);
    }
    return s_widget_application_layer;
}


/*****************************************************************************/
//  Description : create tp move timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ImgviewerCreateTpMoveTimer(MMI_HANDLE_T win_id)
{
	uint32  time_out = 20;
    s_tp_move_timer_id = MMK_CreateWinTimer(win_id, time_out, FALSE);
}

/*****************************************************************************/
//  Description : stop tp move timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ImgviewerStopTpMoveTimer(void)
{
    if (0 != s_tp_move_timer_id)
    {
        MMK_StopTimer(s_tp_move_timer_id);
        s_tp_move_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : restart tp move timer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ImgviewerRestartTpMoveTimer(MMI_HANDLE_T win_id)
{
    ImgviewerStopTpMoveTimer();
    ImgviewerCreateTpMoveTimer(win_id);
}

/*****************************************************************************/
//  Description : read image viewer folder information.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ReadImgViewerFolder()
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMI_GRID_IMGVIEWER_FOLDER, &s_imgviewer_info.img_folder, return_value);

    if (MN_RETURN_SUCCESS == return_value && 0 != s_imgviewer_info.img_folder.name_len)
    {
        s_imgviewer_info.is_set = TRUE;
        s_imgviewer_info.cur_idx = 0;
    }   
#ifdef BAR_PHONE_WIDGET
    else if( 0 == s_imgviewer_info.img_folder.name_len)
    {        
        MMIAPICOM_StrToWstr(img_view_default_folder_name, s_imgviewer_info.img_folder.folder_name);   
        s_imgviewer_info.img_folder.name_len= MMIAPICOM_Wstrlen(s_imgviewer_info.img_folder.folder_name);
        MMINV_WRITE(MMI_GRID_IMGVIEWER_FOLDER, &s_imgviewer_info.img_folder);
        s_imgviewer_info.is_set = TRUE;
        s_imgviewer_info.cur_idx = 0;
    }
#endif
    else
    {
        s_imgviewer_info.is_set = FALSE;      
    }
}

/*****************************************************************************/
//  Description : reset call log button status.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void  ResetWidgetImgViewer()
{
    s_imgviewer_info.is_v_slide = FALSE;
    ImgviewerStopTpMoveTimer();
    UILAYER_RELEASELAYER(&s_widget_application_layer);
}

LOCAL GUI_RECT_T CetAnimCtrlRect(MMI_WIN_ID_T  win_id)
{
    GUI_RECT_T rect = {0};
    MMK_GetWinRect(win_id, &rect);

#ifdef MMI_SAM_THEME   
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        rect.left    += MMI_SAM_IMGVIEWER_FRW_MARGIN;    
        rect.top     += MMI_SAM_IMGVIEWER_FRW_MARGIN;
        rect.bottom  -= MMI_SAM_IMGVIEWER_FRW_MARGIN;
        rect.right   -= MMI_SAM_IMGVIEWER_FRW_MARGIN;    
    }
    else
#endif
    {    
        rect.left   += MMI_IMGVIEWER_FRW_MARGIN;    
        rect.top    += MMI_IMGVIEWER_FRW_MARGIN;
        rect.bottom -= MMI_IMGVIEWER_FRW_MARGIN;
        rect.right  -= MMI_IMGVIEWER_FRW_MARGIN;
    }

	return rect;
}

/*****************************************************************************/
// Description : create image viewer image animation control.
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void UpdateImgAnimCtrl(MMI_WIN_ID_T  win_id, uint32 idx)
{
    MMI_CTRL_ID_T ctrl_id = 0;
    GUI_RECT_T disp_rect = CetAnimCtrlRect(win_id);
  
    ctrl_id = MMIGRID_IMGVIEWER_ANIM0_ID + idx;

    GUIANIM_SetVisible(ctrl_id, FALSE, FALSE);

    //set anim ctrl rect
    GUIANIM_SetCtrlRect(ctrl_id,&disp_rect);

    //set image display rect
    GUIANIM_SetDisplayRect(ctrl_id,&disp_rect,FALSE);

}

LOCAL void CreateImgAnimCtrl(MMI_WIN_ID_T  win_id, uint32 idx)
{
    GUIANIM_INIT_DATA_T init_data = {0};
    MMI_CONTROL_CREATE_T create   = {0};           
    MMI_CTRL_ID_T ctrl_id = 0;
  
    ctrl_id = MMIGRID_IMGVIEWER_ANIM0_ID + idx;

    create.ctrl_id = ctrl_id;
    create.guid = SPRD_GUI_ANIM_ID;
    create.parent_win_handle = win_id;
    create.parent_ctrl_handle = 0;
    create.init_data_ptr = &init_data;
    
    MMK_CreateControl(&create);

    UpdateImgAnimCtrl(win_id, idx);

}

/*****************************************************************************/
// Description : 显示网页加载完成动画的初始/完成状态
// Global resource dependence :
// Author: hongbo.lan
// Note: 
/*****************************************************************************/
LOCAL void SetImgAnimData(uint32 ctrl_idx, uint32 idx)
{
	GUIANIM_CTRL_INFO_T     ctrl_info = {0};
	GUIANIM_FILE_INFO_T file_info  = {0};
	MMI_CTRL_ID_T ctrl_id = 0;
	GUIANIM_DISPLAY_INFO_T display_info = {0};
   	FILEARRAY_DATA_T        file_path = {0};
	ctrl_id = MMIGRID_IMGVIEWER_ANIM0_ID + ctrl_idx;

    ctrl_info.is_wallpaper_ctrl = FALSE;
	ctrl_info.is_ctrl_id        = TRUE;
	ctrl_info.ctrl_id           = ctrl_id;
	ctrl_info.ctrl_ptr          = PNULL;

    //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER SetImgAnimData ctrl_idx = %d,photo_index =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_398_112_2_18_3_7_24_50,(uint8*)"dd",ctrl_idx,idx);
    //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER SetImgAnimData s_imgviewer_info.s_files =%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_400_112_2_18_3_7_24_51,(uint8*)"d",s_imgviewer_info.s_files);
	//set file path
    if(!MMIAPIFILEARRAY_Read(s_imgviewer_info.s_files, idx, &file_path))
    {
        //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER SetImgAnimData  MMIAPIFILEARRAY_Read failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_404_112_2_18_3_7_24_52,(uint8*)"");
        return;
    }
	file_info.full_path_wstr_ptr = file_path.filename;
	file_info.full_path_wstr_len = file_path.name_len;
#if (MMIGRID_IMGVIEWER_ANIM_MAX < 3)                    
	display_info.is_update         = TRUE;
#endif   
	display_info.align_style       = GUIANIM_ALIGN_HVMIDDLE;
	display_info.is_disp_one_frame = FALSE;
	display_info.is_crop_rect      = TRUE;
    display_info.is_free_buf_lose_focus = TRUE;
    
	if(MMIAPIFMM_IsFileExist(file_info.full_path_wstr_ptr, file_info.full_path_wstr_len))
    {  
        display_info.bg.bg_type = GUI_BG_COLOR;
        display_info.bg.color   = MMI_WHITE_COLOR;
    	display_info.bg.shape   = GUI_SHAPE_RECT;
        //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER SetImgAnimData  time = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_420_112_2_18_3_7_24_53,(uint8*)"d",SCI_GetTickCount());
        s_img_bmg_state[ctrl_idx] = 0; //start decoding
   		GUIANIM_SetParam(&ctrl_info,
                         PNULL,
                         &file_info,
                         &display_info);
    }
}
/*****************************************************************************/
// Description :   清除anim控件的buffer
// Global resource dependence :
// Author: guodong.liu
// Note: 
/*****************************************************************************/
LOCAL void ClearImgAnimData(void)
{
	GUIANIM_CTRL_INFO_T     ctrl_info = {0};
	GUIANIM_FILE_INFO_T file_info  = {0};
	uint32 ctrl_idx = 0;
	MMI_CTRL_ID_T ctrl_id = 0;
	GUIANIM_DISPLAY_INFO_T display_info = {0};
   	FILEARRAY_DATA_T        file_path = {0};
	
	ctrl_info.is_wallpaper_ctrl = FALSE;
	ctrl_info.is_ctrl_id        = TRUE;
	ctrl_info.ctrl_id           = ctrl_id;
	ctrl_info.ctrl_ptr          = PNULL;
	
	file_info.full_path_wstr_ptr = PNULL;
	file_info.full_path_wstr_len = 0;
#if (MMIGRID_IMGVIEWER_ANIM_MAX < 3)                    
	display_info.is_update         = TRUE;
#endif   
	display_info.align_style       = GUIANIM_ALIGN_HVMIDDLE;
	display_info.is_disp_one_frame = FALSE;
	display_info.is_crop_rect      = TRUE;
	display_info.is_free_buf_lose_focus = TRUE;
    
	display_info.bg.bg_type = GUI_BG_COLOR;
	display_info.bg.color   = MMI_WHITE_COLOR;
	display_info.bg.shape   = GUI_SHAPE_RECT;
	for(ctrl_idx = 0;ctrl_idx<MMIGRID_IMGVIEWER_ANIM_MAX;ctrl_idx++)
	{         
		ctrl_id = MMIGRID_IMGVIEWER_ANIM0_ID + ctrl_idx;
		ctrl_info.ctrl_id           = ctrl_id;
		
		GUIANIM_SetParam(&ctrl_info,
			PNULL,
			&file_info,
			&display_info);
	}
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ClearImgViewerInfo()
{
    s_imgviewer_info.cur_idx    = 0;
    s_imgviewer_info.files_num  = 0;
    s_imgviewer_info.is_set     = FALSE;
    s_imgviewer_info.is_v_slide = FALSE;

    
    MMIAPIFMM_SearchFileStop();
    
    if(s_imgviewer_info.s_files != 0)
    {
        MMIAPIFILEARRAY_Destroy(&s_imgviewer_info.s_files);   
        s_imgviewer_info.s_files = 0;
    }
    
    //SCI_MEMSET(s_imgviewer_info.img_folder.folder_name, 0, (MMIFILE_FULL_PATH_MAX_LEN+1)*sizeof(wchar));
    s_imgviewer_info.img_folder.name_len = 0;
    ClearImgAnimData();

    MMINV_WRITE(MMI_GRID_IMGVIEWER_FOLDER, &s_imgviewer_info.img_folder);

    
    GUIANIM_SetVisible(MMIGRID_IMGVIEWER_ANIM0_ID, FALSE, FALSE);
    GUIANIM_SetVisible(MMIGRID_IMGVIEWER_ANIM1_ID, FALSE, FALSE);
    GUIANIM_SetVisible(MMIGRID_IMGVIEWER_ANIM2_ID, FALSE, FALSE);
}

LOCAL void DisplayImage(MMI_WIN_ID_T  win_id, int32 ctrl_index, GUI_LCD_DEV_INFO *lcd_dev_info)
{
    GUI_RECT_T       anim_rect    = {0};
    GUI_POINT_T      start_pt     = {0};
    
    if (!UILAYER_IsLayerActive(lcd_dev_info))
    {
        return;
    }   
        
    if(s_img_bmg_state[ctrl_index] == 1)
    {
        GUIIMG_BITMAP_T img_bmp = s_img_bmp[ctrl_index];

        if (PNULL == img_bmp.bit_ptr)
        {
            return;
        }
        
        anim_rect = CetAnimCtrlRect(win_id);

        start_pt.x = (anim_rect.left + anim_rect.right + 1 - img_bmp.img_width) / 2;
        start_pt.y = (anim_rect.top + anim_rect.bottom + 1 - img_bmp.img_height) / 2;
        
    
        GUIIMG_DisplayBmp(FALSE, &anim_rect, &start_pt, &img_bmp, lcd_dev_info);           		  
    }    
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetImgViewerWinMsg(MMI_WIN_ID_T        win_id,
										        MMI_MESSAGE_ID_E    msg_id,
										        DPARAM              param)
{
    uint32       i      = 0;
    MMI_RESULT_E recode = MMI_RESULT_TRUE;	

    switch (msg_id)
    {     
	case MSG_OPEN_WINDOW: 
		for (i = 0; i < MMIGRID_IMGVIEWER_ANIM_MAX; i++)
		{
   	        CreateImgAnimCtrl(win_id, i);
		}
		break;

#if (MMIGRID_IMGVIEWER_ANIM_MAX < 3)  
    case MSG_WIDGET_PAGE_SLIDE_START:  
        GUIANIM_PauseAnim(MMIGRID_IMGVIEWER_ANIM0_ID + GetCurAnimCtrlIdx());
        break;
        
    case MSG_WIDGET_PAGE_SLIDE_END:  
        GUIANIM_ResumeAnim(MMIGRID_IMGVIEWER_ANIM0_ID + GetCurAnimCtrlIdx());
        break;

    case MSG_WIDGET_MODIFY_RECT:  
        {
            MMI_CTRL_ID_T ctrl_id = 0;        
            GUI_RECT_T win_rect = {0};
            ctrl_id = MMIGRID_IMGVIEWER_ANIM0_ID + GetCurAnimCtrlIdx();
            win_rect = CetAnimCtrlRect(win_id); 
            //set anim ctrl rect
            GUIANIM_SetCtrlRect(ctrl_id,&win_rect);
            //set image display rect
            GUIANIM_SetDisplayRect(ctrl_id,&win_rect,FALSE);            
        }
		break;
#endif

    case MSG_GRID_ADD_WIDGET:
		
        ReadImgViewerFolder();
   	    s_imgviewer_info.is_first_create = TRUE;

        GetImageViewerWidgetImage(&s_imgviewer_info.bg_img_id, PNULL);  
#if (MMIGRID_IMGVIEWER_ANIM_MAX < 3)  
     GUIANIM_SetVisible(GetCurAnimCtrlIdx() + MMIGRID_IMGVIEWER_ANIM0_ID, TRUE, FALSE);
#endif
        break;
    
    case MSG_LOSE_FOCUS:
        //reset decode finish flag
        SCI_MEMSET(s_img_bmg_state, 0, sizeof(s_img_bmg_state));
        break;

    case MSG_GET_FOCUS:    
        for (i = 0; i < MMIGRID_IMGVIEWER_ANIM_MAX; i++)
        {
            UpdateImgAnimCtrl(win_id, i);
        }
        break;

	case MSG_APP_UP:
                HandleImgViewer(win_id,TRUE);			
        	break;
        	
	case MSG_APP_DOWN:
                HandleImgViewer(win_id,FALSE);
        break;

#ifdef TOUCH_PANEL_SUPPORT
	case MSG_WIDGET_TP_PRESS_DOWN:
		recode = HandleImgViewerTpDown(win_id, param);
		break;			
		
	case MSG_WIDGET_TP_PRESS_UP:
		recode = HandleImgViewerTpUp(win_id, param);
		break;

	case MSG_WIDGET_TP_PRESS_MOVE:
	    recode = HandleImgViewerTpMove(win_id, param);
	    break;
#if (MMIGRID_IMGVIEWER_ANIM_MAX < 3)          
    case MSG_WIDGET_TP_PRESS_LONG_DOWN:
        GUIANIM_PauseAnim(MMIGRID_IMGVIEWER_ANIM0_ID + GetCurAnimCtrlIdx());
        recode = MMI_RESULT_FALSE;
        break;

    case MSG_WIDGET_TP_PRESS_LONG_UP:
        GUIANIM_ResumeAnim(MMIGRID_IMGVIEWER_ANIM0_ID + GetCurAnimCtrlIdx());
        break;
#endif	
#endif	


	case MSG_FULL_PAINT:
	
 	    if (s_imgviewer_info.is_first_create)
 	    {
 	        s_imgviewer_info.is_first_create = FALSE;
 	        
 	     	if (s_imgviewer_info.is_set)
 	     	{
                UpdateFolderPhoto();

                if (0 == s_imgviewer_info.files_num)
                {
                    s_imgviewer_info.is_set = FALSE;
                    s_imgviewer_info.bg_anim_idx = 0;
                    s_imgviewer_info.cur_anim_idx = 0;
                    s_imgviewer_info.is_up = 0;
                    SCI_MEMSET(s_img_bmp,0,sizeof(s_img_bmp));
                }
                else
                {
                    SetCurPhotoIdx(0);
                    s_imgviewer_info.cur_anim_idx = 0;  

#if (MMIGRID_IMGVIEWER_ANIM_MAX >= 3)                    
        		    //previous
        		    SetImgAnimData(GetPrevAnimCtrlIdx(s_imgviewer_info.cur_anim_idx), GetPrevPhotoIndex(s_imgviewer_info.cur_idx));
        		    //SetImgAnimData(GetPrevAnimCtrlIdx(GetPrevAnimCtrlIdx(s_imgviewer_info.cur_anim_idx)), GetPrevPhotoIndex(GetPrevPhotoIndex(s_imgviewer_info.cur_idx)));
       
                    //next.
                    SetImgAnimData(GetNextAnimCtrlIdx(s_imgviewer_info.cur_anim_idx), GetNextPhotoIndex(s_imgviewer_info.cur_idx));
                    //SetImgAnimData(GetNextAnimCtrlIdx(GetNextAnimCtrlIdx(s_imgviewer_info.cur_anim_idx)), GetNextPhotoIndex(GetNextPhotoIndex(s_imgviewer_info.cur_idx)));
#endif
                     //current.
        		    SetImgAnimData(GetCurAnimCtrlIdx(), s_imgviewer_info.cur_idx);
                    s_imgviewer_info.bg_anim_idx = s_imgviewer_info.cur_anim_idx;
                }              
            }
 	    }
 	    
		DisplayImageViewer(win_id);
		break;

#ifdef BAR_PHONE_WIDGET
    case MSG_WIDGET_UPDATE_SOFTKEY:
        MMIWIDGET_SoftkeySetTextId(win_id,TXT_OPEN,FALSE);
	break;
#endif        

    case MSG_CLOSE_WINDOW:
        break;
        
    case MSG_GRID_REMOVE_WIDGET:
        ClearImgViewerInfo();
        break;

    case MSG_NOTIFY_ANIM_DISPLAY_IND:
        {
            GUIANIM_DISPLAY_IND_T *anim_ind_ptr = (GUIANIM_DISPLAY_IND_T*)param;
            BOOLEAN         is_forbidden_paint = TRUE;    
            MMI_CTRL_ID_T ctrl_id = MMK_GetCtrlId(anim_ind_ptr->ctrl_handle);
            uint32 ctrl_index = ctrl_id - MMIGRID_IMGVIEWER_ANIM0_ID;
            //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER win_handlement  time = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_573_112_2_18_3_7_24_54,(uint8*)"d",SCI_GetTickCount());
            //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER win_handlement ctrl_index = %x ,result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_575_112_2_18_3_7_24_55,(uint8*)"dd", ctrl_index,   anim_ind_ptr->result);

            SCI_TRACE_LOW("MMIGRID_IMGVIEWER HandleWidgetImgViewerWinMsg file index = %d", s_imgviewer_info.cur_idx);
            if(GUIANIM_RESULT_SUCC != anim_ind_ptr->result)
            {
                break;
            }
            
#if defined QBTHEME_SUPPORT
            if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
            {
                is_forbidden_paint = MMIQBIDLE_IsForbidPaint(MMIWIDGET_MP3_ID);                     
            }
#endif
            
#ifdef MMI_GRID_IDLE_SUPPORT
            if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()) 
            {
                is_forbidden_paint = MMIGRID_IsForbidPaint(MMIWIDGET_MP3_ID);
            }
#endif
            
            if(is_forbidden_paint)
            {
                return recode;
            }                
            
            GUIANIM_GetAnimDisplayBitmap(ctrl_id, &s_img_bmp[ctrl_index]);            
            
            s_img_bmg_state[ctrl_index] = 1; //decoding finished, buffer is ready

#if (MMIGRID_IMGVIEWER_ANIM_MAX >= 3)                    

            if (ctrl_index == s_imgviewer_info.bg_anim_idx)
            {
                //DisplayImageViewer(win_id);
                GUI_LCD_DEV_INFO dev_info = {0};
                MMK_GetWinLcdDevInfo(win_id, &dev_info);
				if(s_imgviewer_info.is_set)
					  DisplayImage(win_id, ctrl_index, &dev_info);
            }
            if (ctrl_index == s_imgviewer_info.mov_anim_idx)
            {
                DisplayMovingImg(win_id);
            }
#endif
        }
        break;

    case MSG_APP_WEB:
#ifdef BAR_PHONE_WIDGET
        MMIAPIPICVIEW_OpenPicViewer();
#else
        {
            FILEARRAY ret_array = MMIAPIFILEARRAY_Create();
            MMIFMM_FILTER_T img_filter = {{"*"}};
            if(s_imgviewer_info.is_set)
            {
                return recode;
            }
            if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(PNULL, 
                                      0,
                                      &img_filter,
                                      FUNC_FIND_FILE_AND_FOLDER,
                                      FALSE,
                                      PNULL,
                                      ret_array,
                                  OpenPhotoFolderCallback,
                                  TRUE))
        {
            SCI_TRACE_LOW("MMIGRID IMGVIEWER MSG_APP_WEB destroy file array = %x", ret_array);
            MMIAPIFILEARRAY_Destroy(&ret_array);
            ret_array = PNULL;
        }        
        }
#endif
        break;
    case MSG_TIMER:
        {
            GUI_POINT_T point = {0};
            MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
            MMI_MULTI_KEY_TP_MSG_PARAM_T   para = {0};
#ifdef TOUCH_PANEL_SUPPORT 
            MMK_GetLogicTPMsg(&tp_status, &point);
#endif
            para.cur_tp_point.x = point.x;
            para.cur_tp_point.y = point.y;
            
            ImgviewerRestartTpMoveTimer(win_id);
            HandleImgViewerTpMove(win_id, (DPARAM)&para);     
        }
        break;
	
	default:
		recode = MMI_RESULT_FALSE;
		break;
    }
    
    return recode;        
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayMovingImg(MMI_WIN_ID_T  win_id)
{
    GUI_RECT_T       layer_rect    = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = s_widget_application_layer;
                
    if (s_imgviewer_info.is_v_slide)
    {
        if (!UILAYER_IsLayerActive(&lcd_dev_info) 
            || UILAYER_GetHandleType(&lcd_dev_info) != UILAYER_HANDLE_MULTI)
        {
            return;
        }
        //anim_rect.top += s_imgviewer_info.cur_point.y - s_imgviewer_info.old_point.y;
        layer_rect = UILAYER_GetLayerRect(&lcd_dev_info);
        
        UILAYER_Clear(&lcd_dev_info);
		GUIRES_DisplayImg(PNULL,
                		  &layer_rect,
                		  PNULL,
                		  win_id,
                		  s_imgviewer_info.bg_img_id,
                		  &lcd_dev_info);

        DisplayImage( win_id, s_imgviewer_info.mov_anim_idx, &lcd_dev_info);
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayImageViewer(MMI_WIN_ID_T  win_id)
{
    GUI_RECT_T       win_rect     = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    GUI_POINT_T      point        = {0};
    GUI_RECT_T       anim_rect    = {0};
    uint16           img_height   = 0;
    uint16           img_width    = 0;
    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    
    if (!UILAYER_IsLayerActive(&lcd_dev_info))
    {
        return;
    }
    if(UILAYER_GetHandleType(&lcd_dev_info) == UILAYER_HANDLE_MULTI)
    {
        UILAYER_ClearRect(&lcd_dev_info, win_rect);
    }
        
		point.x = win_rect.left;
		point.y = win_rect.top;
		
		GUIRES_DisplayImg(&point,
                		  PNULL,
                		  PNULL,
                		  win_id,
                		  s_imgviewer_info.bg_img_id,
                		  &lcd_dev_info); 
#if 0   //def UPB_IDLE_SUPPORT
#ifdef MMI_SAM_THEME   
    if (MMIAPISET_GetIdleSubStyle() != MMISET_IDLE_SUBSTYLE_SAM)
#endif    
    {
        //由于是异步解码，UPB无法等待解码完成，暂时只显示加号
        GUIRES_GetImgWidthHeight( &img_width,  &img_height,  IMAGE_GRID_IMGVIEWER_ADD,  win_id);
        anim_rect = GUI_GetCenterRect(win_rect , img_width ,  img_height);
    
    
    	GUIRES_DisplayImg(PNULL,
                		  &anim_rect,
                		  PNULL,
                		  win_id,
                		  IMAGE_GRID_IMGVIEWER_ADD,
                		  &lcd_dev_info);             
#if (MMIGRID_IMGVIEWER_ANIM_MAX >= 3)                    
        DisplayImage( win_id, s_imgviewer_info.bg_anim_idx, &lcd_dev_info);
#endif
    }
#endif
    
    if (!s_imgviewer_info.is_set)
    {
        GUIRES_GetImgWidthHeight( &img_width,  &img_height,  IMAGE_GRID_IMGVIEWER_ADD,  win_id);
        anim_rect = GUI_GetCenterRect(win_rect , img_width ,  img_height);


		GUIRES_DisplayImg(PNULL,
                		  &anim_rect,
                		  PNULL,
                		  win_id,
                		  IMAGE_GRID_IMGVIEWER_ADD,
                		  &lcd_dev_info);             
    }
#if (MMIGRID_IMGVIEWER_ANIM_MAX >= 3)                    
    else //if(s_img_bmg_state[s_imgviewer_info.bg_anim_idx] == 1)
    {
        DisplayImage( win_id, s_imgviewer_info.bg_anim_idx, &lcd_dev_info);
    }
#endif
}
/*****************************************************************************/
//  Description :  update folder photo.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void UpdateFolderPhoto(void)
{
    uint32           file_num = 0;
    MMIFMM_FILTER_T  filter_info = {{"*.bmp *.png *.jpg *.jpeg *.gif *.wbmp"}};

    //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER UpdateFolderPhoto s_imgviewer_info.s_files = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_768_112_2_18_3_7_25_56,(uint8*)"d",s_imgviewer_info.s_files );
    MMIAPIFILEARRAY_Destroy(&s_imgviewer_info.s_files);
    s_imgviewer_info.s_files = MMIAPIFILEARRAY_Create();  
    //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER UpdateFolderPhoto s_imgviewer_info.s_files = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_771_112_2_18_3_7_25_57,(uint8*)"d",s_imgviewer_info.s_files );

    //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER UpdateFolderPhoto start time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_773_112_2_18_3_7_25_58,(uint8*)"d",SCI_GetTickCount());
	MMIAPIFMM_SearchFileInPath(
		s_imgviewer_info.img_folder.folder_name,
        s_imgviewer_info.img_folder.name_len,
		&filter_info,
		FALSE,
		FUNC_FIND_FILE,
		s_imgviewer_info.s_files,
		0,
		0
		);        
    //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER UpdateFolderPhoto end time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_784_112_2_18_3_7_25_59,(uint8*)"d",SCI_GetTickCount());

    file_num = MMIAPIFILEARRAY_GetArraySize(s_imgviewer_info.s_files);
    //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER UpdateFolderPhoto file_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_787_112_2_18_3_7_25_60,(uint8*)"d",file_num);
    
    MMIAPIFILEARRAY_SetBufferSize(s_imgviewer_info.s_files, 2);
    SCI_TRACE_LOW("MMIGRID_IMGVIEWER MMIVIRTUALARRAY_SetBufferSize end time = %d", SCI_GetTickCount());
    s_imgviewer_info.files_num = file_num;

}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void OpenPhotoFolderCallback(BOOLEAN is_success, FILEARRAY file_array)
{
    uint32           file_num = 0;
    FILEARRAY_DATA_T folder_data = {0};
    //FILEARRAY        files_array = PNULL;
    MMIFMM_FILTER_T  filter_info = {{"*.bmp *.png *.jpg *.jpeg *.gif *.wbmp"}};
    
    file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
    //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER OpenPhotoFolderCallback enter file_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_804_112_2_18_3_7_25_61,(uint8*)"d",file_num);
    
    if(is_success && 0 < file_num)
    {
        if(!MMIAPIFILEARRAY_Read(file_array, 0, &folder_data))
        {        
            //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER UpdateFolderPhoto MMIAPIFILEARRAY_Read failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_810_112_2_18_3_7_25_62,(uint8*)"");
            SCI_TRACE_LOW("MMIGRID IMGVIEWER OpenPhotoFolderCallback destroy file array = %x", file_array);
            MMIAPIFILEARRAY_Destroy(&file_array);
            return;
        }

        SCI_MEMSET(s_imgviewer_info.img_folder.folder_name, 0, (sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN+1)));
        
        MMI_WSTRNCPY(s_imgviewer_info.img_folder.folder_name, 
                     MMIFILE_FULL_PATH_MAX_LEN,
                     folder_data.filename,
                     folder_data.name_len,
                     folder_data.name_len);

        s_imgviewer_info.img_folder.name_len = folder_data.name_len;        
        s_imgviewer_info.is_set              = TRUE; 

        MMINV_WRITE(MMI_GRID_IMGVIEWER_FOLDER, &s_imgviewer_info.img_folder);
         
        //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER UpdateFolderPhoto s_imgviewer_info.s_files = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_828_112_2_18_3_7_25_63,(uint8*)"d",s_imgviewer_info.s_files );
        MMIAPIFILEARRAY_Destroy(&s_imgviewer_info.s_files);
        s_imgviewer_info.s_files = MMIAPIFILEARRAY_Create();  
        //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER UpdateFolderPhoto s_imgviewer_info.s_files = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_831_112_2_18_3_7_25_64,(uint8*)"d",s_imgviewer_info.s_files );

        //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER MMIAPIFMM_SearchFileInPath start time = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_833_112_2_18_3_7_25_65,(uint8*)"d",SCI_GetTickCount());

		MMIFMM_SearchFileInPathNoSort(
			s_imgviewer_info.img_folder.folder_name,
            s_imgviewer_info.img_folder.name_len,
			&filter_info,
			FALSE,
			FUNC_FIND_FILE,
			s_imgviewer_info.s_files,
			0,
			0
			);        
        //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER MMIAPIFMM_SearchFileInPath end time = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_844_112_2_18_3_7_25_66,(uint8*)"d",SCI_GetTickCount());

        MMIAPIFILEARRAY_SetBufferSize(s_imgviewer_info.s_files, 2);
        SCI_TRACE_LOW("MMIGRID_IMGVIEWER MMIVIRTUALARRAY_SetBufferSize end time = %d", SCI_GetTickCount());

        //MMIAPIFILEARRAY_Sort(s_imgviewer_info.s_files, FILEARRAY_SORT_NAME_ASCEND);
        file_num = MMIAPIFILEARRAY_GetArraySize(s_imgviewer_info.s_files);
        //SCI_TRACE_LOW:"MMIGRID_IMGVIEWER OpenPhotoFolderCallback file_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIGRID_IMGVIEWER_847_112_2_18_3_7_25_67,(uint8*)"d",file_num);

        s_imgviewer_info.files_num = file_num;

        SetCurPhotoIdx(0);
        s_imgviewer_info.cur_anim_idx = 0;      
        
#if (MMIGRID_IMGVIEWER_ANIM_MAX >= 3)                    
		//previous
		SetImgAnimData(GetPrevAnimCtrlIdx(s_imgviewer_info.cur_anim_idx), GetPrevPhotoIndex(s_imgviewer_info.cur_idx));
		//SetImgAnimData(GetPrevAnimCtrlIdx(GetPrevAnimCtrlIdx(s_imgviewer_info.cur_anim_idx)), GetPrevPhotoIndex(GetPrevPhotoIndex(s_imgviewer_info.cur_idx)));
        
        //next.
        SetImgAnimData(GetNextAnimCtrlIdx(s_imgviewer_info.cur_anim_idx), GetNextPhotoIndex(s_imgviewer_info.cur_idx));
        //SetImgAnimData(GetNextAnimCtrlIdx(GetNextAnimCtrlIdx(s_imgviewer_info.cur_anim_idx)), GetNextPhotoIndex(GetNextPhotoIndex(s_imgviewer_info.cur_idx)));
#endif

        //current.
		SetImgAnimData(GetCurAnimCtrlIdx(), s_imgviewer_info.cur_idx);

        s_imgviewer_info.bg_anim_idx = s_imgviewer_info.cur_anim_idx;
    }

   SCI_TRACE_LOW("MMIGRID IMGVIEWER OpenPhotoFolderCallback destroy file array = %x", file_array);
   MMIAPIFILEARRAY_Destroy(&file_array);
   file_array = PNULL;
}

/*****************************************************************************/
//  Description : set current photo index.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetCurPhotoIdx(uint32 index)
{
    s_imgviewer_info.cur_idx = index;
}

/*****************************************************************************/
//  Description : go to next photo.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 GetNextPhotoIdx()
{
    if (s_imgviewer_info.cur_idx < s_imgviewer_info.files_num - 1)
    {
        return (s_imgviewer_info.cur_idx + 1);
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : go to previous photo.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 GetPrevPhotoIdx()
{
    if (0 == s_imgviewer_info.cur_idx)
    {
        return (s_imgviewer_info.files_num - 1);
    }
    else
    {
        return (s_imgviewer_info.cur_idx - 1);
    }
}

/*****************************************************************************/
//  Description : go to next photo.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 GetNextPhotoIndex(uint32 idx)
{
    if (idx < s_imgviewer_info.files_num - 1)
    {
        return (idx + 1);
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : get previous photo index.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 GetPrevPhotoIndex(uint32 idx)
{
    if (0 == idx)
    {
        return (s_imgviewer_info.files_num - 1);
    }
    else
    {
        return (idx - 1);
    }
}

/*****************************************************************************/
//  Description : go to previous photo.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 GetCurAnimCtrlIdx(void)
{
    return s_imgviewer_info.cur_anim_idx;
}

/*****************************************************************************/
//  Description : go next animation control index.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 GetNextAnimCtrlIdx(uint32 cur_idx)
{
    if (cur_idx == (MMIGRID_IMGVIEWER_ANIM_MAX - 1))
    {
        return 0;
    }
    else
    {
        return cur_idx + 1;
    }
}

/*****************************************************************************/
//  Description : go previous animation control index.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 GetPrevAnimCtrlIdx(uint32 cur_idx)
{
    if (cur_idx == 0)
    {
        return (MMIGRID_IMGVIEWER_ANIM_MAX -1);
    }
    else
    {
        return cur_idx - 1;
    }
}

/*****************************************************************************/
//  Description : go to next photo.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void Go2NextPhoto()
{
    if (s_imgviewer_info.cur_idx < s_imgviewer_info.files_num - 1)
    {    
        SetImgAnimData(0, s_imgviewer_info.cur_idx + 1);
    }
    else
    {
        SetImgAnimData(0, 0);
    }
}

/*****************************************************************************/
//  Description : go to previous photo.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void Go2PrevPhoto()
{
    if (0 == s_imgviewer_info.cur_idx)
    {
        SetImgAnimData(0, s_imgviewer_info.files_num - 1);
    }
    else
    {
        SetImgAnimData(0, s_imgviewer_info.cur_idx--);
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleImgViewerTpDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_FALSE;
    GUI_POINT_T point = {0};
    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    s_imgviewer_info.old_point = point;
    recode = MMI_RESULT_FALSE;


    return recode;
}

/*****************************************************************************/
//  Description : handle image viewer tp move action.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImgViewerTpMove(MMI_WIN_ID_T win_id, DPARAM param)
{
    int16        offsetX      = 0;
    int16        offsetY      = 0;
    uint16       img_width    = 0;
    uint16       img_height   = 0;
	MMI_RESULT_E recode       = MMI_RESULT_FALSE;
	GUI_RECT_T   win_rect     = {0};
	GUI_RECT_T   img_rect     = MMI_GRID_IMGVIEWER_IMG_RECT;    /*lint !e529*/
	GUI_POINT_T  point        = {0};
	GUI_LCD_DEV_INFO dev_info = {0};
	BOOLEAN is_forbidden_refresh = FALSE;
	
	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);
	
	//point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    s_imgviewer_info.cur_point = point;  

#if defined MMI_GRID_IDLE_SUPPORT
    if(MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
    {
        is_forbidden_refresh = MMIGRID_IsForbidPaint(MMIWIDGET_IMG_VIEWER_ID);
    }
#endif
    
#if defined QBTHEME_SUPPORT
    if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
    {
        is_forbidden_refresh = MMIQBIDLE_IsForbidPaint(MMIWIDGET_IMG_VIEWER_ID);
    }
#endif
    if(is_forbidden_refresh)
    {
        return recode;
    }
    if (!s_imgviewer_info.is_set)
    {
        return recode;
	}
    if(s_imgviewer_info.files_num == 0)
    {
        return recode;
    }
        if (   !s_imgviewer_info.is_v_slide 
            && (abs(point.y - s_imgviewer_info.old_point.y) >= 10))
        {
            s_imgviewer_info.is_v_slide = TRUE;

            GUIRES_GetImgWidthHeight(&img_width, &img_height, s_imgviewer_info.bg_img_id, win_id);
            MMK_GetWinRect(win_id, &win_rect);            

            dev_info = CreateApplicationLayer(img_width,img_height);

            ImgviewerCreateTpMoveTimer(win_id);                                               
#if (MMIGRID_IMGVIEWER_ANIM_MAX >= 3)                    
            if (point.y - s_imgviewer_info.old_point.y >= 10)
            {
                s_imgviewer_info.is_up = FALSE;
                s_imgviewer_info.bg_anim_idx  = s_imgviewer_info.cur_anim_idx;
                s_imgviewer_info.mov_anim_idx = GetPrevAnimCtrlIdx(s_imgviewer_info.cur_anim_idx);                                
                UILAYER_SetLayerPosition(&dev_info, win_rect.left , win_rect.top);

                DisplayImageViewer(win_id);
                DisplayMovingImg(win_id);  
                
                UILAYER_SetLayerPosition(&dev_info, win_rect.left , win_rect.top - (img_rect.bottom - img_rect.top));
            }
            else
#endif
            if (point.y - s_imgviewer_info.old_point.y < -10)
            {
                UILAYER_SetLayerPosition(&dev_info, win_rect.left , win_rect.top);
                s_imgviewer_info.is_up = TRUE; 
                s_imgviewer_info.bg_anim_idx  = GetNextAnimCtrlIdx(s_imgviewer_info.cur_anim_idx);
                s_imgviewer_info.mov_anim_idx = s_imgviewer_info.cur_anim_idx;

                DisplayMovingImg(win_id);  
                DisplayImageViewer(win_id);

#if (MMIGRID_IMGVIEWER_ANIM_MAX < 3)   
                SetCurPhotoIdx(GetNextPhotoIdx());
                s_imgviewer_info.cur_anim_idx = GetNextAnimCtrlIdx(s_imgviewer_info.cur_anim_idx);
                SetImgAnimData(s_imgviewer_info.cur_anim_idx,s_imgviewer_info.cur_idx );                                        
                //SetImgAnimData(GetNextAnimCtrlIdx(s_imgviewer_info.cur_anim_idx), GetNextPhotoIndex(s_imgviewer_info.cur_idx));
#endif                
            }


            return MMI_RESULT_TRUE;
        }

        if (s_imgviewer_info.is_v_slide)
        {
            
            s_imgviewer_info.cur_point = point;
            dev_info = CreateApplicationLayer(img_width,img_height);

            if (UILAYER_IsLayerActive(&dev_info))
            {
                GUI_RECT_T layer_rect = {0};
 
                MMK_GetWinRect(win_id, &win_rect);
                
                layer_rect = UILAYER_GetLayerRect(&dev_info);

                UILAYER_GetLayerPosition(&dev_info, &offsetX, &offsetY);

                offsetY += (point.y - s_imgviewer_info.old_point.y); 

                if ((offsetY + (layer_rect.bottom - layer_rect.top)) >= win_rect.bottom)
                {
                    offsetY = win_rect.bottom - (layer_rect.bottom - layer_rect.top);
                }
                
                s_imgviewer_info.old_point.y = point.y;
                UILAYER_SetLayerPosition(&dev_info, offsetX, offsetY);
                return MMI_RESULT_TRUE;
            }
        }        

	
	return recode;
	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImgViewerTpUp(MMI_WIN_ID_T win_id, DPARAM param)
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO     dev_info = s_widget_application_layer;
    GUI_POINT_T point = {0};
    GUI_RECT_T img_rect = CetAnimCtrlRect(win_id);
    uint16  img_width  = 0;
    uint16  img_height = 0;  
    
    GUIRES_GetImgWidthHeight( &img_width,  &img_height,  IMAGE_GRID_IMGVIEWER_ADD,  win_id);
    img_rect = GUI_GetCenterRect(img_rect , img_width ,  img_height);

    
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
	if (s_imgviewer_info.is_set && s_imgviewer_info.is_v_slide)
	{
       
        if (UILAYER_IsLayerActive(&dev_info))
        {
            GUI_RECT_T layer_rect = {0};
            GUI_RECT_T win_rect   = {0};

            MMK_GetWinRect(win_id, &win_rect);
            
            layer_rect = UILAYER_GetLayerRect(&dev_info);

            if (!s_imgviewer_info.is_up)
            {              
                //slide to bottom.
                int16 top_val = layer_rect.top;

                while ((top_val + (layer_rect.bottom - layer_rect.top)) <= win_rect.bottom)
                {                        
                    UILAYER_SetLayerPosition(&dev_info, layer_rect.left, top_val);
                    MMITHEME_UpdateRect();
                    top_val += MMIGRID_IMGVIEWER_ANIM_SETP_OFFSET;
                }

                UILAYER_SetLayerPosition(&dev_info, layer_rect.left, win_rect.top);
                MMITHEME_UpdateRect();

#if (MMIGRID_IMGVIEWER_ANIM_MAX >= 3)                    
                SetCurPhotoIdx(GetPrevPhotoIdx());
                s_imgviewer_info.cur_anim_idx = GetPrevAnimCtrlIdx(s_imgviewer_info.cur_anim_idx);
                //SetImgAnimData(GetPrevAnimCtrlIdx(GetPrevAnimCtrlIdx(s_imgviewer_info.cur_anim_idx)), GetPrevPhotoIndex(GetPrevPhotoIndex(s_imgviewer_info.cur_idx)));	
                SetImgAnimData(GetPrevAnimCtrlIdx(s_imgviewer_info.cur_anim_idx), GetPrevPhotoIndex(s_imgviewer_info.cur_idx));
#endif
                s_imgviewer_info.bg_anim_idx  = s_imgviewer_info.cur_anim_idx;
            }
            else
            {            
                int16 top_val = layer_rect.top; 
                
                while ((top_val + (layer_rect.bottom - layer_rect.top)) > 0)
                {
                    UILAYER_SetLayerPosition(&dev_info, layer_rect.left, top_val);
                    MMITHEME_UpdateRect();
                    top_val -= MMIGRID_IMGVIEWER_ANIM_SETP_OFFSET;                        
                }  

#if (MMIGRID_IMGVIEWER_ANIM_MAX >= 3)                    
                SetCurPhotoIdx(GetNextPhotoIdx());
                s_imgviewer_info.cur_anim_idx = GetNextAnimCtrlIdx(s_imgviewer_info.cur_anim_idx);
                //SetImgAnimData(GetNextAnimCtrlIdx(GetNextAnimCtrlIdx(s_imgviewer_info.cur_anim_idx)), GetNextPhotoIndex(GetNextPhotoIndex(s_imgviewer_info.cur_idx)));                                        
                SetImgAnimData(GetNextAnimCtrlIdx(s_imgviewer_info.cur_anim_idx), GetNextPhotoIndex(s_imgviewer_info.cur_idx));
#endif
                s_imgviewer_info.bg_anim_idx = s_imgviewer_info.cur_anim_idx;
            }	 
        }
	}
	else if(!s_imgviewer_info.is_set)
	{
        if(GUI_PointIsInRect(point, img_rect)
            && GUI_PointIsInRect(s_imgviewer_info.old_point, img_rect))
        {
            FILEARRAY ret_array = MMIAPIFILEARRAY_Create();
            MMIFMM_FILTER_T img_filter = {{"*"}};
            
            SCI_TRACE_LOW("MMIGRID IMGVIEWER HandleImgViewerTpDown create file array= %x", ret_array);
            if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(PNULL, 
                0,
                &img_filter,
                FUNC_FIND_FILE_AND_FOLDER,
                FALSE,
                PNULL,
                ret_array,
                OpenPhotoFolderCallback,
                TRUE))
            {
                SCI_TRACE_LOW("MMIGRID IMGVIEWER HandleImgViewerTpDown destroy file array = %x", ret_array);
                MMIAPIFILEARRAY_Destroy(&ret_array);
                ret_array = PNULL;
            }  
        }

	}
    
    UILAYER_RELEASELAYER(&s_widget_application_layer);
	s_imgviewer_info.is_v_slide = FALSE;
	ImgviewerStopTpMoveTimer();
	return recode;
	
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleImgViewer(MMI_WIN_ID_T win_id,  BOOLEAN is_up)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO     dev_info = {0, UILAYER_NULL_HANDLE}; 
    GUI_RECT_T win_rect   = {0};

    s_imgviewer_info.is_v_slide = TRUE;
    if (s_imgviewer_info.is_set )
    {
        uint16       img_width    = 0;
        uint16       img_height   = 0;

        GUIRES_GetImgWidthHeight(&img_width, &img_height, s_imgviewer_info.bg_img_id, win_id);
        MMK_GetWinRect(win_id, &win_rect);

        if (!is_up)
        {
            //按键不循环
            if(s_imgviewer_info.cur_idx == 0)
            {
            	 s_imgviewer_info.is_v_slide = FALSE;
               return MMI_RESULT_FALSE;
            }
             s_imgviewer_info.bg_anim_idx  = s_imgviewer_info.cur_anim_idx;
             s_imgviewer_info.mov_anim_idx = GetPrevAnimCtrlIdx(s_imgviewer_info.cur_anim_idx);  
        }
        else
        {
            //按键不循环
            if(s_imgviewer_info.cur_idx == (s_imgviewer_info.files_num - 1))
            {
            	 s_imgviewer_info.is_v_slide = FALSE;
               return MMI_RESULT_FALSE;
            }
            s_imgviewer_info.bg_anim_idx  = GetNextAnimCtrlIdx(s_imgviewer_info.cur_anim_idx);
            s_imgviewer_info.mov_anim_idx = s_imgviewer_info.cur_anim_idx;
        }

        dev_info = CreateApplicationLayer(img_width, img_height);
        UILAYER_SetLayerPosition(&dev_info, win_rect.left , win_rect.top);
        DisplayImageViewer(win_id);
        DisplayMovingImg(win_id);
        
        if (UILAYER_IsLayerActive(&dev_info))
        {
            GUI_RECT_T layer_rect = {0};
           
            layer_rect = UILAYER_GetLayerRect(&dev_info);

            
            if (!is_up)
            { 
                {
                    //slide to bottom.
                    int16 top_val = layer_rect.top -(layer_rect.bottom - layer_rect.top) ;
                    layer_rect.left = win_rect.left + ((win_rect.right - win_rect.left )- (layer_rect.right - layer_rect.left))/2;
                    
                    while ((top_val + (layer_rect.bottom - layer_rect.top)) <= (win_rect.bottom -MMIGRID_IMGVIEWER_ANIM_SETP_OFFSET))
                    {                        
                        UILAYER_SetLayerPosition(&dev_info, layer_rect.left, top_val);
                        MMITHEME_UpdateRect();
                        //SCI_Sleep(50);
                        top_val += MMIGRID_IMGVIEWER_ANIM_SETP_OFFSET;
                    }

                    UILAYER_SetLayerPosition(&dev_info, layer_rect.left, win_rect.top);
                    MMITHEME_UpdateRect();
                    SetCurPhotoIdx(GetPrevPhotoIdx());
                    s_imgviewer_info.cur_anim_idx = GetPrevAnimCtrlIdx(s_imgviewer_info.cur_anim_idx);
                    //SetImgAnimData(GetPrevAnimCtrlIdx(GetPrevAnimCtrlIdx(s_imgviewer_info.cur_anim_idx)), GetPrevPhotoIndex(GetPrevPhotoIndex(s_imgviewer_info.cur_idx)));	
                    SetImgAnimData(GetPrevAnimCtrlIdx(s_imgviewer_info.cur_anim_idx), GetPrevPhotoIndex(s_imgviewer_info.cur_idx));
                }
                s_imgviewer_info.bg_anim_idx  = s_imgviewer_info.cur_anim_idx;
            }
            else
            {            
                {
                    int16 top_val = layer_rect.top; 
                    layer_rect.left = win_rect.left + ((win_rect.right - win_rect.left )- (layer_rect.right - layer_rect.left))/2;
                    
                    while ((top_val + (layer_rect.bottom - layer_rect.top)) > 0)
                    {
                        UILAYER_SetLayerPosition(&dev_info, layer_rect.left, top_val);
                        MMITHEME_UpdateRect();
                        //SCI_Sleep(50);
                        top_val -= MMIGRID_IMGVIEWER_ANIM_SETP_OFFSET;                        
                    }  

                    SetCurPhotoIdx(GetNextPhotoIdx());
                    s_imgviewer_info.cur_anim_idx = GetNextAnimCtrlIdx(s_imgviewer_info.cur_anim_idx);
                    //SetImgAnimData(GetNextAnimCtrlIdx(GetNextAnimCtrlIdx(s_imgviewer_info.cur_anim_idx)), GetNextPhotoIndex(GetNextPhotoIndex(s_imgviewer_info.cur_idx)));                                        
                    SetImgAnimData(GetNextAnimCtrlIdx(s_imgviewer_info.cur_anim_idx), GetNextPhotoIndex(s_imgviewer_info.cur_idx));
                }
                s_imgviewer_info.bg_anim_idx = s_imgviewer_info.cur_anim_idx;
            }
        }	    

    DisplayImageViewer(win_id);
    }
    else
    {
    	recode = MMI_RESULT_FALSE;
    }
    
    UILAYER_RELEASELAYER(&s_widget_application_layer);

	s_imgviewer_info.is_v_slide = FALSE;
	return recode;	
}

#endif


