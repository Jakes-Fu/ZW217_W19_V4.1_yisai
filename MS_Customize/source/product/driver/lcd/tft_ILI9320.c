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

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif



#define LCD_CtrlWrite_ILI9320( _cmd )\
				LCD_SendCmd( (_cmd ), 0 );

#define LCD_DataWrite_ILI9320( _data )\
				LCD_SendData( (_data), 0 );

#define LCD_CtrlData_ILI9320( _cmd, _data ) \
				LCD_SendCmdData( (_cmd ),  (_data),0 );  
				
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;	
			
/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ILI9320_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9320_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);

/*********************************************************************/
//  Description:   Initialize color LCD : ILI9320
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E ILI9320_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9320_Invalidate(void);

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
LOCAL ERR_LCD_E ILI9320_InvalidateRect(
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
PUBLIC void ILI9320_set_display_window(
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
LOCAL ERR_LCD_E ILI9320_SetDirection(LCD_DIRECT_E direct_type)
{
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:			
		    LCD_CtrlWrite_ILI9320(0x0003);		    
		    LCD_DataWrite_ILI9320(0x1030);
			break;
			
		case LCD_DIRECT_ROT_90:
			LCD_CtrlWrite_ILI9320(0x0003);		    
		    LCD_DataWrite_ILI9320(0x1028);	 
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlWrite_ILI9320(0x0003);		    
		    LCD_DataWrite_ILI9320(0x1000);	 
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlWrite_ILI9320(0x0003);		    
		    LCD_DataWrite_ILI9320(0x1018);
			break;

		case LCD_DIRECT_MIR_H:
			LCD_CtrlWrite_ILI9320(0x0003);		    
		    LCD_DataWrite_ILI9320(0x1020);
			break;
			
		case LCD_DIRECT_MIR_V:
			LCD_CtrlWrite_ILI9320(0x0003);		    
		    LCD_DataWrite_ILI9320(0x1010);
			break;
			
		case LCD_DIRECT_MIR_HV:
			LCD_CtrlWrite_ILI9320(0x0003);		    
		    LCD_DataWrite_ILI9320(0x1000);
			break;
			
		default:			
			LCD_CtrlWrite_ILI9320(0x0003);		    
		    LCD_DataWrite_ILI9320(0x1030);	
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
__inline void ILI9320_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 top, 	// end Horizon address
	uint16 right, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{   
	LCD_CtrlData_ILI9320(0x0050, left);
	LCD_CtrlData_ILI9320(0x0051, right);
	LCD_CtrlData_ILI9320(0x0052, top);
	LCD_CtrlData_ILI9320(0x0053, bottom);	
	
	switch(s_lcd_direct)
	{
		case LCD_DIRECT_NORMAL:	
		    LCD_CtrlData_ILI9320(0x0020, left);
			LCD_CtrlData_ILI9320(0x0021, top);
			break;            
			
		case LCD_DIRECT_ROT_90:				
			LCD_CtrlData_ILI9320(0x0020, right);
			LCD_CtrlData_ILI9320(0x0021, top);	
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlData_ILI9320(0x0020, right);
			LCD_CtrlData_ILI9320(0x0021, bottom);				
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlData_ILI9320(0x0020, left);
			LCD_CtrlData_ILI9320(0x0021, top);			
			break;

		case LCD_DIRECT_MIR_H:
			LCD_CtrlData_ILI9320(0x0020, right);
			LCD_CtrlData_ILI9320(0x0021, top);
			break;

		case LCD_DIRECT_MIR_V:
			LCD_CtrlData_ILI9320(0x0020, left);
			LCD_CtrlData_ILI9320(0x0021, bottom);
			break;

		case LCD_DIRECT_MIR_HV:
			LCD_CtrlData_ILI9320(0x0020, right);
			LCD_CtrlData_ILI9320(0x0021, bottom);
			break;
			
		default:			
		    LCD_CtrlData_ILI9320(0x0020, left);
			LCD_CtrlData_ILI9320(0x0021, top);
			break;	
	
	}

	LCD_CtrlWrite_ILI9320(0x0022);

}


/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E ILI9320_Init(void)
{

	//SCI_TRACE_LOW:"qinss LCD: in ILI9320_reset"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9320_233_112_2_18_0_33_42_1778,(uint8*)"");
	
    LCD_Reset();//reset lcd throught lcdc	
    LCD_Delayms(100);

    //ILI9320_sendcommand1(format16t18(0xe5),format16t18(0x8000));	
    LCD_CtrlWrite_ILI9320(0xe5);
	LCD_DataWrite_ILI9320(0x8000);
	//ILI9320_sendcommand1(format16t18(0x00),format16t18(0x0001));
	LCD_CtrlWrite_ILI9320(0x00);
	LCD_DataWrite_ILI9320(0x0001);	
	//ILI9320_sendcommand1(format16t18(0x01),format16t18(0x0100)); // drive output control	
	LCD_CtrlWrite_ILI9320(0x01);
	LCD_DataWrite_ILI9320(0x0100);
	//ILI9320_sendcommand1(format16t18(0x02),format16t18(0x0700)); //lcd driving wave control	
	LCD_CtrlWrite_ILI9320(0x02);
	LCD_DataWrite_ILI9320(0x0700);
	//ILI9320_sendcommand1(format16t18(0x03),format16t18(0x1030));	
	LCD_CtrlWrite_ILI9320(0x03);
	LCD_DataWrite_ILI9320(0x1030);
	//ILI9320_sendcommand1(format16t18(0x04),format16t18(0x0000));	
	LCD_CtrlWrite_ILI9320(0x04);
	LCD_DataWrite_ILI9320(0x0000);
	//ILI9320_sendcommand1(format16t18(0x08),format16t18(0x0202));	
	LCD_CtrlWrite_ILI9320(0x08);
	LCD_DataWrite_ILI9320(0x0202);
	//ILI9320_sendcommand1(format16t18(0x09),format16t18(0x0000));	
	LCD_CtrlWrite_ILI9320(0x09);
	LCD_DataWrite_ILI9320(0x0000);
	//ILI9320_sendcommand1(format16t18(0x0a),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x0a);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x0c), format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x0c);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x0d),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x0d);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x0f),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x0f);
	LCD_DataWrite_ILI9320(0x0000);   
	//ILI9320_sendcommand1(format16t18(0x10),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x10);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x11),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x11);
	LCD_DataWrite_ILI9320(0x0000);
	//ILI9320_sendcommand1(format16t18(0x12),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x12);
	LCD_DataWrite_ILI9320(0x0000);
	//ILI9320_sendcommand1(format16t18(0x13),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x13);
	LCD_DataWrite_ILI9320(0x0000);
    LCD_Delayms(100);
	//ILI9320_sendcommand1(format16t18(0x10),format16t18(0x17b0));
	LCD_CtrlWrite_ILI9320(0x10);
	LCD_DataWrite_ILI9320(0x17b0);	
	//ILI9320_sendcommand1(format16t18(0x11),format16t18(0x0137));
	LCD_CtrlWrite_ILI9320(0x11);
	LCD_DataWrite_ILI9320(0x0137);	
    LCD_Delayms(50);
	//ILI9320_sendcommand1(format16t18(0x12),format16t18(0x0139));
	LCD_CtrlWrite_ILI9320(0x12);
	LCD_DataWrite_ILI9320(0x0139);	
    LCD_Delayms(50);
	//ILI9320_sendcommand1(format16t18(0x13),format16t18(0x1c00));
	LCD_CtrlWrite_ILI9320(0x13);
	LCD_DataWrite_ILI9320(0x1c00);	
	//ILI9320_sendcommand1(format16t18(0x29),format16t18(0x0013));
	LCD_CtrlWrite_ILI9320(0x29);
	LCD_DataWrite_ILI9320(0x0013);	
    LCD_Delayms(50);
	//ILI9320_sendcommand1(format16t18(0x20),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x20);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x21),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x21);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x30),format16t18(0x0006));
	LCD_CtrlWrite_ILI9320(0x30);
	LCD_DataWrite_ILI9320(0x0006);	
	//ILI9320_sendcommand1(format16t18(0x31),format16t18(0x0101));
	LCD_CtrlWrite_ILI9320(0x31);
	LCD_DataWrite_ILI9320(0x0101);	
	//ILI9320_sendcommand1(format16t18(0x32),format16t18(0x0003));
	LCD_CtrlWrite_ILI9320(0x32);
	LCD_DataWrite_ILI9320(0x0003);	
	//ILI9320_sendcommand1(format16t18(0x35),format16t18(0x0106));
	LCD_CtrlWrite_ILI9320(0x35);
	LCD_DataWrite_ILI9320(0x0106);	
	//ILI9320_sendcommand1(format16t18(0x36),format16t18(0x0b02));
	LCD_CtrlWrite_ILI9320(0x36);
	LCD_DataWrite_ILI9320(0x0b02);	
	//ILI9320_sendcommand1(format16t18(0x37),format16t18(0x0302));
	LCD_CtrlWrite_ILI9320(0x37);
	LCD_DataWrite_ILI9320(0x0302);	
	//ILI9320_sendcommand1(format16t18(0x38),format16t18(0x0707));
	LCD_CtrlWrite_ILI9320(0x38);
	LCD_DataWrite_ILI9320(0x0707);	
	//ILI9320_sendcommand1(format16t18(0x39),format16t18(0x0007));
	LCD_CtrlWrite_ILI9320(0x39);
	LCD_DataWrite_ILI9320(0x0007);	
	//ILI9320_sendcommand1(format16t18(0x3c),format16t18(0x0600));
	LCD_CtrlWrite_ILI9320(0x3c);
	LCD_DataWrite_ILI9320(0x0600);	
	//ILI9320_sendcommand1(format16t18(0x3d),format16t18(0x020b));
	LCD_CtrlWrite_ILI9320(0x3d);
	LCD_DataWrite_ILI9320(0x020b);
    //ILI9320_sendcommand1(format16t18(0x50),format16t18(0x0000));
    LCD_CtrlWrite_ILI9320(0x50);
	LCD_DataWrite_ILI9320(0x0000);                                                                    
    //ILI9320_sendcommand1(format16t18(0x51),format16t18(0x00EF));  
    LCD_CtrlWrite_ILI9320(0x51);
	LCD_DataWrite_ILI9320(0x00EF);                               
    //ILI9320_sendcommand1(format16t18(0x52),format16t18(0x0000));  
    LCD_CtrlWrite_ILI9320(0x52);
	LCD_DataWrite_ILI9320(0x0000);                                                                      
    //ILI9320_sendcommand1(format16t18(0x53),format16t18(0x013F)); 
    LCD_CtrlWrite_ILI9320(0x53);
	LCD_DataWrite_ILI9320(0x013F);
	//ILI9320_sendcommand1(format16t18(0x60),format16t18(0x2700));
	LCD_CtrlWrite_ILI9320(0x60);
	LCD_DataWrite_ILI9320(0x2700);	
	//ILI9320_sendcommand1(format16t18(0x61),format16t18(0x0001));
	LCD_CtrlWrite_ILI9320(0x61);
	LCD_DataWrite_ILI9320(0x0001);	
	//ILI9320_sendcommand1(format16t18(0x6a),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x6a);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x80),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x80);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x81),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x81);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x82),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x82);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x83),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x83);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x84),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x84);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x85),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x85);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x90),format16t18(0x0010));
	LCD_CtrlWrite_ILI9320(0x90);
	LCD_DataWrite_ILI9320(0x0010);	
	//ILI9320_sendcommand1(format16t18(0x92),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x92);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x93),format16t18(0x0003));
	LCD_CtrlWrite_ILI9320(0x93);
	LCD_DataWrite_ILI9320(0x0003);	
	//ILI9320_sendcommand1(format16t18(0x95),format16t18(0x0110));
	LCD_CtrlWrite_ILI9320(0x95);
	LCD_DataWrite_ILI9320(0x0110);	
	//ILI9320_sendcommand1(format16t18(0x97),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x97);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x98),format16t18(0x0000));
	LCD_CtrlWrite_ILI9320(0x98);
	LCD_DataWrite_ILI9320(0x0000);	
	//ILI9320_sendcommand1(format16t18(0x07),format16t18(0x0173));
	LCD_CtrlWrite_ILI9320(0x07);
	LCD_DataWrite_ILI9320(0x0173);	
	//ILI9320_SEND_COMMAND(0x22);
	LCD_CtrlWrite_ILI9320(0x22);

	//SCI_TRACE_LOW:"qinss LCD: in ILI9320_reset end."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9320_404_112_2_18_0_33_43_1779,(uint8*)"");
	return ERR_LCD_NONE;
}



/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  ILI9320_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"qinss LCD: in ILI9320_EnterSleep, is_sleep = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9320_418_112_2_18_0_33_43_1780,(uint8*)"d", is_sleep);
	if(is_sleep)
	{
		//display off
        //ILI9320_sendcommand1(format16t18(0x0007),format16t18(0x0000));	
        LCD_CtrlWrite_ILI9320(0x0007);
	    LCD_DataWrite_ILI9320(0x0000);
        //ILI9320_sendcommand1(format16t18(0x0010),format16t18(0x0010));	
        LCD_CtrlWrite_ILI9320(0x0010);
	    LCD_DataWrite_ILI9320(0x0000);
        //ILI9320_sendcommand1(format16t18(0x0011),format16t18(0x0000));
        LCD_CtrlWrite_ILI9320(0x0011);
	    LCD_DataWrite_ILI9320(0x0000);
        //ILI9320_sendcommand1(format16t18(0x0012),format16t18(0x0000));
        LCD_CtrlWrite_ILI9320(0x0012);
	    LCD_DataWrite_ILI9320(0x0000);
        //ILI9320_sendcommand1(format16t18(0x0013),format16t18(0x0000));	
        LCD_CtrlWrite_ILI9320(0x0013);
	    LCD_DataWrite_ILI9320(0x0000);
        LCD_Delayms(200);
        //ILI9320_sendcommand1(format16t18(0x0010),format16t18(0x0002));	
        LCD_CtrlWrite_ILI9320(0x0010);
	    LCD_DataWrite_ILI9320(0x0002);
	}
	else
	{
		//ILI9320_sendcommand1(format16t18(0x0010),format16t18(0x0000));	
		LCD_CtrlWrite_ILI9320(0x0010);
	    LCD_DataWrite_ILI9320(0x0000);
		//ILI9320_sendcommand1(format16t18(0x0011),format16t18(0x0000));	
		LCD_CtrlWrite_ILI9320(0x0011);
	    LCD_DataWrite_ILI9320(0x0000);
		//ILI9320_sendcommand1(format16t18(0x0012),format16t18(0x0000));	
		LCD_CtrlWrite_ILI9320(0x0012);
	    LCD_DataWrite_ILI9320(0x0000);
		//ILI9320_sendcommand1(format16t18(0x0013),format16t18(0x0000));
        LCD_CtrlWrite_ILI9320(0x0013);
	    LCD_DataWrite_ILI9320(0x0000);
        LCD_Delayms(200);

		//display on	
		//ILI9320_sendcommand1(format16t18(0x0010),format16t18(0x17b0));	
		LCD_CtrlWrite_ILI9320(0x0010);
	    LCD_DataWrite_ILI9320(0x17b0);
		//ILI9320_sendcommand1(format16t18(0x0011),format16t18(0x0137));	
		LCD_CtrlWrite_ILI9320(0x0011);
	    LCD_DataWrite_ILI9320(0x0137);
		LCD_Delayms(50);
		//ILI9320_sendcommand1(format16t18(0x0012),format16t18(0x0139));	
        LCD_CtrlWrite_ILI9320(0x0012);
	    LCD_DataWrite_ILI9320(0x0139);
        LCD_Delayms(50);
		//ILI9320_sendcommand1(format16t18(0x0013),format16t18(0x1c00));	
		LCD_CtrlWrite_ILI9320(0x0013);
	    LCD_DataWrite_ILI9320(0x1c00);
		//ILI9320_sendcommand1(format16t18(0x0029),format16t18(0x0013));		
		LCD_Delayms(50);
		LCD_CtrlWrite_ILI9320(0x0029);
	    LCD_DataWrite_ILI9320(0x0013);
		//ILI9320_sendcommand1(format16t18(0x0007),format16t18(0x0173));	
		LCD_CtrlWrite_ILI9320(0x0007);
	    LCD_DataWrite_ILI9320(0x0173);
		LCD_Delayms(100);

//		ILI9320_SEND_COMMAND(0x0022); 			// send data.

		ILI9320_Invalidate();
		
	}


	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void ILI9320_Close(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in ILI9320_Close"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9320_498_112_2_18_0_33_43_1781,(uint8*)"");

	//GPIO_SetLcdBackLight( SCI_FALSE );
	ILI9320_EnterSleep( SCI_TRUE );	
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E ILI9320_Invalidate(void)
{

	//ILI9320_set_display_window (0x0, ILI9320_WIDTH - 1, 0x0, ILI9320_HEIGHT -1);
	ILI9320_set_display_window (0x0, 0X0, QVGA_LCD_WIDTH - 1, QVGA_LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E ILI9320_InvalidateRect(
	uint16 left, 	//the left value of the rectangel
	uint16 top, 	//top of the rectangle
	uint16 right, 	//right of the rectangle
	uint16 bottom	//bottom of the rectangle
	)
{
  	left 	= (left >= QVGA_LCD_WIDTH)    ? QVGA_LCD_WIDTH-1 : left;
	right 	= (right >= QVGA_LCD_WIDTH)   ? QVGA_LCD_WIDTH-1 : right;
	top 	= (top >= QVGA_LCD_HEIGHT)    ? QVGA_LCD_HEIGHT-1 : top;
	bottom 	= (bottom >= QVGA_LCD_HEIGHT) ? QVGA_LCD_HEIGHT-1 : bottom;
    
    if ( ( right < left ) || ( bottom < top ) )
    {
    	return ERR_LCD_OPERATE_FAIL;
    }     
   
    ILI9320_set_display_window(left, top, right, bottom); 
    //ILI9320_set_display_window(left, right, top, bottom); 
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
LOCAL ERR_LCD_E ILI9320_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	switch(angle)
	{
		case LCD_ANGLE_0:
			ILI9320_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			ILI9320_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			ILI9320_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			ILI9320_set_display_window(left, top, bottom,right);
			break;
		default:
			ILI9320_set_display_window(left, top, right, bottom);
			break;			
	}
		

	return ERR_LCD_NONE;
}//en of S6D0139_VerticalInvalidateRect	

/******************************************************************************/
//  Description:  set the contrast value 
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E   ILI9320_SetContrast(
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
LOCAL ERR_LCD_E   ILI9320_SetBrightness(
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
LOCAL ERR_LCD_E ILI9320_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	 	
	ILI9320_set_display_window(left, top, right, bottom);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL const LCD_OPERATIONS_T ILI9320_operations = 
{
	ILI9320_Init,
	ILI9320_EnterSleep,
	ILI9320_SetContrast,
	ILI9320_SetBrightness,
	ILI9320_SetDisplayWindow,
	ILI9320_InvalidateRect,
	ILI9320_Invalidate,
	ILI9320_Close,
	ILI9320_RotationInvalidateRect,
	ILI9320_SetDirection,
    NULL,
	NULL
};

/******************************************************************************/
//  Description:  return the ILI9320 lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9320_GetOperations(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in ILI9320_GetOperations"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_ILI9320_648_112_2_18_0_33_43_1782,(uint8*)"");

	return (LCD_OPERATIONS_T*)&ILI9320_operations;//PULL;
}

PUBLIC BOOLEAN ILI9320_Probe(void)
{
	return SCI_TRUE;	
}

LOCAL const LCD_TIMING_U s_ILI9320_timing =
{

	        // LCM_CYCLE_U start(ns)
	10,		// CS setup time for LCM read (optional)  
	150,	// low pulse width for LCM read (according spec)	
	100,	// high pulse width for LCM read (according spec)
	30,		// CS setup time for LCM write  (optional) 	
	60,		// low pulse width for LCM write (according spec)
	70,	    // high pulse width for LCM write (according spec)	
	        // LCM_CYCLE_U end(ns)
};


const LCD_SPEC_T g_lcd_ILI9320 = {
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_16,
	(LCD_TIMING_U *)&s_ILI9320_timing, 
	(LCD_OPERATIONS_T*)&ILI9320_operations,
	0,
	0
};

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 


