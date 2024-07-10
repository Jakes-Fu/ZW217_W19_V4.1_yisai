/******************************************************************************
 ** File Name:     tft_SPFD5420A.c                                              *
 ** Description:															  *
 **    This file contains driver for color LCD.(TFT)						  *
 ** Author:         Liangwen Zhen                                             *
 ** DATE:           27/02/2009                                                *
 ** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 02/04/2004     louis.wei	    Create.
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



#define LCD_CtrlWrite_SPFD5420A( _cmd )\
				LCD_SendCmd( (_cmd ), 0 );

#define LCD_DataWrite_SPFD5420A( _data )\
				LCD_SendData( (_data), 0 );

#define LCD_CtrlData_SPFD5420A( _cmd, _data ) \
				LCD_SendCmdData( (_cmd ),  (_data),0 );  
			

LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;	
						
  /******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void SPFD5420A_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  SPFD5420A_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);


/*********************************************************************/
//  Description:   Initialize color LCD : SPFD5420A
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E SPFD5420A_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E SPFD5420A_Invalidate(void);


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
LOCAL ERR_LCD_E SPFD5420A_InvalidateRect(
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
PUBLIC void SPFD5420A_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 right, 	// end Horizon address
	uint16 top, 		// start Vertical address
	uint16 bottom	// end Vertical address
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
LOCAL ERR_LCD_E SPFD5420A_SetDirection( LCD_DIRECT_E direct_type)
{
//static uint16 test_value = 0x1030;
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:			
		    LCD_CtrlData_SPFD5420A(0x0003, 0x10A0);
			break; 
			
		case LCD_DIRECT_ROT_90:
			LCD_CtrlData_SPFD5420A(0x0003, 0x1038);
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlData_SPFD5420A(0x0003, 0x1010);
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlData_SPFD5420A(0x0003, 0x1018);
			break;

		case LCD_DIRECT_MIR_H:
			LCD_CtrlData_SPFD5420A(0x0003, 0x10b0);
			break;
			
		case LCD_DIRECT_MIR_V:
			LCD_CtrlData_SPFD5420A(0x0003, 0x1000);
			break;
			
		case LCD_DIRECT_MIR_HV:
			LCD_CtrlData_SPFD5420A(0x0003, 0x1010);
			break;
			
		default:			
			LCD_CtrlData_SPFD5420A(0x0003, 0x10A0);

		    direct_type = LCD_DIRECT_NORMAL;
			break;
			
	}	
	s_lcd_direct = direct_type;	

	return ERR_LCD_NONE;
	
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
__inline void SPFD5420A_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 top, 	// end Horizon address
	uint16 right, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{   
	//uint16 cmd;	
	uint16 temp = 0;

	temp = left;
	left  = LCD_WIDTH - right - 1;
	right = LCD_WIDTH - temp - 1;
	temp  = 0;
	

	LCD_CtrlData_SPFD5420A(0x0210, left);
	LCD_CtrlData_SPFD5420A(0x0211, right);
	LCD_CtrlData_SPFD5420A(0x0212, top);
	LCD_CtrlData_SPFD5420A(0x0213, bottom);
  
	switch(s_lcd_direct)
	{
		case LCD_DIRECT_NORMAL:	
			LCD_CtrlData_SPFD5420A(0x0200, left);
			LCD_CtrlData_SPFD5420A(0x0201, top);	
			break;            
			
		case LCD_DIRECT_ROT_90:				
			LCD_CtrlData_SPFD5420A(0x0200, right);
			LCD_CtrlData_SPFD5420A(0x0201, top);	
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlData_SPFD5420A(0x0200, right);
			LCD_CtrlData_SPFD5420A(0x0201, bottom);				
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlData_SPFD5420A(0x0200, left);
			LCD_CtrlData_SPFD5420A(0x0201, top);			
			break;

		case LCD_DIRECT_MIR_H:
			LCD_CtrlData_SPFD5420A(0x0200, right);
			LCD_CtrlData_SPFD5420A(0x0201, top);
			break;

		case LCD_DIRECT_MIR_V:
			LCD_CtrlData_SPFD5420A(0x0200, left);
			LCD_CtrlData_SPFD5420A(0x0201, bottom);
			break;

		case LCD_DIRECT_MIR_HV:
			LCD_CtrlData_SPFD5420A(0x0200, right);
			LCD_CtrlData_SPFD5420A(0x0201, bottom);
			break;
			
		default:			
		    LCD_CtrlData_SPFD5420A(0x0200, left);
			LCD_CtrlData_SPFD5420A(0x0201, top);
			break;	
	
	}
	
	LCD_CtrlWrite_SPFD5420A(0x0202);	

}

/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E SPFD5420A_Init(void)
{

	//SCI_TRACE_LOW:"LCD: in SPFD5420A_reset"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_SPFD5420A_242_112_2_18_0_34_11_1866,(uint8*)"");

	LCD_Reset();//reset lcd throught lcdc	
	LCD_Delayms(50);	
	
//--------------- Normal set ---------------//
	LCD_CtrlData_SPFD5420A(0x0000, 0x0000); 
	LCD_CtrlData_SPFD5420A(0x0001, 0x0000); 
	LCD_CtrlData_SPFD5420A(0x0002, 0x0100); 
	LCD_CtrlData_SPFD5420A(0x0003, 0x10A0); 
	LCD_CtrlData_SPFD5420A(0x0008, 0x0808); 
	LCD_CtrlData_SPFD5420A(0x0009, 0x0001); 
	LCD_CtrlData_SPFD5420A(0x000B, 0x0010); 
	LCD_CtrlData_SPFD5420A(0x000C, 0x0000); 
	LCD_CtrlData_SPFD5420A(0x000F, 0x0000); 
	LCD_CtrlData_SPFD5420A(0x0007, 0x0001); 
//--------------- Panel interface control 1~6 ---------------//
	LCD_CtrlData_SPFD5420A(0x0010, 0x0011);	//0x0012
	LCD_CtrlData_SPFD5420A(0x0011, 0x0200);	//0x0202
	LCD_CtrlData_SPFD5420A(0x0012, 0x0300);	//
	LCD_CtrlData_SPFD5420A(0x0020, 0x021E);	//
	LCD_CtrlData_SPFD5420A(0x0021, 0x0202);	//
	LCD_CtrlData_SPFD5420A(0x0022, 0x0100);	//
	LCD_CtrlData_SPFD5420A(0x0090, 0x8000);	//
//--------------- Powe on sequence ---------------//
	LCD_CtrlData_SPFD5420A(0x0606, 0x0000);
	LCD_Delayms(50);
	LCD_CtrlData_SPFD5420A(0x0007, 0x0001);
	LCD_Delayms(50);
	LCD_CtrlData_SPFD5420A(0x0110, 0x0001);
	LCD_Delayms(50);
	LCD_CtrlData_SPFD5420A(0x0100, 0x17B0);
	LCD_CtrlData_SPFD5420A(0x0101, 0x0147);
	LCD_CtrlData_SPFD5420A(0x0102, 0x0138);	//0x019D
	LCD_CtrlData_SPFD5420A(0x0103, 0x0E00);	//0x3600
	LCD_CtrlData_SPFD5420A(0x0281, 0x0010);
	LCD_Delayms(50);
//--------------- Power control 1~6 ---------------//
	LCD_CtrlData_SPFD5420A(0x0100, 0x14B0);
	LCD_CtrlData_SPFD5420A(0x0101, 0x0147);
	LCD_CtrlData_SPFD5420A(0x0102, 0x01BD);
	LCD_CtrlData_SPFD5420A(0x0103, 0x3000);
	LCD_CtrlData_SPFD5420A(0x0107, 0x0000);
	LCD_CtrlData_SPFD5420A(0x0110, 0x0001);
	LCD_CtrlData_SPFD5420A(0x0280, 0x0000); // NVM write / read
	LCD_CtrlData_SPFD5420A(0x0281, 0x0000); // Vcom high voltage 1
	LCD_CtrlData_SPFD5420A(0x0282, 0x0000); // Vcom high voltage 2
//--------------- Gamma 2.2 control ---------------//
	LCD_CtrlData_SPFD5420A(0x0300, 0x1011);	//0x0101	
	LCD_CtrlData_SPFD5420A(0x0301, 0x2524);	//0x0024
	LCD_CtrlData_SPFD5420A(0x0302, 0x2f1c);	//0x1326
	LCD_CtrlData_SPFD5420A(0x0303, 0x1c2f);	//0x2313
	LCD_CtrlData_SPFD5420A(0x0304, 0x2325);	//0x2400
	LCD_CtrlData_SPFD5420A(0x0305, 0x1110);	//0x0100
	LCD_CtrlData_SPFD5420A(0x0306, 0x0e04);	//0x1704
	LCD_CtrlData_SPFD5420A(0x0307, 0x040e);	//0x0417
	LCD_CtrlData_SPFD5420A(0x0308, 0x0005);	//0x0007
	LCD_CtrlData_SPFD5420A(0x0309, 0x0003);	//0x0105
	LCD_CtrlData_SPFD5420A(0x030A, 0x0F04);	//0x0F05
	LCD_CtrlData_SPFD5420A(0x030B, 0x0F00);	//0x0F01
	LCD_CtrlData_SPFD5420A(0x030C, 0x000F);	//0x010F
	LCD_CtrlData_SPFD5420A(0x030D, 0x040F);	//0x050F
	LCD_CtrlData_SPFD5420A(0x030E, 0x0300);	//0x0501
	LCD_CtrlData_SPFD5420A(0x030F, 0x0500);	//0x0700
	LCD_CtrlData_SPFD5420A(0x0400, 0x3500);	//0x3100
	LCD_CtrlData_SPFD5420A(0x0401, 0x0001);	//0x0001
	LCD_CtrlData_SPFD5420A(0x0404, 0x0000);	//0x0000
//--------------- Partial display ---------------//
	LCD_CtrlData_SPFD5420A(0x0210, 0x0000); 
	LCD_CtrlData_SPFD5420A(0x0211, 0x00EF); 
	LCD_CtrlData_SPFD5420A(0x0212, 0x0000); 
	LCD_CtrlData_SPFD5420A(0x0213, 0x018F); 
	LCD_CtrlData_SPFD5420A(0x0500, 0x0000);
	LCD_CtrlData_SPFD5420A(0x0501, 0x0000);
	LCD_CtrlData_SPFD5420A(0x0502, 0x0000);
	LCD_CtrlData_SPFD5420A(0x0503, 0x0000);
	LCD_CtrlData_SPFD5420A(0x0504, 0x0000);
	LCD_CtrlData_SPFD5420A(0x0505, 0x0000);
	LCD_CtrlData_SPFD5420A(0x0600, 0x0000); 
	LCD_CtrlData_SPFD5420A(0x0606, 0x0000); 
	LCD_CtrlData_SPFD5420A(0x06F0, 0x0000); 
//--------------- Orise mode ---------------//
	LCD_CtrlData_SPFD5420A(0x07F0, 0x5420A);
	LCD_CtrlData_SPFD5420A(0x07de, 0x0000);
	LCD_CtrlData_SPFD5420A(0x07F2, 0x00df);
	LCD_CtrlData_SPFD5420A(0x07F3, 0x088e);	//0x288A
	LCD_CtrlData_SPFD5420A(0x07F4, 0x0022);
	LCD_CtrlData_SPFD5420A(0x07F5, 0x0001);	//
	LCD_CtrlData_SPFD5420A(0x07F0, 0x0000);
	LCD_CtrlData_SPFD5420A(0x0007, 0x0173); // Display on	

	//SCI_TRACE_LOW:"LCD: in SPFD5420A_reset end."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_SPFD5420A_333_112_2_18_0_34_12_1867,(uint8*)"");
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  SPFD5420A_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"LCD: in SPFD5420A_EnterSleep, is_sleep = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_SPFD5420A_345_112_2_18_0_34_12_1868,(uint8*)"d", is_sleep);
	if ( is_sleep ) // enter sleep mode.
	{

	  	LCD_CtrlData_SPFD5420A (0x0007, 0x0170); //Display off
		LCD_Delayms(50);
		LCD_CtrlData_SPFD5420A (0x0007, 0x0000); //Display off
		LCD_Delayms(50);
		LCD_CtrlData_SPFD5420A (0x0100, 0x002); //Set to sleep mode
		
	}
	else 			// out sleep mode 
	{
	 	SPFD5420A_Init();
	}

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void SPFD5420A_Close(void)
{
	//SCI_TRACE_LOW:"LCD: in SPFD5420A_Close"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_SPFD5420A_370_112_2_18_0_34_12_1869,(uint8*)"");
	
	SPFD5420A_EnterSleep( SCI_TRUE );	
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E SPFD5420A_Invalidate(void)
{
	SPFD5420A_set_display_window (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E SPFD5420A_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{
  	left 	= (left  >= LCD_WIDTH)    ? LCD_WIDTH-1 : left;
	right 	= (right >= LCD_WIDTH)   ? LCD_WIDTH-1 : right;
	top 	= (top   >= LCD_HEIGHT)    ? LCD_HEIGHT-1 : top;
	bottom 	= (bottom >= LCD_HEIGHT) ? LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
    	return ERR_LCD_OPERATE_FAIL;
    }     
   
    SPFD5420A_set_display_window(left, top, right, bottom); 
  
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
LOCAL ERR_LCD_E SPFD5420A_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	//int32 error;
	

	switch(angle)
	{
		case LCD_ANGLE_0:
			SPFD5420A_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			SPFD5420A_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			SPFD5420A_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			SPFD5420A_set_display_window(left, top, bottom,right);
			break;
		default:
			SPFD5420A_set_display_window(left, top, right, bottom);
			break;			
	}
		

	return ERR_LCD_NONE;
}//en of SPFD5420A_VerticalInvalidateRect	


/******************************************************************************/
//  Description:  set the contrast value 
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   SPFD5420A_SetContrast(
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
LOCAL ERR_LCD_E   SPFD5420A_SetBrightness(
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
LOCAL ERR_LCD_E SPFD5420A_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	 	
	SPFD5420A_set_display_window(left, top, right, bottom);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL const LCD_OPERATIONS_T SPFD5420A_operations = 
{
	SPFD5420A_Init,
	SPFD5420A_EnterSleep,
	SPFD5420A_SetContrast,
	SPFD5420A_SetBrightness,
	SPFD5420A_SetDisplayWindow,
	SPFD5420A_InvalidateRect,
	SPFD5420A_Invalidate,
	SPFD5420A_Close,
	SPFD5420A_RotationInvalidateRect,
	SPFD5420A_SetDirection,
    NULL,
    NULL
};

/******************************************************************************/
//  Description:  return the SPFD5420A lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* SPFD5420A_GetOperations(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in SPFD5420A_GetOperations"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_SPFD5420A_521_112_2_18_0_34_12_1870,(uint8*)"");

	return (LCD_OPERATIONS_T*)&SPFD5420A_operations;
}

PUBLIC BOOLEAN SPFD5420A_Probe(void)
{
	return SCI_TRUE;	
}

LOCAL const LCD_TIMING_U s_SPFD5420A_timing =
{
     	        // LCM_CYCLE_U start(ns)
	20,		// CS setup time for LCM read (optional)  
	150,	// low pulse width for LCM read (according spec)	
	100,	// high pulse width for LCM read (according spec)
	10,		// CS setup time for LCM write  (optional) 	
	20,		// low pulse width for LCM write (according spec)
	100,	    // high pulse width for LCM write (according spec)	
	        // LCM_CYCLE_U end(ns)
};


const LCD_SPEC_T g_lcd_SPFD5420A[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U*)&s_SPFD5420A_timing, 
	(LCD_OPERATIONS_T*)&SPFD5420A_operations,
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


