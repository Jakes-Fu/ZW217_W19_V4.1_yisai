/******************************************************************************
 ** File Name:    adi_reg_v7.h                                                *
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

#ifndef _ADI_REG_V7_H_
#define _ADI_REG_V7_H_
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
#define  ADI_ARM_STS            (ADI_ARM_BASE_ADDR + 0x4 )
#define  ADI_ARM_RD_CMD         (ADI_ARM_BASE_ADDR + 0x8 )
#define  ADI_ARM_RD_DATA        (ADI_ARM_BASE_ADDR + 0xc)
#define  ADI_GSSI_CTRL0         (ADI_ARM_BASE_ADDR + 0x10)
#define  ADI_GSSI_CTRL1         (ADI_ARM_BASE_ADDR + 0x14)

//ADI_CTL_REG
#define ADI_CTL_ADDR_SEL_MASK   (BIT_1|BIT_0)
#define ADI_CTL_WR_LEVEL        BIT_2
#define ADI_ARM_FIFO_OVF_INT    BIT_3

#define ARM_WR_PRI              4
#define ARM_RD_PRI              6
#define DSP_WR_PRI              8  
#define DSP_RD_PRI              10 

//ADI_STS
#define ADI_ARM_FIFO_OVF_INT_CLR        BIT_0   //write only
#define ADI_ARM_FIFO_OVF_INT_RAW_STS    BIT_1   //read oly  
#define ADI_ARM_FIFO_OVF_INT_MASK_STS   BIT_2   //read only
#define ADI_ARM_WR_REQ_STS              BIT_3
#define ADI_ARM_RD_REQ_STS              BIT_4
#define ADI_DSP_WR_REQ_STS              BIT_5
#define ADI_DSP_RD_REQ_STS              BIT_6

#define ADI_STS_FSM_STATE_MASK   (BIT_12|BIT_11|BIT_10)
#define ADI_STS_FIFO_WPTR_MASK   (BIT_18|BIT_17)
#define ADI_STS_FIFO_RPTR_MASK   (BIT_20|BIT_19)
#define ADI_STS_FIFO_EMPTY_MASK  BIT_21
#define ADI_STS_FIFO_FULL_MASK   BIT_22

//ADI_ARM_RD_DATA 
#define ADI_RD_CMD_BUSY_MASK    BIT_31
#define ADI_ARM_RD_ADDR_MASK    (0x7FFF0000)  	//[30:16],register address [14:0]
#define ADI_ARM_RD_DATA_MASK    (0x0000FFFF)  	//[15:00],read value from analog die

//ADI_GSSI_CTRL0
#define ADI_GSSI_FRAME_LEN_MASK (0x0000003F)    //[5:0]
#define ADI_GSSI_CMD_LEN_MASK   (0x000007C0)    //[10:6]
#define ADI_GSSI_DATA_LEN_MASK  (0x0000F800)    //[15:11]
#define ADI_GSSI_WR_POL         BIT_21          //write/read flag
#define ADI_GSSI_SYNC_SEL       BIT_22
#define ADI_GSSI_SYNC_MODE      BIT_23
#define ADI_GSSI_SYNC_MCU       BIT_24
#define ADI_GSSI_SCK_REV        BIT_25

#define ADI_GSSI_IE_DELAY_CFG   BIT_27
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define ADI_GSSI_FAST_MODE     BIT_28
#define ADI_GSSI_DUMMY_CLK_EN     BIT_29
#endif
#define ADI_GSSI_SCK_ALL_ON     BIT_30
#define ADI_GSSI_WR_DISABLE     BIT_31

//ADI_GSSI_CTRL1
#define ADI_GSSI_STRTBIT_MODE   BIT_31
#define ADI_GSSI_CLK_DIV_MASK   (0x3FC)

#define ANA_REG_ADDR_START      (ANA_REG_BASE)
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


