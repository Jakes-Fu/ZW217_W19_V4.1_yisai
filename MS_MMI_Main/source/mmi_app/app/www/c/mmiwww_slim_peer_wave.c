/******************************************************************************
** File Name:      mmiwww_slim_peer_wave.c                                    *
** Author:                                                                    *
** Date:           06/15/2009                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the slim wave peer for WWW   *
**                 Browser                                                    *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 06/2009        wancan.you        Create                                    *
******************************************************************************/
#define _MMIWWW_SLIM_PEER_WAVE_C_  

/**--------------------------------------------------------------------------*
 **                         Dependencies                                     *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "gui_ucs2b_converter.h"
#include "guifont.h"
#include "mmidisplay_data.h"
#include "mmiwww_slim_peer.h"
#include "mmiwww_func.h"
#include "mmi_theme.h"
#include "guistring.h"

#ifdef BROWSER_SUPPORT_NETFRONT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define SLIM_ROUNDUP_P2(x, y) ((slim_int)(((slim_int)(x)+(slim_int)(y)-1)&~(slim_int)((y)-1)))

#define GUI_888_color_to_565_color(color)   (GUI_COLOR_T) ( ((color >> 8) & 0xF800) | ((color >> 5) & 0x07E0) | ((color >> 3) & 0x001F) )
#define SLIM_PEEK_16_NE_AL(a) ((slim_half)SLIM_CONFIG_CC_PEEK_16_NE_AL((a)))
#define SLIM_POKE_16_NE_AL(a,v) SLIM_CONFIG_CC_POKE_16_NE_AL((a),(slim_half)(v))

#define WAVE_FONT_SPACE            0

#ifdef SLIM_WWWPIXMAP_SCANLINEALIGN
#define WAVE_PEER_SCANLINEALING    SLIM_WWWPIXMAP_SCANLINEALIGN
#else
#define WAVE_PEER_SCANLINEALING    4
#endif

#define WAVE_PAINT_DIRECTSCREEN    0

#define WAV_BROWSWER_TITLE_HEIGHT 28
#define MMIWWW_FONT_ADJUST_FONT_HEIGHT  (2)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
#define MMIWWW_FONT_BOLD  0x00000001
#define MMIWWW_FONT_ITALIC  0x00000002
#define MMIWWW_FONT_TYPE  0xFFFF0000

#define MMIWWW_XXXSMALL_FONT    10
#define MMIWWW_SMALL_FONT    14
#define MMIWWW_MEDIUM_FONT    16
#define MMIWWW_LARGE_FONT    20
#define MMIWWW_XXXLARGE_FONT    24

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : Adjust bitblt rect
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void wvpeer_adjust_bitblt_rect(TRect  *inout_rect);

/*****************************************************************************/
// Description : Transfers bitmap for rgb8
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void wvpeer_bitblt_rgb8(void     *in_dest_bitmap,TRect    *in_dest_r, 
                              void     *in_src_bitmap, void     *in_src_mask,
                              slim_int  in_src_x,      slim_int  in_src_y,
                              slim_int  in_src_width,  slim_int  in_dest_rowbytes);

/*****************************************************************************/
// Description : Transfers bitmap for rgb16
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void wvpeer_bitblt_rgb16(void     *in_dest_bitmap,TRect    *in_dest_r,
                               void     *in_src_bitmap, void     *in_src_mask,
                               slim_int  in_src_x,      slim_int  in_src_y,
                               slim_int  in_src_width,  slim_int  in_dest_rowbytes);

/*****************************************************************************/
// Description : Transfers bitmap for argb16
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void wvpeer_bitblt_argb16(void     *in_dest_bitmap,TRect    *in_dest_r,
                                void     *in_src_bitmap, void     *in_src_mask,
                                slim_int  in_src_x,      slim_int in_src_y,
                                slim_int  in_src_width,  slim_int in_dest_rowbytes);

/*****************************************************************************/
// Description : Transfers bitmap for rgb32
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void wvpeer_bitblt_rgb32(void     *in_dest_bitmap,TRect    *in_dest_r,
                               void     *in_src_bitmap, void     *in_src_mask,
                               slim_int  in_src_x,      slim_int  in_src_y,
                               slim_int  in_src_width,  slim_int  in_dest_rowbytes);

/*****************************************************************************/
// Description : Get font index
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL slim_int wvpeer_fint_font_index(slim_int   in_fontsz,
                                      slim_bool  in_bold,
                                      slim_bool  in_italic);

/*****************************************************************************/
// Description : Get font style by index
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void wvpeer_get_fontstyle(slim_int         font_type,
                                GUISTR_STYLE_T  *out_style,
                                GUISTR_STATE_T  *out_state);

/**--------------------------------------------------------------------------*
 **                         Constant Variables                               *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : Initializes event system
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool waveEventInitializePeer(void)
{
    PEER_DP(("waveEventInitializePeer"));
    return slim_true;
}

/*****************************************************************************/
// Description : Finalizes event system
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void waveEventFinalizePeer(void)
{
    PEER_DP(("waveEventFinalizePeer"));
    return;
}

/*****************************************************************************/
// Description : Gets event
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int waveGetNextEventPeer(TEvent  *out_event, slim_int  in_sleep_msec)
{
    return SLIM_EVENT_NIL;
}

/*****************************************************************************/
// Description : Gets mouse cursor position information
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void waveGetMousePosPeer(TScreenContext  in_context,
                                slim_int       *out_x,
                                slim_int       *out_y)
{
    PEER_DP(("waveGetMousePosPeer"));
    return;
}

/*****************************************************************************/
// Description : Gets status of modifier keys
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int waveGetModifiersPeer(TScreenContext in_context)
{
    PEER_DP(("waveGetModifiersPeer"));
    return 0;
}

/*****************************************************************************/
// Description : Initializes screen
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_bool waveGraphicsInitializePeer(TScreenContext  in_context)
{
    PEER_DP(("waveGraphicsInitializePeer"));
    return slim_true;
}

/*****************************************************************************/
// Description : Finalizes screen
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void waveGraphicsFinalizePeer(TScreenContext  in_context)
{
    PEER_DP(("waveGraphicsFinalizePeer"));
    return;
}

/*****************************************************************************/
// Description : Gets screen drawing capability information
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void waveGetGraphicsCapsPeer(TScreenContext  in_context,
                                    TGraphicsCaps  *out_caps)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    MMIWWW_PEER_DATA_T *peer_ptr = PNULL;

    PEER_DP(("waveGetGraphicsCapsPeer"));

    if (PNULL == out_caps || PNULL == www_instance)
    {
        return;
    }

    peer_ptr = &(www_instance->peer_data);
    out_caps->fFlag = 0;

#if WAVE_PAINT_DIRECTSCREEN
    out_caps->fFlag |= WAVE_SCREEN_CAPS_DIRECTSCREEN;
#else
    out_caps->fFlag |= WAVE_SCREEN_CAPS_INDIRECTSCREEN;
#endif

#ifdef WAVE_PEER_CONFIG_USE_CURSOR
    out_caps->fFlag |= WAVE_SCREEN_CAPS_SUPPORT_CURSOR;
    /* use SYSTEM hardware cursor */
    out_caps->fFlag |= WAVE_SCREEN_CAPS_SYSTEM_CURSOR;
    out_caps->fCursorWidth  = 2;
    out_caps->fCursorHeight = 2;
#else
    /* don't use cursor */
    out_caps->fFlag &= ~WAVE_SCREEN_CAPS_SUPPORT_CURSOR;
    out_caps->fCursorWidth  = 0;
    out_caps->fCursorHeight = 0;
#endif

    out_caps->fWidth = MAX(peer_ptr->fScreenHeight, peer_ptr->fScreenWidth);
    out_caps->fHeight = out_caps->fWidth;
    switch (peer_ptr->fPixelMapType)
    {
    case SLIM_WWWPIXMAP_RGB8:
        out_caps->fBytesPerPixel = 1;
        break;

    case SLIM_WWWPIXMAP_RGB16:
        out_caps->fBytesPerPixel = 2;
        break;

    case SLIM_WWWPIXMAP_RGB32:
        out_caps->fBytesPerPixel = 4;
        break;

    default:
        out_caps->fBytesPerPixel = 2;
        break;
    }

    out_caps->fBytesPerPixel = peer_ptr->fPixelMapType;
    out_caps->fScanLineAlign = SLIM_WWWPIXMAP_SCANLINEALIGN;

    return;
}

/*****************************************************************************/
// Description : Locks drawing screen
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void waveLockScreenPeer(TScreenContext  in_context, slim_int  in_hint)
{
    PEER_DP(("waveLockScreenPeer"));

    return;
}

/*****************************************************************************/
// Description : Unlocks drawing screen
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void waveUnlockScreenPeer(TScreenContext  in_context, slim_int  in_hint)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T update_rect = {0};
    MMIWWW_PEER_DATA_T *peer_data = PNULL;

    PEER_DP(("waveUnlockScreenPeer"));

    if (PNULL == www_instance || !MMIWWW_GetBrowserFocusFlag())
    {
        return;
    }

    peer_data = &(www_instance->peer_data);

    update_rect.left = 0;
    update_rect.top = (uint16)peer_data->fY;
    update_rect.right = (uint16)(peer_data->fScreenWidth - 1);
    update_rect.bottom = (uint16)(update_rect.top + peer_data->fScreenHeight - 1);

    if (WAVE_SCREEN_CONTEXT_PRIMARY == in_context)
    { 
        MMITHEME_StoreUpdateRect(&lcd_dev_info, update_rect);
    }

    return;
}

/*****************************************************************************/
// Description : Creates mouse cursor shape
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_opaque waveCreateCursorShapePeer(TScreenContext  in_context,
                                             void           *in_bitmap,
                                             void           *in_mask,
                                             slim_int        in_width,
                                             slim_int        in_height,
                                             slim_int        in_hotspot_x,
                                             slim_int        in_hotspot_y)
{
    PEER_DP(("waveCreateCursorShapePeer"));

    return (slim_opaque)0;
}

/*****************************************************************************/
// Description : Deletes mouse cursor
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void waveDisposeCursorShapePeer(TScreenContext  in_context,
                                       slim_opaque     in_cursor)
{
    PEER_DP(("waveDisposeCursorShapePeer"));

    return;
}

/*****************************************************************************/
// Description : Changes mouse cursor shape
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void waveSetCursorShapePeer(TScreenContext  in_context,
                                   slim_opaque     in_cursor)
{
    PEER_DP(("waveSetCursorShapePeer"));

    return;
}

/*****************************************************************************/
// Description : Displays/hides mouse cursor
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void waveShowCursorPeer(TScreenContext  in_context, slim_bool  in_show)
{
    PEER_DP(("waveShowCursorPeer"));
    return;
}

/*****************************************************************************/
// Description : Specifies mouse cursor display position
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void waveSetCursorPosPeer(TScreenContext  in_context,
                                 slim_int        in_x,
                                 slim_int        in_y)
{
    PEER_DP(("waveSetCursorPosPeer"));
    return;
}

/*****************************************************************************/
// Description : Searches for font
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int waveFindFontPeer(TScreenContext  in_context,
                                 slim_char      *in_fontfamily,
                                 TFontStyle     *in_fontstyle,
                                 slim_int        in_charset)
{
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    slim_int    font_type = 0;
    slim_int    fontsz = SLIM_FONT_SIZE_MEDIUM;
    slim_bool   bold = slim_false;
    slim_bool   italic = slim_false;

    if (PNULL == in_fontstyle || PNULL == www_instance)
    {
        return font_type;
    }

    PEER_DP(("waveFindFontPeer size=%d style=%d weight=%d,fFontSetting=%d,", in_fontstyle->fFontSize,
            in_fontstyle->fFontStyle, in_fontstyle->fFontWeight, www_instance->peer_data.fFontSetting));

    if (PNULL != in_fontfamily)
    {
        PEER_DP(("waveFindFontPeer in_fontfamily=%d", in_fontfamily));
    }

    if (in_fontstyle->fFontSize >= 0)
    {
        if (in_fontstyle->fFontSize > MMIWWW_XXXSMALL_FONT)
        {
            fontsz = in_fontstyle->fFontSize;
        }
        else
        {
            fontsz = MMIWWW_XXXSMALL_FONT;
        }
    }
    else
    {
        switch (in_fontstyle->fFontSize)
        {
        case SLIM_FONT_SIZE_XXSMALL:
        case SLIM_FONT_SIZE_XSMALL:
            fontsz = MMIWWW_XXXSMALL_FONT;
            break;

        case SLIM_FONT_SIZE_SMALL:
            fontsz = MMIWWW_SMALL_FONT;
            break;

        case SLIM_FONT_SIZE_MEDIUM:
            fontsz = MMIWWW_MEDIUM_FONT;
            break;

        case SLIM_FONT_SIZE_LARGE:
            fontsz = MMIWWW_LARGE_FONT;
            break;

        case SLIM_FONT_SIZE_XLARGE:
        case SLIM_FONT_SIZE_XXLARGE:
        case SLIM_FONT_SIZE_XXXLARGE:
            fontsz = MMIWWW_XXXLARGE_FONT;
            break;

        default:
            fontsz = SLIM_FONT_SIZE_MEDIUM;
            break;
        }
    }

    switch (www_instance->peer_data.fFontSetting)
    {
    case MMIWWW_FONT_TYPE_BIG:
        fontsz = MMIWWW_LARGE_FONT;
        break;

    case MMIWWW_FONT_TYPE_NORMAL:
        //not change
        break;

    case MMIWWW_FONT_TYPE_SMALL:
        fontsz = MMIWWW_XXXSMALL_FONT;
        break;

    default:
        break;
    }

    /* 字体风格 */
    switch(in_fontstyle->fFontStyle)
    {
    case SLIM_FONT_STYLE_ITALIC:
    case SLIM_FONT_STYLE_OBLIQUE:
        italic = slim_true;
        break;

    default:
        italic = slim_false;
        break;
    }

    /* 字体粗细 */
    switch(in_fontstyle->fFontWeight)
    {
    case SLIM_FONT_WEIGHT_600:
    case SLIM_FONT_WEIGHT_BOLD:
    case SLIM_FONT_WEIGHT_800:
    case SLIM_FONT_WEIGHT_900:
        bold = slim_true;
        break;

    default:
        bold = slim_false;
        break;
    }

    font_type = wvpeer_fint_font_index(fontsz, bold, italic);

    PEER_DP(("waveFindFontPeer find type = 0x%04x,fontsz=%d", font_type, fontsz));

    return font_type;
}

/*****************************************************************************/
// Description : Get font index
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL slim_int wvpeer_fint_font_index(slim_int   in_fontsz,
                                      slim_bool  in_bold,
                                      slim_bool  in_italic)
{
    slim_int font_type = 0;

    font_type = (slim_int)GUI_ConvertSizeToGUIFont((uint16)in_fontsz);

    font_type = ((font_type << 16) & MMIWWW_FONT_TYPE);

    if (in_bold)
    {
        font_type = font_type | MMIWWW_FONT_BOLD;
    }

    if (in_italic)
    {
        font_type = font_type | MMIWWW_FONT_ITALIC;
    }

    return font_type;
}

/*****************************************************************************/
// Description : Get font style by index
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void wvpeer_get_fontstyle(slim_int font_type,
                                GUISTR_STYLE_T  *out_style,
                                GUISTR_STATE_T  *out_state)
{
    GUI_FONT_T font = SONG_FONT_16;
    slim_bool is_bold = slim_false;
    slim_bool is_italic = slim_false;

    if (PNULL == out_style || PNULL == out_state)
    {
        return;
    }

    *out_state = GUISTR_STATE_SINGLE_LINE;

    font = (GUI_FONT_T)(((uint32)font_type & MMIWWW_FONT_TYPE) >> 16);
    is_bold = (slim_bool)(MMIWWW_FONT_BOLD == (font_type & MMIWWW_FONT_BOLD));
    is_italic = (slim_bool)(MMIWWW_FONT_ITALIC == ((font_type & MMIWWW_FONT_ITALIC)));

    out_style->font = font;

    if (is_bold)
    {
        *out_state |= GUISTR_STATE_BOLD;
    }
    if (is_italic)
    {
        *out_state |= GUISTR_STATE_ITALIC;
    }

    return;
}

/*****************************************************************************/
// Description : Gets font information
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void waveGetFontInfoPeer(TScreenContext  in_context,
                                slim_int        in_font,
                                TFontMetrics   *out_metrics)
{
    wchar             min = 0x69;/* i */
    GUISTR_STYLE_T    text_style = {0};/*lint !e64*/
    GUISTR_STATE_T    state = GUISTR_STATE_SINGLE_LINE;

    if (PNULL == out_metrics)
    {
        return;
    }
    
    wvpeer_get_fontstyle(in_font, &text_style, &state);

    PEER_DP(("waveGetFontInfoPeer in_font=%d,font=%d", in_font, text_style.font));

    out_metrics->fHeight = (slim_int)GUI_GetFontHeight(text_style.font, UNICODE_HANZI) + MMIWWW_FONT_ADJUST_FONT_HEIGHT;
    out_metrics->fMaxWidth = (slim_int)GUI_GetFontWidth(text_style.font, UNICODE_HANZI);
    
    out_metrics->fMinWidth = (slim_int)GUI_CalculateStringPiexlNum((const wchar *)&min, 1,
                                                                   text_style.font,
                                                                   WAVE_FONT_SPACE);

    out_metrics->fAscent  = (slim_int)(out_metrics->fHeight*4/5);
    out_metrics->fDescent = out_metrics->fHeight-out_metrics->fAscent;
    PEER_DP(("waveGetFontInfoPeer h=%d min=%d max=%d asc=%d des=%d",
            out_metrics->fHeight, out_metrics->fMinWidth, out_metrics->fMaxWidth,
            out_metrics->fAscent, out_metrics->fDescent));
    return;
}

/*****************************************************************************/
// Description : Gets display width of string
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC slim_int waveGetTextWidthPeer(TScreenContext  in_context,
                                     slim_char      *in_text,
                                     slim_int        in_len,
                                     slim_int        in_font)
{
    uint16            w_length = 0;
    wchar            *wstr_ptr = PNULL;
    slim_int          len = 0;
    uint16            length = (uint16)(((uint16)in_len + 1) * sizeof(wchar));
    GUISTR_STYLE_T    text_style = {0};/*lint !e64*/
    GUISTR_STATE_T    state = GUISTR_STATE_SINGLE_LINE;
    MMI_STRING_T string = {0};

    //PEER_DP(("waveGetTextWidthPeer!"));
    if (PNULL == in_text)
    {
        return 0;
    }
    
    wstr_ptr = SCI_ALLOCA((length + 1) * sizeof(wchar));
    if (PNULL == wstr_ptr)
    {
        return 0;
    }
    SCI_MEMSET(wstr_ptr, 0x00, ((length + 1) * sizeof(wchar)));
    w_length = GUI_UTF8ToWstr(wstr_ptr, (uint16)in_len,
                              (const uint8*)in_text, (uint16)in_len);
    if(0 != w_length)
    {
        string.wstr_ptr = wstr_ptr;
        string.wstr_len = w_length;
        wvpeer_get_fontstyle(in_font, &text_style, &state);
        len = GUISTR_GetStringWidth(&text_style, &string, state);
    }

    SCI_FREE(wstr_ptr);
    PEER_DP(("waveGetTextWidthPeer Out len = %d", len));
    return len;
}

/*****************************************************************************/
// Description : Color reduction process
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC TColor waveReduceColorPeer(TScreenContext  in_context, TColor  in_color)
{
    TColor c = in_color;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    
    //PEER_DP(("waveReduceColorPeer!"));
    if (PNULL != www_instance && WAVE_SCREEN_CONTEXT_PRIMARY == in_context)
    {
        switch(www_instance->peer_data.fPixelMapType)
        {
        case SLIM_WWWPIXMAP_RGB16:
            c = GUI_888_color_to_565_color(in_color);
            break;

        case SLIM_WWWPIXMAP_RGB32:
            c = in_color & 0x00ffffff;
            break;

        default:
            PEER_DP(("waveReduceColorPeer! need not reduce"));
            break;
        }
    }

    return c;
}

/*****************************************************************************/
// Description : Transfers bitmap
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void waveBitBltPeer(TScreenContext  in_context,      slim_int  in_src_x,
                           slim_int        in_src_y,        void     *in_bitmap,
                           void           *in_mask,         slim_int  in_bitmapwidth,
                           slim_int        in_bitmapheight, TRect    *in_rect)
{
    void *bit_ptr = PNULL;
    MMIWWW_PEER_DATA_T *peer_ptr = PNULL;
    slim_int dest_rowbytes = 0;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL == www_instance || !MMIWWW_GetBrowserFocusFlag())
    {
        return;
    }

    if (PNULL == in_bitmap || PNULL == in_rect)
    {
        return;
    }

    PEER_DP(("waveBitBltPeer:in_rect = (%d,%d,%d,%d)", in_rect->fX, 
            in_rect->fY, in_rect->fWidth, in_rect->fHeight));
    if (WAVE_SCREEN_CONTEXT_PRIMARY != in_context) return;
    peer_ptr = &(www_instance->peer_data);
    bit_ptr = peer_ptr->lcd_ptr;

    wvpeer_adjust_bitblt_rect(in_rect);

    switch(peer_ptr->fPixelMapType)
    {
    case SLIM_WWWPIXMAP_RGB8:
        dest_rowbytes = SLIM_ROUNDUP_P2(peer_ptr->fScreenWidth, WAVE_PEER_SCANLINEALING);
        wvpeer_bitblt_rgb8(bit_ptr, in_rect, in_bitmap, in_mask, in_src_x,
                           in_src_y, in_bitmapwidth, dest_rowbytes);
        break;

    case SLIM_WWWPIXMAP_RGB16:
        dest_rowbytes = SLIM_ROUNDUP_P2(peer_ptr->fScreenWidth*2, WAVE_PEER_SCANLINEALING);
        if( !in_mask )
        {
            wvpeer_bitblt_rgb16(bit_ptr, in_rect, in_bitmap, in_mask, in_src_x,
                                in_src_y, in_bitmapwidth, dest_rowbytes);
        }
        else
        {
            wvpeer_bitblt_argb16(bit_ptr, in_rect, in_bitmap, in_mask, in_src_x,
                                in_src_y, in_bitmapwidth, dest_rowbytes);
        }
        break;

    case SLIM_WWWPIXMAP_RGB32:
        dest_rowbytes = SLIM_ROUNDUP_P2(peer_ptr->fScreenWidth*4, WAVE_PEER_SCANLINEALING);
        wvpeer_bitblt_rgb32(bit_ptr, in_rect, in_bitmap, in_mask, in_src_x,
                            in_src_y, in_bitmapwidth, dest_rowbytes);
        break;

    default:
        break;
    }
    return;
}

/*****************************************************************************/
// Description : Adjust bitblt rect
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void wvpeer_adjust_bitblt_rect(TRect  *inout_rect)
{
    MMIWWW_PEER_DATA_T *peer_ptr = PNULL;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();

    if (PNULL == www_instance || PNULL == inout_rect)
    {
        return;
    }

    peer_ptr = &(www_instance->peer_data);
    if ((inout_rect->fHeight + inout_rect->fY) > peer_ptr->fScreenHeight)
    {
        inout_rect->fHeight = peer_ptr->fScreenHeight - inout_rect->fY;
    }
    inout_rect->fY += peer_ptr->fY;

    if ((inout_rect->fWidth + inout_rect->fX) > peer_ptr->fScreenWidth)
    {
        inout_rect->fWidth = peer_ptr->fScreenWidth - inout_rect->fX;
    }
    inout_rect->fX += peer_ptr->fX;

    return;
}

/*****************************************************************************/
// Description : Transfers bitmap for rgb8
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void wvpeer_bitblt_rgb8(void     *in_dest_bitmap,TRect    *in_dest_r, 
                              void     *in_src_bitmap, void     *in_src_mask,
                              slim_int  in_src_x,      slim_int  in_src_y,
                              slim_int  in_src_width,  slim_int  in_dest_rowbytes)
{
    slim_byte *dest_pixel = PNULL, *src_pixel = PNULL;
    slim_int y = 0, src_rowbytes = 0;

    PEER_DP(("wvpeer_bitblt_rgb8"));
    if (PNULL == in_src_bitmap || PNULL == in_dest_bitmap || PNULL == in_dest_r || !MMIWWW_GetBrowserFocusFlag())
    {
        return;
    }
    src_pixel = in_src_bitmap;
    dest_pixel = in_dest_bitmap;
    src_rowbytes = SLIM_ROUNDUP_P2( in_src_width, WAVE_PEER_SCANLINEALING);
    src_pixel += src_rowbytes * in_src_y + in_src_x;
    dest_pixel += in_dest_rowbytes * in_dest_r->fY + in_dest_r->fX;

    for (y = 0; y < in_dest_r->fHeight; y++)
    {
        SCI_MEMCPY(dest_pixel, src_pixel, in_dest_r->fWidth);
        src_pixel += src_rowbytes;
        dest_pixel += in_dest_rowbytes;
    }

    return;
}

/*****************************************************************************/
// Description : Transfers bitmap for rgb16
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void wvpeer_bitblt_rgb16(void     *in_dest_bitmap,TRect    *in_dest_r,
                               void     *in_src_bitmap, void     *in_src_mask,
                               slim_int  in_src_x,      slim_int  in_src_y,
                               slim_int  in_src_width,  slim_int  in_dest_rowbytes)
{
    slim_byte *dest_pixel = PNULL, *src_pixel = PNULL;
    slim_int y = 0, src_rowbytes = 0;

    PEER_DP(("wvpeer_bitblt_rgb16")); 
    if (PNULL == in_src_bitmap || PNULL == in_dest_bitmap || PNULL == in_dest_r || !MMIWWW_GetBrowserFocusFlag())
    {
        return;
    }
    src_pixel = in_src_bitmap;
    dest_pixel = in_dest_bitmap;
    src_rowbytes = SLIM_ROUNDUP_P2(in_src_width * 2, WAVE_PEER_SCANLINEALING);
    src_pixel += src_rowbytes * in_src_y + in_src_x * 2;
    dest_pixel += in_dest_rowbytes * in_dest_r->fY + in_dest_r->fX * 2;

    for (y = 0; y < in_dest_r->fHeight; y++)
    {
        SCI_MEMCPY(dest_pixel, src_pixel, in_dest_r->fWidth * 2);
        src_pixel += src_rowbytes;
        dest_pixel += in_dest_rowbytes;
    }

    return;
}

/*****************************************************************************/
// Description : Transfers bitmap for argb16
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void wvpeer_bitblt_argb16(void     *in_dest_bitmap,TRect    *in_dest_r,
                                void     *in_src_bitmap, void     *in_src_mask,
                                slim_int  in_src_x,      slim_int in_src_y,
                                slim_int  in_src_width,  slim_int in_dest_rowbytes)
{
    slim_byte *dest_pixel = PNULL, *src_pixel = PNULL, *dest_p = PNULL, *src_p = PNULL;
    slim_int x, y, src_rowbytes= 0;
    TColor color = 0;

    if (PNULL == in_src_bitmap || PNULL == in_dest_bitmap || PNULL == in_dest_r || !MMIWWW_GetBrowserFocusFlag())
    {
        return;
    }

    src_pixel = in_src_bitmap;
    dest_pixel = in_dest_bitmap;
    src_rowbytes = SLIM_ROUNDUP_P2(in_src_width * 2, WAVE_PEER_SCANLINEALING);
    src_pixel += src_rowbytes * in_src_y + in_src_x * 2;
    dest_pixel += in_dest_rowbytes * in_dest_r->fY + in_dest_r->fX * 2;

    for (y = 0; y < in_dest_r->fHeight; y++)
    {
        src_p = src_pixel;
        dest_p = dest_pixel;
        for (x = 0; x < in_dest_r->fWidth; x++)
        {
            color = SLIM_PEEK_16_NE_AL(src_p);
            if ((color & (1 << 15)) == 0)
            {
                unsigned int r, g, b;
                b = color & 0x1f;
                g = (color >> 5) & 0x1f;
                g <<= 1;
                r = (color >> 10) & 0x1f;
                color = b | (g << 5) | (r << 11);
                SLIM_POKE_16_NE_AL(dest_p, color);
            }
            src_p += 2;
            dest_p += 2;
        }
        src_pixel += src_rowbytes;
        dest_pixel += in_dest_rowbytes;
    }
}

/*****************************************************************************/
// Description : Transfers bitmap for rgb32
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL void wvpeer_bitblt_rgb32(void     *in_dest_bitmap,TRect    *in_dest_r,
                               void     *in_src_bitmap, void     *in_src_mask,
                               slim_int  in_src_x,      slim_int  in_src_y,
                               slim_int  in_src_width,  slim_int  in_dest_rowbytes)
{
    slim_byte *dest_pixel = PNULL, *src_pixel = PNULL;
    slim_int y = 0, src_rowbytes = 0;

    PEER_DP(("wvpeer_bitblt_rgb32")); 
    if (PNULL == in_src_bitmap || PNULL == in_dest_bitmap || PNULL == in_dest_r || !MMIWWW_GetBrowserFocusFlag())
    {
        return;
    }

    src_pixel = in_src_bitmap;
    dest_pixel = in_dest_bitmap;
    src_rowbytes = SLIM_ROUNDUP_P2(in_src_width * 4, WAVE_PEER_SCANLINEALING);
    src_pixel += src_rowbytes * in_src_y + in_src_x * 4;
    dest_pixel += in_dest_rowbytes * in_dest_r->fY + in_dest_r->fX * 4;

    for (y = 0; y < in_dest_r->fHeight; y++)
    {
        SCI_MEMCPY(dest_pixel, src_pixel, in_dest_r->fWidth * 4);
        src_pixel += src_rowbytes;
        dest_pixel += in_dest_rowbytes;
    }

    return;
}

/*****************************************************************************/
// Description : Draws string in bitmap in memory
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void waveDrawScreenTextPeer(TScreenContext in_context, void      *inout_bitmap,
                                   slim_int       in_rowbytes,slim_int   in_x,
                                   slim_int       in_y,       slim_char *in_text,
                                   slim_int       in_len,     TColor     in_color,
                                   slim_int       in_font,    TRect     *in_clip_rect)
{
    uint16 length = 0, w_length = 0;
    wchar *wstr_ptr = PNULL;
    GUI_COLOR_T color = 0;
    void *bit_ptr = PNULL;
    slim_int dest_rowbytes = 0;
#ifndef USE_EXT_DISPLAY_BUFFER
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_POINT_T point = {0};
#endif    
    TRect local_rect = *in_clip_rect;
    MMIWWW_PEER_DATA_T *peer_ptr = PNULL;
    MMI_STRING_T str_data = {0};
    GUISTR_STYLE_T text_style = {0};/*lint !e64*/
    GUISTR_STATE_T state = GUISTR_STATE_SINGLE_LINE;
    MMIWWW_INSTANCE_T *www_instance = MMIWWW_INSTANCE();
    uint16 screen_width = 0;
    uint16 screen_height = 0;

#ifdef USE_EXT_DISPLAY_BUFFER
    GUI_RECT_T text_rect = {0};
    GUISTR_BUF_T des_buf = {0};
#endif

    if (PNULL == www_instance || !MMIWWW_GetBrowserFocusFlag())
    {
        return;
    }

    peer_ptr = &(www_instance->peer_data);

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &screen_width, &screen_height);

    if (PNULL == inout_bitmap || PNULL == in_text || PNULL == in_clip_rect)
    {
        return;
    }

    PEER_DP(("waveDrawScreenTextPeer ptr=0x%p  (%d,%d),(%d,%d,%d,%d) font=0x%x",
            inout_bitmap, in_x, in_y, in_clip_rect->fX, in_clip_rect->fY,
            in_clip_rect->fWidth, in_clip_rect->fHeight, in_font));

    if (WAVE_SCREEN_CONTEXT_PRIMARY == in_context)
    {
        length = (uint16)in_len;
        wstr_ptr = SCI_ALLOCA((length + 1) * sizeof(wchar));
        if (PNULL == wstr_ptr)
        {
            return;
        }

        SCI_MEMSET(wstr_ptr, 0x00, ((length + 1) * sizeof(wchar)));
        w_length = GUI_UTF8ToWstr(wstr_ptr, length, (const uint8*)in_text, length);
        if(0 == w_length)
        {
            SCI_FREE(wstr_ptr);
            return;
        }

        color = (GUI_COLOR_T)waveReduceColorPeer(in_context, in_color);
#ifdef USE_EXT_DISPLAY_BUFFER
        bit_ptr = www_instance->display_buf_ptr;
#else
        bit_ptr = www_instance->peer_data.lcd_ptr;
#endif
        dest_rowbytes = SLIM_ROUNDUP_P2(www_instance->peer_data.fScreenWidth*2, WAVE_PEER_SCANLINEALING);
        local_rect.fY += peer_ptr->fY;
        wvpeer_bitblt_rgb16(bit_ptr, &local_rect, inout_bitmap, 0L, in_clip_rect->fX,
                            in_clip_rect->fY, (uint32)in_rowbytes/sizeof(GUI_COLOR_T), dest_rowbytes);

        // text
        str_data.wstr_ptr = wstr_ptr;
        str_data.wstr_len = w_length;
        // style
        wvpeer_get_fontstyle(in_font, &text_style, &state);
        text_style.font_color = color;
#ifdef USE_EXT_DISPLAY_BUFFER
        text_rect.left = (int16)in_x;
        text_rect.top = (int16)(in_y + peer_ptr->fY);
        text_rect.right = screen_width - 1;
        text_rect.bottom = screen_height - 1 ;
        // buffer
        des_buf.buffer = www_instance->display_buf_ptr;
        des_buf.mem_width = des_buf.width = screen_width;
        des_buf.height = screen_height;
        // style
        text_style.char_space = WAVE_FONT_SPACE;

        PEER_DP(("waveDrawScreenTextPeer start draw font=%d state=%d",
                text_style.font, state));

        GUISTR_DrawTextInRect( 
                                                (GUISTR_BUF_T *)&des_buf,
                                                (const GUI_RECT_T *)&text_rect,       //the fixed display area
                                                (const GUI_RECT_T *)&text_rect,       //用户要剪切的实际区域
                                                (const MMI_STRING_T *)&str_data,
                                                &text_style,
                                                state,
                                                GUISTR_TEXT_DIR_AUTO
                                                );
#else
        point.x = (int16)in_x;
        point.y = (int16)(in_y + peer_ptr->fY);
        GUISTR_DrawTextToLCDMultiLine( 
                                                            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                                            (const GUI_POINT_T *)&point,
                                                            (const MMI_STRING_T *)&str_data,
                                                            (GUISTR_STYLE_T *)&text_style,
                                                            state
                                                            );
#endif
        wvpeer_bitblt_rgb16(inout_bitmap, in_clip_rect, bit_ptr, 0L, local_rect.fX,
                            local_rect.fY, www_instance->peer_data.fScreenWidth, in_rowbytes);

        SCI_FREE(wstr_ptr);
    }

    return;
}

/*****************************************************************************/
// Description : Draws string
// Global resource dependence :
// Author: Bill.Ji
// Note: Not used in "INDIRECT" drawing method
/*****************************************************************************/
PUBLIC void waveDrawTextPeer(TScreenContext in_context,  slim_int   in_x, 
                             slim_int       in_y,        slim_char *in_text,
                             slim_int       in_len,      TColor     in_color,
                             slim_int       in_font,     TRect     *in_clip_rect)
{
    uint16 length = 0, w_length = 0;
    wchar *wstr_ptr = PNULL;
    GUI_COLOR_T color = 0;
    GUI_RECT_T text_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    slim_int title = MMI_STATUSBAR_HEIGHT + WAV_BROWSWER_TITLE_HEIGHT;
    MMI_STRING_T str_data = {0};
    GUISTR_STYLE_T text_style = {0};/*lint !e64*/
    GUISTR_STATE_T state = GUISTR_STATE_SINGLE_LINE;

    if (PNULL == in_text || PNULL == in_clip_rect)
    {
        return;
    }
    
    PEER_DP(("waveDrawTextPeer (%d,%d),(%d,%d,%d,%d)", in_x, in_y, in_clip_rect->fX,
            in_clip_rect->fY, in_clip_rect->fWidth, in_clip_rect->fHeight));

    if (WAVE_SCREEN_CONTEXT_PRIMARY == in_context)
    {
        length = (uint16)in_len;
        wstr_ptr = SCI_ALLOCA((length + 1) * sizeof(wchar));
        if (PNULL == wstr_ptr)
        {
            return;
        }

        SCI_MEMSET(wstr_ptr, 0x00, ((length + 1) * sizeof(wchar)));
        w_length = GUI_UTF8ToWstr(wstr_ptr, length, (const uint8*)in_text, length);
        if(0 == w_length)
        {
            return;
        }

        color = (GUI_COLOR_T)waveReduceColorPeer(in_context, in_color);
#if 0
        text_rect.left   = (uint16)in_clip_rect->fX;
        text_rect.top    = (uint16)in_clip_rect->fY;
        text_rect.right  = (uint16)(in_clip_rect->fX + in_clip_rect->fWidth - 1);
        text_rect.bottom = (uint16)(in_clip_rect->fY + in_clip_rect->fHeight - 1);
#else
        in_y += title;
        in_clip_rect->fY += title;
        text_rect.left   = (uint16)in_x;
        text_rect.top    = (uint16)in_y;
        text_rect.right  = (uint16)(in_clip_rect->fX + in_clip_rect->fWidth - 1);
        text_rect.bottom = (uint16)(in_clip_rect->fY + in_clip_rect->fHeight - 1);
#endif

        str_data.wstr_len = w_length;
        str_data.wstr_ptr = wstr_ptr;
        //
        text_style.font = (GUI_FONT_T)in_font;
        text_style.font_color = color;
        text_style.char_space = WAVE_FONT_SPACE;

        // display
        GUISTR_DrawTextToLCDInRect( 
                                                        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                                                        (const GUI_RECT_T *)&text_rect,       //the fixed display area
                                                        (const GUI_RECT_T *)&text_rect,       //用户要剪切的实际区域
                                                        (const MMI_STRING_T *)&str_data,
                                                        &text_style,
                                                        state,
                                                        GUISTR_TEXT_DIR_AUTO
                                                        ); 

        SCI_FREE(wstr_ptr);
    }

    return;
}

/*****************************************************************************/
// Description : Copies content in rectangular area of drawing buffer
// Global resource dependence :
// Author: Bill.Ji
// Note: Not used in "INDIRECT" drawing method
/*****************************************************************************/
PUBLIC void waveCopyAreaPeer(TScreenContext in_context, TRect   *in_rect, 
                             slim_int       in_dx,      slim_int in_dy)
{
    GUI_COLOR_T *buf_ptr = PNULL;
    uint32 buf_len = 0;
    TRect src = {0};
    TRect dst = {0};
    slim_int title = MMI_STATUSBAR_HEIGHT + WAV_BROWSWER_TITLE_HEIGHT;

    PEER_DP(("waveCopyAreaPeer (%d,%d),(%d,%d,%d,%d)",in_dx, in_dy, in_rect->fX,
            in_rect->fY, in_rect->fWidth, in_rect->fHeight)); 

    in_rect->fY += title;
    // Horizontal Movement
    if (in_dx > 0)
    {
        src.fX     = in_rect->fX;
        src.fY     = in_rect->fY;
        src.fWidth = in_rect->fWidth - in_dx;
        src.fHeight= in_rect->fHeight;
        dst.fX     = in_rect->fX + in_dx;
        dst.fY     = in_rect->fY;
        dst.fWidth = in_rect->fWidth - in_dx;
        dst.fHeight= in_rect->fHeight;
    }
    else if (in_dx < 0)
    {
        src.fX     = in_rect->fX - in_dx;
        src.fY     = in_rect->fY;
        src.fWidth = in_rect->fWidth + in_dx;
        src.fHeight= in_rect->fHeight;
        dst.fX     = in_rect->fX;
        dst.fY     = in_rect->fY;
        dst.fWidth = in_rect->fWidth + in_dx;
        dst.fHeight= in_rect->fHeight;
    }
    // Vertical Movement
    if (in_dy > 0)
    {
        src.fX     = in_rect->fX;
        src.fY     = in_rect->fY;
        src.fWidth = in_rect->fWidth;
        src.fHeight= in_rect->fHeight - in_dy;
        dst.fX     = in_rect->fX;
        dst.fY     = in_rect->fY + in_dy;
        dst.fWidth = in_rect->fWidth;
        dst.fHeight= in_rect->fHeight - in_dy;
    }
    else if (in_dy < 0)
    {
        src.fX     = in_rect->fX;
        src.fY     = in_rect->fY - in_dy;
        src.fWidth = in_rect->fWidth;
        src.fHeight= in_rect->fHeight + in_dy;
        dst.fX     = in_rect->fX;
        dst.fY     = in_rect->fY;
        dst.fWidth = in_rect->fWidth;
        dst.fHeight= in_rect->fHeight + in_dy;
    }
    buf_len = src.fWidth * src.fHeight;
    buf_ptr = SCI_ALLOCA(buf_len * sizeof(GUI_COLOR_T));

    if (PNULL == buf_ptr)
    {
        return;
    }

    //SCI_MEMSET(buf_ptr, 0x00, (buf_len * sizeof(GUI_COLOR_T)));

    LCD_CopyMainLcdBufToMem(buf_ptr, src.fWidth, src.fHeight,
                            src.fX, src.fY, FALSE);

    LCD_CopyMemToMainLcdBufNoKeyColor(buf_ptr, dst.fWidth, dst.fHeight,
                                      dst.fX, dst.fY, FALSE);

    SCI_FREE(buf_ptr);

    return;
}

/*****************************************************************************/
// Description : Fills rectangle
// Global resource dependence :
// Author: Bill.Ji
// Note: Not used in "INDIRECT" drawing method
/*****************************************************************************/
PUBLIC void waveFillRectPeer(TScreenContext in_context, TRect *in_rect, TColor in_color)
{
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_COLOR_T color = 0;
    slim_int title = MMI_STATUSBAR_HEIGHT + WAV_BROWSWER_TITLE_HEIGHT;
    
    if (PNULL == in_rect)
    {
        return;
    }
    
    PEER_DP(("waveFillRectPeer rect{%d,%d,%d,%d} color=%d", in_rect->fX, in_rect->fY, 
            in_rect->fWidth, in_rect->fHeight, in_color));

    if (WAVE_SCREEN_CONTEXT_PRIMARY == in_context)
    {
        in_rect->fY += title;
        rect.left = (uint16)in_rect->fX;
        rect.top = (uint16)in_rect->fY;
        rect.right = (uint16)(in_rect->fX + in_rect->fWidth - 1);
        rect.bottom = (uint16)(in_rect->fY + in_rect->fHeight - 1);
        color = (GUI_COLOR_T)waveReduceColorPeer(in_context, in_color);

        LCD_FillRect(&lcd_dev_info, rect, color);
    }

    return;
}

/*****************************************************************************/
// Description : Gets drawing buffer data
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void waveGetPixelsPeer(TScreenContext  in_context,  slim_int  in_src_x,
                              slim_int        in_src_y,    void     *out_bitmap,
                              slim_int        in_rowbytes, TRect    *in_rect)
{
    // Not used in "INDIRECT" drawing method
    PEER_DP(("waveGetPixelsPeer"));

    return;
}

/*****************************************************************************/
// Description : Reflects drawing result immediately
// Global resource dependence :
// Author: Bill.Ji
// Note: Not used in "INDIRECT" drawing method
/*****************************************************************************/
PUBLIC void waveRefreshScreenPeer(TScreenContext in_context)
{
    PEER_DP(("waveRefreshScreenPeer"));

    if (WAVE_SCREEN_CONTEXT_PRIMARY == in_context)
    {
        MMITHEME_UpdateRect();
    }

    return;
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void slimPixelMapOptimizePeer(slim_int   in_src_type, slim_int   in_width,
                                     slim_int   in_height,   slim_byte *in_pixels,
                                     slim_byte *in_mask,     slim_byte *in_palette,
                                     slim_int   in_dest_type)
{
    // Not used in "INDIRECT" drawing method
    PEER_DP(("slimPixelMapOptimizePeer"));

    return;
}
#endif //#ifdef BROWSER_SUPPORT_NETFRONT
