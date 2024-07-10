#ifdef SEARCH_SUPPORT
#define _MMIWIDGET_SEARCH_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"

#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#ifdef MMI_WIDGET_SEARCH
#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiwidget_position.h"

//#include "mmiset_display.h"
#include "mmipub.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmisearch_export.h"
#include "mmisearch_id.h"
//#include "mmisearch_position.h"
#include "mmisearch_menutable.h"
//#include "mmipicview_text.h"
#include "mmipb_export.h"
//#include "mmi_wallpaper_export.h"
#include "mmimms_export.h"
#include "mmiudisk_export.h"
//#include "mmiset_display.h"
#include "mmifilearray_export.h"
//#include "mmifmm_export.h"

#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "mmisearch_image.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Handle Tp Press Down
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTpPressDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param ,       //IN:
                                      BOOLEAN *is_press_down_ptr
                                      );
/*****************************************************************************/
//  Description : Handle Tp Press Up
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*****************************************************************************/
//  Description : Handle Widget Search Window Msg
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetSearchWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*****************************************************************************/
//  Description : Display Search widget Panel
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/	
LOCAL void DisplaySearchPanel(MMI_WIN_ID_T win_id, BOOLEAN is_press_down);

/*****************************************************************************/
//  Description : Reset Widget Search
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL void ResetWidgetSearch(void);

/*****************************************************************************/
//  Description : get search widget image.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void GetSearchWidgetImage(MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img);

/**--------------------------------------------------------------------------*
 **                         GLOBLE DEFINITION                                *
 **--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_search_widget =
{
    MMIWIDGET_SEARCH_ID, 
    HandleWidgetSearchWinMsg, 
    PNULL, 
    ResetWidgetSearch,
    GetSearchWidgetImage,    
    WIDGET_SEARCH_WIN_ID, 
    IMAGE_WIDGET_SEARCH_SHORTCUT, 
    IMAGE_WIDGET_SEARCH_BG, 
    TXT_SEARCH,
    50,//    75, 
    50,//    75
    FALSE
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DEFINITION                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get search widget image.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void GetSearchWidgetImage(MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img)
{
    if (PNULL != bg_img)
    {
        *bg_img = IMAGE_WIDGET_SEARCH_BG;
    }

    if (PNULL != sc_img)    
    {
        *sc_img = IMAGE_WIDGET_SEARCH_SHORTCUT;
    }
    
#ifdef MMI_SAM_THEME	
    if ((MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM) && (PNULL != bg_img))
    {
        *bg_img = IMAGE_WIDGET_SAM_SEARCH_BG;
    }
#endif    
}

/*****************************************************************************/
//  Description : Get widget panel search type Rect
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL void GetTypeRect(MMI_WIN_ID_T  win_id, GUI_RECT_T *type_rect)
{
    GUI_RECT_T bg_img_rect = {0};
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    uint16 type_width = 0;
    uint16 type_height = 0;
    MMI_IMAGE_ID_T bg_img = 0;
    MMI_IMAGE_ID_T search_icon = IMAGE_SEARCH_ALL;

    GetSearchWidgetImage(&bg_img, PNULL);

    GUIRES_GetImgWidthHeight(&bg_width, &bg_height,bg_img, win_id);
    bg_img_rect.right = (bg_width - 1);
    bg_img_rect.bottom = (bg_height - 1);

#ifdef MMI_SAM_THEME	
    if ((MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM))
    {
        search_icon = IMAGE_WIDGET_SAM_SEARCH_ICON;
    }
#endif    
    GUIRES_GetImgWidthHeight(&type_width, &type_height,search_icon, win_id);
    type_rect->top = bg_img_rect.top + (bg_height - type_height)/2;
    type_rect->left = MMIWIDGET_SEARCH_TYPE_LEFT;
    type_rect->bottom = type_rect->top + type_height;
    type_rect->right = type_rect->left + type_width;    
}
/*****************************************************************************/
//  Description : Get widget panel Edit Rect
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL void GetEditRect(MMI_WIN_ID_T  win_id, GUI_RECT_T *edit_rect)
{
    GUI_RECT_T bg_img_rect = {0};
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    uint16 edit_width = 0;
    uint16 edit_height = 0; 
    uint16 edit_left = MMIWIDGET_SEARCH_EDIT_LEFT;
    MMI_IMAGE_ID_T bg_img = 0;
    MMI_IMAGE_ID_T hl_img = IMAGE_WIDGET_SEARCH_HIGHLIGHT;

#ifdef MMI_SAM_THEME	
    if ((MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM))
    {    
        hl_img    = IMAGE_WIDGET_SAM_SEARCH_HL;
        edit_left = MMIWIDGET_SAM_SEARCH_EDIT_LEFT;
    }
#endif

    GetSearchWidgetImage(&bg_img, PNULL);

    GUIRES_GetImgWidthHeight(&bg_width, &bg_height,bg_img, win_id);
    bg_img_rect.right = (bg_width - 1);
    bg_img_rect.bottom = (bg_height - 1);

    GUIRES_GetImgWidthHeight(&edit_width, &edit_height,hl_img, win_id);
    edit_rect->top = bg_img_rect.top + (bg_height - edit_height)/2 - 2;
    edit_rect->left = edit_left;
    edit_rect->bottom = edit_rect->top + edit_height - 1;
    edit_rect->right = edit_rect->left + edit_width - 1;    
}

/*****************************************************************************/
//  Description : Reset Widget Search
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL void ResetWidgetSearch(void)
{

}
/*****************************************************************************/
//  Description : Handle Widget Search Window Msg
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetSearchWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    static BOOLEAN is_press_down = FALSE;
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            is_press_down = FALSE;
            break;
			
        case MSG_WIDGET_TP_PRESS_DOWN:
            //点中显示图标按下效果
            recode = HandleTpPressDown(win_id, param, &is_press_down);
            if(is_press_down)
            {
                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            is_press_down = FALSE;
            break;
			
        case MSG_WIDGET_TP_PRESS_UP:
            recode = HandleTpPressUp(win_id, param);
            is_press_down = FALSE;
            break;
        case MSG_APP_WEB:
            MMIAPISEARCH_Entry();  
            break;	
        case MSG_FULL_PAINT:
            
            DisplaySearchPanel(win_id,is_press_down);
            
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
return recode;    	
	
}


/*****************************************************************************/
//  Description : Display Search widget Panel
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/	
LOCAL void DisplaySearchPanel(MMI_WIN_ID_T  win_id, BOOLEAN is_press_down)
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    GUI_RECT_T img_rect = {0};
    GUI_RECT_T type_img_rect = {0};
    GUI_RECT_T type_rect = {0};
    GUI_RECT_T edit_rect = {0};
    GUI_RECT_T edit_img_rect = {0};   
    MMI_IMAGE_ID_T bg_img = 0;
    MMI_IMAGE_ID_T search_icon = IMAGE_SEARCH_ALL;   
    MMI_IMAGE_ID_T hl_img = IMAGE_WIDGET_SEARCH_HIGHLIGHT;
    uint16 bg_width = 0;
    uint16 bg_height = 0;
    
    GetSearchWidgetImage(&bg_img, PNULL);
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    if (UILAYER_IsLayerActive(&lcd_dev_info))
    {
        //背景图
        GUIRES_GetImgWidthHeight(&bg_width, &bg_height,bg_img, win_id);
        img_rect.right = (bg_width - 1);
        img_rect.bottom = (bg_height - 1);

        GUIRES_DisplayImg(
            PNULL,
            &win_rect,
            &img_rect,
            win_id,
            bg_img,
            &lcd_dev_info
           );

        GetTypeRect(win_id, &type_rect);

#ifdef MMI_SAM_THEME	
        if ((MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM))
        {
            search_icon = IMAGE_WIDGET_SAM_SEARCH_ICON;
        }
#endif
        
        type_img_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, type_rect);
        GUIRES_DisplayImg(
            PNULL,
            &type_img_rect,
            PNULL,
            win_id,
            search_icon,
            &lcd_dev_info
            );

        GetEditRect(win_id, &edit_rect);
      
        if(is_press_down)
        {
#ifdef MMI_SAM_THEME	
            if ((MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM))
            {
                hl_img = IMAGE_WIDGET_SAM_SEARCH_HL;
            }
#endif  

            edit_img_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, edit_rect);
            
            GUIRES_DisplayImg(
                              PNULL,
                              &edit_img_rect,
                              PNULL,
                              win_id,
                              hl_img,
                              &lcd_dev_info
                              );    
        }      
    }
}

/*****************************************************************************/
//  Description : Handle Tp Press Down
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTpPressDown(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param,        //IN:
                                      BOOLEAN *is_press_down_ptr
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_FALSE;
    GUI_POINT_T point = {0};
    GUI_RECT_T type_rect = {0};
    GUI_RECT_T edit_rect = {0}; 

    GetTypeRect(win_id, &type_rect);
    GetEditRect(win_id, &edit_rect);

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

    if (GUI_PointIsInRect(point, type_rect))
    {

    }
    else if (GUI_PointIsInRect(point, edit_rect))
    {
        *is_press_down_ptr = TRUE;
    }
    else
    {
        recode = MMI_RESULT_FALSE;
    }
    return recode;
}
/*****************************************************************************/
//  Description : Handle Tp Press Up
//  Global resource dependence : 
//  Author: hermann liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T edit_rect = {0};
    GUI_RECT_T type_rect = {0};

    GetTypeRect(win_id, &type_rect);
    GetEditRect(win_id, &edit_rect);
   
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    if (GUI_PointIsInRect(point, edit_rect))
    {
        MMIAPISEARCH_Entry(); 
    }
    else if(GUI_PointIsInRect(point, type_rect))
    {
        MMIAPISEARCH_Entry();
        if(MMK_IsOpenWin(MMISEARCH_MAIN_WIN_ID))
        {
            MMK_PostMsg(MMISEARCH_MAIN_WIN_ID, MSG_SEARCH_OPEN_TYPE_BOX, PNULL, 0);
        }
    }
    else
    {
        recode = MMI_RESULT_FALSE;
    }
    return recode;
}
#endif
#endif
#endif
