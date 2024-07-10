/*****************************************************************************
** File Name:   brw_core_adapter.h                                           *
** Author:      jiqun.li                                                     *
** Date:      05/8/2009                                                      *
** Copyright:   2009 Spreadtrum, Incorporated. All Rights Reserved.          *
** Description:   This file is used to describe the adapter of browser core  *
**              come function is called by browser core
*****************************************************************************/
#ifndef BRW_CORE_ADAPTER_H
#define BRW_CORE_ADAPTER_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "brw_plinterface.h"
#include "brw_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
typedef struct RECT_TAG
{
    int32  left;    
    int32  right;
    int32  top;
    int32  bottom;
} RECT_T;

typedef struct BRW_POINT_TAG
{
    int32  x;
    int32  y;
} BRW_POINT_T;


//----- start
typedef uint8 WAP_STR_INFO_T;  //do not use bitfield to save memory in wap layout
#define WAP_BIT_RTL         (1<<0)
#define WAP_BIT_BDO         (1<<1)
#define WAP_BIT_BOLD        (1<<2)
#define WAP_BIT_ITALIC      (1<<3)
#define WAP_BIT_DEL         (1<<4)
#define WAP_BIT_UNDERLINE   (1<<5)
#define IS_WAP_BIT_RTL(info)      ( 0!=(info&WAP_BIT_RTL) )
//---- end

typedef enum
{
    BRW_PEN_STYLE_DEFAULT,
    BRW_PEN_STYLE_DOT,
    BRW_PEN_STYLE_DASH,
    BRW_PEN_STYLE_DASH_DOT,
    BRW_PEN_STYLE_MAX
}BRW_PEN_STYLE_E;

typedef enum
{
    BRW_CONFIRM_OK,
    BRW_CONFIRM_CANCEL,
	BRW_PAGE_DESTROYED,
	BRW_CONFIRM_NONE,
    BRW_CONFIRM_WAIT
}BRW_CONFIRM_OP_E;

typedef enum
{
    BRW_TRAMPMF_LINE_TOP,
    BRW_TRAMPMF_LINE_BOTTOM,
    BRW_TRAMPMF_LINE_LEFT,
    BRW_TRAMPMF_LINE_RIGHT,
    BRW_TRAMPMF_LINE_MAX
}BRW_TRAMPMF_LINE_DIRECT_E;

typedef struct {
    const wchar     *wstr_ptr;
    uint16          wstr_len;    // -1 means wstr has NULL at tail
    WAP_STR_INFO_T  str_info;
}WAP_STRING_T;

typedef struct  {
    uint32 font_type;
    BOOLEAN is_font_bold;
    BOOLEAN is_font_italic;
    BOOLEAN is_font_underline;
}FONT_TYPE_STATE_T;

typedef struct  {
    int16 word_space;
    BOOLEAN is_space_valid;
#ifdef CSS_SUPPORT
    BOOLEAN is_blinking;
#endif
}TEXT_STYLE_T;

typedef struct {
    const uint16 *str_ptr;
    uint16          str_len;    
    WAP_STR_INFO_T  str_info;
	FONT_TYPE_STATE_T font;
	TEXT_STYLE_T      text_style;
}WAP_STRING_FONT_TEXT_T;

typedef struct {
    RECT_T          *rect_ptr;          //[IN] image show in rect, 网页内坐标
    uint8           *data_ptr;          //[IN] image data
    uint32          data_len;           //[IN] image data length
    BRW_MIME_TYPE_E mime_type;          //[IN] image type
    uint32          width;              //[IN] image width
    uint32          height;             //[IN] image height
    RECT_T          *region_ptr;        //[IN] web page display regin
    uint32          gif_handle;         //[IN][OUT] gif handle. only for gif play
    uint32          frame_delay;        //[IN][OUT] gif framedelay;
    BOOLEAN is_not_need_zoom_in;   //[IN]is not need to zoom
}IMAGE_PLAY_INFO;

typedef struct {
    uint8   *data_ptr;          //[IN] audio data
    uint32  data_len;           //[IN] audio data length
    BRW_MIME_TYPE_E bgsound_type;       // 音频数据类型，如mpeg,mid等
    int32   loop;
    int32   balance;//useless
    int32   volume;//useless,use MMISET_VOL_TWO
    uint32  status;//useless    
}BGSOUND_INFO;

typedef struct tagLAYOUT_CTRL       LAYOUT_CTRL_T;
typedef struct tagXHTML_INPUT       XHTML_INPUT_T;
typedef struct tagWML_LAYOUT_INPUT  WML_LAYOUT_INPUT_T;
typedef struct XHTML_LAYOUT_TAG     XHTML_LAYOUT_T;
typedef struct tagWML_LAYOUT_SELECT WML_LAYOUT_SELECT_T;
typedef struct tagWML_LAYOUT_OPTION WML_LAYOUT_OPTION_T;

typedef struct
{
    XHTML_LAYOUT_T  *layout_ptr;    
    LAYOUT_CTRL_T   *layout_ctrl_ptr;
}XHTML_INPUT_WIN_INFO_T;

typedef struct
{
    XHTML_LAYOUT_T  *select_ptr;    
    LAYOUT_CTRL_T  *layout_ctrl_ptr;
}XHTML_SELECT_WIN_INFO_T;

typedef struct
{
    WML_LAYOUT_INPUT_T  *input_ptr;    
    LAYOUT_CTRL_T       *layout_ctrl_ptr;
}WML_INPUT_WIN_INFO_T;

typedef struct
{
    WML_LAYOUT_SELECT_T  *select_ptr;    
    LAYOUT_CTRL_T        *layout_ctrl_ptr;
}WML_SELECT_WIN_INFO_T;
/*max supported image size*/
#define MAX_IMAGE_WIDTH         800
#define MAX_IMAGE_HEIGHT        800

#define BRW_TEXTLINK_SPACE      2   //space pixles between two anchor link
#define BRW_LINE_SPACE          2   //line space no include input bottom
#define BRW_LINE_SPACE_HEIGHT   3   //line space include input bottom

/*space between font and underline*/
#define BRW_UNDERLINE_SPACE     1

/*text color*/
#define BRW_RGB16_SCROLLBAR_BACKGROUND  (0xc618)    //scroll bar background color
#define BRW_RGB16_TEXT                  (0x0000)    //common text color is black
#define BRW_RGB16_RED                   (0xf800)    //RED
#define BRW_RGB16_TEXTLINK              (0x001f)    //hyper-link text color is blue
#define BRW_RGB16_TEXTLINK_READED       (0xf4c0)    //clicked hyper-link text color
#define BRW_RGB16_BACKGROUND            (0xffff)    //background color is white
#define BRW_RGB16_FOCUSED_BACKGROUND    (0xf4c0)    //focused hyper-link text background color
#define BRW_RGB16_BUTTON_SURFACE        (0xAD53)    //(0xEF5B)  //button suface color
#define BRW_RGB16_BUTTON_BORDER         (0x736C)    //(0xAD53)  //button shadow border color
#define BRW_RGB16_BUTTON_DARK_BORDER    (0x0000)    //(0x736C)  //button dark shdow border color


#define BRW_TABLE_MAX_ROW_SPAN          20   //max supported row span for table
#define BRW_TABLE_MAX_COL_SPAN          20   //max supported column span for table
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : open an alert window for display:Invalid input
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void BrwDisplayInvalidInput(void);

#ifdef JS_SUPPORT
/*****************************************************************************/
//  Description : open an alert window for user
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN BrwXhtmlLayoutOpenAlertWin(const uint16 *str_ptr);

/*****************************************************************************/
//  Description : open a confirm window for user
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN BrwXhtmlLayoutOpenConfirmWin(const uint16 *str_ptr);

/*****************************************************************************/
//  Description : open an prompt window for user input
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN BrwXhtmlLayoutOpenPromptWin(const uint16* str_ptr,const uint16 *default_str, uint16 *user_data_ptr);

PUBLIC BOOLEAN IsWaitUserConfirm(void);

PUBLIC void CancelWaitUserConfirm(void);
/*****************************************************************************/
//  Description : get history num
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC uint16 BrwGetCurHisNum(void);

#endif
/******************************************************************************************
* DESCRIPTION:
*   draw a vertical line
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   y1: [IN] y-coordinate start
*   y2: [IN] y-coordinate end
*   x:  [IN] x-coordinate
*   color:  [IN] RGB565 color
*   region_ptr: [IN] web page display region
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void BrwGraphicsDrawVLine(int32 x, int32 y1, int32 y2, uint16 color, RECT_T *region_ptr);

/******************************************************************************************
* DESCRIPTION:
*   draw a Unicode char width
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   font_type: [IN] font type
*   unicode: [IN] unicode char
*
* RETURN:
*   char width
*
* NOTE:
******************************************************************************************/
uint8 BrwFontGetCharWidth(uint32 font_type, uint16 unicode);

/******************************************************************************************
* DESCRIPTION:
*   get a Chinese char width
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   font_type: [IN] font type
*
* RETURN:
*   Chinese char width
*
* NOTE:
******************************************************************************************/
uint8 BrwFontGetHanziWidth(uint32 font_type);

/******************************************************************************************
* DESCRIPTION:
*   get unicode string width
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   unocode_ptr: [IN] unicode string
*   len:  [IN] unicode char number in unicode string, -1 means unicode string has Zero end
*   font_type_state:   [IN] font type state
*
* RETURN:
*   Chinese char width
*
* NOTE:
******************************************************************************************/
uint16 BrwFontGetStringWidth(uint16 *unicode_ptr,uint32 len, FONT_TYPE_STATE_T font_type_state);

/******************************************************************************************
* DESCRIPTION:
*   get num of lines when showing unicode string
*
* AUTHOR:
*   sally.he
*
* PARAMETERS:
*   pixelnum:    [IN] pixel number
*   unocode_ptr: [IN] unicode string
*   len:  [IN] unicode char number in unicode string, -1 means unicode string has Zero end
*   font_type:   [IN] font type
*
* RETURN:
*   Chinese char width
*
* NOTE:
******************************************************************************************/
PUBLIC uint16 BrwtFontGetStringLines(
                                     uint16    pixelnum,    
                                     uint16    *unicode_ptr, 
                                     uint32    len, 
                                     uint32    font_type
                                     );

/******************************************************************************************
* DESCRIPTION:
*   get a line height in browser layout
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   font_type: [IN] font type
*
* RETURN:
*   line height in browser layout
*
* NOTE:
******************************************************************************************/
uint16 BrwLineHeight(uint32 font_type);

/******************************************************************************************
* DESCRIPTION:
*   draw a unicode string to screen
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   rect_ptr: [IN] pointer to text display region, in logical coordinates.
*   wap_string_ptr: [IN] pointer to the string info to be drawn.
*   color: [IN] specifies the color.
*   font_type: [IN] specifies the font type.
*   region_ptr: [IN] specifies the display region, in physical coordinates.
*   is_line_single:[IN] show text in one line or multiple lines.
regionfont type
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void BrwDrawText(RECT_T *rect, WAP_STRING_FONT_TEXT_T *wap_string_ptr, uint16 color, 
                                 RECT_T *region_ptr, BOOLEAN is_line_single);

/******************************************************************************************
* DESCRIPTION:
*   create a window for user to selecting item in WML web page.
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   select_ptr: [IN] Handle to the <select> layout.
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void WmlLayoutCreateSelectWindow(WML_LAYOUT_SELECT_T *select_ptr, LAYOUT_CTRL_T  *layout_ctrl_ptr);

/******************************************************************************************
* DESCRIPTION:
*   create a window for user to selecting item in HTML web page.
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   select_ptr: [IN] Handle to the HTML <select> layout.
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void XhtmlLayoutCreateSelectWindow(XHTML_LAYOUT_T *select_ptr, LAYOUT_CTRL_T  *layout_ctrl_ptr);

/******************************************************************************************
* DESCRIPTION:
*   create WML edit windows for <input> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] Handle to WML <input> layout
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void WmlLayoutCreateEditWindow(WML_LAYOUT_INPUT_T *input_ptr, LAYOUT_CTRL_T  *layout_ctrl_ptr);

/******************************************************************************************
* DESCRIPTION:
*   create HTML editing windows for <input> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] Handle to HTML <input> layout
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void XhtmlLayoutCreateEditWindow(XHTML_LAYOUT_T *input_ptr, LAYOUT_CTRL_T  *layout_ctrl_ptr);

/*****************************************************************************/
//  Description : open form path win
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC void XhtmlOpenFormFileWin(uint16   *selected_path_ptr);

/******************************************************************************************
* DESCRIPTION:
*   convert gb2312 string to unicode string
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   unicode_ptr: [OUT] destnation unicode string
*   gb2312_ptr:  [IN] source GB2312 string
*   len:         [IN] source GB2312 string length
*
* RETURN:
*   unicode string length, number of unicode char
*
* NOTE:
******************************************************************************************/
uint32 BrwGb2312ToWstr(wchar *unicode_ptr, const uint8 *gb2312_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert gb2312 string to unicode string
*
* AUTHOR:
*   sally.he
*
* PARAMETERS:
*   unicode_ptr: [OUT] destnation gb2312 string
*   gb2312_ptr:  [IN] source unicode string
*   len:         [IN] source unicode string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint16 BrwWstrToGb2312(uint8  *gb2312_ptr, const wchar *wstr_ptr, uint32 len);
/******************************************************************************************
* DESCRIPTION:
*   convert a unicode char to GB2312 buffer
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   gb2312_ptr: [OUT] save destnation GB2312 char
*   unicode:    [IN]  unicode char
*
* RETURN:
*   bytes of GB2312 char
*
* NOTE:
******************************************************************************************/
int32 BrwUnicode2gb2312(uint8 *gb2312_ptr, uint16 unicode);

/******************************************************************************************
* DESCRIPTION:
*   convert big5 string to unicode string
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   unicode_ptr: [OUT] destnation unicode string
*   big5_ptr:  [IN] source big5 string
*   len:         [IN] source big5 string length
*
* RETURN:
*   unicode string length, number of unicode char
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwBig5ToWstr(wchar *unicode_ptr, const uint8 *big5_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert unicode to big5 string string
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   unicode_ptr: [OUT] destnation unicode string
*   big5_ptr:    [IN] source big5 string
*   len:         [IN] source big5 string length
*
* RETURN:
*   unicode string length, number of unicode char
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWstrToBig5(uint8 *big5_ptr, const wchar *unicode_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert windows 1252 string to unicode string
*
* AUTHOR:
*   wenmei.hua
*
* PARAMETERS:
*   unicode_ptr: [OUT] destination unicode string
*   win1252:     [IN] source win1252 string
*   len:         [IN] source win1252 string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWin1252ToWstr(wchar *unicode_ptr, const uint8 *win1252_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert unicode  string to windows 1252 string
*
* AUTHOR:
*   wenmei.hua
*
* PARAMETERS:
*   win1252_ptr: [OUT] destination win1252 string
*   unicode_ptr: [IN] source unicode string
*   len:         [IN] source unicode string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWstrToWin1252( uint8 *win1252_ptr, const wchar *unicode_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert windows 1251 string to unicode string
* AUTHOR:  wenmei.hua
* PARAMETERS:
*   unicode_ptr: [OUT] destination unicode string
*   win1251:     [IN] source win1251 string
*   len:         [IN] source win1251 string length
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWin1251ToWstr(wchar *unicode_ptr, const uint8 *win1251_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert unicode  string to windows 1251 string
* AUTHOR:   wenmei.hua
* PARAMETERS:
*   win1251_ptr: [OUT] destination win1251 string
*   unicode_ptr: [IN] source unicode string
*   len:         [IN] source unicode string length
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWstrToWin1251( uint8 *win1251_ptr, const wchar *unicode_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert windows 1256 string to unicode string
*
* AUTHOR:
*   xianhe.yang
*
* PARAMETERS:
*   unicode_ptr: [OUT] destination unicode string
*   win1256:     [IN] source win1256 string
*   len:         [IN] source win1256 string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWin1256ToWstr(wchar *unicode_ptr, const uint8 *win1256_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert unicode  string to windows 1256 string
*
* AUTHOR:
*   xianhe.yang
*
* PARAMETERS:
*   win1256_ptr: [OUT] destination win1256 string
*   unicode_ptr: [IN] source unicode string
*   len:         [IN] source unicode string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWstrToWin1256( uint8 *win1256_ptr, const wchar *unicode_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert iso-8859-5 string to unicode string
*
* AUTHOR:
*   xianhe.yang
*
* PARAMETERS:
*   unicode_ptr: [OUT] destnation unicode string
*   iso88595_ptr: [IN] source iso-8859-5 string
*   len:         [IN] source iso-8859-5 string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwIso88595ToWstr(wchar *unicode_ptr, const uint8 *iso88595_ptr, uint32 len);
/******************************************************************************************
* DESCRIPTION:
*   convert unicode  string to iso-8859-5 string
*
* AUTHOR:
*   xianhe.yang
*
* PARAMETERS:
*   iso88595_ptr: [OUT] destnation iso-8859-5 string
*   unicode_ptr: [IN] source unicode string
*   len:         [IN] source unicode string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWstrToIso88595( uint8 *iso88595_ptr, const wchar *unicode_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert iso-8859-2 string to unicode string
*
* AUTHOR:
*   xianhe.yang
*
* PARAMETERS:
*   unicode_ptr: [OUT] destnation unicode string
*   iso88595_ptr: [IN] source iso-8859-5 string
*   len:         [IN] source iso-8859-5 string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwIso88592ToWstr(wchar *unicode_ptr, const uint8 *iso88595_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert unicode  string to iso-8859-2 string
*
* AUTHOR:
*   xianhe.yang
*
* PARAMETERS:
*   iso88595_ptr: [OUT] destnation iso-8859-5 string
*   unicode_ptr: [IN] source unicode string
*   len:         [IN] source unicode string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWstrToIso88592( uint8 *iso88595_ptr, const wchar *unicode_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert koi8-r string to unicode string
* AUTHOR:   wenmei.hua
* PARAMETERS:
*   unicode_ptr: [OUT] destnation unicode string
*   koi8r_ptr:   [IN] source koi8r_ptr string
*   len:         [IN] source koi8r_ptr string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwKoi8rToWstr(wchar *unicode_ptr, const uint8 *koi8r_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert unicode  string to koi8r string
* AUTHOR:  wenmei.hua
* PARAMETERS:
*   koi8r_ptr: [OUT] destnation kio8r string
*   unicode_ptr: [IN] source unicode string
*   len:         [IN] source unicode string length
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWstrToKoi8r( uint8 *koi8r_ptr, const wchar *unicode_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert windows874 string to unicode string
* AUTHOR:   wenmei.hua
* PARAMETERS:
*   unicode_ptr:      [OUT] destnation unicode string
*   cp874_ptr:        [IN] source cp874_ptr string
*   len:              [IN] source cp874_ptr string length
*
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwCP874ToWstr(wchar *unicode_ptr, const uint8 *cp874_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   convert unicode  string to cp874 string
* AUTHOR:  wenmei.hua
* PARAMETERS:
*   cp874_ptr:   [OUT] destnation cp874 string
*   unicode_ptr: [IN] source unicode string
*   len:         [IN] source unicode string length
* NOTE:
******************************************************************************************/
PUBLIC uint32 BrwWstrToCP874( uint8 *cp874_ptr, const wchar *unicode_ptr, uint32 len);

/******************************************************************************************
* DESCRIPTION:
*   fill rectangle with RGB565 color
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   rect_ptr: [IN] rectangle
*   rgb16:    [IN] RGB565 color
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void BrwFillLcdRect(RECT_T *rect_ptr,uint16 rgb16);

/******************************************************************************************
* DESCRIPTION:
*   clip and fill rectangle with RGB565 color
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   rect_ptr: [IN] rectangle
*   rgb16:    [IN] RGB565 color
*   region_ptr:[IN] clip rectangle
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void BrwGraphicsFillRect(RECT_T *rect_ptr, uint16 rgb16, RECT_T *region_ptr);

/******************************************************************************************
* DESCRIPTION:
*   draw rectangle with extended mode
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   rect_ptr: [IN] rectangle
*   rgb16:    [IN] RGB565 color
*   is_xor:   [IN] TRUE: XOR operation with background
*   region_ptr:[IN] clip rectangle
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void  BrwGraphicsDrawRectExt(RECT_T *rect_ptr, uint16 rgb16, BOOLEAN is_xor, RECT_T *region_ptr);

/******************************************************************************************
* DESCRIPTION:
*   draw a horizontal line
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   x1: [IN] x-coordinate start
*   x2: [IN] x-coordinate end
*   y:  [IN] y-coordinate
*   color:  [IN] RGB565 color
*   region_ptr: [IN] web page display region
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void BrwGraphicsDrawHLine(int32 x1, int32 y, int32 x2, uint16 color, RECT_T *region_ptr);

/******************************************************************************************
* DESCRIPTION:
*   draw a rectangle
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   rect_ptr: [IN] rectangle pointer
*   color:  [IN] RGB565 color
*   region_ptr: [IN] web page display region
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void BrwGraphicsDrawRect(RECT_T *rect, uint16 rgb16, RECT_T *region_ptr);
/******************************************************************************************
* DESCRIPTION:
*   Draw trapmf with RGB565 color
*
* AUTHOR:
*   sally.he
*
* TOP:

   x1, y1                                        x2,y2
    ---------------------------------------------
     .                                        .     width
      ----------------------------------------
                                           
  BOTTOM:
  
      x1,y1                                  x2,y2
      ----------------------------------------
     .                                        .     width
    ---------------------------------------------
                                              

  LEFT:            Right

  x1,y1                  
  |                   x1,y1 |  
  | .                     . |            
  |  |                   |  |
  |  |                   |  |
  |  |                   |  |
  |  |                   |  |
  | .               x2,y2 . |
  |width              width |      
  x2,y2                  

*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void BrwGraphicsDrawTrampmfLine(int32            pos[4],     //x1, y1, x2, y2
                                       double           scale1,     //slope(x)
                                       double           scale2,     //slope(y)
                                       int32            width,      //line width
                                       uint16           color,      //rgb565
                                       BRW_TRAMPMF_LINE_DIRECT_E  direct,     //0:top, 1:bottom, 2:left, 3:right
                                       RECT_T           *region_ptr //layout rect
                                       );

/******************************************************************************************
* DESCRIPTION:
*   start a timer, called by layout
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   win_id: [IN] parent window handle
*   time_out: [IN] timeout value
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
uint32   BrwTimer_StartTimer(BRW_WIN_HANDLE win_id, uint32 timer_out);

/******************************************************************************************
* DESCRIPTION:
*   stop a timer, called by layout
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   timer_id: [IN] timer id
*
* RETURN:
*   TRUE if timer id is not invalid
*
* NOTE:
******************************************************************************************/
BOOLEAN  BrwTimer_StopTimer(uint32 timer_id);

//return invalid timer id macro
uint32   BrwTimer_InvalidTimerId(void);

//return downloading arrow width
uint16   BrwGetImageDownloadingWidth(void);

//return downloading arrow height
uint16   BrwGetImageDownloadingHeight(void);

//display downloading arrow
void     BrwDisplayDownloadingImage(RECT_T *rect_ptr, RECT_T *region_ptr, uint16 *alt_ptr);

//get downloading failed image widht
uint16   BrwGetImageFailedWidth(void);

//get downloading failed image height
uint16   BrwGetImageFailedHeight(void);

//display downloading failed image
void BrwDisplayFailedImage(RECT_T *rect_ptr, RECT_T *region_ptr, uint16 *alt_ptr);

/************************************************************************/
//  Description : calc image width and height
//  return: FALSE: image unknow or decode error; TRUE: success
/************************************************************************/
BOOLEAN  BrwImageCalc( uint8 *data_ptr,           //[IN] image data
                       uint32 data_len,           //[IN] image data length
                       BRW_MIME_TYPE_E mime_type, //[IN] image mime type
                       uint16 *width,             //[OUT]image width
                       uint16 *height);           //[OUT]image height

//display all format image in web page
BOOLEAN BrwImageDisplay(IMAGE_PLAY_INFO *play_info_ptr);

//draw focus on image
void BrwImageDisplayFocus(RECT_T *rect_ptr, int32 width, int32 height, RECT_T *region_ptr, BOOLEAN is_erase);


/******************************************************************************************
* DESCRIPTION:
*  set select result after user operation WML <select> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   select_ptr: [IN] WML <select> element handle
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void WmlSetSelectResult(WML_LAYOUT_SELECT_T *select_ptr);

/******************************************************************************************
* DESCRIPTION:
*   set the HTML <option> is checked
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   option_ptr: [IN] handle of HTML <option> element
*   is_checked: [IN] the <option> is checked or unchecked
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void XhtmlSetOptionChecked(XHTML_LAYOUT_T *option_ptr, BOOLEAN is_checked);

/******************************************************************************************
* DESCRIPTION:
*   when user done select operation, must call this function to save the result
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   select_ptr: [IN] handle of HTML <select> element
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void XhtmlSetSelectFinish(XHTML_LAYOUT_T *select_ptr);

/******************************************************************************************
* DESCRIPTION:
*   copy <option> value
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   option_ptr: [IN] handle of HTML <option> element
*   dst_pptr:   [OUT] <option> value
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void XhtmlCopyOptionContent(uint16 **dst_ptr, XHTML_LAYOUT_T *option_ptr);

/******************************************************************************************
* DESCRIPTION:
*   copy WML <option> content
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   option_ptr: [IN] handle of WML <option> element
*   dst_pptr:   [OUT] PCDATA of WML <option> content
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void WmlCopyOptionContent(uint16 **dst_ptr, WML_LAYOUT_OPTION_T *option_ptr);

/******************************************************************************************
* DESCRIPTION:
*   set the WML <option> is checked
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   option_ptr: [IN] handle of WML <option> element
*   is_checked: [IN] the <option> is checked or unchecked
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void WmlSetOptionChecked(WML_LAYOUT_OPTION_T *option_ptr, BOOLEAN is_checked);

/******************************************************************************************
* DESCRIPTION:
*   WML set value in <input> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] handle of WML <input> element
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void WmlSetInputValue(WML_LAYOUT_INPUT_T *input_ptr, const uint16 *value_ptr);

/******************************************************************************************
* DESCRIPTION:
*   HTML set input value in <input> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] handle of HTML <input> element
*   value_ptr: [IN] input value string
*
* RETURN:
*   title value
*
* NOTE:
******************************************************************************************/
void XhtmlSetInputValue(XHTML_LAYOUT_T *input_ptr, const uint16 *value_ptr);

/******************************************************************************************
* DESCRIPTION: xhtml compare value in <input> element with value_ptr
* AUTHOR:wenmei.hua
* PARAMETERS:
*   input_ptr: [IN] handle of xhtml <input> element
* RETURN:bool  
* NOTE:
******************************************************************************************/
BOOLEAN XhtmlCmpInputValue(XHTML_LAYOUT_T *input_ptr, const uint16 *value_ptr);

/******************************************************************************************
* DESCRIPTION:
*   detect if <option> is checked
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   option_ptr: [IN]   handle of WML <option> element
*
* RETURN:
*   TRUE: the <option> element is checked
*   FALSE: the <option > element is not checked
*
* NOTE:
******************************************************************************************/
BOOLEAN WmlOptionIsChecked(WML_LAYOUT_OPTION_T *option_ptr);

/******************************************************************************************
* DESCRIPTION:
*   detect if <select> element is multiple
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   select_ptr: [IN]   handle of WML <select> element
*
* RETURN:
*   TRUE: the <select> element is multiple
*   FALSE: the <select > element is single
*
* NOTE:
******************************************************************************************/
BOOLEAN WmlSelectIsMultiple(WML_LAYOUT_SELECT_T *select_ptr);

/******************************************************************************************
* DESCRIPTION:
*   detect if HTML <option> is checked
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   option_ptr: [IN] handle of HTML <option> element
*
* RETURN:
*   TRUE: the <option> element is checked
*   FALSE: the <option > element is not checked
*
* NOTE:
******************************************************************************************/
BOOLEAN XhtmlOptionIsChecked(XHTML_LAYOUT_T *option_ptr);

BOOLEAN XhtmlOptionIsDisabled(XHTML_LAYOUT_T *option_ptr);

/******************************************************************************************
* DESCRIPTION:
*   detect if <select> element is multiple
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   select_ptr: [IN]  handle of HTML <select> element
*
* RETURN:
*   TRUE: the <select> element is multiple
*   FALSE: the <select > element is single
*
* NOTE:
******************************************************************************************/
BOOLEAN XhtmlSelectIsMultiple(XHTML_LAYOUT_T *select_ptr);

/******************************************************************************************
* DESCRIPTION:
*   return TRUE if input is password type
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] handle of WML <input> element
*
* RETURN:
*   TRUE: if input is password type
*   FALSE: input is text type
*
* NOTE:
******************************************************************************************/
BOOLEAN WmlInputIsPasswordType(WML_LAYOUT_INPUT_T *input_ptr);
BOOLEAN WmlProcessOnpickEvent(WML_LAYOUT_OPTION_T *option_ptr, WML_LAYOUT_SELECT_T *select_ptr);

/******************************************************************************************
* DESCRIPTION:
*   get disable attribute in HTML input element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] handle of HTML <input> element
*
* RETURN:
*   return TRUE if HTML input is disable
*
* NOTE:
******************************************************************************************/
BOOLEAN XhtmlInputIsDisable(XHTML_LAYOUT_T *input_ptr);

/******************************************************************************************
* DESCRIPTION:
*   get read only attribute in HTML input element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] handle of HTML <input> element
*
* RETURN:
*   return TRUE if HTML input is read only
*
* NOTE:
******************************************************************************************/
BOOLEAN XhtmlInputIsReadOnly(XHTML_LAYOUT_T *input_ptr);

/******************************************************************************************
* DESCRIPTION:
*   return TRUE if HTML <input> element is password type
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] handle of HTML <input> element
*
* RETURN:
*   TRUE: if input is password type
*   FALSE: input is text type
*
* NOTE:
******************************************************************************************/
BOOLEAN XhtmlInputIsPasswordType(XHTML_LAYOUT_T *input_ptr);

/******************************************************************************************
* DESCRIPTION:
*   WML get format attribute in <input> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] handle of HTML <input> element
*
* RETURN:
*   WML input format string
*
* NOTE:
******************************************************************************************/
const uint8*  WmlGetInputFormat(WML_LAYOUT_INPUT_T *input_ptr);

/******************************************************************************************
* DESCRIPTION:
*   HTML get input format
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] handle of HTML <input> element
*
* RETURN:
*   format string, return NULL if no input format
*
* NOTE:
******************************************************************************************/
const uint8*  XhtmlGetInputFormat(XHTML_LAYOUT_T *input_ptr);

/******************************************************************************************
* DESCRIPTION:
*   WML get title attribute in <select> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] handle of WML <select> element
*
* RETURN:
*   title value
*
* NOTE:
******************************************************************************************/
const uint16* WmlGetSelectTitle(WML_LAYOUT_SELECT_T *select_ptr);

/******************************************************************************************
* DESCRIPTION:
*   get string value in WML <option> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   option_ptr: [IN]   handle of WML <option> element
*
* RETURN:
*   string value in WML <option> element
*
* NOTE:
******************************************************************************************/
const uint16* WmlGetOptionValue(WML_LAYOUT_OPTION_T *option_ptr);

/******************************************************************************************
* DESCRIPTION:
*   WML get title attribute in <input> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] handle of WML <input> element
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
const uint16* WmlGetInputTitle(WML_LAYOUT_INPUT_T *input_ptr);

/******************************************************************************************
* DESCRIPTION:
*   WML get value in <input> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] handle of WML <input> element
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
const uint16* WmlGetInputValue(WML_LAYOUT_INPUT_T *input_ptr);

/******************************************************************************************
* DESCRIPTION:
*   HTML get input value in <input> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] handle of HTML <input> element
*
* RETURN:
*   title value
*
* NOTE:
******************************************************************************************/
const uint16* XhtmlGetInputValue(XHTML_LAYOUT_T *input_ptr);

/******************************************************************************************
* DESCRIPTION:
*   get  <option>item number in HTML <select> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   select_ptr: [IN]   handle of HTML <select> element
*
* RETURN:
*   item number
*
* NOTE:
******************************************************************************************/
uint16 XhtmlGetSelectItemNum(XHTML_LAYOUT_T *select_ptr);

PUBLIC void XhtmlSetSelectNum(XHTML_LAYOUT_T *select_ptr, uint16 select_num);

/******************************************************************************************
* DESCRIPTION:
*   get  <option>item number in WML <select> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   select_ptr: [IN]   handle of WML <select> element
*
* RETURN:
*   item number
*
* NOTE:
******************************************************************************************/
uint16 WmlGetSelectItemNum(WML_LAYOUT_SELECT_T *select_ptr);

/******************************************************************************************
* DESCRIPTION:
*   WML get maxlength attribute in <input> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] handle of WML <input> element
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
uint32 WmlGetInputMaxLength(WML_LAYOUT_INPUT_T *input_ptr);

/******************************************************************************************
* DESCRIPTION:
*   HTML get maxlength attribute in <input> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   input_ptr: [IN] handle of HTML <input> element
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
uint32 XhtmlGetInputMaxLength(XHTML_LAYOUT_T *input_ptr);

/******************************************************************************************
* DESCRIPTION:
*   stop GIF playing
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle: [IN] GIF handle
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
void  BrwStopGif(uint32 handle);

/******************************************************************************************
* DESCRIPTION:
*    find <option> element in <select> element's children list
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   select_ptr:   [IN]  <select> element handle
*   index:        [IN]  <option> index, from zero.
*
* RETURN:
*   <option> element handle
* NOTE:
******************************************************************************************/
WML_LAYOUT_OPTION_T *WmlSelectFindOptionByIndex(WML_LAYOUT_SELECT_T *select_ptr, int32 index);

/******************************************************************************************
* DESCRIPTION:
*  find <option> element by a value in <option> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   select_ptr: [IN]  <selection> handle
*   value_ptr:  [IN]  value in <option> element
*
* RETURN:
*   option element handle
*
* NOTE:
******************************************************************************************/
WML_LAYOUT_OPTION_T *WmlSelectFindOptionByValue(WML_LAYOUT_SELECT_T *select_ptr, uint16 *value_ptr);

/******************************************************************************************
* DESCRIPTION:
*  find <option> under <select> by index in HTML web page
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   layout_ptr: [IN]  HTML layout handle
*   index:      [IN]   <option> index
*
* RETURN:
*   HTML layout handle for <option>
*
* NOTE:
******************************************************************************************/
XHTML_LAYOUT_T* XhtmlSelectFindOptionByIndex(XHTML_LAYOUT_T  *layout_ptr, int32 index);

/************************************************************************
* function:
      move scroll bar windows to new region
* input:
************************************************************************/
void  BrwMoveScrollBarWindow(void *scroll_handle, RECT_T *rect_ptr);
/************************************************************************
* function:
      set scroll bar new scroll info
* input:
      total_item_num: total item numbers in scroll bar
      page_item_num:  one page item numbers in scroll bar
************************************************************************/
void  BrwSetScrollBarInfo(void *scroll_handle, uint32 total_item_num, uint32 page_item_num, uint32 pos);

/******************************************************************************************
* DESCRIPTION:
*   Get string information in line with direction, only encapsulation of GUI function
*
* AUTHOR:
*   jiqun.li
*
* RETURN:
*   BOOLEAN
*
******************************************************************************************/
BOOLEAN BrwGetStringInfoInLineWithDir(WAP_STRING_FONT_TEXT_T str, int32 width, uint16 *char_num_ptr, uint16 *pixel_num_ptr, BOOLEAN is_byword);
/******************************************************************************************
* DESCRIPTION:
*   draw a thick line with color,style and width
*
* AUTHOR:
*   kelly.li
*
* PARAMETERS:
*   first_point[IN]     coordinate of first point
*   econd_point[IN]    coordinate of second point
*   color:[IN] color to draw
*   width:[IN]unit: pixel
*   style:[IN]line display style
*   region_ptr: [IN] web page display region
*
* RETURN:
*   void
*
* NOTE:
******************************************************************************************/
PUBLIC void BrwGraphicsDrawThickLine(
                                BRW_POINT_T first_point,
                                BRW_POINT_T second_point,                               
                                uint16      color,
                                int32       width,
                                BRW_PEN_STYLE_E style,
                                RECT_T          *region_ptr);


/*****************************************************************************/
//  Description : Play BG sound
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BrwPlayBGsound(BGSOUND_INFO *bgsound_ptr);

/*****************************************************************************/
//  Description : stop BG sound
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BrwStopBGsound(void);

/*****************************************************************************/
//  Description : set cookie adapter
//  Global resource dependence : none
//  Author: xianhe.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BrwSetCookieAdapter(uint32 http_context_id, uint8 *cookie_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
#endif
//end of file
