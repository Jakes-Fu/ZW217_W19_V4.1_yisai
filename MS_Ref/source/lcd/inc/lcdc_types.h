#ifndef _LCDC_TYPES_H_
#define _LCDC_TYPES_H_

#include "sci_types.h"

//interrupt handle function declare
typedef void (*LCD_IRQ_FUNC) (void);

typedef enum _lcd_int_type_tag
{
    LCD_INT_LCDC_DONE = 0,
    LCD_INT_LCM_EOF,
    LCD_INT_LCM_SOF,
    LCD_INT_FMARK,
    LCD_INT_MAX
} LCD_INT_TYPE_E;

typedef enum _lcd_layer_id_tag
{
    LCD_LAYER_IMAGE,
    LCD_LAYER_OSD1,
    LCD_LAYER_OSD2,
    LCD_LAYER_OSD3,
    LCD_LAYER_OSD4,
    LCD_LAYER_OSD5,
    LCD_LAYER_ID_MAX
} LCD_LAYER_ID_E;

typedef enum _lcd_fmark_mode_tag
{
    LCD_FMARK_DEVICE,
    LCD_FMARK_NONE,
    LCD_FMARK_MODE_MAX
} LCD_FMARK_MODE_E;

typedef enum _lcd_fmark_pol_tag
{
    LCD_FMARK_HIGH,
    LCD_FMARK_LOW,
    LCD_FMARK_POL_MAX
} LCD_FMARK_POL_E;

typedef enum _lcd_upsample_mode_tag
{
    LCD_UPSAMPLE_DUPLICATE,
    LCD_UPSAMPLE_AVERAGE,
    LCD_UPSAMPLE_MODE_MAX
} LCD_UPSAMPLE_MODE_E;

typedef enum _lcd_data_format_tag
{
    LCD_YUV422 = 0,
    LCD_YUV420,
    LCD_YUV400,
    LCD_RGB888,
    LCD_RGB666,
    LCD_RGB565,
    LCD_RGB555,
    LCD_GREY,
    LCD_MAX
} LCD_DATA_FORMAT_E;

typedef enum _lcd_osd_alpha_tag
{
    LCD_OSD_PIXEL_ALPHA = 0,
    LCD_OSD_BLOCK_ALPHA,
    LCD_OSD_COMBO_ALPHA,
    LCD_OSD_MAX
} LCD_OSD_ALPHA_E;

typedef enum
{
    LCDC_CLOCK_48MHZ = 0,
    LCDC_CLOCK_64MHZ,
    LCDC_CLOCK_96MHZ
}LCDC_CLOCK_E;

typedef enum _lcdc_output_mode_tag
{
    LCDC_CAP_MODE = 0,
    LCDC_DISP_SPI_MODE,
    LCDC_DISP_PARALLEL_MODE,
}LCDC_OUTPUT_MODE_E;

typedef enum _lcdc_cap_master_tag
{
    LCDC_CAP_MASTER_1 = 0,
    LCDC_CAP_MASTER_2,
    LCDC_CAP_MASTER_MAX
}LCDC_CAP_MASTER_E;

typedef enum _lcdc_cap_master_mode_tag
{
    LCDC_FIXD_ADDR =0,
    LCDC_INCRSE_ADDR,
    LCDC_INCRSE_WRAP_ADDR,
    LCDC_ADDR_MAX
}LCDC_CAP_MASTER_MODE_E;

typedef enum _lcdc_disp_rgb_mode_tag
{
    LCDC_DISP_RGB_565 = 0,
    LCDC_DISP_RGB_565E1,
    LCDC_DISP_RGB_565E2,
    LCDC_DISP_RGB_666,
    LCDC_DISP_RGB_666E1,
    LCDC_DISP_RGB_666E2,
    LCDC_DISP_RGB_888,
    LCDC_DISP_RGB_565X2,
    LCDC_DISP_RGB_MAX
}LCDC_DISP_RGB_MODE_E;

typedef enum _lcdc_expand_mode_tag
{
    LCDC_RGB_EXPAND_MSB = 0,
    LCDC_RGB_EXPAND_0,
    LCDC_RGB_EXPAND_LSB,
    LCDC_RGB_EXPAND_MAX
}LCDC_RGB_EXPAND_MODE_E;

typedef enum _lcdc_rotation_tag
{
    LCDC_ROTATION_0 = 0,
    LCDC_ROTATION_90,
    LCDC_ROTATION_180,
    LCDC_ROTATION_270,
    LCDC_ROTATION_MIRROR,
    LCDC_ROTATION_FLIP,
    LCDC_ROTATION_MAX
}LCDC_ROTATION_E;

typedef enum _lcdc_rb_switch_tag
{
    LCDC_RB_SWITCH_NO = 0,
    LCDC_RB_SWITCH_YES,
    LCDC_RB_SWITCH_MAX
}LCDC_RB_SWITCH_E;

typedef enum _lcdc_osd_blend_mode_tag
{
    LCDC_OSD_BLEND_NORMAL = 0,
    LCDC_OSD_BLEND_SPECIAL,
    LCDC_OSD_BLEND_MAX
}LCDC_OSD_BLEND_MODE_E;

typedef enum _lcdc_cap_mode_tag
{
    LCDC_CAP_NORMAL = 0,
    LCDC_CAP_ONLY_OSD,
    LCDC_CAP_MAX
}LCDC_CAP_MODE_E;

typedef enum _lcdc_endian_switch_tag
{
    LCDC_BIG_ENDIAN_SWITCH = 0,
    LCDC_LITTLE_ENDIAN_SWITCH,
    LCDC_SPECIAL_ENDIAN_SWITCH,
}LCDC_ENDIAN_SWITCH_E;

typedef struct _lcd_postion_tag
{
    uint16 x;
    uint16 y;
} LCDC_POS_T;

typedef struct _lcdc_size_tag
{
    uint16 w;
    uint16 h;
} LCDC_SIZE_T;

typedef struct _lcd_rect_tag //rectangle
{
    uint16  x;
    uint16  y;
    uint16  w;
    uint16  h;
} LCDC_RECT_T;

typedef struct _lcd_data_addr_tag
{
    uint32 y_addr;
    uint32 uv_addr;
} LCD_DATA_ADDR_T;

typedef struct _lcd_img_blk_param_tag
{
    LCDC_SIZE_T             src_size;
    LCDC_RECT_T             src_trim_rect;
    LCDC_POS_T              disp_pos;
    LCD_DATA_ADDR_T         src_base_addr;
    LCD_DATA_FORMAT_E       format;
    LCDC_ENDIAN_SWITCH_E    endian_switch;
    LCDC_ENDIAN_SWITCH_E    uv_endian_switch;
    LCDC_RB_SWITCH_E        rb_switch;
    LCDC_ROTATION_E         rotation;
    BOOLEAN                 layer_en;
    uint8 padding0;//reserved
    uint8 padding1;//reserved
    uint8 padding2;//reserved
} LCD_IMG_LAYER_PARAM_T;

typedef struct _lcd_osd_blk_param_tag
{
    LCDC_SIZE_T             src_size;
    LCDC_RECT_T             src_trim_rect;
    LCDC_POS_T              disp_pos;
    uint32                  src_base_addr;
    uint32                  alpha_base_addr;
    uint32                  grey_rgb;
    uint8                   alpha;
    LCD_DATA_FORMAT_E       format;
    LCD_OSD_ALPHA_E         alpha_mode;
    LCDC_ENDIAN_SWITCH_E    endian_switch;
    LCDC_RB_SWITCH_E        rb_switch;
    LCDC_ROTATION_E         rotation;
    LCDC_OSD_BLEND_MODE_E   blend_mode;
    uint32                  colorkey;
    BOOLEAN                 layer_en;
    BOOLEAN                 colorkey_en;
    uint8                   padding0;//reserved
    uint8                   padding1;//reserved
} LCD_OSD_LAYER_PARAM_T;

typedef struct _lcd_yuv2rgb_param_tag
{
    LCD_UPSAMPLE_MODE_E     upsample_mode;
    uint8                   contrast;           //0-255
    uint8                   saturation;         //0-255
    int16                   brightness;         //-256-256
} LCD_YUV2RGB_PARAM_T;

typedef struct _lcdc_cap_param_tag
{
    LCDC_RECT_T             cap_rect;
    LCDC_SIZE_T             dst_size;
    LCDC_RECT_T             dst_trim_rect;
    LCD_DATA_FORMAT_E       format;
    uint32                  dst_base_addr;
    LCDC_ENDIAN_SWITCH_E    endian_switch;
    BOOLEAN                 cap_en;
    uint8                   set_endian_flag;
    uint8                   padding0;//reserved
    uint8                   padding1;//reserved
    LCDC_ROTATION_E         rotation;
    LCDC_RB_SWITCH_E        rb_switch;
    LCDC_CAP_MODE_E         cap_mode;
} LCDC_CAP_PARAM_T;

#endif
