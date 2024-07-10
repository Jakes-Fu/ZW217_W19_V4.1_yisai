#include "gpio_prod_api.h"
#include "dal_lcd.h"
#include "production_test.h"


void SCI_ProductTest_Current_Stop(void)
{
	GPIO_SetKeyPadBackLight(0);
	GPIO_SetLcdBackLight(0);
	GPIO_EnableAmplifier(0);
	GPIO_SetVibrator(0);
}

void SCI_ProductTest_Current_Prepare_DeepSleep(void)
{
    uint32       lcd_num;

    MMK_ResetTp();
    TP_Disable();

	 //deep sleep LCD
	{
	    lcd_num = LCD_GetUsedNum();

	    if (lcd_num == 1)
	    {
                LCD_EnterSleep (MAIN_LCD_ID, SCI_TRUE);
	    }
	    else
	    {
                LCD_EnterSleep (MAIN_LCD_ID, SCI_TRUE);
                LCD_EnterSleep (SUB_LCD_ID, SCI_TRUE);
	    }
	}

//	DC_Init();
//	DC_EnterSleep(1);
	
	GPIO_SetKeyPadBackLight(SCI_FALSE);	
	GPIO_SetLcdBackLight(SCI_FALSE);

	//UIS8910 为双核 AMP所有功能均由CP核控制 取消AMP控制
	//GPIO_EnableAmplifier(SCI_FALSE);

	GPIO_SetVibrator(SCI_FALSE);
    // 20111222: Will.Jiang_NEWMS00143544 : set bt into deepsleep
	GPIO_SetBtReset(SCI_FALSE);
}

void SCI_ProductTest_Current_LED_On(void)
{
	GPIO_SetBackLight(SCI_TRUE);
}

void SCI_ProductTest_Current_Vibrator_On(void)
{
	GPIO_SetVibrator(SCI_TRUE);
}

void SCI_ProductTest_Current_Charge(void)
{
	
}
