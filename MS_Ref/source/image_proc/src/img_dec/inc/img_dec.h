#ifndef _IMG_DEC_H
#define _IMG_DEC_H

#include "img_dec_interface.h"
#include "img_dec_hal.h"
#include "os_api.h"
#include "sfs.h"




#if 0
typedef IMG_DEC_HAL_FRAME_OUT_PARAM_T IMG_DEC_FRAME_OUT_PARAM_HAL_T ;
typedef IMG_DEC_HAL_FRAME_IN_PARAM_T IMG_DEC_FRAME_IN_PARAM_HAL_T ;

#endif


typedef struct _img_decoder_handler_tag
{
	
	
	IMG_DEC_HAL_SRC_T src_param;
	IMG_DEC_INFO_T info_param;
	
	void	 (*app_callback)(uint32 handle, IMG_DEC_RET_E ret, IMG_DEC_FRAME_OUT_PARAM_T *out_param_ptr, uint32 app_param); //if callback == PNULL; mean sync calling, otherwise async calling
	IMG_DEC_RET_E callback_ret;
	IMG_DEC_FRAME_IN_PARAM_T    frame_in_param;
       IMG_DEC_FRAME_OUT_PARAM_T frame_out_param; //Frame Output Parameter

	uint32 app_param;


	void *app_param_ptr;
	uint32 app_param_size;



	uint32 decoder_task_priority;

	//SFS_HANDLE file_handle;
	//uint32 img_handle;  
	//uint32 app_handle;
	
	
}IMG_DEC_HANDLER_T;


//void (*callback)(uint32 handle, IMG_DEC_RET_E ret, IMG_DEC_HAL_FRAME_OUT_PARAM_T *out_param_ptr);

void * IMG_DEC_ALLOC(uint32 size);

void  IMG_DEC_FREE(void * data_ptr);

IMG_DEC_RET_E IMG_DEC_Create_Hal(IMG_DEC_HAL_SRC_T *src_param_ptr,  IMG_DEC_TYPE_E img_type, uint32 *handle_ptr);

PUBLIC IMG_DEC_RET_E IMG_DEC_GetFrame_Hal(IMG_DEC_TYPE_E img_type,
	 											IMG_DEC_FRAME_IN_PARAM_T    *frame_in_param_ptr,
	 											IMG_DEC_FRAME_OUT_PARAM_T *frame_out_param_ptr,
	 											uint32 priority);

PUBLIC IMG_DEC_RET_E IMG_DEC_Destroy_Hal(IMG_DEC_TYPE_E img_type, uint32 handle, IMG_DEC_EXIT_TYPE exit_type);

 
IMG_DEC_TYPE_E IMG_DEC_GetInfo_Fast(uint8 *data_ptr, uint32 );

IMG_DEC_RET_E IMG_DEC_GetInfo_Detail(IMG_DEC_TYPE_E img_type, IMG_DEC_HAL_SRC_T *src_param_ptr, IMG_DEC_INFO_T *info_param_ptr);

void IMG_DEC_Read_Data_Offset(uint8 *data_ptr, uint32 offset, uint32 read_count, uint32 *read_count_ptr);
void IMG_DEC_Read_Data_Cur(uint8 *data_ptr, uint32 read_count, uint32 *read_count_ptr);


void IMG_DEC_File_Set_ActiveHandle(uint32 handle);

uint32 IMG_DEC_File_Open(uint16 * filename);

uint32 IMG_DEC_File_Get_Size(uint32  handle);

IMG_DEC_RET_E IMG_DEC_File_Close(uint32  handle);

PUBLIC IMG_DEC_RET_E IMG_DEC_Reg_Dec_CMD(IMG_DEC_FRAME_IN_PARAM_T *in_param_ptr,IMG_DEC_FRAME_OUT_PARAM_T *out_param_ptr);

//PUBLIC IMG_DEC_RET_E IMG_DEC_GetFrame_Hal(IMG_DEC_TYPE_E img_type, IMG_DEC_HAL_FRAME_IN_PARAM_T    *frame_in_param_ptr,IMG_DEC_HAL_FRAME_OUT_PARAM_T *frame_out_param_ptr);

PUBLIC BOOLEAN IMG_DEC_Task_Init();









#endif
