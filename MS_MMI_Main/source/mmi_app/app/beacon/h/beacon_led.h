/****************************************************************************
** File Name:      mmiphs_app.h                                                *
** Author:                                                                 *
** Date:           03/22/2006                                              *
** Copyright:      2006 TLT, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshengqi         Create
** 
****************************************************************************/
#ifndef  _MMI_BEACON_LED_H_    
#define  _MMI_BEACON_LED_H_   

#ifdef __cplusplus
    extern "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mn_type.h"
#include "mmi_nv.h"
#include "sci_types.h"
#include "mmi_common.h"
#include <stdio.h>
#include "nvitem.h"
#ifdef _RTOS
#include "sci_api.h" /*@tony.yao  replase sci_mem.h with sci_api.h*/
#else
#include "tasks_id.h"
#endif
#include "mn_api.h"
#include "mn_events.h"
#include "mn_type.h"
#include "sig_code.h"


typedef enum
{
    LED_LIGHT_OPEN_TYPE_OFF,
    LED_LIGHT_OPEN_TYPE_ON,
    LED_LIGHT_OPEN_TYPE_TIMES,
    LED_LIGHT_OPEN_TYPE_MAX
}LED_LIGHT_OPEN_TYPE_E;

extern uint8 BEACON_LedRed_Open(LED_LIGHT_OPEN_TYPE_E type,uint32 on_time,uint32 off_time,uint32 num);
extern uint8 BEACON_LedBlue_Open(LED_LIGHT_OPEN_TYPE_E type,uint32 on_time,uint32 off_time,uint32 num);
extern uint8 BEACON_LedYellow_Open(LED_LIGHT_OPEN_TYPE_E type,uint32 on_time,uint32 off_time,uint32 num);
extern void LED_Call_Switch(BOOLEAN onoff);
extern void LED_SMS_Switch(BOOLEAN onoff);
extern void LED_Charge_Switch(BOOLEAN onoff);

extern uint8 MMIBEACON_LED_NV_GetChargeSwitch(void);
extern BOOLEAN  MMIBEACON_LED_NV_SetChargeSwitch(uint8 is_on);
extern uint8 MMIBEACON_LED_GetChargeSwitch(void);

extern uint8 MMIBEACON_LED_NV_GetSmsSwitch(void);
extern BOOLEAN  MMIBEACON_LED_NV_SetSmsSwitch(uint8 is_on);
extern uint8 MMIBEACON_LED_GetSmsSwitch(void);

extern uint8 MMIBEACON_LED_NV_GetCallSwitch(void);
extern BOOLEAN  MMIBEACON_LED_NV_SetCallSwitch(uint8 is_on);
extern uint8 MMIBEACON_LED_GetCallSwitch(void);

extern uint8 MMIBEACON_LED_NV_GetPowerSwitch(void);
extern BOOLEAN  MMIBEACON_LED_NV_SetPowerSwitch(uint8 is_on);
extern uint8 MMIBEACON_LED_GetPowerSwitch(void);

extern uint8 MMIBEACON_LED_NV_GetFlipSwitch(void);
extern BOOLEAN  MMIBEACON_LED_NV_SetFlipSwitch(uint8 is_on);
extern uint8 MMIBEACON_LED_GetFlipSwitch(void);

#ifdef __cplusplus
    }
#endif

#endif
