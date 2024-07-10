/******************************************************************************
 ** File Name:      global_ctl_drvapi.h                                             *
 ** Author:         Yong.Li                                                  *
 ** DATE:           12/22/2011                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic function for ldo management.  *
 ******************************************************************************/

#ifndef _GLOBAL_CTL_DRVAPI_H_
#define _GLOBAL_CTL_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

// global module type define
typedef enum{
	MODULE_TYPE_SIM = 0,
	MODULE_TYPE_SPI,
	MODULE_TYPE_UART,
	MODULE_TYPE_I2C,
	MODULE_TYPE_MAX
}MODULE_TYPE_E;

// global module define
typedef enum{
	SIM_MODULE_START = MODULE_TYPE_SIM << 8,
	SIM0,
	SIM1,
	SIM_MODULE_END,
	
	SPI_MODULE_START = MODULE_TYPE_SPI << 8,
	SPI0,
	SPI1,
	SPI_MODULE_END,
	
	UART_MODULE_START = MODULE_TYPE_UART << 8,
	UART0,
	UART1,
	UART_MODULE_END,
	
	I2C_MODULE_START = MODULE_TYPE_I2C << 8,
	I2C0,
	I2C1,
	I2C_MODULE_END,
	
	GLB_MODULE_MAX = 0x7FFFFFFF
}GLB_MODULE_E;

// global ctl cmd define
typedef enum{
	GLB_CMD_PORT_SEL = 0,
	
	GLB_CMD_MAX
}GLB_CMD_E;

/*****************************************************************************/
//  Description:  Global ctl port select
//  Global resource dependence:
//  Author: Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GlB_CTL_PORT_SEL (GLB_MODULE_E module, uint32 port_num);

/*****************************************************************************/
//  Description:  Global Ctl
//  Global resource dependence:
//  Author: Yong.Li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Global_CTL (GLB_MODULE_E module, GLB_CMD_E cmd, uint32 *arg);


#ifdef   __cplusplus
extern   "C"
{
#endif


#endif

