#ifndef BMP_DITHERING_H
#define BMP_DITHERING_H

#ifdef __cplusplus
    extern   "C"
    {
#endif


//#include "data_type.h"
#include "sci_types.h"

static uint8 BayerMatrix[4*4]={0,4,0,5,6,2,7,3,1,5,1,4,8,3,6,2};
static uint8 BayerMatrix2[2*2]={0,2,3,1};

typedef enum
{
	DITHER_OK = 0,
	DITHER_IMG_SIZE_ERR,
	DITHER_SUB_BLOCK_SIZE_ERR,
	DITHER_SUB_BLOCK_POS_ERR
}DITHER_RETURN_E;

uint8 dith8to6(uint8 input,   //input 8bit data
			   uint8 thr);     //threshold value

uint8 dith8to5(uint8 input,   //input 8bit data
			   uint8 thr);     //threshold value

uint8 matrix_copy(uint8 *srcMatrix,uint8 *dscMatrix);
uint8 matrix_row_exchange(uint8 *matrix);  //1->4,2->1,3->2,4->3
uint8 matrix_col_exchange(uint8 *matrix);  //1->4,2->1,3->2,4->3
uint8 matrix_reverse(uint8 *matrix);


DITHER_RETURN_E IMG_Dithering(uint8  *psrc,	// input RGB888 data
							  uint8  *pdst,	    // output RGB565 data
							  uint16 sub_block_start_x,  // subBlock start row position
							  uint16 sub_block_start_y,  // subBlock start col position
							  uint16 sub_block_width,   // subBlock width
							  uint16 sub_block_height,  // subBlock height
							  uint16 img_width,		// image data width
							  uint16 img_height		// image data height
			                 );
			                 
			                 
#ifdef __cplusplus
    }
#endif
			                 
		
#endif  //BMP_DITHERING_H

