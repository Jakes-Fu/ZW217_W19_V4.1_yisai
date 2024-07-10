/******************************************************************************
 ** File Name:      gpio_ext_cfg.c                                            *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           12/29/2007                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic configure table about extend  *
 					GPIO													  *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/29/2007     Liangwen.Zhen    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "os_api.h"
#include "sci_types.h"
#include "i2c_api.h"
#include "gpio_ext_cfg.h"
#include "gpio_ext_drv.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

 
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
// I2C ADDR: need to be modified by costomer according to the special product
#define PCA9535_1_I2C_ADDR_W		0x42
#define PCA9535_1_I2C_ADDR_R		0x43
#define PCA9535_2_I2C_ADDR_W		0x44
#define PCA9535_2_I2C_ADDR_R		0x45
#define PCA9535_3_I2C_ADDR_W		0x46
#define PCA9535_3_I2C_ADDR_R		0x47

#define GPIO_EXT_DEV_INT_PIN_NUM	0

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/

LOCAL GPIO_EXT_DEV_INFO_T s_gpio_ext_cus_dev_cfg_tab[GPIO_EXT_DEV_MAX_CNT] =
{
	{GPIO_EXT_DEV_MAX_CNT, 0, 0 , 0, 0},/*lint !e651*/
	{GPIO_EXT_DEV_MAX_CNT, 0, 0 , 0, 0},
	{GPIO_EXT_DEV_MAX_CNT, 0, 0 , 0, 0}	
};

// Extend GPIO configure table: need to be modified by costomer according to the special product
LOCAL GPIO_EXT_GPIO_INFO_T s_gpio_ext_cus_gpio_cfg_tab[] =
{
//	 gpio_num       b_used		direction		default_value	gpio_handle

// END flag, Must be here                    		                               		
	{GPIO_EXT_NUM_MAX , SCI_FALSE,	GPIO_OUTPUT, 	SCI_FALSE, 		PNULL},	
};

// customer configure information, don't need to modify
LOCAL GPIO_EXT_CFG_INFO_T s_gpio_ext_cus_cfg_info = 
{
	GPIO_EXT_DEV_INT_PIN_NUM,
	s_gpio_ext_cus_dev_cfg_tab,
	s_gpio_ext_cus_gpio_cfg_tab
};


/**---------------------------------------------------------------------------*
 **                       Public Function Definitions                         *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is used to get customer configure information    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC GPIO_EXT_CFG_T_INFO_PTR GPIO_EXT_GetCusCfgInfo(void)
{
	return PNULL;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of gpio_ext_cfg.c
