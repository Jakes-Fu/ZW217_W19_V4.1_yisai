//##!(important notice: do not remove this line)
/******************************************************************************
 ** File Name:     TFT_HX8347D.c                                             *
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

#include "os_api.h"
#include "spload_main.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"
#include "ref_outport.h"

    
#define  L_YG

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#ifdef LCD_DATA_WIDTH_8BIT

#define LCD_CtrlWrite_HX8347D( _cmd ) /*lint -e572 -e778*/\
				s_lcm_func_table->lcd_sendcmd( ((_cmd )>>8), 0 );\
				s_lcm_func_table->lcd_sendcmd( ((_cmd )&0xff), 0 );

#define LCD_DataWrite_HX8347D( _data ) /*lint -e572 -e778*/\
				 s_lcm_func_table->lcd_senddata( ((_data)>>8), 0 );\
				 s_lcm_func_table->lcd_senddata( ((_data)&0xff), 0 );

#define LCD_CtrlData_HX8347D( _cmd, _data ) /*lint -e572 -e778*/\
				s_lcm_func_table->lcd_sendcmd( ((_cmd )>>8), 0 );\
				s_lcm_func_table->lcd_sendcmd( ((_cmd )&0xff), 0 );\
				 s_lcm_func_table->lcd_senddata( ((_data)>>8), 0 );\
				 s_lcm_func_table->lcd_senddata( ((_data)&0xff), 0 );

#else //LCM_16BITS
#define LCD_CtrlWrite_HX8347D( _cmd )\
				s_lcm_func_table->lcd_sendcmd( (_cmd ), 0 );

#define LCD_DataWrite_HX8347D( _data )\
				 s_lcm_func_table->lcd_senddata( (_data), 0 );

#define LCD_CtrlData_HX8347D( _cmd, _data ) \
				s_lcm_func_table->lcd_sendcmdData( (_cmd ),  (_data),0 );

#endif //LCM_8BITS

#define LCD_Delay( ticks)\
				s_lcm_func_table->lcd_delayms( ticks );
				
LOCAL LCD_DIRECT_E s_lcd_direct = LCD_DIRECT_NORMAL;

  /******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void HX8347D_Close(void);

  /******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8347D_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: exter sleep mode;SCI_FALSE:exit sleep mode.
	);

/*********************************************************************/
//  Description:   Initialize color LCD : HX8347
//  Input:
//      None.
//  Return:
//      None.
//	Note:           
/*********************************************************************/
LOCAL ERR_LCD_E HX8347D_Init(void);

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8347D_Invalidate(void);


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
LOCAL ERR_LCD_E HX8347D_InvalidateRect(
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
PUBLIC void HX8347D_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 right, 	// end Horizon address
	uint16 top, 		// start Vertical address
	uint16 bottom	// end Vertical address
	);

/******************************************************************************/
//  Description:  Set LCD invalidate direction
//  Input:
//      is_invert: 0, horizontal; 1, vertical
//  Return:
//      None.
//	Note: Application should correct invalidate direction right after current
//	 	  image displayed
/******************************************************************************/
LOCAL ERR_LCD_E HX8347D_SetDirection(
	LCD_DIRECT_E direct_type
	)
{
	switch(direct_type)
	{
		case LCD_DIRECT_NORMAL:
			LCD_CtrlWrite_HX8347D(0x0016);
			LCD_DataWrite_HX8347D(0x0008);	
			break;
		case LCD_DIRECT_ROT_90:
			LCD_CtrlWrite_HX8347D(0x0016);
			LCD_DataWrite_HX8347D(0x0068);
			break;
		case LCD_DIRECT_ROT_180:
			LCD_CtrlWrite_HX8347D(0x0016);
			LCD_DataWrite_HX8347D(0x00C8);
			break;
		case LCD_DIRECT_ROT_270:
			LCD_CtrlWrite_HX8347D(0x0016);
			LCD_DataWrite_HX8347D(0x00A8);
			break;
		case LCD_DIRECT_MIR_H:
			LCD_CtrlWrite_HX8347D(0x0016);
			LCD_DataWrite_HX8347D(0x0048);
			break;
		case LCD_DIRECT_MIR_V:
			LCD_CtrlWrite_HX8347D(0x0016);
			LCD_DataWrite_HX8347D(0x0088);
			break;
		case LCD_DIRECT_MIR_HV:
			LCD_CtrlWrite_HX8347D(0x0016);
			LCD_DataWrite_HX8347D(0x0028);
			break;
		default:
			LCD_CtrlWrite_HX8347D(0x0016);
			LCD_DataWrite_HX8347D(0x0008);
			break;
	}
	s_lcd_direct = direct_type;
}

/******************************************************************************/
//  Description:   Set the windows address to display, in this windows
//                 color is  refreshed.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
__inline void HX8347D_set_display_window(
	uint16 left, 	// start Horizon address
	uint16 top, 	// end Horizon address
	uint16 right, 		// start Vertical address
	uint16 bottom	// end Vertical address
	)
{   
	uint16 newtop = top;
	uint16 newbottom = bottom;
    
	if(s_lcd_direct == LCD_DIRECT_ROT_180)
	{
		newtop = LCD_HEIGHT- 1 - bottom;
		newbottom = LCD_HEIGHT - 1- top;         
	}
    
	LCD_CtrlData_HX8347D(0x0002,(left>>8)&0xff); //Column address start 2
	LCD_CtrlData_HX8347D(0x0003,left&0x00ff); // Column address start 1
	LCD_CtrlData_HX8347D(0x0004,(right>>8)&0xff); // Column address end 2
	LCD_CtrlData_HX8347D(0x0005,right&0x00ff); // Column address end 1

	LCD_CtrlData_HX8347D(0x0006,(newtop>>8)&0xff); // Row address start 2
	LCD_CtrlData_HX8347D(0x0007,newtop&0x00ff); // Row address start 1
	LCD_CtrlData_HX8347D(0x0008,(newbottom>>8)&0xff); // Row address end 2
	LCD_CtrlData_HX8347D(0x0009,newbottom&0x00ff); // Row address end 1

	LCD_CtrlWrite_HX8347D(0x0022);
}


LOCAL void HX8347D_driver(void)
{
#ifndef	MAINLCD_PHY_ANGLE_180

	LCD_CtrlData_HX8347D(0x00EA,0x0000);//PTBA[15:8]
	LCD_CtrlData_HX8347D(0x00EB,0x0020);//PTBA[7:0]
	LCD_CtrlData_HX8347D(0x00EC,0x000C);//STBA[15:8]
	LCD_CtrlData_HX8347D(0x00ED,0x00C4);//STBA[7:0]
	LCD_CtrlData_HX8347D(0x00E8,0x0038);//OPON[7:0]
	LCD_CtrlData_HX8347D(0x00E9,0x0038);//10//OPON1[7:0]
	LCD_CtrlData_HX8347D(0x00F1,0x0001);//OTPS1B
	LCD_CtrlData_HX8347D(0x00F2,0x0010);//GEN
	LCD_CtrlData_HX8347D(0x0027,0x00A3);

	LCD_CtrlData_HX8347D(0x0040,0x0000);//设置gamma值
	LCD_CtrlData_HX8347D(0x0041,0x0000);
	LCD_CtrlData_HX8347D(0x0042,0x0001);
	LCD_CtrlData_HX8347D(0x0043,0x0012);
	LCD_CtrlData_HX8347D(0x0044,0x0010);
	LCD_CtrlData_HX8347D(0x0045,0x0026);
	LCD_CtrlData_HX8347D(0x0046,0x0008);
	LCD_CtrlData_HX8347D(0x0047,0x0054);
	LCD_CtrlData_HX8347D(0x0048,0x0002);
	LCD_CtrlData_HX8347D(0x0049,0x0015);
	LCD_CtrlData_HX8347D(0x004a,0x0019);
	LCD_CtrlData_HX8347D(0x004b,0x0019);
	LCD_CtrlData_HX8347D(0x004c,0x0016);

	LCD_CtrlData_HX8347D(0x0050,0x0019);
	LCD_CtrlData_HX8347D(0x0051,0x002f);
	LCD_CtrlData_HX8347D(0x0052,0x002d);
	LCD_CtrlData_HX8347D(0x0053,0x003e);
	LCD_CtrlData_HX8347D(0x0054,0x003f);
	LCD_CtrlData_HX8347D(0x0055,0x003f);
	LCD_CtrlData_HX8347D(0x0056,0x002b);
	LCD_CtrlData_HX8347D(0x0057,0x0077);
	LCD_CtrlData_HX8347D(0x0058,0x0009);
	LCD_CtrlData_HX8347D(0x0059,0x0006);
	LCD_CtrlData_HX8347D(0x005a,0x0006);
	LCD_CtrlData_HX8347D(0x005b,0x000a);
	LCD_CtrlData_HX8347D(0x005c,0x001d);
	LCD_CtrlData_HX8347D(0x005d,0x00cc);
	
	LCD_CtrlData_HX8347D(0x001b,0x001B);//VRH=4.65V
	LCD_CtrlData_HX8347D(0x001A,0x0001);//BT (VGH~15V,VGL~-10V,DDVDH~5V)
	LCD_CtrlData_HX8347D(0x0024,0x0039);//VMH(VCOM High voltage ~3.2V) 
	LCD_CtrlData_HX8347D(0x0025,0x006E);//VML(VCOM Low voltage -1.2V) 
	LCD_CtrlData_HX8347D(0x0023,0x007a);//for Flicker adjust //can reload from OTP 

	LCD_CtrlData_HX8347D(0x0018,0x0036); //I/P_RADJ,N/P_RADJ, Normal mode 75Hz
	LCD_CtrlData_HX8347D(0x0019,0x0001); //OSC_EN='1', start Osc
	LCD_CtrlData_HX8347D(0x0001,0x0000); //DP_STB='0', out deep sleep
	LCD_CtrlData_HX8347D(0x001F,0x0088); //GAS=1, VOMG=00, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=0
	LCD_Delay(5);
	LCD_CtrlData_HX8347D(0x001F,0x0080); //GAS=1, VOMG=00, PON=0, DK=0, XDK=0, DVDH_TRI=0, STB=0
	LCD_Delay(5);
	LCD_CtrlData_HX8347D(0x001F,0x0090); //GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0
	LCD_Delay(5);
	LCD_CtrlData_HX8347D(0x001F,0x00D0); //GAS=1, VOMG=10, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
	LCD_Delay(5);
	LCD_CtrlData_HX8347D(0x0017,0x0005); //default 0x60 262k color // 0x50 65k color

	LCD_CtrlData_HX8347D(0x0036,0x0000); //SS_P, GS_P,REV_P,BGR_P

	LCD_CtrlData_HX8347D(0x0028,0x0038); //GON=1, DTE=1, D=1000
	LCD_Delay(40);
	LCD_CtrlData_HX8347D(0x0028,0x003C); //GON=1, DTE=1, D=1100

	LCD_CtrlData_HX8347D(0x0002,0x0000); 
	LCD_CtrlData_HX8347D(0x0003,0x0000); //Column Start
	LCD_CtrlData_HX8347D(0x0004,0x0000); 
	LCD_CtrlData_HX8347D(0x0005,0x00EF); //Column End

	LCD_CtrlData_HX8347D(0x0006,0x0000);
	LCD_CtrlData_HX8347D(0x0007,0x0000); //Row Start
	LCD_CtrlData_HX8347D(0x0008,0x0001); 
	LCD_CtrlData_HX8347D(0x0009,0x003F); //Row End

	LCD_CtrlWrite_HX8347D(0x0022);//start GRAM write

	LCD_CtrlData_HX8347D(0x003C,0x00F0); //The backlight PWM pulse output duty is equal to (DBV[7:0]+1)/256 x CABC_duty.
	LCD_CtrlData_HX8347D(0x003D,0x002C); //BCTRL-d5; DD-d3; BL-d2; 1 ON; 0 OFF
	LCD_CtrlData_HX8347D(0x003E,0x0000); //C[1:0]选择节电方式
	LCD_CtrlData_HX8347D(0x003F,0x0000); //set the minimum brightness value of the display for CABC function.


#else	/*main lcd phy angle = 180*/
	LCD_CtrlData_HX8347D(0xEA,0x00); //PTBA[15:8]
	LCD_CtrlData_HX8347D(0xEB,0x20); //PTBA[7:0]
	LCD_CtrlData_HX8347D(0xEC,0x0C); //STBA[15:8]
	LCD_CtrlData_HX8347D(0xED,0xC4); //STBA[7:0]
	LCD_CtrlData_HX8347D(0xE8,0x40); //OPON[7:0]
	LCD_CtrlData_HX8347D(0xE9,0x38); //OPON1[7:0]
	LCD_CtrlData_HX8347D(0xF1,0x01); //OTPS1B
	LCD_CtrlData_HX8347D(0xF2,0x10); //GEN
	LCD_CtrlData_HX8347D(0x27,0xA3); //
	//Gamma 2.2 Setting
	LCD_CtrlData_HX8347D(0x40,0x00); //
	LCD_CtrlData_HX8347D(0x41,0x00); //
	LCD_CtrlData_HX8347D(0x42,0x01); //
	LCD_CtrlData_HX8347D(0x43,0x13); //
	LCD_CtrlData_HX8347D(0x44,0x10); //
	LCD_CtrlData_HX8347D(0x45,0x26); //
	LCD_CtrlData_HX8347D(0x46,0x08); //
	LCD_CtrlData_HX8347D(0x47,0x51); //
	LCD_CtrlData_HX8347D(0x48,0x02); //
	LCD_CtrlData_HX8347D(0x49,0x12); //
	LCD_CtrlData_HX8347D(0x4A,0x18); //
	LCD_CtrlData_HX8347D(0x4B,0x19); //
	LCD_CtrlData_HX8347D(0x4C,0x14); //
	LCD_CtrlData_HX8347D(0x50,0x19); //
	LCD_CtrlData_HX8347D(0x51,0x2F); //
	LCD_CtrlData_HX8347D(0x52,0x2C); //
	LCD_CtrlData_HX8347D(0x53,0x3E); //
	LCD_CtrlData_HX8347D(0x54,0x3F); //
	LCD_CtrlData_HX8347D(0x55,0x3F); //
	LCD_CtrlData_HX8347D(0x56,0x2E); //
	LCD_CtrlData_HX8347D(0x57,0x77); //
	LCD_CtrlData_HX8347D(0x58,0x0B); //
	LCD_CtrlData_HX8347D(0x59,0x06); //
	LCD_CtrlData_HX8347D(0x5A,0x07); //
	LCD_CtrlData_HX8347D(0x5B,0x0D); //
	LCD_CtrlData_HX8347D(0x5C,0x1D); //
	LCD_CtrlData_HX8347D(0x5D,0xCC); //
	//Power Voltage Setting
	LCD_CtrlData_HX8347D(0x1B,0x1B); //VRH=4.65V
	LCD_CtrlData_HX8347D(0x1A,0x01); //BT (VGH~15V,VGL~-10V,DDVDH~5V)
	LCD_CtrlData_HX8347D(0x24,0x36); //VMH(VCOM High voltage ~3.2V) 0x2F
	LCD_CtrlData_HX8347D(0x25,0x74); //VML(VCOM Low voltage -1.2V)0x57
	//****VCOM offset**///
	LCD_CtrlData_HX8347D(0x23,0x86); //for Flicker adjust //can reload from OTP
	//Power on Setting
	LCD_CtrlData_HX8347D(0x18,0x36); //I/P_RADJ,N/P_RADJ, Normal mode 75Hz
	LCD_CtrlData_HX8347D(0x19,0x01); //OSC_EN='1', start Osc
	LCD_CtrlData_HX8347D(0x01,0x00); //DP_STB='0', out deep sleep
	LCD_CtrlData_HX8347D(0x1F,0x88);// GAS=1, VOMG=00, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=0
	s_lcm_func_table->lcd_delayms(5);
	LCD_CtrlData_HX8347D(0x1F,0x80);// GAS=1, VOMG=00, PON=0, DK=0, XDK=0, DVDH_TRI=0, STB=0
	s_lcm_func_table->lcd_delayms(5);
	LCD_CtrlData_HX8347D(0x1F,0x90);// GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0
	s_lcm_func_table->lcd_delayms(5);
	LCD_CtrlData_HX8347D(0x1F,0xD0);// GAS=1, VOMG=10, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
	s_lcm_func_table->lcd_delayms(5);
	//262k/65k color selection
	LCD_CtrlData_HX8347D(0x17,0x05); //default 0x06 262k color // 0x05 65k color
	//SET PANEL
	LCD_CtrlData_HX8347D(0x36, 0x000c ); //
	//Display ON Setting
	LCD_CtrlData_HX8347D(0x28,0x38); //GON=1, DTE=1, D=1000
	s_lcm_func_table->lcd_delayms(40);
	LCD_CtrlData_HX8347D(0x28,0x3C); //GON=1, DTE=1, D=1100
	//Set GRAM Area
	LCD_CtrlData_HX8347D(0x02,0x00);
	LCD_CtrlData_HX8347D(0x03,0x00); //Column Start
	LCD_CtrlData_HX8347D(0x04,0x00);
	LCD_CtrlData_HX8347D(0x05,0xEF); //Column End
	LCD_CtrlData_HX8347D(0x06,0x00);
	LCD_CtrlData_HX8347D(0x07,0x00); //Row Start
	LCD_CtrlData_HX8347D(0x08,0x01);
	LCD_CtrlData_HX8347D(0x09,0x3F); //Row End
	LCD_CtrlWrite_HX8347D(0x22); //Start GRAM write
	//##################################################################
	// Enter Sleep mode Setting
	LCD_CtrlData_HX8347D(0x28,0xB8); //GON=’1’ DTE=’1’ D[1:0]=’10’
	s_lcm_func_table->lcd_delayms(40);
	LCD_CtrlData_HX8347D(0x1F,0x89); // GAS=1, VOMG=00, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=1
	s_lcm_func_table->lcd_delayms(40);
	LCD_CtrlData_HX8347D(0x28,0x04); //GON=’0’ DTE=’0’ D[1:0]=’01’
	s_lcm_func_table->lcd_delayms(40);
	LCD_CtrlData_HX8347D(0x19,0x00); //OSC_EN=’0’
	s_lcm_func_table->lcd_delayms(5);
	//#################################################
	// Exit Sleep mode Setting
	LCD_CtrlData_HX8347D(0x18,0x36); //I/P_RADJ,N/P_RADJ, Normal mode 75Hz
	LCD_CtrlData_HX8347D(0x19,0x01); //OSC_EN='1', start Osc
	LCD_CtrlData_HX8347D(0x1F,0x88);// GAS=1, VOMG=00, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=0
	s_lcm_func_table->lcd_delayms(5);
	LCD_CtrlData_HX8347D(0x1F,0x80);// GAS=1, VOMG=00, PON=0, DK=0, XDK=0, DVDH_TRI=0, STB=0
	s_lcm_func_table->lcd_delayms(5);
	LCD_CtrlData_HX8347D(0x1F,0x90);// GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0
	s_lcm_func_table->lcd_delayms(5);
	LCD_CtrlData_HX8347D(0x1F,0xD0);// GAS=1, VOMG=10, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
	s_lcm_func_table->lcd_delayms(5);
	LCD_CtrlData_HX8347D(0x28,0x38); //GON=1, DTE=1, D=1000
	s_lcm_func_table->lcd_delayms(40);
	LCD_CtrlData_HX8347D(0x28,0x3F); //GON=1, DTE=1, D=1100

#endif

	return;
}


/**************************************************************************************/
// Description: initialize all LCD with LCDC MCU MODE and LCDC mcu mode
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
LOCAL ERR_LCD_E HX8347D_Init(void)
{
	HX8347D_driver();	
	return 0;
}

/******************************************************************************/
//  Description:   Enter/Exit sleep mode .
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E  HX8347D_EnterSleep(
	BOOLEAN is_sleep 	//SCI_TRUE: enter sleep mode;SCI_FALSE:exit sleep mode.
	)
{

	if ( is_sleep ) // enter sleep mode.
	{
		LCD_CtrlData_HX8347D(0x28,0x38);
		LCD_Delay(40);
		LCD_CtrlData_HX8347D(0x28,0x04);

		LCD_CtrlData_HX8347D(0x1F,0x08);
		LCD_Delay(5);
		LCD_CtrlData_HX8347D(0x1F,0x09);
		LCD_CtrlData_HX8347D(0x19,0x00); //osc_en="0"
	}
	else 			// out sleep mode 
	{
		s_lcm_func_table->lcd_reset();
		HX8347D_Init();
	}

	return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL void HX8347D_Close(void)
{
	//GPIO_SetLcdBackLight( SCI_FALSE );
	HX8347D_EnterSleep( SCI_TRUE );	
}

/******************************************************************************/
//  Description:   invalidate a rectang of in LCD
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL ERR_LCD_E HX8347D_Invalidate(void)
{

	HX8347D_set_display_window (0x0, 0X0, LCD_WIDTH - 1, LCD_HEIGHT -1);
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
PUBLIC ERR_LCD_E HX8347D_InvalidateRect(
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
   
    HX8347D_set_display_window(left, top, right, bottom); 
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
LOCAL ERR_LCD_E HX8347D_RotationInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom,LCD_ANGLE_E angle)
{
	//int32 error;
	

	switch(angle)
	{
		case LCD_ANGLE_0:
			HX8347D_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_90:
			HX8347D_set_display_window(left, top, bottom,right);
			break;
		case LCD_ANGLE_180:
			HX8347D_set_display_window(left, top, right, bottom);
			break;
		case LCD_ANGLE_270:
			HX8347D_set_display_window(left, top, bottom,right);
			break;
		default:
			HX8347D_set_display_window(left, top, right, bottom);
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
LOCAL ERR_LCD_E   HX8347D_SetContrast(
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
LOCAL ERR_LCD_E   HX8347D_SetBrightness(
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
LOCAL ERR_LCD_E HX8347D_SetDisplayWindow(
	uint16 left, 		//left of the window
	uint16 top,			//top of the window
	uint16 right,		//right of the window
	uint16 bottom		//bottom of the window
	)
{
	 	
	HX8347D_set_display_window(left, top, right, bottom);

	return ERR_LCD_NONE;
}

/******************************************************************************/
// Description: Invalidate Pixel
// Global resource dependence: 
// Author: 
// Note:
/******************************************************************************/
LOCAL void HX8347D_InvalidatePixel(uint16 x, uint16 y, uint32 data)
{
	HX8347D_InvalidateRect(x,y,x,y);
	LCD_DataWrite_HX8347D(data);
}

#if 0
/******************************************************************************/
// Description: 
// Global resource dependence: 
// Author: 
// Note:
/******************************************************************************/
LOCAL uint32 HX8347D_ReadID(void)
{
	LCD_CtrlWrite_HX8347D(0x00);
	return LCD_DataRead_HX8347D()
}
#endif

/******************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
LOCAL const LCD_OPERATIONS_T HX8347D_operations = 
{
	HX8347D_Init,
	HX8347D_EnterSleep,
	HX8347D_SetContrast,
	HX8347D_SetBrightness,
	HX8347D_SetDisplayWindow,
	HX8347D_InvalidateRect,
	HX8347D_Invalidate,
	HX8347D_Close,
	HX8347D_RotationInvalidateRect,
	HX8347D_SetDirection,
	NULL
};


LOCAL const LCD_TIMING_U s_HX8347D_timing =
{

	        // LCM_CYCLE_U start(ns)
	10,		// CS setup time for LCM read (optional)  
	150,	// low pulse width for LCM read (according spec)	
	100,	// high pulse width for LCM read (according spec)
	10,		// CS setup time for LCM write  (optional) 	
	20,		// low pulse width for LCM write (according spec)
	10,	    // high pulse width for LCM write (according spec)	
	        // LCM_CYCLE_U end(ns)
};


PUBLIC const LCD_SPEC_T g_lcd_HX8347D[] ={
{
	LCD_WIDTH,
	LCD_HEIGHT, 
	LCD_MCU, 
	BUS_MODE_8080,
#ifdef LCD_DATA_WIDTH_8BIT
	WIDTH_8,
#else //LCM_16BITS
	WIDTH_16,
#endif //LCM_8BITS
	(LCD_TIMING_U*)&s_HX8347D_timing, 
	(LCD_OPERATIONS_T*)&HX8347D_operations,
	0,
	0}
};

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 


