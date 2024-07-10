/******************************************************************************
 ** File Name:    lcdc_drv.h                                                  *
 ** Author:       Tim.zhu                                                     *
 ** DATE:         06/01/2009                                                  *
 ** Copyright:    2009 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/01/2009    Tim.zhu   Create.                                           *
 ******************************************************************************/
#ifndef _LCDC_DRV_H_
#define _LCDC_DRV_H_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "sc_reg.h"
#include "lcdc_types.h"

#ifdef PLATFORM_SC8800G
#include "dal_lcd.h"
#endif
/**---------------------------------------------------------------------------*
 **                          Compiler Flag                                    *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/
#define LCDC_ZERO       0
#define LCDC_ONE        1
#define LCDC_TWO        2
#define LCDC_THREE      3
#define LCDC_FOUR       4

#define LCDC_MAX_WIDTH  1023
#define LCDC_MAX_HEIGHT 1023

/**----------------------------------------------------------------------------*
**                         Functions                                          **
**----------------------------------------------------------------------------*/


/*************************************************************************************/
//Description: register lcdc module irq handle func
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC void LCDC_DrvIrq (void);
/*************************************************************************************/
//Description: wait the interrupt
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC BOOLEAN LCDC_DrvWaitIntDone (LCD_INT_TYPE_E irq_num, uint32 wait_time);
/*************************************************************************************/
//Description: open lcdc irq
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvOpenInt (LCD_INT_TYPE_E irq_num, LCD_IRQ_FUNC irq_func);
/*************************************************************************************/
//Description: close lcdc irq
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvCloseInt (LCD_INT_TYPE_E irq_num);
/*************************************************************************************/
//Description: clean lcdc irq
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvCleanInt (LCD_INT_TYPE_E irq_num);
/*************************************************************************************/
//Description: get status of INT
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC BOOLEAN LCDC_DrvGetIntStatus (LCD_INT_TYPE_E irq_num);

/*************************************************************************************/
//Description: top lcdc reset
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvReset (void);
/*************************************************************************************/
//Description: lcdc enable
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvEnable (BOOLEAN enable);

/*************************************************************************************/
//Description: lcdc start
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvRun (void);
/*************************************************************************************/
//Description: set lcd frame mark, mode: 0, fmark device; 
//              1: non-fmark device, pol: 0, fmark valid at 0,
//              1, fmark valid at 1
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetFmark (LCD_FMARK_MODE_E mode, LCD_FMARK_POL_E pol);
/*************************************************************************************/
//Description: lcdc set back ground param
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetBGColor (uint32 bg_color);
/*************************************************************************************/
//Description: set the interval between 2 AHB master requests for each client.
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetAhbReqestGap (uint8 cycles);
/*************************************************************************************/
//Description: lcdc set back ground param
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetDitherEnable (BOOLEAN is_dither);
/*************************************************************************************/
//Description: set ahb master writ data addr
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetAhbMasterAddr (LCD_LAYER_ID_E layer_id, 
                                                    uint32 y_addr, 
                                                    uint32 uv_addr
                                                  );
/*************************************************************************************/
//Description: set ahb master writ data addr
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetAlphaBaseAddr (LCD_LAYER_ID_E layer_id, uint32 base_addr);
/*************************************************************************************/
//Description: set lcdc layer data endian
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetEndian (LCD_LAYER_ID_E layer_id, uint32 endian_switch);
/*************************************************************************************/
//Description: set lcdc layer data format
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetDataFormat (LCD_LAYER_ID_E layer_id, 
                                              LCD_DATA_FORMAT_E data_format
                                             );
/*************************************************************************************/
//Description: set lcdc layer enavle
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetLayerEnable (LCD_LAYER_ID_E layer_id, BOOLEAN is_enable);
/*************************************************************************************/
//Description: set lcdc logic size
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetLCDCSize (LCDC_SIZE_T *size);
/*************************************************************************************/
//Description: set lcdc corp pitch
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetPitch (LCD_LAYER_ID_E layer_id, uint32 pitch);
/*************************************************************************************/
//Description: set lcdc blk rect
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetLayerRect (LCD_LAYER_ID_E layer_id, LCDC_RECT_T *disp_rect);
/*************************************************************************************/
//Description: set lcdc layer blend postion
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetAlphaMode (LCD_LAYER_ID_E layer_id, 
                                             LCD_OSD_ALPHA_E alpha_mode
                                             );
/*************************************************************************************/
//Description: set lcdc layer blend postion
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetAlphaValue (LCD_LAYER_ID_E layer_id, uint8 alpha_value);
/*************************************************************************************/
//Description: set lcdc layer blend postion
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetGreyRgb (LCD_LAYER_ID_E layer_id, uint32 grey_value);
/*************************************************************************************/
//Description: set lcdc layer blend postion
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetColorKeyEnable (LCD_LAYER_ID_E layer_id, 
                                                   BOOLEAN is_enable
                                                  );
/*************************************************************************************/
//Description: set lcdc layer blend postion
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetColorkey (LCD_LAYER_ID_E layer_id, uint32 color_key);
/*************************************************************************************/
//Description: lcdc cap enable
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetCapEnable (BOOLEAN is_enable);
/*************************************************************************************/
//Description: lcdc cap disable
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetCapRect (LCDC_RECT_T *rect);
/*************************************************************************************/
//Description: set address of capture
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetCapAddr (uint32 addr);
/*************************************************************************************/
//Description: set data format of capture
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetCapDataFormat (LCD_DATA_FORMAT_E data_format);
/*************************************************************************************/
//Description: set data endian of capture
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetCapDataEndian (uint32 endian_switch);
/*************************************************************************************/
//Description: set data format of capture
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetCapPitch (uint32 pitch);
/*************************************************************************************/
//Description: lcdc cap disable
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvGetMaxWidth (void);
/*************************************************************************************/
//Description: lcdc cap disable
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvGetMaxHeight (void);
/*************************************************************************************/
//Description: lcdc cap disable
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvCapDisable (void);
/*************************************************************************************/
//Description: set lcdc trim rect
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetLcmRect (LCDC_RECT_T *rect_ptr);
/*************************************************************************************/
//Description: set upsample mode of yuv to rgb, 0: duplicate, 1: average
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetY2RUpsampleMode (uint32 mode);
/*************************************************************************************/
//Description: set contrast of yuv to rgb, 0-255
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetY2RContrast (uint8 contrast);
/*************************************************************************************/
//Description: set saturation of yuv to rgb, 0-255
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetY2RSaturation (uint8 saturation);
/*************************************************************************************/
// Description: set brightness of yuv to rgb, 0-255
// Global resource dependence:
// Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetY2RBrightness (int16 brightness);
/*************************************************************************************/
//Description: set high address of block memory
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetAhbHiAddr (uint32 hi_addr);
/*************************************************************************************/
//Description: set lcdc layer enavle
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetLayerBlockEnable (LCD_LAYER_ID_E layer_id, 
                                                      BOOLEAN is_enable
                                                     );
/*************************************************************************************/
//Description: disable all layer
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvDisableAllLayer (void);
/*************************************************************************************/
//Description: clean lcdc raw state
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvCleanRawState (LCD_INT_TYPE_E irq_num);

#ifdef PLATFORM_SC8800G
/*************************************************************************************/
//Description:    set cs address
//Global resource dependence:
//Author: Jianping.wang
/*************************************************************************************/
PUBLIC void LCDC_DrvSetCSAddr (uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id);
/*************************************************************************************/
//Description:    reset LCD
//Global resource dependence:
//Author: Jianping.wang
/*************************************************************************************/
PUBLIC void LCDC_DrvResetLcd (uint32 delay_ms,uint8 rst_level);
/*************************************************************************************/
//Description:  set dest LCD's RGB mode to host
//Global resource dependence:
//Author: Younger.Yang
/*************************************************************************************/
PUBLIC void LCDC_DrvSetRGBMode (uint32 cs_id, LCD_BUS_WIDTH_E rgb_mode);
/*************************************************************************************/
//Description:change pulse width
//Global resource dependence:
//Author: Jianping.wang
/*************************************************************************************/
PUBLIC ERR_LCD_E LCDC_DrvSetTiming (LCD_SPEC_INFO_T_PTR lcd_spec_ptr, 
                                           uint32 ahb_clk
                                          );
/*************************************************************************************/
//Description: set lcm ctrl
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_DrvSetLCMCtrl (uint32 setting);
/*************************************************************************************/
//Description:    send command and data to LCD
//Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
//Author: Jianping.wang
/*************************************************************************************/
PUBLIC int32 LCDC_SendCmdData (uint32 cmd,uint32 data,uint32 lcd_id);
/*************************************************************************************/
//Description:    send command to LCD
//Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
//Author: Jianping.wang
/*************************************************************************************/
PUBLIC int32 LCDC_SendCmd (uint32 cmd,uint32 lcd_id);
/*************************************************************************************/
//Description:    send data to LCD GRAM
//Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
//Author: Jianping.wang
/*************************************************************************************/
PUBLIC int32 LCDC_SendData (uint32 data,uint32 lcd_id);
/*************************************************************************************/
//Description:   Read LCM
//Global resource dependence:
//Author: Younger.Yang
/*************************************************************************************/
PUBLIC uint32 LCDC_Read (uint32 lcd_id, uint32 cd_value);
/********************************************************************************************/
//Description:set timing for read lcd id  
//Global resource dependence:    
//Author: Jianping.wang
/********************************************************************************************/
PUBLIC ERR_LCD_E LCDC_DrvSetTimingInit(void);
#endif
/********************************************************************************************/
//Description:   set LCDC Clock
//Global resource dependence:
//Author: Younger.Yang
//Note: 
/********************************************************************************************/
PUBLIC void LCDC_SetClock(LCDC_CLOCK_E clock_type);
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/


#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
