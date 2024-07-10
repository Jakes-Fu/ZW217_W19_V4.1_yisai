/******************************************************************************
 ** File Name:      spload_main                                                *
 ** Author:           tao.feng                                              *
 ** DATE:            16/09/2010                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME             DESCRIPTION                               *
 ** 16/09/2010     tao.feng          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "stdarg.h"
#include "sci_types.h"
#include "lcm_cfg.h"
#include "spload_cfg.h"
#include "spload_main.h"
#include "sensor_cfg.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
uint32 lcm_heap_addr[128];
LCD_SENSOR_EXT_FUN_TABLE_S *s_lcm_func_table = NULL;

/**---------------------------------------------------------------------------*
 **                         extern Variables                                *
 **---------------------------------------------------------------------------*/
extern uint8 Image$$RAM_RW$$Base;
extern uint8 Image$$RAM_RW$$Length;
extern uint8 Load$$RAM_RW$$Base;
extern uint8 Image$$RAM_ZI$$Base;
extern uint8 Image$$RAM_ZI$$Length;
extern uint8 Load$$RAM_ZI$$Base;

/*****************************************************************************/
//  Function name:  main
//  Description:    used only for compiler and linker.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC int main(void)
{
	LCM_GetFuncTableAddr();
	LCM_GetCfgInfo(0,0);
}

/*****************************************************************************/
//  Function name:  LCM_GetFuncTableAddr
//  Description:      get lcd extend func entry address
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC uint32 LCM_GetFuncTableAddr(void) 
{
	return 0x1000;//##!important notice: be sure not to modify this line, perl script may modify this line.
}

/*****************************************************************************/
//  Function name:  _lcm_getcfginfotable
//  Description:   this function used to get LCM configure information,will be called by main code.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL LCM_CFG_INFO_T_PTR _lcm_getcfginfotable(uint32 lcd_id, uint32* count)
{
	return LCM_GetCfgInfo(lcd_id, count);
}

/*****************************************************************************/
//  Function name:  _lcm_sensor_copyrw
//  Description:   this function used to copy rw section from load region to execute region,will be called by main code.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void _lcm_sensor_copyrw(void)
{
	uint32 cnt ;
	uint32 len = (uint32)&Image$$RAM_RW$$Length;
	uint32 img_base = (uint32)&Image$$RAM_RW$$Base;
	uint32 load_base = (uint32)&Load$$RAM_RW$$Base;
	for(cnt=0x0; cnt<len; cnt++) 
	{
		*(uint8*)(img_base + cnt)= *(uint8*)(load_base + cnt);
	}
}

/*****************************************************************************/
//  Function name:  _lcm_sensor_initzi
//  Description:   this function used to init zi section, will be called by main code.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void _lcm_sensor_initzi(void)
{
	uint32 cnt ;
	uint32 len = (uint32)&Image$$RAM_ZI$$Length;
	uint32 img_base = (uint32)&Image$$RAM_ZI$$Base;
	for(cnt=0x0; cnt<len; cnt++) 
	{
		*(uint8*)(img_base + cnt)= 0;
	}
}

/*****************************************************************************/
//  Function name:  _lcm_sensor_init
//  Description:   this function used to do lcm init
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void _lcm_sensor_init(void)
{
	s_lcm_func_table = (LCD_SENSOR_EXT_FUN_TABLE_S*)LCM_GetFuncTableAddr();
}

/*spload code output function list, will be called by main code*/
#pragma arm section rodata = "LCM_SENSOR_EXT_FUNC"
LOCAL  const LCM_SENSOR_EXT_FUN_TABLE_S _lcd_sensor_ext_func=
{
	_lcm_getcfginfotable,
	_lcm_sensor_copyrw,
	_lcm_sensor_initzi,
	_lcm_sensor_init,
	Sensor_SelectSensorFormat,
    Sensor_GetSensorFormat,
	Sensor_GetInforTab,
	Sensor_GetInforTabLenght,
}; 
#pragma arm section code


/*****************************************************************************/
//  Function name:	LCM_Sensor_GetExtFunc
//  Description:this function only used for compile and link.
//  Global resource dependence: 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 LCM_Sensor_GetExtFunc(void)
{
	return (uint32)&_lcd_sensor_ext_func;
}


#ifdef   __cplusplus
    }
    
#endif  // End of spload_main.c

