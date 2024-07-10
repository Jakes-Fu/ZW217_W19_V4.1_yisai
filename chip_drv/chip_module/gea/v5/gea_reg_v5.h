/******************************************************************************
 ** File Name:    gea_reg_v5.h                                                *
 ** Author:       .	                                                          *
 ** DATE:         06/15/2007                                                  *
 ** Copyright:    2007 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 10/15/2010    lichd           modified for sc6610/6530                    *
 ******************************************************************************/
#ifndef _GEA_REG_V5_H_
#define _GEA_REG_V5_H_
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
//#define GEA_CTL_BEGIN                        0x8D101000(NEMOG)
/*----------GEA Interface Control Registers-----------------------------------*/
#define GEA_CTRL                		(GEA_CTL_BEGIN + 0x0000)
#define GEA_COUNT                       (GEA_CTL_BEGIN + 0x0008)
#define GEA_FRESH              			(GEA_CTL_BEGIN + 0x000C)//GEA1,2:message,GEA3:direction
#define GEA_KC0                 		(GEA_CTL_BEGIN + 0x0010)
#define GEA_KC1                 		(GEA_CTL_BEGIN + 0x0014)
#define GEA_KC2                 		(GEA_CTL_BEGIN + 0x0018)
#define GEA_KC3                 		(GEA_CTL_BEGIN + 0x001C)
#define GEA_POLL_REG                    (GEA_CTL_BEGIN + 0x0020)
#define GEA_DMA_REG                		(GEA_CTL_BEGIN + 0x0024)
#define KASUMI_OUTA						(GEA_CTL_BEGIN + 0x0028)
#define KASUMI_OUTB			 			(GEA_CTL_BEGIN + 0x002C)
#define GEA12_OUT			 			(GEA_CTL_BEGIN + 0x0034)

#define DMA_MEM_BASE                    (GEA_CTL_BEGIN + 0x0040)
#define GEA_DMA_MEM1                    (DMA_MEM_BASE + 0x0000)
#define GEA_DMA_MEM2                    (DMA_MEM_BASE + 0x0004)
#define GEA_DMA_MEM3                    (DMA_MEM_BASE + 0x0008)
#define GEA_DMA_MEM4                    (DMA_MEM_BASE + 0x000C)
// 4word
#define GEA_DMA_FIFO_LEN                (16)

#define MODE_KSMI           0
#define MODE_GEA1           5
#define MODE_GEA2           6
#define MODE_GEA3           3
#define GEA_RESET           BIT_31
#define GEA_NEXT            BIT_5
#define GEA1_DIR_BIT        15
#define GEA2_DIR_BIT        15
#define GEA3_DIR_BIT        0

#define GEA_NOT_COMPLETE    0
#define GEA_COMPLETE        1

/**----------------------------------------------------------------------------*
**       description for GEA_DMA_REG register                                 **
**----------------------------------------------------------------------------*/
#define GEA_DMA_MODE_EN           BIT_13
#define GEA_DMA_RUNNUMBER_MASK    ( 0x1FF8 )
#define GEA_DMA_RUNNUMBER_SHIFT   ( 3 )

// DMA[16:14] and NoneDma[2:0] data switch mode
#define GEA_DMA_SWITCH_SHIFT      (0)
#define GEA_DMA_SWITCH_MASK       (0x07)
#define GEA_NORMA_SWITCH_SHIFT    (14)
#define GEA_NORMAL_SWITCH_MASK    (0x07<<GEA_NORMA_SWITCH_SHIFT)

#define GEA_SWITCH_ABCDEFGH_ABCDEFGH   (0)
#define GEA_SWITCH_ABCDEFGH_DCBAHGFE   (1)
#define GEA_SWITCH_ABCDEFGH_CDABGHEF   (2)

#define GEA_SWITCH_ABCDEFGH_EFGHABCD   (4)
#define GEA_SWITCH_ABCDEFGH_HGFEDCBA   (5)
#define GEA_SWITCH_ABCDEFGH_GHEFCDAB   (6)

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef struct GEA_REG_CFG_TAG  //8d00_0000
{
    volatile uint32 ctrl;       //- 0
    volatile uint32 reserved1;  //- 4
    volatile uint32 count;      //- 8
    volatile uint32 fresh;      //- c
    
    volatile uint32 kc0;        //-10
    volatile uint32 kc1;        //-14
    volatile uint32 kc2;        //-18
    volatile uint32 kc3;        //-1c
    
    volatile uint32 poll;       //-20
    volatile uint32 dma_reg;    //-24
    volatile uint32 kasmi_out1; //-28
    volatile uint32 kasmi_out2; //-2c
    volatile uint32 reserve2;   //-30
    volatile uint32 gea_out;    //-34; output of gea1&gea2
}
GEA_REG_CFG_T;

#define GEA_REQ_ID          DMA_GEA
#define GEA_DMA_REQ_ADDR (DMA_CTL_REG_BASE + 0x2000 + (GEA_REQ_ID-1)*0x04)
#define GEA_DMA_CONNECT(chn) CHIP_REG_SET(GEA_DMA_REQ_ADDR, chn)

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
