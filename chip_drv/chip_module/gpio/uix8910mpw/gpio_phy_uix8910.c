/******************************************************************************
 ** File Name:      gpio_phy_v5.c                                                 *
 ** Author:         Steve.Zhan                                                   *
 ** DATE:           07/28/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the physical layer of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/28/2010     Steve.Zhan       Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "gpio_drvapi.h"
#include "../gpio_phy.h"
#include "gpio_reg_uix8910.h"

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
#ifndef GPIO_VAL_OFFSET
#define GPIO_VAL_OFFSET    0xC
#endif

#ifndef GPI_DATA
#define GPI_DATA           GPIO_VAL_OFFSET
#endif

#ifndef GPO_DATA
#define GPO_DATA           GPIO_VAL_OFFSET
#endif

#ifndef GPIO_VALSET_OFFSET
#define GPIO_VALSET_OFFSET    0x10
#endif

#ifndef GPIO_VALCLR_OFFSET
#define GPIO_VALCLR_OFFSET    0x14
#endif

#ifndef GPIO_INTSET_OFFSET
#define GPIO_INTSET_OFFSET    0x18
#endif

#ifndef GPIO_INTCLR_OFFSET
#define GPIO_INTCLR_OFFSET    0x1C
#endif


#ifndef GPO_TRI
#define GPO_TRI            0xFFFF
#endif

#ifndef GPI_IE
#define GPI_IE             0xFFFF
#endif

#ifndef GPIO_IE
#define GPIO_IE            0xFFFF
#endif

#ifndef GPIO_OEN_OFFSET
#define GPIO_OEN_OFFSET    0x0
#endif

#ifndef GPIO_OENSETOUT_OFFSET
#define GPIO_OENSETOUT_OFFSET    0x4
#endif

#ifndef GPIO_OENSETIN_OFFSET
#define GPIO_OENSETIN_OFFSET     0x8
#endif

#ifndef GPI_MIS
#define GPI_MIS            0xFFFF
#endif

#ifndef GPIO_INTSTS_OFFSET
#define GPIO_INTSTS_OFFSET 0x24
#endif

#ifndef GPI_IC
#define GPI_IC             0xFFFF
#endif

#ifndef GPIO_INTSTSCLR_OFFSET
#define GPIO_INTSTSCLR_OFFSET 0x20
#endif

#ifndef GPI_IEV
#define GPI_IEV            0xFFFF
#endif

#ifndef GPIO_IS
#define GPIO_IS            0xFFFF
#endif

#ifndef GPIO_IC
#define GPIO_IC            0xFFFF
#endif

#ifndef GPIO_IBE
#define GPIO_IBE           0xFFFF
#endif

#ifndef GPI_0CTRL
#define GPI_0CTRL          0xFFFF
#endif

#ifndef GPI_TRIG
#define GPI_TRIG           0xFFFF
#endif

#ifndef GPI_DEBOUNCE_BIT
#define GPI_DEBOUNCE_BIT   255
#endif

#ifndef GPIO_REG_SET
#define GPIO_REG_SET       CHIP_REG_SET
#endif

#ifndef GPIO_REG32
#define GPIO_REG32         REG32
#endif

#ifndef GPIO_REG_AND
#define GPIO_REG_AND       CHIP_REG_AND
#endif

#ifndef GPIO_REG_OR
#define GPIO_REG_OR        CHIP_REG_OR
#endif

extern GPIO_INT_TYPE g_gpio_inttype[8];

/*****************************************************************************/
//  Description:    This function retrun the max number of gpio
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC uint32 GPIO_PHY_GetMaxNumber (void)
{
    return GPIO_MAX_PIN_NUM;

}

/*****************************************************************************/
//  Description:    This function get gpio module base info.
//  Dependency:     Gpio_GetCfgSectionTable(uint32 *table)
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PHY_GetBaseInfo (uint32 gpio_id, GPIO_INFO_T *pGpio_info)
{
    uint32 i = 0;
    uint32 table_size = 0;

    GPIO_SECTION_T  *p_gpio_section_table = (GPIO_SECTION_T *) Gpio_GetCfgSectionTable (&table_size);

    SCI_ASSERT (gpio_id < GPIO_PHY_GetMaxNumber() && table_size > 0);/*assert verified*/

    pGpio_info->baseAddr = GpioCfg_GetBaseAddr (gpio_id);
    pGpio_info->bit_num  = GpioCfg_GetBitOffset (gpio_id);

    for (i = 0; i < table_size; ++i)
    {
        if (p_gpio_section_table[i].gpxx_pagex_base == pGpio_info->baseAddr)
        {
            if (p_gpio_section_table[i].gpxx_pagex_size > pGpio_info->bit_num)
            {
                pGpio_info->gpio_type = p_gpio_section_table[i].gpxx_section_type;
                return;
            }

            break;
        }
    }

    pGpio_info->gpio_type = GPIO_SECTION_INVALID;

    return;
}

/*****************************************************************************/
//  Description:    This function get gpio data register addr.
//  Dependency:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _GPIO_GetGpioDataRegAddr (GPIO_INFO_T *pGpio_info, uint32 *pOffsetAddr)
{
    switch (pGpio_info->gpio_type)
    {
        case GPIO_SECTION_GPI:
            *pOffsetAddr = GPI_DATA;
            break;
        case GPIO_SECTION_GPO:
            *pOffsetAddr = GPO_DATA;
            break;
        case GPIO_SECTION_GPIO:
            *pOffsetAddr = GPIO_VAL_OFFSET;
            break;
        case GPIO_SECTION_INVALID:
            //GPIO_PRINT:"[GPIO_DRV]the GPIO_ID is Invalid in this chip"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_193_112_2_17_23_4_46_1012,(uint8*)"");
            GPIO_SECTION_ASSERT(0);/*assert to do*/
            return SCI_FALSE;
        default:
            SCI_ASSERT (0);/*assert to do*/
            break;
    }

    return TRUE;

}

LOCAL BOOLEAN _GPIO_GetGpioDataSetRegAddr (GPIO_INFO_T *pGpio_info, uint32 *pOffsetAddr)
{
    switch (pGpio_info->gpio_type)
    {
        case GPIO_SECTION_GPI:
            *pOffsetAddr = GPI_DATA;
            break;
        case GPIO_SECTION_GPO:
            *pOffsetAddr = GPO_DATA;
            break;
        case GPIO_SECTION_GPIO:
            *pOffsetAddr = GPIO_VALSET_OFFSET;
            break;
        case GPIO_SECTION_INVALID:
            //GPIO_PRINT:"[GPIO_DRV]the GPIO_ID is Invalid in this chip"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_193_112_2_17_23_4_46_1012,(uint8*)"");
            GPIO_SECTION_ASSERT(0);/*assert to do*/
            return SCI_FALSE;
        default:
            SCI_ASSERT (0);/*assert to do*/
            break;
    }

    return TRUE;

}

/*****************************************************************************/
//  Description:    This function used to get the state of a gpio gpi gpo pin
//  Author:         Zhemin.Lin
//  retread by:     Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_PHY_GetPinData (GPIO_INFO_T *pGpio_info)
{
    uint32 offsetAddr = 0;
    uint32 reg_addr = 0;

    reg_addr = pGpio_info->baseAddr;

    if (_GPIO_GetGpioDataRegAddr (pGpio_info, &offsetAddr))
    {
        reg_addr += offsetAddr;
        return ( (GPIO_REG32 (reg_addr) & (1 << pGpio_info->bit_num)) ? SCI_TRUE : SCI_FALSE);
    }

    return SCI_FALSE;
}

/*****************************************************************************/
//  Description:    This function used to get the state of a gpio pin
//  Author:         Yiyue.He
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PHY_SetPinData (GPIO_INFO_T *pGpio_info ,BOOLEAN b_on)
{
    uint32 offsetAddr = 0;
    uint32 reg_addr = 0;
    BOOLEAN value = (b_on ? SCI_TRUE : SCI_FALSE);

    reg_addr = pGpio_info->baseAddr;

    _GPIO_GetGpioDataSetRegAddr (pGpio_info, &offsetAddr);
    if (value)
    {
        reg_addr += offsetAddr;
    }
    else
    {
        reg_addr += offsetAddr + 0x4;
    }

    GPIO_REG_SET (reg_addr, GPIO_GPIO_SET(1UL << pGpio_info->bit_num));

}

/*****************************************************************************/
//  Description:    This function get gpio data Mask register addr.
//  Dependency:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _GPIO_GetGpioDataMaskRegAddr (GPIO_INFO_T *pGpio_info, uint32 *pOffsetAddr)
{
    return TRUE;
}


/*****************************************************************************/
//  Description:    This function used to know whether the gpio port is enabled.
//  notes:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_PHY_GetDataMask (GPIO_INFO_T *pGpio_info)
{
    return SCI_TRUE;
}

/*****************************************************************************/
//  Description:    This function used to know whether the gpio port is enabled.
//  Note:              0 - disable
//                     1 - enable
/*****************************************************************************/
PUBLIC void GPIO_PHY_SetDataMask (GPIO_INFO_T *pGpio_info, BOOLEAN b_on)
{
}

LOCAL BOOLEAN _GPIO_GetGpioIntMaskAddr (GPIO_INFO_T *pGpio_info, uint32 *pOffsetAddr)
{
    switch (pGpio_info->gpio_type)
    {
        case GPIO_SECTION_GPI:
            *pOffsetAddr = GPI_IE;
            break;
        case GPIO_SECTION_GPO:
            //GPIO_PRINT:"[GPIO_DRV]this opretion can not belong to GPO"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_322_112_2_17_23_4_47_1015,(uint8*)"");
            return SCI_FALSE;

        case GPIO_SECTION_GPIO:
            *pOffsetAddr = GPIO_INTSET_OFFSET;
            break;

        case GPIO_SECTION_INVALID:
            //GPIO_PRINT:"[GPIO_DRV]the GPIO_ID is Invalid in this chip"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_330_112_2_17_23_4_47_1016,(uint8*)"");
            return SCI_FALSE;

        default:
            SCI_ASSERT (0);/*assert to do*/
            break;
    }

    return TRUE;

}

/*****************************************************************************/
//  Description:    This function used to know whether the gpio int mask is enabled.
//                  Interrupt mask register, "1" corresponding pin is not masked.
//                  "0" corresponding pin interrupt is masked
//  Author:         Benjamin.Wang
//  Retreat by:     Steve.Zhan
//  Note:           SCI_FALSE - disable
//                  SCI_TRUE - enable
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_PHY_GetIntIsMask (GPIO_INFO_T *pGpio_info)
{
    uint32 reg_addr = 0;
    uint32 offsetAddr = 0;

    reg_addr = pGpio_info->baseAddr;

    if (_GPIO_GetGpioIntMaskAddr (pGpio_info, &offsetAddr))
    {
        reg_addr += offsetAddr;
        return ( (GPIO_REG32 (reg_addr) & (GPIO_GPINT_R_SET(1 << pGpio_info->bit_num) | GPIO_GPINT_F_SET(1 << pGpio_info->bit_num))) ? SCI_TRUE : SCI_FALSE);
    }

    return FALSE;
}

/*****************************************************************************/
//  Description:    This function used to Set GPIO IE. enable interrupt.
//  Note:
//
/*****************************************************************************/
PUBLIC void GPIO_PHY_SetIntMask (GPIO_INFO_T *pGpio_info)
{
    BOOLEAN value = SCI_FALSE;
    uint32 reg_addr = 0;
    uint32 offsetAddr = 0;

    reg_addr = pGpio_info->baseAddr;

    if (_GPIO_GetGpioIntMaskAddr (pGpio_info, &offsetAddr))
    {
        reg_addr += offsetAddr;
        switch(g_gpio_inttype[pGpio_info->bit_num])
        {
            case GPIO_INT_LEVEL_HIGH:
            case GPIO_INT_EDGES_RISING:
            {
                GPIO_REG_SET (reg_addr, (GPIO_GPINT_R_SET(1 << pGpio_info->bit_num)));
                break;
            }
            case GPIO_INT_LEVEL_LOW:
            case GPIO_INT_EDGES_FALLING:
            {
                GPIO_REG_SET (reg_addr, (GPIO_GPINT_F_SET(1 << pGpio_info->bit_num)));
                break;
            }
            case GPIO_INT_EDGES_BOTH:
            {
                GPIO_REG_SET (reg_addr, (GPIO_GPINT_R_SET(1 << pGpio_info->bit_num)) | (GPIO_GPINT_F_SET(1 << pGpio_info->bit_num)));
                break;
            }
        }
    }

    return;
}

/*****************************************************************************/
//  Description:    This function used to Set GPIO IE. disable interrupt.
//  Note:
//
/*****************************************************************************/
PUBLIC void GPIO_PHY_CleanIntMask (GPIO_INFO_T *pGpio_info)
{
    BOOLEAN value = SCI_FALSE;
    uint32 reg_addr = 0;
    uint32 offsetAddr = 0;

    reg_addr = pGpio_info->baseAddr;

    if (_GPIO_GetGpioIntMaskAddr (pGpio_info, &offsetAddr))
    {
        reg_addr += offsetAddr + 0x4;
        GPIO_REG_SET (reg_addr, (GPIO_GPINT_R_CLR(1 << pGpio_info->bit_num) | GPIO_GPINT_F_CLR(1 << pGpio_info->bit_num)));
    }

    return;

}

/*****************************************************************************/
//  Description:    This function used to get the direction of a gpio pin
//  Note:              0 - Input
//                     1 - Output
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_PHY_GetDirection (GPIO_INFO_T *pGpio_info)
{
    uint32 reg_addr = 0;
    reg_addr = pGpio_info->baseAddr;

    switch (pGpio_info->gpio_type)
    {
        case GPIO_SECTION_GPI:
            return SCI_FALSE;

        case GPIO_SECTION_GPO:
            return SCI_TRUE;

        case GPIO_SECTION_GPIO:
            reg_addr += GPIO_OEN_OFFSET;
            break;

        case GPIO_SECTION_INVALID:
            //GPIO_PRINT:"[GPIO_DRV]the GPIO_ID is Invalid in this chip"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_430_112_2_17_23_4_47_1017,(uint8*)"");
            GPIO_SECTION_ASSERT(0);/*assert to do*/
            return SCI_FALSE;

        default:
            SCI_ASSERT (0);/*assert to do*/
            break;
    }

    return ( (GPIO_REG32 (reg_addr) & (1 << pGpio_info->bit_num)) ? SCI_FALSE : SCI_TRUE);
}

/*****************************************************************************/
//  Description:    This function used to set the direction of a gpio pin
//  Note:              0 - Input
//                        1 - Output
/*****************************************************************************/
PUBLIC void GPIO_PHY_SetDirection (GPIO_INFO_T *pGpio_info, BOOLEAN directions)
{
    BOOLEAN value = (directions ? SCI_TRUE : SCI_FALSE);
    uint32 reg_addr = 0;
    reg_addr = pGpio_info->baseAddr;

    switch (pGpio_info->gpio_type)
    {
        case GPIO_SECTION_GPI:

            if (directions)
            {
                //GPIO_PRINT:"[GPIO_DRV]GPIO_SetDirection error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_457_112_2_17_23_4_47_1018,(uint8*)"");
                GPIO_SECTION_ASSERT(0);/*assert to do*/
            }

            return;

        case GPIO_SECTION_GPO:

            if (!directions)
            {
                //GPIO_PRINT:"[GPIO_DRV]GPIO_SetDirection error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_467_112_2_17_23_4_47_1019,(uint8*)"");
                GPIO_SECTION_ASSERT(0);/*assert to do*/
            }

            return;

        case GPIO_SECTION_GPIO:
            if (directions)
            {
                reg_addr += GPIO_OENSETOUT_OFFSET;
            }
            else
            {
                reg_addr += GPIO_OENSETIN_OFFSET;
            }
            break;
        case GPIO_SECTION_INVALID:
            //GPIO_PRINT:"[GPIO_DRV]the GPIO_ID is Invalid in this chip"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_477_112_2_17_23_4_47_1020,(uint8*)"");
            GPIO_SECTION_ASSERT(0);/*assert to do*/
            return;

        default:
            SCI_ASSERT (0);/*assert to do*/
            break;
    }

    GPIO_REG_SET (reg_addr, GPIO_OEN_VAL(1UL << pGpio_info->bit_num));

}


/*****************************************************************************/
//  Description:    This function used to get the intr state of a gpio pin
//  Author:         Zhemin.Lin
//  retread by:    Yiyue.He
//  Retreat by:     Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_PHY_GetIntState (GPIO_INFO_T *pGpio_info)
{
    uint32 reg_addr = 0;

    reg_addr = pGpio_info->baseAddr;

    switch (pGpio_info->gpio_type)
    {
        case GPIO_SECTION_GPI:
            reg_addr += GPI_MIS;
            break;
        case GPIO_SECTION_GPO:
            //GPIO_PRINT:"[GPIO_DRV]The corresponding reg of this GPIO_ID is a GPO! No Intr!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_508_112_2_17_23_4_47_1021,(uint8*)"");
            return SCI_FALSE;

        case GPIO_SECTION_GPIO:
            reg_addr += GPIO_INTSTS_OFFSET;
            break;
        case GPIO_SECTION_INVALID:
            //GPIO_PRINT:"[GPIO_DRV]the GPIO_ID is Invalid in this chip"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_515_112_2_17_23_4_47_1022,(uint8*)"");
            GPIO_SECTION_ASSERT(0);/*assert to do*/
            return SCI_FALSE;

        default:
            SCI_ASSERT (0);/*assert to do*/
            break;
    }

    return ( (GPIO_REG32 (reg_addr) & (1 << pGpio_info->bit_num)) ? SCI_TRUE : SCI_FALSE);
}

/*****************************************************************************/
//  Description:    This function used to clear the given interrupt status bit.
//  Author:           Benjamin.Wang
// retread by:        Yiyue.He
//  Retreat by:     Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PHY_ClearIntStatus (GPIO_INFO_T *pGpio_info)
{
    BOOLEAN value = SCI_TRUE;
    uint32 reg_addr = 0;

    reg_addr = pGpio_info->baseAddr;

    switch (pGpio_info->gpio_type)
    {
        case GPIO_SECTION_GPI:
            reg_addr += GPI_IC;
            break;
        case GPIO_SECTION_GPO:
            //GPIO_PRINT:"[GPIO_DRV]this opretion can not belong to GPO"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_545_112_2_17_23_4_47_1023,(uint8*)"");
            return;
        case GPIO_SECTION_GPIO:
            reg_addr += GPIO_INTSTSCLR_OFFSET;
            break;
        case GPIO_SECTION_INVALID:
            //GPIO_PRINT:"[GPIO_DRV]the GPIO_ID is Invalid in this chip"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_551_112_2_17_23_4_47_1024,(uint8*)"");
            GPIO_SECTION_ASSERT(0);/*assert to do*/
            return;

        default:
            SCI_ASSERT (0);/*assert to do*/
            break;
    }

    GPIO_REG_SET (reg_addr, GPIO_GPINT_CLR(1UL << pGpio_info->bit_num));
}

/*****************************************************************************/
//  Description:    This function used to set gpio Interrupt sense type.
//  Author:         Benjamin.Wang
//  Retreat by:     Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PHY_SetInterruptSense (GPIO_INFO_T *pGpio_info, GPIO_INT_TYPE sensetype)
{
    uint32 gpio_int_ctrl, gpio_bit;

    gpio_bit = 0x1UL << pGpio_info->bit_num;
    switch (pGpio_info->gpio_type)
    {
        case GPIO_SECTION_GPI:
        {
            break;
        }

        case GPIO_SECTION_GPO:
        {
            //GPIO_PRINT:"[GPIO_DRV]this opretion can not belong to GPO"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_596_112_2_17_23_4_47_1026,(uint8*)"");
            GPIO_SECTION_ASSERT(0);/*assert to do*/
            return;
        }

        case GPIO_SECTION_GPIO:
            {
                switch (sensetype)
                {
                    case GPIO_INT_LEVEL_HIGH:   // detect high level.
                        gpio_int_ctrl = GPIO_GPINT_R_SET(gpio_bit); 
                        GPIO_REG_SET ( (pGpio_info->baseAddr + GPIO_INTSET_OFFSET), gpio_int_ctrl);
                        gpio_int_ctrl = GPIO_GPINT_F_CLR(gpio_bit) | GPIO_GPINT_MODE_SET(gpio_bit); 
                        GPIO_REG_SET ( (pGpio_info->baseAddr + GPIO_INTCLR_OFFSET), gpio_int_ctrl);              
                        break;

                    case GPIO_INT_LEVEL_LOW:    // detect low level.
                        gpio_int_ctrl = GPIO_GPINT_F_SET(gpio_bit); 
                        GPIO_REG_SET ( (pGpio_info->baseAddr + GPIO_INTSET_OFFSET), gpio_int_ctrl);
                        gpio_int_ctrl = GPIO_GPINT_R_CLR(gpio_bit) | GPIO_GPINT_MODE_SET(gpio_bit); 
                        GPIO_REG_SET ( (pGpio_info->baseAddr + GPIO_INTCLR_OFFSET), gpio_int_ctrl);              
                        break;

                    case GPIO_INT_EDGES_BOTH:    // detect the rising edges and falling edges.
                        gpio_int_ctrl = GPIO_GPINT_MODE_SET(gpio_bit) | GPIO_GPINT_F_SET(gpio_bit) | GPIO_GPINT_R_SET(gpio_bit); 
                        GPIO_REG_SET ( (pGpio_info->baseAddr + GPIO_INTSET_OFFSET), gpio_int_ctrl);
                        break;

                    case GPIO_INT_EDGES_RISING:  // detect the rising edges.
                        gpio_int_ctrl = GPIO_GPINT_MODE_SET(gpio_bit) | GPIO_GPINT_R_SET(gpio_bit); 
                        GPIO_REG_SET ( (pGpio_info->baseAddr + GPIO_INTSET_OFFSET), gpio_int_ctrl);
                        gpio_int_ctrl = GPIO_GPINT_F_CLR(gpio_bit); 
                        GPIO_REG_SET ( (pGpio_info->baseAddr + GPIO_INTCLR_OFFSET), gpio_int_ctrl);              
                        break;

                    case GPIO_INT_EDGES_FALLING:
                        gpio_int_ctrl = GPIO_GPINT_MODE_SET(gpio_bit) | GPIO_GPINT_F_SET(gpio_bit); 
                        GPIO_REG_SET ( (pGpio_info->baseAddr + GPIO_INTSET_OFFSET), gpio_int_ctrl);
                        gpio_int_ctrl = GPIO_GPINT_R_CLR(gpio_bit); 
                        GPIO_REG_SET ( (pGpio_info->baseAddr + GPIO_INTCLR_OFFSET), gpio_int_ctrl);              
                        break;

                    default:
                        //GPIO_PRINT:"This operation is invalid. \n"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_635_112_2_17_23_4_47_1027,(uint8*)"");
                }
            }
            break;
        case GPIO_SECTION_INVALID:
            //GPIO_PRINT:"[GPIO_DRV]the GPIO_ID is Invalid in this chip"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_640_112_2_17_23_4_47_1028,(uint8*)"");
            GPIO_SECTION_ASSERT(0);/*assert to do*/
            return;
        default:
            SCI_ASSERT (0);/*assert to do*/
            break;
    }

}

/*****************************************************************************/
//  Description:    This function used to set gpin Debounce time.
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PHY_SetGPIDebounce (GPIO_INFO_T *pGpio_info, uint8 debounce_period)
{
    uint32 i = 0;
    uint32 reg_addr = 0;
    uint32 size  = 0;
    GPIO_SECTION_E *gpio_type = NULL;
    uint32 gpio_int_ctrl, gpio_bit;

    gpio_type = Gpio_GetCfgDebounceGpioTable (&size);

    for (i = 0; i< size ; ++i)
    {
        if ( (int) pGpio_info->gpio_type != (int) gpio_type[i])
        {
            continue;
        }
        else
        {
            break;
        }
    }

    if (i == size)
    {
        //GPIO_PRINT:"[GPIO_DRV]this opretion can not belong to SetGPIDebounce"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_676_112_2_17_23_4_47_1029,(uint8*)"");
        return;
    }

    reg_addr = pGpio_info->baseAddr;
    gpio_bit = 0x1UL << pGpio_info->bit_num;

    if (debounce_period >= 1)
    {
        GPIO_REG_SET (reg_addr + GPIO_INTSET_OFFSET, GPIO_DBN_EN_SET(gpio_bit));
    }
    else// not recommend
    {
        GPIO_REG_SET (reg_addr + GPIO_INTCLR_OFFSET, GPIO_DBN_EN_SET(gpio_bit)); //bypass mode
    }

}

/*****************************************************************************/
//  Description:    This function used to set  gpi  Interrupt Trigering
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PHY_TrigGPIDetect (GPIO_INFO_T *pGpio_info)
{
#if 0
    BOOLEAN value=SCI_TRUE;
    uint32 reg_addr = 0;

    reg_addr = pGpio_info->baseAddr;

    switch (pGpio_info->gpio_type)
    {
        case GPIO_SECTION_GPI:
            reg_addr += GPI_TRIG;
            break;

        case GPIO_SECTION_GPO:
        case GPIO_SECTION_GPIO:
            //GPIO_PRINT:"[GPIO_DRV]this opretion can not belong to GPO/GPIO"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_714_112_2_17_23_4_48_1030,(uint8*)"");
            return;

        case GPIO_SECTION_INVALID:
            //GPIO_PRINT:"[GPIO_DRV]the GPIO_ID is Invalid in this chip"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPIO_PHY_V5_718_112_2_17_23_4_48_1031,(uint8*)"");
            GPIO_SECTION_ASSERT(0);/*assert to do*/
            return;

        default:
            SCI_ASSERT (0);/*assert to do*/
            break;
    }

    GPIO_REG_SET (reg_addr, ( (GPIO_REG32 (reg_addr) & ~ (1UL<<pGpio_info->bit_num)) | (value<<pGpio_info->bit_num)));
#endif
}

/*****************************************************************************/
//  Description:    This function used to Enable gpi Detect function
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void GPIO_PHY_EnableGPIDetect (GPIO_INFO_T *pGpio_info)
{
    switch (pGpio_info->gpio_type)
    {
        case GPIO_SECTION_GPI:
            GPIO_PHY_TrigGPIDetect (pGpio_info);
            break;
        case GPIO_SECTION_GPO:
        case GPIO_SECTION_GPIO:
            return;

        default:
            SCI_ASSERT (0);/*assert to do*/
            break;
    }
}
