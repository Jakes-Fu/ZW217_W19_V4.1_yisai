#ifdef DYNAMIC_MODULE_SUPPORT
/****************************************************************************
** File Name:      guidc.c                                                 *
** Author:         Xiaoqing.Lu                                             *
** Date:           05/30/2008                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2008        Xiaoqing.Lu       Create
** 
****************************************************************************/
#define _CAFCTRLMODULE_C


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "caf.h"
#include "cafmodule.h"
#include "cafmodule_gen.h"
#include "cafdc.h"
#include "guidc.h"
#include "guifont.h"
#include "guiblock.h"
#include "guires.h"
#include "guilcd.h"
#include "guiimg.h"
#include "guistring.h"
#include "mmi_theme.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

// font information
typedef struct _guidc_font_info_t
{
    CAF_FONT_TYPE_E      type;
    CAF_FONT_EFFECT_E    effect;
    CAF_ALIGN_T          align;
    CAF_COLOR_T          color;
}CAFDC_FONT_INFO_T;

// pen information
typedef struct 
{
    uint16             size;
    CAF_COLOR_T        color;
    
} CAFDC_PENINFO_T;
        
typedef struct
{
    CAF_VTBL_T(IDC_T)  *vtbl_ptr; // 函数列表
    
    CAF_GUID_T         guid;
    uint32             ref_num;
   
    CAF_LCD_DEV_INFO   lcd_dev_info;

    CAF_POINT_T        origin;
    
    CAF_RECT_T         rect;
    
    CAFDC_FONT_INFO_T  font_info;
    CAFDC_PENINFO_T    pen_info;
    
}GUIDC_CTRL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : dc query interface
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E GUIDC_QueryInterface( 
                                        IDC_T *idc_ptr, 
                                        CAF_GUID_T guid, 
                                        void** ojbect_pptr 
                                        );

/*****************************************************************************/
// 	Description : gui dc add ref
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 GUIDC_AddRef(
                          IDC_T * idc_ptr 
                          );

/*****************************************************************************/
// 	Description : gui dc release
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 GUIDC_Release(
                           IDC_T * idc_ptr
                           );

/*****************************************************************************/
// 	Description : set lcd id
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 GUIDC_SetLcdId(
                            IDC_T *idc_ptr, 
                            uint32 lcd_id
                            );

/*****************************************************************************/
// 	Description : set block id
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 GUIDC_SetBlockId(
                              IDC_T *idc_ptr, 
                              uint32 block_id
                              );

/*****************************************************************************/
// 	Description : set origin rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_SetOrigin(
                           IDC_T *idc_ptr, 
                           const GUI_POINT_T* point_ptr
                           );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_SetRect(
                         IDC_T *idc_ptr, 
                         const GUI_RECT_T* rect_ptr
                         );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
/*lint -e728*/
LOCAL CAF_FONT_TYPE_E GUIDC_SetFontType(
                                        IDC_T *idc_ptr,
                                        CAF_FONT_TYPE_E type
                                        );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL CAF_FONT_TYPE_E GUIDC_GetFontType(
                                        IDC_T *idc_ptr
                                        );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/       
LOCAL CAF_FONT_EFFECT_E GUIDC_SetFontEffect( 
                                           IDC_T *idc_ptr,
                                           CAF_FONT_EFFECT_E effect
                                           );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL CAF_FONT_EFFECT_E GUIDC_GetFontEffect(
                                           IDC_T *idc_ptr
                                           );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/                 
LOCAL GUI_COLOR_T GUIDC_SetFontColor(
                                     IDC_T *idc_ptr,
                                     GUI_COLOR_T color
                                     );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/   
LOCAL GUI_COLOR_T GUIDC_GetFontColor(
                                     IDC_T *idc_ptr
                                     );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL CAF_ALIGN_T GUIDC_SetTextAlign( 
                                     IDC_T *idc_ptr,
                                     CAF_ALIGN_T align
                                     );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL CAF_ALIGN_T GUIDC_GetTextAlign(
                                     IDC_T *idc_ptr
                                     );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
// LOCAL GUI_ANGLE_E GUIDC_SetTextAngle(
//                                      IDC_T *idc_ptr,
//                                      GUI_ANGLE_E angle
//                                      );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
// LOCAL GUI_ANGLE_E GUIDC_GetTextAngle(
//                                      IDC_T *idc_ptr
//                                      );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL uint16 GUIDC_SetPenSize( 
                              IDC_T *idc_ptr,
                              uint16 size
                              );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL uint16 GUIDC_GetPenSize(
                              IDC_T *idc_ptr
                              );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL GUI_COLOR_T GUIDC_SetPenColor(
                                    IDC_T *idc_ptr,
                                    GUI_COLOR_T color
                                    );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL GUI_COLOR_T GUIDC_GetPenColor(
                                    IDC_T *idc_ptr
                                    );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL GUI_COLOR_T GUIDC_SetPixel( 
                                 IDC_T *idc_ptr,
                                 const GUI_POINT_T *point_ptr,
                                 GUI_COLOR_T color
                                 );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL GUI_COLOR_T GUIDC_GetPixel( 
                                 IDC_T *idc_ptr, 
                                 const GUI_POINT_T *point_ptr
                                 );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL void GUIDC_DrawLine( 
                          IDC_T *idc_ptr,
                          const GUI_POINT_T* start_ptr,
                          const GUI_POINT_T* end_ptr
                          );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL void GUIDC_DrawRect( 
                          IDC_T *idc_ptr,
                          const GUI_RECT_T* rect_ptr
                          );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL void GUIDC_FillRect( 
                          IDC_T *idc_ptr,
                          const GUI_RECT_T* rect_ptr
                          );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_DrawText(
                          IDC_T *idc_ptr,
                          const MMI_STRING_T* str_ptr,
                          const GUI_RECT_T* rect_ptr,
                          CAF_TEXT_FORMAT format
                          );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_DrawTextId(
                            IDC_T *idc_ptr,
                            MMI_TEXT_ID_T text_id, 
                            CAF_HANDLE_T owner_handle,
                            const GUI_RECT_T* rect_ptr,
                            CAF_TEXT_FORMAT format
                            );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_DrawImage( 
                           IDC_T                *idc_ptr,
                           const uint8          *data_ptr,
                           uint32               data_size,
                           const GUI_POINT_T    *point_ptr
                           );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_DrawImageId( 
                           IDC_T *idc_ptr,
                           MMI_IMAGE_ID_T image_id,
                           CAF_HANDLE_T owner_handle,
                           const GUI_POINT_T *point_ptr
                           );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_DrawImageInRect( 
                                 IDC_T              *idc_ptr,
                                 const uint8        *data_ptr,
                                 uint32             data_size,
                                 const GUI_RECT_T   *image_rect_ptr,
                                 const GUI_RECT_T   *disp_rect_ptr
                                 );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_DrawImageIdInRect( 
                                   IDC_T *idc_ptr,
                                   MMI_IMAGE_ID_T image_id,
                                   CAF_HANDLE_T owner_handle,
                                   const GUI_RECT_T *image_rect_ptr,
                                   const GUI_RECT_T *disp_rect_ptr
                                   );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_MistBmp(
                         IDC_T *idc_ptr,
                         const GUI_RECT_T* rect_ptr,
                         GUI_COLOR_T color
                         );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GUIDC_InvalidateRect( 
                                   IDC_T *idc_ptr,
                                   const GUI_RECT_T* rect_ptr
                                   );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void GetAbsoluteRect(
                          GUIDC_CTRL_T *dc_ptr,
                          const GUI_RECT_T* src_ptr,
                          GUI_RECT_T* dst_ptr
                          );

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void GetAbsolutePoint(
                            GUIDC_CTRL_T *dc_ptr,
                            const GUI_POINT_T* src_ptr,
                            GUI_POINT_T* dst_ptr
                            );

#ifdef DYNAMIC_MODULE_SUPPORT    
/*=============================================================
Name : GUIDC_SetDIBitsToDevice
Function : The function sets the pixels in the specified rectangle
		   on the device that is associated with the destination 
		   device context using color data from a device-independent
		   bitmap (DIB). 
Params:	
		IDC_T * dc_ptr[IN] ---  handle to DC
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
		UINT fuColorUse[IN] --- RGB or palette indexes
			Value			Meaning 
			DIB_PAL_COLORS :The bmiColors member is an array of 16-bit 
						indexes into the logical palette of the device
						context specified by hdc. 
			DIB_RGB_COLORS :The BITMAPINFO structure contains an array of
						literal RGB values.
Return : the number of scan lines set: succeeds
		0 : fail
=============================================================*/
LOCAL int GUIDC_SetDIBitsToDevice(
									IDC_T *dc_ptr,				 // handle to DC
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

// add by songliang.liu

LOCAL BOOLEAN  GUIDC_Update ( IDC_T *dc_ptr );
LOCAL uint16 GUIDC_GetFontWidth ( IDC_T *dc_ptr, CAF_FONT_TYPE_E font_type ,uint16 char_unicode);
LOCAL uint16 GUIDC_GetFontHeight ( IDC_T *dc_ptr, CAF_FONT_TYPE_E font_type,uint16 char_unicode );
LOCAL uint16 GUIDC_GetStringWidth ( IDC_T *dc_ptr, CAF_FONT_TYPE_E font_type, CAF_STRING_T *str_ptr );
LOCAL uint16 GUIDC_GetStringHeight ( IDC_T *dc_ptr, CAF_FONT_TYPE_E font_type, CAF_STRING_T *str_ptr );

#endif

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
// functions table
LOCAL const CAF_VTBL_T(IDC_T) s_guidc_vtbl =
{
    GUIDC_QueryInterface,
    GUIDC_AddRef,
    GUIDC_Release,
    GUIDC_SetLcdId,
    GUIDC_SetBlockId,
    GUIDC_SetOrigin,
    GUIDC_SetRect,

    GUIDC_SetFontType,
    GUIDC_GetFontType,
    PNULL,//reserved
    PNULL,//reserved
    GUIDC_SetFontEffect,
    GUIDC_GetFontEffect,
    GUIDC_SetFontColor,
    GUIDC_GetFontColor,
    GUIDC_SetTextAlign,
    GUIDC_GetTextAlign,
    //GUIDC_SetTextAngle,
    //GUIDC_GetTextAngle,

    PNULL,//reserved
    PNULL,//reserved
    GUIDC_SetPenSize,
    GUIDC_GetPenSize,
    GUIDC_SetPenColor,
    GUIDC_GetPenColor,

    GUIDC_SetPixel,
    GUIDC_GetPixel,

    GUIDC_DrawLine,
    GUIDC_DrawRect,
    GUIDC_FillRect,

    GUIDC_DrawText,
    GUIDC_DrawTextId,
    GUIDC_DrawImage,
    GUIDC_DrawImageId,
    GUIDC_DrawImageInRect,
    GUIDC_DrawImageIdInRect,

    GUIDC_MistBmp,
    GUIDC_InvalidateRect,

#ifdef DYNAMIC_MODULE_SUPPORT    
	//add by songliang.liu
	GUIDC_Update,
	GUIDC_GetFontWidth,
	GUIDC_GetFontHeight,
	GUIDC_GetStringWidth,
	GUIDC_GetStringHeight,
	//add by RiancyZhang 2009-04-21
	GUIDC_SetDIBitsToDevice,
#else
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,    
    PNULL
#endif
};

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : new gui control
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E GUIDC_ClassNew( 
                                  CAF_GUID_T guid, 
                                  void**     object_pptr 
                                  );

LOCAL const CAF_STATIC_CLASS_T s_guidc_class_list[] = 
{
    { SPRD_GUID_DC,      GUIDC_ClassNew,  0 }
};

const CAF_STATIC_CLASS_INFO_T g_guidc_static_class_info = 
{ 
    (CAF_STATIC_CLASS_T*)s_guidc_class_list, 
    sizeof(s_guidc_class_list) / sizeof(s_guidc_class_list[0])
};

/*****************************************************************************/
// 	Description : new gui control
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E GUIDC_ClassNew( 
                                  CAF_GUID_T guid, 
                                  void**     object_pptr 
                                  )
{
    CAF_RESULT_E    result  = CAF_RESULT_NOT_SUPPORT;
    GUIDC_CTRL_T    *dc_ptr = PNULL;
    
    if( SPRD_GUID_DC == guid )
    {       
        dc_ptr = SCI_ALLOC_APP(sizeof(GUIDC_CTRL_T));
        
        SCI_MEMSET(dc_ptr, 0, sizeof(GUIDC_CTRL_T));
        dc_ptr->vtbl_ptr = (CAF_VTBL_T(IDC_T) *)&s_guidc_vtbl;
        
        dc_ptr->guid       = guid;
        dc_ptr->ref_num    = 1;
        
        result = CAF_RESULT_SUCCEEDED;
    }
    
    *object_pptr = dc_ptr;
    
    return result;
}

/*****************************************************************************/
// 	Description : dc query interface
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
CAF_RESULT_E GUIDC_QueryInterface( 
                                  IDC_T *idc_ptr, 
                                  CAF_GUID_T guid, 
                                  void** ojbect_pptr 
                                  )
{
    CAF_RESULT_E result = CAF_RESULT_FAILED;
    
    GUIDC_CTRL_T* dc_ptr = (GUIDC_CTRL_T*)idc_ptr;
    
    if ( IUNKNOWN_ID == guid ||  dc_ptr->guid == guid )
    {
        *ojbect_pptr = idc_ptr;
        
        result = CAF_RESULT_SUCCEEDED;
    }
    
    if( CAF_RESULT_SUCCEEDED == result )
    {
        IUNKNOWN_AddRef( *ojbect_pptr );
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : gui dc add ref
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
uint32 GUIDC_AddRef(
                    IDC_T * idc_ptr 
                    )
{
    return ++(((GUIDC_CTRL_T *)idc_ptr)->ref_num);
}

/*****************************************************************************/
// 	Description : gui dc release
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
uint32 GUIDC_Release(
                     IDC_T * idc_ptr
                     )
{   
    GUIDC_CTRL_T * dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    
    if ( --dc_ptr->ref_num )
    {
        return( dc_ptr->ref_num );
    }
    else
    {
        SCI_FREE( idc_ptr );   
        
        return 0;
    }
}

/*****************************************************************************/
// 	Description : set lcd id
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 GUIDC_SetLcdId(
                            IDC_T *idc_ptr, 
                            uint32 lcd_id
                            )
{
    GUIDC_CTRL_T * dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    uint32 old_lcd_id = GUIDC_INVALID_VALUE;
    
    if (PNULL != idc_ptr)
    {
        old_lcd_id = dc_ptr->lcd_dev_info.lcd_id;
        dc_ptr->lcd_dev_info.lcd_id = lcd_id;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_SetLcdId Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_671_112_2_18_3_15_24_64,(uint8*)"");
    }

    return old_lcd_id;
}

/*****************************************************************************/
// 	Description : set block id
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 GUIDC_SetBlockId(
                              IDC_T *idc_ptr, 
                              uint32 block_id
                              )
{
    GUIDC_CTRL_T * dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    uint32 old_block_id = GUIDC_INVALID_VALUE;

    if (PNULL != idc_ptr)
    {
        old_block_id = dc_ptr->lcd_dev_info.block_id;
        dc_ptr->lcd_dev_info.block_id = block_id;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_SetBlockId Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_696_112_2_18_3_15_24_65,(uint8*)"");
    }

    return old_block_id;
}

/*****************************************************************************/
// 	Description : set origin rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_SetOrigin(
                           IDC_T *idc_ptr, 
                           const GUI_POINT_T* point_ptr
                           )
{
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;

    if( PNULL != idc_ptr && PNULL != point_ptr )
    {
        dc_ptr->origin = *point_ptr;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_SetOrigin Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_719_112_2_18_3_15_24_66,(uint8*)"");
    }
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_SetRect(
                         IDC_T *idc_ptr, 
                         const GUI_RECT_T* rect_ptr
                         )
{
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    
    if ( PNULL != idc_ptr && PNULL != rect_ptr )
    {
        dc_ptr->rect = *rect_ptr;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_SetRect Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_740_112_2_18_3_15_24_67,(uint8*)"");
    }
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL CAF_FONT_TYPE_E GUIDC_SetFontType(
                                        IDC_T *idc_ptr,
                                        CAF_FONT_TYPE_E type
                                        )
{
	/*lint -e530*/
    GUIDC_CTRL_T *dc_ptr  = (GUIDC_CTRL_T *)idc_ptr;
    CAF_FONT_TYPE_E   old_type = CAF_FONT_16;

    if ( PNULL != idc_ptr )
    {   
        old_type = dc_ptr->font_info.type;
        
        dc_ptr->font_info.type = type;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_SetFontType Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_765_112_2_18_3_15_24_68,(uint8*)"");
    }
    
    return old_type;
}
    
/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL CAF_FONT_TYPE_E GUIDC_GetFontType(
                                        IDC_T *idc_ptr
                                        )
{
    CAF_FONT_TYPE_E   font_type = CAF_FONT_16;
    GUIDC_CTRL_T *dc_ptr  = (GUIDC_CTRL_T *)idc_ptr;
    
    if ( PNULL != idc_ptr )
    {
        font_type = dc_ptr->font_info.type;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_GetFontType Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_788_112_2_18_3_15_24_69,(uint8*)"");
    }
    
    return font_type;
}
      
/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/       
LOCAL CAF_FONT_EFFECT_E GUIDC_SetFontEffect( 
                                           IDC_T *idc_ptr,
                                           CAF_FONT_EFFECT_E effect
                                           )
{
    GUIDC_CTRL_T     *dc_ptr    = (GUIDC_CTRL_T *)idc_ptr;
    CAF_FONT_EFFECT_E old_effect = CAF_FONT_EFFECT_NONE;
    
    if ( PNULL != idc_ptr )
    {
        old_effect = dc_ptr->font_info.effect;
        
        dc_ptr->font_info.effect = effect;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_SetFontEffect Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_814_112_2_18_3_15_24_70,(uint8*)"");
    }
    
    return old_effect;
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL CAF_FONT_EFFECT_E GUIDC_GetFontEffect(
                                           IDC_T *idc_ptr
                                           )
{
    CAF_FONT_EFFECT_E font_effect = CAF_FONT_EFFECT_NONE;
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    
    if( PNULL != idc_ptr )
    {
        font_effect = dc_ptr->font_info.effect;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_GetFontEffect Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_837_112_2_18_3_15_24_71,(uint8*)"");
    }
    
    return font_effect;
}
    
/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/                 
LOCAL GUI_COLOR_T GUIDC_SetFontColor( 
                                     IDC_T *idc_ptr,
                                     GUI_COLOR_T color
                                     )
{
    GUIDC_CTRL_T* dc_ptr    = (GUIDC_CTRL_T *)idc_ptr;
    GUI_COLOR_T   old_color = 0;
    
    if ( PNULL != idc_ptr )
    {
        old_color = dc_ptr->font_info.color;
        
        dc_ptr->font_info.color = color;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_SetFontColor Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_863_112_2_18_3_15_24_72,(uint8*)"");
    }
    
    return old_color;
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/   
LOCAL GUI_COLOR_T GUIDC_GetFontColor( 
                                     IDC_T *idc_ptr
                                     )
{
    GUI_COLOR_T color = GUIDC_INVALID_VALUE_EXT;
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    
    if( PNULL != idc_ptr )
    {
        color = dc_ptr->font_info.color;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_GetFontColor Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_886_112_2_18_3_15_24_73,(uint8*)"");
    }

    return color;
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL CAF_ALIGN_T GUIDC_SetTextAlign( 
                                     IDC_T *idc_ptr,
                                     CAF_ALIGN_T align
                                     )
{
    GUIDC_CTRL_T* dc_ptr    = (GUIDC_CTRL_T *)idc_ptr;
    CAF_ALIGN_T   old_align = 0;
    
    if ( PNULL != idc_ptr )
    {
        old_align = dc_ptr->font_info.align;
        
        dc_ptr->font_info.align = align;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_SetTextAlign Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_912_112_2_18_3_15_24_74,(uint8*)"");
    }
    
    return old_align;
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL CAF_ALIGN_T GUIDC_GetTextAlign(
                                     IDC_T *idc_ptr
                                     )
{
    CAF_ALIGN_T align = CAF_ALIGN_LEFT;
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    
    if ( PNULL != idc_ptr )
    {
        align = dc_ptr->font_info.align;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_GetTextAlign Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_935_112_2_18_3_15_24_75,(uint8*)"");
    }
    
    return align;
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
// LOCAL GUI_ANGLE_E GUIDC_SetTextAngle(
//                                      IDC_T *idc_ptr,
//                                      GUI_ANGLE_E angle
//                                      )
// {
//     GUIDC_CTRL_T* dc_ptr    = (GUIDC_CTRL_T *)idc_ptr;
//     GUI_ANGLE_E   old_angle = 0;
//     
//     SCI_ASSERT( PNULL != idc_ptr );
//     
//     old_angle = dc_ptr->font_info.angle;
//     
//     dc_ptr->font_info.angle = angle;
//     
//     return old_angle;
// }

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
// LOCAL GUI_ANGLE_E GUIDC_GetTextAngle(
//                                      IDC_T *idc_ptr
//                                      )
// {
//     GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
//     
//     SCI_ASSERT( PNULL != idc_ptr );
//     
//     return dc_ptr->font_info.angle;
// }

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL uint16 GUIDC_SetPenSize( 
                              IDC_T *idc_ptr,
                              uint16 size
                              )
{
    GUIDC_CTRL_T* dc_ptr    = (GUIDC_CTRL_T *)idc_ptr;
    uint16   old_size = GUIDC_INVALID_VALUE_EXT;
    
    if ( PNULL != idc_ptr )
    {
        old_size = dc_ptr->pen_info.size;
        dc_ptr->pen_info.size = size;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_SetPenSize Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_996_112_2_18_3_15_25_76,(uint8*)"");
    }
    
    return old_size;
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL uint16 GUIDC_GetPenSize(
                              IDC_T *idc_ptr
                              )
{
    uint16 size = GUIDC_INVALID_VALUE_EXT;
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    
    if( PNULL != idc_ptr )
    {
        size = dc_ptr->pen_info.size;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_GetPenSize Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_1019_112_2_18_3_15_25_77,(uint8*)"");
    }
    
    return size;
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL GUI_COLOR_T GUIDC_SetPenColor(
                                    IDC_T *idc_ptr,
                                    GUI_COLOR_T color
                                    )
{
    GUIDC_CTRL_T* dc_ptr    = (GUIDC_CTRL_T *)idc_ptr;
    GUI_COLOR_T   old_color = GUIDC_INVALID_VALUE_EXT;
    
    if ( PNULL != idc_ptr )
    {
        old_color = dc_ptr->pen_info.color;
        
        dc_ptr->pen_info.color = color;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_SetPenColor Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_1045_112_2_18_3_15_25_78,(uint8*)"");
    }

    return old_color;
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL GUI_COLOR_T GUIDC_GetPenColor(
                                    IDC_T *idc_ptr
                                    )
{
    GUI_COLOR_T color = GUIDC_INVALID_VALUE_EXT;
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    
    if( PNULL != idc_ptr )
    {
        color = dc_ptr->pen_info.color;
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_GetPenColor Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_1068_112_2_18_3_15_25_79,(uint8*)"");
    }

    return color;
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL GUI_COLOR_T GUIDC_SetPixel( 
                                 IDC_T *idc_ptr,
                                 const GUI_POINT_T *point_ptr,
                                 GUI_COLOR_T color
                                 )
{
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    GUI_COLOR_T  old_color = GUIDC_INVALID_VALUE_EXT;
    GUI_POINT_T  point = {0};
    GUI_RECT_T   rect  = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
    uint16 width = 0;
    uint16 height = 0;
    
    if ( PNULL == idc_ptr || PNULL == point_ptr )
    {
        //SCI_TRACE_LOW:"GUIDC_SetPixel Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_1094_112_2_18_3_15_25_80,(uint8*)"");
        
        return old_color;
    }

    /*lint -e718*/
    old_color = GUIDC_GetPixel( idc_ptr, point_ptr );
    
    GetAbsolutePoint( dc_ptr, point_ptr, &point );
    
    block_buffer_ptr = (GUI_COLOR_T *)GUIBLOCK_GetBuf( &dc_ptr->lcd_dev_info );
    
    GUILCD_GetLogicWidthHeight(dc_ptr->lcd_dev_info.lcd_id,&width,&height);

    rect.right  = (int16)(width - 1);
    rect.bottom = (int16)(height - 1);

    if( GUI_PointIsInRect( point, rect ) )
    {
        block_buffer_ptr[( (uint16)point.y * (int32)width + point.x)] = color;
    }

    return old_color;
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL GUI_COLOR_T GUIDC_GetPixel( 
                                 IDC_T *idc_ptr,
                                 const GUI_POINT_T *point_ptr
                                 )
{
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    GUI_POINT_T  point = {0};
    GUI_RECT_T   rect  = {0};
    GUI_COLOR_T *block_buffer_ptr = PNULL;
    uint16 width = 0;
	uint16 height = 0;

    if ( PNULL == idc_ptr || PNULL == point_ptr )
    {
        //SCI_TRACE_LOW:"GUIDC_GetPixel Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_1137_112_2_18_3_15_25_81,(uint8*)"");

        return GUIDC_INVALID_VALUE_EXT;
    }

    GetAbsolutePoint( dc_ptr, point_ptr, &point );

    block_buffer_ptr = (GUI_COLOR_T *)GUIBLOCK_GetBuf( &dc_ptr->lcd_dev_info );
    
    GUILCD_GetLogicWidthHeight(dc_ptr->lcd_dev_info.lcd_id,&width,&height);


    rect.right  = (int16)(width - 1);
    rect.bottom = (int16)(height - 1);

    if( GUI_PointIsInRect( point, rect ) )
    {
        return block_buffer_ptr[ (uint16)point.y * width + (uint16)point.x];
    }
    else
    {
        return GUIDC_INVALID_VALUE_EXT;
    }
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL void GUIDC_DrawLine( 
                          IDC_T *idc_ptr,
                          const GUI_POINT_T* start_ptr,
                          const GUI_POINT_T* end_ptr
                          )
{
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T*)idc_ptr;
    GUI_POINT_T  start = {0};
    GUI_POINT_T  end = {0};
    
    if ( PNULL != idc_ptr && PNULL != start_ptr && PNULL != end_ptr )
    {
        GetAbsolutePoint( dc_ptr, start_ptr, &start );
        GetAbsolutePoint( dc_ptr, end_ptr, &end );
        
        LCD_DrawThickLine( &dc_ptr->lcd_dev_info, start.x, start.y, end.x, end.y, dc_ptr->pen_info.color, dc_ptr->pen_info.size );
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_DrawLine Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_1185_112_2_18_3_15_25_82,(uint8*)"");
    }

    return;
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL void GUIDC_DrawRect( 
                          IDC_T *idc_ptr,
                          const GUI_RECT_T* rect_ptr
                          )
{
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T*)idc_ptr;
    GUI_RECT_T    rect   = {0};
    
    if ( PNULL != idc_ptr && PNULL != rect_ptr )
    {
        GetAbsoluteRect( dc_ptr, rect_ptr, &rect );
        
        LCD_DrawRect( &dc_ptr->lcd_dev_info, rect, dc_ptr->pen_info.color );
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_DrawRect Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_1211_112_2_18_3_15_25_83,(uint8*)"");
    }

    return;
	/*lint -e533*/
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/  
LOCAL void GUIDC_FillRect( 
                          IDC_T *idc_ptr,
                          const GUI_RECT_T* rect_ptr
                          )
{
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T*)idc_ptr;
    GUI_RECT_T    rect   = {0};

    if ( PNULL != idc_ptr && PNULL != rect_ptr )
    {
        GetAbsoluteRect( dc_ptr, rect_ptr, &rect );
        
        LCD_FillRect( &dc_ptr->lcd_dev_info, rect, dc_ptr->pen_info.color );
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_FillRect Error input param!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_1238_112_2_18_3_15_25_84,(uint8*)"");
    }

    return;
}

/*****************************************************************************/
// 	Description : convert alignment
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL GUI_ALIGN_E ConvertCAFAlignToGUI(
                                       CAF_ALIGN_T caf_align
                                       )
{
    GUI_ALIGN_E gui_align = ALIGN_LEFT;

    CAF_ALIGN_T h_alignment = caf_align & CAF_ALIGN_CENTER;
    CAF_ALIGN_T v_alignment = caf_align & CAF_ALIGN_BASELINE;

    if( CAF_ALIGN_LEFT == h_alignment )
    {
        gui_align = ALIGN_LEFT;
    }
    else if( CAF_ALIGN_RIGHT == h_alignment )
    {
        gui_align = ALIGN_RIGHT;
    }
    else if( CAF_ALIGN_TOP == v_alignment )
    {
        gui_align = ALIGN_TOP;
    }
    else if( CAF_ALIGN_BOTTOM == v_alignment )
    {
        gui_align = ALIGN_BOTTOM;
    }
    else if( CAF_ALIGN_BASELINE == v_alignment && CAF_ALIGN_CENTER == h_alignment )
    {
        gui_align = ALIGN_HVMIDDLE;
    }
    else if( CAF_ALIGN_BASELINE == v_alignment )
    {
        gui_align = ALIGN_VMIDDLE;
    }
    else if( CAF_ALIGN_CENTER == h_alignment )
    {
        gui_align = ALIGN_HMIDDLE;
    }
    
    return gui_align;
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_DrawText(
                          IDC_T *idc_ptr,
                          const MMI_STRING_T* str_ptr,
                          const GUI_RECT_T* rect_ptr,
                          CAF_TEXT_FORMAT format
                          )
{
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    GUI_RECT_T    rect   = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state =         GUISTR_STATE_WORDBREAK;

    BOOLEAN is_reverse = FALSE;
    BOOLEAN is_align = FALSE;

    is_reverse = (BOOLEAN)((format >> 2) & 0x01);
    is_align = (BOOLEAN)((format >> 3) & 0x01);

    if( PNULL == idc_ptr 
        || PNULL == rect_ptr 
        || PNULL == str_ptr 
        || PNULL == str_ptr->wstr_ptr)
    {
        return;
    }
    
    GetAbsoluteRect( dc_ptr, rect_ptr, &rect );

    text_style.font_color = dc_ptr->font_info.color;
    text_style.font = dc_ptr->font_info.type;
    
    if (is_reverse)
    {
        state |= GUISTR_STATE_EFFECT;
        text_style.effect = FONT_EFFECT_REVERSE_EDGE;
    }
    if (is_align)
    {
        state |= GUISTR_STATE_ALIGN;
        text_style.align = ConvertCAFAlignToGUI( dc_ptr->font_info.align );
    }
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&dc_ptr->lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T     *)str_ptr,
        (GUISTR_STYLE_T         *)&text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_DrawTextId(
                            IDC_T *idc_ptr,
                            MMI_TEXT_ID_T text_id, 
                            CAF_HANDLE_T owner_handle,
                            const GUI_RECT_T* rect_ptr,
                            CAF_TEXT_FORMAT format
                            )
{
    GUIDC_CTRL_T        *dc_ptr     = (GUIDC_CTRL_T *)idc_ptr;
    GUI_RECT_T          rect        = {0};
    GUI_POINT_T         point       = {0};
    MMI_STRING_T        string      = {0};
    BOOLEAN             is_reverse  = FALSE;
    BOOLEAN             is_align    = FALSE;
    GUISTR_STYLE_T      text_style  = {0};/*lint !e64*/
    GUISTR_STATE_T      state       = GUISTR_STATE_WORDBREAK;

    if ( PNULL == idc_ptr || PNULL == rect_ptr )
    {
        return;
    }
       
    GetAbsoluteRect( dc_ptr, rect_ptr, &rect );
    
    point.x = rect.left;
    point.y = rect.top;

    MMITHEME_GetResText( text_id, owner_handle, &string );
    is_reverse = (BOOLEAN)((format >> 2) & 0x01);
    is_align = (BOOLEAN)((format >> 3) & 0x01);

    text_style.font_color = dc_ptr->font_info.color;
    text_style.font = dc_ptr->font_info.type;
    if (is_align)
    {
        state |= GUISTR_STATE_ALIGN;
        text_style.align = ConvertCAFAlignToGUI( dc_ptr->font_info.align );
    }
    
    if (is_reverse)
    {
        state |= GUISTR_STATE_EFFECT;
        text_style.effect = FONT_EFFECT_REVERSE_EDGE;
    }
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&dc_ptr->lcd_dev_info,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T     *)&string,
        (GUISTR_STYLE_T         *)&text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_DrawImage( 
                           IDC_T                *idc_ptr,
                           const uint8          *data_ptr,
                           uint32               data_size,
                           const GUI_POINT_T    *point_ptr
                           )
{
    GUIDC_CTRL_T    *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    GUI_POINT_T     point  = {0};

    if ( PNULL == idc_ptr || PNULL == point_ptr )
    {
        return;
    }

    GetAbsolutePoint( dc_ptr, point_ptr, &point );
    
    GUIRES_DisplayImgByPtr((uint8*)data_ptr,
        data_size,
        &point,
        PNULL,
        PNULL,
        &dc_ptr->lcd_dev_info);
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_DrawImageId( 
                           IDC_T *idc_ptr,
                           MMI_IMAGE_ID_T image_id,
                           CAF_HANDLE_T owner_handle,
                           const GUI_POINT_T *point_ptr
                           )
{
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    GUI_POINT_T   point  = {0};

    if ( PNULL == idc_ptr || PNULL == point_ptr )
    {
        return;
    }

    GetAbsolutePoint( dc_ptr, point_ptr, &point );
    
    GUIRES_DisplayImg(&point,
        PNULL,
        PNULL,
        owner_handle,
        image_id,
        &dc_ptr->lcd_dev_info);
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_DrawImageInRect( 
                                 IDC_T              *idc_ptr,
                                 const uint8        *data_ptr,
                                 uint32             data_size,
                                 const GUI_RECT_T   *image_rect_ptr,
                                 const GUI_RECT_T   *disp_rect_ptr
                                 )
{
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    GUI_RECT_T    rect   = {0};

    if ( PNULL == idc_ptr || PNULL == image_rect_ptr || PNULL == disp_rect_ptr)
    {
        return;
    }

    GetAbsoluteRect( dc_ptr, disp_rect_ptr, &rect );

    GUIRES_DisplayImgByPtr((uint8*)data_ptr,
        data_size,
        PNULL,
        (GUI_RECT_T*)disp_rect_ptr,
        (GUI_RECT_T*)image_rect_ptr,
        &dc_ptr->lcd_dev_info);
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_DrawImageIdInRect( 
                                   IDC_T *idc_ptr,
                                   MMI_IMAGE_ID_T image_id,
                                   CAF_HANDLE_T owner_handle,
                                   const GUI_RECT_T *image_rect_ptr,
                                   const GUI_RECT_T *disp_rect_ptr
                                   )
{
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    GUI_RECT_T    rect   = {0};

    if ( PNULL == idc_ptr || PNULL == image_rect_ptr || PNULL == disp_rect_ptr)
    {
        return;
    }

    GetAbsoluteRect( dc_ptr, disp_rect_ptr, &rect );

    GUIRES_DisplayImg(PNULL,
        &rect,
        (GUI_RECT_T*)image_rect_ptr,
        owner_handle,
        image_id,
        &dc_ptr->lcd_dev_info);
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GUIDC_MistBmp(
                         IDC_T *idc_ptr,
                         const GUI_RECT_T* rect_ptr,
                         GUI_COLOR_T color
                         )
{
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    GUI_RECT_T    rect   = {0};
    
    if ( PNULL == idc_ptr || PNULL == rect_ptr )
    {
        return;
    }
    
    GetAbsoluteRect( dc_ptr, rect_ptr, &rect );
    
    LCD_MistBmp( &dc_ptr->lcd_dev_info, rect, color );
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GUIDC_InvalidateRect( 
                                   IDC_T *idc_ptr,
                                   const GUI_RECT_T* rect_ptr
                                   )
{
    GUIDC_CTRL_T *dc_ptr = (GUIDC_CTRL_T *)idc_ptr;
    GUI_RECT_T    rect   = {0};
    
    if ( PNULL == idc_ptr || PNULL == rect_ptr )
    {
        return;
    }
    
    GetAbsoluteRect( dc_ptr, rect_ptr, &rect );

    return GUILCD_InvalidateRect( (GUI_LCD_ID_E)dc_ptr->lcd_dev_info.lcd_id, rect, 1 << dc_ptr->lcd_dev_info.block_id );
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GetAbsoluteRect(
                          GUIDC_CTRL_T *dc_ptr,
                          const GUI_RECT_T* src_ptr,
                          GUI_RECT_T* dst_ptr
                          )
{
    if ( PNULL != dc_ptr && PNULL != src_ptr && PNULL != dst_ptr )
    {
        dst_ptr->left   = (int16)( dc_ptr->origin.x + src_ptr->left );
        dst_ptr->right  = (int16)( dc_ptr->origin.x + src_ptr->right );
        dst_ptr->top    = (int16)( dc_ptr->origin.y + src_ptr->top );
        dst_ptr->bottom = (int16)( dc_ptr->origin.y + src_ptr->bottom );
    }
}

/*****************************************************************************/
// 	Description : set client rectangle
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void GetAbsolutePoint(
                            GUIDC_CTRL_T *dc_ptr,
                            const GUI_POINT_T* src_ptr,
                            GUI_POINT_T* dst_ptr
                            )
{
    if ( PNULL != dc_ptr && PNULL != src_ptr && PNULL != dst_ptr )
    {
        dst_ptr->x  = (int16)( dc_ptr->origin.x + src_ptr->x );
        dst_ptr->y  = (int16)( dc_ptr->origin.y + src_ptr->y );
    }
}


#ifdef DYNAMIC_MODULE_SUPPORT    
// add by songliang.liu
LOCAL BOOLEAN 			 GUIDC_Update				( IDC_T *idc_ptr )
{
    GUIDC_CTRL_T *	dc_ptr = (GUIDC_CTRL_T *)idc_ptr;

    return GUILCD_Invalidate( (GUI_LCD_ID_E)dc_ptr->lcd_dev_info.lcd_id, 1 << dc_ptr->lcd_dev_info.block_id );
}

LOCAL uint16 GUIDC_GetFontWidth ( IDC_T *dc_ptr, CAF_FONT_TYPE_E font_type,uint16 char_unicode )
{
	if ( PNULL != dc_ptr )
    {
        return GUI_GetFontWidth(font_type,char_unicode);
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_GetFontWidth dc_ptr == PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_1621_112_2_18_3_15_26_85,(uint8*)"");
        return 0;
    }
}

LOCAL uint16 GUIDC_GetFontHeight( IDC_T *dc_ptr, CAF_FONT_TYPE_E font_type ,uint16 char_unicode)
{
	if ( PNULL != dc_ptr )
    {
        return 	GUI_GetFontHeight(font_type,char_unicode);
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_GetFontHeight dc_ptr == PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_1634_112_2_18_3_15_26_86,(uint8*)"");
        return 0;
    }
}

LOCAL uint16 GUIDC_GetStringWidth( IDC_T *dc_ptr, CAF_FONT_TYPE_E font_type, CAF_STRING_T *str_ptr )
{
	if ( PNULL != dc_ptr )
    {
        return	GUI_GetStringWidth(font_type,str_ptr->wstr_ptr,str_ptr->wstr_len);
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_GetStringWidth dc_ptr == PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_1647_112_2_18_3_15_26_87,(uint8*)"");
        return 0;
    }
}

LOCAL uint16 GUIDC_GetStringHeight( IDC_T *dc_ptr, CAF_FONT_TYPE_E font_type, CAF_STRING_T *str_ptr )
{
	if ( PNULL != dc_ptr )
    {
        return (uint16)	GUI_GetStringHeight(font_type,str_ptr->wstr_ptr,str_ptr->wstr_len);
    }
    else
    {
        //SCI_TRACE_LOW:"GUIDC_GetStringHeight dc_ptr == PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIDC_1660_112_2_18_3_15_26_88,(uint8*)"");
        return 0;
    }
}

/*=============================================================
Name : GUIDC_SetDIBitsToDevice
Function : The function sets the pixels in the specified rectangle
		   on the device that is associated with the destination 
		   device context using color data from a device-independent
		   bitmap (DIB). 
Params:	
		IDC_T * dc_ptr[IN] ---  handle to DC
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
		UINT fuColorUse[IN] --- RGB or palette indexes
			Value			Meaning 
			DIB_PAL_COLORS :The bmiColors member is an array of 16-bit 
						indexes into the logical palette of the device
						context specified by hdc. 
			DIB_RGB_COLORS :The BITMAPINFO structure contains an array of
						literal RGB values.
Return : the number of scan lines set: succeeds
		0 : fail
=============================================================*/
LOCAL int GUIDC_SetDIBitsToDevice(
								IDC_T *dc_ptr,				 // handle to DC
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
								)
{
	GUIIMG_BITMAP_T	bi;
	GUI_RECT_T		rt;
	GUIDC_CTRL_T *	dc = (GUIDC_CTRL_T *)dc_ptr;
	int32 result;
    GUI_POINT_T     dis_point = {0};

	if(!dc_ptr || !lpvBits || !lpbmi) 
  	  return 0;

	//no used param
	XSrc;
    YSrc;
	YDest;
	uStartScan;
	fuColorUse;
    cScanLines;

    bi.bit_ptr    = (void *)lpvBits;
    bi.img_width  = (uint16)dwWidth;
    bi.img_height = (uint16)dwHeight;

	rt.left = (int16)XDest;
	rt.top	= (int16)YDest;
	rt.right = (int16)((uint32)XDest + dwWidth);
	rt.bottom= (int16)((uint32)YDest + dwHeight);

    dis_point.x = rt.left;
    dis_point.y = rt.top;
    result = GUIIMG_DisplayBmp(TRUE,
                &rt,
                &dis_point,
                &bi,
                &(dc->lcd_dev_info));

	return (result);
	
}
#endif
#endif
