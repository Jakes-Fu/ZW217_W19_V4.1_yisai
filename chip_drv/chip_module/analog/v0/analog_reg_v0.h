/******************************************************************************
 ** File Name:      analog_reg_v0.h                                           *
 ** Author:         Jeff.Li                                                   *
 ** DATE:           11/06/2010                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Analog interface.                                         *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/06/2010     Jeff.Li          Create for new chip driver architecture.  *
 *****************************************************************************/

#ifndef _ANALOG_REG_V0_H_
#define _ANALOG_REG_V0_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         MACOR Definitions                                 *
 **---------------------------------------------------------------------------*/
// Keypad LED current adjust
#define ANA_CTL_KPLED_PD_SET_SHIFT      BIT_7
#define ANA_CTL_KPLED_PD_RST_SHIFT      BIT_8

#define ANA_CTL_KPLED_V_SHIFT           (9)
#define ANA_CTL_KPLED_V_MASK            (0x7 << ANA_CTL_KPLED_V_SHIFT)

// Viberator current adjust
#define ANA_CTL_VIBR_PD_SET_SHIFT       BIT_12
#define ANA_CTL_VIBR_PD_RST_SHIFT       BIT_13

#define ANA_CTL_VIBR_V_SHIFT            (14)
#define ANA_CTL_VIBR_V_MASK             (0x7 << ANA_CTL_VIBR_V_SHIFT)

// White LED current adjust
#define ANA_CTL_WHTLED_PD_SET_SHIFT     BIT_0
#define ANA_CTL_WHTLED_PD_RST_SHIFT     BIT_1

#define ANA_CTL_WHTLED_V_SHIFT          (2)
#define ANA_CTL_WHTLED_V_MASK           (0x1F << ANA_CTL_WHTLED_V_SHIFT)

// White LED Statusa (0x8b00005C[9])
#define WHTLED_PD_STS                         BIT_9

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


#endif    // _ANALOG_REG_V0_H_
