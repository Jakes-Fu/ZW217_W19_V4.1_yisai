/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004      maryxiao       Create
******************************************************************************/

#define _MMISET_WALLPAPER_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "mmi_app_setting_trc.h"
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmiset_id.h"
//#include "mmienvset.h"
#include "mmiset_internal.h"
#include "mmipub.h"
#include "mmiset_text.h"
#include "mmi_appmsg.h"
#include "mmiset_display.h"
#include "guires.h"

#include "mmiwallpaper_image.h"
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmilivewallpaper_export.h"
#endif

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_export.h"
#include "mmiqbtheme_text.h"
#endif

#include "mmi_utility.h"

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern uint16   g_cur_wallpaper_index; //当前墙纸索引
BOOLEAN   s_fmm_wallpaper_call = FALSE;
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
LOCAL MMISET_WALLPAPER_SET_T        s_pre_wallpaper = {0};          //前墙纸的信息
#endif


//#ifdef MMI_PDA_SUPPORT

/*****************************************************************************/
//  Description : append list item NEWMS00120974
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void MMISET_AppendWallpaperSetting(MMI_CTRL_ID_T ctrl_id);


/*****************************************************************************/
// 	Description :HandleSetStaticWallpaperWinMsg
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetStaticWallpaperWinMsg(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT


/*****************************************************************************/
// 	Description : HandleSetDynamicWallpaperWinMsg
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDynamicWallpaperWinMsg(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           );  
#ifdef SLIDESHOW_WALLPAPER_SUPPORT

/*****************************************************************************/
// 	Description : 幻灯片 墙纸
//	Global resource dependence : 
//  Author: xiaohua.liu
//	Note:
/*****************************************************************************/
LOCAL void SetSlideShowWallpaperInfo();
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSlideShowFindPhotoWaitingWin(
                         MMI_WIN_ID_T       win_id,
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         );
/*****************************************************************************/
//  Description : set to filmSlide wallpaper effect
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void SetSlideShowWallpaper();
#endif
#endif
//#endif
/*****************************************************************************/
//  Description : to handle set wallpaper pre window  NEWMS00120974
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetWallpaperWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
/*****************************************************************************/
//  Description : to preview wallpaper
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePreviewWallpaperWindow(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 );                                             



// the window for wallpaper preview
WINDOW_TABLE( MMISET_SET_WALLPAPER_PREVIEW_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePreviewWallpaperWindow ),   
        WIN_ID( MMISET_SET_WALLPAPER_PREVIEW_WIN_ID ),
#if !defined(MMI_PDA_SUPPORT) 
        WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_RETURN),
#endif        
#if defined(MMI_PDA_SUPPORT)
        WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
#endif
        END_WIN
};

// the window for select fixed wallpaper
WINDOW_TABLE( MMISET_SET_WALLPAPER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetWallpaperWindow ),    
        WIN_ID( MMISET_SET_WALLPAPER_WIN_ID ),
        WIN_TITLE( TXT_COMMON_SET_WALLPAPER ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_WALLPAPER_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        END_WIN
};


//#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE( MMISET_SET_STATIC_WALLPAPER_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetStaticWallpaperWinMsg),
    WIN_ID(MMISET_SET_STATIC_WALLPAPER_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_TITLE(TXT_STATIC_WALLPAPER),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_STATIC_WALLPAPER_CTRL_ID),
    END_WIN
};

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
WINDOW_TABLE( MMISET_SET_DYNAMIC_WALLPAPER_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetDynamicWallpaperWinMsg),
    WIN_ID(MMISET_SET_DYNAMIC_WALLPAPER_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_TITLE(TXT_DYNAMIC_WALLPAPER),
#ifdef MMI_PDA_SUPPORT//NEWMS00178460
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
#endif
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_DYNAMIC_WALLPAPER_CTRL_ID),
    END_WIN
};
#endif
//#endif



/*****************************************************************************/
//  Description : interface to choose wallpaper
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/

PUBLIC void MMIAPISET_SetWallpaper(void)
{
    MMK_CreatePubListWin((uint32*)MMISET_SET_WALLPAPER_WIN_TAB,PNULL);
}

//#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : interface to choose game wallpaper
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/

PUBLIC void MMIAPISET_SetGameWallpaper(void)
{
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    MMK_CreatePubListWin((uint32*)MMISET_SET_DYNAMIC_WALLPAPER_WIN_TAB, PNULL);
#endif
}
//#endif

/*****************************************************************************/
//  Description : interface to choose film wallpaper
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/

PUBLIC void MMIAPISET_SetFilmWallpaper(void)
{
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
     MMIAPISETVWP_OpenMainWin();
#endif
}

//#ifdef MMI_PDA_SUPPORT

/*****************************************************************************/
//  Description : append list item NEWMS00120974
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void MMISET_AppendWallpaperSetting(MMI_CTRL_ID_T ctrl_id)
{
    GUIITEM_STYLE_E list_style = GUIITEM_STYLE_ONE_LINE_RADIO;
    MMI_TEXT_ID_T   txt_id = TXT_STATIC_WALLPAPER;

#ifdef QBTHEME_SUPPORT
	if (STYLE_IS_SUPPORT_QBTHEME)
	{
		txt_id = TXT_QB_THEME_WALLPAPER;
	}
#endif
    MMIAPISET_AppendListItemByTextIdExt(txt_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, ctrl_id, list_style);
    MMIAPISET_AppendListItemByTextIdExt(TXT_MORE_PICTURE, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, ctrl_id, list_style);
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
    MMIAPISET_AppendListItemByTextIdExt(TXT_DYNAMIC_WALLPAPER, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, ctrl_id, list_style);
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#if 0 /*def QBTHEME_SUPPORT*/
	if (MMISET_IDLE_QBTHEME != MMIAPISET_GetIdleStyle())
#endif
		MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_VIDEO, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, ctrl_id, list_style);
#endif
   #ifdef SLIDESHOW_WALLPAPER_SUPPORT 
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_WALLPAPER_SLIDESHOW, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, ctrl_id, list_style);
   #endif
}

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#ifdef SLIDESHOW_WALLPAPER_SUPPORT
/*****************************************************************************/
// 	Description : 幻灯片 墙纸
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
LOCAL void SetSlideShowWallpaperInfo()
{

    MMISET_WALLPAPER_SET_T      wallpaper_info  =   {0};

    SCI_MEMSET(&wallpaper_info, 0, sizeof(MMISET_WALLPAPER_SET_T));

    //get the selected item
    //sel_item_index = g_cur_wallpaper_index;

    wallpaper_info.wallpaper_type = MMISET_WALLPAPER_LIVE;
    wallpaper_info.wallpaper_id = MMI_LIVE_WALLPAPER_TYPE_SLIDESHOW;//MMIAPISET_GetDefaultWallpaperIDByStyle();

    MMIAPISET_SetWallpaperSetInfo(wallpaper_info);

}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSlideShowFindPhotoWaitingWin(
                         MMI_WIN_ID_T       win_id,
                         MMI_MESSAGE_ID_E   msg_id, 
                         DPARAM             param
                         )
{
    static FILEARRAY ret_array = PNULL;
    MMIFMM_FILTER_T img_filter = {{"*.bmp *.png *.jpg *.jpeg *.wbmp"}};        
    uint16 array_size = 0;
    uint16 find_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar  find_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    { 
        case MSG_OPEN_WINDOW:
            MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);

            ret_array = MMIAPIFILEARRAY_Create();
            if(PNULL == ret_array)
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
                    &img_filter,FALSE,FUNC_FIND_FILE,ret_array,
                MMISET_SLIDESHOW_FIND_PHOTO_WIN_ID,
                MSG_SET_SLIDESHOW_FIND_PHOTO
                );
            }
            break;
        case MSG_SET_SLIDESHOW_FIND_PHOTO:
            array_size = MMIVIRTUALARRAY_GetArraySize(ret_array);
        if(array_size>0)
        {
            SetSlideShowWallpaperInfo();
            MMIAPILIVEWALLPAPER_CreatePreviewWin();
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_SET_SLIDESHOW_NO_FILE);
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
        
        if(PNULL != ret_array)
        {
            MMIAPIFILEARRAY_Destroy(&ret_array);
            ret_array = PNULL; 
        }
       
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
        
        default:
            result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;
    }

    return result;
}

/*****************************************************************************/
//  Description : set to filmSlide wallpaper effect
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note: 
/*****************************************************************************/
LOCAL void SetSlideShowWallpaper()
{
    MMI_WIN_ID_T    waiting_win_id = MMISET_SLIDESHOW_FIND_PHOTO_WIN_ID;
    uint32          waiting_time_out = 0;

    SCI_TRACE_LOW("SetSlideShowWallpaper, entered");
    
    MMIPUB_OpenAlertWinByTextId(&waiting_time_out, TXT_COMMON_WAITING, TXT_NULL,
        IMAGE_PUBWIN_WAIT, &waiting_win_id,PNULL,MMIPUB_SOFTKEY_NONE,HandleSlideShowFindPhotoWaitingWin);
}
#endif
#endif

/*****************************************************************************/
//  Description : to handle set wallpaper pre window  NEWMS00120974
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetWallpaperWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16       cur_index = 0;
    uint16       sel_index = 0;
    uint16        cur_wallpaper_index = 0;
    MMISET_WALLPAPER_SET_T    wallpaper_info = {0};
    MMI_CTRL_ID_T ctrl_id  = MMISET_SET_WALLPAPER_CTRL_ID;
    
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
    switch(wallpaper_info.wallpaper_type) 
    {
        case MMISET_WALLPAPER_FIXED_PIC:
        case MMISET_WALLPAPER_FIXED_ANIM:
        case MMISET_WALLPAPER_ALBUM:
            cur_index = MMISET_WALLPAPER_TYPE_STATIC;
            cur_wallpaper_index = wallpaper_info.wallpaper_id;
    	    break;
        case MMISET_WALLPAPER_DOWNLOAD:
            cur_index = MMISET_WALLPAPER_TYPE_MORE_PIC;
    	    break;
        #ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
        case MMISET_WALLPAPER_LIVE:
            #ifdef SLIDESHOW_WALLPAPER_SUPPORT
            if(MMI_LIVE_WALLPAPER_TYPE_SLIDESHOW == wallpaper_info.wallpaper_id)
            {
                cur_index = MMISET_WALLPAPER_TYPE_SLIDESHOW;
            }
            else
            #endif
            {
                cur_index = MMISET_WALLPAPER_TYPE_DYNAMIC;
    	    }
            break;
            
        #endif
        #ifdef MMI_VIDEOWALLPAPER_SUPPORT    
        case MMISET_WALLPAPER_VIDEO:
            cur_index = MMISET_WALLPAPER_TYPE_MORE_VIDEO;
    	    break;
        #endif
        
        default:
            break;
    }
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_WALLPAPER_TYPE_MAX,FALSE );
        MMISET_AppendWallpaperSetting(ctrl_id); 
        MMK_SetAtvCtrl(win_id,ctrl_id);
        GUILIST_SetSelectedItem(ctrl_id, cur_index, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,cur_index);
        break;
    case MSG_FULL_PAINT:
        {
            uint16 cur_item = 0;
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            GUILIST_SetSelectedItem(ctrl_id, cur_index, TRUE);    
            GUILIST_SetCurItemIndex(ctrl_id,cur_item);
        }
        break; 
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK: 
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        sel_index = GUILIST_GetCurItemIndex(ctrl_id);
        switch(sel_index)
        {
        case MMISET_WALLPAPER_FIXED_PIC:
#ifdef QBTHEME_SUPPORT
			if (STYLE_IS_SUPPORT_QBTHEME)
			{
				MMIAPIQBTHEME_OpenWallpaperWin();
				MMK_CloseWin(win_id);
				break;
			}
			else
#endif
			{
				MMK_CreatePubListWin((uint32*)MMISET_SET_STATIC_WALLPAPER_WIN_TAB, (ADD_DATA)cur_wallpaper_index);
			}
            break;

        case MMISET_WALLPAPER_TYPE_MORE_PIC:
        {
            MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_PIC_ALL,0,0,0,PNULL,0,-1};
            MMIMULTIM_AS_WALLPAPER_INFO wallpaper_file_ptr = {0};
            MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
            //select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_PICTURE_JPG|MMIFMM_PICTURE_BMP|MMIFMM_PICTURE_WBMP|MMIFMM_PICTURE_PNG|MMIFMM_PICTURE_GIF;
#ifdef DRM_SUPPORT
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL;
#endif
            switch (wallpaper_info.wallpaper_type)
            {
            //case MMISET_WALLPAPER_ALBUM:此类型已经不用了。
            case MMISET_WALLPAPER_DOWNLOAD:
                //get wallpaper file info
                MMIAPIFMM_GetWallPaperInfo(&wallpaper_file_ptr);
                select_info.ticked_file_wstr = wallpaper_file_ptr.full_path_name;
                select_info.ticked_file_len = wallpaper_file_ptr.full_path_len;
                select_info.win_id = win_id;
                s_fmm_wallpaper_call = TRUE;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
                break;
                
            default:
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len = 0;
                select_info.win_id = win_id;
                s_fmm_wallpaper_call = TRUE;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
                break;
            }
#ifdef DRM_SUPPORT
            {
                MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT;
          		limit_value.is_rights_valid = 1;
                MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif
        }
            break;

            
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
        case MMISET_WALLPAPER_TYPE_DYNAMIC:
            MMK_CreatePubListWin((uint32*)MMISET_SET_DYNAMIC_WALLPAPER_WIN_TAB, PNULL);
            break;
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT 
        case MMISET_WALLPAPER_TYPE_MORE_VIDEO:
            MMIAPISETVWP_OpenMainWin();
            //MMK_CloseWin(win_id);
            break;
#endif
        #ifdef SLIDESHOW_WALLPAPER_SUPPORT
		case MMISET_WALLPAPER_TYPE_SLIDESHOW:
			MMIAPISET_GetWallpaperSetInfo(&s_pre_wallpaper);
            
            SetSlideShowWallpaper();
			MMK_CloseWin(win_id);
            
            break;
           #endif 
        default:
            //SCI_TRACE_LOW:"pda HandleSetWallpaperWindow unknow index"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_WALLPAPER_399_112_2_18_2_52_20_375,(uint8*)"");
            break;
        }
        break;

    case MSG_MULTIM_SELECTED_RETURN:
        { 
            MULTIM_SELECT_RETURN_T          *selected_info = (MULTIM_SELECT_RETURN_T *)param;
#ifndef MMI_PIC_CROP_SUPPORT
            MMI_STRING_T wallpaper_label_name = {0};
#endif

#ifdef MMI_PIC_CROP_SUPPORT
            MMIAPIFMM_SetPictureToWallpaperEx(FALSE,selected_info->file_name_ptr, selected_info->file_name_len);

#else
            if (MMIAPIFMM_SetPictureToWallpaper(FALSE,selected_info->file_name_ptr, selected_info->file_name_len))
            {
                wallpaper_label_name.wstr_ptr = selected_info->file_name_ptr;
                wallpaper_label_name.wstr_len = selected_info->file_name_len;
                GUILABEL_SetText(MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID, &wallpaper_label_name, FALSE);

                MMK_CloseWin(win_id);
            }
            else
            {
                //prompt fail
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
#endif
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:        
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        s_fmm_wallpaper_call = FALSE;
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT

/*****************************************************************************/
//  Description : MMISET_ResumePreLivewallpaper
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_ResumePDAPreLivewallpaper(void)
{
    MMIAPISET_SetWallpaperSetInfo(s_pre_wallpaper);
    if(MMISET_WALLPAPER_LIVE == s_pre_wallpaper.wallpaper_type)
    {
        g_cur_wallpaper_index = s_pre_wallpaper.wallpaper_id ;
        MMISET_SetLiveWallpaper(MMISET_SET_DYNAMIC_WALLPAPER_CTRL_ID,(uint32)g_cur_wallpaper_index);
    }
       
}
#endif

/*****************************************************************************/
// 	Description :HandleSetStaticWallpaperWinMsg
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetStaticWallpaperWinMsg(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )

{
    MMI_RESULT_E   recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_SET_STATIC_WALLPAPER_CTRL_ID;
    uint16        cur_wallpaper_index = (uint32)MMK_GetWinAddDataPtr(win_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMISET_WALLPAPER_LIST_FIX_END +1 ,FALSE );
        //MMIAPISET_AppendListItemByTextIdExt(TXT_EXPORT_LOC_DEFAULT,STXT_OK, TXT_NULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendFixedItems(
                                    TXT_SET_WALLPAPER_LIST_PREFIX,
                                    MMISET_WALLPAPER_FIXED_PIC_MAX_NUM,
                                    TXT_COMMON_OK,
                                    TXT_NULL,
                                    STXT_RETURN,
                                    ctrl_id,
                                    GUIITEM_STYLE_ONE_LINE_RADIO
                                    );
        MMK_SetAtvCtrl(win_id,ctrl_id);  
        GUILIST_SetSelectedItem(ctrl_id, cur_wallpaper_index, TRUE);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:   //NEWMS00179276 
    case MSG_NOTIFY_MIDSK:
        MMK_CreateWin((uint32*)MMISET_SET_WALLPAPER_PREVIEW_WIN_TAB,PNULL);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_OK:
    //case MSG_APP_WEB: //NEWMS00179276 
    case MSG_CTL_OK:
   // case MSG_NOTIFY_MIDSK:
        //设为墙纸
        GUILIST_GetSelectedItemIndex(ctrl_id,&g_cur_wallpaper_index,1);
        MMISET_SetFixedWallpaper(ctrl_id);
        if(MMK_IsOpenWin(MMISET_SET_WALLPAPER_WIN_ID))
        {
            MMK_CloseWin(MMISET_SET_WALLPAPER_WIN_ID);
        }
        MMK_CloseWin(win_id);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;       
    }
    return recode;
}
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
// 	Description : HandleSetDynamicWallpaperWinMsg
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetDynamicWallpaperWinMsg(
                                           MMI_WIN_ID_T    win_id, 
                                           MMI_MESSAGE_ID_E   msg_id, 
                                           DPARAM             param
                                           )
{
    MMISET_WALLPAPER_SET_T    wallpaper_info = {0};
    MMI_CTRL_ID_T ctrl_id  = MMISET_SET_DYNAMIC_WALLPAPER_CTRL_ID;
    MMI_RESULT_E  recode   = MMI_RESULT_TRUE;
    uint32        cur_selection = 0;
    static uint32   live_wall_paper_index_array[MMI_LIVE_WALLPAPER_TYPE_MAX] = {0};
    uint32 i = 0;
    uint32 j = 0; 
    uint32 current_select_index = 0;
    LIVE_WALLPAPER_INFO lw_info = {0};
    
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
        MMI_LivewallpaperResListInit();
#endif

        GUILIST_SetMaxItem(ctrl_id,MMI_LIVE_WALLPAPER_TYPE_MAX ,FALSE);
        
        for(i=0,j=0;i<MMI_LIVE_WALLPAPER_TYPE_MAX;i++)
        {
            SCI_MEMSET(&lw_info, 0, sizeof(LIVE_WALLPAPER_INFO));
            if(MMIAPILIVEWALLPAPER_GetAllWallpapersInfo(&lw_info,i))
            {   
                #ifdef SLIDESHOW_WALLPAPER_SUPPORT
                if(TXT_SET_WALLPAPER_SLIDESHOW!=lw_info.thumbnail_info.text_id)
                #endif
                {
                    MMIAPISET_AppendListItemByTextIdExt(lw_info.thumbnail_info.text_id,TXT_COMMON_OK, PNULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
                }
                
                if(i == MMIAPILIVEWALLPAPER_GetCurrentLiveWallPaperIndex())
                {
                    current_select_index = j;
                }
                live_wall_paper_index_array[j++] = lw_info.type;
            } 
        }
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        if(MMISET_WALLPAPER_LIVE == wallpaper_info.wallpaper_type)
        {
            GUILIST_SetCurItemIndex(ctrl_id, current_select_index);
            GUILIST_SetSelectedItem(ctrl_id, current_select_index, TRUE);
        }

#ifdef LIVE_WALLPAPER_RES_OUTSIDE_SUPPORT
        MMI_DestroyLivewallpaperResList();
#endif
        break;

    case MSG_CTL_OK:    //NEWMS00179276 
    case MSG_APP_OK:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);

        if(cur_selection < MMI_LIVE_WALLPAPER_TYPE_MAX)
        {
            cur_selection = (uint32)live_wall_paper_index_array[cur_selection];
        }
        else
        {
            cur_selection = (uint32)live_wall_paper_index_array[0];
        }
        
        MMIAPISET_GetWallpaperSetInfo(&s_pre_wallpaper);
        
        MMISET_SetLiveWallpaper(ctrl_id, (uint32)cur_selection);
        if(MMK_IsOpenWin(MMISET_SET_WALLPAPER_WIN_ID))
        {
            MMK_CloseWin(MMISET_SET_WALLPAPER_WIN_ID);
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_NOTIFY_MIDSK:
        cur_selection = GUILIST_GetCurItemIndex(ctrl_id);

        if(cur_selection < MMI_LIVE_WALLPAPER_TYPE_MAX)
        {
            cur_selection = (uint32)live_wall_paper_index_array[cur_selection];
        }
        else
        {
            cur_selection = (uint32)live_wall_paper_index_array[0];
        }
        
        MMIAPISET_GetWallpaperSetInfo(&s_pre_wallpaper);
        
        MMISET_SetLiveWallpaper(ctrl_id, (uint32)cur_selection);
        
        MMIAPILIVEWALLPAPER_CreatePreviewWin(); 
        break;
        
    case MSG_APP_CANCEL:        
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode   = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
#endif
//#else
#if 0
/*****************************************************************************/
//  Description : to handle set wallpaper
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetWallpaperWindow(
                                             MMI_WIN_ID_T    win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E                recode                  =   MMI_RESULT_TRUE;
    MMISET_WALLPAPER_SET_T      wallpaper_info          =   {0};
    MMIMULTIM_AS_WALLPAPER_INFO *wallpaper_file_ptr     =   PNULL;
    uint16                      cur_selection           =   0;
    uint16                      cur_item                =   0;
//    uint16                      type_num                =   0;
    MMI_STRING_T wallpaper_label_name = {0};
    uint32 text_id[MMISET_WALLPAPER_LIST_MAX/*MMISET_WALLPAPER_FIXED_PIC_MAX_NUM + MMISET_WALLPAPER_SPECIAL_ANIM_NUM + 1*/]=
    {
        TXT_EXPORT_LOC_DEFAULT,
#ifndef MMI_SETTING_CUTDOWN_WALLPAPER            
        TXT_SET_WALLPAPER_1,         
        TXT_SET_WALLPAPER_2,
        TXT_SET_WALLPAPER_3,
#endif
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT            
#ifdef LIVE_WALLPAPER_SUPPORT_LINEAR
        TXT_SET_WALLPAPER_LINEAR,         //线性光幕动态墙纸
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_POP
        TXT_SET_WALLPAPER_POP,         //POP动态墙纸
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_BEE
        TXT_SET_WALLPAPER_BEE,         //BEE动态墙纸
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_GALAXY
        TXT_SET_WALLPAPER_GALAXY,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SAKURA
        TXT_SET_WALLPAPER_SAKURA,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SEAWORLD
        TXT_SET_WALLPAPER_SEAWORLD,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_LIGHTGRID
        TXT_SET_WALLPAPER_LIGHTGRID,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_RIPPLE
        TXT_SET_WALLPAPER_RIPPLE,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION
        TXT_SET_WALLPAPER_COLLISION,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_LITTLECAT
        TXT_SET_WALLPAPER_LITTLECAT,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_SWEETTREE
        TXT_SET_WALLPAPER_SWEETTREE,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION_BALLOON
        TXT_SET_WALLPAPER_COLLISION_BALLOON,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_COLLISION_MENU
        TXT_SET_WALLPAPER_COLLISION_MENU,
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_WINDMILL
        TXT_SET_WALLPAPER_WINDMILL,                           //吹风车
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_RAINBOWCITY
        TXT_SET_WALLPAPER_RAINBOWCITY,                   //彩虹城市
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_PAPERTOWN
        TXT_SET_WALLPAPER_PAPERTOWN,                     //卡通小镇
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_AQUARIUM
        TXT_SET_WALLPAPER_AQUARIUM,                     //水族馆
#endif
#ifdef LIVE_WALLPAPER_SUPPORT_GIRLSWING
	TXT_SET_WALLPAPER_GIRLSWING,                //童话世界
#endif
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT            
            TXT_SET_WALLPAPER_VIDEO,
#endif  
        TXT_SET_WALLPAPER_MORE
    };

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMISET_SET_WALLPAPER_CTRL_ID,
            MMISET_WALLPAPER_LIST_MAX,//MMISET_WALLPAPER_FIXED_PIC_MAX_NUM + MMISET_WALLPAPER_SPECIAL_ANIM_NUM + 1, 
            FALSE );//max item 5
        MMISET_AppendFixedWallpaperListItem(MMISET_SET_WALLPAPER_CTRL_ID); 
        MMK_SetAtvCtrl(win_id,MMISET_SET_WALLPAPER_CTRL_ID);   
        break;
        
    case MSG_FULL_PAINT:
        cur_item = GUILIST_GetCurItemIndex(MMISET_SET_WALLPAPER_CTRL_ID);
#if !defined MMI_PDA_SUPPORT
        MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
        if (MMISET_WALLPAPER_FIXED_PIC == wallpaper_info.wallpaper_type)
        {
//            cur_selection = (uint16)(wallpaper_info.wallpaper_id - 1);
            if (wallpaper_info.wallpaper_id <= 1)
            {
                cur_selection = MMISET_WALLPAPER_LIST_DEFAULT;
            }
            else
            {
                cur_selection = (uint16)(wallpaper_info.wallpaper_id - MMISET_WALLPAPER_LIST_FIX_START);
            }
        }
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT                    
        else if (MMISET_WALLPAPER_LIVE == wallpaper_info.wallpaper_type)
        {
            cur_selection = MMISET_WALLPAPER_FIXED_PIC_MAX_NUM + wallpaper_info.wallpaper_id;
        }
#endif  
#ifdef MMI_VIDEOWALLPAPER_SUPPORT                    
        else if (MMISET_WALLPAPER_VIDEO == wallpaper_info.wallpaper_type)
        {
            cur_selection = MMISET_WALLPAPER_LIST_VIDEO;
        }
#endif        
        else
        {
            cur_selection = MMISET_WALLPAPER_FIXED_PIC_MAX_NUM + MMISET_WALLPAPER_SPECIAL_ANIM_NUM;
        }
        GUILIST_SetSelectedItem( MMISET_SET_WALLPAPER_CTRL_ID, cur_selection, TRUE ); 
#else
        GUILIST_SetSelectedItem( MMISET_SET_WALLPAPER_CTRL_ID, cur_item, TRUE ); 
#endif
        GUILIST_SetCurItemIndex(MMISET_SET_WALLPAPER_CTRL_ID,cur_item);
        GUILIST_SetOwnSofterKey(MMISET_SET_WALLPAPER_CTRL_ID,TRUE);     
        break;
#if !defined MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
        cur_selection = GUILIST_GetCurItemIndex(MMISET_SET_WALLPAPER_CTRL_ID);
        if (MMISET_WALLPAPER_LIST_MORE/*MMISET_WALLPAPER_FIXED_PIC_MAX_NUM + MMISET_WALLPAPER_SPECIAL_ANIM_NUM*/ == cur_selection) // 非固定墙纸
        {
            MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_PIC_ALL,0,0,0,PNULL,0,-1};
            MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
            //select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_PICTURE_JPG|MMIFMM_PICTURE_BMP|MMIFMM_PICTURE_WBMP|MMIFMM_PICTURE_PNG|MMIFMM_PICTURE_GIF;
#ifdef DRM_SUPPORT
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL;
#endif
            switch (wallpaper_info.wallpaper_type)
            {
            case MMISET_WALLPAPER_ALBUM:
            case MMISET_WALLPAPER_DOWNLOAD:
                //get wallpaper file info
                MMIAPIFMM_GetWallPaperInfo(wallpaper_file_ptr);
                select_info.ticked_file_wstr = wallpaper_file_ptr->full_path_name;
                select_info.ticked_file_len = wallpaper_file_ptr->full_path_len;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
                break;
                
            default:
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len = 0;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
                break;
            }
#ifdef DRM_SUPPORT
            {
                MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT;
          		limit_value.is_rights_valid = 1;
                MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif
        }
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT                    
        else if (cur_selection >= MMISET_WALLPAPER_FIXED_PIC_MAX_NUM && cur_selection < MMISET_WALLPAPER_FIXED_PIC_MAX_NUM + MMISET_WALLPAPER_SPECIAL_ANIM_NUM)
        {
            g_cur_wallpaper_index = cur_selection;
            MMISET_SetLiveWallpaper(MMISET_SET_WALLPAPER_CTRL_ID, cur_selection - MMISET_WALLPAPER_FIXED_PIC_MAX_NUM);
            MMI_GetLabelTextByLang(text_id[cur_selection], &wallpaper_label_name);  
            GUILABEL_SetText(MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID, &wallpaper_label_name, FALSE);
            MMK_CloseWin(win_id);
        }
#endif  
#ifdef MMI_VIDEOWALLPAPER_SUPPORT                    
        else if (MMISET_WALLPAPER_LIST_VIDEO == cur_selection) // video墙纸
        {
            g_cur_wallpaper_index = cur_selection;
            MMIAPISETVWP_OpenMainWin();
            MMK_CloseWin(win_id);
        }
#endif        
        else
        {
            g_cur_wallpaper_index = cur_selection;
            MMISET_SetFixedWallpaper(MMISET_SET_WALLPAPER_CTRL_ID);
            MMI_GetLabelTextByLang(text_id[cur_selection], &wallpaper_label_name);  
            GUILABEL_SetText(MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID, &wallpaper_label_name, FALSE);
            MMK_CloseWin(win_id);
        }
        
       // MMI_GetLabelTextByLang(text_id[cur_selection], &wallpaper_label_name);  
       // GUILABEL_SetText(MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID, &wallpaper_label_name, FALSE);
       // MMK_CloseWin(win_id);
       
        break;
#if defined MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        cur_selection = GUILIST_GetCurItemIndex(MMISET_SET_WALLPAPER_CTRL_ID);
        if (MMISET_WALLPAPER_LIST_MORE/*MMISET_WALLPAPER_FIXED_PIC_MAX_NUM + MMISET_WALLPAPER_SPECIAL_ANIM_NUM*/ == cur_selection) 
        {
            MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_PIC_ALL,0,0,0,PNULL,0,-1};
            MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
            //select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_PICTURE_JPG|MMIFMM_PICTURE_BMP|MMIFMM_PICTURE_WBMP|MMIFMM_PICTURE_PNG|MMIFMM_PICTURE_GIF;
			
#ifdef DRM_SUPPORT
            select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif
            switch (wallpaper_info.wallpaper_type)
            {
            case MMISET_WALLPAPER_ALBUM:
            case MMISET_WALLPAPER_DOWNLOAD:
                //get wallpaper file info
                MMIAPIFMM_GetWallPaperInfo(wallpaper_file_ptr);
                select_info.ticked_file_wstr = wallpaper_file_ptr->full_path_name;
                select_info.ticked_file_len = wallpaper_file_ptr->full_path_len;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
                break;
                
            default:
                select_info.ticked_file_wstr = PNULL;
                select_info.ticked_file_len = 0;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
                break;
            }
#ifdef DRM_SUPPORT
            {
                MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT;
            	limit_value.is_rights_valid = 1;
                MMIAPIFMM_SetSelDrmLimitValue(limit_value);
            }
#endif
        }
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT                    
        else if (cur_selection >= MMISET_WALLPAPER_FIXED_PIC_MAX_NUM && cur_selection < MMISET_WALLPAPER_FIXED_PIC_MAX_NUM + MMISET_WALLPAPER_SPECIAL_ANIM_NUM)
        {
        #if 0
            g_cur_wallpaper_index = cur_selection;
            MMISET_SetLiveWallpaper(MMISET_SET_WALLPAPER_CTRL_ID, cur_selection - MMISET_WALLPAPER_FIXED_PIC_MAX_NUM);
            MMI_GetLabelTextByLang(text_id[cur_selection], &wallpaper_label_name);  
            GUILABEL_SetText(MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID, &wallpaper_label_name, FALSE);
            MMK_CloseWin(win_id);
        #else
            MMIAPISET_GetWallpaperSetInfo(&s_pre_wallpaper);
            g_cur_wallpaper_index = cur_selection;        
            MMISET_SetLiveWallpaper(MMISET_SET_WALLPAPER_CTRL_ID, cur_selection - MMISET_WALLPAPER_FIXED_PIC_MAX_NUM);
            MMIAPILIVEWALLPAPER_CreatePreviewWin();        
        #endif
        }
#endif 
#ifdef MMI_VIDEOWALLPAPER_SUPPORT                    
        else if (MMISET_WALLPAPER_LIST_VIDEO == cur_selection) // video墙纸
        {
            g_cur_wallpaper_index = cur_selection;
            MMIAPISETVWP_OpenMainWin();
            MMK_CloseWin(win_id);
        }
#endif  
       
        else
        {
            MMK_CreateWin((uint32*)MMISET_SET_WALLPAPER_PREVIEW_WIN_TAB,PNULL);
        }
        break;

    case MSG_MULTIM_SELECTED_RETURN:
        { 
            MULTIM_SELECT_RETURN_T          *selected_info = (MULTIM_SELECT_RETURN_T *)param;

            if (MMIAPIFMM_SetPictureToWallpaper(FALSE,selected_info->file_name_ptr, selected_info->file_name_len))
            {
                wallpaper_label_name.wstr_ptr = selected_info->file_name_ptr;
                wallpaper_label_name.wstr_len = selected_info->file_name_len;
                GUILABEL_SetText(MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID, &wallpaper_label_name, FALSE);

                MMK_CloseWin(win_id);
            }
            else
            {
                //prompt fail
                MMIPUB_OpenAlertFailWin(TXT_ERROR);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#endif



/*****************************************************************************/
//  Description : to preview wallpaper
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePreviewWallpaperWindow(
                                                 MMI_WIN_ID_T    win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                                 )
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;
    GUI_POINT_T         dis_point       =   {0};
    GUI_LCD_DEV_INFO    lcd_dev_info    =   {GUI_LCD_0, GUI_BLOCK_0};
    MMI_STRING_T        wallpaper_label_name = {0};
    MMI_CTRL_ID_T       ctrl_id    =  MMISET_SET_STATIC_WALLPAPER_CTRL_ID;

    //只有固定图片可以preview，所以只要四项txt就可以了
    uint32 text_id[MMISET_WALLPAPER_FIXED_PIC_MAX_NUM]=
    {
        TXT_EXPORT_LOC_DEFAULT,
#ifndef MMI_SETTING_CUTDOWN_WALLPAPER            
        TXT_SET_WALLPAPER_1,        
        TXT_SET_WALLPAPER_2,
        TXT_SET_WALLPAPER_3, 
#endif       
    };

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //get the selected item
        g_cur_wallpaper_index = GUILIST_GetCurItemIndex(ctrl_id);
#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT
        MMIAPISET_CreatSetButton(win_id,MMISET_SET_SCREENSAVER_PREVIEW_BUTTON_CTRL_ID, (GUIBUTTON_CALLBACK_FUNC)MMIAPISET_SetOkButtonCallback,(GUIBUTTON_CALLBACK_FUNC)MMIAPISET_SetCancelButtonCallback);
        MMIAPISET_CreateSetButtonLayer(win_id,MMISET_SET_SCREENSAVER_PREVIEW_BUTTON_CTRL_ID);
#endif
    	if(MMISET_WALLPAPER_FIXED_PIC_MAX_NUM <= g_cur_wallpaper_index)
    	{
    	    //SCI_TRACE_LOW:"mmisetting_assert HandlePreviewWallpaperWindow g_cur_wallpaper_index %d"
    	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_WALLPAPER_970_112_2_18_2_52_21_376,(uint8*)"d", g_cur_wallpaper_index);
    	    break;
    	}
        break;
        
    case MSG_FULL_PAINT:
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            MMIAPISET_GetWallpaperByIndex(g_cur_wallpaper_index),
            &lcd_dev_info);
        GUIWIN_SetSoftkeyTextId(win_id, STXT_SELECT,TXT_NULL,STXT_RETURN,FALSE);
#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT
        MMIAPISET_AppendSetButtonLayer();
#endif
        break;
    case MSG_APP_LEFT:
        if (g_cur_wallpaper_index > 0)
        {
            g_cur_wallpaper_index--;
        }
        else
        {
            g_cur_wallpaper_index = MMISET_WALLPAPER_FIXED_PIC_MAX_NUM - 1;
        }
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            MMIAPISET_GetWallpaperByIndex(g_cur_wallpaper_index),
            &lcd_dev_info);
        GUIWIN_SetSoftkeyTextId(win_id, STXT_SELECT,TXT_NULL,STXT_RETURN,TRUE);
        break;
        
    case MSG_APP_RIGHT:
        if (g_cur_wallpaper_index < (MMISET_WALLPAPER_FIXED_PIC_MAX_NUM - 1))
        {
            g_cur_wallpaper_index++;
        }
        else
        {
            g_cur_wallpaper_index = 0;
        }
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            MMIAPISET_GetWallpaperByIndex(g_cur_wallpaper_index),
            &lcd_dev_info);
        GUIWIN_SetSoftkeyTextId(win_id, STXT_SELECT,TXT_NULL,STXT_RETURN,TRUE);
        break;

    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
        //设为墙纸
        MMISET_SetFixedWallpaper(ctrl_id);
        
        MMI_GetLabelTextByLang(text_id[g_cur_wallpaper_index], &wallpaper_label_name);  
        GUILABEL_SetText(MMISET_DISPLAY_WALLPAPER_LABEL_EDIT_CTRL_ID, &wallpaper_label_name, FALSE);

        if(MMK_IsOpenWin(MMISET_SET_WALLPAPER_WIN_ID))//NEWMS00179276
        {
            MMK_CloseWin(MMISET_SET_WALLPAPER_WIN_ID);
        }
        if(MMK_IsOpenWin(MMISET_SET_STATIC_WALLPAPER_WIN_ID))
        {
            MMK_CloseWin(MMISET_SET_STATIC_WALLPAPER_WIN_ID);
        }
        MMK_CloseWin( win_id );
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );  
        GUILIST_GetSelectedItemIndex(ctrl_id,&g_cur_wallpaper_index,1);    
        break;

	case MSG_NOTIFY_UPDATE:
#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT
        if(PNULL != param)
        {
            GUI_RECT_T			*display_rect_ptr = PNULL;
            display_rect_ptr = (GUI_RECT_T*)param;
            MMIAPISET_ClearButtonLayer(*display_rect_ptr);
        }
#endif
		break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}





#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT

/*****************************************************************************/
//  Description : MMISET_ResumePreLivewallpaper
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_ResumePreLivewallpaper(void)
{
    MMIAPISET_SetWallpaperSetInfo(s_pre_wallpaper);
    if(MMISET_WALLPAPER_LIVE == s_pre_wallpaper.wallpaper_type)
    {
        g_cur_wallpaper_index = s_pre_wallpaper.wallpaper_id + MMISET_WALLPAPER_FIXED_PIC_MAX_NUM ;
        MMISET_SetLiveWallpaper(MMISET_SET_WALLPAPER_CTRL_ID,0);
    }
}

#endif

/*****************************************************************************/
//  Description : Get wallpaper image id by index.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISET_GetWallpaperByIndex(uint16 index)
{
    MMI_IMAGE_ID_T image_id = IMAGE_THEME_BG;

#ifndef MMI_SETTING_CUTDOWN_WALLPAPER
    if ( index < MMISET_WALLPAPER_FIXED_PIC_MAX_NUM )
    {
        if ( 0 == index)
        {
            image_id = IMAGE_THEME_BG;
        }
        else
        {
            image_id = IMAGE_WALLPAPER_1 + index - 1;
        }
    }
#else
    if (0 != index)
    {
        image_id = IMAGE_WALLPAPER_1;
    }
#endif

    return image_id;
}
/*****************************************************************************/
//  Description : MMIAPISET_IsWallpaperCall
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsWallpaperCall(void)
{
    return s_fmm_wallpaper_call;
}



/*Edit by script, ignore 5 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
