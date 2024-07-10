/******************************************************************************
 ** File Name:        tpc_reg_v3p0.h                                       *
 ** Author:           Ryan.Liao                                                  *
 ** DATE:             07/03/2012                                              *
 ** Copyright:        2012 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE            NAME            DESCRIPTION                               *
 ** 09/20/2012		Ryan.Liao	  Modified for SC6531						  *
 ******************************************************************************/
#ifndef _TPC_REG_R3P0_H_
#define _TPC_REG_R3P0_H_
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
/* TPC control registers map*/
#define TPC_REG_BASE                ADIE_TPC_REG_START
#define TPC_CTRL                    (TPC_REG_BASE + 0x0000)
#define TPC_SAMPLE_CTRL0            (TPC_REG_BASE + 0x0004)
#define TPC_SAMPLE_CTRL1            (TPC_REG_BASE + 0x0008)
#define TPC_SAMPLE_CTRL2            (TPC_REG_BASE + 0x000C)

#define TPC_BOUNCE_CTRL             (TPC_REG_BASE + 0x0010)
#define TPC_FILTER_CTRL             (TPC_REG_BASE + 0x0014)
#define TPC_CALC_CTRL               (TPC_REG_BASE + 0x0018)
#define	TPC_CALC_SEQ				(TPC_REG_BASE + 0x001C)

#define TPC_CALC_X_COEF_A           (TPC_REG_BASE + 0x0020)
#define TPC_CALC_X_COEF_B           (TPC_REG_BASE + 0x0024)
#define TPC_CALC_Y_COEF_A           (TPC_REG_BASE + 0x0028)
#define TPC_CALC_Y_COEF_B           (TPC_REG_BASE + 0x002C)

#define TPC_INT_EN                  (TPC_REG_BASE + 0x0030)
#define TPC_INT_STS                 (TPC_REG_BASE + 0x0034)
#define TPC_INT_RAW                 (TPC_REG_BASE + 0x0038)
#define TPC_INT_CLR                 (TPC_REG_BASE + 0x003C)
#define TPC_BUF_CTRL                (TPC_REG_BASE + 0x0040)

#define TPC_OUT_SEQ0                (TPC_REG_BASE + 0x0044)
#define TPC_OUT_SEQ1                (TPC_REG_BASE + 0x0048)
#define TPC_OUT_SEQ2				(TPC_REG_BASE + 0x004C)

#define TPC_OE_SEQ0                 (TPC_REG_BASE + 0x0050)
#define TPC_OE_SEQ1                 (TPC_REG_BASE + 0x0054)
#define	TPC_OE_SEQ2					(TPC_REG_BASE + 0x0058)

#define TPC_CH_SEQ0                 (TPC_REG_BASE + 0x005C)
#define TPC_CH_SEQ1					(TPC_REG_BASE + 0x0060)

#define TPC_RES_SEQ0				(TPC_REG_BASE + 0x0064)
#define TPC_RES_SEQ1                (TPC_REG_BASE + 0x0068)
#define	TPC_RES_SEQ2				(TPC_REG_BASE + 0x006C)

#define TPC_SEQ_ADDR                (TPC_REG_BASE + 0x0070)
#define TPC_SEQ_DATA                (TPC_REG_BASE + 0x0074)

#define TPC_DATA_MIN                (TPC_REG_BASE + 0x0078)
#define TPC_DATA_MAX                (TPC_REG_BASE + 0x007C)

#define TPC_DBNC_DEBUG              (TPC_REG_BASE + 0x0080)
#define TPC_SAMPLE_DEBUG            (TPC_REG_BASE + 0x0084)
#define TPC_BUF_DEBUG               (TPC_REG_BASE + 0x0088)

#define	TPC_RES_SEL					(TPC_REG_BASE + 0x008C)

//TPC_CTRL BIT map
#define TPC_CTRL_STATIC_MODE        BIT_4
#define TPC_CTRL_STOP               BIT_3
#define TPC_CTRL_RUN                BIT_2
#define TPC_CTRL_PEN_REQ_POL        BIT_1
#define TPC_CTRL_EN                 BIT_0

#define TPC_CTRL_PRESCALE_OFFSET    0x08  //[15:08]
#define TPC_CTRL_PRESCALE_MSK       (0xFF << TPC_CTRL_PRESCALE_OFFSET)

//TPC_SAMPLE_CTRL0 BIT map
#define TPC_SAMPLE_CTRL0_SAMPLE_NUM_OFFSET  0 //[3:0]
#define TPC_SAMPLE_CTRL0_SAMPLE_NUM_MSK     (0xF<<TPC_SAMPLE_CTRL0_SAMPLE_NUM_OFFSET)
#define TPC_SAMPLE_CTRL0_SEQ_NUM_OFFSET     4 //[7:4]
#define TPC_SAMPLE_CTRL0_SEQ_NUM_MSK        (0xF<<TPC_SAMPLE_CTRL0_SEQ_NUM_OFFSET)

//TPC_SAMPLE_CTRL1 BIT map
#define TPC_SAMPLE_CTRL1_SAMPLE_INTERVAL_OFFSET 0 //[7:0]
#define TPC_SAMPLE_CTRL1_SAMPLE_INTERVAL_MSK    (0xFF <<TPC_SAMPLE_CTRL1_SAMPLE_INTERVAL_OFFSET)
#define TPC_SAMPLE_CTRL1_POINT_INTERVAL_OFFSET  8 //[15:8]
#define TPC_SAMPLE_CTRL1_POINT_INTERVAL_MSK     (0xFF <<TPC_SAMPLE_CTRL1_SAMPLE_INTERVAL_OFFSET)

//TPC_SAMPLE_CTRL2 BIT map
#define TPC_SAMPLE_CTRL2_DATA_INTERVAL_OFFSET 0 //[11:0]
#define TPC_SAMPLE_CTRL2_DATA_INTERVAL_MSK    (0xFFF <<0)

//TPC_BOUNCE_CTRL BIT map
#define TPC_DEBOUNCE_EN_BIT                   BIT_0
#define TPC_DEBOUNCE_NUM_OFFSET               4  //[11:4]
#define TPC_DEBOUNCE_NUM_MSK                  (0xFF <<TPC_DEBOUNCE_NUM_OFFSET)

//TPC_FILTER_CTRL BIT map
#define TPC_FILTER_EN                         BIT_0
#define TPC_FILTER_MODE_MEDIA_FILTER          BIT_1

//TPC_CALC_CTRL BIT map
#define TPC_CALC_CTRL_EN                      BIT_0

#define TPC_CALC_SEQX0_OFFSET                  0 //[3:0]
#define TPC_CALC_SEQX0_MSK                     (0xF<<TPC_CALC_SEQX0_OFFSET)
#define	TPC_CALC_SEQY0_OFFSET				   4	//[7:4]
#define	TPC_CALC_SEQY0_MSK					   (0xF<<TPC_CALC_SEQY0_OFFSET)
#define	TPC_CALC_SEQX1_OFFSET				   8	//[11:8]
#define	TPC_CALC_SEQX1_MSK					   (0xF<<TPC_CALC_SEQX1_OFFSET)
#define TPC_CALC_SEQY1_OFFSET                  12 //[15:12]
#define TPC_CALC_SEQY1_MSK                     (0xF<<TPC_CALC_SEQY1_OFFSET)

//TPC_CALC_X_COEF_A
#define TPC_X_COEF_A_MSK                      0x3FF  //[9:0]
#define TPC_X_COEF_B_MSK                      0x1FFF  //[12:0]
#define TPC_Y_COEF_A_MSK                      0x3FF  //[9:0]
#define TPC_Y_COEF_B_MSK                      0x1FFF  //[12:0]

//TPC INT BIT MAP
#define TPC_DONE_IRQ_MSK                      BIT_2
#define TPC_UP_IRQ_MSK                        BIT_1
#define TPC_DOWN_IRQ_MSK                      BIT_0
#define TPC_INT_ALL_MSK         			  (TPC_DONE_IRQ_MSK|TPC_UP_IRQ_MSK|TPC_DOWN_IRQ_MSK)

//TPC_BUF_CTRL BIT map
#define TPC_BUF_CTRL_CLR                      BIT_4
#define TPC_BUF_LENGTH_OFFSET                 0 //[3:0]
#define TPC_BUF_LENGTH_MSK                    (0xF << TPC_BUF_LENGTH_OFFSET)

//TPC_OUT_SEQ0 bit field
#define TPC_OUT_SEQ0_PAT0_OFFSET              0 //[3:0]
#define TPC_OUT_SEQ0_PAT0_MSK                 (0xF<<TPC_OUT_SEQ0_PAT0_OFFSET)
#define TPC_OUT_SEQ0_PAT1_OFFSET              4 //[7:4]
#define TPC_OUT_SEQ0_PAT1_MSK                 (0xF<<TPC_OUT_SEQ0_PAT1_OFFSET)
#define TPC_OUT_SEQ0_PAT2_OFFSET              8 //[11:8]
#define TPC_OUT_SEQ0_PAT2_MSK                 (0xF<<TPC_OUT_SEQ0_PAT2_OFFSET)
#define TPC_OUT_SEQ0_PAT3_OFFSET              12 //[15:12]
#define TPC_OUT_SEQ0_PAT3_MSK                 (0xF<<TPC_OUT_SEQ0_PAT3_OFFSET)

//TPC_OUT_SEQ1 bit field
#define TPC_OUT_SEQ1_PAT4_OFFSET              0 //[3:0]
#define TPC_OUT_SEQ1_PAT4_MSK                 (0xF<<TPC_OUT_SEQ1_PAT4_OFFSET)
#define TPC_OUT_SEQ1_PAT5_OFFSET              4 //[7:4]
#define TPC_OUT_SEQ1_PAT5_MSK                 (0xF<<TPC_OUT_SEQ1_PAT5_OFFSET)
#define TPC_OUT_SEQ1_PAT6_OFFSET              8 //[11:8]
#define TPC_OUT_SEQ1_PAT6_MSK                 (0xF<<TPC_OUT_SEQ1_PAT6_OFFSET)
#define TPC_OUT_SEQ1_PAT7_OFFSET              12 //[15:12]
#define TPC_OUT_SEQ1_PAT7_MSK                 (0xF<<TPC_OUT_SEQ1_PAT7_OFFSET)

//TPC_OUT_SEQ2 bit field
#define TPC_OUT_SEQ2_PAT8_OFFSET              0 //[3:0]
#define TPC_OUT_SEQ2_PAT8_MSK                 (0xF<<TPC_OUT_SEQ1_PAT4_OFFSET)
#define TPC_OUT_SEQ2_PAT9_OFFSET              4 //[7:4]
#define TPC_OUT_SEQ2_PAT9_MSK                 (0xF<<TPC_OUT_SEQ1_PAT5_OFFSET)
#define TPC_OUT_SEQ2_PAT10_OFFSET              8 //[11:8]
#define TPC_OUT_SEQ2_PAT10_MSK                 (0xF<<TPC_OUT_SEQ1_PAT6_OFFSET)
#define TPC_OUT_SEQ2_PAT11_OFFSET              12 //[15:12]
#define TPC_OUT_SEQ2_PAT11_MSK                 (0xF<<TPC_OUT_SEQ1_PAT7_OFFSET)

//TPC_OE_SEQ0 bit field
#define TPC_OE_SEQ0_PAT0_OFFSET               0 //[3:0]
#define TPC_OE_SEQ0_PAT0_MSK                  (0xF<<TPC_OE_SEQ0_PAT0_OFFSET)
#define TPC_OE_SEQ0_PAT1_OFFSET               4 //[7:4]
#define TPC_OE_SEQ0_PAT1_MSK                  (0xF<<TPC_OE_SEQ0_PAT1_OFFSET)
#define TPC_OE_SEQ0_PAT2_OFFSET               8 //[11:8]
#define TPC_OE_SEQ0_PAT2_MSK                  (0xF<<TPC_OE_SEQ0_PAT2_OFFSET)
#define TPC_OE_SEQ0_PAT3_OFFSET               12 //[15:12]
#define TPC_OE_SEQ0_PAT3_MSK                  (0xF<<TPC_OE_SEQ0_PAT3_OFFSET)

//TPC_OE_SEQ1 bit field
#define TPC_OE_SEQ1_PAT4_OFFSET             0 //[3:0]
#define TPC_OE_SEQ1_PAT4_MSK                (0xF<<TPC_OE_SEQ1_PAT4_OFFSET)
#define TPC_OE_SEQ1_PAT5_OFFSET             4 //[7:4]
#define TPC_OE_SEQ1_PAT5_MSK                (0xF<<TPC_OE_SEQ1_PAT5_OFFSET)
#define TPC_OE_SEQ1_PAT6_OFFSET             8 //[11:8]
#define TPC_OE_SEQ1_PAT6_MSK                (0xF<<TPC_OE_SEQ1_PAT6_OFFSET)
#define TPC_OE_SEQ1_PAT7_OFFSET             12 //[15:12]
#define TPC_OE_SEQ1_PAT7_MSK                (0xF<<TPC_OE_SEQ1_PAT7_OFFSET)

//TPC_OE_SEQ2 bit field
#define TPC_OE_SEQ2_PAT8_OFFSET             0 //[3:0]
#define TPC_OE_SEQ2_PAT8_MSK                (0xF<<TPC_OE_SEQ1_PAT4_OFFSET)
#define TPC_OE_SEQ2_PAT9_OFFSET             4 //[7:4]
#define TPC_OE_SEQ2_PAT9_MSK                (0xF<<TPC_OE_SEQ1_PAT5_OFFSET)
#define TPC_OE_SEQ2_PAT10_OFFSET             8 //[11:8]
#define TPC_OE_SEQ2_PAT10_MSK                (0xF<<TPC_OE_SEQ1_PAT6_OFFSET)
#define TPC_OE_SEQ2_PAT11_OFFSET             12 //[15:12]
#define TPC_OE_SEQ2_PAT11_MSK                (0xF<<TPC_OE_SEQ1_PAT7_OFFSET)

//TPC_CH_SEQ0
#define TPC_CH_SEQ0_PAT0_OFFSET             0 //[1:0]
#define TPC_CH_SEQ0_PAT0_MSK                (0x3<<TPC_CH_SEQ_PAT0_OFFSET)
#define TPC_CH_SEQ0_PAT1_OFFSET             2 //[3:2]
#define TPC_CH_SEQ0_PAT1_MSK                (0x3<<TPC_CH_SEQ_PAT1_OFFSET)
#define TPC_CH_SEQ0_PAT2_OFFSET             4 //[5:4]
#define TPC_CH_SEQ0_PAT2_MSK                (0x3<<TPC_CH_SEQ_PAT2_OFFSET)
#define TPC_CH_SEQ0_PAT3_OFFSET             6 //[7:6]
#define TPC_CH_SEQ0_PAT3_MSK                (0x3<<TPC_CH_SEQ_PAT3_OFFSET)
#define TPC_CH_SEQ0_PAT4_OFFSET             8 //[9:8]
#define TPC_CH_SEQ0_PAT4_MSK                (0x3<<TPC_CH_SEQ_PAT4_OFFSET)
#define TPC_CH_SEQ0_PAT5_OFFSET             10 //[11:10]
#define TPC_CH_SEQ0_PAT5_MSK                (0x3<<TPC_CH_SEQ_PAT5_OFFSET)
#define TPC_CH_SEQ0_PAT6_OFFSET             12 //[13:12]
#define TPC_CH_SEQ0_PAT6_MSK                (0x3<<TPC_CH_SEQ_PAT6_OFFSET)
#define TPC_CH_SEQ0_PAT7_OFFSET             14 //[15:14]
#define TPC_CH_SEQ0_PAT7_MSK                (0x3<<TPC_CH_SEQ_PAT7_OFFSET)

//TPC_CH_SEQ1
#define TPC_CH_SEQ1_PAT8_OFFSET             0 //[1:0]
#define TPC_CH_SEQ1_PAT8_MSK                (0x3<<TPC_CH_SEQ_PAT0_OFFSET)
#define TPC_CH_SEQ1_PAT9_OFFSET             2 //[3:2]
#define TPC_CH_SEQ1_PAT9_MSK                (0x3<<TPC_CH_SEQ_PAT1_OFFSET)
#define TPC_CH_SEQ1_PAT10_OFFSET             4 //[5:4]
#define TPC_CH_SEQ1_PAT10_MSK                (0x3<<TPC_CH_SEQ_PAT2_OFFSET)
#define TPC_CH_SEQ1_PAT11_OFFSET             6 //[7:6]
#define TPC_CH_SEQ1_PAT11_MSK                (0x3<<TPC_CH_SEQ_PAT3_OFFSET)


//TPC_RES_SEQ0 bit field
#define TPC_RES_SEQ0_PAT0_OFFSET             0 //[1:0]
#define TPC_RES_SEQ0_PAT0_MSK                (0xF<<TPC_RES_SEQ_PAT0_OFFSET)
#define TPC_RES_SEQ0_PAT1_OFFSET             4 //[3:2]
#define TPC_RES_SEQ0_PAT1_MSK                (0xF<<TPC_RES_SEQ_PAT0_OFFSET)
#define TPC_RES_SEQ0_PAT2_OFFSET             8 //[5:4]
#define TPC_RES_SEQ0_PAT2_MSK                (0xF<<TPC_RES_SEQ_PAT0_OFFSET)
#define TPC_RES_SEQ0_PAT3_OFFSET             12 //[7:6]
#define TPC_RES_SEQ0_PAT3_MSK                (0xF<<TPC_RES_SEQ_PAT0_OFFSET)

//TPC_RES_SEQ1 bit field
#define TPC_RES_SEQ1_PAT4_OFFSET             0 //[9:8]
#define TPC_RES_SEQ1_PAT4_MSK                (0xF<<TPC_RES_SEQ_PAT0_OFFSET)
#define TPC_RES_SEQ1_PAT5_OFFSET             4 //[11:10]
#define TPC_RES_SEQ1_PAT5_MSK                (0xF<<TPC_RES_SEQ_PAT0_OFFSET)
#define TPC_RES_SEQ1_PAT6_OFFSET             8 //[13:12]
#define TPC_RES_SEQ1_PAT6_MSK                (0xF<<TPC_RES_SEQ_PAT0_OFFSET)
#define TPC_RES_SEQ1_PAT7_OFFSET             12 //[15:14]
#define TPC_RES_SEQ1_PAT7_MSK                (0xF<<TPC_RES_SEQ_PAT0_OFFSET)

//TPC_RES_SEQ2 bit field
#define TPC_RES_SEQ2_PAT4_OFFSET             0 //[9:8]
#define TPC_RES_SEQ2_PAT4_MSK                (0xF<<TPC_RES_SEQ_PAT0_OFFSET)
#define TPC_RES_SEQ2_PAT5_OFFSET             4 //[11:10]
#define TPC_RES_SEQ2_PAT5_MSK                (0xF<<TPC_RES_SEQ_PAT0_OFFSET)
#define TPC_RES_SEQ2_PAT6_OFFSET             8 //[13:12]
#define TPC_RES_SEQ2_PAT6_MSK                (0xF<<TPC_RES_SEQ_PAT0_OFFSET)
#define TPC_RES_SEQ2_PAT7_OFFSET             12 //[15:14]
#define TPC_RES_SEQ2_PAT7_MSK                (0xF<<TPC_RES_SEQ_PAT0_OFFSET)

//TPC_SEQ_ADDR
#define TPC_SEQ_ADDR_OFFSET                0 //[2:0]
#define TPC_SEQ_ADDR_MSK                   (0xF<<0)

//TPC_SEQ_DATA
#define TPC_SEQ_DATA_OFFSET                0 //[9:0]
#define TPC_SEQ_DATA_MSK                   (0xFFF<<0)

//TPC_DATA_MIN
#define TPC_DATA_MIN_OFFSET                0 //[9:0]
#define TPC_DATA_MIN_MSK                   (0xFFF<<0)

//TPC_DATA_MAX
#define TPC_DATA_MAX_OFFSET                0 //[9:0]
#define TPC_DATA_MAX_MSK                   (0xFFF<<0)

//TPC_RES_SEL
#define	TPC_RES_SEL_XR_OFFSET				0
#define	TPC_RES_SEL_XR_MSK					(0x3<<TPC_RES_SEL_XR_OFFSET)
#define	TPC_RES_SEL_XL_OFFSET				2
#define	TPC_RES_SEL_XL_MSK					(0x3<<TPC_RES_SEL_XL_OFFSET)
#define	TPC_RES_SEL_YD_OFFSET				4
#define	TPC_RES_SEL_YD_MSK					(0x3<<TPC_RES_SEL_YD_OFFSET)
#define	TPC_RES_SEL_YU_OFFSET				6
#define	TPC_RES_SEL_YU_MSK					(0x3<<TPC_RES_SEL_YU_OFFSET)

#define	TPC_RES_SEL_MSK_ALL					(0xFF)

#define	TPC_RES_SEL_MSK						(0x03)


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


