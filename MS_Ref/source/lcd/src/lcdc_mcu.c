/***************************************************************************************
** File Name:      SC8800H_lcdc_mcu.c                                                      *
** DATE:           19/09/2005                                                          *
** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.                   *
** Description:    this file defines LCDC MCU mode interfaces,                         *
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
#include "ms_ref_lcd_trc.h"
#include "lcdc_data_type.h"
#include "lcdc_common.h"
//#include "chip_plf_export.h"
#include "sc_reg.h"
#include "os_api.h"
#include "tb_comm.h"
#include "ref_outport.h"
#include "arm_mmu.h"
#include "arm_reg.h"
#include "task_monitor.h"
/**-------------------------------------------------------------------------------------*
 **                     Compiler Flag                                                   *
 **-------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define REG32(x)      (*((volatile uint32 *)(x)))
/**********************************************************************************************************/
// extern variable
/**********************************************************************************************************/
extern LCDC_MCU_ADDR_T g_lcdc_mcu_addr[2];
extern LCDC_TYPE_T g_lcdc_type;


#define LCM_AHB_FIFO_TIME_OUT  2000
#define LCDC_MAIN_LCD_ID       0
#define LCDC_SUB_LCD_ID        1

/**********************************************************************************************************/
// Description: initialize lcdc mcu mode using default configure
// Global resource dependence: g_lcd_backdround
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
void lcdc_mcu_init (uint32 mode)
{
    AHB_CTL0_U               *ahb_ptr              = (AHB_CTL0_U *) AHB_CTL0;
    LCDC_LCD_MODE_U          *lcdc_mode_ptr        = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;
    LCDC_LCM_INT_EN_U        *lcdc_int_en_ptr      = (LCDC_LCM_INT_EN_U *) LCDC_LCM_INT_EN;
    LCDC_LCD_BACKGROUND_U    *lcdc_background_ptr  = (LCDC_LCD_BACKGROUND_U *) LCDC_LCD_BACKGROUND;
    LCDC_COLOR_COEFF_A1_U    *lcdc_a1_ptr          = (LCDC_COLOR_COEFF_A1_U *) LCDC_COLOR_COEFF_A1;
    LCDC_COLOR_COEFF_A2_U    *lcdc_a2_ptr          = (LCDC_COLOR_COEFF_A2_U *) LCDC_COLOR_COEFF_A2;
    LCDC_COLOR_COEFF_A3_U    *lcdc_a3_ptr          = (LCDC_COLOR_COEFF_A3_U *) LCDC_COLOR_COEFF_A3;
    LCDC_COLOR_COEFF_B1_U    *lcdc_b1_ptr          = (LCDC_COLOR_COEFF_B1_U *) LCDC_COLOR_COEFF_B1;
    LCDC_COLOR_COEFF_B2_U    *lcdc_b2_ptr          = (LCDC_COLOR_COEFF_B2_U *) LCDC_COLOR_COEFF_B2;
    LCDC_COLOR_COEFF_B3_U    *lcdc_b3_ptr          = (LCDC_COLOR_COEFF_B3_U *) LCDC_COLOR_COEFF_B3;
    LCDC_COLOR_COEFF_C1_U    *lcdc_c1_ptr          = (LCDC_COLOR_COEFF_C1_U *) LCDC_COLOR_COEFF_C1;
    LCDC_COLOR_COEFF_C2_U    *lcdc_c2_ptr          = (LCDC_COLOR_COEFF_C2_U *) LCDC_COLOR_COEFF_C2;
    LCDC_COLOR_COEFF_C3_U    *lcdc_c3_ptr          = (LCDC_COLOR_COEFF_C3_U *) LCDC_COLOR_COEFF_C3;
    LCDC_LCD_INT_ENABLE_U    *lcdc_int_ent_ptr     = (LCDC_LCD_INT_ENABLE_U *) LCDC_LCD_INT_ENABLE;
    LCM_INT_EN_U             *lcm0_int_en_ptr      = (LCM_INT_EN_U *) LCDC_LCM_INT_EN;
    //enable LCDC
    ahb_ptr->mBits.lcdc_eb = 0x1;
    //clear Block register
    {
        uint32 i;

        //0x20700000--0x20700200
        for (i = 0; i < ( (uint32)(LCDC_LCD_BLOCK5CONFIG-LCDC_LCD_BLOCK0ADDR) /4) + 1 ; i++)
        {
            REG32 ((uint32)LCDC_LCD_BLOCK0ADDR + 4*i) = 0;
        }
    }

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

    //set LCDC mode
    lcdc_mode_ptr->mBits.refresh_mode = 0x1;
    lcdc_mode_ptr->mBits.lcdc_mode = mode;
	
	if (LCD_MOD_LCM_SEL)
	{
        lcdc_mode_ptr->mBits.lcm_select = 0x1;  //LCM1 is Selected
        lcdc_mode_ptr->mBits.lcm1_enable = 0x1; //LCM1 Enabled
	}
	else
	{
        ahb_ptr->mBits.lcm_eb = 1;    //enable lcm0
        lcdc_mode_ptr->mBits.lcm_select = 0;//0x1;  //LCM1 is Selected
        lcdc_mode_ptr->mBits.lcm1_enable = 0x0;//0x1; //LCM1 Enabled
        TB_REG_SET (LCM_RW_MD, 0); //set lcm write mode
	}
	
    SCI_Sleep (1);
    TB_REG_SET (LCDC_LCMRSTN,0x1);
    lcdc_mode_ptr->mBits.lcd_enable = 0x1;

    g_lcdc_type.lcdc_refresh_mode = 0x1;    //single frame refresh

    //set default background
    lcdc_background_ptr->dwValue = g_lcdc_type.lcd_background;


    //enable interrupt
	if (LCD_MOD_LCM_SEL)
	{
        lcdc_int_en_ptr->mBits.overflow_en = 0x1;
    }
	else
	{
	    lcm0_int_en_ptr->mBits.overflow_en = 0x1;
	}
	
    lcdc_mode_ptr->mBits.shadow_update = 0x1;
    lcdc_int_ent_ptr->mBits.underflow_int = 0x1;

}//end of lcdc_mcu_init
/**********************************************************************************************************/
// Description: reset LCD throught the LCDC
// Global resource dependence:
// Author: Jianping.wang
// note:
//      reset sequence:1-0(delay)-1
//      LCDC only has one reset-pin
//      delay_ms - delay time(ms)
/**********************************************************************************************************/
void lcdc_mcu_reset (uint32 delay_ms)
{
    TB_REG_SET (LCDC_LCMRSTN,0x1);
    SCI_Sleep (delay_ms);
    TB_REG_SET (LCDC_LCMRSTN,0x0);
    SCI_Sleep (delay_ms);
    TB_REG_SET (LCDC_LCMRSTN,0x1);

}//end of lcdc_mcu_reset
/**********************************************************************************************************/
// Description: configure LCDC RGB mode registers
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          pmcu_lcd_spec - pointer to parameter structure that is related with LCD
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
int32 lcdc_mcu_configure (LCDC_MCU_SPEC_T *pmcu_lcd_spec)
{
    LCDC_LCMMODESEL_U      *lcdc_modesel_ptr = (LCDC_LCMMODESEL_U *) LCDC_LCMMODESEL;
    LCDC_LCM_RGB_MODE_U    *lcdc_rgbmode_ptr = (LCDC_LCM_RGB_MODE_U *) LCDC_LCM_RGB_MODE;
    LCDC_LCM_SELPIN_U      *lcdc_selpin_ptr  = (LCDC_LCM_SELPIN_U *) LCDC_LCM_SELPIN;
    LCDC_LCMPARAMTER0_U    *lcdc_para0_ptr   = (LCDC_LCMPARAMTER0_U *) LCDC_LCMPARAMETER0;
    LCDC_LCMPARAMTER1_U    *lcdc_para1_ptr   = (LCDC_LCMPARAMTER1_U *) LCDC_LCMPARAMETER1;

    if (pmcu_lcd_spec == PNULL)
    {
        return LCD_ERROR_POINTER_NULL;
    }

    pmcu_lcd_spec->parameter0.mBits.res = 0x0;
    pmcu_lcd_spec->parameter1.mBits.res = 0x0;
    pmcu_lcd_spec->modesel.mBits.res    = 0x0;
    pmcu_lcd_spec->rgbmode.mBits.res    = 0x0;

    lcdc_para0_ptr->dwValue = pmcu_lcd_spec->parameter0.value;
    lcdc_para1_ptr->dwValue = pmcu_lcd_spec->parameter1.value;
    
    lcdc_modesel_ptr->dwValue = pmcu_lcd_spec->modesel.value;
    lcdc_rgbmode_ptr->dwValue = pmcu_lcd_spec->rgbmode.value;
    lcdc_selpin_ptr->dwValue  = 0x0;

    return LCD_OK;
}//end of lcdc_mcu_configure
/**********************************************************************************************************/
// Description: enable LCDC mcu mode interrupt
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      osd_int-0:disable OSDDone interrupt;1:enable OSCDone interrupt
/**********************************************************************************************************/
void lcdc_mcu_int_enable (uint32 osd_int)
{
    LCDC_LCM_INT_EN_U    *lcdc_int_en_ptr = (LCDC_LCM_INT_EN_U *) LCDC_LCM_INT_EN;
    LCM_INT_EN_U         *lcm0_int_en_ptr = (LCM_INT_EN_U *) LCDC_LCM_INT_EN;
	
	if (LCD_MOD_LCM_SEL)
	{
        lcdc_int_en_ptr->mBits.osd_done_en = osd_int;
	}
	else
	{
        lcm0_int_en_ptr->mBits.osd_done_en = osd_int;
	}

}//end of lcdc_mcu_int_enable
/**********************************************************************************************************/
// Description: send command and data to LCD
// Global resource dependence: g_main_lcd_cd,g_sub_lcd_cd
// Author: Jianping.wang
// Note:
//      input:
//          cmd - command
//          data - command data
//          lcd_id - LCD ID
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
int32 lcdc_mcu_send_cmd_data (uint32 cmd,uint32 lcd_data,uint32 lcd_id)
{
    uint32 cmd_addr;
    uint32 data_addr;
    uint32 start_time = 0;
    uint32 end_time   = 0;

    cmd_addr  = g_lcdc_mcu_addr[lcd_id].cmd_addr;
    data_addr = g_lcdc_mcu_addr[lcd_id].data_addr;

    start_time = SCI_GetTickCount();

    while (REG32 (LCDC_LCMSTATUS) & BIT_1)
    {
        end_time = SCI_GetTickCount();
        SCI_ASSERT ( (end_time - start_time) < LCM_AHB_FIFO_TIME_OUT);
    }

    TB_REG_SET (cmd_addr, cmd);
    TB_REG_SET (data_addr,lcd_data);

    return LCD_OK;

}//end of lcdc_mcu_send_cmd_data
/**********************************************************************************************************/
// Description: send command to LCD
// Global resource dependence: g_main_lcd_cd,g_sub_lcd_cd
// Author: Jianping.wang
// Note:
//      input:
//          cmd - command
//          lcd_id - LCD ID
//      return:
//          LCD_OK,if succecc
//          onter, if fail
/**********************************************************************************************************/
int32 lcdc_mcu_send_cmd (uint32 cmd,uint32 lcd_id)
{
    uint32 cmd_addr;
    uint32 start_time = 0;
    uint32 end_time   = 0;

    cmd_addr = g_lcdc_mcu_addr[lcd_id].cmd_addr;

    start_time = SCI_GetTickCount();

    while (REG32 (LCDC_LCMSTATUS) & BIT_1)
    {
        end_time = SCI_GetTickCount();
        SCI_ASSERT ( (end_time - start_time) < LCM_AHB_FIFO_TIME_OUT);
    }

    TB_REG_SET (cmd_addr,cmd);

    return LCD_OK;
}//end of lcdc_mcu_send_cmd
/**********************************************************************************************************/
// Description: send display data to LCD
// Global resource dependence: g_main_lcd_cd,g_sub_lcd_cd
// Author: Jianping.wang
// Note:
//      input:
//          data - display data
//          lcd_id - LCD ID
//      return:
//          LCD_OK,if succecc
//          other, if fail
/**********************************************************************************************************/
int32 lcdc_mcu_send_data (uint32 lcd_data,uint32 lcd_id)
{
    uint32 data_addr;
    uint32 start_time = 0;
    uint32 end_time   = 0;

    data_addr = g_lcdc_mcu_addr[lcd_id].data_addr;

    start_time = SCI_GetTickCount();

    while (REG32 (LCDC_LCMSTATUS) & BIT_1)
    {
        end_time = SCI_GetTickCount();
        SCI_ASSERT ( (end_time - start_time) < LCM_AHB_FIFO_TIME_OUT);
    }

    TB_REG_SET (data_addr,lcd_data);

    return LCD_OK;
}//end of lcdc_mcu_send_data
/**********************************************************************************************************/
// Description: start LCDC to wirte display data to LCD GRAM
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          lcd_width - lcd width
//          lcd_height - lcd height
//          lcd_id - LCD ID
//      return:
//          LCD_OK,if success
//          other, if fail
/**********************************************************************************************************/
int32 lcdc_mcu_transcmd (uint16 lcd_width, uint16 lcd_height,uint32 lcd_id)
{
    LCDC_LCD_SIZE_U         *lcdc_size_ptr    = (LCDC_LCD_SIZE_U *) LCDC_LCD_SIZE;
    LCDC_LCD_MODE_U         *lcdc_mode_ptr    = (LCDC_LCD_MODE_U *) LCDC_LCD_MODE;
    LCDC_LCMDATANUMBER_U    *lcdc_datanum_ptr = (LCDC_LCMDATANUMBER_U *) LCDC_LCMDATANUMBER;
    uint32 reg_value;
    uint32 cs_pin = 0;

    if (lcd_id == LCDC_MAIN_LCD_ID)
    {
        cs_pin = g_lcdc_type.main_lcd_cs_pin;
    }
    else if (lcd_id == LCDC_SUB_LCD_ID)
    {
        cs_pin = g_lcdc_type.sub_lcd_cs_pin;
    }
    else
    {
        //SCI_TRACE_LOW:"lcdc_mcu_transcmd lcd_id>1"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_MCU_326_112_2_18_1_45_41_180,(uint8*)"");
    }

    //SCI_ASSERT ( (lcd_id == 0) || (lcd_id == 1));
    //SCI_ASSERT (! (lcd_width % 8));

    TM_SendTestPointRequest (0x1080,lcd_width);

    lcdc_mode_ptr->mBits.shadow_update = 0x1;
    reg_value = lcd_width * lcd_height;
    SCI_ASSERT ( (reg_value%8 == 0));
    SCI_ASSERT (reg_value >= LCDC_UPDATE_PIXELS_MIN);

    reg_value |= (1 << 20);
    reg_value |= (lcd_id << 26);
    lcdc_datanum_ptr->dwValue = reg_value;

    MMU_InvalideDCACHE();
    //start LCDC refresh
    lcdc_mode_ptr->mBits.start_refresh = 0x1;

    return LCD_OK;
}//end of lcdc_mcu_transcmd

/**********************************************************************************************************/
// Description: get LCDC MCU mode configure information
// Global resource dependence: g_main_lcd_cd,g_sub_lcd_cd,g_mcu_lcd_sum
// Author: Jianping.wang
// Note:
//      input:
//          none
//      output:
//          info_ptr - pointer to information structure
//      return:
//          LCD_OK,if success
//          other, if fail
/**********************************************************************************************************/
int32 lcdc_mcu_get_info (LCDC_MCU_INFO_T *info_ptr)
{
    SCI_ASSERT (info_ptr != PNULL);

    info_ptr->parameter0.value = TB_REG_GET (LCDC_LCMPARAMETER0);
    info_ptr->parameter1.value = TB_REG_GET (LCDC_LCMPARAMETER1);
    info_ptr->modesel.value    = TB_REG_GET (LCDC_LCMMODESEL);
    info_ptr->rgbmode.value    = TB_REG_GET (LCDC_LCM_RGB_MODE);
    info_ptr->lcd_cd[0]        = g_lcdc_type.main_lcd_cd;
    info_ptr->lcd_cd[1]        = g_lcdc_type.sub_lcd_cd;
    info_ptr->lcd_sum          = g_lcdc_type.mcu_lcd_sum;

    return LCD_OK;
}//end of lcdc_mcu_get_info
/**********************************************************************************************************/
// Description: LCDC remain data counter that isn't transfered to GRAM of LCD
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          none
//      output:
//          data_num_ptr - pointer to remain data number
//      return:
//          LCD_OK,if success
//          others, if fail
/**********************************************************************************************************/
int32 lcdc_mcu_remain_datacounter (uint32 *data_num_ptr)
{
    SCI_ASSERT (data_num_ptr != PNULL);

    *data_num_ptr = TB_REG_GET (LCDC_LCM_REMAIN);

    return LCD_OK;
}//end of lcdc_mcu_remain_datacounter
/**********************************************************************************************************/
// Description: read LCD index register of lcd
// Global resource dependence: g_main_lcd_cd,g_sub_lcd_cd
// Author: Jianping.wang
// Note:
//      input:
//          lcd_id - LCD ID
//          lcd_index - LCD index register
//      output:
//          data_ptr - pointer to data
//      return:
//          LCD_OK,if success
//          others,if fail
/**********************************************************************************************************/
int32 lcdc_mcu_read_lcdindex (uint32 *data_ptr,uint32 lcd_index,uint32 lcd_id)
{
    uint32 cmd_addr;
    TB_REG_SET (LCM_RW_MD, 0xF); //set lcm0 read mode
    cmd_addr = g_lcdc_mcu_addr[lcd_id].cmd_addr;

    TB_REG_AND (g_lcdc_mcu_addr[lcd_id].data_addr, (0<<18));
    TB_REG_SET (cmd_addr,lcd_index);
    TB_REG_OR (g_lcdc_mcu_addr[lcd_id].data_addr, (1<<18));
    TB_REG_SET (g_lcdc_mcu_addr[lcd_id].data_addr, (1<<18));

    SCI_Sleep (1);

    *data_ptr = TB_REG_GET (LCDC_LCMRDDATA);
    TB_REG_AND (g_lcdc_mcu_addr[lcd_id].data_addr, (0<<18));
    TB_REG_SET (LCM_RW_MD, 0); //set lcm0 write mode
    return LCD_OK;
}//end of lcdc_mcu_read_lcdindex
/**********************************************************************************************************/

/**-------------------------------------------------------------------------------------*
 **                     Compiler Flag                                                   *
 **-------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif


//end of SC8800H_lcdc_mcu.c



