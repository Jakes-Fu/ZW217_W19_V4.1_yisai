/******************************************************************************
 ** File Name:     cstn_pcf8833.c                                             *
 ** Description:                                                              *
 **    This file contains driver for color LCD.(CSTN)                         *
 ** Author:         Jim zhang                                                 *
 ** DATE:           03/01/2004                                                *
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
#include "lcm_drvapi.h"/*lint -esym(766,lcm_drvapi.h)*/

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#ifdef PRODUCT_CONFIG_fp6530_openphone

LOCAL void LCD_CtrlWrite_ILI9328(uint32  _cmd )
{
    LCD_SendCmd( (_cmd ), 0 );
}

LOCAL void LCD_DataWrite_ILI9328(uint32 _data )
{
    LCD_SendData( (_data), 0 );
}

LOCAL void LCD_CtrlData_ILI9328( _cmd, _data )/*lint -esym(832,_cmd,_data)*/
{
    LCD_SendCmdData( (_cmd ),  (_data),0 );  
}

LOCAL uint32 LCD_DataRead_ILI9328(uint32 cs, uint32 cd_level)
{
    uint32 lcm_id=0;    
    lcm_id = LCM_Read(cs, cd_level);

    return (lcm_id);
}
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;    

                        
/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL void ILI9328_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9328_EnterSleep(
    BOOLEAN is_sleep    //SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
    );

/*********************************************************************/
//  Description:   Initialize color LCD : ILI9328
//  Input:
//      None.
//  Return:
//      None.
//  Note:           
/*********************************************************************/
LOCAL ERR_LCD_E ILI9328_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9328_Invalidate(void);


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
LOCAL ERR_LCD_E ILI9328_InvalidateRect(
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
PUBLIC void ILI9328_set_display_window(
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
LOCAL ERR_LCD_E ILI9328_SetDirection( LCD_DIRECT_E direct_type)
{
    switch(direct_type)
    {
        case LCD_DIRECT_NORMAL:         
            LCD_CtrlData_ILI9328(0x0003, 0x1030); /*lint !e746*/      
            break;
            
        case LCD_DIRECT_ROT_90:
            LCD_CtrlData_ILI9328(0x0003, 0x1028);
            break;
            
        case LCD_DIRECT_ROT_180:
            LCD_CtrlData_ILI9328(0x0003, 0x1000);
            break;
            
        case LCD_DIRECT_ROT_270:
            LCD_CtrlData_ILI9328(0x0003, 0x1018);
            break;

        case LCD_DIRECT_MIR_H:
            LCD_CtrlData_ILI9328(0x0003, 0x1020);
            break;
            
        case LCD_DIRECT_MIR_V:
            LCD_CtrlData_ILI9328(0x0003, 0x1010);
            break;
            
        case LCD_DIRECT_MIR_HV:
            LCD_CtrlData_ILI9328(0x0003, 0x1000);
            break;
            
        default:            
            LCD_CtrlData_ILI9328(0x0003, 0x1030);

            direct_type = LCD_DIRECT_NORMAL;
            break;
            
    }   
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
__inline void ILI9328_set_display_window(
    uint16 left,    // start Horizon address
    uint16 top,     // end Horizon address
    uint16 right,       // start Vertical address
    uint16 bottom   // end Vertical address
    )
{   
    LCD_CtrlData_ILI9328(0x0050, left);
    LCD_CtrlData_ILI9328(0x0051, right);
    LCD_CtrlData_ILI9328(0x0052, top);
    LCD_CtrlData_ILI9328(0x0053, bottom);

  
    switch(s_lcd_direct)
    {
        case LCD_DIRECT_NORMAL: 
            LCD_CtrlData_ILI9328(0x0020, left);
            LCD_CtrlData_ILI9328(0x0021, top);
            break;            
            
        case LCD_DIRECT_ROT_90:             
            LCD_CtrlData_ILI9328(0x0020, right);
            LCD_CtrlData_ILI9328(0x0021, top);  
            break;
            
        case LCD_DIRECT_ROT_180:
            LCD_CtrlData_ILI9328(0x0020, right);
            LCD_CtrlData_ILI9328(0x0021, bottom);               
            break;
            
        case LCD_DIRECT_ROT_270:
            LCD_CtrlData_ILI9328(0x0020, left);
            LCD_CtrlData_ILI9328(0x0021, top);          
            break;

        case LCD_DIRECT_MIR_H:
            LCD_CtrlData_ILI9328(0x0020, right);
            LCD_CtrlData_ILI9328(0x0021, top);
            break;

        case LCD_DIRECT_MIR_V:
            LCD_CtrlData_ILI9328(0x0020, left);
            LCD_CtrlData_ILI9328(0x0021, bottom);
            break;

        case LCD_DIRECT_MIR_HV:
            LCD_CtrlData_ILI9328(0x0020, right);
            LCD_CtrlData_ILI9328(0x0021, bottom);
            break;
            
        default:            
            LCD_CtrlData_ILI9328(0x0020, left);
            LCD_CtrlData_ILI9328(0x0021, top);
            break;  
    
    }
    
    LCD_CtrlWrite_ILI9328(0x0022);  

}


/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E ILI9328_Init(void)
{

    //SCI_TRACE_LOW:"qinss LCD: in ILI9328_reset"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9328_250_112_2_18_0_33_47_1792,(uint8*)"");

    LCD_Reset();//reset lcd throught lcdc   
    LCD_Delayms(50);


    //************* Start Initial Sequence **********//
    LCD_CtrlData_ILI9328(0x00E3, 0x3008); // Set internal timing
    LCD_CtrlData_ILI9328(0x00E7, 0x0012); // Set internal timing
    LCD_CtrlData_ILI9328(0x00EF, 0x1231); // Set internal timing
    LCD_CtrlData_ILI9328(0x0001, 0x0100); // set SS and SM bit
    LCD_CtrlData_ILI9328(0x0002, 0x0700); // set 1 line inversion
    LCD_CtrlData_ILI9328(0x0003, 0x1030); // set GRAM write direction and BGR=1.
    LCD_CtrlData_ILI9328(0x0004, 0x0000); // Resize register
    LCD_CtrlData_ILI9328(0x0008, 0x0207); // set the back porch and front porch
    LCD_CtrlData_ILI9328(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
    LCD_CtrlData_ILI9328(0x000A, 0x0000); // FMARK function
    LCD_CtrlData_ILI9328(0x000C, 0x0000); // RGB interface setting
    LCD_CtrlData_ILI9328(0x000D, 0x0000); // Frame marker Position
    LCD_CtrlData_ILI9328(0x000F, 0x0000); // RGB interface polarity
    //*************Power On sequence ****************//
    LCD_CtrlData_ILI9328(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_CtrlData_ILI9328(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
    LCD_CtrlData_ILI9328(0x0012, 0x0000); // VREG1OUT voltage
    LCD_CtrlData_ILI9328(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
    LCD_CtrlData_ILI9328(0x0007, 0x0001);
    LCD_Delayms(200);                        // Dis-charge capacitor power voltage
    LCD_CtrlData_ILI9328(0x0010, 0x1590); // SAP, BT[3:0], AP, DSTB, SLP, STB//1690
    LCD_CtrlData_ILI9328(0x0011, 0x0221); // R11h=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]//0227
    LCD_Delayms(50);                         // Delay 50ms
    LCD_CtrlData_ILI9328(0x0012, 0x009A); // External reference voltage= Vci;//009A
    LCD_Delayms(50);                         // Delay 50ms
    LCD_CtrlData_ILI9328(0x0013, 0x1B00); // VDV[4:0] for VCOM amplitude//
    LCD_CtrlData_ILI9328(0x0029, 0x0013); // VCM[5:0] for VCOMH//0020    »ªÇÚS808 0013 
    LCD_CtrlData_ILI9328(0x002B, 0x000E); // Frame Rate = 91Hz         
    LCD_Delayms(50);                         // Delay 50ms
    LCD_CtrlData_ILI9328(0x0020, 0x0000); // GRAM horizontal Address
    LCD_CtrlData_ILI9328(0x0021, 0x0000); // GRAM Vertical Address
    // ----------- Adjust the Gamma Curve ----------//
    LCD_CtrlData_ILI9328(0x0030, 0x0000);
    LCD_CtrlData_ILI9328(0x0031, 0x0406);
    LCD_CtrlData_ILI9328(0x0032, 0x0004);
    LCD_CtrlData_ILI9328(0x0035, 0x0305);
    LCD_CtrlData_ILI9328(0x0036, 0x0000);
    LCD_CtrlData_ILI9328(0x0037, 0x0207);//0307
    LCD_CtrlData_ILI9328(0x0038, 0x0103);
    LCD_CtrlData_ILI9328(0x0039, 0x0707);
    LCD_CtrlData_ILI9328(0x003C, 0x0503);
    LCD_CtrlData_ILI9328(0x003D, 0x0004);//0000
    //------------------ Set GRAM area ---------------//
    LCD_CtrlData_ILI9328(0x0050, 0x0000); // Horizontal GRAM Start Address
    LCD_CtrlData_ILI9328(0x0051, 0x00EF); // Horizontal GRAM End Address
    LCD_CtrlData_ILI9328(0x0052, 0x0000); // Vertical GRAM Start Address
    LCD_CtrlData_ILI9328(0x0053, 0x013F); // Vertical GRAM Start Address
    LCD_CtrlData_ILI9328(0x0060, 0xA700); // Gate Scan Line
    LCD_CtrlData_ILI9328(0x0061, 0x0001); // NDL,VLE, REV
    LCD_CtrlData_ILI9328(0x006A, 0x0000); // set scrolling line
    //-------------- Partial Display Control ---------//
    LCD_CtrlData_ILI9328(0x0080, 0x0000);
    LCD_CtrlData_ILI9328(0x0081, 0x0000);
    LCD_CtrlData_ILI9328(0x0082, 0x0000);
    LCD_CtrlData_ILI9328(0x0083, 0x0000);
    LCD_CtrlData_ILI9328(0x0084, 0x0000);
    LCD_CtrlData_ILI9328(0x0085, 0x0000);
    //-------------- Panel Control -------------------//
    LCD_CtrlData_ILI9328(0x0090, 0x0010);
    LCD_CtrlData_ILI9328(0x0092, 0x0600);

    LCD_CtrlData_ILI9328(0x0007, 0x0133); // 262K color and display ON

    //------------ display on function ------------//  
    LCD_CtrlData_ILI9328(0x0007,0x0022);

    LCD_CtrlData_ILI9328(0x0007,0x0133);
    LCD_Delayms(30);
    //--------------- write to GRAM ---------------// 
    LCD_CtrlData_ILI9328(0x0020,0x0000);
    LCD_CtrlData_ILI9328(0x0021,0x0000);

    LCD_CtrlWrite_ILI9328(0x0022);   //8 bit   

    return ERR_LCD_NONE;
}


/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9328_EnterSleep(
    BOOLEAN is_sleep    //SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
    )
{
    //SCI_TRACE_LOW:"qinss LCD: in ILI9328_EnterSleep, is_sleep = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9328_343_112_2_18_0_33_48_1793,(uint8*)"d", is_sleep);
    if ( is_sleep ) // enter sleep mode.
    {

        LCD_CtrlData_ILI9328(0x0007, 0x0131); // Set D1=0, D0=1
        LCD_Delayms(10);
        LCD_CtrlData_ILI9328(0x0007, 0x0130); // Set D1=0, D0=0
        LCD_Delayms(10);
        LCD_CtrlData_ILI9328(0x0007, 0x0000); // display OFF
       //************* Power OFF sequence **************//
        LCD_CtrlData_ILI9328(0x0010, 0x0080); // SAP, BT[3:0], APE, AP, DSTB, SLP
        LCD_CtrlData_ILI9328(0x0011, 0x0000); // DC1[2:0], DC0[2:0], VC[2:0]
        LCD_CtrlData_ILI9328(0x0012, 0x0000); // VREG1OUT voltage
        LCD_CtrlData_ILI9328(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
        LCD_Delayms(200);                          // Dis-charge capacitor power voltage
        LCD_CtrlData_ILI9328(0x0010, 0x0082); // SAP, BT[3:0], APE, AP, DSTB, SLP
        
    }
    else            // out sleep mode 
    {
        ILI9328_Init();
    }

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL void ILI9328_Close(void)
{
    //SCI_TRACE_LOW:"qinss LCD: in ILI9328_Close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9328_375_112_2_18_0_33_48_1794,(uint8*)"");

    //GPIO_SetLcdBackLight( SCI_FALSE );
    ILI9328_EnterSleep( SCI_TRUE ); 
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9328_Invalidate(void)
{
    ILI9328_set_display_window (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E ILI9328_InvalidateRect(
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
   
    ILI9328_set_display_window(left, top, right, bottom); 
  
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
LOCAL ERR_LCD_E ILI9328_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
    //int32 error;
    

    switch(angle)
    {
        case LCD_ANGLE_0:
            ILI9328_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_90:
            ILI9328_set_display_window(left, top, bottom,right);
            break;
        case LCD_ANGLE_180:
            ILI9328_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_270:
            ILI9328_set_display_window(left, top, bottom,right);
            break;
        default:
            ILI9328_set_display_window(left, top, right, bottom);
            break;          
    }
        

    return ERR_LCD_NONE;
}//en of ILI9328_VerticalInvalidateRect 


/******************************************************************************/
//  Description:  set the contrast value 
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E   ILI9328_SetContrast(
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
LOCAL ERR_LCD_E   ILI9328_SetBrightness(
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
LOCAL ERR_LCD_E ILI9328_SetDisplayWindow(
    uint16 left,        //left of the window
    uint16 top,         //top of the window
    uint16 right,       //right of the window
    uint16 bottom       //bottom of the window
    )
{
        
    ILI9328_set_display_window(left, top, right, bottom);

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL const LCD_OPERATIONS_T ILI9328_operations = 
{
    ILI9328_Init,
    ILI9328_EnterSleep,
    ILI9328_SetContrast,
    ILI9328_SetBrightness,
    ILI9328_SetDisplayWindow,
    ILI9328_InvalidateRect,
    ILI9328_Invalidate,
    ILI9328_Close,
    ILI9328_RotationInvalidateRect,
    ILI9328_SetDirection,
    NULL,
    NULL
};

/******************************************************************************/
//  Description:  return the ILI9328 lcd driver funtion pointer
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9328_GetOperations(void)
{
    //SCI_TRACE_LOW:"qinss LCD: in ILI9328_GetOperations"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9328_527_112_2_18_0_33_48_1795,(uint8*)"");

    return (LCD_OPERATIONS_T*)&ILI9328_operations;
}

PUBLIC BOOLEAN ILI9328_Probe(void)
{
    return SCI_TRUE;    
}

LOCAL  const LCD_TIMING_U s_ILI9328_timing =
{

            // LCM_CYCLE_U start(ns)
    5,      // CS setup time for LCM read (optional)  
    150,    // low pulse width for LCM read (according spec)    
    150,    // high pulse width for LCM read (according spec)
    10,     // CS setup time for LCM write  (optional)  
    50,     // low pulse width for LCM write (according spec)
    50,     // high pulse width for LCM write (according spec)  
            // LCM_CYCLE_U end(ns)
};


const LCD_SPEC_T g_lcd_ILI9328 = {
    LCD_WIDTH, 
    LCD_HEIGHT, 
    LCD_MCU, 
    BUS_MODE_8080,
    WIDTH_8,
    (LCD_TIMING_U*)&s_ILI9328_timing,
    (LCD_OPERATIONS_T*)&ILI9328_operations,
    0,
    0
};

#else
#define LCD_CtrlWrite_ILI9328( _cmd )\
                LCD_SendCmd( (_cmd ), 0 );

#define LCD_DataWrite_ILI9328( _data )\
                LCD_SendData( (_data), 0 );

#define LCD_CtrlData_ILI9328( _cmd, _data ) \
                LCD_SendCmdData( (_cmd ),  (_data),0 );  
            

LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;    

                        
/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL void ILI9328_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9328_EnterSleep(
    BOOLEAN is_sleep    //SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
    );

/*********************************************************************/
//  Description:   Initialize color LCD : ILI9328
//  Input:
//      None.
//  Return:
//      None.
//  Note:           
/*********************************************************************/
LOCAL ERR_LCD_E ILI9328_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9328_Invalidate(void);


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
LOCAL ERR_LCD_E ILI9328_InvalidateRect(
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
PUBLIC void ILI9328_set_display_window(
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
LOCAL ERR_LCD_E ILI9328_SetDirection( LCD_DIRECT_E direct_type)
{
    switch(direct_type)
    {
        case LCD_DIRECT_NORMAL:         
            LCD_CtrlData_ILI9328(0x0003, 0x1030);       
            break;
            
        case LCD_DIRECT_ROT_90:
            LCD_CtrlData_ILI9328(0x0003, 0x1028);
            break;
            
        case LCD_DIRECT_ROT_180:
            LCD_CtrlData_ILI9328(0x0003, 0x1000);
            break;
            
        case LCD_DIRECT_ROT_270:
            LCD_CtrlData_ILI9328(0x0003, 0x1018);
            break;

        case LCD_DIRECT_MIR_H:
            LCD_CtrlData_ILI9328(0x0003, 0x1020);
            break;
            
        case LCD_DIRECT_MIR_V:
            LCD_CtrlData_ILI9328(0x0003, 0x1010);
            break;
            
        case LCD_DIRECT_MIR_HV:
            LCD_CtrlData_ILI9328(0x0003, 0x1000);
            break;
            
        default:            
            LCD_CtrlData_ILI9328(0x0003, 0x1030);

            direct_type = LCD_DIRECT_NORMAL;
            break;
            
    }   
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
__inline void ILI9328_set_display_window(
    uint16 left,    // start Horizon address
    uint16 top,     // end Horizon address
    uint16 right,       // start Vertical address
    uint16 bottom   // end Vertical address
    )
{   
    LCD_CtrlData_ILI9328(0x0050, left);
    LCD_CtrlData_ILI9328(0x0051, right);
    LCD_CtrlData_ILI9328(0x0052, top);
    LCD_CtrlData_ILI9328(0x0053, bottom);

  
    switch(s_lcd_direct)
    {
        case LCD_DIRECT_NORMAL: 
            LCD_CtrlData_ILI9328(0x0020, left);
            LCD_CtrlData_ILI9328(0x0021, top);
            break;            
            
        case LCD_DIRECT_ROT_90:             
            LCD_CtrlData_ILI9328(0x0020, right);
            LCD_CtrlData_ILI9328(0x0021, top);  
            break;
            
        case LCD_DIRECT_ROT_180:
            LCD_CtrlData_ILI9328(0x0020, right);
            LCD_CtrlData_ILI9328(0x0021, bottom);               
            break;
            
        case LCD_DIRECT_ROT_270:
            LCD_CtrlData_ILI9328(0x0020, left);
            LCD_CtrlData_ILI9328(0x0021, top);          
            break;

        case LCD_DIRECT_MIR_H:
            LCD_CtrlData_ILI9328(0x0020, right);
            LCD_CtrlData_ILI9328(0x0021, top);
            break;

        case LCD_DIRECT_MIR_V:
            LCD_CtrlData_ILI9328(0x0020, left);
            LCD_CtrlData_ILI9328(0x0021, bottom);
            break;

        case LCD_DIRECT_MIR_HV:
            LCD_CtrlData_ILI9328(0x0020, right);
            LCD_CtrlData_ILI9328(0x0021, bottom);
            break;
            
        default:            
            LCD_CtrlData_ILI9328(0x0020, left);
            LCD_CtrlData_ILI9328(0x0021, top);
            break;  
    
    }
    
    LCD_CtrlWrite_ILI9328(0x0022);  

}


/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E ILI9328_Init(void)
{

    //SCI_TRACE_LOW:"qinss LCD: in ILI9328_reset"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9328_765_112_2_18_0_33_48_1796,(uint8*)"");

    LCD_Reset();//reset lcd throught lcdc   
    LCD_Delayms(50);


    //************* Start Initial Sequence **********//
    LCD_CtrlData_ILI9328(0x00E3, 0x3008); // Set internal timing
    LCD_CtrlData_ILI9328(0x00E7, 0x0012); // Set internal timing
    LCD_CtrlData_ILI9328(0x00EF, 0x1231); // Set internal timing
    LCD_CtrlData_ILI9328(0x0001, 0x0100); // set SS and SM bit
    LCD_CtrlData_ILI9328(0x0002, 0x0700); // set 1 line inversion
    LCD_CtrlData_ILI9328(0x0003, 0x1030); // set GRAM write direction and BGR=1.
    LCD_CtrlData_ILI9328(0x0004, 0x0000); // Resize register
    LCD_CtrlData_ILI9328(0x0008, 0x0207); // set the back porch and front porch
    LCD_CtrlData_ILI9328(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
    LCD_CtrlData_ILI9328(0x000A, 0x0000); // FMARK function
    LCD_CtrlData_ILI9328(0x000C, 0x0000); // RGB interface setting
    LCD_CtrlData_ILI9328(0x000D, 0x0000); // Frame marker Position
    LCD_CtrlData_ILI9328(0x000F, 0x0000); // RGB interface polarity
    //*************Power On sequence ****************//
    LCD_CtrlData_ILI9328(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_CtrlData_ILI9328(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
    LCD_CtrlData_ILI9328(0x0012, 0x0000); // VREG1OUT voltage
    LCD_CtrlData_ILI9328(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
    LCD_CtrlData_ILI9328(0x0007, 0x0001);
    LCD_Delayms(200);                        // Dis-charge capacitor power voltage
    LCD_CtrlData_ILI9328(0x0010, 0x1590); // SAP, BT[3:0], AP, DSTB, SLP, STB//1690
    LCD_CtrlData_ILI9328(0x0011, 0x0221); // R11h=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]//0227
    LCD_Delayms(50);                         // Delay 50ms
    LCD_CtrlData_ILI9328(0x0012, 0x009A); // External reference voltage= Vci;//009A
    LCD_Delayms(50);                         // Delay 50ms
    LCD_CtrlData_ILI9328(0x0013, 0x1B00); // VDV[4:0] for VCOM amplitude//
    LCD_CtrlData_ILI9328(0x0029, 0x0013); // VCM[5:0] for VCOMH//0020    »ªÇÚS808 0013 
    LCD_CtrlData_ILI9328(0x002B, 0x000E); // Frame Rate = 91Hz         
    LCD_Delayms(50);                         // Delay 50ms
    LCD_CtrlData_ILI9328(0x0020, 0x0000); // GRAM horizontal Address
    LCD_CtrlData_ILI9328(0x0021, 0x0000); // GRAM Vertical Address
    // ----------- Adjust the Gamma Curve ----------//
    LCD_CtrlData_ILI9328(0x0030, 0x0000);
    LCD_CtrlData_ILI9328(0x0031, 0x0406);
    LCD_CtrlData_ILI9328(0x0032, 0x0004);
    LCD_CtrlData_ILI9328(0x0035, 0x0305);
    LCD_CtrlData_ILI9328(0x0036, 0x0000);
    LCD_CtrlData_ILI9328(0x0037, 0x0207);//0307
    LCD_CtrlData_ILI9328(0x0038, 0x0103);
    LCD_CtrlData_ILI9328(0x0039, 0x0707);
    LCD_CtrlData_ILI9328(0x003C, 0x0503);
    LCD_CtrlData_ILI9328(0x003D, 0x0004);//0000
    //------------------ Set GRAM area ---------------//
    LCD_CtrlData_ILI9328(0x0050, 0x0000); // Horizontal GRAM Start Address
    LCD_CtrlData_ILI9328(0x0051, 0x00EF); // Horizontal GRAM End Address
    LCD_CtrlData_ILI9328(0x0052, 0x0000); // Vertical GRAM Start Address
    LCD_CtrlData_ILI9328(0x0053, 0x013F); // Vertical GRAM Start Address
    LCD_CtrlData_ILI9328(0x0060, 0xA700); // Gate Scan Line
    LCD_CtrlData_ILI9328(0x0061, 0x0001); // NDL,VLE, REV
    LCD_CtrlData_ILI9328(0x006A, 0x0000); // set scrolling line
    //-------------- Partial Display Control ---------//
    LCD_CtrlData_ILI9328(0x0080, 0x0000);
    LCD_CtrlData_ILI9328(0x0081, 0x0000);
    LCD_CtrlData_ILI9328(0x0082, 0x0000);
    LCD_CtrlData_ILI9328(0x0083, 0x0000);
    LCD_CtrlData_ILI9328(0x0084, 0x0000);
    LCD_CtrlData_ILI9328(0x0085, 0x0000);
    //-------------- Panel Control -------------------//
    LCD_CtrlData_ILI9328(0x0090, 0x0010);
    LCD_CtrlData_ILI9328(0x0092, 0x0600);

    LCD_CtrlData_ILI9328(0x0007, 0x0133); // 262K color and display ON

    //------------ display on function ------------//  
    LCD_CtrlData_ILI9328(0x0007,0x0022);

    LCD_CtrlData_ILI9328(0x0007,0x0133);
    LCD_Delayms(30);
    //--------------- write to GRAM ---------------// 
    LCD_CtrlData_ILI9328(0x0020,0x0000);
    LCD_CtrlData_ILI9328(0x0021,0x0000);

    LCD_CtrlWrite_ILI9328(0x0022);   //8 bit   

    return ERR_LCD_NONE;
}


/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9328_EnterSleep(
    BOOLEAN is_sleep    //SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
    )
{
    //SCI_TRACE_LOW:"qinss LCD: in ILI9328_EnterSleep, is_sleep = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9328_858_112_2_18_0_33_49_1797,(uint8*)"d", is_sleep);
    if ( is_sleep ) // enter sleep mode.
    {

        LCD_CtrlData_ILI9328(0x0007, 0x0131); // Set D1=0, D0=1
        LCD_Delayms(10);
        LCD_CtrlData_ILI9328(0x0007, 0x0130); // Set D1=0, D0=0
        LCD_Delayms(10);
        LCD_CtrlData_ILI9328(0x0007, 0x0000); // display OFF
       //************* Power OFF sequence **************//
        LCD_CtrlData_ILI9328(0x0010, 0x0080); // SAP, BT[3:0], APE, AP, DSTB, SLP
        LCD_CtrlData_ILI9328(0x0011, 0x0000); // DC1[2:0], DC0[2:0], VC[2:0]
        LCD_CtrlData_ILI9328(0x0012, 0x0000); // VREG1OUT voltage
        LCD_CtrlData_ILI9328(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
        LCD_Delayms(200);                          // Dis-charge capacitor power voltage
        LCD_CtrlData_ILI9328(0x0010, 0x0082); // SAP, BT[3:0], APE, AP, DSTB, SLP
        
    }
    else            // out sleep mode 
    {
        ILI9328_Init();
    }

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL void ILI9328_Close(void)
{
    //SCI_TRACE_LOW:"qinss LCD: in ILI9328_Close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9328_890_112_2_18_0_33_49_1798,(uint8*)"");

    //GPIO_SetLcdBackLight( SCI_FALSE );
    ILI9328_EnterSleep( SCI_TRUE ); 
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9328_Invalidate(void)
{
    ILI9328_set_display_window (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E ILI9328_InvalidateRect(
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
   
    ILI9328_set_display_window(left, top, right, bottom); 
  
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
LOCAL ERR_LCD_E ILI9328_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
    //int32 error;
    

    switch(angle)
    {
        case LCD_ANGLE_0:
            ILI9328_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_90:
            ILI9328_set_display_window(left, top, bottom,right);
            break;
        case LCD_ANGLE_180:
            ILI9328_set_display_window(left, top, right, bottom);
            break;
        case LCD_ANGLE_270:
            ILI9328_set_display_window(left, top, bottom,right);
            break;
        default:
            ILI9328_set_display_window(left, top, right, bottom);
            break;          
    }
        

    return ERR_LCD_NONE;
}//en of ILI9328_VerticalInvalidateRect 


/******************************************************************************/
//  Description:  set the contrast value 
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL ERR_LCD_E   ILI9328_SetContrast(
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
LOCAL ERR_LCD_E   ILI9328_SetBrightness(
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
LOCAL ERR_LCD_E ILI9328_SetDisplayWindow(
    uint16 left,        //left of the window
    uint16 top,         //top of the window
    uint16 right,       //right of the window
    uint16 bottom       //bottom of the window
    )
{
        
    ILI9328_set_display_window(left, top, right, bottom);

    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
LOCAL const LCD_OPERATIONS_T ILI9328_operations = 
{
    ILI9328_Init,
    ILI9328_EnterSleep,
    ILI9328_SetContrast,
    ILI9328_SetBrightness,
    ILI9328_SetDisplayWindow,
    ILI9328_InvalidateRect,
    ILI9328_Invalidate,
    ILI9328_Close,
    ILI9328_RotationInvalidateRect,
    ILI9328_SetDirection,
    NULL,
    NULL
};

/******************************************************************************/
//  Description:  return the ILI9328 lcd driver funtion pointer
//  Global resource dependence: 
//  Author:         Jim.zhang
//  Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9328_GetOperations(void)
{
    //SCI_TRACE_LOW:"qinss LCD: in ILI9328_GetOperations"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9328_1042_112_2_18_0_33_49_1799,(uint8*)"");

    return (LCD_OPERATIONS_T*)&ILI9328_operations;
}

PUBLIC BOOLEAN ILI9328_Probe(void)
{
    return SCI_TRUE;    
}

LOCAL  const LCD_TIMING_U s_ILI9328_timing =
{

            // LCM_CYCLE_U start(ns)
    5,      // CS setup time for LCM read (optional)  
    150,    // low pulse width for LCM read (according spec)    
    150,    // high pulse width for LCM read (according spec)
    10,     // CS setup time for LCM write  (optional)  
    50,     // low pulse width for LCM write (according spec)
    50,     // high pulse width for LCM write (according spec)  
            // LCM_CYCLE_U end(ns)
};


const LCD_SPEC_T g_lcd_ILI9328 = {
    LCD_WIDTH, 
    LCD_HEIGHT, 
    LCD_MCU, 
    BUS_MODE_8080,
    WIDTH_16,
    (LCD_TIMING_U*)&s_ILI9328_timing,
    (LCD_OPERATIONS_T*)&ILI9328_operations,
    0,
    0
};

#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 


