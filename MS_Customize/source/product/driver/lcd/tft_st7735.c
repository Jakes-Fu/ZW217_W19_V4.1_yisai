/******************************************************************************
 ** File Name:     tft_ST7735.c                                             *
 ** Description:															  *
 **    This file contains driver for color LCD.(CSTN)						  *
 ** Author:         sky.wang                                                 *
 ** DATE:           20/07/2018                                                *
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
#include "timer_drvapi.h"

#include "hal_gouda.h"
#include "ldo_drvapi.h"
#include "lcm_cfg.h"

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define ST7735_LCD_WIDTH 128
#define ST7735_LCD_HEIGHT 128

#define ST7735_X_OFFSET 0
#define ST7735_Y_OFFSET 32

#define SCI_TraceLow_Test SCI_TraceLow

LOCAL uint16 st7735_line_mode = 0;
LOCAL uint16 st7735_farmk = 0;


/*lint -save -e767 -e718*/
//#define LCD_CtrlWrite_ST7735( _cmd ) LCD_SendCmd( (_cmd ), 0 );

//#define LCD_DataWrite_ST7735( _data ) LCD_SendData( (_data), 0 );

void hal_GoudatResetLcdPin(void);


#define LCM_WR_REG(Addr, Data)                              \
    {                                                       \
        while (hal_GoudaWriteReg(Addr, Data) != HAL_ERR_NO) \
            ;                                               \
    }
#define LCD_DataWrite_ST7735(Data)                               \
    {                                                  \
        while (hal_GoudaWriteData(Data) != HAL_ERR_NO) \
            ;                                          \
    }
#define LCD_CtrlWrite_ST7735(Cmd)                              \
    {                                                \
        while (hal_GoudaWriteCmd(Cmd) != HAL_ERR_NO) \
            ;                                        \
    }

//#define LCD_CtrlData_ST7735(cmd, data)  LCD_CtrlWrite_ST7735( cmd ); LCD_DataWrite_ST7735( data );

/*lint -restore*/

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

extern void hal_LcdDelay (int ms_delay);

LOCAL MEM_DATA_ACCESS_CTRL_U s_lcd_madc;


/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ST7735_Close(void);

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ST7735_EnterSleep(BOOLEAN is_sleep);

/*********************************************************************/
//  Description:   Initialize color LCD : ST7735
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL void ST7735_Init(void);

LOCAL ERR_LCD_E ST7735_Invalidate(void);
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
LOCAL ERR_LCD_E ST7735_InvalidateRect(
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
LOCAL void ST7735_set_display_window(
    uint16 left, // start Horizon address
    uint16 right, // end Horizon address
    uint16 top, // start Vertical address
    uint16 bottom // end Vertical address
    );

LOCAL uint32 ST7735_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id);

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

LOCAL ERR_LCD_E ST7735_SetDirection(LCD_DIRECT_E direct_type)
{
   gouda_event(0x90001101);

    SCI_TraceLow_Test("ST7735_SetDirection = %d",direct_type);

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
    LCD_CtrlWrite_ST7735(0x36);
    LCD_DataWrite_ST7735(s_lcd_madc.dataval);
    LCD_CtrlWrite_ST7735(0x2c);
    return ERR_LCD_NONE;

}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//	Note:
/******************************************************************************/
LOCAL void ST7735_set_display_window(
    uint16 left, // start Horizon address
    uint16 top, // start Vertical address
    uint16 right, // end Horizon address
    uint16 bottom // end Vertical address
    )
{       
    uint16 newleft   = 0;
    uint16 newtop    = 0;
    uint16 newright  = 0;
    uint16 newbottom = 0;

    left = (left >=ST7735_LCD_WIDTH) ? ST7735_LCD_WIDTH-1 : left;
    right = (right >= ST7735_LCD_WIDTH) ? ST7735_LCD_WIDTH-1 : right;
    top = (top >= ST7735_LCD_HEIGHT) ? ST7735_LCD_HEIGHT-1 : top;
    bottom = (bottom >= ST7735_LCD_HEIGHT) ? ST7735_LCD_HEIGHT-1 : bottom;

    if ( ( right < left ) || ( bottom < top ) )
    {
        return;
    }
    
    newleft   = left+ST7735_X_OFFSET;
    newtop    = top+ST7735_Y_OFFSET;
    newright  = right+ST7735_X_OFFSET;
    newbottom = bottom+ST7735_Y_OFFSET;
   
    LCD_CtrlWrite_ST7735(0x2a);	// set hori start , end (left, right)
    LCD_DataWrite_ST7735((newleft>>8)&0xff); // left high 8 b
    LCD_DataWrite_ST7735(newleft&0xff); // left low 8 b
    LCD_DataWrite_ST7735((newright>>8)&0xff); // right high 8 b
    LCD_DataWrite_ST7735(newright&0xff); // right low 8 b

    LCD_CtrlWrite_ST7735(0x2b);	// set vert start , end (top, bot)
    LCD_DataWrite_ST7735((newtop>>8)&0xff); // top high 8 b
    LCD_DataWrite_ST7735(newtop&0xff); // top low 8 b
    LCD_DataWrite_ST7735((newbottom>>8)&0xff); // bot high 8 b
    LCD_DataWrite_ST7735(newbottom&0xff); // bot low 8 b
    LCD_CtrlWrite_ST7735(0x2c);	 // recover memory write mode

}

/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL void ST7735_Init(void)
{
#if 0
    hal_GoudatResetLcdPin();                

    hal_LcdDelay(120);                //ms

    LCD_CtrlWrite_ST7735(0x11);     

    hal_LcdDelay(120);                //ms

    LCD_CtrlWrite_ST7735(0xB1);     
    LCD_DataWrite_ST7735(0x05);   
    LCD_DataWrite_ST7735(0x3C);   
    LCD_DataWrite_ST7735(0x3C);   

    LCD_CtrlWrite_ST7735(0xB2);     
    LCD_DataWrite_ST7735(0x05);   
    LCD_DataWrite_ST7735(0x3C);   
    LCD_DataWrite_ST7735(0x3C);   

    LCD_CtrlWrite_ST7735(0xB3);     
    LCD_DataWrite_ST7735(0x05);   
    LCD_DataWrite_ST7735(0x3C);   
    LCD_DataWrite_ST7735(0x3C);   
    LCD_DataWrite_ST7735(0x05);   
    LCD_DataWrite_ST7735(0x3C);   
    LCD_DataWrite_ST7735(0x3C);   

    LCD_CtrlWrite_ST7735(0xB4);     
    LCD_DataWrite_ST7735(0x03);   

    LCD_CtrlWrite_ST7735(0xC0);     
    LCD_DataWrite_ST7735(0x28);   
    LCD_DataWrite_ST7735(0x08);   
    LCD_DataWrite_ST7735(0x84);   

    LCD_CtrlWrite_ST7735(0xC1);     
    LCD_DataWrite_ST7735(0x05);   

    LCD_CtrlWrite_ST7735(0xC2);     
    LCD_DataWrite_ST7735(0x0D);   
    LCD_DataWrite_ST7735(0x00);   

    LCD_CtrlWrite_ST7735(0xC3);     
    LCD_DataWrite_ST7735(0x8D);   
    LCD_DataWrite_ST7735(0xEA);   

    LCD_CtrlWrite_ST7735(0xC4);     
    LCD_DataWrite_ST7735(0x8D);   
    LCD_DataWrite_ST7735(0xEE);   

    LCD_CtrlWrite_ST7735(0xC5);     
    LCD_DataWrite_ST7735(0x00);   

    LCD_CtrlWrite_ST7735(0x36);     
    LCD_DataWrite_ST7735(0xC8);   

    LCD_CtrlWrite_ST7735(0x3A);     
    LCD_DataWrite_ST7735(0x05); 

    LCD_CtrlWrite_ST7735(0xE0);     
    LCD_DataWrite_ST7735(0x1B);   
    LCD_DataWrite_ST7735(0x1A);   
    LCD_DataWrite_ST7735(0x10);   
    LCD_DataWrite_ST7735(0x15);   
    LCD_DataWrite_ST7735(0x34);   
    LCD_DataWrite_ST7735(0x2E);   
    LCD_DataWrite_ST7735(0x2A);   
    LCD_DataWrite_ST7735(0x2A);   
    LCD_DataWrite_ST7735(0x2C);   
    LCD_DataWrite_ST7735(0x27);   
    LCD_DataWrite_ST7735(0x2E);   
    LCD_DataWrite_ST7735(0x39);   
    LCD_DataWrite_ST7735(0x00);   
    LCD_DataWrite_ST7735(0x15);   
    LCD_DataWrite_ST7735(0x02);   
    LCD_DataWrite_ST7735(0x3F);   

    LCD_CtrlWrite_ST7735(0xE1);     
    LCD_DataWrite_ST7735(0x1A);   
    LCD_DataWrite_ST7735(0x1A);   
    LCD_DataWrite_ST7735(0x0F);   
    LCD_DataWrite_ST7735(0x15);   
    LCD_DataWrite_ST7735(0x34);   
    LCD_DataWrite_ST7735(0x2E);   
    LCD_DataWrite_ST7735(0x25);   
    LCD_DataWrite_ST7735(0x2B);   
    LCD_DataWrite_ST7735(0x26);   
    LCD_DataWrite_ST7735(0x26);   
    LCD_DataWrite_ST7735(0x2F);   
    LCD_DataWrite_ST7735(0x3A);   
    LCD_DataWrite_ST7735(0x00);   
    LCD_DataWrite_ST7735(0x15);   
    LCD_DataWrite_ST7735(0x02);   
    LCD_DataWrite_ST7735(0x3F);   

    LCD_CtrlWrite_ST7735(0x29);
#else
    hal_GoudatResetLcdPin(); 
    hal_LcdDelay(20); 
    LCD_CtrlWrite_ST7735(0x11); 

    hal_LcdDelay(120);   //Delay 120ms

    LCD_CtrlWrite_ST7735(0xb1); 
    LCD_DataWrite_ST7735(0x05); 
    LCD_DataWrite_ST7735(0x3a); 
    LCD_DataWrite_ST7735(0x3a); 
    LCD_CtrlWrite_ST7735(0xb2); 
    LCD_DataWrite_ST7735(0x05); 
    LCD_DataWrite_ST7735(0x3c); 
    LCD_DataWrite_ST7735(0x3c); 
    LCD_CtrlWrite_ST7735(0xb3); 
    LCD_DataWrite_ST7735(0x05); 
    LCD_DataWrite_ST7735(0x3c); 
    LCD_DataWrite_ST7735(0x3c); 
    LCD_DataWrite_ST7735(0x05); 
    LCD_DataWrite_ST7735(0x3c); 
    LCD_DataWrite_ST7735(0x3c); 

    LCD_CtrlWrite_ST7735(0xb4); 
    LCD_DataWrite_ST7735(0x03); 

    LCD_CtrlWrite_ST7735(0xc0); 
    LCD_DataWrite_ST7735(0xe0); 
    LCD_DataWrite_ST7735(0x00); 
    LCD_DataWrite_ST7735(0x84); 
    LCD_CtrlWrite_ST7735(0xc1); 
    LCD_DataWrite_ST7735(0x05); 
    LCD_CtrlWrite_ST7735(0xc2); 
    LCD_DataWrite_ST7735(0x0d); 
    LCD_DataWrite_ST7735(0x00); 
    LCD_CtrlWrite_ST7735(0xc3); 
    LCD_DataWrite_ST7735(0x8d); 
    LCD_DataWrite_ST7735(0xea); 
    LCD_CtrlWrite_ST7735(0xc4); 
    LCD_DataWrite_ST7735(0x8d); 
    LCD_DataWrite_ST7735(0xee); 
    LCD_CtrlWrite_ST7735(0xc5); //set vcom  
    LCD_DataWrite_ST7735(0x03); //VCOM=  


    //spi_write_cmd(spi_ctx,0x21); 

    LCD_CtrlWrite_ST7735(0x36); 
    LCD_DataWrite_ST7735(0xd8);//c8 , qiexian wen ti ,yangyu mark

    LCD_CtrlWrite_ST7735(0xe0); 
    LCD_DataWrite_ST7735(0x1b); 
    LCD_DataWrite_ST7735(0x1a); 
    LCD_DataWrite_ST7735(0x10); 
    LCD_DataWrite_ST7735(0x15); 
    LCD_DataWrite_ST7735(0x34); 
    LCD_DataWrite_ST7735(0x2e); 
    LCD_DataWrite_ST7735(0x2a); 
    LCD_DataWrite_ST7735(0x2a); 
    LCD_DataWrite_ST7735(0x2c); 
    LCD_DataWrite_ST7735(0x27); 
    LCD_DataWrite_ST7735(0x2e); 
    LCD_DataWrite_ST7735(0x39); 
    LCD_DataWrite_ST7735(0x00); 
    LCD_DataWrite_ST7735(0x15); 
    LCD_DataWrite_ST7735(0x02); 
    LCD_DataWrite_ST7735(0x3f); 

    LCD_CtrlWrite_ST7735(0xe1); 
    LCD_DataWrite_ST7735(0x1a); 
    LCD_DataWrite_ST7735(0x1a); 
    LCD_DataWrite_ST7735(0x0f); 
    LCD_DataWrite_ST7735(0x15); 
    LCD_DataWrite_ST7735(0x34); 
    LCD_DataWrite_ST7735(0x2e); 
    LCD_DataWrite_ST7735(0x25); 
    LCD_DataWrite_ST7735(0x2b); 
    LCD_DataWrite_ST7735(0x26);
    LCD_DataWrite_ST7735(0x26);  
    LCD_DataWrite_ST7735(0x2f); 
    LCD_DataWrite_ST7735(0x3a); 
    LCD_DataWrite_ST7735(0x00); 
    LCD_DataWrite_ST7735(0x15); 
    LCD_DataWrite_ST7735(0x02); 
    LCD_DataWrite_ST7735(0x3f); 

    LCD_CtrlWrite_ST7735(0x3A);
    LCD_DataWrite_ST7735(0x05); //format is RGB565

    LCD_CtrlWrite_ST7735(0x35); //Tearing Effect Line ON
    LCD_DataWrite_ST7735(0x00);

    LCD_CtrlWrite_ST7735(0x44); //gu ding qie xian wen ti, yangyu mark
    LCD_DataWrite_ST7735(0x00); 
    LCD_DataWrite_ST7735(0x00); 

    LCD_CtrlWrite_ST7735(0x29); 


#endif
}


/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ST7735_EnterSleep(
    BOOLEAN is_sleep //SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
    )
{
     SCI_TraceLow("lcd: ST7735_EnterSleep, is_sleep = %d", is_sleep);
     //ST7735_ReadID(0,0,0);
   gouda_event(0x90001104);

    if(is_sleep)
    {
        gouda_event(0x900f2dc4);
        LCD_CtrlWrite_ST7735(0x28); //display off
        hal_LcdDelay(120);
        LCD_CtrlWrite_ST7735(0x10); // enter sleep mode
        //hal_LcdDelay(150);
        //_GoudaClkDisable();
       //  LDO_TurnOffLDO(LDO_LDO_LCD);
        
    }
    else
    {
        gouda_event(0x9f0f2dd4);
#if 0
        LCD_CtrlWrite_ST7735(0x11); // sleep out mode
        hal_LcdDelay(120);
        LCD_CtrlWrite_ST7735(0x29); // display on
#endif
        ST7735_Init();
    }

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ST7735_Close(void)
{
    SCI_TraceLow("lcd: LCD: in ST7735_Close");
	   gouda_event(0x90001105);

    ST7735_EnterSleep( SCI_TRUE );
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ST7735_Invalidate(void)
{
   gouda_event(0x90001106);

    ST7735_set_display_window (0x0, 0x0, LCD_WIDTH - 1, LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E ST7735_InvalidateRect(
    uint16 left, //the left value of the rectangel
    uint16 top, //top of the rectangle
    uint16 right, //right of the rectangle
    uint16 bottom //bottom of the rectangle
    )
{
   gouda_event(0x90001107);

    left = (left >=LCD_WIDTH) ? LCD_WIDTH-1 : left;
    right = (right >= LCD_WIDTH) ? LCD_WIDTH-1 : right;
    top = (top >= LCD_HEIGHT) ? LCD_HEIGHT-1 : top;
    bottom = (bottom >= LCD_HEIGHT) ? LCD_HEIGHT-1 : bottom;

    if ( ( right < left ) || ( bottom < top ) )
    {
        return ERR_LCD_OPERATE_FAIL;
    }     

    ST7735_set_display_window(left, top, right, bottom);
    return ERR_LCD_NONE;
}

LOCAL ERR_LCD_E ST7735_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
   gouda_event(0x90001108);

    SCI_TraceLow_Test(" ST7735_RotationInvalidateRect");
    switch(angle)
    {
        case LCD_ANGLE_0:
            ST7735_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_90:
            ST7735_set_display_window(left, top, bottom,right);
            break;
        case LCD_ANGLE_180:
            ST7735_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_270:
            ST7735_set_display_window(left, top, bottom,right);
            break;
        default:
            ST7735_set_display_window(left, top, right, bottom);
            break;
    }
    return ERR_LCD_NONE;
}

LOCAL ERR_LCD_E   ST7735_SetContrast(uint16  contrast)
{
   gouda_event(0x90001109);

    SCI_TraceLow_Test(" ST7735_SetContrast");
    return ERR_LCD_FUNC_NOT_SUPPORT;
} 
/*****************************************************************************/
LOCAL ERR_LCD_E   ST7735_SetBrightness(uint16 brightness)
{
    SCI_TraceLow_Test(" ST7735_SetBrightness");
	   gouda_event(0x9000110a);

    return ERR_LCD_FUNC_NOT_SUPPORT;
}

//  Description:    Enable lcd to partial display mode, so can save power.
//	Global resource dependence: 
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//	Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E ST7735_SetDisplayWindow(
    uint16 left, //left of the window
    uint16 top, //top of the window
    uint16 right, //right of the window
    uint16 bottom //bottom of the window
    )
{
   gouda_event(0x9000110b);

    SCI_TraceLow_Test(" ST7735_SetDisplayWindow");
    ST7735_set_display_window(left, top, right, bottom);
    return ERR_LCD_NONE;
}

LOCAL void ST7735_InvalidatePixel(uint16 x, uint16 y, uint32 data)
{
    SCI_TraceLow_Test(" ST7735_InvalidatePixel");
	   gouda_event(0x9000110c);

    ST7735_InvalidateRect(x,y,x,y);
    LCD_DataWrite_ST7735(data);
}

LOCAL void ST7735_GetConfig(void)
{
	uint32 count,k;
	LCM_CFG_INFO_T_PTR cfg_ptr = PNULL;
	cfg_ptr = LCM_GetCfgInfo(0,&count);
	for(k=0;k<count;k++)
	{
		if(cfg_ptr[k].dev_id==LCD_DRV_ID_ST7735)
		{
			st7735_line_mode = cfg_ptr[k].adc_min;
			st7735_farmk = cfg_ptr[k].adc_max;
			
			break;
		}
	}
    SCI_TraceLow("lcd: ST7735_GetConfig LCM: g_two_date = %d , fmark =%d\n",st7735_line_mode,st7735_farmk);  //0x80,0xf5,0x3e
}

LOCAL uint32 ST7735_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint8 id[4] = {0};
	uint32 idlcd=0;
	ST7735_GetConfig();
    hal_gouda_sim_init(st7735_line_mode,st7735_farmk,0x10000);
	mon_Event(0x990b7789);

    hal_LcdDelay(140);
    hwp_analogReg->pad_spi_lcd_cfg2 |= ANALOG_REG_PAD_SPI_LCD_SIO_SPU;
    hal_LcdDelay(10);
    hal_GoudaReadData(0x04,id,4);
    hwp_analogReg->pad_spi_lcd_cfg2 =  hwp_analogReg->pad_spi_lcd_cfg2&(~ANALOG_REG_PAD_SPI_LCD_SIO_SPU);
    hal_LcdDelay(10);
    SCI_TraceLow("lcd: ST7735_ReadID: id0->0x%0x,id1->0x%0x, id2->0x%x, id3->0x%0x\n",id[0],id[3], id[2], id[1] );
    //id[3]=id1   id[2]=id2    id[1]=id3
	
    mon_Event((id[3]<<16)|(id[2] <<8)| id[1]);

    hal_gouda_term();
	idlcd = ((id[3]<<16)|((id[2] <<8)| id[1]));
	mon_Event(idlcd);
	if((idlcd == LCD_DRV_ID_ST7735))
	{
		idlcd = LCD_DRV_ID_ST7735;
		
	}
		
	//idlcd = LCD_DRV_ID_ST7735;// FIH need to support all other LDDs,if id is not exit.

    return idlcd;
}

const LCD_OPERATIONS_T ST7735_operations = 
{
    ST7735_Init,
    ST7735_EnterSleep,
    ST7735_SetContrast,
    ST7735_SetBrightness,
    ST7735_SetDisplayWindow,
    ST7735_InvalidateRect,
    ST7735_Invalidate,
    ST7735_Close,
    ST7735_RotationInvalidateRect,
    ST7735_SetDirection,
    NULL, 
    ST7735_ReadID	
};

LOCAL const LCD_TIMING_U s_ST7735_lcmtiming =
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

LOCAL const LCD_TIMING_U s_ST7735_spitiming =
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
const LCD_SPEC_T g_lcd_ST7735 = 
{
    LCD_WIDTH, 
    LCD_HEIGHT, 
    SCI_NULL, 
    SCI_NULL,
    WIDTH_8,
    (LCD_TIMING_U*)&s_ST7735_spitiming, 
    (LCD_OPERATIONS_T*)&ST7735_operations,
    1,
    0
};
#else // not verify lcm
const LCD_SPEC_T g_lcd_ST7735 = 
{
    LCD_WIDTH, 
    LCD_HEIGHT, 
    LCD_MCU, 
    BUS_MODE_8080,
    WIDTH_8,
    (LCD_TIMING_U*)&s_ST7735_lcmtiming, 
    (LCD_OPERATIONS_T*)&ST7735_operations,
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

