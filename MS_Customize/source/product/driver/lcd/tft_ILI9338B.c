/******************************************************************************
 ** File Name:     TFT_ILI9338B.c                                             *
 ** Description:															  *
 **    This file contains driver for color LCD.(TFT)						  *
 ** Author:         RIYANGXIE                                              *
 ** DATE:           07/22/2011                                                *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/22/2011     XIERIYANG	    Create.
 ******************************************************************************/

#include "ms_customize_trc.h"
#include "os_api.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"

    

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif
#define ILI9338B_LCD_CtrlWrite( _cmd )\
					LCD_SendCmd( (_cmd ), 0);
	
#define ILI9338B_LCD_DataWrite( _data )\
					LCD_SendData( (_data), 0 );
	
#define ILI9338B_LCD_CtrlData( _cmd, _data ) \
					LCD_SendCmd( (_cmd )>>8, 0 );\
					LCD_SendCmd( (_cmd ), 0); \
					LCD_SendData( (_data)>>8, 0);\
					LCD_SendData( (_data), 0 );

#define LCD_CtrlWrite_ILI9338B( _cmd )\
				ILI9338B_LCD_CtrlWrite(_cmd );

#define LCD_DataWrite_ILI9338B( _data )\
				ILI9338B_LCD_DataWrite(_data);

#define LCD_CtrlData_ILI9338B( _cmd, _data ) \
				ILI9338B_LCD_CtrlData(_cmd ,  _data);  

				
#define LCD_Delay( ticks)\
				OS_TickDelay( ticks );
				
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;

LOCAL BOOLEAN g_is_invert = FALSE ;

  /******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ILI9338B_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9338B_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);
/*********************************************************************/
//  Description:   Initialize color LCD : ILI9338B
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E ILI9338B_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9338B_Invalidate(void);


/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:       
//     To improve speed, lcd is operate in HIGH SPEED RAM WRITE MODE(4
//     uint16 are write continuously always.) So, some dummy uint16 
//     should be inserted to satisfy this mode.   Please refer to spec.
/******************************************************************************/
LOCAL ERR_LCD_E ILI9338B_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	);
/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ILI9338B_set_display_window(
	uint16 left, 	
	uint16 top, 	
	uint16 right, 		
	uint16 bottom	
	);


/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

/******************************************************************************/
//  Description:  Set LCD invalidate direction
//  Input:
//      is_invert: 0, horizontal; 1, vertical
//  Return:
//      None.
//	Note: Application should correct invalidate direction right after current
//	 	  image displayed
/******************************************************************************/
LOCAL ERR_LCD_E ILI9338B_SetDirection(
	LCD_DIRECT_E direct_type
	)
{
#if 0 // by riyang.xie
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
#endif
	s_lcd_direct = direct_type;

	//SCI_TRACE_LOW:"ILI9338B set direction,direct_type=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9338B_152_112_2_18_0_33_51_1806,(uint8*)"d",direct_type);
    
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ILI9338B_set_display_window(
	uint16 left, 	
	uint16 top, 	
	uint16 right, 		
	uint16 bottom	
	)
{   
    //SCI_TRACE_LOW:"ILI9338B set dis window,s_lcd_direct=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9338B_169_112_2_18_0_33_51_1807,(uint8*)"d",s_lcd_direct);
	switch(s_lcd_direct)//switch(s_angle)
	{
		case LCD_DIRECT_NORMAL:		    
			LCD_CtrlWrite_ILI9338B(0x2a);
			LCD_DataWrite_ILI9338B((left&0xff00)>>8);
			LCD_DataWrite_ILI9338B(left&0x00ff);
			LCD_DataWrite_ILI9338B((right&0xff00)>>8);
			LCD_DataWrite_ILI9338B(right&0x00ff);

			LCD_CtrlWrite_ILI9338B(0x2b);
			LCD_DataWrite_ILI9338B((top&0xff00)>>8);
			LCD_DataWrite_ILI9338B(top&0x00ff);
			LCD_DataWrite_ILI9338B((bottom&0xff00)>>8);
			LCD_DataWrite_ILI9338B(bottom&0x00ff);
			break;
			
		case LCD_DIRECT_ROT_90:		
			
			break;
			
		case LCD_DIRECT_ROT_180:
			
			break;
			
		case LCD_DIRECT_ROT_270:
			
			break;
			
		default:

			break;	
	}
	LCD_CtrlWrite_ILI9338B(0x29);
	LCD_CtrlWrite_ILI9338B(0x2c);
}


LOCAL void ILI9338B_driver(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in ILI9338B_reset"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9338B_209_112_2_18_0_33_51_1808,(uint8*)"");

	LCD_Reset();//reset lcd throught lcdc	
	LCD_Delayms(50);
#if 0 // by riyang.xie
	//************* Detect Module IC's ID **********//
	LCD_CtrlWrite_ILI9338B(0x0);
	lcm_dev_id = LCDC_Read_Ex(0, 1);	//0x00
	//SCI_TRACE_LOW:"\"\"\"#LCD: in ILI9338B_Init ID(0x0)=,0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9338B_217_112_2_18_0_33_51_1809,(uint8*)"d",lcm_dev_id);
#endif

#if 1 // by riyang.xie
	#define  SLIDE_MM040 1
	
	LCD_CtrlWrite_ILI9338B(0x11); //Exit Sleep
	LCD_Delayms(150);
	
	LCD_CtrlWrite_ILI9338B(0xCB); //AP[2:0]
	LCD_DataWrite_ILI9338B(0x01);
	
	LCD_CtrlWrite_ILI9338B(0xC0); //Power control 1
	LCD_DataWrite_ILI9338B(0x26); //VRH[5:0] 0X26
	LCD_DataWrite_ILI9338B(0x01); //VC[2:0]
	
	LCD_CtrlWrite_ILI9338B(0xC1); //Power control 2
	LCD_DataWrite_ILI9338B(0x44); //SAP[2:0];BT[3:0] ½µµÍVgate
	
	LCD_CtrlWrite_ILI9338B(0xC2); //Power control 3
	LCD_DataWrite_ILI9338B(0x22);
	
	LCD_CtrlWrite_ILI9338B(0xC5); //VCM control
	#if SLIDE_MM040
	LCD_DataWrite_ILI9338B(0x35); //VCOMH 31 2d
	LCD_DataWrite_ILI9338B(0x40); //VCOML 3c 3f
	#else                         
	LCD_DataWrite_ILI9338B(0x2d); //VCOMH 31
	LCD_DataWrite_ILI9338B(0x38); //VCOML 3c
	#endif
	
	LCD_CtrlWrite_ILI9338B(0xC7); //VCM OFFSET control
	#if SLIDE_MM040
	LCD_DataWrite_ILI9338B(0xc1); //Improve flicker 0XBB   //0xbe (2 wld lcd) //--p2_wld--0xbb
	#else
	LCD_DataWrite_ILI9338B(0xc8); //Improve flicker 0XBB
	#endif
	
	LCD_CtrlWrite_ILI9338B(0x36); //Memmory access control
	LCD_DataWrite_ILI9338B(0x08);
	
	LCD_CtrlWrite_ILI9338B(0xB1); // Frame Rate Control
	LCD_DataWrite_ILI9338B(0x00);
	LCD_DataWrite_ILI9338B(0x14); //18
	
	
	LCD_CtrlWrite_ILI9338B(0xB6);
	LCD_DataWrite_ILI9338B(0x0a);
	LCD_DataWrite_ILI9338B(0x22);
	LCD_DataWrite_ILI9338B(0x27);
	
	LCD_CtrlWrite_ILI9338B(0x26); //Gamma curve selected
	LCD_DataWrite_ILI9338B(0x01);
	
	LCD_CtrlWrite_ILI9338B(0xE0); //Set Gamma
	LCD_DataWrite_ILI9338B(0x1f);
	LCD_DataWrite_ILI9338B(0x18);
	LCD_DataWrite_ILI9338B(0x16);
	LCD_DataWrite_ILI9338B(0x08);
	LCD_DataWrite_ILI9338B(0x10);
	LCD_DataWrite_ILI9338B(0x08);
	LCD_DataWrite_ILI9338B(0x45);
	LCD_DataWrite_ILI9338B(0x9b);
	LCD_DataWrite_ILI9338B(0x34); //--p2_wld--3a
	LCD_DataWrite_ILI9338B(0x2a); //--p2_wld--26
	LCD_DataWrite_ILI9338B(0x0f);
	LCD_DataWrite_ILI9338B(0x07);
	LCD_DataWrite_ILI9338B(0x07);
	LCD_DataWrite_ILI9338B(0x07);
	LCD_DataWrite_ILI9338B(0x00);
	
	LCD_CtrlWrite_ILI9338B(0XE1); //Set Gamma
	LCD_DataWrite_ILI9338B(0x00);
	LCD_DataWrite_ILI9338B(0x27);
	LCD_DataWrite_ILI9338B(0x29);
	LCD_DataWrite_ILI9338B(0x08);
	LCD_DataWrite_ILI9338B(0x0f);
	LCD_DataWrite_ILI9338B(0x07);
	LCD_DataWrite_ILI9338B(0x3a);
	LCD_DataWrite_ILI9338B(0x46);
	LCD_DataWrite_ILI9338B(0x4b);
	LCD_DataWrite_ILI9338B(0x15);
	LCD_DataWrite_ILI9338B(0x10);
	LCD_DataWrite_ILI9338B(0x08);
	LCD_DataWrite_ILI9338B(0x38);
	LCD_DataWrite_ILI9338B(0x38);
	LCD_DataWrite_ILI9338B(0x1F);
	
	LCD_CtrlWrite_ILI9338B(0x3A); //Pixel format set
	LCD_DataWrite_ILI9338B(0x05);
	
	LCD_CtrlWrite_ILI9338B(0x29); //display on
	
	LCD_Delayms(40);
	//LCD_CtrlWrite_ILI9338B(0x2c);
#else
	LCD_CtrlWrite_ILI9338B(0x11); //Exit Sleep
	LCD_Delayms(100);
	LCD_CtrlWrite_ILI9338B(0xCB); //AP[2:0]
	LCD_DataWrite_ILI9338B(0x01);
	LCD_CtrlWrite_ILI9338B(0xC0); //Power control
	LCD_DataWrite_ILI9338B(0x26);//VRH[5:0]
	LCD_DataWrite_ILI9338B(0x01);//VC[2:0]
	LCD_CtrlWrite_ILI9338B(0xC1); //Power control
	LCD_DataWrite_ILI9338B(0x44);//SAP[2:0];BT[3:0]
	LCD_CtrlWrite_ILI9338B(0xC5); //VCM control
	LCD_DataWrite_ILI9338B(0x2d);
	LCD_DataWrite_ILI9338B(0x38);
	LCD_CtrlWrite_ILI9338B(0x36);
	LCD_DataWrite_ILI9338B(0x08);
	LCD_CtrlWrite_ILI9338B(0x3a);
	LCD_DataWrite_ILI9338B(0x55);
	LCD_CtrlWrite_ILI9338B(0xB6); //VCM control
	LCD_DataWrite_ILI9338B(0x0A); //00
	LCD_DataWrite_ILI9338B(0x22);//20
	LCD_DataWrite_ILI9338B(0x27);
	LCD_CtrlWrite_ILI9338B(0x26); //Gamma curve selected
	LCD_DataWrite_ILI9338B(0x01);
	LCD_CtrlWrite_ILI9338B(0xE0); //Set Gamma
	LCD_DataWrite_ILI9338B(0x1F);
	LCD_DataWrite_ILI9338B(0x18);
	LCD_DataWrite_ILI9338B(0x16);
	LCD_DataWrite_ILI9338B(0x08);
	LCD_DataWrite_ILI9338B(0x10);
	LCD_DataWrite_ILI9338B(0x08);
	LCD_DataWrite_ILI9338B(0x45);
	LCD_DataWrite_ILI9338B(0x9B);
	LCD_DataWrite_ILI9338B(0x34);
	LCD_DataWrite_ILI9338B(0x2A);
	LCD_DataWrite_ILI9338B(0x0F);
	LCD_DataWrite_ILI9338B(0x07);
	LCD_DataWrite_ILI9338B(0x07);
	LCD_DataWrite_ILI9338B(0x07);
	LCD_DataWrite_ILI9338B(0x00);
	LCD_CtrlWrite_ILI9338B(0XE1); //Set Gamma
	LCD_DataWrite_ILI9338B(0x00);
	LCD_DataWrite_ILI9338B(0x27);
	LCD_DataWrite_ILI9338B(0x29);
	LCD_DataWrite_ILI9338B(0x08);
	LCD_DataWrite_ILI9338B(0x0F);
	LCD_DataWrite_ILI9338B(0x07);
	LCD_DataWrite_ILI9338B(0x3A);
	LCD_DataWrite_ILI9338B(0x46);
	LCD_DataWrite_ILI9338B(0x4B);
	LCD_DataWrite_ILI9338B(0x15);
	LCD_DataWrite_ILI9338B(0x10);
	LCD_DataWrite_ILI9338B(0x08);
	LCD_DataWrite_ILI9338B(0x38);
	LCD_DataWrite_ILI9338B(0x38);
	LCD_DataWrite_ILI9338B(0x1F);

	LCD_CtrlWrite_ILI9338B(0x2a);
	LCD_DataWrite_ILI9338B(0x00);
	LCD_DataWrite_ILI9338B(0x00);
	LCD_DataWrite_ILI9338B(0x00);
	LCD_DataWrite_ILI9338B(0xef);
	
	LCD_CtrlWrite_ILI9338B(0x2b);
	LCD_DataWrite_ILI9338B(0x00);
	LCD_DataWrite_ILI9338B(0x00);
	LCD_DataWrite_ILI9338B(0x01);
	LCD_DataWrite_ILI9338B(0x3f);
	
	LCD_CtrlWrite_ILI9338B(0x29); //display on
#endif

	//SCI_TRACE_LOW:"qinss LCD: in ILI9338B_reset end."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9338B_383_112_2_18_0_33_52_1810,(uint8*)"");
	
	return;
}


/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E ILI9338B_Init(void)
{
	//SCI_TRACE_LOW:"ILI9338B_Init"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9338B_395_112_2_18_0_33_52_1811,(uint8*)"");
 
	ILI9338B_driver();	
	
	return 0;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9338B_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"qinss LCD: in ILI9338B_EnterSleep, is_sleep = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9338B_410_112_2_18_0_33_52_1812,(uint8*)"d", is_sleep);
	
	if ( is_sleep ) // enter sleep mode.
	{
		LCD_CtrlWrite_ILI9338B(0x0028);
		LCD_Delayms(50);
		LCD_CtrlWrite_ILI9338B(0x0010);
	}
	else 			// out sleep mode 
	{
		ILI9338B_Init();
	}

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ILI9338B_Close(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in ILI9338B_Close"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9338B_432_112_2_18_0_33_52_1813,(uint8*)"");

	//GPIO_SetLcdBackLight( SCI_FALSE );
	ILI9338B_EnterSleep( SCI_TRUE );	
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9338B_Invalidate(void)
{

	ILI9338B_set_display_window (0x0, 0X0, QVGA_LCD_WIDTH - 1, QVGA_LCD_HEIGHT -1);
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:       
//     To improve speed, lcd is operate in HIGH SPEED RAM WRITE MODE(4
//     uint16 are write continuously always.) So, some dummy uint16 
//     should be inserted to satisfy this mode.   Please refer to spec.
/******************************************************************************/
PUBLIC ERR_LCD_E ILI9338B_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{
  	left 	= (left >= QVGA_LCD_WIDTH)    ? QVGA_LCD_WIDTH-1 : left;
	right 	= (right >= QVGA_LCD_WIDTH)   ? QVGA_LCD_WIDTH-1 : right;
	top 	= (top >= QVGA_LCD_HEIGHT)    ? QVGA_LCD_HEIGHT-1 : top;
	bottom 	= (bottom >= QVGA_LCD_HEIGHT) ? QVGA_LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
    	return ERR_LCD_OPERATE_FAIL;
    }     
   
    ILI9338B_set_display_window(left, top, right, bottom); 
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
LOCAL ERR_LCD_E ILI9338B_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	//int32 error;
	

	switch(angle)
	{
		case LCD_ANGLE_0:
			ILI9338B_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			ILI9338B_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			ILI9338B_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			ILI9338B_set_display_window(left, top, bottom,right);
			break;
		default:
			ILI9338B_set_display_window(left, top, right, bottom);
			break;			
	}
		

	return ERR_LCD_NONE;
}//en of S6D0139_VerticalInvalidateRect	

/******************************************************************************/
//  Description:  set the contrast value 
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   ILI9338B_SetContrast(
	uint16  contrast	//contrast value to set
	)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
} 


/*****************************************************************************/
//  Description:    Set the brightness of LCD.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/*****************************************************************************/
LOCAL ERR_LCD_E   ILI9338B_SetBrightness(
	uint16 brightness	//birghtness to set
	)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
}

/*****************************************************************************/
//  Description:    Enable lcd to partial display mode, so can save power.
//	Global resource dependence: 
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//	Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E ILI9338B_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	 	
	ILI9338B_set_display_window(left, top, right, bottom);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
const LCD_OPERATIONS_T ILI9338B_operations = 
{
	ILI9338B_Init,
	ILI9338B_EnterSleep,
	ILI9338B_SetContrast,
	ILI9338B_SetBrightness,
	ILI9338B_SetDisplayWindow,
	ILI9338B_InvalidateRect,
	ILI9338B_Invalidate,
	ILI9338B_Close,
	NULL,
	ILI9338B_SetDirection,
	NULL,
	NULL
};

/******************************************************************************/
//  Description:  return the ILI9338B lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9338B_GetOperations(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in ILI9338B_GetOperations"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9338B_583_112_2_18_0_33_52_1814,(uint8*)"");

	return (LCD_OPERATIONS_T*)&ILI9338B_operations;
}

PUBLIC BOOLEAN ILI9338B_Probe(void)
{
	return SCI_TRUE;	
}

LOCAL const LCD_TIMING_U s_ILI9338B_timing =
{

	        // LCM_CYCLE_U start(ns)
	10,		// CS setup time for LCM read (optional)  
	150,	// low pulse width for LCM read (according spec)	
	100,	// high pulse width for LCM read (according spec)
#if 0 // by riyang.xie
	30,		// CS setup time for LCM write  (optional) 	
	50,		// low pulse width for LCM write (according spec)
	70,	    // high pulse width for LCM write (according spec)	
#else
	10,		// CS setup time for LCM write  (optional) 	
	35,		// low pulse width for LCM write (according spec)
	35,	    // high pulse width for LCM write (according spec)	
#endif
	        // LCM_CYCLE_U end(ns)
};


PUBLIC const LCD_SPEC_T g_lcd_ILI9338B[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_8,
	(LCD_TIMING_U*)&s_ILI9338B_timing, 
	(LCD_OPERATIONS_T*)&ILI9338B_operations,
	0,
	0
	}
};

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 


