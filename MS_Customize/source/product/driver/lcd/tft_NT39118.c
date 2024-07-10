/******************************************************************************
 ** File Name:     di_NT39118.c                                             *
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
#define	NT39118_WIDTH	LCD_WIDTH
#define	NT39118_HEIGHT	LCD_HEIGHT
			
/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void NT39118_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  NT39118_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);


/*********************************************************************/
//  Description:   Initialize color LCD : NT39118
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E NT39118_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E NT39118_Invalidate(void);

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
LOCAL ERR_LCD_E NT39118_InvalidateRect(
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
void NT39118_set_display_window(
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
void NT39118_set_start_address(
	uint16 left, 
	uint16 top
	);
/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

#define NT39118_SEND_COMMAND(c)                                                             \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c000000)  = c;                 \
                                 }
	
#define NT39118_SEND_DATA(d)                                                                \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c080000) = d;                  \
                                 }
 
#define NT39118_SEND_COMDATA(c, d)                                                          \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c000000)  = c;                 \
                                     *(volatile uint16 *)(0x0c080000)  = d;                 \
                                 }

/*********************************************************************/
//  Description:   Initialize color LCD : NT39118
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E NT39118_Init(void)
{
        /*===========Initial setting for NT39118==============*/
        NT39118_SEND_COMMAND(0x11);//sleep out	boost on
        SCI_Sleep(50);
        
        NT39118_SEND_COMMAND(0x3A);//interfac pixle format
        NT39118_SEND_DATA(0x05); //16bit/pixle
        
        NT39118_SEND_COMMAND(0xC5);//VCOM control
        NT39118_SEND_DATA(0x40);//Para. 1 Vcom-H //  44>>+3.6v
        NT39118_SEND_DATA(0x42);//Para. 2 Vcom-L //  4C>>-0.55v    Peak-Peak 5.32V
        
        // PWR 1 Control 
        NT39118_SEND_COMMAND(0xC0);//
        NT39118_SEND_DATA(0x0D);// GVDD=4.15V // VCI1=2.5V
        NT39118_SEND_DATA(0x05); 
        SCI_Sleep(50);
        NT39118_SEND_COMMAND(0xC1);//
        NT39118_SEND_DATA(0x03);
        SCI_Sleep(50);
        NT39118_SEND_COMMAND(0xC2);//
        NT39118_SEND_DATA(0x04);
        NT39118_SEND_DATA(0x05); 
        SCI_Sleep(50);
        
        // Gammut control Enable
        NT39118_SEND_COMMAND(0xF2);// Gammut control enable: /disable 0
        NT39118_SEND_DATA(0x01);// enable:1 /disable 0
        
        NT39118_SEND_COMMAND(0xE0);// Positive
        NT39118_SEND_DATA(0x07);
        NT39118_SEND_DATA(0x12);
        NT39118_SEND_DATA(0x14);
        NT39118_SEND_DATA(0x06);
        NT39118_SEND_DATA(0x0d);
        NT39118_SEND_DATA(0x11);
        NT39118_SEND_DATA(0x15);
        NT39118_SEND_DATA(0x7a);
        NT39118_SEND_DATA(0x22);
        NT39118_SEND_DATA(0x0f);
        NT39118_SEND_DATA(0x22);
        NT39118_SEND_DATA(0x27);
        NT39118_SEND_DATA(0x16);
        NT39118_SEND_DATA(0x18);
        NT39118_SEND_DATA(0x33);
        
        
        
        NT39118_SEND_COMMAND(0xE1);// Negative
        NT39118_SEND_DATA(0x0c);
        NT39118_SEND_DATA(0x27);
        NT39118_SEND_DATA(0x29);
        NT39118_SEND_DATA(0x08);//+4
        NT39118_SEND_DATA(0x0d);//+5
        NT39118_SEND_DATA(0x0f);//+6
        NT39118_SEND_DATA(0x1d);//+7
        NT39118_SEND_DATA(0x58);//+8
        NT39118_SEND_DATA(0x2a);
        NT39118_SEND_DATA(0x0d);
        NT39118_SEND_DATA(0x22);
        NT39118_SEND_DATA(0x29);
        NT39118_SEND_DATA(0x2b);
        NT39118_SEND_DATA(0x2d);
        NT39118_SEND_DATA(0x38);
           
        NT39118_SEND_COMMAND(0x29);  //display on  
        NT39118_SEND_COMMAND(0x2c); 
        SCI_Sleep(50);       
		return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E NT39118_Invalidate(void)
{
	NT39118_set_display_window (0x0, NT39118_WIDTH - 1, 0x0, NT39118_HEIGHT -1);
	NT39118_set_start_address (0x0, 0x0);
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
__inline void NT39118_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 right, 	// end Horizon address
	uint16 top, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{
	 NT39118_SEND_COMMAND(0x2A);
	 NT39118_SEND_DATA(0x00);
	 NT39118_SEND_DATA(right);
	 NT39118_SEND_DATA(0x00);
	 NT39118_SEND_DATA(left);

	 NT39118_SEND_COMMAND(0x2B);
	 NT39118_SEND_DATA(0x00);
	 NT39118_SEND_DATA(bottom);
	 NT39118_SEND_DATA(0x00);
	 NT39118_SEND_DATA(top);	 
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
LOCAL ERR_LCD_E NT39118_SetDirection(
	LCD_DIRECT_E direct_type
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
LOCAL ERR_LCD_E NT39118_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{
	left = (left >= NT39118_WIDTH) ? NT39118_WIDTH - 1 : left;
	right = (right >= NT39118_WIDTH) ? NT39118_WIDTH - 1 : right;
	top = (top >= NT39118_HEIGHT) ? NT39118_HEIGHT - 1 : top;
	bottom = (bottom >= NT39118_HEIGHT) ? NT39118_HEIGHT - 1 : bottom;

	if ( ( right < left ) || ( bottom < top ) )
	{
		return ERR_LCD_PARAMETER_WRONG;
	}

	NT39118_set_display_window(left, right, top, bottom);

	NT39118_set_start_address(left, top);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  NT39118_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"NT39118_EnterSleep,%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_NT39118_286_112_2_18_0_34_0_1841,(uint8*)"d", is_sleep);

	if (is_sleep)
	{
		//Set Sleep
            NT39118_SEND_COMMAND(0x28);                //display off
            SCI_Sleep(10);
            
            NT39118_SEND_COMMAND(0x10);                //sleep in
            SCI_Sleep(20);                   //20ms
	}
	else
	{
            NT39118_SEND_COMMAND(0x11);                //sleep out
            SCI_Sleep(100);
			
            NT39118_SEND_COMMAND(0x29);                //display on
            SCI_Sleep(10);                   //10ms
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
LOCAL ERR_LCD_E NT39118_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{


	//SCI_TRACE_LOW:"NT39118_SetDisplayWindow:%d,%d,%d,%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_NT39118_324_112_2_18_0_34_0_1842,(uint8*)"dddd",left,top,right,bottom);

  	left 	= (left >= NT39118_WIDTH)    ? NT39118_WIDTH-1 : left;
	right 	= (right >= NT39118_WIDTH)   ? NT39118_WIDTH-1 : right;
	top 	= (top >= NT39118_HEIGHT)    ? NT39118_HEIGHT-1 : top;
	bottom 	= (bottom >= NT39118_HEIGHT) ? NT39118_HEIGHT-1 : bottom;

	if ( ( right < left ) || ( bottom < top ) )
	{
		//SCI_TRACE_LOW:" NT39118_SetDisplayWindow wrong: right > left or bottom > top"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_NT39118_333_112_2_18_0_34_0_1843,(uint8*)"");
		return ERR_LCD_PARAMETER_WRONG;
	}
 	 	
	NT39118_set_display_window(left, right, top, bottom);

	NT39118_set_start_address(left, top);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:  set the contrast value 
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   NT39118_SetContrast(
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
LOCAL ERR_LCD_E   NT39118_SetBrightness(
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
LOCAL void NT39118_Close(void)
{
    NT39118_EnterSleep( SCI_TRUE );	
}


LOCAL const LCD_OPERATIONS_T NT39118_operations = 
{
	NT39118_Init,
	NT39118_EnterSleep,
	NT39118_SetContrast,
	NT39118_SetBrightness,
	NT39118_SetDisplayWindow,
	NT39118_InvalidateRect,
//	NT39118_InvalidateRect_Image,
	NT39118_Invalidate,
	NT39118_Close,
	NULL,
	NT39118_SetDirection,
    NULL,
    NULL
	
};

/******************************************************************************/
//  Description:  return the NT39118 lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* NT39118_GetOperations()
{
	return &NT39118_operations;
}

PUBLIC BOOLEAN NT39118_Probe(void)
{
	return SCI_TRUE;	
}


LOCAL const LCD_TIMING_U s_NT39118_timing =
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


const LCD_SPEC_T g_lcd_NT39118[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	&s_NT39118_timing, 
	(LCD_OPERATIONS_T*)&NT39118_operations,
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
