/******************************************************************************
 ** File Name:      usc15241_spi_drv.h                                                 *
 ** Author:         ken.kuang                                                 *
 ** DATE:           09/08/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    usc15241 control spi bus driver                            *
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
#ifndef __SPI_DRV_USC15241_H___
#define __SPI_DRV_USC15241_H___

#define ARM_GPIO_BASE_CNT           (0)

#define SPI_USC15241_SCK_GPIO_PIN       0
#define SPI_USC15241_SEN_GPIO_PIN       1
#define SPI_USC15241_DAT_GPIO_PIN       2

#define SPI_USC15241_SCK_GPIO_MASK       (1<<(SPI_USC15241_SCK_GPIO_PIN-ARM_GPIO_BASE_CNT))
#define SPI_USC15241_SEN_GPIO_MASK       (1<<(SPI_USC15241_SEN_GPIO_PIN-ARM_GPIO_BASE_CNT))
#define SPI_USC15241_DAT_GPIO_MASK       (1<<(SPI_USC15241_DAT_GPIO_PIN-ARM_GPIO_BASE_CNT))


PUBLIC void __usc15241_spi_init(void);

PUBLIC void __spi_reg_w(uint32 reg,uint32 val);

PUBLIC uint32 __spi_reg_r(uint32 reg);

#endif

