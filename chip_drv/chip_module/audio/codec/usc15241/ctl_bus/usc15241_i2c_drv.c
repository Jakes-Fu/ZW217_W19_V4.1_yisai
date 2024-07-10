/******************************************************************************
 ** File Name:      usc15241_spi_drv.c                                                 *
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
#ifdef MINICODE_TEST
#define OLD_API_INTERFACE
#endif

#include "sci_types.h"
#include "os_api.h"

#include "chip_plf_export.h"

#ifdef OLD_API_INTERFACE
#include "i2c_drv.h"
#else
#include "i2c_drvapi.h"
#endif
#include "usc15241_i2c_drv.h"

#define I2C_FREQ_100K (100*1000)
#define I2C_FREQ_400K (400*1000)

#define I2C_USC15241_ADDR (0xAA)
#define I2C_USC15241_ACK 1
#define I2C_USC15241_NACK 0

#ifndef OLD_API_INTERFACE
LOCAL I2C_DEV USC15241_I2C_cfg =
{
    I2C_USC15241_PORT,       //id
    I2C_FREQ_100K,          //freq
    0,                      //bus
    I2C_USC15241_ADDR,       //slave_addr
    1,                      //reg_addr_num
    I2C_USC15241_ACK,        //check_ack
    1                       // no_stop
};
#endif

LOCAL int32 USC15241_I2C_Handler = -1;


PUBLIC void __usc15241_i2c_init (void)
{
#ifdef OLD_API_INTERFACE
    uint32 i=0;
    //IIC Soft Reset
    * (volatile uint32 *) (APB_RST0_SET) = I2C_SOFT_RST_SET;

    for (i = 0; i < 10; i++);

    * (volatile uint32 *) (APB_RST0_CLR) = I2C_SOFT_RST_CLR;

    I2C_Init (I2C_FREQ_100K,I2C_USC15241_PORT);
#else

    while (USC15241_I2C_Handler == -1)
    {
        USC15241_I2C_Handler = I2C_HAL_Open (&USC15241_I2C_cfg);
    }

#endif
}

PUBLIC void __usc15241_i2c_deinit (void)
{
#ifdef OLD_API_INTERFACE
    uint32 i=0;
    //IIC Soft Reset
    * (volatile uint32 *) (APB_RST0_SET) = I2C_SOFT_RST_SET;

    for (i = 0; i < 10; i++);

    * (volatile uint32 *) (APB_RST0_CLR) = I2C_SOFT_RST_CLR;
#else

    if (USC15241_I2C_Handler != -1)
    {
        I2C_HAL_Close (USC15241_I2C_Handler);
        USC15241_I2C_Handler = -1;
    }

#endif
}

PUBLIC void __i2c_reg_w (uint8 reg,uint8 val)
{
    uint8 cmd_buf[2];
    cmd_buf[0] = reg;
    cmd_buf[1] = val;
#ifdef OLD_API_INTERFACE
    I2C_WriteCmdArrEx (I2C_USC15241_ADDR,cmd_buf,2,I2C_USC15241_ACK,0);
#else
    I2C_HAL_Write (USC15241_I2C_Handler, &cmd_buf[0], &cmd_buf[1], 1);
#endif
}

PUBLIC uint8 __i2c_reg_r (uint8 reg)
{
    uint8 cmd_buf[2];
    cmd_buf[0] = reg;
#ifdef OLD_API_INTERFACE
    I2C_WriteCmdArrNoStopEx (I2C_USC15241_ADDR,cmd_buf,1,I2C_USC15241_ACK,0);
    I2C_ReadCmdArrEx (I2C_USC15241_ADDR, cmd_buf, 1, I2C_USC15241_ACK, 0);
    return cmd_buf[0];
#else
    I2C_HAL_Read (USC15241_I2C_Handler, &cmd_buf[0], &cmd_buf[1], 1);
    return cmd_buf[1];
#endif
}


