/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/


#ifndef _PWM_H_
#define _PWM_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'pwm'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// PWM_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_PWM_BASE               0x05108000
#else
#define REG_PWM_BASE               0x50108000
#endif

typedef volatile struct
{
    REG32                          PWT_Config;                   //0x00000000
    REG32                          LPG_Config;                   //0x00000004
    REG32                          PWL0_Config;                  //0x00000008
    REG32                          PWL1_Config;                  //0x0000000C
    REG32                          TSC_DATA;                     //0x00000010
    REG32                          GPADC_DATA;                   //0x00000014
} HWP_PWM_T;

#define hwp_pwm                    ((HWP_PWM_T*) REG_ACCESS_ADDRESS(REG_PWM_BASE))


//PWT_Config
typedef union {
    REG32 v;
    struct {
        REG32 pwt_enable : 1; // [0]
        REG32 pwt_start : 1; // [1], read only
        REG32 __3_2 : 2;
        REG32 pwt_duty : 10; // [13:4]
        REG32 __15_14 : 2;
        REG32 pwt_period : 11; // [26:16]
        REG32 __31_27 : 5;
    } b;
} REG_PWM_PWT_CONFIG_T;

//LPG_Config
typedef union {
    REG32 v;
    struct {
        REG32 lpg_reset_l : 1; // [0]
        REG32 lpg_test : 1; // [1]
        REG32 __3_2 : 2;
        REG32 lpg_ontime : 4; // [7:4]
        REG32 __15_8 : 8;
        REG32 lpg_period : 3; // [18:16]
        REG32 __31_19 : 13;
    } b;
} REG_PWM_LPG_CONFIG_T;

//PWL0_Config
typedef union {
    REG32 v;
    struct {
        REG32 pwl_min : 8; // [7:0]
        REG32 pwl_max : 8; // [15:8]
        REG32 pwl0_en_h : 1; // [16], write set
        REG32 pwl0_force_l : 1; // [17], write clear
        REG32 pwl0_force_h : 1; // [18], read only
        REG32 pwl_pulse_en : 1; // [19]
        REG32 pwl0_set_oe : 1; // [20], write set
        REG32 pwl0_clr_oe : 1; // [21], write clear
        REG32 pwl_set_mux : 1; // [22], write set
        REG32 pwl_clr_mux : 1; // [23], write clear
        REG32 pwl_pulse_per : 8; // [31:24]
    } b;
} REG_PWM_PWL0_CONFIG_T;

//PWL1_Config
typedef union {
    REG32 v;
    struct {
        REG32 pwl1_threshold : 8; // [7:0]
        REG32 lfsr_reg : 8; // [15:8], read only
        REG32 pwl1_en_h : 1; // [16], write set
        REG32 pwl1_force_l : 1; // [17], write clear
        REG32 pwl1_force_h : 1; // [18], read only
        REG32 __19_19 : 1;
        REG32 pwl1_set_oe : 1; // [20], write set
        REG32 pwl1_clr_oe : 1; // [21], write clear
        REG32 __31_22 : 10;
    } b;
} REG_PWM_PWL1_CONFIG_T;

//TSC_DATA
typedef union {
    REG32 v;
    struct {
        REG32 tsc_x_value_bit : 10; // [9:0], read only
        REG32 tsc_x_value_valid : 1; // [10], read only
        REG32 tsc_y_value_bit : 10; // [20:11], read only
        REG32 tsc_y_value_valid : 1; // [21], read only
        REG32 __31_22 : 10;
    } b;
} REG_PWM_TSC_DATA_T;

//GPADC_DATA
typedef union {
    REG32 v;
    struct {
        REG32 gpadc_value_bit : 10; // [9:0], read only
        REG32 gpadc_value_valid : 1; // [10], read only
        REG32 __31_11 : 21;
    } b;
} REG_PWM_GPADC_DATA_T;


//PWT_Config
#define PWM_PWT_ENABLE             (1<<0)
#define PWM_PWT_START              (1<<1)
#define PWM_PWT_DUTY(n)            (((n)&0x3FF)<<4)
#define PWM_PWT_PERIOD(n)          (((n)&0x7FF)<<16)

//LPG_Config
#define PWM_LPG_RESET_L            (1<<0)
#define PWM_LPG_TEST               (1<<1)
#define PWM_LPG_ONTIME(n)          (((n)&15)<<4)
#define PWM_LPG_ONTIME_MASK        (15<<4)
#define PWM_LPG_ONTIME_SHIFT       (4)
#define PWM_LPG_ONTIME_UNDEFINED   (0<<4)
#define PWM_LPG_ONTIME_15_6MS      (1<<4)
#define PWM_LPG_ONTIME_31_2MS      (2<<4)
#define PWM_LPG_ONTIME_46_8MS      (3<<4)
#define PWM_LPG_ONTIME_62MS        (4<<4)
#define PWM_LPG_ONTIME_78MS        (5<<4)
#define PWM_LPG_ONTIME_94MS        (6<<4)
#define PWM_LPG_ONTIME_110MS       (7<<4)
#define PWM_LPG_ONTIME_125MS       (8<<4)
#define PWM_LPG_ONTIME_140MS       (9<<4)
#define PWM_LPG_ONTIME_156MS       (10<<4)
#define PWM_LPG_ONTIME_172MS       (11<<4)
#define PWM_LPG_ONTIME_188MS       (12<<4)
#define PWM_LPG_ONTIME_200MS       (13<<4)
#define PWM_LPG_ONTIME_218MS       (14<<4)
#define PWM_LPG_ONTIME_234MS       (15<<4)
#define PWM_LPG_ONTIME_V_UNDEFINED (0)
#define PWM_LPG_ONTIME_V_15_6MS    (1)
#define PWM_LPG_ONTIME_V_31_2MS    (2)
#define PWM_LPG_ONTIME_V_46_8MS    (3)
#define PWM_LPG_ONTIME_V_62MS      (4)
#define PWM_LPG_ONTIME_V_78MS      (5)
#define PWM_LPG_ONTIME_V_94MS      (6)
#define PWM_LPG_ONTIME_V_110MS     (7)
#define PWM_LPG_ONTIME_V_125MS     (8)
#define PWM_LPG_ONTIME_V_140MS     (9)
#define PWM_LPG_ONTIME_V_156MS     (10)
#define PWM_LPG_ONTIME_V_172MS     (11)
#define PWM_LPG_ONTIME_V_188MS     (12)
#define PWM_LPG_ONTIME_V_200MS     (13)
#define PWM_LPG_ONTIME_V_218MS     (14)
#define PWM_LPG_ONTIME_V_234MS     (15)
#define PWM_LPG_PERIOD(n)          (((n)&7)<<16)
#define PWM_LPG_PERIOD_MASK        (7<<16)
#define PWM_LPG_PERIOD_SHIFT       (16)
#define PWM_LPG_PERIOD_0_125S      (0<<16)
#define PWM_LPG_PERIOD_0_25S       (1<<16)
#define PWM_LPG_PERIOD_0_5S        (2<<16)
#define PWM_LPG_PERIOD_0_75S       (3<<16)
#define PWM_LPG_PERIOD_1S          (4<<16)
#define PWM_LPG_PERIOD_1_25S       (5<<16)
#define PWM_LPG_PERIOD_1_5S        (6<<16)
#define PWM_LPG_PERIOD_1_75S       (7<<16)
#define PWM_LPG_PERIOD_V_0_125S    (0)
#define PWM_LPG_PERIOD_V_0_25S     (1)
#define PWM_LPG_PERIOD_V_0_5S      (2)
#define PWM_LPG_PERIOD_V_0_75S     (3)
#define PWM_LPG_PERIOD_V_1S        (4)
#define PWM_LPG_PERIOD_V_1_25S     (5)
#define PWM_LPG_PERIOD_V_1_5S      (6)
#define PWM_LPG_PERIOD_V_1_75S     (7)

//PWL0_Config
#define PWM_PWL_MIN(n)             (((n)&0xFF)<<0)
#define PWM_PWL_MAX(n)             (((n)&0xFF)<<8)
#define PWM_PWL0_EN_H              (1<<16)
#define PWM_PWL0_FORCE_L           (1<<17)
#define PWM_PWL0_FORCE_H           (1<<18)
#define PWM_PWL_PULSE_EN           (1<<19)
#define PWM_PWL0_SET_OE            (1<<20)
#define PWM_PWL0_CLR_OE            (1<<21)
#define PWM_PWL_SET_MUX            (1<<22)
#define PWM_PWL_CLR_MUX            (1<<23)
#define PWM_PWL_PULSE_PER(n)       (((n)&0xFF)<<24)

//PWL1_Config
#define PWM_PWL1_THRESHOLD(n)      (((n)&0xFF)<<0)
#define PWM_LFSR_REG(n)            (((n)&0xFF)<<8)
#define PWM_PWL1_EN_H              (1<<16)
#define PWM_PWL1_FORCE_L           (1<<17)
#define PWM_PWL1_FORCE_H           (1<<18)
#define PWM_PWL1_SET_OE            (1<<20)
#define PWM_PWL1_CLR_OE            (1<<21)

//TSC_DATA
#define PWM_TSC_X_VALUE_BIT(n)     (((n)&0x3FF)<<0)
#define PWM_TSC_X_VALUE_VALID      (1<<10)
#define PWM_TSC_Y_VALUE_BIT(n)     (((n)&0x3FF)<<11)
#define PWM_TSC_Y_VALUE_VALID      (1<<21)

//GPADC_DATA
#define PWM_GPADC_VALUE_BIT(n)     (((n)&0x3FF)<<0)
#define PWM_GPADC_VALUE_VALID      (1<<10)





#endif

