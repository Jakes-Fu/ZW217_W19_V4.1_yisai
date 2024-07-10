/******************************************************************************
 ** File Name:     di_R61505U.c                                             *
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


#define	R61505U_WIDTH	LCD_WIDTH
#define	R61505U_HEIGHT	LCD_HEIGHT
						
  /******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void R61505U_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  R61505U_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);

/*********************************************************************/
//  Description:   Initialize color LCD : R61505U
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E R61505U_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E R61505U_Invalidate(void);

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
LOCAL ERR_LCD_E R61505U_InvalidateRect(
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
__inline void R61505U_set_display_window(
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
__inline void R61505U_set_start_address(
	uint16 left, 
	uint16 top
	);
/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

#define R61505U_SEND_COMMAND(c)                                                             \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c000000)  = c;                 \
                                 }

#define R61505U_SEND_DATA(d)                                                                \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c080000) = d;                  \
                                 }

#define R61505U_SEND_COMDATA(c, d)                                                          \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c000000)  = c;                 \
                                     *(volatile uint16 *)(0x0c080000)  = d;                 \
                                 }

/*********************************************************************/
//  Description:   Initialize color LCD : R61505U
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E R61505U_Init(void)
{
//CMO Initial code
 //===User setting===
      R61505U_SEND_COMDATA(0x0000,0x0000);
      R61505U_SEND_COMDATA(0x00A4,0x0001);
      R61505U_SEND_COMDATA(0x0001,0x0100);
      R61505U_SEND_COMDATA(0x0002,0x0500);//----CLYCE==2.4-F02411-01U
      R61505U_SEND_COMDATA(0x0003,0x1030);//----RGB==2.4-F02411-01U
      R61505U_SEND_COMDATA(0x0004,0x0000);
      R61505U_SEND_COMDATA(0x0008,0x0404);
      R61505U_SEND_COMDATA(0x0009,0x0000);
      R61505U_SEND_COMDATA(0x000A,0x0008);
      R61505U_SEND_COMDATA(0x000D,0x0000);
      R61505U_SEND_COMDATA(0x000F,0x0002);
 
 //===Other mode setting Instruction===
      //===Base image display control===
      R61505U_SEND_COMDATA(0x0060,0x2700);
      R61505U_SEND_COMDATA(0x0061,0x0001);
      R61505U_SEND_COMDATA(0x006A,0x0000);
      //===Partial display control===
      R61505U_SEND_COMDATA(0x0080,0x0000);
      R61505U_SEND_COMDATA(0x0081,0x0000);
      R61505U_SEND_COMDATA(0x0082,0x013F);
      R61505U_SEND_COMDATA(0x0083,0x0000);
      R61505U_SEND_COMDATA(0x0084,0x0000);
      R61505U_SEND_COMDATA(0x0085,0x0000);
      //Panel interface control===
      R61505U_SEND_COMDATA(0x0090,0x0010);//---syn--2.4-F02411-01U
      R61505U_SEND_COMDATA(0x0092,0x0200);
      R61505U_SEND_COMDATA(0x0093,0x0002);
      R61505U_SEND_COMDATA(0x0095,0x0113);
      R61505U_SEND_COMDATA(0x0097,0x0400);
      R61505U_SEND_COMDATA(0x0098,0x0004);
 
 //===Gamma setting===
      R61505U_SEND_COMDATA(0x0030,0x0707);//P0KP1 & P0KP0
      R61505U_SEND_COMDATA(0x0031,0x0607);//P0KP3 & P0KP2
      R61505U_SEND_COMDATA(0x0032,0x0301);//P0KP5 & P0KP4
      R61505U_SEND_COMDATA(0x0033,0x0303);//P0FP1 & P0FP0
      R61505U_SEND_COMDATA(0x0034,0x0303);//P0FP3 & P0FP2
      R61505U_SEND_COMDATA(0x0035,0x0107);//P0RP1 & P0RP0
      R61505U_SEND_COMDATA(0x0036,0x021F);//V0RP1 & V0RP0
      R61505U_SEND_COMDATA(0x0037,0x0705);//P0KN1 & P0KN0
      R61505U_SEND_COMDATA(0x0038,0x0607);//P0KN3 & P0KN2
      R61505U_SEND_COMDATA(0x0039,0x0301);//P0KN5 & P0KN4
      R61505U_SEND_COMDATA(0x003A,0x0303);//P0FN1 & P0FN0
      R61505U_SEND_COMDATA(0x003B,0x0202);//P0FN3 & P0FN2
      R61505U_SEND_COMDATA(0x003C,0x0104);//P0RN1 & P0RN0
      R61505U_SEND_COMDATA(0x003D,0x021F);//V0RN1 & V0RN0
 
 //===Power on sequence===
      R61505U_SEND_COMDATA(0x0007,0x0001);
      R61505U_SEND_COMDATA(0x0017,0x0001);
      R61505U_SEND_COMDATA(0x0010,0x16B0);
      R61505U_SEND_COMDATA(0x0011,0x0007);
      R61505U_SEND_COMDATA(0x0012,0x0118);
      R61505U_SEND_COMDATA(0x0013,0x1A00);
      R61505U_SEND_COMDATA(0x002A,0x0000);
      R61505U_SEND_COMDATA(0x0029,0x0007);
      R61505U_SEND_COMDATA(0x0012,0x0138);
 
 //===RAM start address===
      R61505U_SEND_COMDATA(0x0050,0x0000);
      R61505U_SEND_COMDATA(0x0051,0x00EF);
      R61505U_SEND_COMDATA(0x0052,0x0000);
      R61505U_SEND_COMDATA(0x0053,0x013F);
      SCI_Sleep(40);
 
 //===Display_On_Function===
      R61505U_SEND_COMDATA(0x0007,0x0021);
      SCI_Sleep(40);
      R61505U_SEND_COMDATA(0x0007,0x0061);
      SCI_Sleep(80);
      R61505U_SEND_COMDATA(0x0007,0x0173);

	// clear gram data
	R61505U_SEND_COMDATA(0x0020, 0x0000);         // GRAM horizontal Address
	R61505U_SEND_COMDATA(0x0021, 0x0000);         // GRAM Vertical Address

	return ERR_LCD_NONE;
}




/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E R61505U_Invalidate(void)
{
        R61505U_set_display_window (0x0, R61505U_WIDTH - 1, 0x0, R61505U_HEIGHT -1);
        
        R61505U_set_start_address (0x0, 0x0);

        return ERR_LCD_NONE;
}



/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
__inline void R61505U_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 right, 	// end Horizon address
	uint16 top, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{
	R61505U_SEND_COMDATA(0x0050, left);		// Horizontal GRAM Start Address-----HSA[7:0]
	R61505U_SEND_COMDATA(0x0051, right);		// Horizontal GRAM End Address-----HEA[7:0]
	R61505U_SEND_COMDATA(0x0052, top);		// Vertical GRAM Start Address-----VSA[8:0]
	R61505U_SEND_COMDATA(0x0053, bottom);		// Vertical GRAM Start Address-----VEA[8:0]
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
__inline void R61505U_set_start_address(
	uint16 left, 
	uint16 top
	)
{
	R61505U_SEND_COMDATA(0x0020, left);	// Set start RAM address (AC register)
	R61505U_SEND_COMDATA(0x0021, top);	// Set start RAM address (AC register)
	R61505U_SEND_COMMAND(0x0022); 			// send data.	
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
LOCAL ERR_LCD_E R61505U_SetDirection(
	LCD_DIRECT_E is_invert
	)
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
LOCAL ERR_LCD_E R61505U_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{
	left = (left >= R61505U_WIDTH) ? R61505U_WIDTH - 1 : left;
	right = (right >= R61505U_WIDTH) ? R61505U_WIDTH - 1 : right;
	top = (top >= R61505U_HEIGHT) ? R61505U_HEIGHT - 1 : top;
	bottom = (bottom >= R61505U_HEIGHT) ? R61505U_HEIGHT - 1 : bottom;

	if ( ( right < left ) || ( bottom < top ) )
	{
		return ERR_LCD_PARAMETER_WRONG;
	}

	R61505U_set_display_window(left, right, top, bottom);

	R61505U_set_start_address(left, top);

	return ERR_LCD_NONE;
}




/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  R61505U_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"R61505U_EnterSleep,%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_R61505U_325_112_2_18_0_34_1_1844,(uint8*)"d", is_sleep);

	if (is_sleep)
	{
		//Set Sleep
		//Display OFF sequence
                   R61505U_SEND_COMDATA(0x0007,0x0072);
                   R61505U_SEND_COMDATA(0x0007,0x0001);
                   R61505U_SEND_COMDATA(0x0007,0x0000);
                   R61505U_SEND_COMDATA(0x0010,0x0000);
                   R61505U_SEND_COMDATA(0x0011,0x0060);
                   R61505U_SEND_COMDATA(0x0012,0x0000);
                   R61505U_SEND_COMDATA(0x0013,0x1500);
                   R61505U_SEND_COMDATA(0x0010,0x0002);		
	}
	else
	{
                   R61505U_SEND_COMDATA(0x0010,0x0500);//slp=0
                   SCI_Sleep(5);
                   
                   R61505U_SEND_COMDATA(0x0007,0x0031);
                   R61505U_SEND_COMDATA(0x0017,0x0001);
                   R61505U_SEND_COMDATA(0x0010,0x16B0);
                   R61505U_SEND_COMDATA(0x0011,0x0111);
                   SCI_Sleep(3);
                   
                   R61505U_SEND_COMDATA(0x0012,0x0118);
                   SCI_Sleep(10);
                   
                   R61505U_SEND_COMDATA(0x0013,0x0000);//0x1400
                   R61505U_SEND_COMDATA(0x0029,0x000c);    
                   R61505U_SEND_COMDATA(0x0012,0x01b8);
                   SCI_Sleep(3);
                   
                   //####display on####//
                   R61505U_SEND_COMDATA(0x0007,0x0021);
                   R61505U_SEND_COMDATA(0x0012,0x0118); 
                   SCI_Sleep(2);
                   
                   R61505U_SEND_COMDATA(0x0007,0x0061);
                   R61505U_SEND_COMDATA(0x0012,0x01b8);//
                   SCI_Sleep(2);
                   
                   R61505U_SEND_COMDATA(0x0007,0x0173);
                   SCI_Sleep(5);
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
LOCAL ERR_LCD_E R61505U_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{


	//SCI_TRACE_LOW:"R61505U_SetDisplayWindow:%d,%d,%d,%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_R61505U_390_112_2_18_0_34_1_1845,(uint8*)"dddd",left,top,right,bottom);

  	left 	= (left >= R61505U_WIDTH)    ? R61505U_WIDTH-1 : left;
	right 	= (right >= R61505U_WIDTH)   ? R61505U_WIDTH-1 : right;
	top 	= (top >= R61505U_HEIGHT)    ? R61505U_HEIGHT-1 : top;
	bottom 	= (bottom >= R61505U_HEIGHT) ? R61505U_HEIGHT-1 : bottom;

	if ( ( right < left ) || ( bottom < top ) )
	{
		//SCI_TRACE_LOW:" R61505U_SetDisplayWindow wrong: right > left or bottom > top"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_R61505U_399_112_2_18_0_34_1_1846,(uint8*)"");
		return ERR_LCD_PARAMETER_WRONG;
	}
 	 	
	R61505U_set_display_window(left, right, top, bottom);

	R61505U_set_start_address(left, top);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:  set the contrast value 
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   R61505U_SetContrast(
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
LOCAL ERR_LCD_E   R61505U_SetBrightness(
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
LOCAL void R61505U_Close(void)
{
    R61505U_EnterSleep( SCI_TRUE );	
}


LOCAL const LCD_OPERATIONS_T R61505U_operations = 
{
	R61505U_Init,
	R61505U_EnterSleep,
	R61505U_SetContrast,
	R61505U_SetBrightness,
	R61505U_SetDisplayWindow,
	R61505U_InvalidateRect,
	//R61505U_InvalidateRect_Image,
	R61505U_Invalidate,
	R61505U_Close,
	NULL,
	R61505U_SetDirection,
    NULL,
    NULL
};

/******************************************************************************/
//  Description:  return the R61505U lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* R61505U_GetOperations()
{
	return (LCD_OPERATIONS_T*)&R61505U_operations;
}

PUBLIC BOOLEAN R61505U_Probe(void)
{
	return SCI_TRUE;	
}

LOCAL const LCD_TIMING_U s_R61505U_timing =
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




const LCD_SPEC_T g_lcd_R61505U[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U*)&s_R61505U_timing, 
	(LCD_OPERATIONS_T*)&R61505U_operations,
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
