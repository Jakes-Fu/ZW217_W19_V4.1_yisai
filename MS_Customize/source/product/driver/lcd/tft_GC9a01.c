/******************************************************************************
 ** File Name:     tft_GC9A01.c                                             *
 ** Description:                                                              *
 **    This file contains driver for color LCD.(CSTN)                         *
 ** Author:         sky.wang                                                 *
 ** DATE:           20/07/2018                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 02/102010      lichd            Create.
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
#include "pinmap.h"

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
extern   "C"
{
#endif



#define LCD_DEFAULT_STATUS_09H 0xa0530600


#define SCI_TraceLow_Test SCI_TraceLow

LOCAL uint16 GC9A01_line_mode = 0;
LOCAL uint16 GC9A01_farmk = 0;



#define LCM_WR_REG(Addr, Data)                              \
    {                                                       \
        while (hal_GoudaWriteReg(Addr, Data) != HAL_ERR_NO) \
            ;                                               \
    }
#define LCD_DataWrite_GC9A01(Data)                               \
    {                                                  \
        while (hal_GoudaWriteData(Data) != HAL_ERR_NO) \
            ;                                          \
    }
#define LCD_CtrlWrite_GC9A01(Cmd)                              \
    {                                                \
        while (hal_GoudaWriteCmd(Cmd) != HAL_ERR_NO) \
            ;                                        \
    }

//#define LCD_CtrlData_GC9A01(cmd, data)  LCD_CtrlWrite_GC9A01( cmd ); LCD_DataWrite_GC9A01( data );

/*lint -restore*/

#ifdef CHIP_ENDIAN_LITTLE
typedef union
{
    struct
    {
        unsigned int resv   :2;
        unsigned int mh    :1;  // display data latch order
        unsigned int rgb    :1; // rgb order
        unsigned int ml    :1;      // line address order
        unsigned int mv    :1;  // x/y exchanged
        unsigned int mx    :1;  // horizontal mirror
        unsigned int my    :1;  // vertical mirror
    } bitsval;
    unsigned int dataval;
} MEM_DATA_ACCESS_CTRL_U;
#else
typedef union
{
    struct
    {
        unsigned int my    :1;  // vertical mirror
        unsigned int mx    :1;  // horizontal mirror
        unsigned int mv    :1;  // x/y exchanged
        unsigned int ml    :1;      // line address order
        unsigned int rgb    :1; // rgb order
        unsigned int mh    :1;  // display data latch order
        unsigned int resv   :2;
    } bitsval;
    unsigned int dataval;
} MEM_DATA_ACCESS_CTRL_U;
#endif // CHIP_ENDIAN_LITTLE

LOCAL MEM_DATA_ACCESS_CTRL_U s_lcd_madc;

LOCAL ERR_LCD_E GC9A01_Init(void);
#ifdef LCD_INSTALL_ANGLE_90
void GC9A01_Init_driver_rotation(void);
 void GC9A01_Init_soft_rotation(void);
 #endif

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL void GC9A01_Close(void);
LOCAL void GcCheckDisplayStatus(void);
void hal_GoudatResetLcdPin(void);

LOCAL void GcCheckDisplayStatus(void)
{
    uint8 id[4] = {0};
    uint32 lcdstatus =0;
    //uint32 time1;
    //time1 = timer_TimRealTickGet(OS_TIMER);

    hal_GoudaReadData(0x09,id,4);
    //mon_Event(0x99);
    //mon_Event((id[3]<<24)|(id[2]<<16)|(id[1] <<8)| id[0]);
    lcdstatus = ((id[3]<<24)|(id[2]<<16)|(id[1] <<8)| id[0]);

    SCI_TraceLow("lcd: GC9A01 09=0x%x\n",lcdstatus);
    if(LCD_DEFAULT_STATUS_09H != lcdstatus)
    {
        hal_GoudatResetLcdPin();
        hal_LcdDelay(120);
        GC9A01_Init();
        SCI_TraceLow("lcd: GC9A01 ReadDisplayStatus:rst = %d\n"); //262ms
    }
    else
    {
        LCD_CtrlWrite_GC9A01(0xfe);
        LCD_CtrlWrite_GC9A01(0xef);

        LCD_CtrlWrite_GC9A01(0x3a);
        LCD_DataWrite_GC9A01(0x05);

        LCD_CtrlWrite_GC9A01(0x36);
        LCD_DataWrite_GC9A01(0x48);


    }
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E  GC9A01_EnterSleep(BOOLEAN is_sleep);

/*********************************************************************/
//  Description:   Initialize color LCD : GC9304
//  Input:
//      None.
//  Return:
//      None.
//  Note:
/*********************************************************************/

LOCAL ERR_LCD_E GC9A01_Invalidate(void);
/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
//     To improve speed, lcd is operate in HIGH SPEED RAM WRITE MODE(4
//     uint16 are write continuously always.) So, some dummy uint16
//     should be inserted to satisfy this mode.   Please refer to spec.
/******************************************************************************/
LOCAL ERR_LCD_E GC9A01_InvalidateRect(
    uint16 left, //the left value of the rectangel
    uint16 top, //top of the rectangle
    uint16 right, //right of the rectangle
    uint16 bottom //bottom of the rectangle
);

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL void GC9A01_set_display_window(
    uint16 left, // start Horizon address
    uint16 right, // end Horizon address
    uint16 top, // start Vertical address
    uint16 bottom // end Vertical address
);

LOCAL uint32 GC9A01_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id);

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

LOCAL ERR_LCD_E GC9A01_SetDirection(LCD_DIRECT_E direct_type)
{
    // mon_Event(0x99c01101);mon_Event(direct_type);
    SCI_TraceLow_Test("GC9A01_SetDirection = %d",direct_type);

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
    s_lcd_madc.bitsval.rgb = 0;
    LCD_CtrlWrite_GC9A01(0x36);
    LCD_DataWrite_GC9A01(s_lcd_madc.dataval);
    LCD_CtrlWrite_GC9A01(0x2c);
    return ERR_LCD_NONE;

}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//  Global resource dependence:
//  Note:
/******************************************************************************/
LOCAL void GC9A01_set_display_window(
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
    SCI_TraceLow_Test("GC9A01_set_display_window L = %d, top = %d, R = %d, bottom = %d",left,top,right,bottom);
    gouda_event(0x90001102);
//hal_GoudaChangeSerialMode(HAL_GOUDA_SPI_LINE_3);
    LCD_CtrlWrite_GC9A01(0x2a); // set hori start , end (left, right)
    LCD_DataWrite_GC9A01((newleft>>8)&0xff); // left high 8 b
    LCD_DataWrite_GC9A01(newleft&0xff); // left low 8 b
    LCD_DataWrite_GC9A01((newright>>8)&0xff); // right high 8 b
    LCD_DataWrite_GC9A01(newright&0xff); // right low 8 b

    LCD_CtrlWrite_GC9A01(0x2b); // set vert start , end (top, bot)
    LCD_DataWrite_GC9A01((newtop>>8)&0xff); // top high 8 b
    LCD_DataWrite_GC9A01(newtop&0xff); // top low 8 b
    LCD_DataWrite_GC9A01((newbottom>>8)&0xff); // bot high 8 b
    LCD_DataWrite_GC9A01(newbottom&0xff); // bot low 8 b
    LCD_CtrlWrite_GC9A01(0x2c);  // recover memory write mode
//hal_GoudaChangeSerialMode(HAL_GOUDA_SPI_LINE_3_2_LANE);

}
#ifdef LCD_INSTALL_ANGLE_90
void GC9A01_Init_driver_rotation(void)
{
     LCD_CtrlWrite_GC9A01(0x36);
    LCD_DataWrite_GC9A01(0x28);  //0x28 转90
//0x88 0x18 转180度  //0x08 左右镜像//0x28 转90度
}
void GC9A01_Init_soft_rotation(void)
{
     LCD_CtrlWrite_GC9A01(0x36);
    LCD_DataWrite_GC9A01(0x48);  //0x28 转90
//0x88 0x18 转180度  //0x08 左右镜像//0x28 转90度
}
#endif
/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E GC9A01_Init(void)
{
	SCI_TraceLow_Test("GC9A01_Init otp loop ");
hal_GoudatResetLcdPin();
SCI_Sleep(100);
LCD_CtrlWrite_GC9A01(0xFE);
LCD_CtrlWrite_GC9A01(0xEF);

LCD_CtrlWrite_GC9A01(0xEB);
LCD_DataWrite_GC9A01(0x14);

LCD_CtrlWrite_GC9A01(0x84);
LCD_DataWrite_GC9A01(0x60); //40->60 0xb5 en  20200924  james

LCD_CtrlWrite_GC9A01(0x85);
LCD_DataWrite_GC9A01(0xFF);

LCD_CtrlWrite_GC9A01(0x86);
LCD_DataWrite_GC9A01(0xFF);

LCD_CtrlWrite_GC9A01(0x87);
LCD_DataWrite_GC9A01(0xFF);

LCD_CtrlWrite_GC9A01(0x8E);
LCD_DataWrite_GC9A01(0xFF);

LCD_CtrlWrite_GC9A01(0x8F);
LCD_DataWrite_GC9A01(0xFF);


LCD_CtrlWrite_GC9A01(0x88);
LCD_DataWrite_GC9A01(0x0A);

LCD_CtrlWrite_GC9A01(0x89);
LCD_DataWrite_GC9A01(0x23); ///spi 2data reg en

LCD_CtrlWrite_GC9A01(0x8A);
LCD_DataWrite_GC9A01(0x00);

LCD_CtrlWrite_GC9A01(0x8B);
LCD_DataWrite_GC9A01(0x80);

LCD_CtrlWrite_GC9A01(0x8C);
LCD_DataWrite_GC9A01(0x01);

LCD_CtrlWrite_GC9A01(0x8D);
LCD_DataWrite_GC9A01(0x03);

LCD_CtrlWrite_GC9A01(0xB5);
LCD_DataWrite_GC9A01(0x08);
LCD_DataWrite_GC9A01(0x09);//08->09  james 20200924
LCD_DataWrite_GC9A01(0x14);
LCD_DataWrite_GC9A01(0x08);
//正扫
LCD_CtrlWrite_GC9A01(0xB6);
LCD_DataWrite_GC9A01(0x00);
LCD_DataWrite_GC9A01(0x00); //GS  SS  0x20



LCD_CtrlWrite_GC9A01(0x36);
LCD_DataWrite_GC9A01(0x48);  //0x28 转90
//0x88 0x18 转180度  //0x08 左右镜像//0x28 转90度

LCD_CtrlWrite_GC9A01(0x3A);
LCD_DataWrite_GC9A01(0x05);


LCD_CtrlWrite_GC9A01(0x90);
LCD_DataWrite_GC9A01(0x08);
LCD_DataWrite_GC9A01(0x08);
LCD_DataWrite_GC9A01(0x08);
LCD_DataWrite_GC9A01(0x08);

LCD_CtrlWrite_GC9A01(0xBD);
LCD_DataWrite_GC9A01(0x06);

LCD_CtrlWrite_GC9A01(0xBC);
LCD_DataWrite_GC9A01(0x00);

LCD_CtrlWrite_GC9A01(0xFF);
LCD_DataWrite_GC9A01(0x60);
LCD_DataWrite_GC9A01(0x01);
LCD_DataWrite_GC9A01(0x04);

LCD_CtrlWrite_GC9A01(0xC3);
LCD_DataWrite_GC9A01(0x13);
LCD_CtrlWrite_GC9A01(0xC4);
LCD_DataWrite_GC9A01(0x13);

LCD_CtrlWrite_GC9A01(0xC9);
LCD_DataWrite_GC9A01(0x30);

LCD_CtrlWrite_GC9A01(0xBE);
LCD_DataWrite_GC9A01(0x11);

LCD_CtrlWrite_GC9A01(0xE1);
LCD_DataWrite_GC9A01(0x10);
LCD_DataWrite_GC9A01(0x0E);

LCD_CtrlWrite_GC9A01(0xDF);
LCD_DataWrite_GC9A01(0x21);
LCD_DataWrite_GC9A01(0x0c);
LCD_DataWrite_GC9A01(0x02);

LCD_CtrlWrite_GC9A01(0xF0);
LCD_DataWrite_GC9A01(0x45);
LCD_DataWrite_GC9A01(0x09);
LCD_DataWrite_GC9A01(0x08);
LCD_DataWrite_GC9A01(0x08);
LCD_DataWrite_GC9A01(0x26);
LCD_DataWrite_GC9A01(0x2A);

LCD_CtrlWrite_GC9A01(0xF1);
LCD_DataWrite_GC9A01(0x43);
LCD_DataWrite_GC9A01(0x70);
LCD_DataWrite_GC9A01(0x72);
LCD_DataWrite_GC9A01(0x36);
LCD_DataWrite_GC9A01(0x37);
LCD_DataWrite_GC9A01(0x6F);


LCD_CtrlWrite_GC9A01(0xF2);
LCD_DataWrite_GC9A01(0x45);
LCD_DataWrite_GC9A01(0x09);
LCD_DataWrite_GC9A01(0x08);
LCD_DataWrite_GC9A01(0x08);
LCD_DataWrite_GC9A01(0x26);
LCD_DataWrite_GC9A01(0x2A);

LCD_CtrlWrite_GC9A01(0xF3);
LCD_DataWrite_GC9A01(0x43);
LCD_DataWrite_GC9A01(0x70);
LCD_DataWrite_GC9A01(0x72);
LCD_DataWrite_GC9A01(0x36);
LCD_DataWrite_GC9A01(0x37);
LCD_DataWrite_GC9A01(0x6F);

LCD_CtrlWrite_GC9A01(0xED);
LCD_DataWrite_GC9A01(0x1B);
LCD_DataWrite_GC9A01(0x0B);

LCD_CtrlWrite_GC9A01(0xAE);
LCD_DataWrite_GC9A01(0x77);

LCD_CtrlWrite_GC9A01(0xCD);
LCD_DataWrite_GC9A01(0x63);


LCD_CtrlWrite_GC9A01(0x70);
LCD_DataWrite_GC9A01(0x07);
LCD_DataWrite_GC9A01(0x07);
LCD_DataWrite_GC9A01(0x04);
LCD_DataWrite_GC9A01(0x0E);
LCD_DataWrite_GC9A01(0x0F);
LCD_DataWrite_GC9A01(0x09);
LCD_DataWrite_GC9A01(0x07);
LCD_DataWrite_GC9A01(0x08);
LCD_DataWrite_GC9A01(0x03);

LCD_CtrlWrite_GC9A01(0xE8);
LCD_DataWrite_GC9A01(0x34);
/////////////////////////////////////////////
LCD_CtrlWrite_GC9A01(0x60);
LCD_DataWrite_GC9A01(0x38);
LCD_DataWrite_GC9A01(0x0B);
LCD_DataWrite_GC9A01(0x6D);
LCD_DataWrite_GC9A01(0x6D);

LCD_DataWrite_GC9A01(0x39);
LCD_DataWrite_GC9A01(0xF0);
LCD_DataWrite_GC9A01(0x6D);
LCD_DataWrite_GC9A01(0x6D);


LCD_CtrlWrite_GC9A01(0x61);
LCD_DataWrite_GC9A01(0x38);
LCD_DataWrite_GC9A01(0xF4);
LCD_DataWrite_GC9A01(0x6D);
LCD_DataWrite_GC9A01(0x6D);

LCD_DataWrite_GC9A01(0x38);
//LCD_DataWrite_GC9A01(0xF7); //20200718
LCD_DataWrite_GC9A01(0xF7);
LCD_DataWrite_GC9A01(0x6D);
LCD_DataWrite_GC9A01(0x6D);
/////////////////////////////////////
LCD_CtrlWrite_GC9A01(0x62);
LCD_DataWrite_GC9A01(0x38);
LCD_DataWrite_GC9A01(0x0D);
LCD_DataWrite_GC9A01(0x71);
LCD_DataWrite_GC9A01(0xED);
LCD_DataWrite_GC9A01(0x70);
LCD_DataWrite_GC9A01(0x70);
LCD_DataWrite_GC9A01(0x38);
LCD_DataWrite_GC9A01(0x0F);
LCD_DataWrite_GC9A01(0x71);
LCD_DataWrite_GC9A01(0xEF);
LCD_DataWrite_GC9A01(0x70);
LCD_DataWrite_GC9A01(0x70);

LCD_CtrlWrite_GC9A01(0x63);
LCD_DataWrite_GC9A01(0x38);
LCD_DataWrite_GC9A01(0x11);
LCD_DataWrite_GC9A01(0x71);
LCD_DataWrite_GC9A01(0xF1);
LCD_DataWrite_GC9A01(0x70);
LCD_DataWrite_GC9A01(0x70);
LCD_DataWrite_GC9A01(0x38);
LCD_DataWrite_GC9A01(0x13);
LCD_DataWrite_GC9A01(0x71);
LCD_DataWrite_GC9A01(0xF3);
LCD_DataWrite_GC9A01(0x70);
LCD_DataWrite_GC9A01(0x70);
///////////////////////////////////////////////////////
LCD_CtrlWrite_GC9A01(0x64);
LCD_DataWrite_GC9A01(0x28);
LCD_DataWrite_GC9A01(0x29);
LCD_DataWrite_GC9A01(0xF1);
LCD_DataWrite_GC9A01(0x01);
LCD_DataWrite_GC9A01(0xF1);
LCD_DataWrite_GC9A01(0x00);
LCD_DataWrite_GC9A01(0x07);

//正扫
LCD_CtrlWrite_GC9A01(0x66);
LCD_DataWrite_GC9A01(0x3C);
LCD_DataWrite_GC9A01(0x00);
LCD_DataWrite_GC9A01(0xCD);
LCD_DataWrite_GC9A01(0x67);
LCD_DataWrite_GC9A01(0x45);
LCD_DataWrite_GC9A01(0x45);
LCD_DataWrite_GC9A01(0x10);
LCD_DataWrite_GC9A01(0x00);
LCD_DataWrite_GC9A01(0x00);
LCD_DataWrite_GC9A01(0x00);

LCD_CtrlWrite_GC9A01(0x67);
LCD_DataWrite_GC9A01(0x00);
LCD_DataWrite_GC9A01(0x3C);
LCD_DataWrite_GC9A01(0x00);
LCD_DataWrite_GC9A01(0x00);
LCD_DataWrite_GC9A01(0x00);
LCD_DataWrite_GC9A01(0x01);
LCD_DataWrite_GC9A01(0x54);
LCD_DataWrite_GC9A01(0x10);
LCD_DataWrite_GC9A01(0x32);
LCD_DataWrite_GC9A01(0x98);



LCD_CtrlWrite_GC9A01(0x74);
LCD_DataWrite_GC9A01(0x10);
LCD_DataWrite_GC9A01(0x80);
LCD_DataWrite_GC9A01(0x80);
LCD_DataWrite_GC9A01(0x00);
LCD_DataWrite_GC9A01(0x00);
LCD_DataWrite_GC9A01(0x4E);
LCD_DataWrite_GC9A01(0x00);

LCD_CtrlWrite_GC9A01(0x98);
LCD_DataWrite_GC9A01(0x3e);
LCD_DataWrite_GC9A01(0x07);
LCD_CtrlWrite_GC9A01(0x99);	//bvee 2x
LCD_DataWrite_GC9A01(0x3e);
LCD_DataWrite_GC9A01(0x07);



LCD_CtrlWrite_GC9A01(0x35);
LCD_DataWrite_GC9A01(0x00);
LCD_CtrlWrite_GC9A01(0x21);

//--------end gamma setting--------------//
	LCD_CtrlWrite_GC9A01(0x11);
	SCI_Sleep(120);
	LCD_CtrlWrite_GC9A01(0x29);
	SCI_Sleep(20);
	//LCD_CtrlWrite_GC9A01(0x2c);

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E  GC9A01_EnterSleep(
    BOOLEAN is_sleep //SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
)
{
    SCI_TraceLow("lcd: GC9A01_EnterSleep, is_sleep = %d", is_sleep);
    gouda_event(0x90001104);

    if(is_sleep)
    {
        gouda_event(0x900f2dc4);
        LCD_CtrlWrite_GC9A01(0x28); //display off
        SCI_Sleep(120);
        LCD_CtrlWrite_GC9A01(0x10); // enter sleep mode
    }
    else
    {
        gouda_event(0x9f0f2ddf);
#if 0
        LCD_CtrlWrite_GC9A01(0x11); // sleep out mode
        hal_LcdDelay(120);
        LCD_CtrlWrite_GC9A01(0x29); // display on
#endif
        //hal_GoudatResetLcdPin();
        //hal_LcdDelay(120);
        GC9A01_Init();
    }

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL void GC9A01_Close(void)
{
    SCI_TraceLow("lcd: in GC9304_Close");
    gouda_event(0x90001105);

    GC9A01_EnterSleep( SCI_TRUE );
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E GC9A01_Invalidate(void)
{
    gouda_event(0x90001106);

    GC9A01_set_display_window (0x0, 0x0, LCD_WIDTH - 1, LCD_HEIGHT -1);
    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
//     To improve speed, lcd is operate in HIGH SPEED RAM WRITE MODE(4
//     uint16 are write continuously always.) So, some dummy uint16
//     should be inserted to satisfy this mode.   Please refer to spec.
/******************************************************************************/
PUBLIC ERR_LCD_E GC9A01_InvalidateRect(
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

   GC9A01_set_display_window(left, top, right, bottom);

    return ERR_LCD_NONE;
}

LOCAL ERR_LCD_E GC9A01_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
    gouda_event(0x90001108);

    SCI_TraceLow("lcd: GC9A01_RotationInvalidateRect");
    switch(angle)
    {
        case LCD_ANGLE_0:
            GC9A01_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_90:
            GC9A01_set_display_window(left, top, bottom,right);
            break;
        case LCD_ANGLE_180:
            GC9A01_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_270:
            GC9A01_set_display_window(left, top, bottom,right);
            break;
        default:
            GC9A01_set_display_window(left, top, right, bottom);
            break;
    }
    return ERR_LCD_NONE;
}

LOCAL ERR_LCD_E   GC9A01_SetContrast(uint16  contrast)
{
	//GcCheckDisplayStatus();
	SCI_TraceLow("lcd: GC9A01_SetContrast and check esd status");
	return ERR_LCD_FUNC_NOT_SUPPORT;


}
/*****************************************************************************/
LOCAL ERR_LCD_E   GC9A01_SetBrightness(uint16 brightness)
{
    SCI_TraceLow("lcd: GC9A01_SetBrightness");
    gouda_event(0x9000110a);

    return ERR_LCD_FUNC_NOT_SUPPORT;
}

//  Description:    Enable lcd to partial display mode, so can save power.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//  Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E GC9A01_SetDisplayWindow(
    uint16 left, //left of the window
    uint16 top, //top of the window
    uint16 right, //right of the window
    uint16 bottom //bottom of the window
)
{
    gouda_event(0x9000110b);

    SCI_TraceLow("lcd: GC9A01_SetDisplayWindow");
    GC9A01_set_display_window(left, top, right, bottom);
    return ERR_LCD_NONE;
}

LOCAL void GC9A01_InvalidatePixel(uint16 x, uint16 y, uint32 data)
{
    SCI_TraceLow("lcd: GC9A01_InvalidatePixel");
    gouda_event(0x9000110c);

    GC9A01_InvalidateRect(x,y,x,y);
    LCD_DataWrite_GC9A01(data);
}

LOCAL void GC9A01_GetConfig(void)
{
    uint32 count,k;
    LCM_CFG_INFO_T_PTR cfg_ptr = PNULL;
    cfg_ptr = LCM_GetCfgInfo(0,&count);
    for(k=0; k<count; k++)
    {
        if(cfg_ptr[k].dev_id==LCD_DRV_ID_GC9a01)
        {
            GC9A01_line_mode = cfg_ptr[k].adc_min;
            GC9A01_farmk = cfg_ptr[k].adc_max;

            break;
        }
    }
    SCI_TraceLow("lcd: GC9A01_GetConfig: GC9A01_line_mode = %d , GC9A01_farmk =%d\n",GC9A01_line_mode,GC9A01_farmk);  //0x80,0xf5,0x3e
}


LOCAL uint32 GC9A01_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint8 id[4] = {0};
    GC9A01_GetConfig();

    hal_gouda_sim_init(GC9A01_line_mode,GC9A01_farmk,0x18000);
    mon_Event(0xadff9307);

    hal_LcdDelay(140);
    hwp_iomux_6121e->pad_spi_lcd_sio |= (1<<12);
    hal_LcdDelay(10);
    hal_GoudaReadData(0x04,id,4);
    hwp_iomux_6121e->pad_spi_lcd_sio &= ~(1<<12);
    hal_LcdDelay(10);
    SCI_TraceLow("lcd: GC9A01_ReadID: id1->0x%0x, id2->0x%x, id3->0x%0x\n",id[3], id[2], id[1]);

    mon_Event((id[3]<<16)|(id[2] <<8)| id[1]);

    hal_gouda_term();

    return ((id[3]<<16)|(id[2] <<8)| id[1]);
}


const LCD_OPERATIONS_T GC9A01_operations =
{
    GC9A01_Init,
    GC9A01_EnterSleep,
    GC9A01_SetContrast,
    GC9A01_SetBrightness,
    GC9A01_SetDisplayWindow,
    GC9A01_InvalidateRect,
    GC9A01_Invalidate,
    GC9A01_Close,
    GC9A01_RotationInvalidateRect,
    GC9A01_SetDirection,
    NULL,
    GC9A01_ReadID
};

LOCAL const LCD_TIMING_U s_GC9A01_lcmtiming =
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

LOCAL const LCD_TIMING_U s_GC9A01_spitiming =
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
const LCD_SPEC_T g_lcd_GC9a01 =
{
    LCD_WIDTH,
    LCD_HEIGHT,
    SCI_NULL,
    SCI_NULL,
    WIDTH_8,
    (LCD_TIMING_U*)&s_GC9A01_spitiming,
    (LCD_OPERATIONS_T*)&GC9A01_operations,
    1,
    0
};
#else // not verify lcm
const LCD_SPEC_T g_lcd_GC9a01 =
{
    LCD_WIDTH,
    LCD_HEIGHT,
    LCD_MCU,
    BUS_MODE_8080,
    WIDTH_8,
    (LCD_TIMING_U*)&s_GC9A01_lcmtiming,
    (LCD_OPERATIONS_T*)&GC9A01_operations,
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


