#include "fdl_bootloader_trc.h"
#include "sci_types.h"
#include "dal_lcd.h"
#include "os_api.h"
#include "lcm_drv.h"
#include "tf_lcdc_app.h"
#include "lcd_cfg.h"
#include "sc_reg.h"
#include "ref_lcd.h"
#include "tf_load.h"
#include "hal_gouda.h"

#define SCI_MEMSET      memset
#define TF_UartTrace    rprintf

#define LCDC_OSD_LAYER_NUM      3
#define LCDC_LAYER_NUM          4
#define HW_BLOCK_NUM            4

PUBLIC TF_LCD_INFO s_lcd_info;

LOCAL   uint16              font_color = 0x00;
LOCAL   uint16 			    g_color = 0x00FF;     //blue(0x00FF) white(0xFFFF) green(0x0F0F)
LOCAL   uint32              s_lcd_index = 0;
LOCAL   uint32              s_reg_flag[LCD_SUPPORT_MAX] ;
LOCAL   uint32              s_lcd_used_num = 0;
LOCAL   LCD_SPEC_INFO_T_PTR s_lcd_spec_info_ptr[LCD_SUPPORT_MAX];
LOCAL   LCD_PROPERTY_T      s_lcd_property_table[LCD_SUM];
LOCAL   BLOCK_T             s_buffer_info[LCD_SUPPORT_MAX][BLOCK_SUM];

#pragma arm section zidata = "LCD_FRAME_BUF_AREA"
__align(32) uint16 s_main_lcd_buffer[((LCD_WIDTH+15)/16*16)*((LCD_HEIGHT+7)/8*8)];
__align(32) uint16 s_sub_lcd_buffer[SUBLCD_WIDTH*SUBLCD_HEIGHT];
#pragma arm section zidata	


/**--------------------------------------------------------------------------*
 **                     Font library used in sci_assert                      *
 **--------------------------------------------------------------------------*/

//
//  8 X 16 ( width * height )
//
static const uint8 ascii_8X16[ 96 * 16] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
    0x00, 0x48, 0x6C, 0x24, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x24, 0x24, 0x24, 0x7F, 0x12, 0x12, 0x12, 0x7F, 0x12, 0x12, 0x12, 0x00, 0x00,
    0x00, 0x00, 0x08, 0x1C, 0x2A, 0x2A, 0x0A, 0x0C, 0x18, 0x28, 0x28, 0x2A, 0x2A, 0x1C, 0x08, 0x08,
    0x00, 0x00, 0x00, 0x22, 0x25, 0x15, 0x15, 0x15, 0x2A, 0x58, 0x54, 0x54, 0x54, 0x22, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0C, 0x12, 0x12, 0x12, 0x0A, 0x76, 0x25, 0x29, 0x11, 0x91, 0x6E, 0x00, 0x00,
    0x00, 0x06, 0x06, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x40, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x40, 0x00,
    0x00, 0x02, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x6B, 0x1C, 0x1C, 0x6B, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x7F, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x04, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x20, 0x20, 0x10, 0x08, 0x04, 0x42, 0x7E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x20, 0x18, 0x20, 0x40, 0x40, 0x42, 0x22, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x20, 0x30, 0x28, 0x24, 0x24, 0x22, 0x22, 0x7E, 0x20, 0x20, 0x78, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7E, 0x02, 0x02, 0x02, 0x1A, 0x26, 0x40, 0x40, 0x42, 0x22, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x38, 0x24, 0x02, 0x02, 0x1A, 0x26, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7E, 0x22, 0x22, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x24, 0x18, 0x24, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x64, 0x58, 0x40, 0x40, 0x24, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x04,
    0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x46, 0x40, 0x20, 0x10, 0x10, 0x00, 0x18, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1C, 0x22, 0x5A, 0x55, 0x55, 0x55, 0x55, 0x2D, 0x42, 0x22, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x08, 0x18, 0x14, 0x14, 0x24, 0x3C, 0x22, 0x42, 0x42, 0xE7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1F, 0x22, 0x22, 0x22, 0x1E, 0x22, 0x42, 0x42, 0x42, 0x22, 0x1F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x01, 0x01, 0x01, 0x01, 0x01, 0x42, 0x22, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1F, 0x22, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x22, 0x1F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3F, 0x42, 0x12, 0x12, 0x1E, 0x12, 0x12, 0x02, 0x42, 0x42, 0x3F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3F, 0x42, 0x12, 0x12, 0x1E, 0x12, 0x12, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3C, 0x22, 0x22, 0x01, 0x01, 0x01, 0x71, 0x21, 0x22, 0x22, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x11, 0x0F,
    0x00, 0x00, 0x00, 0x77, 0x22, 0x12, 0x0A, 0x0E, 0x0A, 0x12, 0x12, 0x22, 0x22, 0x77, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x07, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x42, 0x7F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x77, 0x36, 0x36, 0x36, 0x36, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x6B, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xE3, 0x46, 0x46, 0x4A, 0x4A, 0x52, 0x52, 0x52, 0x62, 0x62, 0x47, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3F, 0x42, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x4D, 0x53, 0x32, 0x1C, 0x60, 0x00,
    0x00, 0x00, 0x00, 0x3F, 0x42, 0x42, 0x42, 0x3E, 0x12, 0x12, 0x22, 0x22, 0x42, 0xC7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x02, 0x04, 0x18, 0x20, 0x40, 0x42, 0x42, 0x3E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7F, 0x49, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x22, 0x24, 0x24, 0x14, 0x14, 0x18, 0x08, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x6B, 0x49, 0x49, 0x49, 0x49, 0x55, 0x55, 0x36, 0x22, 0x22, 0x22, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x18, 0x18, 0x18, 0x24, 0x24, 0x42, 0xE7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x77, 0x22, 0x22, 0x14, 0x14, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7E, 0x21, 0x20, 0x10, 0x10, 0x08, 0x04, 0x04, 0x42, 0x42, 0x3F, 0x00, 0x00,
    0x00, 0x78, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x78, 0x00,
    0x00, 0x00, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x20, 0x40, 0x40,
    0x00, 0x1E, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1E, 0x00,
    0x00, 0x38, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0x00, 0x06, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x78, 0x44, 0x42, 0x42, 0xFC, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x1A, 0x26, 0x42, 0x42, 0x42, 0x26, 0x1A, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x02, 0x02, 0x02, 0x44, 0x38, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x60, 0x40, 0x40, 0x40, 0x78, 0x44, 0x42, 0x42, 0x42, 0x64, 0xD8, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x7E, 0x02, 0x02, 0x42, 0x3C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xF0, 0x88, 0x08, 0x08, 0x7E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x22, 0x22, 0x1C, 0x02, 0x3C, 0x42, 0x42, 0x3C,
    0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x3A, 0x46, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x1E,
    0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x72, 0x12, 0x0A, 0x16, 0x12, 0x22, 0x77, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x92, 0x92, 0x92, 0x92, 0x92, 0xB7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x46, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x26, 0x42, 0x42, 0x42, 0x22, 0x1E, 0x02, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x44, 0x42, 0x42, 0x42, 0x44, 0x78, 0x40, 0xE0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x4C, 0x04, 0x04, 0x04, 0x04, 0x1F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x42, 0x02, 0x3C, 0x40, 0x42, 0x3E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x42, 0x42, 0x42, 0x42, 0x62, 0xDC, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x14, 0x08, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEB, 0x49, 0x49, 0x55, 0x55, 0x22, 0x22, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x24, 0x18, 0x18, 0x18, 0x24, 0x6E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x14, 0x18, 0x08, 0x08, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x22, 0x10, 0x08, 0x08, 0x44, 0x7E, 0x00, 0x00,
    0x00, 0xC0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xC0, 0x00,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x00, 0x06, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x06, 0x00,
    0x0C, 0x32, 0xC2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};


PUBLIC uint32 LCD_ReadRegVal (uint32 lcd_id, uint32 reg_addr)
{
    //TODO    
    return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCD_SimpleInit (LCD_SIMPLE_INIT_PARAM_T *param_ptr)
{
    //TODO
    return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCD_Init (void)
{
    LCD_ID_E          lcd_id;
    LCD_INFO_T        lcd_info = {0};
    BLOCKCFG_T        cfg = {0};

    TF_UartTrace("[LCD W217] :: LCD_Init [IN]\n");

    //get lcd spec and reg the info
    for (lcd_id = 0; lcd_id < LCD_SUPPORT_MAX; lcd_id++)
    {
        s_lcd_spec_info_ptr[lcd_id] = LCM_GetLcdSpecInfo (lcd_id);

        if (PNULL != s_lcd_spec_info_ptr[lcd_id])
        {
            SCI_ASSERT (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation);/*assert verified*/

            TF_UartTrace("[LCD W217] :: LCD_Init [Found - %x]\n", s_lcd_spec_info_ptr[lcd_id]);
            s_reg_flag[lcd_id] = 1;
            s_lcd_used_num++;
        }
    }

    if (s_lcd_used_num == 0)
    {
        TF_UartTrace("[LCD W217] :: LCD_Init [Error Not Found]\n");/*assert verified*/
	 return ERR_LCD_NOT_FOUND;
    }

    for (lcd_id=0 ; lcd_id<s_lcd_used_num ; lcd_id++)
    {
        s_lcd_property_table[lcd_id].semaphore = SCI_NULL;
        s_lcd_property_table[lcd_id].brush_mode = LCD_BRUSH_MODE_SYNC;
        s_lcd_property_table[lcd_id].lcd_spec = *s_lcd_spec_info_ptr[lcd_id];
        s_lcd_property_table[lcd_id].operation = BLOCK_OPERATION;
        s_lcd_property_table[lcd_id].sleep_state = SCI_FALSE;
    }

    hal_gouda_init();
    TF_UartTrace("[LCD W217] :: LCD_Init [GOUDA Init]\n");

    /* init lcd buffer */
    for (lcd_id=0; lcd_id<s_lcd_used_num; lcd_id++)
    {
        LCD_GetInfo (lcd_id,&lcd_info);
        cfg.colorkey   = 0;
        cfg.resolution = LCD_RESOLUTION_RGB565;
        cfg.start_x    = 0;
        cfg.start_y    = 0;
        cfg.end_x      = lcd_info.lcd_width - 1;
        cfg.end_y      = lcd_info.lcd_height  - 1;
        cfg.width      = lcd_info.lcd_width;
        cfg.colorkey_en = FALSE;
        cfg.alpha_sel = 1;
        cfg.alpha     = 0xff;
        cfg.priority  = 0;
        cfg.type      = 1;
        LCD_ConfigBlock (lcd_id, 0, cfg);
        LCD_SetBlockBuffer (lcd_id, 0, lcd_info.lcdbuff_ptr);
        LCD_EnableBlock (lcd_id, 0);

        if (lcd_id == 0)
        {
            memcpy(&s_lcd_info.lcd_info, &lcd_info, sizeof(LCD_INFO_T));
            s_lcd_info.bus_width = s_lcd_spec_info_ptr[0]->spec_ptr->bus_width;
            s_lcd_info.data_addr = NULL;
        }
    }

    //init all the lcd support
    for (lcd_id = 0; lcd_id < s_lcd_used_num; lcd_id++)
    {
        if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
        {
            TF_UartTrace("[LCD W217] :: LCD_Init [LCD <%d> Operation Init]\n", lcd_id);
            s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_Init();
            s_lcd_property_table[MAIN_LCD_ID].sleep_state = FALSE;
        }
    }

    TF_UartTrace("[LCD W217] :: LCD_Init [OUT]\n");

    return ERR_LCD_NONE;
}

PUBLIC void LCD_Close (void)
{
    uint16 lcd_id;

    for (lcd_id = 0; lcd_id < s_lcd_used_num; lcd_id++)
    {
        if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
        {
            s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_Close();
        }
    }
    hal_gouda_term();
}

PUBLIC void LCD_Set8Pixels (uint16 cx, uint16 cy, uint8 bit_mask)
{
    uint16 *buffer_ptr = 0;
    uint16 i;
    LCD_INFO_T  lcd_info;   
    uint16 lcd_width = 0;

    LCD_GetInfo (MAIN_LCD_ID,&lcd_info);
    buffer_ptr = lcd_info.lcdbuff_ptr;

    lcd_width = lcd_info.lcd_width;

    for (i=0; i<8; i++)
    {
        if ( (bit_mask & (0x01<<i)) !=0x00)
        {
            * (buffer_ptr+cy*lcd_width+cx+i) = font_color;    //( cx+i, cy, color);
        }
    }
}

PUBLIC void  LCD_Display(uint8 lcd_id,uint16 left, uint16 top, uint16 right,  uint16 bottom)
{
    LCD_EnableBlock (lcd_id, 0);
    LCD_InvalidateRect(lcd_id, left, top, right, bottom);
}

PUBLIC ERR_LCD_E LCD_DisableAllBlock (LCD_ID_E lcd_id)
{
    uint32 i = 0;

    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/

    for(i=0 ; i< LCDC_LAYER_NUM ; i++)
    {
        s_buffer_info[lcd_id][i].is_enable = SCI_FALSE;
    }

    return ERR_LCD_NONE;
}

PUBLIC void LCD_DisplayText (uint16 x, uint16 y, char *text, uint16 len)
{
    uint16  pos_x = x, pos_y = y;
    uint32  offset, i;
    uint8  *data_ptr;
    char    ch;
    LCD_INFO_T  lcd_info;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    uint16 *dest_ptr;
    BLOCKCFG_T cfg;

    TF_UartTrace("[LCD W217] :: LCD_DisplayText [ IN ] ");
    LCD_GetInfo (MAIN_LCD_ID,&lcd_info);
    lcd_width = lcd_info.lcd_width;
    lcd_height = lcd_info.lcd_height;
    TF_UartTrace("[LCD W217] :: LCD_DisplayText [ IN 1] ");
    LCD_DisableAllBlock (MAIN_LCD_ID);
    TF_UartTrace("[LCD W217] :: LCD_DisplayText [ IN ] 2");
    SCI_MEMSET ( (void *) lcd_info.lcdbuff_ptr, 0xFF, lcd_info.lcd_width * lcd_info.lcd_height * 2); //lint !e746

    dest_ptr = lcd_info.lcdbuff_ptr;

    TF_UartTrace("[LCD W217] :: LCD_DisplayText [%d, %d]   lcd_buff = 0x%x\n",  x, y, dest_ptr);

    for (i = 0; i < lcd_info.lcd_height * lcd_info.lcd_width; i++)
    {
        *dest_ptr++ = g_color;
    }

    if ( (len==0) || (text == NULL))
    {
        return;
    }

    if (x > (lcd_width-8))
    {
        pos_x = 0;
        pos_y += 16;
    }

    if (pos_y > (lcd_height-16))
    {
        return;
    }

    
    while (len--)
    {
        ch = *text++;
        //SCI_TraceLow("[LCD W217] :: LCD_DisplayText [%c]\n", ch);
        if ( (ch <= 0x20) || (ch & 0x80))
        {
            offset = 0;  // invalid char, use the default char(' ')
        }
        else
        {
            offset = ( (uint16) ( (uint8) ch - 0x20)) * 16;
        }

        data_ptr = (uint8 *) &ascii_8X16[offset];

        // show this char
        for (i = 0; i < 16; i++)
        {
            LCD_Set8Pixels (pos_x, pos_y+i, *data_ptr++);
        }

        pos_x +=8;

        if ( (pos_x + 8) > lcd_width) // Next line.
        {
            pos_y += 16;
            pos_x  = 0;
        }

        if (pos_y > (lcd_height-16))
        {
            break;
        }
    }

    cfg.colorkey         = 0;
    cfg.resolution       = 2;
    cfg.type             = 1;
    cfg.priority         = 0;
    cfg.start_x          = 0;
    cfg.start_y          = 0;
    cfg.end_x            = lcd_info.lcd_width -1;
    cfg.end_y            = lcd_info.lcd_height - 1;
    cfg.width            =  lcd_info.lcd_width;
    cfg.alpha            = 0xff;
    cfg.alpha_sel        = 1;
    cfg.colorkey_en      = FALSE;

    LCD_ConfigBlock (MAIN_LCD_ID, 0, cfg);
    LCD_SetBlockBuffer (MAIN_LCD_ID, 0, lcd_info.lcdbuff_ptr);
    LCD_EnableBlock(MAIN_LCD_ID, 0);
    LCD_InvalidateRect (MAIN_LCD_ID, 0, 0, lcd_info.lcd_width -1, lcd_info.lcd_height -1);
}

PUBLIC uint32 LCD_GetLcdBusWidth(  LCD_ID_E lcd_id)
{
    return s_lcd_spec_info_ptr[lcd_id]->spec_ptr->bus_width;
}

PUBLIC ERR_LCD_E LCD_ConfigBlock (
        LCD_ID_E lcd_id,
        uint32 blk_num,
        BLOCKCFG_T config
        )
{
    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/
    SCI_PASSERT ( (blk_num<BLOCK_SUM), ("blk_num=%d",blk_num));/*assert verified*/

    if ((config.start_x >= config.end_x) || (config.start_y >= config.end_y))
    {
        return ERR_LCD_PARAMETER_WRONG;
    }

    s_buffer_info[lcd_id][blk_num].cfg.alpha = config.alpha;
    s_buffer_info[lcd_id][blk_num].cfg.width = config.width;
    s_buffer_info[lcd_id][blk_num].cfg.type  = config.type;
    s_buffer_info[lcd_id][blk_num].cfg.start_x = config.start_x;
    s_buffer_info[lcd_id][blk_num].cfg.start_y = config.start_y;
    s_buffer_info[lcd_id][blk_num].cfg.end_x = config.end_x;
    s_buffer_info[lcd_id][blk_num].cfg.end_y = config.end_y;
    s_buffer_info[lcd_id][blk_num].cfg.colorkey  = config.colorkey;
    s_buffer_info[lcd_id][blk_num].cfg.resolution = config.resolution;
    s_buffer_info[lcd_id][blk_num].cfg.colorkey_en = config.colorkey_en;
    s_buffer_info[lcd_id][blk_num].cfg.alpha_sel = config.alpha_sel;
    s_buffer_info[lcd_id][blk_num].cfg.rotation = config.rotation;
    s_buffer_info[lcd_id][blk_num].cfg.rb_switch = config.rb_switch;
    s_buffer_info[lcd_id][blk_num].cfg.blend_mode = config.blend_mode;    
    s_buffer_info[lcd_id][blk_num].cfg.grey_rgb = config.grey_rgb;

    TF_UartTrace("[LCD W217] :: config block (%d:%d) -- [%d, %d, %d, %d] -%d \n", lcd_id, blk_num,
            config.start_x, config.start_y, config.end_x, config.end_y, config.resolution);

    return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCD_SetBlockBuffer (
        LCD_ID_E lcd_id,
        uint32 blk_num,
        uint32 *buf_ptr
        )
{
    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/
    SCI_ASSERT (blk_num < BLOCK_SUM);/*assert verified*/
    SCI_ASSERT (buf_ptr != PNULL);/*assert verified*/

    s_buffer_info[lcd_id][blk_num].mem_ptr = buf_ptr;

    return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCD_EnableBlock (LCD_ID_E lcd_id, uint32 blk_num)
{
    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/
    SCI_ASSERT (blk_num<BLOCK_SUM);/*assert verified*/

    s_buffer_info[lcd_id][blk_num].is_enable = SCI_TRUE;

    return ERR_LCD_NONE;
}

PUBLIC  void* LCD_GetMainLCDBuffer(void)
{
    return (void*)s_main_lcd_buffer;
}

PUBLIC  void* LCD_GetSubLCDBuffer(void)
{
    return (void*)s_sub_lcd_buffer;
}

PUBLIC ERR_LCD_E LCD_GetInfo (
        LCD_ID_E lcd_id,            //ID of lcd to operate
        LCD_INFO_T *lcd_info_ptr    //lcd information struct pointer
        )
{
    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/

    lcd_info_ptr->r_bitmask       = 0xf800;
    lcd_info_ptr->g_bitmask       = 0x07e0;
    lcd_info_ptr->b_bitmask       = 0x001f;
    lcd_info_ptr->bits_per_pixel  = 16;
    lcd_info_ptr->contrast_min    = 0x00;
    lcd_info_ptr->contrast_max    = 63;
    lcd_info_ptr->contrast_defaut = 0x0A;
    lcd_info_ptr->is_use_fmark    = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->is_use_fmark;
    lcd_info_ptr->fmark_pol       = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->fmark_pol;

    LCM_GetLcdDevSize (lcd_id,&lcd_info_ptr->lcd_width,&lcd_info_ptr->lcd_height);

    if (MAIN_LCD_ID == lcd_id)
    {
        lcd_info_ptr->lcdbuff_ptr = (void *) LCD_GetMainLCDBuffer();
    }
    else if (SUB_LCD_ID == lcd_id)
    {
        lcd_info_ptr->lcdbuff_ptr = (void *) LCD_GetSubLCDBuffer();
    }

    return ERR_LCD_NONE;
}

#define IS_VIDEO_LAYER(iLcd, iBlock) \
    (iBlock == LCD_LAYER_IMAGE \
     && (s_buffer_info[iLcd][iBlock].cfg.resolution == LCD_RESOLUTION_YUV420 \
         || s_buffer_info[iLcd][iBlock].cfg.resolution == LCD_RESOLUTION_YUV420_3PLANE\
         || s_buffer_info[iLcd][iBlock].cfg.resolution == LCD_RESOLUTION_YUV422))

LOCAL HAL_GOUDA_IMG_FORMAT_T LAYER_FORMAT(uint8 _resolution)
{
    switch(_resolution)
    {
        case LCD_RESOLUTION_RGB888: return HAL_GOUDA_IMG_FORMAT_RGBA;
        case LCD_RESOLUTION_YUV420: return HAL_GOUDA_IMG_FORMAT_IYUV;
        case LCD_RESOLUTION_YUV420_3PLANE: return HAL_GOUDA_IMG_FORMAT_IYUV;
        case LCD_RESOLUTION_YUV422: return HAL_GOUDA_IMG_FORMAT_UYVY;
        case LCD_RESOLUTION_RGB565: default: return HAL_GOUDA_IMG_FORMAT_RGB565;
    }
}

LOCAL void gouda_config_layers(LCD_ID_E lcd_id)
{
    int32 i = 0;
    HAL_GOUDA_OVL_LAYER_ID_T over_layer_use_id = HAL_GOUDA_OVL_LAYER_ID0;

    for (i=0 ; i<LCDC_LAYER_NUM ; i++)
    {
        if(s_buffer_info[lcd_id][i].is_enable)
        {
            HAL_GOUDA_OVL_LAYER_DEF_T over_layer_cfg;

            over_layer_cfg.addr = (uint32 *)s_buffer_info[lcd_id][i].mem_ptr;
            over_layer_cfg.fmt = LAYER_FORMAT(s_buffer_info[lcd_id][i].cfg.resolution);
            over_layer_cfg.stride = 0;
            over_layer_cfg.pos.tlPX = s_buffer_info[lcd_id][i].cfg.start_x;
            over_layer_cfg.pos.tlPY = s_buffer_info[lcd_id][i].cfg.start_y;
            over_layer_cfg.pos.brPX = s_buffer_info[lcd_id][i].cfg.end_x;
            over_layer_cfg.pos.brPY = s_buffer_info[lcd_id][i].cfg.end_y;
            over_layer_cfg.alpha = s_buffer_info[lcd_id][i].cfg.alpha;
            over_layer_cfg.cKeyColor = 0x33; //s_buffer_info[lcd_id][i].cfg.colorkey;
            over_layer_cfg.cKeyEn = FALSE; //s_buffer_info[lcd_id][i].cfg.colorkey_en;

            // open the layer
            hal_GoudaOvlLayerClose(over_layer_use_id);
            hal_GoudaOvlLayerOpen(over_layer_use_id, &over_layer_cfg);
            TF_UartTrace("[LCD W217] :: configure gouda Layer [%d]   = (enable) buff = 0x%x\n",  i, over_layer_cfg.addr);
            over_layer_use_id++;
        }
        else 
        {
            TF_UartTrace("[LCD W217] :: configure gouda Layer [%d]   = (disable)\n",  i);
        }
    }
}

LOCAL void after_gouda_operation(void)
{
    int i;

    for (i=HAL_GOUDA_OVL_LAYER_ID0 ; i<HAL_GOUDA_OVL_LAYER_ID_QTY ; i++)
    {
            hal_GoudaOvlLayerClose(i);
    }
}

typedef struct
{
    /// \FALSE for a command, \TRUE for a data
    BOOLEAN    isData;
    /// value of data or command
    uint16  value;
} GOUDA_LCD_CMD_T;

typedef struct
{
    /// top left point
    uint16  tlPX;
    uint16  tlPY;
    /// bottom right point
    uint16  brPX;
    uint16  brPY;

} GOUDA_WIN_T;

LOCAL void gouda_update(CONST GOUDA_WIN_T *pWin, BOOLEAN bSync)
{
    GOUDA_LCD_CMD_T cmd[13];

    if (!((pWin->tlPX < pWin->brPX) && (pWin->tlPY < pWin->brPY)))
    {
        after_gouda_operation();
        return;
    }

    if(bSync)
    {
        while (0 != hal_GoudaBlitRoi(pWin, 0, cmd, NULL))
            ; //6ms

        while( hal_GoudaIsActive()) 
            ;

        after_gouda_operation();
    }
    else 
    {
        while (0 != hal_GoudaBlitRoi(pWin, 0, cmd, after_gouda_operation));
    }
}

PUBLIC ERR_LCD_E LCD_InvalidateRect (
        LCD_ID_E lcd_id,//ID of lcd to operate
        uint16 left,     //left of the rectangle
        uint16 top,     //top of the rectangle
        uint16 right,     //right of the rectangle
        uint16 bottom    //bottom of the rectangle.
        )
{
    GOUDA_WIN_T winRect;

    TF_UartTrace("[LCD W217] :: InvalidateRect (%d) [%d %d %d %d]\n", lcd_id, left, top, right, bottom);
    s_lcd_index = lcd_id;

    s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation->lcd_InvalidateRect (left, top, right, bottom);

    winRect.tlPX = left;
    winRect.tlPY = top;
    winRect.brPX = right;
    winRect.brPY = bottom;

    gouda_config_layers(lcd_id);
    gouda_update(&winRect, TRUE);

    TF_UartTrace("[LCD W217] :: InvalidateRect [OUT]\n");

    return ERR_LCD_NONE;
}



