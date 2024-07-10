/****************************************************************************************
** File Name:      tft_R61581_lcdc.c                                           					*
** DATE:           29/4/2007                                                			*
** Copyright:      2007 Spreatrum, Incoporated. All Rights Reserved.         			*
** Description:    this file defines R61581 LCD interface,LCDC MCU MODE			*
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

#define LCD_CtrlWrite_R61581( _data )\
				LCD_SendCmd( (_data ), 0 );

#define LCD_DataWrite_R61581( _data )\
				LCD_SendData( (_data), 0 );

#define LCD_CtrlData_R61581( _cmd, _data ) \
				LCD_SendCmdData( (_cmd ),  (_data),0 );  	
				
//LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;	for pclint no used		
	
/**--------------------------------------------------------------------------*
 **                         Constant DEFINITION                              *
 **--------------------------------------------------------------------------*/ 

/******************************************************************************/
// Description: Close the lcd.(include sub lcd.)
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/******************************************************************************/
LOCAL void R61581_Close(void);
/******************************************************************************/
// Description: Enter/Exit sleep mode .
// Global resource dependence: 
// Author: Jianping.wang
// Note:
//		is_sleep - SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
/******************************************************************************/
LOCAL ERR_LCD_E  R61581_EnterSleep(BOOLEAN is_sleep);

/******************************************************************************/
// Description: Initialize color LCD : R61581
// Global resource dependence: 
// Author: Jianping.wang
// Note:           
/******************************************************************************/
LOCAL ERR_LCD_E R61581_Init(void);
/******************************************************************************/
// Description: invalidate a rectang of in LCD
// Global resource dependence: 
// Author:  Jianping.wang
// Note:
/******************************************************************************/
LOCAL ERR_LCD_E R61581_Invalidate(void);
/******************************************************************************/
// Description: Copy a retangle data from clcd_buffer to display RAM.
//              then the rectangle display is to be refreshed
// Global resource dependence: 
// Author: Jianping.wang
// Note:       
/******************************************************************************/
LOCAL ERR_LCD_E R61581_InvalidateRect(
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
LOCAL ERR_LCD_E R61581_SetDirection( LCD_DIRECT_E direct_type)
{
   // return;
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:			
                    LCD_CtrlWrite_R61581(0x00C0);
                    LCD_DataWrite_R61581(0x0003);
                    LCD_DataWrite_R61581(0x003B);
                    LCD_DataWrite_R61581(0x0000);
                    LCD_DataWrite_R61581(0x0002);
                    LCD_DataWrite_R61581(0x0000);
                    LCD_DataWrite_R61581(0x0001);
                    LCD_DataWrite_R61581(0x0000);
                    LCD_DataWrite_R61581(0x0043);
			break;
			
		case LCD_DIRECT_ROT_90:
                    LCD_CtrlWrite_R61581(0x00C0);
                    LCD_DataWrite_R61581(0x000B);
                    LCD_DataWrite_R61581(0x003B);
                    LCD_DataWrite_R61581(0x0000);
                    LCD_DataWrite_R61581(0x0002);
                    LCD_DataWrite_R61581(0x0000);
                    LCD_DataWrite_R61581(0x0001);
                    LCD_DataWrite_R61581(0x0000);
                    LCD_DataWrite_R61581(0x0043);	 
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlWrite_R61581(0x0003);		    
		    LCD_DataWrite_R61581(0x1000);	 
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlWrite_R61581(0x0003);		    
		    LCD_DataWrite_R61581(0x1018);
			break;

		case LCD_DIRECT_MIR_H:
			LCD_CtrlWrite_R61581(0x0003);		    
		    LCD_DataWrite_R61581(0x1020);
			break;
			
		case LCD_DIRECT_MIR_V:
			LCD_CtrlWrite_R61581(0x0003);		    
		    LCD_DataWrite_R61581(0x1010);
			break;
			
		case LCD_DIRECT_MIR_HV:
			LCD_CtrlWrite_R61581(0x0003);		    
		    LCD_DataWrite_R61581(0x1000);
			break;
			
		default:			
			LCD_CtrlWrite_R61581(0x0003);		    
		    LCD_DataWrite_R61581(0x1030);	
		    direct_type = LCD_DIRECT_NORMAL;
			break;
			
	}	

	//s_lcd_direct = direct_type; for pclint no used

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
__inline void R61581_set_display_window(uint16 left, uint16 top, uint16 right, uint16 bottom)
{	
	LCD_CtrlWrite_R61581(0x002A);
	LCD_DataWrite_R61581(left>>8);
	LCD_DataWrite_R61581(left&0xff);
	LCD_DataWrite_R61581(right>>8);
	LCD_DataWrite_R61581(right&0xff);

	LCD_CtrlWrite_R61581(0x002B);
	LCD_DataWrite_R61581(top>>8);
	LCD_DataWrite_R61581(top&0xff);
	LCD_DataWrite_R61581(bottom>>8);
	LCD_DataWrite_R61581(bottom&0xff);

	LCD_CtrlWrite_R61581(0x002c);
    
}//end of R61581_set_display_window
/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E R61581_Init(void)
{
	//SCI_TRACE_LOW:"R61581 initialize!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_R61581_181_112_2_18_0_34_4_1855,(uint8*)"");


	LCD_Delayms(10);
	//WMLCDCOM(0x0000);WMLCDDATA(0x0001);
	LCD_CtrlWrite_R61581(0xB0);
	LCD_DataWrite_R61581(0x0);
	//	LCD_Delayms(10);
	//WMLCDCOM(0x0007);WMLCDDATA(0x0000);
	LCD_CtrlWrite_R61581(0xB3);
	LCD_DataWrite_R61581(0x0002);
	LCD_DataWrite_R61581(0x0000);
	LCD_DataWrite_R61581(0x0000);
	LCD_DataWrite_R61581(0x0000);
	//WMLCDCOM(0x0013);WMLCDDATA(0x0000);
	LCD_CtrlWrite_R61581(0x00B4);
	LCD_DataWrite_R61581(0x0000);
	//WMLCDCOM(0x0011);WMLCDDATA(0x2604);
	LCD_CtrlWrite_R61581(0x00C0);
	LCD_DataWrite_R61581(0x0003);
	LCD_DataWrite_R61581(0x003B);
	LCD_DataWrite_R61581(0x0000);
	LCD_DataWrite_R61581(0x0002);
	LCD_DataWrite_R61581(0x0000);
	LCD_DataWrite_R61581(0x0001);
	LCD_DataWrite_R61581(0x0000);
	LCD_DataWrite_R61581(0x0043);

	//WMLCDCOM(0x0014);WMLCDDATA(0x1212);
	LCD_CtrlWrite_R61581(0x00C1);
	LCD_DataWrite_R61581(0x0008);
	LCD_DataWrite_R61581(0x0017);
	LCD_DataWrite_R61581(0x0008);
	LCD_DataWrite_R61581(0x0008);
	//WMLCDCOM(0x0010);WMLCDDATA(0x3C00);
	LCD_CtrlWrite_R61581(0x00C4);
	LCD_DataWrite_R61581(0x0022);
	LCD_DataWrite_R61581(0x0002);
	LCD_DataWrite_R61581(0x0000);
	LCD_DataWrite_R61581(0x0000);
	//WMLCDCOM(0x0013);WMLCDDATA(0x0040);
	LCD_CtrlWrite_R61581(0x00C8);
	LCD_DataWrite_R61581(0x0009);
	LCD_DataWrite_R61581(0x0008);
	LCD_DataWrite_R61581(0x0010);//16
	LCD_DataWrite_R61581(0x0085);//5A
	LCD_DataWrite_R61581(0x0007);
	LCD_DataWrite_R61581(0x0008);
	LCD_DataWrite_R61581(0x0016);
	LCD_DataWrite_R61581(0x0005);
	LCD_DataWrite_R61581(0x0000);
	LCD_DataWrite_R61581(0x0032);
	LCD_DataWrite_R61581(0x0005);
	LCD_DataWrite_R61581(0x0016);
	LCD_DataWrite_R61581(0x0008);
	LCD_DataWrite_R61581(0x0088);//43/55
	LCD_DataWrite_R61581(0x0009);
	LCD_DataWrite_R61581(0x0010);
	LCD_DataWrite_R61581(0x0009);
	LCD_DataWrite_R61581(0x0004);
	LCD_DataWrite_R61581(0x0032);
	LCD_DataWrite_R61581(0x0000);

	LCD_CtrlWrite_R61581(0x002A);
	LCD_DataWrite_R61581(0x0000);
	LCD_DataWrite_R61581(0x0000);
	LCD_DataWrite_R61581(0x0001);
	LCD_DataWrite_R61581(0x003F);//320

	LCD_CtrlWrite_R61581(0x002B);
	LCD_DataWrite_R61581(0x0000);
	LCD_DataWrite_R61581(0x0000);
	LCD_DataWrite_R61581(0x0001);
	LCD_DataWrite_R61581(0x00DF);//480

	LCD_CtrlWrite_R61581(0x0035);
	LCD_DataWrite_R61581(0x0000);

	LCD_CtrlWrite_R61581(0x003A);
	LCD_DataWrite_R61581(0x0005);

	LCD_CtrlWrite_R61581(0x0044);
	LCD_DataWrite_R61581(0x0000);
	LCD_DataWrite_R61581(0x0001);

	LCD_CtrlWrite_R61581(0x002C);
	LCD_CtrlWrite_R61581(0x0011);
	LCD_Delayms(150);

	LCD_CtrlWrite_R61581(0x00D0);
	LCD_DataWrite_R61581(0x0007);
	LCD_DataWrite_R61581(0x0007);
	LCD_DataWrite_R61581(0x0016);
	LCD_DataWrite_R61581(0x0072);//0073

	LCD_CtrlWrite_R61581(0x00D1);
	LCD_DataWrite_R61581(0x0003);
	LCD_DataWrite_R61581(0x003A);//VCM  52
	LCD_DataWrite_R61581(0x000A);//VDV  10

	LCD_CtrlWrite_R61581(0x00D2);
	LCD_DataWrite_R61581(0x0002);
	LCD_DataWrite_R61581(0x0044);
	LCD_DataWrite_R61581(0x0004);

	LCD_CtrlWrite_R61581(0x0029);
	LCD_Delayms(10);

	return ERR_LCD_NONE;
}//end of  R61581_drive



/**************************************************************************************/
// Description: Enter/Exit sleep mode
// Global resource dependence:
// Author: Jianping.wnag
// Note:
//		is_sleep - 1:enter into sleep;0:exit sleep
/**************************************************************************************/
LOCAL ERR_LCD_E R61581_EnterSleep(BOOLEAN is_sleep)
{
	if(is_sleep)
	{
            LCD_CtrlWrite_R61581(0x0028); 
            LCD_CtrlWrite_R61581(0x0010); 
            LCD_Delayms(150);
            LCD_CtrlWrite_R61581(0x00B0);
            LCD_DataWrite_R61581(0x0000);
            LCD_CtrlWrite_R61581(0x00B1);
            LCD_DataWrite_R61581(0x0001);
	}
	else
	{	
            LCD_CtrlWrite_R61581(0x00FF); 
            LCD_CtrlWrite_R61581(0x00FF); 
            LCD_CtrlWrite_R61581(0x00FF); 
            LCD_CtrlWrite_R61581(0x00FF); 
            LCD_CtrlWrite_R61581(0x00FF); 
            LCD_CtrlWrite_R61581(0x00FF); 
            LCD_Delayms(15);
            LCD_CtrlWrite_R61581(0x00B0);
            LCD_DataWrite_R61581(0x0000);
            LCD_CtrlWrite_R61581(0x00B1);
            LCD_DataWrite_R61581(0x0000);
            R61581_Init();		
  	}

	 return ERR_LCD_NONE;
}//end of R61581_EnterSleep
/**************************************************************************************/
// Description: close lcdc and lcd
// Global resource dependence:
// Author: Jianping.wang
/**************************************************************************************/
LOCAL void R61581_Close(void)
{
	R61581_EnterSleep(SCI_TRUE);
}//end of R61581_Close
/**************************************************************************************/
// Description: invalidate whole LCD
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E R61581_Invalidate(void)
{		
	R61581_set_display_window(0,0,LCD_WIDTH - 1,LCD_HEIGHT - 1);	  	  
	return ERR_LCD_NONE;
}//end of R61581_Invalidate
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
LOCAL ERR_LCD_E R61581_InvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom)
{
    left 	= (left >= LCD_WIDTH)    ? LCD_WIDTH-1 : left;
    right 	= (right >= LCD_WIDTH)   ? LCD_WIDTH-1 : right;
    top 	= (top >= LCD_HEIGHT)    ? LCD_HEIGHT-1 : top;
    bottom 	= (bottom >= LCD_HEIGHT) ? LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
    	return ERR_LCD_OPERATE_FAIL;
    }     
   
    R61581_set_display_window(left, top, right, bottom); 
    
    return ERR_LCD_NONE;
}//en of R61581_InvalidateRect	
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
LOCAL ERR_LCD_E R61581_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	switch(angle)
	{
		case LCD_ANGLE_0:
			R61581_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			R61581_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			R61581_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			R61581_set_display_window(left, top, bottom,right);
			break;
		default:
			R61581_set_display_window(left, top, right, bottom);
			break;			
	}
		
	return ERR_LCD_NONE;
}//en of R61581_VerticalInvalidateRect	



/**************************************************************************************/
//
/**************************************************************************************/
LOCAL ERR_LCD_E R61581_SetContrast(uint16 contrast)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
}//end of R61581_SetContrast

/**************************************************************************************/
//
/**************************************************************************************/
LOCAL ERR_LCD_E R61581_SetBrightness(uint16 brightness)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
}//end of R61581_SetBrightness

/**************************************************************************************/
//
/**************************************************************************************/
LOCAL ERR_LCD_E R61581_SetDisplayWindow(uint16 left,uint16 top,uint16 right,uint16 bottom)
{
	R61581_set_display_window(left, top, right,bottom);
	return ERR_LCD_NONE;
}//end of R61581_SetDisplayWindow

/**************************************************************************************/
// R61581_operations:LCD drive interfaces struction
/**************************************************************************************/
LOCAL const LCD_OPERATIONS_T R61581_operations = 
{
	R61581_Init,
	R61581_EnterSleep,
	R61581_SetContrast,
	R61581_SetBrightness,
	R61581_SetDisplayWindow,
	R61581_InvalidateRect,
	R61581_Invalidate,
	R61581_Close,
	R61581_RotationInvalidateRect,
	R61581_SetDirection,
    NULL,
    NULL
};
/**************************************************************************************/
// Description: get S1D19105 driver function
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
PUBLIC LCD_OPERATIONS_T* R61581_GetOperations(void)
{
	return PNULL;//(LCD_OPERATIONS_T*)&R61581_operations;
}//end of R61581_GetOperations

PUBLIC BOOLEAN R61581_Probe(void)
{
    return SCI_TRUE;    
}

LOCAL const LCD_TIMING_U s_R61581_timing =
{

	        // LCM_CYCLE_U start(ns)
	170,		// CS setup time for LCM read (optional)  
	170,	// low pulse width for LCM read (according spec)	
	250,	// high pulse width for LCM read (according spec)
	30,		// CS setup time for LCM write  (optional) 	
	30,		// low pulse width for LCM write (according spec)
	30,	    // high pulse width for LCM write (according spec)	
	        // LCM_CYCLE_U end(ns)
};

PUBLIC const LCD_SPEC_T g_lcd_R61581 = 
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U           *)&s_R61581_timing, 
	(LCD_OPERATIONS_T*)&R61581_operations,
	0,
	0
};


/**-----------------------------------------------------------------------------------*
 ** 						Compiler Flag											  *
 **-----------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


