#include "ms_customize_trc.h"
#include "os_api.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"
#ifdef WT_LCD_AUTO_PROBE
#include "tb_comm.h"
#endif
#include "lcdc_drv.h"
    
#define  L_YG

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C"
    {
#endif

//extern uint32 LCDC_Read(uint32 lcd_id, uint32 cd_value);

#define LCD_CtrlWrite( _cmd )\
					LCD_SendCmd( (_cmd ), 0);
	
#define LCD_DataWrite( _data )\
					LCD_SendData( (_data), 0 );




#define ILI9481_WriteCmdData( _cmd, _data )  LCD_CtrlData( _cmd, _data )
#define ILI9481_WriteCmd( _cmd )             LCD_CtrlWrite( _cmd )
#define ILI9481_WriteData( _data )           LCD_DataWrite( _data )
#define LCD_DataRead_ILI9481()				 LCDC_Read(0, 1)
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;

LOCAL BOOLEAN g_is_invert = FALSE ;
/******************************************************************************/
//  Description:    Clear color LCD with one color
//	Global resource dependence:
//  Author:         louis.wei
//	Note:
/******************************************************************************/
LOCAL void ILI9481_Clear(
						uint32 color	//color to fill the whole lcd.
						);

/******************************************************************************/
//  Description:    Close the lcd.(include sub lcd.)
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ILI9481_Close(void);

/******************************************************************************/
//  Description:    Enter/Exit sleep mode .
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9481_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);

/******************************************************************************/
//  Description:    get the important parameter for digital camera
//	Global resource dependence:
//  Author:         Zhemin.lin
//	Note:
/******************************************************************************/
//LOCAL ERR_LCD_E  ILI9481_GetLcdSpec(
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
LOCAL ERR_LCD_E ILI9481_Init(void);

/******************************************************************************/
//  Description:    invalidate a rectang of in LCD
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9481_Invalidate(void);

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
LOCAL ERR_LCD_E ILI9481_InvalidateRect(
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
PUBLIC void ILI9481_set_display_window(
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
LOCAL ERR_LCD_E ILI9481_SetDirection( LCD_DIRECT_E direct_type)
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
	s_lcd_direct = direct_type;
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
__inline void ILI9481_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 top, 	// end Horizon address
	uint16 right, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{
	ILI9481_WriteCmd(0x2a);
	ILI9481_WriteData((left & 0xff00)>>8);
	ILI9481_WriteData(left & 0x00ff);
	ILI9481_WriteData((right & 0xff00)>>8);
	ILI9481_WriteData(right & 0x00ff);
	ILI9481_WriteCmd(0x2b);
	ILI9481_WriteData((top & 0xff00)>>8);
	ILI9481_WriteData(top & 0x00ff);
	ILI9481_WriteData((bottom & 0xff00)>>8);
	ILI9481_WriteData(bottom & 0x00ff);
	ILI9481_WriteCmd(0x2c);
}  


/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E ILI9481_ReInit(void)
{
	ILI9481_WriteCmd(0x11);
	LCD_Delayms(20);   
	ILI9481_WriteCmd(0xD0);
	ILI9481_WriteData(0x07);
	ILI9481_WriteData(0x41);
	ILI9481_WriteData(0x1A);
	ILI9481_WriteCmd(0xD1);
	ILI9481_WriteData(0x00);
	ILI9481_WriteData(0x25);
	ILI9481_WriteData(0x18);
	ILI9481_WriteCmd(0xD2);
	ILI9481_WriteData(0x01);
	ILI9481_WriteData(0x11);
	ILI9481_WriteCmd(0xC0);
	ILI9481_WriteData(0x10);
	ILI9481_WriteData(0x3B);
	ILI9481_WriteData(0x00);
	ILI9481_WriteData(0x02);
	ILI9481_WriteData(0x11);
	ILI9481_WriteCmd(0xC5);
	ILI9481_WriteData(0x04);
	ILI9481_WriteCmd(0xC8);
	ILI9481_WriteData(0x00);
	ILI9481_WriteData(0x53);
	ILI9481_WriteData(0x14);
	ILI9481_WriteData(0x30);
	ILI9481_WriteData(0x04);
	ILI9481_WriteData(0x00);
	ILI9481_WriteData(0x36);
	ILI9481_WriteData(0x42);
	ILI9481_WriteData(0x77);
	ILI9481_WriteData(0x03);
	ILI9481_WriteData(0x04);
	ILI9481_WriteData(0x00);
	ILI9481_WriteCmd(0xE4);
	ILI9481_WriteData(0xA0);
	ILI9481_WriteCmd(0xF0);
	ILI9481_WriteData(0x01);
	ILI9481_WriteCmd(0xF3);
	ILI9481_WriteData(0x40);
	ILI9481_WriteData(0x0A);
	ILI9481_WriteCmd(0xF7);
	ILI9481_WriteData(0x80);
	ILI9481_WriteCmd(0x3A);
	ILI9481_WriteData(0x55);
	ILI9481_WriteCmd(0xB3);
	ILI9481_WriteData(0x00);
	ILI9481_WriteData(0x00);
	ILI9481_WriteData(0x00);
	ILI9481_WriteData(0x20);
	ILI9481_WriteCmd(0x36);
	ILI9481_WriteData(0x0A);
	LCD_Delayms(120);
	ILI9481_WriteCmd(0x29);
	ILI9481_WriteCmd(0x2A);
	ILI9481_WriteData(0x00);
	ILI9481_WriteData(0x00);
	ILI9481_WriteData(0x01);
	ILI9481_WriteData(0x3F);
	ILI9481_WriteCmd(0x2B);
	ILI9481_WriteData(0x00);
	ILI9481_WriteData(0x00);
	ILI9481_WriteData(0x01);
	ILI9481_WriteData(0xDF);
	ILI9481_WriteCmd(0x2C);
	return 0;
}

LOCAL ERR_LCD_E ILI9481_Init(void)
{
	//SCI_TRACE_LOW:"ILI9481_Init"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9481_244_112_2_18_0_33_54_1824,(uint8*)"");
	LCD_Reset();
	LCD_Delayms(50);
	ILI9481_ReInit();
	return 0;
}

/******************************************************************************/
//  Description:   Clear color LCD with one color
//	Global resource dependence:
//  Author:         louis.wei
//	Note:
/******************************************************************************/
LOCAL void ILI9481_Clear(
						uint32 color	//color to fill the whole lcd.
						)
{
	ILI9481_Invalidate();
	return;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9481_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	if(is_sleep)    // enter sleep mode
	{
		ILI9481_WriteCmd (0x28);
        LCD_Delayms(10);
		ILI9481_WriteCmd (0x10);
        LCD_Delayms(120);
	}
	else 			// exit sleep mode
	{
	 	ILI9481_ReInit();
	}
	return ERR_LCD_NONE;
}



/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ILI9481_Close(void)
{
	ILI9481_EnterSleep( SCI_TRUE );
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9481_Invalidate(void)
{
	ILI9481_set_display_window (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E ILI9481_InvalidateRect(
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
    ILI9481_set_display_window(left, top, right, bottom);
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
LOCAL ERR_LCD_E ILI9481_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	switch(angle)
	{
		case LCD_ANGLE_0:
			ILI9481_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			ILI9481_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			ILI9481_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			ILI9481_set_display_window(left, top, bottom,right);
			break;
		default:
			ILI9481_set_display_window(left, top, right, bottom);
			break;
	}
	return ERR_LCD_NONE;
}//en of ILI9481_VerticalInvalidateRect

/******************************************************************************/
//  Description:  set the contrast value
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   ILI9481_SetContrast(
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
LOCAL ERR_LCD_E   ILI9481_SetBrightness(
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
LOCAL ERR_LCD_E ILI9481_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	ILI9481_set_display_window(left, top, right, bottom);
	return ERR_LCD_NONE;
}

#ifdef WT_LCD_AUTO_PROBE
BOOLEAN ILI9481_IDCheck(uint32 lcd_id)
{
    uint32 value[6] ={0};
    uint32 data_addr =0;    
    data_addr = LCDC_Get_LCM_DataAddr(lcd_id);
    LCDC_SendCmd(0xbf,lcd_id);
    LCDC_SendCmd(0xbf,lcd_id);
 
    TB_REG_OR(data_addr, (1<<18));
    SCI_Sleep(10);
    value[0] = TB_REG_GET(0x20700154);

    TB_REG_OR(data_addr, (1<<18));
    SCI_Sleep(10);
    value[1] = TB_REG_GET(0x20700154);

    TB_REG_OR(data_addr, (1<<18));
    SCI_Sleep(10);
    value[2] = TB_REG_GET(0x20700154);

    TB_REG_OR(data_addr, (1<<18));
    SCI_Sleep(10);
    value[3] = TB_REG_GET(0x20700154);

    TB_REG_OR(data_addr, (1<<18));
    SCI_Sleep(10);
    value[4] = TB_REG_GET(0x20700154);

    TB_REG_OR(data_addr, (1<<18));
    SCI_Sleep(10);
    value[5] = TB_REG_GET(0x20700154);
    
    //SCI_TRACE_LOW:"ILI9481_IDCheck dev_id[1]=%x, dev_id[2]=%x, dev_id[3]=%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9481_437_112_2_18_0_33_55_1825,(uint8*)"ddd",value[2]<<16, value[3]<<8, value[4]);
    
    if((0x94 == value[3]<<8)&&(0x81 == value[4]))
        return SCI_TRUE;
    else
       return SCI_FALSE;
}
#endif

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
const LCD_OPERATIONS_T ILI9481_operations =
{
	ILI9481_Init,
	ILI9481_EnterSleep,
	ILI9481_SetContrast,
	ILI9481_SetBrightness,
	ILI9481_SetDisplayWindow,
	ILI9481_InvalidateRect,
	ILI9481_Invalidate,
	ILI9481_Close,
	ILI9481_RotationInvalidateRect,
	ILI9481_SetDirection,
	NULL,
	NULL,
#ifdef WT_LCD_AUTO_PROBE
	ILI9481_IDCheck,
#endif
};



/******************************************************************************/
//  Description:  return the ILI9328 lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9481_GetOperations(void)
{
	return (LCD_OPERATIONS_T*)&ILI9481_operations;
}

PUBLIC BOOLEAN ILI9481_Probe(void)
{
	return SCI_TRUE;
}

LOCAL  const LCD_TIMING_U s_ILI9481_timing =
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

PUBLIC const LCD_SPEC_T g_lcd_ILI9481 = {
	LCD_WIDTH,
	LCD_HEIGHT,
	LCD_MCU,
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U*)&s_ILI9481_timing,
	(LCD_OPERATIONS_T*)&ILI9481_operations,0,0
};


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


