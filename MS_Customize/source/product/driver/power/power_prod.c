/******************************************************************************
 ** File Name:      power_prod.c                                              *
 ** Author:         Mingwei.Zhang                                             *
 ** DATE:           01/10/2011                                                *
 ** Copyright:      2007 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file initiate configuration for LDO by               *
 **                 customer.                                                 *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/10/2011     Mingwei.Zhang    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "power_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Local Function declare                            *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function set ldo config needed by products.
//  Author:         Mingwei.Zhang
//	Note:           
/*****************************************************************************/
PUBLIC void LDO_Prod_Init(void)
{
    LDO_PROD_CFG_T* ldo_cfg_ptr = LDO_GetProdCFGTab();
    uint32 i;
    
    SCI_ASSERT(ldo_cfg_ptr != NULL);/*assert verified*/
/*lint -esym(613, ldo_cfg_ptr) */
    for(i = 0; ldo_cfg_ptr[i].ldo_id != LDO_LDO_MAX; i++)  
    {
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
        if(ldo_cfg_ptr[i].ldo_level != LDO_VOLT_LEVEL_MAX) ///set ldo init level
        {
            LDO_SetVoltLevel (ldo_cfg_ptr[i].ldo_id, ldo_cfg_ptr[i].ldo_level);
        }
#endif        
        if(ldo_cfg_ptr[i].ldo_on)           ///set ldo power
        {
            LDO_TurnOnLDO (ldo_cfg_ptr[i].ldo_id);
        }
        else
        {
            LDO_TurnOffLDO(ldo_cfg_ptr[i].ldo_id);
        }
    }
    return;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


