/****************************************************************************
** File Name:      watch_launcher_common.c                                                                    *
** Author:                                                                                                                    *
** Date:             02/03/2020                                                                                         *
** Copyright:      2020 Spreadtrum, Incoporated. All Rights Reserved.                                  *
** Description:    launcher common function                                                                  *
*****************************************************************************
**                         Important Edit History                                                                        *
** --------------------------------------------------------------------------*
** DATE            NAME             DESCRIPTION                                                                   *
** 02/03/2020  zhikun.lv        Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "graphics_draw.h"
#include "guistring.h"
#include "watch_launcher_common.h"
#include "mmicom_trace.h"
#include "watch_launcher_multe_function_dial.h"

/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/
typedef struct
{
    void   *data_ptr;
    uint32  data_type;  //0: RGB565, 1: ARGB888
    uint16  width;
    uint16  height;
    int16   start_x;
    int16   start_y;
}CLOCK_ALPHA_IMG_T;

typedef enum eDIGITAL_ITEM_TYPE
{
    DIGITAL_ITEM_BG,
    DIGITAL_ITEM_HOUR_NUM0,
    DIGITAL_ITEM_HOUR_NUM1,
    DIGITAL_ITEM_COLON,
    DIGITAL_ITEM_MINUTE_NUM0,
    DIGITAL_ITEM_MINUTE_NUM1,
    DIGITAL_ITEM_YEAR,
    DIGITAL_ITEM_AMPM,
    DIGITAL_ITEM_MONTH,
    DIGITAL_ITEM_DAY,
    DIGITAL_ITEM_WEEK,

    DIGITAL_ITEM_MAX
}eDIGITAL_ITEM_TYPE;


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

#define ARGB_GET_A(_argb) (((_argb) >> 24) & 0xff)
#define ARGB_GET_R(_argb) (((_argb) >> 16) & 0xff)
#define ARGB_GET_G(_argb) (((_argb) >> 8) & 0xff)
#define ARGB_GET_B(_argb) ((_argb) & 0xff)

#define ARGB_SET_A(_argb, _alpha)   ((_argb) | (((_alpha) << 24) & 0xff000000))
#define ARGB_SET_R(_argb, _r)       ((_argb) | (((_r) << 16) & 0xff0000))
#define ARGB_SET_G(_argb, _g)       ((_argb) | (((_g) << 8) & 0xff00))
#define ARGB_SET_B(_argb, _b)       ((_argb) | (((_b)) & 0xff))

#define RGB565_GET_R(_color) (((_color) >> 8) & 0xf8)
#define RGB565_GET_G(_color) (((_color) >> 3) & 0xfc)
#define RGB565_GET_B(_color) (((_color) << 3) & 0xf8)

#define RGB565_SET_R(_color, _r) ((_color) | (((_r) << 8) & 0xf800))
#define RGB565_SET_G(_color, _g) ((_color) | (((_g) << 3) & 0x07e0))
#define RGB565_SET_B(_color, _b) ((_color) | (((_b) >> 3) & 0x001f))

#define RGB888_TO_RGB565(r, g, b)  (((r << 8) & 0xf800) | ((g << 3) & 0x07e0) | ((b >> 3) & 0x1f))
#define RGB8882RGB565(rgb888)   ((((rgb888)&0xf80000)>>8)|(((rgb888)&0xfc00)>>5)|(((rgb888)&0xf8)>>3))
/*lint -e648 -e572 -e778*/
#define RGB5652ARGB8888(rgb565,alpha) ( ((alpha)<<24) |((((rgb565) >> 8) & 0xf8)<<16)|((((rgb565) >> 3) & 0xfc)<<8)|(((rgb565) << 3)  & 0xf8))
/*lint -e648 -e572 -e778*/
#define BLEND_MASK 0x7E0F81F

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/


LOCAL MMI_IMAGE_ID_T s_hour_minute_image_ids[] =
{
    res_aw_number_0,
    res_aw_number_1,
    res_aw_number_2,
    res_aw_number_3,
    res_aw_number_4,
    res_aw_number_5,
    res_aw_number_6,
    res_aw_number_7,
    res_aw_number_8,
    res_aw_number_9
};
LOCAL MMI_TEXT_ID_T s_week_strings[] =
{
    TXT_AW_CLOCK_SUNDAY,
    TXT_AW_CLOCK_MONDAY,
    TXT_AW_CLOCK_TUESDAY,
    TXT_AW_CLOCK_WEDNESDAY,
    TXT_AW_CLOCK_THURSDAY,
    TXT_AW_CLOCK_FRIDAY,
    TXT_AW_CLOCK_SATURDAY
};

/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION  DECLARE                                 *
**---------------------------------------------------------------------------*/
LOCAL void initItemRects(void);



/**--------------------------------------------------------------------------*
**                         LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
void CreateCacheLayer(GUI_LCD_DEV_INFO *out, MMI_WIN_ID_T win_id, uint16 width, uint16 height)
{
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_CREATE_T create_info = {0};
    GUI_LCD_DEV_INFO *lcd_dev_ptr = out;
    if (PNULL == lcd_dev_ptr)
    {
        return;
    }

    if (lcd_dev_ptr->block_id != UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(lcd_dev_ptr);
    }

    lcd_dev_ptr->lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_ptr->block_id = UILAYER_NULL_HANDLE;

    // ´´½¨²ã
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = win_id;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = width;
    create_info.height = height;
    create_info.is_bg_layer = TRUE;
    create_info.is_static_layer = TRUE;

    UILAYER_CreateLayer(
        &create_info,
        lcd_dev_ptr
        );

    //UILAYER_SetLayerColorKey(lcd_dev_ptr, TRUE, MMI_BLACK_COLOR);
    UILAYER_Clear(lcd_dev_ptr);

    if (lcd_dev_ptr->block_id == UILAYER_NULL_HANDLE)
    {
        //SCI_ASSERT(0);
        TRACE_APP_LAUNCHER("lcd_dev_ptr->block_id == UILAYER_NULL_HANDLE");
    }
#endif
}

void DisplayImageOnScreen(uint32 srcWidth,
                                                      uint32 srcHeight,
                                                      void *pSrcDataBuf,
                                                      GUI_RECT_T displayRect,
                                                      const GUI_LCD_DEV_INFO *p_lcd_dev_info)
{
    int i = 0;
    GUI_POINT_T disPointStart = {0};
    GUIIMG_BITMAP_T imgBitmap = {0};
    SCALE_IMAGE_IN_T scale_in = {0};
    SCALE_IMAGE_OUT_T scale_out = {0};
    uint16 picWidth = 0;
    uint16 picHeight = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = *p_lcd_dev_info;

    disPointStart.x = displayRect.left;
    disPointStart.y = displayRect.top;
    picWidth = displayRect.right - displayRect.left;
    picHeight = displayRect.bottom- displayRect.top;

    scale_in.src_format           = IMGREF_FORMAT_RGB565;
    scale_in.src_chn.chn0.ptr     = pSrcDataBuf;
    scale_in.src_chn.chn0.size    = srcWidth * srcHeight * 2;
    scale_in.src_size.w           = srcWidth;
    scale_in.src_size.h           = srcHeight;
    scale_in.src_trim_rect.x      = 0;
    scale_in.src_trim_rect.y      = 0;
    scale_in.src_trim_rect.w      = srcWidth;
    scale_in.src_trim_rect.h      = srcHeight;
    scale_in.target_buf.size      = picWidth * picHeight * sizeof(uint16);
    scale_in.target_buf.ptr       = malloc(scale_in.target_buf.size);
    scale_in.target_format        = IMGREF_FORMAT_RGB565;
    scale_in.target_size.w        = picWidth;
    scale_in.target_size.h        = picHeight;
    scale_in.scale_mode           =SCALE_MODE_NO_DISTORT;

    GRAPH_ScaleImage(&scale_in, &scale_out);

    imgBitmap.img_width = picWidth;
    imgBitmap.img_height = picHeight;
    imgBitmap.bit_ptr = scale_in.target_buf.ptr;
    GUIIMG_DisplayBmp(FALSE, &displayRect, &disPointStart, &imgBitmap, &lcd_dev_info);

    //MMITHEME_StoreUpdateRect(&lcd_dev_info, displayRect);
    //MMITHEME_UpdateRect();

    if (NULL != scale_in.target_buf.ptr)
        free(scale_in.target_buf.ptr);
}


#define LAUNCHER_ANALOG_SUPPORT 1
#ifdef LAUNCHER_ANALOG_SUPPORT
LOCAL __inline uint16 BlendRGB565(uint16 bk_color, uint16 fore_color, uint8 alpha)
{
    uint32 blend_color = 0;
    uint32 temp_back_color = 0;
    uint32 temp_fore_color = 0;

    alpha = (alpha + 1) >> 3;

    temp_fore_color = ( fore_color | (fore_color << 16)) & BLEND_MASK;
    temp_back_color = ( bk_color | (bk_color << 16)) & BLEND_MASK;
    blend_color = ((((temp_fore_color - temp_back_color) * alpha ) >> 5 ) + temp_back_color) & BLEND_MASK;
    blend_color = ((blend_color & 0xFFFF) | (blend_color >> 16));

    return (uint16)blend_color;
}

LOCAL BOOLEAN ProcessAlphaBlending(CLOCK_ALPHA_IMG_T *bk_img_ptr, CLOCK_ALPHA_IMG_T *fore_img_ptr)
{
    uint32 fore_width  = fore_img_ptr->width;
    uint32 fore_height = fore_img_ptr->height;
    uint32 bk_width = bk_img_ptr->width;
    uint32 bk_height = bk_img_ptr->height;
    int32 start_x = fore_img_ptr->start_x;
    int32 start_y = fore_img_ptr->start_y;
    uint32 blend_width = fore_width;
    uint32 blend_height = fore_height;
    uint32 *fore_ptr = (uint32 *)fore_img_ptr->data_ptr;
    GUI_RECT_T dst_rect = {0};
    GUI_RECT_T bk_rect = {0};
    GUI_RECT_T for_rect = {0};
    bk_rect.left  = bk_img_ptr->start_x;
    bk_rect.top   = bk_img_ptr->start_y;
    bk_rect.right = bk_img_ptr->start_x + bk_img_ptr->width;
    bk_rect.bottom  = bk_img_ptr->start_y + bk_img_ptr->height;

    for_rect.left  = fore_img_ptr->start_x;
    for_rect.top   = fore_img_ptr->start_y;
    for_rect.right = fore_img_ptr->start_x + fore_img_ptr->width;
    for_rect.bottom  = fore_img_ptr->start_y + fore_img_ptr->height;

    if (!GUI_IntersectRect(&dst_rect, bk_rect, for_rect))
    {
        return FALSE;
    }

    if (start_y < 0)
    {
        start_y = -start_y;
        blend_height -= start_y;        /*lint !e737*/
        fore_ptr += (start_y * fore_width);     /*lint !e737*/
        start_y = 0;
    }

    if (start_x < 0)
    {
        start_x = -start_x;
        blend_width -= start_x;     /*lint !e737*/
        fore_ptr += start_x;
        start_x = 0;
    }

    //SCI_PASSERT((int32)bk_width >= start_x, ("bk_width=%d, start_x=%d", bk_width, start_x));
    //SCI_PASSERT((int32)bk_height >= start_y, ("bk_height=%d, start_y=%d", bk_height, start_y));

    if ((int32)bk_width < start_x || (int32)bk_height < start_y)
    {
        //SCI_TRACE_LOW:"aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y"
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_ACLOCK_387_112_2_18_3_7_49_166,(uint8*)"");
        return FALSE;
    }

    blend_width = (start_x + blend_width) > bk_width        /*lint !e737*/
                    ? (bk_width - start_x) : blend_width;       /*lint !e737*/
    blend_height = (start_y + blend_height) > bk_height         /*lint !e737*/
                    ? (bk_height - start_y) : blend_height;     /*lint !e737*/

    if (IMGREF_FORMAT_RGB565 == bk_img_ptr->data_type)     //output RGB565
    {
        uint32 i = 0, j = 0;
        uint16 *bk_ptr = (uint16 *)bk_img_ptr->data_ptr;

        bk_ptr += (start_y * bk_width + start_x);       /*lint !e737*/

        for (i=0; i<blend_height; i++)
        {
            for (j=0; j<blend_width; j++)
            {
                uint32 alpha, r, g, b;
                uint32 fore_value = *(fore_ptr + j);
                uint16 bk_value = *(bk_ptr + j);

                alpha = ARGB_GET_A(fore_value);
                r = ARGB_GET_R(fore_value);
                g = ARGB_GET_G(fore_value);
                b = ARGB_GET_B(fore_value);

                fore_value = RGB888_TO_RGB565(r, g, b);
                bk_value = BlendRGB565(bk_value, (uint16)fore_value, alpha);
                *(bk_ptr + j) = bk_value;
            }

            bk_ptr += bk_width;
            fore_ptr += fore_width;
        }
    }
    else                                //output ARGB888
    {
        uint32 i = 0, j = 0;
        uint32 *bk_ptr = (uint32 *)bk_img_ptr->data_ptr;

        bk_ptr += (start_y * bk_width + start_x);       /*lint !e737*/

        for (i=0; i<blend_height; i++)
        {
            for (j=0; j<blend_width; j++)
            {
                int32 alpha, bk_color, fore_color;
                uint32 fore_value = *(fore_ptr + j);
                uint32 bk_value = *(bk_ptr + j);
                uint32 result_value = (bk_value & 0xff000000);

#if 1
                alpha = ARGB_GET_A(bk_value);

                if (0 == alpha) 
                {
                    *(bk_ptr + j) = *(fore_ptr + j);
                    continue;
                }

                alpha = ARGB_GET_A(fore_value);

                //r
                bk_color = ARGB_GET_R(bk_value);
                fore_color = ARGB_GET_R(fore_value);
                bk_color = bk_color + (((fore_color - bk_color) * alpha) >> 8);
                result_value = ARGB_SET_R(result_value, bk_color);

                //g
                bk_color = ARGB_GET_G(bk_value);
                fore_color = ARGB_GET_G(fore_value); 
                bk_color = bk_color + (((fore_color - bk_color) * alpha) >> 8);
                result_value = ARGB_SET_G(result_value, bk_color);

                //b
                bk_color = ARGB_GET_B(bk_value);
                fore_color = ARGB_GET_B(fore_value); 
                bk_color = bk_color + (((fore_color - bk_color) * alpha) >> 8);
                result_value = ARGB_SET_B(result_value, bk_color);
#else
                result_value = GUI_CompoundARGB888(bk_value, fore_value);
#endif
                *(bk_ptr + j) = result_value;
            }

            bk_ptr += bk_width;
            fore_ptr += fore_width;
        }
    }

    return TRUE;
}
#endif

#define LAUNCHER_DIGITAL_SUPPORT 1
#ifdef LAUNCHER_DIGITAL_SUPPORT
LOCAL GUI_RECT_T s_item_rects[DIGITAL_ITEM_MAX] = {0};
LOCAL MMI_IMAGE_ID_T getHourOrMinuteImageId(uint8 num)
{
    return s_hour_minute_image_ids[num];
}

PUBLIC MMI_STRING_T WatchLAUNCHER_GetWeekString(uint8 week_id)
{
    MMI_STRING_T str = {0};
    uint8 size = sizeof(s_week_strings) / sizeof(s_week_strings[0]);
    if (week_id > size - 1)
    {
        //SCI_ASSERT(0);
        TRACE_APP_LAUNCHER("week_id > size - 1");
    }
    MMI_GetLabelTextByLang(s_week_strings[week_id], &str);
    return str;
}
LOCAL void initItemRects()
{
	int16 centerX = LAUNCHER_WIDTH/2;
	int16 centerY = LAUNCHER_HEIGHT/2;
	int16 number_height = DIGITAL_CLOCK_NUMBER_HEIGHT;
	int16 number_width = DIGITAL_CLOCK_NUMBER_WIDTH;
    SCI_MEMSET(s_item_rects, 0, sizeof (s_item_rects));



    s_item_rects[DIGITAL_ITEM_HOUR_NUM0].left = centerX - number_width;
    s_item_rects[DIGITAL_ITEM_HOUR_NUM0].right = centerX;
    s_item_rects[DIGITAL_ITEM_HOUR_NUM0].top = centerY - number_height;
    s_item_rects[DIGITAL_ITEM_HOUR_NUM0].bottom = centerY;

    s_item_rects[DIGITAL_ITEM_HOUR_NUM1].left = centerX;
    s_item_rects[DIGITAL_ITEM_HOUR_NUM1].right = centerX + number_width;
    s_item_rects[DIGITAL_ITEM_HOUR_NUM1].top = centerY - number_height;
    s_item_rects[DIGITAL_ITEM_HOUR_NUM1].bottom = centerY;

    s_item_rects[DIGITAL_ITEM_MINUTE_NUM0].left = centerX - number_width;
    s_item_rects[DIGITAL_ITEM_MINUTE_NUM0].right = centerX;
    s_item_rects[DIGITAL_ITEM_MINUTE_NUM0].top = centerY;
    s_item_rects[DIGITAL_ITEM_MINUTE_NUM0].bottom = centerY + number_height;

    s_item_rects[DIGITAL_ITEM_MINUTE_NUM1].left = centerX;
    s_item_rects[DIGITAL_ITEM_MINUTE_NUM1].right = centerX + number_width;
    s_item_rects[DIGITAL_ITEM_MINUTE_NUM1].top = centerY;
    s_item_rects[DIGITAL_ITEM_MINUTE_NUM1].bottom = centerY + number_height;


    s_item_rects[DIGITAL_ITEM_WEEK].left = LAUNCHER_WIDTH - DIGITAL_CLOCK_WEEK_WIDTH_OFFSET;
    s_item_rects[DIGITAL_ITEM_WEEK].right = LAUNCHER_WIDTH;
    s_item_rects[DIGITAL_ITEM_WEEK].top = centerY - DIGITAL_CLOCK_WEEK_HEIGHT_OFFSET;
    s_item_rects[DIGITAL_ITEM_WEEK].bottom = centerY + DIGITAL_CLOCK_WEEK_HEIGHT_OFFSET;

    s_item_rects[DIGITAL_ITEM_DAY].left = 0;
    s_item_rects[DIGITAL_ITEM_DAY].right = DIGITAL_CLOCK_WEEK_WIDTH_OFFSET;
    s_item_rects[DIGITAL_ITEM_DAY].top = centerY - DIGITAL_CLOCK_WEEK_HEIGHT_OFFSET;
    s_item_rects[DIGITAL_ITEM_DAY].bottom = centerY + DIGITAL_CLOCK_WEEK_HEIGHT_OFFSET;
}

LOCAL void DisplayDigitalClock(MMI_WIN_ID_T win_id,GUI_LCD_DEV_INFO lcd_dev_info, BOOLEAN is_12Hour)
{
    SCI_TIME_T s_sys_time= {0};
    SCI_DATE_T s_sys_date = {0};
    GUI_RECT_T rect = LAUNCHER_CLIENT_RECT;
    GUI_RECT_T bg_image_rect = {0};
    uint8 hour_num0 = 0;
    uint8 hour_num1 = 0;
    uint8 minute_num0 = 0;
    uint8 minute_num1 = 0;

    TM_GetSysTime(&s_sys_time);
    TM_GetSysDate(&s_sys_date);

    hour_num0 = s_sys_time.hour /10;
    hour_num1 = s_sys_time.hour % 10;

    minute_num0 = s_sys_time.min / 10;
    minute_num1 = s_sys_time.min % 10;

    rect = s_item_rects[DIGITAL_ITEM_HOUR_NUM0];
    GUIRES_DisplayImg(NULL, &rect, NULL, win_id, getHourOrMinuteImageId(hour_num0), &lcd_dev_info);

    rect = s_item_rects[DIGITAL_ITEM_HOUR_NUM1];
    GUIRES_DisplayImg(NULL, &rect, NULL, win_id, getHourOrMinuteImageId(hour_num1), &lcd_dev_info);

    rect = s_item_rects[DIGITAL_ITEM_MINUTE_NUM0];
    GUIRES_DisplayImg(NULL, &rect, NULL, win_id, getHourOrMinuteImageId(minute_num0), &lcd_dev_info);

    rect = s_item_rects[DIGITAL_ITEM_MINUTE_NUM1];
    GUIRES_DisplayImg(NULL, &rect, NULL, win_id, getHourOrMinuteImageId(minute_num1), &lcd_dev_info);

    //display week
    do
    {
        MMI_STRING_T testStr = {0};
        GUISTR_STYLE_T text_style = {0};
        GUISTR_STATE_T state = {0};
        testStr = WatchLAUNCHER_GetWeekString(s_sys_date.wday);
        text_style.align = ALIGN_LEFT;
        text_style.font = DP_FONT_16;
        text_style.font_color = MMI_WHITE_COLOR;
        //rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, s_item_rects[DIGITAL_ITEM_WEEK]);
        rect = s_item_rects[DIGITAL_ITEM_WEEK];
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&rect,
            (const GUI_RECT_T      *)&rect,
            (const MMI_STRING_T    *)&testStr,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
    } while (0);


    do
    {
        char day_buffer[5] = {0};
        wchar wday_buffer[5] = {0};
        MMI_STRING_T testStr = {0};
        GUISTR_STYLE_T text_style = {0};
        GUISTR_STATE_T state = {0};
        sprintf(day_buffer, "%02d%02d", s_sys_date.mon,s_sys_date.mday);
        MMIAPICOM_StrToWstr(day_buffer, wday_buffer);
        testStr.wstr_ptr = wday_buffer;
        testStr.wstr_len = MMIAPICOM_Wstrlen(wday_buffer);
        text_style.align = ALIGN_RIGHT;
        text_style.font = DP_FONT_16;
        text_style.font_color = MMI_WHITE_COLOR;
        rect = s_item_rects[DIGITAL_ITEM_DAY];

        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&rect,
            (const GUI_RECT_T      *)&rect,
            (const MMI_STRING_T    *)&testStr,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
    } while (0);
}


#endif


/**--------------------------------------------------------------------------*
**                         PUBLIC FUNCTION                                   *
**---------------------------------------------------------------------------*/

/*****************************************************************************
//  Description : Display Analog Clock on win, the display_rect is paint area.
//  Author: zhikun.lv
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchLAUNCHER_DrawAnalogClock(MMI_WIN_ID_T win_id,
                                                                                            GUI_RECT_T display_rect)
{
#ifdef UI_MULTILAYER_SUPPORT
    uint8 i = 0;
    uint32 rotate_buf_size = 0;
    uint16 angle_arr[3] = {0};
    SCI_TIME_T sys_time= {0};
    GUI_POINT_T clockpanel_pos = {0, 0};

    GUI_RECT_T rect = DP2PX_RECT(0, 0, LAUNCHER_WIDTH, LAUNCHER_HEIGHT);
    GUI_LCD_DEV_INFO lcd = {0};
    GUI_LCD_DEV_INFO lcd_cache_info = {0, UILAYER_NULL_HANDLE};
    uint8 *rotate_buf_ptr = NULL;
    MMI_IMAGE_ID_T image_arr[3] = {res_aw_clock_pointer_hand,res_aw_clock_pointer_minute,
                                                        res_aw_clock_pointer_second};

	MMK_GetWinLcdDevInfo(win_id, &lcd);
    TRACE_APP_LAUNCHER("enter.");
    rotate_buf_size = LAUNCHER_WIDTH * LAUNCHER_HEIGHT * sizeof(uint32);

    //get system time
    TM_GetSysTime(&sys_time);
    TRACE_APP_LAUNCHER("sys_time = %d: %d: %d.", sys_time.hour, sys_time.min, sys_time.sec);
#ifdef WIN32
    do
    {
        LOCAL uint8 localsec = 0;
        localsec++;
        localsec %= 60;
        sys_time.sec= localsec;
    } while (0);
#endif

    GRAPH_GetAngleByTimeValue(sys_time.hour, sys_time.min, sys_time.sec, &angle_arr[0], &angle_arr[1], &angle_arr[2]);
	
    CreateCacheLayer(&lcd_cache_info, win_id, LAUNCHER_WIDTH, LAUNCHER_HEIGHT);



    if (lcd_cache_info.block_id == UILAYER_NULL_HANDLE)
    {
        TRACE_APP_LAUNCHER("CreateCacheLayer [error].");
        return;
    }

	GUIRES_DisplayImg(&clockpanel_pos,
					  PNULL,
					  PNULL,
					  MAIN_IDLE_WIN_ID,
					  res_aw_clock_bg,
					  &lcd_cache_info);

    //step1, draw clock panel
    GUIRES_DisplayImg(&clockpanel_pos,
                      PNULL,
                      PNULL,
                      MAIN_IDLE_WIN_ID,
                      res_aw_clock_pointer_bg,
                      &lcd_cache_info);


    //step2. draw hour, min, second image to dstbuf
    if (rotate_buf_ptr == NULL)
    {
        rotate_buf_ptr = SCI_ALLOC_APPZ(rotate_buf_size);
        TRACE_APP_LAUNCHER("malloc rotate_buf_ptr, addr = 0x%0x.", rotate_buf_ptr);
    }


    //display today number
    do
    {
        MMI_STRING_T textStr ={0};
        MMI_STRING_T weektextStr ={0};
        GUISTR_STYLE_T text_style = {0};
        GUISTR_STYLE_T text_week_style = {0};
        GUISTR_STATE_T state = {0};
        SCI_DATE_T s_sys_data = {0};
        GUI_RECT_T rect_day = DP2PX_RECT(104,164, 104+32, 164+22);
        GUI_RECT_T rect_week = DP2PX_RECT(104,186, 104+32, 186+16);
        uint8 data_str[6] = {0};
        wchar data_wstr[6] = {0};
        TM_GetSysDate(&s_sys_data);
        sprintf(data_str, "%d", s_sys_data.mday);
        MMIAPICOM_StrToWstr(data_str, data_wstr);


		textStr.wstr_ptr = data_wstr;
        textStr.wstr_len = MMIAPICOM_Wstrlen(textStr.wstr_ptr);
        text_style.font_color = RGB888_TO_RGB565(0xf4, 0x43, 0x36);
        text_style.font = DP_FONT_22;
        text_style.align = ALIGN_HVMIDDLE;
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_cache_info,
            (const GUI_RECT_T      *)&rect_day,
            (const GUI_RECT_T      *)&rect_day,
            (const MMI_STRING_T    *)&textStr,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO);

		weektextStr = WatchLAUNCHER_GetWeekString(s_sys_data.wday);
        text_week_style.font_color = RGB888_TO_RGB565(0xff, 0xff, 0xff);
        text_week_style.font = DP_FONT_12;
        text_week_style.align = ALIGN_HVMIDDLE;
        GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_cache_info,
            (const GUI_RECT_T      *)&rect_week,
            (const GUI_RECT_T      *)&rect_week,
            (const MMI_STRING_T    *)&weektextStr,
            &text_week_style,
            state,
            GUISTR_TEXT_DIR_AUTO);
		
    } while (0);


    if (rotate_buf_ptr == NULL)
    {
        TRACE_APP_LAUNCHER("malloc rotate_buf_ptr [error].");
        return;
    }

    //
    for (i = 0; i < 3; i++)
    {
        CLOCK_ALPHA_IMG_T bk = {0};
        CLOCK_ALPHA_IMG_T fg = {0};
        SCI_MEMSET(rotate_buf_ptr, 0, rotate_buf_size);
        DisplayImageToBufferWithAngle(rotate_buf_ptr,
                                                            LAUNCHER_WIDTH, 
                                                            LAUNCHER_HEIGHT,
                                                            win_id,
                                                            image_arr[i],
                                                            angle_arr[i]);
        bk.data_ptr = UILAYER_GetLayerBufferPtr(&lcd_cache_info);
        bk.data_type = 0;
        bk.start_x = 0;
        bk.start_y = 0;
        bk.width = LAUNCHER_WIDTH;
        bk.height = LAUNCHER_HEIGHT;

        fg.data_ptr = rotate_buf_ptr;
        fg.data_type = 1;
        fg.start_x = 0;
        fg.start_y = 0;
        fg.width = LAUNCHER_WIDTH;
        fg.height = LAUNCHER_HEIGHT;
        ProcessAlphaBlending(&bk, &fg);
    }

    if (rotate_buf_ptr != NULL)
    {
        TRACE_APP_LAUNCHER("free rotate_buf_ptr, addr = 0x%0x.", rotate_buf_ptr);
        SCI_FREE(rotate_buf_ptr);
    }

    //step3, draw clock red center
    GUIRES_DisplayImg(&clockpanel_pos,
                      PNULL,
                      PNULL,
                      MAIN_IDLE_WIN_ID,
                      res_aw_clock_pointer_point,
                      &lcd_cache_info);


    UILAYER_SetLayerPosition(&lcd_cache_info, display_rect.left, display_rect.top);
    UILAYER_BltLayerToLayer(&lcd, &lcd_cache_info, &rect, FALSE);

    UILAYER_RELEASELAYER(&lcd_cache_info);
#endif
}

/*****************************************************************************
//  Description : Display Digital Clock on win, the display_rect is paint area.
//  Author: zhikun.lv
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchLAUNCHER_DrawDigitalClock(MMI_WIN_ID_T win_id,
                                                                          GUI_RECT_T display_rect,
                                                                          BOOLEAN is_12hour)
{
#ifdef UI_MULTILAYER_SUPPORT
    uint16 display_width = 0;
    uint16 display_height = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
	GUI_POINT_T clockpanel_pos = {0, 0};
    GUI_LCD_DEV_INFO lcd_cache_info = {0, UILAYER_NULL_HANDLE};
    GUI_RECT_T win_rect = {0, 0, LAUNCHER_WIDTH, LAUNCHER_HEIGHT};
    TRACE_APP_LAUNCHER("enter.");
    initItemRects();
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    display_width = display_rect.right - display_rect.left;
    display_height = display_rect.bottom- display_rect.top;

    if (display_width == 0 || display_height == 0)
    {
        //SCI_ASSERT(0);
        TRACE_APP_LAUNCHER("(display_width == 0 || display_height == 0)");
    }


    CreateCacheLayer(&lcd_cache_info, win_id, LAUNCHER_WIDTH, LAUNCHER_HEIGHT);
    if (lcd_cache_info.block_id == UILAYER_NULL_HANDLE)
    {
        //SCI_ASSERT(0);
        TRACE_APP_LAUNCHER("lcd_cache_info.block_id == UILAYER_NULL_HANDLE");
    }


	GUIRES_DisplayImg(&clockpanel_pos,
					  PNULL,
					  PNULL,
					  MAIN_IDLE_WIN_ID,
					  res_aw_clock_bg,
					  &lcd_cache_info);

    DisplayDigitalClock(win_id, lcd_cache_info, is_12hour);

    UILAYER_SetLayerPosition(&lcd_cache_info, display_rect.left, display_rect.top);
    UILAYER_BltLayerToLayer(&lcd_dev_info, &lcd_cache_info, &win_rect, FALSE);

    //release cache layer
    if (lcd_cache_info.block_id != UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&lcd_cache_info);
    }

    TRACE_APP_LAUNCHER("exit.");
#endif
}


/*****************************************************************************
//  Description : Display MTF Clock on win, the display_rect is paint area.
//  Author: haosheng.cui
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC void WatchLAUNCHER_DrawMTFClock(MMI_WIN_ID_T win_id,
                                                                                            GUI_RECT_T display_rect)
{
#ifdef UI_MULTILAYER_SUPPORT
    uint8 i = 0;
    uint32 rotate_buf_size = 0;
    uint16 angle_arr[3] = {0};
    SCI_TIME_T sys_time= {0};
    GUI_POINT_T clockpanel_pos = {0, 0};

    GUI_RECT_T rect = {0, 0, LAUNCHER_WIDTH, LAUNCHER_HEIGHT};
    GUI_LCD_DEV_INFO lcd = {0};
    GUI_LCD_DEV_INFO lcd_cache_info = {0, UILAYER_NULL_HANDLE};
    uint8 *rotate_buf_ptr = NULL;
    MMI_IMAGE_ID_T image_arr[3] = {res_aw_clock_function_hand,res_aw_clock_function_minute,
                                                        res_aw_clock_function_second};

	MMK_GetWinLcdDevInfo(win_id, &lcd);
    TRACE_APP_LAUNCHER("enter.");
    rotate_buf_size = LAUNCHER_WIDTH * LAUNCHER_HEIGHT * sizeof(uint32);

    //get system time
    TM_GetSysTime(&sys_time);
    TRACE_APP_LAUNCHER("sys_time = %d: %d: %d.", sys_time.hour, sys_time.min, sys_time.sec);
#ifdef WIN32
    do
    {
        LOCAL uint8 localsec = 0;
        localsec++;
        localsec %= 60;
        sys_time.sec= localsec;
    } while (0);
#endif

    GRAPH_GetAngleByTimeValue(sys_time.hour, sys_time.min, sys_time.sec, &angle_arr[0], &angle_arr[1], &angle_arr[2]);

    CreateCacheLayer(&lcd_cache_info, win_id, LAUNCHER_WIDTH, LAUNCHER_HEIGHT);



    if (lcd_cache_info.block_id == UILAYER_NULL_HANDLE)
    {
        TRACE_APP_LAUNCHER("CreateCacheLayer [error].");
        return;
    }

    GUIRES_DisplayImg(&clockpanel_pos,
                      PNULL,
                      PNULL,
                      MAIN_IDLE_WIN_ID,
                      res_aw_clock_bg,
                      &lcd_cache_info);

    //step1, draw clock panel
    GUIRES_DisplayImg(&clockpanel_pos,
                      PNULL,
                      PNULL,
                      MAIN_IDLE_WIN_ID,
                      res_aw_clock_function_bg,
                      &lcd_cache_info);
	DrawMTFControlerItem(win_id, lcd_cache_info);// to do


    //step2. draw hour, min, second image to dstbuf
    if (rotate_buf_ptr == NULL)
    {
        rotate_buf_ptr = SCI_ALLOC_APPZ(rotate_buf_size);
        TRACE_APP_LAUNCHER("malloc rotate_buf_ptr, addr = 0x%0x.", rotate_buf_ptr);
    }




    if (rotate_buf_ptr == NULL)
    {
        TRACE_APP_LAUNCHER("malloc rotate_buf_ptr [error].");
        return;
    }

    //
    for (i = 0; i < 3; i++)
    {
        CLOCK_ALPHA_IMG_T bk = {0};
        CLOCK_ALPHA_IMG_T fg = {0};
        SCI_MEMSET(rotate_buf_ptr, 0, rotate_buf_size);
        DisplayImageToBufferWithAngle(rotate_buf_ptr,
                                                            LAUNCHER_WIDTH, 
                                                            LAUNCHER_HEIGHT,
                                                            win_id,
                                                            image_arr[i],
                                                            angle_arr[i]);
        bk.data_ptr = UILAYER_GetLayerBufferPtr(&lcd_cache_info);
        bk.data_type = 0;
        bk.start_x = 0;
        bk.start_y = 0;
        bk.width = LAUNCHER_WIDTH;
        bk.height = LAUNCHER_HEIGHT;

        fg.data_ptr = rotate_buf_ptr;
        fg.data_type = 1;
        fg.start_x = 0;
        fg.start_y = 0;
        fg.width = LAUNCHER_WIDTH;
        fg.height = LAUNCHER_HEIGHT;
        ProcessAlphaBlending(&bk, &fg);
    }

    if (rotate_buf_ptr != NULL)
    {
        TRACE_APP_LAUNCHER("free rotate_buf_ptr, addr = 0x%0x.", rotate_buf_ptr);
        SCI_FREE(rotate_buf_ptr);
    }

    //step3, draw clock red center
    GUIRES_DisplayImg(&clockpanel_pos,
                      PNULL,
                      PNULL,
                      MAIN_IDLE_WIN_ID,
                      res_aw_clock_pointer_point,
                      &lcd_cache_info);


    UILAYER_SetLayerPosition(&lcd_cache_info, display_rect.left, display_rect.top);
    UILAYER_BltLayerToLayer(&lcd, &lcd_cache_info, &rect, FALSE);

    UILAYER_RELEASELAYER(&lcd_cache_info);
#endif
}


/*****************************************************************************
//  Description : Display image by id to dst buffer whit angle.
//  Author: zhikun.lv
//  Param:N/A
//  Return:
//  Note:
*****************************************************************************/
PUBLIC BOOLEAN DisplayImageToBufferWithAngle(uint8 *dst_buf,
    uint16 dst_width,
    uint16 dst_height,
    MMI_WIN_ID_T win_id,
    MMI_IMAGE_ID_T image_id, 
    uint32 angle)
{
    uint32 image_width = 0;
    uint32 image_height = 0;
    uint8 *image_decompress_ptr = NULL;
    uint32 image_decompress_size = 0;

    IMG_RES_SRC_T img_src = {0};
    IMG_RES_INFO_T img_info = {0};
    IMG_RES_DST_T img_dst = {0};
    IMG_RES_DST_CTL_T dstctl = {0};
    IMG_RES_ERROR_E error = IMG_RES_SUCCESS;
    uint32 image_stream_size = 0;
    uint8 *image_stream_ptr = NULL;
    TRACE_APP_LAUNCHER("enter.");

    if (dst_buf == NULL)
    {
        TRACE_APP_LAUNCHER("dst_buf == NULL.");
        return FALSE;
    }

    //step1. get image base info
    image_stream_ptr = (uint8 *)MMI_GetLabelImage(image_id, win_id, &image_stream_size);
    img_src.imgstream_ptr = image_stream_ptr;
    img_src.imgstreamsize = image_stream_size;

    error = ALPHA_RES_GetImageInfo(&img_src, &img_info);

    if (error != IMG_RES_SUCCESS)
    {
        TRACE_APP_LAUNCHER("ALPHA_RES_GetImageInfo() error. error = %d.", error);
        return FALSE;
    }
    image_width = img_info.width;
    image_height = img_info.height;

    //step 2. decode
    image_decompress_size = image_width * image_height * sizeof(uint32);
    image_decompress_ptr = (uint8 *)SCI_ALLOC_APPZ(image_decompress_size);

    TRACE_APP_LAUNCHER("malloc image_decompress_ptr, addr = 0x%0x.", image_decompress_ptr);
    if (image_decompress_ptr == NULL)
    {
        TRACE_APP_LAUNCHER("malloc image_decompress_ptr = error.");
        return FALSE;
    }

    img_src.imgrect.left = 0;
    img_src.imgrect.right = image_width - 1;
    img_src.imgrect.top = 0;
    img_src.imgrect.bottom = image_height - 1;

    img_dst.dstctl = &dstctl;
    img_dst.dstctl->bg_type = IMG_BG_NORMAL;
    img_dst.dstctl->format = IMGREF_FORMAT_ARGB888;
    img_dst.dstctl->dstmem = image_decompress_ptr;
    img_dst.dstctl->width = img_info.width;
    img_dst.dstctl->height = img_info.height;
    img_dst.dstrect.top = 0;
    img_dst.dstrect.left = 0;
    img_dst.dstrect.right = img_info.width - 1;
    img_dst.dstrect.bottom = img_info.height - 1;

    error = ALPHA_RES_Display_Img(&img_src, &img_dst);

    if (error != IMG_RES_SUCCESS)
    {
        TRACE_APP_LAUNCHER("ALPHA_RES_Display_Img() error. error = %d.", error);
        return FALSE;
    }

    //step3 .rotate image && draw it to dstbuf
    do
    {
        ROTATE_ARBITRARY_IN_T	rot_in = {0};
        ROTATE_ARBITRARY_OUT_T	rot_out = {0};
        CLOCK_ALPHA_IMG_T fore_img = {0};
        CLOCK_ALPHA_IMG_T bk_img = {0};
        rot_in.angle = angle;

        rot_in.src_format = IMGREF_FORMAT_ARGB888;
        rot_in.src_size.w = image_width;
        rot_in.src_size.h = image_height;
        rot_in.src_center.x = image_width >> 1;
        rot_in.src_center.y = image_height >> 1;
        rot_in.src_chn.chn0.ptr = image_decompress_ptr;
        rot_in.src_chn.chn0.size = image_decompress_size;

        rot_in.target_format = IMGREF_FORMAT_ARGB888;
        rot_in.target_size.w = dst_width;
        rot_in.target_size.h = dst_height;
        rot_in.target_chn.chn0.ptr = (void *)dst_buf;
        rot_in.target_chn.chn0.size = dst_width * dst_height * sizeof(uint32);
        rot_in.target_center.x = dst_width / 2;
        rot_in.target_center.y = dst_height / 2;

        rot_in.mode = ROTATE_ARBITRARY_AUTO_CROP;

        if (SCI_SUCCESS != GRAPH_RotateArbitrary(&rot_in, &rot_out))
        {
            SCI_FREE(image_decompress_ptr);
            SCI_TRACE_LOW("invoke GRAPH_RotateArbitrary() error.");
            return FALSE;
        }
    } while (0);

    TRACE_APP_LAUNCHER("free image_decompress_ptr, addr = 0x%0x.", image_decompress_ptr);
    if (image_decompress_ptr != NULL)
    {
        SCI_FREE(image_decompress_ptr);
    }

    TRACE_APP_LAUNCHER("exit.");
}
