/****************************************************************************************
** File Name:      tft_S6D04H0A.c                                           			*
** DATE:           23/11/2010                                                			*
** Copyright:      2010 Samsung Electronics. All Rights Reserved.         			*
** Description:    this file defines S6D04H0A LCD interface,LCDC MCU MODE			*
*****************************************************************************************

*****************************************************************************************
**                         Edit History                                       			*
**--------------------------------------------------------------------------------------*
** DATE                  NAME			  DESCRIPTION               					*
** 24/11/2010     	     tiebing.guo        Create.										*
*****************************************************************************************/

/**-----------------------------------------------------------------------------------*
 **                         Dependencies           			                          *
 **-----------------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "dal_lcd.h"
#include "os_api.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"
#include "ref_outport.h"
/**-----------------------------------------------------------------------------------*
 ** 						Compiler Flag											  *
 **-----------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif



#define LCD_CtrlWrite_S6D04H0A( _data )\
				LCD_SendCmd( (_data ), 0 );

#define LCD_DataWrite_S6D04H0A( _data )\
				LCD_SendData( (_data), 0 );

#define LCD_CtrlData_S6D04H0A( _cmd, _data ) \
				LCD_SendCmdData( (_cmd ),  (_data),0 );  	
				
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;			

/******************************************************************************/
// Description: Close the lcd.(include sub lcd.)
// Global resource dependence: 
// Author: tiebing.guo
// Note:
/******************************************************************************/
LOCAL void S6D04H0A_Close(void);
/******************************************************************************/
// Description: Enter/Exit sleep mode .
// Global resource dependence: 
// Author: tiebing.guo
// Note:
//		is_sleep - SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
/******************************************************************************/
LOCAL ERR_LCD_E  S6D04H0A_EnterSleep(BOOLEAN is_sleep);

/******************************************************************************/
// Description: Initialize color LCD : S6D04H0A
// Global resource dependence: 
// Author: tiebing.guo
// Note:           
/******************************************************************************/
LOCAL ERR_LCD_E S6D04H0A_Init(void);
/******************************************************************************/
// Description: invalidate a rectang of in LCD
// Global resource dependence: 
// Author:  tiebing.guo
// Note:
/******************************************************************************/
LOCAL ERR_LCD_E S6D04H0A_Invalidate(void);
/******************************************************************************/
// Description: Copy a retangle data from clcd_buffer to display RAM.
//              then the rectangle display is to be refreshed
// Global resource dependence: 
// Author: tiebing.guo
// Note:       
/******************************************************************************/
LOCAL ERR_LCD_E S6D04H0A_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	);

/******************************************************************************/
//  Description:  Set LCD invalidate direction
//  Global resource dependence: 
//  Author: tiebing.guo
//  Input:
//      is_invert: 0, horizontal; 1, vertical
//  Return:
//      None.
//	Note: Application should correct invalidate direction right after current
//	 	  image displayed
/******************************************************************************/
LOCAL ERR_LCD_E S6D04H0A_SetDirection( LCD_DIRECT_E direct_type)
{
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:			
			break;
			
		case LCD_DIRECT_ROT_90:
			break;
			
		case LCD_DIRECT_ROT_180:
			break;
			
		case LCD_DIRECT_ROT_270:
			break;

		case LCD_DIRECT_MIR_H:
			break;
			
		case LCD_DIRECT_MIR_V:
			break;
			
		case LCD_DIRECT_MIR_HV:
			break;
			
		default:			
			break;
			
	}	

	s_lcd_direct = direct_type;

	return ERR_LCD_NONE;
}

/**************************************************************************************/
// Description: set display window 
// Global resource dependence:
// Author: tiebing.guo
// Note:
//		left - start Horizon address
//		right - end Horizon address
//		top - start Vertical address
// 		bottom - end Vertical address
/**************************************************************************************/
__inline void S6D04H0A_set_display_window(uint16 left, uint16 top, uint16 right, uint16 bottom)
{	
	//uint16 cmd;	
	uint8 left_byte_0, left_byte_1; //leftmost column
	uint8 right_byte_0, right_byte_1; //rightmost column
	uint8 top_byte_0, top_byte_1; //top line
	uint8 bottom_byte_0, bottom_byte_1; //bottom line

	left_byte_0 = left >> 8;
	left_byte_1 = left&0x00FF;

	right_byte_0 = right >> 8;
	right_byte_1 = right&0x00FF;

	top_byte_0 = top >> 8;
	top_byte_1 = top&0x00FF;

	bottom_byte_0 = bottom >> 8;
	bottom_byte_1 = bottom&0x00FF;


	/* Set Column Address */
	LCD_CtrlWrite_S6D04H0A(0x2A);
	LCD_DataWrite_S6D04H0A(left_byte_0);	
	LCD_DataWrite_S6D04H0A(left_byte_1);	
	LCD_DataWrite_S6D04H0A(right_byte_0);	
	LCD_DataWrite_S6D04H0A(right_byte_1);	

	/* Set Page Address */
	LCD_CtrlWrite_S6D04H0A(0x2B);
	LCD_DataWrite_S6D04H0A(top_byte_0);	
	LCD_DataWrite_S6D04H0A(top_byte_1);	
	LCD_DataWrite_S6D04H0A(bottom_byte_0);	
	LCD_DataWrite_S6D04H0A(bottom_byte_1);	


	
	switch(s_lcd_direct)
	{
		case LCD_DIRECT_NORMAL:	
			break;            
			
		case LCD_DIRECT_ROT_90:				
			break;
			
		case LCD_DIRECT_ROT_180:
			break;
			
		case LCD_DIRECT_ROT_270:
			break;

		case LCD_DIRECT_MIR_H:
			break;

		case LCD_DIRECT_MIR_V:
			break;

		case LCD_DIRECT_MIR_HV:
			break;
			
		default:			
			break;	
	
	}    
	LCD_CtrlWrite_S6D04H0A(0x002C);
}//end of S6D04H0A_set_display_window
/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: tiebing.guo
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E S6D04H0A_Init(void)
{
	//SCI_TRACE_LOW:"S6D04H0A initialize!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_S6D04H0A_199_112_2_18_0_34_8_1859,(uint8*)"");
	
	LCD_Reset();//reset lcd throught lcdc	
	LCD_Delayms(50);
    
	/* Level 2 command access */
	LCD_CtrlWrite_S6D04H0A(0x00F0);
	LCD_DataWrite_S6D04H0A(0x005A);
	LCD_DataWrite_S6D04H0A(0x005A);

	/* Power Setting Sequence */
	LCD_CtrlWrite_S6D04H0A(0x00F4);
	LCD_DataWrite_S6D04H0A(0x000B);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0022);
	LCD_DataWrite_S6D04H0A(0x0067);
	LCD_DataWrite_S6D04H0A(0x0004);
	LCD_DataWrite_S6D04H0A(0x0002);
	LCD_DataWrite_S6D04H0A(0x002A);
	LCD_DataWrite_S6D04H0A(0x007A);
	LCD_DataWrite_S6D04H0A(0x0005);


	LCD_CtrlWrite_S6D04H0A(0x00F5);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0055);
	LCD_DataWrite_S6D04H0A(0x0037);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x000B);

	LCD_CtrlWrite_S6D04H0A(0x00F6);
	LCD_DataWrite_S6D04H0A(0x0007);
	LCD_DataWrite_S6D04H0A(0x0001);
	LCD_DataWrite_S6D04H0A(0x0007);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0002);
	LCD_DataWrite_S6D04H0A(0x000C);
	LCD_DataWrite_S6D04H0A(0x0002);
	LCD_DataWrite_S6D04H0A(0x0008);
	LCD_DataWrite_S6D04H0A(0x0005);


	/* Initialising Sequence */
	LCD_CtrlWrite_S6D04H0A(0x0035);
	LCD_DataWrite_S6D04H0A(0x0000);

	LCD_CtrlWrite_S6D04H0A(0x0036);
	LCD_DataWrite_S6D04H0A(0x0048);

	LCD_CtrlWrite_S6D04H0A(0x003A);
	LCD_DataWrite_S6D04H0A(0x0005);

	LCD_CtrlWrite_S6D04H0A(0x00F7);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);

	LCD_CtrlWrite_S6D04H0A(0x00F2);

	LCD_DataWrite_S6D04H0A(0x0028);
	LCD_DataWrite_S6D04H0A(0x0065);
	LCD_DataWrite_S6D04H0A(0x0003);
	LCD_DataWrite_S6D04H0A(0x0002);
	LCD_DataWrite_S6D04H0A(0x0002);

	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0021);
	LCD_DataWrite_S6D04H0A(0x0048);
	LCD_DataWrite_S6D04H0A(0x0000);

	LCD_DataWrite_S6D04H0A(0x0003);
	LCD_DataWrite_S6D04H0A(0x0001);
	/* Gamma Setting Sequence */
	/* RED */
	LCD_CtrlWrite_S6D04H0A(0x00F9);
	LCD_DataWrite_S6D04H0A(0x0004);

	/* CMD 0xF4 */
	LCD_CtrlWrite_S6D04H0A(0x00F4);
	
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0001);
	LCD_DataWrite_S6D04H0A(0x0018);
	LCD_DataWrite_S6D04H0A(0x0028);

	LCD_DataWrite_S6D04H0A(0x003F);
	LCD_DataWrite_S6D04H0A(0x0029);
	LCD_DataWrite_S6D04H0A(0x0029);
	LCD_DataWrite_S6D04H0A(0x0024);
	LCD_DataWrite_S6D04H0A(0x0028);

	LCD_DataWrite_S6D04H0A(0x002A);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);

	LCD_DataWrite_S6D04H0A(0x0000);
	

	/* CMD 0xFB */
	LCD_CtrlWrite_S6D04H0A(0x00FB);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x001E);
	LCD_DataWrite_S6D04H0A(0x0021);
	LCD_DataWrite_S6D04H0A(0x0021);
	
	LCD_DataWrite_S6D04H0A(0x0026);
	LCD_DataWrite_S6D04H0A(0x0014);
	LCD_DataWrite_S6D04H0A(0x003C);
	LCD_DataWrite_S6D04H0A(0x0027);
	LCD_DataWrite_S6D04H0A(0x0018);

	LCD_DataWrite_S6D04H0A(0x0005);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);

	LCD_DataWrite_S6D04H0A(0x0000);
	
	/* GREEN */
	LCD_CtrlWrite_S6D04H0A(0x00F9);
	LCD_DataWrite_S6D04H0A(0x0002);

	/* CMD 0xF4 */
	LCD_CtrlWrite_S6D04H0A(0x00F4);
	
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0001);
	LCD_DataWrite_S6D04H0A(0x001A);
	LCD_DataWrite_S6D04H0A(0x0030);

	LCD_DataWrite_S6D04H0A(0x003F);
	LCD_DataWrite_S6D04H0A(0x002B);
	LCD_DataWrite_S6D04H0A(0x0024);
	LCD_DataWrite_S6D04H0A(0x0023);
	LCD_DataWrite_S6D04H0A(0x0025);

	LCD_DataWrite_S6D04H0A(0x002A);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);

	LCD_DataWrite_S6D04H0A(0x0000);
	

	/* CMD 0xFB */
	LCD_CtrlWrite_S6D04H0A(0x00FB);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x001E);
	LCD_DataWrite_S6D04H0A(0x0020);
	LCD_DataWrite_S6D04H0A(0x0020);
	
	LCD_DataWrite_S6D04H0A(0x0021);
	LCD_DataWrite_S6D04H0A(0x0010);
	LCD_DataWrite_S6D04H0A(0x003D);
	LCD_DataWrite_S6D04H0A(0x0029);
	LCD_DataWrite_S6D04H0A(0x0019);

	LCD_DataWrite_S6D04H0A(0x0005);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);

	LCD_DataWrite_S6D04H0A(0x0000);

	/* BLUE */
	LCD_CtrlWrite_S6D04H0A(0x00F9);
	LCD_DataWrite_S6D04H0A(0x0001);

	/* CMD 0xF4 */
	LCD_CtrlWrite_S6D04H0A(0x00F4);
	
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0001);
	LCD_DataWrite_S6D04H0A(0x001A);
	LCD_DataWrite_S6D04H0A(0x002D);

	LCD_DataWrite_S6D04H0A(0x003F);
	LCD_DataWrite_S6D04H0A(0x002A);
	LCD_DataWrite_S6D04H0A(0x0026);
	LCD_DataWrite_S6D04H0A(0x0025);
	LCD_DataWrite_S6D04H0A(0x0026);

	LCD_DataWrite_S6D04H0A(0x002A);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);

	LCD_DataWrite_S6D04H0A(0x0000);
	

	/* CMD 0xFB */
	LCD_CtrlWrite_S6D04H0A(0x00FB);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x001E);
	LCD_DataWrite_S6D04H0A(0x0021);
	LCD_DataWrite_S6D04H0A(0x0022);
	
	LCD_DataWrite_S6D04H0A(0x0023);
	LCD_DataWrite_S6D04H0A(0x0011);
	LCD_DataWrite_S6D04H0A(0x003D);
	LCD_DataWrite_S6D04H0A(0x002C);
	LCD_DataWrite_S6D04H0A(0x0019);

	LCD_DataWrite_S6D04H0A(0x0005);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);

	LCD_DataWrite_S6D04H0A(0x0000);

	/* Level2 Command blocking */
	/* CMD 0xF0 */
	LCD_CtrlWrite_S6D04H0A(0x00F0);
	LCD_DataWrite_S6D04H0A(0x00A5);
	LCD_DataWrite_S6D04H0A(0x00A5);

	/* Column Address set */
	/* 0~239 */
	LCD_CtrlWrite_S6D04H0A(0x002A);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x00EF);

	/* Page Address set */
	/* 0~319 */
	LCD_CtrlWrite_S6D04H0A(0x002B);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0000);
	LCD_DataWrite_S6D04H0A(0x0001);
	LCD_DataWrite_S6D04H0A(0x003F);

	/* Sleep out */
	/* CMD 0x11 */
	LCD_CtrlWrite_S6D04H0A(0x0011);

	LCD_Delayms(160);

    	/* Page Address set */
	/* 0~319 */

	/* Display on */
	/* CMD 0x29 */
	LCD_CtrlWrite_S6D04H0A(0x0029);
	return ERR_LCD_NONE;
}//end of  S6D04H0A_drive
/**************************************************************************************/
// Description: Enter/Exit sleep mode
// Global resource dependence:
// Author: tiebing.guo
// Note:
//		is_sleep - 1:enter into sleep;0:exit sleep
/**************************************************************************************/
LOCAL ERR_LCD_E S6D04H0A_EnterSleep(BOOLEAN is_sleep)
{
	if(is_sleep)
	{
		LCD_CtrlWrite_S6D04H0A(0x0010);
	}
	else
	{		
		LCD_CtrlWrite_S6D04H0A(0x0011);
  	}

	 return ERR_LCD_NONE;
}//end of S6D04H0A_EnterSleep
/**************************************************************************************/
// Description: close lcdc and lcd
// Global resource dependence:
// Author: tiebing.guo
/**************************************************************************************/
LOCAL void S6D04H0A_Close(void)
{
	S6D04H0A_EnterSleep(SCI_TRUE);
	
}//end of S6D04H0A_Close
/**************************************************************************************/
// Description: invalidate whole LCD
// Global resource dependence:
// Author: tiebing.guo
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E S6D04H0A_Invalidate(void)
{		
	S6D04H0A_set_display_window(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	return ERR_LCD_NONE;
}//end of S6D04H0A_Invalidate
/**************************************************************************************/
// Description: refresh a rectangle of lcd
// Global resource dependence:
// Author: tiebing.guo
// Note:
//		left - the left value of the rectangel
//   	top - the top value of the rectangel
// 		right - the right value of the rectangel
//		bottom - the bottom value of the rectangel
/**************************************************************************************/
LOCAL ERR_LCD_E S6D04H0A_InvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom)
{
    
    left 	= (left >= LCD_WIDTH)    ? LCD_WIDTH-1 : left;
    right 	= (right >= LCD_WIDTH)   ? LCD_WIDTH-1 : right;
    top 	= (top >= LCD_HEIGHT)    ? LCD_HEIGHT-1 : top;
    bottom 	= (bottom >= LCD_HEIGHT) ? LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
    	return ERR_LCD_OPERATE_FAIL;
    }     
   
    S6D04H0A_set_display_window(left, top, right, bottom); 
    return ERR_LCD_NONE;
}//en of S6D04H0A_InvalidateRect	
/**************************************************************************************/
// Description: refresh a rectangle of lcd
// Global resource dependence:
// Author: tiebing.guo
// Note:
//		left - the left value of the rectangel
//   	top - the top value of the rectangel
// 		right - the right value of the rectangel
//		bottom - the bottom value of the rectangel
/**************************************************************************************/
LOCAL ERR_LCD_E S6D04H0A_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	switch(angle)
	{
		case LCD_ANGLE_0:
			S6D04H0A_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			S6D04H0A_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			S6D04H0A_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			S6D04H0A_set_display_window(left, top, bottom,right);
			break;
		default:
			S6D04H0A_set_display_window(left, top, right, bottom);
			break;			
	}
		

	return ERR_LCD_NONE;
}//en of S6D04H0A_VerticalInvalidateRect	

/**************************************************************************************/
// Description: set LCD contrast.
// Global resource dependence: N/A
// Author: tiebing.guo
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E S6D04H0A_SetContrast(uint16 contrast)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
}//end of S6D04H0A_SetContrast

/**************************************************************************************/
// Description: set LCD Brightness.
// Global resource dependence: N/A
// Author: tiebing.guo
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E S6D04H0A_SetBrightness(uint16 brightness)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
}//end of S6D04H0A_SetBrightness

/**************************************************************************************/
// Description: set display window of LCD
// Global resource dependence:
// Author: tiebing.guo
// Note:
//		left - the left value of the rectangel
//   	top - the top value of the rectangel
// 		right - the right value of the rectangel
//		bottom - the bottom value of the rectangel
/**************************************************************************************/
LOCAL ERR_LCD_E S6D04H0A_SetDisplayWindow(uint16 left,uint16 top,uint16 right,uint16 bottom)
{
	S6D04H0A_set_display_window(left, top, right,bottom);
	return ERR_LCD_NONE;
}//end of S6D04H0A_SetDisplayWindow

/**************************************************************************************/
// S6D04H0A_operations:LCD drive interfaces struction
/**************************************************************************************/
PUBLIC const LCD_OPERATIONS_T S6D04H0A_operations = 
{
	S6D04H0A_Init,
	S6D04H0A_EnterSleep,
	S6D04H0A_SetContrast,
	S6D04H0A_SetBrightness,
	S6D04H0A_SetDisplayWindow,
	S6D04H0A_InvalidateRect,
	S6D04H0A_Invalidate,
	S6D04H0A_Close,
	S6D04H0A_RotationInvalidateRect,
	S6D04H0A_SetDirection,
    NULL,
    NULL
};
/**************************************************************************************/
// Description: get S6D04H0A driver function
// Global resource dependence:
// Author: tiebing.guo
// Note:
/**************************************************************************************/
PUBLIC LCD_OPERATIONS_T* S6D04H0A_GetOperations(void)
{
	return PNULL;//(LCD_OPERATIONS_T*)&S6D04H0A_operations;
}//end of S6D04H0A_GetOperations
/**************************************************************************************/
// Description: LCD probe interface
// Global resource dependence:
// Author: tiebing.guo
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN S6D04H0A_Probe(void)
{
    return SCI_TRUE;    
}
/**************************************************************************************/
// S6D04H0A_timing:LCD access timing table
/**************************************************************************************/
LOCAL const LCD_TIMING_U s_S6D04H0A_timing =
{
// LCM_CYCLE_U start(ns)
	150,    // CS setup time for LCM read (optional)  
	60,		// low pulse width for LCM read (according spec)	
	60,		// high pulse width for LCM read (according spec)
	100,    // CS setup time for LCM write  (optional) 	
	35,     // low pulse width for LCM write (according spec)
	35,     // high pulse width for LCM write (according spec)	
// LCM_CYCLE_U end(ns)
};
/**************************************************************************************/
// S6D04H0A_specification:LCD specification struction
/**************************************************************************************/
PUBLIC const LCD_SPEC_T g_lcd_S6D04H0A = 
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_8,
	(LCD_TIMING_U*)&s_S6D04H0A_timing, 
	(LCD_OPERATIONS_T*)&S6D04H0A_operations,
	0,
	0
};


/**-----------------------------------------------------------------------------------*
 ** 						Compiler Flag											  *
 **-----------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


