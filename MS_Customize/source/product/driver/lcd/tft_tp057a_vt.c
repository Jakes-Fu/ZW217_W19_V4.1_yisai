/******************************************************************************
 ** File Name:     TP057A.c                                                   *
 ** Description:                                                              *
 **    This file contains driver for TFT TP057A.                              *
 ** Author:         Jim zhang                                                 *
 ** DATE:           10/19/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/19/2003     Jim.zhang        Create.                                   *
 ** 12/25/2005     Zhemin.Lin       port to new LCD module TP057A.            *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#include "os_api.h"
#include "sci_api.h"
#include "sci_types.h"
#include "sc8800d_reg_lcd.h"
#include "dal_lcd.h"
#include "deep_sleep.h"

/**---------------------------------------------------------------------------*
 **                        external symbol define in lcd_cfg.c                                     
 **---------------------------------------------------------------------------*/
uint16 g_lcd_version = 0;

extern void *LCD_GetLCDBuffer(void);
extern const uint16 lcd_width;
extern const uint16 lcd_height;

extern const uint32 mv8602_bank_sel_adr;//address of mv8602 bankselect.
extern const uint32 lcd_rs_adr;         //address of register select for main lcd.
extern const uint32 lcd_command_adr;    //address of command for main lcd.
extern const uint32 lcd_data_adr;       //address of data for main lcd. 
extern const uint16 lcd_clear_color;    //when clear lcd, fill this color

extern PUBLIC BOOLEAN MV_ByPassLock(void);
extern PUBLIC BOOLEAN MV_ByPassUnLock(void);
/**---------------------------------------------------------------------------*
 **                         Macro defines                                     
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#define TP057A_BANK0_SELECT             {*((volatile uint16 *)mv8602_bank_sel_adr) = 0x0;}
#define TP057A_SEND_COMMAND(c)          {*((volatile uint16 *)lcd_rs_adr) = 0; *((volatile uint16 *)lcd_command_adr) = c;}
#define TP057A_SEND_DATA(d)             {*((volatile uint16 *)lcd_rs_adr) = 1; *((volatile uint16 *)lcd_data_adr) = d;}
#define TP057A_SEND_DATA_RS             {*((volatile uint16 *)lcd_rs_adr) = 1;}
#define TP057A_SEND_DATA2(d)            {*((volatile uint16 *)lcd_data_adr) = d;}

#define TP057A_SEND_CMD_DATA(c, d) {TP057A_SEND_COMMAND(c); TP057A_SEND_DATA(d);}


LOCAL void TP057A_Clear( uint32 color );

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/

/**
 * Reset the LCD, be sure LCD has been power on, in other words MV8602 has been power on
 */
LOCAL void TP057A_HardwareReset(void)
{
#define UNIT_TEST

#ifdef UNIT_TEST
    mvpu_init();
#endif

    /* hardware reset */
    GPIO_SetLcdReset(1);
    SCI_Sleep(5);
    GPIO_SetLcdReset(0);
    SCI_Sleep(5);
    GPIO_SetLcdReset(1);
    SCI_Sleep(20);
}



/**
 * LCD initialize sequence.
 */
LOCAL ERR_LCD_E TP057A_Init(void)
{
	uint32 i  = 0;
	uint32 bkground = 0xFF00;
	
    /*reset hardware & power off*/
    TP057A_HardwareReset();

	
    if (LCD_InterfaceIsIdle())
    {
        LCD_SetInterfaceIdleFlag(0);
    }
    else
    {
        return ERR_LCD_FUNC_NOT_SUPPORT;
    }

    TP057A_BANK0_SELECT;
	
#ifdef _SP9100_P3_

	SCI_Sleep(15);
	TP057A_SEND_COMMAND(0x0000);
	TP057A_SEND_DATA_RS;
	g_lcd_version = *( (volatile uint16 *)lcd_data_adr );

	//if(g_lcd_version == 0x1503)
	{
		TP057A_SEND_CMD_DATA(0xA4, 0x0001);
		SCI_Sleep(15);
		TP057A_SEND_CMD_DATA(0x07, 0x0021);
		TP057A_SEND_CMD_DATA(0x18, 0x0001);
		TP057A_SEND_CMD_DATA(0x10, 0x12B0);
		TP057A_SEND_CMD_DATA(0x11, 0x0007);
		TP057A_SEND_CMD_DATA(0x12, 0x010C);
		TP057A_SEND_CMD_DATA(0x13, 0x8E8F);
		TP057A_SEND_CMD_DATA(0x14, 0x8000);
		SCI_Sleep(45);
		TP057A_SEND_CMD_DATA(0x12, 0x11AC);
		SCI_Sleep(45);
		TP057A_SEND_CMD_DATA(0x12, 0x11BC);
		SCI_Sleep(128);
		TP057A_SEND_CMD_DATA(0x01, 0x0500);
		TP057A_SEND_CMD_DATA(0x02, 0x0500);
		TP057A_SEND_CMD_DATA(0x03, 0x1030);
		TP057A_SEND_CMD_DATA(0x08, 0x0808);
		TP057A_SEND_CMD_DATA(0x09, 0x0000);
		TP057A_SEND_CMD_DATA(0x0C, 0x0000);
		TP057A_SEND_CMD_DATA(0x30, 0x0003);
		TP057A_SEND_CMD_DATA(0x31, 0x0202);
		TP057A_SEND_CMD_DATA(0x32, 0x0707);
		TP057A_SEND_CMD_DATA(0x33, 0x0001);
		TP057A_SEND_CMD_DATA(0x34, 0x0201);
		TP057A_SEND_CMD_DATA(0x35, 0x0405);
		TP057A_SEND_CMD_DATA(0x36, 0x0606);
		TP057A_SEND_CMD_DATA(0x37, 0x0001);
		TP057A_SEND_CMD_DATA(0x38, 0x0100);
		TP057A_SEND_CMD_DATA(0x39, 0x0100);
		TP057A_SEND_CMD_DATA(0x50, 0x0000);/* sx */
		TP057A_SEND_CMD_DATA(0x51, 0x00AF);/* ex */
		TP057A_SEND_CMD_DATA(0x52, 0x0000);/* sy */
		TP057A_SEND_CMD_DATA(0x53, 0x00DB);/* ey */
		TP057A_SEND_CMD_DATA(0x70, 0x1B00);
		TP057A_SEND_CMD_DATA(0x71, 0x0000);
		TP057A_SEND_CMD_DATA(0x80, 0x0000);
		TP057A_SEND_CMD_DATA(0x81, 0x0000);
		TP057A_SEND_CMD_DATA(0x82, 0x002F);
		TP057A_SEND_CMD_DATA(0x83, 0x0000);
		TP057A_SEND_CMD_DATA(0x84, 0x0000);
		TP057A_SEND_CMD_DATA(0x85, 0x0000);
		TP057A_SEND_CMD_DATA(0x90, 0x0001);
		TP057A_SEND_CMD_DATA(0x20, 0x0000);/* start x */
		TP057A_SEND_CMD_DATA(0x21, 0x0000);/* start y */
		TP057A_SEND_COMMAND(0X22);

		for(i = 0; i < (176 * 220); i++)
		{
			TP057A_SEND_DATA(bkground);
		}
		/*TP057A_SEND_CMD_DATA(0x07, 0x0021);
		SCI_Sleep(50);*/
		TP057A_SEND_CMD_DATA(0x07, 0x0233);
	}
	/*else
	{
		//SCI_TRACE_LOW:"LCD ID ERROR!!!HDr@@@@@@@@@@@@ Lcd_ver:%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_TP057A_VT_176_112_2_18_0_34_14_1877,(uint8*)"d", g_lcd_version);
	}*/
#else

    TP057A_SEND_CMD_DATA(0x01, 0x02); //;SET 8/16BITS       16:02       18:00

    TP057A_SEND_CMD_DATA(0x02, 0x80); // MODE_SEL2

    TP057A_SEND_CMD_DATA(0x18, 0x06); //MODE_SEL3

    TP057A_SEND_CMD_DATA(0x35, 0x03); //STV0=ON

    SCI_Sleep(50);

    //set window address
    TP057A_SEND_CMD_DATA(0x14, 0);

    TP057A_SEND_CMD_DATA(0x15, 0);

    TP057A_SEND_CMD_DATA(0x16, (lcd_width-1));

    TP057A_SEND_CMD_DATA(0x17, (lcd_height-1));
    

    TP057A_SEND_CMD_DATA(0x07, 0x00);       

    TP057A_SEND_CMD_DATA(0x08, 0x00);           

    TP057A_SEND_CMD_DATA(0x04, 0x50); //OP bias 180%          

    TP057A_SEND_CMD_DATA(0x37, 0x3E);           

    TP057A_SEND_CMD_DATA(0x3F, 0x08);           

    TP057A_SEND_CMD_DATA(0x5A, 0x1C);  
#endif
    LCD_SetInterfaceIdleFlag(1);

#ifdef _SP9100_P3_

    TP057A_SEND_COMMAND(0x0000);
	TP057A_SEND_DATA_RS;
	g_lcd_version = *( (volatile uint16 *)lcd_data_adr );

#endif	
    return ERR_LCD_NONE;
}

/**
 * LCD initialize sequence.
 */
LOCAL void TP057A_Initialize(void)
{
	uint32 i  = 0;
	uint32 bkground = 0xFF00;
	
    /*reset hardware & power off*/
    TP057A_HardwareReset();

	
    if (LCD_InterfaceIsIdle())
    {
        LCD_SetInterfaceIdleFlag(0);
    }
    else
    {
        return;
    }

    TP057A_BANK0_SELECT;
	
#ifdef _SP9100_P3_

	SCI_Sleep(15);
	TP057A_SEND_COMMAND(0x0000);
	TP057A_SEND_DATA_RS;
	g_lcd_version = *( (volatile uint16 *)lcd_data_adr );

	//if(g_lcd_version == 0x1503)
	{
		TP057A_SEND_CMD_DATA(0xA4, 0x0001);
		SCI_Sleep(15);
		TP057A_SEND_CMD_DATA(0x07, 0x0021);
		TP057A_SEND_CMD_DATA(0x18, 0x0001);
		TP057A_SEND_CMD_DATA(0x10, 0x12B0);
		TP057A_SEND_CMD_DATA(0x11, 0x0007);
		TP057A_SEND_CMD_DATA(0x12, 0x010C);
		TP057A_SEND_CMD_DATA(0x13, 0x8E8F);
		TP057A_SEND_CMD_DATA(0x14, 0x8000);
		SCI_Sleep(45);
		TP057A_SEND_CMD_DATA(0x12, 0x11AC);
		SCI_Sleep(45);
		TP057A_SEND_CMD_DATA(0x12, 0x11BC);
		SCI_Sleep(128);
		TP057A_SEND_CMD_DATA(0x01, 0x0500);
		TP057A_SEND_CMD_DATA(0x02, 0x0500);
		TP057A_SEND_CMD_DATA(0x03, 0x1030);
		TP057A_SEND_CMD_DATA(0x08, 0x0808);
		TP057A_SEND_CMD_DATA(0x09, 0x0000);
		TP057A_SEND_CMD_DATA(0x0C, 0x0000);
		TP057A_SEND_CMD_DATA(0x30, 0x0003);
		TP057A_SEND_CMD_DATA(0x31, 0x0202);
		TP057A_SEND_CMD_DATA(0x32, 0x0707);
		TP057A_SEND_CMD_DATA(0x33, 0x0001);
		TP057A_SEND_CMD_DATA(0x34, 0x0201);
		TP057A_SEND_CMD_DATA(0x35, 0x0405);
		TP057A_SEND_CMD_DATA(0x36, 0x0606);
		TP057A_SEND_CMD_DATA(0x37, 0x0001);
		TP057A_SEND_CMD_DATA(0x38, 0x0100);
		TP057A_SEND_CMD_DATA(0x39, 0x0100);
		TP057A_SEND_CMD_DATA(0x50, 0x0000);/* sx */
		TP057A_SEND_CMD_DATA(0x51, 0x00AF);/* ex */
		TP057A_SEND_CMD_DATA(0x52, 0x0000);/* sy */
		TP057A_SEND_CMD_DATA(0x53, 0x00DB);/* ey */
		TP057A_SEND_CMD_DATA(0x70, 0x1B00);
		TP057A_SEND_CMD_DATA(0x71, 0x0000);
		TP057A_SEND_CMD_DATA(0x80, 0x0000);
		TP057A_SEND_CMD_DATA(0x81, 0x0000);
		TP057A_SEND_CMD_DATA(0x82, 0x002F);
		TP057A_SEND_CMD_DATA(0x83, 0x0000);
		TP057A_SEND_CMD_DATA(0x84, 0x0000);
		TP057A_SEND_CMD_DATA(0x85, 0x0000);
		TP057A_SEND_CMD_DATA(0x90, 0x0001);
		TP057A_SEND_CMD_DATA(0x20, 0x0000);/* start x */
		TP057A_SEND_CMD_DATA(0x21, 0x0000);/* start y */
		TP057A_SEND_COMMAND(0X22);

		for(i = 0; i < (176 * 220); i++)
		{
			TP057A_SEND_DATA(bkground);
		}
		/*TP057A_SEND_CMD_DATA(0x07, 0x0021);
		SCI_Sleep(50);*/
		TP057A_SEND_CMD_DATA(0x07, 0x0233);
	}
	/*else
	{
		//SCI_TRACE_LOW:"LCD ID ERROR!!!HDr@@@@@@@@@@@@ Lcd_ver:%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TFT_TP057A_VT_312_112_2_18_0_34_15_1878,(uint8*)"d", g_lcd_version);
	}*/
#else

    TP057A_SEND_CMD_DATA(0x01, 0x02); //;SET 8/16BITS       16:02       18:00

    TP057A_SEND_CMD_DATA(0x02, 0x80); // MODE_SEL2

    TP057A_SEND_CMD_DATA(0x18, 0x06); //MODE_SEL3

    TP057A_SEND_CMD_DATA(0x35, 0x03); //STV0=ON

    SCI_Sleep(50);

    //set window address
    TP057A_SEND_CMD_DATA(0x14, 0);

    TP057A_SEND_CMD_DATA(0x15, 0);

    TP057A_SEND_CMD_DATA(0x16, (lcd_width-1));

    TP057A_SEND_CMD_DATA(0x17, (lcd_height-1));
    

    TP057A_SEND_CMD_DATA(0x07, 0x00);       

    TP057A_SEND_CMD_DATA(0x08, 0x00);           

    TP057A_SEND_CMD_DATA(0x04, 0x50); //OP bias 180%          

    TP057A_SEND_CMD_DATA(0x37, 0x3E);           

    TP057A_SEND_CMD_DATA(0x3F, 0x08);           

    TP057A_SEND_CMD_DATA(0x5A, 0x1C);  
#endif
    LCD_SetInterfaceIdleFlag(1);

#ifdef _SP9100_P3_

    TP057A_SEND_COMMAND(0x0000);
	TP057A_SEND_DATA_RS;
	g_lcd_version = *( (volatile uint16 *)lcd_data_adr );

#endif	
}

__inline void TP057A_set_display_window(
    uint8 left,     // start Horizon address
    uint8 right,    // end Horizon address
    uint8 top,      // start Vertical address
    uint8 bottom    // end Vertical address
    )
{
    
#ifdef _SP9100_P3_    
    //set sram address
    TP057A_SEND_CMD_DATA(0x50, left);
    TP057A_SEND_CMD_DATA(0x52, top);
    TP057A_SEND_CMD_DATA(0x51, right);
    TP057A_SEND_CMD_DATA(0x53, bottom);
#else
    TP057A_SEND_CMD_DATA(0x10, left);
    TP057A_SEND_CMD_DATA(0x11, top);
    TP057A_SEND_CMD_DATA(0x12, right);
    TP057A_SEND_CMD_DATA(0x13, bottom);
#endif
}

__inline void TP057A_set_start_address(
    uint8 left, 
    uint8 top
    )
{
#ifdef _SP9100_P3_
    //set window address
    TP057A_SEND_CMD_DATA(0x20, left);
    TP057A_SEND_CMD_DATA(0x21, top);
#else
    TP057A_SEND_CMD_DATA(0x07, left);
    TP057A_SEND_CMD_DATA(0x08, top);
#endif
}

/**
 * Invalidate a rectanglular on LCD
 */
LOCAL ERR_LCD_E TP057A_InvalidateRect( uint16 left, uint16 top, uint16 right, uint16 bottom )
{
    uint32 i, j;
    uint32 row, column;
    uint16 *line_ptr = (uint16 *)LCD_GetLCDBuffer();
    uint16 *buf_ptr;

    left    = (left >= lcd_width)    ? lcd_width-1 : left;
    right   = (right >= lcd_width)   ? lcd_width-1 : right;
    top     = (top >= lcd_height)    ? lcd_height-1 : top;
    bottom  = (bottom >= lcd_height) ? lcd_height-1 : bottom;

    if ( ( right < left ) || ( bottom < top ) )
    {
        return ERR_LCD_PARAMETER_WRONG;
    }

    if (LCD_InterfaceIsIdle())
    {
        LCD_SetInterfaceIdleFlag(0);
    }
    else
    {
        return ERR_LCD_FUNC_NOT_SUPPORT;
    }
    
    if(SCI_InThreadContext())
    {
        if(! MV_ByPassLock())
        {
            LCD_SetInterfaceIdleFlag(1);
			return ERR_LCD_NONE;
        }
	
    }
    
    TP057A_BANK0_SELECT;

    TP057A_set_start_address(left, top);
        
    TP057A_set_display_window(left, right, top, bottom);

	TP057A_SEND_COMMAND(0X22);
	
    TP057A_SEND_DATA_RS;

    line_ptr += lcd_width * top;
    
    for (j = top; j <= bottom; j++)
    {
        buf_ptr = line_ptr + left;
        for (i = left; i <= right; i++) 
        {
            TP057A_SEND_DATA2( *(buf_ptr++));
        }
        line_ptr += lcd_width;
    }

    if(SCI_InThreadContext())
    {
        MV_ByPassUnLock();
    }
    
    LCD_SetInterfaceIdleFlag(1);

    return ERR_LCD_NONE;    
}

/**
 * LCD invalidate
 */
LOCAL ERR_LCD_E TP057A_Invalidate(void)
{
    return TP057A_InvalidateRect(0, 0, lcd_width-1, lcd_height-1);
}

/**
 * Enter sleep or resume
 */
LOCAL ERR_LCD_E TP057A_EnterSleep(  BOOLEAN is_sleep )
{
#ifdef _SP9100_P3_
	uint32 i;
	if (LCD_InterfaceIsIdle())
    {
        LCD_SetInterfaceIdleFlag(0);
    }
    else
    {
        return  ERR_LCD_FUNC_NOT_SUPPORT;
    }

    if(! MV_ByPassLock())
        {
              LCD_SetInterfaceIdleFlag(1);
              return ERR_LCD_NONE;
        }

    TP057A_BANK0_SELECT;

	if (is_sleep)
    {
		TP057A_SEND_CMD_DATA(0x07, 0x0232);
		SCI_Sleep(50);
		TP057A_SEND_CMD_DATA(0x07, 0x0201);
		SCI_Sleep(5);
		TP057A_SEND_CMD_DATA(0x07, 0x0020);
		TP057A_SEND_CMD_DATA(0x09, 0x0000);
		TP057A_SEND_CMD_DATA(0x10, 0x0280);
		TP057A_SEND_CMD_DATA(0x11, 0x0067);
		TP057A_SEND_CMD_DATA(0x12, 0x102c);
		SCI_Sleep(25);
		TP057A_SEND_CMD_DATA(0x10, 0x0000);
		TP057A_SEND_CMD_DATA(0x10, 0x0002);
		
        LCD_SetMainLcdSleepFlag(1);
        LCD_PostProcAfterSleep();   
	  	MV_ByPassUnLock();
        
    }
    else
    {

        SCI_LCDBacklightBrightless_EnableDeepSleep(FALSE); 
        
        LCD_PreProcBeforeResume();
        LCD_SetMainLcdSleepFlag(0);

		/* deep standby mode 重置全部寄存器及GRam内容 */
		TP057A_SEND_CMD_DATA(0x10, 0x0000);
		SCI_Sleep(100);
		
		TP057A_SEND_CMD_DATA(0x07, 0x0021);
		TP057A_SEND_CMD_DATA(0x18, 0x0001);
		TP057A_SEND_CMD_DATA(0x10, 0x12B0);
		TP057A_SEND_CMD_DATA(0x11, 0x0007);
		TP057A_SEND_CMD_DATA(0x12, 0x010C);
		TP057A_SEND_CMD_DATA(0x13, 0x8E8F);
		TP057A_SEND_CMD_DATA(0x14, 0x8000);
		SCI_Sleep(45);
		TP057A_SEND_CMD_DATA(0x12, 0x11AC);
		SCI_Sleep(45);
		TP057A_SEND_CMD_DATA(0x12, 0x11BC);
		SCI_Sleep(128);
		TP057A_SEND_CMD_DATA(0x01, 0x0500);
		TP057A_SEND_CMD_DATA(0x02, 0x0500);
		TP057A_SEND_CMD_DATA(0x03, 0x1030);
		TP057A_SEND_CMD_DATA(0x08, 0x0808);
		TP057A_SEND_CMD_DATA(0x09, 0x0000);
		TP057A_SEND_CMD_DATA(0x0C, 0x0000);
		TP057A_SEND_CMD_DATA(0x30, 0x0003);
		TP057A_SEND_CMD_DATA(0x31, 0x0202);
		TP057A_SEND_CMD_DATA(0x32, 0x0707);
		TP057A_SEND_CMD_DATA(0x33, 0x0001);
		TP057A_SEND_CMD_DATA(0x34, 0x0201);
		TP057A_SEND_CMD_DATA(0x35, 0x0405);
		TP057A_SEND_CMD_DATA(0x36, 0x0606);
		TP057A_SEND_CMD_DATA(0x37, 0x0001);
		TP057A_SEND_CMD_DATA(0x38, 0x0100);
		TP057A_SEND_CMD_DATA(0x39, 0x0100);
		TP057A_SEND_CMD_DATA(0x50, 0x0000);/* sx */
		TP057A_SEND_CMD_DATA(0x51, 0x00AF);/* ex */
		TP057A_SEND_CMD_DATA(0x52, 0x0000);/* sy */
		TP057A_SEND_CMD_DATA(0x53, 0x00DB);/* ey */
		TP057A_SEND_CMD_DATA(0x70, 0x1B00);
		TP057A_SEND_CMD_DATA(0x71, 0x0000);
		TP057A_SEND_CMD_DATA(0x80, 0x0000);
		TP057A_SEND_CMD_DATA(0x81, 0x0000);
		TP057A_SEND_CMD_DATA(0x82, 0x002F);
		TP057A_SEND_CMD_DATA(0x83, 0x0000);
		TP057A_SEND_CMD_DATA(0x84, 0x0000);
		TP057A_SEND_CMD_DATA(0x85, 0x0000);
		TP057A_SEND_CMD_DATA(0x90, 0x0001);
		TP057A_SEND_CMD_DATA(0x20, 0x0000);/* start x */
		TP057A_SEND_CMD_DATA(0x21, 0x0000);/* start y */
		TP057A_SEND_COMMAND(0X22);

		/*TP057A_SEND_CMD_DATA(0x07, 0x0021);
		SCI_Sleep(50);*/
		TP057A_SEND_CMD_DATA(0x07, 0x0233);

        SCI_LCDBacklightBrightless_EnableDeepSleep(TRUE); 
		
		MV_ByPassUnLock();
		SCI_Sleep(50); /*@@@@add for temp*/

    }
#endif

    LCD_SetInterfaceIdleFlag(1);

    return ERR_LCD_NONE;

#ifdef _SP9100_P2_
    if (LCD_InterfaceIsIdle())
    {
        LCD_SetInterfaceIdleFlag(0);
    }
    else
    {
        return ERR_LCD_FUNC_NOT_SUPPORT;
    }
    MV_ByPassLock();
    TP057A_BANK0_SELECT;

    if (is_sleep)
    {
        TP057A_SEND_CMD_DATA(0x02, 0xC0);
        LCD_SetMainLcdSleepFlag(1);
        LCD_PostProcAfterSleep();   
	  MV_ByPassUnLock();
        
    }
    else
    {
        LCD_PreProcBeforeResume();
        LCD_SetMainLcdSleepFlag(0);
        TP057A_SEND_CMD_DATA(0x02, 0x80);
         MV_ByPassUnLock();
        SCI_SLEEP(400);
    }
    
    LCD_SetInterfaceIdleFlag(1);
    return ERR_LCD_NONE;
#endif
}

/**
 * Close operation
 */
LOCAL void TP057A_Close(void)
{
    TP057A_EnterSleep( SCI_TRUE );
    GPIO_SetLcdBackLight( SCI_FALSE );
}

/**
 * Clear LCD with a specified color.
 */
LOCAL void TP057A_Clear( uint32 color )
{
    uint32 i;
    uint16 *buf_ptr = (uint16 *)LCD_GetLCDBuffer();
    
    for( i=0; i<(lcd_width * lcd_height); i++)
    {
        *buf_ptr++ = color;
    }       
            
    TP057A_Invalidate();
}

/**
 *  dumming function, set lcd contrast
 */
LOCAL ERR_LCD_E TP057A_SetContrast(
                                     uint16  contrast   //contrast value to set
                                     )
{
    return ERR_LCD_NONE;
}

/**
 * dumming function, set lcd brightness
 */
LOCAL ERR_LCD_E TP057A_SetBrightness(
                                       uint16 brightness    //birghtness to set
                                       )
{
    return ERR_LCD_FUNC_NOT_SUPPORT;
}

/**
 * dumming function, set display window
 */
LOCAL ERR_LCD_E TP057A_SetDisplayWindow(
                                          uint16 left,  //left of the window
                                          uint16 top,   //top of the window 
                                          uint16 right, //right of the window
                                          uint16 bottom //bottom of the window
                                          )
{
    return ERR_LCD_FUNC_NOT_SUPPORT;
}

/**
 * dumming function, get main lcd specification
 */
LOCAL ERR_LCD_E TP057A_GetMainLcdSpec(
                                        LCD_SPEC_T *spec_ptr //spec struct pointer
                                        )
{
    return ERR_LCD_NONE;
}

/**
 * dumming function, for LCD auto detection
 */
PUBLIC BOOLEAN TP057A_Probe(void)
{
    return SCI_TRUE;
}

/**
 * get lcd information, for GUI app
 */

LOCAL ERR_LCD_E TP057A_GetInfo(
                                 LCD_INFO_T *lcd_info_ptr   //lcd information struct pointer
                                 )
{
    if (PNULL == lcd_info_ptr)
    {
        return ERR_LCD_POINTER_NULL;
    }
    else
    {
        lcd_info_ptr->r_bitmask         = 0xf800;
        lcd_info_ptr->g_bitmask         = 0x07e0;
        lcd_info_ptr->b_bitmask         = 0x001f;
        lcd_info_ptr->bits_per_pixel    = 16;
        lcd_info_ptr->contrast_min      =0x00;
        lcd_info_ptr->contrast_max      =0xff;
        lcd_info_ptr->contrast_defaut   =0xB8;
        
        lcd_info_ptr->lcd_width     = lcd_width;
        lcd_info_ptr->lcd_height    = lcd_height;
        lcd_info_ptr->lcdbuff_ptr   = LCD_GetLCDBuffer();
        return ERR_LCD_NONE;
    }
}

/**
 * get lcd information, for GUI app
 */

LOCAL ERR_LCD_E TP057A_SetStartAddress(uint16 left,uint16 top)
{

    return ERR_LCD_FUNC_NOT_SUPPORT;
}

                                 
/******************************************************************************/
// Description:
// Global resorce dependence:
// Author: Jianping.wang
// Note:
/******************************************************************************/
LOCAL void  *TP057A_GetBuffer(void)
{
	void *buf_ptr;
	
	buf_ptr =(void*) LCD_GetLCDBuffer();
	
	SCI_ASSERT(buf_ptr != PNULL);/*assert verified*/
	
	return buf_ptr;
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
LOCAL ERR_LCD_E TP057A_VerticalInvalidateRect(uint16 left,uint16 top,uint16 right,uint16 bottom)
{
	return ERR_LCD_FUNC_NOT_SUPPORT;
}

LOCAL void TP057A_SetInvalidateDirection(BOOLEAN direction) 
{	
}

/**
 * LCD operations context
 */

const LCD_OPERATIONS_T TP057A_operations = 
{
    TP057A_Init,
    TP057A_EnterSleep,
    TP057A_SetContrast,
    TP057A_SetBrightness,
    TP057A_SetDisplayWindow,
    TP057A_SetStartAddress,
    TP057A_GetInfo,
    TP057A_InvalidateRect,
    TP057A_Invalidate,
    TP057A_Clear,
    TP057A_Close,
    TP057A_Initialize,
    TP057A_GetMainLcdSpec,
    TP057A_GetBuffer,
    TP057A_VerticalInvalidateRect,
    TP057A_SetInvalidateDirection
};

PUBLIC LCD_OPERATIONS_T* TP057A_GetOperations()
{
    return &TP057A_operations;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 



