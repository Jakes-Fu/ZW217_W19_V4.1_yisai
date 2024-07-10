/******************************************************************************
 ** File Name:    adi_reg_v5.h                                                *
 ** Author:       Tim.Luo                                                     *
 ** DATE:         03/03/2010                                                  *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 03/03/2010    Tim.Luo         Create.                                     *
 ******************************************************************************/

#ifndef _ADI_REG_V5_H_
#define _ADI_REG_V5_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/

#define  ADI_ARM_BASE_ADDR       ADI_BASE
#define  ADI_CTL_REG            (ADI_ARM_BASE_ADDR + 0x0 )
#define  ADI_CHANNEL_PRI        (ADI_ARM_BASE_ADDR + 0x4 )
#define  ADI_INT_EN             (ADI_ARM_BASE_ADDR + 0x8 )
#define  ADI_INT_RAW_STS        (ADI_ARM_BASE_ADDR + 0xc)
#define  ADI_INT_MASK_STS       (ADI_ARM_BASE_ADDR + 0x10)
#define  ADI_INT_CLR            (ADI_ARM_BASE_ADDR + 0x14)

#define  ADI_ARM_RD_CMD         (ADI_ARM_BASE_ADDR + 0x18)  //[27:00]
#define  ADI_ARM_RD_DATA        (ADI_ARM_BASE_ADDR + 0x1C)  //[15:00]
#define  ADI_ARM_STS            (ADI_ARM_BASE_ADDR + 0x20)

//ADI_CTL_REG
#define ADI_CTL_FRM_LEN_MASK    (0x000007E0)     //[10:5]
#define ADI_CTL_SCK_EN          BIT_4            //[4]
#define ADI_CTL_CLK_DIV_MASK    (0x000F)         //[3:0]

//ADI interrupt bitpos
#define  ADI_ARM_FIFO_OVF_INT   BIT_2
#define  ADI_ARM_FIFO_EMPTY_INT BIT_1
#define  ADI_ARM_FIFO_FULL_INT  BIT_0

//ADI_ARM_RD_DATA 
#define ADI_RD_CMD_BUSY_MASK    BIT_31
#define ADI_ARM_RD_ADDR_MASK    (0x1FFF0000)  	//[28:16],register address [12:0]
#define ADI_ARM_RD_DATA_MASK    (0x0000FFFF)  	//[15:00],read value from analog die

//ADI_ARM_STS
#define ADI_STS_FIFO_FULL_MASK   BIT_9
#define ADI_STS_FIFO_EMPTY_MASK  BIT_8
#define ADI_STS_FIFO_WPTR_MASK   (BIT_7|BIT_6)
#define ADI_STS_FIFO_RPTR_MASK   (BIT_5|BIT_4
#define ADI_STS_FSM_STATE_MASK   (BIT_2|BIT_1|BIT_0)

//ADI_CHANNEL_PRI bit define
#define    ARM_WR_PRI           0
#define    ARM_RD_PRI           2
#define    DSP_WR_PRI           4
#define    DSP_RD_PRI           6
#define    PD_WR_PRI            8
#define    BURST_ON_PRI         10
#define    FRM_INT_PRI          12
#define    RF_WR_PRI            14
#define    FM_AGC_PRI           16
#define    FM_BAND_PRI           18

#define ANA_REG_ADDR_START      (ANA_REG_BASE        )
#define ANA_REG_ADDR_END        (ANA_REG_BASE + 0xFFF)

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

#endif  //_ADI_REG_V3_H_


