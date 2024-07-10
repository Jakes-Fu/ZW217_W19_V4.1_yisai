/*****************************************************************************
** File Name:      watch_gallery_pic.c                                       *
** Author:         longwei.qiao                                              *
** Date:           02/20/2020                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe gallery pic UI and logic    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2/2020      longwei.qiao          Creat                                   *
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
#include "mmipicview_image.h"
#include "mmidisplay_data.h"
#include "mmipicview_text.h"
#include "mmi_appmsg.h"
#include "mmicom_trace.h"
#include "watch_gallery_internal.h"
#include "guiiconlist.h"
#ifdef BAIDU_DRIVE_SUPPORT
#include "mmipicview_internal.h" 
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//position
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
#ifndef BAIDU_DRIVE_SUPPORT
#define   GALLERY_PIC_DEL_BUTTON_RECT       DP2PX_RECT(96,168,143,215)
#define   GALLERY_PIC_ANIM_RECT             DP2PX_RECT(36,36,203,203)
#else
#define   GALLERY_PIC_DEL_BUTTON_RECT       DP2PX_RECT(96,168,143,215)
#define   GALLERY_PIC_SWITCH_BUTTON_RECT    DP2PX_RECT(95,168,227,215)
#define   GALLERY_PIC_ANIM_RECT             DP2PX_RECT(0,0,240,240)
#define   GALLERY_PIC_DEL_BUTTON_WITH_CLOUD_OFFSET 60
#endif
#else
#define   GALLERY_PIC_DEL_BUTTON_RECT       {11,179,227,227}
#define   GALLERY_PIC_ANIM_RECT             {0,0,239,239}
#endif
/**--------------------------------------------------------------------------*
 **                          LOCAL FUNCTION                                  *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description  : set picture preview button ctrl layout and param
//  Paramter:
//          [In]:win_id
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL void GallerySetPicBtnParam(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T              delete_rect       = GALLERY_PIC_DEL_BUTTON_RECT;
    MMI_CTRL_ID_T           del_button_ctrlid = MMIGALLERY_PIC_DEL_BUTTON_CTRL_ID;
//#ifdef BAIDU_DRIVE_SUPPORT
    GUI_RECT_T              switch_rect       = GALLERY_PIC_SWITCH_BUTTON_RECT;
    MMI_CTRL_ID_T           switch_button_ctrlid = MMIPICVIEW_SET_BUTTON_CTRL_ID;
//#endif

    GUIBUTTON_SetRect(del_button_ctrlid, &delete_rect);

#if !defined( ADULT_WATCH_SUPPORT) && !defined(SCREEN_SHAPE_CIRCULAR) 
    {
        GUI_BG_T                bg_info           = {0};
        MMI_STRING_T            button_text       = {0};
        GUI_FONT_ALL_T          font_info         = {0};
        bg_info.bg_type = GUI_BG_IMG;
        bg_info.img_id  = res_gallery_pic_button_bg;
        /*GUIBUTTON_SetBg(del_button_ctrlid, &bg_info);

        font_info.font  = WATCH_DEFAULT_NORMAL_FONT;
        font_info.color = MMI_WHITE_COLOR;
        MMI_GetLabelTextByLang(TXT_GALLERY_DEL_BUTTON, &button_text);
        GUIBUTTON_SetTextAlign(del_button_ctrlid,ALIGN_HVMIDDLE);
        GUIBUTTON_SetText(del_button_ctrlid,button_text.wstr_ptr,button_text.wstr_len);
        GUIBUTTON_SetFont(del_button_ctrlid,&font_info);*/
    }
#else
/*#if defined (BAIDU_DRIVE_SUPPORT)
	if(MMIAPIBAIDU_IsAccountExist())
	{
		delete_rect.left -=  GALLERY_PIC_DEL_BUTTON_WITH_CLOUD_OFFSET;
		delete_rect.right -=  GALLERY_PIC_DEL_BUTTON_WITH_CLOUD_OFFSET;	
		GUIBUTTON_SetRect(del_button_ctrlid, &delete_rect);	
		GUIBUTTON_SetRect(switch_button_ctrlid, &switch_rect);
	}
	else
	{
		CTRLBUTTON_SetVisible(switch_button_ctrlid , FALSE , FALSE);
	}
#endif*/
	delete_rect.left = 20;
	delete_rect.right = 60;
	delete_rect.top = 20;
	delete_rect.bottom = 60;
	switch_rect.left = 180;
	switch_rect.right = 220;
	switch_rect.top = 20;
	switch_rect.bottom = 60;
	GUIBUTTON_SetRect(del_button_ctrlid, &switch_rect);	
	GUIBUTTON_SetRect(switch_button_ctrlid, &delete_rect);
	GUIBUTTON_SetRunSheen(del_button_ctrlid, FALSE);
	GUIBUTTON_SetRunSheen(switch_button_ctrlid, FALSE);
#endif
    //窗口full paint时，禁止刷新，防止重绘
    GUIBUTTON_SetForbidPaint(del_button_ctrlid, TRUE);
}

LOCAL void AnimOwnerDrawCallback(GUIANIM_OWNER_DRAW_T   *owner_draw_ptr)
{
     if(Gallery_Main_IsDisplayOsd())//在显示图片之后，button图标可在此显示出来
     {

        //if(FALSE == MMK_IsFocusWin(WATCH_GALLERY_MAIN_WIN_ID))
        if(FALSE == MMK_IsFocusWin(MMIPICVIEW_SHOW_PREVIEW_WIN_ID))
        {
            TRACE_APP_GALLERY("not focus win");
            return;
        }
#ifdef ADULT_WATCH_SUPPORT
        Gallery_Main_DrawIndicator(TRUE);
     #endif
/*#ifdef BAIDU_DRIVE_SUPPORT
	if(MMIAPIBAIDU_IsAccountExist())
	{
        GUIBUTTON_SetVisible(MMIPICVIEW_SET_BUTTON_CTRL_ID, TRUE, TRUE);
	}
#endif*/
	GUIBUTTON_SetVisible(MMIGALLERY_PIC_DEL_BUTTON_CTRL_ID, TRUE, TRUE);
	GUIBUTTON_SetVisible(MMIPICVIEW_SET_BUTTON_CTRL_ID, TRUE, TRUE);
     }
}

/*****************************************************************************/
//  Description  : set picture preview anim ctrl param
//  Paramter:
//          [In]:is_update,file_info_ptr,ctrl_id.
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL GUIANIM_RESULT_E GallerySetAnimParam(
    BOOLEAN               is_update,  //是否立即刷新
    FILEARRAY_DATA_T     *file_info_ptr,
    MMI_CTRL_ID_T         ctrl_id
)
{
    GUIANIM_RESULT_E        anim_result  = GUIANIM_RESULT_SUCC;
    FILEARRAY_DATA_T        file_info    = {0};
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_FILE_INFO_T     anim_info    = {0};
    GUIANIM_CALL_BACK_T     call_back    = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    {
        //set anim min period
        GUIANIM_SetPeriod(MMITHEME_ANIM_MIN_PERIOD, ctrl_id);

        //set anim param
        control_info.is_ctrl_id        = TRUE;
        control_info.ctrl_id           = ctrl_id;
        display_info.is_zoom           = TRUE;
        display_info.is_syn_decode     = FALSE;
        display_info.is_auto_zoom_in   = TRUE;

        display_info.is_disp_one_frame = FALSE;

        display_info.align_style    = GUIANIM_ALIGN_HVMIDDLE;
        display_info.is_update      = is_update;
        display_info.bg.bg_type     = GUI_BG_COLOR;

        //背景色设为黑色
        display_info.bg.color       = MMI_BLACK_COLOR;
        display_info.is_no_miniatur = TRUE;
		{
			anim_info.full_path_wstr_ptr = file_info_ptr->filename;
			anim_info.full_path_wstr_len = file_info_ptr->name_len;
			anim_result = GUIANIM_SetParam(&control_info, PNULL, &anim_info, &display_info);
		}
        call_back.OwnerDrawFunc = AnimOwnerDrawCallback;
        GUIANIM_SetOwnerDrawEx(ctrl_id, &call_back);
    }
    TRACE_APP_GALLERY("GallerySetAnimParam result is %d",anim_result);
    return (anim_result);
}

/*****************************************************************************/
//  Description : set pic button is visible
//  Paramter:
//          [In]:win_id,is_visible,is_update
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:当需要隐藏button时，full paint可以刷新anim控件达到清除button图标的效果
/*****************************************************************************/
PUBLIC void Gallery_Pic_SetBtnVisible(MMI_WIN_ID_T win_id,BOOLEAN is_visible,BOOLEAN is_update)
{
    MMI_CTRL_ID_T   DelCtrlId = MMIGALLERY_PIC_DEL_BUTTON_CTRL_ID;
//#ifdef BAIDU_DRIVE_SUPPORT
    MMI_CTRL_ID_T   switch_button_ctrlid = MMIPICVIEW_SET_BUTTON_CTRL_ID;
//#endif	
#ifdef ADULT_WATCH_SUPPORT
    Gallery_Main_DrawIndicator(is_visible);
#endif
    GUIBUTTON_SetVisible(DelCtrlId, is_visible, is_update);
/*#ifdef BAIDU_DRIVE_SUPPORT
	if(MMIAPIBAIDU_IsAccountExist())
	{
	GUIBUTTON_SetVisible(switch_button_ctrlid, is_visible, is_update);
	}
#endif*/
	GUIBUTTON_SetVisible(switch_button_ctrlid, is_visible, is_update);
    if(!is_visible)
    {
        if(MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT,PNULL);
        }
    }
}
//add by fys for bug 10/24
uint8 delete_button_click = 0;//0:no click,1:pic click,2:main click
//add end
LOCAL void PIC_HandlePENOKMsg(MMI_WIN_ID_T  win_id,DPARAM  param)
{
    if(PNULL != param)
    {
       switch(((MMI_NOTIFY_T*) param)->src_id)
       {
           case MMIGALLERY_PIC_DEL_BUTTON_CTRL_ID:
           {
		   	  /*edit by fys 09/26 */
              /*Gallery_Main_SetDisplayOsd(win_id,FALSE);
              Gallery_Pic_SetBtnVisible(win_id,FALSE,FALSE);
              Gallery_Main_OpenDelQueryWin();*/
              /* edit end */
				MMK_CloseWin(win_id);
				//add by fys for bug 10/24
				//MMK_CloseWin(WATCH_GALLERY_MAIN_WIN_ID);
				//if(!delete_button_click){
				//	delete_button_click = 1;
				//	WatchGallery_MainWin_Enter();
				//}
				//add end
           }
           break;

		   case MMIPICVIEW_SET_BUTTON_CTRL_ID:
		   {
			   	MMI_CTRL_ID_T ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
            #ifdef BAIDU_DRIVE_SUPPORT
				MMIAPIPICVIEW_SetShowFolderPath(PICVIEW_SOURCE_CLOUD_THUMBNAIL);
            #endif
				/*edit by fys 09/26 */
				//MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);	
				MMK_CloseWin(win_id);

				//add by fys for bug 10/24
				//if(MMK_IsOpenWin(WATCH_GALLERY_MAIN_WIN_ID)){
				//	GalleryPicListUnmarked(MMIPICVIEW_LIST_CTRL_ID);
				//	Gallery_Main_ReLoadFile();
				//	MMK_SendMsg(WATCH_GALLERY_MAIN_WIN_ID, MSG_FULL_PAINT, PNULL);
				//}
				//add end
		   }
           break;
           default:
           break;
       }
    }
}
LOCAL void PIC_HandleWebKeyMsg(MMI_WIN_ID_T  win_id)
{
     if(Gallery_Main_IsDisplayOsd())
     {
         Gallery_Main_SetDisplayOsd(win_id,FALSE);
         Gallery_Pic_SetBtnVisible(win_id,FALSE,FALSE);
         Gallery_Main_OpenDelQueryWin();
     }
     else
     {
         Gallery_Main_SetDisplayOsd(win_id,TRUE);
         Gallery_Pic_SetBtnVisible(win_id,TRUE,TRUE);
     }
}
/*****************************************************************************/
//  Description : handle gallery pic msg
//  Paramter:
//          [In]:win_id,msg_id,param
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E Gallery_Pic_HandleMsg(
                                                 MMI_WIN_ID_T        win_id,
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                               )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        {
            PIC_HandleWebKeyMsg(win_id);
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
        case MSG_NOTIFY_BUTTON_UP:
        {
            PIC_HandlePENOKMsg(win_id,param);
        }
        break;
#endif
        case MSG_LOSE_FOCUS:
        {
             MMIDEFAULT_EnableKeyRing(GALLERY_MODULE_NAME, TRUE);//1352322
             Gallery_Main_SetDisplayOsd(win_id,FALSE);
        }
        break;
        case MSG_GET_FOCUS:
        {
             MMIDEFAULT_EnableKeyRing(GALLERY_MODULE_NAME, FALSE);//1352322
             Gallery_Main_SetDisplayOsd(win_id,TRUE);//刷新anim控件时，需要把button图标刷新出来
        }
        break;
        case MSG_CTL_ANIM_DISPLAY_IND://1352589
        {
            if(param != PNULL)
            {
                GUIANIM_DISPLAY_IND_T anim_display_ind = *(GUIANIM_DISPLAY_IND_T *)param;
                if(anim_display_ind.result != GUIANIM_RESULT_SUCC)
                {
                    MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
                }
                TRACE_APP_GALLERY("GallerySetAnimParam is %d",anim_display_ind.result);
            }
        }
        break;
        case MSG_APP_OK:
        {
             if(Gallery_Main_IsDisplayOsd())
             {
                 Gallery_Main_SetDisplayOsd(win_id,FALSE);
                 Gallery_Pic_SetBtnVisible(win_id,FALSE,FALSE);
             }
        }
        break;
#if defined( ADULT_WATCH_SUPPORT) || defined(SCREEN_SHAPE_CIRCULAR)
        case MSG_FULL_PAINT:
        {
            GUI_RECT_T       bg_rect     = MMITHEME_GetFullScreenRect();
            GUI_LCD_DEV_INFO lcd_dev_info = {0,0};
            //MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
            LCD_FillRect(&lcd_dev_info, bg_rect,    MMI_BLACK_COLOR);
        }
        break;
#endif
        case MSG_APP_LEFT:
        case MSG_APP_RIGHT:
        {
             /*if(!Gallery_Main_IsDisplayOsd())
             {
                 Gallery_Main_SetDisplayOsd(win_id,TRUE);
                 Gallery_Pic_SetBtnVisible(win_id,TRUE,TRUE);
             }*/
        }
        break;
        case MSG_APP_CANCEL:
        case MSG_KEYUP_RED:
        {
            
            GUICTRL_STATE_T state = 0;
            BOOLEAN is_visible = FALSE;
            state = GUICTRL_STATE_INVISIBLE;
            is_visible = GUIAPICTRL_GetState(MMIGALLERY_PIC_ANIM_CTRL_ID, state);
            is_visible = !is_visible;        
            MMK_CloseWin(win_id);
            TRACE_APP_GALLERY("app cancen is_visible:%d",is_visible);
            if(is_visible)
            {
               WatchGallery_MainWin_Enter();
            }
        }
        break;
        case MSG_KEYDOWN_RED:
            break;
        case MSG_TP_PRESS_UP:
        {
            MMI_TP_FLING_DIRECTION_E fling_direction=((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->tp_fling_direction;
            if(fling_direction == TP_FLING_LEFT)
            {
                 Gallery_NextFilePreview(win_id);
            }
            break;
        }
        default:
        {
             result = MMI_RESULT_FALSE;
        }
        break;
    }
    return result;
}

//ctrl table of preview pic file
WINDOW_TABLE(WATCH_GALLERY_CTRL_TAB_PIC) =
{
	WIN_ID(MMIPICVIEW_SHOW_PREVIEW_WIN_ID),
	WIN_FUNC((uint32)Gallery_Pic_HandleMsg),
	WIN_HIDE_STATUS,
    CREATE_ANIM_CTRL(MMIGALLERY_PIC_ANIM_CTRL_ID, MMIPICVIEW_SHOW_PREVIEW_WIN_ID),
    //CREATE_BUTTON_CTRL(res_zte_gallery_set, MMIGALLERY_PIC_DEL_BUTTON_CTRL_ID),
    CREATE_BUTTON_CTRL(res_zte_gallery_back, MMIPICVIEW_SET_BUTTON_CTRL_ID),
    //WIN_SOFTKEY(TXT_NULL, STXT_OK, STXT_RETURN),
    END_WIN
};
/*****************************************************************************/
//  Description : display pic file preview
//  Paramter:
//          [In]:win_id,file_info
//          [Out]:None
//          [Return]:None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
/*PUBLIC void Gallery_Pic_PictureFilePreview(MMI_WIN_ID_T win_id,FILEARRAY_DATA_T* file_info)
{
    MMI_CTRL_ID_T          animCtrlId  = MMIGALLERY_PIC_ANIM_CTRL_ID;
    GUI_RECT_T             animRect    = {0, 0, 240, 240};//GALLERY_PIC_ANIM_RECT;
    MMK_CreateCtrlByWinTabEx( win_id,(uint32 *)WATCH_GALLERY_CTRL_TAB_PIC);
    GUIAPICTRL_SetRect(animCtrlId, &animRect);
    GallerySetPicBtnParam(win_id);
    GallerySetAnimParam(FALSE,file_info, animCtrlId);
    GUIANIM_SetDefaultIcon(animCtrlId,NULL,NULL);//bug1526520
}*/
/*edit by fys 09/26 */
PUBLIC void Gallery_Pic_PictureFilePreview(MMI_WIN_ID_T win_id,FILEARRAY_DATA_T* file_info)
{
	MMI_HANDLE_T win_handle = 0;
	MMI_CTRL_ID_T animCtrlId  = MMIGALLERY_PIC_ANIM_CTRL_ID;
	GUI_RECT_T rect = {0,0,MMI_MAINSCREEN_WIDTH,MMI_MAINSCREEN_HEIGHT};
	win_id = MMIPICVIEW_SHOW_PREVIEW_WIN_ID;
	if (MMK_IsOpenWin(win_id))
	{
		MMK_CloseWin(win_id);
	}

	win_handle = MMK_CreateWin((uint32*)WATCH_GALLERY_CTRL_TAB_PIC, PNULL);
	MMK_SetWinRect(win_handle, &rect);
	
	GUIAPICTRL_SetRect(animCtrlId, &rect);
    GallerySetPicBtnParam(win_id);
    GallerySetAnimParam(FALSE,file_info, animCtrlId);
    GUIANIM_SetDefaultIcon(animCtrlId,NULL,NULL);
}

#endif

