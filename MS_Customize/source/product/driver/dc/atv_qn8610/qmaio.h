/************************************************Copyright(c)***************************************************
**                                   Quintic(Beijing) Microelectronics Co,Ltd.
**                                   
**                                   http://www.quinticcorp.com 
**
**--------------------File Info----------------------------------------------------------------
** File Name:       qmaio.h
** Last Modified Date: 2009-
** Last Version:     0.1
** Descriptions: The qmaio.h is used to driver I2C interface
**
**----------------------------------------------------------------------------------------
** Created by:   ws
** Created Date: 2009-08-26
** Version:      0.1
** Descriptions: 
**
**-------------------------------------------------------------------------------------
** Modified by:
** Descriptions:   QN8610 A1 version
**
**************************************************************************************************/
#ifndef _QMAIO_H__
#define _QMAIO_H__
#include "qmadriver.h"
#include "qn8610i2c.h"
/* modify this according to I2C device address when you use standard I2C function like I2C_XXXX except AI2C_XXXX*/
#define I2C_DEV0_ADDRESS 0x54
#define I2C_TIMEOUT_TIME    10
#define I2C_TIMEOUT_COUNT    8
#define MS_DELAY_CONST    40
#define QN8610_SCL 66
#define QN8610_SDA 67
#define QN8610_RST 17
#define QN8610_CE 16



/************end*********************/
#if 0
#define SCL_1   GPIO_SetValue(QN8610_SCL, 1)
#define SCL_0   GPIO_SetValue(QN8610_SCL, 0)

#define SDA_1  GPIO_SetValue(QN8610_SDA, 1)
#define SDA_0  GPIO_SetValue(QN8610_SDA, 0)

#define Read_SDA      GPIO_GetValue(QN8610_SDA)
#define Set_Datain    GPIO_SetDirection(QN8610_SDA,0)
#define Set_Dataout   GPIO_SetDirection(QN8610_SDA,1)
#endif

#if 0
#define SCL_1   GPIO_SetValue_Fast(QN8610_SCL, 1)
#define SCL_0   GPIO_SetValue_Fast(QN8610_SCL, 0)

#define SDA_1  GPIO_SetValue_Fast(QN8610_SDA, 1)
#define SDA_0  GPIO_SetValue_Fast(QN8610_SDA, 0)

#define Read_SDA      GPIO_GetValue_Fast(QN8610_SDA)
#define Set_Datain    GPIO_SetDirection_Fast(QN8610_SDA,0)
#define Set_Dataout   GPIO_SetDirection_Fast(QN8610_SDA,1)
#endif

/***************************************************************************************************/

extern void Msdelay(UINT16 dly);
extern UINT8 QMA_I2C_WRITE(UINT8 Regis_Addr,UINT8 Data);
extern UINT8 QMA_I2C_READ(UINT8 Regis_Addr, UINT8 *Regis_Data);
extern UINT8 QMA_I2C_NREAD(UINT8 Regis_Addr, UINT8 *buf, UINT8 n);
extern UINT8 QMA_I2C_NWRITE(UINT8 Regis_Addr, UINT8 *buf, UINT8 n);

#if 0
extern UINT8 QMA_ReadReg(UINT8 adr);
extern UINT8 QMA_WriteReg(UINT8 adr, UINT8 value); 
#endif //add in I2Csample.c
#endif // _QMAIO_H__
