/******************************************************************************
 ** File Name:     cstn_pcf8833.c                                             *
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


#define  L_YG

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif



#define LCD_CtrlWrite_R61509V( _cmd )\
				LCD_SendCmd( (_cmd ), 0 );

#define LCD_DataWrite_R61509V( _data )\
				LCD_SendData( (_data), 0 );

#define LCD_CtrlData_R61509V( _cmd, _data ) \
				LCD_SendCmdData( (_cmd ),  (_data),0 );  
			

LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;	

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void R61509V_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  R61509V_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);



/*********************************************************************/
//  Description:   Initialize color LCD : R61509V
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E R61509V_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E R61509V_Invalidate(void);


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
LOCAL ERR_LCD_E R61509V_InvalidateRect(
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
PUBLIC void R61509V_set_display_window(
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
LOCAL ERR_LCD_E R61509V_SetDirection( LCD_DIRECT_E direct_type)
{
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:			
		    LCD_CtrlData_R61509V(0x0003, 0x1030);		
			break;
			
		case LCD_DIRECT_ROT_90:
			LCD_CtrlData_R61509V(0x0003, 0x1028);
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlData_R61509V(0x0003, 0x1000);
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlData_R61509V(0x0003, 0x1018);
			break;

		case LCD_DIRECT_MIR_H:
			LCD_CtrlData_R61509V(0x0003, 0x1020);
			break;
			
		case LCD_DIRECT_MIR_V:
			LCD_CtrlData_R61509V(0x0003, 0x1010);
			break;
			
		case LCD_DIRECT_MIR_HV:
			LCD_CtrlData_R61509V(0x0003, 0x1000);
			break;
			
		default:			
			LCD_CtrlData_R61509V(0x0003, 0x1030);

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
__inline void R61509V_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 top, 	// end Horizon address
	uint16 right, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{   
	//uint16 cmd;	

	LCD_CtrlData_R61509V(0x0210, left);
	LCD_CtrlData_R61509V(0x0211, right);
	LCD_CtrlData_R61509V(0x0212, top);
	LCD_CtrlData_R61509V(0x0213, bottom);

	switch(s_lcd_direct)
	{
		case LCD_DIRECT_NORMAL:	
		    LCD_CtrlData_R61509V(0x0200, left);
			LCD_CtrlData_R61509V(0x0201, top);
			break;            
			
		case LCD_DIRECT_ROT_90:				
			LCD_CtrlData_R61509V(0x0200, right);
			LCD_CtrlData_R61509V(0x0201, top);	
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlData_R61509V(0x0200, right);
			LCD_CtrlData_R61509V(0x0201, bottom);				
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlData_R61509V(0x0200, left);
			LCD_CtrlData_R61509V(0x0201, top);			
			break;

		case LCD_DIRECT_MIR_H:
			LCD_CtrlData_R61509V(0x0200, right);
			LCD_CtrlData_R61509V(0x0201, top);
			break;

		case LCD_DIRECT_MIR_V:
			LCD_CtrlData_R61509V(0x0200, left);
			LCD_CtrlData_R61509V(0x0201, bottom);
			break;

		case LCD_DIRECT_MIR_HV:
			LCD_CtrlData_R61509V(0x0200, right);
			LCD_CtrlData_R61509V(0x0201, bottom);
			break;
			
		default:			
			LCD_CtrlData_R61509V(0x0200, left);
			LCD_CtrlData_R61509V(0x0201, top);
			break;	
	
	}
	
	LCD_CtrlWrite_R61509V(0x0202);	

}


/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E R61509V_Init(void)
{

	//SCI_TRACE_LOW:"qinss LCD: in R61509V_reset"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_R61509V_237_112_2_18_0_34_2_1847,(uint8*)"");

	LCD_Reset();//reset lcd throught lcdc	
	LCD_Delayms(50);

 
	LCD_CtrlWrite_R61509V(0x0000); 
	LCD_CtrlWrite_R61509V(0x0000); 
	LCD_CtrlWrite_R61509V(0x0000); 
	LCD_CtrlWrite_R61509V(0x0000); 

	LCD_CtrlData_R61509V(0x0600,0x0001);         
	LCD_Delayms(10);
	LCD_CtrlData_R61509V(0x0600,0x0000);        
	LCD_Delayms(10);

	LCD_Delayms(10);

	LCD_CtrlData_R61509V(0x00008,0x0808);                                                                                                                         
	LCD_CtrlData_R61509V(0x0010,0x0017);                                           
	LCD_CtrlData_R61509V(0x0400,0x6200);         

	//*******GAMMA *********
	LCD_CtrlData_R61509V(0x0300,0x0703);                                                                                                                         
	LCD_CtrlData_R61509V(0x0301,0x7811);                                           
	LCD_CtrlData_R61509V(0x0302,0x0D06);                                          
	LCD_CtrlData_R61509V(0x0303,0x060E);     
	LCD_CtrlData_R61509V(0x0304,0x1111);                                              
	LCD_CtrlData_R61509V(0x0305,0x0B06);

	LCD_CtrlData_R61509V(0x0306,0x7509);
	LCD_CtrlData_R61509V(0x0307,0x0201);
	LCD_CtrlData_R61509V(0x0308,0x0307);
	LCD_CtrlData_R61509V(0x0309,0x1111);	
	/*
	LCD_CtrlData_R61509V(0x0300,0x0C06);                                                                                                                         
	LCD_CtrlData_R61509V(0x0301,0x9D0F);                                           
	LCD_CtrlData_R61509V(0x0302,0x0B05);                                          
	LCD_CtrlData_R61509V(0x0303,0x1217);     
	LCD_CtrlData_R61509V(0x0304,0x3333);                                              
	LCD_CtrlData_R61509V(0x0305,0x1712);

	LCD_CtrlData_R61509V(0x0306,0x950B);
	LCD_CtrlData_R61509V(0x0307,0x0F0D);
	LCD_CtrlData_R61509V(0x0308,0x060C);
	LCD_CtrlData_R61509V(0x0309,0x0000);
	*/

	LCD_CtrlData_R61509V(0x0007,0x0001);
	LCD_CtrlData_R61509V(0x0100,0x0730);
	LCD_CtrlData_R61509V(0x0101,0x0237);
	LCD_CtrlData_R61509V(0x0103,0x0C00);
	LCD_CtrlData_R61509V(0x0280,0x4600);
	LCD_CtrlData_R61509V(0x0102,0x81B0);	

	/*    LCD_CtrlData_R61509V(0x0100,0x0230);
	LCD_CtrlData_R61509V(0x0008,0x020E);
	LCD_CtrlData_R61509V(0x0280,0xB9FF);
	LCD_CtrlData_R61509V(0x0210,0x0000);
	LCD_CtrlData_R61509V(0x0211,0x00EF);
	LCD_CtrlData_R61509V(0x0212,0x0000);
	//   *****LCD param********
	LCD_CtrlData_R61509V(0x0213,0x018F);
	LCD_CtrlData_R61509V(0x0200,0x0000);
	LCD_CtrlData_R61509V(0x0201,0x0000); //1030
	LCD_CtrlData_R61509V(0x0401,0x0001);

	*/

	LCD_Delayms(100);

	LCD_CtrlData_R61509V(0x0001,0x0100);
	LCD_CtrlData_R61509V(0x0002,0x0100);
	LCD_CtrlData_R61509V(0x0003,0x1030);

	LCD_CtrlData_R61509V(0x0009,0x0001);
	LCD_CtrlData_R61509V(0x000C,0x0000);
	LCD_CtrlData_R61509V(0x0003,0x1030);	

	LCD_CtrlData_R61509V(0x0011,0x0202);
	LCD_CtrlData_R61509V(0x0012,0x0707);
	LCD_CtrlData_R61509V(0x0013,0x0001);	
	LCD_CtrlData_R61509V(0x0090,0x8000);

	LCD_CtrlData_R61509V(0x0210,0x0000);
	LCD_CtrlData_R61509V(0x0211,0x00EF);
	LCD_CtrlData_R61509V(0x0212,0x0000);
	//*****LCD param********
	LCD_CtrlData_R61509V(0x0213,0x018F);
	LCD_CtrlData_R61509V(0x0200,0x0000);
	LCD_CtrlData_R61509V(0x0201,0x0000); //1030
	LCD_CtrlData_R61509V(0x0401,0x0001);
	LCD_CtrlData_R61509V(0x0404,0x0000);

	//  LCD_CtrlData_R61509V(0x0103,0x0d00);
	//  LCD_CtrlData_R61509V(0x0102,0xc1b0);
	//  LCD_CtrlData_R61509V(0x0007,0x0100);

	LCD_Delayms(40);
	LCD_CtrlData_R61509V(0x0007,0x0100);    

	LCD_Delayms(40);
 
	return ERR_LCD_NONE;
}


/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  R61509V_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"qinss LCD: in R61509V_EnterSleep, is_sleep = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_R61509V_352_112_2_18_0_34_2_1848,(uint8*)"d", is_sleep);
	if ( is_sleep ) // enter sleep mode.
	{
		LCD_CtrlData_R61509V(0x0007,0x0000);
		LCD_CtrlData_R61509V(0x0100,0x0734);
		LCD_Delayms(20);
		LCD_CtrlData_R61509V(0x0102,0x0180);
		LCD_Delayms(20);
	}
	else 			// out sleep mode 
	{
	 	R61509V_Init();
	}

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void R61509V_Close(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in R61509V_Close"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_R61509V_375_112_2_18_0_34_2_1849,(uint8*)"");

	//GPIO_SetLcdBackLight( SCI_FALSE );
	R61509V_EnterSleep( SCI_TRUE );	
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E R61509V_Invalidate(void)
{
	R61509V_set_display_window (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E R61509V_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{
  	left 	= (left >= LCD_WIDTH)    ? LCD_WIDTH-1 : left;
	right 	= (right >= LCD_WIDTH)   ? LCD_WIDTH-1 : right;
	top 	= (top >= LCD_HEIGHT)    ? LCD_HEIGHT-1 : top;
	bottom 	= (bottom >= LCD_HEIGHT) ? LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
    	return ERR_LCD_OPERATE_FAIL;
    }     
   
    R61509V_set_display_window(left, top, right, bottom); 
  
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
LOCAL ERR_LCD_E R61509V_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	switch(angle)
	{
		case LCD_ANGLE_0:
			R61509V_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			R61509V_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			R61509V_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			R61509V_set_display_window(left, top, bottom,right);
			break;
		default:
			R61509V_set_display_window(left, top, right, bottom);
			break;			
	}
		

	return ERR_LCD_NONE;
}//en of R61509V_VerticalInvalidateRect	


/******************************************************************************/
//  Description:  set the contrast value 
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   R61509V_SetContrast(
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
LOCAL ERR_LCD_E   R61509V_SetBrightness(
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
LOCAL ERR_LCD_E R61509V_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	 	
	R61509V_set_display_window(left, top, right, bottom);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL const LCD_OPERATIONS_T R61509V_operations = 
{
	R61509V_Init,
	R61509V_EnterSleep,
	R61509V_SetContrast,
	R61509V_SetBrightness,
	R61509V_SetDisplayWindow,
	R61509V_InvalidateRect,
	R61509V_Invalidate,
	R61509V_Close,
	R61509V_RotationInvalidateRect,
	R61509V_SetDirection,
    NULL,
    NULL
};

/******************************************************************************/
//  Description:  return the R61509V lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* R61509V_GetOperations(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in R61509V_GetOperations"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_R61509V_524_112_2_18_0_34_2_1850,(uint8*)"");

	return (LCD_OPERATIONS_T*)&R61509V_operations;
}

PUBLIC BOOLEAN R61509V_Probe(void)
{
	return SCI_TRUE;	
}

LOCAL const LCD_TIMING_U s_R61509V_timing =
{

	        // LCM_CYCLE_U start(ns)
	5,		// CS setup time for LCM read (optional)  
	150,	// low pulse width for LCM read (according spec)	
	150,	// high pulse width for LCM read (according spec)
	10,		// CS setup time for LCM write  (optional) 	
	30,		// low pulse width for LCM write (according spec)
	25,	    // high pulse width for LCM write (according spec)	
	        // LCM_CYCLE_U end(ns)
};


const LCD_SPEC_T g_lcd_R61509V = {
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U*)&s_R61509V_timing, 
	(LCD_OPERATIONS_T*)&R61509V_operations,
	0,
	0
};

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 


