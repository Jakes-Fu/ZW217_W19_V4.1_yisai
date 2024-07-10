#ifndef _SPLOAD_API_H_
#define _SPLOAD_API_H_
#include "spload_cfg.h"
#include "os_api.h"

extern LCM_SENSOR_EXT_FUN_TABLE_S *s_lcm_ext_func_table ;

/*****************************************************************************/
//  Function name:	LCD_Sensor_Trace
//  Description:this function will be called by spload code
//  Global resource dependence: 
//  Author:  tao.feng
//  Note:
/*****************************************************************************/
PUBLIC void LCD_Sensor_Trace(const char * x_format, ...);

/*****************************************************************************/
//  Function name:	LCD_Sensor_Asseert
//  Description:this function will be called by spload code
//  Global resource dependence: 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void LCD_Sensor_Asseert(int exp);

/*****************************************************************************/
//  Function name:	LCD_GetExtFunc
//  Description:this function used only for link.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC LCD_SENSOR_EXT_FUN_TABLE_S *LCD_Sensor_GetExtFuncTable(void);


/*****************************************************************************/
//  Function name:	LCD_GetExtLcdStackAddr
//  Description:this function used only for link.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC uint32 LCD_Sensor_GetExtStackAddr(void);


/*****************************************************************************/
//  Function name:	LCD_GetExtFunc
//  Description:this function get the pointer of the lcd bin extern function entry address
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC uint32  LCD_Sensor_GetExtFuncTableAddr(void);


 /*****************************************************************************/
//  Description:   This function is used to init seperate lcd driver
//  Author:         tao.feng
//  Note:           
/*****************************************************************************/
PUBLIC void LCM_Sensor_SperateBin_Init(void);

#endif // _SPLOAD_API_H_

