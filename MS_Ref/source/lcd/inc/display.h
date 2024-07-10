/******************************************************************************
 ** File Name:      display.h                                                 *
 ** Author:         Jianping Wang                                             *
 ** DATE:           26/07/2010                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define interfaces API function for display      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 26/07/2010     Jianping Wang      Create                                  *
 ******************************************************************************/

#ifndef _DISPLAY_H
#define _DISPLAY_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "sci_types.h"
#include "os_api.h"
#include "dal_display.h"
#include "ref_lcd.h"

/**---------------------------------------------------------------------------*
 **                         Macro defines.                               
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Enum defines.                               
 **---------------------------------------------------------------------------*/
typedef enum
{
    PERMIT_GUI_INVALIDATE,
    NO_PERMIT_GUI_INVALIDATE,
    LCD_LIMIT_TYPE_MAX
}LCD_LIMIT_TYPE_E;

typedef enum
{
   UI_OPERATION,
   OSD_OPERATION,
   OSD_OPERATION_SPECIAL,   
   DISPLAY_OP_MAX
}DISPLAY_OPERATION_MODE_E;
/**---------------------------------------------------------------------------*
 **                         Struct defines.                               
 **---------------------------------------------------------------------------*/
typedef struct _lcd_buf_info_tag
{
    uint32 addr;
    uint32 size;
    uint32 width;
    uint32 height;
    uint32 alloc_addr;
    uint32 is_alloc; //0: static memory; 1: alloc memory
}LCD_BUF_INFO_T;

typedef struct
{
    uint16 w;
    uint16 h;
}DISPLAY_SIZE_T;

typedef struct _display_memory_info_tag
{
    uint32 buf_addr;
    DISPLAY_SIZE_T dst_size;
    DISPLAY_RECT_T dst_trim_rect;  
    DISPLAY_DATA_TYPE_E data_type;
    DISPLAY_DATA_ENDIA_E data_endian_mode;
}DISPLAY_MEMORY_INFO_T;
typedef struct _display_layer_cfg_tag
{
    DISPLAY_LAYER_INFO_T layer_data;
    uint16               set_endian_flag;
    uint16               endian_mode;
    uint32               is_set_layer;
}DISPLAY_LAYER_CFG_T;
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: get lcd display control semaphor
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_GetLcdCtrlSemaphore(LCD_ID_E lcd_id);
/*****************************************************************************/
//  Description: put lcd display control semaphor
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_PutLcdCtrlSemaphore(LCD_ID_E lcd_id);
/*****************************************************************************/
//  Description: settting buffer for display   
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
ERR_LCD_E LCD_SetDisplayBuffer (
                                      LCD_ID_E  lcd_id, 
                                      uint32 buf_sum,
                                      LCD_BUF_INFO_T buf_array[]
                                      );
/*****************************************************************************/
//  Description:  setting if gui is permitted to invalidate lcd
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
ERR_LCD_E LCD_SetInvalidateLimit (
                                       LCD_ID_E lcd_id, 
                                       LCD_LIMIT_TYPE_E limit_type
                                       );

/*****************************************************************************/
//  Description:  invalidate lcd for dislapy menu
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
ERR_LCD_E LCD_InvalidateRectEx(
                                      LCD_ID_E  lcd_id,                                           
                                      DISPLAY_LAYER_CFG_T *layer_arr[],
                                      uint32 array_size,
                                      DISPLAY_MEMORY_INFO_T  *mem_info_ptr,
                                      LCD_BRUSH_MODE_E brush_mode,
                                      DISPLAY_RECT_T *rect_ptr
                                      );
/*****************************************************************************/
//  Description:  set operation mode of invalidate lcd
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetOpMode(LCD_ID_E lcd_id,DISPLAY_OPERATION_MODE_E op_mode);
/*****************************************************************************/
//  Description: set if use display control semaphore
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_SetIsUseCtrlSm(BOOLEAN is_use);
/*****************************************************************************/
//  Description:  get operation mode of invalidate lcd
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC DISPLAY_OPERATION_MODE_E LCD_GetOpMode(LCD_ID_E lcd_id);
/*****************************************************************************/
//  Description:  get operation mode of invalidate lcd
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LCD_IsOSDMode(LCD_ID_E lcd_id);
/*****************************************************************************/
//  Description:  get if is osd needed copy
//  Global resource dependence:
//  Author: Eddy.wei
//  Note:
/*****************************************************************************/
PUBLIC uint32 LCD_SetCopyOsdMode(LCD_ID_E lcd_id,uint32 is_copy_osd);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif //_DISPLAY_H


