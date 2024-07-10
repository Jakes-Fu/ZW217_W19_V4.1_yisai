/******************************************************************************
 ** File Name:     TFT_HX8352C.c                                               *
 ** Description:                                                              *
 **    This file contains driver for color LCD which controler IC is HX8352C   *
 ** Author:         Huitao Yue                                                *
 ** DATE:           11/23/2011                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 **   DATE           NAME                      DESCRIPTION                    *
 ** 11/23/2011     Huitao Yue                     Create.                     *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "os_api.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                            Macro Define                                   *
 **---------------------------------------------------------------------------*/
#define LCD_Delay( ticks)\
                OS_TickDelay( ticks );

extern uint32 LCM_Read (uint32 lcd_id, uint32 cd_level);

LOCAL void LCD_CtrlWrite_HX8352C( uint32 _cmd )
{
    LCD_SendCmd( (_cmd ), 0 );
}

LOCAL void LCD_DataWrite_HX8352C( uint32 _data )
{
    LCD_SendData( (_data), 0 );
}

LOCAL void LCD_CtrlData_HX8352C( uint32 _cmd, uint32 _data )
{
    LCD_SendCmdData( (_cmd ),  (_data),0 );  
}

LOCAL uint32 LCD_DataRead_HX8352C(uint32 cs, uint32 cd_level)
{
    uint32 lcm_id=0;
    lcm_id = LCM_Read(cs, cd_level);
    return lcm_id;
}

/**---------------------------------------------------------------------------*
 **                         Local Function Prototype                          *
 **---------------------------------------------------------------------------*/
LOCAL ERR_LCD_E HX8352C_Init(void);
LOCAL ERR_LCD_E HX8352C_EnterSleep(BOOLEAN is_sleep);
LOCAL ERR_LCD_E HX8352C_SetContrast(uint16 contrast);
LOCAL ERR_LCD_E HX8352C_SetBrightness(uint16 brightness);
LOCAL ERR_LCD_E HX8352C_SetDisplayWindow(uint16 left, uint16 top, uint16 right, 
    uint16 bottom);
LOCAL ERR_LCD_E HX8352C_InvalidateRect(uint16 left, uint16 top, uint16 right, 
    uint16 bottom);
LOCAL ERR_LCD_E HX8352C_Invalidate(void);
LOCAL void HX8352C_Close(void);
LOCAL ERR_LCD_E HX8352C_RotationInvalidateRect(uint16 left,uint16 top,
    uint16 right,uint16 bottom,LCD_ANGLE_E angle);
LOCAL ERR_LCD_E HX8352C_SetDirection(LCD_DIRECT_E direct_type);
LOCAL uint32 HX8352C_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id);
/**---------------------------------------------------------------------------*
 **                         Data & Function Prototype                         *
 **---------------------------------------------------------------------------*/
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;
LOCAL BOOLEAN      s_is_invert  = SCI_FALSE;
//OPERATIOS TABLE
LOCAL const LCD_OPERATIONS_T s_HX8352C_ops_tab = 
{
    HX8352C_Init,
    HX8352C_EnterSleep,
    HX8352C_SetContrast,
    HX8352C_SetBrightness,
    HX8352C_SetDisplayWindow,
    
    HX8352C_InvalidateRect,
    HX8352C_Invalidate,
    HX8352C_Close,
    HX8352C_RotationInvalidateRect,
    HX8352C_SetDirection,
    
    NULL,
    HX8352C_ReadID
};

//TIME TABLE
LOCAL const LCD_TIMING_U s_HX8352C_timing =
{

            // LCM_CYCLE_U start(ns)
    360,    // CS setup time for LCM read (optional)  
    360,    // high pulse width for LCM read (according spec)
    90,     // low pulse width for LCM read (according spec)    
    15,     // CS setup time for LCM write  (optional)  
    20,     // low pulse width for LCM write (according spec)
    20,     // high pulse width for LCM write (according spec)  
            // LCM_CYCLE_U end(ns)
};

//LCD INFO
PUBLIC const LCD_SPEC_T g_lcd_HX8352C = 
{
    LCD_WIDTH, 
    LCD_HEIGHT, 
    LCD_MCU, 
    BUS_MODE_8080,
    WIDTH_8,
    (LCD_TIMING_U*)&s_HX8352C_timing, 
    (LCD_OPERATIONS_T*)&s_HX8352C_ops_tab,
    0,
    0
};

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Huitao Yue
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E HX8352C_Init(void)
{
    static const uint16 s_hx8352C_init[][2] = 
    {
#if 0
        //Power Voltage Setting
        {0x1A,0x02}, //BT
        {0x1B,0x8B}, //VRH
        
        //****VCOM offset**///
        {0x23,0x00}, //SEL_VCM
        {0x24,0x7f}, //VCM
        {0x25,0x1B}, //VDV
        {0x2D,0x01}, //NOW[2:0]=001
        
        //****OPON**//
        {0xE8,0x60},
        
        //Power on Setting
        {0x18,0x04}, //Frame rate 72Hz
        {0x19,0x01}, //OSC_EN='1', start Osc
        {0x01,0x00}, //DP_STB='0', out deep sleep
        {0x1F,0x88}, //STB=0
        {0xffff,5},
        {0x1F,0x80}, //DK=0
        {0xffff,5},
        {0x1F,0x90}, //PON=1
        {0xffff,5},
        {0x1F,0xD0}, //VCOMG=1
        {0xffff,5},
        
        //262k/65k color selection
        {0x17,0x05}, //default 0x06 262k color // 0x05 65k color
        
        //SET PANEL
        {0x36,0x13},
        {0x29,0x31}, //400 lines
        {0x71,0x1A}, //RTN  
        
        //Gamma 2.2 Setting
        {0x40,0x03},
        {0x41,0x35},
        {0x42,0x33},
        {0x43,0x06},
        {0x44,0x00},
        {0x45,0x08},
        {0x46,0x44},
        {0x47,0x24},
        {0x48,0x47},
        {0x49,0x60},
        {0x4A,0x04},
        {0x4B,0x00},
        {0x4C,0x88},
        {0x4D,0x88},
        {0x4E,0x88},
#if 0 
        //Set GRAM Area
        {0x02,0x00},
        {0x03,0x00}, //Column Start
        {0x04,0x00},
        {0x05,0xEF}, //Column End
        {0x06,0x00},
        {0x07,0x00}, //Row Start
        {0x08,0x01},
        {0x09,0xAF}, //Row End
#endif    
        //Set GRAM Area
        {0x0002,0x0000},
        {0x0003,0x0000},//Column Start
        {0x0004,0x0000},
        {0x0005,0x00EF},//Column End
        {0x0006,0x0000},
        {0x0007,0x0000},//Row Start
        {0x0008,0x0001},
        {0x0009,0x00AF},//Row End
              //Display ON Setting
        {0x0028,0x0038},//GON=1,DTE=1,D=10
        {0xffff,40},
        {0x0028,0x003C}//GON=1,DTE=1,D=11
#endif
        {0x00e8, 0x00c0}, //Enhance driver strength
        {0x001A, 0x0002},//BT 
        {0x001B, 0x008B},//VRH 
         
        //****VCOM offset**/// 
        {0x0023, 0x0000},//SEL_VCM 
        {0x0024, 0x005D},//VCM 
        {0x0025, 0x0015},//VDV 
        {0x002D, 0x0001},//NOW[2:0]=001 
        
        //****OPON**// 
        {0x00E8, 0x0060}, 
         
        //Power on Setting 
        {0x0018, 0x0004},//Frame rate 72Hz 
        {0x0019, 0x0001},//OSC_EN='1');WMLCDDATA( start Os
        {0x0001, 0x0000},//DP_STB='0');WMLCDDATA( out deep
        {0x001F, 0x0088},//STB=0 
        {0xffff, 5}, 
        {0x001F, 0x0080},//DK=0 
        {0xffff, 5}, 
        {0x001F, 0x0090},//PON=1 
        {0xffff, 5}, 
        {0x001F, 0x00D0},//VCOMG=1 
        {0xffff, 5}, 
        //262k/65k color selection 
        {0x0017, 0x0005},//default 0x0006 262k color // 0x0005 65k color 
         
        //SET PANEL 
        {0x0036, 0x0013},//GS_PANEL(0)l,BGR_PANEL(0),SS_PANEL (0)
        {0x0029, 0x0031},//400 lines 
        {0x0071, 0x001A},//RTN   
         
        //Gamma 2.2 Setting 
        {0x0040, 0x0000},
        {0x0041, 0x0077},
        {0x0042, 0x0077},
        {0x0043, 0x0000},
        {0x0044, 0x0004},
        {0x0045, 0x0000},
        {0x0046, 0x0000},
        {0x0047, 0x0000},
        {0x0048, 0x0077},
        {0x0049, 0x0000},
        {0x004A, 0x0000},
        {0x004B, 0x0008},
        {0x004C, 0x0000},
        {0x004D, 0x0000},
        {0x004E, 0x0000},
        
        //Set DGC 
        {0x00FF, 0x0001},//Page1 
        {0x0000, 0x0001},//DGC_EN 
        {0x0001, 0x0000},   
        {0x0002, 0x0006},   
        {0x0003, 0x000C},   
        {0x0004, 0x0012},   
        {0x0005, 0x0016},   
        {0x0006, 0x001C},   
        {0x0007, 0x0023},   
        {0x0008, 0x002E},   
        {0x0009, 0x0036},   
        {0x000A, 0x003F},   
        {0x000B, 0x0047},   
        {0x000C, 0x0050},   
        {0x000D, 0x0057},   
        {0x000E, 0x005F},   
        {0x000F, 0x0067},   
        {0x0010, 0x006F},   
        {0x0011, 0x0076},   
        {0x0012, 0x007F},   
        {0x0013, 0x0087},   
        {0x0014, 0x0090},   
        {0x0015, 0x0099},   
        {0x0016, 0x00A3},   
        {0x0017, 0x00AD},   
        {0x0018, 0x00B4},   
        {0x0019, 0x00BB},   
        {0x001A, 0x00C4},   
        {0x001B, 0x00CE},   
        {0x001C, 0x00D9},   
        {0x001D, 0x00E3},   
        {0x001E, 0x00EC},   
        {0x001F, 0x00F3},   
        {0x0020, 0x00F7},   
        {0x0021, 0x00FC},   
        {0x0022, 0x0000},   
        {0x0023, 0x0006},   
        {0x0024, 0x000C},   
        {0x0025, 0x0012},   
        {0x0026, 0x0016},   
        {0x0027, 0x001C},   
        {0x0028, 0x0023},   
        {0x0029, 0x002E},   
        {0x002A, 0x0036},   
        {0x002B, 0x003F},   
        {0x002C, 0x0047},   
        {0x002D, 0x0050},   
        {0x002E, 0x0057},   
        {0x002F, 0x005F},   
        {0x0030, 0x0067},   
        {0x0031, 0x006F},   
        {0x0032, 0x0076},   
        {0x0033, 0x007F},   
        {0x0034, 0x0087},   
        {0x0035, 0x0090},   
        {0x0036, 0x0099},   
        {0x0037, 0x00A3},   
        {0x0038, 0x00AD},   
        {0x0039, 0x00B4},   
        {0x003A, 0x00BB},   
        {0x003B, 0x00C4},   
        {0x003C, 0x00CE},   
        {0x003D, 0x00D9},   
        {0x003E, 0x00E3},   
        {0x003F, 0x00EC},   
        {0x0040, 0x00F3},   
        {0x0041, 0x00F7},   
        {0x0042, 0x00FC},   
        {0x0043, 0x0000},   
        {0x0044, 0x0006},   
        {0x0045, 0x000C},   
        {0x0046, 0x0012},   
        {0x0047, 0x0016},   
        {0x0048, 0x001C},   
        {0x0049, 0x0023},   
        {0x004A, 0x002E},   
        {0x004B, 0x0036},   
        {0x004C, 0x003F},   
        {0x004D, 0x0047},   
        {0x004E, 0x0050},   
        {0x004F, 0x0057},   
        {0x0050, 0x005F},   
        {0x0051, 0x0067},   
        {0x0052, 0x006F},   
        {0x0053, 0x0076},   
        {0x0054, 0x007F},   
        {0x0055, 0x0087},   
        {0x0056, 0x0090},   
        {0x0057, 0x0099},   
        {0x0058, 0x00A3},   
        {0x0059, 0x00AD},   
        {0x005A, 0x00B4},   
        {0x005B, 0x00BB},   
        {0x005C, 0x00C4},   
        {0x005D, 0x00CE},   
        {0x005E, 0x00D9},   
        {0x005F, 0x00E3},   
        {0x0060, 0x00EC},   
        {0x0061, 0x00F3},   
        {0x0062, 0x00F7},   
        {0x0063, 0x00FC},   
        
        {0x00FF, 0x0000},//Page0 
        //Set GRAM Area 
        {0x0002, 0x0000},
        {0x0003, 0x0000},//Column Start 
        {0x0004, 0x0000},
        {0x0005, 0x00EF},//Column End 
         
        {0x0006, 0x0000},
        {0x0007, 0x0000},//Row Start 
        {0x0008, 0x0001},
        {0x0009, 0x008F},//Row End 
         
        //Display ON Setting 
        {0x0028, 0x0038},//GON=1);WMLCDDATA( DTE=1);WMLCDDATA( D=10 
        {0xffff, 40}, 
        {0x0028, 0x003C},//GON=1);WMLCDDATA( DTE=1);WMLCDDATA( D=11 
    };
    uint32 i;
    uint16 *p_reg_data = (uint16*)s_hx8352C_init;
    uint16 reg;

    //SCI_TRACE_LOW:"\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8352C_393_112_2_18_0_33_36_1769,(uint8*)"",__MODULE__);

    for(i = 0; i < (sizeof(s_hx8352C_init)/sizeof(s_hx8352C_init[0])); i++)
    {
        reg = *p_reg_data++;
        if(reg == 0XFFFF)
        {
            SCI_Sleep(*p_reg_data++);
        }
        else
        {
            LCD_CtrlData_HX8352C(reg, *p_reg_data++);
        }
    }

    LCD_CtrlWrite_HX8352C(0x22); 

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//  Global resource dependence: 
//  Author:         Huitao Yue
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8352C_EnterSleep(
    BOOLEAN is_sleep    //SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
    )
{
    if(is_sleep)
    {
        // Display off Setting
        LCD_CtrlData_HX8352C(0x28,0x38); // GON=1, DTE=1, D[1:0]=10
        SCI_Sleep(50);
        LCD_CtrlData_HX8352C(0x28,0x20); // GON=0, DTE=0, D[1:0]=01
        // Power off Setting
        LCD_CtrlData_HX8352C(0x1F,0x89); // Stop VCOMG
        // GAS_EN=1, VCOMG=0, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
        LCD_CtrlData_HX8352C(0x19,0x00); //OSC_EN=0, Stop to Oscillate
    }
    else
    {
        LCD_Reset();
        HX8352C_Init();        
    }

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:  set the contrast value 
//  Global resource dependence: 
//  Author:        Huitao Yue
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E   HX8352C_SetContrast(
    uint16  contrast    //contrast value to set
    )
{
    return ERR_LCD_FUNC_NOT_SUPPORT;
} 

/*****************************************************************************/
//  Description:    Set the brightness of LCD.
//  Global resource dependence: 
//  Author:         Huitao Yue
//  Note:
/*****************************************************************************/
LOCAL ERR_LCD_E   HX8352C_SetBrightness(
    uint16 brightness   //birghtness to set
    )
{
    return ERR_LCD_FUNC_NOT_SUPPORT;
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//  Global resource dependence: 
//  Author:         Huitao Yue
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8352C_SetDisplayWindow(
    uint16 left,    // start Horizon address
    uint16 top,     // end Horizon address
    uint16 right,   // start Vertical address
    uint16 bottom   // end Vertical address
    )
{
    //SET WINDOW SIZE
    LCD_CtrlData_HX8352C(0x02,left >> 8);   // Column address start 2
    LCD_CtrlData_HX8352C(0x03,left & 0xff); // Column address start 1
    LCD_CtrlData_HX8352C(0x04,right >> 8);  // Column address end 2
    LCD_CtrlData_HX8352C(0x05,right & 0xff);// Column address end 1
    
    LCD_CtrlData_HX8352C(0x06,top >> 8);    // Row address start 2
    LCD_CtrlData_HX8352C(0x07,top & 0xff);  // Row address start 1
    LCD_CtrlData_HX8352C(0x08,bottom >> 8); // Row address end 2
    LCD_CtrlData_HX8352C(0x09,bottom & 0xff); // Row address end 1  

    switch(s_lcd_direct)
    {
        case LCD_DIRECT_NORMAL: 
        default:
            //SET STARTING ADDRESS
            LCD_CtrlData_HX8352C(0x80,left >> 8);   // Column start 2
            LCD_CtrlData_HX8352C(0x81,left & 0xff); // Column start 1
            LCD_CtrlData_HX8352C(0x82,top >> 8);    // Row start 2
            LCD_CtrlData_HX8352C(0x83,top & 0xff);  // Row start 1
            break;
        case LCD_DIRECT_ROT_90: 
            //SET STARTING ADDRESS          
            LCD_CtrlData_HX8352C(0x80,top >> 8);   // Column start 2
            LCD_CtrlData_HX8352C(0x81,top & 0xff); // Column start 1
            LCD_CtrlData_HX8352C(0x82,left >> 8);    // Row start 2
            LCD_CtrlData_HX8352C(0x83,left & 0xff);  // Row start 1         
            break;
            
        case LCD_DIRECT_ROT_180:
        case LCD_DIRECT_MIR_HV:
            //SET STARTING ADDRESS          
            LCD_CtrlData_HX8352C(0x80,right >> 8);   // Column start 2
            LCD_CtrlData_HX8352C(0x81,right & 0xff); // Column start 1
            LCD_CtrlData_HX8352C(0x82,bottom >> 8);  // Row start 2
            LCD_CtrlData_HX8352C(0x83,bottom & 0xff);// Row start 1             
            break;
            
        case LCD_DIRECT_ROT_270:
            //SET STARTING ADDRESS          
            LCD_CtrlData_HX8352C(0x80,bottom >> 8);   // Column start 2
            LCD_CtrlData_HX8352C(0x81,bottom & 0xff); // Column start 1
            LCD_CtrlData_HX8352C(0x82,left >> 8);    // Row start 2
            LCD_CtrlData_HX8352C(0x83,left & 0xff);  // Row start 1             
            break;

        case LCD_DIRECT_MIR_H:
            //SET STARTING ADDRESS          
            LCD_CtrlData_HX8352C(0x80,left >> 8);   // Column start 2
            LCD_CtrlData_HX8352C(0x81,left & 0xff); // Column start 1
            LCD_CtrlData_HX8352C(0x82,bottom >> 8);    // Row start 2
            LCD_CtrlData_HX8352C(0x83,bottom & 0xff);  // Row start 1               
            break;

        case LCD_DIRECT_MIR_V:
            //SET STARTING ADDRESS          
            LCD_CtrlData_HX8352C(0x80,right >> 8);   // Column start 2
            LCD_CtrlData_HX8352C(0x81,right & 0xff); // Column start 1
            LCD_CtrlData_HX8352C(0x82,top >> 8);    // Row start 2
            LCD_CtrlData_HX8352C(0x83,top & 0xff);  // Row start 1              
            break;
    }

    LCD_CtrlWrite_HX8352C(0x22);
    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//  Global resource dependence: 
//  Author:         Huitao Yue
//  Note:       
/******************************************************************************/
LOCAL ERR_LCD_E HX8352C_InvalidateRect(
    uint16 left,    //the left value of the rectangel
    uint16 top,     //top of the rectangle
    uint16 right,   //right of the rectangle
    uint16 bottom   //bottom of the rectangle
    )
{
    left    = (left >= LCD_WIDTH)    ? LCD_WIDTH-1 : left;
    right   = (right >= LCD_WIDTH)   ? LCD_WIDTH-1 : right;
    top     = (top >= LCD_HEIGHT)    ? LCD_HEIGHT-1 : top;
    bottom  = (bottom >= LCD_HEIGHT) ? LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
        return ERR_LCD_OPERATE_FAIL;
    }     
   
    HX8352C_SetDisplayWindow(left, top, right, bottom); 

    return ERR_LCD_NONE;
    
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//  Global resource dependence: 
//  Author:         Huitao Yue
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8352C_Invalidate(void)
{
    HX8352C_SetDisplayWindow (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence: 
//  Author:         Huitao Yue
//  Note:
/******************************************************************************/
LOCAL void HX8352C_Close(void)
{
    HX8352C_EnterSleep( SCI_TRUE );  
}

/**************************************************************************************/
// Description: refresh a rectangle of lcd
// Global resource dependence:
// Author: Huitao Yue
// Note:
//      left - the left value of the rectangel
//      top - the top value of the rectangel
//      right - the right value of the rectangel
//      bottom - the bottom value of the rectangel
/**************************************************************************************/
LOCAL ERR_LCD_E HX8352C_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
    switch(angle)
    {
        case LCD_ANGLE_0:
        default:            
            HX8352C_SetDisplayWindow(left, top, right, bottom);
            break;
        case LCD_ANGLE_90:
            HX8352C_SetDisplayWindow(left, top, bottom,right);
            break;
        case LCD_ANGLE_180:
            HX8352C_SetDisplayWindow(left, top, right, bottom);
            break;
        case LCD_ANGLE_270:
            HX8352C_SetDisplayWindow(left, top, bottom,right);
            break;
    }
        
    return ERR_LCD_NONE;
}

/******************************************************************************/
// Description: refresh a rectangle of lcd
// Global resource dependence:
// Author: Huitao Yue
// Note: Application should correct invalidate direction right after current
//        image displayed
/******************************************************************************/
LOCAL ERR_LCD_E HX8352C_SetDirection(
    LCD_DIRECT_E direct_type
    )
{
    static const uint8 s_dir_control[LCD_DIRECT_MAX] = 
    {
        0x09, //LCD_DIRECT_NORMAL
        0x69, //LCD_DIRECT_ROT_90
        0xc9, //LCD_DIRECT_ROT_180
        0xa9, //LCD_DIRECT_ROT_270
        0xc8, //LCD_DIRECT_MIR_H
        0x08, //LCD_DIRECT_MIR_V
        0x48  //LCD_DIRECT_MIR_HV
    };

    if(direct_type >= LCD_DIRECT_MAX)
    {
        return ERR_LCD_PARAMETER_WRONG;
    }    

    LCD_CtrlData_HX8352C(0x16,  s_dir_control[direct_type]);
    s_lcd_direct = direct_type;

    return ERR_LCD_NONE;
}

/******************************************************************************/
// Description: Read lcm id
// Global resource dependence: 
// Author: 
// Note:
/******************************************************************************/
LOCAL uint32 HX8352C_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint32 lcm_id=0;
    
    LCD_CtrlWrite_HX8352C(0x00);

    lcm_id = LCD_DataRead_HX8352C(lcd_id, DATA_LEVEL);
    lcm_id = lcm_id&0xFF;
    //0x0072    
    return (lcm_id);
}


/******************************************************************************/
// Description: HX8352C_DRIVER_TEST
// Global resource dependence:
// Author: Huitao Yue
// Note: 
/******************************************************************************/
void HX8352C_DRIVER_TEST( void )
{
    uint32 i = 0;

{
    uint32 lcm_id1,lcm_id2=0;
    LCD_CtrlWrite_HX8352C(0x00);

    *(volatile uint32 *)(0x20800010)  = 0x28;  //set buswidth 8bits, num=0    
    lcm_id1 = LCD_DataRead_HX8352C(0, DATA_LEVEL);   
    lcm_id2 = LCD_DataRead_HX8352C(0, DATA_LEVEL); 
    *(volatile uint32 *)(0x20800010)  = 0x01;  //set buswidth 8bits, num=0      
    lcm_id1 = LCD_DataRead_HX8352C(0, DATA_LEVEL);   
    lcm_id2 = LCD_DataRead_HX8352C(0, DATA_LEVEL); 
}

    HX8352C_SetDisplayWindow(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
    LCD_CtrlWrite_HX8352C(0x22);  //default:start write data to GRAM
    
    while(1)//lint !e716
    {
        for(i=0; i<(320*150); i++)  //blue
        {
            uint32 data = 0x001F;
            LCD_DataWrite_HX8352C(data);                    
        }   
        LCD_Delay(20);
        
        for(i=0; i<(320*150); i++)  //green
        {
            uint32 data = 0x07e0;
            LCD_DataWrite_HX8352C(data);        
        }
        LCD_Delay(20);
        
        for(i=0; i<(320*150); i++)  //red
        {
            uint32 data = 0xF100;
            LCD_DataWrite_HX8352C(data);            
        }
        LCD_Delay(20);
        
    }
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
