/******************************************************************************
 ** File Name:      sc6531efm_i2c_cfg.c                                                *
 ** Author:         liuhao                                                   *
 ** DATE:           06/28/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the hal layer of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/28/2010     liuhao     Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "uix8910_reg_base.h"
#include "../../chip_module/i2c/i2c_phy.h"
#include "sc6531efm_i2c_cfg.h"
#include "gpio_prod_cfg.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Local Variables
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Global Variables
 **---------------------------------------------------------------------------*/
extern I2C_PHY_FUN phy_fun_v0;
extern I2C_PHY_FUN phy_fun_v1;

const I2C_PHY_CFG __i2c_phy_cfg[I2C_ID_MAX] =
{
    /*logic id, controller id, port id, method*/
    {0, 0, 1, &phy_fun_v0}, /*hw i2c controller0, port 1, this port is mapped the default sda0/scl0, no pad*/
    {1, 1, 1, &phy_fun_v0}, /*hw i2c controller1, port 1, this port is mapped MTCK/MTMS*/
    {2, 1, 2, &phy_fun_v0}, /*hw i2c controller1, port 2, this port is mapped iisdo/iisclk*/
    {3, 1, 3, &phy_fun_v0}, /*hw i2c controller1, port 3, this port is mapped gpio2/gpio3*/
    {4, 2, 1, &phy_fun_v1}, /*sw simulation i2c controller2, port 1, and its gpio logical numa are GPIO_PROD_I2C_SDA1/SCL1 */
    {5, 3, 1, &phy_fun_v1}  /*sw simulation i2c controller3, port 1, and its gpio logical numa are GPIO_PROD_I2C_SDA2/SCL2 */
};//lint !e785

const I2C_BASE_INFO __i2c_base_info[I2C_BUS_MAX] =
{
    /*hw controller id, base address*/
    {0, I2C_BASE}, /*hw i2c controller1, register base*/
    {1, I2C1_BASE},/*hw i2c controller2, register base*/
    {0},           /* null */ 
    {0},           /* null */ 
};

const I2C_GPIO_INFO __i2c_gpio_info[I2C_BUS_MAX] =
{
    /*sw controller id, sda pin, scl pin*/
    {2, GPIO_PROD_I2C_SDA1, GPIO_PROD_I2C_SCL1}, /*sw simulation i2c controller1, gpio pin*/
    {3, GPIO_PROD_I2C_SDA2, GPIO_PROD_I2C_SCL2}, /*sw simulation i2c controller2, gpio pin*/
    {0}, /* null */ 
    {0}, /* null */ 
};

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

/*  End Of File */
