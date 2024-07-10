#ifndef _SCM_DEVMGR_H_
#define _SCM_DEVMGR_H_

#include "sci_types.h"
#include "scm_api.h"

PUBLIC void _InitDeviceMgr(void);

PUBLIC uint32 _AddSlotToDevice(SCM_SLOT_NAME_E slotNO,SCM_DEVICE_NAME_E DevNO,SCM_PWR pwr);

PUBLIC uint32 _DeleteSlotFromDevice(SCM_SLOT_NAME_E slotNO,SCM_DEVICE_NAME_E DevNO);

PUBLIC BOOLEAN _DevPwr(SCM_DEVICE_NAME_E DevNO,SCM_SLOT_NAME_E slotNO,SLOT_PWR_SWITCH_E pwron);

PUBLIC void _DevEntry(SCM_DEVICE_NAME_E DevNO);

PUBLIC void _DevExit(SCM_DEVICE_NAME_E DevNO);

#endif


