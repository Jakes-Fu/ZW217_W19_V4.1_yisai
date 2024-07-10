/******************************************************************************
 ** File Name:     tft_NV3029.c                                             *
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

#define SCI_TraceLow_Test SCI_TraceLow

LOCAL uint16 NV3029_line_mode = 0;
LOCAL uint16 NV3029_farmk = 0;


#define LCM_WR_REG(Addr, Data)                              \
{                                                       \
    while (hal_GoudaWriteReg(Addr, Data) != HAL_ERR_NO) \
    ;                                               \
}
#define LCD_DataWrite_NV3029(Data)                               \
{                                                  \
    while (hal_GoudaWriteData(Data) != HAL_ERR_NO) \
    ;                                          \
}
#define LCD_CtrlWrite_NV3029(Cmd)                              \
{                                                \
    while (hal_GoudaWriteCmd(Cmd) != HAL_ERR_NO) \
    ;                                        \
}

//#define LCD_CtrlData_NV3029(cmd, data)  LCD_CtrlWrite_NV3029( cmd ); LCD_DataWrite_NV3029( data );

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
LOCAL void NV3029_Close(void);

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  NV3029_EnterSleep(BOOLEAN is_sleep);

/*********************************************************************/
//  Description:   Initialize color LCD : GC9304
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E NV3029_Init(void);

LOCAL ERR_LCD_E NV3029_Invalidate(void);
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
LOCAL ERR_LCD_E NV3029_InvalidateRect(
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
LOCAL void NV3029_set_display_window(
        uint16 left, // start Horizon address
        uint16 right, // end Horizon address
        uint16 top, // start Vertical address
        uint16 bottom // end Vertical address
        );

LOCAL uint32 NV3029_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id);

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

LOCAL ERR_LCD_E NV3029_SetDirection(LCD_DIRECT_E direct_type)
{
   // gouda_event(0x90001101);

    SCI_TraceLow_Test("lcd: NV3029_SetDirection = %d",direct_type);
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
    LCD_CtrlWrite_NV3029(0x36);
    LCD_DataWrite_NV3029(s_lcd_madc.dataval);
    LCD_CtrlWrite_NV3029(0x2c);
	#endif
    return ERR_LCD_NONE;

}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//	Note:
/******************************************************************************/
LOCAL void NV3029_set_display_window(
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
    // SCI_TraceLow_Test("[DRV_LCD]NV3029_set_display_window L = %d, top = %d, R = %d, bottom = %d",left,top,right,bottom);
    gouda_event(0x90001102);
    //hal_GoudaChangeSerialMode(HAL_GOUDA_SPI_LINE_3);
    LCD_CtrlWrite_NV3029(0x2a);	// set hori start , end (left, right)
    LCD_DataWrite_NV3029((newleft>>8)&0xff); // left high 8 b
    LCD_DataWrite_NV3029(newleft&0xff); // left low 8 b
    LCD_DataWrite_NV3029((newright>>8)&0xff); // right high 8 b
    LCD_DataWrite_NV3029(newright&0xff); // right low 8 b

    LCD_CtrlWrite_NV3029(0x2b);	// set vert start , end (top, bot)
    LCD_DataWrite_NV3029((newtop>>8)&0xff); // top high 8 b
    LCD_DataWrite_NV3029(newtop&0xff); // top low 8 b
    LCD_DataWrite_NV3029((newbottom>>8)&0xff); // bot high 8 b
    LCD_DataWrite_NV3029(newbottom&0xff); // bot low 8 b
    LCD_CtrlWrite_NV3029(0x2c);	 // recover memory write mode
    //hal_GoudaChangeSerialMode(HAL_GOUDA_SPI_LINE_3_2_LANE);

}

/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E NV3029_Init(void)
{
    SCI_TraceLow_Test("lcd: NV3029_Init here");

    mon_Event(0x55555555);


#if 0  // use NV3029 spi org init code 
    LCD_CtrlWrite_NV3029(0xfe);
    LCD_CtrlWrite_NV3029(0xef);

    LCD_CtrlWrite_NV3029(0x35);
    LCD_DataWrite_NV3029(0x00);

    LCD_CtrlWrite_NV3029(0x36);
    LCD_DataWrite_NV3029(0x48);

    LCD_CtrlWrite_NV3029(0x3a);
    LCD_DataWrite_NV3029(0x05);

    LCD_CtrlWrite_NV3029(0xa4);
    LCD_DataWrite_NV3029(0x44);
    LCD_DataWrite_NV3029(0x44);

    LCD_CtrlWrite_NV3029(0xa5);
    LCD_DataWrite_NV3029(0x42);
    LCD_DataWrite_NV3029(0x42);

    LCD_CtrlWrite_NV3029(0xaa);
    LCD_DataWrite_NV3029(0x88);
    LCD_DataWrite_NV3029(0x88);

    LCD_CtrlWrite_NV3029(0xe8);
    LCD_DataWrite_NV3029(0x11);
    LCD_DataWrite_NV3029(0x77);

    LCD_CtrlWrite_NV3029(0xe3);
    LCD_DataWrite_NV3029(0x01);
    LCD_DataWrite_NV3029(0x18);

    LCD_CtrlWrite_NV3029(0xe1);
    LCD_DataWrite_NV3029(0x10);
    LCD_DataWrite_NV3029(0x0a);

    LCD_CtrlWrite_NV3029(0xac);
    LCD_DataWrite_NV3029(0x00);

    LCD_CtrlWrite_NV3029(0xaf);
    LCD_DataWrite_NV3029(0x67);

    LCD_CtrlWrite_NV3029(0xa6);
    LCD_DataWrite_NV3029(0x29);
    LCD_DataWrite_NV3029(0x29);

    LCD_CtrlWrite_NV3029(0xa7);
    LCD_DataWrite_NV3029(0x27);
    LCD_DataWrite_NV3029(0x27);

    LCD_CtrlWrite_NV3029(0xa8);
    LCD_DataWrite_NV3029(0x17);
    LCD_DataWrite_NV3029(0x17);

    LCD_CtrlWrite_NV3029(0xa9);
    LCD_DataWrite_NV3029(0x26);
    LCD_DataWrite_NV3029(0x26);

    //----gamma setting---------//
    LCD_CtrlWrite_NV3029(0xf0);
    LCD_DataWrite_NV3029(0x02);
    LCD_DataWrite_NV3029(0x02);
    LCD_DataWrite_NV3029(0x00);
    LCD_DataWrite_NV3029(0x02);
    LCD_DataWrite_NV3029(0x07);
    LCD_DataWrite_NV3029(0x0c);

    LCD_CtrlWrite_NV3029(0xf1);
    LCD_DataWrite_NV3029(0x01);
    LCD_DataWrite_NV3029(0x01);
    LCD_DataWrite_NV3029(0x00);
    LCD_DataWrite_NV3029(0x03);
    LCD_DataWrite_NV3029(0x07);
    LCD_DataWrite_NV3029(0x0f);

    LCD_CtrlWrite_NV3029(0xf2);
    LCD_DataWrite_NV3029(0x0D);
    LCD_DataWrite_NV3029(0x08);
    LCD_DataWrite_NV3029(0x37);
    LCD_DataWrite_NV3029(0x04);
    LCD_DataWrite_NV3029(0x04);
    LCD_DataWrite_NV3029(0x4b);

    LCD_CtrlWrite_NV3029(0xf3);
    LCD_DataWrite_NV3029(0x11);
    LCD_DataWrite_NV3029(0x0c);
    LCD_DataWrite_NV3029(0x37);
    LCD_DataWrite_NV3029(0x04);
    LCD_DataWrite_NV3029(0x04);
    LCD_DataWrite_NV3029(0x47);

    LCD_CtrlWrite_NV3029(0xf4);
    LCD_DataWrite_NV3029(0x0a);
    LCD_DataWrite_NV3029(0x15);
    LCD_DataWrite_NV3029(0x15);
    LCD_DataWrite_NV3029(0x24);
    LCD_DataWrite_NV3029(0x3a);
    LCD_DataWrite_NV3029(0x0F);

    LCD_CtrlWrite_NV3029(0xf5);
    LCD_DataWrite_NV3029(0x07);
    LCD_DataWrite_NV3029(0x0f);
    LCD_DataWrite_NV3029(0x0d);
    LCD_DataWrite_NV3029(0x17);
    LCD_DataWrite_NV3029(0x3a);
    LCD_DataWrite_NV3029(0x0F);

#else

   
		LCD_CtrlWrite_NV3029(0x01); 
		LCD_Delayms(300); 
	
		LCD_CtrlWrite_NV3029(0xfd); 
		LCD_DataWrite_NV3029(0x06); 
		LCD_DataWrite_NV3029(0x07); 
		LCD_CtrlWrite_NV3029(0x66); 
		LCD_DataWrite_NV3029(0x80); 
		LCD_CtrlWrite_NV3029(0x80); 
		LCD_DataWrite_NV3029(0x05); 
		LCD_Delayms(5); 
		LCD_CtrlWrite_NV3029(0x80); 
		LCD_DataWrite_NV3029(0x01); 
		LCD_Delayms(100); 
		LCD_CtrlWrite_NV3029(0xb6); 
		LCD_DataWrite_NV3029(0x02);//rev,gs,ss,sm 
		LCD_DataWrite_NV3029(0xa2); 
		LCD_CtrlWrite_NV3029(0x60); 
		LCD_DataWrite_NV3029(0x2a);//vrh 
		LCD_CtrlWrite_NV3029(0x63); 
		LCD_DataWrite_NV3029(0x08); 
		LCD_CtrlWrite_NV3029(0x64); 
		LCD_DataWrite_NV3029(0x0c); 
		LCD_CtrlWrite_NV3029(0x68); 
		LCD_DataWrite_NV3029(0x70); 
		LCD_CtrlWrite_NV3029(0x69); 
		LCD_DataWrite_NV3029(0x1f); 
		LCD_CtrlWrite_NV3029(0x6a); 
		LCD_DataWrite_NV3029(0xc4); 
		LCD_CtrlWrite_NV3029(0x6b); //VCM5-0 
		LCD_DataWrite_NV3029(0x1f);	//	 23  1f
		//vcomh_rq+=1;
		LCD_CtrlWrite_NV3029(0x6c); //VDV4-0 
		LCD_DataWrite_NV3029(0x1a); 
		LCD_CtrlWrite_NV3029(0x6d); 
		LCD_DataWrite_NV3029(0x77); 
		LCD_CtrlWrite_NV3029(0x6e); 
		LCD_DataWrite_NV3029(0x84); 
		LCD_CtrlWrite_NV3029(0x6f); 
		LCD_DataWrite_NV3029(0x48); 
		LCD_CtrlWrite_NV3029(0xf7); //frc_en 
		LCD_DataWrite_NV3029(0x10); 
		LCD_CtrlWrite_NV3029(0x70); 
		LCD_DataWrite_NV3029(0x44); 
		LCD_CtrlWrite_NV3029(0x71); 
		LCD_DataWrite_NV3029(0x05); 
		LCD_CtrlWrite_NV3029(0xf6); 
		LCD_DataWrite_NV3029(0x01); 
		LCD_DataWrite_NV3029(0x10); 
		LCD_DataWrite_NV3029(0x00);//80--spi_2wire_mode,00 
	
		LCD_CtrlWrite_NV3029(0xed); 
		LCD_DataWrite_NV3029(0xf9); 
		LCD_DataWrite_NV3029(0xf9); 
		LCD_DataWrite_NV3029(0x00); 
		LCD_DataWrite_NV3029(0x00); 
		LCD_DataWrite_NV3029(0x11); 
		LCD_DataWrite_NV3029(0x00); 
	
		LCD_CtrlWrite_NV3029(0xe0);								//gmama set 2.4 
		LCD_DataWrite_NV3029(0x12); //PKP0[4:0] 
		LCD_DataWrite_NV3029(0x1b); //PKP1[4:0] 
		LCD_DataWrite_NV3029(0x13); //PKP2[4:0] 
		LCD_DataWrite_NV3029(0x1b); //PKP3[4:0] 
		LCD_DataWrite_NV3029(0x09); //PKP4[4:0] 
		LCD_DataWrite_NV3029(0x12); //PKP5[4:0] 
		LCD_DataWrite_NV3029(0x17); //PKP6[4:0] 
		LCD_CtrlWrite_NV3029(0xe1); 
		LCD_DataWrite_NV3029(0x2d); //PRP0[6:0] 
		LCD_DataWrite_NV3029(0x72); //PRP1[6:0] 
		LCD_CtrlWrite_NV3029(0xe2); 
		LCD_DataWrite_NV3029(0x09); //VRP0[5:0] 
		LCD_DataWrite_NV3029(0x1e); //VRP1[5:0] 
		LCD_DataWrite_NV3029(0x22); //VRP2[5:0] 
		LCD_DataWrite_NV3029(0x1e); //VRP3[5:0] 
		LCD_DataWrite_NV3029(0x16); //VRP4[5:0] 
		LCD_DataWrite_NV3029(0x18); //VRP5[5:0] 
		LCD_CtrlWrite_NV3029(0xe3); 
		LCD_DataWrite_NV3029(0x0b); //PKN0[4:0] 
		LCD_DataWrite_NV3029(0x16); //PKN1[4:0] 
		LCD_DataWrite_NV3029(0x03); //PKN2[4:0] 
		LCD_DataWrite_NV3029(0x0e); //PKN3[4:0] 
		LCD_DataWrite_NV3029(0x09); //PKN4[4:0] 
		LCD_DataWrite_NV3029(0x12); //PKN5[4:0] 
		LCD_DataWrite_NV3029(0x02); //PKN6[4:0] 
		LCD_CtrlWrite_NV3029(0xe4); 
		LCD_DataWrite_NV3029(0x08); //PRN0[6:0] 
		LCD_DataWrite_NV3029(0x52); //PRN 1[6:0] 
		LCD_CtrlWrite_NV3029(0xe5); 
		LCD_DataWrite_NV3029(0x1c); //VRN0[5:0] 
		LCD_DataWrite_NV3029(0x24); //VRN 1[5:0] 
		LCD_DataWrite_NV3029(0x1a); //VRN2[5:0] 
		LCD_DataWrite_NV3029(0x1d); //VRN3[5:0] 
		LCD_DataWrite_NV3029(0x1d); //VRN4[5:0] 
		LCD_DataWrite_NV3029(0x21); //VRN5[5:0] 
	
		LCD_CtrlWrite_NV3029(0x66);				//OSC trim 
		LCD_DataWrite_NV3029(0x96); 
		LCD_CtrlWrite_NV3029(0x67); //i_trim 
		LCD_DataWrite_NV3029(0x07); 
		LCD_CtrlWrite_NV3029(0xb1); //frame rate 
		LCD_DataWrite_NV3029(0x00);
		LCD_DataWrite_NV3029(0x12); 
		LCD_CtrlWrite_NV3029(0xec); 
		LCD_DataWrite_NV3029(0x33); 
		LCD_DataWrite_NV3029(0x11); 
		LCD_DataWrite_NV3029(0x11); 
		LCD_DataWrite_NV3029(0x00); 
		LCD_DataWrite_NV3029(0x18); 
		LCD_DataWrite_NV3029(0x18); 
	
		LCD_CtrlWrite_NV3029(0xfd); 
		LCD_DataWrite_NV3029(0xfa); 
		LCD_DataWrite_NV3029(0xfb); 
	
		LCD_CtrlWrite_NV3029(0x11); // exit sleep 
		LCD_Delayms(300); 
	
		LCD_CtrlWrite_NV3029(0x36); 
		LCD_DataWrite_NV3029(0x08); 
		LCD_CtrlWrite_NV3029(0x3a); 
		LCD_DataWrite_NV3029(0x65); 
	
		LCD_CtrlWrite_NV3029(0x29); // display on 
		LCD_Delayms(10); 
   
#endif
    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  NV3029_EnterSleep(
        BOOLEAN is_sleep //SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
        )
{
    SCI_TraceLow("lcd: NV3029_EnterSleep, is_sleep = %d", is_sleep);
   // gouda_event(0x90001104);

    if(is_sleep)
    {
       // gouda_event(0x900f2dc4);
        LCD_CtrlWrite_NV3029(0x28); //display off
        hal_LcdDelay(120);
        LCD_CtrlWrite_NV3029(0x10); // enter sleep mode
    }
    else
    {

       // gouda_event(0x9f0f2ddf);
#if 0
        LCD_CtrlWrite_NV3029(0x11); // sleep out mode
        hal_LcdDelay(120);
        LCD_CtrlWrite_NV3029(0x29); // display on
#endif
        NV3029_Init();
    }

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void NV3029_Close(void)
{
    SCI_TraceLow("lcd:  in GC9304_Close");
    gouda_event(0x90001105);

    NV3029_EnterSleep( SCI_TRUE );
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E NV3029_Invalidate(void)
{
    gouda_event(0x90001106);

    NV3029_set_display_window (0x0, 0x0, LCD_WIDTH - 1, LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E NV3029_InvalidateRect(
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

    NV3029_set_display_window(left, top, right, bottom);
    return ERR_LCD_NONE;
}

LOCAL ERR_LCD_E NV3029_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
    gouda_event(0x90001108);

    SCI_TraceLow("lcd:  NV3029_RotationInvalidateRect");
    switch(angle)
    {
        case LCD_ANGLE_0:
            NV3029_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_90:
            NV3029_set_display_window(left, top, bottom,right);
            break;
        case LCD_ANGLE_180:
            NV3029_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_270:
            NV3029_set_display_window(left, top, bottom,right);
            break;
        default:
            NV3029_set_display_window(left, top, right, bottom);
            break;
    }
    return ERR_LCD_NONE;
}

extern void hal_GoudatResetLcdPin(void);

#define LCD_DEFAULT_STATUS_09H 0xa4530000

LOCAL void GcCheckDisplayStatus(void)
{
    uint8 id[4] = {0};
    uint32 lcdstatus =0;

    hal_GoudaReadData(0x09, id, 4);

    lcdstatus = ((id[3]<<24) | (id[2]<<16));
	
    SCI_TraceLow("lcd: NV3029 09=0x%x\n",lcdstatus);
    if(LCD_DEFAULT_STATUS_09H != lcdstatus)
    {
        hal_GoudatResetLcdPin();
        hal_LcdDelay(120);
        NV3029_Init();
        SCI_TraceLow("lcd: NV3029 ReadDisplayStatus: RESET  \n");
    }
    else
    {
        SCI_TraceLow("lcd: NV3029 LCD_DEFAULT_STATUS_09H   \n");
        LCD_CtrlWrite_NV3029(0xfe);
        LCD_CtrlWrite_NV3029(0xef);

        LCD_CtrlWrite_NV3029(0x11);
        LCD_CtrlWrite_NV3029(0x38);
        LCD_CtrlWrite_NV3029(0x20);

        LCD_CtrlWrite_NV3029(0x3a);
        LCD_DataWrite_NV3029(0x05);

        LCD_CtrlWrite_NV3029(0x35);
        LCD_DataWrite_NV3029(0x00);

        LCD_CtrlWrite_NV3029(0x36);
        LCD_DataWrite_NV3029(0x48);

        LCD_CtrlWrite_NV3029(0x29);
    }
}


LOCAL ERR_LCD_E   NV3029_SetContrast(uint16  contrast)
{
    gouda_event(0x90001109);

    SCI_TraceLow("lcd: NV3029_SetContrast");
   // GcCheckDisplayStatus();
    return ERR_LCD_FUNC_NOT_SUPPORT;
} 
/*****************************************************************************/
LOCAL ERR_LCD_E   NV3029_SetBrightness(uint16 brightness)
{
    SCI_TraceLow("lcd:  NV3029_SetBrightness");
   // gouda_event(0x9000110a);

    return ERR_LCD_FUNC_NOT_SUPPORT;
}

//  Description:    Enable lcd to partial display mode, so can save power.
//	Global resource dependence: 
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//	Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E NV3029_SetDisplayWindow(
        uint16 left, //left of the window
        uint16 top, //top of the window
        uint16 right, //right of the window
        uint16 bottom //bottom of the window
        )
{
    gouda_event(0x9000110b);

    SCI_TraceLow("lcd:  NV3029_SetDisplayWindow");
    NV3029_set_display_window(left, top, right, bottom);
    return ERR_LCD_NONE;
}

LOCAL void NV3029_InvalidatePixel(uint16 x, uint16 y, uint32 data)
{
    SCI_TraceLow("lcd:  NV3029_InvalidatePixel");
    gouda_event(0x9000110c);

    NV3029_InvalidateRect(x,y,x,y);
    LCD_DataWrite_NV3029(data);
}

LOCAL void NV3029_GetConfig(void)
{
    uint32 count,k;
    LCM_CFG_INFO_T_PTR cfg_ptr = PNULL;
    cfg_ptr = LCM_GetCfgInfo(0,&count);
    for(k=0;k<count;k++)
    {
        if(cfg_ptr[k].dev_id==LCD_DRV_ID_NV3029)
        {
            NV3029_line_mode = cfg_ptr[k].adc_min;
            NV3029_farmk = cfg_ptr[k].adc_max;

            break;
        }
    }
    SCI_TraceLow("lcd: NV3029_GetConfig: NV3029_line_mode = %d , NV3029_farmk =%d\n",NV3029_line_mode,NV3029_farmk);  //0x80,0xf5,0x3e
}

uint32 _Read_NV3029_ID(void)
{
	uint32 lcm_dev_id = 0;
	uint16 low_id = 0, hi_id=0;
	(*(volatile uint32*)(0x8C000000+0x01E4)=(uint32)(BIT_0|0X00|0X00|(0X00<<5)|0X00|(0X00<<10)|0X00));

	LCD_CtrlWrite_NV3029(0xD3);
	LCM_Read(0,  1);
	LCM_Read(0,  1);
	hi_id =(uint16)LCM_Read(0,	1);
	low_id = (uint16)LCM_Read(0,  1);
	lcm_dev_id = (low_id & 0x00FF) + ((hi_id & 0x00FF)<<8);
	(*(volatile uint32*)(0x8C000000+0x01E4)=(uint32)(BIT_0|0X00|0X00|(0X00<<5)|0X00|(0X01<<10)|0X00));	 
	return lcm_dev_id;

}


LOCAL uint32 NV3029_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint32 lcm_dev_id = 0;
	uint16 low_id = 0, hi_id=0;
	(*(volatile uint32*)(0x8C000000+0x01E4)=(uint32)(BIT_0|0X00|0X00|(0X00<<5)|0X00|(0X00<<10)|0X00));

	LCD_CtrlWrite_NV3029(0xD3);
	LCM_Read(0,  1);
	LCM_Read(0,  1);
	hi_id =(uint16)LCM_Read(0,	1);
	low_id = (uint16)LCM_Read(0,  1);
	lcm_dev_id = (low_id & 0x00FF) + ((hi_id & 0x00FF)<<8);
	(*(volatile uint32*)(0x8C000000+0x01E4)=(uint32)(BIT_0|0X00|0X00|(0X00<<5)|0X00|(0X01<<10)|0X00));	 
	return lcm_dev_id;
}


const LCD_OPERATIONS_T NV3029_operations = 
{
    NV3029_Init,
    NV3029_EnterSleep,
    NV3029_SetContrast,
    NV3029_SetBrightness,
    NV3029_SetDisplayWindow,
    NV3029_InvalidateRect,
    NV3029_Invalidate,
    NV3029_Close,
    NV3029_RotationInvalidateRect,
    NV3029_SetDirection,
    NULL, 
    NV3029_ReadID	
};

LOCAL const LCD_TIMING_U s_NV3029_lcmtiming =
{
    20,  // CS setup time for LCM read (optional)  
    300, // low pulse width for LCM read (according spec)	
    300, // high pulse width for LCM read (according spec)
    15,   // CS setup time for LCM write  (optional) 	
    50,  // low pulse width for LCM write (according spec)
    50,  // high pulse width for LCM write (according spec)	
    // LCM_CYCLE_U end(ns)
};

LOCAL const LCD_TIMING_U s_NV3029_spitiming =
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
const LCD_SPEC_T g_lcd_NV3029 = 
{
    LCD_WIDTH, 
    LCD_HEIGHT, 
    SCI_NULL, 
    SCI_NULL,
    WIDTH_8,
    (LCD_TIMING_U*)&s_NV3029_spitiming, 
    (LCD_OPERATIONS_T*)&NV3029_operations,
    1,
    0
};
#else // not verify lcm
const LCD_SPEC_T g_lcd_NV3029 = 
{
    LCD_WIDTH, 
    LCD_HEIGHT, 
    LCD_MCU, 
    BUS_MODE_8080,
    WIDTH_8,
    (LCD_TIMING_U*)&s_NV3029_lcmtiming, 
    (LCD_OPERATIONS_T*)&NV3029_operations,
    1,
    0
};
#endif



