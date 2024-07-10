/******************************************************************************
 ** File Name:    sc6531efm_gpio_cfg.c                                            *
 ** Author:       steve.zhan                                                 *
 ** DATE:         07/27/2010                                                  *
 ** Copyright:    2010  Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 **07/27/2010     Steve.zhan      Create.                                     *
  **01/17/2012    lichd           Modify for sc6530                           *
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include "os_api.h"
#include "uws6121e_reg_base.h"
#include "sc6531efm_gpio_cfg.h"
#include "uws6121e_int_cfg.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                        Local variable definition                           **
**----------------------------------------------------------------------------*/

// Config GPIO for SC6531EFM
/*
	GPIO[0~127]
	@0X8A000000    @0X8A000080
	@0X8A001000    @0X8A001080
	@0X8A002000    @0X8A002080
	@0X8A003000    @0X8A003080

	GPIO[128~159]
	@0X82001780    @0X820017C0
*/
LOCAL const GPIO_SECTION_T  s_gpio_section_table[] =
{
    {   (GPIO_BASE + 0*0x80),   0x10,     GPIO_SECTION_GPIO    },
    {   (GPIO_BASE + 1*0x80),   0x10,    GPIO_SECTION_GPIO    },
    {   (GPIO_BASE + 2*0x80),   0x10,    GPIO_SECTION_GPIO    },
    {   (GPIO_BASE + 3*0x80),   0x10,    GPIO_SECTION_GPIO    },
    {   (GPIO_BASE + 4*0x80),   0x10,     GPIO_SECTION_GPIO   },
    {   (GPIO_BASE + 5*0x80),   0x10,     GPIO_SECTION_GPIO   },
    {   (GPIO_BASE + 6*0x80),   0x10,     GPIO_SECTION_GPIO   },
    {   (GPIO_BASE + 7*0x80),   0x10,     GPIO_SECTION_GPIO   },
};

LOCAL const GPIO_SECTION_E s_gpio_has_debounce_table[] =
{
    GPIO_SECTION_GPIO,
    GPIO_SECTION_INVALID,//pls don't delete
};

GPIO_INT_T gpio_int_array[] =
{
    {TB_GPIO_INT, (TB_ISR) GPIO_Handler}
};

/**----------------------------------------------------------------------------*
**                        Global variable definition                          **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function should be called at the gpio module, to get
//                  the info(the gpio Pin register addr, section size, gpio type).
//  Dependency:     s_gpio_section_table
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC GPIO_SECTION_T *Gpio_GetCfgSectionTable (uint32 *pSize)
{
    *pSize = sizeof (s_gpio_section_table) /sizeof (s_gpio_section_table[0]);

    return (GPIO_SECTION_T *) s_gpio_section_table;
}

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function get the gpio type that have debounce function.
//
//  Dependency:     s_gpio_has_debounce_table
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
GPIO_SECTION_E *Gpio_GetCfgDebounceGpioTable (uint32 *pSize)
{
    uint32 cfg_debounce_array_size = 0;
    cfg_debounce_array_size = sizeof (s_gpio_has_debounce_table) /sizeof (s_gpio_has_debounce_table[0]);

    cfg_debounce_array_size --;
    *pSize = cfg_debounce_array_size;

    return (GPIO_SECTION_E *) s_gpio_has_debounce_table;
}
GPIO_INT_T *Gpio_GetCfgGpioIntTable (uint32 *pSize)
{
    uint32 cfg_array_size = 0;
    cfg_array_size = sizeof (gpio_int_array) /sizeof (gpio_int_array[0]);
    *pSize = cfg_array_size;

    return (GPIO_INT_T *) gpio_int_array;
}
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
