/******************************************************************************
 ** File Name:      power_cfg.c                                               *
 ** Author:         jiexia.yu                                                 *
 ** DATE:           09/18/2007                                                *
 ** Copyright:      2007 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic configuration for LDO by      *
 **                 customer.                                                 *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/18/2007     jiexia.yu        Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "power_cfg.h"
#include "gpio_prod_api.h" 

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
 #define LDO_POWER_DOWN  0
LOCAL const LDO_PROD_CFG_T ldo_prod_cfg[] = 
{
    {LDO_LDO_SDIO,  LDO_POWER_DOWN, SCI_FALSE   },
    {LDO_LDO_CAMD,  LDO_POWER_DOWN, SCI_FALSE   },
    {LDO_LDO_CAMA,  LDO_POWER_DOWN, SCI_FALSE   },
    {LDO_LDO_MAX,   LDO_POWER_DOWN, SCI_FALSE   }
};

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Return LDO product config table.
//  Author:         Mingwei.Zhang
//  Note:           
/*****************************************************************************/
PUBLIC LDO_PROD_CFG_T* LDO_GetProdCFGTab(void)
{
    return (LDO_PROD_CFG_T*)ldo_prod_cfg; 
}


PUBLIC void Power_SetSensorVoltage(POWER_SENSOR_ID_E      sensor_id, 
                                   POWER_SENSOR_VDD_VAL_E dvdd_val, 
                                   POWER_SENSOR_VDD_VAL_E avdd_val, 
                                   POWER_SENSOR_VDD_VAL_E iodd_val)
{    
    uint32 iodd_ldo_volt_value = LDO_POWER_DOWN;
    uint32 avdd_ldo_volt_value = LDO_POWER_DOWN;
	
    switch(avdd_val)
    {
        case POWER_VDD_2800MV:
            avdd_ldo_volt_value = 2800;  // actually 2.8V   
            break;
            
        case POWER_VDD_2500MV:
            avdd_ldo_volt_value = 2500;  // actually 2.5V   
            break;
            
        case POWER_VDD_1800MV:
        case POWER_VDD_1500MV:
        case POWER_VDD_1300MV:
        case POWER_VDD_1200MV:
            avdd_ldo_volt_value = 1800;  // actually 1.8V   
            break;
            
        case POWER_VDD_3300MV:
        case POWER_VDD_3000MV:
            avdd_ldo_volt_value = 3000;   // actually 3.0V 
            break;  
            
        case POWER_VDD_CLOSED:
        case POWER_VDD_UNUSED:
        default:
            avdd_ldo_volt_value = LDO_POWER_DOWN;   
            break;
    } 
	
    switch(iodd_val)
    {
        case POWER_VDD_1800MV:
            iodd_ldo_volt_value = 1800;  // actually 1.8V  
            break;

        case POWER_VDD_3300MV:
        case POWER_VDD_3000MV:
        case POWER_VDD_2800MV:
        case POWER_VDD_2500MV:            
            iodd_ldo_volt_value = 2800;  // actually 2.8V  
            break;
            
        case POWER_VDD_1500MV:
            iodd_ldo_volt_value = 1500;  // actually 1.5V  
            break;
            
        case POWER_VDD_1300MV:
        case POWER_VDD_1200MV:
            iodd_ldo_volt_value = 1300;  // actually 1.3V  
            break;  
            
        case POWER_VDD_CLOSED:
        case POWER_VDD_UNUSED:
        default:
            iodd_ldo_volt_value = LDO_POWER_DOWN;           
            break;
    }
	
    if (LDO_POWER_DOWN == avdd_ldo_volt_value)
    {
        LDO_TurnOffLDO(LDO_LDO_CAMA);        
    }
    if (LDO_POWER_DOWN == iodd_ldo_volt_value)
    {
	 OS_TickDelay(4);     /*delay 4ms as avdd down to 0*/
	 LDO_TurnOffLDO(LDO_LDO_CAMD);               
    }
	
    if (LDO_POWER_DOWN != iodd_ldo_volt_value)
    {
	LDO_SetVoltValue(LDO_LDO_CAMD, iodd_ldo_volt_value);
	LDO_TurnOnLDO(LDO_LDO_CAMD);  
    }
    if (LDO_POWER_DOWN != avdd_ldo_volt_value)
    {
	LDO_SetVoltValue(LDO_LDO_CAMA, avdd_ldo_volt_value);
	LDO_TurnOnLDO(LDO_LDO_CAMA);  	
    }
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
