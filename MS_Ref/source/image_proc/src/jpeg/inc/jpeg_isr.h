/******************************************************************************
 ** File Name:       jpeg_isr.h                                           	  							*
 ** Author:          Shan.He		                                          						*
 ** DATE:            02/05/2009                                               							*
 ** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         				*
 ** Description:                                                              								*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       								*
 ** -------------------------------------------------------------------------*
 ** DATE              NAME             DESCRIPTION                            						*
 ******************************************************************************/
#ifndef _JPEG_ISR_H_
#define _JPEG_ISR_H_

#if (defined(JPG_DEC_SUPPORT_JPG_HW) || defined(JPG_ENC_SUPPORT_JPG_HW))
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "jpeg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

void JPEG_SetStreamBuffer(uint32 buffer_id);
void JPEG_SetYUVBuffer(uint32 buffer_id);
uint32 JPEG_GetStreamBuffer(void);
uint32 JPEG_GetYUVBuffer(void);
void JPEG_SetStreamBufFirstPingpongFlag(BOOLEAN is_first_pingpong);
void JPEG_SetYUVBufFirstPingpongFlag(BOOLEAN is_first_pingpong);
BOOLEAN JPEG_GetStreamBufFirstPingpongFlag(void);
BOOLEAN JPEG_GetYUVBufFirstPingpongFlag(void);

//used by decoding, must be set to TRUE before the starting of the last slice is start
void JPEG_SetDecLastSliceFlag(BOOLEAN is_last_slice);
//used by decoding, must be set to TRUE before the starting of the last slice is start
void JPEG_SetEncLastSliceFlag(BOOLEAN is_last_slice);

void JPEG_SetMiniState(JPEG_STATE_E mini_state);
void JPEG_SetCurrentState(uint32 cur_state);
void JPEG_ResetHardware(void);
////must be set before enc/dec start, 0: normal mode, 1: switch mode
void JPEG_SetMode(uint32 mode);
void JPEG_SetErrorCount(uint32 count);
uint32 JPEG_GetErrorCount(void);
void JPEG_SetEncodeDone(BOOLEAN is_done);

PUBLIC void JPEG_ClearAllHandler(void);
#ifdef __cplusplus
	}
#endif

#endif

#endif
