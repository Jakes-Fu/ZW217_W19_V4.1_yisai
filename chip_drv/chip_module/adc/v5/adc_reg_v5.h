/******************************************************************************
 ** File Name:        adc_reg_v3.h                                    *
 ** Author:           Yi.Qiu                                                  *
 ** DATE:             09/16/2009                                              *
 ** Copyright:        2009 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE            NAME            DESCRIPTION                               *
 ** 09/16/2011      lichd           Modifyed for sc6530                       *
 ******************************************************************************/
#ifndef _SC6530_REG_ADC_H_
#define _SC6530_REG_ADC_H_
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
/* ADC Ctronl Registers */
#define ADC_REG_BASE                        ADIE_ADC_REG_START     //in a-die

#define ADC_CTRL                            (ADC_REG_BASE + 0x0000)
#define ADC_SW_CH_CFG                       (ADC_REG_BASE + 0x0004)

// FAST_HW_CHx: CH0[0x0008] ~ CH7[0x0024]
#define ADC_FAST_HW_CHx_CFG(x)              (ADC_REG_BASE + 0x0008 + (x)*0x04)

// SLOW_HW_CHx: CH0[0x0028] ~ CH7[0x0044]
#define ADC_SLOW_HW_CHx_CFG(x)              (ADC_REG_BASE + 0x0028 + (x)*0x04)

#define ADC_HW_CH_DELAY                     (ADC_REG_BASE + 0x0048)
#define ADC_DATA                            (ADC_REG_BASE + 0x004C)
#define ADC_INT_EN                          (ADC_REG_BASE + 0x0050)
#define ADC_INT_CLR                         (ADC_REG_BASE + 0x0054)
#define ADC_INT_STS                         (ADC_REG_BASE + 0x0058)
#define ADC_INT_RAW                         (ADC_REG_BASE + 0x005C)
#define ADC_DEBUG                           (ADC_REG_BASE + 0x0060)
 
///ADC_CTRL bit field
#define ADC_CTRL_SWCH_RUN_NUM_SHIFT         4 // [07:04]
#define ADC_CTRL_SWCH_RUN_NUM_MASK          (0xF<<ADC_CTRL_SWCH_RUN_NUM_SHIFT)
#define ADC_CTRL_SWCH_RUN                   BIT_1
#define ADC_CTRL_ADC_EN                     BIT_0

///ADC_SW_CH_CFG bit field
#define ADC_SW_CH_SLOW_MODE_EN              BIT_5
#define ADC_SW_CH_BIG_SCALE_EN              BIT_4
#define ADC_SW_CH_ID_SHIFT                  0 //[3:00]
#define ADC_SW_CH_ID_MASK                   (0x0F<<ADC_SW_CH_ID_SHIFT)


#ifdef CHIP_VER_6530
///ADC_HW_CH_CFG bit field
#define ADC_HW_CH_DLY_EN                    BIT_6
#define ADC_HW_CH_SLOW_MODE_EN              BIT_5
#define ADC_HW_CH_BIG_SCALE_EN              BIT_4
#define ADC_HW_CH_ID_SHIFT                  0 //[3:00]
#define ADC_HW_CH_ID_MASK                   (0x0F<<ADC_SW_CH_ID_SHIFT)
#else
///ADC_HW_CH_CFG bit field
#define ADC_HW_CH_CAPACITY_MODE             BIT_8
#define ADC_HW_CH_DLY_EN                    BIT_7
#define ADC_HW_CH_SLOW_MODE_EN              BIT_6
#define ADC_HW_CH_BIG_SCALE_EN              BIT_5
#define ADC_HW_CH_ID_SHIFT                  0 //[4:00]
#define ADC_HW_CH_ID_MASK                   (0x1F<<ADC_SW_CH_ID_SHIFT)
#endif

///ADC_HW_CH_DELAY bit field
#define ADC_HW_CH_DELAY_SHIFG               0 //[7:0]
#define ADC_HW_CH_DELAY_MASK                (0xFF)

// ADC_DAT bit field [9:0]
#define ADC_DATA_MSK                        (0x3FF)

// ADC convertor in chip
// Used for ADC_IRQ_EN /ADC_IRQ_CLR /ADC_IRQ_STS /ADC_IRQ_RAW
#define ADC_IRQ_MASK                         BIT_0


// Chip Specific information 
#define TPC_YU_REQ_CHANNEL            0
#define TPC_YD_REQ_CHANNEL            1
#define TPC_XL_REQ_CHANNEL            2
#define TPC_XR_REQ_CHANNEL            3

#ifdef CHIP_VER_6530
#if 1
//sc6530 EVB and PHONEV1.0.1 
#define ADC_HW_CHANNEL_FOR_TPCYU  0
#define ADC_HW_CHANNEL_FOR_TPCYD  2
#define ADC_HW_CHANNEL_FOR_TPCXL  1
#define ADC_HW_CHANNEL_FOR_TPCXR  3
#else
//HW board specified
//Old phone
#define ADC_HW_CHANNEL_FOR_TPCYU  1
#define ADC_HW_CHANNEL_FOR_TPCYD  2
#define ADC_HW_CHANNEL_FOR_TPCXL  0
#define ADC_HW_CHANNEL_FOR_TPCXR  3
#endif

#else

#ifdef FPGA_VERIFICATION
#define ADC_HW_CHANNEL_FOR_TPCYU  0
#define ADC_HW_CHANNEL_FOR_TPCYD  2
#define ADC_HW_CHANNEL_FOR_TPCXL  1
#define ADC_HW_CHANNEL_FOR_TPCXR  3
#else
#define ADC_HW_CHANNEL_FOR_TPCXL  9
#define ADC_HW_CHANNEL_FOR_TPCXR  10
#define ADC_HW_CHANNEL_FOR_TPCYD  11
#define ADC_HW_CHANNEL_FOR_TPCYU  12
#endif

#endif

#define ADC_CH_MAX_NUM                  16

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_SC6530_REG_ADC_H_



