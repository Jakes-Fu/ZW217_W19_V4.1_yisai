/******************************************************************************
 ** File Name:      msensor_meye_yangxun.c                                      	  *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           10/28/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of Motion*
 **                 Sensor.                                                   *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/28/2009     Liangwen.Zhen    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 

#include "ms_customize_trc.h"
#include "sci_api.h"
#include "sensor_drv.h"
#include "sensor_cfg.h"
#include "msensor_drv.h"
#include "msensor_cfg.h"
#include "isp_service.h"
#include "msensor_meye_yangxun.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#define MEYE_DEBUG

#ifdef MEYE_DEBUG
#define  MEYE_PRINT       SCI_TRACE_LOW
#else
#define  MEYE_PRINT(_x_)
#endif


/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 _Meye_Init(void);
LOCAL uint32 _Meye_Open(void);
LOCAL uint32 _Meye_Close(void);
LOCAL uint32 _Meye_JudgeMotion(void);
LOCAL uint32 _Meye_GetVector(MSENSOR_MOTION_VECTOR_T *p_vector_info);

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define MEYE_MEM_SIZE 		(MET_IMAGE_WIDTH*MET_IMAGE_WIDTH*2+256)

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/


PUBLIC MET_CAMERA_CONTEXT met_camera_context = {0};


PUBLIC MET_CAMERA_SYSTEM met_camera_system = {
					MET_SENSITIVE_VALUE,
					MET_CAMERA_CLOSE,
					MET_CAMERA_CHBG_DEFAULT_SOUND,
					MET_SENSITIVE_VALUE,
					MET_CAMERA_OPEN,
					MET_CAMERA_CHBG_DEFAULT_SOUND,
					SCI_FALSE,
					SCI_FALSE,
					SCI_FALSE
					};

PUBLIC MSENSOR_CFG_T g_yangxun_sw001_msensor_info = 
{
    "yangxun sw001",
	MSENSOR_TYPE_ACCELEROMETER,
    MSENSOR_STATUS_NOT_INIT,
	_Meye_Init,
	_Meye_Open,
	_Meye_Close,
	_Meye_JudgeMotion,
	_Meye_GetVector
};

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL int32 s_debug_value_i		= 50;	// sensitivity parm 1
LOCAL int32 s_debug_value_ii	= 1400; // sensitivity parm 2
LOCAL int32 s_debug_value_iii	= 2;    // sensitivity parm 3

LOCAL BOOLEAN s_is_camera_open 	   = SCI_FALSE;
LOCAL BOOLEAN s_is_camera_existed  = SCI_TRUE;

#define MEYE_MEM_SIZE             300
LOCAL void*	s_meye_mem_ptr = NULL;
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to check if camera is open
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL BOOLEAN _Camera_IsOpen(void)
{
	return s_is_camera_open;
}

/*****************************************************************************/
//  Description:    This function is used to initialize camera
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL BOOLEAN  _Camera_Init(void)
{
    Sensor_SelectSensorFormat(SENSOR_IMAGE_FORMAT_YUV422);

    if(!Sensor_IsInit())
    {
        if(!Sensor_Init())
        {		
            return SCI_FALSE;
        }
    }

    if(!Sensor_IsOpen())
    {
		Sensor_Open();
       	Sensor_SetMode(SENSOR_MODE_PREVIEW_TWO);
    }
    
    return SCI_TRUE;     
	
}

/*****************************************************************************/
//  Description:    This function is used to open camera module
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL BOOLEAN  _Camera_Open(void)
{
    if(!_Camera_Init())        
	{
		//MEYE_PRINT:"GSensor_Open: Open fail !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_MEYE_YANGXUN_135_112_2_18_0_34_26_1945,(uint8*)"");

		s_is_camera_open 	= SCI_FALSE;
		s_is_camera_existed = SCI_FALSE;		
	}
	else
	{
		s_is_camera_open = SCI_TRUE;		
	}	

	return s_is_camera_open;
}

/*****************************************************************************/
//  Description:    This function is used to close camera module
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/ 
LOCAL void _Camera_Close( void )
{
	if( !_Camera_IsOpen())
	{
		return ;
	}	

    if (SENSOR_OP_SUCCESS != Sensor_Close())
    {        
		SCI_ASSERT(0);/*assert verified*/		    
    }

	s_is_camera_open = SCI_FALSE;

	//MEYE_PRINT:"GSensor_Close !!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_MEYE_YANGXUN_165_112_2_18_0_34_26_1946,(uint8*)"");

}

/*****************************************************************************/
//  Description:    This function is used to get image from camera   
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL BOOLEAN _Camera_GetImage(void)
{		
    ISP_JPEG_PARAM_T	param_ptr	  	= {0x00};
    ISP_DATA_ADDR_T 	capture_data	= {0x00};
    uint16 				capture_width	= 0x00;
    uint16 				capture_height	= 0x00;
	SENSOR_EXP_INFO_T*  img_sensor_ptr  = Sensor_GetInfo();
	
	if( !_Camera_IsOpen() )
	{

		//MEYE_PRINT:"_Camera_GetImage : Gsensor is not open !!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_MEYE_YANGXUN_183_112_2_18_0_34_26_1947,(uint8*)"");
		return SCI_FALSE;
	}

    ISP_ServiceInit( );
    
    ISP_ServiceOpen();

	// set isp paramter   
    param_ptr.input_size.w			= img_sensor_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_TWO].width;
    param_ptr.input_size.h			= img_sensor_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_TWO].height;
    param_ptr.input_rect.x 			= 0x00;
    param_ptr.input_rect.y 			= 0x00;    
    param_ptr.input_rect.w 			= img_sensor_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_TWO].width;
    param_ptr.input_rect.h 			= img_sensor_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_TWO].height;  
    param_ptr.output_size.w			= MET_IMAGE_WIDTH;
    param_ptr.output_size.h			= MET_IMAGE_HEIGHT; 
    param_ptr.capture_addr			= MSensor_GetImageAddr();
    param_ptr.capture_buf_size		= MEYE_MEM_SIZE;
    param_ptr.output_data_format	= ISP_DATA_RGB565;

    //MEYE_PRINT:"_Camera_GetImage: _GSensor_GetCaptureParam %d,%d,0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_MEYE_YANGXUN_207_112_2_18_0_34_26_1948,(uint8*)"ddd",param_ptr.input_size.w,param_ptr.input_size.h, param_ptr.capture_addr);

    ISP_ServiceSetCaptureParam(&param_ptr);

    if(SCI_SUCCESS!=ISP_ServiceStartSyncCaptureExt(&capture_data, &capture_width, &capture_height, 150))
    {
        //MEYE_PRINT:"_Camera_GetImage: Fail !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_MEYE_YANGXUN_213_112_2_18_0_34_26_1949,(uint8*)"");
        return SCI_FALSE;
    }	

	//MEYE_PRINT:"_Camera_GetImage: success."
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_MEYE_YANGXUN_217_112_2_18_0_34_26_1950,(uint8*)"");

	return SCI_TRUE;
}


/*****************************************************************************/
//  Description:    This function is used to open camera module    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _met_camera_open(void)
{
	if( !met_camera_system.open )
	{			
	    met_camera_context.ptr_buffer = (void*)MSensor_GetImageAddr();
		if(PNULL == met_camera_context.ptr_buffer)		    
	    {
	        //MEYE_PRINT:"met_cam_camera_open: MSensor_GetImageAddr is NULL"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_MEYE_YANGXUN_233_112_2_18_0_34_26_1951,(uint8*)"");
			return SCI_ERROR;
	    }

		if( !_Camera_Open( ) )
		{
			return SCI_ERROR;
		}
		
		met_camera_system.open = SCI_TRUE;
	}

	return SCI_SUCCESS;
	

}

/*****************************************************************************/
//  Description:    This function is used to close camera module    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _met_camera_close(void)
{
	//MEYE_PRINT:"_met_camera_close()"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_MEYE_YANGXUN_255_112_2_18_0_34_26_1952,(uint8*)"");

	if(met_camera_system.open)
	{
		_Camera_Close();
		met_camera_context.ptr_buffer = PNULL;
		met_camera_system.open        = SCI_FALSE;
	}

}

/*****************************************************************************/
//  Description:    This function is used to check if meye module is existed .    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _Meye_Init(void)
{
	if(s_is_camera_existed)
	{
		return 0;
	}
	else
	{
		return 0xff;
	}

}

/*****************************************************************************/
//  Description:    This function is used to open meye module    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _Meye_Open(void)
{
	if(s_meye_mem_ptr == NULL)
	{
		s_meye_mem_ptr = SCI_ALLOC_APP(MEYE_MEM_SIZE);
		if(s_meye_mem_ptr == NULL)
		{
			return SCI_ERROR;
		}
	}
	
	if(!met_camera_context.cmotion_is_run)
	{
		if(SCI_SUCCESS == _met_camera_open())
		{	
			met_camera_context.last_capp	  = MET_CAMERA_TYPE_APP_MAX;    
			met_camera_context.preview_width  = MET_IMAGE_WIDTH;
			met_camera_context.preview_height = MET_IMAGE_HEIGHT;
			
			
			MET_camera_cut_init(s_debug_value_i, s_debug_value_ii, s_debug_value_iii);

			met_camera_context.cmotion_is_run = SCI_TRUE;

		    //MEYE_PRINT:"MET_CMOTION_Init ok"
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_MEYE_YANGXUN_309_112_2_18_0_34_26_1953,(uint8*)"");

			met_camera_context.last_capp = MET_CAMERA_TYPE_APP_SHAKE_BACKGROUD;
		}
		else
		{
			return SCI_ERROR;
		}
	}

	return SCI_SUCCESS;

} 

/*****************************************************************************/
//  Description:    This function is used to close meye module 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _Meye_Close(void)
{ 
	if (met_camera_context.cmotion_is_run)
	{		
		MET_camera_cut_exit();
		_met_camera_close();
		met_camera_context.cmotion_is_run = SCI_FALSE;
	}

    //MEYE_PRINT:"met_cam_cmotion_close"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_MEYE_YANGXUN_335_112_2_18_0_34_26_1954,(uint8*)"");
	if(s_meye_mem_ptr != NULL)
	{
		SCI_FREE(s_meye_mem_ptr);
		s_meye_mem_ptr = NULL;
	}
	
	return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is used to judge motion type of meye.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _Meye_JudgeMotion(void)
{
	BOOLEAN is_shake = SCI_FALSE;
	uint32  time 	 = SCI_GetTickCount();

	if(_Camera_GetImage())
	{
		is_shake = MET_camera_judge_Shake_5( );
	}		

	//MEYE_PRINT:"met_cam_cmotion_judge_motion: \" Judge time = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_MEYE_YANGXUN_358_112_2_18_0_34_26_1955,(uint8*)"d", SCI_GetTickCount() - time);
	
	if(met_camera_context.cmotion_is_run && is_shake)
	{
		//MEYE_PRINT:"MET_CAM_CMOTION MOVED! \""
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_MEYE_YANGXUN_362_112_2_18_0_34_26_1956,(uint8*)"");
		return (uint32)MSENSOR_MOTION_TYPE_SHAKE;			
	}
	else
	{
		return (uint32)MSENSOR_MOTION_TYPE_NONE;
	}

}


/******************************************************************************/
// Description: 
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL uint32 _Meye_GetVector(MSENSOR_MOTION_VECTOR_T *p_vector_info)
{
	return MSENSOR_ERROR_NOT_SUPPORT;
}



/*****************************************************************************/
//  Description:    This function is used to alloc the memory for external module    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
void * met_cam_alloc_ext_mem(uint32 size)
{
	void * ptr = SCI_ALLOC_APP(size);
	
	memset(ptr,0,size);

	//MEYE_PRINT:"met_cam_alloc_ext_mem : ptr 0x%x, size %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_MEYE_YANGXUN_393_112_2_18_0_34_26_1957,(uint8*)"dd", ptr, size);

	return ptr;
}


/*****************************************************************************/
//  Description:    This function is used to free the memory for external module    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
void met_cam_free_ext_mem(void ** pointer)
{
	//MEYE_PRINT:"met_cam_free_ext_mem:prt 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MSENSOR_MEYE_YANGXUN_404_112_2_18_0_34_26_1958,(uint8*)"d", *pointer);
	
	SCI_FREE(*pointer);    

}



