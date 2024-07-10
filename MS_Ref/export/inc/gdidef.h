/******************************************************************************
 ** File Name:      GDIDEF.h                                                  *
 ** Author:         Annie.Chen                                                *
 ** DATE:           11/21/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file include macro definitions and typedefs          *
 **                 that used by GDI function.                                *                                                         *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/21/2002     Annie.Chen       Create.                                   *
 ******************************************************************************/

#ifndef _GDI_DEF_H
#define _GDI_DEF_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sci_api.h"

 /**---------------------------------------------------------------------------*
 **                         Compiler Flag                                      *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Handle definitions                                *
 **---------------------------------------------------------------------------*/
typedef uint32 GDI_HWND;
typedef uint32 GDI_HDC;

typedef uint32 GDI_HPEN;
typedef uint32 GDI_HBRUSH;
typedef uint32 GDI_HBITMAP;
typedef uint32 GDI_HFONT;
typedef uint32 GDI_HGDIOBJ;

typedef uint32 GDI_HICON;
typedef uint32 GDI_HMENU;
typedef uint32 GDI_HCURSOR;

#define GDI_GetRValue(rgb)      ((BYTE)(rgb))
#define GDI_GetGValue(rgb)      ((BYTE)(((WORD)(rgb)) >> 8))
#define GDI_GetBValue(rgb)      ((BYTE)((rgb)>>16))
 
#define GDI_INVALID             0xFFFFFFFF

/* 
 * COLOR defination
 */
#define LCD_COLOR_BLACK 0x0
#define LCD_COLOR_WHITE 0xFF

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
/*
 * COLOR definition
 */
typedef uint32 GDI_COLOR;

typedef struct GDI_RECT_tag
{
    uint32   	left;
    uint32    	top;
    uint32    	right;
    uint32    	bottom;
} GDI_RECT, *PGDI_RECT;

typedef struct GDI_POINT_tag
{
    uint32 		x;
    uint32 		y;
} GDI_POINT, *PGDI_POINT;

typedef struct GDI_SIZE_tag
{                        
    uint32 		cx;              
    uint32 		cy;              
} GDI_SIZE, *PGDI_SIZE;                  
           
typedef struct GDI_RGB_tag         
{                        
   uint8 		r;               
   uint8 		g;               
   uint8 		b; 
   uint8        reserve;
}GDI_RGB, *PGDI_RGB;                    

/*
    @Lin.liu(2002-11-25). 
    Added Font Support.
*/
// Font stype
enum 
{
	FNT_NORMAL			= 0x0,
	FNT_ITALIC			= 0x1,
	FNT_BOLD			= 0x2,
	FNT_UNDERLINE		= 0x4
};

// Charset
enum 
{
	CHARSET_DEFAULT		= 0x0,
    CHARSET_ANSI		= 0x1,
	CHARSET_GB2312_80	= 0x2,
	CHARSET_UCS2		= 0x3
};

typedef struct GDI_LOGFONT_tag
{
    uint16      style;              // any combination of FNT_ITALIC, FNT_BOLD, FNT_UNDERLINE
    uint16      width;
    uint16      height;
    uint16      charset;            // one of GB2312-80, ANSI, UTF-16(UCS2), UTF-32(UCS4) etc.
                                    // Default: GB2312-80
    char       *face_name;          // typeface name
        
    struct     FONT_OPS_tag  *ops;  // ops function.
}GDI_LOGFONT, *PGDI_LOGFONT;

typedef struct FONT_OPS_tag
{
    // Get char in the char string
    uint32 (*CharAt)(const uint8 *str, uint index);
    // Get next char in the char string
    uint32 (*GetNextChar)(const uint8 *str, uint8 **next_ptr_ptr);
    // Get the width of the given char.
    uint   (*GetCharacteWidth)(const  GDI_LOGFONT *font, uint32 wch);
    // Get height of the given char.
    uint   (*GetCharacteHeight)(const  GDI_LOGFONT *font, uint32 wch);
    // Get width of the given string.
    uint   (*GetStringWidth)(const GDI_LOGFONT *font, const uint8 *str, uint len);
    // Get the bitmap of the given char.
    // and return monochrome bitmap data.
    const uint8 *(*GetFontBitmap)(const GDI_LOGFONT *font, uint32 wch ); 
}FONT_OPS;
/*
    @ End Added By Lin.liu
*/                        

#undef	BI_RGB
#define BI_RGB			0
#undef	BI_RLE4
#define BI_RLE4			1
#undef	BI_RLE8
#define BI_RLE8			2
#undef	BI_BITFIELDS
#define BI_BITFIELDS	3
#define BMP_HEADERSIZE	0x36

typedef struct BITMAP_tag
{
    uint32     	bmType;           // Specifies the bitmap type;
    uint32     	bmWidth;          // Specifies the width, in pixels, of the bitmap
    uint32     	bmHeight;         // Specifies the height, in pixels, of the bitmap
    uint32    	bmWidthBytes;     // Specifies the number of bytes in each scan line.
    uint16    	bmPlanes;         // Specifies the count of color planes.
    uint16    	bmBitsPixel;      // Specifies the number of bits required to the color of a pixel.
    void*   	bmBits;           // Pointer to the location of the bit values for the bitmap.
}GDI_BITMAP, *PGDI_BITMAP;

typedef struct BMPHANDLE_tag
{
	uint32          mode;
	GDI_BITMAP      bmp;
}GDI_BMPHANDLE, *PGDI_BMPHANDLE;

/*
 *  Bitmap header is 16bits alignment;
 */
//#pragma pack(push, 2)
typedef struct BMPHeader_tag
{
	uint16	identifier;
	uint32	filesize;
	uint32	reserved;
	uint32	dataoffset;
	uint32	headersize;
	uint32	width;
	uint32	height;
	uint16	planes;
	uint16	bpp;
	uint32	compression;
	uint32	datasize;
	uint32	hres;
	uint32	vres;
	uint32	colors;
	uint32	importantcolors;
}BMPHeader,*PBMPHeader;

//#pragma pack( pop )

/*
 * General drawing attributes
 */
#define TRANSPARENT_MODE        1
#define OPAQUE_MODE             0

/* 
 * Display device flags
 */
#define PHYSICAL_DDMODE         0x0001  /* global device*/
#define MEMORY_DDMODE           0x0002  /* memory device*/

/* 
 * Drawing modes
 */
#define MODE_SET                0       /* draw pixels as given (default) */
#define MODE_XOR                1       /* draw pixels using XOR */
#define MODE_OR                 2       /* draw pixels using OR (notimp)*/
#define MODE_AND                3       /* draw pixels using AND (notimp)*/

/*
 * Color definitions
 */
#define COLOR_lightwhite        0
#define COLOR_black             1

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // End of _GDIDEF_H

