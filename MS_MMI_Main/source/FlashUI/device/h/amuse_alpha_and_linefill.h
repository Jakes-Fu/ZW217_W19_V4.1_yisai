#ifndef _AMUSE_ALPHA_AND_LINEFILL_H_ /*perl robot add*/
#define _AMUSE_ALPHA_AND_LINEFILL_H_ /*perl robot add*/
/*
* amuse_alpha_and_linefill.h
* Author: Microrapid
* 
* This file contains interfaces of hardware alpha-blending and line filling
*/

#define BPP 4

#if (BPP==4) /* 32bits */
typedef unsigned int WRT_TYPE;
#endif
#if (BPP==2) /* 16bits */
typedef unsigned short WRT_TYPE;
#endif

/*
*  amuse_fill_direct_line_hw: alpha blending by device.
*
* Parameters: 
* d: the dest pixel
* s: the src pixel
* a: the alpha value
* 
* Returns: the mixed pixel.
*/
WRT_TYPE amuse_mix_alpha_hw(WRT_TYPE d, WRT_TYPE s, WRT_TYPE a);

/*
*  amuse_fill_direct_line_hw: fill line by device.
*
* Parameters: 
* start: the line buffer to be filled
* length: the number of pixels of the line
* color: the line color
* 
* Returns: success: 0; failure: -1
*/
int amuse_fill_direct_line_hw(WRT_TYPE * start, int length, WRT_TYPE color);

#endif  /*perl robot add*/

