/******************************************************************************
 ** File Name:      thocr_lib.c                                              **
 ** Author:         Xiaoguang.Feng                                           **
 ** DATE:           08/23/2006                                               **
 ** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.        **
 ** Description:    load TH-OCR kernal					 					 **	
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                      **
 ** -------------------------------------------------------------------------**
 ** DATE           NAME             DESCRIPTION                              **
 ** 08/23/2006     Xiaoguang.Feng   Create.                                  **
 ******************************************************************************/
#ifndef _THOCR_LIB_C_
	#define _THOCR_LIB_C_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "ms_customize_trc.h"
#include "ocr_driver.h"
#include "tw_ocr32.h"
#include "sfs.h" //for dumping y buffer

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/

// for task
#define OCR_STACK_SIZE     		0x8000
#define OCR_QUEUE_NUM        		20

// for dump y buffer
//#define OCRlib_DUMP_Y_BUF

#define OCRlib_ROOT_DIR    		"/"

#define OCRlib_Y_BUF_FILE		"y_buffer.raw"
#define OCRlib_Y_BUF_FILE_FULL_NAME "D:\\y_buffer.raw"
#define OCRLIB_TRACE			SCI_TRACE_LOW

#define OCRLIB_MAX_STR_LENGTH		80

#define OCRLIB_STOP_THREAD_MAX_WAIT_TIME 40 //4 seconds

#define OCRLIB_CONFIG_RAM_SIZE		1000
/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                  *
 **---------------------------------------------------------------------------*/
/* ocr layout/recognition task id */
BLOCK_ID g_ocr_task_id=0;

/* caller task id */
LOCAL BLOCK_ID s_ocr_request_task_id;

/* ocr library image buffer (global) */
LOCAL uint8** s_ocr_buffer_ptr=NULL;

/* ocr library config info buffer (global) */
LOCAL char *s_ocr_config_ram_ptr =PNULL;

/* layout parameters */
LOCAL OCR_LAYOUT_PARAMETERS_T *s_ocr_layout_params_ptr=NULL;

/* review parameters */
LOCAL OCR_REVIEW_PARAM_T s_ocr_review_params;

/* auto-recognize parameters */
LOCAL OCR_RECOG_PARAMETERS_T *s_ocr_recog_params_ptr=NULL;

/* mcu buffer pointer */
LOCAL int8* s_mcu_buffer_ptr=NULL;

/* only close the task and free the ocr lib when the ocr lib is open */
LOCAL uint8 s_ocr_lib_state = 0; //0: not open, 1: during init, 2: opened

LOCAL uint8	s_layout_task_running = SCI_FALSE;

LOCAL uint8 s_recognize_task_running = SCI_FALSE;
/*--------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                */
/*--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **---------------------------------------------------------------------------*/
extern uint32 GetUExtBuffer (void);

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
/*****************************************************************
 **	Name : 			OCRlib_ThreadLayoutEntry
 ** Description:	auto layout task entry
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************/
LOCAL void OCRlib_ThreadLayoutEntry(uint32 argc, void *argv);

/*****************************************************************
 **	Name : 			OCRlib_Layout
 ** Description:	auto layout function
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************/
LOCAL OCR_ERROR_E OCRlib_Layout(int8* mcu_buffer_ptr, OCR_LAYOUT_PARAMETERS_T* ocr_layout_params);

/*****************************************************************
 **	Name : 			OCRlib_ThreadAutoRecogEntry
 ** Description:	auto recognize task entry
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************/
LOCAL void OCRlib_ThreadAutoRecogEntry(uint32 argc, void *argv);

/*****************************************************************
 **	Name : 			OCRlib_AutoRecog
 ** Description:	auto recognize function
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************/
LOCAL OCR_ERROR_E OCRlib_AutoRecog(OCR_RECOG_PARAMETERS_T* ocr_recog_params);

/*****************************************************************
 **	Name : 			OCRlib_MCU2Y
 ** Description:	mcu buffer to y buffer
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************/
LOCAL uint32 OCRlib_MCU2Y(int8* mcu_buffer_ptr, uint8* y_buffer_ptr, IMG_SIZE_T img_size);

/*****************************************************************
 **	Name : 			OCRlib_Initialize
 ** Description:	initiate ocr lib
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************/
LOCAL OCR_ERROR_E OCRlib_Initialize(int8* mcu_buffer_ptr, IMG_SIZE_T img_size);

/*****************************************************************
 **	Name : 			OCRlib_RecogOneRegion
 ** Description:	recognize one region
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************/
LOCAL OCR_ERROR_E OCRlib_RecogOneRegion(REGION region, TH_CARD_INFO* ocr_result_ptr, uint32 prefered_region);

/*****************************************************************
 **	Name : 			OCRlib_SendMsg
 ** Description:	send message to request task
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************/
LOCAL void OCRlib_SendMsg(OCR_MSG_E sig_code,uint32 param);

/*****************************************************************************
 **	Name : 			OCRlib_CallBackFunc
 ** Description:	ocr lib call back function to show progress
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL int32 OCRlib_CallBackFunc(int32 nState,int32 nInfo, void *pUser);

/*****************************************************************************
 **	Name : 			OCRlib_DumpYBuffer
 ** Description:	dump captured y buffer to file
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL int32 OCRlib_DumpYBuffer(uint8* y_buffer_ptr, uint32 y_buffer_size);

/*****************************************************************************
 **	Name : 			OCRlib_GetOCRImgBuffer
 ** Description:	get ocr image buffer
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL uint32 OCRlib_GetOCRImgBuffer(void);

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************
 **	Name : 			OCRlib_SetLayoutParams
 ** Description:	Set auto-layout parameters and the mcu image buffer
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
PUBLIC void OCRlib_SetLayoutParams(OCR_LAYOUT_PARAMETERS_T* ocr_layout_params_ptr, 
								uint8* mcu_buffer_ptr)
{
	SCI_ASSERT(mcu_buffer_ptr != NULL);/*assert verified*/
	SCI_ASSERT(ocr_layout_params_ptr !=NULL);/*assert verified*/
	
	s_ocr_layout_params_ptr=ocr_layout_params_ptr;
	
	s_mcu_buffer_ptr=(int8*)mcu_buffer_ptr;
	 
	return;
}

/*****************************************************************************
 **	Name : 			OCRlib_ThreadLayoutEntry
 ** Description:	auto layout thread entry
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL void OCRlib_ThreadLayoutEntry(uint32 argc, void *argv)
{
	void * sig_ptr=PNULL;
	
	SCI_ASSERT(s_mcu_buffer_ptr != NULL);/*assert verified*/
	SCI_ASSERT(s_ocr_layout_params_ptr != NULL);/*assert verified*/
	
	//s_layout_task_running=SCI_TRUE;
	
	//OCRLIB_TRACE:"OCRlib_ThreadLayoutEntry: OCRlib_Layout......"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_207_112_2_18_0_34_47_2021,(uint8*)"");
	OCRlib_Layout(s_mcu_buffer_ptr, s_ocr_layout_params_ptr);
	//OCRLIB_TRACE:"OCRlib_ThreadLayoutEntry: OCRlib_Layout done."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_209_112_2_18_0_34_47_2022,(uint8*)"");
	
	OCRlib_SendMsg(OCR_LAYOUT, NULL);

	while (1)
	{
		sig_ptr = (void *)SCI_GetSignal(g_ocr_task_id);
        SCI_ASSERT (sig_ptr);/*assert verified*/
	}
	
	return;
}

/*****************************************************************************
 **	Name : 			OCRlib_LayoutTaskOpen
 ** Description:	to open auto layout thread
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
PUBLIC uint32 OCRlib_LayoutTaskOpen(uint32 ocr_layout_task_priority)  
{   
    s_ocr_request_task_id=SCI_IdentifyThread();
        
    SCI_ASSERT(g_ocr_task_id==0);/*assert verified*/
    
    s_layout_task_running=SCI_TRUE;
    
    g_ocr_task_id = SCI_CreateThread("T_OCR_LAYOUT",
    						"Q_OCR_LAYOUT",
    						OCRlib_ThreadLayoutEntry,
    						0,
    						0,
    						OCR_STACK_SIZE,
    						OCR_QUEUE_NUM,
    						ocr_layout_task_priority,
    						SCI_PREEMPT,
    						SCI_AUTO_START);
    
    SCI_ASSERT(SCI_INVALID_BLOCK_ID != g_ocr_task_id);/*assert verified*/
    //SCI_TraceLow:"ocr_layout_task_open,task_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_248_112_2_18_0_34_47_2023,(uint8*)"d",g_ocr_task_id);
                            
	return SCI_SUCCESS;                            
}

/*****************************************************************************
 **	Name : 			OCRlib_LayoutTaskClose
 ** Description:	to close auto layout thread
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
PUBLIC uint32 OCRlib_LayoutTaskClose(void)
{    
	 void* sig_ptr = PNULL;
	 uint32 wait_index = 0; 	 
	  	
	 //SCI_TraceLow:"ocr_layout_task_close,task_id = %d"
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_264_112_2_18_0_34_47_2024,(uint8*)"d",g_ocr_task_id);
	
	 if (g_ocr_task_id!=0)
	 { 	 
	 	 if (1 == s_ocr_lib_state) //during init...
	 	 {
			 //SCI_TraceLow:"ocr lib being init, can't stop task now, wait."
			 SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_270_112_2_18_0_34_47_2025,(uint8*)"");
		 	 while (1 == s_ocr_lib_state && 
		 	 		wait_index<OCRLIB_STOP_THREAD_MAX_WAIT_TIME)
		 	 {
		 	 	SCI_SLEEP(100);
		 	 	wait_index++;
		 	 }
			 //SCI_TraceLow:"ocr lib is open, ready to stop, wait index=%d."
			 SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_277_112_2_18_0_34_47_2026,(uint8*)"d",wait_index);
		 }
		 	 	 
		 SCI_SuspendThread(g_ocr_task_id);
		 
	     // Empty the signal queue.
	     while (1)
	     {
	     	sig_ptr = (void*)SCI_PeekSignal(g_ocr_task_id);
	        if (sig_ptr != PNULL)
	        {
	        	SCI_FREE(sig_ptr);
	        }
	        else
	        {
	            break;
	        }
	     }
	     
		 SCI_TerminateThread(g_ocr_task_id);
		 
		 SCI_DeleteThread(g_ocr_task_id);
		 
		 g_ocr_task_id=0;
		 
	 	s_layout_task_running=SCI_FALSE;
	}
		 
	return SCI_SUCCESS;
} 

/*****************************************************************************
 **	Name : 			OCRlib_SetRecogParams
 ** Description:	set auto recognize parameters
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
PUBLIC void OCRlib_SetRecogParams(OCR_RECOG_PARAMETERS_T* ocr_recog_params_ptr)
{
	SCI_ASSERT(ocr_recog_params_ptr != NULL);/*assert verified*/

	s_ocr_recog_params_ptr=ocr_recog_params_ptr;
	
	return;
}

/*****************************************************************************
 **	Name : 			OCRlib_ThreadAutoRecogEntry
 ** Description:	auto recognize thread entry
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL void OCRlib_ThreadAutoRecogEntry(uint32 argc, void *argv)
{
	void* sig_ptr = PNULL;
	
	SCI_ASSERT(s_ocr_recog_params_ptr != NULL);/*assert verified*/
	
	//s_recognize_task_running=SCI_TRUE;
	
	OCRlib_AutoRecog(s_ocr_recog_params_ptr);
	//OCRLIB_TRACE:"OCRlib_ThreadAutoRecogEntry: OCRlib_AutoRecog done."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_338_112_2_18_0_34_47_2027,(uint8*)"");
	
	OCRlib_SendMsg(OCR_RECOG, NULL);

	while (1)
	{
		sig_ptr = (void *)SCI_GetSignal(g_ocr_task_id);
        SCI_ASSERT (sig_ptr);/*assert verified*/
	}
	
	return;
}

/*****************************************************************************
 **	Name : 			OCRlib_AutoRecogTaskOpen
 ** Description:	to open auto recognize thread
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
PUBLIC uint32 OCRlib_AutoRecogTaskOpen(uint32 ocr_autorecog_task_priority)  
{   
    s_ocr_request_task_id=SCI_IdentifyThread();
        
    SCI_ASSERT(g_ocr_task_id==0);/*assert verified*/

	s_recognize_task_running=SCI_TRUE;
	
    g_ocr_task_id = SCI_CreateAppThread("T_OCR_AUTORECOG",
    						"Q_OCR_AUTORECOG",
    						OCRlib_ThreadAutoRecogEntry,
    						0,
    						0,
    						OCR_STACK_SIZE,
    						OCR_QUEUE_NUM,
    						ocr_autorecog_task_priority,
    						SCI_PREEMPT,
    						SCI_AUTO_START);
    
    SCI_ASSERT(SCI_INVALID_BLOCK_ID != g_ocr_task_id);/*assert verified*/
	//SCI_TraceLow:"ocr_autorecog_task_open,task_id = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_377_112_2_18_0_34_47_2028,(uint8*)"d",g_ocr_task_id);
                            
	return SCI_SUCCESS;                            
}

/*****************************************************************************
 **	Name : 			OCRlib_AutoRecogTaskClose
 ** Description:	to close auto recognize thread
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
PUBLIC uint32 OCRlib_AutoRecogTaskClose(void)
{    
	 void* sig_ptr = PNULL;
	  	 
	 //SCI_TraceLow:"ocr_autorecog_task_close,task_id = %d"
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_392_112_2_18_0_34_47_2029,(uint8*)"d",g_ocr_task_id);
	 
	 if (g_ocr_task_id!=0)
	 {
		 SCI_SuspendThread(g_ocr_task_id);
		 
	     // Empty the signal queue.
	     while (1)
	     {
	     	sig_ptr = (void*)SCI_PeekSignal(g_ocr_task_id);
	        if (sig_ptr != PNULL)
	        {
	        	SCI_FREE(sig_ptr);
	        }
	        else
	        {
	            break;
	        }
	     }
		 	 
		 SCI_TerminateThread(g_ocr_task_id);
		 
		 SCI_DeleteThread(g_ocr_task_id);

		 g_ocr_task_id=0;
		 
		 s_recognize_task_running=SCI_FALSE;		 
	}
		 
	return SCI_SUCCESS;
} 

/*****************************************************************************
 **	Name : 			OCRlib_SendMsg
 ** Description:	send message to request thread
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL void OCRlib_SendMsg(OCR_MSG_E sig_code,uint32 param)
{
	OCR_SIGNAL_T      *sig_ptr = NULL;
    xSignalHeaderRec  *sig = NULL;
	
	//SCI_TRACE_LOW:"OCR:OCR_hSendMsg sig_code = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_435_112_2_18_0_34_47_2030,(uint8*)"d",sig_code);
	
    sig_ptr = (OCR_SIGNAL_T *)SCI_ALLOC_APP(sizeof(OCR_SIGNAL_T));
    SCI_ASSERT(SCI_NULL != sig_ptr);/*assert verified*/
    
    sig = &sig_ptr->sig;
    sig->SignalSize = sizeof(OCR_SIGNAL_T);
    sig->Sender     = g_ocr_task_id;
    sig->SignalCode = sig_code;
    sig_ptr->param  = param ;
    
    SCI_SEND_SIGNAL((xSignalHeaderRec*)sig_ptr, s_ocr_request_task_id);
}

/*****************************************************************************
 **	Name : 			OCRlib_MCU2Y
 ** Description:	mcu buffer to y buffer
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL uint32 OCRlib_MCU2Y(int8* mcu_buffer_ptr, uint8* y_buffer_ptr, IMG_SIZE_T img_size)
{
	uint32 i = 0;
	uint32 j = 0;
	uint32 m = 0;
	uint32 n = 0;
	uint32 mcu_y_count=img_size.height/8;
	uint32 mcu_x_count=img_size.width/16;
	
	int8  *tmp_mcu_ptr = NULL;
	uint8 *tmp_y_ptr = NULL;
	uint8 *tmp_y_ptr2 = NULL;
	
	uint16 trim_offset_x = 0;
	uint16 trim_offset_y = 0;
	uint16 trim_width = 0;
	uint16 trim_height = 0;
	
	SCI_ASSERT(mcu_buffer_ptr != NULL);/*assert verified*/
	SCI_ASSERT(y_buffer_ptr != NULL);/*assert verified*/

	tmp_mcu_ptr = mcu_buffer_ptr;
	tmp_y_ptr	= y_buffer_ptr;

	for (i=0;i<mcu_y_count;i++)
	{
		for (j=0;j<mcu_x_count;j++)
		{
			// first y block
			tmp_y_ptr=y_buffer_ptr+i*8*img_size.width+16*j;
			for (m=0;m<8;m++)
			{
				for (n=0;n<8;n++)
				{
					*(tmp_y_ptr+n)=*(tmp_mcu_ptr++)+128;
				}
				tmp_y_ptr=tmp_y_ptr+img_size.width;
			}
			
			// second y block
			tmp_y_ptr=y_buffer_ptr+i*8*img_size.width+16*j+8;
			for (m=0;m<8;m++)
			{
				for (n=0;n<8;n++)
				{
					*(tmp_y_ptr+n)=*(tmp_mcu_ptr++)+128;
				}
				tmp_y_ptr=tmp_y_ptr+img_size.width;
			}

			tmp_mcu_ptr+=128;
		}
	}
	
	// trim
	trim_offset_x 	= s_ocr_review_params.st_review_param.st_trim_rect.w_start_x;
	trim_offset_y 	= s_ocr_review_params.st_review_param.st_trim_rect.w_start_y;
	trim_width 		= s_ocr_review_params.st_review_param.st_trim_rect.w_width;
	trim_height		= s_ocr_review_params.st_review_param.st_trim_rect.w_height;

	tmp_y_ptr	= y_buffer_ptr; //write pointer
	tmp_y_ptr2	= y_buffer_ptr+trim_offset_y*img_size.width+trim_offset_x;//read pointer
	for (i=0;i<trim_height;i++)
	{
		for (j=0;j<trim_width;j++)
		{
			*(tmp_y_ptr++)=*(tmp_y_ptr2++);
		}
		tmp_y_ptr2=tmp_y_ptr2+img_size.width-trim_width;
	}	
	
	return SCI_SUCCESS;
}

/*****************************************************************************
 **	Name : 			OCRlib_Layout
 ** Description:	auto layout
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL OCR_ERROR_E OCRlib_Layout(int8* mcu_buffer_ptr, OCR_LAYOUT_PARAMETERS_T* ocr_layout_params)
{
	int32 ret = ERR_NONE;
	uint32 start_time=0;

	SCI_ASSERT(mcu_buffer_ptr != NULL);/*assert verified*/
	SCI_ASSERT(ocr_layout_params != NULL);/*assert verified*/

	if (s_ocr_lib_state!=0)
	{
		OCRlib_Free();
	}
	
	s_ocr_lib_state = 1; //during init...
	
	//OCRLIB_TRACE:"OCRlib_Layout: OCRlib_Initialize....."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_550_112_2_18_0_34_48_2031,(uint8*)"");
	
	OCRlib_Initialize(mcu_buffer_ptr,ocr_layout_params->img_size);
	
	//OCRLIB_TRACE:"OCRlib_Layout: OCRlib_Initialize done."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_554_112_2_18_0_34_48_2032,(uint8*)"");

	s_ocr_lib_state = 2; //opened
	
	//OCRLIB_TRACE:"OCRlib_Layout: ARM clock = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_558_112_2_18_0_34_48_2033,(uint8*)"d", CHIP_GetArmClk());

	// auto layout
	start_time=SCI_GetTickCount();
	ret=TH_Layout(OCRLIB_MAX_LAYOUT_REGION, (int*)&ocr_layout_params->num_of_regions, ocr_layout_params->regions_ptr, s_ocr_config_ram_ptr);

	//OCRLIB_TRACE:"OCRlib_Layout: running time = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_564_112_2_18_0_34_48_2034,(uint8*)"d", SCI_GetTickCount()-start_time);

	//OCRLIB_TRACE:"OCRlib_Layout: Num of Regions: %d."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_566_112_2_18_0_34_48_2035,(uint8*)"d",ocr_layout_params->num_of_regions);

	SCI_ASSERT(ocr_layout_params->num_of_regions<=OCRLIB_MAX_LAYOUT_REGION);/*assert verified*/
	
	if (ret!=ERR_NONE) 
	{
    	//SCI_TraceLow:"ocr layout failed, error code = %d"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_572_112_2_18_0_34_48_2036,(uint8*)"d",ret);
		return OCR_ERROR;
	}

	return OCR_NONE_ERROR;
}

/*****************************************************************************
 **	Name : 			OCRlib_AutoRecog
 ** Description:	auto recognize
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL OCR_ERROR_E OCRlib_AutoRecog(OCR_RECOG_PARAMETERS_T* ocr_recog_params)
{
	uint32 i = 0;
	uint32 index_r = 0;
	uint32 index_w = 0;
	uint32 num_of_regions=ocr_recog_params->num_of_regions;
	uint32 start_time=0;
	uint32 prefer_region_type=TH_LANGUAGE_AUTO;
	uint8  phone_number_done = 0;
	uint8  mobile_number_done = 0;

	SCI_ASSERT(ocr_recog_params != NULL);/*assert verified*/
	
	//OCRLIB_TRACE:"OCRlib_AutoRecog: ARM clock = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_598_112_2_18_0_34_48_2037,(uint8*)"d", CHIP_GetArmClk());
	
	for (i=0;i<num_of_regions;i++)
	{
		start_time=SCI_GetTickCount();
		//OCRLIB_TRACE:"OCRlib_AutoRecog: region %d ..."
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_603_112_2_18_0_34_48_2038,(uint8*)"d",i);
		//OCRLIB_TRACE:"OCRlib_AutoRecog: left=%d top=%d right=%d bottom=%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_607_112_2_18_0_34_48_2039,(uint8*)"dddd",	ocr_recog_params->regions_ptr[i].left,ocr_recog_params->regions_ptr[i].top,ocr_recog_params->regions_ptr[i].right,ocr_recog_params->regions_ptr[i].bottom);
		
		// To improve recognize speed, make the following simplification in recognition type.
		// If need to improve accuracy, or the speed is fast enough, remove this simplification.
		if (0==i) // 1st region, auto class
		{
			prefer_region_type = TH_LANGUAGE_AUTO;
		}
		else //if (i==num_of_regions-1 && num_of_regions>2) // last region, email?
		{
			prefer_region_type = TH_LANGUAGE_ENGLISH;
		}
/*		else // other regions, numbers
		{
			prefer_region_type = TH_LANGUAGE_PHONE;
		} 
*/		
		// to recognize current region
		OCRlib_RecogOneRegion(ocr_recog_params->regions_ptr[i], &(ocr_recog_params->ocr_results_ptr[i]),prefer_region_type);
		
		// region type post processing
		if (CARD_AREA_PHONE == ocr_recog_params->ocr_results_ptr[i].wClass[0])
		{
			if (0 == phone_number_done)
			{
				phone_number_done = 1;
			}
			else
			{
				if (0 == mobile_number_done)
				{
					 ocr_recog_params->ocr_results_ptr[i].wClass[0] = CARD_AREA_MOBILE;
					 mobile_number_done = 1;
				}
				else
				{
					 ocr_recog_params->ocr_results_ptr[i].wClass[0] = 0;
				}
			}
		}
		else if (CARD_AREA_MOBILE == ocr_recog_params->ocr_results_ptr[i].wClass[0])
		{
			if (0 == mobile_number_done)
			{
				mobile_number_done = 1;
			}
			else
			{
				if (0 == phone_number_done)
				{
					 ocr_recog_params->ocr_results_ptr[i].wClass[0] = CARD_AREA_PHONE;
					 phone_number_done = 1;
				}
				else
				{
					 ocr_recog_params->ocr_results_ptr[i].wClass[0] = 0;
				}
			}
		}
		
		index_r=0;
		index_w=0;
		
		// post processing for mobile or phone number (remove non-number characters)
		if (CARD_AREA_MOBILE == ocr_recog_params->ocr_results_ptr[i].wClass[0] ||
			CARD_AREA_PHONE == ocr_recog_params->ocr_results_ptr[i].wClass[0])
		{
			while (	ocr_recog_params->ocr_results_ptr[i].String[index_r]!=0 &&
					index_r < OCRLIB_MAX_STR_LENGTH)
			{
				if (ocr_recog_params->ocr_results_ptr[i].String[index_r]>=48 && // 0 in UNICODE: 0x30
					ocr_recog_params->ocr_results_ptr[i].String[index_r]<=57) // 9 in UNICODE: 0x39
				{
					ocr_recog_params->ocr_results_ptr[i].String[index_w]=ocr_recog_params->ocr_results_ptr[i].String[index_r];
					index_w++;
				}
				index_r++;
			} 
			ocr_recog_params->ocr_results_ptr[i].String[index_w]=0;
		}
		// post processing for email (remove characters before ":" and remove space)
		else if (CARD_AREA_EMAIL ==  ocr_recog_params->ocr_results_ptr[i].wClass[0])
		{
			while (	ocr_recog_params->ocr_results_ptr[i].String[index_r]!=0 &&
					index_r < OCRLIB_MAX_STR_LENGTH)
			{
				if (58==ocr_recog_params->ocr_results_ptr[i].String[index_r]) //":" in UNICODE: 0x3A
				{
					index_w=0; //remove all characters before ":"
				}
				else if (ocr_recog_params->ocr_results_ptr[i].String[index_r]!=32) //space in UNICODE: 0x20
				{
					ocr_recog_params->ocr_results_ptr[i].String[index_w]=ocr_recog_params->ocr_results_ptr[i].String[index_r];
					index_w++;
				}
				index_r++;
			} 
			ocr_recog_params->ocr_results_ptr[i].String[index_w]=0;
		}
		else // post processing for other results (remove space)
		{
			while (	ocr_recog_params->ocr_results_ptr[i].String[index_r]!=0 &&
					index_r < OCRLIB_MAX_STR_LENGTH)
			{
				if (ocr_recog_params->ocr_results_ptr[i].String[index_r]!=32) //space in UNICODE: 0x20
				{
					ocr_recog_params->ocr_results_ptr[i].String[index_w]=ocr_recog_params->ocr_results_ptr[i].String[index_r];
					index_w++;
				}
				index_r++;
			} 
			ocr_recog_params->ocr_results_ptr[i].String[index_w]=0;
		}
		
		//OCRLIB_TRACE:"OCRlib_AutoRecog: UNICODE (first 10) %d,%d,%d,%d,%d,%d,%d,%d,%d,%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_730_112_2_18_0_34_48_2040,(uint8*)"dddddddddd",ocr_recog_params->ocr_results_ptr[i].String[0],ocr_recog_params->ocr_results_ptr[i].String[1],ocr_recog_params->ocr_results_ptr[i].String[2],ocr_recog_params->ocr_results_ptr[i].String[3],ocr_recog_params->ocr_results_ptr[i].String[4],ocr_recog_params->ocr_results_ptr[i].String[5],ocr_recog_params->ocr_results_ptr[i].String[6],ocr_recog_params->ocr_results_ptr[i].String[7],ocr_recog_params->ocr_results_ptr[i].String[8],ocr_recog_params->ocr_results_ptr[i].String[9]);
						
		//OCRLIB_TRACE:"OCRlib_AutoRecog: Class %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_732_112_2_18_0_34_48_2041,(uint8*)"d",ocr_recog_params->ocr_results_ptr[i].wClass[0]);

		//OCRLIB_TRACE:"OCRlib_AutoRecog: region %d done."
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_734_112_2_18_0_34_48_2042,(uint8*)"d",i);
	
		//OCRLIB_TRACE:"OCRlib_AutoRecog: running time = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_736_112_2_18_0_34_48_2043,(uint8*)"d", SCI_GetTickCount()-start_time);
	}

	return OCR_NONE_ERROR;
}

/*****************************************************************************
 **	Name : 			OCRlib_IsLayoutTaskRunning
 ** Description:	return whether layout task is running
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
PUBLIC uint8 OCRlib_IsLayoutTaskRunning(void)
{
	return s_layout_task_running;
}

/*****************************************************************************
 **	Name : 			OCRlib_IsRecognizeTaskRunning
 ** Description:	return whether recognize task is running
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
PUBLIC uint8 OCRlib_IsRecognizeTaskRunning(void)
{
	return s_recognize_task_running;
}

/*****************************************************************************
 **	Name : 			OCRlib_Free
 ** Description:	free ocr lib resources
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
PUBLIC OCR_ERROR_E OCRlib_Free(void)
{
	if (2 == s_ocr_lib_state) //only close it if it's already opened
	{
		THOCRTerm(s_ocr_config_ram_ptr);
		s_ocr_lib_state = 0;
	}

	// NOTE: s_ocr_buffer_ptr is getted from static allocated U buffer.
	// 		 no	need to free s_ocr_buffer_ptr
	
	return OCR_NONE_ERROR;
}

/*****************************************************************************
 **	Name : 			OCRlib_CallBackFunc
 ** Description:	ocr lib call back function to show progress
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL int32 OCRlib_CallBackFunc(int32 nState,int32 nInfo, void *pUser)
{
	//SCI_TRACE_LOW:"OCRlib_CallBackFunc: nState=%d, nInfo=%d\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_792_112_2_18_0_34_48_2044,(uint8*)"dd",nState,nInfo);
	return 0;
}

/*****************************************************************************
 **	Name : 			OCRlib_GetOCRImgBuffer
 ** Description:	get ocr image buffer
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL uint32 OCRlib_GetOCRImgBuffer(void)
{
	return (OCR_YUV_BUF + OCR_GetOCRYUVDataSize());
}

/*****************************************************************************
 **	Name : 			OCRlib_Initialize
 ** Description:	initiate ocr lib
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL OCR_ERROR_E OCRlib_Initialize(int8* mcu_buffer_ptr, IMG_SIZE_T img_size)
{
	uint32 img_height 	= 0;
	uint32 img_width	= 0;
	uint8* y_buffer_ptr	= NULL;
	int32 ret			= ERR_NONE;
	int32 ocr_img_type	= 0;
	uint32 i			= 0;	
	uint8 scale_x		= 0;
	uint8 scale_y		= 0;
	uint32 disp_width	= 0;
	uint32 disp_height	= 0;
	uint32 lib_buffer_size=0;
	
	SCI_ASSERT(mcu_buffer_ptr != NULL);/*assert verified*/
	
	// get review trim info
	s_ocr_review_params=OCR_GetReviewParam();
	
	// get display image scale rate
	OCR_GetScaleFactor(&scale_x, &scale_y);
	
	// get display size
	OCR_GetDispSize(&disp_width, &disp_height);
	
	SCI_ASSERT(scale_x!=0 && scale_y!=0);/*assert verified*/
	
	// adjust trim image size according to display image scale rate
	s_ocr_review_params.st_review_param.st_trim_rect.w_height=disp_height*scale_y/4;
	s_ocr_review_params.st_review_param.st_trim_rect.w_width=disp_width*scale_x/4;

	img_height = s_ocr_review_params.st_review_param.st_trim_rect.w_height;
	img_width = s_ocr_review_params.st_review_param.st_trim_rect.w_width;
	
	// assemble ocr buffer
	//s_ocr_buffer_ptr= (uint8**) SCI_ALLOC_APP(img_height*(img_width+sizeof(uint8 *)));
	s_ocr_buffer_ptr = (uint8**)OCRlib_GetOCRImgBuffer();

	SCI_ASSERT(s_ocr_buffer_ptr != NULL);/*assert verified*/
	
	s_ocr_buffer_ptr[0]= (uint8 *)s_ocr_buffer_ptr + img_height*sizeof(uint8 *);

	for (i=1;i<img_height;i++)
	{
		s_ocr_buffer_ptr[i]= s_ocr_buffer_ptr[i-1]+img_width;
	}

	y_buffer_ptr=s_ocr_buffer_ptr[0];

	//get y buffer from input mcu buffer
	OCRlib_MCU2Y(mcu_buffer_ptr, y_buffer_ptr, img_size);
	//OCRLIB_TRACE("OCRlib_Initialize: MCU2Y done.");
	
	#ifdef OCRlib_DUMP_Y_BUF
		// dump y buffer to file, only for debug use
		//OCRLIB_TRACE:"OCRlib_Initialize: dumping y buffer..."
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_868_112_2_18_0_34_48_2045,(uint8*)"");
		if (!OCRlib_DumpYBuffer(y_buffer_ptr,img_height*img_width))
		{
			//OCRLIB_TRACE:"OCRlib_Initialize: dumping y buffer done."
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_871_112_2_18_0_34_48_2046,(uint8*)"");
		}
		else
		{
			//OCRLIB_TRACE:"OCRlib_Initialize: dumping y buffer failed."
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_875_112_2_18_0_34_48_2047,(uint8*)"");
		}
	#endif
	
	// Get OCR lib version number
	//OCRLIB_TRACE:"OCRlib_Initialize: OCR LIB VER %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_880_112_2_18_0_34_48_2048,(uint8*)"d",GetOCRVersion());
	
	// get the static buffer for the ocr lib
	s_ocr_config_ram_ptr=(char*)OCR_GetOCRBuffer(&lib_buffer_size);
	
	SCI_ASSERT(PNULL!=s_ocr_config_ram_ptr);/*assert verified*/
	
	// Initialize TH-OCR kernal
	ret = THOCRInit(TH_EXPORTCODE_UNICODE, 0x08, NULL, s_ocr_config_ram_ptr, lib_buffer_size);

	if (ret!=ERR_NONE) 
	{
    	//SCI_TraceLow:"ocr initialize failed, error code = %d"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_892_112_2_18_0_34_48_2049,(uint8*)"d",ret);
		return OCR_ERROR;
	}

	TH_CallBackAddress((TH_CallBackFunc)OCRlib_CallBackFunc, NULL, s_ocr_config_ram_ptr);
	
	ocr_img_type=1; //gray image
	
	// set ocr image buffer
	ret = TH_SetImage(s_ocr_buffer_ptr, img_width, img_height, ocr_img_type, s_ocr_config_ram_ptr);
	
	if (ret!=ERR_NONE) 
	{
    	//SCI_TraceLow:"ocr set image failed, error code = %d"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_905_112_2_18_0_34_48_2050,(uint8*)"d",ret);
		return OCR_ERROR;
	}

	return OCR_NONE_ERROR;
}

/*****************************************************************************
 **	Name : 			OCRlib_RecogOneRegion
 ** Description:	recognize one region
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL OCR_ERROR_E OCRlib_RecogOneRegion(REGION region, TH_CARD_INFO* ocr_result_ptr, uint32 prefered_region)
{
	int32 ocr_char_info_size	= 0;
	TH_REGION recog_region;
	TH_CHARINFO char_info_ptr[50];
	int32 ocr_preferred_area	= 0;
	int32 ret	= 0;
    uint16 class_list_ptr[5];

	SCI_ASSERT(ocr_result_ptr != NULL);/*assert verified*/

	// system required classes:
    class_list_ptr[0] = CARD_AREA_EMAIL ;
    class_list_ptr[1] = CARD_AREA_PHONE ;
    class_list_ptr[2] = CARD_AREA_MOBILE;
    class_list_ptr[3] = CARD_AREA_NAME_CHN;
    class_list_ptr[4] = CARD_AREA_NAME_ENG;

	recog_region.bottom 			= region.bottom;
	recog_region.top 				= region.top;
	recog_region.left				= region.left;
	recog_region.right				= region.right;
	recog_region.type				= region.iAttribute;
	recog_region.ImageBackground 	= RGN_IMG_AUTO;
	
	if (TH_LANGUAGE_AUTO == prefered_region 		||
	 	TH_LANGUAGE_SCHINESE == prefered_region 	||
		TH_LANGUAGE_TCHINESE == prefered_region 	||
		TH_LANGUAGE_JAPANESE == prefered_region 	||
		TH_LANGUAGE_KOREAN == prefered_region 		||
		TH_LANGUAGE_ENGLISH == prefered_region 		||
		TH_LANGUAGE_PHONE == prefered_region 		||
		TH_LANGUAGE_EMAIL == prefered_region 		||
		TH_LANGUAGE_WWW == prefered_region 			||
		TH_LANGUAGE_FRENCH == prefered_region 		||
		TH_LANGUAGE_DANISH == prefered_region 		||
		TH_LANGUAGE_FINNISH == prefered_region 		||
		TH_LANGUAGE_GERMAN == prefered_region 		||
		TH_LANGUAGE_ITALIAN == prefered_region 		||
		TH_LANGUAGE_NORWEGIAN == prefered_region	||
		TH_LANGUAGE_PORTUGUESE == prefered_region 	||
		TH_LANGUAGE_SPANISH == prefered_region 		||
		TH_LANGUAGE_SWEDISH == prefered_region)		
	{
		recog_region.font = prefered_region;
		
	}
	else
	{
		recog_region.font = TH_LANGUAGE_AUTO;
	}

	ocr_char_info_size				= 50;

	// recognize one region
	//OCRLIB_TRACE:"OCRlib_RecogOneRegion: TH_RecogOneRegion..."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_973_112_2_18_0_34_49_2051,(uint8*)"");
	ret=TH_RecogOneRegion(&recog_region, char_info_ptr, (int*)&ocr_char_info_size, s_ocr_config_ram_ptr);
	//OCRLIB_TRACE:"OCRlib_RecogOneRegion: TH_RecogOneRegion done."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_975_112_2_18_0_34_49_2052,(uint8*)"");
	if (ret!=ERR_NONE) 
	{
   		//SCI_TraceLow:"ocr recognition failed, error code = %d"
   		SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_978_112_2_18_0_34_49_2053,(uint8*)"d",ret);
		return OCR_ERROR;
	}

	ocr_result_ptr->bottom	= region.bottom;
	ocr_result_ptr->left	= region.left;
	ocr_result_ptr->right	= region.right;
	ocr_result_ptr->top		= region.top;

	// classify the recognition result
	ocr_preferred_area=CARD_AREA_ALL;
		
	//OCRLIB_TRACE:"OCRlib_RecogOneRegion: TH_ClassifyOneRegionByKonwClass..."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_990_112_2_18_0_34_49_2054,(uint8*)"");
	ret=TH_ClassifyOneRegionByKonwClass(ocr_preferred_area, 
										char_info_ptr+1, 
										ocr_char_info_size-1, 
										ocr_result_ptr,
										s_ocr_config_ram_ptr,
										5,
										class_list_ptr);
	//OCRLIB_TRACE:"OCRlib_RecogOneRegion: TH_ClassifyOneRegionByKonwClass done."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_998_112_2_18_0_34_49_2055,(uint8*)"");
	if (ret!=1) // see tw_ocr32.h
	{
   		//SCI_TRACE_LOW:"ocr classification failed, error code = %d"
   		SCI_TRACE_ID(TRACE_TOOL_CONVERT,THOCR_LIB_1001_112_2_18_0_34_49_2056,(uint8*)"d",ret);
		return OCR_ERROR;
	}
	
	return OCR_NONE_ERROR;
}


/*****************************************************************************
 **	Name : 			OCRlib_DumpYBuffer
 ** Description:	dump captured y buffer to file
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL int32 OCRlib_DumpYBuffer(uint8* y_buffer_ptr, uint32 y_buffer_size)
{
	HFS				cur_hfs = 0;
	FFS_ERROR_E		ffs_err = 0;
	uint32 			transmit = 0;
	uint16			full_file_name[128] = {0};
	
	// cd to root directory
	GUI_GB2UCS(full_file_name, (uint8 *)OCRlib_Y_BUF_FILE_FULL_NAME, strlen(OCRlib_Y_BUF_FILE_FULL_NAME));

	// create file 	
	cur_hfs = SFS_CreateFile( full_file_name, FFS_MODE_CREATE_ALWAYS | FFS_MODE_WRITE, NULL, NULL);
	if (cur_hfs == FFS_INVALID_HANDLE)
	{
		return -1;
	}
	
	// write file

	ffs_err = SFS_WriteFile(cur_hfs, y_buffer_ptr, y_buffer_size, &transmit, NULL);
	if (FFS_NO_ERROR == ffs_err)
	{
		// close file
		SFS_CloseFile(cur_hfs);
		return 0;
	}
	else
	{
		//close file
		SFS_CloseFile(cur_hfs);
		return -1;
	}
	
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
// End 
#endif

