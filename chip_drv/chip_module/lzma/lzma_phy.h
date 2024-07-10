/******************************************************************************
 ** File Name:      lzma_phy_v5.h                                           *
 ** Author:         lichd                                                   *
 ** DATE:           06/01/2011                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    define trace interface just for testing usage             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/01/2011     lichd            Create                                    *
 ******************************************************************************/

#ifndef _LZMA_DRV_H_
#define _LZMA_DRV_H_

#include "isr_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **---------------------------------------------------------------------------*/
/*
  Define the interrupt number in LZMA.
*/
#define LZMA_INT_MAX_SIZE     5
#define LZMA_INT_BUF_LEN_ERR  4
#define LZMA_INT_DST_LEN_ERR  3  	
#define LZMA_INT_DEC_ERR      2 	
#define LZMA_INT_MST_ERR      1 
#define LZMA_INT_FINISH       0 

#define LZMA_EN_INT(x)     LZMA_REG_OR(LZMA_INT, ((1<<LZMA_INTEN_BITPOS)<<x))
#define LZMA_DIS_INT(x)    LZMA_REG_AND(LZMA_INT, ~((1<<LZMA_INTEN_BITPOS)<<x))
#define LZMA_CLR_INTSTS(x) LZMA_REG_OR(LZMA_INT,((1<<LZMA_INTCLR_BITPOS)<<x)
#define LZMA_GET_INTSTS(x) (LZMA_REG_GET(LZMA_INT) & ((1<<LZMA_INTSTS_BITPOS)<<x)) 

/**---------------------------------------------------------------------------*
 **                         Protocol Define                                    *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function  Declare                                 *
 **---------------------------------------------------------------------------*/
PUBLIC void _LZMA_BufferEn(uint32 is_en);
PUBLIC void _LZMA_BufferFull(uint32 is_en);

PUBLIC void LZMA_ClrAllIntsts( void );
PUBLIC void LZMA_IntEn(uint32 chn_id , uint32 is_en);

PUBLIC void LZMA_Init(void);
PUBLIC void LZMA_StartRun(void);

PUBLIC void LZMA_SetSrcCfg(uint32 src_start_addr, uint32 src_len);
PUBLIC void LZMA_SetDstCfg(uint32 dst_start_addr, uint32 dst_len);
PUBLIC void LZMA_SetBufCfg(uint32 buf_start_addr, uint32 buf_len);
PUBLIC void LZMA_SetBufStart(uint32 buf_start_len);

PUBLIC uint32 LZMA_WaitingFinish(uint32 timeout);
PUBLIC uint32 LZMA_GetUnzipSize(void);
PUBLIC uint32 LZMA_GetTransferredLen(void);

PUBLIC ISR_EXE_T LZMA_IsrHandler ( uint32 data );
PUBLIC void LZMA_RegCallback( uint32 chan_id, LZMA_CallBack callback );
PUBLIC void LZMA_UNRegCallback( uint32 chan_id, LZMA_CallBack callback);

PUBLIC uint32 LZMA_GetUnZipRawStatus(void);
PUBLIC uint32 LZMA_ClrUnzipRawStatus(void);

#if defined(CHIP_DRV_SC6531) || defined(CHIP_DRV_SC6531EFM) || defined(CHIP_DRV_UIX8910)
PUBLIC ISR_EXE_T LZMA_IsrHandler1st (uint32 data );
PUBLIC ISR_EXE_T LZMA_IsrHandler2nd (uint32 data );
PUBLIC void LZMA_RegCallbackExt(uint32 id, uint32 chan_id, LZMA_CallBack callback );
PUBLIC void LZMA_UNRegCallbackExt(uint32 id, uint32 chan_id, LZMA_CallBack callback );
PUBLIC void LZMA_ResetExtAll(uint32 cycles);
PUBLIC void LZMA_ResetExt(uint32 id, uint32 cycles);
PUBLIC void LZMA_EnableExtAll(uint32 is_en);
PUBLIC void LZMA_EnableExt(uint32 id, uint32 is_en);
PUBLIC void LZMA_BufferEnExt(uint32 id, uint32 is_en);
PUBLIC void LZMA_BufferFullExt(uint32 id, uint32 is_en);
PUBLIC void LZMA_ClrAllIntstsExt(uint32 id);
PUBLIC void LZMA_IntEnExt(uint32 id, uint32 chn_id , uint32 is_en);
PUBLIC void LZMA_InitExt(uint32 id);
PUBLIC void LZMA_InitExtAll(void);
PUBLIC void LZMA_StartRunExt(uint32 id);
PUBLIC void LZMA_SetSrcCfgExt(uint32 id, uint32 src_start_addr, uint32 src_len);
PUBLIC void LZMA_SetDstCfgExt(uint32 id, uint32 dst_start_addr, uint32 dst_len);
PUBLIC void LZMA_SetBufCfgExt(uint32 id, uint32 buf_start_addr, uint32 buf_len);
PUBLIC void LZMA_SetBufStartExt(uint32 id, uint32 buf_start_len);
PUBLIC uint32 LZMA_WaitingFinishExt(uint32 id, uint32 timeout);
PUBLIC uint32 LZMA_GetUnzipSizeExt(uint32 id);
PUBLIC uint32 LZMA_GetTransferredLenExt(uint32 id);
PUBLIC uint32 LZMA_GetUnzipStatusExt(uint32 id);

#endif

#endif //_LZMA_DRV_H_
