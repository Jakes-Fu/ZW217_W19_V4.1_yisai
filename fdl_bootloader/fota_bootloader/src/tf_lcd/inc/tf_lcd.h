/******************************************************************************
 ** File Name:      tf_lcd.h                                                    *
 ** Author:          tao.feng                                               *
 ** DATE:            21/01/2011                                               *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE              NAME             DESCRIPTION                            *
 ** 21/01/2011    tao.feng          Create.                                   *
 ******************************************************************************/


#ifndef _TF_LCD_H
#define _TF_LCD_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"
#include "arm_reg.h"
#include "ref_lcd.h"
#include "dal_lcd.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               
 **---------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description: get rotation mode
/*****************************************************************************/
PUBLIC LCD_ROT_MODE_E TF_GetLcdRotateMode(void);


  /*****************************************************************************/
//  Description:  init the lcd, if have sub lcd, init the sub lcd .
//                [younger]don't change the LCD init sequency
//    Global resource dependence:
//  Author: tao.feng
//    Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E TF_LCDInit(void);


 /*****************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC void TF_LcdClose(void);


 /*****************************************************************************/
//  Description:   get the lcd information.
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E TF_GetLcdInfo(
                            LCD_ID_E lcd_id,            //ID of lcd to operate
                            LCD_INFO_T *lcd_info_ptr    //lcd information struct pointer
                            );


 /*****************************************************************************/
//  Description:   get  lcd size.
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E TF_GetlcdSize(LCD_ID_E lcd_id, LCD_SIZE_T *size_ptr);


 /*****************************************************************************/
//  Description:    Enable lcd to partial display mode, so can save power.
//    Global resource dependence: 
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//    Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
PUBLIC ERR_LCD_E  TF_SetLcdDisplayWindow(
                                      LCD_ID_E lcd_id,//ID of lcd to operate
                                      uint16 left,     //left of the display window
                                      uint16 top,     //top of the display window
                                      uint16 right,     //right of the display window
                                      uint16 bottom    //bottom of the display window
                                      );


 /*****************************************************************************/
//  Description:  Set LCD invalidate direction
//  Input:
//      direct_type: 
//  Return:
//      None.
//	Note: Application should correct invalidate direction right after current
//	 	  image displayed
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E  TF_SetLcdDirection(
                                    LCD_ID_E 		lcd_id,//ID of lcd to operate
	                             LCD_DIRECT_E 	direct_type                   
	                              );


 /*****************************************************************************/
//  Description:  Get LCD invalidate direction type
//  Input:
//      direct_type: 
//  Return:
//      None.
//	Note: Application should correct invalidate direction right after current
//	 	  image displayed
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/
PUBLIC  LCD_DIRECT_E  TF_GetLcdDirection(LCD_ID_E lcd_id);


 /*****************************************************************************/
//  Description:    This function turn on/off the LCD1 backlight
//                  b_on = SCI_TRUE, turn on
//                  b_on = SCI_FALSE, turn off
//                  value = britness
//  Author:     Zhemin.Lin
//  Note:        LCD1 backlight is controlled by GPIO7 on SM5100B EVB1.0
/*****************************************************************************/
PUBLIC void  TF_SetLcdBackLight(BOOLEAN b_on, uint16 value);


 /*****************************************************************************/
//  Description:  LCD Reg Info
//    Global resource dependence:
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/

PUBLIC void TF_RegLcdInfo(LCD_ID_E lcd_id, LCD_SPEC_INFO_T_PTR lcd_spec_ptr);


 /*****************************************************************************/
//  Description:  lcm config port
//    Global resource dependence:
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/
PUBLIC void TF_CfgLCMPort(uint16 lcd_cs,uint16 lcd_cd, uint16 lcd_id);


 /*****************************************************************************/
//  Description:  set dest LCD's RGB mode to host
//    Global resource dependence:
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/
PUBLIC void TF_SetLCDRGBMode(LCD_ID_E lcd_id);



/*****************************************************************************/
//  Description:   reset  lcd.(include sub lcd.)
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC void LCD_Reset(void);


 /******************************************************************************/
// Description: send the command and data to LCD
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
// 		input:
//			cmd - lcd command
//			lcd_data - lcd command data
//			lcd_id - only is used for lcdc mcu mode
// 		return:
//			LCD_OK,if success
//      	other,if fail
/*********************************************************************************/
PUBLIC int32 LCD_SendCmdData(uint32 cmd,uint32 lcd_data,uint32 lcd_id);


 /*****************************************************************************/
// Description: send command to lcd
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//		lcd_id parameter is used when lcdc mcu mode
// 		input:
//			cmd - lcd command
//			lcd_id - LCD ID
// 		return:
//			LCD_OK,if success
//			other,if fail
/*****************************************************************************/
PUBLIC int32 LCD_SendCmd(uint32 cmd,uint32 lcd_id);


 /*****************************************************************************/
// Description: send display data to LCD
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//		this interface is used in lcdc mcu mode
// 		input:
//			lcd_data - display data
//			lcd_id - LCD ID
// 		return:
//			LCD_OK,if success
//			other,if fail
/*****************************************************************************/
PUBLIC int32 LCD_SendData(uint32 lcd_data,uint32 lcd_id);


 /****************************************************************************/
// Description:Read data from lcm
// Global resource dependence: NONE
// Author: Younger.yang
// Note:
//		this interface is read data from lcm
// 		input:
//		      cd_value - cd value
//                  lcd_id-MAIN_LCD_ID/SUB_LCD_ID                   
// 		return:
//			LCD_OK,if success
//			other,if fail
/********************************************************************************/
PUBLIC uint32 LCD_Read(uint32 lcd_id,uint32 cd_value);

 /*****************************************************************************/
//  Description: configure block parameters  
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
//		input:
//			lcd_id  - specify LCD
//			blk_num - LCDC Block number ,0~5
//			config  - Block parameter structure
//		output:
//			none
//		return:
//			result
/*****************************************************************************/
PUBLIC ERR_LCD_E TF_ConfigBlock(
								  LCD_ID_E lcd_id, 
								  uint32 blk_num, 
								  BLOCKCFG_T config
								);

 /*****************************************************************************/
//  Description: set display memory  
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
//		input:
//			lcd_id  - specify LCD
//			blk_num - LCDC Block number ,0~5
//			buf_ptr - buffer address
//		output:
//			none
//		return:
//			result
/*****************************************************************************/
PUBLIC ERR_LCD_E TF_SetBlockBuffer(
                                     LCD_ID_E lcd_id, 
                                     uint32 blk_num, 
                                     uint32 *buf_ptr
                                   );

 /*****************************************************************************/
//  Description: get display memory  
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
//		input:
//			lcd_id  - specify LCD
//			blk_num - LCDC Block number ,0~5
//		output:
//			none
//		return:
//			result
/*****************************************************************************/
PUBLIC uint32 *TF_GetLcdBlockBuffer(LCD_ID_E lcd_id, uint32 blk_num);

 /*****************************************************************************/
//  Description: get block parameters   
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
//		input:
//			lcd_id  - specify LCD
//			blk_num - LCDC Block number ,0~5
//			cfg_ptr  - pointer to Block parameter structure
//		output:
//			none
//		return:
//			result
/*****************************************************************************/
PUBLIC ERR_LCD_E TF_GetLcdConfigBlock(
									 LCD_ID_E lcd_id, 
									 uint32 blk_num, 
									 BLOCKCFG_T *cfg_ptr
								   );

 /*******************************************************************************/
// Description: 
// Global resource dependence:
// Author: Younger.yang
// Note:
/********************************************************************************/	
PUBLIC uint32 TF_GetLcdBusRatio(void);

 /*****************************************************************************/
//  Description:    get the main lcd 's important parameter for Digital cammera
//	Global resource dependence: 
//  Author:         Zhemin.lin
//	Note:
/*****************************************************************************/
PUBLIC LCD_SPEC_INFO_T_PTR TF_GetLcdSpec( LCD_ID_E lcd_id);

 /*****************************************************************************/
//  Description:    LCD delay
//	Global resource dependence: 
//  Author:         
//	Note:
/*****************************************************************************/
PUBLIC void LCD_Delayms (uint32 ms)	;

 /*****************************************************************************/
//  Description: set brush lcd fashion  
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
//		input:
//			lcd_id - specify LCD
//			mode   - sync or async
//		output:
//			none
//		return:
//			result
/*****************************************************************************/
PUBLIC ERR_LCD_E TF_SetLcmBrushMode(LCD_ID_E lcd_id, LCD_BRUSH_MODE_E mode);


/*****************************************************************************/
//  Description: get brush lcd fashion  
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
//		input:
//			lcd_id - specify LCD			
//		output:
//			none
//		return:
//			brush LCD mode
/*****************************************************************************/
PUBLIC LCD_BRUSH_MODE_E TF_GetLcmdBrushMode(LCD_ID_E lcd_id);


/*******************************************************************************/
// Description: get lcd bus width
// Global resource dependence:
// Author: 
// Note:
/********************************************************************************/	
PUBLIC uint32 TF_GetLcdBusWidth(LCD_ID_E lcd_id);

/*****************************************************************************/
//  Function name:	TF_HwInit
//  Description:This function is used to hw init
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC  void TF_LcdInit(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    }
#endif

#endif // _LCD_H
