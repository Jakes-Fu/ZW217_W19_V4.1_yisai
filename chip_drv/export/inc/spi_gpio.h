/******************************************************************************
 ** File Name:      spi_gpio.h                                                 *
 ** Author:         Wuding.Yang                                               *
 ** Date:           2010.2                                                    *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This is the gpio  file for spi protocol           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2010.22        yun.wang     Create                                    *
 ** 2010.11.29       wuding.yang   porting to 6800H                    *
 ******************************************************************************/
#ifndef _SPI_GPIO_H_
#define _SPI_GPIO_H_
#ifdef   __cplusplus
extern   "C"
{
#endif

#include "spi_drvapi.h"

#define DEFULT_SPI_CLK_FREQ     200000

typedef enum
{
    VIR_SPI0_CLK_PIN_ID = 0,
    VIR_SPI0_DIN_PIN_ID,
    VIR_SPI0_DOUT_PIN_ID,
    VIR_SPI0_CS_PIN_ID,
    VIR_SPI1_CLK_PIN_ID,
    VIR_SPI1_DIN_PIN_ID,
    VIR_SPI1_DOUT_PIN_ID,
    VIR_SPI1_CS_PIN_ID
} GPIO_SPI_PIN_ID_E;

typedef struct
{
    uint32 gpio_id;
    uint32 gpio_addr;
    uint32 gpio_bitoffset;
} GPIO_SPI_PIN_CFG_T;

typedef struct
{
    GPIO_SPI_PIN_CFG_T sck_pin;
    GPIO_SPI_PIN_CFG_T scs_pin;
    GPIO_SPI_PIN_CFG_T sdout_pin;
    GPIO_SPI_PIN_CFG_T sdin_pin;
} GPIO_SPI_PIN_T;

typedef struct
{
    GPIO_SPI_PIN_T gpio_spi_pin;
    //SPI_PORT_T *spi_port_ptr;
    BOOLEAN sck_reverse;
    BOOLEAN tx_shift_edge;
} GPIO_SPI_PORT_T;

/* ����Ϊ6302ʹ�õ�Ӳ����������,�û��ɸ���ʵ����Ҫ�޸� */



#define GPIO_DATA                       0x0000//GPIO data register
#define GPIO_DMSK                   0x0004//GPIO data mask register, GPIO pin can be read and write if the mask bit is "1"
#define GPIO_DIR                        0x0008//"1" configure pin to be output"0" configure pin to be input

/* SCK�������ӵ���TP_XR */
#define  SPI_SCK_GPIO(addr,bitoffset)           *(volatile uint32 *)((addr)+GPIO_DMSK) |= (0x01 << (bitoffset))           /* ����GPIO�� MASK �ɶ�*/
#define  SPI_SCK_OUT(addr,bitoffset)            *(volatile uint32 *)((addr)+GPIO_DIR) |= (0x01 << (bitoffset))       //GPIO_SetDirection(SPI_SCK, 1)//IODIR |= SPI_SCK              /* ���� SCK ��Ϊ����� */
#define  SPI_SCK_SET(addr,bitoffset)            *(volatile uint32 *)((addr)+GPIO_DATA) |= (0x01 << (bitoffset))       //GPIO_SetValue(SPI_SCK,1)//IOCLR = SPI_SCK   
#define  SPI_SCK_CLR(addr,bitoffset)            *(volatile uint32 *)((addr)+GPIO_DATA) &= ~(0x01UL << (bitoffset))    //GPIO_SetValue(SPI_SCK,0)//IOCLR = SPI_SCK                   /* �� SCK Ϊ�͵�ƽ */   

/* SPI_DIN ����,����6302 ��SYSRSTB�ܽ����ӵ���TP_YU*/

#define  SPI_DIN_GPIO(addr,bitoffset)              *(volatile uint32 *)((addr)+GPIO_DMSK) |= (0x01 << (bitoffset))           /* ����GPIO�� MASK �ɶ�*/
#define  SPI_DIN_OUT(addr,bitoffset)                 *(volatile uint32 *)((addr)+GPIO_DIR) |=(0x01 << (bitoffset))        //GPIO_SetDirection(SPI_DIN,1)                /* ���� DIN ��Ϊ����� */
#define  SPI_DIN_IN(addr,bitoffset)                     *(volatile uint32 *)((addr)+GPIO_DIR)  &= ~(0x01UL << (bitoffset))   //GPIO_SetDirection(SPI_DIN,0)
#define  SPI_DIN_SET(addr,bitoffset)                  *(volatile uint32 *)((addr)+GPIO_DATA) |= (0x01 << (bitoffset))      //GPIO_SetValue(SPI_DIN,1)               /* �� DIN Ϊ�ߵ�ƽ */
#define  SPI_DIN_CLR(addr,bitoffset)                  *(volatile uint32 *)((addr)+GPIO_DATA) &= ~(0x01UL << (bitoffset))   //GPIO_SetValue(SPI_DIN,0)               /* �� DIN Ϊ�͵�ƽ */
#define  SPI_DIN_READ(addr,bitoffset)               (*(volatile uint32 *)((addr)+GPIO_DATA)&(0x01 << (bitoffset)))       //GPIO_GetValue(SPI_DIN)       

/* SPI_DOUT �������ӵ���TP_XL */
#define  SPI_DOUT_GPIO(addr,bitoffset)       *(volatile uint32 *)((addr)+GPIO_DMSK) |= (0x01 << (bitoffset))           /* ����GPIO�� MASK �ɶ�*/
#define  SPI_DOUT_OUT(addr,bitoffset)           *(volatile uint32 *)((addr)+GPIO_DIR) |= (0x01 << (bitoffset))     /* ���� DATA��Ϊ����� */
#define  SPI_DOUT_SET(addr,bitoffset)           *(volatile uint32 *)((addr)+GPIO_DATA) |= (0x01 << (bitoffset))     /* ���� DATA��Ϊ���Ϊ�ߵ�ƽ*/
#define  SPI_DOUT_CLR(addr,bitoffset)           *(volatile uint32 *)((addr)+GPIO_DATA) &= ~(0x01UL << (bitoffset))   /* ���� DATA��Ϊ���Ϊ�͵�ƽ*/

/* CS ���� ���ӵ���TP_YD*/
#define  SPI_CS_GPIO(addr,bitoffset)            *(volatile uint32 *)((addr)+GPIO_DMSK) |= (0x01 << (bitoffset))                 /* ����GPIO�� MASK �ɶ�*/
#define  SPI_CS_OUT(addr,bitoffset)         *(volatile uint32 *)((addr)+GPIO_DIR) |= (0x01 << (bitoffset))              /* ���� CS ��Ϊ����� */
#define  SPI_CS_SET(addr,bitoffset)         *(volatile uint32 *)((addr)+GPIO_DATA) |= (0x01 << (bitoffset))             /* �� CS Ϊ�ߵ�ƽ */
#define  SPI_CS_CLR(addr,bitoffset)         *(volatile uint32 *)((addr)+GPIO_DATA) &= ~ (0x01UL << (bitoffset))           /* �� CS Ϊ�͵�ƽ */

#ifdef   __cplusplus
}
#endif
#endif

