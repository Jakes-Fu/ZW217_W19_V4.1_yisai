#ifndef _RUN_MODE_H
#define _RUN_MODE_H

#include "sci_types.h"

#ifdef __cplusplus
extern "C" {
#endif

PUBLIC BOOLEAN RM_GetEnableArmSleepFlag(void);
PUBLIC void RM_SetEnableArmSleepFlag(BOOLEAN is_deep_sleep);
PUBLIC uint32 RM_GetUart0Mode(void);
PUBLIC void RM_SetUart0Mode(uint32 mode);
PUBLIC void RM_SetDloaderMode(uint32 mode);
PUBLIC uint32 RM_GetDloaderMode(void);
PUBLIC void RM_SetIQCTestFlag(BOOLEAN is_iqctest);
PUBLIC BOOLEAN RM_GetIQCTestFlag(void);
PUBLIC BOOLEAN RM_GetPinTestFlag(void);
PUBLIC void RM_SetPinTestFlag(BOOLEAN is_pintest_mode);
PUBLIC void RM_SetCalibrationPostMode(BOOLEAN is_calibrationpost);
PUBLIC BOOLEAN RM_GetCalibrationPostMode(void);
PUBLIC void RM_SetCalibrationPostNoLcmMode(BOOLEAN is_calibrationpostnolcm);
PUBLIC BOOLEAN RM_GetCalibrationPostNoLcmMode(void);
PUBLIC void RM_SetCalibrationMode(BOOLEAN is_calibration);
PUBLIC BOOLEAN RM_GetCalibrationMode(void);
PUBLIC void RM_SendRunModeRequest(void);
PUBLIC void RM_GetRunModeResponse(void);
PUBLIC void RM_GetRunMode(void);
PUBLIC void RM_GetVcomRunMode(void);
PUBLIC uint32 RM_GetMcuMode(void);
PUBLIC void RM_Cali_NvAccess(void);
PUBLIC void RM_SendRunModeAck(void);
PUBLIC void RM_SendRes(uint8* src, int32 size);
PUBLIC uint32  IMEI_RM_RecvCMD(uint8 *buffer);
PUBLIC void RM_HandlePowerResetMode(uint32 mode);
PUBLIC BOOLEAN RM_IsUsbCalibrationMode(void);
#ifdef __cplusplus
}
#endif

#endif /* _RUN_MODE_H */
