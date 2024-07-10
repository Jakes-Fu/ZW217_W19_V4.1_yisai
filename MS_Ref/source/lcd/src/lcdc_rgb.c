/***************************************************************************************
** File Name:      SC8800H_lcdc_rgb.c                                                      *
** DATE:           19/09/2005                                                          *
** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.                   *
** Description:    this file defines LCDC RGB mode interfaces,                         *
**                 these interfaces is don't visible for LCDC's user                   *
****************************************************************************************

****************************************************************************************
**                         Edit History                                                *
**-------------------------------------------------------------------------------------*
** DATE                    DESCRIPTION                                                 *
** 19/09/2005              Create.                                                     *
****************************************************************************************/

/**------------------------------------------------------------------------------------*
 **                         Dependencies                                               *
 **------------------------------------------------------------------------------------*/
#include "lcdc_data_type.h"
#include "lcdc_common.h"
//#include "chip_plf_export.h"
#include "sc_reg.h"

#include "os_api.h"
#include "tb_comm.h"
#include "ref_outport.h"
/**------------------------------------------------------------------------------------*
 **                     Compiler Flag                                                  *
 **------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define REG32(x)      (*((volatile uint32 *)(x)))
/**********************************************************************************************************/
// EXTERN VARIABLE
/**********************************************************************************************************/
extern LCDC_TYPE_T g_lcdc_type;

/**********************************************************************************************************/
// Description: initialize lcdc using default configure
// Global resource dependence: g_main_lcd_width,g_main_lcd_height,g_lcd_background
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
void lcdc_rgb_init (uint32 mode)
{
    AHB_CTL0_U               *ahb_ptr             = (AHB_CTL0_U *) AHB_CTL0;
    LCDC_LCD_MODE_U          *lcdc_mode_ptr       = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;
    LCDC_LCD_SIZE_U          *lcdc_size_ptr       = (LCDC_LCD_SIZE_U *) LCDC_LCD_SIZE;
    LCDC_LCD_INT_ENABLE_U    *lcdc_int_en_ptr     = (LCDC_LCD_INT_ENABLE_U *) LCDC_LCD_INT_ENABLE;
    LCDC_LCD_BACKGROUND_U    *lcdc_background_ptr = (LCDC_LCD_BACKGROUND_U *) LCDC_LCD_BACKGROUND;
    LCDC_COLOR_COEFF_A1_U    *lcdc_a1_ptr         = (LCDC_COLOR_COEFF_A1_U *) LCDC_COLOR_COEFF_A1;
    LCDC_COLOR_COEFF_A2_U    *lcdc_a2_ptr         = (LCDC_COLOR_COEFF_A2_U *) LCDC_COLOR_COEFF_A2;
    LCDC_COLOR_COEFF_A3_U    *lcdc_a3_ptr         = (LCDC_COLOR_COEFF_A3_U *) LCDC_COLOR_COEFF_A3;
    LCDC_COLOR_COEFF_B1_U    *lcdc_b1_ptr         = (LCDC_COLOR_COEFF_B1_U *) LCDC_COLOR_COEFF_B1;
    LCDC_COLOR_COEFF_B2_U    *lcdc_b2_ptr         = (LCDC_COLOR_COEFF_B2_U *) LCDC_COLOR_COEFF_B2;
    LCDC_COLOR_COEFF_B3_U    *lcdc_b3_ptr         = (LCDC_COLOR_COEFF_B3_U *) LCDC_COLOR_COEFF_B3;
    LCDC_COLOR_COEFF_C1_U    *lcdc_c1_ptr         = (LCDC_COLOR_COEFF_C1_U *) LCDC_COLOR_COEFF_C1;
    LCDC_COLOR_COEFF_C2_U    *lcdc_c2_ptr         = (LCDC_COLOR_COEFF_C2_U *) LCDC_COLOR_COEFF_C2;
    LCDC_COLOR_COEFF_C3_U    *lcdc_c3_ptr         = (LCDC_COLOR_COEFF_C3_U *) LCDC_COLOR_COEFF_C3;
    //enable LCDC
    ahb_ptr->mBits.lcdc_eb = 0x1;

    //set LCDC mode
    lcdc_mode_ptr->mBits.lcdc_mode = mode;
    lcdc_mode_ptr->mBits.lcd_enable = 0x1;

    //set LCD size
    lcdc_size_ptr->mBits.lcd_size_x =   g_lcdc_type.main_lcd_width - 1;
    lcdc_size_ptr->mBits.lcd_size_y = g_lcdc_type.main_lcd_height - 1;
    lcdc_size_ptr->mBits.reserved_1 = 0x0;
    lcdc_size_ptr->mBits.reserved_2 = 0x0;

    //set default background
    lcdc_background_ptr->dwValue = g_lcdc_type.lcd_background;

    //set color correction coefficient
    lcdc_a1_ptr->dwValue = COLORMATRIX_A1;
    lcdc_a2_ptr->dwValue = COLORMATRIX_A2;
    lcdc_a3_ptr->dwValue = COLORMATRIX_A3;
    lcdc_b1_ptr->dwValue = COLORMATRIX_B1;
    lcdc_b2_ptr->dwValue = COLORMATRIX_B2;
    lcdc_b3_ptr->dwValue = COLORMATRIX_B3;
    lcdc_c1_ptr->dwValue = COLORMATRIX_C1;
    lcdc_c2_ptr->dwValue = COLORMATRIX_C2;
    lcdc_c3_ptr->dwValue = COLORMATRIX_C3;

    //enable Async FIFO underflow interrupt
    lcdc_int_en_ptr->mBits.underflow_int = 0x1;

    //update register
    lcdc_mode_ptr->mBits.shadow_update = 0x1;

    //SPI3
    REG32 (0x8b00002c) |= 0x400000;
    REG32 (0x8b00002c) &= 0xffdfffff;

    REG32 (0x8b000018) |= (0x1<<14);

    TB_REG_SET (SC8800H_UART_IEN,0x1b67);
    TB_REG_SET (SC8800H_UART_CTL0,0x4430);

    REG32 (0x8b00002c) |= 0x00c00000;
    REG32 (0x8b00002c) &= 0xffdfffff;

    REG32 (0x8b00002c) |= 0x40000000;
    REG32 (0x8b000018) |= 0x4000;

    REG32 (0x8f000028) |= (0x1<<12);
    REG32 (0x8f000028) &= 0xfffff0ff;
    REG32 (0x8f00002c) = 0x0;

    g_lcdc_type.lcdc_refresh_mode = 0;//continue refresh

}//end of lcdc_rgb_init
/**********************************************************************************************************/
// Description: reset LCD throught the LCDC
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      reset bit default is 1;
//      when reset lcd, write sequence is 0-1
/**********************************************************************************************************/
void lcdc_rgb_reset (void)
{
    LCDC_LCD_CTRL_REG_U    *lcdc_lcd_ctrl_ptr = (LCDC_LCD_CTRL_REG_U *) LCDC_LCD_CTRL_REG;
    LCDC_LCD_MODE_U        *lcdc_mode_ptr     = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;

    lcdc_lcd_ctrl_ptr->mBits.rgb_lcd_reset = 0x0;
    lcdc_mode_ptr->mBits.shadow_update     = 0x1;

    SCI_Sleep (600);
    lcdc_lcd_ctrl_ptr->mBits.rgb_lcd_reset = 0x1;
    lcdc_mode_ptr->mBits.shadow_update     = 0x1;

}//end of lcdc_rgb_reset
/**********************************************************************************************************/
// Description: configure LCDC RGB mode registers
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          rgb_lcd_spec_ptr - parameter struct pointer,LCDC RGB mode needs parameters that is related with LCD
//      return:
//          LCD_OK: success
//          others: fail
/**********************************************************************************************************/
int32 lcdc_rgb_configure (LCDC_RGB_SPEC_T *rgb_lcd_spec_ptr)
{
    LCDC_LCD_MODE_U    *lcdc_mode_ptr = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;

    SCI_ASSERT (rgb_lcd_spec_ptr != PNULL);


    SCI_ASSERT (rgb_lcd_spec_ptr->lcd_ctrl.mBits.lcd_bpp != 0x3);

    rgb_lcd_spec_ptr->lcd_ctrl.mBits.res   = 0x4;  //when default,RGB reset bit is 1
    rgb_lcd_spec_ptr->lcd_ctrl.mBits.res_1 = 0x0;  //bypass scalingUp mode

    TB_REG_SET (LCDC_LCD_CTRL_REG,rgb_lcd_spec_ptr->lcd_ctrl.value);
    lcdc_mode_ptr->mBits.shadow_update = 0x1;

    return LCD_OK;
}//end of lcdc_rgb_configure
/**********************************************************************************************************/
// Description: enable LCDC RGB mode interrupt
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          int_ptr - pointer to interrupt structure
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
int32 lcdc_rgb_int_enable (LCDC_RGB_INT_ENA_T *int_ptr)
{
    LCDC_LCD_INT_ENABLE_U    *lcdc_int_en_ptr = (LCDC_LCD_INT_ENABLE_U *) LCDC_LCD_INT_ENABLE;
    LCDC_LCD_MODE_U          *lcdc_mode_ptr   = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;

    SCI_ASSERT (int_ptr != PNULL);

    lcdc_int_en_ptr->mBits.end_of_frame   = int_ptr->end_of_frame;
    lcdc_int_en_ptr->mBits.start_of_frame = int_ptr->start_of_frame;

    //update register
    lcdc_mode_ptr->mBits.shadow_update = 0x1;

    return LCD_OK;
}//end of lcdc_rgb_int_enable
/**********************************************************************************************************/
// Description: send the command and the command data to LCD
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      cmd      - LCD command
//      lcd_data - LCD command data
/**********************************************************************************************************/
void lcdc_rgb_send_cmd_data (uint32 cmd,uint32 lcd_data)
{
    TB_REG_SET (SC8800H_LCDC_SPI,cmd);
    TB_REG_SET (SC8800H_LCDC_SPI,lcd_data);
}//end of lcdc_rgb_send_cmd_data
/**********************************************************************************************************/
// Description: send command to LCD
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
void lcdc_rgb_send_cmd (uint32 cmd)
{
    TB_REG_SET (SC8800H_LCDC_SPI,cmd);
}//end of lcdc_rgb_send_cmd
/**********************************************************************************************************/
// Description: start LCDC transfer display data to LCD
// Global resource dependence: g_main_lcd_height,g_main_lcd_width
// Author: Jianping.wang
// Note:
//      input:
//          lcd_width - lcd display width
//          lcd_height - lcd display height
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
int32 lcdc_rgb_transcmd (uint16 lcd_width,uint16 lcd_height)
{
    LCDC_LCD_SIZE_U    *lcdc_size_ptr = (LCDC_LCD_SIZE_U *) LCDC_LCD_SIZE;
    LCDC_LCD_MODE_U    *lcdc_mode_ptr = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;

    if ( (lcd_width > g_lcdc_type.main_lcd_width) || (lcd_height > g_lcdc_type.main_lcd_height))
    {
        return LCD_ERROR_LCD_DISPLAY_SIZE;
    }

    lcdc_size_ptr->mBits.lcd_size_x = lcd_width - 1;
    lcdc_size_ptr->mBits.lcd_size_y = lcd_height - 1;
    lcdc_size_ptr->mBits.reserved_1 = 0x0;
    lcdc_size_ptr->mBits.reserved_2 = 0x0;

    //start LCDC
    lcdc_mode_ptr->mBits.shadow_update = 0x1;
    lcdc_mode_ptr->mBits.start_refresh = 0x1;

    return LCD_OK;
}//end of lcdc_rgb_transcmd
/**********************************************************************************************************/
// Description: handle lcdc rgb mode interrupt
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
void lcdc_rgb_isr_handle (void)
{
    LCDC_LCD_INT_CLEAR_U    *lcdc_int_clear_ptr = (LCDC_LCD_INT_CLEAR_U *) LCDC_LCD_INT_CLEAR;
    LCDC_LCD_MODE_U         *lcdc_mode_ptr      = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;
    uint32 int_status;

    int_status = TB_REG_GET (LCDC_LCD_STATUS);

    if (0x1== (int_status&0x1))
    {
        lcdc_int_clear_ptr->mBits.underflow_int = 0x1;
        lcdc_mode_ptr->mBits.shadow_update      = 0x1;
    }
    else if (0x2== (int_status&0x2))
    {
        lcdc_int_clear_ptr->mBits.end_of_frame = 0x1;
        lcdc_mode_ptr->mBits.shadow_update     = 0x1;
    }
    else if (0x4== (int_status&0x4))
    {
        lcdc_int_clear_ptr->mBits.start_of_frame = 0x1;
        lcdc_mode_ptr->mBits.shadow_update       = 0x1;
    }
}//end of lcdc_rgb_isr_handle
/**********************************************************************************************************/
// Description: get LCDC RGB mode configure information
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          none
//      output:
//          info_ptr - LCDC rgb mode LCD infomation
//      return:
//          LCD_OK,if success
//          others,if fail
/**********************************************************************************************************/
int32 lcdc_rgb_get_info (LCDC_RGB_INFO_T *info_ptr)
{
    SCI_ASSERT (info_ptr != PNULL);

    if (info_ptr == PNULL) /*lint !e774*/
    {
        return LCD_ERROR_POINTER_NULL;
    }

    info_ptr->timing0.value  = TB_REG_GET (LCDC_LCD_TIMING0);
    info_ptr->timing1.value  = TB_REG_GET (LCDC_LCD_TIMING1);
    info_ptr->lcd_ctrl.value = TB_REG_GET (LCDC_LCD_CTRL_REG);

    return LCD_OK;
}//end of lcdc_rgb_get_info
/**********************************************************************************************************/
// Description: read lcd index register
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          cmd - index register of lcd
//      output:
//          data_ptr - pointer to index register value of lcd
//      return:
//          LCD_OK,if success
//          other, if fail
/**********************************************************************************************************/
int32 lcdc_rgb_read_lcdindex (uint32 *data_ptr,uint32 cmd)
{
    SCI_ASSERT (data_ptr != PNULL);

    TB_REG_SET (SC8800H_LCDC_SPI,cmd);
    *data_ptr = TB_REG_GET (SC8800H_LCDC_SPI);

    return LCD_OK;
}//end of lcdc_rgb_read_lcdindex
/**********************************************************************************************************/
// Description: start lcdc rgb mode work in single frame
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
void lcdc_rgb_start_singleframe (void)
{
    LCDC_LCD_MODE_U    *lcdc_mode_ptr = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;

    lcdc_mode_ptr->mBits.refresh_mode  = 0x1;
    lcdc_mode_ptr->mBits.shadow_update = 0x1;
    lcdc_mode_ptr->mBits.start_refresh = 0x1;

}//end of lcdc_rgb_start_singleframe
/**********************************************************************************************************/
// Description: get the current frame status
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          none
//      output:
//          odd_frame_ptr - pointer to frame status
//          *iSOddFrame = 1,the current frame is odd frame
//          *iSOddFrame = 0,the current frame is even frame
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
int32 lcdc_rgb_get_framestatus (uint32 *odd_frame_ptr)
{
    SCI_ASSERT (odd_frame_ptr != PNULL);

    *odd_frame_ptr = TB_REG_GET (LCDC_LCD_STATUS);

    return LCD_OK;
}//end of lcdc_rgb_get_framestatus
/**********************************************************************************************************/

/**-------------------------------------------------------------------------------------*
 **                     Compiler Flag                                                   *
 **-------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif


//end of SC8800H_lcdc_rgb.c










