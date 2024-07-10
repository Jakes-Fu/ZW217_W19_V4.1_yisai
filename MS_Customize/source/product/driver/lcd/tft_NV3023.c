/******************************************************************************
 ** File Name:     tft_NV3023.c                                             *
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

#define NV3023_LCD_WIDTH 128
#define NV3023_LCD_HEIGHT 128

#define NV3023_X_OFFSET 0
#define NV3023_Y_OFFSET 32

#define SCI_TraceLow_Test SCI_TraceLow

LOCAL uint16 NV3023_line_mode = 0;
LOCAL uint16 NV3023_farmk = 0;


#define LCM_WR_REG(Addr, Data)                              \
{                                                       \
    while (hal_GoudaWriteReg(Addr, Data) != HAL_ERR_NO) \
    ;                                               \
}
#define LCD_DataWrite_NV3023(Data)                               \
{                                                  \
    while (hal_GoudaWriteData(Data) != HAL_ERR_NO) \
    ;                                          \
}
#define LCD_CtrlWrite_NV3023(Cmd)                              \
{                                                \
    while (hal_GoudaWriteCmd(Cmd) != HAL_ERR_NO) \
    ;                                        \
}

//#define LCD_CtrlData_NV3023(cmd, data)  LCD_CtrlWrite_NV3023( cmd ); LCD_DataWrite_NV3023( data );

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

LOCAL MEM_DATA_ACCESS_CTRL_U s_lcd_madc;


/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void NV3023_Close(void);

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  NV3023_EnterSleep(BOOLEAN is_sleep);

/*********************************************************************/
//  Description:   Initialize color LCD : GC9304
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E NV3023_Init(void);

LOCAL ERR_LCD_E NV3023_Invalidate(void);
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
LOCAL ERR_LCD_E NV3023_InvalidateRect(
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
LOCAL void NV3023_set_display_window(
        uint16 left, // start Horizon address
        uint16 right, // end Horizon address
        uint16 top, // start Vertical address
        uint16 bottom // end Vertical address
        );

LOCAL uint32 NV3023_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id);

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

LOCAL ERR_LCD_E NV3023_SetDirection(LCD_DIRECT_E direct_type)
{
   // gouda_event(0x90001101);

    SCI_TraceLow_Test("lcd: NV3023_SetDirection = %d",direct_type);
#if 0
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
    LCD_CtrlWrite_NV3023(0x36);
    LCD_DataWrite_NV3023(s_lcd_madc.dataval);
    LCD_CtrlWrite_NV3023(0x2c);
	#endif
    return ERR_LCD_NONE;

}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//	Note:
/******************************************************************************/
LOCAL void NV3023_set_display_window(
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

    left = (left >=NV3023_LCD_WIDTH) ? NV3023_LCD_WIDTH-1 : left;
    right = (right >= NV3023_LCD_WIDTH) ? NV3023_LCD_WIDTH-1 : right;
    top = (top >= NV3023_LCD_HEIGHT) ? NV3023_LCD_HEIGHT-1 : top;
    bottom = (bottom >= NV3023_LCD_HEIGHT) ? NV3023_LCD_HEIGHT-1 : bottom;

    if ( ( right < left ) || ( bottom < top ) )
    {
        return;
    }
    
    newleft   = left+NV3023_X_OFFSET;
    newtop    = top+NV3023_Y_OFFSET;
    newright  = right+NV3023_X_OFFSET;
    newbottom = bottom+NV3023_Y_OFFSET;
    
    // SCI_TraceLow_Test("[DRV_LCD]NV3023_set_display_window L = %d, top = %d, R = %d, bottom = %d",left,top,right,bottom);
    gouda_event(0x90001102);
    //hal_GoudaChangeSerialMode(HAL_GOUDA_SPI_LINE_3);
    LCD_CtrlWrite_NV3023(0x2a);	// set hori start , end (left, right)
    LCD_DataWrite_NV3023((newleft>>8)&0xff); // left high 8 b
    LCD_DataWrite_NV3023(newleft&0xff); // left low 8 b
    LCD_DataWrite_NV3023((newright>>8)&0xff); // right high 8 b
    LCD_DataWrite_NV3023(newright&0xff); // right low 8 b

    LCD_CtrlWrite_NV3023(0x2b);	// set vert start , end (top, bot)
    LCD_DataWrite_NV3023((newtop>>8)&0xff); // top high 8 b
    LCD_DataWrite_NV3023(newtop&0xff); // top low 8 b
    LCD_DataWrite_NV3023((newbottom>>8)&0xff); // bot high 8 b
    LCD_DataWrite_NV3023(newbottom&0xff); // bot low 8 b
    LCD_CtrlWrite_NV3023(0x2c);	 // recover memory write mode
    //hal_GoudaChangeSerialMode(HAL_GOUDA_SPI_LINE_3_2_LANE);

}

extern void hal_GoudatResetLcdPin(void);
/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E NV3023_Init(void)
{
    SCI_TraceLow_Test("lcd: NV3023_Init here");

    mon_Event(0x55555555);

/////////////////////////////////////////// NV3023+CPT2.4  20.10.15
    hal_GoudatResetLcdPin();
    hal_LcdDelay(100); 
    //----------------Star Initial Sequence-------//
    LCD_CtrlWrite_NV3023(0xff);
    LCD_DataWrite_NV3023(0xa5); //
    LCD_CtrlWrite_NV3023(0x3E);
    LCD_DataWrite_NV3023(0x01);
    LCD_CtrlWrite_NV3023(0x3A);
    LCD_DataWrite_NV3023(0x65);
    LCD_CtrlWrite_NV3023(0x82);
    LCD_DataWrite_NV3023(0x00);
    LCD_CtrlWrite_NV3023(0x98);
    LCD_DataWrite_NV3023(0x00);
    LCD_CtrlWrite_NV3023(0x63);
    LCD_DataWrite_NV3023(0x0f);
    LCD_CtrlWrite_NV3023(0x64);
    LCD_DataWrite_NV3023(0x0f);
    LCD_CtrlWrite_NV3023(0xB4);
    LCD_DataWrite_NV3023(0x34);
    LCD_CtrlWrite_NV3023(0xB5);
    LCD_DataWrite_NV3023(0x30);
    LCD_CtrlWrite_NV3023(0x83);
    LCD_DataWrite_NV3023(0x03)
    LCD_CtrlWrite_NV3023(0x86);//
    LCD_DataWrite_NV3023(0x04);
    LCD_CtrlWrite_NV3023(0x87);
    LCD_DataWrite_NV3023(0x16);
    LCD_CtrlWrite_NV3023(0x88);//VCOM
    LCD_DataWrite_NV3023(0x09);
    LCD_CtrlWrite_NV3023(0x89);//
    LCD_DataWrite_NV3023(0x2F);//2 F
    LCD_CtrlWrite_NV3023(0x93); //
    LCD_DataWrite_NV3023(0x63);
    LCD_CtrlWrite_NV3023(0x96);
    LCD_DataWrite_NV3023(0x81);
    LCD_CtrlWrite_NV3023(0xC3);
    LCD_DataWrite_NV3023(0x10);
    LCD_CtrlWrite_NV3023(0xE6);
    LCD_DataWrite_NV3023(0x00);
    LCD_CtrlWrite_NV3023(0x99);
    LCD_DataWrite_NV3023(0x01);
    ////////////////////////gamma_set//////////////////////////////////////
    LCD_CtrlWrite_NV3023(0x70);
    LCD_DataWrite_NV3023(0x07);//VRP 0 1
    LCD_CtrlWrite_NV3023(0x71);
    LCD_DataWrite_NV3023(0x21);//VRP 1 3
    LCD_CtrlWrite_NV3023(0x72);
    LCD_DataWrite_NV3023(0x0a);//VRP 2 7
    LCD_CtrlWrite_NV3023(0x73);
    LCD_DataWrite_NV3023(0x10);//VRP 3 9
    LCD_CtrlWrite_NV3023(0x74);
    LCD_DataWrite_NV3023(0x17);//VRP 6 11
    LCD_CtrlWrite_NV3023(0x75);
    LCD_DataWrite_NV3023(0x1A);//VRP 8 13
    LCD_CtrlWrite_NV3023(0x76);
    LCD_DataWrite_NV3023(0x3F);//VRP 10 5
    LCD_CtrlWrite_NV3023(0x77);
    LCD_DataWrite_NV3023(0x09);//VRP 14 15
    LCD_CtrlWrite_NV3023(0x78);
    LCD_DataWrite_NV3023(0x05);//VRP 17 16
    LCD_CtrlWrite_NV3023(0x79);
    LCD_DataWrite_NV3023(0x3F);//VRP 21 6
    LCD_CtrlWrite_NV3023(0x7a);
    LCD_DataWrite_NV3023(0x05);//VRP 23 14
    LCD_CtrlWrite_NV3023(0x7b);
    LCD_DataWrite_NV3023(0x0C);//VRP 25 12
    LCD_CtrlWrite_NV3023(0x7c);
    LCD_DataWrite_NV3023(0x12);//VRP 28 9
    LCD_CtrlWrite_NV3023(0x7d);
    LCD_DataWrite_NV3023(0x0a);//VRP 29 8
    LCD_CtrlWrite_NV3023(0x7e);
    LCD_DataWrite_NV3023(0x0A);//VRP 30 4
    LCD_CtrlWrite_NV3023(0x7f);
    LCD_DataWrite_NV3023(0x08);//VRP 31 2
    LCD_CtrlWrite_NV3023(0xa0);
    LCD_DataWrite_NV3023(0x0b);//VRN 0 1
    LCD_CtrlWrite_NV3023(0xa1);
    LCD_DataWrite_NV3023(0x30);//VRN 1 3
    LCD_CtrlWrite_NV3023(0xa2);
    LCD_DataWrite_NV3023(0x09);//VRN 2 7
    LCD_CtrlWrite_NV3023(0xa3);
    LCD_DataWrite_NV3023(0x0c);//VRN 3 9
    LCD_CtrlWrite_NV3023(0xa4);
    LCD_DataWrite_NV3023(0x08);//VRN 6 11
    LCD_CtrlWrite_NV3023(0xa5);
    LCD_DataWrite_NV3023(0x22);//VRN 8 13
    LCD_CtrlWrite_NV3023(0xa6);
    LCD_DataWrite_NV3023(0x40);//VRN 10 5
    LCD_CtrlWrite_NV3023(0xa7);
    LCD_DataWrite_NV3023(0x04);//VRN 14 15
    LCD_CtrlWrite_NV3023(0xa8);
    LCD_DataWrite_NV3023(0x05);//VRN 17 16
    LCD_CtrlWrite_NV3023(0xa9);
    LCD_DataWrite_NV3023(0x3F);//VRN 21 6
    LCD_CtrlWrite_NV3023(0xaa);
    LCD_DataWrite_NV3023(0x0A);//VRN 23 14 //
    LCD_CtrlWrite_NV3023(0xab);
    LCD_DataWrite_NV3023(0x11);//VRN 25 12
    LCD_CtrlWrite_NV3023(0xac);
    LCD_DataWrite_NV3023(0x0d);//VRN 28 10
    LCD_CtrlWrite_NV3023(0xad);
    LCD_DataWrite_NV3023(0x06);//VRN 29 8
    LCD_CtrlWrite_NV3023(0xae);
    LCD_DataWrite_NV3023(0x3B);//VRN 30 4
    LCD_CtrlWrite_NV3023(0xaf);
    LCD_DataWrite_NV3023(0x07);//VRN 31 2
    //////////////////////////////////////////////////////////////////
    LCD_CtrlWrite_NV3023(0xff);
    LCD_DataWrite_NV3023(0x00);
    LCD_CtrlWrite_NV3023(0x11);
    hal_LcdDelay(150);
    LCD_CtrlWrite_NV3023(0x21);
    LCD_CtrlWrite_NV3023(0x36);
    LCD_DataWrite_NV3023(0x80);
    LCD_CtrlWrite_NV3023(0x29);
    hal_LcdDelay(10);
    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  NV3023_EnterSleep(
        BOOLEAN is_sleep //SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
        )
{
    SCI_TraceLow("lcd: NV3023_EnterSleep, is_sleep = %d", is_sleep);
   // gouda_event(0x90001104);

    if(is_sleep)
    {
       // gouda_event(0x900f2dc4);
        LCD_CtrlWrite_NV3023(0x28); //display off
        hal_LcdDelay(120);
        LCD_CtrlWrite_NV3023(0x10); // enter sleep mode
    }
    else
    {

       // gouda_event(0x9f0f2ddf);
#if 0
        LCD_CtrlWrite_NV3023(0x11); // sleep out mode
        hal_LcdDelay(120);
        LCD_CtrlWrite_NV3023(0x29); // display on
#endif
        NV3023_Init();
    }

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void NV3023_Close(void)
{
    SCI_TraceLow("lcd:  in GC9304_Close");
    gouda_event(0x90001105);

    NV3023_EnterSleep( SCI_TRUE );
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E NV3023_Invalidate(void)
{
    gouda_event(0x90001106);

    NV3023_set_display_window (0x0, 0x0, LCD_WIDTH - 1, LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E NV3023_InvalidateRect(
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

    NV3023_set_display_window(left, top, right, bottom);
    return ERR_LCD_NONE;
}

LOCAL ERR_LCD_E NV3023_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
    gouda_event(0x90001108);

    SCI_TraceLow("lcd:  NV3023_RotationInvalidateRect");
    switch(angle)
    {
        case LCD_ANGLE_0:
            NV3023_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_90:
            NV3023_set_display_window(left, top, bottom,right);
            break;
        case LCD_ANGLE_180:
            NV3023_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_270:
            NV3023_set_display_window(left, top, bottom,right);
            break;
        default:
            NV3023_set_display_window(left, top, right, bottom);
            break;
    }
    return ERR_LCD_NONE;
}



#define LCD_DEFAULT_STATUS_09H 0xa4530000

LOCAL void GcCheckDisplayStatus(void)
{
    uint8 id[4] = {0};
    uint32 lcdstatus =0;

    hal_GoudaReadData(0x09, id, 4);

    lcdstatus = ((id[3]<<24) | (id[2]<<16));
	
    SCI_TraceLow("lcd: NV3023 09=0x%x\n",lcdstatus);
    if(LCD_DEFAULT_STATUS_09H != lcdstatus)
    {
        hal_GoudatResetLcdPin();
        hal_LcdDelay(120);
        NV3023_Init();
        SCI_TraceLow("lcd: NV3023 ReadDisplayStatus: RESET  \n");
    }
}


LOCAL ERR_LCD_E   NV3023_SetContrast(uint16  contrast)
{
    gouda_event(0x90001109);

    SCI_TraceLow("lcd: NV3023_SetContrast");
   // GcCheckDisplayStatus();
    return ERR_LCD_FUNC_NOT_SUPPORT;
} 
/*****************************************************************************/
LOCAL ERR_LCD_E   NV3023_SetBrightness(uint16 brightness)
{
    SCI_TraceLow("lcd:  NV3023_SetBrightness");
   // gouda_event(0x9000110a);

    return ERR_LCD_FUNC_NOT_SUPPORT;
}

//  Description:    Enable lcd to partial display mode, so can save power.
//	Global resource dependence: 
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//	Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E NV3023_SetDisplayWindow(
        uint16 left, //left of the window
        uint16 top, //top of the window
        uint16 right, //right of the window
        uint16 bottom //bottom of the window
        )
{
    gouda_event(0x9000110b);

    SCI_TraceLow("lcd:  NV3023_SetDisplayWindow");
    NV3023_set_display_window(left, top, right, bottom);
    return ERR_LCD_NONE;
}

LOCAL void NV3023_InvalidatePixel(uint16 x, uint16 y, uint32 data)
{
    SCI_TraceLow("lcd:  NV3023_InvalidatePixel");
    gouda_event(0x9000110c);

    NV3023_InvalidateRect(x,y,x,y);
    LCD_DataWrite_NV3023(data);
}

LOCAL void NV3023_GetConfig(void)
{
    uint32 count,k;
    LCM_CFG_INFO_T_PTR cfg_ptr = PNULL;
    cfg_ptr = LCM_GetCfgInfo(0,&count);
    for(k=0;k<count;k++)
    {
        if(cfg_ptr[k].dev_id==LCD_DRV_ID_NV3023)
        {
            NV3023_line_mode = cfg_ptr[k].adc_min;
            NV3023_farmk = cfg_ptr[k].adc_max;

            break;
        }
    }
    SCI_TraceLow("lcd: NV3023_GetConfig: NV3023_line_mode = %d , NV3023_farmk =%d\n",NV3023_line_mode,NV3023_farmk);  //0x80,0xf5,0x3e
}
//uint32 g_3023_Id[4] = {0};
uint32 _Read_NV3023_ID(void)
{
    HAL_ERR_T r_err;
    uint8 id[4] = {0}, i = 0;

    hal_LcdDelay(140);
    hwp_analogReg->pad_spi_lcd_cfg2 |= ANALOG_REG_PAD_SPI_LCD_SIO_SPU;
    hal_LcdDelay(10);
    r_err = hal_GoudaReadData(0xD3,id,4);
    hwp_analogReg->pad_spi_lcd_cfg2 =  hwp_analogReg->pad_spi_lcd_cfg2&(~ANALOG_REG_PAD_SPI_LCD_SIO_SPU);
    hal_LcdDelay(10); 

#if 0
    for (i = 0; i < 4; i++) {
    	g_3023_Id[i] = id[i];
    }
#endif    
    return ((id[3]<<16)|(id[2] <<8)| id[1]);

}


LOCAL uint32 NV3023_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint8 id[4] = {0};
    NV3023_GetConfig();

    hal_gouda_sim_init(NV3023_line_mode,NV3023_farmk,0x18000);

    hal_LcdDelay(140);
    hwp_analogReg->pad_spi_lcd_cfg2 |= ANALOG_REG_PAD_SPI_LCD_SIO_SPU;
    hal_LcdDelay(10);
    hal_GoudaReadData(0x04,id,4);
    hwp_analogReg->pad_spi_lcd_cfg2 =  hwp_analogReg->pad_spi_lcd_cfg2&(~ANALOG_REG_PAD_SPI_LCD_SIO_SPU);
    hal_LcdDelay(10);
    SCI_TraceLow("lcd: NV3023_ReadID: id1->0x%0x, id2->0x%x, id3->0x%0x\n",id[3], id[2], id[1]);

    mon_Event((id[3]<<16)|(id[2] <<8)| id[1]);

    hal_gouda_term();

    return ((id[3]<<16)|(id[2] <<8)| id[1]);
}


const LCD_OPERATIONS_T NV3023_operations = 
{
    NV3023_Init,
    NV3023_EnterSleep,
    NV3023_SetContrast,
    NV3023_SetBrightness,
    NV3023_SetDisplayWindow,
    NV3023_InvalidateRect,
    NV3023_Invalidate,
    NV3023_Close,
    NV3023_RotationInvalidateRect,
    NV3023_SetDirection,
    NULL, 
    NV3023_ReadID	
};

LOCAL const LCD_TIMING_U s_NV3023_lcmtiming =
{
    20,  // CS setup time for LCM read (optional)  
    300, // low pulse width for LCM read (according spec)	
    300, // high pulse width for LCM read (according spec)
    15,   // CS setup time for LCM write  (optional) 	
    50,  // low pulse width for LCM write (according spec)
    50,  // high pulse width for LCM write (according spec)	
    // LCM_CYCLE_U end(ns)
};

LOCAL const LCD_TIMING_U s_NV3023_spitiming =
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
const LCD_SPEC_T g_lcd_NV3023 = 
{
    NV3023_LCD_WIDTH,
    NV3023_LCD_HEIGHT,
    SCI_NULL,
    SCI_NULL,
    WIDTH_8,
    (LCD_TIMING_U*)&s_NV3023_spitiming, 
    (LCD_OPERATIONS_T*)&NV3023_operations,
    1,
    0
};
#else // not verify lcm
const LCD_SPEC_T g_lcd_NV3023 = 
{
    NV3023_LCD_WIDTH,
    NV3023_LCD_HEIGHT,
    LCD_MCU,
    BUS_MODE_8080,
    WIDTH_8,
    (LCD_TIMING_U*)&s_NV3023_lcmtiming, 
    (LCD_OPERATIONS_T*)&NV3023_operations,
    1,
    0
};
#endif



