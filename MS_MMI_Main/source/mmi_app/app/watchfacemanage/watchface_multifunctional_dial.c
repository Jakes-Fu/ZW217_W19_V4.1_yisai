
/*****************************************************************************
** File Name:      watchface_multifunctional_dial.c                          *
** Author:                                                                   *
** Date:           10/21/2021                                                *
** Copyright:      2020 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    watchface multifunctional dial win
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/21/2021   bin.wang1          Create
******************************************************************************/
#ifdef WATCHFACE_CTRL_EDIT_SUPPORT
#include "mmi_id.h"
#include "mmi_common.h"
#include "window_parse.h"
#include "mmk_type.h"
#include "mmk_timer.h"
#include "guistring.h"
#include "ui_layer.h"
#include "graphics_draw.h"
#include "mmi_text.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"

#include "watch_common.h"
#include "mmicom_trace.h"

#include "json_parser_watchface.h"
#include "watchface_ctrl_adapter.h"
#include "watch_common_list.h"
#include "watchface_config.h"
#include "watchface_multifunctional_dial.h"
#include "watchface_flow.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

#define WATCHFACE_MTF_MAINSCREEN_HEIGHT     DP2PX_VALUE(240)
#define WATCHFACE_MTF_MAINSCREEN_WIDTH      DP2PX_VALUE(240)
#define WATCHFACE_MTF_MAINSCREEN_RECT       DP2PX_RECT(0, 0, 240, 240)
#define WATCHFACE_MTF_BG_ZOOM_RATE          (0.8)

#define WATCHFACE_MTF_POSITION_LEFT_UP      DP2PX_RECT(34, 44, 106, 116)
#define WATCHFACE_MTF_POSITION_UP           DP2PX_RECT(84, 24, 156, 96)
#define WATCHFACE_MTF_POSITION_RIGHT_UP     DP2PX_RECT(124, 44, 196, 116)
#define WATCHFACE_MTF_POSITION_LEFT         DP2PX_RECT(24, 84, 96, 156)
#define WATCHFACE_MTF_POSITION_CENTER       DP2PX_RECT(61, 61, 169, 169)
#define WATCHFACE_MTF_POSITION_RIGHT        DP2PX_RECT(144, 84, 216, 156)
#define WATCHFACE_MTF_POSITION_LEFT_DOWN    DP2PX_RECT(34, 134, 106, 206)
#define WATCHFACE_MTF_POSITION_DOWN         DP2PX_RECT(84, 144, 156, 216)
#define WATCHFACE_MTF_POSITION_RIGHT_DOWN   DP2PX_RECT(124, 134, 196, 206)

#define WATCHFACE_MTF_POSITION_RATIO_1      (0.4)
#define WATCHFACE_MTF_POSITION_RATIO_2      (0.6)


/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/

typedef enum{
    WATCHFACE_MTF_BASE_POSITION_LEFT_UP = 0,
    WATCHFACE_MTF_BASE_POSITION_UP,
    WATCHFACE_MTF_BASE_POSITION_RIGHT_UP,
    WATCHFACE_MTF_BASE_POSITION_LEFT,
    WATCHFACE_MTF_BASE_POSITION_CENTER,
    WATCHFACE_MTF_BASE_POSITION_RIGHT,
    WATCHFACE_MTF_BASE_POSITION_LEFT_DOWN,
    WATCHFACE_MTF_BASE_POSITION_DOWN,
    WATCHFACE_MTF_BASE_POSITION_RIGHT_DOWN,
    MTF_DIAL_CONTROLER_POSITION_MAX
}WATCHFACE_MTF_BASE_POSITION;

typedef struct WATCHFACE_MTF_SOURCE_T
{
    MMI_IMAGE_ID_T              func_img_id;
    MMI_TEXT_ID_T               func_txt_id;
    uint8                       func_type[MAX_NAME_LEN+1];
}WATCHFACE_MTF_SOURCE;

typedef struct WATCHFACE_MTF_CONTROL_INFO_T
{
    MMI_IMAGE_ID_T              func_img_id;
    GUI_RECT_T                  item_rect;
    uint8                       func_type[MAX_NAME_LEN+1];  //每个控制项目前的功能类型
}WATCHFACE_MTF_CONTROL_INFO;


/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/

// TODO: use macro to control func
//手表特性资源组数，讨论后确定目前先使用固定资源同时通过Type和手表资源中的功能做对比
LOCAL WATCHFACE_MTF_SOURCE g_watchface_function_list_source[] = {
    {res_aw_clock_function_menu_ic_charge,         TXT_AW_MTF_LIST_CHARGE,  "power" },
    {res_aw_clock_function_menu_ic_heart_rate,     TXT_AW_MTF_LIST_HEART,   "pulse" },
    {res_aw_clock_function_menu_ic_step_number,    TXT_AW_MTF_LIST_STEP,    "steps" }
};

LOCAL WATCHFACE_MTF_SOURCE g_watchface_function_source[] = {
    {res_aw_clock_function_edit_charge,         TXT_NULL,  "power" },
    {res_aw_clock_function_edit_heart_rate,     TXT_NULL,  "pulse" },
    {res_aw_clock_function_edit_step_number,    TXT_NULL,  "steps" }
};

LOCAL GUI_RECT_T g_watchface_function_base_position[MTF_DIAL_CONTROLER_POSITION_MAX]={
    WATCHFACE_MTF_POSITION_LEFT_UP,
    WATCHFACE_MTF_POSITION_UP,
    WATCHFACE_MTF_POSITION_RIGHT_UP,
    WATCHFACE_MTF_POSITION_LEFT,
    WATCHFACE_MTF_POSITION_CENTER,
    WATCHFACE_MTF_POSITION_RIGHT,
    WATCHFACE_MTF_POSITION_LEFT_DOWN,
    WATCHFACE_MTF_POSITION_DOWN,
    WATCHFACE_MTF_POSITION_RIGHT_DOWN
};

LOCAL uint16                      s_watchface_item_num = 0;
LOCAL uint16                      s_watchface_selected_item_index = 0;        //用户选择的控制项索引
LOCAL uint16                      s_watchface_selected_watchface_index = 0;   //用户选择的表盘索引
LOCAL WATCHFACE_MTF_CONTROL_INFO* s_watchface_multifunction_info = PNULL;
LOCAL BOOLEAN                     s_is_change_multi = FALSE;
/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
LOCAL BOOLEAN InRect( GUI_POINT_T point, GUI_RECT_T rect )
{
    if( (point.x>rect.left)
        &&(point.x<rect.right)
        &&(point.y>rect.top)
        &&(point.y<rect.bottom) )
    {
        return TRUE;
    }
    return FALSE;
}

LOCAL BOOLEAN ZoomImg( GUIIMG_BITMAP_T*  p_data_info, uint32 size, GUIIMG_BITMAP_T* p_zoom_data_info )
{
    BOOLEAN             result = FALSE;
    uint32              scale_result = 1;
    SCALE_IMAGE_IN_T    scale_in = { 0 };
    SCALE_IMAGE_OUT_T   scale_out = { 0 };

    TRACE_WATCHFACE("enter ZoomWatchFaceEditImg");

    if( PNULL == p_data_info )
    {
        TRACE_WATCHFACE("p_data_info is null");
        return result;
    }

    if (PNULL == p_data_info->bit_ptr)
    {
        TRACE_WATCHFACE("p_zoom_data_info->bit_ptr is null");
        return result;
    }

    if (PNULL == p_zoom_data_info)
    {
        TRACE_WATCHFACE("[EDIT]p_data_info is null");
        return result;
    }

    if (PNULL == p_zoom_data_info->bit_ptr)
    {
        TRACE_WATCHFACE("p_zoom_data_info->bit_ptr is null");
        return result;
    }

    scale_in.src_chn.chn0.size    = size;
    scale_in.src_chn.chn0.ptr     = p_data_info->bit_ptr;
    scale_in.src_format           = IMGREF_FORMAT_ARGB888;
    scale_in.src_size.w           = p_data_info->img_width;
    scale_in.src_size.h           = p_data_info->img_height;

    scale_in.src_trim_rect.x      = 0;
    scale_in.src_trim_rect.y      = 0;
    scale_in.src_trim_rect.w      = p_data_info->img_width;
    scale_in.src_trim_rect.h      = p_data_info->img_height;

    scale_in.scale_mode           = SCALE_MODE_NO_DISTORT;
    scale_in.target_buf.size      = p_zoom_data_info->img_height * p_zoom_data_info->img_width * sizeof(uint32);
    scale_in.target_buf.ptr       = p_zoom_data_info->bit_ptr;

    scale_in.target_format        = IMGREF_FORMAT_ARGB888;
    scale_in.target_size.h        = p_zoom_data_info->img_height;
    scale_in.target_size.w        = p_zoom_data_info->img_width;

    scale_result= GRAPH_ScaleImage(&scale_in, &scale_out);
    if( SCI_SUCCESS != scale_result )
    {
        TRACE_WATCHFACE("[EDIT]GRAPH_ScaleImage failed.");
        return result;
    }

    return TRUE;
}

LOCAL void ConvertControlItemRatioPositionToRect( WATCHFACE_RATIO_POSITION_T position, GUI_RECT_T* p_rect )
{
    //通过中心点所在象限决定RECT，如果在轴线上则遵循左上归类原则，如(0.4，0.4)则取左上RECT

    int32 postion_index = WATCHFACE_MTF_BASE_POSITION_LEFT_UP;

    if( PNULL == p_rect )
    {
        TRACE_WATCHFACE("p_rect is PNULL");
        return;
    }

    if( position.ratio_y <= WATCHFACE_MTF_POSITION_RATIO_1 )
    {
        if( position.ratio_x <= WATCHFACE_MTF_POSITION_RATIO_1 )
        {
            postion_index = WATCHFACE_MTF_BASE_POSITION_LEFT_UP;
        }
        else if( position.ratio_x <= WATCHFACE_MTF_POSITION_RATIO_2 )
        {
            postion_index = WATCHFACE_MTF_BASE_POSITION_LEFT;
        }
        else
        {
            postion_index = WATCHFACE_MTF_BASE_POSITION_LEFT_DOWN;
        }
    }
    else if( position.ratio_y <= WATCHFACE_MTF_POSITION_RATIO_2 )
    {
        if( position.ratio_x <= WATCHFACE_MTF_POSITION_RATIO_1 )
        {
            postion_index = WATCHFACE_MTF_BASE_POSITION_LEFT;
        }
        else if( position.ratio_x <= WATCHFACE_MTF_POSITION_RATIO_2 )
        {
            postion_index = WATCHFACE_MTF_BASE_POSITION_CENTER;
        }
        else
        {
            postion_index = WATCHFACE_MTF_BASE_POSITION_RIGHT;
        }
    }
    else
    {
        if( position.ratio_x <= WATCHFACE_MTF_POSITION_RATIO_1 )
        {
            postion_index = WATCHFACE_MTF_BASE_POSITION_LEFT_DOWN;
        }
        else if( position.ratio_x <= WATCHFACE_MTF_POSITION_RATIO_2 )
        {
            postion_index = WATCHFACE_MTF_BASE_POSITION_DOWN;
        }
        else
        {
            postion_index = WATCHFACE_MTF_BASE_POSITION_RIGHT_DOWN;
        }
    }

    p_rect->left = g_watchface_function_base_position[postion_index].left;
    p_rect->top = g_watchface_function_base_position[postion_index].top;
    p_rect->right = g_watchface_function_base_position[postion_index].right;
    p_rect->bottom = g_watchface_function_base_position[postion_index].bottom;

    TRACE_WATCHFACE("p_rect.left %d p_rect.top %d p_rect.right %d p_rect.bottom %d", p_rect->left, p_rect->top, p_rect->right, p_rect->bottom);
}

LOCAL void DestroyMTFWinData( void )
{
    //bug id 1941096
    if( PNULL != s_watchface_multifunction_info )
    {
        SCI_FREE( s_watchface_multifunction_info );
        s_watchface_multifunction_info = PNULL;
    }
}

LOCAL BOOLEAN CreateMTFWinData( uint16 watchface_index, MMI_WIN_ID_T win_id )
{
    uint16                      index = 0;
    uint16                      total_source = 0;
    uint16                      index_source = 0;
    JSONPARSER_RESULT_E         result = JSON_PARSER_SUCCESS;
    WATCHFACE_EDIT_MULTINUM_T   multinum_info = { 0 };

    TRACE_WATCHFACE("enter");

    result = JSONPARSER_GetWatchfaceMultinumInfo( watchface_index, &multinum_info );
    if( JSON_PARSER_SUCCESS != result )
    {
        return FALSE;
    }

    DestroyMTFWinData();

    s_watchface_multifunction_info = (WATCHFACE_MTF_CONTROL_INFO*)SCI_ALLOC_APPZ( sizeof(WATCHFACE_MTF_CONTROL_INFO)*multinum_info.multinum_count );
    if( PNULL == s_watchface_multifunction_info)
    {
        TRACE_WATCHFACE("s_watchface_multifunction_info malloc fail");
        return FALSE;
    }

    SCI_MEMSET( s_watchface_multifunction_info, 0, sizeof(WATCHFACE_MTF_CONTROL_INFO)*multinum_info.multinum_count );

    total_source = sizeof(g_watchface_function_source)/sizeof(WATCHFACE_MTF_SOURCE);
    for( index = 0; index < multinum_info.multinum_count; index++ )
    {
        // image id
        for( index_source = 0; index_source < total_source; index_source++ )
        {
            if( 0 == strcmp(multinum_info.multinum_info[index].data_type, g_watchface_function_source[index_source].func_type) )
            {
                s_watchface_multifunction_info[index].func_img_id = g_watchface_function_source[index_source].func_img_id;
            }
        }

        if( 0 == s_watchface_multifunction_info[index].func_img_id )
        {
            TRACE_WATCHFACE("no match image");

            DestroyMTFWinData();
            return FALSE;
        }

        // type
        SCI_STRCPY( s_watchface_multifunction_info[index].func_type, multinum_info.multinum_info[index].data_type );

        // rect
        ConvertControlItemRatioPositionToRect( multinum_info.multinum_info[index].center_position, &(s_watchface_multifunction_info[index].item_rect) );
    }

    s_watchface_item_num = multinum_info.multinum_count;
    s_watchface_selected_watchface_index = watchface_index;

    return TRUE;
}

LOCAL BOOLEAN GetImageDecodeData( uint16* p_path, uint16 path_len, GUIIMG_BITMAP_T* p_img_info, uint32 size )
{
    GUIIMG_SRC_T            img_src = {0};
    GUIIMG_DISPLAY_T        img_display = {0};
    GUIIMG_DEC_OUT_T        img_output = {0};
    uint16                  img_width = 0;
    uint16                  img_height = 0;
    BOOLEAN                 result = FALSE;

    TRACE_WATCHFACE("enter");

    if( PNULL == p_path || 0 == path_len )
    {
        TRACE_WATCHFACE("[EDIT]Path_ptr is null or path_len is 0!!");
        return result;
    }

    if( PNULL == p_img_info || PNULL == p_img_info->bit_ptr )
    {
        TRACE_WATCHFACE("p_img_info or p_img_info->bit_ptr is PNULL");
        return result;
    }

    // get image width && height
    result = CTRLANIM_GetImgWidthHeight( &img_width, &img_height, p_path, path_len );
    if( FALSE == result )
    {
        return result;
    }

    TRACE_WATCHFACE("img_width is %d, img_height is %d", img_width, img_height);

    if( 0 == img_width || 0 == img_height )
    {
        TRACE_WATCHFACE("[EDIT]Image width or height is 0!!");
        return FALSE;
    }

    img_src.is_file = TRUE;
    img_src.full_path_ptr = p_path;

    img_display.dest_height = img_height;
    img_display.dest_width = img_width;
    img_display.is_handle_transparent = TRUE;

    img_output.data_type = GUIIMG_DATE_TYPE_ARGB888;
    img_output.decode_data_size = size;
    img_output.decode_data_ptr = p_img_info->bit_ptr;

    result = GUIIMG_Decode( &img_src, &img_display, &img_output );
    if( FALSE == result )
    {
        TRACE_WATCHFACE("Decode image fail!!");
        return result;
    }

    p_img_info->date_type = img_output.data_type;
    p_img_info->img_height = img_display.dest_height;
    p_img_info->img_width = img_display.dest_width;

    return result;
}

LOCAL void DrawMTFWinBgImg( MMI_WIN_ID_T win_id, GUI_RECT_T* p_win_rect, GUI_LCD_DEV_INFO* p_lcd_dev_info )
{
    uint16*             p_full_path = PNULL;
    uint16              full_path_len = 0;
    uint32              buf_size = 0;
    BOOLEAN             result = FALSE;
    GUI_POINT_T         start_point = { 0 };
    GUIIMG_BITMAP_T     data_info = { 0 };
    GUIIMG_BITMAP_T     zoom_data_info = { 0 };

    // image path
    p_full_path = SCI_ALLOC_APPZ( sizeof(wchar)*(WATCHFACE_FILES_FULLPATH_LEN + 1) );
    if (PNULL == p_full_path)
    {
        TRACE_WATCHFACE("Alloca for p_full_path fail!!");
        return;
    }

    result = WATCHFACE_Get_PreImgPath_ByIdx( s_watchface_selected_watchface_index, WATCHFACE_FILES_FULLPATH_LEN, p_full_path, &full_path_len );
    if( FALSE == result )
    {
        TRACE_WATCHFACE("GetImgFullPathByIdx failed");

        SCI_FREE(p_full_path);
        p_full_path = PNULL;
        return;
    }

    // image width height
    result = CTRLANIM_GetImgWidthHeight( &data_info.img_width, &data_info.img_height, p_full_path, full_path_len );
    if( FALSE == result )
    {
        TRACE_WATCHFACE("CTRLANIM_GetImgWidthHeight failed");

        SCI_FREE(p_full_path);
        p_full_path = PNULL;
        return;
    }

    // image data
    buf_size = data_info.img_width * data_info.img_height * sizeof(uint32);
    data_info.bit_ptr= (void*)SCI_ALLOC_APPZ( buf_size+1 );
    if( PNULL == data_info.bit_ptr )
    {
        TRACE_WATCHFACE("Alloca for data_info.bit_ptr fail!!");

        SCI_FREE(p_full_path);
        p_full_path = PNULL;
        return;
    }

    result = GetImageDecodeData( p_full_path,full_path_len, &data_info, buf_size );
    if( FALSE == result )
    {
        TRACE_WATCHFACE("GetDecodeData_ByImagePath failed");

        SCI_FREE( p_full_path );
        p_full_path = PNULL;

        SCI_FREE( data_info.bit_ptr );
        data_info.bit_ptr = PNULL;
        return;
    }

#ifdef WIN32
    // position 使图片显示在屏幕中心位置
    start_point.x = (WATCHFACE_MTF_MAINSCREEN_WIDTH - data_info.img_width) / 2;//使图片显示在屏幕中心位置
    start_point.y = (WATCHFACE_MTF_MAINSCREEN_HEIGHT - data_info.img_height) / 2;

    GUIIMG_DisplayBmp( FALSE, p_win_rect, &start_point, &data_info, p_lcd_dev_info );

#else

    zoom_data_info.img_height = WATCHFACE_MTF_MAINSCREEN_HEIGHT*WATCHFACE_MTF_BG_ZOOM_RATE;
    zoom_data_info.img_width = WATCHFACE_MTF_MAINSCREEN_WIDTH*WATCHFACE_MTF_BG_ZOOM_RATE;
    zoom_data_info.date_type = data_info.date_type;
    zoom_data_info.bit_ptr = SCI_ALLOC_APPZ( zoom_data_info.img_height * zoom_data_info.img_width * sizeof(uint32) );
    if(PNULL == zoom_data_info.bit_ptr)
    {
        TRACE_WATCHFACE("[EDIT]Alloca for zoom_data_info.bit_ptr fail!!");
        return;
    }
    result = ZoomImg(&data_info, buf_size, &zoom_data_info);
    if( FALSE == result )
    {
        TRACE_WATCHFACE("ZoomImg fail!!");

        SCI_FREE( p_full_path );
        p_full_path = PNULL;

        SCI_FREE( data_info.bit_ptr );
        data_info.bit_ptr = PNULL;
        return;
    }
    start_point.x = (WATCHFACE_MTF_MAINSCREEN_WIDTH - zoom_data_info.img_width) / 2;//使图片显示在屏幕中心位置
    start_point.y = (WATCHFACE_MTF_MAINSCREEN_HEIGHT - zoom_data_info.img_height) / 2;

    GUIIMG_DisplayBmp( FALSE, p_win_rect, &start_point, &zoom_data_info, p_lcd_dev_info );
#endif

    SCI_FREE( p_full_path );
    p_full_path = PNULL;

    SCI_FREE( data_info.bit_ptr );
    data_info.bit_ptr = PNULL;

}

LOCAL void DrawMTFWinControlerItem( MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO lcd_dev_info )
{
    uint16                      index = 0;

    TRACE_WATCHFACE("enter");

    if( PNULL == s_watchface_multifunction_info )
    {
        TRACE_WATCHFACE("s_watchface_multifunction_info is PNULL");
        return;
    }

    for( index = 0; index < s_watchface_item_num; index++ )
    {
        GUIRES_DisplayImg( PNULL,
                           &(s_watchface_multifunction_info[index].item_rect),
                           PNULL,
                           win_id,
                           s_watchface_multifunction_info[index].func_img_id,
                           &lcd_dev_info );
    }

}

LOCAL void DrawMTFListWinControlerItem( MMI_CTRL_ID_T ctrl_id )
{
    uint16                      index = 0;
    uint16                      total = 0;
    MMI_STRING_T                func_str = { 0 };
    MMI_IMAGE_ID_T              select_image = IMAGE_NULL;
    MMI_TEXT_ID_T               func_text_id = TXT_NULL;
    MMI_STRING_T                str2 = {0};

    TRACE_WATCHFACE("enter");

    if( PNULL == s_watchface_multifunction_info )
    {
        TRACE_WATCHFACE("s_watchface_multifunction_info is PNULL");
        return;
    }

    total = sizeof(g_watchface_function_list_source)/sizeof(WATCHFACE_MTF_SOURCE);
    GUILIST_SetMaxItem( ctrl_id, total, FALSE );

    for( index = 0; index < total; index++ )
    {
        func_text_id = g_watchface_function_list_source[index].func_txt_id;

        // function name text string
        MMI_GetLabelTextByLang( func_text_id, &func_str );

        // 讨论后确定通过对比功能的Type来判断，固定资源中包含和Json中同样的type
        if( 0 == strcmp(s_watchface_multifunction_info[s_watchface_selected_item_index].func_type, g_watchface_function_source[index].func_type) )
        {
            select_image = res_aw_clock_function_menu_ic_done;
        }
        else
        {
            select_image = IMAGE_NULL;
        }
#ifdef  ADULT_WATCH_SUPPORT
        AdultWatchCOM_ListItem_Draw_1Icon_1Str_1Icon_1line( ctrl_id, g_watchface_function_list_source[index].func_img_id, func_str, select_image );
#else
        WatchCOM_ListItem_Draw_1Icon_1Str_1Icon_1Str( ctrl_id, g_watchface_function_list_source[index].func_img_id, func_str, select_image, str2);
#endif
    }

    //set current item
    GUILIST_SetCurItemIndex( ctrl_id, s_watchface_selected_item_index );

}

LOCAL void HandleMTFDialWinFullPaint( MMI_WIN_ID_T win_id )
{
    GUI_RECT_T          win_rect = WATCHFACE_MTF_MAINSCREEN_RECT;
    GUI_LCD_DEV_INFO    lcd_dev_info = { 0 };

    TRACE_WATCHFACE("enter");

    MMK_GetWinLcdDevInfo( win_id, &lcd_dev_info );

    // bg color
    GUI_FillRect(&lcd_dev_info, win_rect, MMI_BLACK_COLOR);

    // bg image
    DrawMTFWinBgImg( win_id, &win_rect, &lcd_dev_info );

    // set gray layer
    GUIRES_DisplayImg( NULL, &win_rect, PNULL, win_id, res_aw_edit_clock_mask, &lcd_dev_info);

    DrawMTFWinControlerItem( win_id, lcd_dev_info );

}

LOCAL void HandleMTFDialWinPressUp( MMI_WIN_ID_T win_id, DPARAM param )
{
    GUI_POINT_T                 point = { 0 };
    BOOLEAN                     ret = TRUE;
    uint16                      index = 0;

    TRACE_WATCHFACE("enter");

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    for( index = 0; index < s_watchface_item_num; index++ )
    {
        ret = InRect( point, s_watchface_multifunction_info[index].item_rect );
        if( TRUE == ret )
        {
            s_watchface_selected_item_index = index;
            WATCHFACE_MTFDial_List_Win_Enter();
            return;
        }
    }

    // 点击其他地方关闭窗口
    MMK_CloseWin(win_id);
}


LOCAL MMI_RESULT_E HandleWatchfaceMTFDialWinMsg(
                                      MMI_WIN_ID_T          win_id,
                                      MMI_MESSAGE_ID_E      msg_id,
                                      DPARAM                param
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    TRACE_WATCHFACE("enter 0x%x", msg_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            break;
        }

        case MSG_FULL_PAINT:
        {
            HandleMTFDialWinFullPaint( win_id );

            break;
        }

        case MSG_APP_WEB:
        {
            MMK_CloseWin( win_id );
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_DOWN:
        {
            break;
        }

        case MSG_TP_PRESS_MOVE:
        {
            break;
        }

        case MSG_TP_PRESS_UP:
        {
            HandleMTFDialWinPressUp( win_id, param );
            break;
        }
#endif

        case MSG_GET_FOCUS:
        {
            break;
        }

        case MSG_KEYDOWN_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            DestroyMTFWinData();
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

WINDOW_TABLE(WATCHFACE_MTFDIAL_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleWatchfaceMTFDialWinMsg),
    WIN_ID(WATCHFACE_MTFDIAL_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

LOCAL void HandleMTFDialListWinOpenWin( MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id )
{
    DrawMTFListWinControlerItem( ctrl_id );
    MMK_SetAtvCtrl(win_id, ctrl_id);
}

LOCAL void HandleMTFDialListWinOK( MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id )
{
    uint8*                      p_type = PNULL;
    uint16                      index = 0;
    JSONPARSER_RESULT_E         result = JSON_PARSER_SUCCESS;
    WATCHFACE_EDIT_MULTINUM_T   multinum_info = { 0 };

    TRACE_WATCHFACE("enter");

    result = JSONPARSER_GetWatchfaceMultinumInfo( s_watchface_selected_watchface_index, &multinum_info );
    if( JSON_PARSER_SUCCESS != result )
    {
        TRACE_WATCHFACE("JSONPARSER_GetWatchfaceMultinumInfo fail");
        return;
    }

    index = GUILIST_GetCurItemIndex( ctrl_id );
    p_type = g_watchface_function_list_source[index].func_type;

    if( 0 == strcmp( multinum_info.multinum_info[s_watchface_selected_item_index].data_type, p_type) )
    {
        TRACE_WATCHFACE("same type no need change");
        MMK_CloseWin( win_id );
        return;
    }
    else
    {
        SCI_MEMCPY( multinum_info.multinum_info[s_watchface_selected_item_index].data_type, p_type, MAX_NAME_LEN );
    }

    // update file data
    result = JSONPARSER_SetWatchfaceMultinumInfo( s_watchface_selected_watchface_index, &multinum_info );
    if( JSON_PARSER_SUCCESS != result )
    {
        TRACE_WATCHFACE("JSONPARSER_SetWatchfaceMultinumInfo fail");
        return;
    }
    s_is_change_multi = TRUE;
    //update s_p_display_info
    WATCHFACE_Construct_Layout_Data();
    // update window data
    CreateMTFWinData( s_watchface_selected_watchface_index, win_id );

    MMK_CloseWin( win_id );
}

LOCAL MMI_RESULT_E HandleWatchfaceMTFDialListWinMsg(
                                      MMI_WIN_ID_T        win_id,
                                      MMI_MESSAGE_ID_E    msg_id,
                                      DPARAM              param
                                      )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = WATCH_FACE_MTF_DIAL_CTRL_LIST_ID;
    uint16              index = 0;

    TRACE_WATCHFACE("enter 0x%x", msg_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            HandleMTFDialListWinOpenWin( win_id, ctrl_id );
            break;
        }

        case MSG_FULL_PAINT:
        {

            break;
        }

        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        {
            HandleMTFDialListWinOK( win_id, ctrl_id );
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_DOWN:
        {
            break;
        }

        case MSG_TP_PRESS_MOVE:
        {
            break;
        }

        case MSG_TP_PRESS_UP:
        {
            break;
        }
#endif

        case MSG_KEYDOWN_CANCEL:
        {
            MMK_CloseWin(win_id);
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
            break;
        }

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}

WINDOW_TABLE(WATCHFACE_MTFDIAL_LIST_WINTAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC( (uint32)HandleWatchfaceMTFDialListWinMsg),
    WIN_ID(WATCHFACE_MTFDIAL_LIST_WIN_ID),
    WIN_TITLE(TXT_AW_MTF_LIST_TITLE_CUSTOM),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, WATCH_FACE_MTF_DIAL_CTRL_LIST_ID),
    END_WIN
};

/**--------------------------------------------------------------------------*
**                         PUBLIC FUNCTION                                   *
**---------------------------------------------------------------------------*/

PUBLIC void WATCHFACE_MTFDial_Win_Enter( uint16 watchface_index )
{
    BOOLEAN                     is_open = TRUE;
    MMI_WIN_ID_T                win_id = WATCHFACE_MTFDIAL_WIN_ID;

    TRACE_WATCHFACE("enter");

    CreateMTFWinData( watchface_index, win_id );

    is_open = MMK_IsOpenWin( win_id );
    if( TRUE == is_open )
    {
        MMK_CloseWin(win_id);
    }

    TRACE_WATCHFACE(" item num %d watch face index %d", s_watchface_item_num, s_watchface_selected_watchface_index);

    MMK_CreateWin(WATCHFACE_MTFDIAL_WINTAB, NULL);
}

PUBLIC void WATCHFACE_MTFDial_List_Win_Enter( void )
{
    BOOLEAN         is_open = TRUE;
    MMI_WIN_ID_T    win_id = WATCHFACE_MTFDIAL_LIST_WIN_ID;

    TRACE_WATCHFACE("enter");

    is_open = MMK_IsOpenWin( win_id );
    if( TRUE == is_open )
    {
        MMK_CloseWin(win_id);
    }

    TRACE_WATCHFACE(" item num %d watch face index %d, item index %d", s_watchface_item_num, s_watchface_selected_watchface_index, s_watchface_selected_item_index);

    MMK_CreateWin(WATCHFACE_MTFDIAL_LIST_WINTAB, NULL);
}
PUBLIC BOOLEAN WATCHFACE_Get_Is_Change_Multi( void )
{
    TRACE_WATCHFACE("enter");
    return s_is_change_multi;
}
PUBLIC void WATCHFACE_Set_Is_Change_Multi( BOOLEAN is_change_multi )
{
    TRACE_WATCHFACE("enter,is_change_multi = %d",is_change_multi);
    s_is_change_multi = is_change_multi;
}
#endif
