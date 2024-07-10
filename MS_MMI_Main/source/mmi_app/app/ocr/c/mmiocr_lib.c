/******************************************************************************
 ** File Name:      mmiocr_lib.c                                             **
 ** Author:         jian.ma                                          		 **
 ** DATE:           05/30/2007                                               **
 ** Copyright:      2006 Spreadtrum, Incroporated. All Rights Reserved.      **
 ** Description:    load TH-OCR kernal					 					 **	
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                      **
 ** -------------------------------------------------------------------------**
 ** DATE           NAME             DESCRIPTION                              **
 ** 05/30/2007     jian.ma    		Create.                                  **
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "mmi_app_ocr_trc.h"
#include "std_header.h"

#ifdef MMI_OCR_ENABLE
#include "sci_api.h"
#include "mmiocr_lib.h"
#include "mmiocr_internal.h"
#include "block_mem.h"
#include "mmiocr_export.h"
#include "freq_cfg.h"

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
#define OCR_STACK_SIZE     			0x8000
#define OCR_QUEUE_NUM        		20

#define OCRLIB_MAX_STR_LENGTH		80
#define OCRLIB_TRACE				SCI_TRACE_LOW
#ifndef  WIN32
LOCAL uint32 s_ocr_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
#else
LOCAL uint32 s_ocr_chng_freq_req_handler = 0;
#endif
/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                 *
 **---------------------------------------------------------------------------*/
/* caller task id */
LOCAL BLOCK_ID s_ocr_request_task_id;

/* ocr layout/recognition task id */
BLOCK_ID g_ocr_task_id=0;

LOCAL uint8 s_recognize_task_running = SCI_FALSE;

/* ocr library config info buffer (global) */
LOCAL char *s_ocr_config_ram_ptr =PNULL;

/* ocr library image buffer (global) */
LOCAL uint8** s_ocr_buffer_ptr=NULL;

LOCAL OCR_CARD_INFO s_card_info[OCR_MAX_RECOG_REGION_NUM];

//#pragma arm section zidata = "OCRBuffer"
//LOCAL uint32 s_ocr_lib_buffer[MMIOCR_MAX_SIZE/sizeof(uint32)];
LOCAL uint32    *s_ocr_lib_buffer = PNULL;
//#pragma arm section zidata
LOCAL BOOLEAN  s_ocr_reg_start = FALSE;
/*--------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                */
/*--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
/*****************************************************************************
 **	Name : 			OCRlib_AutoRecogTaskOpen
 ** Description:	to open auto recognize thread
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL uint32 OCRlib_AutoRecogTaskOpen(uint32 ocr_autorecog_task_priority);

/*****************************************************************************
 **	Name : 			OCRlib_ThreadAutoRecogEntry
 ** Description:	auto recognize thread entry
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL void OCRlib_ThreadAutoRecogEntry(uint32 argc, void *argv);

/*****************************************************************************
 **	Name : 			OCRlib_AutoRecog
 ** Description:	auto recognize
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL OCR_ERROR_E OCRlib_AutoRecog(void);


/*****************************************************************************
 **	Name : 			OCRlib_SendMsg
 ** Description:	send message to request thread
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL void OCRlib_SendMsg(OCR_MSG_E sig_code,uint32 param);

/*****************************************************************************
 **	Name : 			OCRlib_AutoRecogTaskClose
 ** Description:	to close auto recognize thread
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL uint32 OCRlib_AutoRecogTaskClose(void);


/*****************************************************************************
 **	Name : 			OCRlib_Initialize
 ** Description:	initiate ocr lib
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL OCR_ERROR_E OCRlib_Initialize(void);


/*****************************************************************************
 **	Name : 			OCRlib_CallBackFunc
 ** Description:	ocr lib call back function to show progress
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL int32 OCRlib_CallBackFunc(int32 nState,int32 nInfo, void *pUser);

/*****************************************************************************
 ** Description : free OCR buffer .
 **	Global resource dependence : 
 ** Author:jian.ma
 **	Note:		
*****************************************************************************/
LOCAL void FreeOCRBuffer( void );

/*****************************************************************************
 ** Description : Alloc OCR buffer .
 **	Global resource dependence : 
 ** Author:jian.ma
 **	Note:		
*****************************************************************************/
LOCAL BOOLEAN AllocOCRBuffer( void );

/******************************************************************************
 **	Description: 	OCR do auto recognition
 **	Author:			jian.ma
 **	Input:			none
 **	Output:     	none
 **	Return:     	0           successful
 **					ohter: error (refer to PUBLIC OCR_ERROR_E)
 **	Note:       	None
******************************************************************************/
PUBLIC OCR_ERROR_E OCR_AutoRecog(void)
{
    
#ifndef _WIN32
    if (CHNG_FREQ_REQ_HANDLER_NULL == s_ocr_chng_freq_req_handler)
    {
    	s_ocr_chng_freq_req_handler = CHNG_FREQ_GetReqHandler("ocr");
    	SCI_ASSERT (CHNG_FREQ_REQ_HANDLER_NULL != s_ocr_chng_freq_req_handler);/*assert verified*/
    }
    CHNG_FREQ_SetArmClk(s_ocr_chng_freq_req_handler,(CHNG_FREQ_INDEX_E)FREQ_INDEX_APP_HIGH);
   
    if(OCRlib_AutoRecogTaskOpen(PRI_OCR_TASK))
    {
        return OCR_ERROR;
    }
#endif
    s_ocr_reg_start = TRUE;
	return OCR_NONE_ERROR;
}

/*****************************************************************************
 **	Name : 			OCRlib_AutoRecogTaskOpen
 ** Description:	to open auto recognize thread
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL uint32 OCRlib_AutoRecogTaskOpen(uint32 ocr_autorecog_task_priority)  
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
	
                            
	return SCI_SUCCESS;                            
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
	
	OCRlib_AutoRecog();
	//OCRLIB_TRACE:"[MMIOCR]:OCRlib_ThreadAutoRecogEntry: OCRlib_AutoRecog done."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_LIB_216_112_2_18_2_42_8_14,(uint8*)"");
	
	OCRlib_SendMsg(OCR_RECOG, NULL);

	while (1)
	{
		sig_ptr = (void *)SCI_GetSignal(SCI_IdentifyThread());
        SCI_ASSERT (sig_ptr);/*assert verified*/
	}
	
	return;
}


/*****************************************************************************
 **	Name : 			OCRlib_AutoRecog
 ** Description:	auto recognize
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL OCR_ERROR_E OCRlib_AutoRecog()
{
	int nCardInfoSize=OCR_MAX_RECOG_REGION_NUM;
	uint8 ret = 0;
	int16 num = 0;		

	
	OCRlib_Initialize();
	
	SCI_MEMSET(s_card_info,0x00,sizeof(OCR_CARD_INFO)*OCR_MAX_RECOG_REGION_NUM);
	num = SCI_GetTickCount();

	
	ret = TH_RecogNameCard(TH_LANGUAGE_AUTO, 0xffff, s_card_info, &nCardInfoSize, s_ocr_config_ram_ptr);
	
	num = SCI_GetTickCount() - num;

	if(ret == ERR_NONE)
	{
		//SCI_TRACE_LOW:"[MMIOCR]:Recognize OK, Classified regions= %d\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_LIB_255_112_2_18_2_42_8_15,(uint8*)"d", nCardInfoSize);
		MMIOCR_SetRecogCardInfoSizeNum(nCardInfoSize);
	}
	else
	{  		
		return OCR_ERROR;
	}

	return OCR_NONE_ERROR;
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

	
    sig_ptr = (OCR_SIGNAL_T *)SCI_ALLOC_APP(sizeof(OCR_SIGNAL_T));
    SCI_ASSERT(SCI_NULL != sig_ptr);/*assert verified*/
    
    sig = &sig_ptr->sig;
    sig->SignalSize = sizeof(OCR_SIGNAL_T);
    sig->Sender     = g_ocr_task_id;
    sig->SignalCode = sig_code;
    sig_ptr->param  = param ;
    
    SCI_SEND_SIGNAL((xSignalHeaderRec*)sig_ptr, s_ocr_request_task_id);
}


/******************************************************************************
 **	Description: 	OCR stop auto recognition
 **	Author:			Jing.Li
 **	Input:			none
 **	Output:     	none
 **	Return:     	0           successful
 **					ohter: error (refer to PUBLIC OCR_ERROR_E)
 **	Note:       	None
******************************************************************************/
PUBLIC OCR_ERROR_E OCR_AutoRecogStop(void)
{	

    if (s_ocr_reg_start)
    {
        s_ocr_reg_start = FALSE;
        if (CHNG_FREQ_REQ_HANDLER_NULL!=s_ocr_chng_freq_req_handler)
        {
            CHNG_FREQ_RestoreARMClk(s_ocr_chng_freq_req_handler);
        }
    	
    	THOCRTerm(s_ocr_config_ram_ptr);//释放最后识别图像内存
    	
        FreeOCRBuffer();        
    	
    	
    	if(SCI_SUCCESS != OCRlib_AutoRecogTaskClose())
    	{
    		return OCR_ERROR;
    	}
    }

	return OCR_NONE_ERROR;
}

/*****************************************************************************
 **	Name : 			OCRlib_AutoRecogTaskClose
 ** Description:	to close auto recognize thread
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL uint32 OCRlib_AutoRecogTaskClose(void)
{    
	 void* sig_ptr = PNULL;
	  	 
	 //SCI_TraceLow:"[MMIOCR]:ocr_autorecog_task_close,task_id = %d"
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_LIB_335_112_2_18_2_42_8_16,(uint8*)"d",g_ocr_task_id);
	 
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

/******************************************************************************
 **	Description: 	Get buffer used for OCR lib
 **	Author:			jian.ma
 **	Input:			none
 **	Output:     	buffer_size: buffer size
 **	Return:     	buffer pointer
 **	Note:       	None
******************************************************************************/
PUBLIC uint8* OCR_GetOCRBuffer(uint32* buffer_size) 
{
	SCI_ASSERT(buffer_size);/*assert verified*/
	
	if(AllocOCRBuffer())
	{
	    *buffer_size = MMIOCR_MAX_SIZE;
	    return (uint8*)s_ocr_lib_buffer;
	}
	else
	{
	    *buffer_size = 0;
	    return PNULL;
	}
}

/*****************************************************************************
 ** 	Description : free OCR buffer .
 **	Global resource dependence : 
 **  Author:jian.ma
 **	Note:		
*****************************************************************************/
LOCAL void FreeOCRBuffer( void )
{
    //SCI_TRACE_LOW:"[MMIOCR]:FreeOCRBuffer(), s_ocr_lib_buffer=0x%08lx"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_LIB_397_112_2_18_2_42_8_17,(uint8*)"d",s_ocr_lib_buffer);
    if( PNULL != s_ocr_lib_buffer )
    {
        if(NOT_USE != BL_GetStatus(BLOCK_MEM_POOL_OCR))
        {
            MMI_BL_FREE(BLOCK_MEM_POOL_OCR);
        }        
        s_ocr_lib_buffer = PNULL;
    }
}

/*****************************************************************************
 ** 	Description : Alloc OCR buffer .
 **	Global resource dependence : 
 **  Author:jian.ma
 **	Note:		
*****************************************************************************/
LOCAL BOOLEAN AllocOCRBuffer( void )
{
    if(NOT_USE == BL_GetStatus(BLOCK_MEM_POOL_OCR))
    {
         s_ocr_lib_buffer = MMI_BL_MALLOC(BLOCK_MEM_POOL_OCR);
         SCI_ASSERT(PNULL != s_ocr_lib_buffer);/*assert verified*/

         //SCI_TRACE_LOW:"[MMIOCR]:AllocOCRBuffer(), add=0x%x"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_LIB_420_112_2_18_2_42_8_18,(uint8*)"d",s_ocr_lib_buffer);
         return TRUE;
    }
    else
    {        
        return FALSE;            
    }    
}

/*****************************************************************************
 **	Name : 			OCRlib_Initialize
 ** Description:	initiate ocr lib
 **					
 ** Author:			Xiaoguang Feng
 **	Note:
 *****************************************************************************/
LOCAL OCR_ERROR_E OCRlib_Initialize(void)
{

	int32 ret			= ERR_NONE;
	int32 ocr_img_type	= 0;
	uint32 i			= 0;	
	uint32 lib_buffer_size=0;
	uint8 *ocr_buffer = PNULL;
	DCAMERA_RECT_T image_rect= {0};
	IMG_SIZE_T img_size ={0};
	
	MMIOCR_GetImageRect(&image_rect);
	img_size.height = image_rect.h;
	img_size.width = image_rect.w;
	
	
	// Get OCR lib version number
	//OCRLIB_TRACE:"[MMIOCR]:OCRlib_Initialize: OCR LIB VER %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_LIB_452_112_2_18_2_42_8_19,(uint8*)"d",GetOCRVersion());
	
	ocr_buffer = MMIOCR_GetBufData();
	
	s_ocr_buffer_ptr = (uint8 **)ocr_buffer;
	
	s_ocr_buffer_ptr[0]= (uint8 *)s_ocr_buffer_ptr + img_size.height*sizeof(uint8 *);

	for (i=1;i<img_size.height;i++)
	{
		s_ocr_buffer_ptr[i]= s_ocr_buffer_ptr[i-1]+img_size.width;
	}

	SCI_ASSERT(s_ocr_buffer_ptr != NULL);/*assert verified*/
	
	// get the static buffer for the ocr lib
	s_ocr_config_ram_ptr=(char*)OCR_GetOCRBuffer(&lib_buffer_size);
	
	SCI_ASSERT(PNULL!=s_ocr_config_ram_ptr);/*assert verified*/
	
	// Initialize TH-OCR kernal
	ret = THOCRInit(TH_EXPORTCODE_UNICODE, 0x08, NULL, s_ocr_config_ram_ptr, lib_buffer_size);

	if (ret!=ERR_NONE) 
	{
    	//SCI_TraceLow:"[MMIOCR]:ocr initialize failed, error code = %d"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_LIB_477_112_2_18_2_42_8_20,(uint8*)"d",ret);
		return OCR_ERROR;
	}

	TH_CallBackAddress((TH_CallBackFunc)OCRlib_CallBackFunc, NULL, s_ocr_config_ram_ptr);	

	ocr_img_type=1; //gray image
	
	// set ocr image buffer
	ret = TH_SetImage(s_ocr_buffer_ptr, img_size.width, img_size.height, ocr_img_type, s_ocr_config_ram_ptr);	
	
	
	if (ret!=ERR_NONE) 
	{
    	//SCI_TraceLow:"[MMIOCR]:ocr set image failed, error code = %d"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOCR_LIB_491_112_2_18_2_42_9_21,(uint8*)"d",ret);
		return OCR_ERROR;
	}

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
	return 0;
}


/******************************************************************************/
//	Description: 	OCR return recognition result
//	Author:			jian.ma
//	Input:			none
//	Output:     	none
//	Return:     	the result array pointer
//	Note:       	None
/******************************************************************************/
PUBLIC OCR_CARD_INFO* OCR_GetRecogResult(void)
{
	return s_card_info;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
