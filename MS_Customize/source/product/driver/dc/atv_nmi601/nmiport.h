/******************************************************************************
**
**	Copyright (c) Newport Media Inc.  All rights reserved.
**
** 	Module Name:  nmiport.h
**	
**		This module implements the porting interface for the NMI ATV driver.
**		It can be used as an example for the driver porting.      
**
** 
*******************************************************************************/

#ifndef _NMI_PORT_H_
#define _NMI_PORT_H_

/******************************************************************************
**
**	Platform/Os Header Files
**
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "nmitypes.h"


/******************************************************************************
**
**	Nmi Function Prototype (for porting) 
**
*******************************************************************************/
#ifndef ATV_USE_HW_I2C
//set gpio pin as gpio mode
void NMI_SET_GPIO_MODE_ENABLE(unsigned char pin);
//set gpio direction, dir=0: input, dir=1:output
void NMI_SET_GPIO_DIR(unsigned char pin,unsigned char dir);
//set gpio high(level=1) or low(level=0)
void NMI_SET_GPIO_LEVEL(unsigned char pin,unsigned char level);     
//set gpio pin pullup & pulldown disable
void NMI_SET_GPIO_PULL_DISABLE(unsigned char pin); 
//get gpio level
unsigned char NMI_GET_GPIO_LEVEL(unsigned char pin);
#endif          
void nmi_power_up(void);
void nmi_power_down(void);
int nmi_i2c_init(void);
void nmi_i2c_deinit(void);
int nmi_i2c_read(unsigned char, unsigned char *, unsigned long);
int nmi_i2c_write(unsigned char, unsigned char *, unsigned long); 
void nmi_os_delay(unsigned long);
uint32_t nmi_os_read_32bit_register(uint32_t adr);
unsigned long nmi_os_get_tick(void);
void nmi_os_print(char *);
void *nmi_os_create_mutex(void);
void nmi_os_lock_mutex(void *);
void nmi_os_unlock_mutex(void *);
void nmi_os_del_mutex(void *);
void nmi_poll_start(void);
void nmi_poll_stop(void);
#endif
