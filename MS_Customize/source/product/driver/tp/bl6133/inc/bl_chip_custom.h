#ifndef BL_CHIP_CUSTOM_H
#define BL_CHIP_CUSTOM_H
#include "bl_chip_common.h"
#define     TS_CHIP          BL6XX3
#define     TP_BL6133_I2C_WRITE_ADDR		0x5A//0x2C//0x5A//7bit (0x2D)
#define     BTL_CHECK_CHIPID
#define     CTP_USE_HW_I2C
//#define     CTP_USE_SW_I2C
#define     GPIO_EINT
/*Modified by sunwen.he for w217 only use reset wake up*/
//#define     INT_PIN_WAKEUP
//#define     NEED_CONFIG_EINT_RESUME
//#define     SWAP_XY
#define     BL_UPDATE_FIRMWARE_ENABLE
//#define     BL_DEBUG

/*************Betterlife ic debug***********/
#endif
