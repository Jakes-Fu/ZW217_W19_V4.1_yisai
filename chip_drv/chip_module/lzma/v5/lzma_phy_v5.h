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

/* C header file */

#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

/* Body of header file */

/**---------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **---------------------------------------------------------------------------*/
#define LZMA_INT_MAX_SIZE     5
#define LZMA_INT_BUF_LEN_ERR  4
#define LZMA_INT_DST_LEN_ERR  3  	
#define LZMA_INT_DEC_ERR      2 	
#define LZMA_INT_MST_ERR      1 
#define LZMA_INT_FINISH       0 

/**---------------------------------------------------------------------------*
 **                         Protocol Define                                    *
 **---------------------------------------------------------------------------*/
typedef void (*LZMA_INT_CALLBACK)();
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function  Declare                                 *
 **---------------------------------------------------------------------------*/
PUBLIC void LZMA_Init(void);
PUBLIC void LZMA_StartRun( );

PUBLIC void LZMA_SetSrcCfg(uint32 src_start_addr, uint32 src_len);
PUBLIC void LZMA_SetDstCfg(uint32 dst_start_addr, uint32 dst_len);
PUBLIC void LZMA_SetBufCfg(uint32 buf_start_addr, uint32 buf_len);
PUBLIC void LZMA_SetBufStart(uint32 buf_start_len);

PUBLIC uint32 LZMA_GetUnzipSize();
PUBLIC uint32 LZMA_GetTransferredLen();

PUBLIC void LZMA_IsrHandler ( uint32 data );
PUBLIC void LZMA_RegCallback( uint32 chan_id, LZMA_INT_CALLBACK callback );
PUBLIC void LZMA_UNRegCallback( uint32 chan_id, LZMA_INT_CALLBACK callback);





#ifdef __cplusplus  /* Insert end of extern C construct */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */

#endif //_LZMA_DRV_H_