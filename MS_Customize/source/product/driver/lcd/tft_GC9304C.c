/******************************************************************************
 ** File Name:     tft_GC9304C.c                                             *
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
 ** 02/102010      lichd    	    Create.
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
#define LCD_CtrlWrite_GC9304C( _cmd ) LCD_SendCmd( (_cmd ), 0 );

#define LCD_DataWrite_GC9304C( _data ) LCD_SendData( (_data), 0 );

#define LCD_CtrlData_GC9304C(cmd, data)  LCD_CtrlWrite_GC9304C( cmd ); LCD_DataWrite_GC9304C( data );

/*lint -restore*/
#define LCD_Delay( ticks) OS_TickDelay( ticks );

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
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void GC9304C_Close(void);

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  GC9304C_EnterSleep(BOOLEAN is_sleep);

/*********************************************************************/
//  Description:   Initialize color LCD : GC9304
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E GC9304C_Init(void);

LOCAL ERR_LCD_E GC9304C_Invalidate(void);
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
LOCAL ERR_LCD_E GC9304C_InvalidateRect(
    uint16 left, //the left value of the rectangel
    uint16 top, //top of the rectangle
    uint16 right, //right of the rectangle
    uint16 bottom //bottom of the rectangle
    );

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void GC9304C_set_display_window(
    uint16 left, // start Horizon address
    uint16 right, // end Horizon address
    uint16 top, // start Vertical address
    uint16 bottom // end Vertical address
    );

LOCAL uint32 GC9304C_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id);

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

LOCAL ERR_LCD_E GC9304C_SetDirection(LCD_DIRECT_E direct_type)
{

    SCI_TraceLow_Test("GC9304C_SetDirection = %d",direct_type);

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

    s_lcd_madc.bitsval.mx = ~s_lcd_madc.bitsval.mx;
    s_lcd_madc.bitsval.rgb = 1;
    LCD_CtrlWrite_GC9304C(0x36);
    LCD_DataWrite_GC9304C(s_lcd_madc.dataval);
    LCD_CtrlWrite_GC9304C(0x002c);
    return ERR_LCD_NONE;

}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//	Note:
/******************************************************************************/
LOCAL void GC9304C_set_display_window(
    uint16 left, // start Horizon address
    uint16 top, // start Vertical address
    uint16 right, // end Horizon address
    uint16 bottom // end Vertical address
    )
{   
    uint16 newleft   = left;
    uint16 newtop    = top;
    uint16 newright  = right;
    uint16 newbottom = bottom;
    SCI_TraceLow_Test("[DRV_LCD]GC9304C_set_display_window L = %d, top = %d, R = %d, bottom = %d",left,top,right,bottom);

    LCD_CtrlWrite_GC9304C(0x2a);	// set hori start , end (left, right)
    LCD_DataWrite_GC9304C((newleft>>8)&0xff); // left high 8 b
    LCD_DataWrite_GC9304C(newleft&0xff); // left low 8 b
    LCD_DataWrite_GC9304C((newright>>8)&0xff); // right high 8 b
    LCD_DataWrite_GC9304C(newright&0xff); // right low 8 b

    LCD_CtrlWrite_GC9304C(0x2b);	// set vert start , end (top, bot)
    LCD_DataWrite_GC9304C((newtop>>8)&0xff); // top high 8 b
    LCD_DataWrite_GC9304C(newtop&0xff); // top low 8 b
    LCD_DataWrite_GC9304C((newbottom>>8)&0xff); // bot high 8 b
    LCD_DataWrite_GC9304C(newbottom&0xff); // bot low 8 b
    LCD_CtrlWrite_GC9304C(0x2c);	 // recover memory write mode

}

/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E GC9304C_Init(void)
{
    SCI_TraceLow_Test("GC9304C_Init");

    LCD_CtrlWrite_GC9304C(0x11); // sleep out
    //--------------------------------display control setting----------------------------------------//
    LCD_CtrlWrite_GC9304C(0xfe); // inter register enable1
    LCD_CtrlWrite_GC9304C(0xef); // inter register enable2
    LCD_CtrlWrite_GC9304C(0x36);  // memory access control
    LCD_DataWrite_GC9304C(0x48);
#ifdef MAINLCM_INTERFACE_SPI
    #if defined(LCD_SPI_3WIRE_9BIT_2DATA) || defined(LCD_SPI_4WIRE_8BIT_2DATA)
    LCD_CtrlWrite_GC9304C(0xe9);
    LCD_DataWrite_GC9304C(0x08);	//enable two dataline
    #endif
#endif
    LCD_CtrlWrite_GC9304C(0x3a); // pixel format set
    LCD_DataWrite_GC9304C(0x55);
    //LCD_DataWrite_GC9304C(0x66);	//rgb666

    LCD_CtrlWrite_GC9304C(0xe8);  //frame rate
    LCD_DataWrite_GC9304C(0x1f); // 22.5hz
    LCD_DataWrite_GC9304C(0x40); 
    LCD_CtrlWrite_GC9304C(0x35); // TE ON
    LCD_DataWrite_GC9304C(0x01);
    LCD_CtrlWrite_GC9304C(0x44); // tear scan line
    LCD_DataWrite_GC9304C(0x00);
    LCD_DataWrite_GC9304C(0x08);

    //------------------------------end display control setting--------------------------------//
    //------------------------------Power Control Registers Initial------------------------------//
    #if 1
    LCD_CtrlWrite_GC9304C(0xe3);
    LCD_DataWrite_GC9304C(0x01);
    LCD_DataWrite_GC9304C(0x04);

    LCD_CtrlWrite_GC9304C(0xa5);
    LCD_DataWrite_GC9304C(0x40);
    LCD_DataWrite_GC9304C(0x40);
    LCD_CtrlWrite_GC9304C(0xa4);
    LCD_DataWrite_GC9304C(0x44);
    LCD_DataWrite_GC9304C(0x44);

    LCD_CtrlWrite_GC9304C(0xab);
    LCD_DataWrite_GC9304C(0x08);
    LCD_CtrlWrite_GC9304C(0xaa);
    LCD_DataWrite_GC9304C(0x88);
    LCD_DataWrite_GC9304C(0x88);
    LCD_CtrlWrite_GC9304C(0xae);
    LCD_DataWrite_GC9304C(0x0b);
    LCD_CtrlWrite_GC9304C(0xac);
    LCD_DataWrite_GC9304C(0x00);
    LCD_CtrlWrite_GC9304C(0xaf);
    LCD_DataWrite_GC9304C(0x77);
    LCD_CtrlWrite_GC9304C(0xad);
    LCD_DataWrite_GC9304C(0x77);
    #endif
    //------------------------end Power Control Registers Initial------------------------------//

    //----------------------------display window 240X320------------------------------------//
    #if 0
    LCD_CtrlWrite_GC9304C(0x2a);
    LCD_DataWrite_GC9304C(0x00);
    LCD_DataWrite_GC9304C(0x00);
    LCD_DataWrite_GC9304C(0x00);
    LCD_DataWrite_GC9304C(0xef);
    LCD_CtrlWrite_GC9304C(0x2b);
    LCD_DataWrite_GC9304C(0x00);
    LCD_DataWrite_GC9304C(0x00);
    LCD_DataWrite_GC9304C(0x01);
    LCD_DataWrite_GC9304C(0x3f);
    LCD_CtrlWrite_GC9304C(0x2c);
    #endif
    //----------------------------------end display window ----------------------------------------//
    //----------------------------------------gamma setting-----------------------------------------//
    LCD_CtrlWrite_GC9304C(0xf0); // set gamma1
    LCD_DataWrite_GC9304C(0x02);
    LCD_DataWrite_GC9304C(0x00);
    LCD_DataWrite_GC9304C(0x00);
    LCD_DataWrite_GC9304C(0x00);
    LCD_DataWrite_GC9304C(0x03);
    LCD_DataWrite_GC9304C(0x08);
    LCD_CtrlWrite_GC9304C(0xf1); // set gamma2
    LCD_DataWrite_GC9304C(0x01);
    LCD_DataWrite_GC9304C(0x03);
    LCD_DataWrite_GC9304C(0x00);
    LCD_DataWrite_GC9304C(0x00);
    LCD_DataWrite_GC9304C(0x05);
    LCD_DataWrite_GC9304C(0x0c);
    LCD_CtrlWrite_GC9304C(0xf2); // set gamma3
    LCD_DataWrite_GC9304C(0x0a);
    LCD_DataWrite_GC9304C(0x07);
    LCD_DataWrite_GC9304C(0x32);
    LCD_DataWrite_GC9304C(0x05);
    LCD_DataWrite_GC9304C(0x05);
    LCD_DataWrite_GC9304C(0x46);
    LCD_CtrlWrite_GC9304C(0xf3); // set gamma4
    LCD_DataWrite_GC9304C(0x0d);
    LCD_DataWrite_GC9304C(0x09);
    LCD_DataWrite_GC9304C(0x3a);
    LCD_DataWrite_GC9304C(0x04);
    LCD_DataWrite_GC9304C(0x04);
    LCD_DataWrite_GC9304C(0x4b);
    LCD_CtrlWrite_GC9304C(0xf4); // set gamma5
    LCD_DataWrite_GC9304C(0x0e);
    LCD_DataWrite_GC9304C(0x17);
    LCD_DataWrite_GC9304C(0x15);
    LCD_DataWrite_GC9304C(0x1b);
    LCD_DataWrite_GC9304C(0x1e);
    LCD_DataWrite_GC9304C(0x00);
    LCD_CtrlWrite_GC9304C(0xf5); // set gamma6
    LCD_DataWrite_GC9304C(0x0c);
    LCD_DataWrite_GC9304C(0x1a);
    LCD_DataWrite_GC9304C(0x1c);
    LCD_DataWrite_GC9304C(0x17);
    LCD_DataWrite_GC9304C(0x1a);
    LCD_DataWrite_GC9304C(0x00);
    //------------------------------------end gamma setting-----------------------------------------//

    LCD_CtrlWrite_GC9304C(0x29); // display on
    //LCD_CtrlWrite_GC9304C(0x2c);

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  GC9304C_EnterSleep(
    BOOLEAN is_sleep //SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
    )
{
    SCI_TraceLow("GC9304_EnterSleep, is_sleep = %d", is_sleep);

    if(is_sleep)
    {
        LCD_CtrlWrite_GC9304C(0x28);
        LCD_Delay(120);
        LCD_CtrlWrite_GC9304C(0x10);
        LCD_Delay(150);
    }
    else
    {
        LCD_CtrlWrite_GC9304C(0x11);
        LCD_Delay(120);
        LCD_CtrlWrite_GC9304C(0x29);
    }

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void GC9304C_Close(void)
{
    SCI_TraceLow("LCD: in GC9304_Close");
    GC9304C_EnterSleep( SCI_TRUE );
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E GC9304C_Invalidate(void)
{
    GC9304C_set_display_window (0x0, 0x0, LCD_WIDTH - 1, LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E GC9304C_InvalidateRect(
    uint16 left, //the left value of the rectangel
    uint16 top, //top of the rectangle
    uint16 right, //right of the rectangle
    uint16 bottom //bottom of the rectangle
    )
{
    left = (left >=LCD_WIDTH) ? LCD_WIDTH-1 : left;
    right = (right >= LCD_WIDTH) ? LCD_WIDTH-1 : right;
    top = (top >= LCD_HEIGHT) ? LCD_HEIGHT-1 : top;
    bottom = (bottom >= LCD_HEIGHT) ? LCD_HEIGHT-1 : bottom;

    if ( ( right < left ) || ( bottom < top ) )
    {
        return ERR_LCD_OPERATE_FAIL;
    }     

    GC9304C_set_display_window(left, top, right, bottom);
    return ERR_LCD_NONE;
}

LOCAL ERR_LCD_E GC9304C_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
    SCI_TraceLow_Test(" GC9304C_RotationInvalidateRect");
    switch(angle)
    {
        case LCD_ANGLE_0:
            GC9304C_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_90:
            GC9304C_set_display_window(left, top, bottom,right);
            break;
        case LCD_ANGLE_180:
            GC9304C_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_270:
            GC9304C_set_display_window(left, top, bottom,right);
            break;
        default:
            GC9304C_set_display_window(left, top, right, bottom);
            break;
    }
    return ERR_LCD_NONE;
}

LOCAL ERR_LCD_E   GC9304C_SetContrast(uint16  contrast)
{
    SCI_TraceLow_Test(" GC9304C_SetContrast");
    return ERR_LCD_FUNC_NOT_SUPPORT;
} 
/*****************************************************************************/
LOCAL ERR_LCD_E   GC9304C_SetBrightness(uint16 brightness)
{
    SCI_TraceLow_Test(" GC9304C_SetBrightness");
    return ERR_LCD_FUNC_NOT_SUPPORT;
}

//  Description:    Enable lcd to partial display mode, so can save power.
//	Global resource dependence: 
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//	Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E GC9304C_SetDisplayWindow(
    uint16 left, //left of the window
    uint16 top, //top of the window
    uint16 right, //right of the window
    uint16 bottom //bottom of the window
    )
{
    SCI_TraceLow_Test(" GC9304C_SetDisplayWindow");
    GC9304C_set_display_window(left, top, right, bottom);
    return ERR_LCD_NONE;
}

LOCAL void GC9304C_InvalidatePixel(uint16 x, uint16 y, uint32 data)
{
    SCI_TraceLow_Test(" GC9304C_InvalidatePixel");
    GC9304C_InvalidateRect(x,y,x,y);
    LCD_DataWrite_GC9304C(data);
}

LOCAL uint32 GC9304C_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint32 id[4] = {0};

#ifdef MAINLCM_INTERFACE_LCM
    LCM_SendCmd(0x04, lcd_id);
    id[0] = LCM_Read (lcd_id, DATA_LEVEL) & 0xFF; //dummy data
    id[1] = LCM_Read (lcd_id, DATA_LEVEL) & 0xFF; // 0x00, cmd 0xda
    id[2] = LCM_Read (lcd_id, DATA_LEVEL) & 0xFF; // 0x93, cmd 0xdb
    id[3] = LCM_Read (lcd_id, DATA_LEVEL) & 0xFF; // 0x04, cmd 0xdc

    SCI_TraceLow("GC9304_ReadID LCM: 0x%0x, 0x%x, 0x%0x\n", id[1], id[2], id[3]);
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

    SCI_TraceLow("GC9304C_ReadID SPI: 0x%0x, 0x%x, 0x%0x, id = 0x%x\n", id[1], id[2], id[3], (id[2]<<8) | id[3]);
    return ((id[2]<<8) | id[3]); //0x9304
#else
    return 0;
#endif
#endif
}

const LCD_OPERATIONS_T GC9304C_operations = 
{
    GC9304C_Init,
    GC9304C_EnterSleep,
    GC9304C_SetContrast,
    GC9304C_SetBrightness,
    GC9304C_SetDisplayWindow,
    GC9304C_InvalidateRect,
    GC9304C_Invalidate,
    GC9304C_Close,
    GC9304C_RotationInvalidateRect,
    GC9304C_SetDirection,
    NULL, 
    GC9304C_ReadID	
};

LOCAL const LCD_TIMING_U s_GC9304C_lcmtiming =
{
    // LCM_CYCLE_U start(ns)
    5,  // CS setup time for LCM read (optional)  
    150, // low pulse width for LCM read (according spec)	
    150, // high pulse width for LCM read (according spec)
    10,   // CS setup time for LCM write  (optional) 	
    50,  // low pulse width for LCM write (according spec)
    50,  // high pulse width for LCM write (according spec)	
    // LCM_CYCLE_U end(ns)
};

LOCAL const LCD_TIMING_U s_GC9304C_spitiming =
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

#ifdef MAINLCM_INTERFACE_SPI
const LCD_SPEC_T g_lcd_GC9304C = 
{
    LCD_WIDTH, 
    LCD_HEIGHT, 
    SCI_NULL, 
    SCI_NULL,
    WIDTH_8,
    (LCD_TIMING_U*)&s_GC9304C_spitiming, 
    (LCD_OPERATIONS_T*)&GC9304C_operations,
    1,
    0
};
#else // not verify lcm
const LCD_SPEC_T g_lcd_GC9304C = 
{
    LCD_WIDTH, 
    LCD_HEIGHT, 
    LCD_MCU, 
    BUS_MODE_8080,
    WIDTH_8,
    (LCD_TIMING_U*)&s_GC9304C_lcmtiming, 
    (LCD_OPERATIONS_T*)&GC9304C_operations,
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

