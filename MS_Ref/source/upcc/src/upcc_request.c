/******************************************************************************
 ** File Name:    UPCC_request.c                                                *
 ** Author:       yuhua.shi                                                   *
 ** DATE:         3/23/2006                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  This file contain the protocol request for PC Camera.       *
 ******************************************************************************/
 /*****************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/23/2006     yuhua.shi       Create.                                     *
 ******************************************************************************/
#ifndef _UPCC_REQUEST_C_
    #define _UPCC_REQUEST_C_
    
/*----------------------------------------------------------------------------*
 **                         DEPENDENCIES                                      *
 **-------------------------------------------------------------------------- */    
#include "ms_ref_upcc_trc.h"
#include "upcc_request.h"
#include "upcc_camera.h"
#include "upcc_drv.h"
#include "upcc_api.h"
#include "sc_reg.h"
#include "udc_api.h"
/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/
//timer for delay the action of contrast setting
#define UPCC_REQUEST_SET_DELAY_INTERVAL 150

/**---------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                 *
 **---------------------------------------------------------------------------*/
//the delay timer for request
LOCAL SCI_TIMER_PTR s_request_set_delay_timer = PNULL;
//for record the request parameter from pc
LOCAL uint32 s_receive_request_param = 0;

//init control infomation
__align(4) UPCC_INIT_CONTROL_INFO_T g_init_control_info = 
{
    //length
    UPCC_VC_CONTROL_INIT_LEN,
    
    //brightness
    //VC_BRIGHTNESS_CONTROL,
    UPCC_BRIGHTNESS_DEF,
    UPCC_BRIGHTNESS_MIN,
    UPCC_BRIGHTNESS_MAX,
    
    //contrast
    //VC_CONTRAST_CONTROL,
    UPCC_CONTRAST_DEF,
    UPCC_CONTRAST_MIN,
    UPCC_CONTRAST_MAX,
    
    //hue
    //VC_HUE_CONTROL,
    UPCC_HUE_DEF,
    UPCC_HUE_MIN,
    UPCC_HUE_MAX,
    
    //frame rate
    //VC_FRAME_RATE_CONTROL,
    UPCC_FRAME_RATE_DEF,
    UPCC_FRAME_RATE_NUM,    
    UPCC_MJPEG_LOW_FRAME_RATE,
    UPCC_MJPEG_MIDD_FRAME_RATE,
    UPCC_MJPEG_HIGH_FRAME_RATE,   
       
    //frame size
    //VC_FRAME_SIZE_PATTERN_CONTROL,
    UPCC_FRAME_SIZE_DEF,
    UPCC_FRAME_SIZE_NUM,
    UPCC_JPEG_WIDTH_160,
    UPCC_JPEG_HEIGHT_128,
    UPCC_JPEG_WIDTH_320,
    UPCC_JPEG_HEIGHT_240
};/*lint !e785 */

/**---------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                  *
 **---------------------------------------------------------------------------*/
extern BLOCK_ID upcc_main_id;//this task used to collect garbage generated in write operation.

 
/**---------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                 *
 **---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: start the delay timer for request
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void StartRequestDelayTimer(void);

/*****************************************************************************/
//  Description: stop the delay timer for request
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void StopRequestDelayTimer(void);

/*****************************************************************************/
//  Description: judge if the delay timer for request is active
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL BOOLEAN IsRequestDelayTimerActive(void);

/*****************************************************************************/
//  Description: callback function for the delay timer of request
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void RequestDelayTimerCallback(uint32 param);

/*****************************************************************************/
//  Description: process the request to init control   
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void HandleControlInit(uint8 bRequest, uint16 wParam, uint16 wlen);
 
/*****************************************************************************/
//  Description: process the request to brightness control   
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void HandleBrightness(uint8 bRequest, uint16 wParam, uint16 wlen);

/*****************************************************************************/
//  Description: process the request to brightness control   
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void HandleContrast(uint8 bRequest, uint16 wParam, uint16 wlen);

/*****************************************************************************/
//  Description: process the request to brightness control   
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void HandleHue(uint8 bRequest, uint16 wParam, uint16 wlen);

/*****************************************************************************/
//  Description: process the request to frame size pattern control for M-JPEG  
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void HandleFrameSizePattern(uint8 bRequest, uint16 wParam, uint16 wlen);

/*****************************************************************************/
//  Description: process the request to frame rate control for M-JPEG   
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void HandleFrameRate(uint8 bRequest, uint16 wParam, uint16 wlen);

/*****************************************************************************/
//  Description: process the request to image size pattern control for JPEG  
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
//LOCAL void HandleImageSizePattern(uint8 bRequest, uint16 wParam, uint16 wlen);

/*****************************************************************************/
//  Description: process the request to video streaming control
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void HandleVideoStreamingControl(uint8 bRequest, uint16 wParam, uint16 wlen);

/*****************************************************************************/
//  Description: process the request to still image trigger control   
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
//LOCAL void HandleStillImageTrigger(uint8 bRequest, uint16 wParam, uint16 wlen);
 

/*----------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
LOCAL const UPCC_REQUEST_HANDLE_FUNC s_UPCC_Request_Func[VC_CONTROL_MAX] = 
{
    HandleControlInit,
    HandleBrightness,
    HandleContrast,
    HandleHue,
    HandleFrameRate,
    HandleFrameSizePattern,
    HandleVideoStreamingControl
};


/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: start the delay timer for request
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/						 
PUBLIC void StartRequestDelayTimer(void)
{
	if(PNULL != s_request_set_delay_timer)
	{
		SCI_ChangeTimer(s_request_set_delay_timer, RequestDelayTimerCallback, UPCC_REQUEST_SET_DELAY_INTERVAL);
		SCI_ActiveTimer(s_request_set_delay_timer);
	}
}

/*****************************************************************************/
//  Description: stop the delay timer for request
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void StopRequestDelayTimer(void)
{
	if(PNULL != s_request_set_delay_timer)
	{
		SCI_DeactiveTimer(s_request_set_delay_timer);
	}
}

/*****************************************************************************/
//  Description: judge if the delay timer for request is active
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN IsRequestDelayTimerActive(void)
{
	BOOLEAN is_timer_active = FALSE;
	
	if(PNULL != s_request_set_delay_timer)
	{
		is_timer_active = SCI_IsTimerActive(s_request_set_delay_timer);
	}
	
	return is_timer_active;
}

/*****************************************************************************/
//  Description: callback function of the delay timer for request
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void RequestDelayTimerCallback(uint32 param)
{
	xSignalHeader   signal;
	StopRequestDelayTimer();
	

	signal = SCI_ALLOC(sizeof(*signal));
	signal->SignalCode = UPCC_CMD_EXECUTE;
	signal->SignalSize = sizeof(*signal);
	signal->Sender = SCI_IdentifyThread();
	SCI_SendSignal(signal, upcc_main_id);
}

/*****************************************************************************/
//  Description: process the request to init control   
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void HandleControlInit(uint8 bRequest, uint16 wParam, uint16 wlen)
{
	uint8 length = 0;
	char *pData = 0;
    
    length  = UPCC_VC_CONTROL_INIT_LEN;
    UPCC_ASSERT(length <= USB_FIFO_MAX_BYTE); /*assert verified*/
    
    pData 	= (char *)&g_init_control_info;
    
    //send data to fifo
    UPCC_EPxSendData(USB2011_EP0_IN, (uint32*)pData, length);
}
 
 
/*****************************************************************************/
//  Description: process the request to brightness control   
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void HandleBrightness(uint8 bRequest, uint16 wParam, uint16 wlen)
{
    uint8  data_buffer[4];
    uint16 value;

    SCI_MEMSET(data_buffer, 0, sizeof(data_buffer));
    
    switch(bRequest)
    {
    case USB_VIDEO_RC_SET_CUR:
        value = wParam;
        //set brightness value
        if(!UPCC_SetBrightnessValue(value))
        {
			//SCI_TRACE_LOW:"UPCC_request.c:HandleBrightness:set cur value fail."
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_REQUEST_279_112_2_18_1_55_23_43,(uint8*)"");
        }
        break;
        
    case USB_VIDEO_RC_GET_CUR:
    case USB_VIDEO_RC_GET_MIN:   
    case USB_VIDEO_RC_GET_MAX:
    case USB_VIDEO_RC_GET_DEF:
        UPCC_ASSERT(UPCC_VC_REQUEST_PARAM_LEN == wlen); /*assert verified*/
        //get the brightness value
        UPCC_GetBrightnessValue(&value, bRequest);
       
        //data_buffer[0] = (uint8)(value & 0x00FF);
        //data_buffer[1] = (uint8)((value >> 8) & 0x00FF);
        *((uint16*)data_buffer) = value;
        
        //send data to fifo
        UPCC_EPxSendData(USB2011_EP0_IN, (uint32*)data_buffer, wlen);
        break;
 
    default:
		//SCI_TRACE_LOW:"UPCC_request.c:HandleBrightness:invalid request"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_REQUEST_300_112_2_18_1_55_23_44,(uint8*)"");
        break;
    }
}

/*****************************************************************************/
//  Description: process the request to contrast control   
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void HandleContrast(uint8 bRequest, uint16 wParam, uint16 wlen)
{
    uint8  data_buffer[4];
    uint16 value;
    
    SCI_MEMSET(data_buffer, 0, sizeof(data_buffer));
    
    switch(bRequest)
    {
    case USB_VIDEO_RC_SET_CUR:
        value = wParam;
        //set contrast value
        if(!UPCC_SetContrastValue(value))
        {
            //SCI_TRACE_LOW:"UPCC_request.c:HandleContrast:set cur value fail."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_REQUEST_323_112_2_18_1_55_23_45,(uint8*)"");
        }
        break;
        
    case USB_VIDEO_RC_GET_CUR:
    case USB_VIDEO_RC_GET_MIN:   
    case USB_VIDEO_RC_GET_MAX:
    case USB_VIDEO_RC_GET_DEF:
        UPCC_ASSERT(UPCC_VC_REQUEST_PARAM_LEN == wlen); /*assert verified*/
        //get the contrast value
        UPCC_GetContrastValue(&value, bRequest);
       
        *((uint16*)data_buffer) = value;
        
        //send data to fifo
        UPCC_EPxSendData(USB2011_EP0_IN, (uint32*)data_buffer, wlen);
        break; 
        
    default:
        //SCI_TRACE_LOW:"UPCC_request.c:HandleContrast:invalid request"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_REQUEST_342_112_2_18_1_55_23_46,(uint8*)"");
        break;
    }
}

/*****************************************************************************/
//  Description: process the request to hue control   
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void HandleHue(uint8 bRequest, uint16 wParam, uint16 wlen)
{
    uint8  data_buffer[4];
    uint16 value;
    
    SCI_MEMSET(data_buffer, 0, sizeof(data_buffer));
    
    switch(bRequest)
    {
    case USB_VIDEO_RC_SET_CUR:
        value = wParam;
        //set hue value
        if(!UPCC_SetHueValue(value))
        {
            //SCI_TRACE_LOW:"UPCC_request.c:HandleHue:set cur value fail."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_REQUEST_365_112_2_18_1_55_23_47,(uint8*)"");
        }
        break;
        
    case USB_VIDEO_RC_GET_CUR:
        UPCC_ASSERT(UPCC_VC_REQUEST_PARAM_LEN == wlen); /*assert verified*/
        //get the hue value
        UPCC_GetHueValue(&value);
       
        *((uint16*)data_buffer) = value;
        
        //send data to fifo
        UPCC_EPxSendData(USB2011_EP0_IN, (uint32*)data_buffer, wlen);
        break;
        
    default:
        //SCI_TRACE_LOW:"UPCC_request.c:HandleHue:invalid request"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_REQUEST_381_112_2_18_1_55_23_48,(uint8*)"");
        break;
    }
}

/*****************************************************************************/
//  Description: process the request to frame size pattern control for M-JPEG  
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void HandleFrameSizePattern(uint8 bRequest, uint16 wParam, uint16 wlen)
{
    uint8  data_buffer[4] = {0};
    UPCC_VIDEO_SIZE_E  value;
    
    SCI_MEMSET(data_buffer, 0, sizeof(data_buffer));
        
    switch(bRequest)
    {
    case USB_VIDEO_RC_SET_CUR:
        value = (UPCC_VIDEO_SIZE_E)wParam;
        //change the frame size pattern of M-JPEG
        UPCC_SetFrameSizePattern(value);
        break;
        
    case USB_VIDEO_RC_GET_CUR:
        UPCC_ASSERT(UPCC_VC_REQUEST_PARAM_LEN == wlen); /*assert verified*/
        //get the frame size pattern of M-JPEG
        UPCC_GetFrameSizePattern(&value);
       
        data_buffer[1] = (uint8)value;

        //send data to fifo
        UPCC_EPxSendData(USB2011_EP0_IN, (uint32*)data_buffer, wlen);
        break;
             
    default:
        //SCI_TRACE_LOW:"UPCC_request.c:HandleFrameSizePattern:invalid request"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_REQUEST_417_112_2_18_1_55_23_49,(uint8*)"");
        break;
    }
}

/*****************************************************************************/
//  Description: process the request to frame rate control for M-JPEG   
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void HandleFrameRate(uint8 bRequest, uint16 wParam, uint16 wlen)
{
    uint8  data_buffer[4] = {0};
    uint8  value;
    
    SCI_MEMSET(data_buffer, 0, sizeof(data_buffer));
        
    switch(bRequest)
    {
    case USB_VIDEO_RC_SET_CUR:
        value = wParam;
        //change the frame rate of M-JPEG
        UPCC_SetFrameRate(value);
        break;
        
    case USB_VIDEO_RC_GET_CUR:
        UPCC_ASSERT(UPCC_VC_REQUEST_PARAM_LEN == wlen); /*assert verified*/
        //get the frame rate of M-JPEG
        UPCC_GetFrameRate(&value);
       
        data_buffer[1] = value;

        //send data to fifo
        UPCC_EPxSendData(USB2011_EP0_IN, (uint32*)data_buffer, wlen);
        break;
    
    default:
        //SCI_TRACE_LOW:"UPCC_request.c:HandleFrameRate:invalid request"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_REQUEST_453_112_2_18_1_55_23_50,(uint8*)"");
        break;
    }
}

/*****************************************************************************/
//  Description: process the request to video streaming control
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void HandleVideoStreamingControl(uint8 bRequest, uint16 wParam, uint16 wlen)
{
    UPCC_VIDEO_STREAMING_CONT_E  value;
        
    UPCC_ASSERT(UPCC_VC_REQUEST_PARAM_LEN == wlen); /*assert verified*/

    value = (UPCC_VIDEO_STREAMING_CONT_E)wParam;
    //UPCC_TRACE_LOW:"------HandleVideoStreamingControl(start=%d)------"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_REQUEST_469_112_2_18_1_55_24_51,(uint8*)"d",value);
    switch(value)
    {
    case UPCC_VIDEO_STREAMING_START:
        //start the video streaming
        UPCC_StartVideoStreaming();
        UPCC_SetVideoStartFlagByHost(TRUE);
        break;
        
    case UPCC_VIDEO_STREAMING_STOP:
        //stop the video streaming
        UPCC_StopVideoStreaming();
        UPCC_SetVideoStartFlagByHost(FALSE);
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
//  Description: call the related function to handle the request  
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_HandleRequest(void)
{ 
    //call the handle function 
    //s_UPCC_Request_Func[bValue](bRequest, wData, wlen);
	UPCC_StartVideoStreaming();
	UPCC_SetVideoStartFlagByHost(TRUE);
}

/*****************************************************************************/
//  Description: init the delay timer for request
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_InitRequestDelayTimer(void)
{
	if(PNULL == s_request_set_delay_timer)
	{
		s_request_set_delay_timer = SCI_CreateTimer("upcc_request_set_delay_timer", \
							 RequestDelayTimerCallback, 0, UPCC_REQUEST_SET_DELAY_INTERVAL, FALSE);
	}
}		

/*****************************************************************************/
//  Description: deinit the delay timer for request
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_DeInitRequestDelayTimer(void)
{
	if(PNULL != s_request_set_delay_timer)
	{
		SCI_DeleteTimer(s_request_set_delay_timer);
		s_request_set_delay_timer = PNULL;
	}
}

/**---------------------------------------------------------------------------*/
#endif //_UPCC_REQUEST_C_
