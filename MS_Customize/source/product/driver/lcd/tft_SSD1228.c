/******************************************************************************
 ** File Name:     di_SSD1288.c                                             *
 ** Description:															  *
 **    This file contains driver for color LCD.(CSTN)						  *
 ** DATE:           03/01/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 02/04/2004     xiaowei.chen	    Create.
 ******************************************************************************/

#include "ms_customize_trc.h"
#include "os_api.h"
#include "sci_types.h"
#include "lcd_cfg.h"
#include "dal_lcd.h"

 #ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/


#define	SSD1288_WIDTH	LCD_WIDTH
#define	SSD1288_HEIGHT	LCD_HEIGHT


LOCAL BOOLEAN	s_is_invert = FALSE;
						
  /******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void SSD1288_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  SSD1288_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);


/*********************************************************************/
//  Description:   Initialize color LCD : SSD1288
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E SSD1288_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E SSD1288_Invalidate(void);

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
LOCAL ERR_LCD_E SSD1288_InvalidateRect(
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
__inline void SSD1288_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 right, 	// end Horizon address
	uint16 top, 		// start Vertical address
	uint16 bottom	// end Vertical address
	);

/******************************************************************************/
//  Description:  Set start RAM address which is write to AC(Address
//                Counter) register.
//  Input:
//      left: start Horizon address of AC
//      top: start Vertical address of AC.
//  Return:
//      None.
//	Note:           
/******************************************************************************/
__inline void SSD1288_set_start_address(
	uint16 left, 
	uint16 top
	);
/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

#define SSD1288_SEND_COMMAND(c)                                                             \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c000000)  = c;                 \
                                 }

#define SSD1288_SEND_DATA(d)                                                                \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c080000) = d;                  \
                                 }
 
#define SSD1288_SEND_COMDATA(c, d)                                                          \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c000000)  = c;                 \
                                     *(volatile uint16 *)(0x0c080000)  = d;                 \
                                 }



/*********************************************************************/
//  Description:   Initialize color LCD : SSD1288
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E SSD1288_Init(void)
{
    //############# void Power_Set(void) ################//
	SSD1288_SEND_COMDATA(0x0000,0x0001); /* start OSC */
	SCI_Sleep(10);
	SSD1288_SEND_COMDATA(0x0010,0x0000); /* leave sleep mode */
	SCI_Sleep(1);
	SSD1288_SEND_COMDATA(0x0007,0x0033); /* display control */
	SCI_Sleep(1);
	SSD1288_SEND_COMDATA(0x0003,0x0874); /* power control 1 */
	SSD1288_SEND_COMDATA(0x000D,0x000b); /* power control 2 VLCD63 adjustment */
	//Set_LCD_REG(0x000E,0x3000); /* power control 3 VCOML adjustment */	//add by jinxin 070730
	SSD1288_SEND_COMDATA(0x000E,0x3200); /* power control 3 VCOML adjustment */
	SCI_Sleep(10);
	SSD1288_SEND_COMDATA(0x001E,0x00b6); /* power control 4 VCOMH adjustment and OPT control *///b3
	SSD1288_SEND_COMDATA(0x0025,0x8000); /* Frame Freq seting */
	SSD1288_SEND_COMDATA(0x0002,0x0600); /* LCD-Driving-waveform control:line inversion */
	SSD1288_SEND_COMDATA(0x0011,0x6030); /* entry mode */
	SSD1288_SEND_COMDATA(0x0001,0x2adb); /* driver output control */
	SCI_Sleep(10);
	SSD1288_SEND_COMDATA(0x004E,0x0000); /* RAM address set(X address) */
	SSD1288_SEND_COMDATA(0x004F,0x0000); /* RAM address set(Y address) */ 
	SSD1288_SEND_COMDATA(0x000F,0x0000); /* Gate scan position */
	SSD1288_SEND_COMDATA(0x0016,0xaf08); /* Horizontal porch */
	SSD1288_SEND_COMDATA(0x0017,0x0002); /* Vertical porch */
	SSD1288_SEND_COMDATA(0x0044,0xaf00); /* Horizontal RAM address position */
	SSD1288_SEND_COMDATA(0x0045,0x0000); /* Vertical RAM address position(Star address) */
	SSD1288_SEND_COMDATA(0x0046,0x00db); /* Vertical RAM address position(End address) */
	/*===========Gammma Seting==============*/
	SSD1288_SEND_COMDATA(0x0030,0x0100);
	SSD1288_SEND_COMDATA(0x0031,0x0307);
	SSD1288_SEND_COMDATA(0x0032,0x0302);
	SSD1288_SEND_COMDATA(0x0033,0x0002);
	SSD1288_SEND_COMDATA(0x0034,0x0504);
	SSD1288_SEND_COMDATA(0x0035,0x0004);
	SSD1288_SEND_COMDATA(0x0036,0x0706);
	SSD1288_SEND_COMDATA(0x0037,0x0300);
	SSD1288_SEND_COMDATA(0x003A,0x0504);
	SSD1288_SEND_COMDATA(0x003B,0x0006);
	SSD1288_SEND_COMDATA(0x0007,0x0033);
	SCI_Sleep(10);
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E SSD1288_Invalidate(void)
{
        SSD1288_set_display_window (0x0, SSD1288_WIDTH - 1, 0x0, SSD1288_HEIGHT -1);
        SSD1288_set_start_address (0x0, 0x0);
        return ERR_LCD_NONE;
}
/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
__inline void SSD1288_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 right, 	// end Horizon address
	uint16 top, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{
	//SCI_TRACE_LOW:"SSD1288_set_display_window***********"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_SSD1228_199_112_2_18_0_34_13_1871,(uint8*)"");
	SSD1288_SEND_COMDATA(0x0044, (((right& 0x00FF) <<8) | (left & 0x00FF)/*0xAF00*/));
	SSD1288_SEND_COMDATA(0x0045, top);
	SSD1288_SEND_COMDATA(0x0046, bottom);
}


/******************************************************************************/
//  Description:  Set start RAM address which is write to AC(Address
//                Counter) register.
//  Input:
//      left: start Horizon address of AC
//      top: start Vertical address of AC.
//  Return:
//      None.
//	Note:           
/******************************************************************************/
__inline void SSD1288_set_start_address(
	uint16 left, 
	uint16 top
	)
{
	SSD1288_SEND_COMDATA(0x004e, left);	// Set start RAM address (AC register)
	SSD1288_SEND_COMDATA(0x004f, top);	// Set start RAM address (AC register)
	SSD1288_SEND_COMMAND(0x0022); 			// send data.	
}



/******************************************************************************/
//  Description:  Set LCD invalidate direction
//  Input:
//      is_invert: 0, horizontal; 1, vertical
//  Return:
//      None.
//	Note: Application should correct invalidate direction right after current
//	 	  image displayed
/******************************************************************************/
LOCAL ERR_LCD_E SSD1288_SetDirection(LCD_DIRECT_E direct_type)
{	
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
LOCAL ERR_LCD_E SSD1288_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{

	//SCI_TRACE_LOW:"SSD1288_InvalidateRect***********"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_SSD1228_254_112_2_18_0_34_13_1872,(uint8*)"");


	left = (left >= SSD1288_WIDTH) ? SSD1288_WIDTH - 1 : left;
	right = (right >= SSD1288_WIDTH) ? SSD1288_WIDTH - 1 : right;
	top = (top >= SSD1288_HEIGHT) ? SSD1288_HEIGHT - 1 : top;
	bottom = (bottom >= SSD1288_HEIGHT) ? SSD1288_HEIGHT - 1 : bottom;

	if ( ( right < left ) || ( bottom < top ) )
	{
		return ERR_LCD_PARAMETER_WRONG;
	}

	SSD1288_set_display_window(left, right, top, bottom);

	SSD1288_set_start_address(left, top);


	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  SSD1288_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"SSD1288_EnterSleep,%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_SSD1228_283_112_2_18_0_34_13_1873,(uint8*)"d", is_sleep);

	if (is_sleep)
	{
		//Set Sleep
		//Display OFF sequence
		SSD1288_SEND_COMDATA(0x0010,0x0001);
	}
	else
	{
		SSD1288_SEND_COMDATA(0x0010,0x0000);
		SCI_Sleep(200);
	}
	
	return ERR_LCD_NONE;
}


/*****************************************************************************/
//  Description:    Enable lcd to partial display mode, so can save power.
//	Global resource dependence: 
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//	Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E SSD1288_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{

	//SCI_TRACE_LOW:"SSD1288_SetDisplayWindow:%d,%d,%d,%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_SSD1228_314_112_2_18_0_34_13_1874,(uint8*)"dddd",left,top,right,bottom);

  	left 	= (left >= SSD1288_WIDTH)    ? SSD1288_WIDTH-1 : left;
	right 	= (right >= SSD1288_WIDTH)   ? SSD1288_WIDTH-1 : right;
	top 	= (top >= SSD1288_HEIGHT)    ? SSD1288_HEIGHT-1 : top;
	bottom 	= (bottom >= SSD1288_HEIGHT) ? SSD1288_HEIGHT-1 : bottom;

	if ( ( right < left ) || ( bottom < top ) )
	{
		//SCI_TRACE_LOW:" SSD1288_SetDisplayWindow wrong: right > left or bottom > top"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_SSD1228_323_112_2_18_0_34_13_1875,(uint8*)"");
		return ERR_LCD_PARAMETER_WRONG;
	}
 	 	
	SSD1288_set_display_window(left, right, top, bottom);

	SSD1288_set_start_address(left, top);

	return ERR_LCD_NONE;
}



/******************************************************************************/
//  Description:  set the contrast value 
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   SSD1288_SetContrast(
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
LOCAL ERR_LCD_E   SSD1288_SetBrightness(
	uint16 brightness	//birghtness to set
	)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void SSD1288_Close(void)
{
    SSD1288_EnterSleep( SCI_TRUE );	
}


LOCAL const LCD_OPERATIONS_T SSD1288_operations = 
{
	SSD1288_Init,
	SSD1288_EnterSleep,
	SSD1288_SetContrast,
	SSD1288_SetBrightness,
	SSD1288_SetDisplayWindow,	
	SSD1288_InvalidateRect,
	//SSD1288_InvalidateRect_Image,
	SSD1288_Invalidate,
	SSD1288_Close,
	NULL,
	SSD1288_SetDirection,
    NULL,
    NULL
};


#if 0
PUBLIC BOOLEAN SSD1288_Probe(void)
{
	return SCI_TRUE;	
}
#endif //for pclint

LOCAL const LCD_TIMING_U s_SSD1228_timing =
{

	        // LCM_CYCLE_U start(ns)
	30,		// CS setup time for LCM read (optional)  
	250,	// low pulse width for LCM read (according spec)	
	200,	// high pulse width for LCM read (according spec)
	30,		// CS setup time for LCM write  (optional) 	
	45,		// low pulse width for LCM write (according spec)
	15,	    // high pulse width for LCM write (according spec)	
	        // LCM_CYCLE_U end(ns)
};


const LCD_SPEC_T g_lcd_SSD1288[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U*)&s_SSD1228_timing, 
	(LCD_OPERATIONS_T*)&SSD1288_operations,
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
