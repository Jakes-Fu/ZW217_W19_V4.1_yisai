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

#define LCD_CtrlWrite( _cmd )\
					LCD_SendCmd( (_cmd ), 0);
	
#define LCD_DataWrite( _data )\
					LCD_SendData( (_data), 0 );


#define ILI9486_WriteCmdData( _cmd, _data )  LCD_CtrlData( _cmd, _data )
#define ILI9486_WriteCmd( _cmd )             LCD_CtrlWrite( _cmd )
#define ILI9486_WriteData( _data )           LCD_DataWrite( _data )
#define LCD_DataRead_ILI9486()				 LCDC_Read(0, 1)
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;

LOCAL BOOLEAN g_is_invert = FALSE ;
/******************************************************************************/
//  Description:    Clear color LCD with one color
//	Global resource dependence:
//  Author:         louis.wei
//	Note:
/******************************************************************************/
LOCAL void ILI9486_Clear(
						uint32 color	//color to fill the whole lcd.
						);

/******************************************************************************/
//  Description:    Close the lcd.(include sub lcd.)
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ILI9486_Close(void);

/******************************************************************************/
//  Description:    Enter/Exit sleep mode .
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9486_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);

/******************************************************************************/
//  Description:    get the important parameter for digital camera
//	Global resource dependence:
//  Author:         Zhemin.lin
//	Note:
/******************************************************************************/
//LOCAL ERR_LCD_E  ILI9486_GetLcdSpec(
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
LOCAL ERR_LCD_E ILI9486_Init(void);

/******************************************************************************/
//  Description:    invalidate a rectang of in LCD
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9486_Invalidate(void);

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
LOCAL ERR_LCD_E ILI9486_InvalidateRect(
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
PUBLIC void ILI9486_set_display_window(
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
LOCAL ERR_LCD_E ILI9486_SetDirection( LCD_DIRECT_E direct_type)
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
__inline void ILI9486_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 top, 	// end Horizon address
	uint16 right, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{
	ILI9486_WriteCmd(0x2a);
	ILI9486_WriteData((left & 0xff00)>>8);
	ILI9486_WriteData(left & 0x00ff);
	ILI9486_WriteData((right & 0xff00)>>8);
	ILI9486_WriteData(right & 0x00ff);
	ILI9486_WriteCmd(0x2b);
	ILI9486_WriteData((top & 0xff00)>>8);
	ILI9486_WriteData(top & 0x00ff);
	ILI9486_WriteData((bottom & 0xff00)>>8);
	ILI9486_WriteData(bottom & 0x00ff);
	ILI9486_WriteCmd(0x2c);
}  


/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E ILI9486_Init(void)
{
	//SCI_TRACE_LOW:"ILI9486_Init"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9486_170_112_2_18_0_33_55_1826,(uint8*)"");
	LCD_Reset();
	LCD_Delayms(50);
    ILI9486_WriteCmd(0XF7);
    ILI9486_WriteData(0xA9);
    ILI9486_WriteData(0x91);
    ILI9486_WriteData(0x2D);
    ILI9486_WriteData(0x0A);
    ILI9486_WriteData(0x4F);
    ILI9486_WriteCmd(0XF8);
    ILI9486_WriteData(0x21);
    ILI9486_WriteData(0x04);
    ILI9486_WriteCmd(0xE0);
    ILI9486_WriteData(0x00);
    ILI9486_WriteData(0x07);
    ILI9486_WriteData(0x0A);
    ILI9486_WriteData(0x04);
    ILI9486_WriteData(0x13);
    ILI9486_WriteData(0x0B);
    ILI9486_WriteData(0x36);
    ILI9486_WriteData(0x7B);
    ILI9486_WriteData(0x48);
    ILI9486_WriteData(0x07);
    ILI9486_WriteData(0x0F);
    ILI9486_WriteData(0x0B);
    ILI9486_WriteData(0x11);
    ILI9486_WriteData(0x1C);
    ILI9486_WriteData(0x1F);
    ILI9486_WriteCmd(0XE1);
    ILI9486_WriteData(0x00);
    ILI9486_WriteData(0x23);
    ILI9486_WriteData(0x24);
    ILI9486_WriteData(0x04);
    ILI9486_WriteData(0x11);
    ILI9486_WriteData(0x08);
    ILI9486_WriteData(0x37);
    ILI9486_WriteData(0x47);
    ILI9486_WriteData(0x49);
    ILI9486_WriteData(0x04);
    ILI9486_WriteData(0x0C);
    ILI9486_WriteData(0x0B);
    ILI9486_WriteData(0x35);
    ILI9486_WriteData(0x38);
    ILI9486_WriteData(0x1F);
    ILI9486_WriteCmd(0XB1);
    ILI9486_WriteData(0xB0);
    ILI9486_WriteData(0x11);
    ILI9486_WriteCmd(0XB4);
    ILI9486_WriteData(0x00);
    ILI9486_WriteCmd(0XB6);
    ILI9486_WriteData(0x02);
    ILI9486_WriteData(0x22);
    ILI9486_WriteData(0x3B);
    ILI9486_WriteCmd(0XC5);
    ILI9486_WriteData(0x00);
    ILI9486_WriteData(0x16);
    ILI9486_WriteData(0x80);
    ILI9486_WriteCmd(0x36);
    ILI9486_WriteData(0x08);
    ILI9486_WriteCmd(0x3A);
    ILI9486_WriteData(0x55);
    ILI9486_WriteCmd(0x11);
    LCD_Delayms(120);
    ILI9486_WriteCmd(0x29);
	return 0;
}


/******************************************************************************/
//  Description:   Clear color LCD with one color
//	Global resource dependence:
//  Author:         louis.wei
//	Note:
/******************************************************************************/
LOCAL void ILI9486_Clear(
						uint32 color	//color to fill the whole lcd.
						)
{
	ILI9486_Invalidate();
	return;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9486_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	if(is_sleep)    // enter sleep mode
	{
		ILI9486_WriteCmd (0x28);
        LCD_Delayms(10);
		ILI9486_WriteCmd (0x10);
        LCD_Delayms(120);
	}
	else 			// exit sleep mode
	{
	 	ILI9486_Init();
	}
	return ERR_LCD_NONE;
}



/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ILI9486_Close(void)
{
	ILI9486_EnterSleep( SCI_TRUE );
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9486_Invalidate(void)
{
	ILI9486_set_display_window (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E ILI9486_InvalidateRect(
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
    ILI9486_set_display_window(left, top, right, bottom);
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
LOCAL ERR_LCD_E ILI9486_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	switch(angle)
	{
		case LCD_ANGLE_0:
			ILI9486_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			ILI9486_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			ILI9486_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			ILI9486_set_display_window(left, top, bottom,right);
			break;
		default:
			ILI9486_set_display_window(left, top, right, bottom);
			break;
	}
	return ERR_LCD_NONE;
}//en of ILI9486_VerticalInvalidateRect

/******************************************************************************/
//  Description:  set the contrast value
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   ILI9486_SetContrast(
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
LOCAL ERR_LCD_E   ILI9486_SetBrightness(
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
LOCAL ERR_LCD_E ILI9486_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	ILI9486_set_display_window(left, top, right, bottom);
	return ERR_LCD_NONE;
}

#ifdef WT_LCD_AUTO_PROBE
BOOLEAN ILI9486_IDCheck(uint32 lcd_id)
{
    uint32 value[4] ={0};
    uint32 data_addr =0;   
 
    data_addr = LCDC_Get_LCM_DataAddr(lcd_id);
    LCDC_SendCmd(0xd3,lcd_id);
    LCDC_SendCmd(0xd3,lcd_id);
		 
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

    //SCI_TRACE_LOW:"ILI9486_IDCheck dev_id[1]=%x, dev_id[2]=%x, dev_id[3]=%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9486_417_112_2_18_0_33_56_1827,(uint8*)"ddd", value[1]<<16, value[2], value[3]);
    
    if((0x94 == value[2])&&(0x86 == value[3]))
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
const LCD_OPERATIONS_T ILI9486_operations =
{
	ILI9486_Init,
	ILI9486_EnterSleep,
	ILI9486_SetContrast,
	ILI9486_SetBrightness,
	ILI9486_SetDisplayWindow,
	ILI9486_InvalidateRect,
	ILI9486_Invalidate,
	ILI9486_Close,
	ILI9486_RotationInvalidateRect,
	ILI9486_SetDirection,
	NULL,
	NULL,
#ifdef WT_LCD_AUTO_PROBE
	ILI9486_IDCheck,
#endif
};



/******************************************************************************/
//  Description:  return the ILI9328 lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9486_GetOperations(void)
{
	return (LCD_OPERATIONS_T*)&ILI9486_operations;
}

PUBLIC BOOLEAN ILI9486_Probe(void)
{
	return SCI_TRUE;
}

LOCAL  const LCD_TIMING_U s_ILI9486_timing =
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

PUBLIC const LCD_SPEC_T g_lcd_ILI9486 = {
	LCD_WIDTH,
	LCD_HEIGHT,
	LCD_MCU,
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U*)&s_ILI9486_timing,
	(LCD_OPERATIONS_T*)&ILI9486_operations,0,0
};


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


