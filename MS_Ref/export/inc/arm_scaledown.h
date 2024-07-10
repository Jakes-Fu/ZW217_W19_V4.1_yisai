/*************************************************************************
** File Name:      ARM_ScaleDown.h                                       *
** Author:         Reed zhang                                            *
** Date:           06/02/2007                                            *
** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.     *
** Description:                                                          *
**                        Edit History                                   *
** ----------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                           *
** 06/02/2007     Reed zhang       Create.                               *
**************************************************************************/
#include "sci_types.h"

#define RGB_FORMAT565 0
#define RGB_FORMAT666 1
#define RGB_FORMAT888 2
#define RGB_FORMAT555 3

#define MODE565R  0xF800
#define MODE565G  0x07E0
#define MODE565B  0x001F

#define MODE666R  0xFF0000
#define MODE666G  0x00FF00
#define MODE666B  0x0000FF

#define MODE888R  0xFF0000
#define MODE888G  0x00FF00
#define MODE888B  0x0000FF

#define MODE555R  0x7C00
#define MODE555G  0x3E0
#define MODE555B  0x1F


typedef struct scaling_coeff_list
{
	uint16  start_pos_x;     // the start postion x in the image
	uint16  start_pos_y;     // the start postion y in the image
	uint16  before_scale_w;  // the image size which is needed to be scaled
	uint16  before_scale_h;
	uint8   rgb_format;      // 0: 565,  1: 666,  2:888,  3: 555
	uint16  input_orig_w;    // the original image size
	uint16  input_orig_h;     
	uint16  after_scale_w;   // the image size after scaling
	uint16  after_scale_h;
} SCALING_COEFF_LIST_T;

void ScalingDown(uint16              *in_rgb_data_ptr,     // intput data
				 uint16              *out_rgb_data_ptr,    // output data
				 SCALING_COEFF_LIST_T scaling_coef_stru,   // scaling parameter
				 uint16               scaling_mode);       // scaling_mode: 1, 6/7,,,  scaling_mode:2, 7/10
				 
				 
				 
				 


typedef struct asm_scale_coeff_list
{
	uint32  mode_r;     
	uint32  mode_g;     
	uint32  mode_b; 
	uint16  h;
	uint16  w;     
	uint16  orig_w;
	uint16  sign_bit;    
	uint16  after_scale_w;
	uint16  rest_w;
	uint16  cur_pos;
} ASM_SCALE_COEFF_LIST_T;

void ScalingDown(uint16              *in_rgb_data_ptr,
				 uint16              *out_rgb_data_ptr,
				 SCALING_COEFF_LIST_T scaling_coef_stru,
				 uint16               scaling_mode); // scaling_mode: 1, 6/7,,,  scaling_mode:2, 7/10
				 