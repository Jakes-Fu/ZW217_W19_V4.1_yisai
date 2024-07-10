/******************************************************************************
 ** File Name:     TFT_HX8347G_spi.c                                             *
 ** Description:															  *
 **    This file contains driver for color LCD.(CSTN)						  *
 ** Author:         Jason Su                                                 *
 ** DATE:           08/09/2011                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                NAME             DESCRIPTION                               *
 ** 08/07/2011     Jason Su	       Create.
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

#define SCI_TraceLow_Test SCI_TraceLow

/*lint -save -e767 -e718*/
#if 1
#define LCD_CtrlWrite_HX8347G( _cmd )\
				LCD_SendCmd( (_cmd ), 0 );

#define LCD_DataWrite_HX8347G( _data )\
				LCD_SendData( (_data), 0 );
#else
#define LCD_CtrlWrite_HX8347G( _cmd ) LCD_SendCmd( (_cmd ), 0 );
#define LCD_DataWrite_HX8347G( _data ) LCD_SendData( (_data>>8), 0 );\
                                       LCD_SendData( (_data&0xFF),0 );    
#endif


#define LCD_CtrlData_HX8347G(cmd, data)  LCD_CtrlWrite_HX8347G( cmd );  \
                                          LCD_DataWrite_HX8347G( data );    

/*lint -restore*/
#define LCD_Delay( ticks)\
				OS_TickDelay( ticks );
#define HX8347G_SUPPORT_WIDTH   240				
#define HX8347G_SUPPORT_HEIGHT  320				
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;

  /******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL void HX8347G_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8347G_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);

/*********************************************************************/
//  Description:   Initialize color LCD : HX8347
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E HX8347G_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8347G_Invalidate(void);


/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:       
//     To improve speed, lcd is operate in HIGH SPEED RAM WRITE MODE(4
//     uint16 are write continuously always.) So, some dummy uint16 
//     should be inserted to satisfy this mode.   Please refer to spec.
/******************************************************************************/
LOCAL ERR_LCD_E HX8347G_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	);
/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL void HX8347G_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 right, 	// end Horizon address
	uint16 top, 		// start Vertical address
	uint16 bottom	// end Vertical address
	);

/******************************************************************************/
//  Description:  Set LCD invalidate direction
//  Input:
//      is_invert: 0, horizontal; 1, vertical
//  Return:
//      None.
//	Note: Application should correct invalidate direction right after current
//	 	  image displayed
/******************************************************************************/
LOCAL ERR_LCD_E HX8347G_SetDirection(
	LCD_DIRECT_E direct_type
	)
{
    if (LCD_DIRECT_NORMAL == direct_type)
    	{
    	    direct_type = LCD_DIRECT_MIR_H;
    	}
    else if (LCD_DIRECT_MIR_H == direct_type)
    	{
    	    direct_type = LCD_DIRECT_NORMAL;
    	}
    SCI_TraceLow_Test(" HX8347G_SetDirection = %d",direct_type);
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:
			LCD_CtrlWrite_HX8347G(0x0016);
			LCD_DataWrite_HX8347G(0x0008);	
			break;
		case LCD_DIRECT_ROT_90:
			LCD_CtrlWrite_HX8347G(0x0016);
			LCD_DataWrite_HX8347G(0x0068);
			break;
		case LCD_DIRECT_ROT_180:
			LCD_CtrlWrite_HX8347G(0x0016);
			LCD_DataWrite_HX8347G(0x00C8);
			break;
		case LCD_DIRECT_ROT_270:
			LCD_CtrlWrite_HX8347G(0x0016);
			LCD_DataWrite_HX8347G(0x00A8);
			break;
		case LCD_DIRECT_MIR_H:
			LCD_CtrlWrite_HX8347G(0x0016);
			LCD_DataWrite_HX8347G(0x0048);
			break;
		case LCD_DIRECT_MIR_V:
			LCD_CtrlWrite_HX8347G(0x0016);
			LCD_DataWrite_HX8347G(0x0088);
			break;
		case LCD_DIRECT_MIR_HV:
			LCD_CtrlWrite_HX8347G(0x0016);
			LCD_DataWrite_HX8347G(0x0028);
			break;
		default:
			LCD_CtrlWrite_HX8347G(0x0016);
			LCD_DataWrite_HX8347G(0x0008);
			break;
	}
	
	LCD_CtrlWrite_HX8347G(0x0022);
	
	s_lcd_direct = direct_type;
	return ERR_LCD_NONE;//coverity warning
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL void HX8347G_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 top, 	// end Horizon address
	uint16 right, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{   
	uint16 newleft   = left;
	uint16 newtop    = top;
	uint16 newright  = right;
	uint16 newbottom = bottom;
    SCI_TraceLow_Test("[DRV_LCD]HX8347G_set_display_window L = %d, top = %d, R = %d, bottom = %d",left,top,right,bottom);
	SCI_TraceLow_Test("[DRV_LCD]HX8347G_set_display_window s_lcd_direct = %d",s_lcd_direct);


    //SCI_TRACE_LOW:"[REF_LCD] HX8347G new left = %d, top = %d, right = %d, bottom = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347G_SPI_191_112_2_18_0_33_34_1768,(uint8*)"dddd",newleft, newtop, newright, newbottom);
	LCD_CtrlData_HX8347G(0x0002,(newleft>>8)&0xff); //Column address start 2
	LCD_CtrlData_HX8347G(0x0003,newleft&0x00ff); // Column address start 1
	LCD_CtrlData_HX8347G(0x0004,(newright>>8)&0xff); // Column address end 2
	LCD_CtrlData_HX8347G(0x0005,newright&0x00ff); // Column address end 1

	LCD_CtrlData_HX8347G(0x0006,(newtop>>8)&0xff); // Row address start 2
	LCD_CtrlData_HX8347G(0x0007,newtop&0x00ff); // Row address start 1
	LCD_CtrlData_HX8347G(0x0008,(newbottom>>8)&0xff); // Row address end 2
	LCD_CtrlData_HX8347G(0x0009,newbottom&0x00ff); // Row address end 1

	LCD_CtrlWrite_HX8347G(0x0022);
}

LOCAL void HX8347G_driver(void)
{
	SCI_TraceLow_Test("[REF_LCD]HX8347G_driver");
	LCD_CtrlData_HX8347G(0x00EA,0x0000);//PTBA[15:8]
	LCD_CtrlData_HX8347G(0x00EB,0x0020);//PTBA[7:0]
	LCD_CtrlData_HX8347G(0x00EC,0x000C);//STBA[15:8]
	LCD_CtrlData_HX8347G(0x00ED,0x00C4);//STBA[7:0]
	LCD_CtrlData_HX8347G(0x00E8,0x0038);//OPON[7:0]
	LCD_CtrlData_HX8347G(0x00E9,0x0038);//10//OPON1[7:0]
	LCD_CtrlData_HX8347G(0x00F1,0x0001);//OTPS1B
	LCD_CtrlData_HX8347G(0x00F2,0x0010);//GEN
	LCD_CtrlData_HX8347G(0x0027,0x00A3);

	LCD_CtrlData_HX8347G(0x0040,0x0000);//ÉèÖÃgammaÖµ
	LCD_CtrlData_HX8347G(0x0041,0x0000);
	LCD_CtrlData_HX8347G(0x0042,0x0001);
	LCD_CtrlData_HX8347G(0x0043,0x0012);
	LCD_CtrlData_HX8347G(0x0044,0x0010);
	LCD_CtrlData_HX8347G(0x0045,0x0026);
	LCD_CtrlData_HX8347G(0x0046,0x0008);
	LCD_CtrlData_HX8347G(0x0047,0x0054);
	LCD_CtrlData_HX8347G(0x0048,0x0002);
	LCD_CtrlData_HX8347G(0x0049,0x0015);
	LCD_CtrlData_HX8347G(0x004a,0x0019);
	LCD_CtrlData_HX8347G(0x004b,0x0019);
	LCD_CtrlData_HX8347G(0x004c,0x0016);

	LCD_CtrlData_HX8347G(0x0050,0x0019);
	LCD_CtrlData_HX8347G(0x0051,0x002f);
	LCD_CtrlData_HX8347G(0x0052,0x002d);
	LCD_CtrlData_HX8347G(0x0053,0x003e);
	LCD_CtrlData_HX8347G(0x0054,0x003f);
	LCD_CtrlData_HX8347G(0x0055,0x003f);
	LCD_CtrlData_HX8347G(0x0056,0x002b);
	LCD_CtrlData_HX8347G(0x0057,0x0077);
	LCD_CtrlData_HX8347G(0x0058,0x0009);
	LCD_CtrlData_HX8347G(0x0059,0x0006);
	LCD_CtrlData_HX8347G(0x005a,0x0006);
	LCD_CtrlData_HX8347G(0x005b,0x000a);
	LCD_CtrlData_HX8347G(0x005c,0x001d);
	LCD_CtrlData_HX8347G(0x005d,0x00cc);
	
	LCD_CtrlData_HX8347G(0x001b,0x001B);//VRH=4.65V
	LCD_CtrlData_HX8347G(0x001A,0x0001);//BT (VGH~15V,VGL~-10V,DDVDH~5V)
	LCD_CtrlData_HX8347G(0x0024,0x0039);//VMH(VCOM High voltage ~3.2V) 
	LCD_CtrlData_HX8347G(0x0025,0x006E);//VML(VCOM Low voltage -1.2V) 
#ifdef LCD_DATA_WIDTH_8BIT		
	LCD_CtrlData_HX8347G(0x0023,0x0079);//for Flicker adjust //can reload from OTP 
#else
	LCD_CtrlData_HX8347G(0x0023,0x007a);//for Flicker adjust //can reload from OTP 
#endif	

	LCD_CtrlData_HX8347G(0x0018,0x0036); //I/P_RADJ,N/P_RADJ, Normal mode 75Hz
	LCD_CtrlData_HX8347G(0x0019,0x0001); //OSC_EN='1', start Osc
	LCD_CtrlData_HX8347G(0x0001,0x0000); //DP_STB='0', out deep sleep
	LCD_CtrlData_HX8347G(0x001F,0x0088); //GAS=1, VOMG=00, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=0
	LCD_Delay(5);
	LCD_CtrlData_HX8347G(0x001F,0x0080); //GAS=1, VOMG=00, PON=0, DK=0, XDK=0, DVDH_TRI=0, STB=0
	LCD_Delay(5);
	LCD_CtrlData_HX8347G(0x001F,0x0090); //GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0
	LCD_Delay(5);
	LCD_CtrlData_HX8347G(0x001F,0x00D0); //GAS=1, VOMG=10, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
	LCD_Delay(5);
	LCD_CtrlData_HX8347G(0x0017,0x0055); //default 0x60 262k color // 0x50 65k color

	LCD_CtrlData_HX8347G(0x0036,0x0000); //SS_P, GS_P,REV_P,BGR_P

	LCD_CtrlData_HX8347G(0x0028,0x0038); //GON=1, DTE=1, D=1000
	LCD_Delay(40);
	LCD_CtrlData_HX8347G(0x0028,0x003C); //GON=1, DTE=1, D=1100

    // adding by lichd
	LCD_CtrlData_HX8347G(0x0016,0x0008); //BIT[3] is BGR
	LCD_Delay(40);

	LCD_CtrlData_HX8347G(0x0002,0x0000); 
	LCD_CtrlData_HX8347G(0x0003,0x0000); //Column Start
	LCD_CtrlData_HX8347G(0x0004,0x0000); 
	LCD_CtrlData_HX8347G(0x0005,0x00EF); //Column End

	LCD_CtrlData_HX8347G(0x0006,0x0000);
	LCD_CtrlData_HX8347G(0x0007,0x0000); //Row Start
	LCD_CtrlData_HX8347G(0x0008,0x0001); 
	LCD_CtrlData_HX8347G(0x0009,0x003F); //Row End

	LCD_CtrlWrite_HX8347G(0x0022);//start GRAM write
	return;
}

/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jason Su
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E HX8347G_Init(void)
{
	SCI_TraceLow_Test(" HX8347G_Init");
	HX8347G_driver();
	HX8347G_SetDirection(LCD_DIRECT_NORMAL);
	return 0;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8347G_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
SCI_TraceLow_Test(" HX8347G_EnterSleep");

	if ( is_sleep ) // enter sleep mode.
	{
		LCD_CtrlData_HX8347G(0x28,0x38);
		LCD_Delay(40);
		LCD_CtrlData_HX8347G(0x28,0x04);

		LCD_CtrlData_HX8347G(0x1F,0x08);
		LCD_Delay(5);
		LCD_CtrlData_HX8347G(0x1F,0x09);
		LCD_CtrlData_HX8347G(0x19,0x00); //osc_en="0"
	}
	else 			// out sleep mode 
	{
		LCD_Reset();
		HX8347G_Init();
	}

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL void HX8347G_Close(void)
{
	//GPIO_SetLcdBackLight( SCI_FALSE );
SCI_TraceLow_Test(" HX8347G_Close");
	HX8347G_EnterSleep( SCI_TRUE );	
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8347G_Invalidate(void)
{
SCI_TraceLow_Test(" HX8347G_Invalidate");

	HX8347G_set_display_window (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:       
//     To improve speed, lcd is operate in HIGH SPEED RAM WRITE MODE(4
//     uint16 are write continuously always.) So, some dummy uint16 
//     should be inserted to satisfy this mode.   Please refer to spec.
/******************************************************************************/
PUBLIC ERR_LCD_E HX8347G_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{
 SCI_TraceLow_Test(" HX8347G_InvalidateRect");
 	left 	= (left >= LCD_WIDTH)    ? LCD_WIDTH-1 : left;
	right 	= (right >= LCD_WIDTH)   ? LCD_WIDTH-1 : right;
	top 	= (top >= LCD_HEIGHT)    ? LCD_HEIGHT-1 : top;
	bottom 	= (bottom >= LCD_HEIGHT) ? LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
    	return ERR_LCD_OPERATE_FAIL;
    }     
   
    HX8347G_set_display_window(left, top, right, bottom); 
    return ERR_LCD_NONE;
	
}


/**************************************************************************************/
// Description: refresh a rectangle of lcd
// Global resource dependence:
// Author: Jason Su
// Note:
//		left - the left value of the rectangel
//   	top - the top value of the rectangel
// 		right - the right value of the rectangel
//		bottom - the bottom value of the rectangel
/**************************************************************************************/
LOCAL ERR_LCD_E HX8347G_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	//int32 error;
	
SCI_TraceLow_Test(" HX8347G_RotationInvalidateRect");

	switch(angle)
	{
		case LCD_ANGLE_0:
			HX8347G_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			HX8347G_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			HX8347G_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			HX8347G_set_display_window(left, top, bottom,right);
			break;
		default:
			HX8347G_set_display_window(left, top, right, bottom);
			break;			
	}

	return ERR_LCD_NONE;
}//en of S6D0139_VerticalInvalidateRect	


/******************************************************************************/
//  Description:  set the contrast value 
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   HX8347G_SetContrast(
	uint16  contrast	//contrast value to set
	)
{
SCI_TraceLow_Test(" HX8347G_SetContrast");
	return ERR_LCD_FUNC_NOT_SUPPORT;
} 


/*****************************************************************************/
//  Description:    Set the brightness of LCD.
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/*****************************************************************************/
LOCAL ERR_LCD_E   HX8347G_SetBrightness(
	uint16 brightness	//birghtness to set
	)
{
SCI_TraceLow_Test(" HX8347G_SetBrightness");
	return ERR_LCD_FUNC_NOT_SUPPORT;
}

/*****************************************************************************/
//  Description:    Enable lcd to partial display mode, so can save power.
//	Global resource dependence: 
//  Author:         Jason Su
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//	Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E HX8347G_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
SCI_TraceLow_Test(" HX8347G_SetDisplayWindow");
	 	
	HX8347G_set_display_window(left, top, right, bottom);

	return ERR_LCD_NONE;
}

/******************************************************************************/
// Description: Invalidate Pixel
// Global resource dependence: 
// Author: Jason Su
// Note:
/******************************************************************************/
LOCAL void HX8347G_InvalidatePixel(uint16 x, uint16 y, uint32 data)
{
SCI_TraceLow_Test(" HX8347G_InvalidatePixel");
	HX8347G_InvalidateRect(x,y,x,y);
	LCD_DataWrite_HX8347G(data);
}


/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
const LCD_OPERATIONS_T HX8347G_spi_operations = 
{
	HX8347G_Init,
	HX8347G_EnterSleep,
	HX8347G_SetContrast,
	HX8347G_SetBrightness,
	HX8347G_SetDisplayWindow,
	HX8347G_InvalidateRect,
	HX8347G_Invalidate,
	HX8347G_Close,
	HX8347G_RotationInvalidateRect,
	HX8347G_SetDirection,
	NULL,
	NULL
};


LOCAL const LCD_TIMING_U s_HX8347G_timing =
{
	10,		// CS setup time for LCM read (optional)  
	150,	// low pulse width for LCM read (according spec)	
	100,	// high pulse width for LCM read (according spec)

	10,		// CS setup time for LCM write  (optional) 	
	35,		// low pulse width for LCM write (according spec)
	35,	    // high pulse width for LCM write (according spec)		
};

LOCAL const LCD_TIMING_U s_HX8347G_spitiming =
{
    39000000,0,0,
/*    SPI_CLK_52MHZ,        // clk frequency support (unit:MHz)
    SPI_CLK_IDLE_LOW,     // CPOL: 0--SPI_CLK_IDLE_LOW, 1--SPI_CLK_IDLE_HIGH
    SPI_SAMPLING_RISING,  // CPHA: 0--SPI_SAMPLING_RISING,  1--SPI_SAMPLING_FALLING 
*/    
    8,                    // tx bit length: 8/16bits refer to lcm driver ic 
    0,
    0
};

const LCD_SPEC_T g_lcd_HX8347G_spi = 
{
    QVGA_LCD_WIDTH, 
    QVGA_LCD_HEIGHT, 
    SCI_NULL, 
    SCI_NULL,
	WIDTH_8,
    (LCD_TIMING_U*)&s_HX8347G_spitiming, 
    (LCD_OPERATIONS_T*)&HX8347G_spi_operations,
    0,
    0
};

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 


