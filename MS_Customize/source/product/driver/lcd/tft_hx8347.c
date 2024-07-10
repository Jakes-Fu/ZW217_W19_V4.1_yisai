/******************************************************************************
 ** File Name:     TFT_HX8347.c                                             *
 ** Description:															  *
 **    This file contains driver for color LCD.(CSTN)						  *
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



#define LCD_CtrlWrite_HX8347( _cmd )\
				LCD_SendCmd( (_cmd ), 0 );

#define LCD_DataWrite_HX8347( _data )\
				LCD_SendData( (_data), 0 );

#define LCD_CtrlData_HX8347( _cmd, _data ) \
				LCD_SendCmdData( (_cmd ),  (_data),0 );  

LOCAL BOOLEAN g_is_invert = FALSE ;
				
/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void HX8347_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8347_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);

/*********************************************************************/
//  Description:   Initialize color LCD : HX8347
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E HX8347_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8347_Invalidate(void);


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
LOCAL ERR_LCD_E HX8347_InvalidateRect(
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
PUBLIC void HX8347_set_display_window(
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
LOCAL ERR_LCD_E HX8347_SetDirection(
	LCD_DIRECT_E direct_type
	)
{
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:			
		   LCD_CtrlWrite_HX8347(0x0016);		    
		    LCD_DataWrite_HX8347(0x00c8);
			break;
			
		case LCD_DIRECT_ROT_90:
			LCD_CtrlWrite_HX8347(0x0003);		    
		    LCD_DataWrite_HX8347(0x1018);	 
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlWrite_HX8347(0x0003);		    
		    LCD_DataWrite_HX8347(0x1000);	 
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlWrite_HX8347(0x0016);		    
		    LCD_DataWrite_HX8347(0x00a8);
			break;

		case LCD_DIRECT_MIR_H:

			break;
			
		case LCD_DIRECT_MIR_V:

			break;
			
		case LCD_DIRECT_MIR_HV:

			break;
			
		default:			
			LCD_CtrlWrite_HX8347(0x0016);		    
		    LCD_DataWrite_HX8347(0x00c8);
		    direct_type = LCD_DIRECT_NORMAL;
			break;
			
	}	

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
__inline void HX8347_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 top, 	// end Horizon address
	uint16 right, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{   
	uint16 left_temp = 0;
	uint16 right_temp = 0;
	uint16 top_temp = 0 ;
	uint16 bottom_temp = 0 ;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  由于这个IC的特性,以及LCM走线的关系,在反转的时候,需要把X ,Y反
//  过来送 -----Vincent  20071031 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   if(g_is_invert)
   {
        left_temp = left;
        top_temp = top;
        right_temp = right;
        bottom_temp = bottom ;

        left = top_temp ;
        top = left_temp ;
        right = bottom_temp;
        bottom = right_temp;             
   }
}


/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E HX8347_Init(void)
{

	//SCI_TRACE_LOW:"qinss LCD: in HX8347_reset"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347_199_112_2_18_0_33_30_1754,(uint8*)"");
	
    LCD_Reset();//reset lcd throught lcdc	
    SCI_Sleep(400);

//########################################################################## 
// Gamma for CMO 2.8 

	LCD_CtrlData_HX8347(0x0046,0x0095);
	LCD_CtrlData_HX8347(0x0047,0x0051);
	LCD_CtrlData_HX8347(0x0048,0x0000);
	LCD_CtrlData_HX8347(0x0049,0x0036);
	LCD_CtrlData_HX8347(0x004A,0x0011);
	LCD_CtrlData_HX8347(0x004B,0x0066);
	LCD_CtrlData_HX8347(0x004C,0x0014);
	LCD_CtrlData_HX8347(0x004D,0x0077);
	LCD_CtrlData_HX8347(0x004E,0x0013);
	LCD_CtrlData_HX8347(0x004F,0x004C);
	LCD_CtrlData_HX8347(0x0050,0x0046);
	LCD_CtrlData_HX8347(0x0051,0x0046);

	//240x00320 window setting 

	LCD_CtrlData_HX8347(0x0002,0x0000);
	LCD_CtrlData_HX8347(0x0003,0x0000);
	LCD_CtrlData_HX8347(0x0004,0x0000);
	LCD_CtrlData_HX8347(0x0005,0x00EF);


	// Column address start2 
	// Column address start1 
	// Column address end2 
	// Column address end1 
	LCD_CtrlData_HX8347(0x0006,0x0000); // Row address start2 
	LCD_CtrlData_HX8347(0x0007,0x0000); // Row address start1 
	LCD_CtrlData_HX8347(0x0008,0x0001); // Row address end2 
	LCD_CtrlData_HX8347(0x0009,0x003F); // Row address end1 
	// Display Setting 
	LCD_CtrlData_HX8347(0x0001,0x0006); // IDMON=0, INVON=1, NORON=1, PTLON=0 
	LCD_CtrlData_HX8347(0x0016,0x00c8); // MY=0, MX=0, MV=0, ML=1, BGR=0, TEON=0 
	LCD_CtrlData_HX8347(0x0038,0x0000); // RGB_EN=0, use MPU Interface 
	LCD_CtrlData_HX8347(0x0023,0x0095); // N_DC=1001 0101 
	LCD_CtrlData_HX8347(0x0024,0x0095); // P_DC=1001 0101
	LCD_CtrlData_HX8347(0x0025,0x00FF); // I_DC=1111 1111


	LCD_CtrlData_HX8347(0x0027,0x0006); // N_BP=0000 0110
	LCD_CtrlData_HX8347(0x0028,0x0006); // N_FP=0000 0110
	LCD_CtrlData_HX8347(0x0029,0x0006); // P_BP=0000 0110 
	LCD_CtrlData_HX8347(0x002A,0x0006); // P_FP=0000 0110
	LCD_CtrlData_HX8347(0x002C,0x0006); // I_BP=0000 0110
	LCD_CtrlData_HX8347(0x002D,0x0006); // I_FP=0000 0110


	LCD_CtrlData_HX8347(0x003A,0x0001); // N_RTN=0000, N_NW=001
	LCD_CtrlData_HX8347(0x003B,0x0000); // P_RTN=0000, P_NW=000 
	LCD_CtrlData_HX8347(0x003C,0x00F0); // I_RTN=1111, I_NW=000
	LCD_CtrlData_HX8347(0x003D,0x0000); // DIV=00
	SCI_Sleep(20); 


	//240RGB x 320 dot, 262K color, TFT Mobile Single Chip Driver 

	//APPLICATION NOTE V02 

	LCD_CtrlData_HX8347(0x0035,0x0038); // EQS=38h
	LCD_CtrlData_HX8347(0x0036,0x0078); // EQP=78h


	LCD_CtrlData_HX8347(0x003E,0x0038); // SON=38h 

	LCD_CtrlData_HX8347(0x0040,0x000F); // GDON=0Fh
	LCD_CtrlData_HX8347(0x0041,0x00F0); // GDOFF


	// Power Supply Setting 

	LCD_CtrlData_HX8347(0x0019,0x0049); 
	LCD_CtrlData_HX8347(0x0093,0x000C); 
	SCI_Sleep(10); 


	LCD_CtrlData_HX8347(0x0020,0x0040); 

	LCD_CtrlData_HX8347(0x001D,0x0007); 
	LCD_CtrlData_HX8347(0x001E,0x0000); 
	LCD_CtrlData_HX8347(0x001F,0x0004); 


	// VCOM Setting for CMO 2.8” Panel 

	LCD_CtrlData_HX8347(0x0044,0x004a);
	LCD_CtrlData_HX8347(0x0045,0x0011);
	SCI_Sleep(10); 


	LCD_CtrlData_HX8347(0x001C,0x0004); 

	SCI_Sleep(20); 

	// OSCADJ=10 0000, OSD_EN=1 //60Hz 
	// RADJ=1100 
	// BT=0100 
	// VC1=111 
	// VC3=000 
	// VRH=0100 
	// VCM=101 0000 
	// VDV=1 0001 
	// AP=100 
	// GASENB=0, PON=1, DK=1, XDK=0, DDVDH_TRI=0, STB=0 
	// GASENB=0, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0 
	LCD_CtrlData_HX8347(0x001B,0x0018); 
	SCI_Sleep(40); 
	LCD_CtrlData_HX8347(0x001B,0x0010); 
	SCI_Sleep(40); 
	LCD_CtrlData_HX8347(0x0043,0x0080); //Set VCOMG=1 
	SCI_Sleep(100); 
	// Display ON Setting 
	LCD_CtrlData_HX8347(0x0090,0x007F); // SAP=0111 1111 
	LCD_CtrlData_HX8347(0x0026,0x0004); //GON=0, DTE=0, D=01
	SCI_Sleep(40); 
	LCD_CtrlData_HX8347(0x0026,0x0024); //GON=1, DTE=0, D=01
	LCD_CtrlData_HX8347(0x0026,0x002C); //GON=1, DTE=0, D=11 
	SCI_Sleep(40); 


	LCD_CtrlData_HX8347(0x0026,0x003C); //GON=1, DTE=1, D=11 

	// Internal register setting 

	LCD_CtrlData_HX8347(0x0057,0x0002); //Test_Mode Enable
	LCD_CtrlData_HX8347(0x0095,0x0001); // Set Display clock and Pumping clock to synchronize 
	LCD_CtrlData_HX8347(0x0057,0x0000); // Test_Mode Disable 

		
	//SCI_TRACE_LOW:"qinss LCD: in HX8347_reset end."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347_334_112_2_18_0_33_30_1755,(uint8*)"");
	
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8347_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"qinss LCD: in HX8347_EnterSleep, is_sleep = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347_347_112_2_18_0_33_30_1756,(uint8*)"d", is_sleep);
	
	if ( is_sleep ) // enter sleep mode.
	{
		// Display Off
		LCD_CtrlData_HX8347(0x0026, 0x000E);	
		SCI_Sleep(5); 
		LCD_CtrlData_HX8347(0x0026, 0x000A);
		SCI_Sleep(5); 
		LCD_CtrlData_HX8347(0x0026, 0x0000);

		//power off
		LCD_CtrlData_HX8347(0x0094, 0x0000);	
		LCD_CtrlData_HX8347(0x001c, 0x0000);
		LCD_CtrlData_HX8347(0x001B, 0x0000);
		LCD_CtrlData_HX8347(0x001B, 0x0008);
		LCD_CtrlData_HX8347(0x0043, 0x0000);
		LCD_CtrlData_HX8347(0x001B,0x0011);
		SCI_Sleep(5);
		LCD_CtrlData_HX8347(0x0019,0x0048); 
	}
	else 			// out sleep mode 
	{
		HX8347_Init();
	}

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void HX8347_Close(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in HX8347_Close"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347_382_112_2_18_0_33_30_1757,(uint8*)"");

	HX8347_EnterSleep( SCI_TRUE );	
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8347_Invalidate(void)
{
	HX8347_set_display_window (0x0, 0X0, QVGA_LCD_WIDTH - 1, QVGA_LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E HX8347_InvalidateRect(
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
   
    HX8347_set_display_window(left, top, right, bottom); 
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
LOCAL ERR_LCD_E HX8347_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	switch(angle)
	{
		case LCD_ANGLE_0:
			HX8347_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			HX8347_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			HX8347_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			HX8347_set_display_window(left, top, bottom,right);
			break;
		default:
			HX8347_set_display_window(left, top, right, bottom);
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
LOCAL ERR_LCD_E   HX8347_SetContrast(
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
LOCAL ERR_LCD_E   HX8347_SetBrightness(
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
LOCAL ERR_LCD_E HX8347_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	HX8347_set_display_window(left, top, right, bottom);
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL const LCD_OPERATIONS_T HX8347_operations = 
{
	HX8347_Init,
	HX8347_EnterSleep,
	HX8347_SetContrast,
	HX8347_SetBrightness,
	HX8347_SetDisplayWindow,
	HX8347_InvalidateRect,
	HX8347_Invalidate,
	HX8347_Close,
	HX8347_RotationInvalidateRect,
	HX8347_SetDirection,
    NULL,
    NULL
};

/******************************************************************************/
//  Description:  return the HX8347 lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* HX8347_GetOperations(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in HX8347_GetOperations"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347_525_112_2_18_0_33_31_1758,(uint8*)"");

	return (LCD_OPERATIONS_T*)&HX8347_operations;
}

PUBLIC BOOLEAN HX8347_Probe(void)
{
	return SCI_TRUE;	
}

LOCAL const LCD_TIMING_U s_HX8347_timing =
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


PUBLIC const LCD_SPEC_T g_lcd_HX8347[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U*)&s_HX8347_timing, 
	(LCD_OPERATIONS_T*)&HX8347_operations,
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


