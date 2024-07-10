/******************************************************************************
 ** File Name:     di_SPFD54124A.c                                             *
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


#define	SPFD54124A_WIDTH	       LCD_WIDTH
#define	SPFD54124A_HEIGHT	       LCD_HEIGHT

//static uint8 OFFSET_X = 2;




  /******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void SPFD54124A_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  SPFD54124A_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);



/*********************************************************************/
//  Description:   Initialize color LCD : SPFD54124A
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E SPFD54124A_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E SPFD54124A_Invalidate(void);

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
LOCAL ERR_LCD_E SPFD54124A_InvalidateRect(
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
__inline void SPFD54124A_set_display_window(
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
__inline void SPFD54124A_set_start_address(
	uint16 left, 
	uint16 top
	);
/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

#define SPFD54124A_SEND_COMMAND(c)                                                             \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c000000)  = c;                 \
                                 }
	
#define SPFD54124A_SEND_DATA(d)                                                                \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c080000) = d;                  \
                                 }
 
#define SPFD54124A_SEND_COMDATA(c, d)                                                          \
                                 {                                                          \
                                     *(volatile uint16 *)(0x0c000000)  = c;                 \
                                     *(volatile uint16 *)(0x0c080000)  = d;                 \
                                 }





/*********************************************************************/
//  Description:   Initialize color LCD : SPFD54124A
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E SPFD54124A_Init(void)
{
         SPFD54124A_SEND_COMMAND(0x01);                  //software reset 
         SCI_Sleep(1);                    // delay10ms
         SPFD54124A_SEND_COMMAND(0x11);                  //sleep out/power on(SLPOUT)
         SCI_Sleep(2);            // delay50ms
         
        
         SPFD54124A_SEND_COMMAND(0xc0);                 //power control 1      (PWCTR1)  
         SPFD54124A_SEND_DATA(0x07);            //default value  LCM=0 set t)e GVDD voltage=4.65   
         SPFD54124A_SEND_DATA(0x05);  
          
         SPFD54124A_SEND_COMMAND(0xc5);
         SPFD54124A_SEND_DATA(0xb4);            //VCOM) voltage set 4.300V  AGO IS C8H
                          
         SPFD54124A_SEND_COMMAND(0xc6);      
         SPFD54124A_SEND_DATA(0x0b);          //VCOMAC voltage set 5.550V // AGO IS 1FH      0x0010
    
               
         SPFD54124A_SEND_COMMAND(0xe0);                 //Gamma + Polarity correction c)aracteristics set     
         SPFD54124A_SEND_DATA(0x00);                  //Gamma    V1=       V0=        
         SPFD54124A_SEND_DATA(0x00);                  //Gamma    V3=       V2=         
         SPFD54124A_SEND_DATA(0x00);                  //Gamma    V5=       V4=         
         SPFD54124A_SEND_DATA(0x2e);                  //Gamma    V7=       V6=         
         SPFD54124A_SEND_DATA(0x2c);                  //Gamma    V9=       V8=        
         SPFD54124A_SEND_DATA(0x05);                  //Gamma    V11=       V10=       
         SPFD54124A_SEND_DATA(0x1f);                  //Gamma    V13=       V12=  
         SPFD54124A_SEND_DATA(0x02);                  //Gamma    V1=       V0=        
         SPFD54124A_SEND_DATA(0x00);                  //Gamma    V3=       V2=         
         SPFD54124A_SEND_DATA(0x06);                  //Gamma    V5=       V4=         
         SPFD54124A_SEND_DATA(0x0f);                  //Gamma    V7=       V6=         
         SPFD54124A_SEND_DATA(0x0e);                  //Gamma    V9=       V8=        
         SPFD54124A_SEND_DATA(0x01);                  //Gamma    V11=       V10=       
         SPFD54124A_SEND_DATA(0x05); 
         SPFD54124A_SEND_DATA(0x05);                  //Gamma    V11=       V10=       
         SPFD54124A_SEND_DATA(0x08);                  
        
         SPFD54124A_SEND_COMMAND(0xe1);                   //Gamma + negedgtive correction c)aracteristics set          
         SPFD54124A_SEND_DATA(0x00);                  //Gamma    V1=       V0=        
         SPFD54124A_SEND_DATA(0x22);                  //Gamma    V3=       V2=         
         SPFD54124A_SEND_DATA(0x24);                  //Gamma    V5=       V4=         
         SPFD54124A_SEND_DATA(0x0a);                  //Gamma    V7=       V6=         
         SPFD54124A_SEND_DATA(0x0a);                  //Gamma    V9=       V8=        
         SPFD54124A_SEND_DATA(0x05);                  //Gamma    V11=       V10=       
         SPFD54124A_SEND_DATA(0x02);                  //Gamma    V13=       V12=  
         SPFD54124A_SEND_DATA(0x1f);                  //Gamma    V1=       V0=        
         SPFD54124A_SEND_DATA(0x08);                  //Gamma    V3=       V2=         
         SPFD54124A_SEND_DATA(0x05);                  //Gamma    V5=       V4=         
         SPFD54124A_SEND_DATA(0x05);                  //Gamma    V7=       V6=         
         SPFD54124A_SEND_DATA(0x01);                  //Gamma    V9=       V8=        
         SPFD54124A_SEND_DATA(0x0e);                  //Gamma    V11=       V10=       
         SPFD54124A_SEND_DATA(0x0f); 
         SPFD54124A_SEND_DATA(0x06);                  //Gamma    V11=       V10=       
         SPFD54124A_SEND_DATA(0x00); 
        
         SPFD54124A_SEND_COMMAND(0x3a);              //color mode Interface pixel format  (COLMOD)  
         SPFD54124A_SEND_DATA(0x05);            //16-bit/pixel , 1-times data transfer
        
         SCI_Sleep(1);
         SPFD54124A_SEND_COMMAND(0x36);                  //memory data access control  (MADCTR)       
         SPFD54124A_SEND_DATA(0x00);                  //MY=0// MX=0// MV=0// ML=1// RGB=0
       

         SPFD54124A_SEND_COMMAND(0x29);                       //display on

		 return ERR_LCD_NONE;
}



/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E SPFD54124A_Invalidate(void)
{

	        
        SPFD54124A_set_display_window (0x0, SPFD54124A_WIDTH - 1, 0x0, SPFD54124A_HEIGHT -1);

        SPFD54124A_set_start_address (0x0, 0x0);
                

        
        return ERR_LCD_NONE;
}


/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
__inline void SPFD54124A_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 right, 	// end Horizon address
	uint16 top, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{
	 SPFD54124A_SEND_COMMAND(0x2A);
	 SPFD54124A_SEND_DATA(0x00);
	 SPFD54124A_SEND_DATA(right);
	 SPFD54124A_SEND_DATA(0x00);
	 SPFD54124A_SEND_DATA(left);

	 SPFD54124A_SEND_COMMAND(0x2B);
	 SPFD54124A_SEND_DATA(0x00);
	 SPFD54124A_SEND_DATA(bottom);
	 SPFD54124A_SEND_DATA(0x00);
	 SPFD54124A_SEND_DATA(top);	 
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
__inline void SPFD54124A_set_start_address(
	uint16 left, 
	uint16 top
	)
{
        SPFD54124A_SEND_COMMAND(0x2c);
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
LOCAL ERR_LCD_E SPFD54124A_SetDirection(
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
LOCAL ERR_LCD_E SPFD54124A_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{
	left = (left >= SPFD54124A_WIDTH) ? SPFD54124A_WIDTH - 1 : left;
	right = (right >= SPFD54124A_WIDTH) ? SPFD54124A_WIDTH - 1 : right;
	top = (top >= SPFD54124A_HEIGHT) ? SPFD54124A_HEIGHT - 1 : top;
	bottom = (bottom >= SPFD54124A_HEIGHT) ? SPFD54124A_HEIGHT - 1 : bottom;

	if ( ( right < left ) || ( bottom < top ) )
	{
		return ERR_LCD_PARAMETER_WRONG;
	}

	SPFD54124A_set_display_window(left, right, top, bottom);

	SPFD54124A_set_start_address(left, top);

	

	return ERR_LCD_NONE;
}
/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  SPFD54124A_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"SPFD54124A_EnterSleep,%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_SPFD54124A_325_112_2_18_0_34_10_1863,(uint8*)"d", is_sleep);

	if (is_sleep)
	{
		//Set Sleep
            SPFD54124A_SEND_COMMAND(0x28);                //display off
            SCI_Sleep(10);
            
            SPFD54124A_SEND_COMMAND(0x10);                //sleep in
            SCI_Sleep(20);                   //20ms
	}
	else
	{
            SPFD54124A_SEND_COMMAND(0x11);                //sleep out
            SCI_Sleep(100);
			
            SPFD54124A_SEND_COMMAND(0x29);                //display on
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
LOCAL ERR_LCD_E SPFD54124A_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	//SCI_TRACE_LOW:"SPFD54124A_SetDisplayWindow:%d,%d,%d,%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_SPFD54124A_361_112_2_18_0_34_10_1864,(uint8*)"dddd",left,top,right,bottom);

  	left 	= (left >= SPFD54124A_WIDTH)    ? SPFD54124A_WIDTH-1 : left;
	right 	= (right >= SPFD54124A_WIDTH)   ? SPFD54124A_WIDTH-1 : right;
	top 	= (top >= SPFD54124A_HEIGHT)    ? SPFD54124A_HEIGHT-1 : top;
	bottom 	= (bottom >= SPFD54124A_HEIGHT) ? SPFD54124A_HEIGHT-1 : bottom;

	if ( ( right < left ) || ( bottom < top ) )
	{
		//SCI_TRACE_LOW:" SPFD54124A_SetDisplayWindow wrong: right > left or bottom > top"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_SPFD54124A_370_112_2_18_0_34_10_1865,(uint8*)"");
		return ERR_LCD_PARAMETER_WRONG;
	}
 	 	
	SPFD54124A_set_display_window(left, right, top, bottom);

	SPFD54124A_set_start_address(left, top);

	return ERR_LCD_NONE;
}




/******************************************************************************/
//  Description:  set the contrast value 
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   SPFD54124A_SetContrast(
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
LOCAL ERR_LCD_E   SPFD54124A_SetBrightness(
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
LOCAL void SPFD54124A_Close(void)
{
    SPFD54124A_EnterSleep( SCI_TRUE );	
}


LOCAL const LCD_OPERATIONS_T SPFD54124A_operations = 
{
	SPFD54124A_Init,
	SPFD54124A_EnterSleep,
	SPFD54124A_SetContrast,
	SPFD54124A_SetBrightness,
	SPFD54124A_SetDisplayWindow,
	SPFD54124A_InvalidateRect,
	SPFD54124A_Invalidate,
	SPFD54124A_Close,
	NULL,	
	SPFD54124A_SetDirection,
    NULL,
    NULL
	};

/******************************************************************************/
//  Description:  return the SPFD54124A lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* SPFD54124A_GetOperations()
{
	return (LCD_OPERATIONS_T*)&SPFD54124A_operations;
}

PUBLIC BOOLEAN SPFD54124A_Probe(void)
{
	return SCI_TRUE;	
}

LOCAL const LCD_TIMING_U s_SPFD54124A_timing =
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


const LCD_SPEC_T g_lcd_SPFD54124A[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U*)&s_SPFD54124A_timing, 
	(LCD_OPERATIONS_T*)&SPFD54124A_operations,
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

