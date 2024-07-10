/******************************************************************************
 ** File Name:    osc_reg_v5.h                                             *
 ** Author:       Kui.wang                                                     *
 ** DATE:         24/08/2012                                                  *
 ** Copyright:    2009 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *

 ******************************************************************************/

#ifndef _OSC_REG_V0_H_
#define _OSC_REG_V0_H_

#ifdef   __cplusplus
extern   "C"
{
#endif

/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
#define ANA_SELF_CAL_BASE  ANA_CAL_BASE

#define OSC_CAL_START				(ANA_SELF_CAL_BASE + 0x0000)
#define OSC_CAL_CYCLE_P0		(ANA_SELF_CAL_BASE + 0x0004)
#define OSC_CAL_CYCLE_P1		(ANA_SELF_CAL_BASE + 0x0008)
#define OSC_CAL_RESULT_P0		(ANA_SELF_CAL_BASE + 0x000C)
#define OSC_CAL_RESULT_P1		(ANA_SELF_CAL_BASE + 0x0010)
#define OSC_DIV_FAC_UPD			(ANA_SELF_CAL_BASE + 0x0014)
#define OSC_DIV_FAC_P0			(ANA_SELF_CAL_BASE + 0x0018)
#define OSC_DIV_FAC_P1			(ANA_SELF_CAL_BASE + 0x001C)
#define OSC_DIV_INT_EN			(ANA_SELF_CAL_BASE + 0x0020)
#define OSC_DIV_INT_CLR			(ANA_SELF_CAL_BASE + 0x0024)
#define OSC_DIV_INT_RAW			(ANA_SELF_CAL_BASE + 0x0028)

#define CAL_CYCLE_P1_MSK		0x00FF0000
#define CAL_CYCLE_P1_SHIFT		0x00000010
#define CAL_CYCLE_P0_MSK		0x0000FFFF

#define CAL_RESULT_P1_SHIFT		0x00000010

#define OSC_DIV_FAC_INT_SHIFT			0
#define OSC_DIV_FAC_INT_MSK				(0xF << OSC_DIV_FAC_INT_SHIFT)

#define CAL_START_BUSY			BIT_0

#define OSC_DIV_FAC_FRAC_SHIFT			0
#define OSC_DIV_FAC_FRAC_MSK				(0xFFFF << OSC_DIV_FAC_FRAC_SHIFT)
#define OSC_DIV_FAC_UPD_EN					BIT_0

#define OSC_CAL_DONE_INT_CLR_SHIFT			0
#define OSC_CAL_DONE_INT_CLR_MASK				(0x1 << OSC_CAL_DONE_INT_CLR_SHIFT)
#define OSC_FAC_UPD_DONE_INT_CLR_SHIFT			1
#define OSC_FAC_UPD_DONE_INT_CLR_MASK				(0x1 << OSC_FAC_UPD_DONE_INT_CLR_SHIFT)

#define OSC_CAL_DONE_INT_RAW_SHIFT			0
#define OSC_CAL_DONE_INT_RAW_MASK				(0x1 << OSC_CAL_DONE_INT_RAW_SHIFT)
#define OSC_FAC_UPD_DONE_INT_RAW_SHIFT			1
#define OSC_FAC_UPD_DONE_INT_RAW_MASK				(0x1 << OSC_FAC_UPD_DONE_INT_RAW_SHIFT)

#ifdef   __cplusplus
}
#endif

#endif
