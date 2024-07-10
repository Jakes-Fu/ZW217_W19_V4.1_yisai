/****************************************************************************************
** File Name:      tft_S6D0139_lcdc.c                                           					*
** DATE:           29/4/2007                                                			*
** Copyright:      2007 Spreatrum, Incoporated. All Rights Reserved.         			*
** Description:    this file defines S6D0139 LCD interface,LCDC MCU MODE			*
*****************************************************************************************

*****************************************************************************************
**                         Edit History                                       			*
**--------------------------------------------------------------------------------------*
** DATE                  NAME			  DESCRIPTION               					*
** 20/11/2006     	     Jianping.wang    Create.										*
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



#define LCD_CtrlWrite_S6D0139( _data )\
				LCD_SendCmd( (_data ), 0 );

#define LCD_DataWrite_S6D0139( _data )\
				LCD_SendData( (_data), 0 );

#define LCD_CtrlData_S6D0139( _cmd, _data ) \
				LCD_SendCmdData( (_cmd ),  (_data),0 );  	


				
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;			
				
/**--------------------------------------------------------------------------*
 **                         Constant DEFINITION                              *
 **--------------------------------------------------------------------------*/ 

/******************************************************************************/
// Description: Close the lcd.(include sub lcd.)
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/******************************************************************************/
LOCAL void S6D0139_Close(void);
/******************************************************************************/
// Description: Enter/Exit sleep mode .
// Global resource dependence: 
// Author: Jianping.wang
// Note:
//		is_sleep - SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
/******************************************************************************/
LOCAL ERR_LCD_E  S6D0139_EnterSleep(BOOLEAN is_sleep);

/******************************************************************************/
// Description: Initialize color LCD : S6D0139
// Global resource dependence: 
// Author: Jianping.wang
// Note:           
/******************************************************************************/
LOCAL ERR_LCD_E S6D0139_Init(void);
/******************************************************************************/
// Description: invalidate a rectang of in LCD
// Global resource dependence: 
// Author:  Jianping.wang
// Note:
/******************************************************************************/
LOCAL ERR_LCD_E S6D0139_Invalidate(void);
/******************************************************************************/
// Description: Copy a retangle data from clcd_buffer to display RAM.
//              then the rectangle display is to be refreshed
// Global resource dependence: 
// Author: Jianping.wang
// Note:       
/******************************************************************************/
LOCAL ERR_LCD_E S6D0139_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	);

/******************************************************************************/
//  Description:  Set LCD invalidate direction
//  Input:
//      is_invert: 0, horizontal; 1, vertical
//  Return:
//      None.
//	Note: Application should correct invalidate direction right after current
//	 	  image displayed
/******************************************************************************/
LOCAL ERR_LCD_E S6D0139_SetDirection( LCD_DIRECT_E direct_type)
{
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:			
		    LCD_CtrlWrite_S6D0139(0x0003);		    
		    LCD_DataWrite_S6D0139(0x1030);
			break;
			
		case LCD_DIRECT_ROT_90:
			LCD_CtrlWrite_S6D0139(0x0003);		    
		    LCD_DataWrite_S6D0139(0x1028);	 
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlWrite_S6D0139(0x0003);		    
		    LCD_DataWrite_S6D0139(0x1000);	 
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlWrite_S6D0139(0x0003);		    
		    LCD_DataWrite_S6D0139(0x1018);
			break;

		case LCD_DIRECT_MIR_H:
			LCD_CtrlWrite_S6D0139(0x0003);		    
		    LCD_DataWrite_S6D0139(0x1020);
			break;
			
		case LCD_DIRECT_MIR_V:
			LCD_CtrlWrite_S6D0139(0x0003);		    
		    LCD_DataWrite_S6D0139(0x1010);
			break;
			
		case LCD_DIRECT_MIR_HV:
			LCD_CtrlWrite_S6D0139(0x0003);		    
		    LCD_DataWrite_S6D0139(0x1000);
			break;
			
		default:			
			LCD_CtrlWrite_S6D0139(0x0003);		    
		    LCD_DataWrite_S6D0139(0x1030);	
		    direct_type = LCD_DIRECT_NORMAL;
			break;
			
	}	

	s_lcd_direct = direct_type;

	return ERR_LCD_NONE;
}

/**************************************************************************************/
// Description: set display window 
// Global resource dependence:
// Author: Jianping.wang
// Note:
//		left - start Horizon address
//		right - end Horizon address
//		top - start Vertical address
// 		bottom - end Vertical address
/**************************************************************************************/
__inline void S6D0139_set_display_window(uint16 left, uint16 top, uint16 right, uint16 bottom)
{	
	//uint16 cmd;	

	LCD_CtrlData_S6D0139(0x0046, ((right << 8) & 0xff00)|(left&0xff));
	LCD_CtrlData_S6D0139(0x0047, bottom);
	LCD_CtrlData_S6D0139(0x0048, top);
	
	switch(s_lcd_direct)
	{
		case LCD_DIRECT_NORMAL:	
		    LCD_CtrlData_S6D0139(0x0020, left);
			LCD_CtrlData_S6D0139(0x0021, top);
			break;            
			
		case LCD_DIRECT_ROT_90:				
			LCD_CtrlData_S6D0139(0x0020, right);
			LCD_CtrlData_S6D0139(0x0021, top);	
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlData_S6D0139(0x0020, right);
			LCD_CtrlData_S6D0139(0x0021, bottom);				
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlData_S6D0139(0x0020, left);
			LCD_CtrlData_S6D0139(0x0021, top);			
			break;

		case LCD_DIRECT_MIR_H:
			LCD_CtrlData_S6D0139(0x0020, right);
			LCD_CtrlData_S6D0139(0x0021, top);
			break;

		case LCD_DIRECT_MIR_V:
			LCD_CtrlData_S6D0139(0x0020, left);
			LCD_CtrlData_S6D0139(0x0021, bottom);
			break;

		case LCD_DIRECT_MIR_HV:
			LCD_CtrlData_S6D0139(0x0020, right);
			LCD_CtrlData_S6D0139(0x0021, bottom);
			break;
			
		default:			
		    LCD_CtrlData_S6D0139(0x0020, left);
			LCD_CtrlData_S6D0139(0x0021, top);
			break;	
	
	}    
	
	LCD_CtrlWrite_S6D0139(0x0022);
}//end of S6D0139_set_display_window
/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E S6D0139_Init(void)
{
	//SCI_TRACE_LOW:"S6D0139 initialize!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_S6D0139_207_112_2_18_0_34_6_1858,(uint8*)"");
	
    LCD_Reset();//reset lcd throught lcdc	
    LCD_Delayms(100);
    
//    LCD_Delayms(10);
	//WMLCDCOM(0x0000);WMLCDDATA(0x0001);
	LCD_CtrlWrite_S6D0139(0x0000);
	LCD_DataWrite_S6D0139(0x0001);
	LCD_Delayms(10);
	//WMLCDCOM(0x0007);WMLCDDATA(0x0000);
	LCD_CtrlWrite_S6D0139(0x0007);
	LCD_DataWrite_S6D0139(0x0000);
	//WMLCDCOM(0x0013);WMLCDDATA(0x0000);
	LCD_CtrlWrite_S6D0139(0x0013);
	LCD_DataWrite_S6D0139(0x0000);
	//WMLCDCOM(0x0011);WMLCDDATA(0x2604);
	LCD_CtrlWrite_S6D0139(0x0011);
	LCD_DataWrite_S6D0139(0x2604);
	//WMLCDCOM(0x0014);WMLCDDATA(0x1212);
	LCD_CtrlWrite_S6D0139(0x0014);
	LCD_DataWrite_S6D0139(0x1212);
	//WMLCDCOM(0x0010);WMLCDDATA(0x3C00);
	LCD_CtrlWrite_S6D0139(0x0010);
	LCD_DataWrite_S6D0139(0x3C00);
	//WMLCDCOM(0x0013);WMLCDDATA(0x0040);
	LCD_CtrlWrite_S6D0139(0x0013);
	LCD_DataWrite_S6D0139(0x0040);
	LCD_Delayms(10);
	//WMLCDCOM(0x0013);WMLCDDATA(0x0060);
	LCD_CtrlWrite_S6D0139(0x0013);
	LCD_DataWrite_S6D0139(0x0060);
	LCD_Delayms(50);
	//WMLCDCOM(0x0013);WMLCDDATA(0x0070);
	LCD_CtrlWrite_S6D0139(0x0013);
	LCD_DataWrite_S6D0139(0x0070);
	LCD_Delayms(40);
	//WMLCDCOM(0x0001);WMLCDDATA(0x0127);
	LCD_CtrlWrite_S6D0139(0x0001);
	LCD_DataWrite_S6D0139(0x0127);
	//WMLCDCOM(0x0002);WMLCDDATA(0x0700);
	LCD_CtrlWrite_S6D0139(0x0002);
	LCD_DataWrite_S6D0139(0x0700);
	//WMLCDCOM(0x0003);WMLCDDATA(0x1030);
	LCD_CtrlWrite_S6D0139(0x0003);
	LCD_DataWrite_S6D0139(0x1030);
	//WMLCDCOM(0x0007);WMLCDDATA(0x0000);
	LCD_CtrlWrite_S6D0139(0x0007);
	LCD_DataWrite_S6D0139(0x0000);
	//WMLCDCOM(0x0008);WMLCDDATA(0x0404);
	LCD_CtrlWrite_S6D0139(0x0008);
	LCD_DataWrite_S6D0139(0x0404);
	//WMLCDCOM(0x000B);WMLCDDATA(0x0200);
	LCD_CtrlWrite_S6D0139(0x000B);
	LCD_DataWrite_S6D0139(0x0200);
	//WMLCDCOM(0x000C);WMLCDDATA(0x0000);
	LCD_CtrlWrite_S6D0139(0x000C);
	LCD_DataWrite_S6D0139(0x0000);
	//WMLCDCOM(0x0015);WMLCDDATA(0x0000);
	LCD_CtrlWrite_S6D0139(0x0015);
	LCD_DataWrite_S6D0139(0x0000);

	//gamma setting
	//WMLCDCOM(0x0030);WMLCDDATA(0x0000);
	LCD_CtrlWrite_S6D0139(0x0030);
	LCD_DataWrite_S6D0139(0x0000);
	//WMLCDCOM(0x0031);WMLCDDATA(0x0606);
	LCD_CtrlWrite_S6D0139(0x0031);
	LCD_DataWrite_S6D0139(0x0606);
	//WMLCDCOM(0x0032);WMLCDDATA(0x0006);
	LCD_CtrlWrite_S6D0139(0x0032);
	LCD_DataWrite_S6D0139(0x0006);
	//WMLCDCOM(0x0033);WMLCDDATA(0x0403);
	LCD_CtrlWrite_S6D0139(0x0033);
	LCD_DataWrite_S6D0139(0x0403);
	//WMLCDCOM(0x0034);WMLCDDATA(0x0107);
	LCD_CtrlWrite_S6D0139(0x0034);
	LCD_DataWrite_S6D0139(0x0107);
	//WMLCDCOM(0x0035);WMLCDDATA(0x0101);
	LCD_CtrlWrite_S6D0139(0x0035);
	LCD_DataWrite_S6D0139(0x0101);
	//WMLCDCOM(0x0036);WMLCDDATA(0x0707);
	LCD_CtrlWrite_S6D0139(0x0036);
	LCD_DataWrite_S6D0139(0x0707);
	//WMLCDCOM(0x0037);WMLCDDATA(0x0304);
	LCD_CtrlWrite_S6D0139(0x0037);
	LCD_DataWrite_S6D0139(0x0304);
	//WMLCDCOM(0x0038);WMLCDDATA(0x0A00);
	LCD_CtrlWrite_S6D0139(0x0038);
	LCD_DataWrite_S6D0139(0x0A00);
	//WMLCDCOM(0x0039);WMLCDDATA(0x0706);
	LCD_CtrlWrite_S6D0139(0x0039);
	LCD_DataWrite_S6D0139(0x0706);
  
	//WMLCDCOM(0x0040);WMLCDDATA(0x0000);
	LCD_CtrlWrite_S6D0139(0x0040);
	LCD_DataWrite_S6D0139(0x0000);
	//WMLCDCOM(0x0041);WMLCDDATA(0x0000);
	LCD_CtrlWrite_S6D0139(0x0041);
	LCD_DataWrite_S6D0139(0x0000);
	//WMLCDCOM(0x0042);WMLCDDATA(0x013F);
	LCD_CtrlWrite_S6D0139(0x0042);
	LCD_DataWrite_S6D0139(0x013F);
	//WMLCDCOM(0x0043);WMLCDDATA(0x0000);
	LCD_CtrlWrite_S6D0139(0x0043);
	LCD_DataWrite_S6D0139(0x0000);
	//WMLCDCOM(0x0044);WMLCDDATA(0x0000);
	LCD_CtrlWrite_S6D0139(0x0044);
	LCD_DataWrite_S6D0139(0x0000);
	//WMLCDCOM(0x0045);WMLCDDATA(0x0000);
	LCD_CtrlWrite_S6D0139(0x0045);
	LCD_DataWrite_S6D0139(0x0000);
	
	
	//WMLCDCOM(0x0046);WMLCDDATA(0xEF00);
	LCD_CtrlWrite_S6D0139(0x0046);
	LCD_DataWrite_S6D0139(0xEF00);
	//WMLCDCOM(0x0047);WMLCDDATA(0x013F);
	LCD_CtrlWrite_S6D0139(0x0047);
	LCD_DataWrite_S6D0139(0x013F);
	//WMLCDCOM(0x0048);WMLCDDATA(0x0000);
	LCD_CtrlWrite_S6D0139(0x0048);
	LCD_DataWrite_S6D0139(0x0000);
	//WMLCDCOM(0x0007);WMLCDDATA(0x0011);
	LCD_CtrlWrite_S6D0139(0x0007);
	LCD_DataWrite_S6D0139(0x0011);
	LCD_Delayms(40);
	//WMLCDCOM(0x0007);WMLCDDATA(0x0017);
	LCD_CtrlWrite_S6D0139(0x0007);
	LCD_DataWrite_S6D0139(0x0017);
	//WMLCDCOM(0x0020);WMLCDDATA(0x0000);
	LCD_CtrlWrite_S6D0139(0x0020);
	LCD_DataWrite_S6D0139(0x0000);
	//WMLCDCOM(0x0021);WMLCDDATA(0x0000);
	LCD_CtrlWrite_S6D0139(0x0021);
	LCD_DataWrite_S6D0139(0x0000);
	LCD_Delayms(40);
	//WMLCDCOM(0x0022);
	LCD_CtrlWrite_S6D0139(0x0022);

	return ERR_LCD_NONE;
}//end of  S6D0139_drive



/**************************************************************************************/
// Description: Enter/Exit sleep mode
// Global resource dependence:
// Author: Jianping.wnag
// Note:
//		is_sleep - 1:enter into sleep;0:exit sleep
/**************************************************************************************/
LOCAL ERR_LCD_E S6D0139_EnterSleep(BOOLEAN is_sleep)
{
	if(is_sleep)
	{

		LCD_CtrlWrite_S6D0139(0x0007);
		LCD_DataWrite_S6D0139(0x0016);
		LCD_Delayms(40); 
		
		LCD_CtrlWrite_S6D0139(0x0007);
		LCD_DataWrite_S6D0139(0x0014);
		LCD_Delayms(40);
		
		LCD_CtrlWrite_S6D0139(0x0010);
		LCD_DataWrite_S6D0139(0x0000);
		
		LCD_CtrlWrite_S6D0139(0x0013);
		LCD_DataWrite_S6D0139(0x0060);
		LCD_Delayms(60);
		
		LCD_CtrlWrite_S6D0139(0x0013);
		LCD_DataWrite_S6D0139(0x0040);
		LCD_Delayms(60);
		
		LCD_CtrlWrite_S6D0139(0x0013);
		LCD_DataWrite_S6D0139(0x0000);
		LCD_Delayms(60);
		
		LCD_CtrlWrite_S6D0139(0x0010);
		LCD_DataWrite_S6D0139(0x0002);		
		
		//LCD_Delayms(100);
		LCD_Delayms(10);
	}
	else
	{		
		LCD_Delayms(30);
		
		//LCDREG(0x00,0x10);LCDDATA(0x3C,0x00);  
	    //LCDREG(0x00,0x11);LCDDATA(0x26,0x04); 
		//LCDREG(0x00,0x13);LCDDATA(0x00,0x40);LCD_Delayms(10);                                          
		//LCDREG(0x00,0x13);LCDDATA(0x00,0x60);LCD_Delayms(50);                                             
		//LCDREG(0x00,0x13);LCDDATA(0x00,0x70);                
		//LCDREG(0x00,0x14);LCDDATA(0x12,0x12);       
		//LCDREG(0x00,0x07);LCDDATA(0x00,0x11);LCD_Delayms(40);                                            
		//LCDREG(0x00,0x07);LCDDATA(0x00,0x17);    
		LCD_CtrlWrite_S6D0139(0x0010);
		LCD_DataWrite_S6D0139(0x3c00);		
		
		LCD_CtrlWrite_S6D0139(0x0011);
		LCD_DataWrite_S6D0139(0x2604);
		
		LCD_CtrlWrite_S6D0139(0x0013);
		LCD_DataWrite_S6D0139(0x0040);
		LCD_Delayms(10);
		
		LCD_CtrlWrite_S6D0139(0x0013);
		LCD_DataWrite_S6D0139(0x0060);
		LCD_Delayms(50);
		
		LCD_CtrlWrite_S6D0139(0x0013);
		LCD_DataWrite_S6D0139(0x0070);
		
		LCD_CtrlWrite_S6D0139(0x0014);
		LCD_DataWrite_S6D0139(0x1212);
		
		LCD_CtrlWrite_S6D0139(0x0007);
		LCD_DataWrite_S6D0139(0x0011);
		LCD_Delayms(40);
		LCD_CtrlWrite_S6D0139(0x0007);
		LCD_DataWrite_S6D0139(0x0017);
		
  	}

	 return ERR_LCD_NONE;
}//end of S6D0139_EnterSleep
/**************************************************************************************/
// Description: close lcdc and lcd
// Global resource dependence:
// Author: Jianping.wang
/**************************************************************************************/
LOCAL void S6D0139_Close(void)
{
	S6D0139_EnterSleep(SCI_TRUE);
	
}//end of S6D0139_Close
/**************************************************************************************/
// Description: invalidate whole LCD
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E S6D0139_Invalidate(void)
{		
	S6D0139_set_display_window(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);
	return ERR_LCD_NONE;
}//end of S6D0139_Invalidate
/**************************************************************************************/
// Description: refresh a rectangle of lcd
// Global resource dependence:
// Author: Jianping.wang
// Note:
//		left - the left value of the rectangel
//   	top - the top value of the rectangel
// 		right - the right value of the rectangel
//		bottom - the bottom value of the rectangel
/**************************************************************************************/
LOCAL ERR_LCD_E S6D0139_InvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom)
{
    
    left 	= (left >= LCD_WIDTH)    ? LCD_WIDTH-1 : left;
    right 	= (right >= LCD_WIDTH)   ? LCD_WIDTH-1 : right;
    top 	= (top >= LCD_HEIGHT)    ? LCD_HEIGHT-1 : top;
    bottom 	= (bottom >= LCD_HEIGHT) ? LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
    	return ERR_LCD_OPERATE_FAIL;
    }     
   
    S6D0139_set_display_window(left, top, right, bottom); 
    return ERR_LCD_NONE;
}//en of S6D0139_InvalidateRect	
/**************************************************************************************/
// Description: refresh a rectangle of lcd
// Global resource dependence:
// Author: Jianping.wang
// Note:
//		left - the left value of the rectangel
//   	top - the top value of the rectangel
// 		right - the right value of the rectangel
//		bottom - the bottom value of the rectangel
/**************************************************************************************/
LOCAL ERR_LCD_E S6D0139_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	switch(angle)
	{
		case LCD_ANGLE_0:
			S6D0139_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			S6D0139_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			S6D0139_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			S6D0139_set_display_window(left, top, bottom,right);
			break;
		default:
			S6D0139_set_display_window(left, top, right, bottom);
			break;			
	}
		

	return ERR_LCD_NONE;
}//en of S6D0139_VerticalInvalidateRect	

/**************************************************************************************/
//
/**************************************************************************************/
LOCAL ERR_LCD_E S6D0139_SetContrast(uint16 contrast)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
}//end of S6D0139_SetContrast

/**************************************************************************************/
//
/**************************************************************************************/
LOCAL ERR_LCD_E S6D0139_SetBrightness(uint16 brightness)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
}//end of S6D0139_SetBrightness

/**************************************************************************************/
//
/**************************************************************************************/
LOCAL ERR_LCD_E S6D0139_SetDisplayWindow(uint16 left,uint16 top,uint16 right,uint16 bottom)
{
	S6D0139_set_display_window(left, top, right,bottom);
	return ERR_LCD_NONE;
}//end of S6D0139_SetDisplayWindow

/**************************************************************************************/
// S6D0139_operations:LCD drive interfaces struction
/**************************************************************************************/
LOCAL const LCD_OPERATIONS_T S6D0139_operations = 
{
	S6D0139_Init,
	S6D0139_EnterSleep,
	S6D0139_SetContrast,
	S6D0139_SetBrightness,
	S6D0139_SetDisplayWindow,
	S6D0139_InvalidateRect,
	S6D0139_Invalidate,
	S6D0139_Close,
	S6D0139_RotationInvalidateRect,
	S6D0139_SetDirection,
    NULL,
    NULL
};
/**************************************************************************************/
// Description: get S1D19105 driver function
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
PUBLIC LCD_OPERATIONS_T* S6D0139_GetOperations(void)
{
	return PNULL;//(LCD_OPERATIONS_T*)&S6D0139_operations;
}//end of S6D0139_GetOperations

PUBLIC BOOLEAN S6D0139_Probe(void)
{
    return SCI_TRUE;    
}

LOCAL const LCD_TIMING_U s_S6D0139_timing =
{
	        // LCM_CYCLE_U start(ns)
	10,		// CS setup time for LCM read (optional)  
	150,		// low pulse width for LCM read (according spec)	
	100,		// high pulse width for LCM read (according spec)
	30,		// CS setup time for LCM write  (optional) 	
	50,		// low pulse width for LCM write (according spec)
	70,	    // high pulse width for LCM write (according spec)	
	        // LCM_CYCLE_U end(ns)
};

PUBLIC const LCD_SPEC_T g_lcd_S6D0139 = 
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U*)&s_S6D0139_timing, 
	(LCD_OPERATIONS_T*)&S6D0139_operations,
	0,
	0
};


/**-----------------------------------------------------------------------------------*
 ** 						Compiler Flag											  *
 **-----------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


