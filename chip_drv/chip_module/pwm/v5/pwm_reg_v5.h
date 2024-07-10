/******************************************************************************
 ** File Name:    pwm_reg_v5.h                                            *
 ** Author:       Steve.Zhan                                                 *
 ** DATE:         06/05/2010                                                  *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/05/2010    Steve.Zhan      Create.                                     *
 ******************************************************************************/
#ifndef _PWM_REG_V5_H_
#define _PWM_REG_V5_H_
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
typedef struct pwm_tag
{
    VOLATILE uint32     prescale;
    VOLATILE uint32     cnt_duty;
} pwm_s;

#define    PWM_PRESCALE        (0)
#define    PWM_CNT             (0x04)
#define    PWM_DIV             (0x08)
#define    PWM_PAT_LOW         (0x0C)
#define    PWM_PAT_HIGH        (0x10)
/*******************************************************************************
 ** Register Name        : PWM_PRESCALE
 ** Register Offset      : 0x0000
 ** Register Description :  PWM prescale coefficient for work clock.
 ** Register field description:
 ** PWM_BUSY                 : [15   ] PWM status 0- idle; 1- busy.
 ** PWM_EN                   : [8    ] PWMx enable 0: disable; 1: enable.
 ** PWM_PRESCALE             : [7 :0 ] PWMx prescale coefficient.
 **************************   Fields Definition *******************************/
#define    PWM_BUSY                        (BIT_15)
#define    PWM_POLARITY_CTL                (BIT_9)
#define    PWM_EN                          (BIT_8)
#define    PWM_PRESCALE_SHIFT              (0)
#define    PWM_PRESCALE_MSK                (0xFFUL << PWM_PRESCALE_SHIFT)


/*******************************************************************************
 ** Register Name        : PWM_CNT
 ** Register Offset      : 0x0004
 ** Register Description :  PWM counter.
 ** Register field description:
 ** PWM_DUTY                 : [15:8 ] PWMx duty counter.
 ** PWM_MOD                  : [7 :0 ] PWMx mod counter.
 **************************   Fields Definition *******************************/
#define    PWM_DUTY_SHIFT                  (8)
#define    PWM_DUTY_MSK                    (0xFFUL << PWM_DUTY_SHIFT)
#define    PWM_MOD_SHIFT                   (0)
#define    PWM_MOD_MSK                     (0xFFUL << PWM_MOD_SHIFT)

/*******************************************************************************
 ** Register Name        : PWM_DIV
 ** Register Offset      : 0x0008
 ** Register Description :  PWM tone divider.
 ** Register field description:
 ** PWM_DIV                  : [15:0 ] PWMx tone divider.
 **************************   Fields Definition *******************************/
#define    PWM_DIV_SHIFT                   (0)
#define    PWM_DIV_MSK                     (0xFFFFUL << PWM_DIV_SHIFT)

/*******************************************************************************
 ** Register Name        : PWM_PAT_LOW
 ** Register Offset      : 0x000C
 ** Register Description :  PWM pattern low part.
 ** Register field description:
 ** PWM_PAT_LOW              : [15:0 ] PWMx pattern low part.
 **************************   Fields Definition *******************************/
#define    PWM_PAT_LOW_SHIFT               (0)
#define    PWM_PAT_LOW_MSK                 (0xFFFFUL << PWM_PAT_LOW_SHIFT)

/*******************************************************************************
 ** Register Name        : PWM_PAT_HIGH
 ** Register Offset      : 0x0010
 ** Register Description :  PWM pattern high part.
 ** Register field description:
 ** PWM_PAT_HIGH             : [15:0 ] PWMx pattern high part.
 **************************   Fields Definition *******************************/
#define    PWM_PAT_HIGH_SHIFT              (0)
#define    PWM_PAT_HIGH_MSK                (0xFFFFUL << PWM_PAT_HIGH_SHIFT)


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
