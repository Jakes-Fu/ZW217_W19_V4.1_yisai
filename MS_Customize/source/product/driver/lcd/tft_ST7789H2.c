/******************************************************************************
 ** File Name:     TFT_ST7789H2_spi.c                                             *
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
#include "lcm_drvapi.h"
#include "spi_drvapi.h"
    

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
#define LCD_CtrlWrite_ST7789H2( _cmd )\
				LCD_SendCmd( (_cmd ), 0 );

#define LCD_DataWrite_ST7789H2( _data )\
				LCD_SendData( (_data), 0 );
#else
#define LCD_CtrlWrite_ST7789H2( _cmd ) LCD_SendCmd( (_cmd ), 0 );
#define LCD_DataWrite_ST7789H2( _data ) LCD_SendData( (_data>>8), 0 );\
                                       LCD_SendData( (_data&0xFF),0 );    
#endif

#define LCD_CtrlData_ST7789H2(cmd, data)  LCD_CtrlWrite_ST7789H2( cmd );  \
                                          LCD_DataWrite_ST7789H2( data );

/*lint -restore*/
#define LCD_Delay( ticks)\
				SCI_Sleep( ticks );
#define ST7789H2_SUPPORT_WIDTH   240				
#define ST7789H2_SUPPORT_HEIGHT  320				
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;

#ifdef CHIP_ENDIAN_LITTLE
typedef union {
	struct {
		unsigned int resv   :2;
		unsigned int mh    :1;	// display data latch order
		unsigned int rgb    :1;	// rgb order
		unsigned int ml    :1;		// line address order
		unsigned int mv    :1;	// x/y exchanged
		unsigned int mx    :1;	// horizontal mirror
		unsigned int my    :1;	// vertical mirror 
	} bitsval;
	unsigned int dataval;
} MEM_DATA_ACCESS_CTRL_U;
#else
typedef union {
	struct {
		unsigned int my    :1;	// vertical mirror 
		unsigned int mx    :1;	// horizontal mirror
		unsigned int mv    :1;	// x/y exchanged
		unsigned int ml    :1;		// line address order
		unsigned int rgb    :1;	// rgb order
		unsigned int mh    :1;	// display data latch order
		unsigned int resv   :2;
	} bitsval;
	unsigned int dataval;
} MEM_DATA_ACCESS_CTRL_U;
#endif // CHIP_ENDIAN_LITTLE

LOCAL MEM_DATA_ACCESS_CTRL_U s_lcd_madc;

  /******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL void ST7789H2_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ST7789H2_EnterSleep(
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
LOCAL ERR_LCD_E ST7789H2_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ST7789H2_Invalidate(void);


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
LOCAL ERR_LCD_E ST7789H2_InvalidateRect(
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
LOCAL void ST7789H2_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 right, 	// end Horizon address
	uint16 top, 		// start Vertical address
	uint16 bottom	// end Vertical address
	);

LOCAL uint32 ST7789H2_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id);
	
/******************************************************************************/
//  Description:  Set LCD invalidate direction
//  Input:
//      is_invert: 0, horizontal; 1, vertical
//  Return:
//      None.
//	Note: Application should correct invalidate direction right after current
//	 	  image displayed
/******************************************************************************/
LOCAL ERR_LCD_E ST7789H2_SetDirection(
	LCD_DIRECT_E direct_type
	)
{

    SCI_TraceLow_Test(" ST7789H2_SetDirection = %d",direct_type);

    // dirction setting base on address control (8.12), with cmd address 0x36
    switch(direct_type)
    {
        case LCD_DIRECT_NORMAL:
            s_lcd_madc.bitsval.mx = 0;
            s_lcd_madc.bitsval.my = 0;
            s_lcd_madc.bitsval.mv = 0;
            break;
        case LCD_DIRECT_ROT_90:
            s_lcd_madc.bitsval.mx = 1;
            s_lcd_madc.bitsval.my = 0;
            s_lcd_madc.bitsval.mv = 1;
            break;
        case LCD_DIRECT_ROT_180:
            s_lcd_madc.bitsval.mx = 1;
            s_lcd_madc.bitsval.my = 1;
            s_lcd_madc.bitsval.mv = 0;
            break;
        case LCD_DIRECT_ROT_270:
            s_lcd_madc.bitsval.mx = 0;
            s_lcd_madc.bitsval.my = 1;
            s_lcd_madc.bitsval.mv = 1;
            break;
        case LCD_DIRECT_MIR_H:
            s_lcd_madc.bitsval.mx = 1;
            s_lcd_madc.bitsval.my = 0;
            s_lcd_madc.bitsval.mv = 0;
            break;
        case LCD_DIRECT_MIR_V:
            s_lcd_madc.bitsval.mx = 0;
            s_lcd_madc.bitsval.my = 1;
            s_lcd_madc.bitsval.mv = 0;
            break;
        case LCD_DIRECT_MIR_HV:
            s_lcd_madc.bitsval.mx = 1;
            s_lcd_madc.bitsval.my = 1;
            s_lcd_madc.bitsval.mv = 0;
            break;
        default:
            s_lcd_madc.bitsval.mx = 0;
            s_lcd_madc.bitsval.my = 0;
            s_lcd_madc.bitsval.mv = 0;
            break;
    }

    LCD_CtrlWrite_ST7789H2(0x0036);
    LCD_DataWrite_ST7789H2(s_lcd_madc.dataval);
	
    LCD_CtrlWrite_ST7789H2(0x002c);

    s_lcd_direct = direct_type;
    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL void ST7789H2_set_display_window(
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
	SCI_TraceLow_Test("[DRV_LCD]ST7789H2_set_display_window L = %d, top = %d, R = %d, bottom = %d",left,top,right,bottom);
	SCI_TraceLow_Test("[DRV_LCD]ST7789H2_set_display_window s_lcd_direct = %d",s_lcd_direct);

	LCD_CtrlWrite_ST7789H2(0x2a);	// set hori start , end (left, right)
	LCD_DataWrite_ST7789H2((newleft>>8)&0xff);	// left high 8 b
	LCD_DataWrite_ST7789H2(newleft&0xff);		// left low 8 b
	LCD_DataWrite_ST7789H2((newright>>8)&0xff);	// right high 8 b
	LCD_DataWrite_ST7789H2(newright&0xff);		// right low 8 b

	LCD_CtrlWrite_ST7789H2(0x2b);	// set vert start , end (top, bot)
	LCD_DataWrite_ST7789H2((newtop>>8)&0xff);	// top high 8 b
	LCD_DataWrite_ST7789H2(newtop&0xff);		// top low 8 b
	LCD_DataWrite_ST7789H2((newbottom>>8)&0xff);	// bot high 8 b
	LCD_DataWrite_ST7789H2(newbottom&0xff);	// bot low 8 b
	LCD_CtrlWrite_ST7789H2(0x2c);		// recover memory write mode

}

LOCAL void ST7789H2_driver(void)
{
	SCI_TraceLow_Test("[REF_LCD]ST7789H2_driver");
	LCD_CtrlWrite_ST7789H2(0x11);	// Sleep out
	//LCD_Delay(150);

	LCD_CtrlWrite_ST7789H2(0x36);	// Memory data access control
	LCD_DataWrite_ST7789H2(0x00);

	LCD_CtrlWrite_ST7789H2(0x3a);	// interface pixedl format
	LCD_DataWrite_ST7789H2(0x55);	//RGB565, 16 bits, 65k rgb?
#ifdef MAINLCM_INTERFACE_SPI
	#if defined(LCD_SPI_3WIRE_9BIT_2DATA) || defined(LCD_SPI_4WIRE_8BIT_2DATA)
	LCD_CtrlWrite_ST7789H2(0xe7);
	LCD_DataWrite_ST7789H2(0x10);	//enable two dataline
	#endif
#endif
	LCD_CtrlWrite_ST7789H2(0xb2);	// porch setting
	LCD_DataWrite_ST7789H2(0x0c); //default:0x0c	0x7f// back porch
	LCD_DataWrite_ST7789H2(0x0c); //default:0x0c	0x7f//front porch
	LCD_DataWrite_ST7789H2(0x00);	// Disable seperate porch control
	LCD_DataWrite_ST7789H2(0x33);	// B/F porch in idle mode
	LCD_DataWrite_ST7789H2(0x33);	// B/F porch in partial mode?

	LCD_CtrlWrite_ST7789H2(0xb7);	// gate control
	LCD_DataWrite_ST7789H2(0x35);	// vgh = 13.26v, vgl = -10.43v


	LCD_CtrlWrite_ST7789H2(0xbb);	// VCOM setting
	LCD_DataWrite_ST7789H2(0x35);	// VCOM=1.425v

	LCD_CtrlWrite_ST7789H2(0xc0);	// LCM setting?
	LCD_DataWrite_ST7789H2(0x2c);

	LCD_CtrlWrite_ST7789H2(0xc2);	// VDV VRH command enable??
	LCD_DataWrite_ST7789H2(0x01);

	LCD_CtrlWrite_ST7789H2(0xc3);	// VRH set
	LCD_DataWrite_ST7789H2(0x0b);	// VAP(GVDD) = 4.1+

	LCD_CtrlWrite_ST7789H2(0xc4);	// VDV set
	LCD_DataWrite_ST7789H2(0x20);

	LCD_CtrlWrite_ST7789H2(0xc6);	// frame rate control
	LCD_DataWrite_ST7789H2(0x0b); // 0x1f, 39hz; 0x0f, 60hz; 0x0B,69hz

	LCD_CtrlWrite_ST7789H2(0xd0);	// power control 1
	LCD_DataWrite_ST7789H2(0xa4);	// fixed
	LCD_DataWrite_ST7789H2(0xa1);	// AVDD=6.8, AVCL=-4.8, VDS=2.3


	LCD_CtrlWrite_ST7789H2(0xe0);	// positive voltage gamma control
	LCD_DataWrite_ST7789H2(0xd0);
	LCD_DataWrite_ST7789H2(0x00);
	LCD_DataWrite_ST7789H2(0x02);
	LCD_DataWrite_ST7789H2(0x07);
	LCD_DataWrite_ST7789H2(0x0b);
	LCD_DataWrite_ST7789H2(0x1a);
	LCD_DataWrite_ST7789H2(0x31);
	LCD_DataWrite_ST7789H2(0x54);
	LCD_DataWrite_ST7789H2(0x40);
	LCD_DataWrite_ST7789H2(0x29);
	LCD_DataWrite_ST7789H2(0x12);
	LCD_DataWrite_ST7789H2(0x12);
	LCD_DataWrite_ST7789H2(0x12);
	LCD_DataWrite_ST7789H2(0x17);

	LCD_CtrlWrite_ST7789H2(0xe1);	// negative voltage gamma control
	LCD_DataWrite_ST7789H2(0xd0);
	LCD_DataWrite_ST7789H2(0x00);
	LCD_DataWrite_ST7789H2(0x02);
	LCD_DataWrite_ST7789H2(0x07);
	LCD_DataWrite_ST7789H2(0x05);
	LCD_DataWrite_ST7789H2(0x25);
	LCD_DataWrite_ST7789H2(0x2d);
	LCD_DataWrite_ST7789H2(0x44);
	LCD_DataWrite_ST7789H2(0x45);
	LCD_DataWrite_ST7789H2(0x1c);
	LCD_DataWrite_ST7789H2(0x18);
	LCD_DataWrite_ST7789H2(0x16);
	LCD_DataWrite_ST7789H2(0x1c);
	LCD_DataWrite_ST7789H2(0x1d);

	//Set_Tear_Scanline
	LCD_CtrlWrite_ST7789H2(0x44);
	LCD_DataWrite_ST7789H2(0x00);
	LCD_DataWrite_ST7789H2(0x08);

	//enable fmark ST
	LCD_CtrlWrite_ST7789H2(0x35);
	LCD_DataWrite_ST7789H2(0x01);

	LCD_CtrlWrite_ST7789H2(0x29);
#if 0
	LCD_CtrlWrite_ST7789H2(0x2a);		// set hori start and end , 16 bits each 
	LCD_DataWrite_ST7789H2(0x00);
	LCD_DataWrite_ST7789H2(0x00);
	LCD_DataWrite_ST7789H2(0x00);
	LCD_DataWrite_ST7789H2(0xef);

	LCD_CtrlWrite_ST7789H2(0x2b);		// set vert start and end, each 16 bits
	LCD_DataWrite_ST7789H2(0x00);
	LCD_DataWrite_ST7789H2(0x00);
	LCD_DataWrite_ST7789H2(0x01);
	LCD_DataWrite_ST7789H2(0x3f);

	LCD_CtrlWrite_ST7789H2(0x2c);
#endif

	return;
}

/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jason Su
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E ST7789H2_Init(void)
{
	SCI_TraceLow_Test(" ST7789H2_Init");
	ST7789H2_driver();
	//ST7789H2_SetDirection(LCD_DIRECT_NORMAL);  mengzhao
	return 0;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ST7789H2_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
    SCI_TraceLow_Test(" ST7789H2_EnterSleep:%d", is_sleep);

    if(is_sleep)
    {
        LCD_CtrlWrite_ST7789H2(0x28);
        LCD_Delay(120);
        LCD_CtrlWrite_ST7789H2(0x10);
        LCD_Delay(150);
    }
    else
    {
        LCD_CtrlWrite_ST7789H2(0x11);
        LCD_Delay(120);
        LCD_CtrlWrite_ST7789H2(0x29);
    }

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL void ST7789H2_Close(void)
{
    //GPIO_SetLcdBackLight( SCI_FALSE );
    SCI_TraceLow_Test(" ST7789H2_Close");
    ST7789H2_EnterSleep( SCI_TRUE );
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ST7789H2_Invalidate(void)
{
	SCI_TraceLow_Test(" ST7789H2_Invalidate");

	ST7789H2_set_display_window (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E ST7789H2_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{
 SCI_TraceLow_Test(" ST7789H2_InvalidateRect");
 	left 	= (left >= LCD_WIDTH)    ? LCD_WIDTH-1 : left;
	right 	= (right >= LCD_WIDTH)   ? LCD_WIDTH-1 : right;
	top 	= (top >= LCD_HEIGHT)    ? LCD_HEIGHT-1 : top;
	bottom 	= (bottom >= LCD_HEIGHT) ? LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
    	return ERR_LCD_OPERATE_FAIL;
    }     
   
    ST7789H2_set_display_window(left, top, right, bottom); 
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
LOCAL ERR_LCD_E ST7789H2_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	//int32 error;

	SCI_TraceLow_Test(" ST7789H2_RotationInvalidateRect");

	switch(angle)
	{
		case LCD_ANGLE_0:
			ST7789H2_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			ST7789H2_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			ST7789H2_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			ST7789H2_set_display_window(left, top, bottom,right);
			break;
		default:
			ST7789H2_set_display_window(left, top, right, bottom);
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
LOCAL ERR_LCD_E   ST7789H2_SetContrast(
	uint16  contrast	//contrast value to set
	)
{
	SCI_TraceLow_Test(" ST7789H2_SetContrast");
	return ERR_LCD_FUNC_NOT_SUPPORT;
} 


/*****************************************************************************/
//  Description:    Set the brightness of LCD.
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/*****************************************************************************/
LOCAL ERR_LCD_E   ST7789H2_SetBrightness(
	uint16 brightness	//birghtness to set
	)
{
	SCI_TraceLow_Test(" ST7789H2_SetBrightness");
	return ERR_LCD_FUNC_NOT_SUPPORT;
}

/*****************************************************************************/
//  Description:    Enable lcd to partial display mode, so can save power.
//	Global resource dependence: 
//  Author:         Jason Su
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//	Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E ST7789H2_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	SCI_TraceLow_Test(" ST7789H2_SetDisplayWindow");
		
	ST7789H2_set_display_window(left, top, right, bottom);

	return ERR_LCD_NONE;
}

/******************************************************************************/
// Description: Invalidate Pixel
// Global resource dependence: 
// Author: Jason Su
// Note:
/******************************************************************************/
LOCAL void ST7789H2_InvalidatePixel(uint16 x, uint16 y, uint32 data)
{
	SCI_TraceLow_Test(" ST7789H2_InvalidatePixel");
	ST7789H2_InvalidateRect(x,y,x,y);
	LCD_DataWrite_ST7789H2(data);
}
LOCAL uint32 ST7789H2_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint32 id[4] = {0};

#ifdef MAINLCM_INTERFACE_LCM
    LCM_SendCmd(0x04, lcd_id);
    id[0] = LCM_Read (lcd_id, DATA_LEVEL) & 0xFF; //dummy data
    id[1] = LCM_Read (lcd_id, DATA_LEVEL) & 0xFF; // 0x85, cmd 0xda
    id[2] = LCM_Read (lcd_id, DATA_LEVEL) & 0xFF; // 0x85, cmd 0xdb
    id[3] = LCM_Read (lcd_id, DATA_LEVEL) & 0xFF; // 0x52, cmd 0xdc

    SCI_TraceLow("ST7789H2_ReadID LCM: 0x%0x, 0x%x, 0x%0x\n", id[1], id[2], id[3]);
    return ((id[2] <<8)| id[3]);
#else
#ifdef CHIP_VER_6531EFM
#ifdef FPGA_VERIFICATION
    SPI_SetClkDiv(3); // fpga spi clk 26M
#else
    SPI_SetClkDiv(9); // asic ahb spi 104M
#endif
    #if defined(LCD_SPI_3WIRE_9BIT_1DATA) || defined(LCD_SPI_3WIRE_9BIT_2DATA)
    SPI_SetMode( SPI_3WIRE_9BIT_RW);
    #else
    SPI_SetMode( SPI_4WIRE_8BIT_RW);
    #endif
    SPI_SetDatawidth(16);
    SPI_SetReadS(7);
    SPI_SetRWMode();
    SPI_SetCd(0);

    SPI_SetCsLow(0, TRUE);
    id[1] = SPI_WriteReadData( (0xDA<<8), 1, 0) & 0xFF;
    SPI_SetCsLow(0, FALSE);

    SPI_SetCsLow(0, TRUE);
    id[2] = SPI_WriteReadData( (0xDB<<8), 1, 0) & 0xFF;
    SPI_SetCsLow(0, FALSE);

    SPI_SetCsLow(0, TRUE);
    id[3]= SPI_WriteReadData( (0xDC<<8), 1, 0) & 0xFF;
    SPI_SetCsLow(0, FALSE);
    SPI_SetClkDiv(0);

    SCI_TraceLow("ST7789H2_ReadID SPI: 0x%0x, 0x%x, 0x%0x\n", id[1], id[2], id[3]);

    return ((id[2] <<8)| id[3]);
#else
    return 0;
#endif
#endif

}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jason Su
//	Note:
/******************************************************************************/
const LCD_OPERATIONS_T ST7789H2_operations = 
{
	ST7789H2_Init,
	ST7789H2_EnterSleep,
	ST7789H2_SetContrast,
	ST7789H2_SetBrightness,
	ST7789H2_SetDisplayWindow,
	ST7789H2_InvalidateRect,
	ST7789H2_Invalidate,
	ST7789H2_Close,
	ST7789H2_RotationInvalidateRect,
	ST7789H2_SetDirection,
	NULL, 
	ST7789H2_ReadID
};


LOCAL const LCD_TIMING_U s_ST7789H2_timing =
{
	10,		// CS setup time for LCM read (optional)  
	150,	// low pulse width for LCM read (according spec)	
	100,	// high pulse width for LCM read (according spec)

	10,		// CS setup time for LCM write  (optional) 	
	35,		// low pulse width for LCM write (according spec)
	35,	    // high pulse width for LCM write (according spec)		
};

LOCAL const LCD_TIMING_U s_ST7789H2_spitiming =
{
#ifdef FPGA_VERIFICATION
    39000000,
#else
    52000000,
#endif
    0,0,
/*    SPI_CLK_52MHZ,        // clk frequency support (unit:MHz)
    SPI_CLK_IDLE_LOW,     // CPOL: 0--SPI_CLK_IDLE_LOW, 1--SPI_CLK_IDLE_HIGH
    SPI_SAMPLING_RISING,  // CPHA: 0--SPI_SAMPLING_RISING,  1--SPI_SAMPLING_FALLING 
*/    
    8,                    // tx bit length: 8/16bits refer to lcm driver ic 
    0,
    0
};

LOCAL const LCD_TIMING_U s_ST7789H2_lcmtiming =
{
	        // LCM_CYCLE_U start(ns)
	50,		// CS setup time for LCM read (optional)  new
	55,	// low pulse width for LCM read (according spec)	
	110,	// high pulse width for LCM read (according spec)
	20,		// CS setup time for LCM write  (optional) 	
	70,		// low pulse width for LCM write (according spec)
	35,	    // high pulse width for LCM write (according spec)	
	        // LCM_CYCLE_U end(ns)		
};

#ifdef MAINLCM_INTERFACE_SPI
const LCD_SPEC_T g_lcd_ST7789H2 = 
{
    LCD_WIDTH, 
    LCD_HEIGHT, 
    SCI_NULL, 
    SCI_NULL,
    WIDTH_8,
    (LCD_TIMING_U*)&s_ST7789H2_spitiming, 
    (LCD_OPERATIONS_T*)&ST7789H2_operations,
    1,
    0
};
#else
const LCD_SPEC_T g_lcd_ST7789H2 = 
{
    LCD_WIDTH, 
    LCD_HEIGHT, 
    LCD_MCU, 
    BUS_MODE_8080,
    WIDTH_8,
    (LCD_TIMING_U*)&s_ST7789H2_lcmtiming, 
    (LCD_OPERATIONS_T*)&ST7789H2_operations,
    1,
    0
};
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 


