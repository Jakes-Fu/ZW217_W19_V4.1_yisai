/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004      maryxiao       Create
******************************************************************************/

#define _MMISET_SCREENSAVER_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmiset_id.h"
//#include "mmiset_menutable.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmipub.h"
#include "mmiset_text.h"
#include "mmi_appmsg.h"
#include "mmiset_display.h"
#include "guires.h"
#ifdef SCREENSAVER_SUPPORT
#include "mmiss_special.h"
#endif
#include "mmiset_nv.h"
#include "mmiset_wintab.h"
#include "mmiset_internal.h"
#include "mmiudisk_export.h"
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
#include "mmilivewallpaper_export.h"
#endif
#include "guibutton.h"
#include "mmi_nv.h"
/**--------------------------------------------------------------------------*
**                function declaration                                       *
**--------------------------------------------------------------------------*/
#ifdef SCREENSAVER_SUPPORT
BOOLEAN   s_fmm_screensaver_call = FALSE;
/*****************************************************************************/
//  Description : to handle set screensave
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetScreensaverWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );
/*****************************************************************************/
//  Description : to handle screensave preview
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleScreensaverPreviewWindow(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E   msg_id, 
                                                   DPARAM             param
                                                   );



/*****************************************************************************/
//  Description : add screensaver item to radio list
//  Global resource dependence : 
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void AppendScreensaverListItem(
                                     MMI_CTRL_ID_T      ctrl_id
                                     );


/*****************************************************************************/
//  Description : update save screen list
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/

LOCAL void UpdateSaverScreenList(void);
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
// the window for setting screensavers 
WINDOW_TABLE( MMISET_SET_SCREENSAVER_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleSetScreensaverWindow ),    
        WIN_ID( MMISET_SET_SCREENSAVER_WIN_ID ),
        WIN_TITLE( TXT_SET_SCREENSAVER ),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMISET_SET_SCREENSAVER_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        END_WIN
};

// the window for preview screensavers 
WINDOW_TABLE( MMISET_SET_SCREENSAVER_PREVIEW_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleScreensaverPreviewWindow ), 
    WIN_ID( MMISET_SET_SCREENSAVER_PREVIEW_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
#endif  
    CREATE_ANIM_CTRL(MMISET_SET_SCREENSAVER_PREVIEW_CTRL_ID,MMISET_SET_SCREENSAVER_PREVIEW_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};







/*****************************************************************************/
//  Description : update save screen list
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/

LOCAL void UpdateSaverScreenList(void)
{
    MMISET_SCREENSAVER_INFO_T   ssaver_info     =   {0};
    uint16                      cur_selection   =   0;
    MMIAPISET_GetScreensaverInfo(&ssaver_info);
    if (!ssaver_info.is_open)
    {
        cur_selection = MMISET_SCREENSAVER_CLOSED;
    }
    else
    {
        cur_selection = (uint16)ssaver_info.ssaver_type;// + 1;  //Commented by xingdong.li
    }
    
    //set selected item
    GUILIST_SetSelectedItem(MMISET_SET_SCREENSAVER_CTRL_ID, cur_selection, TRUE);
    GUILIST_SetCurItemIndex(MMISET_SET_SCREENSAVER_CTRL_ID, cur_selection);
    GUILIST_SetOwnSofterKey(MMISET_SET_SCREENSAVER_CTRL_ID,TRUE);
}

/*****************************************************************************/
//  Description : to handle set screensave
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetScreensaverWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )
{
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMISET_SCREENSAVER_INFO_T   ssaver_info     =   {0};
    //MULTIM_SELECT_RETURN_T      *win_param      =   NULL;
    MMISET_SCREENSAVER_TYPE_E   scr_type        =   MMISET_SCREENSAVER_CLOSED;
    MMI_CTRL_ID_T               ctrl_id         =   MMISET_SET_SCREENSAVER_CTRL_ID; 
    //uint16                      cur_selection   =   0;
#ifdef MMI_PIC_CROP_SUPPORT
    static   BOOLEAN             is_screen_saver_crop_in    = TRUE;//crop只在开始进入，之后不在进入
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW: 
        GUILIST_SetMaxItem(
            ctrl_id, 
            MMISET_SCREENSAVER_MAX_NUM, FALSE 
            );//max item 6
        AppendScreensaverListItem(ctrl_id);        
       
        UpdateSaverScreenList();
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
#ifndef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif 
    case MSG_CTL_OK:
    case MSG_APP_OK:
        scr_type = GUILIST_GetCurItemIndex(ctrl_id);/*lint !e64*/
        if (MMISET_SCREENSAVER_CLOSED == scr_type)//关闭屏保
        {
            ssaver_info.is_open     = FALSE;
            ssaver_info.ssaver_type = MMISET_SCREENSAVER_CLOSED;
            MMIAPISET_SetScreenSaverInfo(&ssaver_info);
            UpdateSaverScreenList();
            SetScreenSaverParam();
            MMK_CloseWin(win_id);
        }
        else if (MMISET_SCREENSAVER_SPECIAL_BEGIN <= scr_type
            && MMISET_SCREENSAVER_SPECIAL_END >= scr_type 
            )
        {
            MMIAPISET_SaveCurScreenSaver((MMISET_SCREENSAVER_TYPE_E)(scr_type- MMISET_SCREENSAVER_SPECIAL_BEGIN));     /*lint !e64*//*lint !e656*/
        }
        else if (MMISET_SCREENSAVER_ANIM1 == scr_type)
        {
            ssaver_info.is_open     = TRUE;       
            ssaver_info.ssaver_type = scr_type;
            MMIAPISET_SetScreenSaverInfo(&ssaver_info);
            UpdateSaverScreenList();
            SetScreenSaverParam();
            MMK_CloseWin(MMISET_SET_SCREENSAVER_WIN_ID);
        }
#ifdef VIDEO_PLAYER_SUPPORT
        else if(MMISET_SCREENSAVER_MORE_ANIMATION == scr_type)
        {
            MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_MOVIE_ALL,0,0,0,PNULL,0,-1};
            MMIAPISET_GetScreensaverInfo(&ssaver_info);
#ifdef DRM_SUPPORT
        	select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif               
            {
                select_info.win_id = win_id;
                select_info.ticked_file_wstr = ssaver_info.more_ani_saver_info.name_wstr;
                select_info.ticked_file_len = ssaver_info.more_ani_saver_info.name_wstr_len;
                MMIAPIFMM_OpenSelectMovieWin(&select_info);
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
#endif
        else if(MMISET_SCREENSAVER_MORE_PICTURE == scr_type)
        {
            MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_PIC_ALL,0,0,0,PNULL,0,-1};
            MMIAPISET_GetScreensaverInfo(&ssaver_info);
#ifdef DRM_SUPPORT
        	select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif         
			s_fmm_screensaver_call = TRUE;
            {
                select_info.ticked_file_wstr = ssaver_info.more_pic_saver_info.name_wstr;
                select_info.ticked_file_len = ssaver_info.more_pic_saver_info.name_wstr_len;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
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

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif//TOUCH_PANEL_SUPPORT //IGNORE9527        
#endif 
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        scr_type = GUILIST_GetCurItemIndex(ctrl_id);/*lint !e64*/
#ifdef MMI_PDA_SUPPORT
        GUILIST_SetSelectedItem(ctrl_id, scr_type,TRUE);
#endif
        if (MMISET_SCREENSAVER_CLOSED == scr_type)//关闭屏保
        {
            //@xingdong.li
            ssaver_info.is_open     = FALSE;
            ssaver_info.ssaver_type = MMISET_SCREENSAVER_CLOSED;
            MMIAPISET_SetScreenSaverInfo(&ssaver_info);
            UpdateSaverScreenList();
            SetScreenSaverParam();
            MMK_CloseWin(win_id);
        }
        else if (MMISET_SCREENSAVER_SPECIAL_BEGIN <= scr_type
            && MMISET_SCREENSAVER_SPECIAL_END >= scr_type)
        {
            //算法屏保
            MMIIDLE_OpenSpeScrSaverPreviewWin(MMIAPIIDLESS_ConvertSCRSaverType(scr_type));/*lint !e656 !e64*/ 
        }
#ifdef VIDEO_PLAYER_SUPPORT
        else if(MMISET_SCREENSAVER_MORE_ANIMATION == scr_type)
        {
            MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_MOVIE_ALL,0,0,0,PNULL,0,-1};
            MMIAPISET_GetScreensaverInfo(&ssaver_info);
#ifdef DRM_SUPPORT
        	select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif               
            {
                select_info.win_id = win_id;
                select_info.ticked_file_wstr = ssaver_info.more_ani_saver_info.name_wstr;
                select_info.ticked_file_len = ssaver_info.more_ani_saver_info.name_wstr_len;
                MMIAPIFMM_OpenSelectMovieWin(&select_info);
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
#endif
        else if(MMISET_SCREENSAVER_MORE_PICTURE == scr_type)
        {
            MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_PIC_ALL,0,0,0,PNULL,0,-1};
            MMIAPISET_GetScreensaverInfo(&ssaver_info);
#ifdef DRM_SUPPORT
        	select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif               
			s_fmm_screensaver_call = TRUE;
            {
                select_info.ticked_file_wstr = ssaver_info.more_pic_saver_info.name_wstr;;
                select_info.ticked_file_len = ssaver_info.more_pic_saver_info.name_wstr_len;
                select_info.win_id = win_id;
                MMIAPIFMM_OpenSelectPictureWin(&select_info);
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
        else
        {
            //进入屏保预览窗口
            MMK_CreateWin((uint32*)MMISET_SET_SCREENSAVER_PREVIEW_WIN_TAB,(ADD_DATA)scr_type);       
        }
        break;
     case MSG_MULTIM_SELECTED_RETURN: //@xingdong.li
        {
            MULTIM_SELECT_RETURN_T      *win_param      =   NULL;

    
            //scr_sel = GUILIST_GetCurItemIndex(MMISET_SELECT_MORE_SCREENSAVER_CTRL_ID);/*lint !e64*/
            //ssaver_info.ssaver_more_select = scr_sel;
             win_param = (MULTIM_SELECT_RETURN_T *)param;
   
            ssaver_info.is_open     = TRUE;
            if(win_param->file_type == MMIFMM_FILE_TYPE_PICTURE)
            {
                ssaver_info.more_pic_saver_info.name_wstr_len = (uint16)MIN(
                win_param->file_name_len,
                MMISET_SCREENSAVER_NAME_MAX_LEN
                );

                MMI_WSTRNCPY(
                ssaver_info.more_pic_saver_info.name_wstr, 
                MMISET_SCREENSAVER_NAME_MAX_LEN,
                win_param->file_name_ptr, 
                win_param->file_name_len,
                ssaver_info.more_pic_saver_info.name_wstr_len
                );
                ssaver_info.ssaver_type = MMISET_SCREENSAVER_MORE_PICTURE;
#ifdef MMI_PIC_CROP_SUPPORT
				if(is_screen_saver_crop_in)//crop_support return
				{
					is_screen_saver_crop_in = FALSE;
#if defined MMI_WALLPAPER_SUPPORT
					MMIAPIFMM_SetPictureToPowerAndSaverEx(win_param->file_name_ptr, win_param->file_name_len);
#endif
				}
#endif 
			}
#ifdef VIDEO_PLAYER_SUPPORT
            else if(win_param->file_type == MMIFMM_FILE_TYPE_MOVIE)
            {
                ssaver_info.more_ani_saver_info.name_wstr_len = (uint16)MIN(
                win_param->file_name_len,
                MMISET_SCREENSAVER_NAME_MAX_LEN
                );

                MMI_WSTRNCPY(
                ssaver_info.more_ani_saver_info.name_wstr, 
                MMISET_SCREENSAVER_NAME_MAX_LEN,
                win_param->file_name_ptr, 
                win_param->file_name_len,
                ssaver_info.more_ani_saver_info.name_wstr_len
                );
                ssaver_info.ssaver_type = MMISET_SCREENSAVER_MORE_ANIMATION;
            }
 #endif             
            MMIAPISET_SetScreenSaverInfo(&ssaver_info);
            //AppendScreensaverListItem(MMISET_SET_SCREENSAVER_CTRL_ID);
            UpdateSaverScreenList();
            /* Modify by michael on 2/23/2012 for  NEWMS00171611: 删除"成功"提示框*/
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            /************************Modify over*************************/

            SetScreenSaverParam();

            MMK_CloseWin(win_id);
        }
        break;       
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
	case MSG_CLOSE_WINDOW:
        s_fmm_screensaver_call = FALSE;
#ifdef MMI_PIC_CROP_SUPPORT
        is_screen_saver_crop_in    = TRUE;//crop只在开始进入，之后不在进入
#endif
		break;   
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
//  Description : to handle screensave preview
//  Global resource dependence : 
//  Author:xiangjie
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleScreensaverPreviewWindow(
                                                   MMI_WIN_ID_T    win_id, 
                                                   MMI_MESSAGE_ID_E   msg_id, 
                                                   DPARAM             param
                                                   )
{
    static uint32               sel_item_index  =   0;
    MMI_RESULT_E                recode          =   MMI_RESULT_TRUE;
    MMISET_SCREENSAVER_INFO_T   ssaver_info     =   {0};      //@xingdong.li
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (PNULL == param)
        {
            sel_item_index = MMISET_SCREENSAVER_SEA_WORLD;
        }
        else
        {
            sel_item_index = (uint32)MMK_GetWinAddDataPtr(win_id);
        }
        //SCI_ASSERT( (MMISET_SCREENSAVER_MAX_NUM > sel_item_index)&&(0 < sel_item_index));
    	if(MMISET_SCREENSAVER_MAX_NUM <= sel_item_index)
    	{
    	    //SCI_TRACE_LOW:"mmisetting_assert HandleScreensaverPreviewWindow sel_item_index %d"
    	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_SCREENSAVER_422_112_2_18_2_51_49_309,(uint8*)"d", sel_item_index);
    	    return MMI_RESULT_FALSE;
    	} 
    	
#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT 
        MMIAPISET_CreatSetButton(win_id,MMISET_SET_SCREENSAVER_PREVIEW_BUTTON_CTRL_ID, (GUIBUTTON_CALLBACK_FUNC)MMIAPISET_SetOkButtonCallback,(GUIBUTTON_CALLBACK_FUNC)MMIAPISET_SetCancelButtonCallback);
        MMIAPISET_CreateSetButtonLayer(win_id,MMISET_SET_SCREENSAVER_PREVIEW_BUTTON_CTRL_ID);
#endif
        MMK_SetAtvCtrl(win_id,MMISET_SET_SCREENSAVER_PREVIEW_CTRL_ID);  
        //set softkey
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK,TXT_NULL,STXT_RETURN,FALSE);
        break;
        
    case MSG_FULL_PAINT:
        // 设置动画属性数据
        {
            GUIANIM_CTRL_INFO_T     control_info    =   {0};
            GUIANIM_DATA_INFO_T     data_info       =   {0};
            GUIANIM_DISPLAY_INFO_T  display_info    =   {0};
            
            control_info.is_ctrl_id = TRUE;
            control_info.ctrl_id = MMISET_SET_SCREENSAVER_PREVIEW_CTRL_ID;
            
            data_info.img_id = IMAGE_SCREEN_SAVER_1;
            
            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
            
            GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
#ifdef MMI_PDA_PREVIEW_BUTTON_SUPPORT
            MMIAPISET_AppendSetButtonLayer();
#endif
        }
        break;
    case MSG_LOSE_FOCUS:
    case MSG_BACKLIGHT_TURN_OFF:
         MMK_CloseWin(win_id);
         break;
    case MSG_APP_LEFT:        
        if (MMISET_SCREENSAVER_SEA_WORLD == sel_item_index)
        {
            sel_item_index = MMISET_SCREENSAVER_SPECIAL_END;
        }
        else
        {
            sel_item_index --;
        }  
        
        if (MMISET_SCREENSAVER_SPECIAL_BEGIN <= sel_item_index
            && MMISET_SCREENSAVER_SPECIAL_END >= sel_item_index)
        {            
            //算法屏保
            MMIIDLE_OpenSpeScrSaverPreviewWin(MMIAPIIDLESS_ConvertSCRSaverType(sel_item_index));/*lint !e64*/
            MMK_CloseWin(win_id);
        }
        else
        {       
            MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
        }
        break;
    case MSG_APP_RIGHT:       
        if ((MMISET_SCREENSAVER_SPECIAL_END) == sel_item_index)
        {
            sel_item_index = MMISET_SCREENSAVER_SEA_WORLD;
        }
        else
        {
            sel_item_index ++;
        }      
        if (MMISET_SCREENSAVER_SPECIAL_BEGIN <= sel_item_index
            && MMISET_SCREENSAVER_SPECIAL_END >= sel_item_index)
        {           
            //算法屏保
            MMIIDLE_OpenSpeScrSaverPreviewWin(MMIAPIIDLESS_ConvertSCRSaverType(sel_item_index));/*lint !e64*/
            MMK_CloseWin(win_id);
        }
        else
        {   
            MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
        }
        break;
    case MSG_CTL_OK:    
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //SetScreensaver(MMISET_SET_SCREENSAVER_CTRL_ID);
        //@xingdong.li
        ssaver_info.is_open     = TRUE;       
        ssaver_info.ssaver_type = (MMISET_SCREENSAVER_TYPE_E) sel_item_index;
        MMIAPISET_SetScreenSaverInfo(&ssaver_info);
        UpdateSaverScreenList();
        SetScreenSaverParam();

        MMK_CloseWin(MMISET_SET_SCREENSAVER_WIN_ID);

        MMK_CloseWin( win_id );
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );  
        break;
//    case MSG_LOSE_FOCUS:
//#if defined(MMI_PDA_SUPPORT) 
//        MMIAPISET_DestroySetButtonLayer(win_id,MMISET_SET_SCREENSAVER_PREVIEW_BUTTON_CTRL_ID);
//#endif
//        break;
//    case MSG_GET_FOCUS:
//#if defined(MMI_PDA_SUPPORT) 
//        MMIAPISET_CreateSetButtonLayer(win_id,MMISET_SET_SCREENSAVER_PREVIEW_BUTTON_CTRL_ID);
//#endif
//        break;
//    case MSG_LCD_SWITCH:
//#if defined(MMI_PDA_SUPPORT) 
//        MMIAPISET_DestroySetButtonLayer(win_id,MMISET_SET_SCREENSAVER_PREVIEW_BUTTON_CTRL_ID);
//        MMIAPISET_CreateSetButtonLayer(win_id,MMISET_SET_SCREENSAVER_PREVIEW_BUTTON_CTRL_ID);
//#endif
//        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
//  Description : add screensaver item to radio list
//  Global resource dependence : 
//  Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void AppendScreensaverListItem(
                                     MMI_CTRL_ID_T      ctrl_id
                                     )
{
    uint16                      cur_selection   =   0;
    
    MMISET_SCREENSAVER_INFO_T   setting_info    =   {0};
    
#ifndef MID_SOFTKEY_USE_IMG_SUPPORT //CR166050
	uint32 mid_softkey = (MMI_TEXT_ID_T)STXT_PREVIEW;
#else           
	uint32 mid_softkey = (MMI_IMAGE_ID_T)IMAGE_COMMON_SOFTKEY_START_ICON;
#endif
    //add items to listbox
    MMIAPISET_AppendListItemByTextIdExt(TXT_CLOSE,STXT_OK, TXT_NULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    //MMIAPISET_AppendListItemByTextId(TXT_SET_SCREENSAVER_MATCH,STXT_PREVIEW, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_SCREENSAVER_SEA,TXT_COMMON_OK, mid_softkey, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);  
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_SCREENSAVER_STAR,TXT_COMMON_OK, mid_softkey, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);    
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_SCREENSAVER_SNAKE,TXT_COMMON_OK, mid_softkey, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);    
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_SCREENSAVER_SEED,TXT_COMMON_OK, mid_softkey, STXT_RETURN, ctrl_id,  GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_SCREENSAVER_PICTURE, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO); 
    MMIAPISET_AppendListItemByTextIdExt(TXT_SET_SCREENSAVER_ANIMATION, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO); 

    //MMIAPISET_AppendListItemByTextIdExt(TXT_SET_SCREENSAVER_MORE, STXT_OK, TXT_NULL, STXT_RETURN, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO); 

    //get current keyboard light
    MMIAPISET_GetScreensaverInfo(&setting_info);
    if (!setting_info.is_open)
    {
        cur_selection = MMISET_SCREENSAVER_CLOSED;
    }
    else
    {
        cur_selection = (uint16)setting_info.ssaver_type;// + 1;  //Commented by xingdong.li
    }
    
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}



/*****************************************************************************/
//  Description : Set ScreenSaver information
//  Global resource dependence : 
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_SetScreenSaverInfo(MMISET_SCREENSAVER_INFO_T *ssaver_info_ptr)
{
    MMINV_WRITE(MMINV_SCREENSAVER_SETTING_INFO, ssaver_info_ptr);
    SCI_TRACE_LOW(
        "MMIAPISET_SetScreenSaverInfo: file name length is %d",
        ssaver_info_ptr->more_pic_saver_info.name_wstr_len
        );
}


/*****************************************************************************/
//  Description : 保存当前选中的屏保
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SaveCurScreenSaver(MMISET_SCREENSAVER_TYPE_E  ssaver_type)
{
    MMISET_SCREENSAVER_INFO_T   ssaver_info     =   {0};
    
    ssaver_info.is_open     =   TRUE;
    ssaver_info.ssaver_type =   ssaver_type + MMISET_SCREENSAVER_SPECIAL_BEGIN;/*lint !e656*/
    MMIAPISET_SetScreenSaverInfo(&ssaver_info);
    UpdateSaverScreenList();

    SetScreenSaverParam();

    MMK_CloseWin(MMISET_SET_SCREENSAVER_WIN_ID);
}
/*****************************************************************************/
//  Description : 打开屏保预览窗口
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_OpenScreenSaverWin(uint16     select_item_id)
{
    //进入屏保预览窗口
    MMK_CreateWin((uint32*)MMISET_SET_SCREENSAVER_PREVIEW_WIN_TAB,(ADD_DATA)select_item_id);   
    
}
/*****************************************************************************/
//  Description : create screensaver window
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMISETAPI_CreateScreensaverWin(void)
{
    MMK_CreatePubListWin((uint32*)MMISET_SET_SCREENSAVER_WIN_TAB,PNULL);
}
/*****************************************************************************/
//  Description : MMIAPISET_IsScreensaverCall
//  Global resource dependence : 
//  Author:liyan.zhu
//  Note: copy MMIAPISET_IsWallpaperCall
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_IsScreensaverCall(void)
{
    return s_fmm_screensaver_call;
}
#endif


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
