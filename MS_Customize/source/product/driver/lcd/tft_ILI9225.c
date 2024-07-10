/******************************************************************************
 ** File Name:     tft_ILI9225B.c                                             *
 ** Description:															  *
 **    This file contains driver for color LCD.(CSTN)						  *
 ** Author:         Jim zhang                                                 *
 ** DATE:           03/01/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 02/04/2004     louis.wei	    Create.
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
#ifndef LCD_FLIP
#define LCD_FLIP
#endif

#define LCD_CtrlWrite_ILI9225B( _cmd )\
                LCD_SendCmd( (_cmd ), 0 );

#define LCD_DataWrite_ILI9225B( _data )\
                LCD_SendData( (_data), 0 );

#define LCD_CtrlData_ILI9225B( _cmd, _data ) \
                LCD_SendCmdData( (_cmd ),  (_data),0 );  

LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;	
				
/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ILI9225B_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9225B_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);



/*********************************************************************/
//  Description:   Initialize color LCD : ILI9225B
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E ILI9225B_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9225B_Invalidate(void);


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
LOCAL ERR_LCD_E ILI9225B_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	);
/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC void ILI9225B_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 right, 	// end Horizon address
	uint16 top, 		// start Vertical address
	uint16 bottom	// end Vertical address
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
//	Note: Application should correct invalidate direction right after current
//	 	  image displayed
/******************************************************************************/
LOCAL ERR_LCD_E ILI9225B_SetDirection( LCD_DIRECT_E direct_type)
{
    switch(direct_type)
    {
        case LCD_DIRECT_NORMAL:         
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0003, 0x1000);      
#else
            LCD_CtrlData_ILI9225B(0x0003, 0x1030); // set GRAM write direction and BGR=1.
#endif
            break;
            
        case LCD_DIRECT_ROT_90:
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0003, 0x1018);
#else
            LCD_CtrlData_ILI9225B(0x0003, 0x1028); // set GRAM write direction and BGR=1.
#endif
            break;
            
        case LCD_DIRECT_ROT_180:
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0003, 0x1030);
#else
            LCD_CtrlData_ILI9225B(0x0003, 0x1000); // set GRAM write direction and BGR=1.
#endif
            break;
            
        case LCD_DIRECT_ROT_270:
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0003, 0x1028);
#else
            LCD_CtrlData_ILI9225B(0x0003, 0x1018); // set GRAM write direction and BGR=1.
#endif
            break;

		case LCD_DIRECT_MIR_H:
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0003, 0x1010);
#else
                LCD_CtrlData_ILI9225B(0x0003, 0x1020); // set GRAM write direction and BGR=1.
#endif
			break;
			
		case LCD_DIRECT_MIR_V:
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0003, 0x1020);
#else
            LCD_CtrlData_ILI9225B(0x0003, 0x1010); // set GRAM write direction and BGR=1.
#endif
			break;
			
		case LCD_DIRECT_MIR_HV:
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0003, 0x1030);
#else
            LCD_CtrlData_ILI9225B(0x0003, 0x1000); // set GRAM write direction and BGR=1.
#endif
			break;
			
		default:			
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0003, 0x1000);
#else
            LCD_CtrlData_ILI9225B(0x0003, 0x1030); // set GRAM write direction and BGR=1.
#endif

            direct_type = LCD_DIRECT_NORMAL;
            break;
            
    }   
    s_lcd_direct = direct_type; 

    return ERR_LCD_NONE;
    
}


/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
__inline void ILI9225B_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 top, 	// end Horizon address
	uint16 right, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{   
#ifdef LCD_FLIP
    //uint16 cmd;   
    uint16 left_1;    // start Horizon address
    uint16 top_1;     // end Horizon address
    uint16 right_1;       // start Vertical address
    uint16 bottom_1;   // end Vertical address

    left_1 = QCIF_LCD_WIDTH - 1 - right;
    top_1 = QCIF_LCD_HEIGHT - 1 - bottom;
    right_1 = QCIF_LCD_WIDTH - 1 - left;
    bottom_1 = QCIF_LCD_HEIGHT - 1 - top;
    LCD_CtrlData_ILI9225B(0x0036, right_1);
    LCD_CtrlData_ILI9225B(0x0037, left_1);
    LCD_CtrlData_ILI9225B(0x0038, bottom_1);
    LCD_CtrlData_ILI9225B(0x0039, top_1);
#else
    LCD_CtrlData_ILI9225B(0x0036, right);
    LCD_CtrlData_ILI9225B(0x0037, left);
    LCD_CtrlData_ILI9225B(0x0038, bottom);
    LCD_CtrlData_ILI9225B(0x0039, top);
#endif
  
    switch(s_lcd_direct)
    {
        case LCD_DIRECT_NORMAL: 
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0020, right_1);
            LCD_CtrlData_ILI9225B(0x0021, bottom_1);
#else
            LCD_CtrlData_ILI9225B(0x0020, left);
            LCD_CtrlData_ILI9225B(0x0021, top);
#endif
            break;            
            
        case LCD_DIRECT_ROT_90:             
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0020, left_1);
            LCD_CtrlData_ILI9225B(0x0021, bottom_1);    
#else
            LCD_CtrlData_ILI9225B(0x0020, right);
            LCD_CtrlData_ILI9225B(0x0021, top);
#endif
            break;
            
        case LCD_DIRECT_ROT_180:
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0020, left_1);
            LCD_CtrlData_ILI9225B(0x0021, top_1);               
#else
            LCD_CtrlData_ILI9225B(0x0020, right);
            LCD_CtrlData_ILI9225B(0x0021, bottom);
#endif
            break;
            
        case LCD_DIRECT_ROT_270:
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0020, right_1);
            LCD_CtrlData_ILI9225B(0x0021, top_1);           
#else
            LCD_CtrlData_ILI9225B(0x0020, left);
            LCD_CtrlData_ILI9225B(0x0021, top);
#endif
            break;

        case LCD_DIRECT_MIR_H:
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0020, left_1);
            LCD_CtrlData_ILI9225B(0x0021, bottom_1);
#else
            LCD_CtrlData_ILI9225B(0x0020, right);
            LCD_CtrlData_ILI9225B(0x0021, top);
#endif
            break;

        case LCD_DIRECT_MIR_V:
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0020, right_1);
            LCD_CtrlData_ILI9225B(0x0021, top_1);
#else
            LCD_CtrlData_ILI9225B(0x0020, left);
            LCD_CtrlData_ILI9225B(0x0021, bottom);
#endif
            break;

        case LCD_DIRECT_MIR_HV:
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0020, left_1);
            LCD_CtrlData_ILI9225B(0x0021, top_1);
#else
            LCD_CtrlData_ILI9225B(0x0020, right);
            LCD_CtrlData_ILI9225B(0x0021, bottom);
#endif
            break;
            
        default:            
#ifdef LCD_FLIP
            LCD_CtrlData_ILI9225B(0x0020, right_1);
            LCD_CtrlData_ILI9225B(0x0021, bottom_1);
#else
            LCD_CtrlData_ILI9225B(0x0020, left);
            LCD_CtrlData_ILI9225B(0x0021, top);
#endif
            break;  
    
    }
    
    LCD_CtrlWrite_ILI9225B(0x0022); 

}


/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E ILI9225B_Init(void)
{
    // VCI=2.8V
    //************* Reset LCD Driver ****************//
    LCD_Reset();//reset lcd
    LCD_Delayms(40);

    //************* Start Initial Sequence **********//
    LCD_CtrlData_ILI9225B(0x0001, 0x011C); // set SS and NL bit
    LCD_CtrlData_ILI9225B(0x0002, 0x0100); // set 1 line inversion
#ifdef LCD_FLIP
    LCD_CtrlData_ILI9225B(0x0003, 0x1000); // set GRAM write direction and BGR=1.
#else
    LCD_CtrlData_ILI9225B(0x0003, 0x1030); // set GRAM write direction and BGR=1.
#endif
    LCD_CtrlData_ILI9225B(0x0008, 0x0808); // set BP and FP
    LCD_CtrlData_ILI9225B(0x000C, 0x0000); // RGB interface setting R0Ch=0x0110 for RGB 18Bit and R0Ch=0111for RGB16Bit

    LCD_CtrlData_ILI9225B(0x000F, 0x0B01); // Set frame rate
#ifdef LCD_FLIP
    LCD_CtrlData_ILI9225B(0x0020, 0x00AF); // Set GRAM Address
    LCD_CtrlData_ILI9225B(0x0021, 0x00DB); // Set GRAM Address
#else
    LCD_CtrlData_ILI9225B(0x0020, 0x0000); // Set GRAM Address
    LCD_CtrlData_ILI9225B(0x0021, 0x0000); // Set GRAM Address
#endif
    //*************Power On sequence ****************//

    LCD_Delayms(70); // Delay 50ms
    LCD_CtrlData_ILI9225B(0x0010, 0x0800); // Set SAP,DSTB,STB
    LCD_CtrlData_ILI9225B(0x0011, 0x1038); // Set APON,PON,AON,VCI1EN,VC
    LCD_Delayms(70); // Delay 50ms
    LCD_CtrlData_ILI9225B(0x0012, 0x1121); // Internal reference voltage= Vci;

    LCD_CtrlData_ILI9225B(0x0013, 0x0063); // Set GVDD
    LCD_CtrlData_ILI9225B(0x0014, 0x3205); // Set VCOMH/VCOML voltage//3944
    //------------- Set GRAM area ------------------//
    LCD_CtrlData_ILI9225B(0x0030, 0x0000);
    LCD_CtrlData_ILI9225B(0x0031, 0x00DB);
    LCD_CtrlData_ILI9225B(0x0032, 0x0000);
    LCD_CtrlData_ILI9225B(0x0033, 0x0000);
    LCD_CtrlData_ILI9225B(0x0034, 0x00DB);
    LCD_CtrlData_ILI9225B(0x0035, 0x0000);
    LCD_CtrlData_ILI9225B(0x0036, 0x00AF);
    LCD_CtrlData_ILI9225B(0x0037, 0x0000);
    LCD_CtrlData_ILI9225B(0x0038, 0x00DB);
    LCD_CtrlData_ILI9225B(0x0039, 0x0000);
    // ----------- Adjust the Gamma Curve ----------//

    LCD_CtrlData_ILI9225B(0x0050, 0x0003);
    LCD_CtrlData_ILI9225B(0x0051, 0x0900);
    LCD_CtrlData_ILI9225B(0x0052, 0x0d05);
    LCD_CtrlData_ILI9225B(0x0053, 0x0900);
    LCD_CtrlData_ILI9225B(0x0054, 0x0407);
    LCD_CtrlData_ILI9225B(0x0055, 0x0502);
    LCD_CtrlData_ILI9225B(0x0056, 0x0000);
    LCD_CtrlData_ILI9225B(0x0057, 0x0005);
    LCD_CtrlData_ILI9225B(0x0058, 0x1700);
    LCD_CtrlData_ILI9225B(0x0059, 0x001F);

    LCD_Delayms(50); // Delay 50ms
    LCD_CtrlData_ILI9225B(0x0007, 0x1017);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9225B_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"qinss LCD: in ILI9225B_EnterSleep, is_sleep = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9225_393_112_2_18_0_33_40_1772,(uint8*)"d", is_sleep);
	if ( is_sleep ) // enter sleep mode.
	{
        LCD_CtrlData_ILI9225B(0x0007, 0x0000); // Set D1=0, D0=1
        LCD_Delayms(50);
        LCD_CtrlData_ILI9225B(0x0011, 0x0007); // Set APON,PON,AON,VCI1EN,VC
        LCD_Delayms(50);
        LCD_CtrlData_ILI9225B(0x0010, 0x0801); // Enter Standby mode

	}
	else 			// out sleep mode 
	{
	 	ILI9225B_Init();
	}

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ILI9225B_Close(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in ILI9225B_Close"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9225_417_112_2_18_0_33_40_1773,(uint8*)"");

	//GPIO_SetLcdBackLight( SCI_FALSE );
	ILI9225B_EnterSleep( SCI_TRUE );	
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9225B_Invalidate(void)
{
	ILI9225B_set_display_window (0x0, 0X0, QCIF_LCD_WIDTH - 1, QCIF_LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E ILI9225B_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{
  	left 	= (left >= QCIF_LCD_WIDTH)    ? QCIF_LCD_WIDTH-1 : left;
	right 	= (right >= QCIF_LCD_WIDTH)   ? QCIF_LCD_WIDTH-1 : right;
	top 	= (top >= QCIF_LCD_HEIGHT)    ? QCIF_LCD_HEIGHT-1 : top;
	bottom 	= (bottom >= QCIF_LCD_HEIGHT) ? QCIF_LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
    	return ERR_LCD_OPERATE_FAIL;
    }     
   
    ILI9225B_set_display_window(left, top, right, bottom); 
    return ERR_LCD_NONE;
	
}


/**************************************************************************************/
// Description: refresh a rectangle of lcd
// Global resource dependence:
// Author: Jianping.wang
// Note:
//		left - the left value of the rectangel
//   	top - the top value of the rectangel
// 		right - the right value of the rectangel
//		bottom - the bottom value of the rectangel
/**************************************************************************************/
LOCAL ERR_LCD_E ILI9225B_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	//int32 error;
	

	switch(angle)
	{
		case LCD_ANGLE_0:
			ILI9225B_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			ILI9225B_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			ILI9225B_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			ILI9225B_set_display_window(left, top, bottom,right);
			break;
		default:
			ILI9225B_set_display_window(left, top, right, bottom);
			break;			
	}
		

	return ERR_LCD_NONE;
}//en of ILI9225B_VerticalInvalidateRect	


/******************************************************************************/
//  Description:  set the contrast value 
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   ILI9225B_SetContrast(
	uint16  contrast	//contrast value to set
	)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
} 


/*****************************************************************************/
//  Description:    Set the brightness of LCD.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/*****************************************************************************/
LOCAL ERR_LCD_E   ILI9225B_SetBrightness(
	uint16 brightness	//birghtness to set
	)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
}

/*****************************************************************************/
//  Description:    Enable lcd to partial display mode, so can save power.
//	Global resource dependence: 
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//	Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
LOCAL ERR_LCD_E ILI9225B_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	ILI9225B_set_display_window(left, top, right, bottom);
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
const LCD_OPERATIONS_T ILI9225B_operations = 
{
	ILI9225B_Init,
	ILI9225B_EnterSleep,
	ILI9225B_SetContrast,
	ILI9225B_SetBrightness,
	ILI9225B_SetDisplayWindow,
	ILI9225B_InvalidateRect,
	ILI9225B_Invalidate,
	ILI9225B_Close,
	ILI9225B_RotationInvalidateRect,
	ILI9225B_SetDirection,
    NULL,
    NULL
 
};

/******************************************************************************/
//  Description:  return the ILI9225B lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9225B_GetOperations(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in ILI9225B_GetOperations"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9225_567_112_2_18_0_33_40_1774,(uint8*)"");

	return (LCD_OPERATIONS_T*)&ILI9225B_operations;
}

PUBLIC BOOLEAN ILI9225B_Probe(void)
{
	return SCI_TRUE;	
}

LOCAL const LCD_TIMING_U s_ILI9225B_timing =
{

	        // LCM_CYCLE_U start(ns)
	5,		// CS setup time for LCM read (optional)  
	150,	// low pulse width for LCM read (according spec)	
	150,	// high pulse width for LCM read (according spec)
	10,		// CS setup time for LCM write  (optional) 	
	50,		// low pulse width for LCM write (according spec)
	50,	    // high pulse width for LCM write (according spec)	
	        // LCM_CYCLE_U end(ns)
};

const LCD_SPEC_T g_lcd_ILI9225B[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16, 
	&s_ILI9225B_timing, 
	(LCD_OPERATIONS_T*)&ILI9225B_operations,
	0,
	0
	}
};

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 



