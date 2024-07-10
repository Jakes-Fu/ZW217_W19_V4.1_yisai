/******************************************************************************
 ** File Name:      pinmap.c                                                  *
 ** Author:         Richard.Yang                                              *
 ** DATE:           03/08/2004                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the pin contrl interface for 6600 chip  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03/08/2004     Richard.Yang     Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "chip_plf_export.h"
#include "pinmap_drvapi.h"
#include "gpio_drvapi.h"
#include "lps_apb.h"
#if defined(PLATFORM_UWS6121E)
#include "../../../../../chip_drv/chip_module/pin/uws6121e/pinmap.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define     KBPIN_FRC_OUT_HIGH     (PIN_OUT_FRC_HIGH | PIN_PULL_FRC_NONE | 0xF)
#define     KBPIN_FRC_OUT_LOW      (PIN_OUT_FRC_LOW | PIN_PULL_FRC_NONE | 0xF)
#define     SCI_FAILURE       0x01

/**---------------------------------------------------------------------------*
 **                         Data                                              *
 **---------------------------------------------------------------------------*/
LOCAL void PM_GPIOInit (void);


/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Return the INT sense type according to GPIO number in
//                  pm_gpio_default_map[]
//  Author:         Jeff.Li
//  Note:
/*****************************************************************************/
PUBLIC PM_IS_E PM_GetGPIOIntType (uint32 gpio_num)
{
    uint32 i = 0;

    for (;;)
    {
        if (PM_INVALID_SHORT_VAL == pm_gpio_default_map[i].gpio_num)
        {
            break;
        }

        if (PM_INPUT == pm_gpio_default_map[i].dir)
        {
            if (gpio_num == pm_gpio_default_map[i].gpio_num)
            {
                return pm_gpio_default_map[i].int_sense;
            }
        }

        i++;
    }

    return PM_INVALID_INT;
}

PUBLIC void PM_Init (void)
{
    int i = 0;
    //pin_enable(TRUE);
    hwp_lpsApb->aon_ahb_lp_ctrl |= BIT_1; 
    hwp_lpsApb->cfg_io_deep_sleep = 0x30fff;
    while (1) {
        if (pm_func[i].addr == PM_INVALID_VAL)
            break;
        if (!ANA_IS_ANA_REG (pm_func[i].addr))
            CHIP_REG_SET (pm_func[i].addr, pm_func[i].value);
        else
            ANA_REG_SET (pm_func[i].addr, pm_func[i].value);
        i++;
    }
    //CHIP_REG_AND (REG_PIN_CTRL_REG2, BIT_31|BIT_30|BIT_29);
    PM_GPIOInit();
#if 1
    ANA_REG_OR(ANA_MODULE_EN0_RTC, BIT_4);
    ANA_REG_SET(0x7c0, 0x146);
    ANA_REG_SET(0x7c4, 0x146);
    ANA_REG_SET(0x7c8, 0x146);
    ANA_REG_SET(0x7cc, 0x146);
    ANA_REG_SET(0x7d0, 0x146);
    ANA_REG_SET(0x7d4, 0x146);
    ANA_REG_SET(0x7d8, 0x146);
    ANA_REG_SET(0x7dc, 0x146);
    ANA_REG_SET(0x7e0, 0x146);
    ANA_REG_SET(0x7e4, 0x146);
    ANA_REG_SET(0x7e8, 0x146);
    ANA_REG_SET(0x7ec, 0x146);
    ANA_REG_SET(0x7f0, 0x146);
    ANA_REG_SET(0x7f4, 0x146);
    ANA_REG_SET(0x7f8, 0x146);
    ANA_REG_SET(0x7fc, 0x146);
    ANA_REG_AND(ANA_MODULE_EN0_RTC, ~BIT_4);
#endif
}

LOCAL void PM_GPIOInit (void)
{
    int i = 0;
    uint16 gpio_num;

    // Enable all GPIO pages

    for (;;)
    {
        if (pm_gpio_default_map[i].gpio_num == PM_INVALID_SHORT_VAL)
        {
            break;
        }

        gpio_num  = pm_gpio_default_map[i].gpio_num;

        if (pm_gpio_default_map[i].dir == PM_OUTPUT)
        {
            // first Set default value
            if (0xFFFF != pm_gpio_default_map[i].default_val)
            {
                GPIO_SetValue (gpio_num,pm_gpio_default_map[i].default_val);
            }

            // Output only
            // Config it can be READ and WRITE.(1)
            GPIO_SetDataMask (gpio_num,SCI_TRUE);
            // Config it to be OUTPUT.(1)
            GPIO_SetDirection (gpio_num,SCI_TRUE);
            // MASK it.(0)
            GPIO_DisableIntCtl (gpio_num);
        }
        else
        {
            // Config it can be READ and WRITE.(1)
            GPIO_SetDataMask (gpio_num,SCI_TRUE);
            // Config it to be INPUT.(0)
            GPIO_SetDirection (gpio_num,SCI_FALSE);
        }

        i++;
    }
}

uint8 PM_GPIOKeyPinCtrl(uint8 dir, uint32 io_index, uint8 *pvalue)
{
#if 0
    uint32 gpioex_kbpin_addr;
    uint32 gpioex_kbpin_value[2] = {KBPIN_FRC_OUT_LOW, KBPIN_FRC_OUT_HIGH};
    const uint32 gpioex_kb_set_addr[]=
    {
        REG_PIN_KEYIN0,
        REG_PIN_KEYIN1,
        REG_PIN_KEYIN2,
        REG_PIN_KEYIN3,
        REG_PIN_KEYIN4,
        REG_PIN_KEYIN5,
        NULL,
        NULL,
        REG_PIN_KEYOUT0,
        REG_PIN_KEYOUT1,
        REG_PIN_KEYOUT2,
        REG_PIN_KEYOUT3,
        REG_PIN_KEYOUT4,
        REG_PIN_KEYOUT5,
        NULL,
        NULL
    };    
    uint8 result = SCI_SUCCESS;

    if (0 == dir)    //out
    {
        if ((io_index >= (sizeof(gpioex_kb_set_addr) / sizeof(uint32))) || (*pvalue > 1))
        {
            result = SCI_FAILURE; 
        }
        else
        {
            if (NULL != gpioex_kb_set_addr[io_index])
            {
                CHIP_REG_SET (gpioex_kb_set_addr[io_index] ,gpioex_kbpin_value[*pvalue]);
            }
            else
            {
                result = SCI_FAILURE; 
            }
        }    
    }
    else    //in
    {
         result = SCI_FAILURE; 
    }

    return result;
#endif
    return SCI_FAILURE; 
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

// End of pinmap.c
