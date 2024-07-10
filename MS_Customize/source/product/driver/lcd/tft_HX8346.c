/******************************************************************************
 ** File Name:     TFT_HX8346.c                                             *
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


#define LCD_CtrlWrite_HX8346( _cmd )\
				LCD_SendCmd( (_cmd ), 0 );

#define LCD_DataWrite_HX8346( _data )\
				LCD_SendData( (_data), 0 );

#define LCD_CtrlData_HX8346( _cmd, _data ) \
				LCD_SendCmdData( (_cmd ),  (_data),0 );  
				
/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void HX8346_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8346_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);

/*********************************************************************/
//  Description:   Initialize color LCD : HX8346
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E HX8346_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8346_Invalidate(void);


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
LOCAL ERR_LCD_E HX8346_InvalidateRect(
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
PUBLIC void HX8346_set_display_window(
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
LOCAL ERR_LCD_E HX8346_SetDirection(
	LCD_DIRECT_E direct_type
	)
{
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:			
		    LCD_CtrlWrite_HX8346(0x0003);		    
		    LCD_DataWrite_HX8346(0x1030);
			break;
			
		case LCD_DIRECT_ROT_90:
			LCD_CtrlWrite_HX8346(0x0003);		    
		    LCD_DataWrite_HX8346(0x1028);	 
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlWrite_HX8346(0x0003);		    
		    LCD_DataWrite_HX8346(0x1000);	 
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlWrite_HX8346(0x0003);		    
		    LCD_DataWrite_HX8346(0x1018);
			break;

		case LCD_DIRECT_MIR_H:
			LCD_CtrlWrite_HX8346(0x0003);		    
		    LCD_DataWrite_HX8346(0x1020);
			break;
			
		case LCD_DIRECT_MIR_V:
			LCD_CtrlWrite_HX8346(0x0003);		    
		    LCD_DataWrite_HX8346(0x1010);
			break;
			
		case LCD_DIRECT_MIR_HV:
			LCD_CtrlWrite_HX8346(0x0003);		    
		    LCD_DataWrite_HX8346(0x1000);
			break;
			
		default:			
			LCD_CtrlWrite_HX8346(0x0003);		    
		    LCD_DataWrite_HX8346(0x1030);	
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
__inline void HX8346_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 top, 	// end Horizon address
	uint16 right, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{   

}

/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E HX8346_Init(void)
{

	//SCI_TRACE_LOW:"qinss LCD: in HX8346_reset"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8346_181_112_2_18_0_33_29_1749,(uint8*)"");
	
    LCD_Reset();//reset lcd throught lcdc	
    SCI_Sleep(100);

    LCD_CtrlData_HX8346(0x0046,0x0094); //IDMON=0,INVON=1,NORON=1,PTLON=0
 	SCI_Sleep(5);
	LCD_CtrlData_HX8346(0x0047,0x0041); //MY=0,MX=0,MV=0,ML=0,BGR=0,TEON=0
 	SCI_Sleep(5);
	LCD_CtrlData_HX8346(0x0048,0x0000); //N_DC=1001 0101
 	SCI_Sleep(5);
	LCD_CtrlData_HX8346(0x0049,0x0033); //P_DC=1001 0101
 	SCI_Sleep(5);
	LCD_CtrlData_HX8346(0x004A,0x0023); //I_DC=1111 1111
 	SCI_Sleep(5);	
	LCD_CtrlData_HX8346(0x004B,0x0045); //N_BP=0000 0010
 	//SCI_Sleep(5);
	LCD_CtrlData_HX8346(0x004C,0x0044); //N_FP=0000 0010
 	//SCI_Sleep(5);
	LCD_CtrlData_HX8346(0x004D,0x0077); //P_BP=0000 0010
 	//SCI_Sleep(5);
	LCD_CtrlData_HX8346(0x004E,0x0012); //P_FP=0000 0010
 	//SCI_Sleep(5);
	LCD_CtrlData_HX8346(0x004F,0x00CC); //I_BP=0000 0010
 	//SCI_Sleep(5);
	LCD_CtrlData_HX8346(0x0050,0x0046); //I_FP=0000 0010
 	//SCI_Sleep(5);
	LCD_CtrlData_HX8346(0x0051,0x0082);  //N_RTN=0000,N_NW=001
 	//SCI_Sleep(5);	
	
//******** 240X320 Windows Setting **********************

  	LCD_CtrlData_HX8346(0x0002,0x0000);  //P_RTN=0000,P_NW=001
 	//SCI_Sleep(5);
	LCD_CtrlData_HX8346(0x0003,0x0000);  //I_RTN=1111,I_NW=000
 	//SCI_Sleep(5);
	LCD_CtrlData_HX8346(0x0004,0x0000);  //DIV=00
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0005,0x00EF); // Column address end 1
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0006,0x0000); // Row address start 2
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0007,0x0000); // Row address start 1
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0008,0x0001); // Row address end 2
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0009,0x003F); // Row address end 1
 	//SCI_Sleep(5);

  
//******** Gamma Setting ***********************************

 //LCD_CtrlData_HX8346(0x0001,0x0006);
 //SCI_Sleep(5);
	LCD_CtrlData_HX8346(0x0016,0x0008);//0008
	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0023,0x0095);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0024,0x0095);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0025,0x00FF);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0027,0x0002);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0028,0x0002);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0029,0x0002);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x002A,0x0002);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x002C,0x0002);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x002D,0x0002);
 	//SCI_Sleep(5);
  
  	LCD_CtrlData_HX8346(0x003A,0x0001);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x003B,0x0001);
 	//SCI_Sleep(5);

  	LCD_CtrlData_HX8346(0x003C,0x00F0);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x003D,0x0000);
 	//SCI_Sleep(5);

  	SCI_Sleep(20); 
  	LCD_CtrlData_HX8346(0x0035,0x0038);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0036,0x0078);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x003E,0x0038);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0040,0x000F);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0041,0x00F0);
 	//SCI_Sleep(5);
  
  //******** Power supply Setting ***********************************

  	LCD_CtrlData_HX8346(0x0019,0x0049); //OSCADJ=10 1000  OSD_EN=1
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0093,0x000F);
 	//SCI_Sleep(5);
  	SCI_Sleep(10);
  	LCD_CtrlData_HX8346(0x0020,0x0030); //BT=0100             
 	//SCI_Sleep(5);
  
  	LCD_CtrlData_HX8346(0x001D,0x0007); //VC2=100,VC1=100     DDVDH=VBGP*5.47   =4.6625
 	SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x001E,0x0000); //VC3=000             VREG3=DDVDH,
 	SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x001F,0x0007); //VRH=0110            VREG1=VBGP*2.8    =3.5
 	SCI_Sleep(5);
  
  //******** Vcom Setting for CMO 3.2"Panel ***********************************

  	LCD_CtrlData_HX8346(0x0044,0x003a); //VCM=101 1010   5A  36
 	SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0045,0x000A); //VDV=1 0001
 	//SCI_Sleep(5);
  	SCI_Sleep(10);
  
  	LCD_CtrlData_HX8346(0x001C,0x0004); //AP=100
 	//SCI_Sleep(5);
  	SCI_Sleep(20);
  	LCD_CtrlData_HX8346(0x0043,0x0080); //VCOMG=1
 	//SCI_Sleep(5);
  	SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x001B,0x0008); //GASENB=0,PON=1,DK=1,XDK=0,DDVDH_TRI=0,STB=0
 	//SCI_Sleep(5);
  	SCI_Sleep(40);
  	LCD_CtrlData_HX8346(0x001B,0x0010); //GASENB=0,PON=1,DK=0,XDK=0,DDVDH_TRI=0,STB=0
 	//SCI_Sleep(5);
  	SCI_Sleep(40);
  
//******** Display on  Setting *********************************** 

  	LCD_CtrlData_HX8346(0x0090,0x007F);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0026,0x0004); //SAPS1=1000
 	//SCI_Sleep(5);
  	SCI_Sleep(40);  
  	LCD_CtrlData_HX8346(0x0026,0x0024); //GON=1,DTE=0,D=01
 	SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0026,0x002C); //GON=0,DTE=0,D=11
 	//SCI_Sleep(5);
  	SCI_Sleep(40);
	LCD_CtrlData_HX8346(0x0026,0x003C); //GON=1,DTE=1,D=11
 	//SCI_Sleep(5);
 	LCD_CtrlData_HX8346(0x0001,0x0006);//0002
 	//SCI_Sleep(5);
 
//******** Set internal VDDD voltage *********************************** 
  	LCD_CtrlData_HX8346(0x0057,0x0002);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0055,0x0000);
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0057,0x0000); 
 	//SCI_Sleep(5);
  	LCD_CtrlData_HX8346(0x0070,0x0060); 
 	//SCI_Sleep(5);
	LCD_CtrlWrite_HX8346(0x0022);
	
	//SCI_TRACE_LOW:"qinss LCD: in HX8346_reset end."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8346_344_112_2_18_0_33_29_1750,(uint8*)"");
	
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8346_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"qinss LCD: in HX8346_EnterSleep, is_sleep = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8346_357_112_2_18_0_33_29_1751,(uint8*)"d", is_sleep);
	
	if ( is_sleep ) // enter sleep mode.
	{
		// Display Off
		LCD_CtrlData_HX8346(0x0026, 0x000E);	
		SCI_Sleep(5); 
		LCD_CtrlData_HX8346(0x0026, 0x000A);
		SCI_Sleep(5); 
		LCD_CtrlData_HX8346(0x0026, 0x0000);

		//power off
		LCD_CtrlData_HX8346(0x0094, 0x0000);	
		LCD_CtrlData_HX8346(0x001c, 0x0000);
		LCD_CtrlData_HX8346(0x001B, 0x0000);
		LCD_CtrlData_HX8346(0x001B, 0x0008);
		LCD_CtrlData_HX8346(0x0043, 0x0000);
		LCD_CtrlData_HX8346(0x001B,0x0011);
		SCI_Sleep(5);
		LCD_CtrlData_HX8346(0x0019,0x0048); 
	}
	else 			// out sleep mode 
	{
		HX8346_Init();
	}

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void HX8346_Close(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in HX8346_Close"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8346_392_112_2_18_0_33_29_1752,(uint8*)"");

	//GPIO_SetLcdBackLight( SCI_FALSE );
	HX8346_EnterSleep( SCI_TRUE );	
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8346_Invalidate(void)
{

	//HX8346_set_display_window (0x0, HX8346_WIDTH - 1, 0x0, HX8346_HEIGHT -1);
	HX8346_set_display_window (0x0, 0X0, QVGA_LCD_WIDTH - 1, QVGA_LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E HX8346_InvalidateRect(
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
   
    HX8346_set_display_window(left, top, right, bottom); 
    //HX8346_set_display_window(left, right, top, bottom); 
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
LOCAL ERR_LCD_E HX8346_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	switch(angle)
	{
		case LCD_ANGLE_0:
			HX8346_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			HX8346_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			HX8346_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			HX8346_set_display_window(left, top, bottom,right);
			break;
		default:
			HX8346_set_display_window(left, top, right, bottom);
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
LOCAL ERR_LCD_E   HX8346_SetContrast(
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
LOCAL ERR_LCD_E   HX8346_SetBrightness(
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
LOCAL ERR_LCD_E HX8346_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	 	
	HX8346_set_display_window(left, top, right, bottom);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL const LCD_OPERATIONS_T HX8346_operations = 
{
	HX8346_Init,
	HX8346_EnterSleep,
	HX8346_SetContrast,
	HX8346_SetBrightness,
	HX8346_SetDisplayWindow,
	HX8346_InvalidateRect,
	HX8346_Invalidate,
	HX8346_Close,
	HX8346_RotationInvalidateRect,
	HX8346_SetDirection,
    NULL,
    NULL
};

/******************************************************************************/
//  Description:  return the HX8346 lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* HX8346_GetOperations(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in HX8346_GetOperations"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8346_541_112_2_18_0_33_29_1753,(uint8*)"");

	return (LCD_OPERATIONS_T*)&HX8346_operations;
}

PUBLIC BOOLEAN HX8346_Probe(void)
{
	return SCI_TRUE;	
}

LOCAL const LCD_TIMING_U s_HX8346_timing =
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


const LCD_SPEC_T g_lcd_HX8346[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U*)&s_HX8346_timing, 
	(LCD_OPERATIONS_T*)&HX8346_operations,
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


