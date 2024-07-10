/******************************************************************************
 ** File Name:     cstn_pcf8833.c                                             *
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


#define ST7781R_WriteCmd( _cmd )  /*lint -e572 -e778*/\
					LCD_SendCmd( (_cmd)>>8, 0);\
					LCD_SendCmd( (_cmd )&0xff, 0);

#define ST7781R_WriteData( _data )  /*lint -e572 -e778*/\
					LCD_SendData( (_data)>>8, 0);\
					LCD_SendData( (_data)&0xff, 0 );

#define ST7781R_WriteCmdData( _cmd, _data ) \
					ST7781R_WriteCmd( _cmd )\
					ST7781R_WriteData( _data )

//LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;

LOCAL BOOLEAN g_is_invert = FALSE ;
/******************************************************************************/
//  Description:    Clear color LCD with one color
//	Global resource dependence:
//  Author:         louis.wei
//	Note:
/******************************************************************************/
LOCAL void ST7781R_Clear(
						uint32 color	//color to fill the whole lcd.
						);

/******************************************************************************/
//  Description:    Close the lcd.(include sub lcd.)
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ST7781R_Close(void);

/******************************************************************************/
//  Description:    Enter/Exit sleep mode .
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ST7781R_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);

/******************************************************************************/
//  Description:    get the important parameter for digital camera
//	Global resource dependence:
//  Author:         Zhemin.lin
//	Note:
/******************************************************************************/
//LOCAL ERR_LCD_E  ST7781R_GetLcdSpec(
//	LCD_SPEC_T *spec_ptr 	//spec struct pointer
//	);

/*********************************************************************/
//  Description:   Initialize color LCD : ILI9328
//  Input:
//      None.
//  Return:
//      None.
//	Note:
/*********************************************************************/
LOCAL ERR_LCD_E ST7781R_Init(void);

/******************************************************************************/
//  Description:    invalidate a rectang of in LCD
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ST7781R_Invalidate(void);

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
LOCAL ERR_LCD_E ST7781R_InvalidateRect(
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
PUBLIC void ST7781R_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 right, 	// end Horizon address
	uint16 top, 		// start Vertical address
	uint16 bottom	// end Vertical address
	);


//LOCAL uint32 LCD_Write_regTab(const LCD_REG_T *tab_ptr);
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
LOCAL ERR_LCD_E ST7781R_SetDirection( LCD_DIRECT_E direct_type)
{
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:
		case LCD_DIRECT_ROT_90:
		case LCD_DIRECT_ROT_180:
		case LCD_DIRECT_ROT_270:
		case LCD_DIRECT_MIR_H:
		case LCD_DIRECT_MIR_V:
		case LCD_DIRECT_MIR_HV:
		default:
		    direct_type = LCD_DIRECT_NORMAL;
			break;

	}
//	s_lcd_direct = direct_type;
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
__inline void ST7781R_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 top, 	// end Horizon address
	uint16 right, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{
	ST7781R_WriteCmdData(0x0050, left);
	ST7781R_WriteCmdData(0x0051, right);
	ST7781R_WriteCmdData(0x0052, top);
	ST7781R_WriteCmdData(0x0053, bottom);	
	ST7781R_WriteCmdData(0x0020, left);
	ST7781R_WriteCmdData(0x0021, top);
	ST7781R_WriteCmd(0x0022);
}


/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E ST7781R_Init(void)
{
	//SCI_TRACE_LOW:"ST7781R_Init"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ST7781RB8_178_112_2_18_0_34_13_1876,(uint8*)"");
	LCD_Reset();
	LCD_Delayms(50);
    ST7781R_WriteCmd(0x0001);ST7781R_WriteData(0x0100);
    ST7781R_WriteCmd(0x0002);ST7781R_WriteData(0x0700);
    ST7781R_WriteCmd(0x0003);ST7781R_WriteData(0x10B0);
    ST7781R_WriteCmd(0x0004);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x0008);ST7781R_WriteData(0x0302);
    ST7781R_WriteCmd(0x0009);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x000A);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x000C);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x000D);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x000F);ST7781R_WriteData(0x0000);
    LCD_Delayms(100);
    ST7781R_WriteCmd(0x0010);ST7781R_WriteData(0x10E0);
    LCD_Delayms(200);
    ST7781R_WriteCmd(0x0030);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x0031);ST7781R_WriteData(0x0405);
    ST7781R_WriteCmd(0x0032);ST7781R_WriteData(0x0203);
    ST7781R_WriteCmd(0x0035);ST7781R_WriteData(0x0004);
    ST7781R_WriteCmd(0x0036);ST7781R_WriteData(0x0b07);
    ST7781R_WriteCmd(0x0037);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x0038);ST7781R_WriteData(0x0405);
    ST7781R_WriteCmd(0x0039);ST7781R_WriteData(0x0203);
    ST7781R_WriteCmd(0x003c);ST7781R_WriteData(0x0004);
    ST7781R_WriteCmd(0x003d);ST7781R_WriteData(0x0b07);
    ST7781R_WriteCmd(0x0020);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x0021);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x0050);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x0051);ST7781R_WriteData(0x00ef);
    ST7781R_WriteCmd(0x0052);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x0053);ST7781R_WriteData(0x013f);
    ST7781R_WriteCmd(0x0060);ST7781R_WriteData(0xA700);
    ST7781R_WriteCmd(0x0061);ST7781R_WriteData(0x0001);
    ST7781R_WriteCmd(0x006A);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x0090);ST7781R_WriteData(0x0030);
    ST7781R_WriteCmd(0x0095);ST7781R_WriteData(0x021E);
    ST7781R_WriteCmd(0x0080);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x0081);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x0082);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x0083);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x0084);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x0085);ST7781R_WriteData(0x0000);
    ST7781R_WriteCmd(0x00FF);ST7781R_WriteData(0x0001);
    ST7781R_WriteCmd(0x00B0);ST7781R_WriteData(0x140D);
    ST7781R_WriteCmd(0x00FF);ST7781R_WriteData(0x0000);
    LCD_Delayms(100);
    ST7781R_WriteCmd(0x0007);ST7781R_WriteData(0x0133);
    LCD_Delayms(50);
	return 0;
}


/******************************************************************************/
//  Description:   Clear color LCD with one color
//	Global resource dependence:
//  Author:         louis.wei
//	Note:
/******************************************************************************/
LOCAL void ST7781R_Clear(
						uint32 color	//color to fill the whole lcd.
						)
{
	ST7781R_Invalidate();
	return;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ST7781R_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	if(is_sleep)    // enter sleep mode
	{
        ST7781R_WriteCmd(0x0007);
        ST7781R_WriteData(0x0131);
        LCD_Delayms(100);
        ST7781R_WriteCmd(0x0007);
        ST7781R_WriteData(0x0020);
        LCD_Delayms(100);
        ST7781R_WriteCmd(0x0010);
        ST7781R_WriteData(0x0082);		
	}
	else 			// exit sleep mode
	{
	 	ST7781R_Init();
	}
	return ERR_LCD_NONE;
}



/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ST7781R_Close(void)
{
	ST7781R_EnterSleep( SCI_TRUE );
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ST7781R_Invalidate(void)
{
	ST7781R_set_display_window (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E ST7781R_InvalidateRect(
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
    ST7781R_set_display_window(left, top, right, bottom);
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
LOCAL ERR_LCD_E ST7781R_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	switch(angle)
	{
		case LCD_ANGLE_0:
			ST7781R_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			ST7781R_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			ST7781R_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			ST7781R_set_display_window(left, top, bottom,right);
			break;
		default:
			ST7781R_set_display_window(left, top, right, bottom);
			break;
	}
	return ERR_LCD_NONE;
}//en of ST7781R_VerticalInvalidateRect

/******************************************************************************/
//  Description:  set the contrast value
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   ST7781R_SetContrast(
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
LOCAL ERR_LCD_E   ST7781R_SetBrightness(
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
LOCAL ERR_LCD_E ST7781R_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	ST7781R_set_display_window(left, top, right, bottom);
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
const LCD_OPERATIONS_T ST7781R_operations =
{
	ST7781R_Init,
	ST7781R_EnterSleep,
	ST7781R_SetContrast,
	ST7781R_SetBrightness,
	ST7781R_SetDisplayWindow,
	ST7781R_InvalidateRect,
	ST7781R_Invalidate,
	ST7781R_Close,
	ST7781R_RotationInvalidateRect,
	ST7781R_SetDirection,
	NULL,
	NULL
};



/******************************************************************************/
//  Description:  return the ILI9328 lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ST7781R_GetOperations(void)
{
	return (LCD_OPERATIONS_T*)&ST7781R_operations;
}

PUBLIC BOOLEAN ST7781R_Probe(void)
{
	return SCI_TRUE;
}

LOCAL  const LCD_TIMING_U s_ST7781R_timing =
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

PUBLIC const LCD_SPEC_T g_lcd_ST7781RB8 = {
	LCD_WIDTH,
	LCD_HEIGHT,
	LCD_MCU,
	BUS_MODE_8080,	
	WIDTH_8,
	(LCD_TIMING_U*)&s_ST7781R_timing,
	(LCD_OPERATIONS_T*)&ST7781R_operations,
	0,
	0
};


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


