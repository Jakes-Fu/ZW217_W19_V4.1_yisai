/******************************************************************************
 ** File Name:      msensor_mir3da.c                                          *
 ** Author:          Yc.Wang                                                *
 ** DATE:            2015.08.31                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2015.08.31   Yc.Wang            Create.                               *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "os_api.h"
#include "gpio_prod_api.h"
#include "gpio_prod_cfg.h"
#include "Power_cfg.h"
#include "timer_drvapi.h"
#include "zdt_hsensor.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/

#ifdef WIN32
#define __func__ 
#endif

#ifndef ZDT_DELAY_US
#define ZDT_DELAY_US TIMER_DELAY_US
#endif

#ifndef ZDT_DELAY_MS
#define ZDT_DELAY_MS hal_LcdDelay
#endif

static uint16  CT1711_DIO_GPIO_NUM = 24;
static void ct1711_dev_init(void)
{
    GPIO_CFG_INFO_T_PTR cfg_ptr = GPIO_PROD_GetCfgInfo (GPIO_PROD_TW_DIO_ID);
    if(cfg_ptr != PNULL)
    {
        CT1711_DIO_GPIO_NUM = cfg_ptr->gpio_num;
    }
    
    GPIO_SetDirection(CT1711_DIO_GPIO_NUM, 1);
    GPIO_SetValue (CT1711_DIO_GPIO_NUM,0);
    ZDT_DELAY_US(600);
    GPIO_SetValue (CT1711_DIO_GPIO_NUM,1);
}
static uint8 ct1711_read_bit_first(void)
{
    uint8  data_bit = 0;
	  GPIO_SetDirection(CT1711_DIO_GPIO_NUM, 1);
    GPIO_SetValue(CT1711_DIO_GPIO_NUM, 0);
    ZDT_DELAY_US(1);
     GPIO_SetDirection(CT1711_DIO_GPIO_NUM, 0);
    //GPIO_SetValue(CT1711_DIO_GPIO_NUM, 1);
    ZDT_DELAY_US(5); //old 20
    //GPIO_SetDirection(CT1711_DIO_GPIO_NUM, 0);
    data_bit = (uint8)GPIO_GetValue(CT1711_DIO_GPIO_NUM);
    //ZDT_DELAY_US(1);
    //GPIO_SetDirection(CT1711_DIO_GPIO_NUM, 1);
    //GPIO_SetValue(CT1711_DIO_GPIO_NUM, 1);
    ZDT_DELAY_US(40);
    return data_bit;
}
static uint8 ct1711_read_bit(void)
{
    uint8  data_bit = 0;
	  GPIO_SetDirection(CT1711_DIO_GPIO_NUM, 1);
    GPIO_SetValue(CT1711_DIO_GPIO_NUM, 0);
    ZDT_DELAY_US(1);
    GPIO_SetValue(CT1711_DIO_GPIO_NUM, 1);
    GPIO_SetDirection(CT1711_DIO_GPIO_NUM, 0);
    ZDT_DELAY_US(6); //old 20
    //GPIO_SetDirection(CT1711_DIO_GPIO_NUM, 0);
    data_bit = (uint8)GPIO_GetValue(CT1711_DIO_GPIO_NUM);
    //ZDT_DELAY_US(1);
    //GPIO_SetDirection(CT1711_DIO_GPIO_NUM, 1);
   // GPIO_SetValue(CT1711_DIO_GPIO_NUM, 1);
    ZDT_DELAY_US(40);
    return data_bit;
}

static uint8 ct1711_read_byte(void)
{
	uint8  data_byte = 0;
	int  i = 0;
	
      for (i = 8; i > 0; i--) {
          	data_byte <<= 1;
          	data_byte |= ct1711_read_bit();
      }
	return data_byte;
}

static double ct1711_read_temp_degree(void)
{
      double temp = 0.00;
	uint8 bit_cc0=0, bit_cc1=0, bit_sign=0;
	uint8 temp_byte0=0,temp_byte1=0,temp_byte2=0;
	int temp_val=0;
	
	ct1711_dev_init();
	//ZDT_DELAY_MS(150);   
	ZDT_DELAY_MS(200);        
	bit_cc0 = ct1711_read_bit_first();
	ZDT_DELAY_US(10);
	bit_cc1 = ct1711_read_bit();
	ZDT_DELAY_US(10);
	bit_cc0 = bit_cc0 & 0x01;
	bit_cc1 = bit_cc1 & 0x01;
	if ((bit_cc0 == 0x00) && (bit_cc1 == 0x00)) {
		bit_sign = ct1711_read_bit();
	  ZDT_DELAY_US(10);
	  temp_byte0 = ct1711_read_byte();
	  ZDT_DELAY_US(10);
	  temp_byte1 = ct1711_read_byte();
	  ZDT_DELAY_US(10);
                  
	  temp_val = (temp_byte0 << 8) +temp_byte1;
	  if (bit_sign == 0x01) {
	  	temp_val =~ temp_val;
	  	temp_val &= 0xFFFF;
	  	temp_val++;
            temp = -3.90625*temp_val/1000.00;
	  }
        else
        {
            temp = 3.90625*temp_val/1000.00;
        }
	}
	return temp;
}

double ct1711_get_temp_degree(void)
{
      double temp = 0.00;
      temp = ct1711_read_temp_degree();
	return temp;
}
#ifdef __cplusplus
}
#endif  // End of file


