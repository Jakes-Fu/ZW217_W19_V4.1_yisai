/******************************************************************************
 ** File Name:     tft_R61529.c                                               *
 ** Description:                                                              *
 **    This file contains driver for color LCD.(CSTN)                          *
 ** Author:         Huitao Yue                                                 *
 ** DATE:           11/23/2011                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/23/2011     Huitao Yue       Create.
 ******************************************************************************/

#include "ms_customize_trc.h"
#include "os_api.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"


/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define R61529_LCD_INDEX MAIN_LCD_ID

extern uint32 LCM_Read (uint32 lcd_id, uint32 cd_level);
#define BUS_WIDTH_R61529    g_lcd_R61529.bus_width


LOCAL void LCD_CtrlWrite_R61529( uint32 _cmd )
{
    LCD_SendCmd( (_cmd ), 0 );
}

LOCAL void LCD_DataWrite_R61529( uint32 _data )
{
    LCD_SendData( (_data), 0 );
}

LOCAL void LCD_CtrlData_R61529( uint32 _cmd, uint32 _data )
{
    LCD_SendCmdData( (_cmd ),  (_data),0 );  
}

LOCAL uint32 LCD_DataRead_R61529(uint32 cs, uint32 cd_level)
{
    uint32 lcm_id=0;    
    lcm_id = LCM_Read(cs, cd_level);

    return (lcm_id);
}

#define LCD_Delay( ticks)\
                OS_TickDelay( ticks );

LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;    

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//    Global resource dependence: 
//  Author:         Huitao Yue
//    Note:
/******************************************************************************/
LOCAL void R61529_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//    Global resource dependence: 
//  Author:         Huitao Yue
//    Note:
/******************************************************************************/
LOCAL ERR_LCD_E  R61529_EnterSleep(
    BOOLEAN is_sleep     //SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
    );



/*********************************************************************/
//  Description:   Initialize color LCD : R61529
//  Input:
//      None.
//  Return:
//      None.
//    Note:           
/*********************************************************************/
LOCAL ERR_LCD_E R61529_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//    Global resource dependence: 
//  Author:         Huitao Yue
//    Note:
/******************************************************************************/
LOCAL ERR_LCD_E R61529_Invalidate(void);


/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//    Global resource dependence: 
//  Author:         Huitao Yue
//    Note:       
//     To improve speed, lcd is operate in HIGH SPEED RAM WRITE MODE(4
//     uint16 are write continuously always.) So, some dummy uint16 
//     should be inserted to satisfy this mode.   Please refer to spec.
/******************************************************************************/
LOCAL ERR_LCD_E R61529_InvalidateRect(
    uint16 left,     //the left value of the rectangel
    uint16 top,     //top of the rectangle
    uint16 right,     //right of the rectangle
    uint16 bottom    //bottom of the rectangle
    );
/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//    Global resource dependence: 
//  Author:         Huitao Yue
//    Note:
/******************************************************************************/
PUBLIC void R61529_set_display_window(
    uint16 left,     // start Horizon address
    uint16 right,     // end Horizon address
    uint16 top,         // start Vertical address
    uint16 bottom    // end Vertical address
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
//    Note: Application should correct invalidate direction right after current
//           image displayed
/******************************************************************************/
LOCAL ERR_LCD_E R61529_SetDirection( LCD_DIRECT_E direct_type)
{
    return ERR_LCD_NONE;
    
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//    Global resource dependence: 
//  Author:         Huitao Yue
//    Note:
/******************************************************************************/
__inline void R61529_set_display_window(
    uint16 left,     // start Horizon address
    uint16 top,     // end Horizon address
    uint16 right,         // start Vertical address
    uint16 bottom    // end Vertical address
    )
{   
    LCD_CtrlWrite_R61529(0x002A); 
    LCD_DataWrite_R61529(((left)>>8)&0x00FF);
    LCD_DataWrite_R61529((left)&0x00FF);
    LCD_DataWrite_R61529(((right)>>8)&0x00FF);
    LCD_DataWrite_R61529((right)&0x00FF);
    LCD_CtrlWrite_R61529(0x002B); 
    LCD_DataWrite_R61529(((top)>>8)&0x00FF);
    LCD_DataWrite_R61529((top)&0x00FF);
    LCD_DataWrite_R61529(((bottom)>>8)&0x00FF);
    LCD_DataWrite_R61529((bottom)&0x00FF);
    LCD_CtrlWrite_R61529(0x002c);

}

/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E R61529_Init(void)
{

    //SCI_TRACE_LOW:"qinss LCD: in R61529_reset"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_R61529_179_112_2_18_0_34_3_1851,(uint8*)"");
    LCD_CtrlWrite_R61529(0xB0);
    LCD_DataWrite_R61529(0x04);

    LCD_CtrlWrite_R61529(0xB3);
    LCD_DataWrite_R61529(0x02); //WEMODE=1
    LCD_DataWrite_R61529(0x00);
    LCD_DataWrite_R61529(0x00);
    LCD_DataWrite_R61529(0x00);
    
    LCD_CtrlWrite_R61529(0x36);    
    LCD_DataWrite_R61529(0x00);    
    
    LCD_CtrlWrite_R61529(0x3A);    
    LCD_DataWrite_R61529(0x55);   
    
    LCD_CtrlWrite_R61529(0xB4);    
    LCD_DataWrite_R61529(0x00); 
    
    LCD_CtrlWrite_R61529(0xC0);
    LCD_DataWrite_R61529(0x03);//0013
    LCD_DataWrite_R61529(0xDF);//480
    LCD_DataWrite_R61529(0x40);
    LCD_DataWrite_R61529(0x12);
    LCD_DataWrite_R61529(0x00);
    LCD_DataWrite_R61529(0x01);
    LCD_DataWrite_R61529(0x00);
    LCD_DataWrite_R61529(0x43);
    
    
    LCD_CtrlWrite_R61529(0xC1);//frame frequency
    LCD_DataWrite_R61529(0x05);//BCn,DIVn[1:0
    LCD_DataWrite_R61529(0x2f);//RTNn[4:0] 
    LCD_DataWrite_R61529(0x08);// BPn[7:0]
    LCD_DataWrite_R61529(0x08);// FPn[7:0]
    LCD_DataWrite_R61529(0x00);
    
    
    
    LCD_CtrlWrite_R61529(0xC4);
    LCD_DataWrite_R61529(0x63);
    LCD_DataWrite_R61529(0x00);
    LCD_DataWrite_R61529(0x08);
    LCD_DataWrite_R61529(0x08);
    
     LCD_CtrlWrite_R61529(0xC8);//Gamma
    LCD_DataWrite_R61529(0x06);
    LCD_DataWrite_R61529(0x0c);
    LCD_DataWrite_R61529(0x16);
    LCD_DataWrite_R61529(0x24);//26
    LCD_DataWrite_R61529(0x30);//32 
    LCD_DataWrite_R61529(0x48);
    LCD_DataWrite_R61529(0x3d);
    LCD_DataWrite_R61529(0x28);
    LCD_DataWrite_R61529(0x20);
    LCD_DataWrite_R61529(0x14);
    LCD_DataWrite_R61529(0x0c);
    LCD_DataWrite_R61529(0x04);
    
    LCD_DataWrite_R61529(0x06);
    LCD_DataWrite_R61529(0x0c);
    LCD_DataWrite_R61529(0x16);
    LCD_DataWrite_R61529(0x24);
    LCD_DataWrite_R61529(0x30);
    LCD_DataWrite_R61529(0x48);
    LCD_DataWrite_R61529(0x3d);
    LCD_DataWrite_R61529(0x28);
    LCD_DataWrite_R61529(0x20);
    LCD_DataWrite_R61529(0x14);
    LCD_DataWrite_R61529(0x0c);
    LCD_DataWrite_R61529(0x04);
    
    
    
    LCD_CtrlWrite_R61529(0xC9);//Gamma
    LCD_DataWrite_R61529(0x06);
    LCD_DataWrite_R61529(0x0c);
    LCD_DataWrite_R61529(0x16);
    LCD_DataWrite_R61529(0x24);//26
    LCD_DataWrite_R61529(0x30);//32 
    LCD_DataWrite_R61529(0x48);
    LCD_DataWrite_R61529(0x3d);
    LCD_DataWrite_R61529(0x28);
    LCD_DataWrite_R61529(0x20);
    LCD_DataWrite_R61529(0x14);
    LCD_DataWrite_R61529(0x0c);
    LCD_DataWrite_R61529(0x04);
    
    LCD_DataWrite_R61529(0x06);
    LCD_DataWrite_R61529(0x0c);
    LCD_DataWrite_R61529(0x16);
    LCD_DataWrite_R61529(0x24);
    LCD_DataWrite_R61529(0x30);
    LCD_DataWrite_R61529(0x48);
    LCD_DataWrite_R61529(0x3d);
    LCD_DataWrite_R61529(0x28);
    LCD_DataWrite_R61529(0x20);
    LCD_DataWrite_R61529(0x14);
    LCD_DataWrite_R61529(0x0c);
    LCD_DataWrite_R61529(0x04);
    
    
    
    LCD_CtrlWrite_R61529(0xCA);//Gamma
    LCD_DataWrite_R61529(0x06);
    LCD_DataWrite_R61529(0x0c);
    LCD_DataWrite_R61529(0x16);
    LCD_DataWrite_R61529(0x24);//26
    LCD_DataWrite_R61529(0x30);//32 
    LCD_DataWrite_R61529(0x48);
    LCD_DataWrite_R61529(0x3d);
    LCD_DataWrite_R61529(0x28);
    LCD_DataWrite_R61529(0x20);
    LCD_DataWrite_R61529(0x14);
    LCD_DataWrite_R61529(0x0c);
    LCD_DataWrite_R61529(0x04);
    
    LCD_DataWrite_R61529(0x06);
    LCD_DataWrite_R61529(0x0c);
    LCD_DataWrite_R61529(0x16);
    LCD_DataWrite_R61529(0x24);
    LCD_DataWrite_R61529(0x30);
    LCD_DataWrite_R61529(0x48);
    LCD_DataWrite_R61529(0x3d);
    LCD_DataWrite_R61529(0x28);
    LCD_DataWrite_R61529(0x20);
    LCD_DataWrite_R61529(0x14);
    LCD_DataWrite_R61529(0x0c);
    LCD_DataWrite_R61529(0x04);
    
    
    LCD_CtrlWrite_R61529(0xD0);
    LCD_DataWrite_R61529(0x95);
    LCD_DataWrite_R61529(0x06);
    LCD_DataWrite_R61529(0x08);
    LCD_DataWrite_R61529(0x10);
    LCD_DataWrite_R61529(0x3f);
    
    
    LCD_CtrlWrite_R61529(0xD1);
    LCD_DataWrite_R61529(0x02);
    LCD_DataWrite_R61529(0x38);
    LCD_DataWrite_R61529(0x38);
    LCD_DataWrite_R61529(0x40);
    
    LCD_CtrlWrite_R61529(0xE1);    
    LCD_DataWrite_R61529(0x00);    
    LCD_DataWrite_R61529(0x00);    
    LCD_DataWrite_R61529(0x00);    
    LCD_DataWrite_R61529(0x00);    
    LCD_DataWrite_R61529(0x00);   
    LCD_DataWrite_R61529(0x00);   
    
    LCD_CtrlWrite_R61529(0xE2);    
    LCD_DataWrite_R61529(0x80);    
    
    LCD_CtrlWrite_R61529(0x2A);    
    LCD_DataWrite_R61529(0x00);    
    LCD_DataWrite_R61529(0x00);    
    LCD_DataWrite_R61529(0x01);    
    LCD_DataWrite_R61529(0x3F);    
    
    LCD_CtrlWrite_R61529(0x2B);    
    LCD_DataWrite_R61529(0x00);    
    LCD_DataWrite_R61529(0x00);    
    LCD_DataWrite_R61529(0x01);    
    LCD_DataWrite_R61529(0xDF);    
    
    LCD_CtrlWrite_R61529(0x11);
    
    SCI_Sleep(120);
    
    LCD_CtrlWrite_R61529(0x29);

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//    Global resource dependence: 
//  Author:         Huitao Yue
//    Note:
/******************************************************************************/
LOCAL ERR_LCD_E  R61529_EnterSleep(
    BOOLEAN is_sleep     //SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
    )
{
    //SCI_TRACE_LOW:"qinss LCD: in R61529_EnterSleep, is_sleep = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_R61529_364_112_2_18_0_34_3_1852,(uint8*)"d", is_sleep);
    if ( is_sleep ) // enter sleep mode.
    {
        LCD_CtrlWrite_R61529(0x10);
        SCI_Sleep(10);
    }
    else             // out sleep mode 
    {
        LCD_Reset();
        R61529_Init();
    }
    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//    Global resource dependence: 
//  Author:         Huitao Yue
//    Note:
/******************************************************************************/
LOCAL void R61529_Close(void)
{
    //SCI_TRACE_LOW:"qinss LCD: in R61529_Close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_R61529_385_112_2_18_0_34_3_1853,(uint8*)"");
    R61529_EnterSleep( SCI_TRUE );    
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//    Global resource dependence: 
//  Author:         Huitao Yue
//    Note:
/******************************************************************************/
LOCAL ERR_LCD_E R61529_Invalidate(void)
{
    R61529_set_display_window (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Copy a retangle data from clcd_buffer to display RAM.
//                     then the rectangle display is to be refreshed
//    Global resource dependence: 
//  Author:         Huitao Yue
//    Note:       
//     To improve speed, lcd is operate in HIGH SPEED RAM WRITE MODE(4
//     uint16 are write continuously always.) So, some dummy uint16 
//     should be inserted to satisfy this mode.   Please refer to spec.
/******************************************************************************/
PUBLIC ERR_LCD_E R61529_InvalidateRect(
    uint16 left,     //the left value of the rectangel
    uint16 top,     //top of the rectangle
    uint16 right,     //right of the rectangle
    uint16 bottom    //bottom of the rectangle
    )
{
      left     = (left >= LCD_WIDTH)    ? LCD_WIDTH-1 : left;
    right     = (right >= LCD_WIDTH)   ? LCD_WIDTH-1 : right;
    top     = (top >= LCD_HEIGHT)    ? LCD_HEIGHT-1 : top;
    bottom     = (bottom >= LCD_HEIGHT) ? LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
        return ERR_LCD_OPERATE_FAIL;
    }     
   
    R61529_set_display_window(left, top, right, bottom); 
  
    return ERR_LCD_NONE;
    
}

/******************************************************************************/
//  Description:  set the contrast value 
//    Global resource dependence: 
//  Author:         Huitao Yue
//    Note:
/******************************************************************************/
LOCAL ERR_LCD_E   R61529_SetContrast(
    uint16  contrast    //contrast value to set
    )
{
    return ERR_LCD_FUNC_NOT_SUPPORT;
} 


/*****************************************************************************/
//  Description:    Set the brightness of LCD.
//    Global resource dependence: 
//  Author:         Huitao Yue
//    Note:
/*****************************************************************************/
LOCAL ERR_LCD_E   R61529_SetBrightness(
    uint16 brightness    //birghtness to set
    )
{
    return ERR_LCD_FUNC_NOT_SUPPORT;
}

/*****************************************************************************/
//  Description:    Enable lcd to partial display mode, so can save power.
//    Global resource dependence: 
//  Author:         Huitao Yue
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//    Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E R61529_SetDisplayWindow(
    uint16 left,         //left of the window
    uint16 top,            //top of the window
    uint16 right,        //right of the window
    uint16 bottom        //bottom of the window
    )
{
         
    R61529_set_display_window(left, top, right, bottom);

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
LOCAL ERR_LCD_E R61529_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
    switch(angle)
    {
        case LCD_ANGLE_0:
            R61529_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_90:
            R61529_set_display_window(left, top, bottom,right);
            break;
        case LCD_ANGLE_180:
            R61529_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_270:
            R61529_set_display_window(left, top, bottom,right);
            break;
        default:
            R61529_set_display_window(left, top, right, bottom);
            break;          
    }
        

    return ERR_LCD_NONE;
}//en of R61529_VerticalInvalidateRect  

/******************************************************************************/
// Description: Read lcm id
// Global resource dependence: 
// Author: 
// Note:  return 0x1529 in 16-bit mode
/******************************************************************************/
LOCAL uint32 R61529_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint32 lcm_id0=0,lcm_id1=0,lcm_id2=0,lcm_id3=0,lcm_id4=0;

    //Manufacturer command access protect
    LCD_CtrlWrite_R61529(0xB0);
    LCD_DataWrite_R61529(0x04);

    LCD_CtrlWrite_R61529(0xBF);
    {
        // Get:XX 01 22 15 29
        lcm_id0 = LCD_DataRead_R61529(lcd_id, DATA_LEVEL) & 0xFF;   
        lcm_id1 = LCD_DataRead_R61529(lcd_id, DATA_LEVEL) & 0xFF;   
        lcm_id2 = LCD_DataRead_R61529(lcd_id, DATA_LEVEL) & 0xFF; 
        lcm_id3 = LCD_DataRead_R61529(lcd_id, DATA_LEVEL) & 0xFF;   
        lcm_id4 = LCD_DataRead_R61529(lcd_id, DATA_LEVEL) & 0xFF;

        return ((lcm_id3<<8)|(lcm_id4));
    }

}

void R61529_DRIVER_TEST( void )
{
    uint32 i = 0;
{
    uint32 lcm_id0=0,lcm_id1=0,lcm_id2=0,lcm_id3=0,lcm_id4=0;
    LCD_CtrlWrite_R61529(0xBF);

    // Get:XX 01 22 15 29 16bit-mode
    lcm_id0 = LCD_DataRead_R61529(0, DATA_LEVEL);   
    lcm_id1 = LCD_DataRead_R61529(0, DATA_LEVEL);   
    lcm_id2 = LCD_DataRead_R61529(0, DATA_LEVEL); 
    lcm_id3 = LCD_DataRead_R61529(0, DATA_LEVEL);   
    lcm_id4 = LCD_DataRead_R61529(0, DATA_LEVEL);   
    *(volatile uint32 *)(0x20800010)  = 0x01;  //set buswidth 8bits, num=0      

}

    R61529_SetDisplayWindow(0,0,319,479);
    LCD_CtrlWrite_R61529(0x002c);  //default:start write data to GRAM
    

while(1)//lint !e716
{
    for(i=0; i<(320*100); i++)  //blue
    {
        uint32 data = 0x001F;
        LCD_DataWrite_R61529(data);                 
    }   
    LCD_Delay(20);
    
    for(i=0; i<(320*100); i++)  //green
    {
        uint32 data = 0x07e0;
        LCD_DataWrite_R61529(data);     
    }
    LCD_Delay(20);
    
    for(i=0; i<(320*100); i++)  //red
    {
        uint32 data = 0xF100;
        LCD_DataWrite_R61529(data);         
    }
    LCD_Delay(20);
    
}

}


LOCAL const LCD_OPERATIONS_T R61529_operations = 
{
    R61529_Init,
    R61529_EnterSleep,
    R61529_SetContrast,
    R61529_SetBrightness,
    R61529_SetDisplayWindow,
    
    R61529_InvalidateRect,
    R61529_Invalidate,
    R61529_Close,
    R61529_RotationInvalidateRect,
    R61529_SetDirection,
    
    NULL,
    R61529_ReadID
};

/******************************************************************************/
//  Description:  return the R61529 lcd driver funtion pointer
//    Global resource dependence: 
//  Author:         Huitao Yue
//    Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* R61529_GetOperations(void)
{
    //SCI_TRACE_LOW:"qinss LCD: in R61529_GetOperations"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_R61529_604_112_2_18_0_34_4_1854,(uint8*)"");

    return (LCD_OPERATIONS_T*)&R61529_operations;
}

PUBLIC BOOLEAN R61529_Probe(void)
{
    return SCI_TRUE;    
}

LOCAL const LCD_TIMING_U s_R61529_timing =
{
#if 1
            // LCM_CYCLE_U start(ns)
    170,        // CS setup time for LCM read (optional)  
    170,    // low pulse width for LCM read (according spec)    
    250,    // high pulse width for LCM read (according spec)
    8,        // CS setup time for LCM write  (optional)     
    15,        // low pulse width for LCM write (according spec)
    15,        // high pulse width for LCM write (according spec)    
            // LCM_CYCLE_U end(ns)
#else
            // LCM_CYCLE_U start(ns)
    170,        // CS setup time for LCM read (optional)  
    170,    // low pulse width for LCM read (according spec)    
    250,    // high pulse width for LCM read (according spec)
    20,     // CS setup time for LCM write  (optional) .WriteCycleTime=50ns 
    20,     // low pulse width for LCM write (according spec)
    20,     // high pulse width for LCM write (according spec)  
#endif          // LCM_CYCLE_U end(ns)
};


const LCD_SPEC_T g_lcd_R61529 = {
    LCD_WIDTH, 
    LCD_HEIGHT, 
    LCD_MCU, 
    BUS_MODE_8080,
    WIDTH_8,
    (LCD_TIMING_U*)&s_R61529_timing, 
    (LCD_OPERATIONS_T*)&R61529_operations,
    0,
    0
};

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 


