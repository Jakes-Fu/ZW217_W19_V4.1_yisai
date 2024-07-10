/******************************************************************************
 ** File Name:      sc6531efm_spi_cfg.c                                                *
 ** Author:         liuhao                                                   *
 ** DATE:           12/20/2010                                                 *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the hal layer of spi device.      *
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
#include "../../chip_module/spi/spi_phy.h"
#include "../../chip_module/pin/SC6531efm/pin_reg_sc6531efm.h"
#include "sc6531efm_spi_cfg.h"
#include "../../chip_module/spi/v5/spi_reg_v5.h"
#include "../../chip_plf/common/inc/chip_drv_common_io.h"

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

extern SPI_PHY_FUN spi_phy_fun_v5;

#ifdef GPIO_SIMULATE_SPI_SUPPORT
extern SPI_PHY_FUN spi_phy_fun_simu;
#endif
/*lint -save -e785 */	
const SPI_PHY_CFG __spi_phy_cfg[SPI_ID_MAX] =   //only one cs line
{
    /*logic id, controller id, cs id, method*/
    {SPI0_0_CS0, SPI_BUS_ID_0, SPI_CS_ID_0, SPI_PIN_GID_0, &spi_phy_fun_v5}, /*hw spi controller1, cs 0*/
    //{SPI0_0_CS1, SPI_BUS_ID_0, SPI_CS_ID_1, SPI_PIN_GID_0, &spi_phy_fun_v5}, /*hw spi controller1, cs 1*/
#ifdef GPIO_SIMULATE_SPI_SUPPORT
    {SPI1_0_CS0, SPI_BUS_ID_1, SPI_CS_ID_0, SPI_PIN_GID_0, &spi_phy_fun_v5}, /*hw spi controller1, cs 1*/
#else
    {SPI1_0_CS0, SPI_BUS_ID_1, SPI_CS_ID_0, SPI_PIN_GID_0, &spi_phy_fun_v5},
    {SPI1_0_CD,  SPI_BUS_ID_1, SPI_CS_ID_0, SPI_PIN_GID_1, &spi_phy_fun_v5},
#endif
};
/*lint -restore*/
PUBLIC SPI_BASE_INFO __spi_base_info[SPI_BUS_MAX] =
{
    /*hw controller id, base address, tx chanel, rx chanel*/
    {SPI_BUS_ID_0, SPI0_BASE, INVALID_U16, INVALID_U16, SPI0_TX_REQ_ID, SPI0_RX_REQ_ID},/*hw spi controller1, register base*/
    {SPI_BUS_ID_1, SPI1_BASE, INVALID_U16, INVALID_U16, SPI1_TX_REQ_ID, SPI1_RX_REQ_ID},/*hw spi controller1, register base*/
#ifdef GPIO_SIMULATE_SPI_SUPPORT
    {SPI_BUS_ID_1, INVALID_U32, INVALID_U16, INVALID_U16, INVALID_U16, INVALID_U16}, /*sw spi controller1, no register base*/
    {SPI_BUS_ID_2, INVALID_U32, INVALID_U16, INVALID_U16, INVALID_U16, INVALID_U16}, /*sw spi controller2, no register base*/
    {SPI_BUS_ID_3, INVALID_U32, INVALID_U16, INVALID_U16, INVALID_U16, INVALID_U16} /*sw spi controller3, no register base*/
#endif
};

const SPI_PAD_INFO __spi_pad_func_info[]=
{
    /*SPI_PAD_NAME_E, pin_addr, value_spi, value_gpio, gpio_id*/
    {PAD_SPI0_0_CLK,  PIN_SPI0_CLK_REG, PIN_FUNC_0, PIN_FUNC_3, 21 },
    {PAD_SPI0_0_DI,  PIN_SPI0_DI_REG,  PIN_FUNC_0, PIN_FUNC_3, 23 },
    {PAD_SPI0_0_DO,  PIN_SPI0_DO_REG,  PIN_FUNC_0, PIN_FUNC_3, 24 },
    {PAD_SPI0_0_CS0,  PIN_SPI0_CS0_REG, PIN_FUNC_0, PIN_FUNC_3, 22 },

    {PAD_SPI1_0_CLK,  PIN_SPI1_CLK_REG, PIN_FUNC_0, PIN_FUNC_3, 27 },
    {PAD_SPI1_0_DI,  PIN_SPI1_DI_REG,  PIN_FUNC_0, PIN_FUNC_3, 25 },
    {PAD_SPI1_0_DO,  PIN_SPI1_DO_REG,  PIN_FUNC_0, PIN_FUNC_3, 26 },
    {PAD_SPI1_0_CS0,  PIN_SPI1_CS_REG, PIN_FUNC_0, PIN_FUNC_3, 28 },
    {INVALID_U32, INVALID_U32,      INVALID_U32,   INVALID_U32, INVALID_U32}
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
