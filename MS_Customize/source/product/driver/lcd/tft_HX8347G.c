/******************************************************************************
 ** File Name:     TFT_HX8347G.c                                             *
 ** Description:															  *
 **    This file contains driver for color LCD.(CSTN)						  *
 ** Author:         xuechunguang                                              *
 ** DATE:           06/09/2010                                                *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/09/2010     xuechunguang	    Create.
 ******************************************************************************/

#include "ms_customize_trc.h"
#include "os_api.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"

    
//#define  L_YG

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif
#define HX8347G_LCD_CtrlWrite( _cmd )\
					LCD_SendCmd( (_cmd )>>8, 0);\
					LCD_SendCmd( (_cmd ), 0);
	
#define HX8347G_LCD_DataWrite( _data )\
					LCD_SendData( (_data)>>8, 0);\
					LCD_SendData( (_data), 0 );
	
#define HX8347G_LCD_CtrlData( _cmd, _data ) \
					LCD_SendCmd( (_cmd )>>8, 0 );\
					LCD_SendCmd( (_cmd ), 0); \
					LCD_SendData( (_data)>>8, 0);\
					LCD_SendData( (_data), 0 );

#if 0
#define LCD_CtrlWrite_HX8347G( _cmd )\
				LCD_SendCmd( (_cmd ), 0 );

#define LCD_DataWrite_HX8347G( _data )\
				LCD_SendData( (_data), 0 );

#define LCD_CtrlData_HX8347G( _cmd, _data ) \
				LCD_SendCmdData( (_cmd ),  (_data),0 );  
				
#else
#define LCD_CtrlWrite_HX8347G( _cmd )\
				HX8347G_LCD_CtrlWrite(_cmd );

#define LCD_DataWrite_HX8347G( _data )\
				HX8347G_LCD_DataWrite(_data);

#define LCD_CtrlData_HX8347G( _cmd, _data ) \
				HX8347G_LCD_CtrlData(_cmd ,  _data);  

				
#endif
#define LCD_Delay( ticks)\
				OS_TickDelay( ticks );
				
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;

LOCAL BOOLEAN g_is_invert = FALSE ;

  /******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void HX8347G_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8347G_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);
/*********************************************************************/
//  Description:   Initialize color LCD : HX8347G
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E HX8347G_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8347G_Invalidate(void);


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
LOCAL ERR_LCD_E HX8347G_InvalidateRect(
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
LOCAL void HX8347G_set_display_window(
	uint16 left, 	
	uint16 top, 	
	uint16 right, 		
	uint16 bottom	
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
LOCAL ERR_LCD_E HX8347G_SetDirection(
	LCD_DIRECT_E direct_type
	)
{
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:			
		    LCD_CtrlWrite_HX8347G(0x0016);		    
		    LCD_DataWrite_HX8347G(0x0000);//(0x00c8);
		    //LCD_CtrlData_HX8347G(0x0016,0x0000);
			break;
			
		case LCD_DIRECT_ROT_90:
			LCD_CtrlWrite_HX8347G(0x0016);//(0x0003);		    
		    LCD_DataWrite_HX8347G(0x00a0);//(0x1018);	 
		    //LCD_CtrlData_HX8347G(0x0016,0x00a0);
			break;
			
		case LCD_DIRECT_ROT_180:
			LCD_CtrlWrite_HX8347G(0x0016);//(0x0003);		    
		    LCD_DataWrite_HX8347G(0x00c0);//(0x1000);	 
		    //LCD_CtrlData_HX8347G(0x0016,0x00c0);
			break;
			
		case LCD_DIRECT_ROT_270:
			LCD_CtrlWrite_HX8347G(0x0016);//(0x0016);		    
		    LCD_DataWrite_HX8347G(0x0060);//(0x00a8);
		    //LCD_CtrlData_HX8347G(0x0016,0x0060);
			break;

		case LCD_DIRECT_MIR_H:

			break;
			
		case LCD_DIRECT_MIR_V:

			break;
			
		case LCD_DIRECT_MIR_HV:

			break;
			
		default:			
			LCD_CtrlWrite_HX8347G(0x0016);		    
		    LCD_DataWrite_HX8347G(0x0000);//(0x00c8);
		    //LCD_CtrlData_HX8347G(0x0016,0x0000);
		    direct_type = LCD_DIRECT_NORMAL;
			break;
			
	}	

	s_lcd_direct = direct_type;

    //SCI_TRACE_LOW:"HX8347G set direction,direct_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347G_193_112_2_18_0_33_33_1759,(uint8*)"d",direct_type);
    
	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void HX8347G_set_display_window(
	uint16 left, 	
	uint16 top, 	
	uint16 right, 		
	uint16 bottom	
	)
{   
	uint16 cmd;	
	uint16 left_temp = 0;
	uint16 right_temp = 0;
	uint16 top_temp = 0 ;
	uint16 bottom_temp = 0 ;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  由于这个IC的特性,以及LCM走线的关系,在反转的时候,需要把X ,Y反
//  过来送 -----Vincent  20071031 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   if(g_is_invert)
   {
        left_temp = left;
        top_temp = top;
        right_temp = right;
        bottom_temp = bottom ;

        left = top_temp ;
        top = left_temp ;
        right = bottom_temp;
        bottom = right_temp;             
   }
#if 1   
    //SCI_TRACE_LOW:"HX8347G set dis window,s_lcd_direct=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347G_232_112_2_18_0_33_33_1760,(uint8*)"d",s_lcd_direct);
	switch(s_lcd_direct)//switch(s_angle)
	{
		case LCD_DIRECT_NORMAL:		    
			LCD_CtrlData_HX8347G(0x0002,0x0000); // Column address start 2
			LCD_CtrlData_HX8347G(0x0003,left); // Column address start 1
			LCD_CtrlData_HX8347G(0x0004,0x0000); // Column address end 2
			LCD_CtrlData_HX8347G(0x0005,right); // Column address end 1
			
			LCD_CtrlData_HX8347G(0x0006,(top>>8)&0xff); // Row address start 2
			LCD_CtrlData_HX8347G(0x0007,top&0x00ff); // Row address start 1
			LCD_CtrlData_HX8347G(0x0008,(bottom>>8)&0xff); // Row address end 2
			LCD_CtrlData_HX8347G(0x0009,bottom&0x00ff); // Row address end 1

			LCD_CtrlWrite_HX8347G(0x0022);
			break;
			
		case LCD_DIRECT_ROT_90:		
			
			break;
			
		case LCD_DIRECT_ROT_180:
			
			break;
			
		case LCD_DIRECT_ROT_270:
			//left
			cmd = ((left>>8)&0x01);
			LCD_CtrlWrite_HX8347G(0x0002);
			LCD_DataWrite_HX8347G(cmd);
			cmd = (left&0xff);
			LCD_CtrlWrite_HX8347G(0x0003);
			LCD_DataWrite_HX8347G(cmd) ;
			
			//right
			cmd = ((right>>8)&0x01);
			LCD_CtrlWrite_HX8347G(0x0004);
			LCD_DataWrite_HX8347G(cmd);
			cmd = (right&0xff);
			LCD_CtrlWrite_HX8347G(0x0005);
			LCD_DataWrite_HX8347G(cmd) ;
			
			//top
			cmd = ((top>>8)&0x01);
			LCD_CtrlWrite_HX8347G(0x0006);
			LCD_DataWrite_HX8347G(cmd);
			cmd = (top&0xff);
			LCD_CtrlWrite_HX8347G(0x0007);
			LCD_DataWrite_HX8347G(cmd) ;
			
			//bottom
			cmd = ((bottom>>8)&0x01);
			LCD_CtrlWrite_HX8347G(0x0008);
			LCD_DataWrite_HX8347G(cmd);
			cmd = (bottom&0xff);
			LCD_CtrlWrite_HX8347G(0x0009);
			LCD_DataWrite_HX8347G(cmd) ;

			LCD_CtrlWrite_HX8347G(0x0022);
			
			break;
			
		default:
			LCD_CtrlData_HX8347G(0x0002,0x0000); // Column address start 2
			LCD_CtrlData_HX8347G(0x0003,left); // Column address start 1
			LCD_CtrlData_HX8347G(0x0004,0x0000); // Column address end 2
			LCD_CtrlData_HX8347G(0x0005,right); // Column address end 1
			LCD_CtrlData_HX8347G(0x0006,(top>>8)&0xff); // Row address start 2
			LCD_CtrlData_HX8347G(0x0007,top&0x00ff); // Row address start 1
			LCD_CtrlData_HX8347G(0x0008,(bottom>>8)&0xff); // Row address end 2
			LCD_CtrlData_HX8347G(0x0009,bottom&0x00ff); // Row address end 1

			LCD_CtrlWrite_HX8347G(0x0022);
			break;	
	}
#endif
}


LOCAL void HX8347G_driver(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in HX8347G_reset"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347G_313_112_2_18_0_33_33_1761,(uint8*)"");
	
    LCD_Reset();//reset lcd throught lcdc	
    LCD_Delayms(50);
#if 0 // by riyang.xie
	//************* Detect Module IC's ID **********//
	LCD_CtrlWrite_HX8347G(0x0);
	lcm_dev_id = LCDC_Read_Ex(0, 1);	//0x00
	//SCI_TRACE_LOW:"\"\"\"#LCD: in HX8347G_Init ID(0x0)=,0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347G_321_112_2_18_0_33_33_1762,(uint8*)"d",lcm_dev_id);
#endif

#if defined (FP880XG2_DUALSIM_PROJECT) // by riyang.xie
	LCD_CtrlData_HX8347G(0xEA,0x00);   //PTBA[15:8] 
	LCD_CtrlData_HX8347G(0xEB,0x20);   //PTBA[7:0] 
	LCD_CtrlData_HX8347G(0xEC,0x3C);   //STBA[15:8] 
	LCD_CtrlData_HX8347G(0xED,0xC4);   //STBA[7:0] 
	LCD_CtrlData_HX8347G(0xE8,0x48);   //OPON[7:0] 
	LCD_CtrlData_HX8347G(0xE9,0x38);   //OPON1[7:0]
	LCD_CtrlData_HX8347G(0xF1,0x01);   //OTPS1B 
	LCD_CtrlData_HX8347G(0xF2,0x08);   //GEN 
	
	//after modified
	//Gamma 2.2Setting
	LCD_CtrlData_HX8347G(0x40,0x01);
	LCD_CtrlData_HX8347G(0x41,0x15);
	LCD_CtrlData_HX8347G(0x42,0x14);
	LCD_CtrlData_HX8347G(0x43,0x30);
	LCD_CtrlData_HX8347G(0x44,0x30);
	LCD_CtrlData_HX8347G(0x45,0x3c);
	LCD_CtrlData_HX8347G(0x46,0x10);
	LCD_CtrlData_HX8347G(0x47,0x6e);
	LCD_CtrlData_HX8347G(0x48,0x07);
	LCD_CtrlData_HX8347G(0x49,0x0f);
	LCD_CtrlData_HX8347G(0x4A,0x12);
	LCD_CtrlData_HX8347G(0x4B,0x12);
	LCD_CtrlData_HX8347G(0x4C,0x19);
	
	LCD_CtrlData_HX8347G(0x50,0x03);
	LCD_CtrlData_HX8347G(0x51,0x0f);
	LCD_CtrlData_HX8347G(0x52,0x0f);
	LCD_CtrlData_HX8347G(0x53,0x2b);
	LCD_CtrlData_HX8347G(0x54,0x2a);
	LCD_CtrlData_HX8347G(0x55,0x3E);
	LCD_CtrlData_HX8347G(0x56,0x11);
	LCD_CtrlData_HX8347G(0x57,0x67);
	LCD_CtrlData_HX8347G(0x58,0x06);
	LCD_CtrlData_HX8347G(0x59,0x0d);
	LCD_CtrlData_HX8347G(0x5A,0x0d);
	LCD_CtrlData_HX8347G(0x5B,0x10);
	LCD_CtrlData_HX8347G(0x5C,0x18);
	LCD_CtrlData_HX8347G(0x5D,0xCC);
	
	// after modified
	//Power Voltage Setting
	
	LCD_CtrlData_HX8347G(0x1B,0x1B);  //VRH=4.65V
	LCD_CtrlData_HX8347G(0x1A,0x01);  //BT (VGH~15V,VGL~-10V,DDVDH~5V)
	LCD_CtrlData_HX8347G(0x24,0x60);  //VMH(VCOM High voltage ~4.2V)
	LCD_CtrlData_HX8347G(0x25,0x57);  //VML(VCOM Low voltage -1.2V)
	//****VCOM offset**///
	LCD_CtrlData_HX8347G(0x23,0x7a); //for Flicker adjust //can reload from OTP 
	//LCD_CtrlData_HX8347G(0x16,0x00);
	/////////////////////////////     
		
	//Power on Setting
	LCD_CtrlData_HX8347G(0x18,0x36);  //I/P_RADJ,N/P_RADJ,Normal mode 70Hz
	LCD_CtrlData_HX8347G(0x19,0x01);  //OSC_EN='1', start Osc
	LCD_CtrlData_HX8347G(0x01,0x00);  //DP_STB='0', out deep sleep
	LCD_CtrlData_HX8347G(0x1F,0x88); 
	LCD_Delayms(5);
	LCD_CtrlData_HX8347G(0x1F,0x80);
	LCD_Delayms(5);
	LCD_CtrlData_HX8347G(0x1F,0x90);
	LCD_Delayms(5);
	LCD_CtrlData_HX8347G(0x1F,0xD0);
	LCD_Delayms(5);
	
	//262k/65k color selection 
	LCD_CtrlData_HX8347G(0x17,0x05);  //default 0x06 262k color // 0x05 65k color 
	
	//SET PANEL
	LCD_CtrlData_HX8347G(0x36,0x09);
	
	//Display ON Setting 
	LCD_CtrlData_HX8347G(0x28,0x38);
	LCD_Delayms(40) ;  
	LCD_CtrlData_HX8347G(0x28,0x3F);
	
	//Set GRAM Area 
	LCD_CtrlData_HX8347G(0x02,0x00); 
	LCD_CtrlData_HX8347G(0x03,0x00); //Column Start 
	LCD_CtrlData_HX8347G(0x04,0x00); 
	LCD_CtrlData_HX8347G(0x05,0xEF); //Column End 
	
	LCD_CtrlData_HX8347G(0x06,0x00); 
	LCD_CtrlData_HX8347G(0x07,0x00); //Row Start 
	LCD_CtrlData_HX8347G(0x08,0x01); 
	LCD_CtrlData_HX8347G(0x09,0x3F); //Row End 


#else
    LCD_CtrlData_HX8347G(0x00EA,0x0000);
    LCD_CtrlData_HX8347G(0x00EB,0x0020);
    LCD_CtrlData_HX8347G(0x00EC,0x007C);
    LCD_CtrlData_HX8347G(0x00ED,0x00C4);
    LCD_CtrlData_HX8347G(0x00E8,0x0048);
    LCD_CtrlData_HX8347G(0x00E9,0x0038);
    LCD_CtrlData_HX8347G(0x00F1,0x0001);
    LCD_CtrlData_HX8347G(0x00F2,0x0008);

    LCD_CtrlData_HX8347G(0x0040,0x0001);
    LCD_CtrlData_HX8347G(0x0041,0x0007);
    LCD_CtrlData_HX8347G(0x0042,0x0009);
    LCD_CtrlData_HX8347G(0x0043,0x0018);
    LCD_CtrlData_HX8347G(0x0044,0x0015);
    LCD_CtrlData_HX8347G(0x0045,0x002F);
    LCD_CtrlData_HX8347G(0x0046,0x0013);
    LCD_CtrlData_HX8347G(0x0047,0x0062);
    LCD_CtrlData_HX8347G(0x0048,0x0004);
    LCD_CtrlData_HX8347G(0x0049,0x0015);
    LCD_CtrlData_HX8347G(0x004A,0x0019);
    LCD_CtrlData_HX8347G(0x004B,0x0019);
    LCD_CtrlData_HX8347G(0x004C,0x0018);
    
    LCD_CtrlData_HX8347G(0x0050,0x0010);
    LCD_CtrlData_HX8347G(0x0051,0x002A);
    LCD_CtrlData_HX8347G(0x0052,0x0027);
    LCD_CtrlData_HX8347G(0x0053,0x0036);
    LCD_CtrlData_HX8347G(0x0054,0x0038);
    LCD_CtrlData_HX8347G(0x0055,0x003E);
    LCD_CtrlData_HX8347G(0x0056,0x001D);
    LCD_CtrlData_HX8347G(0x0057,0x006C);
    LCD_CtrlData_HX8347G(0x0058,0x0007);
    LCD_CtrlData_HX8347G(0x0059,0x0006);
    LCD_CtrlData_HX8347G(0x005A,0x0006);
    LCD_CtrlData_HX8347G(0x005B,0x000A);
    LCD_CtrlData_HX8347G(0x005C,0x001B);
    LCD_CtrlData_HX8347G(0x005D,0x00CC);
    
    LCD_CtrlData_HX8347G(0x001B,0x001B);
    LCD_CtrlData_HX8347G(0x001A,0x0001);
    LCD_CtrlData_HX8347G(0x0024,0x0060);
    LCD_CtrlData_HX8347G(0x0025,0x0054);
    LCD_CtrlData_HX8347G(0x0023,0x0076);
    LCD_CtrlData_HX8347G(0x0018,0x0036);
    LCD_CtrlData_HX8347G(0x0019,0x0001);
    LCD_CtrlData_HX8347G(0x0001,0x0000);
    LCD_CtrlData_HX8347G(0x001F,0x0088);
	LCD_Delayms(5);
    LCD_CtrlData_HX8347G(0x001F,0x0080);
	LCD_Delayms(5);
    LCD_CtrlData_HX8347G(0x001F,0x0090);
	LCD_Delayms(5);
    LCD_CtrlData_HX8347G(0x001F,0x00D0);
	LCD_Delayms(5);
    LCD_CtrlData_HX8347G(0x0017,0x0005);
    LCD_CtrlData_HX8347G(0x0036,0x0009);
    LCD_CtrlData_HX8347G(0x0028,0x0038);
	LCD_Delayms(40);
    LCD_CtrlData_HX8347G(0x0028,0x003F);
    
    LCD_CtrlData_HX8347G(0x00E1,0x004F);
    
    LCD_CtrlData_HX8347G(0x0002,0x0000);
    LCD_CtrlData_HX8347G(0x0003,0x0000);
    LCD_CtrlData_HX8347G(0x0004,0x0000);
    LCD_CtrlData_HX8347G(0x0005,0x00EF);
    LCD_CtrlData_HX8347G(0x0006,0x0000);
    LCD_CtrlData_HX8347G(0x0007,0x0000);
    LCD_CtrlData_HX8347G(0x0008,0x0001);
    LCD_CtrlData_HX8347G(0x0009,0x003F);
#endif
    //LCD_CtrlWrite_HX8347G(0x0022);

	//SCI_TRACE_LOW:"qinss LCD: in HX8347G_reset end."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347G_487_112_2_18_0_33_33_1763,(uint8*)"");
	
	return;
}


/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E HX8347G_Init(void)
{
	//SCI_TRACE_LOW:"HX8347G_Init"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347G_499_112_2_18_0_33_33_1764,(uint8*)"");
 
	HX8347G_driver();	
	
	return 0;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8347G_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{
	//SCI_TRACE_LOW:"qinss LCD: in HX8347G_EnterSleep, is_sleep = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347G_514_112_2_18_0_33_33_1765,(uint8*)"d", is_sleep);
	
	if ( is_sleep ) // enter sleep mode.
	{
	    LCD_CtrlData_HX8347G(0x28,0x38); //GON=’1’ DTE=’1’ D[1:0]=’10’       
        LCD_Delayms(40);        
        LCD_CtrlData_HX8347G(0x1F,0x89); // GAS=1, VOMG=00, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=1        
        LCD_Delayms(40);        
        LCD_CtrlData_HX8347G(0x28,0x20); //GON=’1’ DTE=’0’ D[1:0]=’00’        
        LCD_Delayms(40);        
        LCD_CtrlData_HX8347G(0x19,0x00); //OSC_EN=’0’        
        LCD_Delayms(5);	
	}
	else 			// out sleep mode 
	{
		HX8347G_Init();
	}

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void HX8347G_Close(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in HX8347G_Close"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347G_541_112_2_18_0_33_33_1766,(uint8*)"");

	//GPIO_SetLcdBackLight( SCI_FALSE );
	HX8347G_EnterSleep( SCI_TRUE );	
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8347G_Invalidate(void)
{

	HX8347G_set_display_window (0x0, 0X0, QVGA_LCD_WIDTH - 1, QVGA_LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E HX8347G_InvalidateRect(
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
   
    HX8347G_set_display_window(left, top, right, bottom); 
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
LOCAL ERR_LCD_E HX8347G_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	//int32 error;
	

	switch(angle)
	{
		case LCD_ANGLE_0:
			HX8347G_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			HX8347G_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			HX8347G_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			HX8347G_set_display_window(left, top, bottom,right);
			break;
		default:
			HX8347G_set_display_window(left, top, right, bottom);
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
LOCAL ERR_LCD_E   HX8347G_SetContrast(
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
LOCAL ERR_LCD_E   HX8347G_SetBrightness(
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
LOCAL ERR_LCD_E HX8347G_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	 	
	HX8347G_set_display_window(left, top, right, bottom);

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
const LCD_OPERATIONS_T HX8347G_operations = 
{
	HX8347G_Init,
	HX8347G_EnterSleep,
	HX8347G_SetContrast,
	HX8347G_SetBrightness,
	HX8347G_SetDisplayWindow,
	HX8347G_InvalidateRect,
	HX8347G_Invalidate,
	HX8347G_Close,
	NULL,
	HX8347G_SetDirection,
    NULL,
    NULL
};

/******************************************************************************/
//  Description:  return the HX8347G lcd driver funtion pointer
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* HX8347G_GetOperations(void)
{
	//SCI_TRACE_LOW:"qinss LCD: in HX8347G_GetOperations"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_HX8347G_692_112_2_18_0_33_34_1767,(uint8*)"");

	return (LCD_OPERATIONS_T*)&HX8347G_operations;
}

PUBLIC BOOLEAN HX8347G_Probe(void)
{
	return SCI_TRUE;	
}

LOCAL const LCD_TIMING_U s_HX8347G_timing =
{

	        // LCM_CYCLE_U start(ns)
	10,		// CS setup time for LCM read (optional)  
	150,	// low pulse width for LCM read (according spec)	
	100,	// high pulse width for LCM read (according spec)
#if 0 // by riyang.xie
	30,		// CS setup time for LCM write  (optional) 	
	50,		// low pulse width for LCM write (according spec)
	70,	    // high pulse width for LCM write (according spec)	
#else
	10,		// CS setup time for LCM write  (optional) 	
	35,		// low pulse width for LCM write (according spec)
	35,	    // high pulse width for LCM write (according spec)	
#endif
	        // LCM_CYCLE_U end(ns)
};


PUBLIC const LCD_SPEC_T g_lcd_HX8347G[] = {
{
	LCD_WIDTH, 
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
	WIDTH_8,
	(LCD_TIMING_U*)&s_HX8347G_timing, 
	(LCD_OPERATIONS_T*)&HX8347G_operations,
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


