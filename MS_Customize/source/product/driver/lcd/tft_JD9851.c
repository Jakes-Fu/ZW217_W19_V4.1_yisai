/******************************************************************************
 ** File Name:     tft_JD9851.c                                             *
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


/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
extern   "C"
{
#endif


#define SCI_TraceLow_Test SCI_TraceLow

LOCAL uint16 JD9851_line_mode = 0;
LOCAL uint16 JD9851_farmk = 0;


#define LCM_WR_REG(Addr, Data)                              \
    {                                                       \
        while (hal_GoudaWriteReg(Addr, Data) != HAL_ERR_NO) \
            ;                                               \
    }
#define LCD_DataWrite_JD9851(Data)                               \
    {                                                  \
        while (hal_GoudaWriteData(Data) != HAL_ERR_NO) \
            ;                                          \
    }
#define LCD_CtrlWrite_JD9851(Cmd)                              \
    {                                                \
        while (hal_GoudaWriteCmd(Cmd) != HAL_ERR_NO) \
            ;                                        \
    }

//#define LCD_CtrlData_JD9851(cmd, data)  LCD_CtrlWrite_JD9851( cmd ); LCD_DataWrite_JD9851( data );

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



void JD9851_Init(void);

LOCAL MEM_DATA_ACCESS_CTRL_U s_lcd_madc;
uint32 jdidlcd=0;

void JD_CheckDisplayStatus(void);
void hal_GoudatResetLcdPin(void);
#define TX_AUTO_ACTIVATE    1


#define LCD_DEFAULT_STATUS_09H 0x80530600
#define LCD_DEFAULT_STATUS_0aH 0x39
LOCAL void JD_CheckDisplayStatus(void)
{
	uint8 id[4] = {0};
	uint32 lcdstatus =0;
	uint32 lcdstatus2 =0;
	uint32 lcdstatus45_1 =0;
	uint32 lcdstatus45_2 =0;
	uint32 lcdstatus45_3 =0;
		hal_GoudaReadData(0x0a,id,2);
	lcdstatus = id[0]&0xff;
	hal_GoudaReadData(0x09,id,4);
	lcdstatus2 = ((id[3]<<24)|(id[2]<<16)|(id[1] <<8)| id[0]);
	hal_GoudaReadData(0x45,id,2);
	lcdstatus45_1 = (id[0]<<8|id[1]);
	hal_GoudaReadData(0x45,id,2);
	lcdstatus45_2 = (id[0]<<8|id[1]);
	hal_GoudaReadData(0x45,id,2);
	lcdstatus45_3 = (id[0]<<8|id[1]);
	SCI_TraceLow("lcd: JD9851 0ah=0x%x 09h= 0x%x \n",lcdstatus,lcdstatus2);
	if((lcdstatus!=LCD_DEFAULT_STATUS_0aH)||(lcdstatus2!=LCD_DEFAULT_STATUS_09H))
	//	||((lcdstatus45_1==lcdstatus45_2)&&(lcdstatus45_2==lcdstatus45_3)))
	{
		hal_GoudatResetLcdPin();
		LDO_TurnOffLDO(LDO_LDO_LCD);
		hal_LcdDelay(100);
		LDO_TurnOnLDO(LDO_LDO_LCD); 
		hal_LcdDelay(100);
		JD9851_Init();
		SCI_TraceLow("lcd: JD9851 ReadDisplayStatus:rst \n");
	}
	else
	{
		LCD_CtrlWrite_JD9851(0xdf);
		LCD_DataWrite_JD9851(0x98);  
		LCD_DataWrite_JD9851(0x51);  
		LCD_DataWrite_JD9851(0xe9);  

 
		LCD_CtrlWrite_JD9851(0x11);
		hal_LcdDelay(10);
		LCD_CtrlWrite_JD9851(0xd7);
		LCD_DataWrite_JD9851(0x20); 
		
		
		LCD_CtrlWrite_JD9851(0xbb);
		LCD_DataWrite_JD9851(0x06);  
		LCD_DataWrite_JD9851(0x7a);  
		LCD_DataWrite_JD9851(0x30);  
		LCD_DataWrite_JD9851(0x30);  
		LCD_DataWrite_JD9851(0x6c);  
		LCD_DataWrite_JD9851(0x70);  
		LCD_DataWrite_JD9851(0x50);  
		LCD_DataWrite_JD9851(0x70);  
		LCD_CtrlWrite_JD9851(0x38);
		LCD_CtrlWrite_JD9851(0x20);
		LCD_CtrlWrite_JD9851(0x3a);
		LCD_DataWrite_JD9851(0x05);  

		LCD_CtrlWrite_JD9851(0x35);
		LCD_DataWrite_JD9851(0x00);  

		LCD_CtrlWrite_JD9851(0x36);
		LCD_DataWrite_JD9851(0x00);  
		LCD_CtrlWrite_JD9851(0x29);

	}

}



/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL void JD9851_Close(void);

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E  JD9851_EnterSleep(BOOLEAN is_sleep);

/*********************************************************************/
//  Description:   Initialize color LCD : GC9304
//  Input:
//      None.
//  Return:
//      None.
//  Note:
/*********************************************************************/
LOCAL void JD9851_Init(void);

LOCAL ERR_LCD_E JD9851_Invalidate(void);
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
LOCAL ERR_LCD_E JD9851_InvalidateRect(
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
LOCAL void JD9851_set_display_window(
    uint16 left, // start Horizon address
    uint16 right, // end Horizon address
    uint16 top, // start Vertical address
    uint16 bottom // end Vertical address
);

LOCAL uint32 JD9851_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id);

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

LOCAL ERR_LCD_E JD9851_SetDirection(LCD_DIRECT_E direct_type)
{
    // mon_Event(0x99c01101);mon_Event(direct_type);
    SCI_TraceLow_Test("lcd: JD9851_SetDirection = %d",direct_type);
	return ERR_LCD_NONE;
    /* bug 977659 63467 cause UNREACHABLE 
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
    LCD_CtrlWrite_JD9851(0x36);
    LCD_DataWrite_JD9851(s_lcd_madc.dataval);
    LCD_CtrlWrite_JD9851(0x2c);
    return ERR_LCD_NONE;
    */

}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//  Global resource dependence:
//  Note:
/******************************************************************************/
LOCAL void JD9851_set_display_window(
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
    //SCI_TraceLow_Test("[DRV_LCD]JD9851_set_display_window L = %d, top = %d, R = %d, bottom = %d",left,top,right,bottom);
    gouda_event(0x90001102);
//hal_GoudaChangeSerialMode(HAL_GOUDA_SPI_LINE_3);
    LCD_CtrlWrite_JD9851(0x2a); // set hori start , end (left, right)
    LCD_DataWrite_JD9851((newleft>>8)&0xff); // left high 8 b
    LCD_DataWrite_JD9851(newleft&0xff); // left low 8 b
    LCD_DataWrite_JD9851((newright>>8)&0xff); // right high 8 b
    LCD_DataWrite_JD9851(newright&0xff); // right low 8 b

    LCD_CtrlWrite_JD9851(0x2b); // set vert start , end (top, bot)
    LCD_DataWrite_JD9851((newtop>>8)&0xff); // top high 8 b
    LCD_DataWrite_JD9851(newtop&0xff); // top low 8 b
    LCD_DataWrite_JD9851((newbottom>>8)&0xff); // bot high 8 b
    LCD_DataWrite_JD9851(newbottom&0xff); // bot low 8 b
    LCD_CtrlWrite_JD9851(0x2c);  // recover memory write mode
//hal_GoudaChangeSerialMode(HAL_GOUDA_SPI_LINE_3_2_LANE);

}

#if 0
LOCAL ERR_LCD_E JD9851_Init(void)

{

    SCI_TraceLow_Test("lcd: JD9851_Init otp 77 ");

    LCD_CtrlWrite_JD9851(0xdf);    
    LCD_DataWrite_JD9851(0x98);
	LCD_DataWrite_JD9851(0x51);
    LCD_DataWrite_JD9851(0xe9);

    hal_LcdDelay(200);
    LCD_CtrlWrite_JD9851(0xde);   // page 0
    LCD_DataWrite_JD9851(0x00);

    LCD_CtrlWrite_JD9851(0x11);
    hal_LcdDelay(120);

    LCD_CtrlWrite_JD9851(0x3a);
    LCD_DataWrite_JD9851(0x05); //65k color
    hal_LcdDelay(20);

    LCD_CtrlWrite_JD9851(0x35);    
    LCD_DataWrite_JD9851(0x00);

    LCD_CtrlWrite_JD9851(0xb7);    
    LCD_DataWrite_JD9851(0x33);
    LCD_DataWrite_JD9851(0xa0);
    LCD_DataWrite_JD9851(0x33);
    LCD_DataWrite_JD9851(0x24);


    LCD_CtrlWrite_JD9851(0xc3);    
    LCD_DataWrite_JD9851(0x0c);
	LCD_DataWrite_JD9851(0x00);
    LCD_DataWrite_JD9851(0x0a);
    LCD_DataWrite_JD9851(0x10);
    LCD_DataWrite_JD9851(0x08);
    LCD_DataWrite_JD9851(0x54);
    LCD_DataWrite_JD9851(0x45);
    LCD_DataWrite_JD9851(0x71);
    LCD_DataWrite_JD9851(0x2c);
	
    hal_LcdDelay(20);

    LCD_CtrlWrite_JD9851(0x29);
    hal_LcdDelay(20);

    LCD_CtrlWrite_JD9851(0xd7);
    LCD_DataWrite_JD9851(0x20);

    LCD_DataWrite_JD9851(0x00);
	
    LCD_CtrlWrite_JD9851(0x29);


}

#endif
/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
#if 1
LOCAL void JD9851_Init(void)
{

#if 0
//PASSWORD
LCD_CtrlWrite_JD9851(0xDF); //Password
LCD_DataWrite_JD9851(0x98);
LCD_DataWrite_JD9851(0x51);
LCD_DataWrite_JD9851(0xE9);


//---------------- PAGE0 --------------
LCD_CtrlWrite_JD9851(0xDE);  	
LCD_DataWrite_JD9851(0x00);  



//VGMP,VGSP,VGMN,VGSN
LCD_CtrlWrite_JD9851(0xB7);  	
LCD_DataWrite_JD9851(0x39);   	
LCD_DataWrite_JD9851(0xB7);	
LCD_DataWrite_JD9851(0x40);  	
LCD_DataWrite_JD9851(0x29); 


//Set_R_GAMMA
LCD_CtrlWrite_JD9851(0xC8);  	
LCD_DataWrite_JD9851(0x3C);	
LCD_DataWrite_JD9851(0x35);
LCD_DataWrite_JD9851(0x31);  	
LCD_DataWrite_JD9851(0x32);  	
LCD_DataWrite_JD9851(0x36);  	
LCD_DataWrite_JD9851(0x3A);  	
LCD_DataWrite_JD9851(0x37);  	
LCD_DataWrite_JD9851(0x36);  	
LCD_DataWrite_JD9851(0x36);  	
LCD_DataWrite_JD9851(0x33);  	
LCD_DataWrite_JD9851(0x2D);  	
LCD_DataWrite_JD9851(0x1D);  	
LCD_DataWrite_JD9851(0x16);  	
LCD_DataWrite_JD9851(0x0F);  	
LCD_DataWrite_JD9851(0x0A);  	
LCD_DataWrite_JD9851(0x04);  	
LCD_DataWrite_JD9851(0x3C);	
LCD_DataWrite_JD9851(0x35);
LCD_DataWrite_JD9851(0x31);  	
LCD_DataWrite_JD9851(0x32);  	
LCD_DataWrite_JD9851(0x36);  	
LCD_DataWrite_JD9851(0x3A);  	
LCD_DataWrite_JD9851(0x37);  	
LCD_DataWrite_JD9851(0x36);  	
LCD_DataWrite_JD9851(0x36);  	
LCD_DataWrite_JD9851(0x33);  	
LCD_DataWrite_JD9851(0x2D);  	
LCD_DataWrite_JD9851(0x1D);  	
LCD_DataWrite_JD9851(0x16);  	
LCD_DataWrite_JD9851(0x0F);  	
LCD_DataWrite_JD9851(0x0A);  	
LCD_DataWrite_JD9851(0x04);  	

//POW_CTRL
LCD_CtrlWrite_JD9851(0xB9);  	
LCD_DataWrite_JD9851(0x33);	   	
LCD_DataWrite_JD9851(0x68);
LCD_DataWrite_JD9851(0xCC);

//DCDC_SEL 
LCD_CtrlWrite_JD9851(0xBB);  	
LCD_DataWrite_JD9851(0x4F);  	
LCD_DataWrite_JD9851(0x7E);
LCD_DataWrite_JD9851(0xC0);  	
LCD_DataWrite_JD9851(0x40);  	
LCD_DataWrite_JD9851(0x6C);  	
LCD_DataWrite_JD9851(0x50);  	
LCD_DataWrite_JD9851(0x60);  	
LCD_DataWrite_JD9851(0x70);

//VDDD_CTRL 
LCD_CtrlWrite_JD9851(0xBC);  	
LCD_DataWrite_JD9851(0x38);  	
LCD_DataWrite_JD9851(0x3C);

//SETSTBA  
LCD_CtrlWrite_JD9851(0xC0);
LCD_DataWrite_JD9851(0x31);
LCD_DataWrite_JD9851(0x20);

//SETPANEL(default)  
LCD_CtrlWrite_JD9851(0xC1);  	
LCD_DataWrite_JD9851(0x12); 

//SETRGBCYC 
LCD_CtrlWrite_JD9851(0xC3);  	
LCD_DataWrite_JD9851(0x08);
LCD_DataWrite_JD9851(0x00);  	
LCD_DataWrite_JD9851(0x0F);
LCD_DataWrite_JD9851(0x1F);  	
LCD_DataWrite_JD9851(0x1C);  	
LCD_DataWrite_JD9851(0x7E);  	
LCD_DataWrite_JD9851(0x7C);  	
LCD_DataWrite_JD9851(0x98);  	
LCD_DataWrite_JD9851(0x2C);

//SETRGBCYC(default)  
LCD_CtrlWrite_JD9851(0xC4);  	
LCD_DataWrite_JD9851(0x00);
LCD_DataWrite_JD9851(0x78);  	
LCD_DataWrite_JD9851(0x9E);
LCD_DataWrite_JD9851(0x0E);  	
LCD_DataWrite_JD9851(0x0A);  	
LCD_DataWrite_JD9851(0x16);  	
LCD_DataWrite_JD9851(0x9E);  	
LCD_DataWrite_JD9851(0x0E);  	
LCD_DataWrite_JD9851(0x0A);
LCD_DataWrite_JD9851(0x16);  	
LCD_DataWrite_JD9851(0x9E);  	
LCD_DataWrite_JD9851(0x0E);  	
LCD_DataWrite_JD9851(0x0A);  	
LCD_DataWrite_JD9851(0x16);  	
LCD_DataWrite_JD9851(0x82);  	
LCD_DataWrite_JD9851(0x00);  	
LCD_DataWrite_JD9851(0x03);


//SET_GD(default)
LCD_CtrlWrite_JD9851(0xD0);  	
LCD_DataWrite_JD9851(0x04);
LCD_DataWrite_JD9851(0x3F);  	
LCD_DataWrite_JD9851(0x90);
LCD_DataWrite_JD9851(0x0E);  	
LCD_DataWrite_JD9851(0x00);  	
LCD_DataWrite_JD9851(0x03);

//RAMCTRL(default)
LCD_CtrlWrite_JD9851(0xD7);  	
LCD_DataWrite_JD9851(0x00);
LCD_DataWrite_JD9851(0x00);  

//---------------- PAGE2 --------------
LCD_CtrlWrite_JD9851(0xDE);  	
LCD_DataWrite_JD9851(0x02);  
hal_LcdDelay(1);

//DCDC_SET
LCD_CtrlWrite_JD9851(0xB8);  	
LCD_DataWrite_JD9851(0x1D);
LCD_DataWrite_JD9851(0xB6);
LCD_DataWrite_JD9851(0x2F);
LCD_DataWrite_JD9851(0x08);
LCD_DataWrite_JD9851(0x1E);

//POWER_OPT
LCD_CtrlWrite_JD9851(0xB2);  	
LCD_DataWrite_JD9851(0x30);
LCD_DataWrite_JD9851(0xF8);
LCD_DataWrite_JD9851(0x93);
LCD_DataWrite_JD9851(0x6E);

//SETRGBCYC2
LCD_CtrlWrite_JD9851(0xC1);  	
LCD_DataWrite_JD9851(0x10);
LCD_DataWrite_JD9851(0x66);
LCD_DataWrite_JD9851(0x66);
LCD_DataWrite_JD9851(0x01);

LCD_CtrlWrite_JD9851(0xC4);  	
LCD_DataWrite_JD9851(0x7E);
LCD_DataWrite_JD9851(0x0E);


//---------------- PAGE0 --------------
LCD_CtrlWrite_JD9851(0xDE);  	
LCD_DataWrite_JD9851(0x00);  


// sleep out
//SSD_Start();//CSn pull low
LCD_CtrlWrite_JD9851(0x11);  	
hal_LcdDelay(120);

LCD_CtrlWrite_JD9851(0x29);  	// SLPOUT
LCD_CtrlWrite_JD9851(0x2C);
#endif

#if 1
hal_LcdDelay(120);

LCD_CtrlWrite_JD9851(0xDF); //Password
LCD_DataWrite_JD9851(0x98);
LCD_DataWrite_JD9851(0x51);
LCD_DataWrite_JD9851(0xE9);


LCD_CtrlWrite_JD9851(0xDE);
LCD_DataWrite_JD9851(0x00);


LCD_CtrlWrite_JD9851(0xB7);
LCD_DataWrite_JD9851(0x23);
LCD_DataWrite_JD9851(0x7A);
LCD_DataWrite_JD9851(0x23);
LCD_DataWrite_JD9851(0x1E);


LCD_CtrlWrite_JD9851(0xC8);
LCD_DataWrite_JD9851(0x3F);	//RPA
LCD_DataWrite_JD9851(0x3C);
LCD_DataWrite_JD9851(0x3C);
LCD_DataWrite_JD9851(0x3E);
LCD_DataWrite_JD9851(0x3F);
LCD_DataWrite_JD9851(0x3F);
LCD_DataWrite_JD9851(0x3A);
LCD_DataWrite_JD9851(0x3B);
LCD_DataWrite_JD9851(0x3A);
LCD_DataWrite_JD9851(0x3A);
LCD_DataWrite_JD9851(0x39);
LCD_DataWrite_JD9851(0x35);
LCD_DataWrite_JD9851(0x34);
LCD_DataWrite_JD9851(0x34);
LCD_DataWrite_JD9851(0x31);
LCD_DataWrite_JD9851(0x0E);
LCD_DataWrite_JD9851(0x3F);	//RPA
LCD_DataWrite_JD9851(0x3C);
LCD_DataWrite_JD9851(0x3C);
LCD_DataWrite_JD9851(0x3E);
LCD_DataWrite_JD9851(0x3F);
LCD_DataWrite_JD9851(0x3F);
LCD_DataWrite_JD9851(0x3A);
LCD_DataWrite_JD9851(0x3B);
LCD_DataWrite_JD9851(0x3A);
LCD_DataWrite_JD9851(0x3A);
LCD_DataWrite_JD9851(0x39);
LCD_DataWrite_JD9851(0x35);
LCD_DataWrite_JD9851(0x34);
LCD_DataWrite_JD9851(0x34);
LCD_DataWrite_JD9851(0x31);
LCD_DataWrite_JD9851(0x0E);

LCD_CtrlWrite_JD9851(0xB9);
LCD_DataWrite_JD9851(0x33);
LCD_DataWrite_JD9851(0x28);
LCD_DataWrite_JD9851(0xCC);

LCD_CtrlWrite_JD9851(0xBB);
LCD_DataWrite_JD9851(0x47);
LCD_DataWrite_JD9851(0x5A);
LCD_DataWrite_JD9851(0x30);
LCD_DataWrite_JD9851(0x30);
LCD_DataWrite_JD9851(0x6C);
LCD_DataWrite_JD9851(0x60);
LCD_DataWrite_JD9851(0x60);
LCD_DataWrite_JD9851(0x70);

LCD_CtrlWrite_JD9851(0xBC);
LCD_DataWrite_JD9851(0x38);
LCD_DataWrite_JD9851(0x3C);

LCD_CtrlWrite_JD9851(0xC0);
LCD_DataWrite_JD9851(0x22);
LCD_DataWrite_JD9851(0x20);

LCD_CtrlWrite_JD9851(0xC1);
LCD_DataWrite_JD9851(0x16);

LCD_CtrlWrite_JD9851(0xC3);
LCD_DataWrite_JD9851(0x08);
LCD_DataWrite_JD9851(0x00);
LCD_DataWrite_JD9851(0x0A);
LCD_DataWrite_JD9851(0x10);
LCD_DataWrite_JD9851(0x08);
LCD_DataWrite_JD9851(0x54);
LCD_DataWrite_JD9851(0x45);
LCD_DataWrite_JD9851(0x71);
LCD_DataWrite_JD9851(0x2C);

LCD_CtrlWrite_JD9851(0xC4);
LCD_DataWrite_JD9851(0x00);
LCD_DataWrite_JD9851(0xA0);
LCD_DataWrite_JD9851(0x79);
LCD_DataWrite_JD9851(0x0E);
LCD_DataWrite_JD9851(0x0A);
LCD_DataWrite_JD9851(0x16);
LCD_DataWrite_JD9851(0x79);
LCD_DataWrite_JD9851(0x0E);
LCD_DataWrite_JD9851(0x0A);
LCD_DataWrite_JD9851(0x16);
LCD_DataWrite_JD9851(0x79);
LCD_DataWrite_JD9851(0x0E);
LCD_DataWrite_JD9851(0x0A);
LCD_DataWrite_JD9851(0x16);
LCD_DataWrite_JD9851(0x82);
LCD_DataWrite_JD9851(0x00);
LCD_DataWrite_JD9851(0x03);


LCD_CtrlWrite_JD9851(0xD0);
LCD_DataWrite_JD9851(0x04);
LCD_DataWrite_JD9851(0x0C);
LCD_DataWrite_JD9851(0x6B);
LCD_DataWrite_JD9851(0x0F);
LCD_DataWrite_JD9851(0x01);
LCD_DataWrite_JD9851(0x03);

LCD_CtrlWrite_JD9851(0xD7);
LCD_DataWrite_JD9851(0x00);
LCD_DataWrite_JD9851(0x00);

LCD_CtrlWrite_JD9851(0xDE);
LCD_DataWrite_JD9851(0x02);

LCD_CtrlWrite_JD9851(0xB8);
LCD_DataWrite_JD9851(0x1D);
LCD_DataWrite_JD9851(0xA0);
LCD_DataWrite_JD9851(0x2F);
LCD_DataWrite_JD9851(0x04);
LCD_DataWrite_JD9851(0x24);

LCD_CtrlWrite_JD9851(0xB2);
LCD_DataWrite_JD9851(0x30);
LCD_DataWrite_JD9851(0xF8);
LCD_DataWrite_JD9851(0x93);
LCD_DataWrite_JD9851(0x6E);

LCD_CtrlWrite_JD9851(0xC1);
LCD_DataWrite_JD9851(0x10);
LCD_DataWrite_JD9851(0x66);
LCD_DataWrite_JD9851(0x66);
LCD_DataWrite_JD9851(0x01);

//---------------- PAGE0 --------------
 LCD_CtrlWrite_JD9851(0xDE);  	
LCD_DataWrite_JD9851(0x00);


LCD_CtrlWrite_JD9851(0x11);  	// SLPOUT
hal_LcdDelay(120);


LCD_CtrlWrite_JD9851(0xDE);
LCD_DataWrite_JD9851(0x02);

LCD_CtrlWrite_JD9851(0xC4);
LCD_DataWrite_JD9851(0x76);
LCD_DataWrite_JD9851(0x0E);

LCD_CtrlWrite_JD9851(0xC5);
LCD_DataWrite_JD9851(0x01); 	//10MHz
LCD_DataWrite_JD9851(0x00);
LCD_DataWrite_JD9851(0x00);


LCD_CtrlWrite_JD9851(0xCA);
LCD_DataWrite_JD9851(0x10);
LCD_DataWrite_JD9851(0x20);
LCD_DataWrite_JD9851(0xF4);

LCD_CtrlWrite_JD9851(0xDE);
LCD_DataWrite_JD9851(0x00);

LCD_CtrlWrite_JD9851(0x3A);
LCD_DataWrite_JD9851(0x05); 	//rgb 666 bit

LCD_CtrlWrite_JD9851(0x35);
LCD_DataWrite_JD9851(0x00); 

LCD_CtrlWrite_JD9851(0x36);
LCD_DataWrite_JD9851(0x00); 

LCD_CtrlWrite_JD9851(0x21);

LCD_CtrlWrite_JD9851(0x2A);
LCD_DataWrite_JD9851(0x00);
LCD_DataWrite_JD9851(0x00);
LCD_DataWrite_JD9851(0x00);
LCD_DataWrite_JD9851(0xEF);

LCD_CtrlWrite_JD9851(0x2B);
LCD_DataWrite_JD9851(0x00);
LCD_DataWrite_JD9851(0x00);
LCD_DataWrite_JD9851(0x01);
LCD_DataWrite_JD9851(0x3F);

LCD_CtrlWrite_JD9851(0x29);  	// SLPOUT
LCD_CtrlWrite_JD9851(0x2C);
#endif


}
#endif

	

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E  JD9851_EnterSleep(
    BOOLEAN is_sleep //SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
)
{
    SCI_TraceLow("lcd: JD9851_EnterSleep, is_sleep = %d", is_sleep);
    gouda_event(0x90001104);

    if(is_sleep)
    {

		
        LCD_CtrlWrite_JD9851(0x28); //display off
        hal_LcdDelay(120);
        LCD_CtrlWrite_JD9851(0x10); // enter sleep mode
       /// hal_LcdDelay(150);
       // _GoudaClkDisable();
       // LDO_TurnOffLDO(LDO_LDO_LCD);

    }
    else
    {
	//JD9851_ReadID();
	
        gouda_event(0x9f0f2ddf);
	hal_GoudatResetLcdPin();
	
#if 0
        LCD_CtrlWrite_JD9851(0x11); // sleep out mode
        hal_LcdDelay(120);
        LCD_CtrlWrite_JD9851(0x29); // display on
#endif
        JD9851_Init();

    }

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL void JD9851_Close(void)
{
    SCI_TraceLow("lcd:  in GC9304_Close");
    gouda_event(0x90001105);

    JD9851_EnterSleep( SCI_TRUE );
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E JD9851_Invalidate(void)
{
    gouda_event(0x90001106);

    JD9851_set_display_window (0x0, 0x0, LCD_WIDTH - 1, LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E JD9851_InvalidateRect(
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

    JD9851_set_display_window(left, top, right, bottom);
    return ERR_LCD_NONE;
}

LOCAL ERR_LCD_E JD9851_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
    gouda_event(0x90001108);

    SCI_TraceLow("lcd:  JD9851_RotationInvalidateRect");
    switch(angle)
    {
        case LCD_ANGLE_0:
            JD9851_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_90:
            JD9851_set_display_window(left, top, bottom,right);
            break;
        case LCD_ANGLE_180:
            JD9851_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_270:
            JD9851_set_display_window(left, top, bottom,right);
            break;
        default:
            JD9851_set_display_window(left, top, right, bottom);
            break;
    }
    return ERR_LCD_NONE;
}

LOCAL ERR_LCD_E   JD9851_SetContrast(uint16  contrast)
{
    
///	JD_CheckDisplayStatus();

    SCI_TraceLow("lcd: JD9851_SetContrast and check esd status");
    return ERR_LCD_FUNC_NOT_SUPPORT;
}
/*****************************************************************************/
LOCAL ERR_LCD_E   JD9851_SetBrightness(uint16 brightness)
{
    SCI_TraceLow("lcd:  JD9851_SetBrightness");
    gouda_event(0x9000110a);

    return ERR_LCD_FUNC_NOT_SUPPORT;
}

//  Description:    Enable lcd to partial display mode, so can save power.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//  Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E JD9851_SetDisplayWindow(
    uint16 left, //left of the window
    uint16 top, //top of the window
    uint16 right, //right of the window
    uint16 bottom //bottom of the window
)
{
    gouda_event(0x9000110b);

    SCI_TraceLow("lcd:  JD9851_SetDisplayWindow");
    JD9851_set_display_window(left, top, right, bottom);
    return ERR_LCD_NONE;
}

LOCAL void JD9851_InvalidatePixel(uint16 x, uint16 y, uint32 data)
{
    SCI_TraceLow("lcd:  JD9851_InvalidatePixel");
    gouda_event(0x9000110c);

    JD9851_InvalidateRect(x,y,x,y);
    LCD_DataWrite_JD9851(data);
}

LOCAL void JD9851_GetConfig(void)
{
    uint32 count,k;
    LCM_CFG_INFO_T_PTR cfg_ptr = PNULL;
    cfg_ptr = LCM_GetCfgInfo(0,&count);
    for(k=0; k<count; k++)
    {
        if(cfg_ptr[k].dev_id==LCD_DRV_ID_JD9851)
        {
            JD9851_line_mode = cfg_ptr[k].adc_min;
            JD9851_farmk = cfg_ptr[k].adc_max;

            break;
        }
    }
    SCI_TraceLow("lcd: JD9851_GetConfig: JD9851_line_mode = %d , JD9851_farmk =%d\n",JD9851_line_mode,JD9851_farmk);  //0x80,0xf5,0x3e
}

#if 0
#define _TE_DELAY_3MS      0x92aaa 
#define _TE_DELAY_1p5MS    0x58000 

LOCAL uint32 JD9851_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint8 id[4] = {0};
	
	uint32 temp=0;
    JD9851_GetConfig();

    hal_gouda_sim_init(JD9851_line_mode,JD9851_farmk,_TE_DELAY_3MS);
    mon_Event(0xadff9307);

    hal_LcdDelay(140);
    hwp_analogReg->pad_spi_lcd_cfg2 |= ANALOG_REG_PAD_SPI_LCD_SIO_SPU;
    hal_LcdDelay(10);
    hal_GoudaReadData(0x04,id,4);
    hwp_analogReg->pad_spi_lcd_cfg2 =  hwp_analogReg->pad_spi_lcd_cfg2&(~ANALOG_REG_PAD_SPI_LCD_SIO_SPU);
    hal_LcdDelay(10);
    SCI_TraceLow("lcd: JD9851_ReadID: id1->0x%0x, id2->0x%x, id3->0x%0x\n",id[3], id[2], id[1]);

    mon_Event((id[3]<<16)|(id[2] <<8)| id[1]);
	jdidlcd = ((id[3]<<16)|((id[2] <<8)| id[1]));
	if((jdidlcd == LCD_DRV_ID_JD9851)||(jdidlcd == LCD_DRV_ID_JD9851_2))
		{
			temp = LCD_DRV_ID_JD9851;
			
		}
    hal_gouda_term();

    return temp;
}
#endif

#define _TE_DELAY_3MS      0x92aaa 
#define _TE_DELAY_1p5MS    0x58000 
uint32 zdt_lcd_id = 0;
uint8 zdt_id[4] = {0};
LOCAL uint32 JD9851_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint8 id[4] = {0};
 
 uint32 temp=0;
    JD9851_GetConfig();

    hal_gouda_sim_init(JD9851_line_mode,JD9851_farmk,_TE_DELAY_3MS);
    mon_Event(0xadff9307);

    hal_LcdDelay(140);
    hwp_analogReg->pad_spi_lcd_cfg2 |= ANALOG_REG_PAD_SPI_LCD_SIO_SPU;
    hal_LcdDelay(10);
    hal_GoudaReadData(0x04,id,4);
    hwp_analogReg->pad_spi_lcd_cfg2 =  hwp_analogReg->pad_spi_lcd_cfg2&(~ANALOG_REG_PAD_SPI_LCD_SIO_SPU);
    hal_LcdDelay(10);
    //SCI_TraceLow("lcd: JD9851_ReadID: id1->0x%0x, id2->0x%x, id3->0x%0x\n",id[3], id[2], id[1]);

    mon_Event((id[3]<<16)|(id[2] <<8)| id[1]);
 jdidlcd = ((id[3]<<16)|((id[2] <<8)| id[1]));
 zdt_lcd_id = jdidlcd;
 if((jdidlcd == LCD_DRV_ID_JD9851)||(jdidlcd == LCD_DRV_ID_JD9851_2)||(jdidlcd == LCD_DRV_ID_JD9851_3))
  {
   temp = LCD_DRV_ID_JD9851;
   
  }
    hal_gouda_term();

    return temp;
}

const LCD_OPERATIONS_T JD9851_operations =
{
    JD9851_Init,
    JD9851_EnterSleep,
    JD9851_SetContrast,
    JD9851_SetBrightness,
    JD9851_SetDisplayWindow,
    JD9851_InvalidateRect,
    JD9851_Invalidate,
    JD9851_Close,
    JD9851_RotationInvalidateRect,
    JD9851_SetDirection,
    NULL,
    JD9851_ReadID
};

LOCAL const LCD_TIMING_U s_JD9851_lcmtiming =
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

LOCAL const LCD_TIMING_U s_JD9851_spitiming =
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
const LCD_SPEC_T g_lcd_JD9851 =
{
    LCD_WIDTH,
    LCD_HEIGHT,
    SCI_NULL,
    SCI_NULL,
    WIDTH_8,
    (LCD_TIMING_U*)&s_JD9851_spitiming,
    (LCD_OPERATIONS_T*)&JD9851_operations,
    1,
    0
};
#else // not verify lcm
const LCD_SPEC_T g_lcd_JD9851 =
{
    LCD_WIDTH,
    LCD_HEIGHT,
    LCD_MCU,
    BUS_MODE_8080,
    WIDTH_8,
    (LCD_TIMING_U*)&s_JD9851_lcmtiming,
    (LCD_OPERATIONS_T*)&JD9851_operations,
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


