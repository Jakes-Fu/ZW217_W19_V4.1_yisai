//##!(important notice: do not remove this line)
/******************************************************************************
 ** File Name:     TFT_HX8357.c                                               *
 ** Description:                                                              *
 **    This file contains driver for color LCD which controler IC is HX8357   *
 ** Author:         Haifeng.Yang                                              *
 ** DATE:           07/06/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 **   DATE           NAME                      DESCRIPTION                    *
 ** 07/06/2010     Haifeng.Yang                   Create.                     *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "spload_main.h"
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
#define LCD_CtrlWrite_HX8357(_cmd)       s_lcm_func_table->lcd_sendcmd((_cmd ), 0)
#define LCD_DataWrite_HX8357(_data)       s_lcm_func_table->lcd_senddata((_data), 0)
#define LCD_CtrlData_HX8357(_cmd, _data) s_lcm_func_table->lcd_sendcmdData((_cmd ), (_data), 0)

/**---------------------------------------------------------------------------*
 **                         Local Function Prototype                          *
 **---------------------------------------------------------------------------*/
LOCAL ERR_LCD_E HX8357_Init(void);
LOCAL ERR_LCD_E HX8357_EnterSleep(BOOLEAN is_sleep);
LOCAL ERR_LCD_E HX8357_SetContrast(uint16 contrast);
LOCAL ERR_LCD_E HX8357_SetBrightness(uint16 brightness);
LOCAL ERR_LCD_E HX8357_SetDisplayWindow(uint16 left, uint16 top, uint16 right, 
    uint16 bottom);
LOCAL ERR_LCD_E HX8357_InvalidateRect(uint16 left, uint16 top, uint16 right, 
    uint16 bottom);
LOCAL ERR_LCD_E HX8357_Invalidate(void);
LOCAL void HX8357_Close(void);
LOCAL ERR_LCD_E HX8357_RotationInvalidateRect(uint16 left,uint16 top,
    uint16 right,uint16 bottom,LCD_ANGLE_E angle);
LOCAL ERR_LCD_E HX8357_SetDirection(LCD_DIRECT_E direct_type);
/**---------------------------------------------------------------------------*
 **                         Data & Function Prototype                         *
 **---------------------------------------------------------------------------*/
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;
LOCAL BOOLEAN      s_is_invert  = SCI_FALSE;
//OPERATIOS TABLE
LOCAL const LCD_OPERATIONS_T s_HX8357_ops_tab = 
{
    HX8357_Init,
    HX8357_EnterSleep,
    HX8357_SetContrast,
    HX8357_SetBrightness,
    HX8357_SetDisplayWindow,
    HX8357_InvalidateRect,
    HX8357_Invalidate,
    HX8357_Close,
    HX8357_RotationInvalidateRect,
    HX8357_SetDirection,
    NULL
};

//TIME TABLE
LOCAL const LCD_TIMING_U s_HX8357_timing =
{

	        // LCM_CYCLE_U start(ns)
	360,	// CS setup time for LCM read (optional)  
	90,	    // low pulse width for LCM read (according spec)	
	360,	// high pulse width for LCM read (according spec)
	10,		// CS setup time for LCM write  (optional) 	
	15,		// low pulse width for LCM write (according spec)
	15,	    // high pulse width for LCM write (according spec)	
	        // LCM_CYCLE_U end(ns)

};

//LCD INFO
PUBLIC const LCD_SPEC_T g_lcd_HX8357 = 
{
    LCD_WIDTH, 
    LCD_HEIGHT, 
    LCD_MCU, 
    BUS_MODE_8080,
#ifdef LCD_DATA_WIDTH_8BIT
	WIDTH_8,
#else
    WIDTH_16,
#endif
    (LCD_TIMING_U*)&s_HX8357_timing, 
    (LCD_OPERATIONS_T*)&s_HX8357_ops_tab,
    1,
    0
};

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Haifeng.Yang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E HX8357_Init(void)
{
    static const uint16 s_hx8357_init[][2] = 
    {
        {0xFF,0x00}, // Command page 0
        {0xffff,15}, // Delay
        {0x1A,0x04}, // VGH VGL VCL  DDVDH
        {0x1B,0x1C},
        
        // Power Setting
        {0x23,0x91}, // Set VCOM offset, VMF=0x52   
        {0x24,0x69}, // Set VCOMH voltage, VHH=0x64
        {0x25,0x63}, // Set VCOML voltage, VML=0x71
        {0x19,0x01}, 
        {0xffff,10}, // Delay
        {0x1A,0x01}, // VGH VGL VCL  DDVDH
        {0x1F,0x8A}, //
        {0x01,0x00}, //
        {0x1C,0x05}, // 
        {0x1F,0x82}, //
        {0xffff,10}, // Delay
        {0x1F,0x92}, //
        {0xffff,10}, // Delay
        {0x1F,0xD4}, //

        // Set GRAM area 320x480
        {0x02,0x00},
        {0x03,0x00},
        {0x04,0x01},
        {0x05,0x3F},
        {0x06,0x00},
        {0x07,0x00},
        {0x08,0x01},
        
           
        {0x16,0x09},//set my mx mv bgr...
        {0x17,0x05},//16-bit/pixel      
        {0x18,0x22},//Fosc=130%*5.2MHZ   21
        {0x1D,0x00},//FS0[1:0]=01, Set the operating frequency of the step-up circuit 1        
        {0x1E,0x00},    
                
        {0x26,0x33},
        {0x27,0x01},
        {0x29,0x00},
        {0x2A,0x00},
        {0x2B,0x01},//0A
        {0x2C,0x0A},
        {0x2D,0x20},
        {0x2E,0xA3},
        {0x2F,0x00},
        {0x31,0x00},
        {0x32,0x00},
        {0x33,0x08},
        {0x34,0x02},
        {0x36,0x02},//REV
                
        // Gamma 
        {0x40,0x01},
        {0x41,0x12},
        {0x42,0x19},
        {0x43,0x26},
        {0x44,0x2C},
        {0x45,0x3C},
        {0x46,0x0B},
        {0x47,0x5F},
        {0x48,0x00},
        {0x49,0x08},
        {0x4A,0x0B},
        {0x4B,0x10},
        {0x4C,0x16},
                   
        {0x50,0x01},
        {0x51,0x1D},
        {0x52,0x21},
        {0x53,0x34},
        {0x54,0x37},
        {0x55,0x3F},
        {0x56,0x29},
        {0x57,0x7F},
        {0x58,0x01},
        {0x59,0x12},
        {0x5A,0x1B},
        {0x5B,0x1E},
        {0x5C,0x1A},
        {0x5D,0x55},
                  
        {0x60,0x08},
        {0xF2,0x00},
        {0xE4,0x1F}, //EQVCI_M1=0x00
        {0xE5,0x1F}, //EQGND_M1=0x1C
        {0xE6,0x20}, //EQVCI_M0=0x1C
        {0xE7,0x00}, //EQGND_M0=0x1C
        {0xE8,0xD1},
        {0xE9,0xC0},
        {0x28,0x38},
        {0xffff,80}, // Delay
        {0x28,0x3C}, // GON=0, DTE=0, D[1:0]=01 
                   
        {0x80,0x00},
        {0x81,0x00},
        {0x82,0x00},
        {0x83,0x00}
    };
    uint32 i;
    uint16 *p_reg_data = (uint16*)s_hx8357_init;
    uint16 reg;

    s_lcm_func_table->trace("\r\n",__MODULE__);    

    for(i = 0; i < (sizeof(s_hx8357_init)/sizeof(s_hx8357_init[0])); i++)
    {
        reg = *p_reg_data++;
        if(reg == 0XFFFF)
        {
            s_lcm_func_table->lcd_delayms(*p_reg_data++);
        }
        else
        {
            LCD_CtrlData_HX8357(reg, *p_reg_data++);
        }
    }

    LCD_CtrlWrite_HX8357(0x22); 

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//  Global resource dependence: 
//  Author:         Haifeng.Yang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8357_EnterSleep(
    BOOLEAN is_sleep    //SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
    )
{
    if(is_sleep)
    {
        LCD_CtrlData_HX8357(0xFF,0x00); //Select Command Page 0
        // Display off Setting
        LCD_CtrlData_HX8357(0x28,0x38); // GON=1, DTE=1, D[1:0]=10
        s_lcm_func_table->lcd_delayms(40);
        LCD_CtrlData_HX8357(0x28,0x04); // GON=0, DTE=0, D[1:0]=01
        // Power off Setting
        LCD_CtrlData_HX8357(0x1F,0x90); // Stop VCOMG
        // GAS_EN=1, VCOMG=0, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
        s_lcm_func_table->lcd_delayms(5);
        LCD_CtrlData_HX8357(0x1F,0x88); // Stop step-up circuit
        // GAS_EN=1, VCOMG=1, PON=0, DK=1, XDK=1, DDVDH_TRI=0, STB=0
        LCD_CtrlData_HX8357(0x1C,0x00); // AP=000
        LCD_CtrlData_HX8357(0x1F,0x89); // Enter Standby mode
        //GAS_EN=1, VCOMG=0, PON=0, DK=1, XDK=0, DDVDH_TRI=0, STB=1
        LCD_CtrlData_HX8357(0x19,0x00); //OSC_EN=0, Stop to Oscillate
    }
    else
    {
        LCD_CtrlData_HX8357(0xFF,0x00); //Select Command Page 0
        LCD_CtrlData_HX8357(0x19,0x01); //OSC_EN=1, Start to Oscillate
        s_lcm_func_table->lcd_delayms(5);
        LCD_CtrlData_HX8357(0x1F,0x88);
        //GAS_EN=1, VCOMG=0, PON=0, DK=1, XDK=0, DDVDH_TRI=0, STB=0
        // Power on Setting
        LCD_CtrlData_HX8357(0x1C,0x03); // AP=011
        LCD_CtrlData_HX8357(0x1F,0x80); // Exit standby mode and Step-up circuit 1 enable
        // GAS_EN=1, VCOMG=0, PON=0, DK=0, XDK=0, DDVDH_TRI=0, STB=0
        s_lcm_func_table->lcd_delayms(5);
        LCD_CtrlData_HX8357(0x1F,0x90); // Step-up circuit 2 enable
        // GAS_EN=1, VCOMG=0, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
        s_lcm_func_table->lcd_delayms(5);
        LCD_CtrlData_HX8357(0x1F,0xD4);
        // GAS_EN=1, VCOMG=1, PON=1, DK=0, XDK=1, DDVDH_TRI=0, STB=0
        s_lcm_func_table->lcd_delayms(5);
        // Display on Setting
        LCD_CtrlData_HX8357(0x28,0x08); // GON=0, DTE=0, D[1:0]=01
        s_lcm_func_table->lcd_delayms(40);
        LCD_CtrlData_HX8357(0x28,0x38); // GON=1, DTE=1, D[1:0]=10
        s_lcm_func_table->lcd_delayms(40);
        LCD_CtrlData_HX8357(0x28,0x3C); // GON=1, DTE=1, D[1:0]=11
    }

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:  set the contrast value 
//  Global resource dependence: 
//  Author:        Haifeng.Yang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E   HX8357_SetContrast(
    uint16  contrast    //contrast value to set
    )
{
    return ERR_LCD_FUNC_NOT_SUPPORT;
} 

/*****************************************************************************/
//  Description:    Set the brightness of LCD.
//  Global resource dependence: 
//  Author:         Haifeng.Yang
//  Note:
/*****************************************************************************/
LOCAL ERR_LCD_E   HX8357_SetBrightness(
    uint16 brightness   //birghtness to set
    )
{
    return ERR_LCD_FUNC_NOT_SUPPORT;
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//  Global resource dependence: 
//  Author:         Haifeng.Yang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8357_SetDisplayWindow(
    uint16 left,    // start Horizon address
    uint16 top,     // end Horizon address
    uint16 right,   // start Vertical address
    uint16 bottom   // end Vertical address
    )
{
	//SET WINDOW SIZE
    LCD_CtrlData_HX8357(0x02,left >> 8);   // Column address start 2
    LCD_CtrlData_HX8357(0x03,left & 0xff); // Column address start 1
    LCD_CtrlData_HX8357(0x04,right >> 8);  // Column address end 2
    LCD_CtrlData_HX8357(0x05,right & 0xff);// Column address end 1
    
    LCD_CtrlData_HX8357(0x06,top >> 8);    // Row address start 2
    LCD_CtrlData_HX8357(0x07,top & 0xff);  // Row address start 1
    LCD_CtrlData_HX8357(0x08,bottom >> 8); // Row address end 2
    LCD_CtrlData_HX8357(0x09,bottom & 0xff); // Row address end 1	

    switch(s_lcd_direct)
    {
        case LCD_DIRECT_NORMAL: 
        default:
			//SET STARTING ADDRESS
            LCD_CtrlData_HX8357(0x80,left >> 8);   // Column start 2
            LCD_CtrlData_HX8357(0x81,left & 0xff); // Column start 1
            LCD_CtrlData_HX8357(0x82,top >> 8);    // Row start 2
            LCD_CtrlData_HX8357(0x83,top & 0xff);  // Row start 1
            break;
            
        case LCD_DIRECT_ROT_90: 
			//SET STARTING ADDRESS			
            LCD_CtrlData_HX8357(0x80,top >> 8);   // Column start 2
            LCD_CtrlData_HX8357(0x81,top & 0xff); // Column start 1
            LCD_CtrlData_HX8357(0x82,left >> 8);    // Row start 2
            LCD_CtrlData_HX8357(0x83,left & 0xff);  // Row start 1			
            break;
            
        case LCD_DIRECT_ROT_180:
        case LCD_DIRECT_MIR_HV:
			//SET STARTING ADDRESS			
            LCD_CtrlData_HX8357(0x80,right >> 8);   // Column start 2
            LCD_CtrlData_HX8357(0x81,right & 0xff); // Column start 1
            LCD_CtrlData_HX8357(0x82,bottom >> 8);  // Row start 2
            LCD_CtrlData_HX8357(0x83,bottom & 0xff);// Row start 1				
            break;
            
        case LCD_DIRECT_ROT_270:
			//SET STARTING ADDRESS			
            LCD_CtrlData_HX8357(0x80,bottom >> 8);   // Column start 2
            LCD_CtrlData_HX8357(0x81,bottom & 0xff); // Column start 1
            LCD_CtrlData_HX8357(0x82,left >> 8);    // Row start 2
            LCD_CtrlData_HX8357(0x83,left & 0xff);  // Row start 1				
            break;

        case LCD_DIRECT_MIR_H:
			//SET STARTING ADDRESS			
            LCD_CtrlData_HX8357(0x80,left >> 8);   // Column start 2
            LCD_CtrlData_HX8357(0x81,left & 0xff); // Column start 1
            LCD_CtrlData_HX8357(0x82,bottom >> 8);    // Row start 2
            LCD_CtrlData_HX8357(0x83,bottom & 0xff);  // Row start 1				
            break;

        case LCD_DIRECT_MIR_V:
			//SET STARTING ADDRESS			
            LCD_CtrlData_HX8357(0x80,right >> 8);   // Column start 2
            LCD_CtrlData_HX8357(0x81,right & 0xff); // Column start 1
            LCD_CtrlData_HX8357(0x82,top >> 8);    // Row start 2
            LCD_CtrlData_HX8357(0x83,top & 0xff);  // Row start 1				
            break;

    }

    LCD_CtrlWrite_HX8357(0x22);
    return ERR_LCD_NONE;
}
/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//  Global resource dependence: 
//  Author:         Haifeng.Yang
//  Note:       
/******************************************************************************/
LOCAL ERR_LCD_E HX8357_InvalidateRect(
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
   
    HX8357_SetDisplayWindow(left, top, right, bottom); 

    return ERR_LCD_NONE;
    
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//  Global resource dependence: 
//  Author:         Haifeng.Yang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8357_Invalidate(void)
{
    HX8357_SetDisplayWindow (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence: 
//  Author:         Haifeng.Yang
//  Note:
/******************************************************************************/
LOCAL void HX8357_Close(void)
{
    HX8357_EnterSleep( SCI_TRUE );  
}

/**************************************************************************************/
// Description: refresh a rectangle of lcd
// Global resource dependence:
// Author: Haifeng.Yang
// Note:
//      left - the left value of the rectangel
//      top - the top value of the rectangel
//      right - the right value of the rectangel
//      bottom - the bottom value of the rectangel
/**************************************************************************************/
LOCAL ERR_LCD_E HX8357_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
    switch(angle)
    {
        case LCD_ANGLE_0:
		default:			
            HX8357_SetDisplayWindow(left, top, right, bottom);
            break;
        case LCD_ANGLE_90:
            HX8357_SetDisplayWindow(left, top, bottom,right);
            break;
        case LCD_ANGLE_180:
            HX8357_SetDisplayWindow(left, top, right, bottom);
            break;
        case LCD_ANGLE_270:
            HX8357_SetDisplayWindow(left, top, bottom,right);
            break;
    }
        
    return ERR_LCD_NONE;
}
/******************************************************************************/
// Description: refresh a rectangle of lcd
// Global resource dependence:
// Author: Haifeng.Yang
// Note: Application should correct invalidate direction right after current
//        image displayed
/******************************************************************************/
LOCAL ERR_LCD_E HX8357_SetDirection(
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

    LCD_CtrlData_HX8357(0x16,  s_dir_control[direct_type]);
    s_lcd_direct = direct_type;

    return ERR_LCD_NONE;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
