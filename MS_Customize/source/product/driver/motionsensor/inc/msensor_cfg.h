/******************************************************************************
 ** File Name:      msensor_drv.h                                             *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           08/31/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of Motion*
 **                 Sensor.                                                   *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/31/2009     Liangwen.Zhen    Create.                                   *
 ******************************************************************************/

#ifndef _MSENSOR_CFG_H_
#define _MSENSOR_CFG_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Include file                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "i2c_api.h"
#include "msensor_drv.h"
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef enum
{
	MSENSOR_STATUS_NOT_INIT = 0,
	MSENSOR_STATUS_INIT,
	MSENSOR_STATUS_OPEN,	
	MSENSOR_STATUS_CLOSE,
	MSENSOR_STATUS_MAX
}MSENSOR_STATUS_E;

// the real placement of msensor, where is soldered in PCB
// it used to auto re-calculate the event from chip to reflect the real motion

/* MSENSOR_PIN1_AT_LEFT_UP and MSENSOR_LCD_FACE_SAME( or REVERSE)
  |-------------|
  |*(pin1)      |   
  |             |
  |  CHIP       |
  |_____________|

*/

typedef enum
{
	MSENSOR_LCD_FACE_SAME = 0,  // chip and main screen are face the same direct
	MSENSOR_LCD_FACE_REVERSE    // chip is solder at reversed side of the scree
}MSENSOR_LCD_PLACEMENT_E;

typedef enum
{
	MSENSOR_PIN1_RIGHT_UP = 0,   // pin1 of chip is at right-up position.
	MSENSOR_PIN1_RIGHT_DOWN,     // at rithg-donw,  90 degrees clockwise
	MSENSOR_PIN1_LEFT_DONW,      // at left-down,  180 degrees clockwise
	MSENSOR_PIN1_LEFT_UP         // at left-up,    270 degrees clockwise
}MSENSOR_PCB_PLACEMENT_E;

typedef struct
{
    const char *            name;               // name of msensor
	MSENSOR_TYPE_E  	    type;               // types of msensor
    MSENSOR_STATUS_E        status;             // statuses of msensor
	uint32  				(*init)(void);      // point of init function
	uint32  				(*open)(void);      // point of open function
	uint32  				(*close)(void);     // point of close function
	uint32  				(*ioctl)(uint32, void*); // point of ioctl function
}MSENSOR_CFG_T, *MSENSOR_CFG_T_PRT;


typedef struct //msensor_misc_cfg_tag
{
	I2C_DEV  	            i2c_cfg;        // i2c config of BB to control msensor
    MSENSOR_LCD_PLACEMENT_E lcd_face;       // face lcd the same or reverse
    MSENSOR_PCB_PLACEMENT_E pin1;           // pin1 at PCB placement
}MSENSOR_MISC_CFG_T, *MSENSOR_MISC_CFG_T_PRT;

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is used to get motion sensor information table    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC MSENSOR_CFG_T_PRT * MSensor_GetCfgTab(void);

/*****************************************************************************/
//  Description:    This function is used to get motion sensor information table length   
//  Author:         Tim.Zhu
//  Note:           
/*****************************************************************************/
PUBLIC uint32 MSensor_GetCfgTabLength(void);

/*****************************************************************************/
//  Description:  get useful configuration
//  Note:      msensor_ptr: in;    msensor_misc_cfg_ptr: out  
/*****************************************************************************/
PUBLIC void MSensor_GetMiscCfg(
    MSENSOR_CFG_T_PRT msensor_ptr, 
    MSENSOR_MISC_CFG_T_PRT msensor_misc_cfg_ptr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _MSENSOR_DRV_H_

// End of msensor_drv.h
