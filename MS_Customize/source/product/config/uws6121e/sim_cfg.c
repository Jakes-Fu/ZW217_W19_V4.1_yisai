/******************************************************************************
 ** File Name:      sim_cfg.c                                             *
 ** Author:         wuding.yang                                             *
 ** DATE:           01/11/2011                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the config information of quad sim       *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE              NAME            DESCRIPTION                               *
 ** 01/11/2011   wuding.yang     Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sc_reg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

typedef enum _SIM_CONNECT_6302_E
{
    BB_SIM_0,
    BB_SIM_1,
    BB_SIM_2,
    BB_SIM_NOT_CONNECT_6302        
}SIM_CONNECT_6302_E;

typedef struct
{
    SIM_CONNECT_6302_E sim_index;
    uint32 spi_logic_id;
    uint32 gpio_id_6302_sysrstb;
}SIM_QUAD_CFG;

#define SIM_6302_SYSRSTB_GPIO 23//77//gpio id which connected to 6302 SYSRSTB,6531efm use 23,6530 use 77

static SIM_QUAD_CFG sim_quad_cfg =
#ifdef GPIO_SIMULATE_SPI_SUPPORT
//{BB_SIM_0,SPI2_0_CS0,SIM_6302_SYSRSTB_GPIO};
//{BB_SIM_1,SPI2_0_CS0,SIM_6302_SYSRSTB_GPIO};
{BB_SIM_2,SPI2_0_CS0,SIM_6302_SYSRSTB_GPIO};
#else
//{BB_SIM_0,SPI1_3_CS0,SIM_6302_SYSRSTB_GPIO};//SPI1_3_0 is the real SPI of BB, 6302 is connected to sim0.
{BB_SIM_1,SPI0_0_CS0,SIM_6302_SYSRSTB_GPIO};//6302 is connected to sim1,6531efm use SPI0_0_CS0,6530 use SPI1_3_CS0 
//{BB_SIM_2,SPI1_3_CS0,SIM_6302_SYSRSTB_GPIO};//SPI1_3_0 is the real SPI of BB, 6302 is connected to sim2.
//{BB_SIM_NOT_CONNECT_6302,0,0};//no 6302, no quadsim.
#endif

/*****************************************************************************/
//  Description:    Get the GPIO Id or SPI which is connected to 6302 for quad-sim platform
//  Input      :
//  Return     :    spi logic id
//  Author     :    wuding.yang
//	Note       :
/*****************************************************************************/
PUBLIC uint32 GPIO_SPI_Quadsim_Get_Config(uint8 *which_sim_connect_6302)
{
    SPI_DEV spiconfg;

    spiconfg.id = sim_quad_cfg.spi_logic_id;
    spiconfg.mode = CPOL0_CPHA0;
    spiconfg.tx_bit_length = 8;
    spiconfg.freq = 10000000;
    spiconfg.spi_cs_cb = NULL;
    spiconfg.spi_tx_cb = NULL;
    spiconfg.spi_rx_cb = NULL;

    if ((0xFF != spiconfg.id) && (BB_SIM_NOT_CONNECT_6302 != sim_quad_cfg.sim_index))
    {
        SPI_HAL_Open(&spiconfg);

#if 0//6530 use SPI1_3_CLK_ALTERNATE,but 6531efm do not need
        if (SPI1_3_CS0 == spiconfg.id)
        {
            //If you use LCMD14/GPIO_75 as SPI1_3_CLK, you should 
            //use this code, other SPI will not use this code.
            SPI_PAD_CFG_T spi_pad_cfg;

            spi_pad_cfg.pad_name = PAD_SPI1_3_CLK_ALTERNATE;
            spi_pad_cfg.pad_fun  = SPI_PAD_SPI;
            SPI_HAL_Ioctl (SPI1_3_CS0, SPI_CTL_S_PAD, (uint32 *)&spi_pad_cfg);
        }
#endif

        //Finish the Reset of the 6302 chip.
        GPIO_SetValue(SIM_6302_SYSRSTB_GPIO,TRUE);//GPIO is low in pinmap. Low is RESET for 6302.
    }

    *which_sim_connect_6302 = (uint8)sim_quad_cfg.sim_index;

    return spiconfg.id;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
