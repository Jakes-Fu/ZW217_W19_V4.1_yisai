/******************************************************************************
 ** File Name:     tft_HX8340B.c                                             *
 ** Description:															  *
 **    This file contains driver for color LCD.(TFT)						  *
 ** Author:         Riyang.Xie                                                 *
 ** DATE:           03/21/2011                                                *
 ** Copyright:      All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/21/2011     Riyang.Xie	    Create.
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



#define LCD_CtrlWrite_HX8340B( _cmd )\
				LCD_SendCmd( (_cmd ), 0 );

#define LCD_DataWrite_HX8340B( _data )\
				LCD_SendData( (_data), 0 );

#define LCD_CtrlData_HX8340B( _cmd, _data ) \
				LCD_SendCmdData( (_cmd ),  (_data),0 );  
			

LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;	

						
LOCAL void HX8340B_Close(void);

LOCAL ERR_LCD_E  HX8340B_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);
	
LOCAL ERR_LCD_E HX8340B_Init(void);

LOCAL ERR_LCD_E HX8340B_Invalidate(void);

LOCAL ERR_LCD_E HX8340B_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	);

PUBLIC void HX8340B_set_display_window(
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
LOCAL ERR_LCD_E HX8340B_SetDirection( LCD_DIRECT_E direct_type)
{
	#if 0
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:			
		    LCD_CtrlData_HX8340B(0x0016, 0x1030);		
			break;
			
		case LCD_DIRECT_ROT_90:
			LCD_CtrlData_HX8340B(0x0003, 0x1028);
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlData_HX8340B(0x0003, 0x1000);
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlData_HX8340B(0x0003, 0x1018);
			break;

		case LCD_DIRECT_MIR_H:
			LCD_CtrlData_HX8340B(0x0003, 0x1020);
			break;
			
		case LCD_DIRECT_MIR_V:
			LCD_CtrlData_HX8340B(0x0003, 0x1010);
			break;
			
		case LCD_DIRECT_MIR_HV:
			LCD_CtrlData_HX8340B(0x0003, 0x1000);
			break;
			
		default:			
			LCD_CtrlData_HX8340B(0x0003, 0x1030);

		    direct_type = LCD_DIRECT_NORMAL;
			break;
			
	}	
	s_lcd_direct = direct_type;	
	#endif
	
	return ERR_LCD_NONE;
	
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 

//	Note:
/******************************************************************************/
__inline void HX8340B_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 top, 	// end Horizon address
	uint16 right, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{
	LCD_CtrlData_HX8340B(0x02,((left&0xff00)>>8));
	LCD_CtrlData_HX8340B(0x03,(left&0x00ff)); //Column Start
	LCD_CtrlData_HX8340B(0x04,((right&0xff00)>>8));
	LCD_CtrlData_HX8340B(0x05,(right&0x00ff)); //Column End
	LCD_CtrlData_HX8340B(0x06,((top&0xff00)>>8));
	LCD_CtrlData_HX8340B(0x07,(top&0x00ff)); //Row Start
	LCD_CtrlData_HX8340B(0x08,((bottom&0xff00)>>8));
	LCD_CtrlData_HX8340B(0x09,(bottom&0x00ff)); //Row End
	
	LCD_CtrlWrite_HX8340B(0x22); //Start GRAM write
}


/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E HX8340B_Init(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in HX8340B_reset"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8340B_159_112_2_18_0_33_28_1744,(uint8*)"");

	LCD_Reset();//reset lcd throught lcdc	
	LCD_Delayms(50);

#if 0 // by riyang.xie
	//************* Detect Module IC's ID **********//
	LCD_CtrlWrite_HX8340B(0x93);
	lcm_dev_id = LCDC_Read_Ex(0, 1);	//0x00
	//SCI_TRACE_LOW:"\"\"\"#LCD: in HX8340B_Init ID(0x93)=,0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8340B_168_112_2_18_0_33_28_1745,(uint8*)"d",lcm_dev_id);
#endif
	//************* Start Initial Sequence **********//
	//Driving ability Setting
	LCD_CtrlData_HX8340B(0x0060,0x0080);
	LCD_CtrlData_HX8340B(0x0061,0x0006);
	LCD_CtrlData_HX8340B(0x0062,0x0000);
	LCD_CtrlData_HX8340B(0x0063,0x00D9);
	LCD_CtrlData_HX8340B(0x0073,0x0070);
	
	LCD_Delayms(20);
	
	//Gamma 2.2 Setting
	LCD_CtrlData_HX8340B(0x0040,0x0020);
	LCD_CtrlData_HX8340B(0x0041,0x0041);
	LCD_CtrlData_HX8340B(0x0042,0x0041);
	LCD_CtrlData_HX8340B(0x0043,0x0001);
	LCD_CtrlData_HX8340B(0x0044,0x0034);
	LCD_CtrlData_HX8340B(0x0045,0x0002);
	LCD_CtrlData_HX8340B(0x0046,0x0008);
	LCD_CtrlData_HX8340B(0x0047,0x00D1);
	LCD_CtrlData_HX8340B(0x0048,0x0005);
	
	LCD_CtrlData_HX8340B(0x0050,0x0067);
	LCD_CtrlData_HX8340B(0x0051,0x0000);
	LCD_CtrlData_HX8340B(0x0052,0x0027);
	LCD_CtrlData_HX8340B(0x0053,0x000B);
	LCD_CtrlData_HX8340B(0x0054,0x0083);
	LCD_CtrlData_HX8340B(0x0055,0x000E);
	LCD_CtrlData_HX8340B(0x0056,0x0001);
	LCD_CtrlData_HX8340B(0x0057,0x0037);
	
	//Power Voltage Setting
	LCD_CtrlData_HX8340B(0x001F,0x0003);
	LCD_CtrlData_HX8340B(0x0020,0x0002);
	LCD_CtrlData_HX8340B(0x0024,0x001c);
	LCD_CtrlData_HX8340B(0x0025,0x0034);
	
	//VCOM offset
	LCD_CtrlData_HX8340B(0x0023,0x0051);
	
	//Power on Setting
	LCD_CtrlData_HX8340B(0x0018,0x0022);//OSC * 1.17
	//LCD_CtrlData_HX8340B(0x001b,0x0022); 
	LCD_CtrlData_HX8340B(0x0021,0x0001);
	LCD_CtrlData_HX8340B(0x0001,0x0000);////
	LCD_CtrlData_HX8340B(0x001C,0x0003);
	LCD_CtrlData_HX8340B(0x0019,0x0006);
	LCD_CtrlData_HX8340B(0x002F,0x0011);//line inversion
	LCD_Delayms(200);
	
	//Display on Setting
	LCD_CtrlData_HX8340B(0x0026,0x0084);
	LCD_Delayms(50);
	LCD_CtrlData_HX8340B(0x0026,0x00B8);
	LCD_Delayms(10);
	LCD_CtrlData_HX8340B(0x0026,0x00BC);
	LCD_Delayms(50);
	
	//Set GRAM Area
	LCD_CtrlData_HX8340B(0x0002,0x0000);
	LCD_CtrlData_HX8340B(0x0003,0x0000);
	LCD_CtrlData_HX8340B(0x0004,0x0000);
	LCD_CtrlData_HX8340B(0x0005,0x00AF);////
	
	LCD_CtrlData_HX8340B(0x0006,0x0000);
	LCD_CtrlData_HX8340B(0x0007,0x0000);
	LCD_CtrlData_HX8340B(0x0008,0x0000);
	LCD_CtrlData_HX8340B(0x0009,0x00DB);////
	
	
	LCD_CtrlData_HX8340B(0x0017,0x0005); //05 16bit/pixel; // 06  18bit/pixel
	
	LCD_CtrlWrite_HX8340B(0x0022);
	
	return ERR_LCD_NONE;
}


/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 

//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8340B_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"qinss LCD: in HX8340B_EnterSleep, is_sleep = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8340B_255_112_2_18_0_33_28_1746,(uint8*)"d", is_sleep);
	if ( is_sleep ) // enter sleep mode.
	{
		LCD_CtrlData_HX8340B(0x26,0xB8); //GON=¡¯1¡¯ DTE=¡¯1¡¯ D[1:0]=¡¯10¡¯
		LCD_Delayms(40);
		LCD_CtrlData_HX8340B(0x19,0x01); //VCOMG=¡¯0¡¯, PON=¡¯0¡¯, DK=¡¯1¡¯
		LCD_Delayms(40);
		LCD_CtrlData_HX8340B(0x26,0xA4); //GON=¡¯1¡¯ DTE=¡¯0¡¯ D[1:0]=¡¯01¡¯
		LCD_Delayms(40);
		LCD_CtrlData_HX8340B(0x26,0x84); //GON=¡¯0¡¯ DTE=¡¯0¡¯ D[1:0]=¡¯01¡¯
		LCD_Delayms(40);
		LCD_CtrlData_HX8340B(0x1C,0x00); //AP[2:0]=¡¯000¡¯
		LCD_CtrlData_HX8340B(0x01,0x02); //SLP=¡¯1¡¯
		LCD_CtrlData_HX8340B(0x21,0x00); //OSC_EN=¡¯0¡¯		
	}
	else 			// out sleep mode 
	{
		#if 1
	 	HX8340B_Init();
	 	#else
		LCD_CtrlData_HX8340B(0x18,0x33); //I/P_RADJ,N/P_RADJ, Normal mode 60Hz
		LCD_CtrlData_HX8340B(0x21,0x01); //OSC_EN='1', start Osc
		LCD_CtrlData_HX8340B(0x01,0x00); //SLP='0', out sleep
		LCD_CtrlData_HX8340B(0x1C,0x03); //AP=011
		LCD_CtrlData_HX8340B(0x19,0x06); // VOMG=1,PON=1, DK=0,
		LCD_Delayms(5);
		LCD_CtrlData_HX8340B(0x26,0x84); //PT=10,GON=0, DTE=0, D=0100
		LCD_Delayms(40);
		LCD_CtrlData_HX8340B(0x26,0xB8); //PT=10,GON=1, DTE=1, D=1000
		LCD_Delayms(40);
		LCD_CtrlData_HX8340B(0x26,0xBC); //PT=10,GON=1, DTE=1, D=1100
	 	#endif
	 	
	}

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 

//	Note:
/******************************************************************************/
LOCAL void HX8340B_Close(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in HX8340B_Close"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8340B_299_112_2_18_0_33_28_1747,(uint8*)"");

	//GPIO_SetLcdBackLight( SCI_FALSE );
	HX8340B_EnterSleep( SCI_TRUE );	
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 

//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8340B_Invalidate(void)
{
	HX8340B_set_display_window (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//	Global resource dependence: 

//	Note:       
//     To improve speed, lcd is operate in HIGH SPEED RAM WRITE MODE(4
//     uint16 are write continuously always.) So, some dummy uint16 
//     should be inserted to satisfy this mode.   Please refer to spec.
/******************************************************************************/
PUBLIC ERR_LCD_E HX8340B_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{
  	left 	= (left >= LCD_WIDTH)    ? LCD_WIDTH-1 : left;
	right 	= (right >= LCD_WIDTH)   ? LCD_WIDTH-1 : right;
	top 	= (top >= LCD_HEIGHT)    ? LCD_HEIGHT-1 : top;
	bottom 	= (bottom >= LCD_HEIGHT) ? LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
    	return ERR_LCD_OPERATE_FAIL;
    }     
   
    HX8340B_set_display_window(left, top, right, bottom); 
  
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
LOCAL ERR_LCD_E HX8340B_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	//int32 error;
	

	switch(angle)
	{
		case LCD_ANGLE_0:
			HX8340B_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			HX8340B_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			HX8340B_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			HX8340B_set_display_window(left, top, bottom,right);
			break;
		default:
			HX8340B_set_display_window(left, top, right, bottom);
			break;			
	}
		

	return ERR_LCD_NONE;
}//en of HX8340B_VerticalInvalidateRect	


/******************************************************************************/
//  Description:  set the contrast value 
//	Global resource dependence: 

//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   HX8340B_SetContrast(
	uint16  contrast	//contrast value to set
	)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
} 


/*****************************************************************************/
//  Description:    Set the brightness of LCD.
//	Global resource dependence: 

//	Note:
/*****************************************************************************/
LOCAL ERR_LCD_E   HX8340B_SetBrightness(
	uint16 brightness	//birghtness to set
	)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
}

/*****************************************************************************/
//  Description:    Enable lcd to partial display mode, so can save power.
//	Global resource dependence: 

//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//	Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E HX8340B_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	 	
	HX8340B_set_display_window(left, top, right, bottom);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 

//	Note:
/******************************************************************************/
LOCAL const LCD_OPERATIONS_T HX8340B_operations = 
{
	HX8340B_Init,
	HX8340B_EnterSleep,
	HX8340B_SetContrast,
	HX8340B_SetBrightness,
	HX8340B_SetDisplayWindow,
	HX8340B_InvalidateRect,
	HX8340B_Invalidate,
	HX8340B_Close,
	HX8340B_RotationInvalidateRect,
	HX8340B_SetDirection,
    NULL,
    NULL
};

/******************************************************************************/
//  Description:  return the HX8340B lcd driver funtion pointer
//	Global resource dependence: 

//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* HX8340B_GetOperations(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in HX8340B_GetOperations"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8340B_451_112_2_18_0_33_28_1748,(uint8*)"");

	return (LCD_OPERATIONS_T*)&HX8340B_operations;
}

PUBLIC BOOLEAN HX8340B_Probe(void)
{
	return SCI_TRUE;	
}

LOCAL  const LCD_TIMING_U s_HX8340B_timing =
{

	        // LCM_CYCLE_U start(ns)
	5,		// CS setup time for LCM read (optional)  
	150,	// low pulse width for LCM read (according spec)	
	150,	// high pulse width for LCM read (according spec)
	10,		// CS setup time for LCM write  (optional) 	
	50,		// low pulse width for LCM write (according spec)
	50,	    // high pulse width for LCM write (according spec)	
	        // LCM_CYCLE_U end(ns)
};


const LCD_SPEC_T g_lcd_HX8340B = {
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_8,
	(LCD_TIMING_U*)&s_HX8340B_timing,
	(LCD_OPERATIONS_T*)&HX8340B_operations,
	0,
	0
};

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 


