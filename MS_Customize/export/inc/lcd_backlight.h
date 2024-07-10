/******************************************************************************
 ** File Name:      lcd_backlight.h                                           *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           12/25/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the product lcd back light interface    * 					
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/25/2007     Liangwen.Zhen	  Create.                         		  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"


#ifndef  _LCD_BACKLIGHT_H_
#define _LCD_BACKLIGHT_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structure                                    *
 **---------------------------------------------------------------------------*/
typedef enum
{
	LCD_BACKLIGHT_TYPE_PWM_POS = 0x00,		// POSITAVE valid
	LCD_BACKLIGHT_TYPE_PWM_NEG,				// NEGATIVE valid
	
	LCD_BACKLIGHT_TYPE_AAT3194,
	LCD_BACKLIGHT_TYPE_AAT3155,
	LCD_BACKLIGHT_TYPE_RT9364,
	LCD_BACKLIGHT_TYPE_RT9285,
	LCD_BACKLIGHT_TYPE_SSD1228,
	LCD_BACKLIGHT_TYPE_HX8347,
	LCD_BACKLIGHT_TYPE_NT39118,

	LCD_BACKLIGHT_TYPE_BB_ANALOG,
    LCD_BACKLIGHT_TYPE_GPIO, 
	LCD_BACKLIGHT_TYPE_MAX					// less than 0xff

}LCD_BACKLIGHT_TYPE_E;

 typedef struct
{
	LCD_BACKLIGHT_TYPE_E 	type;
	uint32				 	param;
	uint32                  bl_min;
	uint32                  bl_max;
}LCD_BACKLIGHT_INFO_T, * LCD_BACKLIGHT_INFO_T_PTR;
 
/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to set lcd back light brightness    
//  Author:         Liangwen.Zhen
//  Note:
//   brightness : 0 - 100  // when the brightness is 0, turn off the backlight.           
/*****************************************************************************/
PUBLIC void LCD_SetBackLightBrightness( uint32  brightness ) ;

/* Add for IB Calibration */
PUBLIC void IB_Calibration();

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // end of lcd_backlight.h    
#endif  // End of _LCD_BACKLIGHT_H_

