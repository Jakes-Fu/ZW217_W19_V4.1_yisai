/******************************************************************************
 ** File Name:      gpio_ext_cfg.h                                            *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           03/08/2006                                                *
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 DVB extend GPIO device.                                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/08/2006     Liangwen.Zhen     Create.                                  *
 ******************************************************************************/

#ifndef _GPIO_EXT_CFG_H
#define _GPIO_EXT_CFG_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "gpio_ext_drv.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                Basic type and interface define                            *
 **---------------------------------------------------------------------------*/  
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

// PORT
typedef struct
{
	uint8 port[2];
}PORT_T;

// PCA9535 IC info
typedef struct
{
	uint8 	ic_num;
	uint8 	i2c_addr_w;
	uint8 	i2c_addr_r;	
	PORT_T  input;
}GPIO_EXT_DEV_INFO_T, *GPIO_EXT_DEV_INFO_T_PTR;

typedef struct
{
	uint16 		gpio_num;
	BOOLEAN		b_used;
	BOOLEAN  	direction;
	BOOLEAN		default_value;
	GPIO_EXT_CALLBACK_PFUNC		gpio_callback;
}GPIO_EXT_GPIO_INFO_T, *GPIO_EXT_GPIO_INFO_T_PTR;

typedef struct
{
	uint32						int_gpio_num;
	GPIO_EXT_DEV_INFO_T_PTR  	dev_tab_ptr;  
	GPIO_EXT_GPIO_INFO_T_PTR	gpio_tab_ptr;	
	
}GPIO_EXT_CFG_INFO_T, *GPIO_EXT_CFG_T_INFO_PTR;

/**---------------------------------------------------------------------------*
 **                       Public Function Definitions                         *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is used to get customer configure information    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC GPIO_EXT_CFG_T_INFO_PTR GPIO_EXT_GetCusCfgInfo(void);

 /**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/   
#ifdef __cplusplus
}
#endif

#endif  // _GPIO_EXT_CFG_H

// End of gpio_ext_cfg.h

