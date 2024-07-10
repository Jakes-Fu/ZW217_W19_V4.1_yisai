/******************************************************************************
 ** File Name:    lzma_reg_v5.h                                                *
 ** Author:       changde                                                     *
 ** DATE:         09/18/2011                                                  *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 09/18/2011    changde         Create.                                     *
 ******************************************************************************/

#ifndef _SC6530_REG_LZMA_H_
#define _SC6530_REG_LZMA_H_


#include "sci_types.h"
#include "arm_reg.h"

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
#define  LZMA_CTRL    	    	(LZMA_REG_BASE + 0x00 )
#define  LZMA_CTRL_STS          (LZMA_REG_BASE + 0x04 )
#define  LZMA_INT        	    (LZMA_REG_BASE + 0x08 )
#define  LZMA_SRC_SADDR    	    (LZMA_REG_BASE + 0x0c )

#define  LZMA_DST_SADDR    	    (LZMA_REG_BASE + 0x10 )
#define  LZMA_SRC_LEN      	    (LZMA_REG_BASE + 0x14 )
#define  LZMA_DST_LEN           (LZMA_REG_BASE + 0x18 )  
#define  LZMA_BUFFER_SADDR      (LZMA_REG_BASE + 0x1C ) 

#define  LZMA_BUFFER_LEN        (LZMA_REG_BASE + 0x20 )  
#define  LZMA_BUFFER_START_LEN  (LZMA_REG_BASE + 0x24 )
#define  LZMA_PROC_POS          (LZMA_REG_BASE + 0x28 )
#define  LZMA_TRANS_LEN         (LZMA_REG_BASE + 0x2C )
#define  LZMA_UNZIP_SIZE        (LZMA_REG_BASE + 0x30 )

// LZMA_CTRL bit field
#define LZMA_CTRL_BUFFER_FULL_BIT     BIT_3     //
#define LZMA_CTRL_BUFFER_EN_BIT       BIT_2     //
#define LZMA_CTRL_TRANSFER_BIT        BIT_1 
#define LZMA_CTRL_START_BIT           BIT_0 

// LZMA_CTRL_STS bit field
#define LZMA_CTRL_STS_BUFFER_FULL_BIT BIT_3     //
#define LZMA_CTRL_STS_BUFFER_EN_BIT   BIT_2     //
#define LZMA_CTRL_STS_TRANSFER_BIT    BIT_1 
#define LZMA_CTRL_STS_START_BIT       BIT_0

//LZMA_INT bit field
#define LZMA_INTRAW_BITPOS           (24)
#define LZMA_INTSTS_BITPOS           (16)
#define LZMA_INTCLR_BITPOS           (8)
#define LZMA_INTEN_BITPOS            (0)

#define LZMA_INTRAW_BUF_LEN_ERR_BIT  BIT_28
#define LZMA_INTRAW_DST_LEN_ERR_BIT  BIT_27  	
#define LZMA_INTRAW_DEC_ERR_BIT      BIT_26 	
#define LZMA_INTRAW_MST_ERR_BIT      BIT_25 
#define LZMA_INTRAW_FINISH_BIT       BIT_24

#define LZMA_INTSTS_BUF_LEN_ERR_BIT  BIT_20
#define LZMA_INTSTS_DST_LEN_ERR_BIT  BIT_19  	
#define LZMA_INTSTS_DEC_ERR_BIT      BIT_18 	
#define LZMA_INTSTS_MST_ERR_BIT      BIT_17 
#define LZMA_INTSTS_FINISH_BIT       BIT_16 

#define LZMA_INTCLR_BUF_LEN_ERR_BIT  BIT_12
#define LZMA_INTCLR_DST_LEN_ERR_BIT  BIT_11  	
#define LZMA_INTCLR_DEC_ERR_BIT      BIT_10 	
#define LZMA_INTCLR_MST_ERR_BIT      BIT_9 
#define LZMA_INTCLR_FINISH_BIT       BIT_8 

#define LZMA_INTEN_BUF_LEN_ERR_BIT   BIT_4
#define LZMA_INTEN_DST_LEN_ERR_BIT   BIT_3  	
#define LZMA_INTEN_DEC_ERR_BIT       BIT_2 	
#define LZMA_INTEN_MST_ERR_BIT       BIT_1 
#define LZMA_INTEN_FINISH_BIT        BIT_0 

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

#endif  //_SC6530_REG_LZMA_H_


