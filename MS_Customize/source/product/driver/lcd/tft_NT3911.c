/******************************************************************************
 ** File Name:     di_NT3911.c                                             *
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

 #ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/


#define	NT3911_WIDTH	LCD_WIDTH
#define	NT3911_HEIGHT	LCD_HEIGHT

//static uint8 OFFSET_X = 2;
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;
					
  /******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void NT3911_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  NT3911_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);


/*********************************************************************/
//  Description:   Initialize color LCD : NT3911
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E NT3911_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E NT3911_Invalidate(void);

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
LOCAL ERR_LCD_E NT3911_InvalidateRect(
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
void NT3911_set_display_window(
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
void NT3911_set_start_address(
	uint16 left, 
	uint16 top
	);
/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

#define NT3911_SEND_COMMAND(c)                                                             \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c000000)  = c;                 \
                                 }
	
#define NT3911_SEND_DATA(d)                                                                \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c080000) = d;                  \
                                 }
 
#define NT3911_SEND_COMDATA(c, d)                                                          \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c000000)  = c;                 \
                                     *(volatile uint16 *)(0x0c080000)  = d;                 \
                                 }


/*********************************************************************/
//  Description:   Initialize color LCD : NT3911
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E NT3911_Init(void)
{
        NT3911_SEND_COMDATA(0x0000,0x0001);   //Start OSC
        SCI_Sleep(50);				/* delay 10ms */
        
        //	Power supply setting
        NT3911_SEND_COMDATA(0x0007,0x0000);
        SCI_Sleep(10);
        NT3911_SEND_COMDATA(0x000d,0x0908);        
        SCI_Sleep(10);
        NT3911_SEND_COMDATA(0x000e,0x2818);  
        SCI_Sleep(20);
        
        NT3911_SEND_COMDATA(0x0001,0x0113);  
        NT3911_SEND_COMDATA(0x0002,0x0700);  
        NT3911_SEND_COMDATA(0x0005,0x1030);  
        NT3911_SEND_COMDATA(0x0006,0x0000);  
        NT3911_SEND_COMDATA(0x000b,0x8400); 
        NT3911_SEND_COMDATA(0x000c,0x0002); 
        
        NT3911_SEND_COMDATA(0x0003,0x000C);      
        SCI_Sleep(5);
        NT3911_SEND_COMDATA(0x0004,0x8000);        
        NT3911_SEND_COMDATA(0x000e,0x2e17);
        SCI_Sleep(50);       
        
        NT3911_SEND_COMDATA(0x000d,0x0519);	
        SCI_Sleep(50);
        
        NT3911_SEND_COMDATA(0x0021,0x0000);	        
        NT3911_SEND_COMDATA(0x0030,0x0000);        
        NT3911_SEND_COMDATA(0x0031,0x0603);      
        NT3911_SEND_COMDATA(0x0032,0x0707);     
        NT3911_SEND_COMDATA(0x0033,0x0007);
        NT3911_SEND_COMDATA(0x0034,0x0004);
        NT3911_SEND_COMDATA(0x0035,0x0000);		
        NT3911_SEND_COMDATA(0x0036,0x0707);
        NT3911_SEND_COMDATA(0x0037,0x0304);
        NT3911_SEND_COMDATA(0x000f,0x0000);
        NT3911_SEND_COMDATA(0x0011,0x0000);	
        NT3911_SEND_COMDATA(0x0014,0x9f00);	
        NT3911_SEND_COMDATA(0x0015,0x0000);
        NT3911_SEND_COMDATA(0x0016,0x7f00);	
        NT3911_SEND_COMDATA(0x0017,0x9F00);
        NT3911_SEND_COMDATA(0x003a,0x1f00);
        NT3911_SEND_COMDATA(0x003b,0x1e00);
        SCI_Sleep(200);
        NT3911_SEND_COMDATA(0x0007,0x0005);
        SCI_Sleep(40);
        
        NT3911_SEND_COMDATA(0x0007,0x0025);
        NT3911_SEND_COMDATA(0x0007,0x0027);
        SCI_Sleep(40);
        NT3911_SEND_COMDATA(0x0007,0x0037);

		return ERR_LCD_NONE;
}



/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E NT3911_Invalidate(void)
{
        NT3911_set_display_window (0x0, NT3911_WIDTH - 1, 0x0, NT3911_HEIGHT -1);
        NT3911_set_start_address (0x0, 0x0);
        return ERR_LCD_NONE;
}


/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
__inline void NT3911_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 right, 	// end Horizon address
	uint16 top, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{
	 NT3911_SEND_COMDATA(0x0016, (((right& 0x00FF) <<8) | (left & 0x00FF)));
	 NT3911_SEND_COMDATA(0x0017, (((bottom& 0x00FF) <<8) | (top & 0x00FF)));
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
LOCAL ERR_LCD_E NT3911_SetDirection(
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
LOCAL ERR_LCD_E NT3911_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{
	left = (left >= NT3911_WIDTH) ? NT3911_WIDTH - 1 : left;
	right = (right >= NT3911_WIDTH) ? NT3911_WIDTH - 1 : right;
	top = (top >= NT3911_HEIGHT) ? NT3911_HEIGHT - 1 : top;
	bottom = (bottom >= NT3911_HEIGHT) ? NT3911_HEIGHT - 1 : bottom;

	if ( ( right < left ) || ( bottom < top ) )
	{
		return ERR_LCD_PARAMETER_WRONG;
	}

	NT3911_set_display_window(left, right, top, bottom);

	NT3911_set_start_address(left, top);

	return ERR_LCD_NONE;
}



/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  NT3911_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"NT3911_EnterSleep,%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_NT3911_275_112_2_18_0_33_59_1838,(uint8*)"d", is_sleep);

	if (is_sleep)
	{
		//Set Sleep
		//Display OFF sequence
            NT3911_SEND_COMDATA(0x0007,0x0027);
            SCI_Sleep(1);
            
            NT3911_SEND_COMDATA(0x0007,0x0025);
            SCI_Sleep(1);
            
            NT3911_SEND_COMDATA(0x0007,0x0005);
            SCI_Sleep(1);
            
            NT3911_SEND_COMDATA(0x0007,0x0000);
            SCI_Sleep(1);
            
            NT3911_SEND_COMDATA(0x0003,0x0012);
	}
	else
	{
            NT3911_SEND_COMDATA(0x000d,0x0908);
            SCI_Sleep(10);	
            
            NT3911_SEND_COMDATA(0x000e,0x2818);
            SCI_Sleep(20);		
            
            NT3911_SEND_COMDATA(0x000c,0x0003);
            
            NT3911_SEND_COMDATA(0x0003,0x000c);
            SCI_Sleep(5);		
            
            NT3911_SEND_COMDATA(0x000e,0x2e17);
            SCI_Sleep(50);		
            
            NT3911_SEND_COMDATA(0x000d,0x0819);
            SCI_Sleep(50);	
            
            NT3911_SEND_COMDATA(0x0007,0x0005);
            SCI_Sleep(1);	
            
            NT3911_SEND_COMDATA(0x0007,0x0025);
            SCI_Sleep(1);	
            
            NT3911_SEND_COMDATA(0x0007,0x0027);
            SCI_Sleep(1);
            
            NT3911_SEND_COMDATA(0x0007,0x0037);
            SCI_Sleep(1);
            
            NT3911_SEND_COMDATA(0x0016,0x7f00);
            
            NT3911_SEND_COMDATA(0x0017,0x9f00);
            
            NT3911_SEND_COMDATA(0x0021,0x0000);
            SCI_Sleep(5);	
            
            NT3911_SEND_COMMAND(0x0022);
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
LOCAL ERR_LCD_E NT3911_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	//SCI_TRACE_LOW:"NT3911_SetDisplayWindow:%d,%d,%d,%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_NT3911_353_112_2_18_0_33_59_1839,(uint8*)"dddd",left,top,right,bottom);

  	left 	= (left >= NT3911_WIDTH)    ? NT3911_WIDTH-1 : left;
	right 	= (right >= NT3911_WIDTH)   ? NT3911_WIDTH-1 : right;
	top 	= (top >= NT3911_HEIGHT)    ? NT3911_HEIGHT-1 : top;
	bottom 	= (bottom >= NT3911_HEIGHT) ? NT3911_HEIGHT-1 : bottom;

	if ( ( right < left ) || ( bottom < top ) )
	{
		//SCI_TRACE_LOW:" NT3911_SetDisplayWindow wrong: right > left or bottom > top"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_NT3911_362_112_2_18_0_33_59_1840,(uint8*)"");
		return ERR_LCD_PARAMETER_WRONG;
	}
 	 	
	NT3911_set_display_window(left, right, top, bottom);

	NT3911_set_start_address(left, top);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:  set the contrast value 
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   NT3911_SetContrast(
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
LOCAL ERR_LCD_E   NT3911_SetBrightness(
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
LOCAL void NT3911_Close(void)
{
    NT3911_EnterSleep( SCI_TRUE );	
}


LOCAL const LCD_OPERATIONS_T NT3911_operations = 
{
	NT3911_Init,
	NT3911_EnterSleep,
	NT3911_SetContrast,
	NT3911_SetBrightness,
	NT3911_SetDisplayWindow,
	NT3911_InvalidateRect,
	NT3911_Invalidate,
	NT3911_Close,
	NULL,
	NT3911_SetDirection,
    NULL,
    NULL
};



LOCAL const LCD_TIMING_U s_NT3911_timing =
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


const LCD_SPEC_T g_lcd_NT3911[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	&s_NT3911_timing, 
	(LCD_OPERATIONS_T*)&NT3911_operations,
	0,
	0
	}
};



/******************************************************************************/
//  Description:  return the NT3911 lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* NT3911_GetOperations()
{
	return &NT3911_operations;
}

PUBLIC BOOLEAN NT3911_Probe(void)
{
	return SCI_TRUE;	
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 
