/******************************************************************************
 ** File Name:      dal_display.h                                             *
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

#ifndef _DAL_DISPLAY_H
#define _DAL_DISPLAY_H

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
#include "dal_lcd.h"

/**---------------------------------------------------------------------------*
 **                         Macro defines.                               
 **---------------------------------------------------------------------------*/
#ifdef  IMG_ASSERT_MODE
    #define IMG_ASSERT_WARNING(expr) SCI_ASSERT(expr)
#else
    #define IMG_ASSERT_WARNING(expr) if (!(expr)) { SCI_TRACE_LOW("IMG_ASSERT_WARNING %s, line=%ld", __FILE__, __LINE__); }
#endif

/**---------------------------------------------------------------------------*
 **                         Enum defines.                               
 **---------------------------------------------------------------------------*/
typedef enum
{
    ERR_DISPLAY_NONE = SCI_SUCCESS,     // Success,no error
    ERR_DISPLAY_FUNC_NOT_SUPPORT,       // not support this function
    ERR_DISPLAY_PARAMETER_WRONG,        // Parameter is wrong 
    ERR_DISPLAY_POINTER_NULL,           // Input pointer is PNULL
    ERR_DISPLAY_OPERATE_FAIL,           // Operate fail 
    ERR_DISPLAY_NOT_FOUND               // LCD not found 
} ERR_DISPLAY_E;

typedef enum
{
    DISPLAY_ANGLE_0,
    DISPLAY_ANGLE_90,
    DISPLAY_ANGLE_180,
    DISPLAY_ANGLE_270
}DISPLAY_ANGLE_E;

typedef enum
{
    DATA_TYPE_ARGB888 = 0, 
    DATA_TYPE_ARGB666,
    DATA_TYPE_ARGB565,   //RGB565 with pixel alpha
    DATA_TYPE_RGB565,
    DATA_TYPE_ARGB555,
    DATA_TYPE_GREY,
    DATA_TYPE_YUV422,
    DATA_TYPE_YUV420,
    DATA_TYPE_YUV400,
    DATA_TYPE_PMARGB888,
    DATA_TYPE_PMARGB666,
    DATA_TYPE_PMARGB565,
    DATA_TYPE_MAX
}DISPLAY_DATA_TYPE_E;

typedef enum _dislpay_alpha_type_tag
{   
   DISPLAY_PIXEL_ALPHA = 0,
   DISPLAY_BLOCK_ALPHA,
   DISPLAY_COMBO_ALPHA
}DISPLAY_ALPHA_TYPE_E;


typedef enum 
{
    ENDIAN_MODE_0 = 0,
    ENDIAN_MODE_1,
    ENDIAN_MODE_2,
    ENDIAN_MODE_MAX
}DISPLAY_DATA_ENDIA_E;


/**---------------------------------------------------------------------------*
 **                         Struct defines.                               
 **---------------------------------------------------------------------------*/
typedef struct _display_pos_tag
{
    int16 x;
    int16 y;    
}DISLPAY_POS_T;

typedef struct _display_rect_t_tag
{
    int16 left;  //left position of rectangle
    int16 top;   //top position of rectangle
    int16 right; //right position of rectangle
    int16 bottom;//position of rectangle
} DISPLAY_RECT_T;

typedef struct _display_data_property_tag
{
    DISPLAY_DATA_TYPE_E    type;            //data type
    uint32                 grey_value;      // grey value when use GREY          
    uint32                 alpha;           //alpha value 
    DISPLAY_ALPHA_TYPE_E   alpha_sel;       //pixel alpha or block alpha
    uint32                 colorkey;        // colorkey value
    BOOLEAN                colorkey_en;     // 1:use colorkey;0: don't use colorkey  
    BOOLEAN                immutable;       //reserved
    uint8                  padding1;        //reserved
    uint8                  padding2;        //reserved
    uint32                 rotation;
    uint32                 rb_switch;
    uint32                 cap_mode;
}DISPLAY_DATA_PROPERTY_T;

typedef struct _display_layer_info_tag
{
    uint32                   layer_id;      //0~3
    uint32                   is_enable;     // 1:enable this blcok,0: don't enable this block
    uint32                   buf_addr;      //it is buffer address of display data for ARGB888/ARGB666/ARGB565/RGB565/RGB555
                                            // or buffer address of Y data for YUV422/420/400
    uint32                   uv_buf_addr;   //it is buffer address of UV data when use YUV422/420/400 
    uint32                   alpha_buf_addr;//it is buffer of alpha when use ARGB565
    uint16                   width;         //widht of block ,uinit is pixel
    uint16                   height;        //height of block,unit is pixel
    DISLPAY_POS_T            pos;           //start position of display block
    DISPLAY_DATA_PROPERTY_T  layer_data;    //property of dispay data
    DISPLAY_RECT_T           clip_rect;
}DISPLAY_LAYER_INFO_T;

typedef struct _display_invalidate_pos_tag
{
    DISPLAY_RECT_T              rect;
    DISPLAY_ANGLE_E             rot_angle;    
}DISPLAY_INVALIDATE_POS_T;

typedef struct _display_convert_input_tag
{
    uint16 logic_lcd_width;
    uint16 logic_lcd_height;
    DISPLAY_RECT_T  logic_coordinate;
    DISPLAY_ANGLE_E logic_angle;
}DISPLAY_CONVERT_INPUT_T;

typedef struct _display_convert_output_tag
{
    DISPLAY_RECT_T  physical_coordinate;
    DISPLAY_ANGLE_E rotation_angle;
}DISPLAY_CONVERT_OUTPUT_T;

typedef struct _display_blend_layer_tag
{
    int16                interval_left;
    int16                interval_right;     
    uint32               layer_buf_size; 
    uint32               alpha_buf_size;
    DISPLAY_LAYER_INFO_T layer_info;
}DISPLAY_BLEND_LAYER_T;
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:  convert from logic coordinate to physical coordinate
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_DISPLAY_E DISPLAY_ConvertCoordinate (
                                                         LCD_ID_E   lcd_id, 
                                                         DISPLAY_CONVERT_INPUT_T *input_ptr,
                                                         DISPLAY_CONVERT_OUTPUT_T *out_ptr
                                                         );

/*****************************************************************************/
//  Description:  invalidate lcd for dislapy menu
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_DISPLAY_E DISPLAY_InvalidateRect (
                                                    LCD_ID_E    lcd_id, 
                                                    DISPLAY_LAYER_INFO_T *layer_arr[],
                                                    uint32 array_size,
                                                    DISPLAY_INVALIDATE_POS_T *param_ptr
                                                    );
/*****************************************************************************/
//  Description:  multi layer blend interface for the APP
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_DISPLAY_E DISPLAY_BlendLayer(  LCD_ID_E lcd_id,
                                                 DISPLAY_BLEND_LAYER_T *dst_layer_ptr,
                                                 DISPLAY_BLEND_LAYER_T *layer_arr[],
                                                 uint32 array_size,
                                                 DISPLAY_RECT_T *rect_ptr);

/*****************************************************************************/
//  Description:  disable display layer for invalidate LCD function
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_DISPLAY_E DISPLAY_DisableLayer(LCD_ID_E lcd_id, uint32 layer_id);
/*****************************************************************************/
//  Description:  enter or exit sleep
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_DISPLAY_E DISPLAY_EnterSleep(LCD_ID_E lcd_id, BOOLEAN is_sleep);
/*****************************************************************************/
//  Description: if permit to update UI to LCD
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:is_permit - TRUE,permit to update UI to LCD
//                   FALSE,don't permit to update UI to LCD
/*****************************************************************************/
ERR_DISPLAY_E DISPLAY_IsPermit(LCD_ID_E lcd_id,BOOLEAN is_permit);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Macro defines.
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/
typedef void (*DISPLAY_CALLBACK)( uint32 param );

typedef struct tag_DISPLAY_INVALIDATE_PARM_T{
    DISPLAY_LAYER_INFO_T     **layer_arr;
    uint32                   layer_num;
    DISPLAY_INVALIDATE_POS_T *param_ptr;
}DISPLAY_INVALIDATE_PARM_T;

typedef struct tag_DISPLAY_BLENDLAYER_PARM_T{
    DISPLAY_BLEND_LAYER_T *dst_layer_ptr;
    DISPLAY_BLEND_LAYER_T **layer_arr;
    uint32 layer_num;
    DISPLAY_RECT_T *rect_ptr;

    DISPLAY_CALLBACK callback;
    uint32 param;

}DISPLAY_BLENDLAYER_PARM_T;


PUBLIC ERR_DISPLAY_E DISPLAY_BlendAndInvalidateRect(
                                                    LCD_ID_E    lcd_id, 
                                                    DISPLAY_BLENDLAYER_PARM_T *blend,                                                       
                                                    DISPLAY_INVALIDATE_PARM_T *invalidate
                                                    );
                                                    
                                                    
PUBLIC ERR_LCD_E LCDAPI_GetInfo (
                                    LCD_ID_E   lcd_id,          //id of lcd to operate
                                    LCD_INFO_T *lcd_info_ptr    //lcd information struct pointer
                                   );                

/****************************************************************************************/
// Description: get lcdc algin parameter
// Global resource dependence:
// Author: Eddy.wei
/*****************************************************************************************/
PUBLIC BOOLEAN LCDC_GetAlginParam (DISPLAY_DATA_TYPE_E data_format, 
                                   uint32 *width_algin, 
                                   uint32 *height_algin);
#ifdef __cplusplus
    }
#endif

#endif //_DAL_DISPLAY_H

