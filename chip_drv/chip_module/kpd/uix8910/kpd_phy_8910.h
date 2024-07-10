/******************************************************************************
 ** File Name:    kpd_reg_v0.h                                            *
 ** Author:       hao.liu                                                 *
 ** DATE:         06/11/2010                                                  *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/11/2010    hao.liu      Create.                                     *
 ******************************************************************************/
#ifndef _KPD_REG_V5_H_
#define _KPD_REG_V5_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

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
// =============================================================================
//  MACROS
// =============================================================================
/// Number of key in the keypad
#define KEY_NB                                  (40)   // 36
/// Number of key in the low data register
#define LOW_KEY_NB                              (32)   // 30
/// Number of key in the high data register
#define HIGH_KEY_NB                             (6)    // 4

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
#define REG_KEYPAD_BASE            0x50106000
#endif

typedef volatile struct
{
    VOLATILE uint32                          KP_DATA_L;                    //0x00000000
    VOLATILE uint32                          KP_DATA_H;                    //0x00000004
    VOLATILE uint32                          KP_STATUS;                    //0x00000008
    VOLATILE uint32                          KP_CTRL;                      //0x0000000C
    VOLATILE uint32                          KP_IRQ_MASK;                  //0x00000010
    VOLATILE uint32                          KP_IRQ_CAUSE;                 //0x00000014
    VOLATILE uint32                          KP_IRQ_CLR;                   //0x00000018
} HWP_KEYPAD_T;

#define hwp_keypad                 ((HWP_KEYPAD_T*) REG_ACCESS_ADDRESS(REG_KEYPAD_BASE))


//KP_STATUS
typedef union {
    VOLATILE uint32 v;
    struct {
        VOLATILE uint32 keyin_status : 8; // [7:0], read only
        VOLATILE uint32 __30_8 : 23;
        VOLATILE uint32 kp_on : 1; // [31], read only
    } b;
} REG_KEYPAD_KP_STATUS_T;

//KP_CTRL
typedef union {
    VOLATILE uint32 v;
    struct {
        VOLATILE uint32 kp_en : 1; // [0]
        VOLATILE uint32 __1_1 : 1;
        VOLATILE uint32 kp_dbn_time : 8; // [9:2]
        VOLATILE uint32 kp_itv_time : 6; // [15:10]
        VOLATILE uint32 kp_in_mask : 8; // [23:16]
        VOLATILE uint32 kp_out_mask : 8; // [31:24]
    } b;
} REG_KEYPAD_KP_CTRL_T;

//KP_IRQ_MASK
typedef union {
    VOLATILE uint32 v;
    struct {
        VOLATILE uint32 kp_evt0_irq_mask : 1; // [0]
        VOLATILE uint32 kp_evt1_irq_mask : 1; // [1]
        VOLATILE uint32 kp_itv_irq_mask : 1; // [2]
        VOLATILE uint32 __31_3 : 29;
    } b;
} REG_KEYPAD_KP_IRQ_MASK_T;

//KP_IRQ_CAUSE
typedef union {
    VOLATILE uint32 v;
    struct {
        VOLATILE uint32 kp_evt0_irq_cause : 1; // [0], read only
        VOLATILE uint32 kp_evt1_irq_cause : 1; // [1], read only
        VOLATILE uint32 kp_itv_irq_cause : 1; // [2], read only
        VOLATILE uint32 __15_3 : 13;
        VOLATILE uint32 kp_evt0_irq_status : 1; // [16], read only
        VOLATILE uint32 kp_evt1_irq_status : 1; // [17], read only
        VOLATILE uint32 kp_itv_irq_status : 1; // [18], read only
        VOLATILE uint32 __31_19 : 13;
    } b;
} REG_KEYPAD_KP_IRQ_CAUSE_T;

//KP_IRQ_CLR
typedef union {
    VOLATILE uint32 v;
    struct {
        VOLATILE uint32 kp_irq_clr : 1; // [0], write clear
        VOLATILE uint32 __31_1 : 31;
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

typedef enum
{
    HAL_KEY_DOWN = 0,
    HAL_KEY_PRESSED,
    HAL_KEY_UP
}HAL_KEY_STATE_T;


/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
