/******************************************************************************
 ** File Name:      ref_lcd.h                                                 *
 ** Author:         Jim zhang                                                 *
 ** DATE:           06/03/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the color LCD API function for up layer  *
 **                 1. LCD driver support a LCD buffer according to lcd size  *
 **                    to hold the pixel color. Up layer can get the buffer   *
 **                    pointer and operate on the LCD buffer.                 *
 **                 2. LCD driver is only responsible of copying the pixel    *
 **                    color in LCD buffer to LCD (hardware).                 *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/03/2004     Jim.zhang        Update file used for lcd api for up layer *
 ** 10/22/2004     Jim.zhang        Delete some api.                          *
 ******************************************************************************/

#ifndef _REF_LCD_H_
#define _REF_LCD_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "lcd_controller.h"
#include "lcdc_types.h"
#include "dal_lcd.h"
#include "dal_display.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defines.
 **---------------------------------------------------------------------------*/
#define LCD_SUM                              2
#define BLOCK_SUM                            6

#define LCD_SUPPORT_MAX                      2
#define LCD_BACKGROUND                       0x0

//Priority field configures the time ratio of LCM operation to NAND operation.
#define LCD_BUSRATIO                         1  // Configure the bus priority.

#define BLOCK_ID_WITH_SINGLE_PIXEL_ALPHA     1
#define BLOCK_IMAGE_ID                       0

// LCD operation fashion
typedef enum
{
    BLOCK_OPERATION = 0   ,
    AHB_COMMAND_OPERATION
}LCD_OPERATION_E;

//LCD status
typedef enum
{
    LCD_STATUS_IDLE   = 0,
    LCD_STATUS_BUSY
} LCD_STATUS_E;

typedef enum
{
    LCD_IDLE = 0,
    LCD_READY   ,
    LCD_BUSY    ,
    LCD_ERROR
} LCD_STATE_E;



typedef struct block_tag
{
    uint16      is_enable;  //1: enable this block;0: disable this block
    uint32      *mem_ptr;  
    uint32      *alpha_addr;
    uint32      *uv_addr;
    uint32      *v_addr; 
    BLOCKCFG_T  cfg;
} BLOCK_T;

typedef enum
{
    MEMORY_IDLE = 0,
    MEMORY_READY   ,
    MEMORY_BUSY    ,
    MEMORY_ERROR
} LCD_MEMORY_STATE_E;

typedef enum
{
    SYS_STATE_ASSERT = 0,           // Assert state
    SYS_STATE_NORMAL    ,           // Normal Run
    SYS_STATE_INT                   // Interrupt state
} SYS_STATE_E;

typedef enum
{
    LCD_ROT_MODE_LCD = 0x00,
    LCD_ROT_MODE_DMA,
    LCD_ROT_MODE_MAX
}LCD_ROT_MODE_E;
/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/

typedef struct _osd_rect_tag //rectangle
{
    uint16  x;
    uint16  y;
    uint16  w;
    uint16  h;
} OSD_RECT_T;

typedef struct _osd_size_tag
{
    uint16 w;
    uint16 h;
} OSD_SIZE_T;

typedef struct _osd_info_tag
{
    OSD_RECT_T rect;
    uint32 addr;
    uint16 colorkey;
    uint16  alpha;
    struct _osd_info_tag *next_ptr;
} OSD_INFO_T;

typedef struct _isp_osd_info_tag
{
    uint32 enable;
    OSD_SIZE_T background_size;
    OSD_INFO_T *inf_ptr;
} ISP_OSD_PARAM_T;


typedef struct block_group_tag
{
    uint32    lcd_id;
    BLOCK_T   block[BLOCK_SUM];
} BLOCK_GROUP_T;

typedef struct lcd_property_tag
{
    SCI_SEMAPHORE_PTR   semaphore;
    LCD_BRUSH_MODE_E    brush_mode;// 1 if brush lcd with synchronistic mean, or else 0
    LCD_OPERATION_E     operation;  //block operation or AHB command operation
    uint32              sleep_state;
    LCD_SPEC_INFO_T     lcd_spec;
} LCD_PROPERTY_T;


typedef struct _lcd_out_position_tag
{
    LCD_ID_E lcd_id;   //lcd id
    uint16   start_x;  //output start x at lcd
    uint16   start_y;  //output start y at lcd
    uint16   width;    //output data width
    uint16   height;   //output data height
    uint32   *data_buf;//memory address which lcd output data to
} LCD_OUT_POSITION_T;

typedef struct _lcd_size_tag
{
    uint16 w;
    uint16 h;
} LCD_SIZE_T;

typedef struct _lcd_rect_t_tag
{
    uint16 left;  //LCD display window left position
    uint16 top;   //LCD display window top position
    uint16 right; //LCD display window right position
    uint16 bottom;//LCD display window bottom position
} LCD_RECT_T;

typedef LCDC_CAP_PARAM_T LCD_CAP_PARAM_T;
typedef struct _lcd_memory_info_tag
{
    uint32 buf_addr;
    LCD_RECT_T cap_rect;
    LCD_SIZE_T dst_size;
    LCD_RECT_T dst_trim_rect;
    LCD_DATA_FORMAT_E data_type;  
} LCD_MEMORY_INFO_T;

typedef void (* lcd_invalidate_done_func_t) (uint32 param); //callback when invalidate end

/**---------------------------------------------------------------------------*
 **                         Function Prototypes
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Invalidate a rectangle of LCD--Refresh it.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Return:         error code.
//  Note:1. the left up point is (0,0)
//       2. Before call InvalidateRect(),write LCD buffer with color,
//          and then InvalidateRect() the spefic area.
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_InvalidateRect ( 
                                            LCD_ID_E lcd_id, //id of lcd to operate
                                            uint16 left,     //left of the windows to be refreshed.
                                            uint16 top,      //top of the widonws to be refreshed.
                                            uint16 right,    //right of the windows to be refreshed.
                                            uint16 bottom    //bottom of the windows to be refreshed.
                                        );

/*****************************************************************************/
//  Description:    Invalidate the whole lcd.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_Invalidate ( //Retrurn the operate status
                                    LCD_ID_E  lcd_id            //id of lcd to operate
                                    );

/*****************************************************************************/
//  Description:    Enable lcd to partial display mode, so can save power.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//  Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
PUBLIC ERR_LCD_E  LCD_SetDisplayWindow (
                                                LCD_ID_E lcd_id,//ID of lcd to operate
                                                uint16 left,  //left of the display window
                                                uint16 top,   //top of the display window
                                                uint16 right,     //right of the display window
                                                uint16 bottom //bottom of the display window
                                               );

/*****************************************************************************/
//  Description: configure block parameters
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_ConfigBlock (
                                        LCD_ID_E lcd_id,
                                        uint32 blk_num,
                                        BLOCKCFG_T config
                                       );
/*****************************************************************************/
//  Description: get block parameters
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_GetConfigBlock (
                                            LCD_ID_E lcd_id,
                                            uint32 blk_num,
                                            BLOCKCFG_T *cfg_ptr
                                           );
/*****************************************************************************/
//  Description: Enable Block
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_EnableBlock (LCD_ID_E lcd_id, uint32 blk_num);
/*****************************************************************************/
//  Description:disable block
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_DisableBlock (LCD_ID_E lcd_id, uint32 blk_num);

/*****************************************************************************/
//  Description:disable all block
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_DisableAllBlock (LCD_ID_E lcd_id);
/*****************************************************************************/
/*****************************************************************************/
//  Description: set display memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBlockBuffer (
                                            LCD_ID_E lcd_id,
                                            uint32 blk_num ,
                                            uint32 *buf_ptr
                                          );
/*****************************************************************************/
//  Description:Get Block is enabled or not
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LCD_GetBlockIsEnable (LCD_ID_E lcd_id, uint32 blk_num);
/*****************************************************************************/
//  Description: get display memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 *LCD_GetBlockBuffer (LCD_ID_E lcd_id, uint32 blk_num);
/*****************************************************************************/
//  Description:  set background color
//  Global resource dependence:
//  Author: Younger.Yang
//  Note: color is RGB888
/*****************************************************************************/
PUBLIC void LCD_SetBackground (uint32 color);
/*****************************************************************************/
//  Description:  disable/enable lcd invalidate rect
/*****************************************************************************/
//PUBLIC void LCD_InvalidateRect_Enable (BOOLEAN is_enable);
/*****************************************************************************/
//  Description:  set rotation mode
/*****************************************************************************/
PUBLIC void LCD_SetRotateMode (LCD_ROT_MODE_E mode);
/*****************************************************************************/
//  Description: set alpha buffer address when use pixel alpha ,
//              data format of block is RGB565
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetPixelAlphaBuffer (
                                                 LCD_ID_E lcd_id,
                                                 uint32 blk_num,
                                                 uint32 *buf_ptr
                                                );
/*****************************************************************************/
//  Description: get alpha buffer address when use pixel alpha ,
//              data format of block is RGB565
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 *LCD_GetPixelAlphaBuffer (LCD_ID_E lcd_id, uint32 blk_num);
/*****************************************************************************/
//  Description: set UV buffer for block 0
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetUVBuffer (
                                        LCD_ID_E lcd_id,
                                        uint32 blk_num,
                                        uint32 *buf_ptr
                                       );
PUBLIC ERR_LCD_E LCD_SetVBuffer (
                                        LCD_ID_E lcd_id,
                                        uint32 blk_num,
                                        uint32 *buf_ptr
                                       );
/*****************************************************************************/
//  Description: config capture parameters of 6800h
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_ConfigCapture ( 
                                            LCD_ID_E lcd_id, 
                                            LCD_CAP_PARAM_T *param_ptr
                                          );
/*****************************************************************************/
//  Description: set fmark mode
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetFmarkMode (
                                            LCD_ID_E lcd_id,
                                            LCD_FMARK_MODE_E fmark_mode,
                                            LCD_FMARK_POL_E fmark_pol
                                          );
/*****************************************************************************/
// Description:register callback when invalidate done
// Author: Jianping.Wang
/*****************************************************************************/
PUBLIC void LCD_RegisterCallBack (lcd_invalidate_done_func_t func);
/*****************************************************************************/
// Description:register callback when invalidate done
// Author: Jianping.Wang
/*****************************************************************************/
PUBLIC void LCD_UnRegisterCallBack (void);
/*****************************************************************************/
// Description: get block information
// Global resource dependence:
// Author: Jianping.wang
// Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_GetBlockInfo (LCD_ID_E lcd_id,BLOCK_T *blk_info_ptr);
/*****************************************************************************/
//  Description: LCD out data to memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_InvalidateRectToMemory (
                                                        LCD_ID_E lcd_id,
                                                        LCD_MEMORY_INFO_T *param_ptr
                                                      );
/*****************************************************************************/
//  Description: get UV memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 *LCD_GetBlockUVBuffer (LCD_ID_E lcd_id, uint32 blk_num);

/*****************************************************************************/
//  Description:  Get LCD invalidate direction type
//  Input:
//      direct_type:
//  Return:
//      None.
//  Note: Application should correct invalidate direction right after current
//        image displayed
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/
PUBLIC LCD_DIRECT_E  LCD_GetDirection (LCD_ID_E lcd_id);

/*****************************************************************************/
//  Description:  clear the whole lcd to one color.
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_Clear (
                            LCD_ID_E lcd_id,     //id of lcd to operate.
                            uint32 color        //color to fill the whole lcd.
                         );
/********************************************************************************/
//  Description:  Set LCD invalidate direction
//  Input:
//  Note: Application should correct invalidate direction right after current
/********************************************************************************/
PUBLIC ERR_LCD_E  LCD_SetDirection (
                                            LCD_ID_E lcd_id,//ID of lcd to operate
                                            LCD_DIRECT_E     direct_type
                                        );
/*****************************************************************************/
//Description: set brush lcd fashion
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBrushMode (LCD_ID_E lcd_id, LCD_BRUSH_MODE_E mode);
/*****************************************************************************/
//Description: get brush lcd fashion
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC LCD_BRUSH_MODE_E LCD_GetBrushMode (LCD_ID_E lcd_id);
/******************************************************************************/
// Description:register callback when invalidate done
// Author: Jianping.Wang
/******************************************************************************/
PUBLIC void LCD_RegCallback(lcd_invalidate_done_func_t func);
/******************************************************************************/
// Description:register callback when invalidate done
// Author: Jianping.Wang
/******************************************************************************/
PUBLIC void LCD_UnRegCallback(void);
/*****************************************************************************/
//  Description:    Invalidate a rectangle of LCD--Refresh it.
//  Global resource dependence:
//  Author:         hansen.sun
//  Return:         error code.
//  Note:1. the left up point is (0,0)
//       2. Before call InvalidateRect(),write LCD buffer with color,
//          and then InvalidateRect() the spefic area.
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_InvalidateRect_DMA (LCD_ID_E lcd_id,
                                         DISPLAY_RECT_T disp_rect,
                                         DISPLAY_RECT_T crop_rect ,
                                         uint32 src_w,
                                         LCD_DIRECT_E rot_angle);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
PUBLIC ERR_LCD_E LCD_SetImageLayerSourceSize(LCD_ID_E lcd_id, uint32 width, uint32 height);

#ifdef __cplusplus
    }
#endif

#endif 
