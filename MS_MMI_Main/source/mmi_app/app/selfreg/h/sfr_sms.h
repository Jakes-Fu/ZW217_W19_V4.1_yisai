#ifndef __SFR_SMS_H__
#define __SFR_SMS_H__

PUBLIC BOOLEAN MMISFR_SMSGetRunningStatus(MN_DUAL_SYS_E dual_sys);
PUBLIC void MMISFR_SMSSetCnfOk(MN_DUAL_SYS_E dual_sys, BOOLEAN isSuccess);
PUBLIC void MMISFR_SMS_Run(MN_DUAL_SYS_E dual_sys);

#endif // __SFR_SMS_H__

