/******************************************************************************
 ** File Name:     tft_ILI9326.c                                              *
 ** Description:															  *
 **    This file contains driver for color LCD.     						  *
 ** Author:         Jim zhang                                                 *
 ** DATE:           03/01/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
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

#define LCD_CtrlWrite_ILI9326( _cmd )\
				LCD_SendCmd( (_cmd ), 0 );

#define LCD_DataWrite_ILI9326( _data )\
				LCD_SendData( (_data), 0 );

#define LCD_CtrlData_ILI9326( _cmd, _data ) \
				LCD_SendCmdData( (_cmd ),  (_data),0 );  
				

LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;	

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ILI9326_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9326_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);

/*********************************************************************/
//  Description:   Initialize color LCD : ILI9326
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E ILI9326_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9326_Invalidate(void);


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
LOCAL ERR_LCD_E ILI9326_InvalidateRect(
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
PUBLIC void ILI9326_set_display_window(
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
LOCAL ERR_LCD_E ILI9326_SetDirection( LCD_DIRECT_E direct_type)
{
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:			
		    LCD_CtrlData_ILI9326(0x0003, 0x1030);		
			break;
			
		case LCD_DIRECT_ROT_90:
			LCD_CtrlData_ILI9326(0x0003, 0x1028);
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlData_ILI9326(0x0003, 0x1000);
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlData_ILI9326(0x0003, 0x1018);
			break;

		case LCD_DIRECT_MIR_H:
			LCD_CtrlData_ILI9326(0x0003, 0x1020);
			break;
			
		case LCD_DIRECT_MIR_V:
			LCD_CtrlData_ILI9326(0x0003, 0x1010);
			break;
			
		case LCD_DIRECT_MIR_HV:
			LCD_CtrlData_ILI9326(0x0003, 0x1000);
			break;
			
		default:			
			LCD_CtrlData_ILI9326(0x0003, 0x1030);

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
__inline void ILI9326_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 top, 	// end Horizon address
	uint16 right, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{   
	uint16 cmd;	
	
    cmd = 0x210;
	LCD_CtrlWrite_ILI9326(cmd);
	
	//cmd = ((right << 8) & 0xff00)|(left&0xff);
	cmd = left;
	LCD_DataWrite_ILI9326(cmd);
	
	cmd = 0x211;
	LCD_CtrlWrite_ILI9326(cmd);
	
	cmd = right;
	LCD_DataWrite_ILI9326(cmd);
	
	cmd = 0x212;
	LCD_CtrlWrite_ILI9326(cmd);
	
	cmd = top;
	LCD_DataWrite_ILI9326(cmd);

	cmd = 0x213;
	LCD_CtrlWrite_ILI9326(cmd);
	
	cmd = bottom;
	LCD_DataWrite_ILI9326(cmd);
	
	switch(s_lcd_direct)
	{
		case LCD_DIRECT_NORMAL:	
		    LCD_CtrlData_ILI9326(0x0200, left);
			LCD_CtrlData_ILI9326(0x0201, top);
			break;            
			
		case LCD_DIRECT_ROT_90:				
			LCD_CtrlData_ILI9326(0x0200, right);
			LCD_CtrlData_ILI9326(0x0201, top);	
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlData_ILI9326(0x0200, right);
			LCD_CtrlData_ILI9326(0x0201, bottom);				
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlData_ILI9326(0x0200, left);
			LCD_CtrlData_ILI9326(0x0201, top);			
			break;

		case LCD_DIRECT_MIR_H:
			LCD_CtrlData_ILI9326(0x0200, right);
			LCD_CtrlData_ILI9326(0x0201, top);
			break;

		case LCD_DIRECT_MIR_V:
			LCD_CtrlData_ILI9326(0x0200, left);
			LCD_CtrlData_ILI9326(0x0201, bottom);
			break;

		case LCD_DIRECT_MIR_HV:
			LCD_CtrlData_ILI9326(0x0200, right);
			LCD_CtrlData_ILI9326(0x0201, bottom);
			break;
			
		default:			
		    LCD_CtrlData_ILI9326(0x0200, left);
			LCD_CtrlData_ILI9326(0x0201, top);
			break;
	
	}
    LCD_CtrlWrite_ILI9326(0x202);
}

/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E ILI9326_Init(void)
{
    LCD_Reset();//reset lcd throught lcdc	
    LCD_Delayms(100);

	//------------- Start Initial Sequence ----------//
	LCD_CtrlData_ILI9326(0x0702, 0x3008);// Set internal timing, don¡¯t change this value
	LCD_CtrlData_ILI9326(0x0705, 0x0036);// Set internal timing, don¡¯t change this value
	LCD_CtrlData_ILI9326(0x070B, 0x1213);// Set internal timing, don¡¯t change this value
	LCD_CtrlData_ILI9326(0x0001, 0x0100);// set SS and SM bit
	LCD_CtrlData_ILI9326(0x0002, 0x0100);// set 1 line inversion
	LCD_CtrlData_ILI9326(0x0003, 0x1030);// set GRAM write direction and BGR=1.
	LCD_CtrlData_ILI9326(0x0008, 0x0202);// set the back porch and front porch
	LCD_CtrlData_ILI9326(0x0009, 0x0000);// set non-display area refresh cycle ISC[3:0]
	LCD_CtrlData_ILI9326(0x000C, 0x0000);// RGB interface setting
	LCD_CtrlData_ILI9326(0x000F, 0x0000);// RGB interface polarity

	
	//-------------Power On sequence ----------------//
	LCD_CtrlData_ILI9326(0x0100, 0x0000);// SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_CtrlData_ILI9326(0x0102, 0x0000);// VREG1OUT voltage
	LCD_CtrlData_ILI9326(0x0103, 0x0000); // VDV[4:0] for VCOM amplitude
	LCD_Delayms(200); // Dis-charge capacitor power voltage
	LCD_CtrlData_ILI9326(0x0100, 0x1190); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_CtrlData_ILI9326(0x0101, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
	LCD_Delayms(50); // Delay 50ms
	LCD_CtrlData_ILI9326(0x0102, 0x01BD); // VREG1OUT voltage
	LCD_Delayms(50); // Delay 50ms
	LCD_CtrlData_ILI9326(0x0103, 0x2D00); // VDV[4:0] for VCOM amplitude  0x2D00
	LCD_CtrlData_ILI9326(0x0281, 0x000a); // VCM[5:0] for VCOMH             0x000E
	LCD_Delayms(50);
	LCD_CtrlData_ILI9326(0x0200, 0x0000); // GRAM horizontal Address
	LCD_CtrlData_ILI9326(0x0201, 0x0000); // GRAM Vertical Address
	
	// ----------- Adjust the Gamma Curve ----------//
	LCD_CtrlData_ILI9326(0x0300, 0x0000);
	LCD_CtrlData_ILI9326(0x0301, 0x0707);
	LCD_CtrlData_ILI9326(0x0302, 0x0606);
	LCD_CtrlData_ILI9326(0x0305, 0x0000);
	LCD_CtrlData_ILI9326(0x0306, 0x0D00);
	LCD_CtrlData_ILI9326(0x0307, 0x0706);
	LCD_CtrlData_ILI9326(0x0308, 0x0005);
	LCD_CtrlData_ILI9326(0x0309, 0x0007);
	LCD_CtrlData_ILI9326(0x030C, 0x0000);
	LCD_CtrlData_ILI9326(0x030D, 0x000A);
	
	//------------------ Set GRAM area ---------------//
	LCD_CtrlData_ILI9326(0x0210, 0x0000);// Horizontal GRAM Start Address
	LCD_CtrlData_ILI9326(0x0211, 0x00EF);// Horizontal GRAM End Address
	LCD_CtrlData_ILI9326(0x0212, 0x0000);// Vertical GRAM Start Address
	LCD_CtrlData_ILI9326(0x0213, 0x01AF);// Vertical GRAM Start Address
	LCD_CtrlData_ILI9326(0x0400, 0x3100);// Gate Scan Line 400 lines
	LCD_CtrlData_ILI9326(0x0401, 0x0001);// NDL,VLE, REV
	LCD_CtrlData_ILI9326(0x0404, 0x0000);// set scrolling line
	
	//-------------- Partial Display Control ---------//
	LCD_CtrlData_ILI9326(0x0500, 0x0000);//Partial Image 1 Display Position
	LCD_CtrlData_ILI9326(0x0501, 0x0000);//Partial Image 1 RAM Start/End Address
	LCD_CtrlData_ILI9326(0x0502, 0x0000);//Partial Image 1 RAM Start/End Address
	LCD_CtrlData_ILI9326(0x0503, 0x0000);//Partial Image 2 Display Position
	LCD_CtrlData_ILI9326(0x0504, 0x0000);//Partial Image 2 RAM Start/End Address
	LCD_CtrlData_ILI9326(0x0505, 0x0000);//Partial Image 2 RAM Start/End Address
	
	//-------------- Panel Control -------------------//
	LCD_CtrlData_ILI9326(0x0010, 0x0010);//DIVI[1:0];RTNI[4:0]
	LCD_CtrlData_ILI9326(0x0011, 0x0600);//NOWI[2:0];SDTI[2:0]
	LCD_CtrlData_ILI9326(0x0020, 0x0002);//DIVE[1:0];RTNE[5:0]
	LCD_CtrlData_ILI9326(0x0007, 0x0173);// 262K color and display ON

    LCD_CtrlWrite_ILI9326(0x202);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9326_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"qinss LCD: in ILI9326_EnterSleep, is_sleep = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9326_325_112_2_18_0_33_46_1789,(uint8*)"d", is_sleep);
	if ( is_sleep ) // enter sleep mode.
	{
		LCD_CtrlData_ILI9326(0x0007, 0x0131); // Set D1=0, D0=1
		LCD_Delayms(10);
		LCD_CtrlData_ILI9326(0x0007, 0x0130); // Set D1=0, D0=0
		LCD_Delayms(10);
		LCD_CtrlData_ILI9326(0x0007, 0x0000); // display OFF
		
		//------------- Power OFF sequence --------------//
		LCD_CtrlData_ILI9326(0x0100, 0x0000); // SAP, BT[3:0], APE, AP, DSTB, SLP
		LCD_Delayms(200); // Dis-charge capacitor power voltage
		LCD_CtrlData_ILI9326(0x0100, 0x0002); // SAP, BT[3:0], APE, AP, DSTB, SLP
	}
	else 	// out sleep mode 
	{
	      // ILI9326_Init();

		  //-------------Power On sequence ------------------//
		LCD_CtrlData_ILI9326(0x0100, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP
		LCD_Delayms(200); // Dis-charge capacitor power voltage
		LCD_CtrlData_ILI9326(0x0100, 0x1190); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_CtrlData_ILI9326(0x0101, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
		LCD_Delayms(50); // Delay 50ms
		LCD_CtrlData_ILI9326(0x0102, 0x01BD); // VREG1OUT voltage
		LCD_Delayms(50); // Delay 50ms
		LCD_CtrlData_ILI9326(0x0103, 0x2D00); // VDV[4:0] for VCOM amplitude
		LCD_Delayms(50); // Delay 50ms
		LCD_CtrlData_ILI9326(0x0007, 0x0173); // 262K color and display ON
		
	}

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ILI9326_Close(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in ILI9326_Close"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9326_366_112_2_18_0_33_46_1790,(uint8*)"");

	//GPIO_SetLcdBackLight( SCI_FALSE );
	ILI9326_EnterSleep( SCI_TRUE );	
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9326_Invalidate(void)
{

	//ILI9326_set_display_window (0x0, ILI9326_WIDTH - 1, 0x0, ILI9326_HEIGHT -1);
	ILI9326_set_display_window (0x0, 0X0, WQVGA_LCD_WIDTH - 1, WQVGA_LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E ILI9326_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{
  	left 	= (left >= WQVGA_LCD_WIDTH)    ? WQVGA_LCD_WIDTH-1 : left;
	right 	= (right >= WQVGA_LCD_WIDTH)   ? WQVGA_LCD_WIDTH-1 : right;
	top 	= (top >= WQVGA_LCD_HEIGHT)    ? WQVGA_LCD_HEIGHT-1 : top;
	bottom 	= (bottom >= WQVGA_LCD_HEIGHT) ? WQVGA_LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
    	return ERR_LCD_OPERATE_FAIL;
    }     
   
    ILI9326_set_display_window(left, top, right, bottom); 
    //ILI9326_set_display_window(left, right, top, bottom); 
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
LOCAL ERR_LCD_E ILI9326_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	switch(angle)
	{
		case LCD_ANGLE_0:
			ILI9326_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			ILI9326_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			ILI9326_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			ILI9326_set_display_window(left, top, bottom,right);
			break;
		default:
			ILI9326_set_display_window(left, top, right, bottom);
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
LOCAL ERR_LCD_E   ILI9326_SetContrast(
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
LOCAL ERR_LCD_E   ILI9326_SetBrightness(
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
LOCAL ERR_LCD_E ILI9326_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	 	
	ILI9326_set_display_window(left, top, right, bottom);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL const LCD_OPERATIONS_T ILI9326_operations = 
{
	ILI9326_Init,
	ILI9326_EnterSleep,
	ILI9326_SetContrast,
	ILI9326_SetBrightness,
	ILI9326_SetDisplayWindow,
	ILI9326_InvalidateRect,
	ILI9326_Invalidate,
	ILI9326_Close,
	ILI9326_RotationInvalidateRect,
	ILI9326_SetDirection,
    NULL,
    NULL
};

/******************************************************************************/
//  Description:  return the ILI9326 lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9326_GetOperations(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in ILI9326_GetOperations"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9326_517_112_2_18_0_33_47_1791,(uint8*)"");

	return (LCD_OPERATIONS_T*)&ILI9326_operations;
}

PUBLIC BOOLEAN ILI9326_Probe(void)
{
	return SCI_TRUE;	
}

LOCAL const LCD_TIMING_U s_ILI9326_timing =
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


const LCD_SPEC_T g_lcd_ILI9326[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U*)&s_ILI9326_timing, 
	(LCD_OPERATIONS_T*)&ILI9326_operations,
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


