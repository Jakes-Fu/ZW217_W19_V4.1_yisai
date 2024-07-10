#ifndef __pah_unit_h__
#define __pah_unit_h__

// c
#include "os_api.h"
#include "sci_types.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>


//============================================================================
// Default
//============================================================================
#define PAH_800x_DEFAULT_PPG_INITIAL_EXPOSURE       (400)

#define PAH_800x_DEFAULT_ENABLE_PPG_CONVERGENCE     true


#define PAH_800x_DEFAULT_PPG_CONVERGENCE_COUNT   				(0)
#define PAH_800x_DEFAULT_PPG_CONVERGENCE_T_WEIGHT_NORMAL_MODE   (8)
#define PAH_800x_DEFAULT_PPG_CONVERGENCE_T_WEIGHT_FAST_MODE     (16)
#define PAH_800x_DEFAULT_PPG_CONVERGENCE_T_MIN                  (10)
#define PAH_800x_DEFAULT_PPG_CONVERGENCE_T_MAX                  (1792)
#define PAH_800x_DEFAULT_PPG_CONVERGENCE_RAW_OUTER_HIGH_BOUND   (12288)
#define PAH_800x_DEFAULT_PPG_CONVERGENCE_RAW_INNER_HIGH_TARGET  (10240)
#define PAH_800x_DEFAULT_PPG_CONVERGENCE_RAW_INNER_LOW_TARGET   (8192)
#define PAH_800x_DEFAULT_PPG_CONVERGENCE_RAW_OUTER_LOW_BOUND    (6144)

#define PAH_800x_DEFAULT_ENABLE_PPG_RECONVERGENCE_IF_OUT_OF_BOUNDS  true


//============================================================================
// Definition
//============================================================================

// I2C 7-bit ID
#define PAH_800x_DEFAULT_I2C_SLAVE_ID_1  (0x40)
#define PAH_800x_DEFAULT_I2C_SLAVE_ID_2  (0x10)

#define  PAH_800x_PRODUCT_ID        (0x8008)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @file pah_ret.h
 *
 * Copyright (c) 2016-2018 PixArt Imaging Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - PixArt Imaging Inc.
 **/

typedef enum {

    PAH_RET_SUCCESS = 0,            // Success
    
    PAH_RET_PENDING,

    PAH_RET_FAILED,                 // Unknown error, generally there exists bug in this driver

    PAH_RET_PLAT_FAILED,            // Platform API failed
    PAH_RET_VERIFY_FAILED,          // Verify device failed

    PAH_RET_NOT_IMPLEMENTED,        // This API is not supported or is not implemented

    PAH_RET_INVALID_ARGUMENT,       // Invalid argument to API
    PAH_RET_INVALID_OPERATION,      // This operation is not available at this time

    PAH_RET_FIFO_CKS_FAILED,        // Failed to compare check sum with fifo data
    PAH_RET_FIFO_OVERFLOW,          // Fifo in device is overflow

    PAH_RET_ASYNC_PENDING,

} pah_ret;

typedef enum {
    PAH_800x_CH_NUM_A = 0,
    PAH_800x_CH_NUM_B,
    PAH_800x_CH_NUM_C,
    PAH_800x_MAX_CH_NUM,
} pah_800x_ch_num;

#define PAH_SUCCEEDED(ret)              (ret == PAH_RET_SUCCESS)
#define PAH_FAILED(ret)                 (!PAH_SUCCEEDED(ret))


#define PAH_CHECK_RET(ret) \
    if (PAH_FAILED(ret)) { \
        return ret; \
    }

#define PAH_CHECK_NULL(p) \
    if (!p) { \
        return PAH_RET_INVALID_ARGUMENT; \
    }



/**
 * @file pah_plat.h
 *
 * Copyright (c) 2016-2018 PixArt Imaging Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - PixArt Imaging Inc.
 **/




//============================================================================
// Definition
//============================================================================

//============================================================================
// Macro
//============================================================================
#ifndef __FILENAME__
#   define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/**
 * @file pah_util.h
 *
 * Copyright (c) 2016-2018 PixArt Imaging Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - PixArt Imaging Inc.
 **/



#define PAH_ARRAY_SIZE(arr)         (sizeof(arr) / sizeof(arr[0]))

#define PAH_SET_BIT(var,pos)        do { ((var) |= (1 << (pos))); } while(0)
#define PAH_CLEAR_BIT(var,pos)      do { ((var) &= ~(1 << (pos))); } while(0)
#define PAH_CHECK_BIT(var,pos)      (((var) >> (pos)) & 1)

//
// PAH_SET_BITSLICE
//
// ex1:
//      // var    = 0x00 : 00000000
//      // pos    = 2    :      ^
//      // mask   = 0x3C : 00111100
//      // value  = 0x0D :   1101
//      // ret    = 0x34 : 00110100
//      uint8_t data = 0x00;
//      uint8_t ret = PAH_SET_BITSLICE(data, 2, 0x3C, 0x0D);
//
// ex2:
//      // var    = 0x6D : 01101101
//      // pos    = 6    :  ^
//      // mask   = 0xC0 : 11000000
//      // value  = 0x02 : 10
//      // ret    = 0xAD : 10101101
//      uint8_t data = 0x6D;
//      uint8_t ret = PAH_SET_BITSLICE(data, 6, 0xC0, 0x02);
//
#define PAH_SET_BITSLICE(var, pos, mask, value) \
    ((var & ~mask) | ((value << pos) & mask))

//
// PAH_GET_BITSLICE
//
// ex1:
//      // var    = 0x5D : 01011101
//      // pos    = 2    :      ^
//      // mask   = 0x3C : 00111100
//      // ret    = 0x07 :   0111
//      uint8_t ret = PAH_GET_BITSLICE(0x5D, 2, 0x3C);
//
// ex2:
//      // var    = 0xA6 : 10100110
//      // pos    = 5    :   ^
//      // mask   = 0xE0 : 11100000
//      // ret    = 0x05 : 101
//      uint8_t ret = PAH_GET_BITSLICE(0xA6, 5, 0xE0);
//
#define PAH_GET_BITSLICE(var, pos, mask) \
    ((var & mask) >> pos)

#define PAH_UINT64_HIGH(var)        ((uint32_t)(var >> 32))
#define PAH_UINT64_LOW(var)         ((uint32_t)(var & UINT32_MAX))

#define PAH_UINT16_HIGH(var)        ((uint8_t)(var >> 8))
#define PAH_UINT16_LOW(var)         ((uint8_t)(var & UINT8_MAX))

#define PAH_UNUSED_VAR(var)         (void)(var)
#define UNUSED_VAR(var)         (void)(var)
  
#ifndef __FILENAME__
#   define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @file pah_reg.h
 *
 * Copyright (c) 2016-2018 PixArt Imaging Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - PixArt Imaging Inc.
 **/




//============================================================================
// Definition
//============================================================================
typedef const uint8_t (pah_reg)[2];

typedef struct {
    pah_reg     *reg_arr;
    uint32_t     reg_arr_len;  
}pah_reg_arr;


#define PAH_REG_ARR_SETUP(pah_reg_arr, reg_array) \
    do { \
        pah_reg_arr.reg_arr     = (pah_reg*)reg_array; \
        pah_reg_arr.reg_arr_len = PAH_ARRAY_SIZE(reg_array); \
    } while(0)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//============================================================================
// Register address
//============================================================================

/*#define PAH_800x_ADDR_00_PRODUCT_ID                   (0x00)
#define PAH_800x_ADDR_03                                (0x03)
#define PAH_800x_ADDR_03_BIT_R_PS_EN                    (0)
#define PAH_800x_ADDR_03_BIT_R_ALS_EN                   (1)
#define PAH_800x_ADDR_03_BIT_SW_SUSPEND                 (5)

#define PAH_800x_ADDR_04                                (0x04)
#define PAH_800x_ADDR_04_BIT_ALS_OVERFLOW_ECO           (7)

#define PAH_800x_ADDR_09                                (0x09)
#define PAH_800x_ADDR_09_BIT_ALS_INTERRUPT              (2)
#define PAH_800x_ADDR_09_BIT_PS_INTERRUPT               (5)
#define PAH_800x_ADDR_09_BIT_PS_APPROACH                (6)

#define PAH_800x_ADDR_10_PS_ONOFF_SYNC                  (0x10)

#define PAH_800x_ADDR_11                                (0x11)
#define PAH_800x_ADDR_11_BIT_R_LED_CONTROL_OUT_EN       (4)
#define PAH_800x_ADDR_11_BIT_R_PS_INTERRUPT_EN          (7)

#define PAH_800x_ADDR_0E_ALS_LUX_SYNC                   (0x0E)

#define PAH_800x_ADDR_34                                (0x34)
#define PAH_800x_ADDR_34_BIT_R_EXPOSURE_ALS_F0          (0)

#define PAH_800x_ADDR_35                                (0x35)
#define PAH_800x_ADDR_35_BIT_R_EXPOSURE_ALS_F0          (0)

#define PAH_800x_ADDR_38                                (0x38)
#define PAH_800x_ADDR_38_BIT_0_LOCK_EXPOTIME            (0)
#define PAH_800x_ADDR_38_BIT_1_CLEAR_ALS_OVERFLOW_ECO   (1)

#define PAH_800x_ADDR_EC                                (0xEC)
#define PAH_800x_ADDR_EC_BIT_7_R_CLK_MANUAL             (7)*/

#define PAH_800x_ADDR_00_PRODUCT_ID                     (0x00)

#define PAH_800x_ADDR_07_TG                             (0X07)
#define PAH_800x_ADDR_07_TG_BIT_EN                      (0)

#define PAH_800x_ADDR_20                                (0x20)
#define PAH_800x_ADDR_20_BIT_DR_FLAG                    (0)
#define PAH_800x_ADDR_20_BIT_OVERFLOW_FLAG              (1)
#define PAH_800x_ADDR_20_BIT_TOUCH_FLAG                 (2)

//expo A Sync
#define PAH_800x_ADDR_2A                                (0x2A)
#define PAH_800x_ADDR_2B                                (0x2B)

#define PAH_800x_ADDR_35                                (0x35)
#define PAH_800x_ADDR_36                                (0x36)

#define PAH_800x_ADDR_40                                (0x40)
#define PAH_800x_ADDR_41                                (0x41)

#define PAH_800x_ADDR_11_EnCh                           (0x11)

//expo A write
#define PAH_800x_ADDR_17                                (0x17)
#define PAH_800x_ADDR_18                                (0x18)

#define PAH_800x_ADDR_19                                (0x19)
#define PAH_800x_ADDR_1A                                (0x1A)

#define PAH_800x_ADDR_1B                                (0x1B)
#define PAH_800x_ADDR_1C                                (0x1C)

//frame period

#define PAH_800x_ADDR_2C                                (0x2C)
#define PAH_800x_ADDR_2D                                (0x2D)

#define PAH_800x_ADDR_37                                (0x37)
#define PAH_800x_ADDR_38                                (0x38)

#define PAH_800x_ADDR_42                                (0x42)
#define PAH_800x_ADDR_43                                (0x43)

//adc

#define PAH_800x_ADDR_2E                                (0x2E)
#define PAH_800x_ADDR_2F                                (0x2F)

#define PAH_800x_ADDR_39                                (0x39)
#define PAH_800x_ADDR_3A                                (0x3A)

#define PAH_800x_ADDR_44                                (0x44)
#define PAH_800x_ADDR_45                                (0x45)

#define PAH_800x_ADDR_27_RAWDATA_A_SYNC                 (0x27)
#define PAH_800x_ADDR_32_RAWDATA_B_SYNC                 (0x32)
#define PAH_800x_ADDR_3D_RAWDATA_C_SYNC                 (0x3D)

#define PAH_800x_ADDR_E8                                (0xE8)
#define PAH_800x_ADDR_E8_CMD_SUSPEND_MODE_BIT           (0)

#define PAH_800x_ADDR_EB                                (0xEB)
#define PAH_800x_ADDR_E8_UPDATE_FLAG                    (0x80)

#define PAH_800x_ADDR_EE                                (0xEE)
#define PAH_800x_ADDR_EE_SW_RESET                       (0x1F)//(0x07)
#define PAH_800x_ADDR_D2                                (0xD2)
#define PAH_800x_ADDR_D2_OD_EnH                      	(5)

#endif  // header guard
