#include "mmi_app_camera_trc.h"
#ifdef CAMERA_SUPPORT

#if !defined(MMIDC_DESKTOP_PDA)
#ifdef WIN32
#include "std_header.h"
#endif

#include "mmi_default.h"
#include "mmidc_display.h"
#include "block_mem.h"
#include "guiblock.h"
#include "guifont.h"
#include "mmi_theme.h"
#include "mmidc_flow.h"
#include "mmidc_save.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"
#include "window_parse.h"
//#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_menutable.h"
#include "guimenu.h"
#include "mmi_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidc_export.h"
#include "mmidc_camera_text.h"
#include "mmidc_camera_id.h"
#include "mmidc_gui.h"
#include "mmidc_setting.h"
#include "mmidc_option.h"
#include "guires.h"
#include "mmidc_setting.h"
#include "mmiudisk_export.h"
#if defined(MMIDC_F_U_CAMERA) && defined(MMIDC_F_N_IN_ONE)
#include "mmidc_imageprocess.h"
#include "mmidc_ninone.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define				MAX_ITEMS	16
#define				MAX_VALUES	10


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

#define DISPLAY_ARROW_TOP

typedef struct 
{
    int8                                    _is_gray[MAX_ITEMS]; // item can not done
    GUI_COLOR_T                      _gray_item_color ;     //gray item text color
    GUI_COLOR_T                      _gray_item_bg_color; //gray item background color 
    

    int16					_start_position_x;
    int16                   _start_position_y;
    uint32					_menu_backgroud_color;
    int32					_focus_item_background_image;
    uint32					_focus_item_background_color;
    
    int32					_item_text[MAX_ITEMS];
    int32					_item_title;
    MENU_ITEM_CALL_BACK		_setting_call_back[MAX_ITEMS];
    int32					_menu_item_number;
    uint32					_text_backgroud_color;
    int32					_selected_text_background_image_data;
    uint32					_selected_text_background_color;
    int32					_selected_item_index;
    int32					_focused_item_index;
    DISPLAY_DIRECT_E		_display_mode;
    int32					_display_start_index;
    GUI_RECT_T				_menu_display_rect;
    int32					_display_number;
    int16                   _embed_display_number; //items are between arrows
    GUI_LCD_DEV_INFO		_lcd_dev_info;
    BOOLEAN					_is_open;
    BOOLEAN					_is_hide_selected_sign;
    BOOLEAN					_is_not_callback_focus;
}DC_MENU_ITEMS_DATA;

typedef struct 
{
    int16					_start_position_x;
    int16					_start_position_y;
    
    int32					_icon_image[MAX_ITEMS];
    int32					_icon_image_focus[MAX_ITEMS];
    ICON_CALL_BACK			_setting_call_back[MAX_ITEMS];
    ICON_CALL_BACK			_ok_call_back[MAX_ITEMS];
    int32					_icon_number;
    int32					_focused_item_index;
    DISPLAY_DIRECT_E		_display_mode;
    int32					_display_start_index;
    GUI_RECT_T				_icon_display_rect;
    int32					_display_number;
    uint8					_icon_display_width;
    uint8                   _focus_icon_display_width;
    uint8                   _focus_icon_display_height;
    BOOLEAN					_is_open;
}DC_ICON_DATA;

typedef struct 
{
    DC_MENU_ITEMS_OP			_menu_op;
    DC_MENU_ITEMS_DATA		    _menu_data;

    DC_ICONS_OP				    _icon_op;
    DC_ICON_DATA				_icon_data;
}DC_GUI_DATA_STRUCT;

static DC_GUI_DATA_STRUCT*        s_gui_data = PNULL;
static DC_MENU_ITEMS_DATA*        s_menu_data = PNULL;
static DC_ICON_DATA*              s_icon_data = PNULL;


/* menu member functions begin */

/*****************************************************************************/
// 	Description : clear menu rect
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: for touch panel
/*****************************************************************************/
LOCAL void MenuClearRect(void)
{
    GUI_COLOR_T      color = MMIDC_GetTransparentColor();

    if (PNULL != s_menu_data)
    {
        MMIDC_ClearRect(s_menu_data->_menu_display_rect, color);
    }
}

/*****************************************************************************/
// 	Description : close the menu
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: for touch panel
/*****************************************************************************/
LOCAL void MenuClose(void)
{
    MENU_ITEM_CALL_BACK callback = PNULL;
    
    if(s_menu_data->_is_open)
    {
        if(s_menu_data->_selected_item_index != s_menu_data->_focused_item_index)
        {
            callback = s_menu_data->_setting_call_back[s_menu_data->_selected_item_index];
            if(callback != PNULL && !s_menu_data->_is_not_callback_focus)
            {
                callback();
            }
        }
        else
        {           
            if(TXT_DC_CIRCLE == s_menu_data->_item_title)
            {
                MMIDC_SetFrameIndex((FRAME_E)s_menu_data->_selected_item_index, FALSE);
            }
        }
        
        s_menu_data->_menu_display_rect.top --;
        MenuClearRect();
            
        SCI_MEMSET(s_menu_data, 0, sizeof(DC_MENU_ITEMS_DATA));
        s_menu_data->_is_open = FALSE;
        
        MMIDC_ResetScrollNumber();      
    }
}

/*****************************************************************************/
// 	Description : inti menu and return menu handle
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_MENU_ITEMS_OP* MMIDC_InitMenu(void)
{
    int32 lcd_size = MMIDC_GetLCDSize();
    int16 display_number = 0;
    
    if(s_menu_data->_is_open)
    {
        MenuClose();
    }
    else
    {
        SCI_MEMSET(s_menu_data, 0, sizeof(DC_MENU_ITEMS_DATA));
    }

    if(MMIDC_QCIF_PREVIEW_HEIGHT == lcd_size)
    {
        display_number = 5;
    }
    else
    {
        display_number = 5;
    }
    
    s_menu_data->_display_number = display_number;
#ifdef DISPLAY_ARROW_TOP    
    s_menu_data->_embed_display_number = display_number - 2;
#else
    s_menu_data->_embed_display_number = display_number;
#endif

    MMIDC_CloseAdjust();
    s_menu_data->_is_open = TRUE;
    
    return &s_gui_data->_menu_op;
}

/*****************************************************************************/
// 	Description : is the menu open
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: for touch panel
/*****************************************************************************/
LOCAL void MenuSetDisplayNumber(uint32 number)
{
    s_menu_data->_display_number = number;
}

/*****************************************************************************/
// 	Description : is the menu open
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: for touch panel
/*****************************************************************************/
LOCAL BOOLEAN MenuIsOpen(void)
{
    return s_menu_data->_is_open;
}

/*****************************************************************************/
// 	Description : get menu handle
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_MENU_ITEMS_OP* MMIDC_GetMenuHandle(void)
{
    SCI_ASSERT(PNULL != &s_gui_data->_menu_op); /*assert verified*/
    return &s_gui_data->_menu_op;
}

/*****************************************************************************/
// 	Description : set item text start position
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void MenuSetStartPositionX(uint32 x)
{
    s_menu_data->_start_position_x = x;
}

/*****************************************************************************/
// 	Description : set item image start position
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void MenuSetStartPositionY(uint32 y)
{
    s_menu_data->_start_position_y = y;
}

/*****************************************************************************/
// 	Description : set item text background color
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void MenuSetMenuBackGroudColor(uint32 color)
{
    s_menu_data->_menu_backgroud_color = color;
}

/*****************************************************************************/
// 	Description : set selected item text background image
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void MenuSetFocusItemBackGroundImage(int32 image_data)
{
    s_menu_data->_focus_item_background_image = image_data;
}

/*****************************************************************************/
// 	Description : set selected item text background color
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void MenuSetFocusItemBackGroundColor(uint32 color)
{
    s_menu_data->_focus_item_background_color = color;
}

/*****************************************************************************/
// 	Description : set setting callback function of menu item
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void MenuSetItemCallback(int32 index, MENU_ITEM_CALL_BACK call_back)
{
    if (index >= s_menu_data->_menu_item_number)
    {
        //SCI_TRACE_LOW:"[MMIDC] MenuSetItemCallback index is overflow !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUI_286_112_2_18_2_8_43_219,(uint8*)"");
        return;
    }
    
    s_menu_data->_setting_call_back[index] = call_back;
}

/*****************************************************************************/
// 	Description : append menu item text
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void MenuAppendItemText(int32 text_data)
{
    s_menu_data->_item_text[s_menu_data->_menu_item_number] = text_data;
    s_menu_data->_menu_item_number ++;
}

/*****************************************************************************/
// 	Description : set  menu item gray text
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
LOCAL void MenuSetItemGray(int32 index)
{
    s_menu_data->_is_gray[index] = TRUE;
}
/*****************************************************************************/
// 	Description : set menu item text
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void MenuSetItemText(int32 index, int32 text_data)
{
    if (index >= s_menu_data->_menu_item_number)
    {
        //SCI_TRACE_LOW:"[MMIDC] MenuSetItemText index is overflow !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUI_319_112_2_18_2_8_43_220,(uint8*)"");
        return;
    }
    
    s_menu_data->_item_text[index] = text_data;
}

/*****************************************************************************/
// 	Description : set menu item title
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void MenuSetMenuTitle(int32 text_data)
{
    s_menu_data->_item_title = text_data;
}

/*****************************************************************************/
// 	Description : select the item
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void MenuSelectItem(uint32 index)
{
    s_menu_data->_selected_item_index = index;
}

/*****************************************************************************/
// 	Description : focus the item
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void MenuFocusItem(uint32 index)
{
    int16  display_number = s_menu_data->_display_number;

    s_menu_data->_focused_item_index = index;

    //if display arrow, then change display number
    if(s_menu_data->_menu_item_number > display_number)
    {
        display_number = s_menu_data->_embed_display_number;
    }
    
    if(s_menu_data->_focused_item_index >= display_number)
    {
        s_menu_data->_display_start_index = s_menu_data->_focused_item_index - (display_number - 1);
    }
}

/*****************************************************************************/
// 	Description : select next item
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void MenuFocusNextItem(void)
{
    MENU_ITEM_CALL_BACK callback = PNULL;
    int16  display_number = s_menu_data->_display_number;

    
    if(s_menu_data->_focused_item_index < s_menu_data->_menu_item_number - 1)
    {
        s_menu_data->_focused_item_index ++;

        //if display arrow, then change display number
        if(s_menu_data->_menu_item_number > display_number)
        {
            display_number = s_menu_data->_embed_display_number;
        }

        if(s_menu_data->_focused_item_index - (int32)s_menu_data->_display_start_index > display_number - 1)
        {
            s_menu_data->_display_start_index = s_menu_data->_focused_item_index - (display_number - 1);
        }
    }
    else
    {
        s_menu_data->_focused_item_index = 0;
        s_menu_data->_display_start_index = 0;
    }
    callback = s_menu_data->_setting_call_back[s_menu_data->_focused_item_index];
    if(callback != PNULL && !s_menu_data->_is_not_callback_focus)
    {
        callback();
    }
}

/*****************************************************************************/
// 	Description : select previous item
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void MenuFocusPreviousItem(void)
{
    MENU_ITEM_CALL_BACK callback = PNULL;
    int16  display_number = s_menu_data->_display_number;
    
    if(s_menu_data->_focused_item_index > 0)
    {
        s_menu_data->_focused_item_index --;
        if(s_menu_data->_focused_item_index < s_menu_data->_display_start_index)
        {
            s_menu_data->_display_start_index = s_menu_data->_focused_item_index;
        }
    }
    else
    {
        s_menu_data->_focused_item_index = s_menu_data->_menu_item_number - 1;
        //if display arrow, then change display number
        if(s_menu_data->_menu_item_number > display_number)
        {
            display_number = s_menu_data->_embed_display_number;
        }

        s_menu_data->_display_start_index = s_menu_data->_focused_item_index - (display_number - 1);
    }
    callback = s_menu_data->_setting_call_back[s_menu_data->_focused_item_index];
    if(callback != PNULL && !s_menu_data->_is_not_callback_focus)
    {
        callback();
    }
}



/*****************************************************************************/
// 	Description : display and handle tp menu items
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
//   Modifer:robert.wang
//              display bmp mask for vector font
/*****************************************************************************/
LOCAL BOOLEAN MenuDisplayAndTP(BOOLEAN is_tp, uint32 x, uint32 y)
{
    MMI_STRING_T text_str = {0};
    GUI_RECT_T   rect = {0};
    uint16       text_height = 0;
    uint16       text_width = 0;

    int32  i = 0;
    uint16 max_menu_width = 0;
    uint16 menu_width = 0;  // 113
    uint16 item_height = MMIDC_ITEM_TEXT_HEIGHT;//28 ; // 24;  

    DC_MENU_ITEMS_DATA* data_ptr = s_menu_data;
    GUI_POINT_T point = {0};

    uint16 display_number = data_ptr->_display_number;

    uint16 arrow_width = 0;//default 11
    uint16 arrow_height = 0; //default  11;
    uint16  radio_icon_height = 0; //default  13;
    uint16  radio_icon_width = 0 ; //default 13;

    int32   adjust_start_text = 3; //betweeen radio icon and text space
#if !defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_ADD_SPECIAL_EFFECT
    int32   adjust_top_space = 1 ;// between  up arrow and active bar
#endif
    MMI_STRING_T  title_str = {0};
    uint16 title_str_width = 0;
    int16  screen_width = MMIDC_GetPreviewWidth();
    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();
    int16  key_width = MMIDC_GetMaxWidthOfSoftkey();
    int16  menu_margin = MMIDC_MENU_MARGIN;
    int16  start_x = 0;
    BOOLEAN is_display_arrow = TRUE;
#ifdef DISPLAY_ARROW_TOP    
    BOOLEAN is_display_arrow_top = TRUE; //top or right
#else
    BOOLEAN is_display_arrow_top = FALSE; //top or right
#endif
#if defined(MMIDC_F_U_CAMERA) && defined(MMIDC_F_N_IN_ONE)
	GUI_POINT_T     start_point = MMIDC_GetNInOneRectStartPoint();	
#endif

    BOOLEAN ret = FALSE;

    if(!data_ptr->_is_open)
    {
        return FALSE;
    }

    point.x = x;
    point.y = y;

#if defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_MINI_3264_STYLE	
	if (DV_REVIEW_MODE == MMIDC_GetCurrentMode())
    {
        MMIDC_DisplayLastFrame();
        MMIDC_DisplayCurrentFileName();
		MMIDC_ShowDVReviewSoftKey();
    }
#endif
	
    //get resources width or height
#if !defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_ADD_SPECIAL_EFFECT
    GUIRES_GetImgWidthHeight(&arrow_width,&arrow_height,IMG_DC_MENU_DOWN,MMIDC_MAIN_WIN_ID);
#endif
    GUIRES_GetImgWidthHeight(&radio_icon_width,&radio_icon_height,IMAGE_RADIO_UNSELECTED_ICON,MMIDC_MAIN_WIN_ID);

    //adjust  display number 
    if(data_ptr->_menu_item_number <= display_number)
    {
        display_number = data_ptr->_menu_item_number;
        data_ptr->_display_number = display_number;
        is_display_arrow = FALSE;
    }
    else
    {
        display_number = data_ptr->_embed_display_number;
        
    }

    //get max display width to lcd screen
    if ((MMIDC_LAYOUT_RIGHTWARD == layout_style)
        || (MMIDC_LAYOUT_LEFTWARD == layout_style)
       )
    {
        max_menu_width = screen_width - key_width;
    }
    else
    {
        max_menu_width = screen_width;
    }
    max_menu_width -= 2 * menu_margin;
    
    //get title text width for adjusting  total width
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)data_ptr->_item_title, &title_str);	
    title_str_width = MMIDC_GetStringWidth(&title_str);

    //get max text width
    menu_width = title_str_width;
    for(i = 0; i < data_ptr->_menu_item_number; i ++)
    {
        if (TXT_COMM_SIZE == data_ptr->_item_title)
        {
            MMIDC_GetSizeString(data_ptr->_item_text[i], &text_str);
        }
        else
        {
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)data_ptr->_item_text[i], &text_str);
        }
        text_width = MMIDC_GetStringWidth(&text_str);
        menu_width = MAX(text_width, menu_width);
    }

    //get max menu item width, 
    //include radion icon, arrow, text, and adjust space, text right space
    menu_width += radio_icon_width + 2*adjust_start_text + adjust_start_text;


    //arrow icon display to right
    if (!is_display_arrow_top)  /*lint !e774*/
    {
        if (is_display_arrow)
        {
            menu_width += arrow_width;
        }
    }

    
    if(menu_width > max_menu_width)
    {
        menu_width = max_menu_width;
    }

    //get left leftward layout
    if (MMIDC_LAYOUT_LEFTWARD == layout_style)
    {
        start_x = menu_margin + key_width;
    }
    else
    {
        start_x = menu_margin;
    }    

    //convert relative coordinate to physical coordinate
    //if the menu exceed the width of screen,
    //then adjust start x
    if((data_ptr->_start_position_x + menu_width - 1) > (start_x + max_menu_width - 1))
    {
        data_ptr->_start_position_x = start_x + max_menu_width -1 - menu_width + 1;
    }

    //again adjust x
    if (MMIDC_LAYOUT_LEFTWARD == layout_style)
    {
        if (data_ptr->_start_position_x < start_x)
        {
            data_ptr->_start_position_x = start_x;
        }
    }
    else
    {
        if (data_ptr->_start_position_x < 0)
        {
            data_ptr->_start_position_x = 0;
        }
    }
    
    //menu rect, include icon ,sting, arrow rect
    //display background color,
#if !defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_ADD_SPECIAL_EFFECT
    rect.top = data_ptr->_start_position_y - item_height * (display_number + 1) + 1;
#else
    rect.top = data_ptr->_start_position_y - item_height * display_number + 1;
#endif

    //arrow icon display to top
    if (is_display_arrow_top) /*lint !e774*/
    {
        if (is_display_arrow)
        {
            rect.top -= 2*arrow_height;
        }
    }
    
    if(rect.top < 0)
    {
        rect.top = 0;
    }
    rect.bottom = data_ptr->_start_position_y;
    rect.left = data_ptr->_start_position_x;
    if(rect.left < 0)
    {
        rect.left = 0;
    }
    rect.right = rect.left + menu_width - 1;
#if defined(MMIDC_F_U_CAMERA) && defined(MMIDC_F_N_IN_ONE)	
	if(MMIDC_IsUCameraEnable() && MMIDC_IsNInOneEnable())
	{
		if(rect.right > start_point.x)
		{
			rect.right = start_point.x - 1;
			if(menu_width <= rect.right)
			{
				rect.left = rect.right - menu_width + 1;
				data_ptr->_start_position_x = rect.left;
			}
			else
			{
				rect.left = 1;
				menu_width = rect.right - rect.left + 1;
				data_ptr->_start_position_x = rect.left;
			}
		}
	}
#endif 

    if(!is_tp)
    {
        //store menu rect, include icon and string
        SCI_MEMCPY(&(data_ptr->_menu_display_rect), &rect, sizeof(GUI_RECT_T));

        //display  bmp mask, include title, arrows, items
        MMIDC_DisplayBMPMask(rect.left,rect.top,rect);
    }
    else
    {
        //if point is in menu rectangle,then osd menu is keeped to display,
        //and do not be closed.
        if (GUI_PointIsInRect(point, rect))
        {
            ret = TRUE;
        }
    }


    //adjust rect for displaying text
    //display all items
    if(s_menu_data->_is_hide_selected_sign)
    {
        rect.left = data_ptr->_start_position_x;
    }
    else
    {
        rect.left = data_ptr->_start_position_x + radio_icon_width + 2*adjust_start_text - 1;
    }

    if (is_display_arrow_top) /*lint !e774*/
    {
        if (is_display_arrow)
        {
            rect.top += arrow_height - 1;
        }
    }
    else
    {
        if (is_display_arrow)
        {
            rect.right = rect.right - arrow_width + 1;
        }
    }
    
    rect.top = rect.top - item_height ;
    for(i = 0; i < display_number; i ++)
    {
        rect.top = rect.top + item_height ;
        rect.bottom = rect.top + item_height - 1;
        
        if(is_tp)
        {
            rect.left = data_ptr->_start_position_x;
            if(GUI_PointIsInRect(point, rect))
            {
                MENU_ITEM_CALL_BACK callback = PNULL;
                
                if(data_ptr->_focused_item_index == i + data_ptr->_display_start_index)
                {
                    if(data_ptr->_is_gray[i + data_ptr->_display_start_index])
                    {
                        ;
                    }
                    else
                    { 
                        data_ptr->_selected_item_index = data_ptr->_focused_item_index;
                        callback = data_ptr->_setting_call_back[data_ptr->_focused_item_index];
                        if(callback != PNULL)
                        {
                            callback(); //here is callback function,it may be send command to dc driver.
                            if(TXT_DC_CIRCLE == s_menu_data->_item_title)
                            {
                                int32 index = s_menu_data->_selected_item_index;
                                
                                //NO frame
                                //if (FRAME_DISABLE == index)
                                //{
                                //    ;//NO respond
                                //}
                                //else
                                {
                                    //if(MMIDC_NeedReStartPreview(MMIDC_GePhotoSizetVerLcd()) && 
                                        //MMIDC_GetScreenMode() == SCREEN_MODE_VERTICAL ||
                                        //MMIDC_NeedReStartPreview(MMIDC_GePhotoSizetHorLcd()) && 
                                        //MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL)
                                    {
                                                                        
                                        
                                        MMIDC_CloseAllOSDMenu();
                                        
                                        MMIDC_FlowFunction(DC_EXIT);
                                        if(SCREEN_MODE_HORIIZONTAL == MMIDC_GetScreenMode())
                                        {
                                            MMIAPIDC_SetPhotoSize(MMIDC_GePhotoSizetHorLcd());
                                        }
                                        else
                                        {
                                            MMIAPIDC_SetPhotoSize(MMIDC_GePhotoSizetVerLcd());
                                        }
                                        MMIDC_FlowStart();
                                        MMIDC_SetFrameIndex((FRAME_E)index, FALSE);
                                    }
                                    //else
                                    //{
                                    //    MMIDC_SetFrameIndex((FRAME_E)s_menu_data->_selected_item_index, FALSE);
                                    //}
                                }// end no frame
                            }// end TXT_DC_CIRCLE == s_menu_data->_item_title
                        }
                        
                        //MMIDC_ClearMenuInPreview();
                    }//end else !gray
                }
                else
                {
                    data_ptr->_focused_item_index = i + data_ptr->_display_start_index;
                    callback = data_ptr->_setting_call_back[data_ptr->_focused_item_index];
                    if(callback != PNULL && !data_ptr->_is_not_callback_focus)
                    {
                        callback();
                    }
                }
                return TRUE;
            }
        }
        else
        {
            GUI_RECT_T rect2 = {0};
            GUI_RECT_T rect3 = {0};

            if (TXT_COMM_SIZE == s_menu_data->_item_title)
            {
                MMIDC_GetSizeString(data_ptr->_item_text[i + data_ptr->_display_start_index], &text_str);
            }
            else
            {
               MMI_GetLabelTextByLang((MMI_TEXT_ID_T)data_ptr->_item_text[i + data_ptr->_display_start_index], &text_str);
            }
            
            if(data_ptr->_display_start_index + i == data_ptr->_focused_item_index)
            {                
                rect3.left = data_ptr->_start_position_x;
                rect3.top = rect.top ;
                rect3.right = rect.right;
                rect3.bottom = rect.bottom ;
                
                MMIDC_ClearRect(rect3, s_menu_data->_focus_item_background_color);
            }

            text_height = MMIDC_GetStringHeight(&text_str);
            rect3.left = rect.left;
            rect3.top = rect.top + (item_height - text_height)/2;
            rect3.right = rect.right;
            rect3.bottom = rect.bottom ;//- (item_height - text_height)/2;

            //is gray  
            if(data_ptr->_is_gray[data_ptr->_display_start_index + i])
            {
                MMIDC_DisplayGrayString(rect3, &text_str, MMIDC_GetTransparentColor(), FALSE);
            }else if(data_ptr->_display_start_index + i == data_ptr->_focused_item_index)
            {
                MMIDC_DisplayString(rect3, &text_str, s_menu_data->_focus_item_background_color, TRUE);
            }
            else
            {             
                MMIDC_DisplayString(rect3,&text_str, MMIDC_GetTransparentColor(), FALSE);
            }

            rect2.top = rect.top + (item_height - radio_icon_height)/2;
            IMG_EnableTransparentColor(TRUE);
            if(!s_menu_data->_is_hide_selected_sign)
            {
                rect2.left = data_ptr->_start_position_x + adjust_start_text;
                if(data_ptr->_selected_item_index == i + data_ptr->_display_start_index)
                {
                    MMIDC_DisplayBmp(rect2.left, rect2.top, IMAGE_RADIO_SELECTED_ICON);
                }
                else
                {
                    MMIDC_DisplayBmp(rect2.left, rect2.top, IMAGE_RADIO_UNSELECTED_ICON);
                }
            }
            IMG_EnableTransparentColor(FALSE);
        }
    }
    
    // display up and down arrows
#if !defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_ADD_SPECIAL_EFFECT
    IMG_EnableTransparentColor(TRUE);
    if(is_display_arrow)
    {
        //arrow icon display to top
        if (is_display_arrow_top) /*lint !e774*/
        {
            //down arrow
            rect.left = data_ptr->_start_position_x + (menu_width - arrow_height) / 2;
            rect.top = data_ptr->_start_position_y - arrow_height - item_height - adjust_top_space + 1;
            if(is_tp)
            {
                //menu箭头区域太小，触笔很难精确的点到，扩大响应范围
                rect.bottom = rect.top + arrow_height - 1 + 3;
                
                rect.left = data_ptr->_start_position_x;
                rect.right = rect.left + menu_width - 1;
                rect.top -= 1;
                if (rect.top < 0)
                {
                    rect.top = 0;
                }
                if(GUI_PointIsInRect(point, rect))
                {
                    MenuFocusNextItem();
                    MenuDisplayAndTP(FALSE, 0, 0);
                    return TRUE;
                }
            }
            else
            {
                MMIDC_DisplayBmp(rect.left, rect.top, IMG_DC_MENU_DOWN);
            }

            //up arrow
            rect.top = data_ptr->_start_position_y - (item_height * display_number) - 2*arrow_height - item_height - 3*adjust_top_space + 1;

            if(is_tp)
            {
                //menu箭头区域太小，触笔很难精确的点到，扩大响应范围
                rect.bottom = rect.top + arrow_height - 1 + 1;
                
                rect.left = data_ptr->_start_position_x;            
                rect.right = rect.left + menu_width - 1;
                rect.top -= 6;
                if (rect.top < 0)
                {
                    rect.top = 0;
                }
                if(GUI_PointIsInRect(point, rect))
                {
                    MenuFocusPreviousItem();
                    MenuDisplayAndTP(FALSE, 0, 0);
                    return TRUE;
                }
            }
            else
            {
                MMIDC_DisplayBmp(rect.left, rect.top, IMG_DC_MENU_UP);
            }
        }
        else
        {
            //arrow icon display to right
            //down arrow
            rect.left = data_ptr->_start_position_x + menu_width - arrow_width;
            rect.top = data_ptr->_start_position_y - item_height - arrow_height + 1;
            if(is_tp)
            {
                //menu箭头区域太小，触笔很难精确的点到，扩大响应范围
                rect.right = rect.left + arrow_width - 1;
                rect.bottom = rect.top + arrow_height - 1;
                
                rect.left -= 3;
                rect.right +=3;
                rect.top -= 5;
                if (rect.top < 0)
                {
                    rect.top = 0;
                }
                rect.bottom +=3;
                if(GUI_PointIsInRect(point, rect))
                {
                    MenuFocusNextItem();
                    MenuDisplayAndTP(FALSE, 0, 0);
                    return TRUE;
                }
            }
            else
            {
                MMIDC_DisplayBmp(rect.left, rect.top, IMG_DC_MENU_DOWN);
            }

            //up arrow
            rect.top = data_ptr->_start_position_y - item_height * (display_number + 1) - adjust_top_space + 1;

            if(is_tp)
            {
                //menu箭头区域太小，触笔很难精确的点到，扩大响应范围
                rect.right = rect.left + arrow_width - 1;
                rect.bottom = rect.top + arrow_height - 1;
                
                rect.left -= 3;
                rect.right +=3;
                rect.top -= 2;
                if (rect.top < 0)
                {
                    rect.top = 0;
                }
                rect.bottom +=5;

                if(GUI_PointIsInRect(point, rect))
                {
                    MenuFocusPreviousItem();
                    MenuDisplayAndTP(FALSE, 0, 0);
                    return TRUE;
                }
            }
            else
            {
                MMIDC_DisplayBmp(rect.left, rect.top, IMG_DC_MENU_UP);
            }
        }
    }
    IMG_EnableTransparentColor(FALSE);

    //display menu title
    rect.top = data_ptr->_start_position_y - item_height +1;
    rect.bottom = rect.top + item_height -1;

    rect.left =data_ptr->_start_position_x;
    rect.right = rect.left + menu_width - 1;

    if(!is_tp)
    {
#ifndef MMIDC_MINI_RESOURCE_SIZE        
		MMIDC_DisplayTitleBackground(rect.left, rect.top,rect);
#else
		GUI_LCD_DEV_INFO lcd_dev_info = {0};
		LCD_FillRect(&lcd_dev_info, rect, MMIDC_MENU_TITLE_BACKGROUND);   
#endif

        text_height = MMIDC_GetStringHeight(&title_str);
        
        rect.top = rect.top + (item_height -text_height)/2;
        rect.left = rect.left + (menu_width - title_str_width) / 2 ;

        MMIDC_DisplayTitleString(rect, &title_str,MMIDC_GetTransparentColor(), FALSE);
    }
#endif

    return ret;
}
/*****************************************************************************/
// 	Description : display menu items
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void MenuDisplay(void)
{
    if(!s_menu_data->_is_open)
    {
        return;
    }
    MenuDisplayAndTP(FALSE, 0, 0);
}

/*****************************************************************************/
// 	Description : Handle touch panel Down
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: for touch panel
/*****************************************************************************/
LOCAL BOOLEAN MenuHandleTpDown(uint32 x, uint32 y)
{
    if(!s_menu_data->_is_open)
    {
        return FALSE;
    }
    return MenuDisplayAndTP(TRUE, x, y);
}

/*****************************************************************************/
// 	Description : Handle key Down
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: for touch panel
/*****************************************************************************/
LOCAL BOOLEAN MenuHandleKeyDown(DIRECTION_KEY key)
{
    BOOLEAN res = TRUE;
    MENU_ITEM_CALL_BACK callback = PNULL;
    DISPLAY_DIRECT_E mode = s_menu_data->_display_mode;
    int32  selected_index = 0;
    
    if(!s_menu_data->_is_open)
    {
        return FALSE;
    }
    switch(key)
    {
    case UP_KEY:
        if(DISPLAY_DIRECT_VERTICAL == mode)
        {
            MenuFocusPreviousItem();
        }
        break;
        
    case DOWN_KEY:
        if(DISPLAY_DIRECT_VERTICAL == mode)
        {
            MenuFocusNextItem();
        }
        break;
        
    case LEFT_KEY:
        if(DISPLAY_DIRECT_HORIZONTAL == mode)
        {
            MenuFocusPreviousItem();
        }
        break;
        
    case RIGHT_KEY:
        if(DISPLAY_DIRECT_HORIZONTAL == mode)
        {
            MenuFocusNextItem();
        }
        break;
        
    case OK_KEY:
       selected_index = s_menu_data->_focused_item_index;
       if(s_menu_data->_is_gray[selected_index])
       {
             ;
        }
        else
        {
            s_menu_data->_selected_item_index = selected_index;
            callback = s_menu_data->_setting_call_back[s_menu_data->_selected_item_index];
            if(callback != PNULL)
            {
                callback();
                if(TXT_DC_CIRCLE == s_menu_data->_item_title)
                {
                    int32 index = s_menu_data->_selected_item_index;

                    //NO frame
                    //if (FRAME_DISABLE == index)
                    //{
                    //    ;//NO respond
                    //}
                    //else
                    {
                        //if(MMIDC_NeedReStartPreview(MMIDC_GePhotoSizetVerLcd()) && 
                            //MMIDC_GetScreenMode() == SCREEN_MODE_VERTICAL ||
                            //MMIDC_NeedReStartPreview(MMIDC_GePhotoSizetHorLcd()) && 
                            //MMIDC_GetScreenMode() == SCREEN_MODE_HORIIZONTAL)
                        {

                            MMIDC_CloseAllOSDMenu();
                            MMIDC_FlowFunction(DC_EXIT);
                            
                            if(SCREEN_MODE_HORIIZONTAL == MMIDC_GetScreenMode() )
                            {
                                 MMIAPIDC_SetPhotoSize(MMIDC_GePhotoSizetHorLcd());
                            }
                            else
                            {  
                                MMIAPIDC_SetPhotoSize(MMIDC_GePhotoSizetVerLcd());
                            }

                            MMIDC_FlowStart();
                            MMIDC_SetFrameIndex((FRAME_E)index, FALSE);
                        }
                        //else
                        //{
                        //    MMIDC_SetFrameIndex((FRAME_E)s_menu_data->_selected_item_index, FALSE);
                        //}
                    }
                }
            }
        }
        break;
        
    case CANCEL_KEY:
        MenuClose();
        break;
        
    default:
        res = FALSE;
        break;
    }
    return res;
}

/*****************************************************************************/
// 	Description : set display mode
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: for touch panel
/*****************************************************************************/
LOCAL void MenuSetDisplayMode(DISPLAY_DIRECT_E display_mode)
{
    s_menu_data->_display_mode = display_mode;
}

/*****************************************************************************/
// 	Description : hide selected sign
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: for touch panel
/*****************************************************************************/
LOCAL void MenuHideSelectedSign(void)
{
    s_menu_data->_is_hide_selected_sign = TRUE;
}

/*****************************************************************************/
// 	Description : not callback when item get focus
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: for touch panel
/*****************************************************************************/
LOCAL void MenuSetNotCallbackFocus(void)
{
    s_menu_data->_is_not_callback_focus = TRUE;
}
/* menu member functions end */


/* icon member functions begin */
/*****************************************************************************/
// 	Description : init and get icon handle
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_ICONS_OP* MMIAPIDC_InitIcon(void)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE
    uint16 img_icon_width = 0;
    uint16 img_icon_height = 0;
    uint16 img_focus_icon_width = 0;
    uint16 img_focus_icon_height = 0;
    uint16 icon_space = 0;

    MMIDC_LAYOUT_E  layout_style = MMIDC_GetLayoutStyle();
    int16 screen_width = MMIDC_GetPreviewWidth();
    int16 key_width = 0;
    int16 display_number = 0;
    int16 area_width = 0;
    uint16 arrow_width = 0;
    uint16 arrow_height = 0;
    int16  icon_display_width = 0;
    int16  focus_icon_display_width = 0;

    MMIDC_InitAdjustData();
    
    SCI_MEMSET(s_icon_data, 0, sizeof(DC_ICON_DATA));

    key_width = MMIDC_GetMaxWidthOfSoftkey();       

    if( (MMIDC_LAYOUT_LEFTWARD == layout_style)
        || (MMIDC_LAYOUT_RIGHTWARD == layout_style)
       )
    {
        area_width = screen_width - key_width;
    }
    else
    {
        area_width = screen_width;
    }


    //get resources width or height
    GUIRES_GetImgWidthHeight(&img_icon_width,&img_icon_height,IMG_DC_SIZE,VIRTUAL_WIN_ID);
    GUIRES_GetImgWidthHeight(&img_focus_icon_width,&img_focus_icon_height,IMG_DC_SIZE_FOCUS,VIRTUAL_WIN_ID);
    GUIRES_GetImgWidthHeight(&arrow_width,&arrow_height,IMG_DC_LEFT_ARROW,VIRTUAL_WIN_ID);

    icon_space = MENU_ICON_SPACE;
    icon_display_width = img_icon_width + icon_space;
    focus_icon_display_width = img_focus_icon_width + icon_space;

    //display_number not include left and right arrow icon image
    display_number = (area_width - focus_icon_display_width - 2 * arrow_width) / icon_display_width;
    
    s_icon_data->_display_number = display_number + 1;

    s_icon_data->_is_open = TRUE;

    s_icon_data->_icon_display_width = icon_display_width;
    s_icon_data->_focus_icon_display_width = focus_icon_display_width;
    s_icon_data->_focus_icon_display_height = img_focus_icon_height;
    
    //此处应以逻辑屏的宽高以及控件的方向来判断,OSD图标的宽度需统一
    if(!MMIAPIDC_IsOpened())
    {
        s_icon_data->_display_number = 4;
        s_icon_data->_icon_display_width = 30 + 2;
        s_icon_data->_focus_icon_display_width = 30 + 2;
        s_icon_data->_focus_icon_display_height = 30;
    }
#endif
    return &s_gui_data->_icon_op;
}

/*****************************************************************************/
// 	Description : clear icon rect
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: for touch panel
/*****************************************************************************/
LOCAL void IconClearRect(void)
{
    GUI_COLOR_T color = MMIDC_GetTransparentColor();

    if (PNULL != s_icon_data)
    {
        MMIDC_ClearRect(s_icon_data->_icon_display_rect, color);
    }
}

/*****************************************************************************/
// 	Description : close the menu
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: for touch panel
/*****************************************************************************/
LOCAL void IconClose(void)
{
    if(s_icon_data->_is_open)
    {
        IconClearRect();
        
        SCI_MEMSET(s_icon_data, 0, sizeof(DC_ICON_DATA));
        s_icon_data->_is_open = FALSE;
    }
}

/*****************************************************************************/
// 	Description : is the menu open
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: for touch panel
/*****************************************************************************/
LOCAL BOOLEAN IconIsOpen(void)
{
    return s_icon_data->_is_open;
}

/*****************************************************************************/
// 	Description : get display icon numbers 
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: 
/*****************************************************************************/
LOCAL int32 IconGetDisplayNumber(void)
{
    return (MIN(s_icon_data->_display_number, s_icon_data->_icon_number));
}

/*****************************************************************************/
// 	Description : get icon width
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: 
/*****************************************************************************/
LOCAL int32 IconGetIconWidth(void)
{
    return s_icon_data->_icon_display_width;
}
/*****************************************************************************/
// 	Description : get focus icon width
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: 
/*****************************************************************************/
LOCAL int32 IconGetFocusIconWidth(void)
{
    return s_icon_data->_focus_icon_display_width;
}
/*****************************************************************************/
// 	Description : get focus icon height
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: 
/*****************************************************************************/
LOCAL int32 IconGetFocusIconHeight(void)
{
    return s_icon_data->_focus_icon_display_height;
}
/*****************************************************************************/
// 	Description : get icon handle
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_ICONS_OP* MMIDC_GetIconHandle(void)
{
    SCI_ASSERT(PNULL != &s_gui_data->_icon_op); /*assert verified*/
    return &s_gui_data->_icon_op;
}

/*****************************************************************************/
// 	Description : set icon start position x
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void IconSetStartPositionX(int32 x)
{
    s_icon_data->_start_position_x = x;
}

/*****************************************************************************/
// 	Description : set icon start position y
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void IconSetStartPositionY(int32 y)
{
    s_icon_data->_start_position_y = y;
}

/*****************************************************************************/
// 	Description : append icon image id
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void IconAppendImage(int32 image_data, int32 focus_image_data)
{
    s_icon_data->_icon_image[s_icon_data->_icon_number] = image_data;
    s_icon_data->_icon_image_focus[s_icon_data->_icon_number] = focus_image_data;
    s_icon_data->_icon_number ++;
}

/*****************************************************************************/
// 	Description : set icon image id
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void IconSetImage(int32 index, int32 image_data, int32 focus_image_data)
{
    if (index >= s_icon_data->_icon_number)
    {
        //SCI_TRACE_LOW:"[MMIDC] IconSetImage index is overflow !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUI_1311_112_2_18_2_8_46_221,(uint8*)"");
        return;
    }
    
    s_icon_data->_icon_image[index] = image_data;
    s_icon_data->_icon_image_focus[index] = focus_image_data;
}

/*****************************************************************************/
// 	Description : call back focus icon function 
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void IconCallBackFocusIcon(ICON_CALL_BACK callback)
{
    uint32 x = 0;
    uint32 y = 0;

    if (PNULL == callback)
    {
        //SCI_TRACE_LOW:"[MMIDC] IconCallBackFocusIcon is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUI_1330_112_2_18_2_8_46_222,(uint8*)"");
        return ;
    }
    
    if(DISPLAY_DIRECT_HORIZONTAL == s_icon_data->_display_mode)
    {
        y = s_icon_data->_start_position_y - 2;
        x = s_icon_data->_start_position_x + s_icon_data->_icon_display_width * (s_icon_data->_focused_item_index - s_icon_data->_display_start_index);
        callback(x, y);
    }
    else
    {
        x = s_icon_data->_start_position_x - 2;
        y = s_icon_data->_start_position_y + s_icon_data->_icon_display_width * (s_icon_data->_focused_item_index - s_icon_data->_display_start_index);
        callback(x, y);
    }
}

/*****************************************************************************/
// 	Description : move focus to next icon
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void IconFocusNext(void)
{
    ICON_CALL_BACK callback = PNULL;
    
    if(s_icon_data->_focused_item_index < s_icon_data->_icon_number - 1)
    {
        s_icon_data->_focused_item_index ++;
        if(s_icon_data->_focused_item_index - s_icon_data->_display_start_index > s_icon_data->_display_number - 1)
        {
            s_icon_data->_display_start_index = s_icon_data->_focused_item_index - (s_icon_data->_display_number - 1);
        }
    }
    else
    {
        s_icon_data->_focused_item_index = 0;
        s_icon_data->_display_start_index = 0;
    }
    callback = s_icon_data->_setting_call_back[s_icon_data->_focused_item_index];
    if(callback != PNULL)
    {
        IconCallBackFocusIcon(callback);
    }
}

/*****************************************************************************/
// 	Description : move focus to previous icon
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
void IconFocusPrevious(void)
{
    ICON_CALL_BACK callback = PNULL;
    
    if(s_icon_data->_focused_item_index > 0)
    {
        s_icon_data->_focused_item_index --;
        if(s_icon_data->_focused_item_index < s_icon_data->_display_start_index)
        {
            s_icon_data->_display_start_index = s_icon_data->_focused_item_index;
        }
    }
    else
    {
        s_icon_data->_focused_item_index = s_icon_data->_icon_number - 1;
        s_icon_data->_display_start_index = s_icon_data->_focused_item_index - (s_icon_data->_display_number - 1);
    }
    callback = s_icon_data->_setting_call_back[s_icon_data->_focused_item_index];
    if(callback != PNULL)
    {
        IconCallBackFocusIcon(callback);
    }
}

/*****************************************************************************/
// 	Description : handle icon key down
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IconHandleKeyDown(DIRECTION_KEY key)
{
    BOOLEAN res = TRUE;
    DISPLAY_DIRECT_E mode = s_icon_data->_display_mode;
    ICON_OK_CALL_BACK callback = PNULL;
    
    if(!s_icon_data->_is_open)
    {
        return FALSE;
    }
    switch(key)
    {
    case UP_KEY:
        if(DISPLAY_DIRECT_VERTICAL == mode)
        {
            IconFocusPrevious();
        }
        else
        {
            res = FALSE; 
        }
        break;
        
    case DOWN_KEY:
        if(DISPLAY_DIRECT_VERTICAL == mode)
        {
            IconFocusNext();
        }
        else
        {
            res = FALSE; 
        }
        break;
        
    case LEFT_KEY:
        if(DISPLAY_DIRECT_HORIZONTAL == mode)
        {
            IconFocusPrevious();
        }
        else
        {
            res = FALSE; 
        }
        break;
        
    case RIGHT_KEY:
        if(DISPLAY_DIRECT_HORIZONTAL == mode)
        {
            IconFocusNext();
        }
        else
        {
            res = FALSE; 
        }
        break;
        
    case CANCEL_KEY:
        IconClose();
        break;
        
    case OK_KEY:
        callback = (ICON_OK_CALL_BACK)s_icon_data->_ok_call_back[s_icon_data->_focused_item_index];
        if(callback != PNULL)
        {
            callback();
        }
        else
        {
            res = FALSE;
        }
        break;
        
    default:
        res = FALSE;
        break;
    }
    return res;
}

/*****************************************************************************/
// 	Description : select focus to the icon
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void IconFocus(int32 index)
{
    ICON_CALL_BACK callback = PNULL;
    
    if (index >= s_icon_data->_icon_number)
    {
        //SCI_TRACE_LOW:"[MMIDC] IconFocus index is overflow !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUI_1497_112_2_18_2_8_46_223,(uint8*)"");
        return ;
    }
    
    s_icon_data->_focused_item_index = index;
    callback = s_icon_data->_setting_call_back[s_icon_data->_focused_item_index];
    if(PNULL != callback)
    {
        IconCallBackFocusIcon(callback);
    }
}

/*****************************************************************************/
// 	Description : set callback function of the icon
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void IconSetCallback(int32 index, ICON_CALL_BACK call_back)
{
    if (index >= s_icon_data->_icon_number)
    {
        //SCI_TRACE_LOW:"[MMIDC] IconSetCallback index is overflow"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUI_1517_112_2_18_2_8_46_224,(uint8*)"");
        return ;
    }
    
    s_icon_data->_setting_call_back[index] = call_back;
}

/*****************************************************************************/
// 	Description : set callback function of the icon
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void IconSetOKCallback(int32 index, ICON_OK_CALL_BACK ok_call_back)
{
    if (index >= s_icon_data->_icon_number)
    {
        //SCI_TRACE_LOW:"[MMIDC] IconSetOKCallback index is overflow !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUI_1532_112_2_18_2_8_46_225,(uint8*)"");
        return ;
    }
    
    s_icon_data->_ok_call_back[index] = (ICON_CALL_BACK)ok_call_back;
}

/*****************************************************************************/
// 	Description : set icon display mode, such as hor and ver mode
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void IconSetDisplayMode(DISPLAY_DIRECT_E display_mode)
{
    s_icon_data->_display_mode = display_mode;
}

/*****************************************************************************/
// 	Description : display and handle tp icon
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IconDisplayAndTPVertical(BOOLEAN is_tp, uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE
    GUI_RECT_T rect = {0};
    int32 i = 0;
    int16 icon_width = s_icon_data->_icon_display_width;
    GUI_POINT_T point = {0};
    uint8 display_number = s_icon_data->_display_number;
    GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO dev_info_ptr = {0};
    
    if(!s_icon_data->_is_open)
    {
        return FALSE;
    }
    //s_icon_data->_display_number = 5;
    point.x = x;
    point.y = y;
    if(s_icon_data->_icon_number < display_number)
    {
        display_number = s_icon_data->_icon_number;
        s_icon_data->_display_number = display_number;
    }
    //store icon rect
    rect.top = s_icon_data->_start_position_y;
    rect.bottom = rect.top + icon_width * display_number + icon_width * 2;
    rect.left = s_icon_data->_start_position_x;
    rect.right = rect.left + icon_width;
    if(!is_tp)
    {
        SCI_MEMCPY(&(s_icon_data->_icon_display_rect), &rect, sizeof(GUI_RECT_T));
    }
    
    //display all items
    for(i = 0; i < display_number; i ++)
    {
        rect.top = s_icon_data->_start_position_y + icon_width * i;
        rect.bottom = rect.top + icon_width;
        
        if(is_tp)
        {
            if(GUI_PointIsInRect(point, rect))
            {
                if(s_icon_data->_focused_item_index == i + s_icon_data->_display_start_index)
                {
                    ICON_OK_CALL_BACK callback = (ICON_OK_CALL_BACK)s_icon_data->_ok_call_back[s_icon_data->_focused_item_index];
                    if(callback != PNULL)
                    {
                        callback();
                    }
                }
                else
                {
                    IconFocus(i + s_icon_data->_display_start_index);
                }
                return TRUE;
            }
        }
        else
        {
            //display all icons
            if(s_icon_data->_focused_item_index == i + s_icon_data->_display_start_index)
            {
                dis_point.x = rect.left;
                dis_point.y = rect.top;
                GUIRES_DisplayImg(&dis_point,
                    PNULL,
                    PNULL,
                    VIRTUAL_WIN_ID,
                    s_icon_data->_icon_image_focus[i + s_icon_data->_display_start_index],
                    &dev_info_ptr);
            }
            else
            {
                dis_point.x = rect.left;
                dis_point.y = rect.top;
                GUIRES_DisplayImg(&dis_point,
                    PNULL,
                    PNULL,
                    VIRTUAL_WIN_ID,
                    s_icon_data->_icon_image[i + s_icon_data->_display_start_index],
                    &dev_info_ptr);
            }
        }
    }
    
    // display left and right arrows
    rect.top = s_icon_data->_start_position_y - 24;
    rect.left += 3;
    if(is_tp)
    {
        rect.bottom = rect.top + icon_width;
        if(GUI_PointIsInRect(point, rect))
        {
            IconFocusPrevious();
            return TRUE;
        }
    }
    else
    {
        dis_point.x = rect.left;
        dis_point.y = rect.top;
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            VIRTUAL_WIN_ID,
            IMG_DC_UP_ARROW,
            &dev_info_ptr);
    }
    rect.top = s_icon_data->_start_position_y + icon_width * display_number;
    if(is_tp)
    {
        rect.bottom = rect.top + 24;
        if(GUI_PointIsInRect(point, rect))
        {
            IconFocusNext();
            return TRUE;
        }
    }
    else
    {
        dis_point.x = rect.left;
        dis_point.y = rect.top;
        GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            VIRTUAL_WIN_ID,
            IMG_DC_DOWN_ARROW,
            &dev_info_ptr);
    }

#endif //#ifndef MMIDC_MINI_RESOURCE_SIZE
    return FALSE;
}

/*****************************************************************************/
// 	Description : display and handle tp icon
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IconDisplayAndTP(BOOLEAN is_tp, uint32 x, uint32 y)
{
#ifndef MMIDC_MINI_RESOURCE_SIZE
    GUI_RECT_T rect = {0};
    int32 i = 0;
    int16 icon_width = IconGetIconWidth();
    int16 focus_icon_width = IconGetFocusIconWidth();
    int16 focus_icon_height = IconGetFocusIconHeight();
    int16 icon_display_width = 0;
    int16 focus_icon_offset_number = 0;
    GUI_POINT_T point = {0};
    uint8 display_number = s_icon_data->_display_number;
    uint16 arrow_width = 0;
    uint16 arrow_height = 0;
    
    if(!s_icon_data->_is_open)
    {
        return FALSE;
    }
    
    point.x = x;
    point.y = y;
    
    if(s_icon_data->_icon_number < display_number)
    {
        display_number = s_icon_data->_icon_number;
        s_icon_data->_display_number = display_number;
    }
    
    //get resources width or height
    GUIRES_GetImgWidthHeight(&arrow_width,&arrow_height,IMG_DC_LEFT_ARROW,MMIDC_MAIN_WIN_ID);
    
    //store icon rect
    rect.top = s_icon_data->_start_position_y;
    rect.bottom = rect.top + focus_icon_height - 1;
    rect.left = s_icon_data->_start_position_x - arrow_width;
    rect.right = rect.left + focus_icon_width + icon_width * (display_number - 1) + 2 * arrow_width - 1;
    if(!is_tp)
    {
        SCI_MEMCPY(&(s_icon_data->_icon_display_rect), &rect, sizeof(GUI_RECT_T));
    }
    
    
    focus_icon_offset_number = s_icon_data->_focused_item_index - s_icon_data->_display_start_index;
    //display all items
    for(i = 0; i < display_number; i ++)
    {
        if ( i > focus_icon_offset_number)
        {
            rect.left = s_icon_data->_start_position_x + focus_icon_width + icon_width * (i - 1);
        }
        else
        {
            rect.left = s_icon_data->_start_position_x + icon_width * i;
        }
        
        //focus icon
        if (s_icon_data->_focused_item_index == i + s_icon_data->_display_start_index)
        {
            icon_display_width = focus_icon_width;
        }
        else
        {
            icon_display_width = icon_width;
        }
        rect.right = rect.left + icon_display_width - 1;
        
        if(is_tp)
        {
            if(GUI_PointIsInRect(point, rect))
            {
                

                IconFocus(i + s_icon_data->_display_start_index);
                return TRUE;
            }
        }
        else
        {
            //display all icons
            if(s_icon_data->_focused_item_index == i + s_icon_data->_display_start_index)
            {
                MMIDC_DisplayBmp(rect.left, rect.top, 
                    s_icon_data->_icon_image_focus[i + s_icon_data->_display_start_index]);
            }
            else
            {
                MMIDC_DisplayBmp(rect.left, rect.top,
                    s_icon_data->_icon_image[i + s_icon_data->_display_start_index]);
            }
        }
    }
    
    // display left and right arrows
    rect.left = s_icon_data->_start_position_x - arrow_width + 1;

    if(is_tp)
    {
        rect.right = rect.left + icon_width -1;
        if(GUI_PointIsInRect(point, rect))
        {
            

            IconFocusPrevious();
            return TRUE;
        }
    }
    else
    {
        MMIDC_DisplayBmp(rect.left, rect.top, IMG_DC_LEFT_ARROW);
    }
    rect.left = s_icon_data->_start_position_x + focus_icon_width + icon_width * (display_number - 1) - 1;
    if(is_tp)
    {
        rect.right = rect.left+ icon_width -1;
        if(GUI_PointIsInRect(point, rect))
        {
            

            IconFocusNext();
            return TRUE;
        }
    }
    else
    {
        MMIDC_DisplayBmp(rect.left, rect.top, IMG_DC_RIGHT_ARROW);
    }
#endif    

    return FALSE;
}

/*****************************************************************************/
// 	Description : handle icon tp down
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IconHandleTpDown(uint32 x, uint32 y)
{
    BOOLEAN res = FALSE;

    if(!s_icon_data->_is_open)
    {
        return FALSE;
    }
    if(DISPLAY_DIRECT_HORIZONTAL == s_icon_data->_display_mode)
    {
        res = IconDisplayAndTP(TRUE, x, y);
    }
    else
    {
        res = IconDisplayAndTPVertical(TRUE, x, y);
    }
    return res;
}

/*****************************************************************************/
// 	Description : display icon
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
LOCAL void IconDisplay(void)
{
    if(DISPLAY_DIRECT_HORIZONTAL == s_icon_data->_display_mode)
    {
        IconDisplayAndTP(FALSE, 0, 0);
    }
    else
    {
        IconDisplayAndTPVertical(FALSE, 0, 0);
    }
}

/* icon member functions end */

/*****************************************************************************/
// 	Description : init menu and icon
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_FreeGUI(void)
{
    //SCI_TRACE_LOW:"[MMIDC]: MMIAPIDC_FreeGUI"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_GUI_1867_112_2_18_2_8_47_226,(uint8*)"");
    
    if(PNULL != s_gui_data)
    {
        SCI_FREE(s_gui_data);
        s_gui_data = PNULL;
        
        s_menu_data = PNULL;
        s_icon_data = PNULL;
    }
}

/*****************************************************************************/
// 	Description : init menu and icon
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIAPIDC_InitOSDMenuIcon(void)
{
    DC_ICONS_OP* icon_op = PNULL;
    DC_MENU_ITEMS_OP* menu_op = PNULL;

    if(PNULL == s_gui_data)
    {
        s_gui_data = (DC_GUI_DATA_STRUCT*)SCI_ALLOC_APP(sizeof(DC_GUI_DATA_STRUCT));
        SCI_MEMSET(s_gui_data, 0, sizeof(DC_GUI_DATA_STRUCT));
    }

    s_menu_data = &s_gui_data->_menu_data;
    s_icon_data = &s_gui_data->_icon_data;

    icon_op = &s_gui_data->_icon_op;
    menu_op = &s_gui_data->_menu_op;

    menu_op->MenuDisplay = MenuDisplay;
    menu_op->MenuHandleTpDown = MenuHandleTpDown;
    menu_op->MenuHandleKeyDown = MenuHandleKeyDown;
    menu_op->MenuClose = MenuClose;
    menu_op->MenuIsOpen = MenuIsOpen;
    menu_op->MenuSetDisplayNumber = MenuSetDisplayNumber;
    menu_op->MenuSetStartPositionX = MenuSetStartPositionX;
    menu_op->MenuSetStartPositionY = MenuSetStartPositionY;
    menu_op->MenuSetMenuBackGroudColor = MenuSetMenuBackGroudColor;
    menu_op->MenuSetFocusItemBackGroundImage = MenuSetFocusItemBackGroundImage;
    menu_op->MenuSetFocusItemBackGroundColor = MenuSetFocusItemBackGroundColor;
    menu_op->MenuAppendItemText = MenuAppendItemText;
    menu_op->MenuSetItemText = MenuSetItemText;
    menu_op->MenuSetMenuTitle = MenuSetMenuTitle;
    menu_op->MenuFocusPreviousItem = MenuFocusPreviousItem;
    menu_op->MenuFocusNextItem = MenuFocusNextItem;
    menu_op->MenuSelectItem = MenuSelectItem;
    menu_op->MenuFocusItem = MenuFocusItem;
    menu_op->MenuSetItemCallback = MenuSetItemCallback;
    menu_op->MenuSetDisplayMode = MenuSetDisplayMode;
    menu_op->MenuHideSelectedSign = MenuHideSelectedSign;
    menu_op->MenuSetNotCallbackFocus = MenuSetNotCallbackFocus;
    menu_op->MenuSetItemGray = MenuSetItemGray;
    menu_op->MenuClearRect = MenuClearRect;
    
    icon_op->IconDisplay = IconDisplay;
    icon_op->IconHandleTpDown = IconHandleTpDown;
    icon_op->IconHandleKeyDown = IconHandleKeyDown;	
    icon_op->IconClose = IconClose;
    icon_op->IconIsOpen = IconIsOpen;
    icon_op->IconSetStartPositionX = IconSetStartPositionX;
    icon_op->IconSetStartPositionY = IconSetStartPositionY;
    icon_op->IconAppendImage = IconAppendImage;
    icon_op->IconSetImage = IconSetImage;
    icon_op->IconFocusPrevious = IconFocusPrevious;
    icon_op->IconFocus = IconFocus;
    icon_op->IconSetOKCallback = IconSetOKCallback;
    icon_op->IconSetCallback = IconSetCallback;
    icon_op->IconSetDisplayMode = IconSetDisplayMode;
    icon_op->IconGetDisplayNumber = IconGetDisplayNumber;
    icon_op->IconGetIconWidth = IconGetIconWidth;
    icon_op->IconGetFocusIconWidth = IconGetFocusIconWidth;
    icon_op->IconGetFocusIconHeight = IconGetFocusIconHeight;
    icon_op->IconClearRect = IconClearRect;   


    return 0;
}

/*****************************************************************************/
// 	Description :  focused menu item is gray or not
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsFocusMenuGray(void)
{
    BOOLEAN is_gray = FALSE;
    
    if (s_menu_data->_is_gray[s_menu_data->_focused_item_index])
    {
        is_gray = TRUE;
    }

    return is_gray;
}
#endif
#endif  //#ifdef  CAMERA_SUPPORT

