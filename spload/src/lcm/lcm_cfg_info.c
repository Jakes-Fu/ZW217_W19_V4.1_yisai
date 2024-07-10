//##!(important notice: do not remove this line)
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
//#include "spload_main.h"
#include "lcm_cfg.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"
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
LOCAL LCM_CFG_INFO_T s_main_lcm_cfg_tab[] =
{
	{ LCD_DRV_ID_ILI9328, 	0, 0, 0, 0, 0, LCD_CTRL_MODULE_LCDC, (LCD_SPEC_T_PTR)&g_lcd_ILI9328,  LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL,"ILI9328"},/*lint !e651 */	
	{ LCD_DRV_ID_HX8357, 	0, 0, 0, 0, 0, LCD_CTRL_MODULE_LCDC, (LCD_SPEC_T_PTR)&g_lcd_HX8357,  LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL,"HX8357"},	
	{ LCD_DRV_ID_HX8347D, 	0, 0, 0, 0, 0, LCD_CTRL_MODULE_LCDC, (LCD_SPEC_T_PTR)&g_lcd_HX8347D,  LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL,"HX8347D"},	
	//{ LCD_DRV_ID_S6D0139, 	0, 0, 0, 0, 0, LCD_CTRL_MODULE_LCDC, &g_lcd_S6D0139,  LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL},
	//{ LCD_DRV_ID_ILI9325, 	0, 0, 0, 2, 16, LCD_CTRL_MODULE_LCDC, (LCD_SPEC_T_PTR)&g_lcd_ILI9325,  LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL},
	//{ LCD_DRV_ID_ILI9320, 	0, 0, 0, 2, 16, LCD_CTRL_MODULE_LCDC, (LCD_SPEC_T_PTR)&g_lcd_ILI9320,  LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL},
	//{ LCD_DRV_ID_SPFD5408, 	0, 0, 0, 2, 16, LCD_CTRL_MODULE_LCDC, (LCD_SPEC_T_PTR)&g_lcd_SPDF5408, LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL},
	//{ LCD_DRV_ID_S6D0139, 	0, 0, 0, 2, 16, LCD_CTRL_MODULE_LCDC, &g_lcd_S6D0139,  LCD_BACKLIGHT_TYPE_BB_ANALOG, 0, 0, 0, PNULL}
	
};


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
	
	return cfg_ptr;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of lcm_cfg_info.c
