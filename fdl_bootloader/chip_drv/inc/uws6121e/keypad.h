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


#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'keypad'."
#endif



// =============================================================================
//  MACROS
// =============================================================================
/// Number of key in the keypad
#define KEY_NB                                  (36)
/// Number of key in the low data register
#define LOW_KEY_NB                              (30)
/// Number of key in the high data register
#define HIGH_KEY_NB                             (6)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// KEYPAD_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#if defined(REG_ADDRESS_FOR_GGE)
#define REG_KEYPAD_BASE            0x05106000
#else
#define REG_KEYPAD_BASE            0x51706000
#endif

typedef volatile struct
{
    REG32                          KP_DATA_L;                    //0x00000000
    REG32                          KP_DATA_H;                    //0x00000004
    REG32                          KP_STATUS;                    //0x00000008
    REG32                          KP_CTRL;                      //0x0000000C
    REG32                          KP_IRQ_MASK;                  //0x00000010
    REG32                          KP_IRQ_CAUSE;                 //0x00000014
    REG32                          KP_IRQ_CLR;                   //0x00000018
} HWP_KEYPAD_T;

#define hwp_keypad                 ((HWP_KEYPAD_T*) REG_ACCESS_ADDRESS(REG_KEYPAD_BASE))


//KP_STATUS
typedef union {
    REG32 v;
    struct {
        REG32 keyin_status : 8; // [7:0], read only
        REG32 __30_8 : 23;
        REG32 kp_on : 1; // [31], read only
    } b;
} REG_KEYPAD_KP_STATUS_T;

//KP_CTRL
typedef union {
    REG32 v;
    struct {
        REG32 kp_en : 1; // [0]
        REG32 __1_1 : 1;
        REG32 kp_dbn_time : 8; // [9:2]
        REG32 kp_itv_time : 6; // [15:10]
        REG32 kp_in_mask : 8; // [23:16]
        REG32 kp_out_mask : 8; // [31:24]
    } b;
} REG_KEYPAD_KP_CTRL_T;

//KP_IRQ_MASK
typedef union {
    REG32 v;
    struct {
        REG32 kp_evt0_irq_mask : 1; // [0]
        REG32 kp_evt1_irq_mask : 1; // [1]
        REG32 kp_itv_irq_mask : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_KEYPAD_KP_IRQ_MASK_T;

//KP_IRQ_CAUSE
typedef union {
    REG32 v;
    struct {
        REG32 kp_evt0_irq_cause : 1; // [0], read only
        REG32 kp_evt1_irq_cause : 1; // [1], read only
        REG32 kp_itv_irq_cause : 1; // [2], read only
        REG32 __15_3 : 13;
        REG32 kp_evt0_irq_status : 1; // [16], read only
        REG32 kp_evt1_irq_status : 1; // [17], read only
        REG32 kp_itv_irq_status : 1; // [18], read only
        REG32 __31_19 : 13;
    } b;
} REG_KEYPAD_KP_IRQ_CAUSE_T;

//KP_IRQ_CLR
typedef union {
    REG32 v;
    struct {
        REG32 kp_irq_clr : 1; // [0], write clear
        REG32 __31_1 : 31;
    } b;
} REG_KEYPAD_KP_IRQ_CLR_T;


//KP_DATA_L
#define KEYPAD_KP_DATA_L(n)        (((n)&0xFFFFFFFF)<<0)
#define KEYPAD_KP_DATA_L_MASK      (0xFFFFFFFF<<0)
#define KEYPAD_KP_DATA_L_SHIFT     (0)

//KP_DATA_H
#define KEYPAD_KP_DATA_H(n)        (((n)&0xFFFFFFFF)<<0)
#define KEYPAD_KP_DATA_H_MASK      (0xFFFFFFFF<<0)
#define KEYPAD_KP_DATA_H_SHIFT     (0)

//KP_STATUS
#define KEYPAD_KEYIN_STATUS(n)     (((n)&0xFF)<<0)
#define KEYPAD_KEYIN_STATUS_MASK   (0xFF<<0)
#define KEYPAD_KEYIN_STATUS_SHIFT  (0)
#define KEYPAD_KP_ON               (1<<31)
#define KEYPAD_KP_ON_MASK          (1<<31)
#define KEYPAD_KP_ON_SHIFT         (31)

//KP_CTRL
#define KEYPAD_KP_EN               (1<<0)
#define KEYPAD_KP_DBN_TIME(n)      (((n)&0xFF)<<2)
#define KEYPAD_KP_ITV_TIME(n)      (((n)&0x3F)<<10)
#define KEYPAD_KP_IN_MASK(n)       (((n)&0xFF)<<16)
#define KEYPAD_KP_OUT_MASK(n)      (((n)&0xFF)<<24)

//KP_IRQ_MASK
#define KEYPAD_KP_EVT0_IRQ_MASK    (1<<0)
#define KEYPAD_KP_EVT1_IRQ_MASK    (1<<1)
#define KEYPAD_KP_ITV_IRQ_MASK     (1<<2)

//KP_IRQ_CAUSE
#define KEYPAD_KP_EVT0_IRQ_CAUSE   (1<<0)
#define KEYPAD_KP_EVT1_IRQ_CAUSE   (1<<1)
#define KEYPAD_KP_ITV_IRQ_CAUSE    (1<<2)
#define KEYPAD_KP_EVT0_IRQ_STATUS  (1<<16)
#define KEYPAD_KP_EVT1_IRQ_STATUS  (1<<17)
#define KEYPAD_KP_ITV_IRQ_STATUS   (1<<18)

//KP_IRQ_CLR
#define KEYPAD_KP_IRQ_CLR          (1<<0)





#endif

