/******************************************************************************
 ** File Name:    uws6121e_gpio_cfg.h                                         *
 ** Author:       Steve.Zhan                                                 *
 ** DATE:         06/05/2010                                                  *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/05/2010    Steve.Zhan      Create.                                     *
 ******************************************************************************/
#ifndef _UWS6121E_GPIO_CFG_H_
#define _UWS6121E_GPIO_CFG_H_

#include "../common/inc/chip_drv_common_io.h"
#include "uws6121e_reg_base.h"
#include "isr_drvapi.h"
#include "chip_drvapi.h"

#include "../../chip_module/adi/v7/adi_reg_v7.h"
#include "../export/inc/adi_hal_internal.h"

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

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
typedef enum
{
    GPIO_SECTION_GPI = 0x0,
    GPIO_SECTION_GPO,
    GPIO_SECTION_GPIO,
    GPIO_SECTION_INVALID
}
GPIO_SECTION_E;


typedef struct
{
    uint32 gpxx_pagex_base;
    uint32 gpxx_pagex_size;
    GPIO_SECTION_E gpxx_section_type;
} GPIO_SECTION_T;

typedef struct
{
    uint32 int_logic_num;
    TB_ISR isr_func;

} GPIO_INT_T;

//configable begin
//#define GPIO_DRV_DEBUG
//#define GPIO_ASSERT_IF_WRONG_SECTION //is only using for debug mode

#define MIN_DESHAKING_INTERVAL  10
#define GPI_DEBOUNCE_PERIED                255
#define GPI_MAX_DEBOUCE_INTERVAL  (GPI_DEBOUNCE_PERIED+ MIN_DESHAKING_INTERVAL*4) /*255+ 40(ignore)*/

static __inline uint32 GpioCfg_GetBaseAddr (uint32 gpio_id)
{
    if (gpio_id < 16)
    {
        return REG_GPIO1_BASE;
    }
    else
    {
        return REG_GPIO2_BASE;
    }
}
static __inline uint32 GpioCfg_GetBitOffset (uint32 gpio_id)
{
    if (gpio_id < 16)
    {
        return (gpio_id & 0x1F);
    }
    else
    {
        return ((gpio_id - 16) & 0x1F);
    }
}

static __inline void gpio_chip_reg_set (uint32 reg_addr, uint32 value)
{
    if (!ANA_IS_ANA_REG (reg_addr))
    {
        CHIP_REG_SET (reg_addr,value);
    }
    else
    {
        ANA_REG_SET (reg_addr,value);
    }
    return;
}

static __inline uint32 gpio_chip_reg32 (uint32 reg_addr)
{
    if (!ANA_IS_ANA_REG (reg_addr))
    {
        return REG32 (reg_addr);
    }
    else
    {
        return ANA_REG_GET (reg_addr);
    }
}
static __inline void gpio_chip_reg_and (uint32 reg_addr, uint32 value)
{
    if (!ANA_IS_ANA_REG (reg_addr))
    {
        CHIP_REG_AND (reg_addr,value);
    }
    else
    {
        ANA_REG_AND (reg_addr,value);
    }
}
static __inline void gpio_chip_reg_or (uint32 reg_addr, uint32 value)
{
    if (!ANA_IS_ANA_REG (reg_addr))
    {
        CHIP_REG_OR (reg_addr,value);
    }
    else
    {
        ANA_REG_OR (reg_addr,value);
    }
}

//configable end

#define GPIO_REG_SET(x,y) gpio_chip_reg_set(x,y)
#define GPIO_REG32(x) gpio_chip_reg32(x)
#define GPIO_REG_AND(reg_addr, value) gpio_chip_reg_and(reg_addr, value)
#define GPIO_REG_OR(reg_addr, value) gpio_chip_reg_or(reg_addr, value)

/*****************************************************************************/
//  Description:    This function should be called at the gpio module, to get
//                  the info(the gpio Pin register addr, section size, gpio type).
//  Dependency:     s_gpio_section_table
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC GPIO_SECTION_T *Gpio_GetCfgSectionTable (uint32 *size);


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function get the gpio type that have debounce function.
//
//  Dependency:     s_gpio_section_table
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
GPIO_SECTION_E *Gpio_GetCfgDebounceGpioTable (uint32 *pSize);
/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
//  Description:    This function be called by interrupt handler when is
//                  GPIO interrupt.
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC ISR_EXE_T GPIO_Handler (uint32 int_num);



PUBLIC GPIO_INT_T *Gpio_GetCfgGpioIntTable (uint32 *pSize);

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
