/*****************************************************************************
** File Name:      mmidc_guiiconbar.c                                                                                 *
** Author:                                                                                                                        *
** Date:           11/12/2011                                                                                              *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.                                  *
** Description:    This file is used to camera iconbar UI                                                        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2011      robert.wang          Creat
******************************************************************************/

#define _MMIDC_GUIICONBAR_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_camera_trc.h"
#ifdef CAMERA_SUPPORT
#ifdef MMIDC_DESKTOP_ICON_BAR
#ifdef WIN32 
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "mmi_default.h"
#include "guiblock.h"
#include "guifont.h"
#include "mmi_theme.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"
#include "window_parse.h"
#include "mmi_module.h"
#include "mmi_menutable.h"
#include "guimenu.h"
#include "mmi_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_menutable.h"
#include "block_mem.h"

#include "cafcontrol.h"
#include "guisetlist.h"
#include "guilistbox.h"


#include "guiiconlist.h"
#include "mmk_tp.h"
#include "mmi_theme.h"
#include "mmi_appmsg.h"

#if !defined(MMIDC_DESKTOP_PDA)
#include "guires.h"
#include "guibutton.h"
#endif
#if defined(MMIDC_F_WORK_MODE)
#include "mmidc_workmode.h"
#endif
//other module
#include "mmiudisk_export.h"
#include "mmifmm_export.h"
#include "mmipic_edit_export.h"
#if defined(MMIDC_F_U_CAMERA)
#include "mmidc_imageprocess.h"
#include "mmidc_ninone.h"
#endif

//this module
#include "mmidc_camera_text.h"
#include "mmidc_camera_id.h"
#include "mmidc_camera_image.h"
#include "mmidc_camera_anim.h"
#include "mmidc_setting.h"
#include "mmidc_display.h"
#if !defined(MMIDC_DESKTOP_PDA)
#include "mmidc_gui.h"
#include "mmidc_option.h"
#endif
#include "mmidc_setting.h"
#include "mmidc_flow.h"
#include "mmidc_save.h"
#include "mmidc_guiiconbar.h"
#include "mmidc_option.h"
#include "mmidc_main.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
 

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**--------------------------------------------------------------------------*/
LOCAL MMIDC_GUI_DESKTOP_ICON_T  s_gui_desktop_icon = {0};
LOCAL  GUI_RECT_T icon_off_space = {ICON_OFF_LEFT, 0, ICON_OFF_RIGHT, 0};


/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                               *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//Description :  init gui desktop icon data
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitGUIDesktopIcon(void)
{
    SCI_MEMSET(&s_gui_desktop_icon, 0x00, sizeof(s_gui_desktop_icon));
    
    s_gui_desktop_icon.is_open = FALSE;
}

/*****************************************************************************/
//Description :  init icon desktop display rect list
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void InitIconDesktopDisplayRectList(void)
{
    s_gui_desktop_icon.icon_disp_number = 0;
    SCI_MEMSET(s_gui_desktop_icon.icon_disp, 0x00, sizeof(s_gui_desktop_icon.icon_disp));
}

/*****************************************************************************/
//Description :  add icon rect to icon display list
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void AppendIconDisplayRect(MMIDC_ICON_RECT_E rect_index,
                                        GUI_RECT_T  rect
                                        )
{
    int16 index = 0;

    index = s_gui_desktop_icon.icon_disp_number;
    s_gui_desktop_icon.icon_disp[index].rect_index = rect_index;
    s_gui_desktop_icon.icon_disp[index].rect = rect;
    s_gui_desktop_icon.icon_disp_number++;
}
/*****************************************************************************/
//Description :  get icon string state
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL GUISTR_STATE_T GetIconStringState(void)
{
    GUISTR_STATE_T state = GUISTR_STATE_EFFECT | GUISTR_STATE_NOT_HALF_CHAR | GUISTR_STATE_SINGLE_LINE;

    return state;
}
/*****************************************************************************/
//Description :  get icon string style
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void GetIconStringStyle(GUISTR_STYLE_T *style_ptr //out
                                  )
{
    // text style
    style_ptr->align = ALIGN_LEFT; // 这里设置为Left,因为Label的位置自有自己调整区域
    style_ptr->char_space = 1;
    style_ptr->line_space = 1;
    style_ptr->effect = FONT_EFFECT_REVERSE_EDGE;//FONT_EFFECT_CUSTOM;
    style_ptr->font = SONG_FONT_8;
    style_ptr->font_color = MMIDC_TEXT_COLOR;
    style_ptr->edge_color = MMI_BACKGROUND_COLOR; //MMI_BACKGROUND_COLOR;
    style_ptr->bg_color = MMI_BACKGROUND_COLOR;
    style_ptr->region_num = 0;
    style_ptr->region_ptr = PNULL;
}
/*****************************************************************************/
//Description :  get icon string height
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL uint16 GetIconStringHeight(GUI_FONT_T  font, //in
                                      MMI_STRING_T *str_ptr //in
                                      )
{
    GUISTR_STYLE_T style = {0};/*lint !e64*/
    GUISTR_STATE_T state = GetIconStringState();
    GUISTR_INFO_T str_info = {0};


    GetIconStringStyle(&style);
    style.font = font;

    GUISTR_GetStringInfo(&style, str_ptr, state, &str_info );

    return (str_info.height);
}

/*****************************************************************************/
//Description :  get icon string width
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL uint16 GetIconStringWidth(GUI_FONT_T font, //in
                                     MMI_STRING_T  *str_ptr //in
                                     )
{
    GUISTR_STYLE_T style = {0};/*lint !e64*/
    GUISTR_STATE_T state = GetIconStringState();

    GetIconStringStyle(&style);
    style.font = font;
    
    return (GUISTR_GetStringWidth(&style, str_ptr, state));
}

/*****************************************************************************/
//Description :  get img list and size
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void GetIconImageList(MMI_IMAGE_ID_T *img_list_ptr, //out
                                    uint32 img_list_size, //in
                                    MMIDC_SIZE_ATTRIBUTE_T  *size_list_ptr, //out
                                    uint32 size_list_size //in
                                     )
{
    MMI_IMAGE_ID_T img_list[] =
    {
    //#ifdef TOUCH_PANEL_SUPPORT 
        IMG_DC_ICON_BRIGHTNESS_BG,   //ICON_IMG_BRIGHTNESS,      
    //#else
    //    IMG_DC_ICON_NOTP_BRIGHTNESS_BG,   //ICON_IMG_BRIGHTNESS,      
    //#endif
        IMG_DC_ICON_BRIGHT_UP,            //ICON_IMG_BRIGHT_UP,       
        IMG_DC_ICON_GRAY_BRIGHT_UP,       //ICON_IMG_GRAY_BRIGHT_UP,       
        IMG_DC_ICON_FOCUS_BRIGHT_UP,      //ICON_IMG_FOCUS_BRIGHT_UP,      
        IMG_DC_ICON_BRIGHT_DOWN,           //ICON_IMG_BRIGHT_DOWN,      
        IMG_DC_ICON_GRAY_BRIGHT_DOWN,      //ICON_IMG_GRAY_BRIGHT_DOWN,      
        IMG_DC_ICON_FOCUS_BRIGHT_DOWN,     //ICON_IMG_FOCUS_BRIGHT_RIGHT,     
    //#ifdef TOUCH_PANEL_SUPPORT 
        IMG_DC_ICON_ZOOM_BG,         //ICON_IMG_ZOOM,        
    //#else
    //    IMG_DC_ICON_NOTP_ZOOM_BG,         //ICON_IMG_ZOOM,        
    //#endif
    //#ifdef TOUCH_PANEL_SUPPORT
        IMG_DC_ICON_CONTRAST_BG,     //ICON_IMG_CONTRAST,       
    //#else
    //    IMG_DC_ICON_NOTP_CONTRAST_BG,     //ICON_IMG_CONTRAST,       
    //#endif    
        IMG_DC_ICON_UP,              //ICON_IMG_UP,         
        IMG_DC_ICON_GRAY_UP,         //ICON_IMG_GRAY_UP,         
        IMG_DC_ICON_FOCUS_UP,        //ICON_IMG_FOCUS_UP,     
        IMG_DC_ICON_DOWN,            //ICON_IMG_DOWN,            
        IMG_DC_ICON_GRAY_DOWN,       //ICON_IMG_GRAY_DOWN,            
        IMG_DC_ICON_FOCUS_DOWN,      //ICON_IMG_FOCUS_DOWN,      
        IMG_DC_ICON_PHOTO,           //ICON_IMG_PHOTO,           
        IMG_DC_ICON_VIDEO,           //ICON_IMG_VIDEO,  
        IMG_DC_ICON_SENSOR_REAR,    //ICON_IMG_SENSOR_REAR,        
        IMG_DC_ICON_SENSOR_FRONT,    //ICON_IMG_SENSOR_FRONT,   
        IMG_DC_ICON_SIZE,            //ICON_IMG_SIZE,    
        IMG_DC_ICON_NIGHT,           //ICON_IMG_NIGHT,           
        IMG_DC_ICON_NIGHT_OFF,       //ICON_IMG_NIGHT_OFF,   
        IMG_DC_ICON_EF_NORMAL,       //ICON_IMG_EF_NORMAL,    
        IMG_DC_ICON_EF_BLACKWHITE,   //ICON_IMG_EF_BLACKWHITE,
        IMG_DC_ICON_EF_RED,          //ICON_IMG_EF_RED,       
        IMG_DC_ICON_EF_GREEN,        //ICON_IMG_EF_GREEN,     
        IMG_DC_ICON_EF_BLUE,         //ICON_IMG_EF_BLUE,      
        IMG_DC_ICON_EF_YELLOW,       //ICON_IMG_EF_YELLOW,    
        IMG_DC_ICON_EF_NEGATIVE,     //ICON_IMG_EF_NEGATIVE,  
        IMG_DC_ICON_EF_CANVAS,       //ICON_IMG_EF_CANVAS,    
        IMG_DC_ICON_WB_INCANDESCENCE,//ICON_IMG_WB_INCANDESCENCE,
        IMG_DC_ICON_WB_SUN,          //ICON_IMG_WB_SUN,          
        IMG_DC_ICON_WB_CLOUDY,       //ICON_IMG_WB_CLOUDY,       
        IMG_DC_ICON_WB_FLUORESCENT,  //ICON_IMG_WB_FLUORESCENT,  
        IMG_DC_ICON_WB_AUTO,         //ICON_IMG_WB_AUTO,         
        IMG_DC_ICON_10S,             //ICON_IMG_10S,             
        IMG_DC_ICON_3S,              //ICON_IMG_3S,              
        IMG_DC_ICON_DELAY_OFF,       //ICON_IMG_DELAY_OFF,       
        IMG_DC_ICON_NO_MULTI,        //ICON_IMG_NO_MULTI,              
        IMG_DC_ICON_3_MULTI,         //ICON_IMG_3_MULTI,           
        IMG_DC_ICON_6_MULTI         //ICON_IMG_6_MULTI,        
    };
    
    uint16 img_width = 0;
    uint16 img_height = 0;
    int16 i = 0;
    uint16 total_img_number = 0;

    if((PNULL == img_list_ptr) || (PNULL == size_list_ptr) ||
        (img_list_size < sizeof(MMI_IMAGE_ID_T) * ICON_IMG_MAX) || 
        (size_list_size < sizeof(MMIDC_SIZE_ATTRIBUTE_T) * ICON_IMG_MAX))
    {
        return;
    }

    total_img_number = ARR_SIZE(img_list);
    if (ICON_IMG_MAX != total_img_number) /*lint !e774*/
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] GetIconImageList ICON_IMG_MAX != total_img_number !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_287_112_2_18_2_8_48_227,(uint8*)"");
        return;
    }

    SCI_MEMCPY(img_list_ptr,img_list, sizeof(MMI_IMAGE_ID_T) * total_img_number);

    //get all img size list
    for (i = 0; i < total_img_number; i++)
    {
        img_width = 0;
        img_height = 0;
        if (GUIRES_GetImgWidthHeight(&img_width, &img_height, img_list[i], MMIDC_MAIN_WIN_ID))
        {
            size_list_ptr[i].size_width = img_width;
            size_list_ptr[i].size_height = img_height;
        }
    }

}

/*****************************************************************************/
//Description :  display icon string
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayIconString(GUI_RECT_T box,
                                 GUI_FONT_T  font,
                                 MMI_STRING_T *str_ptr //in
                                )
{
    GUI_LCD_DEV_INFO    dev_info = MMIDC_GetOSDBlockInfo();
    GUISTR_STYLE_T style = {0};/*lint !e64*/
    GUISTR_STATE_T state = GetIconStringState();

    GetIconStringStyle(&style);

    style.font = font;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&dev_info,
        (const GUI_RECT_T      *)&box,       //the fixed display area
        (const GUI_RECT_T      *)&box,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)str_ptr,
        &style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
}
/*****************************************************************************/
//Description :  display brightness bg and arrows
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayIconBrightness(CAMERA_MODE_E mode,
                                       GUI_RECT_T start_rect, 
                                       DISPLAY_DIRECT_E direct,
                                       GUI_RECT_T *out_rect_ptr, //out
                                       MMI_IMAGE_ID_T *img_list_ptr, //in
                                       MMIDC_SIZE_ATTRIBUTE_T *size_list_ptr //in
                                      )
{
    MMI_IMAGE_ID_T bright_img_id = IMAGE_NULL;
    MMI_IMAGE_ID_T left_img_id = IMAGE_NULL;
    MMI_IMAGE_ID_T right_img_id = IMAGE_NULL;
    
    MMIDC_SIZE_ATTRIBUTE_T  bright_size = {0};
    MMIDC_SIZE_ATTRIBUTE_T  left_size = {0};
    MMIDC_SIZE_ATTRIBUTE_T  right_size = {0};

//#ifdef TOUCH_PANEL_SUPPORT 
    GUI_RECT_T left_rect = {0};
    GUI_RECT_T right_rect = {0};
//#endif    
    GUI_RECT_T text_rect = {0};

    GUI_FONT_T   font = ICON_NORMAL_FONT;
    MMI_STRING_T  text_str = {0};
    int16         text_width = 0;
    int16         text_height = 0;
    int16  brightness_val[BRIGHTNESS_LEVEL_MAX]=
    {
        -3, -2, -1, 0, 1, 2 , 3
    };
    char         text_char[3+1] = {0};
    wchar        text_wchar[3+1] = {0};
    BRIGHTNESS_E bright_level = 0;

    BOOLEAN is_up_gray = FALSE;
    BOOLEAN is_down_gray = FALSE;

    if (CAMERA_MODE_DC == mode)
    {
        bright_level = MMIDC_GetPhotoBrightness();
    }
    else
    {
        bright_level = MMIDC_GetVideoBrightness();
    }

    if (BRIGHTNESS_LEVEL_0 >= bright_level)
    {
        is_down_gray = TRUE;
    }
    if (BRIGHTNESS_LEVEL_6 <= bright_level) 
    {
        is_up_gray = TRUE;
    }

    //get img id
    bright_img_id = img_list_ptr[ICON_IMG_BRIGHTNESS];

    if (!is_up_gray)
    {
        left_img_id = img_list_ptr[ICON_IMG_BRIGHT_UP];
    }
    else
    {
        left_img_id = img_list_ptr[ICON_IMG_GRAY_BRIGHT_UP];
    }
    if (!is_down_gray)
    {
        right_img_id = img_list_ptr[ICON_IMG_BRIGHT_DOWN];
    }
    else
    {
        right_img_id = img_list_ptr[ICON_IMG_GRAY_BRIGHT_DOWN];
    }

    //get img size 
    bright_size = size_list_ptr[ICON_IMG_BRIGHTNESS];
    left_size = size_list_ptr[ICON_IMG_BRIGHT_UP];
    right_size = size_list_ptr[ICON_IMG_BRIGHT_DOWN];


    *out_rect_ptr = start_rect;
    //display brightness img
    MMIDC_DisplayBmp(out_rect_ptr->left, out_rect_ptr->top, bright_img_id);
    if (DISPLAY_DIRECT_HORIZONTAL == direct)
    {
        out_rect_ptr->right = out_rect_ptr->left + bright_size.size_width - 1;
    }
    AppendIconDisplayRect(ICON_RECT_BRIGHTNESS, *out_rect_ptr);


//#ifdef TOUCH_PANEL_SUPPORT 
    //get left img rect
    left_rect.left = start_rect.left + icon_off_space.left;
    left_rect.top = start_rect.top + (bright_size.size_height - left_size.size_height) / 2;
    left_rect.right = left_rect.left + left_size.size_width - 1;
    left_rect.bottom = left_rect.top + left_size.size_height - 1;
    //display left img
    MMIDC_DisplayBmp(left_rect.left, left_rect.top, left_img_id);
    AppendIconDisplayRect(ICON_RECT_LEFT, left_rect);


    //get right img rect
    right_rect.right = out_rect_ptr->right - icon_off_space.right;
    right_rect.top = left_rect.top;
    right_rect.left = right_rect.right - right_size.size_width + 1;
    right_rect.bottom = right_rect.top + right_size.size_height - 1;
    //display right img
    MMIDC_DisplayBmp(right_rect.left, right_rect.top, right_img_id);
    AppendIconDisplayRect(ICON_RECT_RIGHT, right_rect);
    
//#endif //TOUCH_PANEL_SUPPORT

    //get text rect
    SCI_MEMSET(text_char, 0x00, sizeof(text_char));
    SCI_MEMSET(text_wchar, 0x00, sizeof(text_wchar));

    sprintf(text_char, "%d", brightness_val[bright_level]);
    text_str.wstr_ptr = text_wchar;
    text_str.wstr_len = strlen(text_char);
    MMI_STRNTOWSTR(text_str.wstr_ptr, text_str.wstr_len, (uint8*)text_char, text_str.wstr_len, text_str.wstr_len);

    text_width = GetIconStringWidth(font, &text_str);
    text_height = GetIconStringHeight(font, &text_str);

//#ifdef TOUCH_PANEL_SUPPORT 
    text_rect.left = left_rect.right + 1;
    text_rect.right = right_rect.left - 1;    
    text_rect.top = left_rect.top + (left_size.size_height - text_height) / 2;   
//#else
//    text_rect.left = out_rect_ptr->left + icon_off_space.left;
//    text_rect.right = out_rect_ptr->right - icon_off_space.right + 1;    
//    text_rect.top = out_rect_ptr->top + (bright_size.size_height - text_height) / 2;
//#endif //TOUCH_PANEL_SUPPORT

    text_rect.bottom = text_rect.top + text_height - 1;
    text_rect.left +=(text_rect.right - text_rect.left + 1 - text_width) / 2;

    DisplayIconString(text_rect, font, &text_str);
    
}

/*****************************************************************************/
//Description :  display contrast bg and arrows
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayIconContrast(CAMERA_MODE_E mode,
                                   GUI_RECT_T start_rect, 
                                   DISPLAY_DIRECT_E direct,
                                   GUI_RECT_T *out_rect_ptr, //out
                                   MMI_IMAGE_ID_T *img_list_ptr, //in
                                   MMIDC_SIZE_ATTRIBUTE_T *size_list_ptr //in
                                  )
{
    MMI_IMAGE_ID_T contrast_img_id = IMAGE_NULL;
    MMI_IMAGE_ID_T left_img_id = IMAGE_NULL;
    MMI_IMAGE_ID_T right_img_id = IMAGE_NULL;
    
    MMIDC_SIZE_ATTRIBUTE_T  contrast_size = {0};
    MMIDC_SIZE_ATTRIBUTE_T  left_size = {0};
    MMIDC_SIZE_ATTRIBUTE_T  right_size = {0};

//#ifdef TOUCH_PANEL_SUPPORT 
    GUI_RECT_T left_rect = {0};
    GUI_RECT_T right_rect = {0};
//#endif    
    GUI_RECT_T text_rect = {0};

    GUI_FONT_T   font = ICON_NORMAL_FONT;
    MMI_STRING_T  text_str = {0};
    int16         text_width = 0;
    int16         text_height = 0;
    int16  contrast_val[CONTRAST_MAX]=
    {
        -3, -2, -1, 0, 1, 2 , 3
    };
    char         text_char[3+1] = {0};
    wchar        text_wchar[3+1] = {0};
    CONTRAST_E contrast_level = 0;

    BOOLEAN is_up_gray = FALSE;
    BOOLEAN is_down_gray = FALSE;

    
    if (CAMERA_MODE_DC == mode)
    {
        contrast_level = MMIDC_GetContrast();
    }
    else
    {
        contrast_level = MMIDC_GetVideoContrast();
    }

    if (CONTRAST_0 >= contrast_level)
    {
        is_down_gray = TRUE;
    }
    if (CONTRAST_6 <= contrast_level)
    {
        is_up_gray = TRUE;
    }
    

    //get img id
    contrast_img_id = img_list_ptr[ICON_IMG_CONTRAST];

    if (!is_up_gray)
    {
        left_img_id = img_list_ptr[ICON_IMG_UP];
    }
    else
    {
        left_img_id = img_list_ptr[ICON_IMG_GRAY_UP];
    }
    if (!is_down_gray)
    {
        right_img_id = img_list_ptr[ICON_IMG_DOWN];
    }
    else
    {
        right_img_id = img_list_ptr[ICON_IMG_GRAY_DOWN];
    }

    //get img size 
    contrast_size = size_list_ptr[ICON_IMG_CONTRAST];
    left_size = size_list_ptr[ICON_IMG_UP];
    right_size = size_list_ptr[ICON_IMG_DOWN];


    *out_rect_ptr = start_rect;
    //display contrast img
    MMIDC_DisplayBmp(out_rect_ptr->left, out_rect_ptr->top, contrast_img_id);
    if (DISPLAY_DIRECT_HORIZONTAL == direct)
    {
        out_rect_ptr->right = out_rect_ptr->left + contrast_size.size_width - 1;
    }
    AppendIconDisplayRect(ICON_RECT_CONTRAST, *out_rect_ptr);

//#ifdef TOUCH_PANEL_SUPPORT 
    //get left img rect
    left_rect.left = start_rect.left + icon_off_space.left;
    left_rect.top = start_rect.top + (contrast_size.size_height - left_size.size_height) / 2;
    left_rect.right = left_rect.left + left_size.size_width - 1;
    left_rect.bottom = left_rect.top + left_size.size_height - 1;
    //display left img
    MMIDC_DisplayBmp(left_rect.left, left_rect.top, left_img_id);
    AppendIconDisplayRect(ICON_RECT_UP, left_rect);


    //get right img rect
    right_rect.right = out_rect_ptr->right - icon_off_space.right;
    right_rect.top = left_rect.top;
    right_rect.left = right_rect.right - right_size.size_width + 1;
    right_rect.bottom = right_rect.top + right_size.size_height - 1;
    //display right img
    MMIDC_DisplayBmp(right_rect.left, right_rect.top, right_img_id);
    AppendIconDisplayRect(ICON_RECT_DOWN, right_rect);
//#endif //TOUCH_PANEL_SUPPORT

    //get text rect
    SCI_MEMSET(text_char, 0x00, sizeof(text_char));
    SCI_MEMSET(text_wchar, 0x00, sizeof(text_wchar));

    sprintf(text_char, "%d", contrast_val[contrast_level]);
    text_str.wstr_ptr = text_wchar;
    text_str.wstr_len = strlen(text_char);
    MMI_STRNTOWSTR(text_str.wstr_ptr, text_str.wstr_len, (uint8*)text_char, text_str.wstr_len, text_str.wstr_len);

    text_width = GetIconStringWidth(font, &text_str);
    text_height = GetIconStringHeight(font, &text_str);
    
//#ifdef TOUCH_PANEL_SUPPORT     
    text_rect.left = left_rect.right + 1;
    text_rect.right = right_rect.left - 1;    
    text_rect.top = left_rect.top + (left_size.size_height - text_height) / 2;
//#else
//    text_rect.left = out_rect_ptr->left + icon_off_space.left;
//    text_rect.right = out_rect_ptr->right - icon_off_space.right + 1;    
//    text_rect.top = out_rect_ptr->top + (contrast_size.size_height - text_height) / 2;
//#endif //TOUCH_PANEL_SUPPORT

    text_rect.bottom = text_rect.top + text_height - 1;
    text_rect.left +=(text_rect.right - text_rect.left + 1 - text_width) / 2;
    
    DisplayIconString(text_rect, font, &text_str);
    
}

/*****************************************************************************/
//Description :  display zoom bg and arrows
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayIconZoom(CAMERA_MODE_E mode,
                                GUI_RECT_T start_rect, 
                                DISPLAY_DIRECT_E direct,
                                GUI_RECT_T *out_rect_ptr, //out
                                MMI_IMAGE_ID_T *img_list_ptr, //in
                                MMIDC_SIZE_ATTRIBUTE_T *size_list_ptr //in
                                )
{
    MMI_IMAGE_ID_T zoom_img_id = IMAGE_NULL;
    MMI_IMAGE_ID_T left_img_id = IMAGE_NULL;
    MMI_IMAGE_ID_T right_img_id = IMAGE_NULL;
    
    MMIDC_SIZE_ATTRIBUTE_T  zoom_size = {0};
    MMIDC_SIZE_ATTRIBUTE_T  left_size = {0};
    MMIDC_SIZE_ATTRIBUTE_T  right_size = {0};

    MMIDC_ICON_IMG_E  img_up_index = 0;
    MMIDC_ICON_IMG_E  img_down_index = 0;

//#ifdef TOUCH_PANEL_SUPPORT 
    GUI_RECT_T left_rect = {0};
    GUI_RECT_T right_rect = {0};
//#endif    
    GUI_RECT_T text_rect = {0};

    GUI_FONT_T    font = ICON_NORMAL_FONT;
    MMI_STRING_T  text_str = {0};
    int16         text_width = 0;
    int16         text_height = 0;
    char         text_char[4+1] = {0};
    wchar        text_wchar[4+1] = {0};
    int32        zoom_val = 0;
    int32        max_zoom = 0;
    
    MULTI_SHOOT_E  multi = 0;
    FRAME_E        frame_index = 0;
    
    BOOLEAN is_up_gray = FALSE;
    BOOLEAN is_down_gray = FALSE;

    multi = MMIDC_GetMultiShootEnum();
    frame_index = MMIDC_GetFrameIndex();
    
    
    if (CAMERA_MODE_DC == mode)
    {
        zoom_val = MMIDC_GetPhotoZoomValue();
        max_zoom = MMIDC_GetMaxZoomLevel();
    }
    else
    {
#if defined(MMIDC_F_DV_ZOOM)
        zoom_val = MMIDC_GetDVZoomValue();
        max_zoom = MMIDC_GetDVMaxZoomLevel();
#endif
    }

    //if frame or multi shot
    //zoom 1X is least to display
    if ((1 < max_zoom)
        && (FRAME_DISABLE == frame_index)
        //&& (MULTI_SHOOT_DISABLE == multi)
       )
    {
        zoom_val += 1;
    }
    else
    {
        zoom_val = 1;
    }

    if ((0 != max_zoom )
        //&& (MULTI_SHOOT_DISABLE == multi)
        )
    {
        if (max_zoom <= zoom_val)
        {
            is_up_gray = TRUE;
        }
        
        if (1 >= zoom_val)
        {
            is_down_gray = TRUE;
        }        

        if (!is_up_gray)
        {
            img_up_index = ICON_IMG_UP;
        }
        else
        {
            img_up_index = ICON_IMG_GRAY_UP;
        }
        
        if (!is_down_gray)
        {
            img_down_index = ICON_IMG_DOWN;
        }
        else
        {
            img_down_index = ICON_IMG_GRAY_DOWN;
        }
    }
    else
    {
        img_up_index = ICON_IMG_GRAY_UP;
        img_down_index = ICON_IMG_GRAY_DOWN;
    }

    //get img id
    zoom_img_id = img_list_ptr[ICON_IMG_ZOOM];
    left_img_id = img_list_ptr[img_up_index];
    right_img_id = img_list_ptr[img_down_index];

    //get img size 
    zoom_size = size_list_ptr[ICON_IMG_ZOOM];
    left_size = size_list_ptr[img_up_index];
    right_size = size_list_ptr[img_down_index];


    *out_rect_ptr = start_rect;
    //display zoom img
    MMIDC_DisplayBmp(out_rect_ptr->left, out_rect_ptr->top, zoom_img_id);
    if (DISPLAY_DIRECT_HORIZONTAL == direct)
    {
        out_rect_ptr->right = out_rect_ptr->left + zoom_size.size_width - 1;
    }
    AppendIconDisplayRect(ICON_RECT_ZOOM, *out_rect_ptr);

//#ifdef TOUCH_PANEL_SUPPORT     
    //get left img rect
    left_rect.left = start_rect.left + icon_off_space.left;
    left_rect.top = start_rect.top + (zoom_size.size_height- left_size.size_height) / 2;
    left_rect.right = left_rect.left + left_size.size_width - 1;
    left_rect.bottom = left_rect.top + left_size.size_height - 1;
    //display left img
    MMIDC_DisplayBmp(left_rect.left, left_rect.top, left_img_id);
    AppendIconDisplayRect(ICON_RECT_UP, left_rect);


    //get right img rect
    right_rect.right = out_rect_ptr->right - icon_off_space.right;
    right_rect.top = left_rect.top;
    right_rect.left = right_rect.right - right_size.size_width + 1;
    right_rect.bottom = right_rect.top + right_size.size_height - 1;
    //display right img
    MMIDC_DisplayBmp(right_rect.left, right_rect.top, right_img_id);
    AppendIconDisplayRect(ICON_RECT_DOWN, right_rect);
//#endif //TOUCH_PANEL_SUPPORT

    //get text rect
    SCI_MEMSET(text_char, 0x00, sizeof(text_char));
    SCI_MEMSET(text_wchar, 0x00, sizeof(text_wchar));

    sprintf(text_char, "%dX", zoom_val);
    text_str.wstr_ptr = text_wchar;
    text_str.wstr_len = strlen(text_char);
    MMI_STRNTOWSTR(text_str.wstr_ptr, text_str.wstr_len, (uint8*)text_char, text_str.wstr_len, text_str.wstr_len);

    text_width = GetIconStringWidth(font, &text_str);
    text_height = GetIconStringHeight(font, &text_str);

//#ifdef TOUCH_PANEL_SUPPORT     
    text_rect.left = left_rect.right + 1;
    text_rect.right = right_rect.left - 1;    
    text_rect.top = left_rect.top + (left_size.size_height - text_height) / 2;
//#else
//    text_rect.left = out_rect_ptr->left + icon_off_space.left;
//    text_rect.right = out_rect_ptr->right - icon_off_space.right + 1;    
//    text_rect.top = out_rect_ptr->top + (zoom_size.size_height - text_height) / 2;
//#endif //TOUCH_PANEL_SUPPORT

    text_rect.bottom = text_rect.top + text_height - 1;
    text_rect.left +=(text_rect.right - text_rect.left + 1 - text_width) / 2;
    
    DisplayIconString(text_rect, font, &text_str);
    
}

/*****************************************************************************/
//Description :  display icon mode photo or video
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayIconMode(CAMERA_MODE_E mode,
                                GUI_RECT_T start_rect, 
                                DISPLAY_DIRECT_E direct,
                                GUI_RECT_T *out_rect_ptr, //out
                                MMI_IMAGE_ID_T *img_list_ptr, //in
                                MMIDC_SIZE_ATTRIBUTE_T *size_list_ptr //in
                                )
{
    MMIDC_ICON_IMG_E img_index = 0;
    MMI_IMAGE_ID_T img_id = IMAGE_NULL;
    MMIDC_SIZE_ATTRIBUTE_T  img_size = {0};
    
    GUI_RECT_T  capture_rect = {0};
    GUI_RECT_T  recorder_rect = {0};

    *out_rect_ptr = start_rect;
    if (CAMERA_MODE_DC == mode)
    {
        img_index = ICON_IMG_PHOTO;
        
    }
    else
    {
        img_index = ICON_IMG_VIDEO;
    }

    img_id = img_list_ptr[img_index];
    img_size = size_list_ptr[img_index];
    
    //display img
    MMIDC_DisplayBmp(out_rect_ptr->left, out_rect_ptr->top, img_id);
    if (DISPLAY_DIRECT_HORIZONTAL == direct)
    {
        out_rect_ptr->right = out_rect_ptr->left + img_size.size_width - 1;
    }
    AppendIconDisplayRect(ICON_RECT_MODE, *out_rect_ptr);

    //capture rect
    capture_rect = *out_rect_ptr; 
    capture_rect.right = capture_rect.left + img_size.size_width / 2 - 1;
    AppendIconDisplayRect(ICON_RECT_CAPTURE, capture_rect);

    //recorder rect
    recorder_rect = *out_rect_ptr; 
    recorder_rect.left = recorder_rect.right - img_size.size_width / 2 + 1 + 1;//adjust 1 
    AppendIconDisplayRect(ICON_RECT_RECORDER, recorder_rect);
}

/*****************************************************************************/
//Description :  display icon mode photo or video
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayIconSensor(CAMERA_MODE_E mode,
                                  GUI_RECT_T start_rect, 
                                  DISPLAY_DIRECT_E direct,
                                  GUI_RECT_T *out_rect_ptr, //out
                                  MMI_IMAGE_ID_T *img_list_ptr, //in
                                  MMIDC_SIZE_ATTRIBUTE_T *size_list_ptr //in
                                  )
{
    MMIDC_ICON_IMG_E img_index = 0;
    MMI_IMAGE_ID_T img_id = IMAGE_NULL;
    MMIDC_SIZE_ATTRIBUTE_T  img_size = {0};

    SETTING_SENSOR_ID_E sensor_id = 0;

    *out_rect_ptr = start_rect;

    sensor_id = MMIDC_GetSettingSensorID();
    if (SETTING_SENSOR_FRONT == sensor_id)
    {
        img_index = ICON_IMG_SENSOR_FRONT;
    }
    else
    {
        img_index = ICON_IMG_SENSOR_REAR;
    }

    img_id = img_list_ptr[img_index];
    img_size = size_list_ptr[img_index];
    
    //display img
    MMIDC_DisplayBmp(out_rect_ptr->left, out_rect_ptr->top, img_id);
    if (DISPLAY_DIRECT_VERTICAL == direct)
    {
        out_rect_ptr->bottom = out_rect_ptr->top + img_size.size_height - 1;
    }
    else
    {
        out_rect_ptr->right = out_rect_ptr->left + img_size.size_width - 1;
    }    
    
    AppendIconDisplayRect(ICON_RECT_SENSOR, *out_rect_ptr);
}

/*****************************************************************************/
//Description :  display brightness bg and arrows
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayIconSize(CAMERA_MODE_E mode,
                               GUI_RECT_T start_rect, 
                               DISPLAY_DIRECT_E direct,
                               GUI_RECT_T *out_rect_ptr, //out
                               MMI_IMAGE_ID_T *img_list_ptr, //in
                               MMIDC_SIZE_ATTRIBUTE_T *size_list_ptr //in
                              )
{
    MMI_IMAGE_ID_T img_id = IMAGE_NULL;
    
    MMIDC_SIZE_ATTRIBUTE_T  img_size = {0};

    GUI_RECT_T up_rect = {0};
    GUI_RECT_T down_rect = {0};

    GUI_FONT_T    font = ICON_SIZE_FONT;
    MMI_STRING_T  text_str = {0};
    int16         text_width = 0;
    int16         text_height = 0;

    char         text_char[SIZE_STR_LENGTH+1] = {0};
    wchar        text_wchar[SIZE_STR_LENGTH+1] = {0};
    
    int16        photo_size_index = 0;
    MMIDC_SIZE_ATTRIBUTE_T photo_size_val = {0};

    //get img id
    img_id = img_list_ptr[ICON_IMG_SIZE];

    //get img size 
    img_size = size_list_ptr[ICON_IMG_SIZE];


    *out_rect_ptr = start_rect;
    //display brightness img
    MMIDC_DisplayBmp(out_rect_ptr->left, out_rect_ptr->top, img_id);
    if (DISPLAY_DIRECT_VERTICAL == direct)
    {
        out_rect_ptr->bottom = out_rect_ptr->top + img_size.size_height - 1;
    }
    else
    {
        out_rect_ptr->right = out_rect_ptr->left + img_size.size_width - 1;
    }
    AppendIconDisplayRect(ICON_RECT_SIZE, *out_rect_ptr);

    //get text rect
    if (CAMERA_MODE_DC == mode)
    {
        photo_size_index = MMIDC_GetPhotoSize();
        MMIDC_GetPhotoSizeWidthAndHeight((MMIDC_PHOTO_SIZE_E)photo_size_index, &photo_size_val);
    }
    else
    {
        photo_size_index = MMIDC_GetVideoSize();
        MMIDC_GetVideoSizeWidthAndHeight((VIDEO_SIZE_E)photo_size_index, &photo_size_val);
    }

    SCI_MEMSET(text_char, 0x00, sizeof(text_char));
    SCI_MEMSET(text_wchar, 0x00, sizeof(text_wchar));

    sprintf(text_char, "%d", photo_size_val.size_width);
    text_str.wstr_ptr = text_wchar;
    text_str.wstr_len = strlen(text_char);
    MMI_STRNTOWSTR(text_str.wstr_ptr, text_str.wstr_len, (uint8*)text_char, text_str.wstr_len, text_str.wstr_len);

    text_width = GetIconStringWidth(font, &text_str);
    text_height = GetIconStringHeight(font, &text_str);

    //get up text rect
    up_rect.left = start_rect.left + (img_size.size_width - text_width) / 2 ;
    up_rect.top = start_rect.top + (img_size.size_height - text_height * 2) / 2;
    up_rect.right = up_rect.left + text_width - 1;
    up_rect.bottom = up_rect.top + text_height - 1;
#if defined (MAINLCD_SIZE_240X320) && defined(VECTOR_FONT_SUPPORT_ETRUMP)
	up_rect.left = up_rect.left + 1;
#endif
	DisplayIconString(up_rect, font, &text_str);

    //get down text rect
    SCI_MEMSET(text_char, 0x00, sizeof(text_char));
    SCI_MEMSET(text_wchar, 0x00, sizeof(text_wchar));

    sprintf(text_char, "x%d", photo_size_val.size_height);
    text_str.wstr_ptr = text_wchar;
    text_str.wstr_len = strlen(text_char);
    MMI_STRNTOWSTR(text_str.wstr_ptr, text_str.wstr_len, (uint8*)text_char, text_str.wstr_len, text_str.wstr_len);

    text_width = GetIconStringWidth(font, &text_str);
    text_height = GetIconStringHeight(font, &text_str);

    //get up text rect
    down_rect.left = start_rect.left + (img_size.size_width - text_width) / 2 ;
    down_rect.top = up_rect.top + text_height;
    down_rect.right = down_rect.left + text_width - 1;
    down_rect.bottom = down_rect.top + text_height - 1;
#if defined (MAINLCD_SIZE_240X320) && defined(VECTOR_FONT_SUPPORT_ETRUMP)
	down_rect.left = down_rect.left + ICON_SIZE_RIGHT_OFFSET;
#endif
	DisplayIconString(down_rect, font, &text_str);
    
}

/*****************************************************************************/
//Description :  display icon night mode
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayIconNight(CAMERA_MODE_E mode,
                              GUI_RECT_T start_rect, 
                              DISPLAY_DIRECT_E direct,
                              GUI_RECT_T *out_rect_ptr, //out
                              MMI_IMAGE_ID_T *img_list_ptr, //in
                              MMIDC_SIZE_ATTRIBUTE_T *size_list_ptr //in
                              )
{
    MMIDC_ICON_IMG_E img_index = 0;
    MMI_IMAGE_ID_T img_id = IMAGE_NULL;
    MMIDC_SIZE_ATTRIBUTE_T  img_size = {0};
    
    ENVIRONMENT_E env = 0;


    *out_rect_ptr = start_rect;
    env = MMIDC_GetPhotoEnvironment();

    if (ENVIRONMENT_NIGHT == env)
    {
        img_index = ICON_IMG_NIGHT;
    }
    else
    {
        img_index = ICON_IMG_NIGHT_OFF;
    }

    img_id = img_list_ptr[img_index];
    img_size = size_list_ptr[img_index];
    
    //display img
    MMIDC_DisplayBmp(out_rect_ptr->left, out_rect_ptr->top, img_id);
    if (DISPLAY_DIRECT_VERTICAL == direct)
    {
        out_rect_ptr->bottom = out_rect_ptr->top + img_size.size_height - 1;
    }
    else
    {
        out_rect_ptr->right = out_rect_ptr->left + img_size.size_width - 1;
    }
    
    AppendIconDisplayRect(ICON_RECT_NIGHT, *out_rect_ptr);
}

/*****************************************************************************/
//Description :  display icon white balance
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayIconWhiteBalance(CAMERA_MODE_E mode,
                                          GUI_RECT_T start_rect, 
                                          DISPLAY_DIRECT_E direct,
                                          GUI_RECT_T *out_rect_ptr, //out
                                          MMI_IMAGE_ID_T *img_list_ptr, //in
                                          MMIDC_SIZE_ATTRIBUTE_T *size_list_ptr //in
                                          )
{
    MMIDC_ICON_IMG_E img_index = 0;
    MMI_IMAGE_ID_T img_id = IMAGE_NULL;
    MMIDC_SIZE_ATTRIBUTE_T  img_size = {0};
    
    WHITE_BALANCE_E wb = 0;


    *out_rect_ptr = start_rect;
    
    wb = MMIDC_GetPhotoWhiteBalance();
    switch(wb)
    {
    case WHITE_BALANCE_AUTO:
        img_index = ICON_IMG_WB_AUTO;
        break;
    case WHITE_BALANCE_INCANDESCENCE:
        img_index = ICON_IMG_WB_INCANDESCENCE;
        break;
    case WHITE_BALANCE_FLUORESCENT:
        img_index = ICON_IMG_WB_FLUORESCENT;
        break;
    case WHITE_BALANCE_SUN:
        img_index = ICON_IMG_WB_SUN;
        break;
    case WHITE_BALANCE_CLOUDY:
        img_index = ICON_IMG_WB_CLOUDY;
        break;
    default:
        img_index = ICON_IMG_WB_AUTO;
        break;
    }

    img_id = img_list_ptr[img_index];
    img_size = size_list_ptr[img_index];
    
    //display img
    MMIDC_DisplayBmp(out_rect_ptr->left, out_rect_ptr->top, img_id);
    if (DISPLAY_DIRECT_VERTICAL == direct)
    {
        out_rect_ptr->bottom = out_rect_ptr->top + img_size.size_height - 1;
    }
    else
    {
        out_rect_ptr->right = out_rect_ptr->left + img_size.size_width - 1;
    }
    
    AppendIconDisplayRect(ICON_RECT_WB, *out_rect_ptr);
}

/*****************************************************************************/
//Description :  display icon effect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayIconEffect(CAMERA_MODE_E mode,
                                  GUI_RECT_T start_rect, 
                                  DISPLAY_DIRECT_E direct,
                                  GUI_RECT_T *out_rect_ptr, //out
                                  MMI_IMAGE_ID_T *img_list_ptr, //in
                                  MMIDC_SIZE_ATTRIBUTE_T *size_list_ptr //in
                                  )
{
    MMIDC_ICON_IMG_E img_index = 0;
    MMI_IMAGE_ID_T img_id = IMAGE_NULL;
    MMIDC_SIZE_ATTRIBUTE_T  img_size = {0};
    
    DC_EFFECT_E ef = 0;


    *out_rect_ptr = start_rect;
    if (CAMERA_MODE_DC == mode)
    {
        ef = MMIDC_GetPhotoEffect();
    }
    else
    {
        ef = MMIDC_GetVideoEffect();
    }
    
    switch(ef)
    {
    case DC_EFFECT_NORMAL:
        img_index = ICON_IMG_EF_NORMAL;
        break;
    case DC_EFFECT_BLACKWHITE:
        img_index = ICON_IMG_EF_BLACKWHITE;
        break;
    case DC_EFFECT_RED:
        img_index = ICON_IMG_EF_RED;
        break;
    case DC_EFFECT_GREEN:
        img_index = ICON_IMG_EF_GREEN;
        break;
    case DC_EFFECT_BLUE:
        img_index = ICON_IMG_EF_BLUE;
        break;
    case DC_EFFECT_YELLOW:
        img_index = ICON_IMG_EF_YELLOW;
        break;
    case DC_EFFECT_NEGATIVE:
        img_index = ICON_IMG_EF_NEGATIVE;
        break;
    case DC_EFFECT_CANVAS:
        img_index = ICON_IMG_EF_CANVAS;
        break;
    default:
        img_index = ICON_IMG_EF_NORMAL;
        break;
    }

    img_id = img_list_ptr[img_index];
    img_size = size_list_ptr[img_index];
    
    //display img
    MMIDC_DisplayBmp(out_rect_ptr->left, out_rect_ptr->top, img_id);
    if (DISPLAY_DIRECT_VERTICAL == direct)
    {
        out_rect_ptr->bottom = out_rect_ptr->top + img_size.size_height - 1;
    }
    else
    {
        out_rect_ptr->right = out_rect_ptr->left + img_size.size_width - 1;
    }
    
    AppendIconDisplayRect(ICON_RECT_EFFECT, *out_rect_ptr);
}

/*****************************************************************************/
//Description :  display icon delay
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayIconDelay(CAMERA_MODE_E mode,
                              GUI_RECT_T start_rect, 
                              DISPLAY_DIRECT_E direct,
                              GUI_RECT_T *out_rect_ptr, //out
                              MMI_IMAGE_ID_T *img_list_ptr, //in
                              MMIDC_SIZE_ATTRIBUTE_T *size_list_ptr //in
                              )
{
    MMIDC_ICON_IMG_E img_index = 0;
    MMI_IMAGE_ID_T img_id = IMAGE_NULL;
    MMIDC_SIZE_ATTRIBUTE_T  img_size = {0};
    
    SELF_SHOOT_E delay = 0;

    *out_rect_ptr = start_rect;
    delay = MMIDC_GetSelfShootDelayTime();

    switch(delay)
    {
    case SELF_SHOOT_DISABLE:
        img_index = ICON_IMG_DELAY_OFF;
        break;
    case SELF_SHOOT_3S:
        img_index = ICON_IMG_3S;
        break;
    case SELF_SHOOT_10S:
        img_index = ICON_IMG_10S;
        break;    
    default:
        img_index = ICON_IMG_DELAY_OFF;
        break;
    }

    img_id = img_list_ptr[img_index];
    img_size = size_list_ptr[img_index];
    
    //display img
    MMIDC_DisplayBmp(out_rect_ptr->left, out_rect_ptr->top, img_id);
    if (DISPLAY_DIRECT_VERTICAL == direct)
    {
        out_rect_ptr->bottom = out_rect_ptr->top + img_size.size_height - 1;
    }
    else
    {
        out_rect_ptr->right = out_rect_ptr->left + img_size.size_width - 1;
    }
    
    AppendIconDisplayRect(ICON_RECT_DELAY, *out_rect_ptr);
}

/*****************************************************************************/
//Description :  display icon store
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayIconStore(CAMERA_MODE_E mode,
                              GUI_RECT_T start_rect, 
                              DISPLAY_DIRECT_E direct,
                              GUI_RECT_T *out_rect_ptr, //out
                              MMI_IMAGE_ID_T *img_list_ptr, //in
                              MMIDC_SIZE_ATTRIBUTE_T *size_list_ptr //in
                              )
{
    //MMIDC_ICON_IMG_E img_index = 0;
    MMI_IMAGE_ID_T img_id = IMAGE_NULL;
    MMIDC_SIZE_ATTRIBUTE_T  img_size = {0};
    MMIFILE_DEVICE_E    dc_dev = 0;

    *out_rect_ptr = start_rect;    
    
    if ((MMIAPIFMM_GetFirstValidDevice() >= MMI_DEVICE_NUM))
    {
        dc_dev = MMI_DEVICE_NUM;
    }
    else
    {
        if (CAMERA_MODE_DC == mode)
        {
            dc_dev = MMIDC_GetPrefPhotoStorageDevice();
        }
        else
        {
            dc_dev = MMIDC_GetPrefVideoStorageDevice();
        }
    }

    img_id = MMIAPIFMM_GetStorageIcon(dc_dev);
    GUIRES_GetImgWidthHeight(&img_size.size_width, &img_size.size_height, img_id, MMIDC_MAIN_WIN_ID);
    
    //display img
    MMIDC_DisplayBmp(out_rect_ptr->left, out_rect_ptr->top, img_id);
    if (DISPLAY_DIRECT_VERTICAL == direct)
    {
        out_rect_ptr->bottom = out_rect_ptr->top + img_size.size_height - 1;
    }
    else
    {
        out_rect_ptr->right = out_rect_ptr->left + img_size.size_width - 1;
    }
    
    AppendIconDisplayRect(ICON_RECT_STORE, *out_rect_ptr);
}

/*****************************************************************************/
//Description :  display icon multi shot
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL void DisplayIconMultiShot(CAMERA_MODE_E mode,
                                      GUI_RECT_T start_rect, 
                                      DISPLAY_DIRECT_E direct,
                                      GUI_RECT_T *out_rect_ptr, //out
                                      MMI_IMAGE_ID_T *img_list_ptr, //in
                                      MMIDC_SIZE_ATTRIBUTE_T *size_list_ptr //in
                                      )
{
    MMIDC_ICON_IMG_E img_index = 0;
    MMI_IMAGE_ID_T img_id = IMAGE_NULL;
    MMIDC_SIZE_ATTRIBUTE_T  img_size = {0};
    
    MULTI_SHOOT_E multi = 0;

    *out_rect_ptr = start_rect;
    multi = MMIDC_GetMultiShootEnum();

    switch(multi)
    {
    case MULTI_SHOOT_DISABLE:
        img_index = ICON_IMG_NO_MULTI;
        break;
    case MULTI_SHOOT_3:
        img_index = ICON_IMG_3_MULTI;
        break;
    case MULTI_SHOOT_6:
        img_index = ICON_IMG_6_MULTI;
        break;    
    default:
        img_index = ICON_IMG_NO_MULTI;
        break;
    }

    img_id = img_list_ptr[img_index];
    img_size = size_list_ptr[img_index];
    
    //display img
    MMIDC_DisplayBmp(out_rect_ptr->left, out_rect_ptr->top, img_id);
    if (DISPLAY_DIRECT_VERTICAL == direct)
    {
        out_rect_ptr->bottom = out_rect_ptr->top + img_size.size_height - 1;
    }
    else
    {
        out_rect_ptr->right = out_rect_ptr->left + img_size.size_width - 1;
    }
    
    AppendIconDisplayRect(ICON_RECT_MULTI, *out_rect_ptr);
}

/*****************************************************************************/
//Description :  display icon desktop
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayIconDesktop(void)
{
    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();
    CAMERA_MODE_E   mode = MMIDC_GetCameraMode();
    
    int16 screen_width = MMIDC_GetPreviewWidth();
    int16 screen_height = MMIDC_GetPreviewHeight();
    int16 key_width = MMIDC_GetMaxWidthOfSoftkey();
    int16 key_height = MMIDC_ITEM_TEXT_HEIGHT;
    int16 display_rect_width = 0;
    int16 display_rect_height = 0;

    int16 first_space = 0;
    int16 second_space = 0;
    GUI_RECT_T  first_rect = {0};
    GUI_RECT_T  second_rect = {0};    
    int16 first_region_width = 0;
    int16 first_region_height = 0;
    int16 second_region_width = 0;
    int16 second_region_height = 0;
    
    int16 first_img_number = 0;
    int16 second_img_number = 0;    
    MMIDC_SIZE_ATTRIBUTE_T first_img1 = {0};
    MMIDC_SIZE_ATTRIBUTE_T first_img2 = {0};
    MMIDC_SIZE_ATTRIBUTE_T first_img3 = {0};
    int16 first_img_width = 0;
    int16 first_img_height = 0;
    int16 second_img_width = 0;
    int16 second_img_height = 0;
    int16 first_img_ver_adjust_space = 0;
    int16 first_img_hor_adjust_space = 0;
    int16 second_img_ver_adjust_space = 0;
    int16 second_img_hor_adjust_space = 0;

    DISPLAY_DIRECT_E first_disp_direct = 0;
    DISPLAY_DIRECT_E second_disp_direct = 0;

    int16 *first_tag_right_ptr = PNULL;
    int16 *first_tag_left_ptr = PNULL;
    int16 *second_tag_right_ptr = PNULL;
    int16 *second_tag_left_ptr = PNULL;
    
    MMI_IMAGE_ID_T img_list[ICON_IMG_MAX] = {0};
    MMIDC_SIZE_ATTRIBUTE_T size_list[ICON_IMG_MAX] = {0};
    int16 total_img_number = 0;
    
    BOOLEAN is_open = FALSE;
    int16 sensor_number = 0;
#if defined(MMIDC_F_U_CAMERA) && defined(MMIDC_F_N_IN_ONE)
    int16 ninone_width = 0;
    int16 ninone_height = 0;

    MMIDC_GetNInOneDisplayWidthAndHeight(&ninone_width, &ninone_height);
#endif

    is_open = MMIDC_IsIconDesktopOpen();
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayIconDesktop is_open =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_1411_112_2_18_2_8_50_228,(uint8*)"d", is_open);
    if (!is_open)
    {
        return FALSE;
    }

    //clear last rect list
    MMIDC_ClearIconDesktopRectList();

    GetIconImageList(img_list, sizeof(img_list), size_list, sizeof(size_list));

    total_img_number = ARR_SIZE(img_list);
    if (ICON_IMG_MAX != total_img_number) /*lint !e774*/
    {
        //MMIDC_ASSERT_WARNING:"[MMIDC] MMIDC_DisplayIconDesktop ICON_IMG_MAX != total_img_number !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_1425_112_2_18_2_8_50_229,(uint8*)"");
        return FALSE;
    }

    //get standard img size 
    first_img1 = size_list[ICON_IMG_BRIGHTNESS];
    first_img2 = size_list[ICON_IMG_ZOOM];
#if !defined(MMIDC_F_DV_ZOOM) 
    if (CAMERA_MODE_DV == mode)
    {
        first_img2 = size_list[ICON_IMG_CONTRAST];
    }
#endif
    first_img3 = size_list[ICON_IMG_PHOTO];

    first_img_width = first_img1.size_width + first_img2.size_width + first_img3.size_width;
    first_img_height = first_img1.size_height;
    second_img_width = size_list[ICON_IMG_SIZE].size_width;
    second_img_height = size_list[ICON_IMG_SIZE].size_height;


    sensor_number = MMIDC_GetDCameraSensorNumber();
    
    //img number
    if (CAMERA_MODE_DC == mode)
    {
#if defined(MMIDC_F_U_CAMERA)
        if(MMIDC_IsUCameraEnable())
        {
            first_img_number = 1;
            if (MAX_MMI_SENSOR_NUM <= sensor_number)
            {
                second_img_number = 4;
            }
            else
            {
                second_img_number = 3;
            }
#ifdef MMIDC_F_N_IN_ONE
            if(MMIDC_IsNInOneEnable())
            {
                second_img_number++;
            }
#endif
        }
        else
#endif
#if defined (MMIDC_F_WORK_MODE)
        if(MMIDC_IsWorkMode())
        {
            first_img_number = 2;
            if (MAX_MMI_SENSOR_NUM <= sensor_number)
            {
                second_img_number = 6;
            }
            else
            {
                second_img_number = 5;
            }
        }
        else
#endif
        {
            first_img_number = 3;
            if (MAX_MMI_SENSOR_NUM <= sensor_number)
            {
                second_img_number = 7;
            }
            else
            {
                second_img_number = 6;
            }
        }
    }
    else
    {
#if defined (MMIDC_F_WORK_MODE)
        if(MMIDC_IsWorkMode())
        {
            first_img_number = 2;
            second_img_number = 4;
        }
        else
#endif
        {
            first_img_number = 3;
            second_img_number = 4;
        }
    }
    
    switch(layout_style)
    {
    case MMIDC_LAYOUT_RIGHTWARD:
        //all icon display rect
        display_rect_width = screen_width - key_width;
        display_rect_height = screen_height;
        
        //first region width and height, space
        first_img_ver_adjust_space = 2;
        first_img_hor_adjust_space = 0;
        first_region_width =  first_img_hor_adjust_space + first_img_width;
        first_region_height = first_img_ver_adjust_space + first_img_height;
        first_space = (display_rect_width - first_region_width) / (first_img_number + 1);

        first_disp_direct = DISPLAY_DIRECT_HORIZONTAL;

        //first start rect
        first_rect.top = first_img_ver_adjust_space;
        first_rect.bottom = first_rect.top + first_img1.size_height - 1;
        first_rect.left = first_img_hor_adjust_space + first_space;
        first_rect.right = first_rect.left + first_img1.size_width - 1;

        //first tag ptr
        first_tag_left_ptr = &first_rect.left;
        first_tag_right_ptr = &first_rect.right;

        //second region width and height, space
        second_img_ver_adjust_space = 2;
        second_img_hor_adjust_space = 0;
        second_region_width = second_img_hor_adjust_space + second_img_width * second_img_number;
#if defined(MMIDC_F_U_CAMERA) && defined(MMIDC_F_N_IN_ONE)
        if(MMIDC_IsUCameraEnable() && MMIDC_IsNInOneEnable())
        {
            second_region_width = second_region_width - second_img_width + ninone_width;
        }
#endif
        second_region_height = second_img_ver_adjust_space + second_img_height;
        second_space = (display_rect_width - second_region_width) / (second_img_number + 1);

        second_disp_direct = DISPLAY_DIRECT_HORIZONTAL;

        //second start rect
        second_rect.top = display_rect_height - second_region_height;
        second_rect.bottom = second_rect.top + second_img_height - 1;
        second_rect.left = second_img_hor_adjust_space + second_space;
        second_rect.right = second_rect.left + second_img_width - 1; 

        //second tag ptr
        second_tag_left_ptr = &second_rect.left;
        second_tag_right_ptr = &second_rect.right;                        
        break;
    case MMIDC_LAYOUT_TRADITION:        
        //all icon display rect
        display_rect_width = screen_width;
        display_rect_height = screen_height - key_height;

        //first region width and height, space
        first_img_ver_adjust_space = 2;
        first_img_hor_adjust_space = 0;
        first_region_width =  first_img_hor_adjust_space + first_img_width;
        first_region_height = first_img_ver_adjust_space + first_img_height;
        first_space = (display_rect_width - first_region_width) / (first_img_number + 1);

        first_disp_direct = DISPLAY_DIRECT_HORIZONTAL;

        //first start rect
        first_rect.top = first_img_ver_adjust_space;
        first_rect.bottom = first_rect.top + first_img1.size_height - 1;
        first_rect.left = first_img_hor_adjust_space + first_space;
        first_rect.right = first_rect.left + first_img1.size_width - 1;

        //first tag ptr and change first rect
        first_tag_left_ptr = &first_rect.left;
        first_tag_right_ptr = &first_rect.right;

        //second region width and height, space
        second_img_ver_adjust_space = 0;
        second_img_hor_adjust_space = 2;
        second_region_width =  second_img_hor_adjust_space + second_img_width ;
        second_region_height = second_img_ver_adjust_space + second_img_height * second_img_number;
#if defined(MMIDC_F_U_CAMERA) && defined(MMIDC_F_N_IN_ONE)
        if(MMIDC_IsUCameraEnable() && MMIDC_IsNInOneEnable())
        {
            second_region_height = second_region_height - second_img_height + ninone_height;
        }
#endif
        second_space = (display_rect_height - first_region_height - second_region_height) / (second_img_number + 1);

        second_disp_direct = DISPLAY_DIRECT_VERTICAL;

        //second start rect 
        second_rect.top = first_region_height + second_img_ver_adjust_space + second_space;
        second_rect.bottom = second_rect.top + second_img_height - 1;
        second_rect.left = display_rect_width - second_region_width;
        second_rect.right = second_rect.left + second_img_width - 1; 

        //second tag ptr and change second rect
        second_tag_left_ptr = &second_rect.top;
        second_tag_right_ptr = &second_rect.bottom;                       
        break;
    default:
        break;
    }


    //display all first and second region
    
    if ((PNULL == first_tag_left_ptr)
        || (PNULL == first_tag_right_ptr)
        || (PNULL == second_tag_left_ptr)
        || (PNULL == second_tag_right_ptr)
       )
    {
        //SCI_TRACE_LOW:"[MMIDC] MMIDC_DisplayIconDesktop tag ptr NULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_1610_112_2_18_2_8_50_230,(uint8*)"");
        return FALSE;
    }

    //first region
#if defined (MMIDC_F_U_CAMERA)
    if(!MMIDC_IsUCameraEnable())
#endif    
    {
        //display brightness img
        DisplayIconBrightness(mode, first_rect, first_disp_direct, 
                              &first_rect, img_list, size_list);
    }

    //next start rect                      
    *first_tag_left_ptr = *first_tag_right_ptr + first_space;
    if (CAMERA_MODE_DC == mode)
    {
        //display zoom bg
        DisplayIconZoom(mode, first_rect, first_disp_direct, 
                            &first_rect, img_list, size_list);
    }
    else
    {
#if defined(MMIDC_F_DV_ZOOM)
        //display zoom bg
        DisplayIconZoom(mode, first_rect, first_disp_direct, 
                            &first_rect, img_list, size_list);
#else
        //display contrast bg
        DisplayIconContrast(mode, first_rect, first_disp_direct, 
                            &first_rect, img_list, size_list);
#endif
    }
    
#if defined (MMIDC_F_U_CAMERA)
    if(!MMIDC_IsUCameraEnable())
#endif    
    {
#if defined (MMIDC_F_WORK_MODE)
        if(!MMIDC_IsWorkMode())
#endif
        {
            //display camera mode
            *first_tag_left_ptr = *first_tag_right_ptr + first_space;
            DisplayIconMode(mode, first_rect, first_disp_direct, 
                        &first_rect, img_list, size_list);
        }
    }


    //second region
    if (MAX_MMI_SENSOR_NUM <= sensor_number)
    {
        //display sensor img
        DisplayIconSensor(mode, second_rect, second_disp_direct, 
                         &second_rect, img_list, size_list);

        //next rect
        *second_tag_left_ptr = *second_tag_right_ptr + second_space;
    }
    
    //display size img
    DisplayIconSize(mode, second_rect, second_disp_direct, 
                     &second_rect, img_list, size_list);

    if (CAMERA_MODE_DC == mode)
    {
#if defined (MMIDC_F_U_CAMERA)    
        if(!MMIDC_IsUCameraEnable())
 #endif 
       {
            //display night img
            *second_tag_left_ptr = *second_tag_right_ptr + second_space;
            DisplayIconNight(mode, second_rect, second_disp_direct, 
                             &second_rect, img_list, size_list);
                             
            //display whitebalance img
            *second_tag_left_ptr = *second_tag_right_ptr + second_space;
            DisplayIconWhiteBalance(mode, second_rect, second_disp_direct, 
                             &second_rect, img_list, size_list);

        }

        //display delay img
        *second_tag_left_ptr = *second_tag_right_ptr + second_space;
        DisplayIconDelay(mode, second_rect, second_disp_direct, 
                         &second_rect, img_list, size_list);
    }
    else
    {
        //display effect img
        *second_tag_left_ptr = *second_tag_right_ptr + second_space;
        DisplayIconEffect(mode, second_rect, second_disp_direct, 
                         &second_rect, img_list, size_list);
    }
    
    //display store img
    *second_tag_left_ptr = *second_tag_right_ptr + second_space;
    DisplayIconStore(mode, second_rect, second_disp_direct, 
                     &second_rect, img_list, size_list);

    *second_tag_left_ptr = *second_tag_right_ptr + second_space;
    if (CAMERA_MODE_DC == mode)
    {
#if defined (MMIDC_F_U_CAMERA)    
        if(!MMIDC_IsUCameraEnable())
#endif 
       {
#if defined (MMIDC_F_WORK_MODE)
            if(!MMIDC_IsWorkMode())
#endif
            {
            //display multi img
                DisplayIconMultiShot(mode, second_rect, second_disp_direct, 
                                 &second_rect, img_list, size_list);
            }
        }

#if defined(MMIDC_F_U_CAMERA) && defined(MMIDC_F_N_IN_ONE)
        if(MMIDC_IsUCameraEnable() && MMIDC_IsNInOneEnable())
        {
            GUI_POINT_T ninone_point = {0};
            
            *second_tag_left_ptr = *second_tag_right_ptr + second_space;
            if (DISPLAY_DIRECT_VERTICAL == second_disp_direct)
            {
                ninone_point.x = second_rect.left - (ninone_width - second_region_width + 1) + 1;
                ninone_point.y = second_rect.top;
            }
            else
            {
                ninone_point.x = second_rect.left;
                ninone_point.y = second_rect.top - (ninone_height - second_region_height + 1) + 1;
            }
            MMIDC_SetNInOneStartPoint(ninone_point);
        }
#endif
    }
    
    return TRUE;
}

/*****************************************************************************/
//Description :  get icon desktop rect list
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC int16 MMIDC_GetIconDesktopRectList(int16 max_number, 
                                                    MMIDC_ICON_DISP_T *disp_list //out
                                                    )
{
    uint16 total_num = 0;
    uint32 total_bytes = 0;

    total_num = s_gui_desktop_icon.icon_disp_number;

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_GetIconDesktopRectList max_number=%d, total_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_1746_112_2_18_2_8_51_231,(uint8*)"dd", max_number, total_num);
    if (max_number < total_num)
    {
        return 0;
    }

    if (PNULL != disp_list)
    {
        total_bytes = total_num * sizeof(s_gui_desktop_icon.icon_disp[0]);
        SCI_MEMCPY(disp_list, s_gui_desktop_icon.icon_disp, total_bytes);
    }
    
    return total_num;
}

/*****************************************************************************/
//Description :  shortcut for capture rect by TP
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForCapture(void)
{
    CAMERA_MODE_E   mode = CAMERA_MODE_DC;

    mode = MMIDC_GetCameraMode();
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForCapture mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_1770_112_2_18_2_8_51_232,(uint8*)"d", mode);
    if (CAMERA_MODE_DC != mode)
    {
        SetPhotoMode();
    }
}

/*****************************************************************************/
//Description :  shortcut for recorder rect by TP
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForRecorder(void)
{
    CAMERA_MODE_E   mode = CAMERA_MODE_DC;

    mode = MMIDC_GetCameraMode();
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForRecorder mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_1786_112_2_18_2_8_51_233,(uint8*)"d", mode);
    if (CAMERA_MODE_DV != mode)
    {
        SetVideoMode();
    }
}

/*****************************************************************************/
//Description :  shortcut for camera mode
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForCameraMode(void)
{
    CAMERA_MODE_E   mode = CAMERA_MODE_DC;

    mode = MMIDC_GetCameraMode();
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForCameraMode mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_1802_112_2_18_2_8_51_234,(uint8*)"d", mode);
    if (CAMERA_MODE_DC == mode)
    {
        SetVideoMode();
    }
    else
    {
        SetPhotoMode();
    }
}

/*****************************************************************************/
//Description :  shortcut for camera mode
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForSize(void)
{
    CAMERA_MODE_E   mode = CAMERA_MODE_DC;
    int32 lcd_size = 0;
    SCREEN_MODE_E  screen_mode = 0;

    
    MMIDC_PHOTO_SIZE_E photo_size_list[PHOTO_SIZE_MAX] = {0};
    VIDEO_SIZE_E video_size_list[VIDEO_SIZE_MAX] = {0};
    MENU_ITEM_CALL_BACK photo_func_list[PHOTO_SIZE_MAX] = {0};
    MENU_ITEM_CALL_BACK video_func_list[VIDEO_SIZE_MAX] = {0};
    int16 total_size_list_number = 0;
    int16 total_func_list_number = 0;

    int16  i = 0;   
    int16  total_size_item = 0;
    int16  total_func_item = 0;
    int16 cur_size = 0;
    int16 next_size = 0;
    int32 *size_list_ptr = PNULL;
    MENU_ITEM_CALL_BACK *callback_ptr = PNULL;


    mode = MMIDC_GetCameraMode();

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForSize mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_1842_112_2_18_2_8_51_235,(uint8*)"d", mode);
    if (CAMERA_MODE_DC == mode)
    {
#if defined (MMIDC_F_U_CAMERA)
        if(!MMIDC_IsUCameraEnable())
#endif
        {
            //DC MODE

            total_func_item = ARR_SIZE(photo_func_list);
            total_size_item = ARR_SIZE(photo_size_list);

            lcd_size = MMIDC_GetLCDSize();
            screen_mode = MMIDC_GetScreenMode();        
            if (SCREEN_MODE_VERTICAL == screen_mode)
            {
                //for phone size list, may horzontal screen
                if (SCREEN_MODE_HORIIZONTAL == MMIDC_GetDefaultScreenMode())
                {
                    screen_mode = SCREEN_MODE_HORIIZONTAL;
                }
            }

            //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForSize lcd_size=%d,screen_mode=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_1865_112_2_18_2_8_51_236,(uint8*)"dd", lcd_size, screen_mode);
            //size list from high size to low size
            total_size_list_number = GetPhonePhotoSizeList(total_size_item, lcd_size, screen_mode, photo_size_list);
            //get all size function list
            total_func_list_number = GetPhonePhotoSizeFunctionList(total_func_item, photo_func_list);
            if (PHOTO_SIZE_MAX != total_func_list_number)
            {
                return ;
            }
            cur_size = MMIDC_GetPhotoSize();

            //get process pointer
            size_list_ptr = photo_size_list;
            callback_ptr = photo_func_list;

            for (i = 0; i < total_size_list_number ; i++)
            {
                if (cur_size == size_list_ptr[i])
                {
                    break;
                }
            }
            
            if (i < (total_size_list_number - 1))
            {
                next_size = size_list_ptr[i+1];
            }
            else
            {
                next_size = size_list_ptr[0];
            }
        }
    }
    else
    {
        //DV MODE
        total_func_item = ARR_SIZE(video_func_list);
        total_size_item = ARR_SIZE(video_size_list);


        //size list from high size to low size
        total_size_list_number = MMIDC_GetPhoneVideoSizeList(total_size_item, video_size_list);
        total_func_list_number = MMIDC_GetPhoneVideoSizeFunctionList(total_func_item, video_func_list);
        if (VIDEO_SIZE_MAX != total_func_list_number)
        {
            return ;
        }

        cur_size = MMIDC_GetVideoSize();

        //get process pointer
        size_list_ptr = video_size_list;
        callback_ptr = video_func_list;

        for (i = 0; i < total_size_list_number ; i++)
        {
            if (cur_size == size_list_ptr[i])
            {
                break;
            }
        }
        
        if (i < (total_size_list_number - 1))
        {
            next_size = size_list_ptr[i+1];
        }
        else
        {
            next_size = size_list_ptr[0];
        }

    }


    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForSize total_size_list_number=%d, total_func_list_number=%d,cur_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_1939_112_2_18_2_8_51_237,(uint8*)"ddd", total_size_list_number, total_func_list_number, cur_size);
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForSize i=%d, next_size=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_1940_112_2_18_2_8_51_238,(uint8*)"dd", i, next_size);

    //handle callback function
    if (PNULL != callback_ptr[next_size])/*lint !e613*/
    {
        callback_ptr[next_size]();/*lint !e613*/
    }

}

/*****************************************************************************/
//Description :  shortcut for environment
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForEnvironment(void)
{
    ENVIRONMENT_E  env = 0;
    
    env = MMIDC_GetPhotoEnvironment();

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForEnvironment env=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_1960_112_2_18_2_8_51_239,(uint8*)"d", env);
    if (ENVIRONMENT_AUTO == env)
    {
        SetPhotoEnvNight();
    }
    else
    {
        SetPhotoEnvNormal();
    }
}

/*****************************************************************************/
//Description :  shortcut for effect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForEffect(void)
{
    CAMERA_MODE_E mode = 0;
    DC_EFFECT_E  ef = 0;

    mode = MMIDC_GetCameraMode();
    if (CAMERA_MODE_DC == mode)
    {
        ef = MMIDC_GetPhotoEffect();
    }
    else
    {
        ef = MMIDC_GetVideoEffect();
    }
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForEffect ef=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_1990_112_2_18_2_8_51_240,(uint8*)"d", ef);

    if (ef < DC_EFFECT_NEGATIVE)
    {
        ef++;
    }
    else
    {
        ef = 0;
    }

    if (CAMERA_MODE_DC == mode)
    {
        MMIDC_SetPhotoEffect(ef);
    }
    else
    {
        MMIDC_SetVideoEffect(ef);
    }
}

/*****************************************************************************/
//Description :  shortcut for white balance
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForWhiteBalance(void)
{
    WHITE_BALANCE_E  wb = 0;
    
    wb = MMIDC_GetPhotoWhiteBalance();
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForWhiteBalance wb=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_2020_112_2_18_2_8_51_241,(uint8*)"d", wb);

    switch(wb)
    {
    case WHITE_BALANCE_AUTO:
        wb = WHITE_BALANCE_INCANDESCENCE;
        break;
        
    case WHITE_BALANCE_INCANDESCENCE:
        wb = WHITE_BALANCE_FLUORESCENT;
        break;
        
    case WHITE_BALANCE_FLUORESCENT:
        wb = WHITE_BALANCE_SUN;
        break;
        
    case WHITE_BALANCE_SUN:
        wb = WHITE_BALANCE_CLOUDY;
        break;
    
    case WHITE_BALANCE_CLOUDY:
        wb = WHITE_BALANCE_AUTO;
        break;
        
    default:    
        wb = WHITE_BALANCE_AUTO;
        break;
    }
    MMIDC_SetPhotoWhiteBalance(wb);
}

/*****************************************************************************/
//Description :  shortcut for delay time
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForDelayTime(void)
{
    SELF_SHOOT_E    self_time = 0;
    
    self_time = MMIDC_GetSelfShootDelayTime();    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForDelayTime self_time=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_2060_112_2_18_2_8_51_242,(uint8*)"d", self_time);
    if (self_time < (SELF_SHOOT_MAX -1))
    {
        self_time++;
    }
    else
    {
        self_time = 0;
    }    
    MMIDC_SetSelfShootDelayTime(self_time);
}

/*****************************************************************************/
//Description :  shortcut for store
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForStore(void)
{
    CAMERA_MODE_E      mode = 0;
    MMIFILE_DEVICE_E   dc_dev = 0;
    MMIFILE_DEVICE_E   device = MMI_DEVICE_UDISK;

	MMIDC_PHOTO_SIZE_E photo_size = MMIDC_GetPhotoSize();
	uint32 photo_bytes = 0;
	MMIFILE_DEVICE_E cur_file_dev = MMIDC_GetPhotoStorageDevice();
	MMIFILE_DEVICE_E suitable_dev = 0;

    //MMIFILE_DEVICE_E sd_type = 0;
    int8 count_dev_num = 0;

     mode = MMIDC_GetCameraMode();
        
    if (CAMERA_MODE_DC == mode)
    {
        dc_dev = MMIDC_GetPrefPhotoStorageDevice();
    }
    else
    {
        dc_dev = MMIDC_GetPrefVideoStorageDevice();
    }

    device = dc_dev;
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForStore dc_dev=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_2100_112_2_18_2_8_51_243,(uint8*)"d", dc_dev);
    
    //find next existed device
    do
    {   
        device++;

        //if DV not support UDISK
#if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)
        if ((CAMERA_MODE_DV == mode)
            && (MMI_DEVICE_UDISK == device)
            )
        {
            device++;
        }
#endif

        if (MMI_DEVICE_NUM <= device)
        {
            device = MMI_DEVICE_UDISK;
            
            //if DV not support UDISK
            #if defined (MMI_UDISK_MEM_ENABLE) && !defined (MMI_DV_UDISK_ENABLE)
                if (CAMERA_MODE_DV == mode)
                    //&& (MMI_DEVICE_UDISK == device)
                   // )
                {
                    device++;
                }
            #endif
        }



        if(MMIAPIFMM_GetDeviceTypeStatus(device))
        {
            //find next existed device
            if(device != dc_dev)
            {
                //next valid device exist
                dc_dev = device;
                break;
            }
        }
        
        count_dev_num++;
    }while(count_dev_num < (MMI_DEVICE_NUM - 1));


    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForStore dc_dev =%d, "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_2149_112_2_18_2_8_51_244,(uint8*)"d", dc_dev);
    {
        if(CAMERA_MODE_DC == mode)
        {
            photo_bytes = MMIDC_GetJPEGSizeByPhotoSize(photo_size);
			if(SFS_ERROR_NONE == MMIAPIFMM_GetSuitableFileDev(dc_dev, photo_bytes, &suitable_dev))
			{
				if(suitable_dev != dc_dev)
				{
					dc_dev = cur_file_dev;
					MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE); 
				}
			}

			if(dc_dev != MMIDC_GetPrefPhotoStorageDevice())
            {                         
                MMIDC_PostCreateFileHandleMsg();
            }
            MMIDC_SetPrefPhotoStorageDevice(dc_dev);
            
        }
        else
        {            
            MMIDC_SetPrefVideoStorageDevice(dc_dev);
        }
    }
}

/*****************************************************************************/
//Description :  shortcut for brightness to high
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForBrightnessToHigh(void)
{
    CAMERA_MODE_E  mode = 0;
    BRIGHTNESS_E   brightness = 0;

    mode = MMIDC_GetCameraMode();

    if ( CAMERA_MODE_DC == mode)
    {
        brightness = MMIDC_GetPhotoBrightness();        
        if (brightness < BRIGHTNESS_LEVEL_6)
        {
            MMIDC_SetPhotoBrightness((BRIGHTNESS_E)(brightness + 1));
        }
    }
    else
    {
        brightness = MMIDC_GetVideoBrightness();
        if (brightness < BRIGHTNESS_LEVEL_6)
        {
            MMIDC_SetVideoBrightness((BRIGHTNESS_E)(brightness + 1));
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForBrightnessToHigh brighness=%d, mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_2193_112_2_18_2_8_52_245,(uint8*)"dd", brightness, mode);

}  

/*****************************************************************************/
//Description :  shortcut for brightness to low
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForBrightnessToLow(void)
{
    CAMERA_MODE_E  mode = 0;
    BRIGHTNESS_E   brightness = 0;

    mode = MMIDC_GetCameraMode();
    
    if (CAMERA_MODE_DC == mode)
    {
        brightness = MMIDC_GetPhotoBrightness();
        if (brightness > BRIGHTNESS_LEVEL_0)
        {
            MMIDC_SetPhotoBrightness((BRIGHTNESS_E)(brightness - 1));
        }
    }
    else
    {
        brightness = MMIDC_GetVideoBrightness();
        if (brightness > BRIGHTNESS_LEVEL_0)
        {
            MMIDC_SetVideoBrightness((BRIGHTNESS_E)(brightness - 1));
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForBrightnessToLow brighness=%d, mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_2225_112_2_18_2_8_52_246,(uint8*)"dd", brightness, mode);
}   

/*****************************************************************************/
//Description :  shortcut for contrast to high
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForContrastToHigh(void)
{
    CAMERA_MODE_E  mode = 0;
    CONTRAST_E   contrast = 0;

    mode = MMIDC_GetCameraMode();

    if ( CAMERA_MODE_DC == mode)
    {
        contrast = MMIDC_GetContrast();        
        if (contrast < CONTRAST_6)
        {
            MMIDC_SetContrast((CONTRAST_E)(contrast + 1));
        }
    }
    else
    {
        contrast = MMIDC_GetVideoContrast();
        if (contrast < CONTRAST_6)
        {
            MMIDC_SetVideoContrast((CONTRAST_E)(contrast + 1));
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForContrastToHigh contrast=%d, mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_2256_112_2_18_2_8_52_247,(uint8*)"dd", contrast, mode);

}  

/*****************************************************************************/
//Description :  shortcut for contrast to low
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForContrastToLow(void)
{
    CAMERA_MODE_E  mode = 0;
    CONTRAST_E   contrast = 0;

    mode = MMIDC_GetCameraMode();
    
    if (CAMERA_MODE_DC == mode)
    {
        contrast = MMIDC_GetContrast();
        if (contrast > CONTRAST_0)
        {
            MMIDC_SetContrast((CONTRAST_E)(contrast - 1));
        }
    }
    else
    {
        contrast = MMIDC_GetVideoContrast();
        if (contrast > CONTRAST_0)
        {
            MMIDC_SetVideoContrast((CONTRAST_E)(contrast - 1));
        }
    }

    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForContrastToLow brighness=%d, mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_2288_112_2_18_2_8_52_248,(uint8*)"dd", contrast, mode);
}   

/*****************************************************************************/
//Description :  shortcut for zoom to high
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForZoomToHigh(void)
{
    CAMERA_MODE_E  mode = 0;

    mode = MMIDC_GetCameraMode();
    if (CAMERA_MODE_DC == mode)
    {
        MMIDC_IncreaseZoomValue();
    }
}

/*****************************************************************************/
//Description :  shortcut for zoom to low
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForZoomToLow(void)
{
    CAMERA_MODE_E  mode = 0;

    mode = MMIDC_GetCameraMode();
    if (CAMERA_MODE_DC == mode)
    {
        MMIDC_DecreaseZoomValue();
    }
}

/*****************************************************************************/
//Description :  shortcut for multi shot
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForMultiShot(void)
{
    MULTI_SHOOT_E    multi = 0;
    
    multi = MMIDC_GetMultiShootEnum();    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ShortcutForMultiShot multi=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_2330_112_2_18_2_8_52_249,(uint8*)"d", multi);
    if (multi < MULTI_SHOOT_6)
    {
        multi++;
    }
    else
    {
        multi = 0;
    }    
    MMIDC_SetMultiShootEnum(multi);
}

/*****************************************************************************/
//Description :  display icon desktop
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DisplayIconDesktop(void)
{
    BOOLEAN ret = FALSE;
    
    ret = DisplayIconDesktop();

    return ret;
}
/*****************************************************************************/
//Description :  clear icon desktop rect list
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearIconDesktopRectList(void)
{
    GUI_COLOR_T color = MMIDC_GetTransparentColor();
    GUI_RECT_T disp_rect = {0};
    int16 i = 0;
    int16 total_num = 0;

    total_num = s_gui_desktop_icon.icon_disp_number;;
    for (i = 0; i < total_num; i++)
    {
        disp_rect = s_gui_desktop_icon.icon_disp[i].rect;
        MMIDC_ClearRect(disp_rect, color);        
    }  
    
    InitIconDesktopDisplayRectList();
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_ClearIconDesktopRectList"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_2374_112_2_18_2_8_52_250,(uint8*)"");
}

/*****************************************************************************/
//Description :  close GUI icon desktop data
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseGUIIconDesktop(void)
{
    MMIDC_ClearIconDesktopRectList();
    InitGUIDesktopIcon();
    
    //SCI_TRACE_LOW:"[MMIDC] MMIDC_CloseGUIIconDesktop"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUIICONBAR_2386_112_2_18_2_8_52_251,(uint8*)"");
}

/*****************************************************************************/
//Description :   GUI icon desktop is open or not
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsIconDesktopOpen(void)
{
    BOOLEAN ret = FALSE;
    
    if (s_gui_desktop_icon.is_open)
    {
        ret = TRUE;
    }

    return ret;
}

/*****************************************************************************/
//Description :   open icon desktop
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenIconDesktop(void)
{
    InitGUIDesktopIcon();
    
    MMIDC_SetIconDesktopVisibility(TRUE);

    MMIDC_DisplayIconDesktop();
}

/*****************************************************************************/
//Description :   Open OSD menu
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenMoreOption(void)
{
    //close all menu
    MMIDC_CloseAllOSDMenu();

    MMIDC_OpenDesktopTipOSD();
}

/*****************************************************************************/
//Description : set icon desktop visible or not
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetIconDesktopVisibility(BOOLEAN is_visible)
{
    if (is_visible)   
    {
        s_gui_desktop_icon.is_open = TRUE;
    }
    else
    {
        s_gui_desktop_icon.is_open = FALSE;
    }
}

#if defined(MMIDC_F_DV_ZOOM)
/*****************************************************************************/
//Description : get zoom button rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetZoomButtonRect(GUI_RECT_T * rect_ptr)
{
    MMI_IMAGE_ID_T img_list[ICON_IMG_MAX] = {0};
    MMIDC_SIZE_ATTRIBUTE_T size_list[ICON_IMG_MAX] = {0};

    if(PNULL == rect_ptr)
    {
        return;
    }

    GetIconImageList(img_list, sizeof(img_list), size_list, sizeof(size_list));

    if(MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL)   
    {
        rect_ptr->top = ICON_ZOOM_H_Y;
        rect_ptr->left = ICON_ZOOM_H_X;
    }
    else
    {
        rect_ptr->top = ICON_ZOOM_V_Y;
        rect_ptr->left = ICON_ZOOM_V_X;
    }

    rect_ptr->bottom = rect_ptr->top + size_list[ICON_IMG_ZOOM].size_height;
    rect_ptr->right = rect_ptr->left + size_list[ICON_IMG_ZOOM].size_width;
    
}

/*****************************************************************************/
//Description : display zoom button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayZoomButton(void)
{
    CAMERA_MODE_E   mode = MMIDC_GetCameraMode();
    GUI_RECT_T  display_rect = {0};
    DISPLAY_DIRECT_E disp_direct = DISPLAY_DIRECT_HORIZONTAL;
    MMI_IMAGE_ID_T img_list[ICON_IMG_MAX] = {0};
    MMIDC_SIZE_ATTRIBUTE_T size_list[ICON_IMG_MAX] = {0};

     //clear last rect list
    MMIDC_ClearIconDesktopRectList();
   
    GetIconImageList(img_list, sizeof(img_list), size_list, sizeof(size_list));
    MMIDC_GetZoomButtonRect(&display_rect);    
    //display zoom bg
    DisplayIconZoom(mode, display_rect, disp_direct, 
                        &display_rect, img_list, size_list);

}
#endif 
#endif //#ifdef MMIDC_DESKTOP_ICON_BAR
#endif

