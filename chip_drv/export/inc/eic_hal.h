/******************************************************************************
 ** File Name:    eic_hal.h                                                   *
 ** Author:       lichd                                                       *
 ** DATE:         6/14/2011                                                   *
 ** Copyright:    2008 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 **                                    *
 ******************************************************************************/
#ifndef _EIC_HAL_H_
#define _EIC_HAL_H_

#include "sci_types.h"
#include "gpio_drvapi.h"
#include "../../chip_module/eic/eic_cfg.h"
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                             Public Function                               *
 **---------------------------------------------------------------------------*/
/******************************************************************************/
//  Description:    eic HAL init, register its isr
//  Author:         lichd
//  Note:           
/******************************************************************************/
void EIC_HAL_Init (void);

/******************************************************************************/
//  Description:    Get the level of a channel
//  Author:         lichd
//  Note:                 
/******************************************************************************/
BOOLEAN EIC_HAL_GetValue (uint32 id_num);

/******************************************************************************/
//  Description:    Set trig condition for the interrupt
//  Author:         lichd
//  Note:                 
/******************************************************************************/
void EIC_HAL_SetIntSense (uint32 id_num, GPIO_INT_TYPE sense_type);

/******************************************************************************/
//  Description:    Set the trig parameter for EIC_DBNC interrupt.
//  Author:         lichd
//  Note:           EIC_DBNC specify       
/******************************************************************************/
void EIC_HAL_TrgCfg (uint32 id_num, BOOLEAN valide_value, uint32 dbnc_cnt_ms);

/******************************************************************************/
//  Description:    Register callback from EIC_DBNC module
//  Author:         lichd
//  Note:           
/******************************************************************************/
BOOLEAN EIC_DBNC_RegCallback (
                                uint32           id_num,
                                BOOLEAN          valide_value,    
                                BOOLEAN          shaking_en,
                                uint32           shaking_interval_ms,
                                EIC_CALLBACK     eic_callback
);

void EIC_HAL_TrgWithSleep (uint32 id_num, BOOLEAN valide_value, BOOLEAN trg_with_sleep_en);

/******************************************************************************/
//  Description:    Register callback from EIC_LATCH module
//  Author:         lichd
//  Note:           
/******************************************************************************/
BOOLEAN EIC_LATCH_RegCallback (
                                uint32           id_num,
                                BOOLEAN          valide_value,    
                                BOOLEAN          sleepout_en,
                                EIC_CALLBACK     eic_callback
);

/******************************************************************************/
//  Description:    Enable or disable the interrupt
//  Author:         lichd
//  Note:                 
/******************************************************************************/
void EIC_HAL_EnableInt (uint32 id_num, uint32 is_en);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End _EIC_HAL_H_

