/******************************************************************************
 ** File Name:      usc15241_spi_drv.h                                                 *
 ** Author:         ken.kuang                                                 *
 ** DATE:           09/08/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    usc15241 control i2c bus driver                            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/08/2011     ken.kuang          Create.                                 *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifndef __I2C_DRV_USC15241_H___
#define __I2C_DRV_USC15241_H___


#define I2C_USC15241_PORT 1

PUBLIC void __usc15241_i2c_init(void);

PUBLIC void __usc15241_i2c_deinit (void);

PUBLIC void __i2c_reg_w(uint8 reg,uint8 val);

PUBLIC uint8 __i2c_reg_r(uint8 reg);


#endif
