/******************************************************************************
 ** File Name:     TFT_RM68040.c                                               *
 ** Description:                                                              *
 **    This file contains driver for color LCD which controler IC is RM68040   *
 ** Author:         Haifeng.Yang                                              *
 ** DATE:           07/06/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 **   DATE           NAME                      DESCRIPTION                    *
 ** 07/06/2010     Haifeng.Yang                   Create.                     *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "os_api.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C" 
    {
#endif

#define LCD_PRINT   SCI_TRACE_LOW

/**---------------------------------------------------------------------------*
 **                            Macro Define                                   *
 **---------------------------------------------------------------------------*/
#define LCD_CtrlWrite_RM68040(_cmd)       LCD_SendCmd((_cmd ), 0)
#define LCD_DataWrite_RM68040(_data)      LCD_SendData((_data), 0)
#define LCD_CtrlData_RM68040(_cmd, _data) LCD_SendCmdData((_cmd ), (_data), 0)

/**---------------------------------------------------------------------------*
 **                         Local Function Prototype                          *
 **---------------------------------------------------------------------------*/
LOCAL ERR_LCD_E RM68040_Init(void);
LOCAL ERR_LCD_E RM68040_EnterSleep(BOOLEAN is_sleep);
LOCAL ERR_LCD_E RM68040_SetContrast(uint16 contrast);
LOCAL ERR_LCD_E RM68040_SetBrightness(uint16 brightness);
LOCAL ERR_LCD_E RM68040_SetDisplayWindow(uint16 left, uint16 top, uint16 right, 
    uint16 bottom);
LOCAL ERR_LCD_E RM68040_InvalidateRect(uint16 left, uint16 top, uint16 right, 
    uint16 bottom);
LOCAL ERR_LCD_E RM68040_Invalidate(void);
LOCAL void RM68040_Close(void);
LOCAL ERR_LCD_E RM68040_RotationInvalidateRect(uint16 left,uint16 top,
    uint16 right,uint16 bottom,LCD_ANGLE_E angle);
LOCAL ERR_LCD_E RM68040_SetDirection(LCD_DIRECT_E direct_type);
/**---------------------------------------------------------------------------*
 **                         Data & Function Prototype                         *
 **---------------------------------------------------------------------------*/
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;
LOCAL BOOLEAN      s_is_invert  = SCI_FALSE;
//OPERATIOS TABLE
LOCAL const LCD_OPERATIONS_T s_RM68040_ops_tab = 
{
    RM68040_Init,
    RM68040_EnterSleep,
    RM68040_SetContrast,
    RM68040_SetBrightness,
    RM68040_SetDisplayWindow,
    RM68040_InvalidateRect,
    RM68040_Invalidate,
    RM68040_Close,
    RM68040_RotationInvalidateRect,
    RM68040_SetDirection,
    NULL,
    NULL
};

//TIME TABLE
LOCAL const LCD_TIMING_U s_RM68040_timing =
{

	        // LCM_CYCLE_U start(ns)
	360,	// CS setup time for LCM read (optional)  
	90,	    // low pulse width for LCM read (according spec)	
	360,	// high pulse width for LCM read (according spec)
	35,		// CS setup time for LCM write  (optional) 	
	20,		// low pulse width for LCM write (according spec)
	20,	    // high pulse width for LCM write (according spec)	
	        // LCM_CYCLE_U end(ns)

};

//LCD INFO
PUBLIC const LCD_SPEC_T g_lcd_RM68040 = 
{
    LCD_WIDTH, 
    LCD_HEIGHT, 
    LCD_MCU, 
    BUS_MODE_8080,
    WIDTH_16,
    (LCD_TIMING_U*)&s_RM68040_timing, 
    (LCD_OPERATIONS_T*)&s_RM68040_ops_tab,
    0,
    0
};

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Haifeng.Yang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E RM68040_Init(void)
{
  
  //LCD_PRINT:"RM68040_Init\r\n"
  SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_RM68040_112_112_2_18_0_34_5_1856,(uint8*)"");
  
  LCD_CtrlWrite_RM68040(0x11); 		
	SCI_Sleep(20);

	LCD_CtrlWrite_RM68040(0xd0); 		
	LCD_DataWrite_RM68040(0x07);
	LCD_DataWrite_RM68040(0x41);//42
	LCD_DataWrite_RM68040(0x1d);//0c

	LCD_CtrlWrite_RM68040(0xd1); 		
	LCD_DataWrite_RM68040(0x00);
	LCD_DataWrite_RM68040(0x02);//15
	LCD_DataWrite_RM68040(0x12);//13

	LCD_CtrlWrite_RM68040(0xd2); 		
	LCD_DataWrite_RM68040(0x01);
	LCD_DataWrite_RM68040(0x22);//22

	LCD_CtrlWrite_RM68040(0xc0); 		
	LCD_DataWrite_RM68040(0x00);//00
	LCD_DataWrite_RM68040(0x3b);//3b
	LCD_DataWrite_RM68040(0x00);
	LCD_DataWrite_RM68040(0x02);
	LCD_DataWrite_RM68040(0x11);  

	LCD_CtrlWrite_RM68040(0xc5); 		
	LCD_DataWrite_RM68040(0x03);  


	//gamma
	LCD_CtrlWrite_RM68040(0xc8); 		
	LCD_DataWrite_RM68040(0x02);
	LCD_DataWrite_RM68040(0x75);
	LCD_DataWrite_RM68040(0x77);
	LCD_DataWrite_RM68040(0x05);
	LCD_DataWrite_RM68040(0x03);
	LCD_DataWrite_RM68040(0x01);
	LCD_DataWrite_RM68040(0x00);
	LCD_DataWrite_RM68040(0x20);
	LCD_DataWrite_RM68040(0x57);
	LCD_DataWrite_RM68040(0x50);
	LCD_DataWrite_RM68040(0x01);
	LCD_DataWrite_RM68040(0x03);


	LCD_CtrlWrite_RM68040(0xF8); 		
	LCD_DataWrite_RM68040(0x01);


	LCD_CtrlWrite_RM68040(0xfe); 		
	LCD_DataWrite_RM68040(0x00);
	LCD_DataWrite_RM68040(0x02);


	LCD_CtrlWrite_RM68040(0x36); 	//set scan direction	
	LCD_DataWrite_RM68040(0x0a);	//


	LCD_CtrlWrite_RM68040(0x3a); //set pixel format		
	LCD_DataWrite_RM68040(0x55);	//16bpp

	LCD_CtrlWrite_RM68040(0x2a); 	//set hor address 	
	LCD_DataWrite_RM68040(0x00);
	LCD_DataWrite_RM68040(0x00);	//start
	LCD_DataWrite_RM68040(0x01);
	LCD_DataWrite_RM68040(0x3f);	//end

	LCD_CtrlWrite_RM68040(0x2b); 	//set ver address	
	LCD_DataWrite_RM68040(0x00);
	LCD_DataWrite_RM68040(0x00);	//start
	LCD_DataWrite_RM68040(0x01);
	LCD_DataWrite_RM68040(0xdf);	//end

	SCI_Sleep(120);

	LCD_CtrlWrite_RM68040(0x29); 	//display on
	LCD_CtrlWrite_RM68040(0x2c); 	//start refresh
	
  return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//  Global resource dependence: 
//  Author:         Haifeng.Yang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E  RM68040_EnterSleep(
    BOOLEAN is_sleep    //SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
    )
{
   if(is_sleep)
   {
        LCD_CtrlWrite_RM68040(0x10);
   }
   else
   {
        LCD_CtrlWrite_RM68040(0x11); 
        SCI_Sleep(120);
   }
  return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:  set the contrast value 
//  Global resource dependence: 
//  Author:        Haifeng.Yang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E   RM68040_SetContrast(
    uint16  contrast    //contrast value to set
    )
{
    return ERR_LCD_FUNC_NOT_SUPPORT;
} 

/*****************************************************************************/
//  Description:    Set the brightness of LCD.
//  Global resource dependence: 
//  Author:         Haifeng.Yang
//  Note:
/*****************************************************************************/
LOCAL ERR_LCD_E   RM68040_SetBrightness(
    uint16 brightness   //birghtness to set
    )
{
    return ERR_LCD_FUNC_NOT_SUPPORT;
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//  Global resource dependence: 
//  Author:         Haifeng.Yang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E RM68040_SetDisplayWindow(
    uint16 left,    // start Horizon address
    uint16 top,     // end Horizon address
    uint16 right,   // start Vertical address
    uint16 bottom   // end Vertical address
    )
{
	//LCD_PRINT:"rm68040_set_window\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_RM68040_248_112_2_18_0_34_5_1857,(uint8*)"");
	
	LCD_CtrlWrite_RM68040(0x2a); 	//set hor address 	
	LCD_DataWrite_RM68040((left >> 8));
	LCD_DataWrite_RM68040((left & 0xff));	//start
	LCD_DataWrite_RM68040((right >> 8));
	LCD_DataWrite_RM68040((right & 0xff));	//end

	LCD_CtrlWrite_RM68040(0x2b); 	//set ver address	
	LCD_DataWrite_RM68040((top >> 8));
	LCD_DataWrite_RM68040((top & 0xff));	//start
	LCD_DataWrite_RM68040((bottom >> 8));
	LCD_DataWrite_RM68040((bottom & 0xff));	//end

#if 0
	switch (self->direction) {
	case LCD_DIRECT_NORMAL:
    default:
		LCD_CtrlWrite_RM68040_data(0x0080, (left >> 8));
		LCD_CtrlWrite_RM68040_data(0x0081, (left & 0xff));
		LCD_CtrlWrite_RM68040_data(0x0082, (top >> 8));
		LCD_CtrlWrite_RM68040_data(0x0083, (top & 0xff));
        break;
	case LCD_DIRECT_ROT_90:
		LCD_CtrlWrite_RM68040_data(0x0080, (top >> 8));
		LCD_CtrlWrite_RM68040_data(0x0081, (top & 0xff));
		LCD_CtrlWrite_RM68040_data(0x0082, (left >> 8));
		LCD_CtrlWrite_RM68040_data(0x0083, (left & 0xff));
	    break;
    case LCD_DIRECT_ROT_180:
	case LCD_DIRECT_MIR_HV:
		LCD_CtrlWrite_RM68040_data(0x0080, (right >> 8));
		LCD_CtrlWrite_RM68040_data(0x0081, (right & 0xff));
		LCD_CtrlWrite_RM68040_data(0x0082, (bottom >> 8));
		LCD_CtrlWrite_RM68040_data(0x0083, (bottom & 0xff));
	    break;
	case LCD_DIRECT_ROT_270:
		LCD_CtrlWrite_RM68040_data(0x0080, (bottom >> 8));
		LCD_CtrlWrite_RM68040_data(0x0081, (bottom & 0xff));
		LCD_CtrlWrite_RM68040_data(0x0082, (left >> 8));
		LCD_CtrlWrite_RM68040_data(0x0083, (left & 0xff));
	    break;
	case LCD_DIRECT_MIR_H:
		LCD_CtrlWrite_RM68040_data(0x0080, (left >> 8));
		LCD_CtrlWrite_RM68040_data(0x0081, (left & 0xff));
		LCD_CtrlWrite_RM68040_data(0x0082, (bottom >> 8));
		LCD_CtrlWrite_RM68040_data(0x0083, (bottom & 0xff));
	    break;
	case LCD_DIRECT_MIR_V:
		LCD_CtrlWrite_RM68040_data(0x0080, (right >> 8));
		LCD_CtrlWrite_RM68040_data(0x0081, (right & 0xff));
		LCD_CtrlWrite_RM68040_data(0x0082, (top >> 8));
		LCD_CtrlWrite_RM68040_data(0x0083, (top & 0xff));
	    break;
	}
#endif	
	LCD_CtrlWrite_RM68040(0x29);
	LCD_CtrlWrite_RM68040(0x2c); 	//start refresh

    return ERR_LCD_NONE;
}
/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//  Global resource dependence: 
//  Author:         Haifeng.Yang
//  Note:       
/******************************************************************************/
LOCAL ERR_LCD_E RM68040_InvalidateRect(
    uint16 left,    //the left value of the rectangel
    uint16 top,     //top of the rectangle
    uint16 right,   //right of the rectangle
    uint16 bottom   //bottom of the rectangle
    )
{
    left    = (left >= LCD_WIDTH)    ? LCD_WIDTH-1 : left;
    right   = (right >= LCD_WIDTH)   ? LCD_WIDTH-1 : right;
    top     = (top >= LCD_HEIGHT)    ? LCD_HEIGHT-1 : top;
    bottom  = (bottom >= LCD_HEIGHT) ? LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
        return ERR_LCD_OPERATE_FAIL;
    }     
   
    RM68040_SetDisplayWindow(left, top, right, bottom); 

    return ERR_LCD_NONE;
    
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//  Global resource dependence: 
//  Author:         Haifeng.Yang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E RM68040_Invalidate(void)
{
    RM68040_SetDisplayWindow (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence: 
//  Author:         Haifeng.Yang
//  Note:
/******************************************************************************/
LOCAL void RM68040_Close(void)
{
   
}

/**************************************************************************************/
// Description: refresh a rectangle of lcd
// Global resource dependence:
// Author: Haifeng.Yang
// Note:
//      left - the left value of the rectangel
//      top - the top value of the rectangel
//      right - the right value of the rectangel
//      bottom - the bottom value of the rectangel
/**************************************************************************************/
LOCAL ERR_LCD_E RM68040_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
    switch(angle)
    {
        case LCD_ANGLE_0:
		default:			
            RM68040_SetDisplayWindow(left, top, right, bottom);
            break;
        case LCD_ANGLE_90:
            RM68040_SetDisplayWindow(left, top, bottom,right);
            break;
        case LCD_ANGLE_180:
            RM68040_SetDisplayWindow(left, top, right, bottom);
            break;
        case LCD_ANGLE_270:
            RM68040_SetDisplayWindow(left, top, bottom,right);
            break;
    }
        
    return ERR_LCD_NONE;
}
/******************************************************************************/
// Description: refresh a rectangle of lcd
// Global resource dependence:
// Author: Haifeng.Yang
// Note: Application should correct invalidate direction right after current
//        image displayed
/******************************************************************************/
LOCAL ERR_LCD_E RM68040_SetDirection(
    LCD_DIRECT_E direct_type
    )
{
  

    return ERR_LCD_NONE;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
