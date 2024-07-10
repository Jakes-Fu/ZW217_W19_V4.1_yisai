#ifdef SCRIPTEASY_SUPPORT
/*****************************************************************************
** File Name:     gui_multilanguage_func.c                                      *
** Author:                                                                   *
** Date:           4/05/2008                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file provide functions about font liabrary and display string               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME             DESCRIPTION                               *
** 4/05/2008       Jim.Zhang           Create                                  *
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "gui_scripteasy.h"
#include "guicommon.h"
#include "guilcd.h"
#include "mmi_theme.h"
#include "mmi_textfun.h"
//#include "guieditbox.h"

#include "guistring.h"
#include "spml_api.h"

/**---------------------------------------------------------------------------*
**                         Macro Declaration                                  *
**---------------------------------------------------------------------------*/
#ifdef _MSC_VER

/* Suppress Microsoft Visial C++ warning: unreferenced formal parameter */
#pragma warning(disable: 4100)

#endif /* _MSC_VER */

/* For languages using base latin charset */

#define GUI_SE_MAX_FONT_WIDTH 64
#define GUI_SE_MAX_FONT_HEIGHT 64
#define SCRIPTEASY_CACHE_SIZE (24*1024)

//#define SE_ALPHA_ADJUST				1
//#define SE_AUTO_ALPHA_ADJUST		1

#if SE_ALPHA_ADJUST
    #include "se_pallete.h"    
    #define SE_DEFAULT_PALETTE  0
    #define SE_BW_PALETTE   PALETTEBW
    #define SE_WB_PALETTE   SE_DEFAULT_PALETTE
    static const unsigned char *g_SePalette = SE_DEFAULT_PALETTE;
#endif /* SE_ALPHA_ADJUST */

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
// LOCAL uint8* s_font_table_ptr[GUI_SE_MAX_FONT_LIB_NUM + 1];


LOCAL unsigned short Scripteasy_MapFontSize(int size);

LOCAL LcdDeviceInfo s_lcd_di = {0};

LOCAL SeDeviceContext s_default_device_context = {0};

//建立双向链表来构造临时特效显示结点
typedef struct _region_node_tag GUISTRING_REGION_NODE_T;
struct _region_node_tag
{
    GUISTRING_REGION_NODE_T *prev;
    GUISTRING_REGION_NODE_T *next;
    GUIFONT_REGION_STYLE_T region;
};

//下面是双向链表的常规操作，insert、append和remove
LOCAL __inline void InsertNodePtr(
    GUISTRING_REGION_NODE_T *new_ptr,
    GUISTRING_REGION_NODE_T *org_ptr
)
{
    if (PNULL != new_ptr && PNULL != org_ptr)
    {
        if (PNULL != org_ptr->prev) org_ptr->prev->next = new_ptr;
        
        new_ptr->prev = org_ptr->prev;
        new_ptr->next = org_ptr;
        
        org_ptr->prev = new_ptr;
    }

    return;
}

LOCAL __inline void AppendNodePtr(
    GUISTRING_REGION_NODE_T *org_ptr,
    GUISTRING_REGION_NODE_T *new_ptr
)
{
    if(PNULL != new_ptr && PNULL != org_ptr)
    {
        if (PNULL != org_ptr->next) org_ptr->next->prev = new_ptr;
        
        new_ptr->prev = org_ptr;
        new_ptr->next = org_ptr->next;
        
        org_ptr->next = new_ptr;
    }

    return;
}

LOCAL __inline void RemoveNodePtr(GUISTRING_REGION_NODE_T *node_ptr)
{
    if(PNULL != node_ptr)
    {
        if (PNULL != node_ptr->prev) node_ptr->prev->next = node_ptr->next;
        if (PNULL != node_ptr->next) node_ptr->next->prev = node_ptr->prev;
    }

    return;
}

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : scripteasy color to blended lcd color
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL GUI_COLOR_T gui_888_color_to_blended_lcd_color(
                                uint32             src,
                                GUI_COLOR_T         dst,
                                uint8       alpha);

/*****************************************************************************/
//  Description : Device context of Lcd Draw Pixel
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL void lcd_draw_pixel(
            void*               device_data,
            int                 x,
            int                 y,
            unsigned int        color);

/*****************************************************************************/
//  Description : Device context of Lcd Draw Bitmap
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL void lcd_draw_bitmap(
            void*               device_data,
            int                 x_dst,
            int                 y_dst,
            const SeBitmap*     bitmap,
            int                 x_src,
            int                 y_src,
            int                 width,
            int                 height,
            unsigned int        color);

/*****************************************************************************/
//  Description : Device context of Lcd Draw Bitmap
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL void lcd_draw_bitmap_888(
            void*               device_data,
            int                 x_dst,
            int                 y_dst,
            const SeBitmap*     bitmap,
            int                 x_src,
            int                 y_src,
            int                 width,
            int                 height,
            unsigned int        color);

/*****************************************************************************/
//  Description : Device context of Lcd Draw Rect
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL void lcd_draw_rect(
            void*               device_data,
            const SeRectangle*  rect,
            unsigned int        color);

/*****************************************************************************/
//  Description : Device context of Lcd Draw Revert Rect
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL void lcd_revert_rect(
            void*               device_data,
            const SeRectangle*  rect) ;
/*****************************************************************************/
//  Description : Device context of Lcd Update
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL void lcd_update(
            void*               device_data,
            const SeRectangle*  rect);

/*****************************************************************************/
//  Description : Display String in multi-line
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
// LOCAL void display_string(
//             const SPML_TEXT_PARAM_T* param_ptr,  //????
//             const wchar*             text_ptr,   //unicode???
//             uint16                   len,        //????
//             SPML_TEXT_METRICS_T*     metrics_ptr //out, ??????, ???PNULL
//             );


/*******************************************************************************************/
// 	Description : 计算节点数，创建数组
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL void DestoryPtr(
                      void *node_ptr
                      );

/*==============================================================================
Description: CombineNode
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL int32 CombineNode(GUISTRING_REGION_NODE_T **head_pptr);

/*==============================================================================
Description: InsertNode
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL int32 InsertNode(
    GUISTRING_REGION_NODE_T **head_pptr,
    GUIFONT_REGION_STYLE_T const *region_ptr
);

/*******************************************************************************************/
// 	Description : 计算节点数，创建数组
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL int32 CreateRegionPtr(
                            GUIFONT_REGION_STYLE_T const *in_region_ptr, 
                            int32 region_num,
                            GUIFONT_REGION_STYLE_T **out_region_ptr
                            );

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINE                                                                             */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : 888 color to blended 565 color
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL GUI_COLOR_T gui_888_color_to_blended_lcd_color(
                                uint32             src,
                                GUI_COLOR_T         dst,
                                uint8       alpha)
{
    uint8 src_r = 0;
    uint8 src_g = 0;
    uint8 src_b = 0;
    uint8 dst_r = 0;
    uint8 dst_g = 0;
    uint8 dst_b = 0;
    uint8 r = 0;
    uint8 g = 0;
    uint8 b = 0;

    src_r = (uint8) (src >> 16);
    src_g = (uint8) (src >> 8);
    src_b = (uint8) (src);

    dst_r = (uint8) ((dst >> 8) & 0xF8);   //取出5位r放入8位中的高位
    dst_g = (uint8) ((dst >> 3) & 0xFC);
    dst_b = (uint8) (dst << 3);

    r = (uint8) (src_r + (dst_r - src_r) * alpha / 255);
    g = (uint8) (src_g + (dst_g - src_g) * alpha / 255);
    b = (uint8) (src_b + (dst_b - src_b) * alpha / 255);

    return (GUI_COLOR_T) ( ((r << 8) & 0xF800) | ((g << 3) & 0x07E0) | (b >> 3) );
}



/*****************************************************************************/
//  Description : Device context of Lcd Get Pixel
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL secolor lcd_get_pixel(
            void*               device_data,
            int x,
            int y) 
{
    LcdDeviceInfo*      device_info = PNULL;
    GUI_COLOR_T* dst_color = PNULL;

    device_info = (LcdDeviceInfo*)device_data;

    dst_color = device_info->buffer + (y * device_info->width + x);
    return GUI_565_color_to_888_color(*dst_color);
}

/*****************************************************************************/
//  Description : Device context of Lcd Draw Pixel
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL void lcd_draw_pixel(
            void*               device_data,
            int                 x,
            int                 y,
            unsigned int        color)
{
    LcdDeviceInfo*      device_info = PNULL;
    GUI_COLOR_T* dst_color = PNULL;
    uint8 alpha = 0;
    SCI_ASSERT(PNULL != device_data);/*assert verified*/
    device_info = (LcdDeviceInfo*)device_data;

    alpha = (uint8)(color >> 24);

    if (alpha == 255)       /* Transparent */
    {
        return;
    }
    else if (alpha == 0)    /* Opaque */
    {
        dst_color = device_info->buffer + (y * device_info->width + x);
        *dst_color = GUI_888_color_to_565_color(color);
    }
    else                    /* Blended */
    {
        dst_color = device_info->buffer + (y * device_info->width + x);
        *dst_color = gui_888_color_to_blended_lcd_color(color, *dst_color, alpha);
    }
}

#if SE_ALPHA_ADJUST && SE_AUTO_ALPHA_ADJUST

    #define YUV_Y(r, g, b) ((299 * (r) + 587 * (g) + 114 * (b)) >> 10)

    static void se_pt_switch_palette(
        unsigned short  *dest_buf,
        int             dest_width,
        int             width,
        int             height,
        unsigned short  fg_color)
    {
        unsigned short bg_clrs[4];
        unsigned int   temp;
        unsigned short bg_color;
        int            yuv1, yuv2;
        int            r, g, b;
        #define COMP_MASK       0x7C0F81F
        #define COMP_COLOR(c)   (((c) << 16 | (c)) & COMP_MASK)
        
        bg_clrs[0] = *dest_buf;
        bg_clrs[1] = *(dest_buf + width - 1);
        dest_buf += dest_width * (height - 1);
        bg_clrs[2] = *dest_buf;
        bg_clrs[3] = *(dest_buf + width - 1);

        temp = ((COMP_COLOR(bg_clrs[0]) + COMP_COLOR(bg_clrs[1]) + COMP_COLOR(bg_clrs[2]) + COMP_COLOR(bg_clrs[3])) >> 2) & COMP_MASK;
        bg_color = (unsigned short) (temp >> 16 | temp);

        r = (fg_color & 0x1F) << 3;
        g = (fg_color >> 3) & 0xFC;
        b = (fg_color >> 8) & 0xF8;
        yuv1 = YUV_Y(r, g, b);

        r = (bg_color & 0x1F) << 3;
        g = (bg_color >> 3) & 0xFC;
        b = (bg_color >> 8) & 0xF8;
        yuv2 = YUV_Y(r, g, b);

        if(yuv2 < yuv1)
        {
            g_SePalette = SE_BW_PALETTE;
        }
        else
        {
            g_SePalette = SE_DEFAULT_PALETTE;
        }
    }

#endif /* #if SE_ALPHA_ADJUST && SE_AUTO_ALPHA_ADJUST */

/*****************************************************************************/
//  Description : Device context of Lcd Draw Bitmap
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL void lcd_draw_bitmap(
            void*               device_data,
            int                 x_dst,
            int                 y_dst,
            const SeBitmap*     bitmap,
            int                 x_src,
            int                 y_src,
            int                 width,
            int                 height,
            unsigned int        color)
{
    LcdDeviceInfo*      device_info = PNULL;
    int32 i = 0, j = 0;
    const uint8* src_color = PNULL;
    GUI_COLOR_T* dst_color = PNULL;
    uint8 src_r = 0, src_g = 0, src_b = 0;
    GUI_COLOR_T src_565_color = {0};
   

    SCI_ASSERT(PNULL != device_data);/*assert verified*/
    SCI_ASSERT(PNULL != bitmap);/*assert verified*/
    device_info = (LcdDeviceInfo*)device_data;

    src_r = (uint8) (color >> 16);
    src_g = (uint8) (color >> 8);
    src_b = (uint8) (color);    

    src_565_color = (GUI_COLOR_T) ( ((src_r << 8) & 0xF800) | ((src_g << 3) & 0x07E0) | (src_b >> 3) );

    dst_color = device_info->buffer + (y_dst * device_info->width + x_dst);

    if (bitmap->format == SE_BITMAP_GRAY_256)
    {
        uint32 src = ((src_565_color << 16) | src_565_color)  & 0x07E0F81F;
        src_color = (const uint8*)(bitmap->buffer) + (y_src * bitmap->pitch + x_src);
        
		#if SE_ALPHA_ADJUST && SE_AUTO_ALPHA_ADJUST
        se_pt_switch_palette(dst_color, device_info->width, width, height, src_565_color);
        #endif

        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                uint8 alpha = src_color[j];
				#if SE_ALPHA_ADJUST
                if(g_SePalette)
                    alpha = g_SePalette[alpha];
                #endif

				alpha = (uint8)((255 - alpha) >> 3);
                if (alpha == 31)
                {
                }
                else if (alpha == 0)    /* Opaque */
                {
                    dst_color[j] = src_565_color;
                }
                else                    /* Blended */
                {
                    uint32 dst = ((dst_color[j] << 16) | dst_color[j]) & 0x07E0F81F;
                    dst = ((((dst - src) * alpha) >> 5) + src) & 0x07E0F81F;
                    dst_color[j] = (GUI_COLOR_T)((dst >> 16) | dst);
                }
            }
            src_color += bitmap->pitch;
            dst_color += device_info->width;
        }
    }
    else /* bitmap->format == SE_BITMAP_MONO */
    {
        /* A minor optimization to speed up draw bitmap
         *
         * lvjun@komoxo.com
         */
        unsigned int init_mod = ((unsigned int)x_src) % 8;

        src_color = (const uint8*)(bitmap->buffer) + (y_src * bitmap->pitch + ((unsigned int)x_src) / 8);/*lint !e737 */
        for (i = 0; i < height; i++)
        {
            const uint8* b = src_color;
            unsigned int mod = init_mod;

            for (j = 0; j < width; j++)
            {
                uint8 value = *b;
                unsigned int bit = 7 - mod;
                
                if (!bit)
                {
                    b++;
                    mod = 0;
                }
                else
                    mod++;

                if ((value) & (1 << bit))
                {
                    dst_color[j] = src_565_color;
                }
            }
            src_color += bitmap->pitch;
            dst_color += device_info->width;
        }
    }
}

/*lint -esym(526,lcd_draw_mono_bitmap_888)*/
extern void lcd_draw_mono_bitmap_888( 
    unsigned int            init_mod,
    const unsigned char*    src_color,
    unsigned int            pitch,
    int                     width,
    int                     height,
    unsigned long*          dst_color,
    unsigned int            color,
    unsigned int            device_width
);

/*****************************************************************************/
//  Description : Device context of Lcd Draw Bitmap
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL void lcd_draw_bitmap_888(
            void*               device_data,
            int                 x_dst,
            int                 y_dst,
            const SeBitmap*     bitmap,
            int                 x_src,
            int                 y_src,
            int                 width,
            int                 height,
            unsigned int        color)
{
    LcdDeviceInfo*      device_info = PNULL;
    int32 i = 0, j = 0;
    const uint8* src_color = PNULL;
    uint32* dst_color = PNULL;
    uint8 src_r = 0, src_g = 0, src_b = 0;
   
    uint16 src_565_color;

    SCI_ASSERT(PNULL != device_data);/*assert verified*/
    SCI_ASSERT(PNULL != bitmap);/*assert verified*/
    device_info = (LcdDeviceInfo*)device_data;

    src_r = (uint8) (color >> 16);
    src_g = (uint8) (color >> 8);
    src_b = (uint8) (color);    

    src_565_color = (GUI_COLOR_T) ( ((src_r << 8) & 0xF800) | ((src_g << 3) & 0x07E0) | (src_b >> 3) );

    dst_color = (uint32*)(device_info->buffer) + (y_dst * device_info->width + x_dst);

    if (bitmap->format == SE_BITMAP_GRAY_256)
    {
        src_color = (const uint8*)(bitmap->buffer) + (y_src * bitmap->pitch + x_src);
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                uint8 alpha = (uint8)(src_color[j]);

                dst_color[j] = GUI_CalcAlphaAdd(dst_color[j], src_565_color, alpha);
            }
            src_color += bitmap->pitch;
            dst_color += device_info->width;
        }
    }
    else /* bitmap->format == SE_BITMAP_MONO */
    {
        /* A minor optimization to speed up draw bitmap
         *
         * lvjun@komoxo.com
         */
        unsigned int init_mod = ((unsigned int)x_src) % 8;
        src_color = (const uint8*)(bitmap->buffer) + (y_src * bitmap->pitch + ((unsigned int)x_src) / 8);/*lint !e737 */
        lcd_draw_mono_bitmap_888(
            init_mod,
            src_color,
            bitmap->pitch,
            width,
            height,
            dst_color,
            color,
            device_info->width);
    }
}

/*****************************************************************************/
//  Description : Device context of Lcd Draw Rect
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL void lcd_draw_rect(
            void*               device_data,
            const SeRectangle*  rect,
            unsigned int        color)
{
    LcdDeviceInfo*      device_info = PNULL;
    int32 i = 0, j = 0;
    GUI_COLOR_T* dest_color = PNULL;
    GUI_COLOR_T src_565_color = 0;

    device_info = (LcdDeviceInfo*)device_data;

    src_565_color = GUI_888_color_to_565_color(color);

    dest_color = device_info->buffer + (rect->top * device_info->width + rect->left);
    for (i = 0; i < rect->bottom - rect->top; i++)
    {
        for (j = 0; j < rect->right - rect->left; j++)
        {
            dest_color[j] = src_565_color;
        }
        dest_color += device_info->width;
    }
}

/*****************************************************************************/
//  Description : Device context of Lcd Draw Revert Rect
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL void lcd_revert_rect(
            void*               device_data,
            const SeRectangle*  rect) 
{
    LcdDeviceInfo*      device_info = PNULL;
    int32 i = 0, j = 0;
    GUI_COLOR_T* dst_color = PNULL;

    device_info = (LcdDeviceInfo*)device_data;

    dst_color = device_info->buffer + (rect->top * device_info->width + rect->left);
    for (i = 0; i < rect->bottom - rect->top; i++)
    {
        for (j = 0; j < rect->right - rect->left; j++)
        {
            dst_color[j] = (GUI_COLOR_T)(~dst_color[j]);
        }
        dst_color += device_info->width;
    }    
}

/*****************************************************************************/
//  Description : Device context of Lcd Update
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
LOCAL void lcd_update(
            void*               device_data,
            const SeRectangle*  rect)
{
    LcdDeviceInfo*      device_info = PNULL;
    GUI_RECT_T box = {0};

    device_info = (LcdDeviceInfo*)device_data;

    box.left   = (int16)(rect->left);
    box.top    = (int16)(rect->top);
    box.right  = (int16)(rect->right);
    box.bottom = (int16)(rect->bottom);

    MMITHEME_StoreUpdateRect(device_info->dev_info_ptr, box);
}


/*****************************************************************************/
//  Description : Get Device Context
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
PUBLIC SeDeviceContext* GUI_Get_Default_Device_Context(const GUI_LCD_DEV_INFO* dev_info_ptr)
{
    GUI_COLOR_T *buffer;
    uint16 width;
    uint16 height;
    LCD_INFO_T  lcd_info;

    SeDeviceContext *dc = PNULL;

    dc = &s_default_device_context;

    SCI_MEMSET(dc, 0, sizeof(SeDeviceContext));

    if (dev_info_ptr == NULL)
        dev_info_ptr = MMITHEME_GetDefaultLcdDev();

    memset(&lcd_info, 0, sizeof(LCD_INFO_T));
    LCD_GetInfo(dev_info_ptr->lcd_id, &lcd_info);

    buffer = lcd_info.lcdbuff_ptr;
    width = lcd_info.lcd_width;
    height = lcd_info.lcd_height;

    //GUILCD_GetLcdInfo(&buffer, &width, &height, dev_info_ptr);

    s_lcd_di.width = width;
    s_lcd_di.height = height;
    s_lcd_di.buffer = buffer;
    s_lcd_di.dev_info_ptr = dev_info_ptr;

    dc->device_width  = width;
    dc->device_height = height;
    dc->device_data   = &s_lcd_di;

    dc->draw_pixel    = lcd_draw_pixel;
    dc->draw_bitmap   = lcd_draw_bitmap;
    dc->draw_rect     = lcd_draw_rect;
    dc->revert_rect   = lcd_revert_rect;
    dc->update        = lcd_update;

    dc->clip.right    = dc->device_width;
    dc->clip.bottom   = dc->device_height;

    return dc;
}

/*****************************************************************************/
//  Description : Set SE Device Context Font
//  Global resource dependence : 
//   Author: 
//  Note:
/****************************************************************************/
PUBLIC void GUI_Set_Device_Context_Font(SeDeviceContext* dc, GUI_FONT_T font)
{
    dc->font.size = Scripteasy_MapFontSize(font);
/*    dc->font.attr |= SE_LOGFONT_PREFERRED;
    dc->font.id = (unsigned char) GUI_GetLogicFont();*/
    
}

/*****************************************************************************/
//  Description : Init multilanguage
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Initialize_Multilanguage(int language)
{
    uint16 lcdwidth=0;
    uint16 lcdheight=0;
    GUI_COLOR_T *lcdbuf_ptr = PNULL;
    LCD_INFO_T lcd_info;

    GUI_Initialize_Scripteasy_Library(language);

    memset(&lcd_info, 0, sizeof(LCD_INFO_T));
    LCD_GetInfo(MMITHEME_GetDefaultLcdDev()->lcd_id, &lcd_info);

    lcdbuf_ptr = lcd_info.lcdbuff_ptr;
    lcdwidth = lcd_info.lcd_width;
    lcdheight = lcd_info.lcd_height;

//    GUILCD_GetLcdInfo(&lcdbuf_ptr, &lcdwidth, &lcdheight, MMITHEME_GetDefaultLcdDev());

    SCI_MEMSET(&s_lcd_di, 0x00, sizeof(s_lcd_di));

    s_lcd_di.buffer = lcdbuf_ptr;
    s_lcd_di.width = lcdwidth;
    s_lcd_di.height = lcdheight;
    s_lcd_di.dev_info_ptr = MMITHEME_GetDefaultLcdDev();
}

static int Scripteasy_calc_text_length(const uint16 *text)
{
    int len = 0;
    if (text == PNULL)
        return len;

    while(*text++)
        len++;

    return len;
}

static void GUI_DrawTextMultilineWrapChar(SeDeviceContext *dc, 
										   GUI_RECT_T *box,
                                           GUISTR_STATE_T state, 
										   GUI_FONT_T font_type,
										   const wchar* text,
										   uint16 text_length
                                          )
{
	int i, line_num;
	int x, y;
	uint16 char_num = 0;
	uint16 pixel_num = 0;
	uint16 height = (uint16) font_type;
	const wchar* p = text;
	line_num = (box->bottom - box->top) / height;
	x = box->left;
	y = box->top;
	for (i = 0; i < line_num; i++)
	{
		GUI_AnalysisNextLine(p, text_length, state, font_type, (uint16) (box->right - box->left), &char_num, &pixel_num);
		GUI_Draw_text(dc, x, y, p, char_num);
		p += char_num;
		y += height;
		text_length = (uint16) (text_length - char_num);
	}
}
static void	GUI_DrawTextMultilineEllipsize(SeDeviceContext *dc, 
										   GUI_RECT_T *box,
                                           GUISTR_STATE_T state, 
										   GUI_FONT_T font_type,
										   const wchar* text,
										   uint16 text_length)
{
	int i, line_num;
	int x, y;
	uint16 char_num = 0;
	uint16 pixel_num = 0;
	uint16 height = (uint16) font_type;

	const wchar* p = text;

    dc->text_flags |= SE_FLAG_MULTILINE;
    dc->alignment = SE_ALIGN_CENTER;

	line_num = (box->bottom - box->top) / height;

	x = box->left;
	y = box->top;
	for (i = 0; i < line_num - 1; i++)
	{
		GUI_AnalysisNextLine(p, text_length, state, font_type, (uint16) (box->right - box->left), &char_num, &pixel_num);
		GUI_Draw_text(dc, x, y, p, char_num);

		p += char_num;
		y += height;
		text_length = (uint16) (text_length - char_num);
	}

	dc->text_flags &= ~SE_FLAG_MULTILINE;
	dc->ellipsize = SE_ELLIPSIZE_END;
	dc->ellipsize_width = box->right - box->left;
    x = (box->left + box->right) / 2;

	GUI_Draw_text(dc, x, y, p, text_length);

}

LOCAL unsigned short Scripteasy_MapFontSize(int size)
{
#ifdef FONT_TYPE_SUPPORT_VECTOR
    return (unsigned short) size;
#else
    #if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_TRAD_CHINESE_SUPPORT)
        #if defined MAINLCD_SIZE_128X160 || defined MAINLCD_SIZE_176X220
			if (size >= 16)
				return 16;
			else 
				return 14;
        #elif defined MAINLCD_SIZE_128X64
            return 14;
        #else
            if (size <= 18)
                return 14;
            else 
                return 20;
        #endif
    #else
        if (size <= 14)
            return 14;
        else if (size <= 16)
            return 16;
        else if (size <= 18)
            return 18;
        else if (size <= 20)
            return 20;
        else 
            return 24;
    #endif
#endif
}

LOCAL void To_Hex_Str(unsigned char v, wchar* s) {
    int hi = v >> 4;
    int lo = v & 0xF;
    if (hi < 10) {
        s[0] = (wchar) ('0' + hi);
    } else {
        s[0] = (wchar) ('a' + hi - 10);
    }

    if (lo < 10) {
        s[1] = (wchar) ('0' + lo);
    } else {
        s[1] = (wchar) ('a' + lo - 10);
    }
}
LOCAL int Scripteasy_EscapeDelta(const MMI_STRING_T *str_ptr)
{
	int i;
	int delta = 0;
	for (i = 0; i < str_ptr->wstr_len; ++i)
	{
		switch (str_ptr->wstr_ptr[i])
		{
		case '<':
			delta += 3;
			break;
		case '>':
			delta += 3;
			break;
		case '&':
			delta += 4;
			break;
		case '"':
			delta += 5;
			break;
		case '\\':
			delta += 5;
			break;
		}
	}
	return delta;
}
LOCAL int Scripteasy_AppendStr(wchar *dest, const wchar *src, int len)
{
	#define LT_ESCAPE_STR L"&lt;"
	#define GT_ESCAPE_STR L"&gt;"
	#define AMP_ESCAPE_STR L"&amp;"
	#define QUOT_ESCAPE_STR L"&quot;"
	#define APOS_ESCAPE_STR L"&apos;"
    #define WSTR_LEN(s) (sizeof(s) / sizeof(s[0]) - 1)
	int dest_len = 0;
	int i;
	for (i = 0; i < len; ++i)
	{
		wchar c = src[i];
		switch (c)
		{
		case '<':
			MMIAPICOM_Wstrncpy(dest + dest_len, LT_ESCAPE_STR, WSTR_LEN(LT_ESCAPE_STR));
			dest_len += WSTR_LEN(LT_ESCAPE_STR);
			break;
		case '>':
			MMIAPICOM_Wstrncpy(dest + dest_len, GT_ESCAPE_STR, WSTR_LEN(GT_ESCAPE_STR));
			dest_len += WSTR_LEN(GT_ESCAPE_STR);
			break;
		case '&':
			MMIAPICOM_Wstrncpy(dest + dest_len, AMP_ESCAPE_STR, WSTR_LEN(AMP_ESCAPE_STR));
			dest_len += WSTR_LEN(AMP_ESCAPE_STR);
			break;
		case '"':
			MMIAPICOM_Wstrncpy(dest + dest_len, QUOT_ESCAPE_STR, WSTR_LEN(QUOT_ESCAPE_STR));
			dest_len += WSTR_LEN(QUOT_ESCAPE_STR);
			break;
		case '\\':
			MMIAPICOM_Wstrncpy(dest + dest_len, APOS_ESCAPE_STR, WSTR_LEN(APOS_ESCAPE_STR));
			dest_len += WSTR_LEN(APOS_ESCAPE_STR);
			break;
		default:
			dest[dest_len++] = c;
			break;
		}
	}
	#undef WSTR_LEN
	return dest_len;
}
/*lint -e737*/

#define FONT_MARKUP_SAMPLE  L"<font color=\"#000000\" background=\"#000000\">"
#define FONT_MARKUP_PART1   L"<font color=\"#"
#define FONT_MARKUP_PART2   L"\" background=\"#"
#define FONT_MARKUP_PART3   L"\">"
#define FONT_MARKUP_END L"</font>"
#define U_MARKUP L"<u>"
#define U_MARKUP_END L"</u>"
#define B_MARKUP L"<b>"
#define B_MARKUP_END L"</b>"
#define I_MARKUP L"<i>"
#define I_MARKUP_END L"</i>"
#define WSTR_LEN(s) (sizeof(s) / sizeof(s[0]) - 1)

#define INVERSE_FLAG    0x01
#define UNDERLINE_FLAG  0x02
#define ITALIC_FLAG     0x04
#define BOLD_FLAG       0x08

LOCAL int Scripteasy_WriteBeginTag(wchar *new_str, unsigned short flag, secolor text_color, secolor bg_color)
{
    int new_length = 0;
    int offset = 0;

    if (flag & INVERSE_FLAG)
    {
        new_length +=  WSTR_LEN(FONT_MARKUP_SAMPLE);
        if (new_str){
            secolor inverse_text_color = ~text_color;
            secolor inverse_bg_color = ~bg_color;
            MMIAPICOM_Wstrncpy(new_str + offset, FONT_MARKUP_PART1, WSTR_LEN(FONT_MARKUP_PART1));
            offset += WSTR_LEN(FONT_MARKUP_PART1);
            To_Hex_Str((unsigned char) (inverse_text_color >> 16), new_str + offset);
            offset += 2;
            To_Hex_Str((unsigned char) (inverse_text_color >> 8), new_str + offset);
            offset += 2;
            To_Hex_Str((unsigned char) (inverse_text_color), new_str + offset);
            offset += 2;
            MMIAPICOM_Wstrncpy(new_str + offset, FONT_MARKUP_PART2, WSTR_LEN(FONT_MARKUP_PART2));
            offset += WSTR_LEN(FONT_MARKUP_PART2);
            To_Hex_Str((unsigned char) (inverse_bg_color >> 16), new_str + offset);
            offset += 2;
            To_Hex_Str((unsigned char) (inverse_bg_color >> 8), new_str + offset);
            offset += 2;
            To_Hex_Str((unsigned char) (inverse_bg_color), new_str + offset);
            offset += 2;
            MMIAPICOM_Wstrncpy(new_str + offset, FONT_MARKUP_PART3, WSTR_LEN(FONT_MARKUP_PART3));
            offset += WSTR_LEN(FONT_MARKUP_PART3);
        }
    }

    if (flag & UNDERLINE_FLAG)
    {
        new_length += WSTR_LEN(U_MARKUP);
        if (new_str){
            MMIAPICOM_Wstrncpy(new_str + offset, U_MARKUP, WSTR_LEN(U_MARKUP));
            offset += WSTR_LEN(U_MARKUP);
        }
    }

    if (flag & ITALIC_FLAG)
    {
        new_length += WSTR_LEN(I_MARKUP);
        if (new_str){
            MMIAPICOM_Wstrncpy(new_str + offset, I_MARKUP, WSTR_LEN(I_MARKUP));
            offset += WSTR_LEN(I_MARKUP);                
        }
    }

    if (flag & BOLD_FLAG )
    {
        new_length += WSTR_LEN(B_MARKUP);
        if (new_str){
            MMIAPICOM_Wstrncpy(new_str + offset, B_MARKUP, WSTR_LEN(B_MARKUP));
            offset += WSTR_LEN(B_MARKUP);
        }
    }

    return new_length;
}

LOCAL int Scripteasy_WriteEndTag(wchar *new_str, unsigned short flag)
{
    int new_length = 0;
    int offset = 0;

    if (flag & BOLD_FLAG )
    {
        new_length += WSTR_LEN(B_MARKUP_END);
        if (new_str) {
            MMIAPICOM_Wstrncpy(new_str + offset, B_MARKUP_END, WSTR_LEN(B_MARKUP_END));
            offset += WSTR_LEN(B_MARKUP_END);
        }
    }

    if (flag & ITALIC_FLAG)
    {
        new_length += WSTR_LEN(I_MARKUP_END);
        if (new_str){
            MMIAPICOM_Wstrncpy(new_str + offset, I_MARKUP_END, WSTR_LEN(I_MARKUP_END));
            offset += WSTR_LEN(I_MARKUP_END);                
        }
    }

    if (flag & UNDERLINE_FLAG)
    {
        new_length += WSTR_LEN(U_MARKUP_END);
        if (new_str) {
            MMIAPICOM_Wstrncpy(new_str + offset, U_MARKUP_END, WSTR_LEN(U_MARKUP_END));
            offset += WSTR_LEN(U_MARKUP_END);
        }
    }

    if (flag & INVERSE_FLAG)
    {
        new_length +=  WSTR_LEN(FONT_MARKUP_END);
        if (new_str){
            MMIAPICOM_Wstrncpy(new_str + offset, FONT_MARKUP_END, WSTR_LEN(FONT_MARKUP_END));
            offset += WSTR_LEN(FONT_MARKUP_END);
        }
    }

    return new_length;
}

/*lint -e737*/
LOCAL BOOLEAN Scripteasy_BuildMarkupText(
    const MMI_STRING_T    *str_ptr,
    GUISTR_STYLE_T        *style_ptr,
    secolor               text_color,
    secolor               bg_color,
    MMI_STRING_T          *new_str_ptr
                                         )
{
    int i;
	int j = 0;
    int start, end;
    int seg_len;
    int len = 0;
    int new_length = 0;
    int offset;
    wchar *new_str;
    unsigned short flag;
    unsigned short *str_flags;
	unsigned short *pFlag = PNULL;
	GUIFONT_REGION_STYLE_T *region_ptr = PNULL;
	GUISTR_STATE_T state = GUISTR_STATE_NONE;

    str_flags = SCI_Malloc(str_ptr->wstr_len * sizeof(unsigned short), __FILE__, __LINE__);
    if (!str_flags)
        return FALSE;
    memset(str_flags, 0, sizeof(unsigned short) * str_ptr->wstr_len);
    
    for (i = 0; i < style_ptr->region_num; i++)
    {
		//get current region pointer and its state
		region_ptr = style_ptr->region_ptr + i;
        state = region_ptr->state;
		
		//check up the start to avoid buffer overflow
		//default rule:reset start and length
		if (region_ptr->start >= str_ptr->wstr_len)
		{
			region_ptr->start  = 0;
			region_ptr->length = 1;
		}
        pFlag = str_flags + region_ptr->start;

		for (j = 0; j < region_ptr->length && (region_ptr->start + j < str_ptr->wstr_len); j++)
		{			
			if (state & GUISTR_STATE_INVERSE)
			{
				*pFlag = (*pFlag) | INVERSE_FLAG;
			}
			
			if (state & GUISTR_STATE_UNDERLINE)
			{
				*pFlag = (*pFlag) | UNDERLINE_FLAG;
			}
			
			if (state & GUISTR_STATE_ITALIC)
			{
				*pFlag = (*pFlag) | ITALIC_FLAG;
			}
			
			if (state & GUISTR_STATE_BOLD )
			{
				*pFlag = (*pFlag) | BOLD_FLAG;
			}
			
			pFlag++;
		}
    }

    new_length = str_ptr->wstr_len + Scripteasy_EscapeDelta(str_ptr);
    flag = 0;
    for (i = 0; i < str_ptr->wstr_len; i++)
    {
        if (flag != str_flags[i])
        {
            new_length += Scripteasy_WriteEndTag(0, flag);
            new_length += Scripteasy_WriteBeginTag(0, str_flags[i], text_color, bg_color);
            flag = str_flags[i];
        }
    }
    if (flag)
        new_length += Scripteasy_WriteEndTag(0, flag);

    new_str = SCI_Malloc(new_length * sizeof(wchar), __FILE__, __LINE__);
    if (!new_str)
        return FALSE;

    offset = 0;
    flag = 0;
    for (i = 0; i < str_ptr->wstr_len; i++)
    {
        if (flag != str_flags[i])
        {
            offset += Scripteasy_WriteEndTag(new_str + offset, flag);
            offset += Scripteasy_WriteBeginTag(new_str + offset, str_flags[i], text_color, bg_color);
            flag = str_flags[i];
        }

        offset += Scripteasy_AppendStr(new_str + offset, str_ptr->wstr_ptr + i, 1);
    }
    if (flag)
        offset += Scripteasy_WriteEndTag(new_str + offset, flag);

    new_str_ptr->wstr_ptr = new_str;
    new_str_ptr->wstr_len = (uint16) new_length;

    SCI_Free(str_flags);
    return TRUE;
}

/*****************************************************************************/
//  Description : Display String in multi-line
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
GUI_RECT_T Scripteasy_DrawText(
                            GUISTR_BUF_T          *des_buf_ptr,
                            const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                            const GUI_RECT_T      *clip_rect_ptr,       //用户要剪切的实际区域
                            uint16                x_offset,
                            const MMI_STRING_T    *str_ptr,
                            GUISTR_STYLE_T        *style_ptr,
                            GUISTR_STATE_T        state,
                            int8                  dir)
{
    GUI_RECT_T   box;
    int x = 0, y = 0;
    int width = 0, height = 0;
    SeDeviceContext dc = {0};
    int box_height = disp_rect_ptr->bottom - disp_rect_ptr->top + 1;
    LcdDeviceInfo dev_info = {0};

    int text_length = Scripteasy_calc_text_length(str_ptr->wstr_ptr);
    if (text_length > str_ptr->wstr_len)
        text_length = str_ptr->wstr_len;

	box.left = disp_rect_ptr->left;
    box.top  = disp_rect_ptr->top;
    box.right = (int16)(disp_rect_ptr->right + 1);
    box.bottom = (int16)(disp_rect_ptr->bottom + 1);

    if (PNULL == str_ptr->wstr_ptr)
        return box;
    SCI_ASSERT(PNULL != str_ptr->wstr_ptr);/*assert verified*/

	dc.device_width  = des_buf_ptr->width;
	dc.device_height = des_buf_ptr->height;
	
	dc.draw_pixel    = lcd_draw_pixel;
    if (des_buf_ptr->type == GUISTR_BUF_ARGB888)
        dc.draw_bitmap   = lcd_draw_bitmap_888;
    else
        dc.draw_bitmap   = lcd_draw_bitmap;
    dc.draw_rect     = lcd_draw_rect;
    dc.revert_rect   = lcd_revert_rect;
    dc.update        = 0;//lcd_update;

    /* lvjun@komoxo.com to fix netfront text display bug begin */
    dev_info.buffer = des_buf_ptr->buffer;
    dev_info.width = des_buf_ptr->width;
    dev_info.height = des_buf_ptr->height;
    dev_info.type = des_buf_ptr->type;
    dc.device_data = &dev_info;
    /* lvjun@komoxo.com to fix netfront text display bug end */

    dc.clip.left   = disp_rect_ptr->left;
    dc.clip.top    = disp_rect_ptr->top;
    dc.clip.right  = disp_rect_ptr->right + 1;
    dc.clip.bottom = disp_rect_ptr->bottom + 1;

    if (clip_rect_ptr)
    {
		SeRectangle rect;
        rect.left   = clip_rect_ptr->left;
        rect.top    = clip_rect_ptr->top;
        rect.right  = clip_rect_ptr->right + 1;
        rect.bottom = clip_rect_ptr->bottom + 1;

		se_intersect_rect(&dc.clip, &dc.clip, &rect);
    }
    
    dc.font.size = Scripteasy_MapFontSize(style_ptr->font);
/*    dc.font.attr |= SE_LOGFONT_PREFERRED;
    dc.font.id = (unsigned char) GUI_GetLogicFont();*/

    if (state & GUISTR_STATE_SINGLE_LINE)
    {
        if (state & GUISTR_STATE_ELLIPSIS)
        {
            dc.ellipsize = SE_ELLIPSIZE_END;
            dc.ellipsize_width = disp_rect_ptr->right - disp_rect_ptr->left + 1;
        }
    }
    else
    {
        dc.text_flags = SE_FLAG_MULTILINE;
        dc.wrap_width = disp_rect_ptr->right - disp_rect_ptr->left + 1;
    }
    
    dc.text_color = GUI_565_color_to_888_color(style_ptr->font_color);


    if (FONT_EFFECT_REVERSE_EDGE & style_ptr->effect)
    {
        dc.text_flags |= SE_FLAG_BORDER;
        dc.bk_color = GUI_565_color_to_888_color((GUI_COLOR_T)(~(style_ptr->font_color)));
        se_inflate_rect(&dc.clip, 1, 1);
    }
    else if (FONT_EFFECT_WIHTE_EDGE & style_ptr->effect)
    {
        dc.text_flags |= SE_FLAG_BORDER;
        dc.bk_color = GUI_565_color_to_888_color(0xFFFF);
        se_inflate_rect(&dc.clip, 1, 1);
    }
    else if (FONT_EFFECT_CUSTOM & style_ptr->effect)
    {
        dc.text_flags |= SE_FLAG_BORDER;
        dc.bk_color = GUI_565_color_to_888_color(style_ptr->edge_color);
        se_inflate_rect(&dc.clip, 1, 1);
    }


    if (GUISTR_STATE_UNDERLINE & state)
    {
        dc.font.attr |= SE_LOGFONT_UNDERLINE;
        dc.underline_color = GUI_565_color_to_888_color(style_ptr->line_color);
    }

    if (GUISTR_STATE_ITALIC & state)
    {
        dc.font.attr |= SE_LOGFONT_ITALIC;
    }

    if (GUISTR_STATE_BOLD & state)
    {
        dc.font.attr |= SE_LOGFONT_BOLD;
    }

    x = box.left;
    y = box.top;
    
    //get single line height
    GUI_Get_Text_Info_Ex(&dc, str_ptr->wstr_ptr, text_length, dir == TEXT_DIR_RTL ? 1 : 0, &width, &height, 0);

    if (state & GUISTR_STATE_ALIGN)
    {
        int align = style_ptr->align;
        if ((state & GUISTR_STATE_ALIGN_BY_DIR) && dir == TEXT_DIR_AUTO)
        {
            if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection()) {
                if (align == ALIGN_LEFT) 
                    align = ALIGN_RIGHT;
                else if (align == ALIGN_LVMIDDLE)
                    align = ALIGN_RVMIDDLE;
                else if (align == ALIGN_RIGHT) 
                    align = ALIGN_LEFT;
                else if (align == ALIGN_RVMIDDLE)
                    align = ALIGN_LVMIDDLE;
            }
        }

        if (dir == TEXT_DIR_RTL) {
            if (align == ALIGN_LEFT || align == ALIGN_LVMIDDLE)
            {
                if (dc.text_flags & SE_FLAG_MULTILINE)
                    dc.alignment = SE_ALIGN_RIGHT;
                else
                    x = box.right - width;
            }
            else if (align == ALIGN_HMIDDLE || align == ALIGN_HVMIDDLE)
            {
                if (dc.text_flags & SE_FLAG_MULTILINE)
                    dc.alignment = SE_ALIGN_CENTER;
                else {
                    x += (box.right - box.left)  / 2;
                    dc.alignment = SE_ALIGN_CENTER;
                }
            }
        }
        else {
            if (align == ALIGN_RIGHT || align == ALIGN_RVMIDDLE)
            {
                if (dc.text_flags & SE_FLAG_MULTILINE)
                    dc.alignment = SE_ALIGN_RIGHT;
                else
                    x = box.right - width;
            }
            else if (align == ALIGN_HMIDDLE || align == ALIGN_HVMIDDLE)
            {
                if (dc.text_flags & SE_FLAG_MULTILINE)
                    dc.alignment = SE_ALIGN_CENTER;
                else {
                    x += (box.right - box.left)  / 2;
                    dc.alignment = SE_ALIGN_CENTER;
                }
            }
        }
    }
    else if (state & GUISTR_STATE_ALIGN_BY_DIR)
    {
        if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
        {
            x = box.right + x_offset;
            dc.alignment = SE_ALIGN_RIGHT;
        }
        else
        {
            x = box.left - x_offset;
            dc.alignment = SE_ALIGN_LEFT;
        }
    }
    else
    {
        if (style_ptr->align == ALIGN_RIGHT || style_ptr->align == ALIGN_RVMIDDLE)
        {
            if (dc.text_flags & SE_FLAG_MULTILINE)
                dc.alignment = SE_ALIGN_RIGHT;
            else
                x = box.right - width;
        }
        else if (style_ptr->align == ALIGN_HMIDDLE || style_ptr->align == ALIGN_HVMIDDLE)
        {
            if (dc.text_flags & SE_FLAG_MULTILINE)
                dc.alignment = SE_ALIGN_CENTER;
            else {
                x += (box.right - box.left)  / 2;
                dc.alignment = SE_ALIGN_CENTER;
            }
        }
    }

    if ((box.bottom - box.top) > height)
    {
        if (style_ptr->align == ALIGN_BOTTOM)
        {
            y = box.bottom - height;
        }
        else if (style_ptr->align == ALIGN_VMIDDLE || 
                 style_ptr->align == ALIGN_HVMIDDLE || 
                 style_ptr->align == ALIGN_LVMIDDLE || 
                 style_ptr->align == ALIGN_RVMIDDLE )
        {
            y = box.top + (box_height - height) / 2;
        }
    }
    else if (style_ptr->align == ALIGN_HVMIDDLE)
    {

		GUI_DrawTextMultilineEllipsize(&dc, &box, state, style_ptr->font, str_ptr->wstr_ptr, (uint16) text_length);
		box.right = (int16)(box.left + width - 1);
		box.bottom = (int16)(box.top + height - 1);

		return box;
    }

    //make effective clip bottom
    //dc->clip.bottom = y + height; /* Buggy. Removed by lvjun@komoxo.com at 20100510 */

    if (style_ptr->region_num == 0)
    {
        if (((dc.text_flags & SE_FLAG_MULTILINE) && (state & GUISTR_STATE_WORDBREAK) == 0))
        {
            GUI_DrawTextMultilineWrapChar(&dc, &box, state, style_ptr->font, str_ptr->wstr_ptr, (uint16) text_length);
        }
        else
        {
        GUI_Draw_text_Ex(&dc, x, y, str_ptr->wstr_ptr, text_length, dir == TEXT_DIR_RTL ? 1 : 0);
        }
    }
    else
    {
        MMI_STRING_T new_str;
        secolor bg_color = GUI_565_color_to_888_color(style_ptr->bg_color);//lcd_get_pixel(dc.device_data, x + width / 2, y + dc.font.size / 2);
        secolor text_color = GUI_565_color_to_888_color(style_ptr->region_ptr->font_color);
        if (Scripteasy_BuildMarkupText(str_ptr, style_ptr, text_color, bg_color, &new_str))
        {
            dc.text_flags |= SE_FLAG_MARKUP;
            GUI_Draw_text_Ex(&dc, x, y, new_str.wstr_ptr, new_str.wstr_len, dir == TEXT_DIR_RTL ? 1 : 0);
            SCI_Free(new_str.wstr_ptr);
        }
        else 
        {
	        GUI_Draw_text_Ex(&dc, x, y, str_ptr->wstr_ptr, text_length, dir == TEXT_DIR_RTL ? 1 : 0);
        }
    }

    box.right = (int16)(box.left + width - 1);
    box.bottom = (int16)(box.top + height - 1);

    return box;
}


/* 
 *
 *  implementions of guistring.c
 *
 */

/*****************************************************************************/
// 	Description : 计算剪切区域, 得到两个区域的共同区域
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetTrimRect(
                             GUI_RECT_T rect1, 
                             GUI_RECT_T rect2
                             )
{
    GUI_RECT_T trim_rect = {0};

    trim_rect.left = (int16)MAX(rect1.left, rect2.left);
    trim_rect.top = (int16)MAX(rect1.top, rect2.top);
    trim_rect.right = (int16)MIN(rect1.right, rect2.right);
    trim_rect.bottom = (int16)MIN(rect1.bottom, rect2.bottom);

    return (trim_rect);
}

/*****************************************************************************/
// 	Description : 转为string的buffer类型
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
LOCAL GUISTR_BUF_TYPE_T ConvertStringBufferType(
                                                DISPLAY_DATA_TYPE_E type
                                                )
{
    GUISTR_BUF_TYPE_T result = GUISTR_BUF_RGB565;
    
    if ( DATA_TYPE_ARGB888 == type )
    {
        result = GUISTR_BUF_ARGB888;
    }
    
    return result;
}


/*****************************************************************************/
// 	Description : 往LCD上画字符串
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDSingleLine( 
                                           const GUI_LCD_DEV_INFO *dev_info_ptr,
                                           const GUI_POINT_T      *point_ptr,
                                           const MMI_STRING_T     *str_ptr,
                                           GUISTR_STYLE_T         *style_ptr,
                                           GUISTR_STATE_T         state
                                           )
{
    GUI_RECT_T      box = {0};
    GUISTR_BUF_T    des_buf_data = {0};
#ifdef UI_MULTILAYER_SUPPORT
    GUI_RECT_T      blt_rect = {0};
#endif
    GUI_POINT_T     point = {0};
    GUILCD_DRAW_DC_T draw_dc = {0};

    // 入参检测
    if (PNULL == dev_info_ptr || PNULL == point_ptr || PNULL == str_ptr || PNULL == style_ptr)
    {
        return;
    }

    if (!LCD_PrepareDrawParam(dev_info_ptr, &draw_dc))
    {
        return;
    }
    des_buf_data.buffer = draw_dc.lcd_buffer_ptr;
    des_buf_data.mem_width = (uint16) draw_dc.mem_width;
    des_buf_data.width = (uint16) draw_dc.width;
    des_buf_data.height = (uint16) draw_dc.height;
    des_buf_data.type = ConvertStringBufferType( draw_dc.data_type );

    point_ptr = LCD_ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, point_ptr, &point);

#ifdef UI_MULTILAYER_SUPPORT
    blt_rect.left = point_ptr->x;
    blt_rect.top = point_ptr->y;
    blt_rect.right = (short) (draw_dc.width-1);
    blt_rect.bottom = (short) (draw_dc.height-1);
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, (GUI_LCD_DEV_INFO *)dev_info_ptr, &blt_rect, &blt_rect);
    UILAYER_BltLowerLayerWhenTransparent((GUI_LCD_DEV_INFO *)dev_info_ptr, &blt_rect);
#endif

    box = GUISTR_DrawTextSingleLine(&des_buf_data, point_ptr, str_ptr, style_ptr, state);

    MMITHEME_StoreUpdateRect(dev_info_ptr, box);
}
/*****************************************************************************/
// 	Description : 往buffer上画文本信息，以起始点为基准
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 起始点的位置相对于buffer的原点
/*****************************************************************************/
PUBLIC GUI_RECT_T GUISTR_DrawTextSingleLine( 
                                            GUISTR_BUF_T           *des_buf_ptr,
                                            const GUI_POINT_T      *point_ptr,      //start point to display the string relative des_buf
                                            const MMI_STRING_T     *str_ptr,
                                            GUISTR_STYLE_T         *style_ptr,
                                            GUISTR_STATE_T         state
                                            )
{
    GUI_RECT_T   box = {0};
    GUI_RECT_T   disp_rect = {0};
    GUI_RECT_T   orig_clip_rect = {0};

    // 入参检测
    if (PNULL == des_buf_ptr || PNULL == point_ptr || PNULL == str_ptr || PNULL == str_ptr->wstr_ptr || PNULL == style_ptr)
    {
        return orig_clip_rect;
    }

    // 计算显示的区域，相对于des buffer的原点
    disp_rect.left = point_ptr->x;
    disp_rect.top = point_ptr->y;
    disp_rect.right = (short) (des_buf_ptr->width - 1);
    disp_rect.bottom = (short) (des_buf_ptr->height - 1);

    // 检查现实的区域是否有效
    if (GUI_IsInvalidRect(disp_rect))
    {
        return orig_clip_rect;
    }

    orig_clip_rect = disp_rect;

    state = (state | GUISTR_STATE_SINGLE_LINE);

    box = Scripteasy_DrawText(des_buf_ptr, &disp_rect, NULL, 0, str_ptr, style_ptr, state, TEXT_DIR_AUTO);

    orig_clip_rect = GetTrimRect(orig_clip_rect, box);

    if (GUI_IsInvalidRect(orig_clip_rect))
    {
        SCI_MEMSET(&orig_clip_rect, 0, sizeof(GUI_RECT_T));
    }

    return orig_clip_rect;
}

#ifndef GUI_STR_MINI_SUPPORT
/*****************************************************************************/
// 	Description : 往LCD上画字符串
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDMultiLine( 
                                          const GUI_LCD_DEV_INFO *dev_info_ptr,
                                          const GUI_POINT_T      *point_ptr,
                                          const MMI_STRING_T     *str_ptr,
                                          GUISTR_STYLE_T         *style_ptr,
                                          GUISTR_STATE_T         state
                                          )
{
    GUI_RECT_T      box = {0};
    GUISTR_BUF_T    des_buf_data = {0};
#ifdef UI_MULTILAYER_SUPPORT
    GUI_RECT_T      blt_rect = {0};
#endif
    GUI_POINT_T     point = {0};
    GUILCD_DRAW_DC_T draw_dc = {0};

    // 入参检测
    if (PNULL == dev_info_ptr || PNULL == point_ptr || PNULL == str_ptr || PNULL == style_ptr)
    {
        return;
    }

    if (!LCD_PrepareDrawParam(dev_info_ptr, &draw_dc))
    {
        return;
    }
    des_buf_data.buffer = draw_dc.lcd_buffer_ptr;
    des_buf_data.mem_width = (uint16) draw_dc.mem_width;
    des_buf_data.width = (uint16) draw_dc.width;
    des_buf_data.height = (uint16) draw_dc.height;
    des_buf_data.type = ConvertStringBufferType( draw_dc.data_type );

    point_ptr = LCD_ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, point_ptr, &point);
#ifdef UI_MULTILAYER_SUPPORT
    blt_rect.left = point_ptr->x;
    blt_rect.top = point_ptr->y;
    blt_rect.right = (short) (draw_dc.width-1);
    blt_rect.bottom = (short) (draw_dc.height-1);
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_DISP2LCD, (GUI_LCD_DEV_INFO *)dev_info_ptr, &blt_rect, &blt_rect);
    UILAYER_BltLowerLayerWhenTransparent((GUI_LCD_DEV_INFO *)dev_info_ptr, &blt_rect);
#endif

    box = GUISTR_DrawTextMultiLine(&des_buf_data, point_ptr, str_ptr, style_ptr, state);

    MMITHEME_StoreUpdateRect(dev_info_ptr, box);
}

/*****************************************************************************/
// 	Description : 往buffer上画文本信息，以起始点为基准，以buffer的宽度和高度为界限
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 起始点的位置相对于buffer的原点
/*****************************************************************************/
PUBLIC GUI_RECT_T GUISTR_DrawTextMultiLine( 
                                           GUISTR_BUF_T           *des_buf_ptr,
                                           const GUI_POINT_T      *point_ptr,      //start point to display the string relative des_buf
                                           const MMI_STRING_T     *str_ptr,
                                           GUISTR_STYLE_T         *style_ptr,
                                           GUISTR_STATE_T         state
                                           )
{
    GUI_RECT_T   disp_rect  = {0};
    GUI_RECT_T   act_rect   = {0};

    // 入参检测
    if (PNULL == des_buf_ptr || PNULL == point_ptr || PNULL == str_ptr || PNULL == style_ptr)
    {
        return act_rect;
    }

    // 计算显示的区域，相对于des buffer的原点
    disp_rect.left = point_ptr->x;
    disp_rect.top = point_ptr->y;
    disp_rect.right = (short) (des_buf_ptr->width - 1);
    disp_rect.bottom = (short) (des_buf_ptr->height - 1);

    return GUISTR_DrawTextInRect( des_buf_ptr, &disp_rect, &disp_rect, str_ptr, style_ptr, state, TEXT_DIR_AUTO );
}
#endif /* #ifndef GUI_STR_MINI_SUPPORT */

/*****************************************************************************/
// 	Description : 根据原始区域坐标和旋转角度，计算旋转后的坐标
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 起始点的位置相对于buffer的原点
/*****************************************************************************/
LOCAL BOOLEAN CalculateRotateRect(
                                  const GUI_RECT_T *orig_rect_ptr,  // 原始区域坐标
                                  int16 x_offset,             // 坐标原点在x方向上的位移
                                  int16 y_offset,             // 坐标原点在y方向上的位移
                                  GUI_ANGLE_E angle,          // 旋转角度
                                  GUI_RECT_T *result_rect_ptr // 输出参数
                                  )
{
    BOOLEAN reslut = FALSE;
    GUI_POINT_T orig_point1 = {0}; // 对应于原始区域的左上角坐标
    GUI_POINT_T orig_point2 = {0}; // 对应于原始区域的右上角坐标
    GUI_POINT_T orig_point3 = {0}; // 对应于原始区域的右下角坐标
    GUI_POINT_T orig_point4 = {0}; // 对应于原始区域的左下角坐标
    GUI_POINT_T tmp_point = {0};   // 临时点
    int16 orig_rect_width = 0;
    int16 orig_rect_height = 0;

    if(PNULL != orig_rect_ptr && PNULL != result_rect_ptr && !GUI_IsInvalidRect(*orig_rect_ptr))
    {
        // 初始化宽度和高度，目的是为了计算四个点的原始坐标
        orig_rect_width = (short) (orig_rect_ptr->right - orig_rect_ptr->left + 1);
        orig_rect_height = (short) (orig_rect_ptr->bottom - orig_rect_ptr->top + 1);
        // 初始化坐标值
        orig_point1.x = orig_rect_ptr->left;
        orig_point1.y = orig_rect_ptr->top;
        orig_point2.x = orig_rect_ptr->right;
        orig_point2.y = orig_rect_ptr->top;
        orig_point3.x = orig_rect_ptr->right;
        orig_point3.y = orig_rect_ptr->bottom;
        orig_point4.x = orig_rect_ptr->left;
        orig_point4.y = orig_rect_ptr->bottom;

        // 在x和y方向上平移之后的坐标
        orig_point1.x = (short) (orig_point1.x - x_offset);
        orig_point1.y = (short) (orig_point1.y - y_offset);
        orig_point2.x = (short) (orig_point2.x - x_offset);
        orig_point2.y = (short) (orig_point2.y - y_offset);
        orig_point3.x = (short) (orig_point3.x - x_offset);
        orig_point3.y = (short) (orig_point3.y - y_offset);
        orig_point4.x = (short) (orig_point4.x - x_offset);
        orig_point4.y = (short) (orig_point4.y - y_offset);
        // 按角度旋转之后的坐标
        switch(angle)
        {
        case ANGLE_0:
            // 旋转角度为0的时候坐标不变
            result_rect_ptr->left = orig_point1.x;
            result_rect_ptr->top = orig_point1.y;
            result_rect_ptr->right = orig_point3.x;
            result_rect_ptr->bottom = orig_point3.y;
            break;
            
        case ANGLE_90:
            // 旋转90度时，区域的变成了由2、4两点确定
            tmp_point = orig_point2;
            orig_point2.x = tmp_point.y;
            orig_point2.y = (short) -tmp_point.x;
            tmp_point = orig_point4;
            orig_point4.x = tmp_point.y;
            orig_point4.y = (short) -tmp_point.x;
            result_rect_ptr->left = orig_point2.x;
            result_rect_ptr->top = orig_point2.y;
            result_rect_ptr->right = orig_point4.x;
            result_rect_ptr->bottom = orig_point4.y;
            break;
            
        case ANGLE_180:
            // 旋转180度，区域变为由3、1确定
            tmp_point = orig_point1;
            orig_point1.x = (short) -tmp_point.x;
            orig_point1.y = (short) -tmp_point.y;
            tmp_point = orig_point3;
            orig_point3.x = (short) -tmp_point.x;
            orig_point3.y = (short) -tmp_point.y;
            result_rect_ptr->left = orig_point3.x;
            result_rect_ptr->top = orig_point3.y;
            result_rect_ptr->right = orig_point1.x;
            result_rect_ptr->bottom = orig_point1.y;
            break;
            
        case ANGLE_270:
            // 旋转270度，区域由4、2确定
            tmp_point = orig_point4;
            orig_point4.x = (short) -tmp_point.y;
            orig_point4.y = tmp_point.x;
            tmp_point = orig_point2;
            orig_point2.x = (short) -tmp_point.y;
            orig_point2.y = tmp_point.x;
            result_rect_ptr->left = orig_point4.x;
            result_rect_ptr->top = orig_point4.y;
            result_rect_ptr->right = orig_point2.x;
            result_rect_ptr->bottom = orig_point2.y;
            break;
            
        default:
		return FALSE;
            /*SCI_ASSERT(0);  不支持其他角度的旋转assert verified
            break;*/
        }
        reslut = TRUE;
    }

    return reslut;
}

/*****************************************************************************/
// 	Description : copy buffer, 原始buffer和目标buffer的大小一定相同，否则会出现异常
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 按顺时针方向旋转
/*****************************************************************************/
LOCAL BOOLEAN RotateBufferData(
                               GUISTR_BUF_TYPE_T type,
                               const void *src_data_ptr, 
                               int16  src_x,      // 原始起始点x坐标
                               int16  src_y,      // 原始起始点y坐标
                               uint16 src_width,  // 原始buffer的宽度
                               uint16 src_heigth, // 原始buffer的高度
                               void *dst_data_ptr,
                               int16  dst_x,      // 目标起始点x坐标
                               int16  dst_y,      // 目标起始点y坐标
                               uint16 dst_width,  // 目标buffer的宽度
                               uint16 dst_heigth, // 目标buffer的高度
                               GUI_ANGLE_E angle,  // 旋转角度
                               uint16 cp_width,  // 被copy的宽度
                                uint16 cp_heigth // 被copy的高度
                               )
{
    BOOLEAN result = FALSE;
    int16 i = 0, j = 0;
    
    SCI_ASSERT(PNULL != src_data_ptr && PNULL != dst_data_ptr);/*assert verified*/
    
    //只支持565和888的buffer旋转
    if ( GUISTR_BUF_RGB565 != type
        && GUISTR_BUF_ARGB888 != type )
    {
        return result;
    }

    //只支持565和888的buffer旋转
    if ( GUISTR_BUF_RGB565 == type )
    {
        const GUI_COLOR_T *tmp_src_ptr = PNULL;
        const GUI_COLOR_T *src_ptr = (const GUI_COLOR_T *)src_data_ptr;
        GUI_COLOR_T* dst_ptr = (GUI_COLOR_T*)dst_data_ptr;
        
        // 源buffer的起始位置
        tmp_src_ptr = src_ptr + src_y*src_width + src_x;
        
        switch(angle)
        {
        case ANGLE_0:
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    //if ( tmp_src_ptr[j] )
                    {
                        dst_ptr [( dst_x + j ) + ( dst_y + i ) * dst_width] = tmp_src_ptr[j];
                    }
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        case ANGLE_90:
            // 旋转之后，目标起始点x坐标，y坐标不变
            dst_x = (short) (dst_x + cp_heigth - 1);
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    dst_ptr [( dst_x - i ) + ( dst_y + j ) * dst_width] = tmp_src_ptr[j];
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        case ANGLE_180:
            // 旋转之后，x，y坐标均发生变化
            dst_x = (short) (dst_x + cp_width - 1);
            dst_y = (short) (dst_y + cp_heigth - 1);
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    //if ( tmp_src_ptr[j] )
                    {
                        dst_ptr [( dst_x - j ) + ( dst_y - i ) * dst_width] = tmp_src_ptr[j];
                    }
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        case ANGLE_270:
            // 旋转之后，y坐标发生变化，x坐标不变
            dst_y = (short) (dst_y + cp_width - 1);
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    //if ( tmp_src_ptr[j] )
                    {
                        dst_ptr [( dst_x + i ) + ( dst_y - j ) * dst_width] = tmp_src_ptr[j];
                    }
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        default:
		return FALSE;

            /*SCI_ASSERT(0);  不支持其他角度旋转
            break;*/
        }
    }
    else
    {
        const uint32 *tmp_src_ptr = PNULL;
        const uint32 *src_ptr = (const uint32 *)src_data_ptr;
        uint32* dst_ptr = (uint32*)dst_data_ptr;
        
        // 源buffer的起始位置
        tmp_src_ptr = src_ptr + src_y*src_width + src_x;
        
        switch(angle)
        {
        case ANGLE_0:
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    //if ( tmp_src_ptr[j] )
                    {
                        dst_ptr [( dst_x + j ) + ( dst_y + i ) * dst_width] = tmp_src_ptr[j];
                    }
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        case ANGLE_90:
            // 旋转之后，目标起始点x坐标，y坐标不变
            dst_x = (short) (dst_x + cp_heigth - 1);
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    //if ( tmp_src_ptr[j] )
                    {
                        dst_ptr [( dst_x - i ) + ( dst_y + j ) * dst_width] = tmp_src_ptr[j];
                    }
                    
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        case ANGLE_180:
            // 旋转之后，x，y坐标均发生变化
            dst_x = (short)(dst_x + cp_width - 1);
            dst_y = (short) (dst_y + cp_heigth - 1);
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    //if ( tmp_src_ptr[j] )
                    {
                        dst_ptr [( dst_x - j ) + ( dst_y - i ) * dst_width] = tmp_src_ptr[j];
                    }
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        case ANGLE_270:
            // 旋转之后，y坐标发生变化，x坐标不变
            dst_y = (short) (cp_width - 1);
            for(i = 0; i < cp_heigth; i++)
            {
                for (j = 0; j < cp_width; j++)
                {
                    //if ( tmp_src_ptr[j] )
                    {
                        dst_ptr [( dst_x + i ) + ( dst_y - j ) * dst_width] = tmp_src_ptr[j];
                    }
                }
                
                tmp_src_ptr += (src_width);
            }
            break;
            
        default:
		return FALSE;

            /*SCI_ASSERT(0);  不支持其他角度旋转
            break;*/
        }
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : 往LCD上画字符串
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDInRect( 
                                       const GUI_LCD_DEV_INFO *dev_info_ptr,
                                       const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                       const GUI_RECT_T      *clip_rect_ptr,       //用户要剪切的实际区域
                                       const MMI_STRING_T    *str_ptr,
                                       GUISTR_STYLE_T        *style_ptr,
                                       GUISTR_STATE_T        state,
                                       int8                  dir
                                       )
{
    GUI_RECT_T      box = {0};
#ifdef UI_MULTILAYER_SUPPORT
    GUI_RECT_T      disp_rect = {0};
    GUI_RECT_T      clip_rect = {0};
#endif
    GUISTR_BUF_T    des_buf_data = {0};
    GUILCD_DRAW_DC_T draw_dc = {0};

    SCI_ASSERT(PNULL != dev_info_ptr);/*assert verified*/

    if (!LCD_PrepareDrawParam(dev_info_ptr, &draw_dc))
    {
        return;
    }

    des_buf_data.buffer = draw_dc.lcd_buffer_ptr;
    des_buf_data.mem_width = (uint16) draw_dc.mem_width;
    des_buf_data.width = (uint16) draw_dc.width;
    des_buf_data.height = (uint16) draw_dc.height;
    des_buf_data.type = ConvertStringBufferType( draw_dc.data_type );

#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_BltLowerLayerWhenTransparent((GUI_LCD_DEV_INFO *)dev_info_ptr, disp_rect_ptr);
    disp_rect_ptr = LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, disp_rect_ptr, &disp_rect);
    clip_rect_ptr = LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO*)dev_info_ptr, clip_rect_ptr, &clip_rect);
#endif

    box = GUISTR_DrawTextInRect(&des_buf_data, disp_rect_ptr, clip_rect_ptr, str_ptr, style_ptr, state, dir);

    MMITHEME_StoreUpdateRect(dev_info_ptr, box);
}

/*****************************************************************************/
// 	Description : 往buffer上画文本信息，所画的区域为
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 起始点的位置相对于buffer的原点
/*****************************************************************************/
PUBLIC GUI_RECT_T GUISTR_DrawTextInRect( 
                                        GUISTR_BUF_T          *des_buf_ptr,
                                        const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                        const GUI_RECT_T      *clip_rect_ptr,       //用户要剪切的实际区域
                                        const MMI_STRING_T    *str_ptr,
                                        GUISTR_STYLE_T        *style_ptr,
                                        GUISTR_STATE_T        state,
                                        int8                  dir
                                        )
{
    GUI_RECT_T  box = {0};
    GUI_RECT_T  buff_rect = {0};
    GUI_RECT_T  orig_clip_rect = {0};

    // 入参检测
    if (PNULL == des_buf_ptr || PNULL == disp_rect_ptr 
        || PNULL == str_ptr || PNULL == str_ptr->wstr_ptr || PNULL == style_ptr 
        || PNULL == clip_rect_ptr)
    {
        return orig_clip_rect;
    }

    // 检查区域是否有效，如果传入的区域无效，则会直接返回
    if (GUI_IsInvalidRect(*disp_rect_ptr) || GUI_IsInvalidRect(*clip_rect_ptr))
    {
        return orig_clip_rect;
    }

    buff_rect.left = buff_rect.top = 0;
    buff_rect.right = (short) (des_buf_ptr->width - 1);
    buff_rect.bottom = (short) (des_buf_ptr->height - 1);
    
    orig_clip_rect = GetTrimRect(*disp_rect_ptr, *clip_rect_ptr);
    orig_clip_rect = GetTrimRect(orig_clip_rect, buff_rect);

    if ((state&GUISTR_STATE_ANGLE) && ANGLE_0 != style_ptr->angle)
    {
        // 对于0度的，不旋转，则不会走该分支
        void *tmp_buffer = PNULL;
        GUI_RECT_T  disp_rect = {0};
        GUI_RECT_T  new_buff_rect = {0};
        GUISTR_BUF_T new_des_buf = {0};
        uint16 temp_width = 0;
        uint16 temp_height = 0;
        uint32 temp_size = 0;

        //只支持565和888的buffer旋转
        if ( GUISTR_BUF_RGB565 != des_buf_ptr->type
            && GUISTR_BUF_ARGB888 != des_buf_ptr->type )
        {
            return orig_clip_rect;
        }

        if ( GUISTR_BUF_RGB565 == des_buf_ptr->type )
        {
            temp_size = (orig_clip_rect.right - orig_clip_rect.left + 1)*(orig_clip_rect.bottom - orig_clip_rect.top + 1)*(int16)sizeof(GUI_COLOR_T);
            
        }
        else
        {
            temp_size = (orig_clip_rect.right - orig_clip_rect.left + 1)*(orig_clip_rect.bottom - orig_clip_rect.top + 1)*(int16)sizeof(uint32);
        }

        // 申请一段新的buffer
        tmp_buffer = SCI_ALLOC_APP( temp_size );
        SCI_MEMSET( tmp_buffer, 0, temp_size );

        new_des_buf.buffer = tmp_buffer;
        new_des_buf.type = des_buf_ptr->type;

        // 这里计算旋转之后的显示区域
        switch(style_ptr->angle)
        {
        case ANGLE_0: // 本case代码测试所用，因为当角度为0的时候，不会走这个分支
            
            new_des_buf.mem_width = new_des_buf.width = (uint16) (orig_clip_rect.right - orig_clip_rect.left + 1);
            new_des_buf.height = (uint16) (orig_clip_rect.bottom - orig_clip_rect.top + 1);
            // buffer rect
            new_buff_rect.left = new_buff_rect.right = 0;
            new_buff_rect.right = (uint16) (orig_clip_rect.right - orig_clip_rect.left);
            new_buff_rect.bottom = (uint16) (orig_clip_rect.bottom - orig_clip_rect.top);
            // display rect
            CalculateRotateRect(disp_rect_ptr, orig_clip_rect.left, orig_clip_rect.top, ANGLE_0, &disp_rect);

            if ( GUISTR_BUF_RGB565 == des_buf_ptr->type )
            {
                // 这里应该先把背景拷贝到buffer上
                RotateBufferData( des_buf_ptr->type, des_buf_ptr->buffer, orig_clip_rect.left, orig_clip_rect.top, des_buf_ptr->width, des_buf_ptr->height,
                    new_des_buf.buffer, 0, 0, new_des_buf.width, new_des_buf.height, ANGLE_0, new_des_buf.width, new_des_buf.height);
            }
            break;
            
        case ANGLE_90:
            
            new_des_buf.mem_width = new_des_buf.width = (uint16) (orig_clip_rect.bottom - orig_clip_rect.top + 1);
            new_des_buf.height = (uint16) (orig_clip_rect.right - orig_clip_rect.left + 1);
            // buffer rect
            new_buff_rect.left = new_buff_rect.right = 0;
            new_buff_rect.right = (uint16) (orig_clip_rect.bottom - orig_clip_rect.top);
            new_buff_rect.bottom = (uint16) (orig_clip_rect.right - orig_clip_rect.left);
            // display rect
            CalculateRotateRect(disp_rect_ptr, orig_clip_rect.right, orig_clip_rect.top, ANGLE_90, &disp_rect);

            if ( GUISTR_BUF_RGB565 == des_buf_ptr->type )
            {
                // 这里应该先把背景拷贝到buffer上, 旋转角度为270，因为拷贝回来的时候的旋转角度为90度
                RotateBufferData( des_buf_ptr->type, des_buf_ptr->buffer, orig_clip_rect.left, orig_clip_rect.top, des_buf_ptr->width, des_buf_ptr->height,
                    new_des_buf.buffer, 0, 0, new_des_buf.width, new_des_buf.height, ANGLE_270, new_des_buf.height, new_des_buf.width);
            }
            break;
            
        case ANGLE_180:
            
            new_des_buf.mem_width = new_des_buf.width = (uint16) (orig_clip_rect.right - orig_clip_rect.left + 1);
            new_des_buf.height = (uint16) (orig_clip_rect.bottom - orig_clip_rect.top + 1);
            // buffer rect
            new_buff_rect.left = new_buff_rect.right = 0;
            new_buff_rect.right = (uint16) (orig_clip_rect.right - orig_clip_rect.left);
            new_buff_rect.bottom = (uint16) (orig_clip_rect.bottom - orig_clip_rect.top);
            CalculateRotateRect(disp_rect_ptr, orig_clip_rect.right, orig_clip_rect.bottom, ANGLE_180, &disp_rect);

            if ( GUISTR_BUF_RGB565 == des_buf_ptr->type )
            {
                // 这里应该先把背景拷贝到buffer上, 旋转角度为180，因为拷贝回来的时候的旋转角度为180度
                RotateBufferData( des_buf_ptr->type, des_buf_ptr->buffer, orig_clip_rect.left, orig_clip_rect.top, des_buf_ptr->width, des_buf_ptr->height,
                    new_des_buf.buffer, 0, 0, new_des_buf.width, new_des_buf.height, ANGLE_180, new_des_buf.width, new_des_buf.height);
            }
            break;
            
        case ANGLE_270:
            
            new_des_buf.mem_width = new_des_buf.width = (uint16) (orig_clip_rect.bottom - orig_clip_rect.top + 1);
            new_des_buf.height = (uint16) (orig_clip_rect.right - orig_clip_rect.left + 1);
            // buffer rect
            new_buff_rect.left = new_buff_rect.right = 0;
            new_buff_rect.right = (uint16) (orig_clip_rect.bottom - orig_clip_rect.top);
            new_buff_rect.bottom = (uint16) (orig_clip_rect.right - orig_clip_rect.left);
            CalculateRotateRect(disp_rect_ptr, orig_clip_rect.left, orig_clip_rect.bottom, ANGLE_270, &disp_rect);

            if ( GUISTR_BUF_RGB565 == des_buf_ptr->type )
            {
                // 这里应该先把背景拷贝到buffer上, 旋转角度为90，因为拷贝回来的时候的旋转角度为270度
                RotateBufferData( des_buf_ptr->type, des_buf_ptr->buffer, orig_clip_rect.left, orig_clip_rect.top, des_buf_ptr->width, des_buf_ptr->height,
                    new_des_buf.buffer, 0, 0, new_des_buf.width, new_des_buf.height, ANGLE_90, new_des_buf.height, new_des_buf.width);
            }
            break;
            
        default:
            SCI_ASSERT(0);/*assert verified*/
            break;
        }

        // 将数据画到临时的buffer上
        box = Scripteasy_DrawText(&new_des_buf, &disp_rect, &new_buff_rect, 0, str_ptr, style_ptr, state, dir);

        // z这里需要从临时buffer上copy数据到目的buffer
        RotateBufferData( des_buf_ptr->type, new_des_buf.buffer, 0, 0, new_des_buf.width, new_des_buf.height, 
            des_buf_ptr->buffer, orig_clip_rect.left, orig_clip_rect.top, des_buf_ptr->width, des_buf_ptr->height, 
            style_ptr->angle, new_des_buf.width, new_des_buf.height);

        temp_width  = (uint16) (box.right - box.left);
        temp_height = (uint16) (box.bottom - box.top);

        switch(style_ptr->angle)
        {
        case ANGLE_0: // 本case代码测试所用，因为当角度为0的时候，不会走这个分支
            box.left   = disp_rect_ptr->left;
            box.top    = disp_rect_ptr->top;
            box.right  = (short) (box.left + temp_width);
            box.bottom = (short) (box.top + temp_height);
            break;
            
        case ANGLE_90:
            box.left   = (short) (disp_rect_ptr->right - temp_height);
            box.top    = disp_rect_ptr->top;
            box.right  = (short) (box.left + temp_height);
            box.bottom = (short) (box.top + temp_width);
            break;
            
        case ANGLE_180:
            box.left   = (short) (disp_rect_ptr->right - temp_width);
            box.top    = (short) (disp_rect_ptr->bottom - temp_height);
            box.right  = (short) (box.left + temp_width);
            box.bottom = (short) (box.top + temp_height);
            break;
            
        case ANGLE_270:
            box.left   = disp_rect_ptr->left;
            box.top    = (short) (disp_rect_ptr->bottom - temp_width);
            box.right  = (short) (box.left + temp_height);
            box.bottom = (short) (box.top + temp_width);
            break;
            
        default:
            SCI_ASSERT(0);/*assert verified*/
            break;
        }

        // 释放内存
        SCI_FREE(tmp_buffer);
    }
    else
    {
        box = Scripteasy_DrawText(des_buf_ptr, disp_rect_ptr, clip_rect_ptr, 0, str_ptr, style_ptr, state, dir);
    }

    orig_clip_rect = GetTrimRect(orig_clip_rect, box);

    if ( GUI_IsInvalidRect( orig_clip_rect ) )
    {
        SCI_MEMSET( &orig_clip_rect, 0, sizeof(GUI_RECT_T) );
    }

    return orig_clip_rect;

}

/*****************************************************************************/
// 	Description : 往LCD上画字符串
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC void GUISTR_DrawTextToLCDByOffset( 
                                         const GUI_LCD_DEV_INFO *dev_info_ptr,
                                         const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                         const GUI_RECT_T      *clip_rect_ptr,       //the fixed display area
                                         int16                 x_offset, // x方向向左移动还是向右移动由系统决定
                                         int16                 y_offset, // y方向向上移动为负值,向下移动为正值
                                         const MMI_STRING_T    *str_ptr,
                                         GUISTR_STYLE_T        *style_ptr,
                                         GUISTR_STATE_T        state
                                         )
{
    GUI_RECT_T      box = {0};
#ifdef UI_MULTILAYER_SUPPORT
    GUI_RECT_T      disp_rect = {0};
    GUI_RECT_T      clip_rect = {0};
#endif
    GUISTR_BUF_T    des_buf_data = {0};
    GUILCD_DRAW_DC_T draw_dc = {0};

    SCI_ASSERT(PNULL != dev_info_ptr);/*assert verified*/
    // 入参检测
    if (PNULL == disp_rect_ptr || PNULL == str_ptr || PNULL == style_ptr)
    {
        return;
    }

    if (!LCD_PrepareDrawParam(dev_info_ptr, &draw_dc))
    {
        return;
    }
    des_buf_data.buffer = draw_dc.lcd_buffer_ptr;
    des_buf_data.mem_width = (uint16) draw_dc.mem_width;
    des_buf_data.width = (uint16) draw_dc.width;
    des_buf_data.height = (uint16) draw_dc.height;
    des_buf_data.type = ConvertStringBufferType( draw_dc.data_type );

#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_BltLowerLayerWhenTransparent((GUI_LCD_DEV_INFO *)dev_info_ptr, disp_rect_ptr);
    disp_rect_ptr = LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, disp_rect_ptr, &disp_rect);
    if (PNULL != clip_rect_ptr)
    {
        clip_rect_ptr = LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, (GUI_LCD_DEV_INFO *)dev_info_ptr, clip_rect_ptr, &clip_rect);
    }
#endif

    // 将字符刷新到buffer内
    box = GUISTR_DrawTextByOffset(&des_buf_data, disp_rect_ptr, clip_rect_ptr, x_offset, y_offset, str_ptr, style_ptr, state);

    // 刷新屏幕
    MMITHEME_StoreUpdateRect(dev_info_ptr, box);
}

/*****************************************************************************/
// 	Description : 往buffer上画文本信息，所画的区域为
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 起始点的位置相对于buffer的原点
/*****************************************************************************/
PUBLIC GUI_RECT_T GUISTR_DrawTextByOffset( 
                                          GUISTR_BUF_T          *des_buf_ptr,
                                          const GUI_RECT_T      *disp_rect_ptr,       //the fixed display area
                                          const GUI_RECT_T      *clip_rect_ptr,       //the fixed display area
                                          int16                 x_offset, // x方向向左移动还是向右移动由系统决定
                                          int16                 y_offset, // y方向向上移动为负值,向下移动为正值
                                          const MMI_STRING_T    *str_ptr,
                                          GUISTR_STYLE_T        *style_ptr,
                                          GUISTR_STATE_T        state
                                          )
{
    GUI_RECT_T  box = {0};
    MMI_DIRECTION_SET_E direction = MMITHEME_GetDisDirection();
    GUI_RECT_T clip_rect = {0};
    GUI_RECT_T buf_rect = {0};
    GUI_RECT_T disp_rect = {0};

    // 入参检测
    if (PNULL == des_buf_ptr || PNULL == disp_rect_ptr || PNULL == str_ptr || PNULL == str_ptr->wstr_ptr || PNULL == style_ptr)
    {
        return clip_rect;
    }

    buf_rect.left = 0;
    buf_rect.top = 0;
    buf_rect.right = (short) (des_buf_ptr->width - 1);
    buf_rect.bottom = (short) (des_buf_ptr->height - 1);

    disp_rect = *disp_rect_ptr;
    clip_rect = GetTrimRect(disp_rect, buf_rect);

    if (PNULL != clip_rect_ptr)
    {
        clip_rect = GetTrimRect(clip_rect, *clip_rect_ptr);
    }
    

    if (PNULL != str_ptr->wstr_ptr && 0 != str_ptr->wstr_len) 
    {   
        //向右滚动, 从尾显示
        if ( MMI_DIRECTION_RIGHT == direction )
        {
            disp_rect.right = (short) (disp_rect.right + x_offset);

            state |= GUISTR_STATE_FROM_TAIL;
        }
        else
        {
            disp_rect.left = (short) (disp_rect.left - x_offset);
        }
        disp_rect.top = (short) (disp_rect.top + y_offset);

        box = Scripteasy_DrawText(des_buf_ptr, &disp_rect, &clip_rect, NULL, str_ptr, style_ptr, state, TEXT_DIR_AUTO);
    }

    return box;
/*
    box = Scripteasy_DrawText(des_buf_ptr, disp_rect_ptr, NULL, x_offset, str_ptr, style_ptr, state, -1);

    orig_clip_rect = GetTrimRect(orig_clip_rect, box);

    if ( GUI_IsInvalidRect( orig_clip_rect ) )
    {
        SCI_MEMSET( &orig_clip_rect, 0, sizeof(GUI_RECT_T) );
    }

    return orig_clip_rect;
*/
}

/*****************************************************************************/
// 	Description : 根据偏移量，计算剩余字符串的特殊效果字段数
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUISTR_CreateRegionStyleByPos(
                                             const MMI_STRING_T *str_ptr, 
                                             GUIFONT_REGION_STYLE_T *src_region_style_ptr, 
                                             uint16 src_region_num,
                                             uint16 pos,
                                             GUIFONT_REGION_STYLE_T **des_region_style_pptr, 
                                             uint16 *des_region_num_ptr
                                             )
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : 释放特效字段内存
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUISTR_DestroyRegionStyle(
                                         GUIFONT_REGION_STYLE_T *region_style_ptr
                                         )
{    
    return FALSE;
}

/*******************************************************************************************/
// 	Description : 获得字符串的宽度
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 注意该接口与guifont中的GUI_GetStringWidth及GUI_GetStringWidthEx的区别为该接口的参数
/*********************************************************************************************/
PUBLIC uint16 GUISTR_GetStringWidth(
                                    const GUISTR_STYLE_T *style_ptr,    
                                    const MMI_STRING_T   *str_ptr,
                                    GUISTR_STATE_T       state
                                    )
{
    GUISTR_INFO_T str_info = {0};

    GUISTR_GetStringInfo( style_ptr, str_ptr, state, &str_info );

    return str_info.width;
}

/*******************************************************************************************/
// 	Description : 获得字符串的高度
//	Global resource dependence :
//  Author: yongwei.he
//	Note:   字符串的高度为所有字符的最大高度
/*********************************************************************************************/

PUBLIC uint16 GUI_GetStringHeight(
                                  GUI_FONT_T   font_type,     
                                  const wchar* str_ptr,
                                  uint16       str_len
                                  )
{
    return Scripteasy_MapFontSize(font_type);
}

/*******************************************************************************************/
// 	Description : 获得字符串的宽高
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 注意该接口与guifont中的GUI_GetStringWidth及GUI_GetStringWidthEx的区别为该接口的参数
/*********************************************************************************************/
PUBLIC BOOLEAN GUISTR_GetStringInfo(
                                    const GUISTR_STYLE_T *style_ptr,    
                                    const MMI_STRING_T   *str_ptr,
                                    GUISTR_STATE_T       state,
                                    GUISTR_INFO_T        *info_ptr //out
                                    )
{
    int width = 0;
    SeDeviceContext* dc;

    // 参数检查
    if ( PNULL == style_ptr 
        || PNULL == str_ptr 
        || PNULL == str_ptr->wstr_ptr 
        || 0 == str_ptr->wstr_len
        || PNULL == info_ptr ) 
    {
        return FALSE;
    }

    dc = GUI_Get_Default_Device_Context(NULL);

    dc->font.size = Scripteasy_MapFontSize(style_ptr->font);
    
   
    // 设置文本其他状态
    //粗体
    if ( state & GUISTR_STATE_BOLD )
    {
        dc->font.attr |= SE_LOGFONT_BOLD;
    }
    //斜体
    if ( state & GUISTR_STATE_ITALIC )
    {
        dc->font.attr |= SE_LOGFONT_ITALIC;
    }
    //下划线
    if ( state & GUISTR_STATE_UNDERLINE )
    {
        dc->font.attr  |= SE_LOGFONT_UNDERLINE;
    }

/*    dc->font.attr |= SE_LOGFONT_PREFERRED;
    dc->font.id = (unsigned char) GUI_GetLogicFont();*/
    
    GUI_Get_Text_Info(dc, str_ptr->wstr_ptr, str_ptr->wstr_len, &width, NULL, NULL);

    info_ptr->height = dc->font.size;
    info_ptr->width  = (uint16) width;
    
    return TRUE;
}

/*******************************************************************************************/
// 	Description : 获得字符串的宽度
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 字形间距可理解为之前的字间距, 但不允许用户自己计算, 必须通过该函数
/*********************************************************************************************/
PUBLIC uint16 GUI_GetStringWidthEx(
                                   GUI_FONT_T	font_type,     
                                   const wchar* str_ptr,
                                   uint16 str_len,
                                   uint16 char_space
                                   )
{
    int width = 0;
    SeDeviceContext* dc;
    dc = GUI_Get_Default_Device_Context(NULL);
    dc->font.size = Scripteasy_MapFontSize(font_type);
/*    dc->font.attr |= SE_LOGFONT_PREFERRED;
    dc->font.id = (unsigned char) GUI_GetLogicFont();
    */
    GUI_Get_Text_Info(dc, str_ptr, str_len, &width, NULL, NULL);

    return (uint16) width;
}

/*******************************************************************************************/
// 	Description : GUI_GetStringInfoInLine的扩展, 支持所有特效
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 
/*******************************************************************************************/
PUBLIC BOOLEAN GUISTR_LineBreak(
                                int8                 dir,
                                const wchar	         *str_ptr,
                                uint16      	     str_len,
                                const GUISTR_STYLE_T *style_ptr,
                                GUISTR_STATE_T       state,
                                uint16      	     width,
                                uint16			     *char_num,
                                uint16			     *pixel_num
                                )
{
    SeDeviceContext* dc;
    
    dc = GUI_Get_Default_Device_Context(NULL);

    SCI_ASSERT( PNULL != char_num && PNULL != pixel_num );/*assert verified*/
    
    if( PNULL == str_ptr || 0 == *str_ptr || 0 == str_len || 0 == width 
        || PNULL == style_ptr )
    {
        return FALSE;
    }

    GUI_AnalysisNextLine(str_ptr, str_len, state, style_ptr->font, width, char_num, pixel_num);

    return TRUE;
}

PUBLIC int16 GUI_GetCusorCoordinate(//绝对坐标
                                    GUI_ALIGN_E       align,
                                    const GUI_RECT_T* rect_ptr,
                                    int8              dir,
                                    const wchar*      str_ptr,
                                    uint16            str_len,
                                    uint16            cursor_pos,
                                    GUI_FONT_T	      font_type,
                                    uint16            char_space
                                    )
{
    int16 x = 0;
    int16 total = 0;
	if (PNULL == rect_ptr)
	{
        SCI_TRACE_LOW("== GUI_GetCusorCoordinate == param rect_ptr is PNULL");
        return x;
	}

    if (PNULL == str_ptr || 0 == str_len )
    {	
		if (ALIGN_RIGHT == align || ALIGN_RVMIDDLE == align)
		{
			return (rect_ptr->right);
		}
		else
		{
			return (rect_ptr->left);
		}
    }

#if 0
    x = GUI_GetStringWidthEx(font_type, str_ptr, cursor_pos, char_space);
    if (cursor_pos == str_len)
        total = x;
    else
        total = GUI_GetStringWidthEx(font_type, str_ptr, str_len, char_space);
#else
    {
        SeDeviceContext dc;
        sestatus status;
        int width;
        int xx;
        
        dc = s_default_device_context;
		dc.font.attr = 0;
        dc.font.size = Scripteasy_MapFontSize(font_type);
/*        dc.font.attr |= SE_LOGFONT_PREFERRED;
        dc.font.id = (unsigned char) GUI_GetLogicFont();*/
        status = se_get_caret_pos(&dc, str_ptr, str_len, cursor_pos, dir == TEXT_DIR_RTL ? 1 : 0, &xx, &width);
        x = (short) xx;
        total = (short) width;
    }
#endif

    if (dir == TEXT_DIR_RTL)
    {
        if (align == ALIGN_HMIDDLE || align == ALIGN_HVMIDDLE)   // middle alignment
        {
            return (int16) (rect_ptr->left + (rect_ptr->right - rect_ptr->left - total + 1) / 2 + x);
        }
        else if (align == ALIGN_LEFT || align == ALIGN_LVMIDDLE)   // left alignment
        {
            return (int16) (rect_ptr->right - total + x);
        }
        else        // right alignment
        {
            return (int16) (rect_ptr->left + x);
        }
    }   
    else {
        if (align == ALIGN_HMIDDLE || align == ALIGN_HVMIDDLE)   // middle alignment
        {
            return (int16) (rect_ptr->left + (rect_ptr->right - rect_ptr->left - total + 1) / 2 + x);
        }
        else if (align == ALIGN_RIGHT || align == ALIGN_RVMIDDLE)   // right alignment
        {
            return (int16) (rect_ptr->right - total + x);
        }
        else        // left alignment
        {
            return (int16) (rect_ptr->left + x);
        }
    }
}

PUBLIC BOOLEAN Scripteasy_TestChar(wchar ch) 
{
    return TRUE;
}

/*****************************************************************************/
// 	Description : get text direction
//	Note:获得文字的方向, TEXT_DIR_LTR or TEXT_DIR_RTL, 如果系统中没有阿拉伯语
//       或希伯来语等RTL语言, 恒返回TEXT_DIR_LTR
/*****************************************************************************/
PUBLIC SPML_TEXT_DIR_T Scripteasy_GetTextDirection(const wchar* text_ptr, //unicode字符串
												int32 len              //字符个数
												)
{
    sestatus status;
    int dir;

    status = se_get_text_dir(text_ptr, len, &dir);
/*    if (status != SE_STATUS_OK || dir == SE_DIR_NEUTRAL)
        return TEXT_DIR_AUTO;*/

    if (dir == SE_DIR_RTL || dir == SE_DIR_WEAK_RTL)
        return TEXT_DIR_RTL;

    return TEXT_DIR_LTR;
}

LOCAL int32 SCRIPTEASY_GetCursorInfo(const wchar*             text_ptr,  //unicode字符串
                                   uint16                   len,       //字符个数
                                   BOOLEAN                  is_left,   //left or right
                                   int32*                   pos_ptr    //光标原位置, in\out
                                   )
{
    if (!text_ptr)
        return 0;

    if (!pos_ptr)
        return 0;

    if (*pos_ptr > len)
    {
        *pos_ptr = len;
    }

    {
        int prev, next;
        prev = 0;
        next = 0;
        while (next < *pos_ptr) {
            int offset;
            sestatus status = se_find_writing_unit(text_ptr + next, len - next, &offset);
            if (status != SE_STATUS_OK)
                return 0;

            prev = next;
            next += offset;
        }

        if (next == *pos_ptr) 
            return 0;

        if (is_left)
        {
            *pos_ptr = prev;
        } else {
            *pos_ptr = next;
        }

        return 0;
    }
}

/*******************************************************************************************/
// 	Description : 获得光标的x坐标并微调光标位置
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 
/*********************************************************************************************/
PUBLIC int16 GUI_GetCursorInfo(
                               GUI_ALIGN_E       align,
                               const GUI_RECT_T	*rect_ptr,
                               int8				 dir,
                               const wchar		*str_ptr,
                               uint16            str_len,
                               BOOLEAN           is_left,
                               GUI_FONT_T	     font_type,
                               uint16            char_space,
                               int32*            cursor_ptr
                               )
{													/*lint !e14*/
    int16 x = 0;
    
	if (PNULL == rect_ptr)
	{
        return x;
	}

    if (PNULL == str_ptr || 0 == str_len )
    {
        return (rect_ptr->left);
    }

    x = (int16)SCRIPTEASY_GetCursorInfo(str_ptr, str_len, is_left, cursor_ptr);

    return x;
}

/*******************************************************************************************/
// 	Description : 获得光标索引
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 
/*********************************************************************************************/
PUBLIC uint16 GUI_GetCursorPos(//相对位置
                               GUI_ALIGN_E      align,
							   const GUI_RECT_T	*rect_ptr,      //in:
							   int8				dir,            //in:
							   const wchar		*str_ptr,       //in:
							   uint16           str_len,        //in:
							   uint16           cursor_pos,	    //in:相对位置
							   int16            cursor_x,	    //in:绝对坐标
							   GUI_FONT_T	    font_type,      //in:
							   uint16           char_space,     //in:
                               BOOLEAN          *is_find_ptr    //in/out:may PNULL
							   )
{
    int pos = cursor_pos;
	int cursor;
    int total;

    if ( PNULL == rect_ptr || PNULL == str_ptr || 0 == str_len )
    {
        if (PNULL != is_find_ptr)
        {
            *is_find_ptr = FALSE;
        }

        return 0;
    }

    total = GUI_GetStringWidthEx(font_type, str_ptr, str_len, char_space);
    if (align == ALIGN_HMIDDLE || align == ALIGN_HVMIDDLE)   // middle alignment
    {
        int left = (rect_ptr->right - rect_ptr->left - total) / 2 + rect_ptr->left;
        if (cursor_x < left)
        {
            if (PNULL != is_find_ptr)
                *is_find_ptr = TRUE;
            return 0;
        }
        else
            cursor = cursor_x - left;
    }
    else if (align == ALIGN_RIGHT || align == ALIGN_RVMIDDLE)   // right alignment
    {
        if (dir == TEXT_DIR_LTR)
        {
            int left = rect_ptr->right - total;
            if (cursor_x < left)
            {
                if (PNULL != is_find_ptr)
                    *is_find_ptr = TRUE;
                return 0;
            }
            else
                cursor = cursor_x - left;
        }
        else 
        {
            cursor = cursor_x - rect_ptr->left;
        }
    }
    else        // left alignment
    {
        if (dir == TEXT_DIR_RTL)
        {
            int left = rect_ptr->right - total;
            if (cursor_x < left)
            {
                if (PNULL != is_find_ptr)
                    *is_find_ptr = TRUE;
                return 0;
            }
            else
                cursor = cursor_x - left;
        }
        else 
        {
            cursor = cursor_x - rect_ptr->left;
        }
    }

    {
        SeDeviceContext dc;
        sestatus status;

        dc = s_default_device_context;
        dc.font.size = Scripteasy_MapFontSize(font_type);
/*        dc.font.attr |= SE_LOGFONT_PREFERRED;
        dc.font.id = (unsigned char) GUI_GetLogicFont();
*/
        status = se_get_caret_index_by_pos(&dc,
                                          str_ptr,
                                          str_len,
                                          cursor,
                                          dir == TEXT_DIR_RTL ? 1 : 0,
                                          &pos);
        if (SE_STATUS_OK != status)
            pos = cursor_pos;
    }

    return (uint16) pos;
        
}

/*==============================================================================
Description: 获得光标索引
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 增强型，同时获得光标与字符的相对位置关系
==============================================================================*/
PUBLIC BOOLEAN GUI_GetCursorPosEx(
                                  GUI_ALIGN_E      align,
                                  const GUI_RECT_T *rect_ptr,
                                  int8         dir,
                                  const wchar  *str_ptr,
                                  uint16       str_len,
                                  int16        cursor_x,
                                  GUI_FONT_T   font_type,
                                  uint16       char_space,
                                  int32        *nearest_chr_idx_ptr,
                                  int32        *under_chr_idx_ptr,
                                  int32        *under_len_ptr
                                  )
{
    BOOLEAN is_find = FALSE;
	int pos;
	int cursor;
    int total;

    if (PNULL == nearest_chr_idx_ptr
        || PNULL == under_chr_idx_ptr
        || PNULL == under_len_ptr)
    {
        return FALSE;
    }

    if (PNULL == rect_ptr || PNULL == str_ptr || 0 == str_len)
    {
        *nearest_chr_idx_ptr = -1;
        return is_find;
    }

    total = GUI_GetStringWidthEx(font_type, str_ptr, str_len, char_space);
    if (align == ALIGN_HMIDDLE || align == ALIGN_HVMIDDLE)   // middle alignment
    {
        int left = (rect_ptr->right - rect_ptr->left - total) / 2 + rect_ptr->left;
        if (cursor_x < left)
        {
		    *nearest_chr_idx_ptr = 0;
		    *under_chr_idx_ptr = 0;
		    *under_len_ptr = 0;
            return TRUE;
        }
        else
            cursor = cursor_x - left;
    }
    else if (align == ALIGN_RIGHT || align == ALIGN_RVMIDDLE)   // right alignment
    {
        if (dir == TEXT_DIR_LTR)
        {
            int left = rect_ptr->right - total;
            if (cursor_x < left)
            {
		        *nearest_chr_idx_ptr = 0;
		        *under_chr_idx_ptr = 0;
		        *under_len_ptr = 0;
                return TRUE;
            }
            else
                cursor = cursor_x - left;
        }
        else 
        {
            cursor = cursor_x - rect_ptr->left;
        }
    }
    else        // left alignment
    {
        if (dir == TEXT_DIR_RTL)
        {
            int left = rect_ptr->right - total;
            if (cursor_x < left)
            {
                *nearest_chr_idx_ptr = 0;
		        *under_chr_idx_ptr = 0;
		        *under_len_ptr = 0;
                return TRUE;
            }
            else
                cursor = cursor_x - left;
        }
        else 
        {
            cursor = cursor_x - rect_ptr->left;
        }
    }

    {
        SeDeviceContext dc;
        sestatus status;

        dc = s_default_device_context;
        dc.font.size = Scripteasy_MapFontSize(font_type);
/*        dc.font.attr |= SE_LOGFONT_PREFERRED;
        dc.font.id = (unsigned char) GUI_GetLogicFont();
*/
        status = se_get_caret_index_by_pos(&dc,
                                          str_ptr,
                                          str_len,
                                          cursor,
                                          dir == TEXT_DIR_RTL ? 1 : 0,
                                          &pos);
        if (SE_STATUS_OK == status) {
            is_find = TRUE;
            *nearest_chr_idx_ptr = pos;
		    *under_chr_idx_ptr = pos;
		    *under_len_ptr = 0;
        }
    }

    return is_find;
}

/*******************************************************************************************/
// 	Description : 计算节点数，创建数组
//	Global resource dependence :
//  Author: xiaoqing.lu
//	Note: 
/*********************************************************************************************/
LOCAL void DestoryPtr(
                      void *node_ptr
                      )
{
    if (PNULL != node_ptr)
    {
        SCI_FREE(node_ptr);
    }

    return;
}


/*==============================================================================
Description: CombineNode
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:合并相同的连续的特效区域
==============================================================================*/
LOCAL int32 CombineNode(GUISTRING_REGION_NODE_T **head_pptr)
{
    GUISTRING_REGION_NODE_T *cur_node_ptr = PNULL;
    BOOLEAN is_same_state = FALSE;
    BOOLEAN is_series = FALSE;
    BOOLEAN is_same_color = FALSE;
    int16 cur_next = 0;
    int32 node_cnt = 0;

    SCI_ASSERT(PNULL != head_pptr); /*assert verified*/

    if (PNULL == *head_pptr) return 0;

    cur_node_ptr = *head_pptr;
    node_cnt = 1;

    while (PNULL != cur_node_ptr)
    {
        if (PNULL == cur_node_ptr->next) break;

        is_same_state = (BOOLEAN) (cur_node_ptr->region.state == cur_node_ptr->next->region.state);
        cur_next = (short) (cur_node_ptr->region.start + cur_node_ptr->region.length);
        //force check
        SCI_ASSERT(cur_next <= cur_node_ptr->next->region.start); /*assert verified*/
        is_series = (BOOLEAN) (cur_next == cur_node_ptr->next->region.start);
        is_same_color = (BOOLEAN) (cur_node_ptr->region.font_color== cur_node_ptr->next->region.font_color);

        if (is_same_state && is_series && is_same_color)
        {
            cur_node_ptr->region.length = (uint16) (cur_node_ptr->region.length + cur_node_ptr->next->region.length);
            
            RemoveNodePtr(cur_node_ptr->next);
            SCI_FREE(cur_node_ptr->next);

        }
        else
        {
            cur_node_ptr = cur_node_ptr->next;
            node_cnt++;
        }
    }

    return node_cnt;
}

/*==============================================================================
Description: InsertNode
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:向特效链表中插入一个新的特效结点
==============================================================================*/
LOCAL int32 InsertNode(
    GUISTRING_REGION_NODE_T **head_pptr,
    GUIFONT_REGION_STYLE_T const *region_ptr
)
{
    GUISTRING_REGION_NODE_T *cur_node_ptr = PNULL;
    GUISTRING_REGION_NODE_T *prev_ptr = PNULL;
    GUIFONT_REGION_STYLE_T region = {0};
    
    SCI_ASSERT(PNULL != head_pptr); /*assert verified*/

    cur_node_ptr = *head_pptr;
    region = *region_ptr;

    for(;;)
    {
        GUISTRING_REGION_NODE_T *new_node_ptr = PNULL;
        GUISTRING_REGION_NODE_T *cover_node_ptr = PNULL;
        GUISTRING_REGION_NODE_T *remain_node_ptr = PNULL;
        uint16 cur_end = 0;
        uint16 new_end = 0;

        if (PNULL == cur_node_ptr)
        {
            new_node_ptr = SCI_ALLOC_APP(sizeof(GUISTRING_REGION_NODE_T));

            new_node_ptr->region = region;
            new_node_ptr->prev = prev_ptr;
            new_node_ptr->next = PNULL;

            if (PNULL == prev_ptr)
            {
                *head_pptr = new_node_ptr;
            }
            else
            {
                prev_ptr->next = new_node_ptr;
            }

            break;
        }

        cur_end = (uint16) (cur_node_ptr->region.start + cur_node_ptr->region.length - 1);
        new_end = (uint16) (region.start + region.length - 1);
        
        if (region.start < cur_node_ptr->region.start)
        {
            //NNN????????????
            //      CCCCCC
            new_node_ptr = SCI_ALLOC_APP(sizeof(GUISTRING_REGION_NODE_T));
            new_node_ptr->region.start = region.start;
            new_node_ptr->region.state = region.state;
            new_node_ptr->region.font_color = region.font_color;

            InsertNodePtr(new_node_ptr, cur_node_ptr);
            
            if (PNULL == prev_ptr)
            {
                *head_pptr = new_node_ptr;
            }
            else
            {
                prev_ptr->next = new_node_ptr;
            }

            if (new_end < cur_node_ptr->region.start)
            {
                //NNN---
                //      CCCCCC
                new_node_ptr->region.length = region.length;
            }
            else
            {
                //NNNNNNN????????
                //      CCCCCC
                new_node_ptr->region.length = (uint16) (cur_node_ptr->region.start - region.start);

                if (new_end < cur_end)
                {
                    //NNNNNNN----
                    //      CCCCCC
                    cover_node_ptr = SCI_ALLOC_APP(sizeof(GUISTRING_REGION_NODE_T));
                    cover_node_ptr->region.start = cur_node_ptr->region.start;
                    cover_node_ptr->region.length = (uint16) (new_end - cur_node_ptr->region.start + 1);
                    cover_node_ptr->region.state = cur_node_ptr->region.state | region.state;
                    cover_node_ptr->region.font_color = region.font_color;

                    cur_node_ptr->region.start = (uint16) (new_end + 1);
                    cur_node_ptr->region.length = (uint16) (cur_end - new_end);

                    InsertNodePtr(cover_node_ptr, cur_node_ptr);
                }
                else
                {
                    //NNNNNNNNNNNN???
                    //      CCCCCC
                    cur_node_ptr->region.state |= region.state;

                    if (cur_end < new_end)
                    {
                        //NNNNNNNNNNNNN--
                        //      CCCCCC
                        region.start = (uint16) (cur_end + 1);
                        region.length = (uint16) (new_end - cur_end);

                        prev_ptr = cur_node_ptr;
                        cur_node_ptr = cur_node_ptr->next;
                        continue;
                    }
                }
            }
        }
        else if (region.start <= cur_end)
        {
            //?NN??????
            //CCCCCC
            if (region.start > cur_node_ptr->region.start)
            {
                // NN??????
                //CCCCCC
                new_node_ptr = SCI_ALLOC_APP(sizeof(GUISTRING_REGION_NODE_T));
                new_node_ptr->region.start = cur_node_ptr->region.start;
                new_node_ptr->region.state = cur_node_ptr->region.state;
                new_node_ptr->region.length = (uint16) (region.start - cur_node_ptr->region.start);
                new_node_ptr->region.font_color = cur_node_ptr->region.font_color;

                InsertNodePtr(new_node_ptr, cur_node_ptr);
                
                if (PNULL == prev_ptr)
                {
                    *head_pptr = new_node_ptr;
                }
                else
                {
                    prev_ptr->next = new_node_ptr;
                }

                cur_node_ptr->region.start = region.start;
            }

            if (new_end < cur_end)
            {
                //?NN??
                //CCCCCC
                remain_node_ptr = SCI_ALLOC_APP(sizeof(GUISTRING_REGION_NODE_T));
                remain_node_ptr->region.start = (uint16) (new_end + 1);
                remain_node_ptr->region.length = (uint16) (cur_end - new_end);
                remain_node_ptr->region.state = cur_node_ptr->region.state;
                remain_node_ptr->region.font_color = region.font_color;

                cur_node_ptr->region.state |= region.state;
                cur_node_ptr->region.length = region.length;

                AppendNodePtr(cur_node_ptr, remain_node_ptr);
            }
            else
            {
                //?NNNNN???
                //CCCCCC
                cur_node_ptr->region.state |= region.state;
                cur_node_ptr->region.length = (uint16) (cur_end - cur_node_ptr->region.start + 1);

                if (cur_end < new_end)
                {
                    //?NNNNNNNN?
                    //CCCCCC
                    region.start = (uint16) (cur_end + 1);
                    region.length = (uint16) (new_end - cur_end);

                    prev_ptr = cur_node_ptr;
                    cur_node_ptr = cur_node_ptr->next;
                    continue;
                }
            }
        }
        else
        {
            //      NNN???
            //CCC???
            prev_ptr = cur_node_ptr;
            cur_node_ptr = cur_node_ptr->next;
            continue;
        }

        break;
    }


    return CombineNode(head_pptr);
}

/*==============================================================================
Description: CreateRegionPtr
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL int32 CreateRegionPtr(
                            GUIFONT_REGION_STYLE_T const *in_region_ptr, 
                            int32 region_num,
                            GUIFONT_REGION_STYLE_T **out_region_ptr
                            )
{
    GUISTRING_REGION_NODE_T *head_ptr = PNULL;
    GUISTRING_REGION_NODE_T *next_ptr = PNULL;
    int32 i = 0;
    int32 node_cnt = 0;
    uint32 node_arr_size = 0;

    for (i = 0; i < region_num; i++)
    {
        node_cnt = InsertNode(&(head_ptr), in_region_ptr + i);
    }

    if (0 == node_cnt) return 0;

    //force check
    SCI_ASSERT(PNULL != head_ptr); /*assert verified*/

    node_arr_size = sizeof(GUIFONT_REGION_STYLE_T)*(uint32)node_cnt;
    *out_region_ptr = SCI_ALLOC_APP(node_arr_size);

    for (i = 0; i < node_cnt; i++)
    {
        (*out_region_ptr)[i] = head_ptr->region;
        
        next_ptr = head_ptr->next;
        SCI_FREE(head_ptr);
        head_ptr = next_ptr;
    }

    //force check
    SCI_ASSERT(PNULL == head_ptr); /*assert verified*/

    return node_cnt;
}

/*****************************************************************************/
// 	Description : get text metrics
//	Note:获得字符串宽高
/*****************************************************************************/
PUBLIC int32 SPMLAPI_GetTextMetrics(const SPML_TEXT_PARAM_T* param_ptr,  //显示参数
                                    const wchar*             text_ptr,   //unicode字符串
                                    uint16                   len,        //字符个数
                                    SPML_TEXT_METRICS_T*     metrics_ptr //out, 字符串的宽高
                                    )
{
    int width = 0;
    SeDeviceContext* dc;

    dc = GUI_Get_Default_Device_Context(NULL);

    dc->font.size = Scripteasy_MapFontSize(param_ptr->size);
   
    // 设置文本其他状态
    //粗体
    if ( param_ptr->format & TEXT_FORMAT_BOLD )
    {
        dc->font.attr |= SE_LOGFONT_BOLD;
    }
    //斜体
    if ( param_ptr->format & TEXT_FORMAT_ITALIC )
    {
        dc->font.attr |= SE_LOGFONT_ITALIC;
    }
    //下划线
    if ( param_ptr->format & TEXT_FORMAT_UNDERLINE )
    {
        dc->font.attr  |= SE_LOGFONT_UNDERLINE;
    }

    if ( param_ptr->format & TEXT_FORMAT_EDGE )
    {
        dc->text_flags  |= SE_FLAG_BORDER;
    }

/*    dc->font.attr |= SE_LOGFONT_PREFERRED;
    dc->font.id = (unsigned char) GUI_GetLogicFont();*/
    
    GUI_Get_Text_Info(dc, text_ptr, len, &width, NULL, NULL);

    metrics_ptr->height = dc->font.size;
    metrics_ptr->width  = (uint16) width;

	return 0;
}


/*****************************************************************************/
// 	Description : draw text
//	Note:绘制字符串
/*****************************************************************************/
PUBLIC int32 SPMLAPI_DrawText(const SPML_TEXT_PARAM_T* param_ptr,  //显示参数
                              const wchar*             text_ptr,   //unicode字符串
                              uint16                   len,        //字符个数
                              SPML_TEXT_METRICS_T*     metrics_ptr //out, 字符串的宽高, 可以为PNULL
                              )
{
    GUI_RECT_T   box;
    int x = 0, y = 0;
    int width = 0, height = 0;
    SeDeviceContext dc = {0};
	const SPML_RECT_T *disp_rect_ptr = &param_ptr->display_rect;
	const SPML_RECT_T *clip_rect_ptr = &param_ptr->clip_rect;
    LcdDeviceInfo dev_info = {0};
	int buffer_type = GUISTR_BUF_RGB565;

    int text_length = Scripteasy_calc_text_length(text_ptr);
    if (text_length > len)
        text_length = len;

	box.left = disp_rect_ptr->left;
    box.top  = disp_rect_ptr->top;
    box.right = (int16)(disp_rect_ptr->right + 1);
    box.bottom = (int16)(disp_rect_ptr->bottom + 1);

    if (PNULL == text_ptr)
        return 0;

	dc.device_width  = param_ptr->buffer_width;
	dc.device_height = param_ptr->buffer_height;
	
	dc.draw_pixel    = lcd_draw_pixel;
    if (param_ptr->buffer_type == SPML_BUF_ARGB888)
	{
        dc.draw_bitmap   = lcd_draw_bitmap_888;
		buffer_type = GUISTR_BUF_ARGB888;
	}
    else
        dc.draw_bitmap   = lcd_draw_bitmap;
    dc.draw_rect     = lcd_draw_rect;
    dc.revert_rect   = lcd_revert_rect;
    dc.update        = 0;//lcd_update;

    /* lvjun@komoxo.com to fix netfront text display bug begin */
    dev_info.buffer = param_ptr->buffer_ptr;
    dev_info.width = param_ptr->buffer_width;
    dev_info.height = param_ptr->buffer_height;
    dev_info.type = buffer_type;
    dc.device_data = &dev_info;
    /* lvjun@komoxo.com to fix netfront text display bug end */

    dc.clip.left   = disp_rect_ptr->left;
    dc.clip.top    = disp_rect_ptr->top;
    dc.clip.right  = disp_rect_ptr->right + 1;
    dc.clip.bottom = disp_rect_ptr->bottom + 1;

    if (clip_rect_ptr)/*lint !e774*/
    {
		SeRectangle rect;
        rect.left   = clip_rect_ptr->left;
        rect.top    = clip_rect_ptr->top;
        rect.right  = clip_rect_ptr->right + 1;
        rect.bottom = clip_rect_ptr->bottom + 1;

		se_intersect_rect(&dc.clip, &dc.clip, &rect);
    }
    
    dc.font.size = Scripteasy_MapFontSize(param_ptr->size);
/*    dc.font.attr |= SE_LOGFONT_PREFERRED;
    dc.font.id = (unsigned char) GUI_GetLogicFont();*/

    if (param_ptr->format & TEXT_FORMAT_ELLIPSIS)
    {
        dc.ellipsize = SE_ELLIPSIZE_END;
        dc.ellipsize_width = disp_rect_ptr->right - disp_rect_ptr->left + 1;
    }
    
    dc.text_color = GUI_565_color_to_888_color(param_ptr->font_color);

	if (param_ptr->format & TEXT_FORMAT_EDGE)
    {
        dc.text_flags |= SE_FLAG_BORDER;
        dc.bk_color = GUI_565_color_to_888_color(param_ptr->edge_color);
        se_inflate_rect(&dc.clip, 1, 1);
    }

    if (param_ptr->format & TEXT_FORMAT_UNDERLINE)
    {
        dc.font.attr |= SE_LOGFONT_UNDERLINE;
        dc.underline_color = GUI_565_color_to_888_color(param_ptr->line_color);
    }

    if (param_ptr->format & TEXT_FORMAT_ITALIC)
    {
        dc.font.attr |= SE_LOGFONT_ITALIC;
    }

    if (param_ptr->format & TEXT_FORMAT_BOLD)
    {
        dc.font.attr |= SE_LOGFONT_BOLD;
    }

    x = box.left;
    y = box.top;
    
    //get single line height
    GUI_Get_Text_Info_Ex(&dc, text_ptr, text_length, param_ptr->dir == TEXT_DIR_RTL ? 1 : 0, &width, &height, 0);

    if (param_ptr->format & TEXT_FORMAT_RIGHT)
	{
		x = box.right - width;
	}
	else if (param_ptr->format & TEXT_FORMAT_RIGHT)
	{
		x = (box.right + box.left - width) / 2;
	}

	if (box.bottom - box.top > height)
	{
		if (param_ptr->format & TEXT_FORMAT_BOTTOM)
		{
			y = box.bottom - height;
		}
		else if (param_ptr->format & TEXT_FORMAT_RIGHT)
		{
			y = (box.top + box.bottom - width) / 2;
		}
	}

    if (param_ptr->region_num == 0)
    {
        GUI_Draw_text_Ex(&dc, x, y, text_ptr, text_length, param_ptr->dir == TEXT_DIR_RTL ? 1 : 0);
    }
    else
    {
		//TODO:
        GUI_Draw_text_Ex(&dc, x, y, text_ptr, text_length, param_ptr->dir == TEXT_DIR_RTL ? 1 : 0);
    }

	if (metrics_ptr)
	{
		metrics_ptr->width = (unsigned short) width;
		metrics_ptr->height = (unsigned short) height;
	}

    return text_length;
}

/*****************************************************************************/
// 	Description : line break
//	Note:断行函数, 分为按像素断行和按词断行, 返回显示宽度中能显示的字符个数
/*****************************************************************************/
PUBLIC uint16 SPMLAPI_LineBreak(const SPML_TEXT_PARAM_T* param_ptr,     //显示参数
							    const wchar*             text_ptr,      //unicode字符串
							    uint16                   len,           //字符个数
							    uint16                   disp_width,    //显示宽度
							    uint16*                  out_width_ptr, //out, 能显示的字符的宽度
							    BOOLEAN                  by_word        //是否按词断行
								)
{
	GUISTR_STATE_T state = 0;
	GUI_FONT_T font_type = param_ptr->size;/*lint !e64*/
    uint16 char_num;
    uint16 pixel_num;
 
    if (param_ptr->format & TEXT_FORMAT_UNDERLINE)
    {
        state |= GUISTR_STATE_UNDERLINE;
    }

    if (param_ptr->format & TEXT_FORMAT_ITALIC)
    {
        state |= GUISTR_STATE_ITALIC;
    }

    if (param_ptr->format & TEXT_FORMAT_BOLD)
    {
        state |= GUISTR_STATE_BOLD;
    }

	GUI_AnalysisNextLine(text_ptr, len, state, font_type, disp_width, &char_num, &pixel_num);

	if (out_width_ptr)
	    *out_width_ptr = (uint16)pixel_num;
		
    return char_num;
}

#endif  /* SCRIPTEASY_SUPPORT */
