#ifndef _IMG_DEC_HAL_H
#define _IMG_DEC_HAL_H

#include "img_dec_interface.h"
#include "sfs.h"
#include "img_dec_hal.h"

typedef enum
{
	
	IMG_DEC_EXIT_NORMAL = 0,
	IMG_DEC_EXIT_HALT
	

}IMG_DEC_EXIT_TYPE;

typedef struct img_dec_frame_extra_param_tag
{
	uint32 priority;

}IMG_DEC_FRAME_EXTRA_T;

typedef struct img_dec_hal_src_tag
{
	uint8 			*src_ptr;  
	uint32 			src_file_size; 
	uint32			src_file_handle;
	
	void 			*(*img_alloc)(uint32 size);
	void 			(*img_free)(void *);
	BOOLEAN              src_change;
}IMG_DEC_HAL_SRC_T;

typedef struct img_dec_fun_tab_tag
{

	uint32  (*create_decoder)(IMG_DEC_HAL_SRC_T *src_param_ptr);
       IMG_DEC_RET_E (*getframe)(IMG_DEC_FRAME_IN_PARAM_T *in_param_ptr, IMG_DEC_FRAME_OUT_PARAM_T *out_param_ptr, IMG_DEC_FRAME_EXTRA_T *extra_info_ptr);	
	IMG_DEC_RET_E (*destroy_decoder)(uint32 handle, IMG_DEC_EXIT_TYPE exit_type);	

}IMG_DEC_FUN_TAB_T;

#endif
