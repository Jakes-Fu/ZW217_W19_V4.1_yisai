/******************************************************************************
 ** File Name:      lcd_backlight.c                                           *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           12/25/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the product lcd back light interface    * 					
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/25/2007     Liangwen.Zhen	  Create.                         		  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#include "ms_customize_trc.h"
#include "os_api.h"
#include "tb_hal.h"
#include "gpio_prod_api.h"
#include "lcd_backlight.h"
#include "tx_port_thumb.h"
#include "dal_lcd.h"
#include "lcm_prod.h"
#include "analog_drv.h"

#define DEBUG_LCD_BACKLIGHT
#ifdef  DEBUG_LCD_BACKLIGHT



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/
#define LCD_BACKLIGHT_FREQ     			128000
 #define L_delay_us(us)\
{\
   uint32 l,m,k;\
   k = CHIP_GetArmClk()/ARM_CLK_13M;\
   for (l = 0; l < us; l++) \
   {\
     m = k; \
     while(m--){ };\
   }\
}
/**---------------------------------------------------------------------------*
 **                         Data Structure                                    *
 **---------------------------------------------------------------------------*/
 typedef void (* LCD_BACKLIGHT_FUN_PTR)(uint32 brightness); 		// brightness : 0 - 100
 

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL void _LCD_SetBackLightBrightness_PWM	  ( uint32  brightness );

LOCAL void _LCD_SetBackLightBrightness_AAT3194( uint32  brightness );
LOCAL void _LCD_SetBackLightBrightness_AAT3155( uint32  brightness );
LOCAL void _LCD_SetBackLightBrightness_RT9364( uint32  brightness );
LOCAL void _LCD_SetBackLightBrightness_RT9285( uint32  brightness );
LOCAL void _LCD_SetBackLightBrightness_BB_Analog( uint32  brightness );

LOCAL const LCD_BACKLIGHT_FUN_PTR s_lcd_bl_fun_table[LCD_BACKLIGHT_TYPE_MAX] =
{
	_LCD_SetBackLightBrightness_PWM,	// POSITIVE
	_LCD_SetBackLightBrightness_PWM,	// NEGATIVE
	
	
	_LCD_SetBackLightBrightness_AAT3194,
	_LCD_SetBackLightBrightness_AAT3155,
	_LCD_SetBackLightBrightness_RT9364,
	_LCD_SetBackLightBrightness_RT9285,
	NULL,
	NULL,
	NULL,
	_LCD_SetBackLightBrightness_BB_Analog,
	NULL //add by brezen
};


LOCAL LCD_BACKLIGHT_INFO_T s_lcd_bl_info = {0};

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
 // temp function

/*****************************************************************************/
//  Description:    This function is used to convert backlight in different project, if this project 
//                        backlight control is PWM                           
//  Author:           Rui.Zhang
//  Note:           
/*****************************************************************************/

LOCAL  uint32 BackLight_Parameter_Conversion(uint32 bl_min, uint32 bl_max, uint32 brightness)
{    
    	uint32 backlight_ness = 0;
       uint32 middle_point = 0;
	// don't need convert  brightness	
	if(   	bl_min == 0 
	  && bl_max == 0)
	{
		backlight_ness = brightness;
	}
	// need convert brightness
	else
	{
		middle_point = (bl_min + bl_max)/2;
		if(brightness <= middle_point)
		{
			backlight_ness = bl_min+((brightness* (middle_point - bl_min)/middle_point) );
		}
		else
		{
			backlight_ness = middle_point+((brightness - middle_point) * (bl_max - middle_point) /(100 - middle_point));
		}

		if(brightness == 0)
		{
			backlight_ness = 0;
		}
	}

	return backlight_ness;
}


LOCAL BOOLEAN _LCD_GetBackLightType(LCD_BACKLIGHT_INFO_T_PTR bl_info_ptr)
{
	LCD_BACKLIGHT_INFO_T_PTR info_ptr = LCM_GetBackLightInfo(MAIN_LCD_ID);

	if(PNULL != info_ptr)
	{
		SCI_MEMCPY(bl_info_ptr, info_ptr, sizeof(LCD_BACKLIGHT_INFO_T));//lint !e516

		//SCI_TRACE_LOW:"_LCD_GetBackLightType: addr 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_BACKLIGHT_144_112_2_18_0_33_24_1729,(uint8*)"d", bl_info_ptr);
		//SCI_TRACE_LOW:"_LCD_GetBackLightType: type %d, param %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_BACKLIGHT_147_112_2_18_0_33_24_1730,(uint8*)"dd", bl_info_ptr->type,bl_info_ptr->param);
		return SCI_TRUE;
	}
	else
	{
		return SCI_FALSE;
	}	
}

 /*****************************************************************************/
//  Description:    This function is used to delay time. Unit: 1 ms
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _LCD_BL_DelayMS(uint32 ms)
{
	uint32 end = SCI_GetTickCount() + ms;
	
	while(SCI_GetTickCount() < end)	//PC-LINT "Suspicious use of ;"
	{
	}

}

/*****************************************************************************/
//  Description:    set the lcd backlight brightness by PWM
//
//  Input      : 
//    brightness : 0 - 100  // when the brightness is 0, turn off the backlight.
/*****************************************************************************/
LOCAL void _LCD_SetBackLightBrightness_PWM( uint32  brightness )
{
    BOOLEAN is_on = SCI_FALSE;
    
    if( brightness <= 100 )
    {
        if( brightness )
        {
        	if(LCD_BACKLIGHT_TYPE_PWM_POS == s_lcd_bl_info.type)
        	{
        		PWM_Config( s_lcd_bl_info.param, LCD_BACKLIGHT_FREQ, brightness); 
        	}
        	else if(LCD_BACKLIGHT_TYPE_PWM_NEG == s_lcd_bl_info.type)
        	{
        		PWM_Config( s_lcd_bl_info.param, LCD_BACKLIGHT_FREQ, 100 - brightness); 
        	}

            is_on = SCI_TRUE;
        }
        else
        {
            PWM_Config( s_lcd_bl_info.param, 0, 0 );
        }       
    }  
}

/*****************************************************************************/
//  Description:    set the lcd backlight brightness
//
//  Input      : 
//    brightness : 0 - 100  // when the brightness is 0, turn off the backlight.
/*****************************************************************************/
LOCAL void _LCD_SetBackLightBrightness_AAT3194( uint32  brightness )
{
	uint32 i = 0;
	TX_INTERRUPT_SAVE_AREA

	if( 0 != brightness )
	{
		if(brightness >= 40)
		{
			brightness -= 40;
		}
		
		brightness /= 2;
		brightness += 3;
		brightness = (brightness>31)?31:brightness;						

		// reset internal register
		GPIO_SetLcdBackLightness( SCI_FALSE );	
			
		_LCD_BL_DelayMS(2);
				
		TX_DISABLE_FIQ_IRQ

		for(i=0 ; i < brightness ; i++)
		{
			GPIO_SetLcdBackLightness( SCI_TRUE );	
			
			GPIO_SetLcdBackLightness( SCI_FALSE );			
		}	

		GPIO_SetLcdBackLightness( SCI_TRUE ); 

		TX_RESTORE
	}	
  	else
  	{
  		GPIO_SetLcdBackLightness( SCI_FALSE ); 
  	}
	
				
}

 /*****************************************************************************/
//  Description:    set the lcd backlight brightness
//
//  Input      : 
//    brightness : 0 - 100  // when the brightness is 0, turn off the backlight.
/*****************************************************************************/
LOCAL void _LCD_SetBackLightBrightness_AAT3155( uint32  brightness )
{
	uint32 i = 0;
	TX_INTERRUPT_SAVE_AREA

	if(0 != brightness)
	{

	        if(brightness >= 100)
	        {
	        	brightness = 96;
	        }
        
		if(brightness>=6)
		{
			brightness = brightness/6;
		}
		else
		{
			brightness = 1;
		}
		
		brightness = 16 - brightness;	
		
		GPIO_SetLcdBackLightness( SCI_FALSE );	
		_LCD_BL_DelayMS(2);

		TX_DISABLE_FIQ_IRQ

		GPIO_SetLcdBackLightness( SCI_TRUE );	
		L_delay_us(30);
		GPIO_SetLcdBackLightness( SCI_FALSE );	
		L_delay_us(30);

		for(i = 1 ; i < brightness ; i++)
		{
			GPIO_SetLcdBackLightness( SCI_TRUE );	
			L_delay_us(30);
			GPIO_SetLcdBackLightness( SCI_FALSE );	
    		L_delay_us(30);
		}	

		GPIO_SetLcdBackLightness( SCI_TRUE );

		TX_RESTORE	
	}
	else
	{  	
  		GPIO_SetLcdBackLightness( SCI_FALSE );   		
	}

	// Delay more than 500us time to set Latch and off time
	_LCD_BL_DelayMS(1);
			
}

 LOCAL void _LCD_SetBackLightBrightness_RT9364( uint32  brightness )
{
	uint32 i = 0;
	TX_INTERRUPT_SAVE_AREA

	if(0 != brightness)
	{
        // modify start for cr118426
        if(brightness >= 100)
        {
        	brightness = 96;
        }
        
		if(brightness>=6)
		{
			brightness = brightness/6;
		}
		else
		{
			brightness = 1;
		}
		brightness = 16 - brightness;
        // modify end for cr118426

		GPIO_SetLcdBackLightness( SCI_FALSE );
		_LCD_BL_DelayMS(5);
		GPIO_SetLcdBackLightness( SCI_TRUE );
		L_delay_us(80);
				
		TX_DISABLE_FIQ_IRQ

		for(i=0 ; i < brightness ; i++)
		{
			GPIO_SetLcdBackLightness( SCI_FALSE);	
			L_delay_us(30);
			GPIO_SetLcdBackLightness( SCI_TRUE);
			L_delay_us(30);
		}	

		//GPIO_SetLcdBackLightness( SCI_TRUE );

		TX_RESTORE	
	}
	else
	{  	
  		GPIO_SetLcdBackLightness( SCI_FALSE );   		
	}
			
}
#define RT9285C_BACKLIGHT_LEVEL5		1	/*5 Max*/
#define RT9285C_BACKLIGHT_LEVEL4		4
#define RT9285C_BACKLIGHT_LEVEL3		7
#define RT9285C_BACKLIGHT_LEVEL2		10
#define RT9285C_BACKLIGHT_LEVEL1		12	/*1 Min*/
#define RT9285C_BACKLIGHT_LEVEL0		14	/*0 Dim*/

uint8 aBacklightTable[11] = { 0,16,0,RT9285C_BACKLIGHT_LEVEL0,0,0,
		RT9285C_BACKLIGHT_LEVEL1,
		RT9285C_BACKLIGHT_LEVEL2,
		RT9285C_BACKLIGHT_LEVEL3,
		RT9285C_BACKLIGHT_LEVEL4,
		RT9285C_BACKLIGHT_LEVEL5};


LOCAL uint32 gSavedBrightness = 0;
LOCAL void _LCD_SetBackLightBrightness_RT9285( uint32  brightness )
{
	uint32 i;
	TX_INTERRUPT_SAVE_AREA

	i = brightness/10;

	if( gSavedBrightness != i )
	{
		
		if(i==0)
		{
	  		GPIO_SetLcdBackLightness( SCI_FALSE );   		
		}
		else if(i <= 10)
		{
	  		uint8 j, vcount = 0;

			if(gSavedBrightness < i)
			{
				vcount = aBacklightTable[1] - aBacklightTable[gSavedBrightness] + aBacklightTable[i];
			}
			else
			{
				vcount = aBacklightTable[i] - aBacklightTable[gSavedBrightness];
			}

			GPIO_SetLcdBackLightness( SCI_FALSE );
			L_delay_us(10);
			GPIO_SetLcdBackLightness( SCI_TRUE );
			L_delay_us(10);			
			for(j=0 ; j < vcount-1 ; j++)
			{
				TX_DISABLE_FIQ_IRQ
				GPIO_SetLcdBackLightness( SCI_FALSE);	
				L_delay_us(1);
				TX_RESTORE
				GPIO_SetLcdBackLightness( SCI_TRUE);
				L_delay_us(1);
			}	
			
		}
		else
		{
		}
		gSavedBrightness = i;
	}
}

 LOCAL void _LCD_SetBackLightBrightness_BB_Analog( uint32  brightness )
{
	ANA_SetDevValule(ANA_DEV_ID_LCM_BRIGHTNESS, brightness);
}

/**Add for IB calibration**/
PUBLIC void IB_Calibration()
{
    BOOLEAN ret = SCI_FALSE;
    uint32 IBCaliData;
    uint32 val;

    ret = EFUSE_HAL_GetIBCaliData(&IBCaliData);
    if (SCI_TRUE != ret)
    {
        return;
    }

    val = ANA_REG_GET(ANA_IB_CTRL);
    val &= ~(IB_TRIM_MSK);
    val |= (IBCaliData << 2);
    ANA_REG_SET(ANA_IB_CTRL, val);
}


/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to set lcd back light brightness    
//  Author:         Liangwen.Zhen
//  Note:
//   brightness : 0 - 100  // when the brightness is 0, turn off the backlight.           
/*****************************************************************************/
PUBLIC void LCD_SetBackLightBrightness( uint32  brightness ) 
{  
	uint32 backlight_ness = 0;
	
	if(_LCD_GetBackLightType(&s_lcd_bl_info))
	{
		SCI_ASSERT(brightness <= 100);/*assert verified*/
		SCI_ASSERT(s_lcd_bl_info.type < LCD_BACKLIGHT_TYPE_MAX);/*assert verified*/

		//if (REFPARAM_GetLCDBrightless() != brightness)
		{
			//REFPARAM_SetLCDBrightless (brightness);
		}	
		
		//SCI_TRACE_LOW:"LCD_SetBackLightBrightness: BackLight type %d, Brightness %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_BACKLIGHT_446_112_2_18_0_33_25_1731,(uint8*)"dd", s_lcd_bl_info.type, brightness);
		backlight_ness = BackLight_Parameter_Conversion(s_lcd_bl_info.bl_min,  s_lcd_bl_info.bl_max,  brightness);
		
		if(PNULL != s_lcd_bl_fun_table[s_lcd_bl_info.type])
		{
			s_lcd_bl_fun_table[s_lcd_bl_info.type](backlight_ness);
		}
		else
		{
			//SCI_PASSERT(0, ("LCD backlight type %d err", lcd_bl_type));
		}
	}
   
   //SCI_TRACE_LOW:"LCD_SetBackLightBrightness: BackLight type %d, backlight_ness %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_BACKLIGHT_459_112_2_18_0_33_25_1732,(uint8*)"dd", s_lcd_bl_info.type, backlight_ness);
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // end of lcd_backlight.c    
#endif  // End of DEBUG_LCD_BACKLIGHT

