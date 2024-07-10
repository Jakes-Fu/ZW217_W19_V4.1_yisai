#ifndef _DISPLAY_CFG_H
#define _DISPLAY_CFG_H

#include "lcd_cfg.h"
#include "sci_types.h"
#include "dal_lcd.h"

/*****************************************************************************/
//  Description:  get display number of layer on specified lcd
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
uint32 DISPLAYCFG_GetLayerNum(LCD_ID_E lcd_id);
/*****************************************************************************/
//  Description:  get display memory  address of layer on specified lcd
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
uint32 DISPLAYCFG_GetMemAddr(LCD_ID_E lcd_id);
/*****************************************************************************/
//  Description:  get display memory size of layer on specified lcd
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
uint32 DISPLAYCFG_GetMemSize(LCD_ID_E lcd_id,uint32 layer_id);

PUBLIC uint32 Res_Dec_Func1(uint32 *res_dec_ptr);

PUBLIC uint32 Res_Dec_Func2(uint32 *res_dec_ptr);

PUBLIC uint32 Res_Dec_Func3(uint32 *res_dec_ptr);

PUBLIC uint32 Res_Dec_Func4(uint32 *res_dec_ptr);

#endif//_DISPLAY_CFG_H