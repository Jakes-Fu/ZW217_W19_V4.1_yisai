/******************************************************************************
 ** File Name:      spload_api.c                                                 *
 ** Author:           tao.feng                                              *
 ** DATE:            01/15/2008                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of LCM   *
 **					about product
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME             DESCRIPTION                               *
 ** 10/28/2010     tao.feng         Create.                         *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "tb_hal.h"
#include "dal_lcd.h"
#include "lcm_prod.h"
#include "lcm_cfg.h"
#include "i2c_drvapi.h"
#include "chip.h"
#include "sensor_drv.h"
#include "spload_cfg.h"
#include "spload_api.h"
#include "gpio_prod_api.h"
#include "flash.h"


#define DEBUG_SPLOAD_API
#ifdef  DEBUG_SPLOAD_API
	#define LCM_SENSOR_TRACE   SCI_TRACE_LOW 
#else
	#define LCM_SENSOR_TRACE( _format_string )  
#endif

/*----------------------------------------------------------------------------*
**                         Extern  Function Prototype                           *
**---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
LCM_SENSOR_EXT_FUN_TABLE_S *s_lcm_ext_func_table = NULL;

#pragma arm section zidata = "LCD_SENSOR_EXT_DATA"
uint8 lcd_sensor_heap[0x4000] = {0x0};    /*lint !e552*/
#pragma arm section zidata

/*main code output function list, will be called by spload code*/
#pragma arm section rodata = "LCD_SENSOR_EXT_FUNC"
LOCAL const LCD_SENSOR_EXT_FUN_TABLE_S _lcd_sensor_ext_func=
{
    /*lcd opeartions*/
	LCD_SendCmd,
	LCD_SendData,
	LCD_SendCmdData,
	LCD_Reset,
	LCD_Delayms,
	NULL,
	LCD_ReadRegVal,

    /*system operations*/
	LCD_Sensor_Trace,
	SCI_Assert,  /*assert verified*/
	LCD_Sensor_Asseert,
	SCI_Sleep,

    /*i2c operations*/
	I2C_HAL_Write,
	I2C_HAL_Read,
	
    /*sensor operations*/
	Sensor_ReadReg,
	Sensor_WriteReg,
	Sensor_SetMCLK,
	Sensor_SetVoltage,
	Sensor_SetResetLevel,
	Sensor_SetSensorExifInfo,
	Sensor_SetCurId,
	Sensor_GetCurId,
	Sensor_SetMode,
	Sensor_GetI2cHnadler,
    Sensor_SetInfo,

	GPIO_SetSensorPwdn,
	GPIO_SetFrontSensorPwdn,
	
	CHIP_DetectMemType,
}; 
#pragma arm section code

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Function name:	LCD_Sensor_Trace
//  Description:this function will be called by spload code
//  Global resource dependence: 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void LCD_Sensor_Trace(const char * x_format, ...)
{
	SCI_TRACE_LOW(x_format);
}

/*****************************************************************************/
//  Function name:	LCD_Sensor_Asseert
//  Description:this function will be called by spload code
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC void LCD_Sensor_Asseert(int exp)
{
	SCI_ASSERT(exp); /*assert verified*/
}

/*****************************************************************************/
//  Function name:	LCD_GetExtFunc
//  Description:this function used only for link.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC LCD_SENSOR_EXT_FUN_TABLE_S *LCD_Sensor_GetExtFuncTable(void)
{
     return (LCD_SENSOR_EXT_FUN_TABLE_S *)(&_lcd_sensor_ext_func);  
}

/*****************************************************************************/
//  Function name:	LCD_GetExtLcdStackAddr
//  Description:this function used only for link.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC uint32 LCD_Sensor_GetExtStackAddr(void)
{
      return (uint32)((uint32*)&lcd_sensor_heap[0]); 
}

/*****************************************************************************/
//  Function name:	LCD_GetExtFunc
//  Description:this function get the pointer of the lcd bin extern function entry address
//  Global resource dependence: 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32  LCD_Sensor_GetExtFuncTableAddr(void)
{
	 return (uint32)(FLASH_GetSperateLoadAddr() + 0x200);  
}

 /*****************************************************************************/
//  Function name:	LCM_Sensor_SperateBin_Init
//  Description:This function is used to init seperate lcd  sensor driver
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC void LCM_Sensor_SperateBin_Init(void)
{
	LCD_Sensor_GetExtFuncTable();//only for compile!!!
	LCD_Sensor_GetExtStackAddr();//only for compile!!!
	s_lcm_ext_func_table = (LCM_SENSOR_EXT_FUN_TABLE_S *)LCD_Sensor_GetExtFuncTableAddr();
	s_lcm_ext_func_table->_lcm_sensor_copyrw();
	s_lcm_ext_func_table->_lcm_sensor_initzi();
	s_lcm_ext_func_table->_lcm_sensor_init();
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of lcm_prod.c
