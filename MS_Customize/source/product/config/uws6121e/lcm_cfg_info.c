/******************************************************************************
 ** File Name:      lcm_cfg_info.c                                                 *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           01/15/2008                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of LCM   *
 **					about product
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/15/2008     Liangwen.Zhen    Create.                         *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#include "os_api.h"
#include "lcm_cfg.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"
#include "spi_drvapi.h"
#ifdef SPLOAD_ENABLE
#include "spload_cfg.h"
#include "spload_api.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
 // Customer need to configure the tables below according to the special product
/*lint -save -e651*/
LOCAL LCM_CFG_INFO_T s_main_lcm_cfg_tab[] =
{
#ifdef ZDT_PCBA_ZW201_SUPPORT
        #if defined(DRV_LCD_7789P3_240X284)||defined(W217_W18_DRV_LCD_7789P3_240X284)
        { LCD_DRV_ID_ST7789P3, 0, ONE_DATA_LINE_LCD, ENABLE_FMARK_LCD, 0, 0, LCD_CTRL_MODULE_LCM, (LCD_SPEC_T_PTR)&g_lcd_ST7789v2,  LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL,"ST7789P3"},// 7789P3
        #else
	{ LCD_DRV_ID_ST7789V2, 0, ONE_DATA_LINE_LCD, ENABLE_FMARK_LCD, 0, 0, LCD_CTRL_MODULE_LCM, (LCD_SPEC_T_PTR)&g_lcd_ST7789v2,	LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL,"ST7789v2"},
	#endif
	{ LCD_DRV_ID_GC9307, 0, ONE_DATA_LINE_LCD, ENABLE_FMARK_LCD, 0, 0, LCD_CTRL_MODULE_LCM, (LCD_SPEC_T_PTR)&g_lcd_GC9307,	LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL,"GC9307"},   
#else
	//{ LCD_DRV_ID_GC9306, 0, ONE_DATA_LINE_LCD, ENABLE_FMARK_LCD, 0, 0, LCD_CTRL_MODULE_LCM, (LCD_SPEC_T_PTR)&g_lcd_GC9306,  LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL,"GC9306"},
    { LCD_DRV_ID_GC9a01, 0, ONE_DATA_LINE_LCD, ENABLE_FMARK_LCD, 0, 0, LCD_CTRL_MODULE_LCM, (LCD_SPEC_T_PTR)&g_lcd_GC9a01,  LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL,"GC9a01"},
	//{ LCD_DRV_ID_JD9851, 0, TWO_DATA_LINE_LCD, ENABLE_FMARK_LCD, 0, 0, LCD_CTRL_MODULE_LCM, (LCD_SPEC_T_PTR)&g_lcd_JD9851,	LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL,"JD9851"},
	{ LCD_DRV_ID_ST7789V2, 0, ONE_DATA_LINE_LCD, ENABLE_FMARK_LCD, 0, 0, LCD_CTRL_MODULE_LCM, (LCD_SPEC_T_PTR)&g_lcd_ST7789v2,	LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL,"ST7789v2"},
	//{ LCD_DRV_ID_ST7735S, 0, ONE_DATA_LINE_LCD, ENABLE_FMARK_LCD, 0, 0, LCD_CTRL_MODULE_LCM, (LCD_SPEC_T_PTR)&g_lcd_ST7735S,	LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL,"ST7735S"},
#endif
};
/*lint -restore*/


#ifdef PRODUCT_DRIVER_SUB_LCD_ENABLE

LOCAL LCM_CFG_INFO_T s_sub_lcm_cfg_tab[] =
{
	{ LCD_DRV_ID_ILI9328, 	0, 0, 0, 0, 0, LCD_CTRL_MODULE_LCDC, (LCD_SPEC_T_PTR)&g_lcd_ILI9328,  LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL},

};

#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Pblic Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to get LCM configure information
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC LCM_CFG_INFO_T_PTR LCM_GetCfgInfo(uint32 lcd_id, uint32* count)
{
	LCM_CFG_INFO_T_PTR cfg_ptr = PNULL;

	*count = 0;
#ifdef SPLOAD_ENABLE
	cfg_ptr = s_lcm_ext_func_table->_lcm_getcfginfotable( lcd_id, count );
#else
	if(MAIN_LCD_ID == lcd_id)
	{
		*count = sizeof(s_main_lcm_cfg_tab)/sizeof(LCM_CFG_INFO_T);
		cfg_ptr = s_main_lcm_cfg_tab;
	}
	else if( SUB_LCD_ID == lcd_id )
	{
#ifdef PRODUCT_DRIVER_SUB_LCD_ENABLE
		*count = sizeof(s_sub_lcm_cfg_tab)/sizeof(LCM_CFG_INFO_T);
		cfg_ptr = s_sub_lcm_cfg_tab;
#endif
	}
#endif /*SPLOAD_ENABLE*/

	return cfg_ptr;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif  // End of lcm_cfg_info.c
