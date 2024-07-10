/******************************************************************************
 ** File Name:                                                *
 ** Description:                                                              *
 **    This file contains driver for color LCD.(CSTN)                         *
 ** Author:                                                                         *
 ** DATE:           04/26/2010                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 02/04/2004     louis.wei        Create.
 ******************************************************************************/

#include "ms_customize_trc.h"
#include "os_api.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"


#define  L_YG

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define ILI9342_LCD_WIDTH   320 //LCD_WIDTH  
#define ILI9342_LCD_HEIGHT   240 //LCD_HEIGHT

LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;

extern uint32 LCM_Read (uint32 lcd_id, uint32 cd_level);
#define BUS_WIDTH_ILI9342    g_lcd_ILI9342.bus_width

#define LCD_USED_INDEX   MAIN_LCD_ID

LOCAL void LCD_CtrlWrite_ILI9342( uint32 _cmd )
{    
    LCD_SendCmd(_cmd , MAIN_LCD_ID);  
}

LOCAL void LCD_DataWrite_ILI9342( uint32 _data )
{
    LCD_SendData(_data, MAIN_LCD_ID );
}

LOCAL void LCD_CtrlData_ILI9342(uint32 _cmd, uint32 _data)
{
    LCD_CtrlWrite_ILI9342( _cmd );
    LCD_DataWrite_ILI9342( _data );
}


/******************************************************************************/
//  Description:   Read ILI9225 device ID
//  Global resource dependence: 
//  Author:         
//  Note:
/******************************************************************************/
LOCAL uint32 LCD_DataRead_ILI9342( ) 
{
    return (LCM_Read(MAIN_LCD_ID, DATA_LEVEL));
}

/******************************************************************************/
//  Description:   Clear color LCD with one color
//  Global resource dependence: 
//  Author:         louis.wei
//  Note:
/******************************************************************************/
LOCAL void ILI9342_Clear(
                        uint32 color    //color to fill the whole lcd.
                        );
  /******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL void ILI9342_Close(void);
  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9342_EnterSleep(
    BOOLEAN is_sleep    //SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
    );

/*********************************************************************/
//  Description:   Initialize color LCD : ILI9342
//  Input:
//      None.
//  Return:
//      None.
//  Note:           
/*********************************************************************/
LOCAL ERR_LCD_E ILI9342_Init(void);
/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9342_Invalidate(void);
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
LOCAL ERR_LCD_E ILI9342_InvalidateRect(
    uint16 left,    //the left value of the rectangel
    uint16 top,     //top of the rectangle
    uint16 right,   //right of the rectangle
    uint16 bottom   //bottom of the rectangle
    );
/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
PUBLIC void ILI9342_set_display_window(
    uint16 left,    // start Horizon address
    uint16 right,   // end Horizon address
    uint16 top,         // start Vertical address
    uint16 bottom   // end Vertical address
    );
/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/
/******************************************************************************/
//  Description:  Set LCD invalidate direction
//  Input:
//      is_invert: 0, horizontal; 1, vertical
//  Return:
//      None.
//  Note: Application should correct invalidate direction right after current
//        image displayed
/******************************************************************************/
LOCAL ERR_LCD_E ILI9342_SetDirection( LCD_DIRECT_E direct_type)
{
    switch(direct_type)
    {
        case LCD_DIRECT_NORMAL:
            LCD_CtrlData_ILI9342(0x0036, 0x00);
            break;
            
        case LCD_DIRECT_ROT_90:
            LCD_CtrlData_ILI9342(0x0036, 0x60);//c8
            break;
            
        case LCD_DIRECT_ROT_180:            
            LCD_CtrlData_ILI9342(0x0036, 0xc0);
            break;
            
        case LCD_DIRECT_ROT_270:            
            break;
            
        case LCD_DIRECT_MIR_H:  
            LCD_CtrlData_ILI9342(0x0036, 0xE0);
            break;
            
        case LCD_DIRECT_MIR_V:
            LCD_CtrlData_ILI9342(0x0036, 0x20); 
            break;
            
        case LCD_DIRECT_MIR_HV:        
            break;  
            
        default:            
            LCD_CtrlData_ILI9342(0x0036, 0xc0);
                direct_type = LCD_DIRECT_NORMAL;
            break;
    }   
    //SCI_TRACE_LOW:"Direction LCD: ILI9342 Direction=%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9342_177_112_2_18_0_33_53_1815,(uint8*)"d",direct_type);
    s_lcd_direct = direct_type; 

    return ERR_LCD_NONE;
}
/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
__inline void ILI9342_set_display_window(
    uint16 left,// start Horizon address
    uint16 top ,    // end Vertical address
    uint16 right, // start Vertical address
    uint16 bottom       // end Horizon address

    )
{   
    //SCI_TRACE_LOW:"ILI9342 window:left=%x,right=%x,top=%x,bottom=%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9342_195_112_2_18_0_33_53_1816,(uint8*)"dddd",left,right,top,bottom);

    switch(s_lcd_direct)
    {
        case LCD_DIRECT_NORMAL:
            LCD_CtrlWrite_ILI9342(0x2a);
            LCD_DataWrite_ILI9342((left>>8)&0xff);      
            LCD_DataWrite_ILI9342(left&0x00ff);         
            LCD_DataWrite_ILI9342((right>>8)&0xff);
            LCD_DataWrite_ILI9342(right&0x00ff);            

            LCD_CtrlWrite_ILI9342(0x2b);
            LCD_DataWrite_ILI9342(0x00);
            LCD_DataWrite_ILI9342(top&0x00ff);
            LCD_DataWrite_ILI9342(0x00);
            LCD_DataWrite_ILI9342(bottom&0x00ff); 
            break;

        case LCD_DIRECT_ROT_90:
            break;

        case LCD_DIRECT_ROT_180:
            LCD_CtrlWrite_ILI9342(0x2a);
            LCD_DataWrite_ILI9342((left>>8)&0xff);      
            LCD_DataWrite_ILI9342(left&0x00ff);         
            LCD_DataWrite_ILI9342((right>>8)&0xff);
            LCD_DataWrite_ILI9342(right&0x00ff);            

            LCD_CtrlWrite_ILI9342(0x2b);
            LCD_DataWrite_ILI9342(0x00);
            LCD_DataWrite_ILI9342(top&0x00ff);
            LCD_DataWrite_ILI9342(0x00);
            LCD_DataWrite_ILI9342(bottom&0x00ff);           
            break;

        case LCD_DIRECT_ROT_270:
            break;

        case LCD_DIRECT_MIR_H:
            break;

        case LCD_DIRECT_MIR_V:
            break;

        case LCD_DIRECT_MIR_HV:
            break;

        default:
            LCD_CtrlWrite_ILI9342(0x2a);
            LCD_DataWrite_ILI9342((left>>8)&0xff);      
            LCD_DataWrite_ILI9342(left&0x00ff);         
            LCD_DataWrite_ILI9342((right>>8)&0xff);
            LCD_DataWrite_ILI9342(right&0x00ff);            

            LCD_CtrlWrite_ILI9342(0x2b);
            LCD_DataWrite_ILI9342(0x00);
            LCD_DataWrite_ILI9342(top&0x00ff);
            LCD_DataWrite_ILI9342(0x00);
            LCD_DataWrite_ILI9342(bottom&0x00ff); 
            break;
    }
    
    LCD_CtrlWrite_ILI9342(0x2C);    
}

LOCAL void ILI9342_driver(void)
{
    //SCI_TRACE_LOW:" ILI9342_driver"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9342_262_112_2_18_0_33_53_1817,(uint8*)"");
    LCD_CtrlWrite_ILI9342(0xB9);  // Set EXTC
    LCD_DataWrite_ILI9342 (0xFF);
    LCD_DataWrite_ILI9342 (0x93);
    LCD_DataWrite_ILI9342 (0x42);

    LCD_CtrlWrite_ILI9342(0x36);  // Memory Access Control

    //LCD_DataWrite_ILI9342 (0x08);// MY,MX,MV,ML,BGR,MH bit
    LCD_DataWrite_ILI9342 (0x00);// MY,MX,MV,ML,BGR,MH bit

    LCD_CtrlWrite_ILI9342(0x3A);  // Pixel Format Set
    LCD_DataWrite_ILI9342 (0x55);// DPI[2:0]&DBI[2:0]=16 bits / pixel

    LCD_CtrlWrite_ILI9342(0xB1);  // Display Waveform Cycle
    LCD_DataWrite_ILI9342 (0x00);// DIVA[1:0],division ratio£¬0x00=Fosc
    LCD_DataWrite_ILI9342 (0x10);// RTNA[4:0]£¬Clock per Line£¬0x10=16 clocks

    LCD_CtrlWrite_ILI9342(0xB6);  // Display Function Control
    LCD_DataWrite_ILI9342 (0x0A);// PTG[1:0],PT[1:0] bit
    LCD_DataWrite_ILI9342 (0xE2);// REV,GS,SS,SM,ISC[3:0] bit
    LCD_DataWrite_ILI9342 (0x1D);// NL[5:0],0x1D=240 lines

    LCD_CtrlWrite_ILI9342(0xC0);  // Power Control 1
    LCD_DataWrite_ILI9342 (0x28);// Set the VREG1OUT level,VRH[5:0],0x28=4.85V
    LCD_DataWrite_ILI9342 (0x0A);// Sets VCI1 level,VC[3:0],0x2A=2.80V

    LCD_CtrlWrite_ILI9342(0xC1);  // Power Control 2
    LCD_DataWrite_ILI9342 (0x02);// SAP[2:0], BT[3:0] VGH/VGL=6/-3

    LCD_CtrlWrite_ILI9342(0xC5);  // VCOM Control 1

    LCD_DataWrite_ILI9342 (0x31);// VMH[6:0],0x31=3.925V

    LCD_DataWrite_ILI9342 (0x3C);// VML[6:0],0x3C=-1.000V

    LCD_CtrlWrite_ILI9342(0xB8);  // Oscillator Control
    LCD_DataWrite_ILI9342 (0x0A);// FOSC[3:0],0x0A=93Hz

    LCD_CtrlWrite_ILI9342(0x26);  // Gamma Set
    LCD_DataWrite_ILI9342 (0x01);// GC[7:0],Gamma curve 2.2

    LCD_CtrlWrite_ILI9342(0xC7);  // VCOM Control 2
    LCD_DataWrite_ILI9342 (0xBF);// VMF[6:0],VCOM offset voltage,0xBF=VMH+31,VML+31

    LCD_CtrlWrite_ILI9342(0xE0);  // Positive Gamma Correction
    LCD_DataWrite_ILI9342 (0x08);
    LCD_DataWrite_ILI9342 (0x1C);
    LCD_DataWrite_ILI9342 (0x1B);
    LCD_DataWrite_ILI9342 (0x09);
    LCD_DataWrite_ILI9342 (0x0D);
    LCD_DataWrite_ILI9342 (0x08);
    LCD_DataWrite_ILI9342 (0x4B);
    LCD_DataWrite_ILI9342 (0xB8);
    LCD_DataWrite_ILI9342 (0x3B);
    LCD_DataWrite_ILI9342 (0x08);
    LCD_DataWrite_ILI9342 (0x10);
    LCD_DataWrite_ILI9342 (0x08);
    LCD_DataWrite_ILI9342 (0x20);
    LCD_DataWrite_ILI9342 (0x20);
    LCD_DataWrite_ILI9342 (0x08);

    LCD_CtrlWrite_ILI9342(0xE1);  // Negative Gamma Correction
    LCD_DataWrite_ILI9342 (0x07);
    LCD_DataWrite_ILI9342 (0x23);
    LCD_DataWrite_ILI9342 (0x24);
    LCD_DataWrite_ILI9342 (0x06);
    LCD_DataWrite_ILI9342 (0x12);
    LCD_DataWrite_ILI9342 (0x07);
    LCD_DataWrite_ILI9342 (0x34);
    LCD_DataWrite_ILI9342 (0x47);
    LCD_DataWrite_ILI9342 (0x44);
    LCD_DataWrite_ILI9342 (0x07);
    LCD_DataWrite_ILI9342 (0x0F);
    LCD_DataWrite_ILI9342 (0x07);
    LCD_DataWrite_ILI9342 (0x1F);
    LCD_DataWrite_ILI9342 (0x1F);
    LCD_DataWrite_ILI9342 (0x07);

    LCD_CtrlWrite_ILI9342(0x11);  // Exit Sleep
    LCD_Delayms(80);
    LCD_CtrlWrite_ILI9342(0x11);  // Exit Sleep
    LCD_Delayms(80);
    LCD_CtrlWrite_ILI9342(0x29);  // Display On

    LCD_CtrlWrite_ILI9342(0x2C);
    

    //SCI_TRACE_LOW:" ILI9342_driver end "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9342_350_112_2_18_0_33_53_1818,(uint8*)"");
}
/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E ILI9342_Init(void)
{
    //SCI_TRACE_LOW:"ILI9342_Init"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9342_358_112_2_18_0_33_53_1819,(uint8*)"");
 
    ILI9342_driver();   
    
    return 0;
}
/******************************************************************************/
//  Description:   Clear color LCD with one color
//  Global resource dependence: 
//  Author:         louis.wei
//  Note:
/******************************************************************************/
LOCAL void ILI9342_Clear(
                        uint32 color    //color to fill the whole lcd.
                        )
{
    ILI9342_Invalidate();

    //SCI_TRACE_LOW:"qinss LCD: in ILI9342_Clear end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9342_374_112_2_18_0_33_53_1820,(uint8*)"");

    return;
}
/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9342_EnterSleep(
    BOOLEAN is_sleep    //SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
    )
{
    //SCI_TRACE_LOW:"qinss LCD: in ILI9342_EnterSleep, is_sleep = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9342_386_112_2_18_0_33_53_1821,(uint8*)"d", is_sleep);
    
    if ( is_sleep ) // enter sleep mode.
    {
    LCD_CtrlWrite_ILI9342(0x28);
    LCD_CtrlWrite_ILI9342(0x10);
    LCD_Delayms(30);
    }
    else            // out sleep mode 
    {
    LCD_CtrlWrite_ILI9342(0x11);
    LCD_CtrlWrite_ILI9342(0x29);
    LCD_Delayms(200);
    ILI9342_Init();
    }

    return ERR_LCD_NONE;
}
/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL void ILI9342_Close(void)
{
    //SCI_TRACE_LOW:"qinss LCD: in ILI9342_Close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9342_410_112_2_18_0_33_53_1822,(uint8*)"");

    //GPIO_SetLcdBackLight( SCI_FALSE );
    ILI9342_EnterSleep( SCI_TRUE ); 
}
/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9342_Invalidate(void)
{
    ILI9342_set_display_window (0x0, 0X0, 320-1, 240-1);//ILI9342_LCD_WIDTH - 1, ILI9342_LCD_HEIGHT -1);

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
PUBLIC ERR_LCD_E ILI9342_InvalidateRect(
    uint16 left,    //the left value of the rectangel
    uint16 top,     //top of the rectangle
    uint16 right,   //right of the rectangle
    uint16 bottom   //bottom of the rectangle
    )
{
    left    = (left >= ILI9342_LCD_WIDTH)    ? ILI9342_LCD_WIDTH-1 : left;
    right   = (right >= ILI9342_LCD_WIDTH)   ? ILI9342_LCD_WIDTH-1 : right;
    top     = (top >= ILI9342_LCD_HEIGHT)    ? ILI9342_LCD_HEIGHT-1 : top;
    bottom  = (bottom >= ILI9342_LCD_HEIGHT) ? ILI9342_LCD_HEIGHT-1 : bottom;

    if ( ( right < left ) || ( bottom < top ) )
    {
        return ERR_LCD_OPERATE_FAIL;
    }     

    ILI9342_set_display_window(left, top, right, bottom); 

    return ERR_LCD_NONE;
}
/**************************************************************************************/
// Description: refresh a rectangle of lcd
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      left - the left value of the rectangel
//      top - the top value of the rectangel
//      right - the right value of the rectangel
//      bottom - the bottom value of the rectangel
/**************************************************************************************/
LOCAL ERR_LCD_E ILI9342_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
    switch(angle)
    {
        case LCD_ANGLE_0:
            ILI9342_set_display_window(left,  top,right,bottom);
            break;
        case LCD_ANGLE_90:
            ILI9342_set_display_window(left, top, bottom,right);
            break;
        case LCD_ANGLE_180:
            ILI9342_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_270:
            ILI9342_set_display_window(left, top, bottom,right);
            break;
        default:
            ILI9342_set_display_window(left, top, right, bottom);
            break;          
    }

    return ERR_LCD_NONE;
}//en of ILI9342_VerticalInvalidateRect 
/******************************************************************************/
//  Description:  set the contrast value 
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E   ILI9342_SetContrast(
    uint16  contrast    //contrast value to set
    )
{
    return ERR_LCD_FUNC_NOT_SUPPORT;
} 
/*****************************************************************************/
//  Description:    Set the brightness of LCD.
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/*****************************************************************************/
#ifdef LCD_DATA_WIDTH_8BIT
LOCAL void  ILI9342_InvalidatePixel(uint16 x, uint16 y, uint32 data)
{
    ILI9342_InvalidateRect(x,y,x,y);
    LCD_DataWrite_ILI9342(data);
}
#endif

LOCAL ERR_LCD_E   ILI9342_SetBrightness(
    uint16 brightness   //birghtness to set
    )
{
    return ERR_LCD_FUNC_NOT_SUPPORT;
}
/*****************************************************************************/
//  Description:    Enable lcd to partial display mode, so can save power.
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//  Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E ILI9342_SetDisplayWindow(
    uint16 left,        //left of the window
    uint16 top,         //top of the window
    uint16 right,       //right of the window
    uint16 bottom       //bottom of the window
    )
{
    ILI9342_set_display_window(left, top, right, bottom);

    return ERR_LCD_NONE;
}

/******************************************************************************/
// Description: Read lcm id
// Global resource dependence: 
// Author: 
// Note:  CMD: R(0xD3)
//        DATA:DUMMY(Byte)+DUMMY(Byte)+0x93+0x42
/******************************************************************************/
LOCAL uint32 ILI9342_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint32 lcm_id0=0,lcm_id1=0,dummy=0;
    
    LCD_CtrlWrite_ILI9342(0xD3);
    
    {
        // Get:XX XX 93 42
        dummy = LCD_DataRead_ILI9342() & 0xFF;   
        dummy = LCD_DataRead_ILI9342() & 0xFF;   
        lcm_id0 = LCD_DataRead_ILI9342() & 0xFF; 
        lcm_id1 = LCD_DataRead_ILI9342() & 0xFF;   

        return ((lcm_id0<<8)|(lcm_id1));
    }

}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
const LCD_OPERATIONS_T ILI9342_operations = 
{
    ILI9342_Init,
    ILI9342_EnterSleep,
    ILI9342_SetContrast,
    ILI9342_SetBrightness,
    ILI9342_SetDisplayWindow,
    
    ILI9342_InvalidateRect,
    ILI9342_Invalidate,
    ILI9342_Close,
    ILI9342_RotationInvalidateRect,
    ILI9342_SetDirection,
    
    NULL,
    ILI9342_ReadID
};
/******************************************************************************/
//  Description:  return the ILI9342 lcd driver funtion pointer
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9342_GetOperations(void)
{
    //SCI_TRACE_LOW:"qinss LCD: in ILI9342_GetOperations"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9342_581_112_2_18_0_33_54_1823,(uint8*)"");

    return (LCD_OPERATIONS_T*)&ILI9342_operations;
}

PUBLIC BOOLEAN ILI9342_Probe(void)
{
    return SCI_TRUE;    
}

LOCAL const LCD_TIMING_U s_ILI9342_timing =
{
            // LCM_CYCLE_U start(ns)
    15,     // CS setup time for LCM read (optional)  
    120,    // low pulse width for LCM read (according spec)    
    75, // high pulse width for LCM read (according spec)
    15,     // CS setup time for LCM write  (optional)  
    35,//40,        // low pulse width for LCM write (according spec)
    35,//25,        // high pulse width for LCM write (according spec)  
            // LCM_CYCLE_U end(ns)
};

const LCD_SPEC_T g_lcd_ILI9342 =
{
    ILI9342_LCD_WIDTH, 
    ILI9342_LCD_HEIGHT, 
    LCD_MCU, 
    BUS_MODE_8080,
    #ifdef LCD_DATA_WIDTH_8BIT
    WIDTH_8,
    #else
    WIDTH_16,
    #endif
    (LCD_TIMING_U*)&s_ILI9342_timing, 
    (LCD_OPERATIONS_T*)&ILI9342_operations,
    0,
    0
};
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
