#ifndef _JPGINIT_H
#define _JPGINIT_H

#include "sci_types.h"

/************************************************************************/
/* Alloc the MCU buffer                                                 */
/************************************************************************/
void AllocMCUBuf();

/************************************************************************/
/* Initiate the Encoder                                                 */
/************************************************************************/
uint32 JPEG_EncInit(  uint16 width, 				//image width
					  uint16 height,				//image height
					  uint16 compress_level,		//compress level
					  uint8 * out_stream_ptr		//output jpg stream
					);
/************************************************************************/
/* Initiate the Decoder                                                 */
/************************************************************************/
uint32 JPEG_DecInit(  uint8 * input_stream_ptr,				//
					  uint32 len					//
					);


#endif