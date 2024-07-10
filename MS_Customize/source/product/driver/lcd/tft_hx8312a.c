/****************************************************************************************
** File Name:      tft_HX8312A.c                                      					*
** DATE:           20/11/2006                                                			*
** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         			*
** Description:    this file defines HX8312 LCD interface,LCDC MCU MODE					*
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
#include "lcd_cfg.h"

/**-----------------------------------------------------------------------------------*
 ** 						Compiler Flag											  *
 **-----------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


#define LCD_CtrlWrite_HX8312(_cmd)\
                LCD_SendCmd( (_cmd ), 0 );	
                
#define LCD_DataWrite_HX8312(_data)\
                LCD_SendData( (_data), 0 );                 	
                                 
#define LCD_CtrlDataWrite_HX8312(_cmd, _data)\
                LCD_SendCmdData( (_cmd ),  (_data),0 );   
                
#define LCD_Delay( ticks)\
				OS_TickDelay( ticks );                                
                                 
                                 
/**--------------------------------------------------------------------------*
 **                         Constant DEFINITION                              *
 **--------------------------------------------------------------------------*/ 
 
/******************************************************************************/
// Description: Close the lcd.(include sub lcd.)
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/******************************************************************************/
LOCAL void HX8312_Close(void);
/******************************************************************************/
// Description: Enter/Exit sleep mode .
// Global resource dependence: 
// Author: Jianping.wang
// Note:
//		is_sleep - SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
/******************************************************************************/
LOCAL ERR_LCD_E  HX8312_EnterSleep(BOOLEAN is_sleep);

/******************************************************************************/
// Description: Initialize color LCD : HX8312
// Global resource dependence: 
// Author: Jianping.wang
// Note:           
/******************************************************************************/
LOCAL ERR_LCD_E HX8312_Init(void);
/******************************************************************************/
// Description: invalidate a rectang of in LCD
// Global resource dependence: 
// Author:  Jianping.wang
// Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8312_Invalidate(void);
/******************************************************************************/
// Description: Copy a retangle data from clcd_buffer to display RAM.
//              then the rectangle display is to be refreshed
// Global resource dependence: 
// Author: Jianping.wang
// Note:       
/******************************************************************************/
LOCAL ERR_LCD_E HX8312_InvalidateRect(
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
LOCAL ERR_LCD_E HX8312_SetDirection(LCD_DIRECT_E direct_type) 
{
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:			
			//H01<-10,control register 
		    LCD_CtrlWrite_HX8312(0x0110);
		    //H05<-10,RAM access control register
		    LCD_CtrlWrite_HX8312(0x0510);	
			break;
			
		case LCD_DIRECT_ROT_90:
			//H01<-10,control register 
		    LCD_CtrlWrite_HX8312(0x0150);
		    //H05<-10,RAM access control register
		    LCD_CtrlWrite_HX8312(0x0514);	 
			break;
			
		case LCD_DIRECT_ROT_180:
			//H01<-10,control register 
		    LCD_CtrlWrite_HX8312(0x01D0);
		    //H05<-10,RAM access control register
		    LCD_CtrlWrite_HX8312(0x0510);	 
			break;
			
		case LCD_DIRECT_ROT_270:
			//H01<-90,control register 
	    	LCD_CtrlWrite_HX8312(0x0190);
	    	//H05<-14,RAM access control register
	    	LCD_CtrlWrite_HX8312(0x0514); 
			break;

		case LCD_DIRECT_MIR_H:

			break;
			
		case LCD_DIRECT_MIR_V:

			break;
			
		case LCD_DIRECT_MIR_HV:

			break;
			
		default:			
			//H01<-10,control register 
		    LCD_CtrlWrite_HX8312(0x0110);
		    //H05<-10,RAM access control register
		    LCD_CtrlWrite_HX8312(0x0510);
		    direct_type = LCD_DIRECT_NORMAL;
			break;
			
	}	

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
__inline void HX8312_set_display_window(uint16 left, uint16 top, uint16 right, uint16 bottom)
{	
	//uint16 cmd;	
	
	LCD_CtrlWrite_HX8312(0x0020);

}//end of HX8312_set_display_window
/**************************************************************************************/
// Description: drive LCD
// Global resource dependence:
// Author: Jianping.wang
// Note:
// 		
/**************************************************************************************/
LOCAL ERR_LCD_E HX8312_Init(void)
{

	//SCI_TRACE_LOW:"HX8312 initialize!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8312A_165_112_2_18_0_33_26_1742,(uint8*)"");

    LCD_Reset(); //reset lcd throught lcdc	
    SCI_Sleep(100);

    //H59<-01,
    LCD_CtrlWrite_HX8312(0x5901);
    
    //H60<-22
    LCD_CtrlWrite_HX8312(0x6022);
    
    //H59<-00
    LCD_CtrlWrite_HX8312(0x5900);
    
    SCI_Sleep(50);
    
    //H00<-A0,controle register 1
    LCD_CtrlWrite_HX8312(0x00A0);
    
    //H02<-00,RGB interface register
    LCD_CtrlWrite_HX8312(0x0200);
    
    //H01<-10,control register 
    LCD_CtrlWrite_HX8312(0x0110);
    //H05<-10,RAM access control register
    LCD_CtrlWrite_HX8312(0x0510);

    //H06<-00,data reserve register
    LCD_CtrlWrite_HX8312(0x0600);
    
    //H07<-00
    LCD_CtrlWrite_HX8312(0x0700);
    
    //H0D<-00,display size control register
    LCD_CtrlWrite_HX8312(0x0D00);
    
    SCI_Sleep(50);
    //H2B<-00,OSC resistance control
    LCD_CtrlWrite_HX8312(0x2B00);
    
    //H9D<-00,extend mode register
    LCD_CtrlWrite_HX8312(0x9D00);
   
    //H1D<-08,power supply system control register 6
    LCD_CtrlWrite_HX8312(0x1D08);
    
    //H86<-00,gate on interval control register
    LCD_CtrlWrite_HX8312(0x8600);
    
    //H87<-3F
    LCD_CtrlWrite_HX8312(0x873F);
    
    //H88<-02,source on interval control register
    LCD_CtrlWrite_HX8312(0x8802);
    
    //H89<-05
    LCD_CtrlWrite_HX8312(0x8905);
    
    //H8D<-01,line frequency control register
    LCD_CtrlWrite_HX8312(0x8D01);
     
    //H8B<-28,line clock register 1
    LCD_CtrlWrite_HX8312(0x8B28);
    
    //H33<-01
    LCD_CtrlWrite_HX8312(0x3301);
    
    //H37<-01
    LCD_CtrlWrite_HX8312(0x3701);
    
    //H76<-00
    LCD_CtrlWrite_HX8312(0x7600);
    
    SCI_Sleep(50);
    
    //H0E<-00
    LCD_CtrlWrite_HX8312(0x0E00);
    
    //H0F<-00
    LCD_CtrlWrite_HX8312(0x0F00);
    
    //H10<-00,first display window area starting register 1
    LCD_CtrlWrite_HX8312(0x1000);
    
    //H11<-00,first display window area starting register 2
    LCD_CtrlWrite_HX8312(0x1100);
    
    //H12<-00,second display window area starting register 1 
    LCD_CtrlWrite_HX8312(0x1200);
    
    //H13<-1A,second display window area starting register 2
    LCD_CtrlWrite_HX8312(0x131A);
    
    //H14<-00,first display window area display line number 1
    LCD_CtrlWrite_HX8312(0x1400);
    
    //H15<-07,first display window area display line number 2
    LCD_CtrlWrite_HX8312(0x1507);
    
    //H16<-00,second display window area display line number 1
    LCD_CtrlWrite_HX8312(0x1600);
    
    //H17<-11,second display window area display line number 2
    LCD_CtrlWrite_HX8312(0x1711);
    
    //H34<-01
    LCD_CtrlWrite_HX8312(0x3401);
    
    //H35<-00
    LCD_CtrlWrite_HX8312(0x3500);
    
    //H4B<-00
    LCD_CtrlWrite_HX8312(0x4B00);
    
    //H4C<-00
    LCD_CtrlWrite_HX8312(0x4C00);
    
    //H4D<-00
    LCD_CtrlWrite_HX8312(0x4D00);
    
    //H4E<-00
    LCD_CtrlWrite_HX8312(0x4E00);
    
    //H4F<-00
    LCD_CtrlWrite_HX8312(0x4F00);
    
    //H50<-00
    LCD_CtrlWrite_HX8312(0x5000);
    
    SCI_Sleep(50);
    
    //H42<-00,x address register,0~239
    LCD_CtrlWrite_HX8312(0x4200);
    
    //H43<-00,y address register 0~319
    LCD_CtrlWrite_HX8312(0x4300);
    
    //H44<-00
    LCD_CtrlWrite_HX8312(0x4400);
    
    //H45<-00,x start address ,0~239
    LCD_CtrlWrite_HX8312(0x4500);
    
    //H46<-EF,x end address
    LCD_CtrlWrite_HX8312(0x46EF);
    
    //H47<-00,y start address,0~319
    LCD_CtrlWrite_HX8312(0x4700);
    
    //H48<-00
    LCD_CtrlWrite_HX8312(0x4800);
    
    //H49<-01,y end address
    LCD_CtrlWrite_HX8312(0x4901);
    
    //H4A<-3F
    LCD_CtrlWrite_HX8312(0x4A3F);
    
    SCI_Sleep(50);
    
    //H2C<-00
    LCD_CtrlWrite_HX8312(0x2C00);
    
    //H3C<-00,rgb start line register 1
    LCD_CtrlWrite_HX8312(0x3C00);
    
    //H3D<-00,rgb start line register 2
    LCD_CtrlWrite_HX8312(0x3D00);
    
    //H3E<-01,,rgb end line register 1
    LCD_CtrlWrite_HX8312(0x3E01);
    
    //H3F<-3F,,rgb end line register
    LCD_CtrlWrite_HX8312(0x3F3F);
    
    //H40<-02
    LCD_CtrlWrite_HX8312(0x4002);
    
    //H41<-02
    LCD_CtrlWrite_HX8312(0x4102);
    
    SCI_Sleep(50);
    
    //H8F<-00 gamma control register 1
    LCD_CtrlWrite_HX8312(0x8F00);
    
    //H90<-56 gamma control register 2
    LCD_CtrlWrite_HX8312(0x9056);
    
    //H91<-07 gamma control register 3
    LCD_CtrlWrite_HX8312(0x9107);
    
    //H92<-62 gamma control register 4
    LCD_CtrlWrite_HX8312(0x9262);
    
    //H93<-07 gamma control register 5
    LCD_CtrlWrite_HX8312(0x9307);
    
    //H94<-12 gamma control register 6
    LCD_CtrlWrite_HX8312(0x9412);
    
    //H95<-77 gamma control register 7
    LCD_CtrlWrite_HX8312(0x9577);
    
    //H96<-26 gamma control register 8
    LCD_CtrlWrite_HX8312(0x9626);

    //H97<-00 gamma control register 9
    LCD_CtrlWrite_HX8312(0x9700);    
    
    //H98<-0C gamma control register 10
    LCD_CtrlWrite_HX8312(0x980C);
    
    //H99<-06 gamma control register 11
    LCD_CtrlWrite_HX8312(0x9906);
    
    //H9A<-00 gamma control register 12
    LCD_CtrlWrite_HX8312(0x9A00);
      
    //HC0<-00 off mode register
    LCD_CtrlWrite_HX8312(0xC000);
     
    //HC1<-01,RGB mode register
    LCD_CtrlWrite_HX8312(0xC101);
         
    SCI_Sleep(50);
    
    //H28<-20
    LCD_CtrlWrite_HX8312(0x2820);
    
    //H1A<-05
    LCD_CtrlWrite_HX8312(0x1A05);
     
    //H1C<-33
    LCD_CtrlWrite_HX8312(0x1C33);
      
    //H55<-08
    LCD_CtrlWrite_HX8312(0x5508);
    
    //H24<-74
    LCD_CtrlWrite_HX8312(0x2474);
    
    //H25<-0A
    LCD_CtrlWrite_HX8312(0x250A);
    
    SCI_Sleep(50);
    
    //H19<-00
    LCD_CtrlWrite_HX8312(0x1900);
    
    //H18<-C1
    LCD_CtrlWrite_HX8312(0x18C1);
    
    //H1E<-01
    LCD_CtrlWrite_HX8312(0x1E01);
    
    //H18<-C5
    LCD_CtrlWrite_HX8312(0x18C5);
    
    //H18<-E5
    LCD_CtrlWrite_HX8312(0x18E5);
    
    //H18<-F5
    LCD_CtrlWrite_HX8312(0x18F5);
    
    //H1B<-09
    LCD_CtrlWrite_HX8312(0x1B09);
    
    //H1F<-0C
    LCD_CtrlWrite_HX8312(0x1F0C);
    
    //H20<-0E
    LCD_CtrlWrite_HX8312(0x200E);
    
    //H1E<-81
    LCD_CtrlWrite_HX8312(0x1E81);
       
    //H3B<-01
    LCD_CtrlWrite_HX8312(0x3B01);
    
    //H00<-20
    LCD_CtrlWrite_HX8312(0x0020);

	return ERR_LCD_NONE;
}


/**************************************************************************************/
// Description: Enter/Exit sleep mode
// Global resource dependence:
// Author: Jianping.wnag
// Note:
//		is_sleep - 1:enter into sleep;0:exit sleep
/**************************************************************************************/
LOCAL ERR_LCD_E HX8312_EnterSleep(BOOLEAN is_sleep)
{
	//SCI_TRACE_LOW:"HX8312_EnterSleep,%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8312A_459_112_2_18_0_33_27_1743,(uint8*)"d", is_sleep);
	if(is_sleep)
	{//into standby mode
		LCD_CtrlWrite_HX8312(0x00A0);
		SCI_Sleep(50);
		LCD_CtrlWrite_HX8312(0x3B00);
		LCD_CtrlWrite_HX8312(0x1E01);
		LCD_CtrlWrite_HX8312(0x1B08);
		LCD_CtrlWrite_HX8312(0x1C00);
		LCD_CtrlWrite_HX8312(0x2400);
		LCD_CtrlWrite_HX8312(0x1800);
		LCD_CtrlWrite_HX8312(0x0111);
		LCD_CtrlWrite_HX8312(0x0028);
		
		SCI_Sleep(10);
	}
	else
	{	
		SCI_Sleep(30);
		
		LCD_CtrlWrite_HX8312(0x00A0);
		LCD_CtrlWrite_HX8312(0x0110);

		SCI_Sleep(50);
		LCD_CtrlWrite_HX8312(0x1C34);
		LCD_CtrlWrite_HX8312(0x2474);
		LCD_CtrlWrite_HX8312(0x1E01);
		LCD_CtrlWrite_HX8312(0x18C1);
		SCI_Sleep(50);
		LCD_CtrlWrite_HX8312(0x18C5);
		LCD_CtrlWrite_HX8312(0x18E5);
		SCI_Sleep(50);
		LCD_CtrlWrite_HX8312(0x18F5);
		SCI_Sleep(50);
		LCD_CtrlWrite_HX8312(0x1B09);
		SCI_Sleep(50);
		LCD_CtrlWrite_HX8312(0x1E81);
		SCI_Sleep(50);
		LCD_CtrlWrite_HX8312(0x3B01);
		SCI_Sleep(50);
		LCD_CtrlWrite_HX8312(0x0020);
       	SCI_Sleep(10);
  	}

	 return ERR_LCD_NONE;
}
/**************************************************************************************/
// Description: close lcdc and lcd
// Global resource dependence:
// Author: Jianping.wang
/**************************************************************************************/
LOCAL void HX8312_Close(void)
{
	HX8312_EnterSleep(SCI_TRUE);
}
/**************************************************************************************/
// Description: invalidate whole LCD
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E HX8312_Invalidate(void)
{				
    
	HX8312_set_display_window(0, 0, QVGA_LCD_WIDTH - 1, QVGA_LCD_HEIGHT - 1);
  	  
	return ERR_LCD_NONE;
}
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
LOCAL ERR_LCD_E HX8312_InvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom)  
{

    left 	= (left >= QVGA_LCD_WIDTH)    ? QVGA_LCD_WIDTH-1 : left;
    right 	= (right >= QVGA_LCD_WIDTH)   ? QVGA_LCD_WIDTH-1 : right;
    top 	= (top >= QVGA_LCD_HEIGHT)    ? QVGA_LCD_HEIGHT-1 : top;
    bottom 	= (bottom >= QVGA_LCD_HEIGHT) ? QVGA_LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
    	return ERR_LCD_OPERATE_FAIL;
    }     
   
    HX8312_set_display_window(left, top, right, bottom); 
    
    return ERR_LCD_NONE;
}

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
LOCAL ERR_LCD_E HX8312_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{

	switch(angle)
	{
		case LCD_ANGLE_0:
			HX8312_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			HX8312_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			HX8312_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			HX8312_set_display_window(left, top, bottom,right);
			break;
		default:
			HX8312_set_display_window(left, top, right, bottom);
			break;			
	}
		

	return ERR_LCD_NONE;
}

/**************************************************************************************/
//
/**************************************************************************************/
LOCAL ERR_LCD_E HX8312_SetContrast(uint16 contrast)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
}

/**************************************************************************************/
//
/**************************************************************************************/
LOCAL ERR_LCD_E HX8312_SetBrightness(uint16 brightness)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
}

/**************************************************************************************/
//
/**************************************************************************************/
LOCAL ERR_LCD_E HX8312_SetDisplayWindow(uint16 left,uint16 top,uint16 right,uint16 bottom)
{
	
    HX8312_set_display_window(left, top, right, bottom);
	return ERR_LCD_NONE;
}

/**************************************************************************************/
// S1D19105_operations:LCD drive interfaces struction
/**************************************************************************************/
LOCAL const LCD_OPERATIONS_T HX8312_operations = 
{
	HX8312_Init,
	HX8312_EnterSleep,
	HX8312_SetContrast,
	HX8312_SetBrightness,
	HX8312_SetDisplayWindow,
	HX8312_InvalidateRect,
	HX8312_Invalidate,
	HX8312_Close,
	HX8312_RotationInvalidateRect,
	HX8312_SetDirection,
    NULL,
    NULL
};


/**************************************************************************************/
// Description: get S1D19105 driver function
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
PUBLIC LCD_OPERATIONS_T* HX8312_GetOperations(void)
{
	return (LCD_OPERATIONS_T*)&HX8312_operations;
}

/**************************************************************************************/
// Description:probe lcd
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN HX8312_Probe(void)
{
	return SCI_TRUE;	
}

LOCAL const LCD_TIMING_U s_HX8312_timing =
{

	        // LCM_CYCLE_U start(ns)
	10,		// CS setup time for LCM read (optional)  
	150,	// low pulse width for LCM read (according spec)	
	100,	// high pulse width for LCM read (according spec)
	30,		// CS setup time for LCM write  (optional) 	
	50,		// low pulse width for LCM write (according spec)
	70,	    // high pulse width for LCM write (according spec)	
	        // LCM_CYCLE_U end(ns)
};


const LCD_SPEC_T g_lcd_HX8312[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U*)&s_HX8312_timing, 
	(LCD_OPERATIONS_T*)&HX8312_operations,
	0,
	0
	}
};

/**-----------------------------------------------------------------------------------*
 ** 						Compiler Flag											  *
 **-----------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


//end of tft_HX8312A.c
