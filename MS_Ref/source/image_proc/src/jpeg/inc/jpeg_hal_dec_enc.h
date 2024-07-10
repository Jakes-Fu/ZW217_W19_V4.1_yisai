/******************************************************************************
 ** File Name:	jpeg_hal_sc6600l.h                                               	  
 ** Author:		shan.he		                                          
 ** DATE:		13/01/2009                                               
 ** Copyright:	2009 Spreatrum, Incoporated. All Rights Reserved.         
 ** Description:                                                              
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 ** ------------------------------------------------------------------------- 
 ** DATE              NAME             DESCRIPTION       
 ** 13/01/2009        shan.he		   Create
 ******************************************************************************/

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "jpeg.h"
#include "jpeg_hal.h"
#include "jpeg_algorithm.h"
#include "jpeg_task.h"

#define JPEG_CALLER_EXTERNAL		0
#define JPEG_CALLER_INTERNAL		1

#if (defined(JPG_DEC_SUPPORT_JPG_HW) || defined(JPG_ENC_SUPPORT_JPG_HW))
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
// Purpose:	initialize 6600l hardware	
// Author:	shan.he
 // Input:        None		
// Output:  	None	
// Return:  	
// Note:    
******************************************************************************/
PUBLIC BOOLEAN JPEG_InitializeHardware(void);

/******************************************************************************
// Purpose:	release 6600l hardware	
// Author:	shan.he
 // Input:        None		
// Output:  	None	
// Return:  	
// Note:    
******************************************************************************/
PUBLIC void JPEG_ReleaseHardware(void);

/******************************************************************************
// Purpose:	encode jpeg file using HW platform,very fast,trust me.			
// Author:	frank.yang
// Input:		context_ptr-jpeg encode context pointer
//				out_param_ptr-output parameter pointer
// Output: 	None	
// Return:  	operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_HalEncode(JPEG_ENC_CONTEXT_T *context_ptr, 
										JINF_ENC_OUT_PARAM_T *out_param_ptr);

/******************************************************************************
// Purpose:	decode jpeg file using HW platform,very fast,trust me.			
// Author:	shan.he
// Input:		context_ptr--jpeg decode context pointer
//				out_param_ptr--output parameter pointer
// Output: 	None	
// Return:  	operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_HalDecode(JPEG_DEC_CONTEXT_T *context_ptr, 
											JINF_DEC_OUT_PARAM_T *out_param_ptr);
											
/******************************************************************************
// Purpose:	decode res jpeg  using HW platform,very fast,trust me.			
// Author:	shan.he
// Input:		context_ptr--jpeg decode context pointer
//				out_param_ptr--output parameter pointer
// Output: 	None	
// Return:  	operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_Decode_Res(JPEG_DEC_CONTEXT_T *context_ptr, 
											JINF_DEC_RES_OUT_PARAM_T *out_param_ptr);

/******************************************************************************
// Purpose:	start encode		
// Author:	shan.he
// Input:		context_ptr--jpeg encode context pointer
// Output: 	none	
// Return:  operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_StartEncode(JPEG_ENC_CONTEXT_T *context_ptr, JPEG_SIGNAL_T *jpeg_sig_ptr);

/******************************************************************************
// Purpose:	stop encode		
// Author:	shan.he
// Input:		context_ptr--jpeg encode context pointer
// Output: 	none	
// Return:  operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_StopEncode(JPEG_ENC_CONTEXT_T *context_ptr, JPEG_SIGNAL_T *jpeg_sig_ptr);

/******************************************************************************
// Purpose:	both yuv ping and pong buffer are empty for the first time, write yuv data.
// Author:	shan.he
// Input:		context_ptr--jpeg encode context pointer
// Output: 	none	
// Return:  operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_EncodeWritePingPongBufferFirst(JPEG_ENC_CONTEXT_T *context_ptr,
																JPEG_SIGNAL_T *jpeg_sig_ptr);

/******************************************************************************
// Purpose:	yuv ping or pong buffer is empty, write yuv data the empty buffer and start the other buffer.
// Author:	shan.he
// Input:		context_ptr--jpeg encode context pointer
// Output: 	none	
// Return:  operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_EncodeWritePingPongBuffer(JPEG_ENC_CONTEXT_T *context_ptr,
															JPEG_SIGNAL_T *jpeg_sig_ptr);

/******************************************************************************
// Purpose:	both stream ping and pong buffer are full for the first time, read the data away.
// Author:	shan.he
// Input:		context_ptr--jpeg encode context pointer
// Output: 	none	
// Return:  operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_EncodeReadPingPongBufferFirst(JPEG_ENC_CONTEXT_T *context_ptr,
																JPEG_SIGNAL_T *jpeg_sig_ptr);

/******************************************************************************
// Purpose:	both stream ping or pong buffer is full, read the data away and start the other buffer.
// Author:	shan.he
// Input:		context_ptr--jpeg encode context pointer
// Output: 	none	
// Return:  operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_EncodeReadPingPongBuffer(JPEG_ENC_CONTEXT_T *context_ptr,
															JPEG_SIGNAL_T *jpeg_sig_ptr);

/******************************************************************************
// Purpose:	start decode		
// Author:	shan.he
// Input:		context_ptr--jpeg decode context pointer
// Output: 	none	
// Return:  operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_StartDecode(JPEG_DEC_CONTEXT_T *dec_cxt_ptr, 
										JPEG_ENC_CONTEXT_T *enc_cxt_ptr, 
										JPEG_SIGNAL_T *jpeg_sig_ptr);

/******************************************************************************
// Purpose:	stop decode		
// Author:	shan.he
// Input:		context_ptr--jpeg decode context pointer
// Output: 	none	
// Return:  operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_StopDecode(JPEG_DEC_CONTEXT_T *context_ptr, JPEG_SIGNAL_T *jpeg_sig_ptr);

/******************************************************************************
// Purpose:	both stream ping and pong buffer are empty for the first time, write stream data.
// Author:	shan.he
// Input:		context_ptr--jpeg decode context pointer
// Output: 	none	
// Return:  operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_DecodeWritePingPongBufferFirst(JPEG_DEC_CONTEXT_T *context_ptr,
																JPEG_SIGNAL_T *jpeg_sig_ptr);

/******************************************************************************
// Purpose:	stream ping or pong buffer is empty, write stream data to the empty buffer and start the other one
// Author:	shan.he
// Input:		context_ptr--jpeg decode context pointer
// Output: 	none	
// Return:  operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_DecodeWritePingPongBuffer(JPEG_DEC_CONTEXT_T *context_ptr,
															JPEG_SIGNAL_T *jpeg_sig_ptr);

/******************************************************************************
// Purpose:	both yuv ping and pong buffer are full for the first time, read the data away.
// Author:	shan.he
// Input:		context_ptr--jpeg decode context pointer
// Output: 	none	
// Return:  operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_DecodeReadPingPongBufferFirst(JPEG_DEC_CONTEXT_T *context_ptr,
																JPEG_SIGNAL_T *jpeg_sig_ptr);

/******************************************************************************
// Purpose:	stream ping or pong buffer is full, read the data away from the empty buffer and start the other one
// Author:	shan.he
// Input:		context_ptr--jpeg decode context pointer
// Output: 	none	
// Return:  operation results
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_DecodeReadPingPongBuffer(JPEG_DEC_CONTEXT_T *context_ptr,
															JPEG_SIGNAL_T *jpeg_sig_ptr);


/****************************************************************************/
/* Purpose:	JPEG hardware decode, software scaling down				 		*/
/* then encode into jpeg with small size and small resolution.				*/		
/* Author:	shan.he															*/
/* Input:      																*/
/*			in_param_ptr	--points to in parameter						*/	
/*			out_param_ptr   --points to out parameter						*/
/*			jpeg_info_ptr	--points to the jpeg basic info                 */
/* Output:	none															*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/
PUBLIC JINF_RET_E JPEG_HardwareDecode(
											JINF_DEC_IN_PARAM_T  *in_param_ptr, 
											JINF_DEC_OUT_PARAM_T *out_param_ptr,
											JPEG_BASIC_INFO_T *jpeg_info_ptr);

/****************************************************************************/
/* Purpose:	create miniature: decode and scaling down jpeg first, then encode. Both       */
/*			decoding and encoding are use hardware							*/	
/* Author:	shan.he															*/
/* Input:      																*/
/*			in_param_ptr	--points to in parameter						*/	
/*			out_param_ptr   --points to out parameter						*/
/* Output:	none															*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/
PUBLIC JINF_RET_E JPEG_CreateMiniature(JINF_MINI_IN_PARAM_T *in_param_ptr,
									JINF_MINI_OUT_PARAM_T *out_param_ptr);

/****************************************************************************/
/* Purpose:	get decode buffer size											*/
/* Author:	shan.he															*/
/* Input:      																*/
/*			in_param_ptr			--pointer of input parameters			*/
/*			out_param_ptr			--pointer of output parameters			*/	
/* Output:	none															*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/
PUBLIC JINF_RET_E JPEG_HalGetDecodeBufSize(JINF_GET_DEC_BUF_SIZE_IN_T *in_param_ptr,
										   JINF_GET_DEC_BUF_SIZE_OUT_T *out_param_ptr); 

/****************************************************************************/
/* Purpose:	reload the stream and redecode the previous slice 			 	*/
/* Author:	shan.he															*/
/* Input:      																*/
/*			context_ptr		--points to context parameter					*/	
/*			jpeg_sig_ptr  --points to the signal parameter					*/
/* Output:	none															*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/
PUBLIC JPEG_RET_E JPEG_DecodeErrorRestart(JPEG_DEC_CONTEXT_T *context_ptr,
												JPEG_SIGNAL_T *jpeg_sig_ptr);

/****************************************************************************/
/* Purpose:	Set jpeg function caller										*/
/* Author:																	*/
/* Input:      																*/
/*			caller_flag: internal or external caller						*/
/* Output:																	*/
/* Return:																	*/
/* Note:    																*/
/****************************************************************************/
PUBLIC void JPEG_SetCaller(uint32 caller_flag);

/******************************************************************************
// Purpose:	release decode resource by
// Author:	shan.he
 // Input:        None		
// Output:  	None	
// Return:  	
// Note:    
******************************************************************************/
PUBLIC void JPEG_FreeScaleRes(uint32 flag);

#endif