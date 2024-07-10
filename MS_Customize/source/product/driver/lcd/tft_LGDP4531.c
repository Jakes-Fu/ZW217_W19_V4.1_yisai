/******************************************************************************
 ** File Name:     tft_lgdp4531.c                                             *
 ** Description:															  *
 **    This file contains driver for color LCD.(CSTN)						  *
 ** Author:                                                          *
 ** DATE:           04/24/2008                                                *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/24/2008     	    			Create.
 ******************************************************************************/

#include "ms_customize_trc.h"
#include "os_api.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"


#define  L_YG

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif



#define LCD_CtrlWrite_LGDP4531( _cmd )\
				LCD_SendCmd( (_cmd ), 0 );

#define LCD_DataWrite_LGDP4531( _data )\
				LCD_SendData( (_data), 0 );

#define LCD_CtrlData_LGDP4531( _cmd, _data ) \
				LCD_SendCmdData( (_cmd ),  (_data),0 );  
				

LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;
					
/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void LGDP4531_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  LGDP4531_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);


/*********************************************************************/
//  Description:   Initialize color LCD : LGDP4531
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E LGDP4531_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E LGDP4531_Invalidate(void);


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
LOCAL ERR_LCD_E LGDP4531_InvalidateRect(
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
PUBLIC void LGDP4531_set_display_window(
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
LOCAL ERR_LCD_E LGDP4531_SetDirection(
	LCD_DIRECT_E direct
	)
{
	uint32 reg_val  = 0;
	
	switch(direct)
	{
		case LCD_DIRECT_NORMAL:			
			reg_val = 0x1030;
		  
			break;
		case LCD_DIRECT_ROT_90:
			reg_val = 0x1018;
		    
			break;
		case LCD_DIRECT_ROT_180:
			reg_val = 0x1000;

			break;
		case LCD_DIRECT_ROT_270:
			reg_val = 0x1028;	 
	    	
			break;
		case LCD_DIRECT_MIR_H:
			reg_val = 0x1020;	 
	    	
			break;
		case LCD_DIRECT_MIR_V:
			reg_val = 0x1010;
	    	
			break;
		case LCD_DIRECT_MIR_HV:
			reg_val = 0x1028;
	    	
			break;		
		default:
			reg_val = 0x1030;	
		   
			break;
	}

	LCD_CtrlData_LGDP4531(0x0030, reg_val);
	
	s_lcd_direct = direct;	
		
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
__inline void LGDP4531_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 top, 	// end Horizon address
	uint16 right, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{   
	uint32 start_x = 0;
	uint32 start_y = 0;
	
	LCD_CtrlData_LGDP4531(0x0050, left);
	LCD_CtrlData_LGDP4531(0x0051, right);
	LCD_CtrlData_LGDP4531(0x0052, top);
	LCD_CtrlData_LGDP4531(0x0053, bottom);	
	
	switch(s_lcd_direct)
	{
		case LCD_DIRECT_NORMAL:	
			start_x = left;
			start_y = top;
			break;            
			
		case LCD_DIRECT_ROT_90:	
			start_x = right;
			start_y = top;
			break;
			
		case LCD_DIRECT_ROT_180:
			start_x = right;
			start_y = bottom;		
			break;
			
		case LCD_DIRECT_ROT_270:
			start_x = left;
			start_y = top;			
			break;

		case LCD_DIRECT_MIR_H:
			start_x = right;
			start_y = top;
			break;

		case LCD_DIRECT_MIR_V:
			start_x = left;
			start_y = bottom;
			break;

		case LCD_DIRECT_MIR_HV:
			start_x = right;
			start_y = bottom;
			break;
			
		default:			
		    start_x = left;
			start_y = top;
			break;	
	
	}

	LCD_CtrlData_LGDP4531(0x0020, start_x);
	LCD_CtrlData_LGDP4531(0x0021, start_y);
	
	LCD_CtrlWrite_LGDP4531(0x0022);
	
}

/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E LGDP4531_Init(void)
{

	//SCI_TRACE_LOW:"LCD: in LGDP4531_reset"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_LGDP4531_241_112_2_18_0_33_56_1828,(uint8*)"");
	
    LCD_Reset();//reset lcd throught lcdc	
    LCD_Delayms(100);


 	LCD_CtrlData_LGDP4531(0x0007,0x0000);	//Display Control 1 (R07h) PTDE[1:0] 0 0 0 BASEE 0 0 GON DTE COL 0 D[1:0]DTE=0,D=2h'0,GON=0
	LCD_CtrlData_LGDP4531(0x0012,0x0000);	//Power Control 3 (R12h) VRH PON=0
	LCD_CtrlData_LGDP4531(0x0013,0x0000);	//Power Control 4 (R13h) VCOMG=0
	LCD_Delayms(40);
	
	// Power Set
	LCD_CtrlData_LGDP4531(0x0010,0x0600); //Power Control 1 (R10h) BT	
	LCD_CtrlData_LGDP4531(0x0011,0x0000); //Power Control 2 (R11h) DC1, DC0, VC	
	LCD_CtrlData_LGDP4531(0x0012,0x0005); //Power Control 3 (R12h) VRH	
	LCD_CtrlData_LGDP4531(0x0013,0x1052); //Power Control 4 (R13h) VDV, VCM	
	LCD_CtrlData_LGDP4531(0x0015,0x0030); //RV		

	LCD_CtrlData_LGDP4531(0x0010,0x1628); //Power Control 1 (R10h) SAP, AP
    LCD_CtrlData_LGDP4531(0x0012,0x0013); //Power Control 4 (R13h) PON=1	
    LCD_Delayms(40);
      	     	
	LCD_CtrlData_LGDP4531(0x0010,0x2620); //Power Control 1 (R10h) DK=0
	LCD_CtrlData_LGDP4531(0x0013,0x3052); //Power Control 4 (R13h) VCOMG=1 0x3049
	LCD_Delayms(100);	
		
	// Other Set
	LCD_CtrlData_LGDP4531(0x0001,0x0100);//Driver output control (R01h)
	LCD_CtrlData_LGDP4531(0x0002,0x0300);//LCD Driving Wave Control (R02h)
	LCD_CtrlData_LGDP4531(0x0003,0x1030);//Entry Mode (R03h)
	LCD_CtrlData_LGDP4531(0x0008,0x0604);//Display Control 2 (R08h)
	LCD_CtrlData_LGDP4531(0x000A,0x0008);//Display Control 4 (R0Ah)
	LCD_CtrlData_LGDP4531(0x0041,0x0002);//EPROM Control Register 2 (R41h)
	LCD_CtrlData_LGDP4531(0x0060,0x2700);//Driver Output Control (R60h)
	LCD_CtrlData_LGDP4531(0x0061,0x0001);//Base Image Display Control (R61h)
	LCD_CtrlData_LGDP4531(0x0090,0x0178);//Panel Interface Control 1 (R90h) 0X0184
	LCD_CtrlData_LGDP4531(0x0092,0x0105);//Panel Interface Control 2 (R90h)
	LCD_CtrlData_LGDP4531(0x0093,0x0101);//Panel Interface Control 3 (R90h)
	LCD_CtrlData_LGDP4531(0x00A3,0x0010);//Test Register 4 (RA3h)
	LCD_Delayms(100);
	
	// Gamma Set
	LCD_CtrlData_LGDP4531(0x0030,0x0201);
	LCD_CtrlData_LGDP4531(0x0031,0x0007);
	LCD_CtrlData_LGDP4531(0x0032,0x0101);
	LCD_CtrlData_LGDP4531(0x0033,0x0000);
	LCD_CtrlData_LGDP4531(0x0034,0x0004);
	LCD_CtrlData_LGDP4531(0x0035,0x0002);
	LCD_CtrlData_LGDP4531(0x0036,0x0604);
	LCD_CtrlData_LGDP4531(0x0037,0x0000);
	LCD_CtrlData_LGDP4531(0x0038,0x0718);
	LCD_CtrlData_LGDP4531(0x0039,0x0000);
	LCD_CtrlData_LGDP4531(0x003A,0x0200);
	LCD_CtrlData_LGDP4531(0x003B,0x0000);
	LCD_CtrlData_LGDP4531(0x003C,0x0300);
	LCD_CtrlData_LGDP4531(0x003D,0x0002); 
	LCD_CtrlData_LGDP4531(0x003E,0x0001); 
	LCD_CtrlData_LGDP4531(0x003F,0x0000);  
	LCD_Delayms(40);	
	
	//Display On Sequence
	LCD_CtrlData_LGDP4531(0x0007,0x0001);//Display Control 1 (R07h) PTDE[1:0] 0 0 0 BASEE 0 0 GON DTE COL 0 D[1:0]
	LCD_CtrlData_LGDP4531(0x0007,0x0021);//Display Control 1 (R07h) PTDE[1:0] 0 0 0 BASEE 0 0 GON DTE COL 0 D[1:0]
	LCD_CtrlData_LGDP4531(0x0007,0x0023);//Display Control 1 (R07h) PTDE[1:0] 0 0 0 BASEE 0 0 GON DTE COL 0 D[1:0]
	LCD_CtrlData_LGDP4531(0x0007,0x0133);//Display Control 1 (R07h) PTDE[1:0] 0 0 0 BASEE 0 0 GON DTE COL 0 D[1:0]
	LCD_CtrlWrite_LGDP4531(0x0022);

	//SCI_TRACE_LOW:"LCD: in LGDP4531_reset end."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_LGDP4531_308_112_2_18_0_33_56_1829,(uint8*)"");
	
	return ERR_LCD_NONE;
}



/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  LGDP4531_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:" LCD: in LGDP4531_EnterSleep, is_sleep = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_LGDP4531_323_112_2_18_0_33_56_1830,(uint8*)"d", is_sleep);
	if (is_sleep)//enter into sleep
	{
                  
        LCD_CtrlData_LGDP4531(0x0007, 0x0132); // display OFF
        LCD_CtrlData_LGDP4531(0x0007, 0x0122); // display OFF
        LCD_CtrlData_LGDP4531(0x0007, 0x0102); // display OFF
        LCD_CtrlData_LGDP4531(0x0007, 0x0000); // display OFF
	     //*************Power OFF sequence ******************//
	    LCD_CtrlData_LGDP4531(0x0010, 0x0000); // SAP, BT[3:0], APE, AP, DSTB, SLP
	    LCD_CtrlData_LGDP4531(0x0011, 0x0000); // DC1[2:0], DC0[2:0], VC[2:0]
	    LCD_CtrlData_LGDP4531(0x0012, 0x0000); // VREG1OUT voltage
	    LCD_CtrlData_LGDP4531(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
	    LCD_Delayms(200); // Dis-charge capacitor power voltage
	    LCD_CtrlData_LGDP4531(0x0010, 0x0002); // SAP, BT[3:0], APE, AP, DSTB, SLP
        LCD_Delayms(150);	
	}
	else//exit sleep
	{
	        // Power Set
		LCD_CtrlData_LGDP4531(0x0010,0x0600); //Power Control 1 (R10h) BT	
		LCD_CtrlData_LGDP4531(0x0011,0x0000); //Power Control 2 (R11h) DC1, DC0, VC	
		LCD_CtrlData_LGDP4531(0x0012,0x0005); //Power Control 3 (R12h) VRH	
		LCD_CtrlData_LGDP4531(0x0013,0x1052); //Power Control 4 (R13h) VDV, VCM	
		LCD_CtrlData_LGDP4531(0x0015,0x0030); //RV		

		LCD_CtrlData_LGDP4531(0x0010,0x1628); //Power Control 1 (R10h) SAP, AP
	    LCD_CtrlData_LGDP4531(0x0012,0x0013); //Power Control 4 (R13h) PON=1	
	    LCD_Delayms(40);
	      	     	
		LCD_CtrlData_LGDP4531(0x0010,0x2620); //Power Control 1 (R10h) DK=0
		LCD_CtrlData_LGDP4531(0x0013,0x3052); //Power Control 4 (R13h) VCOMG=1 0x3049
		LCD_Delayms(100);

	       //Display On Sequence
		LCD_CtrlData_LGDP4531(0x0007,0x0001);//Display Control 1 (R07h) PTDE[1:0] 0 0 0 BASEE 0 0 GON DTE COL 0 D[1:0]
		LCD_CtrlData_LGDP4531(0x0007,0x0021);//Display Control 1 (R07h) PTDE[1:0] 0 0 0 BASEE 0 0 GON DTE COL 0 D[1:0]
		LCD_CtrlData_LGDP4531(0x0007,0x0023);//Display Control 1 (R07h) PTDE[1:0] 0 0 0 BASEE 0 0 GON DTE COL 0 D[1:0]
		LCD_CtrlData_LGDP4531(0x0007,0x0133);//Display Control 1 (R07h) PTDE[1:0] 0 0 0 BASEE 0 0 GON DTE COL 0 D[1:0]
		
	}
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void LGDP4531_Close(void)
{
	//SCI_TRACE_LOW:"LCD: in LGDP4531_Close"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_LGDP4531_373_112_2_18_0_33_57_1831,(uint8*)"");

	LGDP4531_EnterSleep( SCI_TRUE );	
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E LGDP4531_Invalidate(void)
{
	LGDP4531_set_display_window (0x0, 0X0, QVGA_LCD_WIDTH - 1, QVGA_LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E LGDP4531_InvalidateRect(
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
   
    LGDP4531_set_display_window(left, top, right, bottom); 
    //LGDP4531_set_display_window(left, right, top, bottom); 
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
LOCAL ERR_LCD_E LGDP4531_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{


	switch(angle)
	{
		case LCD_ANGLE_0:
			LGDP4531_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			LGDP4531_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			LGDP4531_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			LGDP4531_set_display_window(left, top, bottom,right);
			break;
		default:
			LGDP4531_set_display_window(left, top, right, bottom);
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
LOCAL ERR_LCD_E   LGDP4531_SetContrast(
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
LOCAL ERR_LCD_E   LGDP4531_SetBrightness(
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
LOCAL ERR_LCD_E LGDP4531_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	 	
	LGDP4531_set_display_window(left, top, right, bottom);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL LCD_OPERATIONS_T LGDP4531_operations = 
{
	LGDP4531_Init,
	LGDP4531_EnterSleep,
	LGDP4531_SetContrast,
	LGDP4531_SetBrightness,
	LGDP4531_SetDisplayWindow,
	LGDP4531_InvalidateRect,
	LGDP4531_Invalidate,
	LGDP4531_Close,
	LGDP4531_RotationInvalidateRect,
	LGDP4531_SetDirection,
    NULL,
    NULL
};

/******************************************************************************/
//  Description:  return the LGDP4531 lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* LGDP4531_GetOperations(void)
{
	//SCI_TRACE_LOW:"LCD: in LGDP4531_GetOperations"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_LGDP4531_523_112_2_18_0_33_57_1832,(uint8*)"");

	return (LCD_OPERATIONS_T*)&LGDP4531_operations;
}

PUBLIC BOOLEAN LGDP4531_Probe(void)
{
	return SCI_TRUE;	
}

LOCAL const LCD_TIMING_U s_LGDP4531_timing =
{

	        // LCM_CYCLE_U start(ns)
	10,		// CS setup time for LCM read (optional)  
	150,	// low pulse width for LCM read (according spec)	
	100,	// high pulse width for LCM read (according spec)
	30,		// CS setup time for LCM write  (optional) 	
	60,		// low pulse width for LCM write (according spec)
	70,	    // high pulse width for LCM write (according spec)	
	        // LCM_CYCLE_U end(ns)
};


const LCD_SPEC_T g_lcd_LGDP4531[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U*)&s_LGDP4531_timing, 
	(LCD_OPERATIONS_T*)&LGDP4531_operations,
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


