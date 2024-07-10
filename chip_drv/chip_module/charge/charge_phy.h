/******************************************************************************
 ** File Name:      charge_phy.h                                              *
 ** Author:         MingweiZhang                                              *
 ** DATE:           08/26/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the basic hw interfaces of I2C device.   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/26/2011     MingweiZhang     Create.                                   *
 ******************************************************************************/

#ifndef _CHARGE_PHY_H_
#define _CHARGE_PHY_H_
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

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function initialize Charger function.
//  Author:         Benjamin.Wang
//  Note:           This module should be initialized after GPIO_Init.
/*****************************************************************************/
PUBLIC void CHG_PHY_Init (void);

/*****************************************************************************/
//  Description:    This function indicates that the charger input Vchg is above 3V.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CHG_PHY_GetCHGIntStatus (void);

/*****************************************************************************/
//  Description:    This function is used to shut down the charger.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_ShutDown (void);

/*****************************************************************************/
//  Description:    This function is used to turn on the charger.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_TurnOn (void);

/*****************************************************************************/
//  Description:    This function indicates the charger status.If it is "1", the charge is done.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CHG_PHY_GetCHGDoneStatus (void);

/*****************************************************************************/
//  Description:    This function restart the charger.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_SetRecharge (void);

/*****************************************************************************/
//  Description:    This function stops the recharge mode.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_StopRecharge (void);

/*****************************************************************************/
//  Description:    This function sets the lowest switchover point between constant-current
//                      and constant-voltage modes.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHG_PHY_SetSwitchoverPoint (CHG_SWITPOINT_E eswitchpoint);

/*****************************************************************************/
//  Description:    This function is used to update one level of the lowest switchover point
//                      between constant-current and constant-voltage modes.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHG_PHY_UpdateSwitchoverPoint (BOOLEAN);

PUBLIC void CHG_PHY_SetChgCurrent(uint16 current);

PUBLIC CHGMNG_ADAPTER_TYPE_E CHG_PHY_IdentifyAdpType(void);

PUBLIC void CHG_PHY_SetChargeEndVolt(uint32 mv);
PUBLIC void CHG_PHY_SetVBatOvpVolt(uint32 mv);
PUBLIC void CHG_PHY_SetVChgOvpVolt(uint32 mv);
PUBLIC void CHG_PHY_SetChargeEndCurrent(uint32 ma);

PUBLIC BOOLEAN CHG_PHY_IsChargerCvStatus(void);
PUBLIC BOOLEAN CHG_PHY_IsChargerPresent(void);
PUBLIC BOOLEAN CHG_PHY_IsChargerReady(void);
PUBLIC BOOLEAN CHG_PHY_IsVBatOv(void);
PUBLIC BOOLEAN CHG_PHY_IsVChgOv(void);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

