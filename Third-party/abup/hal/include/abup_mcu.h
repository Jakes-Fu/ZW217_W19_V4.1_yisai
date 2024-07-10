#ifndef _ABUP_TIMER_H_
#define _ABUP_TIMER_H_

abup_extern abup_int16 abup_patch_mcu1(void);
abup_extern abup_int16 abup_patch_mcu2(void);

abup_extern abupMcuPatch abup_patch_mcu[ADUPS_FOTA_SDK_MAX_MCU_NUM];

#endif /* _ABUP_TIMER_H_ */

