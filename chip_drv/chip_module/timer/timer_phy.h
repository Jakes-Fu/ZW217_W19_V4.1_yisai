/******************************************************************************
 ** File Name:      timer_phy.h                                                 *
 ** Author:         DingJi                                                   *
 ** DATE:           08/09/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME        DESCRIPTION                               *
 ** 08/09/2010      DingJi      Create.                                   *
 ******************************************************************************/

#ifndef _TIMER_PHY_H_
#define _TIMER_PHY_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

#include "sci_types.h"

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

typedef struct
{
    TIMER_PHY_MODE_E mode; //work mode
    uint32 clk_freq; //frequency of timer
    uint32 load_count; //the count which will be load by timer at first, it depends on clk_freq
    uint32 irq_op; //enable irq or disable irq
} TIMER_PHY_CONFIG_T;

typedef struct
{
    int32 (*init) (uint32 timer_id);
    int32 (*enable) (uint32 timer_id);
    int32 (*disable) (uint32 timer_id);
    int32 (*config) (uint32 timer_id, TIMER_PHY_CONFIG_T *timer_config);
    int32 (*query) (uint32 timer_id, TIMER_PHY_STATUS_T *timer_status);
    int32 (*clr_irq) (uint32 timer_id);
    int32 (*reload) (uint32 timer_id);
} TIMER_PHY_OPERATION_T;

/**----------------------------------------------------------------------------*
**                         Symbol Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC TIMER_PHY_OPERATION_T *timer_get_phy_operation (void);
PUBLIC void TIMER_DelayUs (uint32 us);
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
