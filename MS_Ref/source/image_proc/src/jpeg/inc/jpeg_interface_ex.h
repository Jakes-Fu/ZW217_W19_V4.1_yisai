#ifndef _JPEG_INTERFACE_EX_H_
#define _JPEG_INTERFACE_EX_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "jpeg_interface.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/****************************************************************************/
/* Purpose:	Encode YUV image data to a JPEG file					 		*/
/* Author:																	*/
/* Input:      																*/
/*			in_param_ptr	--points to in parameter						*/	
/*			out_param_ptr   --points to out parameter						*/
/* Output:	none															*/
/* Return:	operation results											    */
/* Note:    																*/
/****************************************************************************/
PUBLIC JINF_RET_E IMGJPEG_EncodeEx(JINF_ENC_IN_PARAM_T *in_param_ptr,
								    JINF_ENC_OUT_PARAM_T *out_param_ptr);


/****************************************************************************/
/* Purpose:	Decode JPEG file to YUV or RGB image data				 		*/
/* then encode into jpeg with small size and small resolution.				*/		
/* Author:																	*/
/* Input:      																*/
/*			in_param_ptr	--points to in parameter						*/	
/*			out_param_ptr   --points to out parameter						*/
/* Output:	none															*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/
PUBLIC JINF_RET_E IMGJPEG_DecodeEx(
											JINF_DEC_IN_PARAM_T  *in_param_ptr, 
											JINF_DEC_OUT_PARAM_T *out_param_ptr);

/****************************************************************************/
/* Purpose:	create a miniature for fast view, that is, decode jpeg with big size                  */
/* then encode into jpeg with small size and small resolution.				*/		
/* Author:																	*/
/* Input:      																*/
/*			in_param_ptr	--points to in parameter					    */	
/*			out_param_ptr   --points to out parameter						*/
/* Output:	none															*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/
PUBLIC  JINF_RET_E IMGJPEG_CreateMiniatureEx(
									JINF_MINI_IN_PARAM_T *in_param_ptr,
									JINF_MINI_OUT_PARAM_T *out_param_ptr);

/****************************************************************************/
/* Purpose:	Decode JPEG resource file to RGB565 data			 			*/
/* Author:																	*/
/* Input:      																*/
/*			in_param_ptr	--points to in parameter						*/	
/*			out_param_ptr   --points to out parameter						*/
/* Output:	none															*/
/* Return:	operation results												*/
/* Note:    																*/
/****************************************************************************/
PUBLIC JINF_RET_E IMGJPEG_DecodeResEx(
									JINF_DEC_RES_IN_PARAM_T  *in_param_ptr, 
									JINF_DEC_RES_OUT_PARAM_T *out_param_ptr);

/****************************************************************************/
/* Purpose:	call the FreeLock and freeLock before and after  the decode/encode             */
/*			/createMinature function task being killed	     				*/
/* Author:																	*/
/* Input:      																*/
/* Output:																	*/
/* Return:																	*/
/* Note:    																*/
/****************************************************************************/
PUBLIC void IMGJPEG_FreeLock(void);

/****************************************************************************/
/* Purpose:	call the FreeLock and freeLock before and after  the decode/encode             */
/*			/createMinature function task being killed	     				*/
/* Author:																	*/
/* Input:      																*/
/* Output:																	*/
/* Return:																	*/
/* Note:    																*/
/****************************************************************************/
PUBLIC void IMGJPEG_FreeUnlock(void);
									  
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif // _JPEG_INTERFACE_H_
