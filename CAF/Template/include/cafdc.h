/****************************************************************************
 ** File Name:      cafdc.h                                                 *
 ** Author:         Xiaoqing.Lu                                             *
 ** Date:           05/08/2008                                              *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:    This file is used to describe the application modual.   *
 ****************************************************************************
 **                         Important Edit History                          *
 ** ------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                             *
 ** 05/2008        Xiaoqing.Lu       Create
 ** 
 ****************************************************************************/

#ifndef  _CAFDC_H_    
#define  _CAFDC_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "cafhelp.h"
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

typedef struct IDC_T IDC_T;

// Font Type
typedef enum
{
    CAF_FONT_0,
    CAF_FONT_1,
    CAF_FONT_2,
    CAF_FONT_3,
    CAF_FONT_4,
    CAF_FONT_5,
    CAF_FONT_6,
    CAF_FONT_7,
	CAF_FONT_8,
    CAF_FONT_9,
	CAF_FONT_10,
    CAF_FONT_11,
	CAF_FONT_12,
    CAF_FONT_13,
	CAF_FONT_14,
    CAF_FONT_15,
	CAF_FONT_16,
    CAF_FONT_17,
	CAF_FONT_18,
    CAF_FONT_19,
    CAF_FONT_20,
    CAF_FONT_21,
    CAF_FONT_22,
    CAF_FONT_23,
    CAF_FONT_24,
    CAF_FONT_25,
    CAF_FONT_26,
    CAF_FONT_27,
    CAF_FONT_28,
	CAF_FONT_29,
    CAF_FONT_30,
    CAF_FONT_31,
    CAF_FONT_32,
    CAF_FONT_33,
    CAF_FONT_34,
    CAF_FONT_35,
    CAF_FONT_36,
    CAF_FONT_37,
    CAF_FONT_38,
    CAF_FONT_39,
    CAF_FONT_40,
    CAF_FONT_41,
    CAF_FONT_42,
    CAF_FONT_43,
    CAF_FONT_44,
    CAF_FONT_45,
    CAF_FONT_46,
    CAF_FONT_47,
    CAF_FONT_48,
    CAF_FONT_49,
    CAF_FONT_50,
    CAF_FONT_51,
    CAF_FONT_52,
    CAF_FONT_53,
    CAF_FONT_54,
    CAF_FONT_55,
    CAF_FONT_56,
    CAF_FONT_57,
    CAF_FONT_58,
    CAF_FONT_59,
    CAF_FONT_60,
    CAF_FONT_61,
    CAF_FONT_62,
    CAF_FONT_63,
    CAF_FONT_64,
	CAF_MAX_FONT_NUM
} CAF_FONT_TYPE_E;

// font style, reserved
typedef enum 
{
    CAF_FONT_STYLE_NONE,
}CAF_FONT_STYLE_E;

// font effect
typedef enum 
{
    CAF_FONT_EFFECT_NONE,
    CAF_FONT_EFFECT_REVERSE_EDGE,

}CAF_FONT_EFFECT_E;

// pen style, reserved
typedef enum 
{
    CAF_PEN_STYLE_NONE,
}CAF_PEN_STYLE_E;

typedef uint32 CAF_ALIGN_T;
#define CAF_ALIGN_LEFT                      0
#define CAF_ALIGN_RIGHT                     2
#define CAF_ALIGN_CENTER                    6

#define CAF_ALIGN_TOP                       0
#define CAF_ALIGN_BOTTOM                    8
#define CAF_ALIGN_BASELINE                  24

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
#ifdef WIN32
#define PACKED
#else
#define PACKED __packed    /* Byte alignment for communication structures.*/
#endif

typedef PACKED struct 	  /**** BMP file info structure ****/
{
	uint32	biSize; 		  /* Size of info header */
	uint32	biWidth;		  /* Width of image */
	uint32	biHeight;		  /* Height of image */
	uint16	biPlanes;		  /* Number of color planes */
	uint16	biBitCount; 	  /* Number of bits per pixel */
	uint32	biCompression;	  /* Type of compression to use */
	uint32	biSizeImage;	  /* Size of image data */
	uint32	biXPelsPerMeter;  /* X pixels per meter */
	uint32	biYPelsPerMeter;  /* Y pixels per meter */
	uint32	biClrUsed;		  /* Number of colors used */
	uint32	biClrImportant;   /* Number of important colors */
} CAFBITMAPINFOHEADER;

typedef PACKED struct 			 /**** Colormap entry structure ****/
{
	uint8  rgbBlue; 		 /* Blue value */
	uint8  rgbGreen;		 /* Green value */
	uint8  rgbRed;			 /* Red value */
	uint8  rgbReserved; 	 /* Reserved */
} CAFRGBQUAD;

typedef PACKED struct 						/**** Bitmap information structure ****/
{
	CAFBITMAPINFOHEADER bmiHeader; 				/* Image header */
	CAFRGBQUAD 		 bmiColors[256];	/* Image colormap */
} CAFBITMAPINFO;

/*! 
 *  \brief define color type
 */
typedef uint16                  CAF_COLOR_T;
typedef uint32                  CAF_TEXT_FORMAT;

// Inherit the base class of the IUNKNOWN
CAF_INTERFACE(IDC_T)
{
    INHERIT_IUNKNOWN(IDC_T);

    // set param
    uint32           (*SetLcdId)         ( IDC_T *me, uint32 lcd_id );
    uint32           (*SetBlockId)       ( IDC_T *me, uint32 block_id );

    void             (*SetOrigin)        ( IDC_T *me, const CAF_POINT_T* point_ptr );
    void             (*SetRect)          ( IDC_T *me, const CAF_RECT_T* rect_ptr );

    // about font
    CAF_FONT_TYPE_E   (*SetFontType)      ( IDC_T *me, CAF_FONT_TYPE_E type );
    CAF_FONT_TYPE_E   (*GetFontType)      ( IDC_T *me );
    CAF_FONT_STYLE_E  (*SetFontStyle)     ( IDC_T *me, CAF_FONT_STYLE_E type ); //reserved
    CAF_FONT_STYLE_E  (*GetFontStype)     ( IDC_T *me );                       //reserved
    CAF_FONT_EFFECT_E (*SetFontEffect)    ( IDC_T *me, CAF_FONT_EFFECT_E effect );
    CAF_FONT_EFFECT_E (*GetFontEffect)    ( IDC_T *me );
    CAF_COLOR_T      (*SetFontColor)     ( IDC_T *me, CAF_COLOR_T color );
    CAF_COLOR_T      (*GetFontColor)     ( IDC_T *me );
    
    CAF_ALIGN_T      (*SetTextAlign)     ( IDC_T *me, CAF_ALIGN_T align );
    CAF_ALIGN_T      (*GetTextAlign)     ( IDC_T *me );

    CAF_PEN_STYLE_E   (*SetPenStyle)      ( IDC_T *me, CAF_PEN_STYLE_E type );  //reserved
    CAF_PEN_STYLE_E   (*GetPenStype)      ( IDC_T *me );                       //reserved
    uint16           (*SetPenSize)       ( IDC_T *me, uint16 size );
    uint16           (*GetPenSize)       ( IDC_T *me );
    CAF_COLOR_T      (*SetPenColor)      ( IDC_T *me, CAF_COLOR_T color );
    CAF_COLOR_T      (*GetPenColor)      ( IDC_T *me );

    CAF_COLOR_T      (*SetPixel)         ( IDC_T *me, const CAF_POINT_T *point_ptr, CAF_COLOR_T color );
    CAF_COLOR_T      (*GetPixel)         ( IDC_T *me, const CAF_POINT_T *point_ptr );

    void             (*DrawLine)         ( IDC_T *me, const CAF_POINT_T* start_ptr, const CAF_POINT_T* end_ptr );
    void             (*DrawRect)         ( IDC_T *me, const CAF_RECT_T* rect_ptr );
    void             (*FillRect)         ( IDC_T *me, const CAF_RECT_T* rect_ptr );

    // draw text
    void             (*DrawText)         ( IDC_T *me, const CAF_STRING_T* str_ptr, const CAF_RECT_T* rect_ptr, CAF_TEXT_FORMAT format );
    void             (*DrawTextId)       ( IDC_T *me, uint32 text_id, CAF_HANDLE_T owner_handle, const CAF_RECT_T* rect_ptr, CAF_TEXT_FORMAT format );

    // draw image
    void             (*DrawImage)        ( IDC_T *me, const uint8 *data_ptr, uint32 data_size, const CAF_POINT_T *point_ptr );
    void             (*DrawImageId)      ( IDC_T *me, uint32 image_id, CAF_HANDLE_T owner_handle, const CAF_POINT_T *point_ptr );
    void             (*DrawImageInRect)  ( IDC_T *me, const uint8 *data_ptr, uint32 data_size, const CAF_RECT_T *image_rect_ptr, const CAF_RECT_T *disp_rect_ptr );
    void             (*DrawImageIdInRect)( IDC_T *me, uint32 image_id, CAF_HANDLE_T owner_handle, const CAF_RECT_T *image_rect_ptr, const CAF_RECT_T *disp_rect_ptr );

    void             (*MistBmp)          ( IDC_T *me, const CAF_RECT_T* rect_ptr, CAF_COLOR_T color ); //temp function for game link only!!!

    BOOLEAN          (*InvalidateRect)   ( IDC_T *me, const CAF_RECT_T* rect_ptr );

//add by songliang.liu
	BOOLEAN          (*Update)   		 	( IDC_T *me );
	uint16 			 (*GetFontWidth)	 	( IDC_T *me, CAF_FONT_TYPE_E font_type ,uint16 char_unicode );
	uint16 			 (*GetFontHeight)	 	( IDC_T *me, CAF_FONT_TYPE_E font_type ,uint16 char_unicode);
	uint16 			 (*GetStringWidth)		( IDC_T *me, CAF_FONT_TYPE_E font_type, CAF_STRING_T *str_ptr );
	uint16 			 (*GetStringHeight)		( IDC_T *me, CAF_FONT_TYPE_E font_type, CAF_STRING_T *str_ptr );

	int 			(*SetDIBitsToDevice)(
							IDC_T *me,				 // handle to DC
							int XDest,				 // x-coord of destination upper-left corner
							int YDest,				 // y-coord of destination upper-left corner 
							uint32 dwWidth, 		 // source rectangle width
							uint32 dwHeight,		 // source rectangle height
							int XSrc,				 // x-coord of source lower-left corner
							int YSrc,				 // y-coord of source lower-left corner
							uint32 uStartScan,		 // first scan line in array
							uint32 cScanLines,		 // number of scan lines
							const void *lpvBits,	 // array of DIB bits
							const CAFBITMAPINFO *lpbmi, // bitmap information
							uint32 fuColorUse		 // RGB or palette indexes
							);
	
};


// Define pointer to poit the virtual table of the control base class
struct IDC_T
{
    CAF_VTBL_T(IDC_T)  *vtbl_ptr;
};

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

static __inline CAF_RESULT_E IDC_QueryInterface( IDC_T *me, CAF_GUID_T guid, void ** object_pptr ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->QueryInterface( me, guid, object_pptr );
}

static __inline uint32 IDC_AddRef( IDC_T *me ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->AddRef( me );
}

static __inline uint32 IDC_Release( IDC_T *me ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->Release( me );
}

static __inline uint32 IDC_SetLcdId( IDC_T *me, uint32 lcd_id )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->SetLcdId( me, lcd_id );
}

static __inline uint32 IDC_SetBlockId( IDC_T *me, uint32 block_id )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->SetBlockId( me, block_id );
}

static __inline void IDC_SetOrigin( IDC_T *me, const CAF_POINT_T* point_ptr )
{
    CAF_ASSERT( PNULL != me );
    CAF_GET_VTBLPTR( me, IDC_T )->SetOrigin( me, point_ptr );
}

static __inline void IDC_SetRect( IDC_T *me, const CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me );
    CAF_GET_VTBLPTR( me, IDC_T )->SetRect( me, rect_ptr );
}

static __inline CAF_FONT_TYPE_E IDC_SetFontType( IDC_T *me, CAF_FONT_TYPE_E type )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->SetFontType( me, type );
}

static __inline CAF_FONT_TYPE_E IDC_GetFontType( IDC_T *me )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->GetFontType( me );
}

static __inline CAF_FONT_EFFECT_E IDC_SetFontEffect( IDC_T *me, CAF_FONT_EFFECT_E effect )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->SetFontEffect( me, effect );
}

static __inline CAF_FONT_EFFECT_E IDC_GetFontEffect( IDC_T *me )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->GetFontEffect( me );
}

static __inline CAF_COLOR_T IDC_SetFontColor( IDC_T *me, CAF_COLOR_T color )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->SetFontColor( me, color );
}

static __inline CAF_COLOR_T IDC_GetFontColor( IDC_T *me )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->GetFontColor( me );
}

static __inline CAF_ALIGN_T IDC_SetTextAlign( IDC_T *me, CAF_ALIGN_T align )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->SetTextAlign( me, align );
}

static __inline CAF_ALIGN_T IDC_GetTextAlign( IDC_T *me )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->GetTextAlign( me );
}

static __inline uint16 IDC_SetPenSize( IDC_T *me, uint16 size )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->SetPenSize( me, size );
}

static __inline uint16 IDC_GetPenSize( IDC_T *me )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->GetPenSize( me );
}

static __inline CAF_COLOR_T IDC_SetPenColor( IDC_T *me, CAF_COLOR_T color )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->SetPenColor( me, color );
}

static __inline CAF_COLOR_T IDC_GetPenColor( IDC_T *me )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->GetPenColor( me );
}

static __inline CAF_COLOR_T IDC_SetPixel( IDC_T *me, const CAF_POINT_T *point_ptr, CAF_COLOR_T color )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->SetPixel( me, point_ptr, color );
}

static __inline CAF_COLOR_T IDC_GetPixel( IDC_T *me, const CAF_POINT_T *point_ptr )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->GetPixel( me, point_ptr );
}

static __inline void IDC_DrawLine( IDC_T *me, const CAF_POINT_T* start_ptr, const CAF_POINT_T* end_ptr )
{
    CAF_ASSERT( PNULL != me );
    CAF_GET_VTBLPTR( me, IDC_T )->DrawLine( me, start_ptr, end_ptr );
}

static __inline void IDC_DrawRect( IDC_T *me, const CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me );
    CAF_GET_VTBLPTR( me, IDC_T )->DrawRect( me, rect_ptr );
}

static __inline void IDC_FillRect( IDC_T *me, const CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me );
    CAF_GET_VTBLPTR( me, IDC_T )->FillRect( me, rect_ptr );
}

static __inline void IDC_DrawText( IDC_T *me, const CAF_STRING_T* str_ptr, const CAF_RECT_T* rect_ptr, CAF_TEXT_FORMAT format )
{
    CAF_ASSERT( PNULL != me );
    CAF_GET_VTBLPTR( me, IDC_T )->DrawText( me, str_ptr, rect_ptr, format );
}

static __inline void IDC_DrawTextId( IDC_T *me, uint32 text_id, CAF_HANDLE_T owner_handle, const CAF_RECT_T* rect_ptr, CAF_TEXT_FORMAT format )
{
    CAF_ASSERT( PNULL != me );
    CAF_GET_VTBLPTR( me, IDC_T )->DrawTextId( me, text_id, owner_handle, rect_ptr, format );
}

static __inline void IDC_DrawImage( IDC_T *me, const uint8 *data_ptr, uint32 data_size, const CAF_POINT_T *point_ptr )
{
    CAF_ASSERT( PNULL != me );
    CAF_GET_VTBLPTR( me, IDC_T )->DrawImage( me, data_ptr, data_size, point_ptr );
}

static __inline void IDC_DrawImageId( IDC_T *me, uint32 image_id, CAF_HANDLE_T owner_handle, const CAF_POINT_T *point_ptr )
{
    CAF_ASSERT( PNULL != me );
    CAF_GET_VTBLPTR( me, IDC_T )->DrawImageId( me, image_id, owner_handle, point_ptr );
}

static __inline void IDC_DrawImageInRect( IDC_T *me, const uint8 *data_ptr, uint32 data_size, const CAF_RECT_T *image_rect_ptr, const CAF_RECT_T *disp_rect_ptr )
{
    CAF_ASSERT( PNULL != me );
    CAF_GET_VTBLPTR( me, IDC_T )->DrawImageInRect( me, data_ptr, data_size, image_rect_ptr, disp_rect_ptr );
}

static __inline void IDC_DrawImageIdInRect( IDC_T *me, uint32 image_id, CAF_HANDLE_T owner_handle, const CAF_RECT_T *image_rect_ptr, const CAF_RECT_T *disp_rect_ptr )
{
    CAF_ASSERT( PNULL != me );
    CAF_GET_VTBLPTR( me, IDC_T )->DrawImageIdInRect( me, image_id, owner_handle, image_rect_ptr, disp_rect_ptr );
}

static __inline void IDC_TransparentBmp( IDC_T *me, const CAF_RECT_T* rect_ptr, CAF_COLOR_T color )
{
    CAF_ASSERT( PNULL != me );
    CAF_GET_VTBLPTR( me, IDC_T )->MistBmp( me, rect_ptr, color );
}

static __inline BOOLEAN IDC_InvalidateRect( IDC_T *me, const CAF_RECT_T* rect_ptr )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->InvalidateRect( me, rect_ptr );
}



//add by songliang.liu

static __inline BOOLEAN IDC_Update( IDC_T *me )
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->Update( me );
}

static __inline uint16	IDC_GetFontWidth		( IDC_T *me, CAF_FONT_TYPE_E font_type ,uint16 char_unicode)
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IDC_T )->GetFontWidth( me, font_type ,char_unicode);
}

static __inline uint16	IDC_GetFontHeight		( IDC_T *me, CAF_FONT_TYPE_E font_type,uint16 char_unicode )
{
		CAF_ASSERT( PNULL != me );
		return CAF_GET_VTBLPTR( me, IDC_T )->GetFontHeight( me, font_type ,char_unicode);
}
	
static __inline uint16	IDC_GetStringWidth	( IDC_T *me, CAF_FONT_TYPE_E font_type, CAF_STRING_T *str_ptr )
{
		CAF_ASSERT( PNULL != me );
		return CAF_GET_VTBLPTR( me, IDC_T )->GetStringWidth( me, font_type ,str_ptr);
}
static __inline uint16	IDC_GetStringHeight ( IDC_T *me, CAF_FONT_TYPE_E font_type, CAF_STRING_T *str_ptr )
{
		CAF_ASSERT( PNULL != me );
		return CAF_GET_VTBLPTR( me, IDC_T )->GetStringHeight( me, font_type ,str_ptr);
}	
//add by RiancyZhang 2009-04-21
/*=============================================================
Name : IDC_SetDIBitsToDevice
Function : The function sets the pixels in the specified rectangle
		   on the device that is associated with the destination 
		   device context using color data from a device-independent
		   bitmap (DIB). 
Params:	
		IDC_T *me[IN] ---  handle to DC
		int XDest[IN] ---  x-coord of destination upper-left corner
		int YDest[IN] ---  y-coord of destination upper-left corner
		uint32 dwWidth[IN] ---  source rectangle width
		uint32 dwHeight[IN] ---  source rectangle height
		int XSrc[IN] ---  x-coord of source lower-left corner
		int YSrc[IN] --- y-coord of source lower-left corner
		uint32 uStartScan[IN] ---  first scan line in array
		uint32 cScanLines[IN] ---  number of scan lines
		const void *lpvBits[IN] ---  array of DIB bits
		const CAFBITMAPINFO *lpbmi[IN] --- bitmap information
		uint32 fuColorUse[IN] --- RGB or palette indexes
			Value			Meaning 
			DIB_PAL_COLORS :The bmiColors member is an array of 16-bit 
						indexes into the logical palette of the device
						context specified by hdc. 
			DIB_RGB_COLORS :The BITMAPINFO structure contains an array of
						literal RGB values.
Return : the number of scan lines set: succeeds
		0 : fail
=============================================================*/

static __inline int	IDC_SetDIBitsToDevice(
	IDC_T *me,				 // handle to DC
	int XDest,				 // x-coord of destination upper-left corner
	int YDest,				 // y-coord of destination upper-left corner 
	uint32 dwWidth,			 // source rectangle width
	uint32 dwHeight, 		 // source rectangle height
	int XSrc,				 // x-coord of source lower-left corner
	int YSrc,				 // y-coord of source lower-left corner
	uint32 uStartScan,		 // first scan line in array
	uint32 cScanLines,		 // number of scan lines
	const void *lpvBits,	 // array of DIB bits
	const CAFBITMAPINFO *lpbmi, // bitmap information
	uint32 fuColorUse 		 // RGB or palette indexes
	)
{
		CAF_ASSERT( PNULL != me );
		return CAF_GET_VTBLPTR( me, IDC_T )->SetDIBitsToDevice(
			me,				 // handle to DC
			XDest,				 // x-coord of destination upper-left corner
			YDest,				 // y-coord of destination upper-left corner 
			dwWidth,			 // source rectangle width
			dwHeight, 		 // source rectangle height
			XSrc,				 // x-coord of source lower-left corner
			YSrc,				 // y-coord of source lower-left corner
			uStartScan,		 // first scan line in array
			cScanLines,		 // number of scan lines
			lpvBits,	 // array of DIB bits
			lpbmi, // bitmap information
			fuColorUse 		 // RGB or palette indexes
			);
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif //_CAFDIAPLAY_H_
