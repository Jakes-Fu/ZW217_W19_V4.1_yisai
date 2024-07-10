/***************************************************************************************
** File Name:      sc6600l_isp_service.c                                      			   *
** DATE:           12/06/2008                                                		   *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         		   *
** Description:                                                                        *
****************************************************************************************

****************************************************************************************
**                         Edit History                                       		   *
**-------------------------------------------------------------------------------------*
** DATE                    DESCRIPTION                               				   *
** 12/06/2008     	       Create.													   *
****************************************************************************************/


/**------------------------------------------------------------------------------------*
 **                         Dependencies           			                           *
 **------------------------------------------------------------------------------------*/ 
#include "ms_ref_isp_service_trc.h"
#include "os_api.h"
#include "tb_comm.h"
#include "threadx_os.h"
#include "isp_service.h"
#include "isp_drv.h"
#include "ivsp_drv.h"
#include "sensor_drv.h"
#include "arm_reg.h"
#include "dal_lcd.h"
#include "sc_reg.h"
#include "sc_reg.h"
#include "iram_mgr.h"
#include "chip.h"
#include "rotation_app.h"
#include "dc_cfg.h"
#include "display.h"
#include "lcdc_app.h"
#include "ref_lcd.h"    
#include "gen_scale_coef.h"
#include "priority_system.h"
/**------------------------------------------------------------------------------------*
 **                         Compiler Flag                               		       *
 **------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif
/*lint -save -e551 */
/**------------------------------------------------------------------------------------*
 **                         ENUM define                                		       *
 **------------------------------------------------------------------------------------*/
typedef enum
{
    ISP_OP_READ=0x01,		
    ISP_OP_WRITE,
    ISP_OP_READ_WRITE,
    ISP_OP_MAX
}ISP_OP_MODE_E;

/**------------------------------------------------------------------------------------*
 **                         Macro define                                		       *
 **------------------------------------------------------------------------------------*/
#define ISP_STACK_SIZE  	(1024*4) 
#define ISP_QUEUE_NUM		50

#define ISP_BUFFER_ADDR_ALIGN		255
#define ISP_BUFFER_ALIGN_BIT		8

#define ISP_ALIGN_2     0x01
#define ISP_ALIGN_4     0x03
#define ISP_ALIGN_8     0x07
#define ISP_ALIGN_16    0x0f

#define ISP_INPUT_SCALE_ALIGN_BIT   0x02
#define ISP_INPTU_REVIEW_ALIGN      0x03
#if defined(PLATFORM_SC6600L)
#define ISP_OUTPUT_RGB_SCALE_ALIGN  0x03
#elif defined(PLATFORM_SC6800H)
#define ISP_OUTPUT_RGB_SCALE_ALIGN  0x01
#endif
#define ISP_OUTPUT_SCALE_ALIGN  0x03

#define ISP_SCALE_LEVEL     64
#define ISP_MAX_SCALE_LEVEL 256
#define ISP_MIN_SCALE_LEVEL 16
#define ISP_SCALE_2X        2
#define ISP_SCALE_4X        4

#define ISP_OXFF    0xff
#define ISP_FOUR    0x04
#define ISP_THREE   0x03
#define ISP_TWO     0x02
#define ISP_ONE     0x01
#define ISP_ZERO    0x00

#define ISP_JPEG_BANK_SIZE (1024*16)

//event
#define ISP_OPEN_EVENT                  BIT_0
#define ISP_CLOSE_EVENT                 BIT_1
#define ISP_CAPTURE_EVENT               BIT_2
#define ISP_DISPLAY_EVENT               BIT_3
#define ISP_REVIEW_EVENT                BIT_4
#define ISP_MOTION_JPEG_START_EVENT     BIT_5
#define ISP_MOTION_JPEG_PAUSE_EVENT     BIT_6
#define ISP_MOTION_JPEG_STOP_EVENT      BIT_7
#define ISP_STOP_EVENT                  BIT_8
#define ISP_SCALE_EVENT                 BIT_9
#define ISP_UPCC_EVENT                  BIT_10
#define ISP_CAPTURE_STOP_EVENT          BIT_11
#define ISP_REVIEW_STOP_EVENT           BIT_12
#define ISP_SCALE_STOP_EVENT            BIT_13
#define ISP_START_TIMEOUT_EVENT         BIT_14
#define ISP_SENSOR_EOF_EVENT            BIT_15
#define ISP_STOP_TIMEOUT_EVENT          BIT_16
#define ISP_SET_PARAM_EVENT             BIT_17

#define ISP_SHIFT_BIT1		0x01
#define ISP_SHIFT_BIT2		0x02
#define ISP_SHIFT_BIT3		0x03
#define ISP_SHIFT_BIT4		0x04
#define ISP_SHIFT_BIT5		0x05
#define ISP_SHIFT_BIT6		0x06
#define ISP_SHIFT_BIT7		0x07
#define ISP_SHIFT_BIT8		0x08

#define ISP_WAIT_TIME   0xfffffff
#define ISP_EVENT_STOP_PREVIEW_TIMEOUT   30000 //30s
#define ISP_EVENT_TIMEOUT   0x1388 //5s
#define ISP_PREVIEW_TIMEOUT   2000 //ms
#define ISP_PREVIEW_STOP_TIMEOUT   300
#define ISP_CAPTURE_TIMEOUT   3000 //ms
#define ISP_REVIEW_TIMEOUT   2000 //ms
#define ISP_SCALE_TIMEOUT   2000 //ms
#define ISP_ATV_TIMEOUT   200 //ms
#define ISP_STOP_TIMEOUT   200 //ms
#define ISP_EVENT_TIMEOUT_HANDLE   1000 // 1s

#define ISP_ZOOM_TAB_NUM        0x11
#define ISP_MAX_ZOOM_LEVEL      0x05
#define ISP_SMOOTH_ZOOM_STEP    0x04
#define ISP_ZOOM_RECT_NUM       0x04

#define ISP_MAX_SKIP_NUM   0xffffffff
#define ISP_NORMAL_SKIP_NUM   0x02

#define ISP_ATV_TIME_OUT_HANDLE_NUM   0x02

#define LCD_MAX_ID 5
#ifdef PLATFORM_SC6600L
#define OSD_ROM_CODE
#define OSD_ROM_CODE_ADDR 0x3000A690
#endif

#define ISP_SERVICE_PRIO 29                 //to adjust ISP task priority, added @201106

#define ISP_RETURN_IF_ERR(exp, ret, warning) do{if(SCI_SUCCESS!=exp) {ISP_ERR_TRACE(warning); return ret;}}while(0)

/**------------------------------------------------------------------------------------*
 **                         Sruct define                                 		       *
 **------------------------------------------------------------------------------------*/
 

/**------------------------------------------------------------------------------------*
 **                         Static varible                                		       *
 **------------------------------------------------------------------------------------*/
LOCAL BOOLEAN s_dma_flag=SCI_FALSE;
LOCAL BOOLEAN  s_is_isp_service_thread_active = SCI_FALSE;
LOCAL uint32 s_isp_service_thread_id = SCI_INVALID_BLOCK_ID;
LOCAL uint32  s_offset_addr = 0x00;
LOCAL SCI_TIMER_PTR  s_isp_timer = PNULL;
LOCAL SCI_EVENT_GROUP_PTR  s_isp_event = PNULL;
LOCAL SCI_MUTEX_PTR	s_isp_mutex_ptr = PNULL;
LOCAL SCI_MUTEX_PTR	s_isp_peek_sig_mutex_ptr = PNULL;       //a special mutex for peek singal protection

PUBLIC ISP_SERVICE_T s_isp_service_info={0x00};
LOCAL SCI_SEMAPHORE_PTR s_isp_semap_ptr	= PNULL;
LOCAL ISP_SERVICE_OWNER_E	s_isp_owner = ISP_SERVICE_OWNER_NONE;

LOCAL ISP_MPG_DISPLAY_CALLBACK s_isp_mpeg_display_callback = PNULL;

LOCAL uint32 isp_start_time=0x00;
LOCAL uint32 isp_end_time=0x00;
LOCAL uint32 isp_trace_eb=0x01;

LOCAL uint32 s_max_zoom_level=ISP_SERVICE_ZOOM_200X;
LOCAL uint16 s_isp_zoom_tab[ISP_ZOOM_TAB_NUM][4]={0x00};

LOCAL BOOLEAN s_Isp_Peek_Stop_State = 0x00;             //the flag determine that if the peeksignal  mutex  is bypassed
                                                          //FALSE: no bypass    TRUE: bypass
                                                          
/**------------------------------------------------------------------------------------*
 **                         Macro define                                		       *
 **------------------------------------------------------------------------------------*/
LOCAL uint32 _ISP_InitializeBuf(ISP_SERVICE_ID_E service);
LOCAL void _ISP_ATVTimerDActive(void);
LOCAL void _ISP_TimerDActive(void);
LOCAL uint32 _ISP_Service_GetIspPriority(void);
LOCAL uint32 _ISP_Service_SetIspPriority(uint32 task_priority);
LOCAL uint32 _ISP_Service_GetCallerPriority(void);
LOCAL void _ISP_ISR_PathFIFOOverflowedHigh(void);


LOCAL void _ISP_GetMutex(void);
LOCAL void _ISP_PutMutex(void);

LOCAL void _ISP_GetPeekSigMutex(void);
LOCAL void _ISP_PutPeekSigMutex(void);


////////////////////////////////////////////////////////////////////////
extern uint32  SCI_GetSemaphoreInfo(SCI_SEMAPHORE_PTR   sem_ptr,        //[in]  semphore ptr
                                    char*               sem_name,       //[out] semphore name
                                    uint32*             sem_count,      //[out] semphore count
                                    uint32*             susp_count);

/****************************************************************************/
/* Purpose:	get mutex														*/
/* Author:																	*/
/* Input:      																*/
/* Output:																	*/
/* Return:																	*/
/* Note:    																*/
/****************************************************************************/
LOCAL void _ISP_CreateSemaphore()
{

	if (PNULL == s_isp_semap_ptr)
	{
		s_isp_semap_ptr = SCI_CreateSemaphore("ISP_SERVICE_SEMAP", 1);
	}

	SCI_PASSERT(s_isp_semap_ptr, ("[_ISP_ExitCriticalSection] semap_ptr is NULL"));/*assert verified*/


}

/****************************************************************************/
//  Description: Get isp context
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			none
//		output:
//			none
//		return:
//			ISP_SERVICE_T_PTR: the pointer of isp context
/*****************************************************************************/
LOCAL ISP_SERVICE_T_PTR _ISP_GetContext(void)
{
    return &s_isp_service_info;
}

/****************************************************************************/
//  Description: Get isp context
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			none
//		output:
//			none
//		return:
//			ISP_SERVICE_T_PTR: the pointer of isp context
/*****************************************************************************/
LOCAL uint32 _ISP_UpdateCache(uint32 buffer_addr, uint32 size, ISP_OP_MODE_E op_mode)
{
    if(SCI_FALSE==ISP_IsSupportUpdateCache())
    {
        return SCI_SUCCESS;
    }

    if((NULL!=buffer_addr)&&(0x00<size))
    {
        DMABUFFER_DIRECTION_E direction=DMABUFFER_BIDIRECTIONAL;

        switch(op_mode)
        {
            case ISP_OP_READ:
                direction=DMABUFFER_TO_DEVICE;
                break;

            case ISP_OP_WRITE:
                direction=DMABUFFER_FROM_DEVICE;
                break;

            case ISP_OP_READ_WRITE:
                direction=DMABUFFER_BIDIRECTIONAL;
                break;

            default:
                //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_UpdateCache default %d line error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_275_112_2_18_1_43_35_16,(uint8*)"d", __LINE__);
                break;			
        }

        MMU_DmaCacheSync(buffer_addr, size, direction);
    }

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Allocate scaling coefficient buffer for MJPEG
// Global resource dependence: 
// Author: Jianping.wang
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL BOOLEAN _ISP_AllocMjpegScaleCoeff(void)
{
	ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();    

	//SCI_TRACE_ISP:"ISP_SERVICE:_ISP_AllocMjpegScaleCoeff "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_299_112_2_18_1_43_35_17,(uint8*)"");
	
	if(PNULL == pISPContext->mjpg_info.capture_h_scale_coeff_ptr)
	{
		pISPContext->mjpg_info.capture_h_scale_coeff_ptr = (uint32 *)SCI_ALLOCA(SCALER_COEF_MAX_SIZE);
		if(PNULL == pISPContext->mjpg_info.capture_h_scale_coeff_ptr)
		{
			return FALSE;
		}

		//SCI_TRACE_ISP:"ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, capture_h_scale_coeff_ptr = 0x%x "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_310_112_2_18_1_43_35_18,(uint8*)"d", pISPContext->mjpg_info.capture_h_scale_coeff_ptr);
	}
	
	if(PNULL == pISPContext->mjpg_info.capture_v_scale_coeff_ptr)
	{
		pISPContext->mjpg_info.capture_v_scale_coeff_ptr = (uint32 *)SCI_ALLOCA(SCALER_COEF_MAX_SIZE);
		if(PNULL == pISPContext->mjpg_info.capture_v_scale_coeff_ptr)
		{
			return FALSE;
		}

		//SCI_TRACE_ISP:"ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, capture_v_scale_coeff_ptr = 0x%x "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_322_112_2_18_1_43_35_19,(uint8*)"d", pISPContext->mjpg_info.capture_v_scale_coeff_ptr);
	}
	
	if(PNULL == pISPContext->mjpg_info.review_h_scale_coeff_ptr)
	{
		pISPContext->mjpg_info.review_h_scale_coeff_ptr = (uint32 *)SCI_ALLOCA(SCALER_COEF_MAX_SIZE);
		if(PNULL == pISPContext->mjpg_info.review_h_scale_coeff_ptr)
		{
			return FALSE;
		}

		//SCI_TRACE_ISP:"ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, review_h_scale_coeff_ptr = 0x%x "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_334_112_2_18_1_43_35_20,(uint8*)"d", pISPContext->mjpg_info.review_h_scale_coeff_ptr);
	}
	
	if(PNULL == pISPContext->mjpg_info.review_v_scale_coeff_ptr)
	{
		pISPContext->mjpg_info.review_v_scale_coeff_ptr = (uint32 *)SCI_ALLOCA(SCALER_COEF_MAX_SIZE);
		if(PNULL == pISPContext->mjpg_info.review_v_scale_coeff_ptr)
		{
			return FALSE;
		}

		//SCI_TRACE_ISP:"ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, review_v_scale_coeff_ptr = 0x%x "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_346_112_2_18_1_43_35_21,(uint8*)"d", pISPContext->mjpg_info.review_v_scale_coeff_ptr);
	}
	
	if(PNULL == pISPContext->mjpg_info.jpeg_h_scale_coeff_ptr)
	{
		pISPContext->mjpg_info.jpeg_h_scale_coeff_ptr = (uint32 *)SCI_ALLOCA(SCALER_COEF_MAX_SIZE);
		if(PNULL == pISPContext->mjpg_info.jpeg_h_scale_coeff_ptr)
		{
			return FALSE;
		}

		//SCI_TRACE_ISP:"ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, jpeg_h_scale_coeff_ptr = 0x%x "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_358_112_2_18_1_43_35_22,(uint8*)"d", pISPContext->mjpg_info.jpeg_h_scale_coeff_ptr);
	}
	
	if(PNULL == pISPContext->mjpg_info.jpeg_v_scale_coeff_ptr)
	{
		pISPContext->mjpg_info.jpeg_v_scale_coeff_ptr = (uint32 *)SCI_ALLOCA(SCALER_COEF_MAX_SIZE);
		if(PNULL == pISPContext->mjpg_info.jpeg_v_scale_coeff_ptr)
		{
			return FALSE;
		}

		//SCI_TRACE_ISP:"ISP_SERVICE:_ISP_AllocMjpegScaleCoeff, jpeg_v_scale_coeff_ptr = 0x%x "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_370_112_2_18_1_43_35_23,(uint8*)"d", pISPContext->mjpg_info.jpeg_v_scale_coeff_ptr);
	}

	return TRUE;
	
}

/****************************************************************************************/
// Description: Allocate scaling coefficient buffer
// Global resource dependence: 
// Author: Jianping.wang
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL BOOLEAN _ISP_AllocScaleCoeff(void)
{
	ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext(); 

	//SCI_TRACE_ISP:"ISP_SERVICE:_ISP_AllocScaleCoeff "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_391_112_2_18_1_43_35_24,(uint8*)"");
	
	if (NULL == pISPContext->scale_vcoeff_tmp_ptr)
	{
		pISPContext->scale_vcoeff_tmp_ptr = (uint32 *)SCI_ALLOCA(SCALER_COEF_MAX_SIZE);
		if (NULL == pISPContext->scale_vcoeff_tmp_ptr)
		{
			return FALSE;
		}

		//SCI_TRACE_ISP:"ISP_SERVICE:_ISP_AllocScaleCoeff, scale_vcoeff_tmp_ptr = 0x%x "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_401_112_2_18_1_43_35_25,(uint8*)"d", pISPContext->scale_vcoeff_tmp_ptr);
	}

	if (NULL == pISPContext->scale_hcoeff_tmp_ptr)
	{
		pISPContext->scale_hcoeff_tmp_ptr = (uint32 *)SCI_ALLOCA(SCALER_COEF_MAX_SIZE);
		if (NULL == pISPContext->scale_hcoeff_tmp_ptr)
		{
			return FALSE;
		}		

		//SCI_TRACE_ISP:"ISP_SERVICE:_ISP_AllocScaleCoeff, scale_hcoeff_tmp_ptr = 0x%x "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_412_112_2_18_1_43_35_26,(uint8*)"d", pISPContext->scale_hcoeff_tmp_ptr);
	}

	if (NULL == pISPContext->scale_tmp_ptr)
	{
		pISPContext->scale_tmp_size = SCALER_TEMP_BUF_SIZE;
		pISPContext->scale_tmp_ptr = (uint32 *)SCI_ALLOCA(pISPContext->scale_tmp_size);
		if (NULL == pISPContext->scale_tmp_ptr)
		{
			return FALSE;
		}		

		//SCI_TRACE_ISP:"ISP_SERVICE:_ISP_AllocScaleCoeff, scale_tmp_ptr = 0x%x "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_424_112_2_18_1_43_35_27,(uint8*)"d", pISPContext->scale_tmp_ptr);
	}	

	pISPContext->h_scale_coeff_ptr = pISPContext->scale_hcoeff_tmp_ptr;
	pISPContext->v_scale_coeff_ptr = pISPContext->scale_vcoeff_tmp_ptr;

	//SCI_TRACE_ISP:"ISP_SERVICE:_ISP_AllocScaleCoeff , h_ptr = 0x%x, v_ptr = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_431_112_2_18_1_43_35_28,(uint8*)"dd",pISPContext->h_scale_coeff_ptr, pISPContext->v_scale_coeff_ptr);

	if (!_ISP_AllocMjpegScaleCoeff())
	{
		return FALSE;
	}

	return TRUE;
}

/****************************************************************************************/
// Description: Free all scaling coefficient buffer
// Global resource dependence: 
// Author: Jianping.wang
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL BOOLEAN _ISP_FreeScaleCoeff(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();  

	//SCI_TRACE_ISP:"ISP_SERVICE:_ISP_FreeScaleCoeff "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_455_112_2_18_1_43_35_29,(uint8*)"");

	if(PNULL != pISPContext->mjpg_info.capture_h_scale_coeff_ptr)
	{
		SCI_FREE(pISPContext->mjpg_info.capture_h_scale_coeff_ptr);
		pISPContext->mjpg_info.capture_h_scale_coeff_ptr = PNULL;
	}
	if(PNULL != pISPContext->mjpg_info.capture_v_scale_coeff_ptr)
	{
		SCI_FREE(pISPContext->mjpg_info.capture_v_scale_coeff_ptr);
		pISPContext->mjpg_info.capture_v_scale_coeff_ptr = PNULL;
	}
	if(PNULL != pISPContext->mjpg_info.review_h_scale_coeff_ptr)
	{
		SCI_FREE(pISPContext->mjpg_info.review_h_scale_coeff_ptr);
		pISPContext->mjpg_info.review_h_scale_coeff_ptr = PNULL;
	}
	if(PNULL != pISPContext->mjpg_info.review_v_scale_coeff_ptr)
	{
		SCI_FREE(pISPContext->mjpg_info.review_v_scale_coeff_ptr );
		pISPContext->mjpg_info.review_v_scale_coeff_ptr = PNULL;
	}
	if(PNULL != pISPContext->mjpg_info.jpeg_h_scale_coeff_ptr)
	{
		SCI_FREE(pISPContext->mjpg_info.jpeg_h_scale_coeff_ptr );
		pISPContext->mjpg_info.capture_h_scale_coeff_ptr = PNULL;
	}
	if(PNULL != pISPContext->mjpg_info.jpeg_v_scale_coeff_ptr)
	{
		SCI_FREE(pISPContext->mjpg_info.jpeg_v_scale_coeff_ptr);
		pISPContext->mjpg_info.jpeg_v_scale_coeff_ptr = PNULL;
	}

	if(PNULL !=pISPContext->scale_vcoeff_tmp_ptr )
	{
		SCI_FREE(pISPContext->scale_vcoeff_tmp_ptr );
		pISPContext->scale_vcoeff_tmp_ptr = PNULL;
	}

	if(PNULL != pISPContext->scale_hcoeff_tmp_ptr)
	{
		SCI_FREE(pISPContext->scale_hcoeff_tmp_ptr);
		pISPContext->scale_hcoeff_tmp_ptr = PNULL;
	}

	if(PNULL != pISPContext->scale_tmp_ptr )
	{
		SCI_FREE(pISPContext->scale_tmp_ptr );
		pISPContext->scale_tmp_ptr = PNULL;
	}

	pISPContext->h_scale_coeff_ptr = NULL;
	pISPContext->v_scale_coeff_ptr = NULL;
	
	return TRUE;
	
}

/****************************************************************************/
//  Description: Get isp context
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			none
//		output:
//			none
//		return:
//			ISP_SERVICE_T_PTR: the pointer of isp context
/*****************************************************************************/
LOCAL uint32 _ISP_GetImgSize(ISP_DATA_LEN_T* data_len, uint16 width, uint16 height, ISP_DATA_FORMAT_E data_format)
{
    data_len->y_len=0x00;
    data_len->uv_len=0x00;
    data_len->v_len=0x00;

    switch(data_format)
    {
        case ISP_DATA_YUV422:
            data_len->y_len=width*height;
            data_len->uv_len=width*height;
            break;

        case ISP_DATA_YUV420:
            data_len->y_len=width*height;
            data_len->uv_len=(width*height)>>ISP_SHIFT_BIT1;
            break;

        case ISP_DATA_YUV400:
            data_len->y_len=width*height;
            break;
            
        case ISP_DATA_YUV420_3FRAME:
            data_len->y_len=width*height;
            data_len->uv_len=(width*height)>>ISP_SHIFT_BIT1;
            data_len->v_len=(width*height)>>ISP_SHIFT_BIT1;
            break;
            
        case ISP_DATA_RGB565:
            data_len->y_len=(width*height)<<ISP_SHIFT_BIT1;
            break;
            
        case ISP_DATA_RGB666:
            data_len->y_len=width*height;
            break;
        case ISP_DATA_RGB888:
            data_len->y_len=(width*height)<<ISP_SHIFT_BIT2;
            break;

        default:
            //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_GetImgSize default %d line error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_563_112_2_18_1_43_36_30,(uint8*)"d", __LINE__);
            break;			
    }

    return SCI_SUCCESS;
}

/****************************************************************************/
//  Description: Get isp context
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			none
//		output:
//			none
//		return:
//			ISP_SERVICE_T_PTR: the pointer of isp context
/*****************************************************************************/
LOCAL uint32 _ISP_SetUpdateCacheSize(uint16 width, uint16 height, ISP_DATA_FORMAT_E data_format)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    
    _ISP_GetImgSize(&pISPContext->update_cache, width, height, data_format);

    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description: Get the Lcdc data format
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			fromat: isp data format
//		output:
//			none
//		return:
//			data_format: lcdc data format
/*****************************************************************************/
LOCAL LCD_DATA_FORMAT_E _ISP_GetLcdcDataFormat(ISP_DATA_FORMAT_E format)
{
    LCD_DATA_FORMAT_E data_format=LCD_MAX;
    //0:RGB888; 1:RGB666; 2:RGB565(RGB555 if OSD);3:RGB555;4:GREY,5:YUV422;6:YUV420;7:YUV400
    switch(format)
    {
        case ISP_DATA_YUV422:
            data_format=0x05;
            break;

        case ISP_DATA_YUV420:
            data_format=0x06;
            break;

        case ISP_DATA_YUV400:
            data_format=0x07;
            break;                 

        case ISP_DATA_RGB888:
            data_format=0x00;
            break;

        case ISP_DATA_RGB666:
            data_format=0x01;
            break; 

        case ISP_DATA_RGB565:
            data_format=0x02;
            break;  

        default :
            break;
    }

    return data_format;

}

/*****************************************************************************/
//  Description: set Dcam clk
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			pclk: img sensor output pclk
//		output:
//			none
//		return:
//			SCI_SUCCESS: success
//			other: not support the pclk
/*****************************************************************************/
LOCAL uint32 _ISP_SetDcamClk(uint32 pclk)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint32 rtn=SCI_SUCCESS;

    if(0x00==pclk)
    {
        pISPContext->dcam_clk=IVSP_CLK_48M;
    }
    else if(0x0c>pclk)
    {
        pISPContext->dcam_clk=IVSP_CLK_48M;
        rtn=SCI_ERROR;
    }
    else if(0x20>=pclk)
    {
        pISPContext->dcam_clk=IVSP_CLK_48M;
    }
    else if(0x30>=pclk)
    {
        pISPContext->dcam_clk=IVSP_CLK_64M;
    }
    else if(0x60>=pclk)
    {
        pISPContext->dcam_clk=IVSP_CLK_96M;
    }
    else
    {
        pISPContext->dcam_clk=IVSP_CLK_48M;
        rtn=SCI_ERROR;
    }

    return rtn;
}

/*****************************************************************************/
//  Description: Get dma rotation data format
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			format: isp data format
//		output:
//			none
//		return:
//			data_format: rotation data format
/*****************************************************************************/
LOCAL ROATION_DATA_FORMAT_E _ISP_GetRotationDataFormat(ISP_DATA_FORMAT_E format)
{
    ROATION_DATA_FORMAT_E data_format=ROTATION_MAX;
    
    switch(format)
    {
        case ISP_DATA_YUV422:
            data_format=ROTATION_YUV422;
            break;

        case ISP_DATA_YUV420:
            data_format=ROTATION_YUV420;
            break;

        case ISP_DATA_YUV400:
            data_format=ROTATION_YUV400;
            break;                 

        case ISP_DATA_RGB888:
            data_format=ROTATION_RGB888;
            break;

        case ISP_DATA_RGB666:
            data_format=ROTATION_RGB666;
            break; 

        case ISP_DATA_RGB565:
            data_format=ROTATION_RGB565;
            break;  

        default :
            break;
    }

    return data_format;

}

/*****************************************************************************/
//  Description: Get rotation mode
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			rotation: isp rotation mode
//		output:
//			none
//		return:
//			rotation_mode: rotation mode
/*****************************************************************************/
LOCAL ROTATION_MODE_E _ISP_GetRotationMode(ISP_ROTATION_E rotation)
{
    ROTATION_MODE_E rotation_mode=ROTATION_DIR_MAX;
    switch(rotation)
    {
        case ISP_ROTATION_90:
            rotation_mode=ROTATION_90;
            break;

        case ISP_ROTATION_180:
            rotation_mode=ROTATION_180;
            break;

        case ISP_ROTATION_270:
            rotation_mode=ROTATION_270;
            break;           
            
        default :
            break;
    }

    return rotation_mode;

}

/*****************************************************************************/
//  Description: get dma rotation patameter
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			src_buf: srouce image buf pointer
//			src_width: srouce image width
//			src_height: srouce image height
//			rotation: rotation
//		output:
//			param_ptr: rotation parameter pointer
//		return:
//			none
/*****************************************************************************/
void _ISP_GetRotationParam(ISP_DATA_ADDR_T* src_buf, 
                                    uint16 img_width, 
                                    uint16 img_height, 
                                    ISP_ROTATION_E rotation, 
                                    ROTATION_PARAM_T* param_ptr, 
                                    ISP_DATA_FORMAT_E rot_format)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
   
    param_ptr->img_size.w=img_width;
    param_ptr->img_size.h=img_height; 

    param_ptr->data_format=_ISP_GetRotationDataFormat(rot_format);
    param_ptr->rotation_mode=_ISP_GetRotationMode(rotation);

    param_ptr->src_addr.y_addr=src_buf->y_addr;
    param_ptr->src_addr.uv_addr=src_buf->uv_addr;    
    param_ptr->src_addr.v_addr=src_buf->v_addr;
    
    param_ptr->dst_addr.y_addr=pISPContext->rotation_buf[pISPContext->rotation_buf_id].y_addr;
    param_ptr->dst_addr.uv_addr=pISPContext->rotation_buf[pISPContext->rotation_buf_id].uv_addr;    
    param_ptr->dst_addr.v_addr= pISPContext->rotation_buf[pISPContext->rotation_buf_id].v_addr;
    
    pISPContext->rotation_buf_id++;
    
    if(pISPContext->rotation_loop_buf_num<pISPContext->rotation_buf_id)
    {
        pISPContext->rotation_buf_id=ISP_ROTATION_BUF_0;
    }

}

/*****************************************************************************/
//  Description: Get preview display buffer
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			src_buf_ptr: srouce image buf pointer
//			src_width: srouce image width
//			src_height: srouce image height
//			rotation: rotation
//		output:
//			buf_ptr: display buf pointer
//		return:
//			none
/*****************************************************************************/
LOCAL void _ISP_GetDisplayBuf(ISP_DATA_ADDR_T* src_buf_ptr, 
                                uint16 src_width, 
                                uint16 src_height, 
                                ISP_ROTATION_E rotation, 
                                LCD_DATA_ADDR_T* buf_ptr,
                                ISP_DATA_FORMAT_E disp_format)
{
    ROTATION_PARAM_T rotation_param={0x00};
    ROTATION_PARAM_T_PTR rotation_param_ptr=NULL;
    uint32 data_len=0x00;
    
    rotation_param_ptr=&rotation_param;
    
    _ISP_GetRotationParam(src_buf_ptr, src_width, src_height, rotation, rotation_param_ptr, disp_format);
        
    if(ISP_ROTATION_0==rotation)
    {
        data_len=src_width*src_height;
        
        switch(disp_format)
        {
            case ISP_DATA_YUV422:
                SCI_MEMCPY((void *)rotation_param_ptr->dst_addr.y_addr, (void *)rotation_param_ptr->src_addr.y_addr, data_len);
                SCI_MEMCPY((void *)rotation_param_ptr->dst_addr.uv_addr, (void *)rotation_param_ptr->src_addr.uv_addr, data_len);
                break;

            case ISP_DATA_YUV420:
                SCI_MEMCPY((void *)rotation_param_ptr->dst_addr.y_addr, (void *)rotation_param_ptr->src_addr.y_addr, data_len);
                SCI_MEMCPY((void *)rotation_param_ptr->dst_addr.uv_addr, (void *)rotation_param_ptr->src_addr.uv_addr, (data_len>>ISP_SHIFT_BIT1));
                break;

            case ISP_DATA_YUV400:
                SCI_MEMCPY((void *)rotation_param_ptr->dst_addr.y_addr, (void *)rotation_param_ptr->src_addr.y_addr, data_len);
                break;                 

            case ISP_DATA_RGB888:
                SCI_MEMCPY((void *)rotation_param_ptr->dst_addr.y_addr, (void *)rotation_param_ptr->src_addr.y_addr, (data_len<<ISP_SHIFT_BIT2));
                break;

            case ISP_DATA_RGB666:
            case ISP_DATA_RGB565:
                SCI_MEMCPY((void *)rotation_param_ptr->dst_addr.y_addr, (void *)rotation_param_ptr->src_addr.y_addr, (data_len<<ISP_SHIFT_BIT1));
                break; 
                
            default :
                break;
        }
 
    }
    else
    {
        Rotation_AppStart(rotation_param_ptr);
    }
    
    buf_ptr->y_addr=rotation_param_ptr->dst_addr.y_addr;
    buf_ptr->uv_addr=rotation_param_ptr->dst_addr.uv_addr;   
    
}

/*****************************************************************************/
//  Description: Get preview display buffer
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			src_buf_ptr: srouce image buf pointer
//			src_width: srouce image width
//			src_height: srouce image height
//			rotation: rotation
//		output:
//			buf_ptr: display buf pointer
//		return:
//			none
/*****************************************************************************/
LOCAL void _ISP_Rotation(ISP_DATA_ADDR_T* src_buf_ptr, 
                                uint16 src_width, 
                                uint16 src_height, 
                                ISP_ROTATION_E rotation, 
                                ISP_DATA_ADDR_T* buf_ptr, 
                                ISP_DATA_FORMAT_E rot_format)
{
    #if (defined(PLATFORM_SC6600L))
    buf_ptr->y_addr=src_buf_ptr->y_addr;
    buf_ptr->uv_addr=src_buf_ptr->uv_addr; 
    buf_ptr->v_addr=src_buf_ptr->v_addr;    
    #elif (defined(PLATFORM_SC6800H))
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    ROTATION_PARAM_T rotation_param={0x00};
    ROTATION_PARAM_T_PTR rotation_param_ptr=NULL;
    uint32 data_len=0x00;

    rotation_param_ptr=&rotation_param;
    
    _ISP_GetRotationParam(src_buf_ptr, src_width, src_height, rotation, rotation_param_ptr, rot_format);


    if(ISP_ROTATION_0==rotation)
    {
        if(pISPContext->is_copy_display_data)
        {
            data_len=src_width*src_height;
		
            switch(rot_format)
            {
                case ISP_DATA_YUV422:
                    SCI_MEMCPY((void *)rotation_param_ptr->dst_addr.y_addr, (void *)rotation_param_ptr->src_addr.y_addr, data_len);
                    SCI_MEMCPY((void *)rotation_param_ptr->dst_addr.uv_addr, (void *)rotation_param_ptr->src_addr.uv_addr, data_len);
                    break;

                case ISP_DATA_YUV420:
                    SCI_MEMCPY((void *)rotation_param_ptr->dst_addr.y_addr, (void *)rotation_param_ptr->src_addr.y_addr, data_len);
                    SCI_MEMCPY((void *)rotation_param_ptr->dst_addr.uv_addr, (void *)rotation_param_ptr->src_addr.uv_addr, (data_len>>ISP_SHIFT_BIT1));
                    break;

                case ISP_DATA_YUV400:
                    SCI_MEMCPY((void *)rotation_param_ptr->dst_addr.y_addr, (void *)rotation_param_ptr->src_addr.y_addr, data_len);
                    break;                 

                case ISP_DATA_RGB888:
                    SCI_MEMCPY((void *)rotation_param_ptr->dst_addr.y_addr, (void *)rotation_param_ptr->src_addr.y_addr, (data_len<<ISP_SHIFT_BIT2));
                    break;

                case ISP_DATA_RGB666:
                case ISP_DATA_RGB565:
                    SCI_MEMCPY((void *)rotation_param_ptr->dst_addr.y_addr, (void *)rotation_param_ptr->src_addr.y_addr, (data_len<<ISP_SHIFT_BIT1));
                    break; 
                    
                default :
                    break;
            }
        }
        else
        {
            rotation_param_ptr->dst_addr.y_addr=src_buf_ptr->y_addr;
            rotation_param_ptr->dst_addr.uv_addr=src_buf_ptr->uv_addr;
            rotation_param_ptr->dst_addr.v_addr=src_buf_ptr->v_addr;
        }
    }
    else
    {
        Rotation_AppStart(rotation_param_ptr);
    }
    
    buf_ptr->y_addr=rotation_param_ptr->dst_addr.y_addr;
    buf_ptr->uv_addr=rotation_param_ptr->dst_addr.uv_addr; 
    buf_ptr->v_addr=rotation_param_ptr->dst_addr.v_addr;  
    #endif    
}

/*****************************************************************************/
//  Description: isp display image
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			Lcd_id: lcd id
//                target_x: display h start point
//                target_y: display v start point
//                target_width: display target_width
//                target_height: display target_height
//                disp_buf: display src buf addr
//                rot: rotation 
//		output:
//			none
//		return:
//			none
/*****************************************************************************/

LOCAL void _ISP_InvalidateRect_Image(
                                   LCD_ID_E lcd_id,
                                   uint16 disp_x,
                                   uint16 disp_y,
                                   uint16 disp_width,
                                   uint16 disp_height,
                                   uint16 target_x,
                                   uint16 target_y,
                                   uint16 target_width,
                                   uint16 target_height,
                                   ISP_DATA_ADDR_T disp_buf,
                                   ISP_ROTATION_E rot,
                                   ISP_DATA_FORMAT_E disp_format)
{
    #if (defined(PLATFORM_SC6600L))
    BLOCKCFG_T cfg = {0};
    BLOCKCFG_T rst_cfg = {0};    
    LCD_BRUSH_MODE_E lcd_brush_mode=LCD_BRUSH_MODE_ASYNC;
    uint16 temp = 0;
    uint32 *rst_lcd_buf=LCD_GetBlockBuffer(lcd_id, 0);
    
    switch(rot)
    {
        case ISP_ROTATION_0:
            //invalidate lcd
            break;            

        case ISP_ROTATION_90:
            LCD_SetRotateMode(LCD_ROT_MODE_DMA);
            LCD_SetDirection(lcd_id, LCD_DIRECT_ROT_90);
            temp = target_width;
            target_width = target_height;
            target_height = temp;		
            temp = target_x;
            target_x = target_y;
            target_y = temp;
            break;

        case ISP_ROTATION_180:
            LCD_SetRotateMode(LCD_ROT_MODE_LCD);
            LCD_SetDirection(lcd_id, LCD_DIRECT_ROT_180);
            break;

        case ISP_ROTATION_270: 
            LCD_SetRotateMode(LCD_ROT_MODE_DMA);
            LCD_SetDirection(lcd_id, LCD_DIRECT_ROT_270);
            temp = target_width;
            target_width = target_height;
            target_height = temp;		
            temp = target_x;
            target_x = target_y;
            target_y = temp;
            break;

        default:
            //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_InvalidateRect_Image default %d line error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_1043_112_2_18_1_43_37_31,(uint8*)"d", __LINE__);
            break;
    }

    LCD_GetConfigBlock(lcd_id, 0, &rst_cfg);
    LCD_GetConfigBlock(lcd_id, 0, &cfg);
    lcd_brush_mode=LCD_GetBrushMode(lcd_id);

    cfg.start_x=target_x;
    cfg.start_y=target_y;
    cfg.end_x=target_x+target_width-1;
    cfg.end_y=target_y+target_height-1;
    cfg.width=target_width;
   
    LCD_SetBlockBuffer(lcd_id, 0, (uint32*)disp_buf.y_addr);
    LCD_ConfigBlock(lcd_id , 0, cfg);
    
    if(ISP_ServiceGetAtvIsEnable())
    {
        LCD_SetBrushMode(lcd_id, LCD_BRUSH_MODE_SYNC);    
    }

    LCD_InvalidateRect(lcd_id, target_x, target_y, target_x+target_width-1, target_y+target_height-1);
    
    if(ISP_ServiceGetAtvIsEnable())
    {
        LCD_SetBrushMode(lcd_id, lcd_brush_mode);   
    }
    LCD_ConfigBlock(lcd_id , 0, rst_cfg);
    LCD_SetBlockBuffer(lcd_id, 0, (uint32*)rst_lcd_buf);
    #endif


    #if (defined(PLATFORM_SC6800H))
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    BLOCKCFG_T lcdc_block_cfg={0x00};
    LCD_INFO_T lcd_info={0x00};
    ISP_DATA_ADDR_T rot_scr_buf={0x00};
    ISP_DATA_ADDR_T rot_dst_buf={0x00};

    LCD_BRUSH_MODE_E brush_mode = LCD_BRUSH_MODE_ASYNC;

    if(ISP_SERVICE_INVALIDATE_SYNC == pISPContext->invalidate_mode)
    {
        brush_mode = LCD_BRUSH_MODE_SYNC;
    }

    LCD_GetLcdCtrlSemaphore(lcd_id);  

    // rotation
    rot_scr_buf.y_addr=disp_buf.y_addr;
    rot_scr_buf.uv_addr=disp_buf.uv_addr;    
    _ISP_Rotation(&rot_scr_buf, target_width, target_height, rot, &rot_dst_buf, disp_format);
    
     //invalidate lcd 
    LCD_GetInfo(lcd_id, &lcd_info);
     
    LCD_SetBrushMode(lcd_id, brush_mode);

    LCD_GetConfigBlock(lcd_id, 0, &lcdc_block_cfg);

    target_x = (target_x + 1)/2 * 2;  

    switch(rot)
    {
        case ISP_ROTATION_0:
        case ISP_ROTATION_180:
            lcdc_block_cfg.start_x=target_x;
            lcdc_block_cfg.start_y=target_y;
            lcdc_block_cfg.end_x=target_x+target_width-1;
            lcdc_block_cfg.end_y=target_y+target_height-1;
            break;

        case ISP_ROTATION_90:
        case ISP_ROTATION_270:
            lcdc_block_cfg.start_x=target_x;
            lcdc_block_cfg.start_y=target_y;
            lcdc_block_cfg.end_x=target_x+target_height-1;
            lcdc_block_cfg.end_y=target_y+target_width-1;
            break;

        default:
            //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_InvalidateRect_Image default %d line error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_1125_112_2_18_1_43_37_32,(uint8*)"d", __LINE__);
            break;
    }

    lcdc_block_cfg.resolution=_ISP_GetLcdcDataFormat(disp_format);
    lcdc_block_cfg.colorkey_en=SCI_FALSE;
    lcdc_block_cfg.width=lcdc_block_cfg.end_x-lcdc_block_cfg.start_x+0x01;
    lcdc_block_cfg.endian_switch=SCI_FALSE;
    lcdc_block_cfg.alpha=0xff;

    LCDC_AppSetBGColor(0x0000);
    LCD_ConfigBlock(lcd_id, 0, lcdc_block_cfg);
    LCD_SetBlockBuffer(lcd_id, 0, (uint32*)rot_dst_buf.y_addr);
    LCD_SetUVBuffer(lcd_id, 0, (uint32*)rot_dst_buf.uv_addr);
    LCD_EnableBlock(lcd_id, 0);
    LCD_InvalidateRect(lcd_id, 
                        disp_x, 
                        disp_y, 
                        disp_x+disp_width-1, 
                        disp_y+disp_height-1);
    #endif
    
    return ;
}

/*****************************************************************************/
//  Description: isp display image using call back function
//	Global resource dependence:
//  Author: Xi.Zhang
//	Note:
//		input:
//                target_x: display h start point
//                target_y: display v start point
//                target_width: display target_width
//                target_height: display target_height
//                disp_buf: display src buf addr
//                rot: rotation 
//                display format: display format
//		output:
//			none
//		return:
//			none
/*****************************************************************************/
LOCAL void _ISP_DispImgCallback(
                                   uint16 target_x,
                                   uint16 target_y,
                                   uint16 target_width,
                                   uint16 target_height,
                                   ISP_DATA_ADDR_T disp_buf,
                                   ISP_ROTATION_E rot,
                                   ISP_DATA_FORMAT_E disp_format)
{
    BLOCKCFG_T lcdc_block_cfg={0x00};
    ISP_DATA_ADDR_T rot_scr_buf={0x00};
    ISP_DATA_ADDR_T rot_dst_buf={0x00};
    ISP_MPEG_DISPLAY_DATA_T callback_param = {0};

    //SCI_TRACE_LOW:"_ISP_DispImgCallback"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_1180_112_2_18_1_43_37_33,(uint8*)"");


    // rotation
    rot_scr_buf.y_addr=disp_buf.y_addr;
    rot_scr_buf.uv_addr=disp_buf.uv_addr;    
    _ISP_Rotation(&rot_scr_buf, target_width, target_height, rot, &rot_dst_buf, disp_format);
    
    target_x = (target_x + 1)/2 * 2;  

    switch(rot)
    {
        case ISP_ROTATION_0:
        case ISP_ROTATION_180:
            lcdc_block_cfg.start_x=target_x;
            lcdc_block_cfg.start_y=target_y;
            lcdc_block_cfg.end_x=target_x+target_width-1;
            lcdc_block_cfg.end_y=target_y+target_height-1;
            break;

        case ISP_ROTATION_90:
        case ISP_ROTATION_270:
            lcdc_block_cfg.start_x=target_x;
            lcdc_block_cfg.start_y=target_y;
            lcdc_block_cfg.end_x=target_x+target_height-1;
            lcdc_block_cfg.end_y=target_y+target_width-1;
            break;

        default:
            //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_DispImgUseCallback rotation angel error at line %d!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_1209_112_2_18_1_43_37_34,(uint8*)"d", __LINE__);
            break;
    }

    callback_param.disp_w = lcdc_block_cfg.end_x - lcdc_block_cfg.start_x  + 1;
    callback_param.disp_h = lcdc_block_cfg.end_y - lcdc_block_cfg.start_y + 1;
    callback_param.target_start_x = 0;
    callback_param.target_start_y = 0;
    callback_param.target_end_x = callback_param.disp_w - 1;
    callback_param.target_end_y = callback_param.disp_h - 1;
    callback_param.disp_buf = rot_dst_buf;
    _ISP_GetMutex();
    if(PNULL != s_isp_mpeg_display_callback)
    {
        s_isp_mpeg_display_callback(SCI_SUCCESS,                \
                                (void*)&callback_param,         \
                                (uint32)(callback_param.disp_w * callback_param.disp_h));
    }
    _ISP_PutMutex();

    return ;
}

/*****************************************************************************/
//  Description: isp display image
//	Global resource dependence:
//  Author: Xi.zhang
//	Note:
//		input:
//		output:
//			none
//		return:
//			none
/*****************************************************************************/
/*
LOCAL void _ISP_test_Lcd_Brush_Image(
                                    uint16 ret_value, 
                                    void* param_ptr, 
                                    uint32 param_size)
{

    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    BLOCKCFG_T lcdc_block_cfg={0x00};
    LCD_INFO_T lcd_info={0x00};
    ISP_MPEG_DISPLAY_DATA_T callback_param = *(ISP_MPEG_DISPLAY_DATA_T *)param_ptr;

    ISP_DATA_ADDR_T disp_buf = callback_param.disp_buf;
    uint16 target_x = callback_param.target_start_x;
    uint16 target_y = callback_param.target_start_y;
    uint16 target_width = callback_param.target_end_x - callback_param.target_start_x + 1;
    uint16 target_height = callback_param.target_end_y - callback_param.target_start_y+ 1;
    LCD_ID_E lcd_id = pISPContext->lcd_id;
    ISP_DATA_FORMAT_E disp_format = pISPContext->disp_format;

    
    LCD_BRUSH_MODE_E brush_mode = LCD_BRUSH_MODE_ASYNC;

//    SCI_ASSERT(0);

    if(ISP_SERVICE_INVALIDATE_SYNC == pISPContext->invalidate_mode)
    {
        brush_mode = LCD_BRUSH_MODE_SYNC;
    }

    LCD_GetLcdCtrlSemaphore(lcd_id);  

     //invalidate lcd 
    LCD_GetInfo(lcd_id, &lcd_info);
     
    LCD_SetBrushMode(lcd_id, brush_mode);

    LCD_GetConfigBlock(lcd_id, 0, &lcdc_block_cfg);

    lcdc_block_cfg.start_x=target_x;
    lcdc_block_cfg.start_y=target_y;
    lcdc_block_cfg.end_x=target_x+target_width-1;
    lcdc_block_cfg.end_y=target_y+target_height-1;

    lcdc_block_cfg.resolution=_ISP_GetLcdcDataFormat(disp_format);
    lcdc_block_cfg.colorkey_en=SCI_FALSE;
    lcdc_block_cfg.width=lcdc_block_cfg.end_x-lcdc_block_cfg.start_x+0x01;
    lcdc_block_cfg.endian_switch=SCI_FALSE;
    lcdc_block_cfg.alpha=0xff;

    LCDC_AppSetBGColor(0x0000);
    LCD_ConfigBlock(lcd_id, 0, lcdc_block_cfg);
    LCD_SetBlockBuffer(lcd_id, 0, (uint32*)disp_buf.y_addr);
    LCD_SetUVBuffer(lcd_id, 0, (uint32*)disp_buf.uv_addr);
    LCD_EnableBlock(lcd_id, 0);
    LCD_InvalidateRect(lcd_id, 
                        pISPContext->disp_rect.x, 
                        pISPContext->disp_rect.y, 
                        pISPContext->disp_rect.x+pISPContext->disp_rect.w-1, 
                        pISPContext->disp_rect.y+pISPContext->disp_rect.h-1);
    
    return ;
}
*/

/*****************************************************************************/
//  Description: Create Mutex
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			none
//		output:
//			none
//		return:
//			Mutex
/*****************************************************************************/
LOCAL void _ISP_CreateMutex(void)
{	
	s_isp_mutex_ptr = SCI_CreateMutex("ISP_SERVICE_SYNC_MUTEX", SCI_INHERIT);
	SCI_PASSERT((s_isp_mutex_ptr!=PNULL),("ISP_SERVICE Creat MUTEX fail!"));/*assert verified*/	
	
}//end of IMG_CreateMutex

/*****************************************************************************/
//  Description: Get Mutex
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			sm    -  mutex
//		output:
//			none
//		return:
//			none
/*****************************************************************************/
LOCAL void _ISP_GetMutex(void)
{
    uint32 ret;
    if(PNULL==s_isp_mutex_ptr)
    {
        _ISP_CreateMutex();
    }

    ret = SCI_GetMutex(s_isp_mutex_ptr, SCI_WAIT_FOREVER);
    SCI_ASSERT(ret==SCI_SUCCESS);/*assert verified*/
}
/*****************************************************************************/
//  Description: Put mutex
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		input:
//			sm    -  mutex
//		output:
//			none
//		return:
//			none
/*****************************************************************************/
LOCAL void _ISP_PutMutex(void)
{
    uint32 ret;    
    if(PNULL==s_isp_mutex_ptr)
    {
        //SCI_TRACE_ISP:"NULL isp mutex, direct return"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_1359_112_2_18_1_43_37_35,(uint8*)"");
        return;
    }
    
    ret = SCI_PutMutex( s_isp_mutex_ptr );	
    SCI_ASSERT(ret==SCI_SUCCESS);/*assert verified*/
}

/*****************************************************************************/
//  Description: Create Mutex
//	Global resource dependence:
//  Author: XZ
//	Note:
//		input:
//			none
//		output:
//			none
//		return:
//			Mutex
/*****************************************************************************/
LOCAL void _ISP_CreatePeekSigMutex(void)
{	
	s_isp_peek_sig_mutex_ptr = SCI_CreateMutex("ISP_SERVICE_PEEK_SIG_MUTEX", SCI_INHERIT);
	SCI_PASSERT((s_isp_peek_sig_mutex_ptr!=PNULL),("ISP_SERVICE Creat peek signal MUTEX fail!"));/*assert verified*/	
	
}//end of IMG_CreateMutex

/*****************************************************************************/
//  Description: Get Mutex
//	Global resource dependence:
//  Author: XZ
//	Note:
//		input:
//			sm    -  mutex
//		output:
//			none
//		return:
//			none
/*****************************************************************************/
LOCAL void _ISP_GetPeekSigMutex(void)
{
    uint32 ret;
    if(PNULL == s_isp_peek_sig_mutex_ptr)
    {
        _ISP_CreatePeekSigMutex();
    }

    ret = SCI_GetMutex(s_isp_peek_sig_mutex_ptr, SCI_WAIT_FOREVER);
    SCI_ASSERT(ret==SCI_SUCCESS);/*assert verified*/
}
/*****************************************************************************/
//  Description: Put mutex
//	Global resource dependence:
//  Author: XZ
//	Note:
//		input:
//			sm    -  mutex
//		output:
//			none
//		return:
//			none
/*****************************************************************************/
LOCAL void _ISP_PutPeekSigMutex(void)
{
    uint32 ret;    
    if(PNULL == s_isp_peek_sig_mutex_ptr)
    {
        //SCI_TRACE_ISP:"NULL isp peek signal mutex, direct return"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_1420_112_2_18_1_43_37_36,(uint8*)"");
        return;
    }
    
    ret = SCI_PutMutex(s_isp_peek_sig_mutex_ptr );	
    SCI_ASSERT(ret==SCI_SUCCESS);/*assert verified*/
}
/****************************************************************************************/
// Description: Pause the dma 
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//                      None
// Note:
/*****************************************************************************************/
LOCAL void _ISP_PauseDMA(void)
{
    if(!ISP_GetAnalogTVDmaEb())
    {
        return ;
    }

    if(!s_dma_flag)
    {       
        SCI_DisableIRQ();  
        SCI_DisableFIQ(); 
        DMA_HAL_SetPause(SCI_TRUE);
        SCI_RestoreFIQ();
        SCI_RestoreIRQ();        
        s_dma_flag=SCI_TRUE;        
    }

    return ;
}

/****************************************************************************************/
// Description: resume the dma
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//                      None
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ResumDMA(void)
{
    if(!ISP_GetAnalogTVDmaEb())
    {
        return ;
    }

    if(s_dma_flag)
    {   
        SCI_DisableIRQ();  
        SCI_DisableFIQ(); 
        DMA_HAL_SetPause(SCI_FALSE);
        SCI_RestoreFIQ();
        SCI_RestoreIRQ(); 
        s_dma_flag=SCI_FALSE;        
    }

    return ;    
}

/****************************************************************************************/
// Description: Isp set event
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      uint32 isp_set_flags : event put flags
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_SetEvent(uint32 isp_set_flags)
{
    if(PNULL!=s_isp_event)
    {   
        SCI_SetEvent(s_isp_event, isp_set_flags, SCI_OR);
    }
}

/****************************************************************************************/
// Description: Isp get event
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      uint32 isp_requested_flags : event request flags
//                      uint32 isp_wait_option : event wait option
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail   
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_GetEvent(uint32 isp_requested_flags, uint32 isp_wait_option)
{
    uint32 actual_event = 0;
    uint32 status = SCI_ERROR;

    if(PNULL!=s_isp_event)
    {
        status = SCI_GetEvent(s_isp_event, 
                                isp_requested_flags, 
                                SCI_OR_CLEAR,
                                &actual_event, 
                                isp_wait_option);
    }

    return status;
}

/****************************************************************************************/
// Description: Send the message to isp service thread 
// Global resource dependence: 
// Author: Jianping.wang
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
LOCAL int _ISP_ServiceSendMsg(ISP_SERVICE_ID_E service,ISP_SERVICE_SIG_E command,uint32 param0,uint32 param1,void * ptr)
{
    ISP_SIGNAL_T *sig_ptr=NULL;
    xSignalHeaderRec *sig=NULL;

    sig_ptr=(ISP_SIGNAL_T *)SCI_ALLOCA(sizeof(ISP_SIGNAL_T));

    SCI_PASSERT(SCI_NULL!= sig_ptr, ("ISP_SERVICE: the sig_ptr is PNULL"));/*assert verified*/

    sig=&sig_ptr->sig;
    sig->SignalSize=sizeof(ISP_SIGNAL_T);
    sig->Sender=s_isp_service_thread_id;
    sig->SignalCode=(service&0x0FFFFUL);
    sig_ptr->command=command;
    sig_ptr->service=service;
    sig_ptr->param0=param0;
    sig_ptr->param1=param1;
    sig_ptr->ptr=ptr;

    SCI_SendSignal((xSignalHeader)sig_ptr, s_isp_service_thread_id);

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Clean the all isp signal
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail   
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_PeekSignal(void)
{
    uint32 			signal_count = 0;
    ISP_SIGNAL_T*   isp_sig_ptr = SCI_NULL;

    //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_PeekSignal at %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_1585_112_2_18_1_43_38_37,(uint8*)"d",SCI_GetTickCount());

    _ISP_GetPeekSigMutex();
    //protect the peekSignal action if need protected in other task(for close or stop action)
    if(s_Isp_Peek_Stop_State)
    {
        _ISP_PutPeekSigMutex();
        return SCI_SUCCESS;
            
    }
    
    while(1)/*lint !e716*/
    {
        isp_sig_ptr = (void*)SCI_PeekSignal(s_isp_service_thread_id);
        if (PNULL!=isp_sig_ptr)
        {
            SCI_FREE(isp_sig_ptr);
            signal_count++;           
            isp_sig_ptr=PNULL;
            //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_PeekSignal:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_1604_112_2_18_1_43_38_38,(uint8*)"d",signal_count);
        }
        else
        {
            break;
        }
    }   

    //unlock the protection
    _ISP_PutPeekSigMutex();

    //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_PeekSignal OK at %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_1615_112_2_18_1_43_38_39,(uint8*)"d",SCI_GetTickCount());
  
    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: The interrupt handle of camera sensor start of frame
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_SensorStartOfFrameLow(void)
{
    //SCI_TRACE_ISP("ISP_SERVICE:_ISP_ISR_SensorStartOfFrameLow");
}

/****************************************************************************************/
// Description: The interrupt handle of camera sensor start of frame
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_SensorStartOfFrameHigh(void)
{
    //SCI_TRACE_ISP("ISP_SERVICE:_ISP_ISR_SensorStartOfFrameHigh");
}

/****************************************************************************************/
// Description: The interrupt handle of camera sensor end of frame
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_SensorEndOfFrameLow(void)
{
    //SCI_TRACE_ISP("ISP_SERVICE:_ISP_ISR_SensorEndOfFrameLow");
}

/****************************************************************************************/
// Description: The interrupt handle of camera sensor end of frame
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_SensorEndOfFrameHigh(void)
{
    //SCI_TRACE_ISP("ISP_SERVICE:_ISP_ISR_SensorEndOfFrameHigh");
    _ISP_ServiceSendMsg(ISP_ZERO, ISP_SERVICE_SENSOR_EOF_SIG,0,0,0);
}

/****************************************************************************************/
// Description: The interrupt handle of cap start of frame
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_CapStartOfFrameLow(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    switch(pISPContext->service)
    {                 
        case ISP_SERVICE_PREVIEW:
        {
            break;            
        }  
        case ISP_SERVICE_CAPTURE:
        { 
            //close cap
            ISP_CapDisable();       
            //disable interrupt
            IVsp_CloseInt(CAP_SOF_ID); 
            break;            
        }
        case ISP_SERVICE_ATV_PREVIEW:        
        case ISP_SERVICE_ATV_RECORDER:
        { 
            _ISP_PauseDMA();
            break;
        }   
        default : 
            break;
    }
    //SCI_TRACE_ISP("ISP_SERVICE:_ISP_ISR_CapStartOfFrameLow");
    
}

/****************************************************************************************/
// Description: The interrupt handle of cap start of frame
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_CapStartOfFrameHigh(void)
{
    //SCI_TRACE_ISP("ISP_SERVICE:_ISP_ISR_CapStartOfFrameHigh");
}

/****************************************************************************************/
// Description: The interrupt handle of cap end of frame
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_CapEndOfFrameLow(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    switch(pISPContext->service)
    {
        case ISP_SERVICE_CAPTURE:
        {     
            break;
        }
        
        case ISP_SERVICE_ATV_PREVIEW:        
        case ISP_SERVICE_ATV_RECORDER:
        {
            IVsp_CloseInt(CAP_SOF_ID);              
            IVsp_ClearInt(CAP_SOF_ID);
            _ISP_ResumDMA();
            break;
        }     
        
        default:
        {
            //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ISR_CapEndOfFrameLow default %d line error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_1769_112_2_18_1_43_38_40,(uint8*)"d", __LINE__);
            break;			
        }
    }
    
    //SCI_TRACE_ISP("ISP_SERVICE:_ISP_ISR_CapEndOfFrameLow");
}

/****************************************************************************************/
// Description: The interrupt handle of cap end of frame
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_CapEndOfFrameHigh(void)
{    
    //SCI_TRACE_ISP("ISP_SERVICE:_ISP_ISR_CapEndOfFrame");
}

/****************************************************************************************/
// Description: The interrupt handle of data transfer end like preview/capture/review/scale
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_TrnasferDoneLow(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    //SCI_TRACE_ISP("ISP_SERVICE:_ISP_ISR_TrnasferDoneLow: time: %d", SCI_GetTickCount());  

    pISPContext->is_tx_done=SCI_TRUE;

    if(SCI_TRUE== pISPContext->is_timeout)
    {
        return;
    }

    switch(pISPContext->service)
    {
        case ISP_SERVICE_PREVIEW:
        {   
            if(SCI_TRUE!=pISPContext->is_stop)
            {
                if(0x00==pISPContext->skip_counter)
                {
                    if(ISP_SERVICE_DISPLAY_END==pISPContext->display_state)
                    {
                        AHB_MASTER_MODULE_T addr;

                        pISPContext->preview_isp_out_buf_addr.y_addr=pISPContext->preview_buf[pISPContext->preview_buf_id].y_addr;
                        pISPContext->preview_isp_out_buf_addr.uv_addr=pISPContext->preview_buf[pISPContext->preview_buf_id].uv_addr;                        

                        pISPContext->preview_buf_id++;
                        if(pISPContext->preview_loop_buf_num<pISPContext->preview_buf_id)
                        {
                            pISPContext->preview_buf_id=ISP_PREVIEW_BUF_0;
                        }

                        addr.output_addr.y_addr = pISPContext->preview_buf[pISPContext->preview_buf_id].y_addr;
                        addr.output_addr.uv_addr = pISPContext->preview_buf[pISPContext->preview_buf_id].uv_addr;
                        
                        ISP_SetAhbMaster(&addr);            
                        ISP_ForceCopy();

                        if(SCI_TRUE==pISPContext->is_send_zoom_sig)
                        {//digital zoom
                            ISP_SoftReset();
                        }
                    }              
                }              
            }
            break;
        }         
        
        default:
        {
            break;			
        }
    }

}

/****************************************************************************************/
// Description: The interrupt handle of data transfer end like preview/capture/review/scale
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_TrnasferDoneHigh(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    //SCI_TRACE_ISP("ISP_SERVICE:_ISP_ISR_TrnasferDone: time: %d", SCI_GetTickCount());  

    if(SCI_TRUE== pISPContext->is_timeout)
    {
        return;
    }
   
    switch(pISPContext->service)
    {
        case ISP_SERVICE_PREVIEW:
        {   
            if(SCI_TRUE==pISPContext->is_stop)
            {
                ISP_SoftReset(); 
                _ISP_ServiceSendMsg(ISP_SERVICE_PREVIEW,ISP_SERVICE_STOP_SIG,0,0,0);
            }
            else
            {
                if(0x00==pISPContext->skip_counter)
                {
                    if(ISP_SERVICE_DISPLAY_END==pISPContext->display_state)
                    {
                        pISPContext->display_state=ISP_SERVICE_DISPLAY_STATE;                        

                        _ISP_ServiceSendMsg(ISP_SERVICE_PREVIEW,ISP_SERVICE_TRANSFER_DONE_SIG,0,0,0);

                        if(SCI_TRUE==pISPContext->is_send_zoom_sig)
                        {//digital zoom
                            _ISP_ServiceSendMsg(ISP_SERVICE_DC_ZOOM,ISP_SERVICE_START_SIG,0,0,0);
                            pISPContext->is_send_zoom_sig=SCI_FALSE;
                        }
                    }
                }
                else
                {
                    pISPContext->skip_counter--;
                    if(0x00==pISPContext->skip_counter)
                    {
                        _ISP_ServiceSendMsg(ISP_SERVICE_SET_SENSOR_CALLBACK, ISP_SERVICE_START_SIG,0,0,0);
                    }
                }                
            }

            break;
        }
        
        case ISP_SERVICE_CAPTURE:
        {			
            _ISP_ServiceSendMsg(ISP_SERVICE_CAPTURE, ISP_SERVICE_TRANSFER_DONE_SIG, 0, 0, 0);
            break;                    
        }

        case ISP_SERVICE_REVIEW:
        {
            _ISP_ServiceSendMsg(ISP_SERVICE_REVIEW,ISP_SERVICE_TRANSFER_DONE_SIG, 0,0,0);
            break;
        }
        
        case ISP_SERVICE_MOTION_JPEG:
        {
            ISP_SetMode(ISP_MODE_IDLE);
            _ISP_ServiceSendMsg(ISP_SERVICE_MOTION_JPEG,ISP_SERVICE_TRANSFER_DONE_SIG, 0,0,0);   
            break;
        } 
        
        case ISP_SERVICE_UPCC:
        {
            ISP_SetMode(ISP_MODE_IDLE);
            _ISP_ServiceSendMsg(ISP_SERVICE_UPCC,ISP_SERVICE_TRANSFER_DONE_SIG, 0,0,0);   
            break;
        }  
        
        case ISP_SERVICE_SCALE:
        case ISP_SERVICE_SLICE_SCALE:
        {
            _ISP_ServiceSendMsg(ISP_SERVICE_SCALE,ISP_SERVICE_TRANSFER_DONE_SIG, 0,0,0);
            break;
        }  

        case ISP_SERVICE_ATV_PREVIEW:
        {
            ISP_SetMode(ISP_MODE_IDLE);
            _ISP_ServiceSendMsg(ISP_SERVICE_ATV_PREVIEW, ISP_SERVICE_TRANSFER_DONE_SIG, 0,0,0);               
            break;
        }  

        case ISP_SERVICE_ATV_RECORDER:
        {
            ISP_SetMode(ISP_MODE_IDLE);
            _ISP_ServiceSendMsg(ISP_SERVICE_ATV_RECORDER,ISP_SERVICE_TRANSFER_DONE_SIG, 0,0,0);
            break;
        }          
        
        default:
        {
            break;			
        }
    }

}

/****************************************************************************************/
// Description: The interrupt handle of img sensor output bad line 
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_SensorLineErrLow(void)
{
    //reset isp
    ISP_SoftReset();

    //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ISR_SensorLineErr\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_1988_112_2_18_1_43_38_41,(uint8*)"");
}

/****************************************************************************************/
// Description: The interrupt handle of img sensor output bad line 
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_SensorLineErrHigh(void)
{
    //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ISR_SensorLineErrHigh"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2003_112_2_18_1_43_39_42,(uint8*)"");
    _ISP_ISR_PathFIFOOverflowedHigh();
}

/****************************************************************************************/
// Description: The interrupt handle of path fifo everflowed
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_PathFIFOOverflowedLow(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext(); 
    
    //reset isp
    ISP_SoftReset();
    
    //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ISR_PathFIFOOverflowed service: 0x%x "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2024_112_2_18_1_43_39_43,(uint8*)"d", pISPContext->service);
}
/****************************************************************************************/
// Description: The interrupt handle of path fifo everflowed
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_PathFIFOOverflowedHigh(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    switch(pISPContext->service)
    {
        case ISP_SERVICE_MOTION_JPEG:
        case ISP_SERVICE_ATV_PREVIEW: 
        case ISP_SERVICE_ATV_RECORDER:
        {
            if(SCI_TRUE==pISPContext->is_tx_done)
            {
                return ;
            }
            break;
        }
        default :
            break;
    }

    IVsp_UnRegisterIntFunc(ISP_TX_DONE_ID);

    switch(pISPContext->service)
    {
        case ISP_SERVICE_PREVIEW:
            ISP_CapDisable();
            break;
        case ISP_SERVICE_ATV_PREVIEW: 
        case ISP_SERVICE_ATV_RECORDER:
            _ISP_ATVTimerDActive();
            break;
        case ISP_SERVICE_MOTION_JPEG:
            _ISP_TimerDActive();
            break;
        default :
            break;
    }

    _ISP_ServiceSendMsg(ISP_ZERO, ISP_SERVICE_OVERFLOW_SIG,0,0,0);

    IVsp_UnRegisterIntFunc(ISP_BUFFER_OVF_ID);

    //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ISR_PathFIFOOverflowed service: 0x%x "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2078_112_2_18_1_43_39_44,(uint8*)"d", pISPContext->service);
}
/****************************************************************************************/
// Description: The interrupt handle of img sensor output bad fram
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_SensorFrameErrLow(void)
{
    //reset isp
    ISP_SoftReset();

    //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ISR_SensorFrameErrLow"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2095_112_2_18_1_43_39_45,(uint8*)"");
}
/****************************************************************************************/
// Description: The interrupt handle of img sensor output bad fram
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_SensorFrameErrHigh(void)
{
    //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ISR_SensorFrameErrHigh"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2109_112_2_18_1_43_39_46,(uint8*)"");
    _ISP_ISR_PathFIFOOverflowedHigh( );
 
}

/****************************************************************************************/
// Description: The interrupt handle of jpg buffer overflow
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_JpegBufferverflowedLow(void)
{
    //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ISR_JpegBufferverflowedLow"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2126_112_2_18_1_43_39_47,(uint8*)"");
}

/****************************************************************************************/
// Description: The interrupt handle of jpg buffer overflow
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ISR_JpegBufferverflowedHigh(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    switch(pISPContext->service)
    {
        case ISP_SERVICE_CAPTURE:
        {
            if(pISPContext->state == ISP_SERVICE_CAPTURE_STATE)
            {			
                _ISP_ServiceSendMsg(ISP_SERVICE_CAPTURE, ISP_SERVICE_OVERFLOW_SIG, 0, 0, 0);
            }
            break;                    
        }

        default:
            //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ISR_JpegBufferverflowedHigh default %d line error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2155_112_2_18_1_43_39_48,(uint8*)"d", __LINE__);
            break;			
    }

    //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ISR_JpegBufferverflowedHigh"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2159_112_2_18_1_43_39_49,(uint8*)"");
}

/****************************************************************************************/
// Description: The timeout handle of cap get analog tv frame data 
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_TimerOutHandler(uint32 param)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_TimerOutHandler service: %d , %dms"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2175_112_2_18_1_43_39_50,(uint8*)"dd",pISPContext->service,SCI_GetTickCount());

    pISPContext->is_timeout=SCI_TRUE;
    pISPContext->time_out_counter++;
    
    switch(pISPContext->service)
    {
        case ISP_SERVICE_MOTION_JPEG:
        case ISP_SERVICE_UPCC:            
        {
            break;
        }

        case ISP_SERVICE_ATV_PREVIEW:
        case ISP_SERVICE_ATV_RECORDER:
        {
            _ISP_ResumDMA();
            break;
        }

        default :
            break;
    }
    
    if(SCI_TRUE!=pISPContext->is_stop)
    {
        _ISP_ServiceSendMsg(pISPContext->service, ISP_SERVICE_START_SIG,0,0,0);
    }
    else
    {
        _ISP_ServiceSendMsg(pISPContext->service, ISP_SERVICE_STOP_SIG,0,0,0);        
    }

    return ;
}

/****************************************************************************************/
// Description: analog tv timer actibe
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ATVTimerActive(void)
{ 
    if(ISP_ServiceGetAtvIsEnable())  
    {
        if (NULL!= s_isp_timer)
        {
            SCI_ChangeTimer(s_isp_timer, _ISP_TimerOutHandler, ISP_ATV_TIMEOUT);
            SCI_ActiveTimer(s_isp_timer);
        }
    }
}

/****************************************************************************************/
// Description: anglog tv timer dactive
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ATVTimerDActive(void)
{    
    if(ISP_ServiceGetAtvIsEnable())  
    {    
        if(NULL!=s_isp_timer)
        {
            ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
            pISPContext->time_out_counter=0x00;

            if(SCI_IsTimerActive(s_isp_timer))
            {
                SCI_DeactiveTimer(s_isp_timer);
            }
        }
    }
}

/****************************************************************************************/
// Description: isp timer active
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      timer_expire: timer timeout expire
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_TimerActive(uint32 timer_expire)
{ 
    if (NULL!= s_isp_timer)
    {
        SCI_ChangeTimer(s_isp_timer, _ISP_TimerOutHandler, timer_expire);
        SCI_ActiveTimer(s_isp_timer);
    }
}

/****************************************************************************************/
// Description: isp timer dactive
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_TimerDActive(void)
{    
    if(NULL!=s_isp_timer)
    {
        ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
        pISPContext->time_out_counter=0x00;
        if(SCI_IsTimerActive(s_isp_timer))
        {
            SCI_DeactiveTimer(s_isp_timer);
        }
    }
}

/****************************************************************************************/
// Description: Get is yuv to rgb
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_DATA_FORMAT_E data_format: data format of process
// output parameter:
//                      None
// return:
//          BOOLEAN:SCI_TRUE is yuv to rgb, SCI_FALSE no yuv to rgb      
// Note:
/*****************************************************************************************/
LOCAL BOOLEAN _ISP_GetIsYuv2Rgb(ISP_DATA_FORMAT_E data_format)
{
    switch(data_format)
    {
        case ISP_DATA_RGB565:
        case ISP_DATA_RGB888: 
            return SCI_TRUE;
            
        default :
            break;
    }
    return SCI_FALSE;
}

/****************************************************************************************/
// Description: Get is rgb
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_DATA_FORMAT_E data_format: data format of process
// output parameter:
//                      None
// return:
//          BOOLEAN:SCI_TRUE is rgb, SCI_FALSE no rgb      
// Note:
/*****************************************************************************************/
LOCAL BOOLEAN _ISP_GetIsRgb(ISP_DATA_FORMAT_E data_format)
{
    switch(data_format)
    {
        case ISP_DATA_RGB565:
        case ISP_DATA_RGB888: 
            return SCI_TRUE;
            
        default :
            break;
    }
    return SCI_FALSE;
}

/****************************************************************************************/
// Description: Get is rgb565
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_DATA_FORMAT_E data_format: data format of process
// output parameter:
//                      None
// return:
//          BOOLEAN:SCI_TRUE is rgb565, SCI_FALSE no rgb565
// Note:
/*****************************************************************************************/
LOCAL BOOLEAN _ISP_GetIsRgb565(ISP_DATA_FORMAT_E data_format)
{

    if( ISP_DATA_RGB565==data_format)
    {
        return SCI_TRUE;
    }

    return SCI_FALSE;
}

/******************************************************************************
// Description:	get isp num align of output width
// Author:		Tim.zhu
// Input:		src_width: source length
// Return:		uint32: align length
// Note:			
******************************************************************************/ 
LOCAL uint32 _ISP_GetOutputWidthAlign(uint32 src_width, ISP_DATA_FORMAT_E data_format)
{
    uint32 align_width=0x00;
    uint32 shift_bit=0x00;

    switch(ISP_ServiceGetOutputWidthAlign(data_format))
    {
        case ISP_ONE:
            shift_bit=ISP_ZERO;
            break;
        case ISP_TWO:
            shift_bit=ISP_SHIFT_BIT1;
            break;
        case ISP_FOUR:
            shift_bit=ISP_SHIFT_BIT2;
            break;
        default:
            shift_bit=ISP_SHIFT_BIT2;
            break;

    }
    
    align_width=ISP_Align(src_width, ISP_ZERO, shift_bit); 

    return align_width;
}

/****************************************************************************************/
// Description: calc the subsample mode
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                None
// output parameter:
//                None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_CalcSubSampleMode(void)
{ 
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    
    pISPContext->subsample_eb=SCI_FALSE;

    if(ISP_IsSupportSubsample())
    {
        if((pISPContext->scale_trim_rect.w>(pISPContext->output_size.w<<2))
            ||(pISPContext->scale_trim_rect.h>(pISPContext->output_size.h<<2)))
        { //check is need subsample
            pISPContext->subsample_eb=SCI_TRUE;
            
            if((pISPContext->scale_trim_rect.w*pISPContext->output_size.h)>(pISPContext->scale_trim_rect.h*pISPContext->output_size.w))
            {
                if(pISPContext->scale_trim_rect.w<((pISPContext->output_size.w<<2)<<1))
                {
                    pISPContext->subsample_mode=ISP_SUB_2;
                }
                else if(pISPContext->scale_trim_rect.w<((pISPContext->output_size.w<<2)<<2))
                {
                    pISPContext->subsample_mode=ISP_SUB_4;
                }
                else if(pISPContext->scale_trim_rect.w<((pISPContext->output_size.w<<2)<<3))
                {
                    pISPContext->subsample_mode=ISP_SUB_8;
                }
                else if(pISPContext->scale_trim_rect.w<((pISPContext->output_size.w<<2)<<4))
                {
                    pISPContext->subsample_mode=ISP_SUB_16;
                } 
            }
            else
            {
                if(pISPContext->scale_trim_rect.h<((pISPContext->output_size.h<<2)<<1))
                {
                    pISPContext->subsample_mode=ISP_SUB_2;
                }
                else if(pISPContext->scale_trim_rect.h<((pISPContext->output_size.h<<2)<<2))
                {
                    pISPContext->subsample_mode=ISP_SUB_4;
                }
                else if(pISPContext->scale_trim_rect.h<((pISPContext->output_size.h<<2)<<3))
                {
                    pISPContext->subsample_mode=ISP_SUB_8;
                }
                else if(pISPContext->scale_trim_rect.h<((pISPContext->output_size.h<<2)<<4))
                {
                    pISPContext->subsample_mode=ISP_SUB_16;
                }  
            }

            if(ISP_ZERO!=(pISPContext->scale_trim_rect.w&((0xffff)>>(0x0d-pISPContext->subsample_mode))))
            {
                ISP_RECT_T trim_rect={0x00};

                trim_rect.w=((pISPContext->scale_trim_rect.w>>(pISPContext->subsample_mode+0x03))<<(pISPContext->subsample_mode+0x03));
                trim_rect.h=(trim_rect.w*pISPContext->scale_trim_rect.h)/pISPContext->scale_trim_rect.w;
                pISPContext->scale_trim_rect.w=trim_rect.w;
                pISPContext->scale_trim_rect.h=trim_rect.h;
            }

        }
    }
    
}

/****************************************************************************************/
// Description: Calculate Scaling coeff
// Global resource dependence: 
// Author: Jianping.wang
// input parameter:
//                      uint32 input_width: intput image width
//                      uint32 input_height: input image height 
//                      uint32 output_width: output image width 
//                      uint32 output_height: output image height
// output parameter:
//                      None
// return:
//          scale_ptr: the scaling coeff buf pointer     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_CalculateScaleCoeff(uint32 input_width, uint32 input_height, uint32 output_width, uint32 output_height)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint32 v_coeff=0x00;
    uint32 h_coeff=0x00;    
    uint32 in_width=input_width;
    uint32 in_height=input_height;        
    uint32 out_width=output_width;
    uint32 out_height=output_height; 
    uint32 shift_bits=pISPContext->subsample_mode+1; 

	//SCI_TRACE_ISP:"[_ISP_CalculateScaleCoeff] shift_bits = %d, sub enable = %d, in size (%d, %d), out size = (%d, %d)"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2506_112_2_18_1_43_40_51,(uint8*)"dddddd", shift_bits, pISPContext->subsample_eb, input_width, input_height, output_width, output_height);

    in_width=(in_width>>ISP_SHIFT_BIT2)<<ISP_SHIFT_BIT2;
    in_height=((in_height>>ISP_SHIFT_BIT1)<<ISP_SHIFT_BIT1);    // align 2

    out_width=((out_width+ISP_OUTPUT_SCALE_ALIGN)>>ISP_SHIFT_BIT2)<<ISP_SHIFT_BIT2;  
    out_height=((out_height>>ISP_SHIFT_BIT1)<<ISP_SHIFT_BIT1);      // align 2	

    pISPContext->scale_trim_rect.w=in_width;
    pISPContext->scale_trim_rect.h=in_height;

    pISPContext->output_size.w=out_width;
    pISPContext->output_size.h=out_height;

    if((in_width!=out_width)
        ||(in_height!=out_height)
        ||(SCI_TRUE==ISP_ServiceGetAtvIsEnable())
        ||(ISP_SERVICE_SCALE_SLICE==pISPContext->scale_mode))
    {
        pISPContext->is_bypass_scale=SCI_FALSE;
    }
    else
    {
        pISPContext->is_bypass_scale=SCI_TRUE;
    }
    
    if(SCI_TRUE==pISPContext->subsample_eb)
    {
        in_width>>=shift_bits;
        in_height>>=shift_bits;
    }


	//[1/4, 4]
	if (ISP_SCALE_4X * out_width < in_width || ISP_SCALE_4X * out_height < in_height)
	{
        //SCI_TRACE_ISP:"ISP_SERVICE: ISP Scale coeff error ! in (%d X %d), out (%d X %d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2543_112_2_18_1_43_40_52,(uint8*)"dddd",in_width, in_height, out_width, out_height);
        return SCI_ERROR;
	}

	if (ISP_SCALE_4X * in_width < out_width || ISP_SCALE_4X * in_height < out_height)
	{
        //SCI_TRACE_ISP:"ISP_SERVICE: ISP Scale coeff error ! in (%d X %d), out (%d X %d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2550_112_2_18_1_43_40_53,(uint8*)"dddd",in_width, in_height, out_width, out_height);
        return SCI_ERROR;
	}	
	
	if (NULL == pISPContext->h_scale_coeff_ptr || NULL == pISPContext->v_scale_coeff_ptr
		|| NULL == pISPContext->scale_tmp_ptr)
	{
		//SCI_TRACE_ISP:"ISP_SERVICE: GenScaleCoeff memory error !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2557_112_2_18_1_43_40_54,(uint8*)"");
        return SCI_ERROR;
	}
	
	if(!GenScaleCoeff(in_width, in_height, out_width, out_height, 
	  				  pISPContext->h_scale_coeff_ptr, pISPContext->v_scale_coeff_ptr, 
	  				  pISPContext->scale_tmp_ptr, pISPContext->scale_tmp_size))
	{
		//SCI_TRACE_ISP:"ISP_SERVICE: GenScaleCoeff failed !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2565_112_2_18_1_43_40_55,(uint8*)"");
        return SCI_ERROR;
	}

	if (out_width >= in_width)
	{
		uint32 *h_coef_ptr = ISP_GetHScaleCoeffTab(h_coeff);
		SCI_MEMCPY(pISPContext->h_scale_coeff_ptr, h_coef_ptr, SCALER_COEF_HOR_SIZE);
		//SCI_TRACE_ISP:"ISP_SERVICE: reset hor coef to scaling up coef"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2573_112_2_18_1_43_40_56,(uint8*)"");
	}

	if (out_height >= in_height)
	{
		uint32 *v_coef_ptr = ISP_GetVScaleCoeffTab(v_coeff);
		SCI_MEMCPY(pISPContext->v_scale_coeff_ptr, v_coef_ptr, SCALER_COEF_VER_SIZE);
		//SCI_TRACE_ISP:"ISP_SERVICE: reset ver coef to scaling up coef"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2580_112_2_18_1_43_40_57,(uint8*)"");
	}	

	//SCI_TRACE_ISP:"ISP_SERVICE: GenScaleCoeff in size (%d X %d), out size (%d X %d)"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2584_112_2_18_1_43_40_58,(uint8*)"dddd",in_width, in_height, out_width, out_height);

	//SCI_TRACE_ISP:"ISP_SERVICE: GenScaleCoeff hor ptr = 0x%x, ver ptr = 0x%x, temp = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2587_112_2_18_1_43_40_59,(uint8*)"ddd",pISPContext->h_scale_coeff_ptr, pISPContext->v_scale_coeff_ptr, pISPContext->scale_tmp_ptr);

    return SCI_SUCCESS;	
    
}

/****************************************************************************************/
// Description: Calculate Scaling coeff and trim rect
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//                      None
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_CalcScaleParam(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint32 ret=SCI_SUCCESS;

    // calculate the scale coeff
    ret=_ISP_CalculateScaleCoeff(pISPContext->scale_trim_rect.w, pISPContext->scale_trim_rect.h, pISPContext->output_size.w, pISPContext->output_size.h);

    // set scale trim eb
    if((pISPContext->scale_trim_rect.w!=pISPContext->cap_output_size.w)||(pISPContext->scale_trim_rect.h!=pISPContext->cap_output_size.h))
    {  // trim scale input algin 4
        pISPContext->scale_trim_rect.x= (pISPContext->scale_trim_rect.x>>ISP_SHIFT_BIT2)<<ISP_SHIFT_BIT2;
        pISPContext->scale_trim_rect.y= (pISPContext->scale_trim_rect.y>>ISP_SHIFT_BIT2)<<ISP_SHIFT_BIT2;
        pISPContext->scale_trim_eb=SCI_TRUE;
    }
    else
    {
        pISPContext->scale_trim_eb=SCI_FALSE;
    }
    
    return ret;	
    
}

/****************************************************************************************/
// Description: set Scaling coeff
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//                      None
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_SetScaleCoeff(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    ISP_BypassScale(pISPContext->is_bypass_scale);

    if(0x00==pISPContext->is_bypass_scale)
    {
        ISP_SetVerDownTap(pISPContext->v_scale_coeff_ptr[36]);

        ISP_SetClkAhbDomain();

        ISP_SetScaleCoeff(pISPContext->h_scale_coeff_ptr, pISPContext->v_scale_coeff_ptr);

        ISP_SetClkDcamDomain();	
    }

    return SCI_SUCCESS;	
    
}

/****************************************************************************************/
// Description: set Scaling coeff
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//                      None
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_ImgTerminalLineCopy(ISP_DATA_ADDR_T data_addr, ISP_SIZE_T size, ISP_DATA_FORMAT_E data_format)
{
    ISP_DATA_ADDR_T src_addr={0x00};
    ISP_DATA_ADDR_T dst_addr={0x00};

    switch(data_format)
    {
        case ISP_DATA_RGB565:
        {
            src_addr.y_addr=data_addr.y_addr+(size.w<<0x01)*(size.h-0x02);
            dst_addr.y_addr=data_addr.y_addr+(size.w<<0x01)*(size.h-0x01);
            SCI_MEMCPY((void*)dst_addr.y_addr,(void*)src_addr.y_addr,(size.w<<0x01));
            break;
        }
        case ISP_DATA_YUV422:
        {
            src_addr.y_addr=data_addr.y_addr+size.w*(size.h-0x02);
            dst_addr.y_addr=data_addr.y_addr+size.w*(size.h-0x01);
            SCI_MEMCPY((void*)dst_addr.y_addr,(void*)src_addr.y_addr,size.w);
            src_addr.uv_addr=data_addr.uv_addr+size.w*(size.h-0x02);
            dst_addr.uv_addr=data_addr.uv_addr+size.w*(size.h-0x01);
            SCI_MEMCPY((void*)dst_addr.uv_addr,(void*)src_addr.uv_addr,size.w);
            break;
        }
        default:
            //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ImgTerminalLineCopy default %d line error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2693_112_2_18_1_43_40_60,(uint8*)"d", __LINE__);
            break;

    }

    return SCI_SUCCESS;

}

/****************************************************************************************/
// Description: set the max zoom level
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      none
// output parameter:
//                      None
// return:
//          SCI_SUCCESS : success
//          other : fail
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_SetMaxZoomLevel(uint32 level)
{
    if(ISP_SERVICE_ZOOM_200X<level)
    {
        s_max_zoom_level=ISP_SERVICE_ZOOM_200X;        
    }
    else
    {
        s_max_zoom_level=level;
    }

    return SCI_SUCCESS;    
}

/****************************************************************************************/
// Description: get the max zoom level
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      none
// output parameter:
//                      None
// return:
//          uint32 : the max zoom level
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_GetMaxZoomLevel(void)
{
    return s_max_zoom_level;    
}

/****************************************************************************************/
// Description: get the pointer of preview zoom table
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      none
// output parameter:
//                      None
// return:
//          uint16* : the pointer of zoom table     
// Note:
/*****************************************************************************************/
LOCAL ISP_RECT_T_PTR _ISP_GetZoomTabPtr(void)
{
    return (ISP_RECT_T_PTR)&s_isp_zoom_tab;
}

/****************************************************************************************/
// Description: set dc zoom tab
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      none
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_SetZoomTab(uint16 input_start_x, 
                                    uint16 input_start_y, 
                                    uint16 input_width, 
                                    uint16 input_height, 
                                    uint16 output_width, 
                                    uint16 output_height)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint16 in_width=0x00;
    uint16 in_height=0x00;
    uint16 out_width=0x00;
    uint16 out_height=0x00;

    in_width=input_width;
    in_height=input_height;
    out_width=(output_width*pISPContext->cap_deci)/ISP_GetMaxPreviewScaleUpCoeff();
    out_height=(output_height*pISPContext->cap_deci)/ISP_GetMaxPreviewScaleUpCoeff();

    if(((in_width==out_width)&&(in_height==out_height)) || ISP_SERVICE_ZOOM_100X == _ISP_GetMaxZoomLevel() 
        ||ISP_ServiceGetAtvIsEnable())
    {
        pISPContext->zoom_array_ptr=NULL;
    }
    else
    {
        ISP_CalcZoomTab(input_start_x, input_start_y, input_width, input_height, output_width, output_height);        
        pISPContext->zoom_array_ptr=(uint16 *)_ISP_GetZoomTabPtr();
    }
    
    return SCI_SUCCESS;
}

PUBLIC uint32 ISP_Service_GetDirectMaxScaleWidth(void)
{
	return 480;
}



/****************************************************************************************/
// Description: calc the cap deci
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      input_width: the width of input image
//                      output_width: the width of output image
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success     
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32 _ISP_CalcCapDeci(uint16 input_width, uint16 input_height, uint16 output_width, uint16 output_height)//DCAMERA_RECT_T* src_rect_ptr, DCAMERA_RECT_T* dst_rect_ptr) 
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint32 scale_coeff=0x00;
    uint32 i=0x00;
    uint16 input_coeff=0x00;
    uint16 output_coeff=0x00;
    
    scale_coeff=ISP_GetMaxScaleDownCoeff();

    pISPContext->cap_deci=ISP_CAP_DECI_DISABLE;

    if(output_width*input_height<input_width*output_height)
    {// depend on out height
        input_coeff=input_height;
        output_coeff=output_height;
    }
    else
    {// depend on out width
        input_coeff=input_width;
        output_coeff=output_width;
    }
    
    if((output_coeff*scale_coeff)<input_coeff)
    {
        for(i=ISP_CAP_DECI_DISABLE; i<ISP_CAP_DECI_MAX; i++)
        {
            if(input_coeff<=(output_coeff*scale_coeff*i))
            {
                pISPContext->cap_deci=i;
                break;
            }
        }
    }

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: check the preview if the full view mode  
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_CheckIsFullView(void)
{
    #if (defined(PLATFORM_SC6600L))
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    LCD_INFO_T lcd_info={0x00};

    LCD_GetInfo(0,&lcd_info);
    
    if((pISPContext->disp_rect.w==lcd_info.lcd_width)&&(pISPContext->disp_rect.h!=lcd_info.lcd_height))
    {
        SCI_MEMCPY((void*)&(pISPContext->full_screen_rect),(void*)&(pISPContext->disp_rect),sizeof(ISP_RECT_T));
        pISPContext->is_full_view=SCI_TRUE;
        pISPContext->disp_rect.x=0;            
        pISPContext->disp_rect.y=0;           
        pISPContext->disp_rect.w=lcd_info.lcd_width;            
        pISPContext->disp_rect.h=lcd_info.lcd_height; 
    }
    else if((pISPContext->disp_rect.w==lcd_info.lcd_height)&&(pISPContext->disp_rect.h!=lcd_info.lcd_width))
    {
        SCI_MEMCPY((void*)&(pISPContext->full_screen_rect),(void*)&(pISPContext->disp_rect),sizeof(ISP_RECT_T));
        pISPContext->is_full_view=SCI_TRUE;        
        pISPContext->disp_rect.x=0;            
        pISPContext->disp_rect.y=0;           
        pISPContext->disp_rect.w=lcd_info.lcd_height;            
        pISPContext->disp_rect.h=lcd_info.lcd_width;
    }
    else if((pISPContext->disp_rect.w!=lcd_info.lcd_height)&&(pISPContext->disp_rect.h==lcd_info.lcd_width))
    {

    }
    #endif
    
    return SCI_SUCCESS;
} 

/****************************************************************************************/
// Description: check the img sensor output bad frame enable  
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_CheckImgBadFrameEb(void)
{
    if(ISP_BadFrameCheckSupport())
    {
        #if (defined(PLATFORM_SC6800H))
        //register interrupt handle
        IVsp_RegisterIntFunc(CAP_LINE_ERR_ID,_ISP_ISR_SensorLineErrLow);
        //enable interrupt
        IVsp_ClearInt(CAP_LINE_ERR_ID);
        IVsp_OpenInt(CAP_LINE_ERR_ID);

        //register interrupt handle
        IVsp_RegisterIntFunc(CAP_FRAME_ERR_ID,_ISP_ISR_SensorFrameErrLow);
        //enable interrupt
        IVsp_ClearInt(CAP_FRAME_ERR_ID);
        IVsp_OpenInt(CAP_FRAME_ERR_ID);
        #endif
    }
    
    return SCI_SUCCESS;
} 

/****************************************************************************************/
// Description: Save the preview parameter of dc to isp service com parameter  
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_PREVIEW_PARAM_T * param_ptr: the preview parameter pointer
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_SetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint32 rtn=SCI_SUCCESS;
    uint16* zoom_ptr=NULL;

    pISPContext->subsample_eb=SCI_FALSE;    //FOR SCALING CALCUATION ISSUE.
    
    if(ISP_DATA_YUV422==pISPContext->input_image_format)
    {
        if(SCI_TRUE==pISPContext->is_video_mode)
        {
            pISPContext->preview_deci_frame_num=pISPContext->video_preview_deci_frame_num;
            ISP_ServiceCleanVideFlag();
        }
    }
    else
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: _ISP_SetPreviewParam img format is %x  %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_2963_112_2_18_1_43_41_61,(uint8*)"dd", pISPContext->input_image_format, __LINE__);
        return SCI_ERROR;
    }

    ISP_RETURN_IF_ERR(_ISP_SetDcamClk(param_ptr->pclk), SCI_ERROR, ("ISP_SERVICE: _ISP_SetPreviewParam pclk:%d, %d line error",param_ptr->pclk, __LINE__));

    if(ISP_SERVICE_ZOOM_100X==param_ptr->init_zoom_level)
    {
        pISPContext->is_zoom=SCI_FALSE;
    }
    else
    {
        pISPContext->is_zoom=SCI_TRUE;
    }

    pISPContext->is_full_view=SCI_FALSE;      
    pISPContext->init_zoom_level=param_ptr->init_zoom_level;
    pISPContext->zoom_ctl.zoom_level=pISPContext->init_zoom_level;
    pISPContext->is_send_zoom_sig=SCI_FALSE;
    pISPContext->is_stop=SCI_FALSE;
    pISPContext->is_first_frame = SCI_TRUE; 
    pISPContext->isp_feature_enable=SCI_TRUE;
    pISPContext->is_timeout=SCI_FALSE; 
    pISPContext->yuv2rgb_eb=SCI_TRUE;
    pISPContext->output_image_format=ISP_GetDispDataFormat();
    pISPContext->disp_format=ISP_GetDispDataFormat();

    if(ISP_DATA_RGB565!=pISPContext->output_image_format)
    {
        pISPContext->yuv2rgb_eb=SCI_FALSE;
    }

    
    SCI_MEMCPY((void*)&(pISPContext->preview_input_rect),(void*)&(param_ptr->input_rect),sizeof(ISP_RECT_T));
    SCI_MEMCPY((void*)&(pISPContext->input_size),(void*)&(param_ptr->input_size),sizeof(ISP_SIZE_T));
    SCI_MEMCPY((void*)&(pISPContext->cap_trim_rect),(void*)&(param_ptr->input_rect),sizeof(ISP_RECT_T));
    SCI_MEMCPY((void*)&(pISPContext->disp_rect),(void*)&(param_ptr->disp_rect),sizeof(ISP_RECT_T));
    SCI_MEMCPY((void*)&(pISPContext->target_rect),(void*)&(param_ptr->target_rect),sizeof(ISP_RECT_T));
    
    // preview mode rgb output format align two      
    pISPContext->disp_rect.h=((pISPContext->disp_rect.h>>ISP_SHIFT_BIT1)<<ISP_SHIFT_BIT1);

    _ISP_CalcCapDeci(pISPContext->cap_trim_rect.w,pISPContext->cap_trim_rect.h,pISPContext->target_rect.w,pISPContext->target_rect.h);

    _ISP_SetZoomTab(pISPContext->cap_trim_rect.x, pISPContext->cap_trim_rect.y, pISPContext->cap_trim_rect.w, pISPContext->cap_trim_rect.h, pISPContext->target_rect.w, pISPContext->target_rect.h);

    _ISP_CheckIsFullView();
 
    if(NULL!=pISPContext->zoom_array_ptr)
    {
        zoom_ptr=pISPContext->zoom_array_ptr+((pISPContext->init_zoom_level*ISP_SMOOTH_ZOOM_STEP)*ISP_ZOOM_RECT_NUM);
        SCI_MEMCPY((void*)&(pISPContext->zoom_rect),(void*)(zoom_ptr),sizeof(ISP_RECT_T));  
        SCI_MEMCPY((void*)&(pISPContext->cap_trim_rect),(void*)(zoom_ptr),sizeof(ISP_RECT_T));    
    }
    else
    {
        SCI_MEMCPY((void*)&(pISPContext->zoom_rect),(void*)&(param_ptr->input_rect),sizeof(ISP_RECT_T));  
        SCI_MEMCPY((void*)&(pISPContext->cap_trim_rect),(void*)&(param_ptr->input_rect),sizeof(ISP_RECT_T));    
    }
    
    pISPContext->cap_output_size.w=pISPContext->cap_trim_rect.w/pISPContext->cap_deci;
    pISPContext->cap_output_size.h=pISPContext->cap_trim_rect.h/pISPContext->cap_deci;

    pISPContext->scale_trim_rect.w=pISPContext->cap_output_size.w;
    pISPContext->scale_trim_rect.h=pISPContext->cap_output_size.h;
    pISPContext->scale_trim_rect.x=(pISPContext->cap_output_size.w-pISPContext->scale_trim_rect.w)>>ISP_SHIFT_BIT1;
    pISPContext->scale_trim_rect.y=(pISPContext->cap_output_size.h-pISPContext->scale_trim_rect.h)>>ISP_SHIFT_BIT1;

    pISPContext->output_size.w=param_ptr->target_rect.w;
    pISPContext->output_size.h=param_ptr->target_rect.h;
    // preview mode rgb output format align two  
    pISPContext->output_size.h=((pISPContext->output_size.h>>ISP_SHIFT_BIT1)<<ISP_SHIFT_BIT1);    
    
    if(SCI_TRUE==pISPContext->is_full_view)
    {
        pISPContext->isp_buf_addr=param_ptr->disp_addr+(((((pISPContext->target_rect.w*pISPContext->full_screen_rect.y)<<ISP_SHIFT_BIT1)+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes
        s_offset_addr=(pISPContext->target_rect.w*pISPContext->full_screen_rect.y)<<ISP_SHIFT_BIT1; 
    } 
    else
    {
        pISPContext->isp_buf_addr=param_ptr->disp_addr;
    }

    pISPContext->isp_buf_size=param_ptr->disp_buf_size;

    pISPContext->rot_buf_addr=param_ptr->rot_buf_addr;
    pISPContext->rot_buf_size=param_ptr->rot_buf_size;
    
    pISPContext->lcd_id=param_ptr->disp_lcd_id;
    pISPContext->rotation = param_ptr->disp_rotation;
    // zoom callback
    pISPContext->dc_callback=param_ptr->suc_callback;  

    _ISP_UpdateCache(pISPContext->isp_buf_addr, pISPContext->isp_buf_size, ISP_OP_READ_WRITE);
    
    // calculate the scale coeff
    _ISP_CalcScaleParam();

    _ISP_InitializeBuf(ISP_SERVICE_PREVIEW);
    
    return rtn;
}
/****************************************************************************************/
// Description: Set the pareview parameter to chip and start preview
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_StartPreview(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();	
    AHB_MASTER_MODULE_T addr={0x00};
    ISP_RECT_T scale_trim_size={0x00};
    uint32 param = 0;

    IVsp_SetClk(pISPContext->dcam_clk);
    ISP_SelectISP();
	ISP_SoftReset();
    ISP_SetMode(ISP_MODE_IDLE);	

    //set skip num
    ISP_SetCAP(CAP_PRE_SKIP_CNT,pISPContext->preview_skip_frame_num);  
    // set deci num
    ISP_SetCAP(CAP_CAP_FRM_DECI, pISPContext->preview_deci_frame_num);          

    //set cap module  
    ISP_SetCAP(CAP_SYNC_POL,((pISPContext->vsync_polarity<<16)|(pISPContext->hsync_polarity)));
    ISP_SetCAP(CAP_FIFO_DATA_RATE,CAP_FIFO_FULL_SPEED);

    ISP_SetCAP(CAP_CAP_DECI,pISPContext->cap_deci);

    ISP_SetCAPInputFormat(CAP_INPUT_FORMAT_YUV);	    
    ISP_SetCAP(CAP_YUV_TYPE,pISPContext->input_image_pattern);

    //set cap trim size  equal to dcam input size  
    ISP_SetCAP(CAP_CAP_START,(((pISPContext->cap_trim_rect.y)<<16)|(pISPContext->cap_trim_rect.x*2)));	
    param = (pISPContext->cap_trim_rect.x+pISPContext->cap_trim_rect.w)*2-1;
    param |= ((pISPContext->cap_trim_rect.y+pISPContext->cap_trim_rect.h-1)<<16);
    ISP_SetCAP(CAP_CAP_END,param);

    //set scale module
    ISP_SetScaleInputFormat(pISPContext->input_image_format);
    //set cap output size equal to scale input size
    ISP_SetSrcSize(pISPContext->cap_output_size.w,pISPContext->cap_output_size.h);     
    //set scale trim size    
    scale_trim_size.x=pISPContext->scale_trim_rect.x;
    scale_trim_size.y=pISPContext->scale_trim_rect.y;
    scale_trim_size.w=pISPContext->scale_trim_rect.w;
    scale_trim_size.h=pISPContext->scale_trim_rect.h;		
    ISP_SetScaleTrimRect(scale_trim_size);
    ISP_EnableScaleTrim(pISPContext->scale_trim_eb);     
    //set scale outpur size equal to dcam output size
    ISP_SerDstSize(pISPContext->output_size.w,pISPContext->output_size.h);
 
    //set scale
    _ISP_SetScaleCoeff();
    
    //enable yuv2rgb module
    ISP_EnableYUV2RGB(pISPContext->yuv2rgb_eb);
    
    ISP_SetMode(ISP_MODE_PREVIEW);

    //set the currect fill buf
    addr.output_addr.y_addr=pISPContext->preview_buf[pISPContext->preview_buf_id].y_addr;	
    addr.output_addr.uv_addr=pISPContext->preview_buf[pISPContext->preview_buf_id].uv_addr;
    
    ISP_SetAhbMaster(&addr);
    ISP_AutoCopy();

    IVsp_RegisterIntFuncEx(ISP_TX_DONE_ID, _ISP_ISR_TrnasferDoneLow, _ISP_ISR_TrnasferDoneHigh);
    IVsp_ClearInt(ISP_TX_DONE_ID);
    IVsp_OpenInt(ISP_TX_DONE_ID);

    IVsp_RegisterIntFuncEx(CAP_FRAME_ERR_ID, _ISP_ISR_SensorFrameErrLow, _ISP_ISR_SensorFrameErrHigh);
    IVsp_ClearInt(CAP_FRAME_ERR_ID);
    IVsp_OpenInt(CAP_FRAME_ERR_ID);

    IVsp_RegisterIntFuncEx(ISP_BUFFER_OVF_ID, _ISP_ISR_PathFIFOOverflowedLow, _ISP_ISR_PathFIFOOverflowedHigh);
    IVsp_ClearInt(ISP_BUFFER_OVF_ID);
    IVsp_OpenInt(ISP_BUFFER_OVF_ID);
    
    IVsp_RegisterIntFuncEx(CAP_LINE_ERR_ID, _ISP_ISR_SensorLineErrLow, _ISP_ISR_SensorLineErrHigh);
    IVsp_ClearInt(CAP_LINE_ERR_ID);
    IVsp_OpenInt(CAP_LINE_ERR_ID);

    ISP_Start();
}

/****************************************************************************************/
// Description: Stop pareview and release isp
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_StopPreview(void)
{
    //SCI_TRACE_ISP:"ISP_SERVICE: _ISP_StopPreview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_3167_112_2_18_1_43_41_62,(uint8*)"");
    
    ISP_FreeISP();
}

/****************************************************************************************/
// Description: Stop the Capture and set data address is null
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_StopCapture(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();	
    
    ISP_FreeISP();

    pISPContext->output_addr.y_addr=PNULL;
    pISPContext->output_addr.uv_addr=PNULL;
    pISPContext->output_size.w=ISP_ZERO;
    pISPContext->output_size.h=ISP_ZERO;
    pISPContext->is_success=SCI_ERROR;    
    
    if(PNULL!=pISPContext->get_data)
    {
        pISPContext->get_data(pISPContext->output_addr, (pISPContext->output_size.w), (pISPContext->output_size.h));
        pISPContext->get_data=PNULL;
    }                            
    
}

/****************************************************************************************/
// Description: Stop the scale and set data address is null
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_StopScale(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();	
    
    ISP_FreeISP();

    pISPContext->output_addr.y_addr=PNULL;
    pISPContext->output_addr.uv_addr=PNULL;
    pISPContext->output_size.w=ISP_ZERO;
    pISPContext->output_size.h=ISP_ZERO;
    pISPContext->is_success=SCI_ERROR;    
    
    if(PNULL!=pISPContext->get_data)
    {
        pISPContext->get_data(pISPContext->output_addr, (pISPContext->output_size.w), (pISPContext->output_size.h));
        pISPContext->get_data=PNULL;
    }                            
    
}

/****************************************************************************************/
// Description: Set the buffer addr when isp mode
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_SERVICE_ID_E service: isp service id
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_InitializeBuf(ISP_SERVICE_ID_E service)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint32 buf_size = 0;
    uint32 i=0x00;

    switch(service)
    {
        case ISP_SERVICE_PREVIEW:
        {
            LCD_INFO_T lcd_info={0x00};

            LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

            buf_size=ISP_Align((lcd_info.lcd_width*(lcd_info.lcd_height+0x01)), ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);

            for(i=0x00; i<ISP_PREVIEW_BUF_NUM; i++)
            {
                if(((buf_size*ISP_TWO)*(i+ISP_ONE))<=pISPContext->isp_buf_size)
                {
                    pISPContext->preview_buf[i].y_addr=pISPContext->isp_buf_addr+buf_size*i*2;
                    pISPContext->preview_buf[i].uv_addr=pISPContext->isp_buf_addr+buf_size*(i*2+0x01);
                    pISPContext->preview_loop_buf_num=i;
                }
                else
                {
                    break ;
                }
            }
            pISPContext->preview_buf_id=ISP_PREVIEW_BUF_0;

            for(i=0x00; i<ISP_ROTATION_BUF_NUM; i++)
            {
                if(((buf_size*ISP_TWO)*(i+ISP_ONE))<=pISPContext->rot_buf_size)
                {
                    pISPContext->rotation_buf[i].y_addr=pISPContext->rot_buf_addr+buf_size*i;
                    pISPContext->rotation_buf[i].uv_addr=pISPContext->rot_buf_addr+buf_size*(i+0x01);
                    pISPContext->rotation_loop_buf_num=i;
                }
                else
                {
                    break ;
                }
            }
            pISPContext->rotation_buf_id=ISP_ROTATION_BUF_0;           

            break;
        }

        case ISP_SERVICE_CAPTURE:
        {  
            buf_size=(((pISPContext->output_size.w*pISPContext->output_size.h+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes
            pISPContext->output_addr.y_addr=(((pISPContext->isp_buf_addr+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes;
            pISPContext->output_addr.uv_addr=pISPContext->output_addr.y_addr+buf_size;
            break;
        }

        case ISP_SERVICE_REVIEW:
        { 
            pISPContext->output_addr.y_addr=(((pISPContext->isp_buf_addr+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes;
            if((ISP_DATA_YUV422==pISPContext->output_image_format)||(ISP_DATA_YUV420==pISPContext->output_image_format))
            {
                if(ISP_SERVICE_SCALE_NORMAL==pISPContext->scale_mode)
                {
                    buf_size = (((pISPContext->output_size.w*pISPContext->output_size.h+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes
                }
                else
                {
                    buf_size = (((pISPContext->output_size.w*(((pISPContext->slice_line*pISPContext->output_size.h)/pISPContext->scale_trim_rect.h)+0x03)+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes
                }            
               
                pISPContext->output_addr.uv_addr=pISPContext->output_addr.y_addr+buf_size;
            }
            else
            {
                pISPContext->output_addr.uv_addr=PNULL;
            }
            pISPContext->review_display_buf_addr.y_addr=pISPContext->output_addr.y_addr;
            pISPContext->review_display_buf_addr.uv_addr=pISPContext->output_addr.uv_addr;
            break;
        }        

        case ISP_SERVICE_MOTION_JPEG:
        {
		
            // mjpeg review rgb buf
            buf_size = (((pISPContext->disp_rect.w*pISPContext->disp_rect.h+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes            
            pISPContext->mjpg_info.review_rgb_addr.y_addr=(((pISPContext->mjpg_info.review_rgb_addr.y_addr+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT);  
            pISPContext->mjpg_info.review_rgb_addr.uv_addr=pISPContext->mjpg_info.review_rgb_addr.y_addr+buf_size;
			
   	    
		buf_size = ((pISPContext->mjpg_info.capture_yuv_size/2+255)>>8)<<8;
	     //capture size is larger than scale size. from the head of the memory.
	     if(pISPContext->mjpg_info.capture_size.w*pISPContext->mjpg_info.capture_size.h>pISPContext->mjpg_info.jpeg_target_size.w* pISPContext->mjpg_info.jpeg_target_size.h)
	     {
	              
			pISPContext->mjpg_info.capture_yuv_addr.y_addr=(((pISPContext->mjpg_info.capture_yuv_addr.y_addr+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT);
            		pISPContext->mjpg_info.capture_yuv_addr.uv_addr=pISPContext->mjpg_info.capture_yuv_addr.y_addr+buf_size;

            		pISPContext->mjpg_info.jpeg_yuv_addr.y_addr= pISPContext->mjpg_info.capture_yuv_addr.y_addr;
            		pISPContext->mjpg_info.jpeg_yuv_addr.uv_addr=pISPContext->mjpg_info.capture_yuv_addr.uv_addr;
	     }
	     else
	     {
	     	       uint32 real_size = ((pISPContext->mjpg_info.capture_size.w*pISPContext->mjpg_info.capture_size.h+255)>>8)<<8;
			uint32 base_addr = ((pISPContext->mjpg_info.capture_yuv_addr.y_addr+255)>>8)<<8;
			uint32 total_size = (pISPContext->mjpg_info.capture_yuv_size>>8)<<8;

			pISPContext->mjpg_info.capture_yuv_addr.uv_addr= base_addr + total_size-real_size;			
			pISPContext->mjpg_info.capture_yuv_addr.y_addr=base_addr+buf_size-real_size;	

            		pISPContext->mjpg_info.jpeg_yuv_addr.y_addr= pISPContext->mjpg_info.capture_yuv_addr.y_addr;
            		pISPContext->mjpg_info.jpeg_yuv_addr.uv_addr=pISPContext->mjpg_info.capture_yuv_addr.uv_addr;

            		pISPContext->mjpg_info.jpeg_yuv_addr.y_addr= base_addr;
            		pISPContext->mjpg_info.jpeg_yuv_addr.uv_addr=base_addr+buf_size;
	    }		

            break;
        }

        case ISP_SERVICE_UPCC:
        {
            buf_size = (((pISPContext->output_size.w*pISPContext->output_size.h+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes
            pISPContext->output_addr.y_addr=(((pISPContext->isp_buf_addr+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT);
            pISPContext->output_addr.uv_addr=pISPContext->output_addr.y_addr+buf_size;
            break;
        }        

        case ISP_SERVICE_SCALE:
        {
            pISPContext->output_addr.y_addr=(((pISPContext->isp_buf_addr+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes; 

            if(ISP_DATA_RGB565!=pISPContext->output_image_format)
            {
                if(ISP_SERVICE_SCALE_NORMAL==pISPContext->scale_mode)
                {
                    buf_size = (((pISPContext->output_size.w*pISPContext->output_size.h+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes
                }
                else
                {
                    buf_size = (((pISPContext->output_size.w*(((pISPContext->slice_line*pISPContext->output_size.h)/pISPContext->scale_trim_rect.h)+0x03)+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes
                }
                pISPContext->output_addr.uv_addr=pISPContext->output_addr.y_addr+buf_size;
            }
            else
            {
                pISPContext->output_addr.uv_addr=PNULL;
            }            
            break;
        }         

        default:
            //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_InitializeBuf default %d line error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_3395_112_2_18_1_43_41_63,(uint8*)"d", __LINE__);
            break;
    }

    return SCI_SUCCESS;
    
}

/****************************************************************************************/
// Description: Set jpg capture buffer limit size
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      uint32 jpg_buf_size: the buffer size of capture jpg
// output parameter:
//                      None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_SetJpgBufLimitSize(uint32 jpg_buf_size)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint32 jpg_bank_num=0x00;

    jpg_bank_num=jpg_buf_size/ISP_JPEG_BANK_SIZE;

    if(ISP_OXFF<jpg_bank_num)
    {
        pISPContext->jpg_buf_limit=ISP_ZERO;
    }
    else
    {
        pISPContext->jpg_buf_limit=jpg_bank_num;
    }

    //SCI_TRACE_ISP:"ISP_SERVICE: _ISP_SetJpgBufLimitSize img format : %d, %d, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_3429_112_2_18_1_43_42_64,(uint8*)"ddd", jpg_buf_size, jpg_bank_num, pISPContext->jpg_buf_limit);

    return ;
}

/****************************************************************************************/
// Description: Save the capture parameter of dc to isp service com parmaeter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_JPEG_PARAM_T *param_ptr: the pointer of jpeg param
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/

#define ISP_COMPARE_LEVEL 32

LOCAL uint32 _ISP_SetCaptureParam(ISP_JPEG_PARAM_T *param_ptr)
{
    uint32 rtn=SCI_SUCCESS;
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    ISP_RETURN_IF_ERR(_ISP_SetDcamClk(param_ptr->pclk), SCI_ERROR, ("ISP_SERVICE: _ISP_SetCaptureParam pclk:%d, %d line error",param_ptr->pclk, __LINE__));

    pISPContext->capture_skip_frame_num=param_ptr->skip_num;
    pISPContext->input_image_format=param_ptr->sensor_data_format;
    pISPContext->output_image_format=param_ptr->output_data_format;
    pISPContext->isp_buf_addr=param_ptr->capture_addr;
    pISPContext->input_buf_size=param_ptr->capture_buf_size;
    pISPContext->get_data=param_ptr->get_cap_data;
    pISPContext->isp_feature_enable=SCI_TRUE;
    pISPContext->yuv2rgb_eb=SCI_FALSE;
    if(ISP_DATA_RGB565==pISPContext->output_image_format)
    {
        pISPContext->yuv2rgb_eb=SCI_TRUE;
    }

    if(!((ISP_DATA_YUV422==pISPContext->input_image_format)
        ||(ISP_DATA_JPEG==pISPContext->input_image_format)))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: _ISP_SetCaptureParam img format is %x  %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_3470_112_2_18_1_43_42_65,(uint8*)"dd", pISPContext->input_image_format, __LINE__);
        return SCI_ERROR;
    }

    // digital zoom capture, if capture size equal to 640*480 capture 320*240 yuv then dc interplate to 640X480
    // if capture size more than 640X480, sensor do scale output size is half capture size
    SCI_MEMCPY((void*)&(param_ptr->zoom_rect), (void*)&(pISPContext->zoom_rect), sizeof(ISP_RECT_T));

    if(SCI_FALSE==pISPContext->is_zoom)
    {
        SCI_MEMCPY((void*)&(pISPContext->cap_trim_rect), (void*)&(param_ptr->input_rect), sizeof(ISP_RECT_T));
    }
    else if(SCI_TRUE==pISPContext->is_zoom)
    {
    	uint32 factor1 = 0, factor2 = 0;
		//SCI_TRACE_ISP:"ISP_SERVICE: _ISP_SetCaptureParam, ow:%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_3485_112_2_18_1_43_42_66,(uint8*)"d", param_ptr->output_size.w);
		//SCI_TRACE_ISP:"ISP_SERVICE: _ISP_SetCaptureParam, oh:%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_3486_112_2_18_1_43_42_67,(uint8*)"d", param_ptr->output_size.h);
		//SCI_TRACE_ISP:"ISP_SERVICE: _ISP_SetCaptureParam, zw:%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_3487_112_2_18_1_43_42_68,(uint8*)"d", pISPContext->zoom_rect.w);
		//SCI_TRACE_ISP:"ISP_SERVICE: _ISP_SetCaptureParam, zh:%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_3488_112_2_18_1_43_42_69,(uint8*)"d", pISPContext->zoom_rect.h);

		factor1 = (param_ptr->output_size.w*pISPContext->zoom_rect.h);
		factor2 = (param_ptr->output_size.h*pISPContext->zoom_rect.w);

		if(factor2 != 0)
		{
			factor1 = factor1 * ISP_COMPARE_LEVEL / factor2;
			
		}
		else
		{
			return SCI_ERROR;
		}
		//SCI_TRACE_LOW:"ISP_SERVICE: factor1: %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_3502_112_2_18_1_43_42_70,(uint8*)"d", factor1);
		
		if(((ISP_COMPARE_LEVEL-1) <=factor1) && (factor1 <=(ISP_COMPARE_LEVEL + 1)))
		{// equal to 
            //SCI_TRACE_ISP:"ISP_SERVICE: _ISP_SetCaptureParam, cap trim 2"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_3506_112_2_18_1_43_42_71,(uint8*)"");
            
            if((pISPContext->input_size.w==param_ptr->input_size.w)
                &&(pISPContext->input_size.h==param_ptr->input_size.h))
            {
                pISPContext->cap_trim_rect.x=pISPContext->zoom_rect.x;
                pISPContext->cap_trim_rect.y=pISPContext->zoom_rect.y;
                pISPContext->cap_trim_rect.w=pISPContext->zoom_rect.w;  
                pISPContext->cap_trim_rect.h=pISPContext->zoom_rect.h;
            }
            else
            {
                pISPContext->cap_trim_rect.w=(pISPContext->zoom_rect.w*param_ptr->input_rect.w)/pISPContext->preview_input_rect.w;
                pISPContext->cap_trim_rect.h=(pISPContext->zoom_rect.h*param_ptr->input_rect.h)/pISPContext->preview_input_rect.h;
                pISPContext->cap_trim_rect.x=param_ptr->input_rect.x+((param_ptr->input_rect.w-pISPContext->cap_trim_rect.w)>>ISP_SHIFT_BIT1);
                pISPContext->cap_trim_rect.y=param_ptr->input_rect.y+((param_ptr->input_rect.h-pISPContext->cap_trim_rect.h)>>ISP_SHIFT_BIT1);
            }
        }
        else if((param_ptr->output_size.w*pISPContext->zoom_rect.h)<(param_ptr->output_size.h*pISPContext->zoom_rect.w))
        {// depend on horzontial
            //SCI_TRACE_ISP:"ISP_SERVICE: _ISP_SetCaptureParam, cap trim 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_3526_112_2_18_1_43_42_72,(uint8*)"");
            pISPContext->cap_trim_rect.w=(pISPContext->zoom_rect.w*param_ptr->input_rect.w)/pISPContext->preview_input_rect.w;
            pISPContext->cap_trim_rect.h=(pISPContext->cap_trim_rect.w*param_ptr->input_rect.h)/param_ptr->input_rect.w;
            pISPContext->cap_trim_rect.x=param_ptr->input_rect.x+((param_ptr->input_rect.w-pISPContext->cap_trim_rect.w)>>ISP_SHIFT_BIT1);
            pISPContext->cap_trim_rect.y=param_ptr->input_rect.y+((param_ptr->input_rect.h-pISPContext->cap_trim_rect.h)>>ISP_SHIFT_BIT1);
        }
        else 
        {// depend on vertical
            //SCI_TRACE_ISP:"ISP_SERVICE: _ISP_SetCaptureParam, cap trim 1"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_3534_112_2_18_1_43_42_73,(uint8*)"");
            pISPContext->cap_trim_rect.h=(pISPContext->zoom_rect.h*param_ptr->input_rect.h)/pISPContext->preview_input_rect.h;
            pISPContext->cap_trim_rect.w=(param_ptr->input_rect.w*pISPContext->cap_trim_rect.h)/param_ptr->input_rect.h;
            pISPContext->cap_trim_rect.x=param_ptr->input_rect.x+((param_ptr->input_rect.w-pISPContext->cap_trim_rect.w)>>ISP_SHIFT_BIT1);
            pISPContext->cap_trim_rect.y=param_ptr->input_rect.y+((param_ptr->input_rect.h-pISPContext->cap_trim_rect.h)>>ISP_SHIFT_BIT1);
        }
#if 0
        else   //the same rect between cap rect and disp rect
        {// equal to 
            //SCI_TRACE_ISP:"ISP_SERVICE: _ISP_SetCaptureParam, cap trim 2"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_3543_112_2_18_1_43_42_74,(uint8*)"");
            
            if((pISPContext->input_size.w==param_ptr->input_size.w)
                &&(pISPContext->input_size.h==param_ptr->input_size.h))
            {
                pISPContext->cap_trim_rect.x=pISPContext->zoom_rect.x;
                pISPContext->cap_trim_rect.y=pISPContext->zoom_rect.y;
                pISPContext->cap_trim_rect.w=pISPContext->zoom_rect.w;  
                pISPContext->cap_trim_rect.h=pISPContext->zoom_rect.h;
            }
            else
            {
                pISPContext->cap_trim_rect.w=(pISPContext->zoom_rect.w*param_ptr->input_rect.w)/pISPContext->preview_input_rect.w;
                pISPContext->cap_trim_rect.h=(pISPContext->zoom_rect.h*param_ptr->input_rect.h)/pISPContext->preview_input_rect.h;
                pISPContext->cap_trim_rect.x=param_ptr->input_rect.x+((param_ptr->input_rect.w-pISPContext->cap_trim_rect.w)>>ISP_SHIFT_BIT1);
                pISPContext->cap_trim_rect.y=param_ptr->input_rect.y+((param_ptr->input_rect.h-pISPContext->cap_trim_rect.h)>>ISP_SHIFT_BIT1);
            }
        }
#endif
    }

    _ISP_CalcCapDeci(pISPContext->cap_trim_rect.w,pISPContext->cap_trim_rect.h,param_ptr->output_size.w,param_ptr->output_size.h);
    
    pISPContext->cap_trim_rect.x=ISP_Align(pISPContext->cap_trim_rect.x, ISP_ZERO, ISP_TWO);
    pISPContext->cap_trim_rect.w=ISP_Align(pISPContext->cap_trim_rect.w, ISP_TWO, ISP_TWO);
    pISPContext->cap_trim_rect.h=ISP_Align(pISPContext->cap_trim_rect.h, ISP_TWO, ISP_TWO);

    pISPContext->cap_output_size.w=pISPContext->cap_trim_rect.w/pISPContext->cap_deci; 
    pISPContext->cap_output_size.h=pISPContext->cap_trim_rect.h/pISPContext->cap_deci;

    pISPContext->scale_trim_rect.w=pISPContext->cap_output_size.w;
    pISPContext->scale_trim_rect.h=pISPContext->cap_output_size.h;   
    pISPContext->scale_trim_rect.x=ISP_Align((pISPContext->cap_output_size.w-pISPContext->scale_trim_rect.w), ISP_ZERO, ISP_TWO);
    pISPContext->scale_trim_rect.y=ISP_Align((pISPContext->cap_output_size.h-pISPContext->scale_trim_rect.h), ISP_ZERO, ISP_TWO);

    SCI_MEMCPY((void*)&(param_ptr->input_rect), (void*)&(pISPContext->cap_trim_rect), sizeof(ISP_RECT_T));

    if(SCI_FALSE==pISPContext->is_zoom)
    {
        SCI_MEMCPY((void*)&(pISPContext->output_size), (void*)&(param_ptr->output_size), sizeof(ISP_SIZE_T));
    }
    else
    {
        if((ISP_GetMaxScaleWidth()>=param_ptr->output_size.w)
            &&(ISP_GetMaxScaleHeight()>=param_ptr->output_size.h))
        {
            SCI_MEMCPY((void*)&(pISPContext->output_size), (void*)&(param_ptr->output_size), sizeof(ISP_SIZE_T));
        }
        else
        {
            pISPContext->output_size.w=pISPContext->scale_trim_rect.w;
            pISPContext->output_size.h=pISPContext->scale_trim_rect.h;
        }
    }

    if(ISP_DATA_JPEG!=pISPContext->input_image_format)
    {
        pISPContext->scale_mode=ISP_SERVICE_SCALE_NORMAL;/*lint !e64*/
        _ISP_CalcScaleParam();
    }
    else
    {
        _ISP_SetJpgBufLimitSize(pISPContext->input_buf_size);
    }

    _ISP_UpdateCache(pISPContext->isp_buf_addr, pISPContext->input_buf_size, ISP_OP_WRITE);
    
    _ISP_InitializeBuf(ISP_SERVICE_CAPTURE);   

    return rtn;
	
}

/****************************************************************************************/
// Description: Set analog tv preview parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_SetATVPreviewParam(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    pISPContext->capture_skip_frame_num=0x00;
    pISPContext->yuv2rgb_eb=SCI_FALSE;
    pISPContext->is_tx_done=SCI_FALSE;

    SetATVPreviewParam();
 
    // calculate the scale coeff
    _ISP_CalcScaleParam();

    pISPContext->output_addr.y_addr=(((pISPContext->isp_buf_addr+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes;
    pISPContext->output_addr.uv_addr=((((pISPContext->output_addr.y_addr+pISPContext->output_size.w*pISPContext->output_size.h)+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes;

}

/****************************************************************************************/
// Description: Set the capture jpg data stream parameter to chip and start capture the jpg data
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_StartCaptureJPEG(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();	
    AHB_MASTER_MODULE_T addr={0x00};

    IVsp_SetClk(pISPContext->dcam_clk);
    ISP_SelectISP();

    ISP_SoftReset();

    ISP_SetMode(ISP_MODE_IDLE);	

    //set skip frame number
    ISP_SetCAP(CAP_CAP_JPEG_DROP_NUM, pISPContext->capture_skip_frame_num);
    ISP_SetCAP(CAP_CAP_JPEG_GET_NUM,0x00);

    //set vsyn and hsyn polarity
    ISP_SetCAP(CAP_SYNC_POL,((pISPContext->vsync_polarity<<16)|(pISPContext->hsync_polarity)));

    //set cap fifo speed
    ISP_SetCAP(CAP_FIFO_DATA_RATE,CAP_FIFO_FULL_SPEED);

    //set jpeg buffer size 0x00->no limit
    ISP_SetCAP(CAP_MEM_SIZE, pISPContext->jpg_buf_limit);

    //disable scale
    ISP_BypassScale(SCI_TRUE);	

    ISP_EnableScaleTrim(SCI_FALSE);    

    //disable YUV2RGB	
    ISP_EnableYUV2RGB(SCI_FALSE);

    //set isp capture mode
    ISP_SetMode(ISP_MODE_CAPTURE);	

    //set isp output address
    addr.output_addr.y_addr=pISPContext->output_addr.y_addr;	
    addr.output_addr.uv_addr=pISPContext->output_addr.uv_addr;
    ISP_SetAhbMaster(&addr);

    //register interrupt handle	
    IVsp_RegisterIntFuncEx(ISP_TX_DONE_ID, _ISP_ISR_TrnasferDoneLow, _ISP_ISR_TrnasferDoneHigh);
    IVsp_ClearInt(ISP_TX_DONE_ID);
    IVsp_OpenInt(ISP_TX_DONE_ID);

    IVsp_RegisterIntFuncEx(JPEG_BUF_OVF_ID,_ISP_ISR_JpegBufferverflowedLow, _ISP_ISR_JpegBufferverflowedHigh);    
    IVsp_ClearInt(JPEG_BUF_OVF_ID);
    IVsp_OpenInt(JPEG_BUF_OVF_ID);        

    IVsp_RegisterIntFuncEx(CAP_FRAME_ERR_ID, _ISP_ISR_SensorFrameErrLow, _ISP_ISR_SensorFrameErrHigh);
    IVsp_ClearInt(CAP_FRAME_ERR_ID);
    IVsp_OpenInt(CAP_FRAME_ERR_ID);

    //set cap input format
    ISP_SetCAPInputFormat(CAP_INPUT_FORMAT_JPEG);	

    ISP_ForceCopy();
    ISP_AutoCopy();

    ISP_Start();
}
/****************************************************************************************/
// Description: Set the capture parameter to chip and start capture yuv/rgb565s data
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_StartCaptureNormal(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    AHB_MASTER_MODULE_T addr={0x00};
    ISP_RECT_T scale_trim_rect={0x00};
    uint32 param=0x00;

    IVsp_SetClk(pISPContext->dcam_clk);
    ISP_SelectISP();

    ISP_SoftReset();
    
    ISP_SetMode(ISP_MODE_IDLE);	

    //set skip num
    ISP_SetCAP(CAP_PRE_SKIP_CNT,pISPContext->capture_skip_frame_num);

    //set vsync and hsync polarity
    ISP_SetCAP(CAP_SYNC_POL,((pISPContext->vsync_polarity<<16)|(pISPContext->hsync_polarity)));

    //set yuv pattern
    ISP_SetCAP(CAP_YUV_TYPE,pISPContext->input_image_pattern);

    //set cap fifo speed
    ISP_SetCAP(CAP_FIFO_DATA_RATE,CAP_FIFO_FULL_SPEED);

    //set cap deci coeff
    ISP_SetCAP(CAP_CAP_DECI,pISPContext->cap_deci);

    //set cap input format
    ISP_SetCAPInputFormat(pISPContext->input_image_format);

    //set cap start
    ISP_SetCAP(CAP_CAP_START,(((pISPContext->cap_trim_rect.y)<<16)|(pISPContext->cap_trim_rect.x*2)));

    //set cap end
    param = (pISPContext->cap_trim_rect.x+pISPContext->cap_trim_rect.w)*2-1;
    param |= ((pISPContext->cap_trim_rect.y+pISPContext->cap_trim_rect.h-1)<<16);
    ISP_SetCAP(CAP_CAP_END,param);

    //set scale module
    ISP_SetScaleInputFormat(pISPContext->input_image_format);

    //set isp scale src size	
    ISP_SetSrcSize(pISPContext->cap_output_size.w,pISPContext->cap_output_size.h);    

    scale_trim_rect.x=pISPContext->scale_trim_rect.x;
    scale_trim_rect.y=pISPContext->scale_trim_rect.y;
    scale_trim_rect.w=pISPContext->scale_trim_rect.w;
    scale_trim_rect.h=pISPContext->scale_trim_rect.h;		
    ISP_SetScaleTrimRect(scale_trim_rect);
    ISP_EnableScaleTrim(pISPContext->scale_trim_eb);

    //ser isp dst size
    ISP_SerDstSize(pISPContext->output_size.w,pISPContext->output_size.h);

    //set scale
    _ISP_SetScaleCoeff();

    if(ISP_ServiceGetAtvIsEnable())
    {
        ISP_ARV_CTRL_T atv_ctrl={0x00};
        atv_ctrl.is_eb=pISPContext->threshold_eb;
        atv_ctrl.mode=pISPContext->threshold_mode;/*lint !e64*/
        atv_ctrl.start=pISPContext->threshold_start;
        atv_ctrl.end=pISPContext->threshold_end;
        ISP_SetATVMode(&atv_ctrl);
    }

    //disable YUV2RGB	
    ISP_EnableYUV2RGB(pISPContext->yuv2rgb_eb);
    
    //set isp capture mode
    ISP_SetMode(ISP_MODE_CAPTURE);		

    //set isp output address
    addr.output_addr.y_addr  = pISPContext->output_addr.y_addr;	
    addr.output_addr.uv_addr = pISPContext->output_addr.uv_addr;
    ISP_SetAhbMaster(&addr);

    //update cap reg
    ISP_ForceCopy();
    ISP_AutoCopy();
  
    IVsp_RegisterIntFuncEx(ISP_TX_DONE_ID, _ISP_ISR_TrnasferDoneLow, _ISP_ISR_TrnasferDoneHigh);
    IVsp_ClearInt(ISP_TX_DONE_ID);
    IVsp_OpenInt(ISP_TX_DONE_ID);

    IVsp_RegisterIntFuncEx(ISP_BUFFER_OVF_ID, _ISP_ISR_PathFIFOOverflowedLow, _ISP_ISR_PathFIFOOverflowedHigh);
    IVsp_ClearInt(ISP_BUFFER_OVF_ID);
    IVsp_OpenInt(ISP_BUFFER_OVF_ID);
    
    IVsp_RegisterIntFuncEx(CAP_LINE_ERR_ID, _ISP_ISR_SensorLineErrLow, _ISP_ISR_SensorLineErrHigh);
    IVsp_ClearInt(CAP_LINE_ERR_ID);
    IVsp_OpenInt(CAP_LINE_ERR_ID);

    IVsp_RegisterIntFuncEx(CAP_FRAME_ERR_ID, _ISP_ISR_SensorFrameErrLow, _ISP_ISR_SensorFrameErrHigh);
    IVsp_ClearInt(CAP_FRAME_ERR_ID);
    IVsp_OpenInt(CAP_FRAME_ERR_ID);

    ISP_Start();
}

/****************************************************************************************/
// Description: Start capture that sel the yuv mode jpg mode or rgb565
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_StartCapture(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    if(ISP_DATA_JPEG==pISPContext->input_image_format)
    {
        _ISP_StartCaptureJPEG();
    }
    else if(ISP_DATA_YUV422==pISPContext->input_image_format)
    {
        _ISP_StartCaptureNormal();
    }    
}

/****************************************************************************************/
// Description: Save the Review parameter of dc to isp service com parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_REVIEW_PARAM_T *param_ptr: the pointer of review parameter
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_SetReviewParam(ISP_REVIEW_PARAM_T *param_ptr)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    pISPContext->isp_feature_enable=SCI_TRUE;
    pISPContext->dcam_clk=IVSP_CLK_48M;
    
    SCI_MEMCPY((void*)&(pISPContext->cap_output_size),(void*)&(param_ptr->input_size),sizeof(ISP_SIZE_T));
    SCI_MEMCPY((void*)&(pISPContext->scale_trim_rect),(void*)&(param_ptr->input_rect),sizeof(ISP_RECT_T));
    SCI_MEMCPY((void*)&(pISPContext->disp_rect),(void*)&(param_ptr->disp_rect),sizeof(ISP_RECT_T));
    SCI_MEMCPY((void*)&(pISPContext->target_rect),(void*)&(param_ptr->target_rect),sizeof(ISP_RECT_T));
    SCI_MEMCPY((void*)&(pISPContext->rotation),(void*)&(param_ptr->disp_rotation),sizeof(ISP_ROTATION_E));

    SCI_MEMCPY((void*)&(pISPContext->input_addr),(void*)&(param_ptr->src_addr),sizeof(ISP_DATA_ADDR_T));

    _ISP_SetUpdateCacheSize(pISPContext->cap_output_size.w, pISPContext->cap_output_size.h, param_ptr->input_format);
    _ISP_UpdateCache(pISPContext->input_addr.y_addr, pISPContext->update_cache.y_len, ISP_OP_READ);
    _ISP_UpdateCache(pISPContext->input_addr.uv_addr, pISPContext->update_cache.uv_len, ISP_OP_READ);
    _ISP_UpdateCache(pISPContext->input_addr.v_addr, pISPContext->update_cache.uv_len, ISP_OP_READ);


    if(pISPContext->is_dir_display)
    {
        pISPContext->target_rect.w=param_ptr->input_rect.w*ISP_GetMaxScaleUpCoeff();
        pISPContext->target_rect.h=param_ptr->input_rect.h*ISP_GetMaxScaleUpCoeff();

        if(param_ptr->target_rect.w<pISPContext->target_rect.w)
        {
            pISPContext->target_rect.w=param_ptr->target_rect.w;
        }

        if(param_ptr->target_rect.h<pISPContext->target_rect.h)
        {
            pISPContext->target_rect.h=param_ptr->target_rect.h;
        }        
        
        if((ISP_ROTATION_90==param_ptr->disp_rotation)
            ||(ISP_ROTATION_270==param_ptr->disp_rotation))
        {
            pISPContext->target_rect.x+=((param_ptr->target_rect.h-pISPContext->target_rect.h)>>ISP_SHIFT_BIT1);
            pISPContext->target_rect.y+=((param_ptr->target_rect.w-pISPContext->target_rect.w)>>ISP_SHIFT_BIT1);            
        }
        else
        {
            pISPContext->target_rect.x+=((param_ptr->target_rect.w-pISPContext->target_rect.w)>>ISP_SHIFT_BIT1);
            pISPContext->target_rect.y+=((param_ptr->target_rect.h-pISPContext->target_rect.h)>>ISP_SHIFT_BIT1);
        }

    }

    if(SCI_FALSE==ISP_CheckScaleUpAbility(pISPContext->scale_trim_rect.w, pISPContext->scale_trim_rect.h, pISPContext->target_rect.w, pISPContext->target_rect.h))
    {
        pISPContext->target_rect.w=param_ptr->input_rect.w;
        pISPContext->target_rect.h=param_ptr->input_rect.h; 
        
        if(param_ptr->target_rect.w<pISPContext->target_rect.w)
        {
            pISPContext->target_rect.w=param_ptr->target_rect.w;
        }

        if(param_ptr->target_rect.h<pISPContext->target_rect.h)
        {
            pISPContext->target_rect.h=param_ptr->target_rect.h;
        }        
        
        pISPContext->target_rect.x+=((param_ptr->target_rect.w-pISPContext->target_rect.w)>>ISP_SHIFT_BIT1);
        pISPContext->target_rect.y+=((param_ptr->target_rect.h-pISPContext->target_rect.h)>>ISP_SHIFT_BIT1);
    }

    pISPContext->output_size.w=pISPContext->target_rect.w;
    pISPContext->output_size.h=pISPContext->target_rect.h;  

    if(SCI_TRUE==pISPContext->is_full_view)
    {
        SCI_MEMSET((void *)param_ptr->disp_addr, 0x00, param_ptr->disp_buf_size); 
        pISPContext->isp_buf_addr=((((param_ptr->disp_addr+(( pISPContext->disp_rect.w*param_ptr->target_rect.y)<<ISP_SHIFT_BIT1))+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes
        pISPContext->review_display_buf_addr.y_addr=(pISPContext->isp_buf_addr-((pISPContext->disp_rect.w*param_ptr->target_rect.y)<<ISP_SHIFT_BIT1));
    }
    else
    {
        pISPContext->isp_buf_addr=param_ptr->disp_addr;
        pISPContext->review_display_buf_addr.y_addr=param_ptr->disp_addr;
    }

    _ISP_UpdateCache(param_ptr->disp_addr, param_ptr->disp_buf_size, ISP_OP_WRITE);

    pISPContext->input_image_format=param_ptr->input_format;
    pISPContext->output_image_format=param_ptr->disp_format;
    pISPContext->disp_format=param_ptr->disp_format;
    pISPContext->yuv2rgb_eb=_ISP_GetIsYuv2Rgb(param_ptr->disp_format);
    pISPContext->lcd_id=param_ptr->disp_lcd_id;
    pISPContext->rotation = param_ptr->disp_rotation;

    pISPContext->dc_callback=param_ptr->suc_callback;    

    pISPContext->invalidate_mode=param_ptr->mode;
    pISPContext->is_copy_display_data=param_ptr->is_copy_display_data;

    _ISP_CalcSubSampleMode();
 
    // calculate the scale param
    _ISP_CalcScaleParam();
    pISPContext->is_bypass_scale=SCI_FALSE;
    pISPContext->rot_buf_addr=param_ptr->rot_addr.y_addr;
    pISPContext->rot_buf_size=param_ptr->rot_buf_len.y_len;
    pISPContext->rotation_buf[ISP_ROTATION_BUF_0].y_addr=ISP_Align(param_ptr->rot_addr.y_addr, ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
    pISPContext->rotation_buf[ISP_ROTATION_BUF_0].uv_addr=ISP_Align(param_ptr->rot_addr.uv_addr, ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
    pISPContext->rotation_loop_buf_num=ISP_ZERO;
    pISPContext->rotation_buf_id=ISP_ROTATION_BUF_0;            
    _ISP_UpdateCache(pISPContext->rot_buf_addr, pISPContext->rot_buf_size, ISP_OP_WRITE);
    
    // set review buf
    _ISP_InitializeBuf(ISP_SERVICE_REVIEW);    

    _ISP_SetUpdateCacheSize(pISPContext->output_size.w, pISPContext->output_size.h, pISPContext->input_image_format);
    _ISP_UpdateCache(pISPContext->input_addr.y_addr, pISPContext->update_cache.y_len, ISP_OP_READ);
    _ISP_UpdateCache(pISPContext->input_addr.uv_addr, pISPContext->update_cache.uv_len, ISP_OP_READ);
    _ISP_UpdateCache(pISPContext->input_addr.v_addr, pISPContext->update_cache.uv_len, ISP_OP_READ);

    pISPContext->isp_feature_enable=SCI_TRUE;

}

/****************************************************************************************/
// Description: Save the Review parameter of review analog tv data
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_SetATVReviewParam(void)
{
    SetTVReviewParam(); 
    // calculate the scale param
    _ISP_CalcScaleParam(); 
    
    return ;
}

/****************************************************************************************/
// Description: Dump the review parameter to chip  and start review 
// Global resource dependence: 
// Author: Jianping.wang
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_StartReview(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();	
    AHB_MASTER_MODULE_T addr={0x00};
    ISP_RECT_T scale_trim_rect={0x00};

    IVsp_SetClk(pISPContext->dcam_clk);
    ISP_SelectISP();

    ISP_SoftReset();

    ISP_SetMode(ISP_MODE_IDLE);	

     //set scale module
    ISP_SetScaleInputFormat(pISPContext->input_image_format);
    ISP_SetSubSampleMode(pISPContext->subsample_eb, pISPContext->subsample_mode);
    ISP_SetSrcSize(pISPContext->cap_output_size.w,pISPContext->cap_output_size.h);

    scale_trim_rect.x = pISPContext->scale_trim_rect.x;
    scale_trim_rect.y = pISPContext->scale_trim_rect.y;
    scale_trim_rect.w = pISPContext->scale_trim_rect.w;
    scale_trim_rect.h = pISPContext->scale_trim_rect.h;
    ISP_SetScaleTrimRect(scale_trim_rect);
    ISP_EnableScaleTrim(pISPContext->scale_trim_eb);      

    ISP_SerDstSize(pISPContext->output_size.w,pISPContext->output_size.h);	

    ISP_4MultipleScaleUpEnable();   
    
    _ISP_SetScaleCoeff();

    ISP_SetMode(ISP_MODE_REVIEW);

    //enable yuv2rgb module
    ISP_EnableYUV2RGB(pISPContext->yuv2rgb_eb);
    addr.input_addr.y_addr   = pISPContext->input_addr.y_addr;
    addr.input_addr.uv_addr  = pISPContext->input_addr.uv_addr;
    addr.input_addr.v_addr  = pISPContext->input_addr.v_addr;

    addr.output_addr.y_addr  = pISPContext->output_addr.y_addr; 	
    addr.output_addr.uv_addr = pISPContext->output_addr.uv_addr;

    ISP_SetAhbMaster(&addr);	
    ISP_AutoCopy();

    IVsp_RegisterIntFuncEx(ISP_TX_DONE_ID, _ISP_ISR_TrnasferDoneLow, _ISP_ISR_TrnasferDoneHigh);
    IVsp_ClearInt(ISP_TX_DONE_ID);
    IVsp_OpenInt(ISP_TX_DONE_ID);

    IVsp_RegisterIntFuncEx(ISP_BUFFER_OVF_ID, _ISP_ISR_PathFIFOOverflowedLow, _ISP_ISR_PathFIFOOverflowedHigh);
    IVsp_ClearInt(ISP_BUFFER_OVF_ID);
    IVsp_OpenInt(ISP_BUFFER_OVF_ID);   

    ISP_Start();
}
/****************************************************************************************/
// Description: Save the Mjpg parameter of dc to isp service com parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                 ISP_MJPEG_PARAM_T *param_ptr: the pointer of mjpg parameter
// output parameter:
//                 None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_SetMjpegParam(ISP_MJPEG_PARAM_T *param_ptr)
{
    uint32 rtn=SCI_SUCCESS;
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint16* zoom_ptr=NULL;

    if(ISP_DATA_YUV422!=pISPContext->input_image_format)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: _ISP_SetMjpegParam img format is %x  %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_4088_112_2_18_1_43_43_75,(uint8*)"dd", pISPContext->input_image_format, __LINE__);
        return SCI_ERROR;
    }	

    ISP_RETURN_IF_ERR(_ISP_SetDcamClk(param_ptr->pclk), SCI_ERROR, ("ISP_SERVICE: _ISP_SetMjpegParam pclk:%d, %d line error",param_ptr->pclk, __LINE__));
    
    pISPContext->capture_skip_frame_num=0x00;
    pISPContext->is_pause=param_ptr->is_pause;  
    pISPContext->is_disp=param_ptr->is_disp;      
    pISPContext->is_zoom=SCI_FALSE;
    pISPContext->is_stop=SCI_FALSE;
    pISPContext->isp_feature_enable=SCI_TRUE;
    pISPContext->is_first_frame=SCI_TRUE; 
    pISPContext->is_full_view=SCI_FALSE;

    _ISP_UpdateCache(param_ptr->capture_addr, param_ptr->capture_buf_size, ISP_OP_READ_WRITE);
    _ISP_UpdateCache(param_ptr->scale_addr, param_ptr->scale_buf_size, ISP_OP_READ_WRITE);
    _ISP_UpdateCache(param_ptr->disp_addr, param_ptr->disp_buf_size, ISP_OP_READ_WRITE);

    _ISP_UpdateCache(param_ptr->rot_addr.y_addr, param_ptr->rot_buf_len.y_len, ISP_OP_READ_WRITE);
    _ISP_UpdateCache(param_ptr->rot_addr.uv_addr, param_ptr->rot_buf_len.uv_len, ISP_OP_READ_WRITE);
    _ISP_UpdateCache(param_ptr->rot_addr.v_addr, param_ptr->rot_buf_len.v_len, ISP_OP_READ_WRITE);

    _ISP_CalcCapDeci(param_ptr->input_rect.w,param_ptr->input_rect.h,param_ptr->capture_size.w,param_ptr->capture_size.h);

    //***********Mjpg capture yuv************//
    // set cap rect for capture yuv
    _ISP_SetZoomTab(param_ptr->input_rect.x, 
                        param_ptr->input_rect.y,
                        param_ptr->input_rect.w, 
                        param_ptr->input_rect.h, 
                        param_ptr->capture_size.w, 
                        param_ptr->capture_size.h);

    _ISP_CheckIsFullView();

    if(NULL!=pISPContext->zoom_array_ptr)
    {
        zoom_ptr=pISPContext->zoom_array_ptr+((pISPContext->zoom_ctl.zoom_level*ISP_SMOOTH_ZOOM_STEP)*ISP_ZOOM_RECT_NUM); 
        SCI_MEMCPY((void*)&(pISPContext->cap_trim_rect),(void*)(zoom_ptr),sizeof(ISP_RECT_T)); 
    }
    else
    {
        SCI_MEMCPY((void*)&(pISPContext->cap_trim_rect),(void*)&(param_ptr->input_rect),sizeof(ISP_RECT_T)); 
    }
    
    // set cap output size
    pISPContext->cap_output_size.w=pISPContext->cap_trim_rect.w/pISPContext->cap_deci;
    pISPContext->cap_output_size.h=pISPContext->cap_trim_rect.h/pISPContext->cap_deci;
    
    // set capture scale input size (as cap output size)
    pISPContext->mjpg_info.capture_scale_input_size.w=pISPContext->cap_output_size.w;
    pISPContext->mjpg_info.capture_scale_input_size.h=pISPContext->cap_output_size.h;   
    
    // set capture scale trim rect
    pISPContext->scale_trim_rect.x=0x00;
    pISPContext->scale_trim_rect.y=0x00;
    pISPContext->scale_trim_rect.w=pISPContext->cap_output_size.w;
    pISPContext->scale_trim_rect.h=pISPContext->cap_output_size.h;   

    SCI_MEMCPY((void*)&(pISPContext->output_size), (void*)&(param_ptr->capture_size), sizeof(ISP_SIZE_T));  
    
    // calculate mjpg capture yuv scale param   
    rtn = _ISP_CalcScaleParam();
	if (SCI_SUCCESS != rtn)
	{
		//SCI_TRACE_ISP:"ISP_SERVICE: _ISP_CalcScaleParam failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_4154_112_2_18_1_43_43_76,(uint8*)"");
		return rtn;
	}
	
    pISPContext->mjpg_info.capture_scale_bypass=pISPContext->is_bypass_scale;
    SCI_MEMCPY(pISPContext->mjpg_info.capture_h_scale_coeff_ptr, pISPContext->v_scale_coeff_ptr, SCALER_COEF_MAX_SIZE);
	SCI_MEMCPY(pISPContext->mjpg_info.capture_v_scale_coeff_ptr, pISPContext->h_scale_coeff_ptr, SCALER_COEF_MAX_SIZE);                	

    pISPContext->mjpg_info.capture_scale_trim_eb=pISPContext->scale_trim_eb;  
    SCI_MEMCPY((void*)&(pISPContext->mjpg_info.capture_scale_trim_rect), (void*)&(pISPContext->scale_trim_rect), sizeof(ISP_RECT_T));  
    SCI_MEMCPY((void*)&(pISPContext->mjpg_info.capture_size), (void*)&(pISPContext->output_size), sizeof(ISP_SIZE_T));    

    // capture yuv buf addr
    pISPContext->mjpg_info.capture_yuv_addr.y_addr=param_ptr->capture_addr;
    pISPContext->mjpg_info.capture_yuv_size =param_ptr->capture_buf_size;  //for scale buffer and yuv buffer share to reduce memory consumption
    
    //**********Mjpg review (change yuv to rgb for display)**************//
    //set mjpg review scale input size for display
    SCI_MEMCPY((void*)&(pISPContext->cap_output_size), (void*)&(pISPContext->mjpg_info.capture_size), sizeof(ISP_SIZE_T));      
    SCI_MEMCPY((void*)&(pISPContext->mjpg_info.review_scale_input_size), (void*)&(pISPContext->mjpg_info.capture_size), sizeof(ISP_SIZE_T));  

    SCI_MEMCPY((void*)&(pISPContext->scale_trim_rect), (void*)&(param_ptr->target_src_rect), sizeof(ISP_RECT_T));   

    pISPContext->output_size.w=param_ptr->target_rect.w;
    pISPContext->output_size.h=param_ptr->target_rect.h;

     // calculate mjpg review scale param (mjpg capture size equal to mjpg review source size)     
    rtn = _ISP_CalcScaleParam();
	if (SCI_SUCCESS != rtn)
	{
		//SCI_TRACE_ISP:"ISP_SERVICE: _ISP_CalcScaleParam failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_4184_112_2_18_1_43_43_77,(uint8*)"");
		return rtn;
	}
	
    pISPContext->mjpg_info.review_scale_bypass=pISPContext->is_bypass_scale;

    SCI_MEMCPY(pISPContext->mjpg_info.review_h_scale_coeff_ptr, pISPContext->v_scale_coeff_ptr, SCALER_COEF_MAX_SIZE);
	SCI_MEMCPY(pISPContext->mjpg_info.review_v_scale_coeff_ptr, pISPContext->h_scale_coeff_ptr, SCALER_COEF_MAX_SIZE);            

    pISPContext->mjpg_info.review_scale_trim_eb=pISPContext->scale_trim_eb;
    SCI_MEMCPY((void*)&(pISPContext->mjpg_info.review_scale_trim_rect), (void*)&(pISPContext->scale_trim_rect), sizeof(ISP_RECT_T));  
    SCI_MEMCPY((void*)&(pISPContext->mjpg_info.review_size), (void*)&(pISPContext->output_size), sizeof(ISP_SIZE_T));  
    //set display rect
    SCI_MEMCPY((void*)&(pISPContext->target_rect),(void*)&(param_ptr->target_rect),sizeof(ISP_RECT_T));    
    SCI_MEMCPY((void*)&(pISPContext->disp_rect),(void*)&(param_ptr->disp_rect),sizeof(ISP_RECT_T));

    // review rgb addr

    pISPContext->rotation = param_ptr->disp_rotation;
    
    if(SCI_TRUE==pISPContext->is_full_view)
    {
        SCI_MEMCPY((void*)&(pISPContext->full_screen_rect),(void*)&(param_ptr->target_rect),sizeof(ISP_RECT_T));
        pISPContext->mjpg_info.review_rgb_addr.y_addr=((((param_ptr->disp_addr+((pISPContext->target_rect.w*pISPContext->full_screen_rect.y)<<ISP_SHIFT_BIT1))+ISP_BUFFER_ADDR_ALIGN)>>ISP_BUFFER_ALIGN_BIT)<<ISP_BUFFER_ALIGN_BIT); //align 256 bytes
        pISPContext->review_display_buf_addr.y_addr=( pISPContext->mjpg_info.review_rgb_addr.y_addr-((pISPContext->target_rect.w*pISPContext->full_screen_rect.y)<<ISP_SHIFT_BIT1));
        s_offset_addr=pISPContext->target_rect.w*pISPContext->full_screen_rect.y; 
    }
    else
    {
        pISPContext->mjpg_info.review_rgb_addr.y_addr=param_ptr->disp_addr;   
        pISPContext->review_display_buf_addr.y_addr=param_ptr->disp_addr;   
    }

    pISPContext->rot_buf_addr=param_ptr->rot_addr.y_addr;
    pISPContext->rot_buf_size=param_ptr->rot_buf_len.y_len;
    pISPContext->rotation_buf[ISP_ROTATION_BUF_0].y_addr=ISP_Align(param_ptr->rot_addr.y_addr, ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
    pISPContext->rotation_buf[ISP_ROTATION_BUF_0].uv_addr=ISP_Align(param_ptr->rot_addr.uv_addr, ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
    pISPContext->rotation_loop_buf_num=ISP_ZERO;
    pISPContext->rotation_buf_id=ISP_ROTATION_BUF_0;  
    pISPContext->disp_format=ISP_DATA_RGB565;

    //Mjpg yuv encoder scale
    //set mjpg scale yuv scale input size for encoder
    SCI_MEMCPY((void*)&(pISPContext->cap_output_size), (void*)&(pISPContext->mjpg_info.capture_size), sizeof(ISP_SIZE_T));        
    SCI_MEMCPY((void*)&(pISPContext->mjpg_info.jpeg_scale_input_size), (void*)&(pISPContext->mjpg_info.capture_size), sizeof(ISP_SIZE_T));   

    // set scale trim rect
    SCI_MEMCPY((void*)&(pISPContext->scale_trim_rect), (void*)&(param_ptr->jpeg_target_rect), sizeof(ISP_RECT_T));      

    pISPContext->output_size.w=param_ptr->jpeg_target_size.w;
    pISPContext->output_size.h=param_ptr->jpeg_target_size.h;

    rtn = _ISP_CalcScaleParam();
	if (SCI_SUCCESS != rtn)
	{
		//SCI_TRACE_ISP:"ISP_SERVICE: _ISP_CalcScaleParam failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_4239_112_2_18_1_43_43_78,(uint8*)"");
		return rtn;
	}
	
    pISPContext->mjpg_info.jpeg_scale_bypass=pISPContext->is_bypass_scale;

    SCI_MEMCPY(pISPContext->mjpg_info.jpeg_h_scale_coeff_ptr, pISPContext->v_scale_coeff_ptr, SCALER_COEF_MAX_SIZE);
	SCI_MEMCPY(pISPContext->mjpg_info.jpeg_v_scale_coeff_ptr, pISPContext->h_scale_coeff_ptr, SCALER_COEF_MAX_SIZE);        

    pISPContext->mjpg_info.jpeg_scale_trim_eb=pISPContext->scale_trim_eb;
    SCI_MEMCPY((void*)&(pISPContext->mjpg_info.jpeg_scale_trim_rect), (void*)&(pISPContext->scale_trim_rect), sizeof(ISP_RECT_T));     
    SCI_MEMCPY((void*)&(pISPContext->mjpg_info.jpeg_target_size), (void*)&(pISPContext->output_size), sizeof(ISP_SIZE_T)); 

    // set the mjpg buf addr   
    pISPContext->mjpg_info.jpeg_yuv_addr.y_addr=param_ptr->scale_addr;   
    pISPContext->mjpg_info.jpge_yuv_size =param_ptr->scale_buf_size;   

    pISPContext->invalidate_mode = param_ptr->mode;
    pISPContext->is_copy_display_data =  param_ptr->is_copy_display_data;
         
    _ISP_InitializeBuf(ISP_SERVICE_MOTION_JPEG);    

    return rtn;
	
}

/****************************************************************************************/
// Description: Save the Upcc parameter of dc to isp service com parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                 ISP_MJPEG_PARAM_T *param_ptr: the pointer of upcc parameter
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_SetUpccParam(ISP_MJPEG_PARAM_T *param_ptr)
{
    uint32 rtn=SCI_SUCCESS;
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    if(ISP_DATA_YUV422!=pISPContext->input_image_format)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: _ISP_SetMjpegParam img format is %x  %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_4282_112_2_18_1_43_43_79,(uint8*)"dd", pISPContext->input_image_format, __LINE__);
        return SCI_ERROR;
    }

    ISP_RETURN_IF_ERR(_ISP_SetDcamClk(param_ptr->pclk), SCI_ERROR, ("ISP_SERVICE: _ISP_SetUpccParam pclk:%d, %d line error",param_ptr->pclk, __LINE__));

    pISPContext->capture_skip_frame_num=0x00;
    pISPContext->is_pause=param_ptr->is_pause;  
    pISPContext->is_disp=param_ptr->is_disp;      
    pISPContext->is_zoom=SCI_FALSE;
    pISPContext->is_stop=SCI_FALSE;
    pISPContext->isp_feature_enable=SCI_TRUE;
    pISPContext->is_first_frame=SCI_TRUE;
    pISPContext->yuv2rgb_eb=SCI_FALSE;

    //***********Mjpg capture yuv************//
    // set cap rect for capture yuv
    SCI_MEMCPY((void*)&(pISPContext->cap_trim_rect), (void*)&(param_ptr->input_rect), sizeof(ISP_RECT_T)); 
    
    // set cap output size
    pISPContext->cap_output_size.w=param_ptr->input_rect.w;
    pISPContext->cap_output_size.h=param_ptr->input_rect.h;
    
    // set capture scale input size (as cap output size)
    pISPContext->mjpg_info.capture_scale_input_size.w=param_ptr->input_rect.w;
    pISPContext->mjpg_info.capture_scale_input_size.h=param_ptr->input_rect.h;   
    
    // set capture scale trim rect
    pISPContext->scale_trim_rect.x=0x00;
    pISPContext->scale_trim_rect.y=0x00;
    pISPContext->scale_trim_rect.w=param_ptr->input_rect.w;
    pISPContext->scale_trim_rect.h=param_ptr->input_rect.h;   

    SCI_MEMCPY((void*)&(pISPContext->output_size), (void*)&(param_ptr->jpeg_target_size), sizeof(ISP_SIZE_T));      
    
    // calculate mjpg capture yuv scale param   
    _ISP_CalcScaleParam();
    
    pISPContext->isp_buf_addr=param_ptr->capture_addr;

    _ISP_InitializeBuf(ISP_SERVICE_UPCC);

    return rtn;
	
}

/****************************************************************************************/
// Description: Set the Mjpg capture yuv parameter to isp 
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_SetMjpegCpatureParam(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    // calculate mjpg review scale param (mjpg capture size equal to mjpg review source size)  
    SCI_MEMCPY((void*)&(pISPContext->cap_output_size), (void*)&(pISPContext->mjpg_info.capture_scale_input_size), sizeof(ISP_SIZE_T));    
    SCI_MEMCPY((void*)&(pISPContext->scale_trim_rect), (void*)&(pISPContext->mjpg_info.capture_scale_trim_rect), sizeof(ISP_RECT_T));      
    SCI_MEMCPY((void*)&(pISPContext->output_size), (void*)&(pISPContext->mjpg_info.capture_size), sizeof(ISP_SIZE_T)); 

    pISPContext->is_bypass_scale=pISPContext->mjpg_info.capture_scale_bypass;

    SCI_MEMCPY(pISPContext->v_scale_coeff_ptr, pISPContext->mjpg_info.capture_h_scale_coeff_ptr, SCALER_COEF_MAX_SIZE);
	SCI_MEMCPY(pISPContext->h_scale_coeff_ptr, pISPContext->mjpg_info.capture_v_scale_coeff_ptr, SCALER_COEF_MAX_SIZE);        

    pISPContext->scale_trim_eb=pISPContext->mjpg_info.capture_scale_trim_eb;

    // set the capture addr
    SCI_MEMCPY((void*)&(pISPContext->output_addr), (void*)&(pISPContext->mjpg_info.capture_yuv_addr), sizeof(ISP_DATA_ADDR_T));      
    
    pISPContext->input_image_format=ISP_DATA_YUV422;
    pISPContext->output_image_format=ISP_DATA_YUV422;
    pISPContext->yuv2rgb_eb=SCI_FALSE;
    pISPContext->is_tx_done=SCI_FALSE;

}

/****************************************************************************************/
// Description: Set the Mjpge review parameter to isp com parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_SetMjpegReviewParam(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    // calculate mjpg review scale param (mjpg capture size equal to mjpg review source size)
    SCI_MEMCPY((void*)&(pISPContext->cap_output_size), (void*)&(pISPContext->mjpg_info.review_scale_input_size), sizeof(ISP_SIZE_T));    
    SCI_MEMCPY((void*)&(pISPContext->scale_trim_rect), (void*)&(pISPContext->mjpg_info.review_scale_trim_rect), sizeof(ISP_RECT_T));      
    SCI_MEMCPY((void*)&(pISPContext->output_size), (void*)&(pISPContext->mjpg_info.review_size), sizeof(ISP_SIZE_T)); 

    pISPContext->is_bypass_scale=pISPContext->mjpg_info.review_scale_bypass;

    SCI_MEMCPY(pISPContext->v_scale_coeff_ptr, pISPContext->mjpg_info.review_h_scale_coeff_ptr, SCALER_COEF_MAX_SIZE);
	SCI_MEMCPY(pISPContext->h_scale_coeff_ptr, pISPContext->mjpg_info.review_v_scale_coeff_ptr, SCALER_COEF_MAX_SIZE);      

    pISPContext->scale_trim_eb=pISPContext->mjpg_info.review_scale_trim_eb;

    // set the review addr
    SCI_MEMCPY((void*)&(pISPContext->input_addr), (void*)&(pISPContext->mjpg_info.capture_yuv_addr), sizeof(ISP_DATA_ADDR_T));   
    SCI_MEMCPY((void*)&(pISPContext->output_addr), (void*)&(pISPContext->mjpg_info.review_rgb_addr), sizeof(ISP_DATA_ADDR_T)); 

    pISPContext->output_addr.uv_addr=0x00;

    #if (defined(PLATFORM_SC6800H))
    SCI_MEMCPY((void*)&(pISPContext->review_display_buf_addr), (void*)&(pISPContext->mjpg_info.review_rgb_addr), sizeof(ISP_DATA_ADDR_T)); 
    #endif

    pISPContext->input_image_format=ISP_DATA_YUV422;
    pISPContext->output_image_format=ISP_DATA_RGB565;        
    pISPContext->yuv2rgb_eb=_ISP_GetIsYuv2Rgb(pISPContext->output_image_format);

}

/****************************************************************************************/
// Description: Set the Mjpg sclaing parameter to isp com parameter, scale yuv
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_SetMjpegScaleParam(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    // calculate mjpg review scale param (mjpg capture size equal to mjpg review source size)
    SCI_MEMCPY((void*)&(pISPContext->cap_output_size), (void*)&(pISPContext->mjpg_info.jpeg_scale_input_size), sizeof(ISP_SIZE_T));    
    SCI_MEMCPY((void*)&(pISPContext->scale_trim_rect), (void*)&(pISPContext->mjpg_info.jpeg_scale_trim_rect), sizeof(ISP_RECT_T));      
    SCI_MEMCPY((void*)&(pISPContext->output_size), (void*)&(pISPContext->mjpg_info.jpeg_target_size), sizeof(ISP_SIZE_T)); 

    pISPContext->is_bypass_scale=pISPContext->mjpg_info.jpeg_scale_bypass;
   
    SCI_MEMCPY(pISPContext->v_scale_coeff_ptr, pISPContext->mjpg_info.jpeg_h_scale_coeff_ptr, SCALER_COEF_MAX_SIZE);
	SCI_MEMCPY(pISPContext->h_scale_coeff_ptr, pISPContext->mjpg_info.jpeg_v_scale_coeff_ptr, SCALER_COEF_MAX_SIZE);    
    pISPContext->scale_trim_eb=pISPContext->mjpg_info.jpeg_scale_trim_eb;

      // set the review addr
    SCI_MEMCPY((void*)&(pISPContext->input_addr), (void*)&(pISPContext->mjpg_info.capture_yuv_addr), sizeof(ISP_DATA_ADDR_T));   
    SCI_MEMCPY((void*)&(pISPContext->output_addr), (void*)&(pISPContext->mjpg_info.jpeg_yuv_addr), sizeof(ISP_DATA_ADDR_T));         

    pISPContext->input_image_format=ISP_DATA_YUV422;
    pISPContext->output_image_format=ISP_DATA_YUV422;
    
    if(ISP_DATA_RGB565==pISPContext->output_image_format)
    {
        pISPContext->yuv2rgb_eb=SCI_TRUE;        
    }
    else
    {
        pISPContext->yuv2rgb_eb=SCI_FALSE;
    }        

}

/****************************************************************************************/
// Description: Stop the Mjpg recordr
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_StopMjpg(void)
{   
    //SCI_TRACE_ISP:"ISP_SERVICE: _ISP_StopMjpg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_4461_112_2_18_1_43_44_80,(uint8*)"");
    
    ISP_FreeISP();

    return ;
}

/****************************************************************************************/
// Description: Get the Camera digital zoom input range parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail  
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_GetZoomInputRange(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    uint16* zoom_range_ptr=NULL;

    zoom_range_ptr=pISPContext->zoom_array_ptr;

    if(ISP_SERVICE_ZOOM_IN==pISPContext->zoom_ctl.zoom_mode)// zoom in
    {
        zoom_range_ptr+=((pISPContext->zoom_ctl.zoom_level-1)*ISP_SMOOTH_ZOOM_STEP+pISPContext->zoom_ctl.zoom_step)*ISP_ZOOM_RECT_NUM;
    }
    else if(ISP_SERVICE_ZOOM_OUT==pISPContext->zoom_ctl.zoom_mode)    // zoom out
    {
        zoom_range_ptr+=((pISPContext->zoom_ctl.zoom_level+1)*ISP_SMOOTH_ZOOM_STEP-pISPContext->zoom_ctl.zoom_step)*ISP_ZOOM_RECT_NUM;
    }

    SCI_MEMCPY((void*)&(pISPContext->zoom_rect),(void*)(zoom_range_ptr),sizeof(ISP_RECT_T));     
    SCI_MEMCPY((void*)&(pISPContext->cap_trim_rect),(void*)(zoom_range_ptr),sizeof(ISP_RECT_T)); 

    pISPContext->cap_output_size.w=pISPContext->cap_trim_rect.w/pISPContext->cap_deci;
    pISPContext->cap_output_size.h=pISPContext->cap_trim_rect.h/pISPContext->cap_deci;

    pISPContext->scale_trim_rect.x=0x00;    
    pISPContext->scale_trim_rect.y=0x00;    
    pISPContext->scale_trim_rect.w=pISPContext->cap_output_size.w;    
    pISPContext->scale_trim_rect.h=pISPContext->cap_output_size.h;   

    //SCI_TRACE_ISP:"ISP_SERVICE:Zoom array ptr=0x%x, input.x = %d, input.y = %d,input.w = %d, input.h = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_4508_112_2_18_1_43_44_81,(uint8*)"ddddd",pISPContext->zoom_array_ptr,pISPContext->cap_trim_rect.x, pISPContext->cap_trim_rect.y,pISPContext->cap_trim_rect.w,pISPContext->cap_trim_rect.h);

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Save the camera parameter to isp service com parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_SetDcZoomParam(void)
{
    _ISP_GetZoomInputRange();
    // calculate the scale coeff
    _ISP_CalcScaleParam();

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Start camera still digital zoom
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail 
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_StartDcZoom(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();	
    AHB_MASTER_MODULE_T addr={0x00};
    ISP_RECT_T scale_trim_size={0x00};
    uint32 param = 0;
    
    pISPContext->is_first_frame= SCI_FALSE;

    _ISP_SetDcZoomParam();

    ISP_FreeISP();
    ISP_SelectISP();
    ISP_SoftReset();

    ISP_SetMode(ISP_MODE_IDLE);	

    //set skip num
    ISP_SetCAP(CAP_PRE_SKIP_CNT,0x01);    

    //set cap module  
    ISP_SetCAP(CAP_SYNC_POL,((pISPContext->vsync_polarity<<16)|(pISPContext->hsync_polarity)));
    ISP_SetCAP(CAP_FIFO_DATA_RATE,CAP_FIFO_FULL_SPEED);

    ISP_SetCAP(CAP_CAP_DECI,pISPContext->cap_deci);

    ISP_SetCAPInputFormat(CAP_INPUT_FORMAT_YUV);	    
    ISP_SetCAP(CAP_YUV_TYPE,pISPContext->input_image_pattern);

    //set cap trim size  equal to dcam input size  
    ISP_SetCAP(CAP_CAP_START,(((pISPContext->cap_trim_rect.y)<<16)|(pISPContext->cap_trim_rect.x*2)));	
    param = (pISPContext->cap_trim_rect.x+pISPContext->cap_trim_rect.w)*2-1;
    param |= ((pISPContext->cap_trim_rect.y+pISPContext->cap_trim_rect.h-1)<<16);
    ISP_SetCAP(CAP_CAP_END,param);	

    //set scale module
    ISP_SetScaleInputFormat(pISPContext->input_image_format);
    //set cap output size equal to scale input size
    ISP_SetSrcSize(pISPContext->cap_output_size.w,pISPContext->cap_output_size.h);    
    //set scale trim size    
    scale_trim_size.x=pISPContext->scale_trim_rect.x;
    scale_trim_size.y=pISPContext->scale_trim_rect.y;
    scale_trim_size.w=pISPContext->scale_trim_rect.w;
    scale_trim_size.h=pISPContext->scale_trim_rect.h;		
    ISP_SetScaleTrimRect(scale_trim_size);
    ISP_EnableScaleTrim(pISPContext->scale_trim_eb);     
    //set scale outpur size equal to dcam output size
    ISP_SerDstSize(pISPContext->output_size.w,pISPContext->output_size.h);

    //set scale
    _ISP_SetScaleCoeff();
    
    //enable yuv2rgb module
    ISP_EnableYUV2RGB(pISPContext->yuv2rgb_eb);
    ISP_SetMode(ISP_MODE_PREVIEW);

    //set the currect fill buf
    addr.output_addr.y_addr=pISPContext->preview_buf[pISPContext->preview_buf_id].y_addr;	
    addr.output_addr.uv_addr=pISPContext->preview_buf[pISPContext->preview_buf_id].uv_addr; 

    ISP_SetAhbMaster(&addr);
    ISP_AutoCopy();

    IVsp_RegisterIntFuncEx(ISP_TX_DONE_ID, _ISP_ISR_TrnasferDoneLow, _ISP_ISR_TrnasferDoneHigh);
    IVsp_ClearInt(ISP_TX_DONE_ID);
    IVsp_OpenInt(ISP_TX_DONE_ID);

    IVsp_RegisterIntFuncEx(CAP_FRAME_ERR_ID, _ISP_ISR_SensorFrameErrLow, _ISP_ISR_SensorFrameErrHigh);
    IVsp_ClearInt(CAP_FRAME_ERR_ID);
    IVsp_OpenInt(CAP_FRAME_ERR_ID);

    IVsp_RegisterIntFuncEx(ISP_BUFFER_OVF_ID, _ISP_ISR_PathFIFOOverflowedLow, _ISP_ISR_PathFIFOOverflowedHigh);
    IVsp_ClearInt(ISP_BUFFER_OVF_ID);
    IVsp_OpenInt(ISP_BUFFER_OVF_ID);   

    ISP_Start();

    if(ISP_SMOOTH_ZOOM_STEP>pISPContext->zoom_ctl.zoom_step)
    {
        pISPContext->is_send_zoom_sig=SCI_TRUE;
        pISPContext->zoom_ctl.zoom_step++;        
    }
    else
    {
        pISPContext->is_first_frame= SCI_TRUE;	
        pISPContext->is_send_zoom_sig=SCI_FALSE;
        //digital zoom callback
        if((ISP_SERVICE_ZOOM_OUT==pISPContext->zoom_ctl.zoom_mode)&&(ISP_SERVICE_ZOOM_100X==pISPContext->zoom_ctl.zoom_level))
        {
            pISPContext->is_zoom=SCI_FALSE;
        }
        if(PNULL!=pISPContext->zoom_ctl.zoom_success)
        {
            //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceZoom call back 1"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_4637_112_2_18_1_43_44_82,(uint8*)"");
            pISPContext->zoom_ctl.zoom_success(SCI_TRUE);
        }
    }

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Start camera video digital zoom
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail   
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_StartDvZoom(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();	
	uint32 rtn = SCI_SUCCESS;

    //SCI_TRACE_ISP:"ISP_Service: _ISP_StartDvZoom"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_4661_112_2_18_1_43_44_83,(uint8*)"");

    _ISP_GetZoomInputRange();

    //***********Mjpg capture yuv************//
    // set cap output size
    SCI_MEMCPY((void*)&(pISPContext->output_size), (void*)&(pISPContext->mjpg_info.capture_size), sizeof(ISP_SIZE_T));     

    // calculate mjpg capture yuv scale param   
    rtn = _ISP_CalcScaleParam();
	if (SCI_SUCCESS != rtn)
	{
		//SCI_TRACE_ISP:"ISP_Service: _ISP_StartDvZoom, _ISP_CalcScaleParam failed"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_4673_112_2_18_1_43_44_84,(uint8*)"");
		return rtn;
	}
	
    pISPContext->mjpg_info.capture_scale_bypass=pISPContext->is_bypass_scale;	

    SCI_MEMCPY(pISPContext->mjpg_info.capture_h_scale_coeff_ptr, pISPContext->v_scale_coeff_ptr, SCALER_COEF_MAX_SIZE);
	SCI_MEMCPY(pISPContext->mjpg_info.capture_v_scale_coeff_ptr, pISPContext->h_scale_coeff_ptr, SCALER_COEF_MAX_SIZE);
	
    pISPContext->mjpg_info.capture_scale_trim_eb=pISPContext->scale_trim_eb; 
    
    SCI_MEMCPY((void*)&(pISPContext->mjpg_info.capture_scale_input_size), (void*)&(pISPContext->cap_output_size), sizeof(ISP_SIZE_T));      
    SCI_MEMCPY((void*)&(pISPContext->mjpg_info.capture_scale_trim_rect), (void*)&(pISPContext->scale_trim_rect), sizeof(ISP_RECT_T));  
    SCI_MEMCPY((void*)&(pISPContext->mjpg_info.capture_size), (void*)&(pISPContext->output_size), sizeof(ISP_SIZE_T));  

    if(ISP_SMOOTH_ZOOM_STEP>pISPContext->zoom_ctl.zoom_step)
    {            
        pISPContext->zoom_ctl.zoom_step++;
    }
    else
    {
        pISPContext->is_zoom=SCI_FALSE;
        //digital zoom callback     
        if(PNULL!=pISPContext->zoom_ctl.zoom_success)
        {
            pISPContext->zoom_ctl.zoom_success(SCI_TRUE);
            //SCI_TRACE_ISP:"ISP_Service: _ISP_StartDvZoom call back"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_4699_112_2_18_1_43_44_85,(uint8*)"");
        }
    }    
  
    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Save the scaling parameter to isp service parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_SCALE_PARAM_T *param_ptr: the pointer of scale parameter
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_SetScaleParam(ISP_SCALE_PARAM_T *param_ptr)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint16 scale_input_hieht=0x00;

    pISPContext->isp_feature_enable=SCI_FALSE; 
    pISPContext->dcam_clk=IVSP_CLK_48M;
    pISPContext->slice_complete_line=ISP_ZERO;
	
    SCI_MEMCPY((void*)&(pISPContext->cap_output_size),(void*)&(param_ptr->input_size),sizeof(ISP_SIZE_T));
    SCI_MEMCPY((void*)&(pISPContext->scale_trim_rect),(void*)&(param_ptr->input_rect),sizeof(ISP_RECT_T));
    SCI_MEMCPY((void*)&(pISPContext->output_size),(void*)&(param_ptr->output_size),sizeof(ISP_SIZE_T));
    SCI_MEMCPY((void*)&(pISPContext->input_addr),(void*)&(param_ptr->input_addr),sizeof(ISP_DATA_ADDR_T));

    if(ISP_SERVICE_SCALE_SLICE==param_ptr->scale_mode)
    {
        scale_input_hieht=param_ptr->slice_line;
    }
    else
    {
        scale_input_hieht=param_ptr->input_size.h;
    }

    _ISP_SetUpdateCacheSize(param_ptr->input_size.w, scale_input_hieht, param_ptr->input_format);
    _ISP_UpdateCache(pISPContext->input_addr.y_addr, pISPContext->update_cache.y_len, ISP_OP_READ);
    _ISP_UpdateCache(pISPContext->input_addr.uv_addr, pISPContext->update_cache.uv_len, ISP_OP_READ);
    _ISP_UpdateCache(pISPContext->input_addr.v_addr, pISPContext->update_cache.v_len, ISP_OP_READ);

    pISPContext->input_image_format=param_ptr->input_format; 
    pISPContext->is_rgb_mode=_ISP_GetIsRgb(param_ptr->input_format);
    pISPContext->is_rgb565=_ISP_GetIsRgb565(param_ptr->input_format);    

    pISPContext->output_image_format=param_ptr->output_format; 
    pISPContext->yuv2rgb_eb=_ISP_GetIsYuv2Rgb(param_ptr->output_format);

    pISPContext->isp_buf_addr=param_ptr->output_addr.y_addr;
    pISPContext->output_addr.y_addr=param_ptr->output_addr.y_addr;
    pISPContext->output_addr.uv_addr=param_ptr->output_addr.uv_addr;
    _ISP_UpdateCache(pISPContext->isp_buf_addr, param_ptr->output_buf_size, ISP_OP_WRITE);

    pISPContext->scale_mode=param_ptr->scale_mode;/*lint !e64*/     
    pISPContext->get_data=param_ptr->get_scale_data; 
    // revime mode input align four
    pISPContext->scale_trim_rect.x=ISP_Align(pISPContext->scale_trim_rect.x, ISP_ZERO, ISP_SHIFT_BIT2);
    pISPContext->scale_trim_rect.w=ISP_Align(pISPContext->scale_trim_rect.w, ISP_ZERO, ISP_SHIFT_BIT2);
    // review mode yuv output format align eight  
    pISPContext->output_size.w=_ISP_GetOutputWidthAlign(pISPContext->output_size.w, param_ptr->output_format); 

    pISPContext->is_first=param_ptr->is_first;
    _ISP_CalcSubSampleMode();
	pISPContext->start_scale_callback = param_ptr->start_scale_callback;
	pISPContext->start_scale_param = param_ptr->start_scale_param;

    // calculate the scale param
    if(ISP_SERVICE_SCALE_SLICE==param_ptr->scale_mode)
    {
        pISPContext->swap_addr.y_addr=param_ptr->swap_buf_addr;    
        pISPContext->swap_addr.uv_addr=param_ptr->swap_buf_addr+ISP_Align((pISPContext->output_size.w*(param_ptr->input_rect.y+param_ptr->slice_line)), ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
        pISPContext->line_buf_addr=param_ptr->line_buf_addr;
        pISPContext->slice_line=param_ptr->slice_line;
        pISPContext->is_last=param_ptr->is_last;

        _ISP_UpdateCache(param_ptr->swap_buf_addr, param_ptr->swap_buf_size, ISP_OP_READ_WRITE);
        _ISP_UpdateCache(param_ptr->line_buf_addr, param_ptr->line_buf_size, ISP_OP_READ_WRITE);
    }
	
    _ISP_CalcScaleParam(); 

    if(ISP_SERVICE_SCALE_SLICE!=pISPContext->scale_mode)
    {
        _ISP_InitializeBuf(ISP_SERVICE_SCALE);
    }

}

/****************************************************************************************/
// Description: Save the scaling parameter to isp service parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_SCALE_PARAM_T *param_ptr: the pointer of scale parameter
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_SetSliceScaleParam(ISP_SCALE_PARAM_T *param_ptr)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    pISPContext->dcam_clk=IVSP_CLK_48M;
    pISPContext->is_first=param_ptr->is_first;
    pISPContext->is_last=param_ptr->is_last;
    pISPContext->slice_line=param_ptr->slice_line;

    SCI_MEMCPY((void*)&(pISPContext->input_addr),(void*)&(param_ptr->input_addr),sizeof(ISP_DATA_ADDR_T));
    SCI_MEMCPY((void*)&(pISPContext->output_addr),(void*)&(param_ptr->output_addr),sizeof(ISP_DATA_ADDR_T));

    _ISP_SetUpdateCacheSize(pISPContext->cap_output_size.w, pISPContext->slice_line, param_ptr->input_format);
    _ISP_UpdateCache(pISPContext->input_addr.y_addr, pISPContext->update_cache.y_len, ISP_OP_READ);
    _ISP_UpdateCache(pISPContext->input_addr.uv_addr, pISPContext->update_cache.uv_len, ISP_OP_READ);
    _ISP_UpdateCache(pISPContext->input_addr.v_addr, pISPContext->update_cache.v_len, ISP_OP_READ);

    _ISP_UpdateCache(pISPContext->output_addr.y_addr, param_ptr->output_buf_size, ISP_OP_WRITE);

    pISPContext->get_data=param_ptr->get_scale_data; 
	pISPContext->start_scale_callback = param_ptr->start_scale_callback;
	pISPContext->start_scale_param = param_ptr->start_scale_param;
}

/****************************************************************************************/
// Description: Dump the scaling parameter to chip and start scale
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_StartScale(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();	
    AHB_MASTER_MODULE_T addr={0x00};
    ISP_RECT_T scale_trim_size=pISPContext->scale_trim_rect; 

    IVsp_SetClk(pISPContext->dcam_clk);
    ISP_SelectISP();
    if(SCI_TRUE==pISPContext->is_first)
    {
        ISP_SoftReset();
    }

    ISP_SetMode(ISP_MODE_IDLE);

     //set scale module
    ISP_SetScaleMode(pISPContext->scale_mode);
    ISP_SetScaleInputFormat(pISPContext->input_image_format);
    ISP_SetScaleInputType(pISPContext->is_rgb_mode);
    ISP_SetScaleInputRgbFormat(pISPContext->is_rgb565);
    ISP_SetSubSampleMode(pISPContext->subsample_eb, pISPContext->subsample_mode);
    
    if(ISP_SERVICE_SCALE_SLICE==pISPContext->scale_mode)
    {
        ISP_SetScaleSliceHeight(pISPContext->slice_line);
        ISP_SetScaleLastSlice(pISPContext->is_last);
    }        

    ISP_SetSrcSize(pISPContext->cap_output_size.w,pISPContext->cap_output_size.h);

    //set scale trim size    
    ISP_SetScaleTrimRect(scale_trim_size);
    ISP_EnableScaleTrim(pISPContext->scale_trim_eb);   
    
    ISP_SerDstSize(pISPContext->output_size.w,pISPContext->output_size.h);

    //set scale
    _ISP_SetScaleCoeff();

    ISP_EnableYUV2RGB(pISPContext->yuv2rgb_eb); 

	//disable the dither if the input data is RGB565 for the dither defect
	//must be called after the function ISP_EnableYUV2RGB
	if (ISP_DATA_RGB565 == pISPContext->input_image_format
		|| ISP_DATA_RGB666 == pISPContext->input_image_format)
	{
		ISP_EnableDither(FALSE);
	}

    ISP_SetMode(ISP_MODE_REVIEW);

    addr.input_addr.y_addr   = pISPContext->input_addr.y_addr;
    addr.input_addr.uv_addr  = pISPContext->input_addr.uv_addr;
    addr.output_addr.y_addr  = pISPContext->output_addr.y_addr;
    addr.output_addr.uv_addr = pISPContext->output_addr.uv_addr;
    ISP_SetAhbMaster(&addr);
    
    if((ISP_IsSupportSliceScale())
        &&(ISP_SERVICE_SCALE_SLICE==pISPContext->scale_mode))
    {
        addr.input_addr.y_addr=pISPContext->swap_addr.y_addr;
        addr.input_addr.uv_addr=pISPContext->swap_addr.uv_addr;
        addr.output_addr.y_addr=pISPContext->line_buf_addr;
        addr.output_addr.uv_addr=NULL;
        ISP_SetScaleSliceSwapMaster(&addr);
    }      
    
    ISP_AutoCopy();

    IVsp_RegisterIntFuncEx(ISP_TX_DONE_ID, _ISP_ISR_TrnasferDoneLow, _ISP_ISR_TrnasferDoneHigh);
    IVsp_ClearInt(ISP_TX_DONE_ID);
    IVsp_OpenInt(ISP_TX_DONE_ID);

    IVsp_RegisterIntFuncEx(ISP_BUFFER_OVF_ID, _ISP_ISR_PathFIFOOverflowedLow, _ISP_ISR_PathFIFOOverflowedHigh);
    IVsp_ClearInt(ISP_BUFFER_OVF_ID);
    IVsp_OpenInt(ISP_BUFFER_OVF_ID);  

    ISP_Start();

	if (PNULL != pISPContext->start_scale_callback)
	{
		pISPContext->start_scale_callback(pISPContext->start_scale_param);
	}

}

/****************************************************************************************/
// Description: Dump the scaling parameter to chip and start slice scale
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_StartSliceScale(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();	
    AHB_MASTER_MODULE_T addr={0x00};
    ISP_RECT_T scale_trim_size=pISPContext->scale_trim_rect; 
    
	if(ISP_IsSupportSliceScale())
	{
        ISP_SelectISP();
        ISP_SetMode(ISP_MODE_IDLE);

        ISP_SetSrcSize(pISPContext->cap_output_size.w,pISPContext->cap_output_size.h);

        //set scale trim size    
        ISP_SetScaleTrimRect(scale_trim_size);
        ISP_SetScaleTrimRectY(0x00);  
        ISP_EnableScaleTrim(pISPContext->scale_trim_eb);   

        ISP_SerDstSize(pISPContext->output_size.w,pISPContext->output_size.h);    

        //set scale
        _ISP_SetScaleCoeff();	
    	
    	ISP_SetScaleSliceHeight(pISPContext->slice_line);

        if(pISPContext->is_last)
        {
            ISP_SetScaleLastSlice(SCI_TRUE);
        }

        ISP_SetMode(ISP_MODE_REVIEW);	

        addr.input_addr.y_addr   = pISPContext->input_addr.y_addr;
        addr.input_addr.uv_addr  = pISPContext->input_addr.uv_addr;
        addr.output_addr.y_addr  = pISPContext->output_addr.y_addr;
        addr.output_addr.uv_addr = pISPContext->output_addr.uv_addr;
        ISP_SetAhbMaster(&addr);


        addr.input_addr.y_addr=pISPContext->swap_addr.y_addr;
        addr.input_addr.uv_addr=pISPContext->swap_addr.uv_addr;
        addr.output_addr.y_addr=pISPContext->line_buf_addr;
        addr.output_addr.uv_addr=NULL;
        ISP_SetScaleSliceSwapMaster(&addr);

        ISP_AutoCopy();
    	
        IVsp_RegisterIntFuncEx(ISP_TX_DONE_ID, _ISP_ISR_TrnasferDoneLow, _ISP_ISR_TrnasferDoneHigh);
        IVsp_ClearInt(ISP_TX_DONE_ID);
        IVsp_OpenInt(ISP_TX_DONE_ID);

        IVsp_RegisterIntFuncEx(ISP_BUFFER_OVF_ID, _ISP_ISR_PathFIFOOverflowedLow, _ISP_ISR_PathFIFOOverflowedHigh);
        IVsp_ClearInt(ISP_BUFFER_OVF_ID);
        IVsp_OpenInt(ISP_BUFFER_OVF_ID);
        
        ISP_Start();

		if (PNULL != pISPContext->start_scale_callback)
		{
			pISPContext->start_scale_callback(pISPContext->start_scale_param);
		}		
    }

    return ;
}

/****************************************************************************************/
// Description: isp get frame data
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_GetFrameData(ISP_DATA_FORMAT_E data_format)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    if((SCI_TRUE==pISPContext->is_get_frame_data)
        &&(data_format==pISPContext->frame_data_format))
    {
       // pISPContext->is_stop=SCI_TRUE;

       // pISPContext->is_get_frame_data=SCI_FALSE;

        if(PNULL!=pISPContext->get_frame_data)
        {
            pISPContext->get_frame_data(pISPContext->output_addr.y_addr, pISPContext->output_addr.uv_addr, pISPContext->output_size.w, pISPContext->output_size.h);
        }

        //_ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_STOP_SIG,0,0,0);
    }    
}

/****************************************************************************************/
// Description: close the isp
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ServiceClose(void)
{
    //SCI_TRACE_ISP:"ISP_Service: _ISP_ServiceClose"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5039_112_2_18_1_43_45_86,(uint8*)"");
    
    ISP_Stop();

    ISP_FreeISP(); 
     
    return ;
}

/****************************************************************************************/
// Description: osd overlay
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      uint16* src_addr: the pointer of source addr
//                      ISP_OSD_PARAM_T *osd_cfg_ptr: the pointer of osd cfg
// output parameter:
//                      None
// return:
//          None 
// Note:
/*****************************************************************************************/
#ifndef OSD_ROM_CODE
#pragma arm section code = "ISP_OSD"
// allready fixed code, can not modify the code below about osd pile up
PUBLIC void _Osd_Pileup(uint16 *src_addr, ISP_OSD_PARAM_T *osd_cfg_ptr)
{ 	
#define ALPHA_MASK          		0x7bef
#define ALPHA_DIS					0XFF
#define PLANE_COLOR_MASK    		0x0020 

    OSD_INFO_T *osd_info_ptr = osd_cfg_ptr->inf_ptr;
    uint32 src_width = osd_cfg_ptr->background_size.w;
    uint32 src_height = osd_cfg_ptr->background_size.h;

    register uint16 *src_ptr = src_addr;
    register uint32 *osd_word_ptr = NULL;
    register uint16 *osd_ptr 	= NULL;
    register uint32 color_key = 0;
    register uint32 osd_width = 0;
    register uint32 osd_height = 0;
    register uint32 src_value = 0;
    register uint32 osd_word_value 	= 0;
    register uint16 osd_value = 0;
    register uint32 src_stride = 0;
    register uint32 i 	= 0;
    register uint32 j = 0;		

    while (PNULL != osd_info_ptr)
    {
        osd_width 	 = osd_info_ptr->rect.w;
        osd_height 	 = osd_info_ptr->rect.h;    
        osd_word_ptr = (uint32 *)osd_info_ptr->addr;
        osd_ptr 	 = (uint16 *)osd_info_ptr->addr;
        src_stride 	 = src_width - osd_width;
        color_key 	 = osd_info_ptr->colorkey;

        if ((osd_width == src_width && osd_height == src_height)    		
        ||((osd_width == src_width) && (osd_height != src_height) && (ALPHA_DIS == osd_info_ptr->alpha)))

        {
            if(osd_height == src_height)
            {    			
                src_ptr = src_addr;
            }
            else
            {
                src_ptr = src_addr + src_width * osd_info_ptr->rect.y;
            }

            i = (src_width * osd_height) >> 1;
            while (i--)
            {
                osd_word_value = *osd_word_ptr++;
                osd_value = (uint16)(osd_word_value >> 16);

                if (osd_value != color_key)
                {
                    *src_ptr = osd_value; 
                } 

                src_ptr++;

                osd_value = (uint16)osd_word_value;

                if (osd_value != color_key)
                {
                    *src_ptr = osd_value; 
                }

                src_ptr++;      
            }		
        }
        else
        {
            src_ptr = src_addr + src_width * osd_info_ptr->rect.y + osd_info_ptr->rect.x;

            for (i=0; i<osd_height; i++)
            {
                for (j=0; j<osd_width; j++)
                {
                    osd_value = *osd_ptr++;
                    if (osd_value != color_key)
                    {
                        if (0 == (osd_value & PLANE_COLOR_MASK))
                        {
                            // do alpha
                            src_value = *src_ptr;
                            src_value = ((osd_value >> 1) & ALPHA_MASK) + ((src_value >> 1) & ALPHA_MASK);                            
                        }
                        else
                        {
                            src_value = osd_value;
                        }

                        *src_ptr = src_value;
                    } 

                    src_ptr++;
                }

                src_ptr += src_stride;
            }   
        }

        osd_info_ptr = osd_info_ptr->next_ptr;
    }
}
#pragma arm section code 
#endif

typedef void (* OSD_PILEUP)(uint16 *src_addr, ISP_OSD_PARAM_T *osd_cfg_ptr);

PUBLIC void _ISP_Osd_Pileup(uint16 *src_addr)
{
#if (defined(PLATFORM_SC6600L))
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint8 i=0x00;
    OSD_PILEUP osd_pileup_ptr = PNULL;
    BLOCKCFG_T lcdc_osd_block_cfg={0x00};
    ISP_OSD_PARAM_T* isp_osd_cfg_ptr=NULL;    
    ISP_OSD_PARAM_T isp_osd_cfg={0x00};
    OSD_INFO_T isp_osd_info={0x00};

    LCDC_GetBlockCtrl(pISPContext->lcd_id, 2);

    osd_pileup_ptr = (OSD_PILEUP)OSD_ROM_CODE_ADDR; 
    //osd_pileup_ptr = _Osd_Pileup;

    for(i=0x02; i<0xff; i++)
    {
        if(0x00==LCDC_GetBlockState(pISPContext->lcd_id, i))
        {
            break;
        }

        LCDC_GetConfigBlock(pISPContext->lcd_id, i, &lcdc_osd_block_cfg);

        isp_osd_cfg_ptr=LCDC_GetOsdBlockRect();

        SCI_MEMCPY((void*)&(isp_osd_cfg),(void*)(isp_osd_cfg_ptr),sizeof(ISP_OSD_PARAM_T));

        if(SCI_FALSE==isp_osd_cfg.enable)
        {
            
            if((ISP_ROTATION_0==pISPContext->rotation)||(ISP_ROTATION_180==pISPContext->rotation))
            {
                isp_osd_cfg.background_size.w=pISPContext->disp_rect.w;
                isp_osd_cfg.background_size.h=pISPContext->disp_rect.h;
            }
            else
            {
                isp_osd_cfg.background_size.w=pISPContext->disp_rect.h;
                isp_osd_cfg.background_size.h=pISPContext->disp_rect.w;
            }

            isp_osd_cfg.inf_ptr=&isp_osd_info;
            
            isp_osd_cfg.inf_ptr->rect.x=lcdc_osd_block_cfg.start_x;
            isp_osd_cfg.inf_ptr->rect.y=lcdc_osd_block_cfg.start_y;
            isp_osd_cfg.inf_ptr->rect.w=lcdc_osd_block_cfg.end_x-lcdc_osd_block_cfg.start_x+0x01;            
            isp_osd_cfg.inf_ptr->rect.h=lcdc_osd_block_cfg.end_y-lcdc_osd_block_cfg.start_y+0x01;
            isp_osd_cfg.inf_ptr->addr=(uint32)LCDC_GetBlockBuffer(pISPContext->lcd_id, i);
            isp_osd_cfg.inf_ptr->colorkey=lcdc_osd_block_cfg.colorkey;
            isp_osd_cfg.inf_ptr->alpha=0xff;
            isp_osd_cfg.inf_ptr->next_ptr=PNULL;
            
        }

        osd_pileup_ptr(src_addr, &isp_osd_cfg);
        
    }
    
    LCDC_ReleaseBlockCtrl(pISPContext->lcd_id, 2);    
#endif
        
}

/****************************************************************************************/
// Description: Set img sensor parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                None
// output parameter:
//                None
// return:
//       None
// Note:
/*****************************************************************************************/
LOCAL void _ISP_SetSensorParam(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    
    if(PNULL!=pISPContext->isp_sensor_ctl.set_param)
    {
        pISPContext->isp_sensor_ctl.set_param(pISPContext->isp_sensor_ctl.cmd, pISPContext->isp_sensor_ctl.arg);
        pISPContext->isp_sensor_ctl.set_param=PNULL;

        if(PNULL==pISPContext->isp_sensor_ctl.set_param_success)
        {
            _ISP_SetEvent(ISP_SET_PARAM_EVENT);
        }
    }

    return ;
}

/****************************************************************************************/
// Description: Set img sensor parameter callback
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                None
// output parameter:
//                None
// return:
//       None
// Note:
/*****************************************************************************************/
LOCAL void _ISP_SetSensorParamCallback(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    
    if((PNULL==pISPContext->isp_sensor_ctl.set_param)
        &&(PNULL!=pISPContext->isp_sensor_ctl.set_param_success))
    {        
        pISPContext->isp_sensor_ctl.set_param_success(NULL);
        pISPContext->isp_sensor_ctl.set_param_success=PNULL;
    }     

    return ;
}

/****************************************************************************************/
// Description: Set img sensor parameter ctrl
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_SENSOR_PARAM_T_PTR: the pointer of sensor parameter
//                      syn_write: the flag of syn write
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail 
// Note:
/*****************************************************************************************/
LOCAL void _ISP_SetSensorParamCtrl(ISP_SENSOR_PARAM_T_PTR param_ptr)
{
    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceSetSensorParam2, 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5301_112_2_18_1_43_46_87,(uint8*)"d", param_ptr);
    
    if(NULL!=param_ptr)
    {
        ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

        pISPContext->isp_sensor_ctl.cmd=param_ptr->cmd;
        pISPContext->isp_sensor_ctl.arg=param_ptr->arg; 
        pISPContext->isp_sensor_ctl.syn_ctrl=param_ptr->syn_ctrl;
        pISPContext->isp_sensor_ctl.set_param=param_ptr->set_param;
        pISPContext->isp_sensor_ctl.set_param_success=param_ptr->set_param_success;

        //SCI_MEMCPY(pISPContext->isp_sensor_ctl, param_ptr, sizeof(ISP_SENSOR_PARAM_T));

        SCI_FREE(param_ptr);
        //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceSetSensorParam2, 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5316_112_2_18_1_43_46_88,(uint8*)"d", pISPContext->isp_sensor_ctl.syn_ctrl);
        if(SCI_TRUE==pISPContext->isp_sensor_ctl.syn_ctrl)
        {
            pISPContext->skip_counter=ISP_MAX_SKIP_NUM;
            _ISP_SetSensorParam();
            pISPContext->skip_counter=ISP_NORMAL_SKIP_NUM;
        }
    }

    return ;    
}

/****************************************************************************************/
// Description: Set img sensor eof ctrl
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_SENSOR_FRAME_COUNTER_T_PTR: the pointer of sensor frame counter
// output parameter:
//                      None
// return:
//          None
// Note:
/*****************************************************************************************/
LOCAL void _ISP_SetSensorEofCtrl(ISP_SENSOR_FRAME_COUNTER_T_PTR param_ptr)
{
    //SCI_TRACE_ISP:"ISP_SERVICE: _ISP_SetSensorEofCtrl, 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5340_112_2_18_1_43_46_89,(uint8*)"d", param_ptr);

    if(NULL!=param_ptr)
    {
        ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

        pISPContext->isp_sensor_frame_counter.num=param_ptr->num;
        pISPContext->isp_sensor_frame_counter.cur_num=ISP_ZERO;
        pISPContext->isp_sensor_frame_counter.callback=param_ptr->callback;

        SCI_FREE(param_ptr);

        IVsp_RegisterIntFuncEx(SENSOR_EOF_ID, _ISP_ISR_SensorEndOfFrameLow, _ISP_ISR_SensorEndOfFrameHigh);
        IVsp_ClearInt(SENSOR_EOF_ID);
        IVsp_OpenInt(SENSOR_EOF_ID);
    }

    return ;
}

/****************************************************************************************/
// Description: Check the preview parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_PREVIEW_PARAM_T * param_ptr: the pointer of review parameter
// output parameter:
//                      None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_CheckPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
    if((NULL!=(param_ptr->input_rect.x&ISP_ALIGN_4))
        ||(NULL!=(param_ptr->input_rect.w&ISP_ALIGN_4))
        ||(NULL!=(param_ptr->target_rect.w&ISP_ALIGN_4))
        ||(NULL!=(param_ptr->target_rect.h&ISP_ALIGN_2)))
    {	
        //SCI_TRACE_ISP:"ISP_SERVICE: All width must be 4 pixel align rect x%d, inpur rect w%d, disp rect w%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5377_112_2_18_1_43_46_90,(uint8*)"ddd",param_ptr->input_rect.x, param_ptr->input_rect.w, param_ptr->disp_rect.w%4);
        return SCI_ERROR;
    }

    if(param_ptr->disp_lcd_id >LCD_MAX_ID)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Invalid lcd id %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5383_112_2_18_1_43_46_91,(uint8*)"d", param_ptr->disp_lcd_id );
        return SCI_ERROR;
    }

    if(ISP_SERVICE_ZOOM_MAX<=param_ptr->init_zoom_level)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Invalid zoom level %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5389_112_2_18_1_43_46_92,(uint8*)"d", param_ptr->init_zoom_level);
        return SCI_ERROR;
    }

    if((param_ptr->input_size.w < (param_ptr->input_rect.x + param_ptr->input_rect.w)) ||
        (param_ptr->input_size.h < (param_ptr->input_rect.y + param_ptr->input_rect.h)))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5398_112_2_18_1_43_46_93,(uint8*)"dddddd",param_ptr->input_rect.x, param_ptr->input_rect.y, param_ptr->input_rect.w, param_ptr->input_rect.h, param_ptr->input_size.w, param_ptr->input_size.h);
        return SCI_ERROR;
    }

    //hardware don't support scale up when preview mode
    if((param_ptr->input_rect.w>(param_ptr->target_rect.w*ISP_GetMaxScaleDownCoeff()*ISP_GetMaxCapDeci()))
        ||(param_ptr->input_rect.h>(param_ptr->target_rect.h*ISP_GetMaxScaleDownCoeff()*ISP_GetMaxCapDeci()))
        ||((param_ptr->input_rect.w*ISP_GetMaxPreviewScaleUpCoeff())<param_ptr->target_rect.w)
        ||((param_ptr->input_rect.h*ISP_GetMaxPreviewScaleUpCoeff())<param_ptr->target_rect.h))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5408_112_2_18_1_43_46_94,(uint8*)"dddd",param_ptr->input_rect.w,param_ptr->input_rect.h,param_ptr->target_rect.w,param_ptr->target_rect.h);
        return SCI_ERROR;
    }

    if(ISP_ROTATION_MAX<param_ptr->disp_rotation)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: the lcd ration param %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5414_112_2_18_1_43_46_95,(uint8*)"d",param_ptr->disp_rotation);
        return SCI_ERROR;
    }

    if((ISP_ROTATION_90 == param_ptr->disp_rotation) || (ISP_ROTATION_270 == param_ptr->disp_rotation)) // rotation 90 or 270
    {			
        if(((param_ptr->disp_rect.y+param_ptr->disp_rect.h) > ISP_GetMaxDisplayWidth())
            ||((param_ptr->disp_rect.x+param_ptr->disp_rect.w)>ISP_GetMaxDisplayHeight()))
        {
            //SCI_TRACE_ISP:"ISP_SERVICE: Display size is larger than max lcd size"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5423_112_2_18_1_43_46_96,(uint8*)"");
            return SCI_ERROR;
        }
    }
    else
    {
        if(((param_ptr->disp_rect.x+param_ptr->disp_rect.w) > ISP_GetMaxDisplayWidth())
            ||((param_ptr->disp_rect.y+param_ptr->disp_rect.h)>ISP_GetMaxDisplayHeight()))
        {
            //SCI_TRACE_ISP:"ISP_SERVICE: Display size is larger than max lcd size"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5432_112_2_18_1_43_46_97,(uint8*)"");
            return SCI_ERROR;
        }
    }

    return SCI_SUCCESS;

}
/****************************************************************************************/
// Description: Check the capture param
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_JPEG_PARAM_T * param_ptr: the pointer of capture parameter
// output parameter:
//                      None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_CheckCaptureParam(ISP_JPEG_PARAM_T * param_ptr)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();    
    uint32 capture_addr=0x00;    
    uint32 capture_size=0x00;

    if(ISP_DATA_JPEG != param_ptr->sensor_data_format)
    {
        if((param_ptr->input_size.w < (param_ptr->input_rect.x + param_ptr->input_rect.w)) ||
            (param_ptr->input_size.h < (param_ptr->input_rect.y + param_ptr->input_rect.h)))
        {
            //SCI_TRACE_ISP:"ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5463_112_2_18_1_43_46_98,(uint8*)"dddddd",param_ptr->input_rect.x, param_ptr->input_rect.y, param_ptr->input_rect.w, param_ptr->input_rect.h, param_ptr->input_size.w, param_ptr->input_size.h);
            return SCI_ERROR;
        }
        
        // 422 jpg encoder (include the all scal/detic/Trim range) 		
        if((NULL!=(param_ptr->input_rect.x&ISP_ALIGN_4))
            ||(NULL!=(param_ptr->input_rect.w&ISP_ALIGN_4))
            ||(NULL!=(param_ptr->output_size.w&ISP_ALIGN_4)))
        {
           //SCI_TRACE_ISP:"ISP_SERVICE: All width must be 4 pixel align, input rect w%d h%d, output w%d h%d"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5472_112_2_18_1_43_46_99,(uint8*)"dddd", param_ptr->input_rect.w,param_ptr->input_rect.h, param_ptr->output_size.w, param_ptr->output_size.h);
        }

        if(SCI_FALSE==pISPContext->is_zoom)
        {
            capture_addr=ISP_Align(param_ptr->capture_addr, ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);            
            capture_size=ISP_Align((param_ptr->output_size.w*param_ptr->output_size.h), ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
            capture_addr+=(capture_size+(param_ptr->output_size.w*param_ptr->output_size.h));
        }
        else
        {
            capture_addr=ISP_Align(param_ptr->capture_addr, ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);            
            capture_size=ISP_Align(((param_ptr->output_size.w*param_ptr->output_size.h)>>ISP_SHIFT_BIT2), ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
            capture_addr+=(capture_size+((param_ptr->output_size.w*param_ptr->output_size.h)>>ISP_SHIFT_BIT2));
        }
        
        if((param_ptr->capture_addr+param_ptr->capture_buf_size)<capture_addr)
        {
            //SCI_TRACE_ISP:"ISP_SERVICE:The capture size w:%d h:%d ,the capture buffer 0x%x is not enough"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5490_112_2_18_1_43_46_100,(uint8*)"ddd",param_ptr->output_size.w, param_ptr->output_size.h, param_ptr->capture_buf_size);
            return SCI_ERROR;
        }

    }
    else
    {
        if((0x00==param_ptr->capture_addr)
            ||(0x00!=(0xff&param_ptr->capture_addr)))
        {
            //SCI_TRACE_ISP:"ISP_SERVICE: Jpg sensor output addr 0x%x is error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5500_112_2_18_1_43_46_101,(uint8*)"d",param_ptr->capture_addr);
            return SCI_ERROR;
        }
    }

    return SCI_SUCCESS;
}
/****************************************************************************************/
// Description: Check the review parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_REVIEW_PARAM_T *param_ptr: the pointer of review parameter 
// output parameter:
//                      None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_CheckReviewParam(ISP_REVIEW_PARAM_T *param_ptr)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint32 buf_size=0x00;
    uint32 buf_addr=0x00;

    pISPContext->is_dir_display=SCI_FALSE;
    if((param_ptr->input_size.w < (param_ptr->input_rect.x + param_ptr->input_rect.w)) ||
        (param_ptr->input_size.h < (param_ptr->input_rect.y + param_ptr->input_rect.h)))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5529_112_2_18_1_43_46_102,(uint8*)"dddddd",param_ptr->input_rect.x, param_ptr->input_rect.y, param_ptr->input_rect.w, param_ptr->input_rect.h, param_ptr->input_size.w, param_ptr->input_size.h);

        return SCI_ERROR;
    }

    if((param_ptr->input_rect.x&ISP_ALIGN_4)
        ||(param_ptr->input_rect.w&ISP_ALIGN_4)
        ||(param_ptr->target_rect.w&ISP_ALIGN_4)
        ||(param_ptr->target_rect.h&ISP_ALIGN_2))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Review Paramter not align Input rect: x:%d, w:%d output w:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5540_112_2_18_1_43_46_103,(uint8*)"ddd",param_ptr->input_rect.x, param_ptr->input_rect.w, param_ptr->target_rect.w);

        return SCI_ERROR;
    }    

    if((param_ptr->input_rect.w>param_ptr->target_rect.w*ISP_GetMaxScaleDownCoeff()*ISP_GetMaxSubSampleMul())
        ||(param_ptr->input_rect.h>param_ptr->target_rect.h*ISP_GetMaxScaleDownCoeff()*ISP_GetMaxSubSampleMul()))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5548_112_2_18_1_43_46_104,(uint8*)"dddd",param_ptr->input_rect.w,param_ptr->input_rect.h,param_ptr->disp_rect.w,param_ptr->disp_rect.h);
        return SCI_ERROR;	
    }

    if((param_ptr->input_rect.w*ISP_GetMaxScaleUpCoeff()<param_ptr->target_rect.w)
        ||(param_ptr->input_rect.h*ISP_GetMaxScaleUpCoeff()<param_ptr->target_rect.h))
    {
	    if((param_ptr->input_rect.w>param_ptr->target_rect.w)||(param_ptr->input_rect.h>param_ptr->target_rect.h))
	    {
	        //SCI_TRACE_ISP:"ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5557_112_2_18_1_43_46_105,(uint8*)"dddd",param_ptr->input_rect.w,param_ptr->input_rect.h,param_ptr->target_rect.w,param_ptr->target_rect.h);
	        return SCI_ERROR;	
	    }    
        pISPContext->is_dir_display=SCI_TRUE;
        //SCI_TRACE_ISP:"ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5561_112_2_18_1_43_46_106,(uint8*)"dddd",param_ptr->input_rect.w,param_ptr->input_rect.h,param_ptr->target_rect.w,param_ptr->target_rect.h);
    }

    if((param_ptr->input_format != ISP_DATA_YUV422)
        &&(param_ptr->input_format != ISP_DATA_YUV420)
        &&(param_ptr->input_format != ISP_DATA_YUV400) 
        #if (defined(PLATFORM_SC6800H))
        &&(param_ptr->input_format != ISP_DATA_YUV420_3FRAME)
        &&(param_ptr->input_format != ISP_DATA_RGB888)         
        #endif
        &&(param_ptr->input_format != ISP_DATA_RGB565))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Input format %d is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5573_112_2_18_1_43_46_107,(uint8*)"d", param_ptr->input_format);
        
        return SCI_ERROR;
    }

    if((NULL!=(param_ptr->src_addr.y_addr&ISP_BUFFER_ADDR_ALIGN))
        ||(NULL!=(param_ptr->src_addr.uv_addr&ISP_BUFFER_ADDR_ALIGN)))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Input data y addr 0x%x uv addr 0x%x isn't aligned!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5581_112_2_18_1_43_46_108,(uint8*)"dd",param_ptr->src_addr.y_addr,param_ptr->src_addr.uv_addr);

        return SCI_ERROR;
    }
    
    if((ISP_ROTATION_90 == param_ptr->disp_rotation) || (ISP_ROTATION_270 == param_ptr->disp_rotation)) // rotation 90 or 270
    {			
        if(((param_ptr->disp_rect.y+param_ptr->disp_rect.h) > ISP_GetMaxDisplayWidth())
            ||((param_ptr->disp_rect.x+param_ptr->disp_rect.w)>ISP_GetMaxDisplayHeight()))
        {
            //SCI_TRACE_ISP:"ISP_SERVICE: Display size is larger than max lcd size"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5591_112_2_18_1_43_46_109,(uint8*)"");
            
            return SCI_ERROR;
        }
    }
    else
    {
        if(((param_ptr->disp_rect.x+param_ptr->disp_rect.w) > ISP_GetMaxDisplayWidth())
            ||((param_ptr->disp_rect.y+param_ptr->disp_rect.h)>ISP_GetMaxDisplayHeight()))
        {
            //SCI_TRACE_ISP:"ISP_SERVICE: Display size is larger than max lcd size"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5601_112_2_18_1_43_46_110,(uint8*)"");
            
            return SCI_ERROR;
        }
    }

    // check the source buf
    buf_size=param_ptr->input_size.w*param_ptr->input_size.h;

    if(ISP_DATA_RGB565==param_ptr->input_format)
    {
        buf_size<<=ISP_SHIFT_BIT1;
        if(param_ptr->src_len.y_len<buf_size)
        {
            //SCI_TRACE_ISP:"ISP_SERVICE:The src size w:%d h:%d ,The source review buffer 0x%x is not enough"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5615_112_2_18_1_43_46_111,(uint8*)"ddd",param_ptr->input_size.w, param_ptr->input_size.h, param_ptr->src_len.y_len);

            return SCI_ERROR;
        }
    }
    else
    {
        if(param_ptr->src_len.y_len<buf_size)
        {
            //SCI_TRACE_ISP:"ISP_SERVICE:The src size w:%d h:%d ,The source review y buffer 0x%x is not enough"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5624_112_2_18_1_43_46_112,(uint8*)"ddd",param_ptr->input_size.w, param_ptr->input_size.h, param_ptr->src_len.y_len);

            return SCI_ERROR;
        }

        switch(param_ptr->input_format)
        {
            case ISP_DATA_YUV422:           
                buf_size=param_ptr->input_size.w*param_ptr->input_size.h;            
                break;
                
            case ISP_DATA_YUV420:
                buf_size=((param_ptr->input_size.w*param_ptr->input_size.h)>>ISP_SHIFT_BIT1);                  
                break;

            case ISP_DATA_YUV420_3FRAME:
                buf_size=((param_ptr->input_size.w*param_ptr->input_size.h)>>2);          
                break;

            case ISP_DATA_YUV400:
                buf_size=0x00;                  
                break;

            default :
                //SCI_TRACE_ISP:"ISP_SERVICE:The review data format %d error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5648_112_2_18_1_43_46_113,(uint8*)"d",param_ptr->input_format);
                return SCI_ERROR;
        }

        if(param_ptr->src_len.uv_len<buf_size)
        {
            //SCI_TRACE_ISP:"ISP_SERVICE:The src size w:%d h:%d ,The source review uv buffer 0x%x is not enough"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5654_112_2_18_1_43_46_114,(uint8*)"ddd",param_ptr->input_size.w, param_ptr->input_size.h, param_ptr->src_len.y_len);
            return SCI_ERROR;
        }        
        
    }

    // check the distion buf
    buf_size=param_ptr->target_rect.w*param_ptr->target_rect.h;
    
    if(ISP_DATA_RGB565==param_ptr->disp_format)
    {
        buf_size=ISP_Align((buf_size<<ISP_SHIFT_BIT1), ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
    }
    else
    {
        buf_size=ISP_Align(buf_size, ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT)<<ISP_SHIFT_BIT1;
    }

    buf_addr=ISP_Align(param_ptr->disp_addr, ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
    buf_addr+=buf_size;

    if((param_ptr->disp_addr+param_ptr->disp_buf_size)<buf_addr)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE:The review disp size w:%d h:%d ,the review buffer 0x%x is not enough"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5677_112_2_18_1_43_46_115,(uint8*)"ddd",param_ptr->disp_rect.w, param_ptr->disp_rect.h, param_ptr->disp_buf_size);
        return SCI_ERROR;
    }
    
    return SCI_SUCCESS;	
}

/****************************************************************************************/
// Description: Check the Mjpg parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                     ISP_MJPEG_PARAM_T * param_ptr: the pointer of Mjpg parameter
// output parameter:
//                      None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_CheckMjpegParam(ISP_MJPEG_PARAM_T * param_ptr)
{
    uint32 capture_addr=0x00;    
    uint32 capture_size=0x00;

    uint32 disp_addr=0x00;       
    uint32 disp_size=0x00;

    if((param_ptr->input_size.w < (param_ptr->input_rect.x + param_ptr->input_rect.w)) ||
        (param_ptr->input_size.h < (param_ptr->input_rect.y + param_ptr->input_rect.h)))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5707_112_2_18_1_43_47_116,(uint8*)"dddddd",param_ptr->input_rect.x, param_ptr->input_rect.y, param_ptr->input_rect.w, param_ptr->input_rect.h, param_ptr->input_size.w, param_ptr->input_size.h);
        return SCI_ERROR;
    }

    if((param_ptr->input_rect.w<param_ptr->capture_size.w) || 
        (param_ptr->input_rect.h<param_ptr->capture_size.h))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE:Input rect w%d, h%d capture w%d, h%d ISP don't support scale up when Mjpeg capture!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5715_112_2_18_1_43_47_117,(uint8*)"dddd",param_ptr->input_rect.w,param_ptr->input_rect.h,param_ptr->capture_size.w,param_ptr->capture_size.h);
        return SCI_ERROR;
    }

    if(ISP_ROTATION_MAX<param_ptr->disp_rotation)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: the lcd ration param %d is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5721_112_2_18_1_43_47_118,(uint8*)"d",param_ptr->disp_rotation);
        return SCI_ERROR;
    }

    if((ISP_ROTATION_90 == param_ptr->disp_rotation) || (ISP_ROTATION_270 == param_ptr->disp_rotation)) // rotation 90 or 270
    {			
        if(((param_ptr->disp_rect.y+param_ptr->disp_rect.h) > ISP_GetMaxDisplayWidth())
            ||((param_ptr->disp_rect.x+param_ptr->disp_rect.w)>ISP_GetMaxDisplayHeight()))
        {
            //SCI_TRACE_ISP:"ISP_SERVICE: Display size is larger than max lcd size"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5730_112_2_18_1_43_47_119,(uint8*)"");
            return SCI_ERROR;
        }
    }
    else
    {
        if(((param_ptr->disp_rect.x+param_ptr->disp_rect.w) > ISP_GetMaxDisplayWidth())
            ||((param_ptr->disp_rect.y+param_ptr->disp_rect.h)>ISP_GetMaxDisplayHeight()))
        {
            //SCI_TRACE_ISP:"ISP_SERVICE: Display size is larger than max lcd size"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5739_112_2_18_1_43_47_120,(uint8*)"");
            return SCI_ERROR;
        }
    }

    capture_size=ISP_Align((param_ptr->capture_size.w*param_ptr->capture_size.h), ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
    capture_addr=ISP_Align(param_ptr->capture_addr, ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
    capture_addr+=(capture_size<<ISP_SHIFT_BIT1);
    if((param_ptr->capture_addr+param_ptr->capture_buf_size)<capture_addr)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE:The mjpg capture size w:%d h:%d ,the mjpg capture buffer 0x%x is not enough"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5749_112_2_18_1_43_47_121,(uint8*)"ddd",param_ptr->capture_size.w, param_ptr->capture_size.h, param_ptr->capture_buf_size);
        return SCI_ERROR;
    }

    disp_size=(param_ptr->target_rect.w*param_ptr->target_rect.h)<<ISP_SHIFT_BIT1;
    disp_addr=ISP_Align(param_ptr->disp_addr, ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
    disp_addr+=disp_size;
    
    if((param_ptr->disp_addr+param_ptr->disp_buf_size)<disp_addr)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE:The mjpg review disp size w:%d h:%d ,the review buffer 0x%x is not enough"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5759_112_2_18_1_43_47_122,(uint8*)"ddd",param_ptr->target_rect.w, param_ptr->target_rect.h, param_ptr->disp_buf_size);
        return SCI_ERROR;
    }	

    if(PNULL==param_ptr->get_cap_data)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: The mjpg get capture data callback is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5765_112_2_18_1_43_47_123,(uint8*)"");
        return SCI_ERROR;
    }    

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Check the UPCC parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                     ISP_MJPEG_PARAM_T * param_ptr: the pointer of Mjpg parameter
// output parameter:
//                      None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_CheckUpccParam(ISP_MJPEG_PARAM_T * param_ptr)
{
    uint32 capture_addr=0x00;    
    uint32 capture_size=0x00;

    if((param_ptr->input_size.w < (param_ptr->input_rect.x + param_ptr->input_rect.w)) ||
        (param_ptr->input_size.h < (param_ptr->input_rect.y + param_ptr->input_rect.h)))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5792_112_2_18_1_43_47_124,(uint8*)"dddddd",param_ptr->input_rect.x, param_ptr->input_rect.y, param_ptr->input_rect.w, param_ptr->input_rect.h, param_ptr->input_size.w, param_ptr->input_size.h);
        return SCI_ERROR;
    }

    if((param_ptr->input_rect.w<param_ptr->capture_size.w) || 
        (param_ptr->input_rect.h<param_ptr->capture_size.h))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE:Input rect w%d, h%d capture w%d, h%d ISP don't support scale up when Mjpeg capture!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5800_112_2_18_1_43_47_125,(uint8*)"dddd",param_ptr->input_rect.w,param_ptr->input_rect.h,param_ptr->capture_size.w,param_ptr->capture_size.h);
        return SCI_ERROR;
    }

    if((param_ptr->input_rect.w>param_ptr->jpeg_target_size.w*ISP_GetMaxScaleDownCoeff())
        ||(param_ptr->input_rect.h>param_ptr->jpeg_target_size.h*ISP_GetMaxScaleDownCoeff()))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: The input size w:%d, h:%d, output size w:%d, h:%d, the scale error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5807_112_2_18_1_43_47_126,(uint8*)"dddd",param_ptr->input_rect.w,param_ptr->input_rect.h,param_ptr->jpeg_target_size.w,param_ptr->jpeg_target_size.h);
        return SCI_ERROR;
    }    

    capture_size=ISP_Align((param_ptr->jpeg_target_size.w*param_ptr->jpeg_target_size.h), ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
    capture_addr=ISP_Align(param_ptr->capture_addr, ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
    capture_addr+=(capture_size<<ISP_SHIFT_BIT1);
    if((param_ptr->capture_addr+param_ptr->capture_buf_size)<capture_addr)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE:The mjpg capture size w:%d h:%d ,the mjpg capture buffer 0x%x is not enough"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5816_112_2_18_1_43_47_127,(uint8*)"ddd",param_ptr->jpeg_target_size.w, param_ptr->jpeg_target_size.h, param_ptr->capture_buf_size);
        return SCI_ERROR;
    }

    if(PNULL==param_ptr->get_cap_data)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: The mjpg get capture data callback is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5822_112_2_18_1_43_47_128,(uint8*)"");
        return SCI_ERROR;
    }    

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Get the scale input height
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_SCALE_PARAM_T * param_ptr: the pointer of scaling parameter
// output parameter:
//                      None
// return:
//          NSUCCESS: scale input height     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_GetScaleInputHeight(ISP_SCALE_PARAM_T * param_ptr)
{
    if(ISP_SERVICE_SCALE_NORMAL==param_ptr->scale_mode)
    {
        return param_ptr->input_size.h;
    }
    else if(ISP_SERVICE_SCALE_SLICE==param_ptr->scale_mode)
    {
        return param_ptr->slice_line;
    }

    return 0x00;
}

/****************************************************************************************/
// Description: Get the scale output height
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_SCALE_PARAM_T * param_ptr: the pointer of scaling parameter
// output parameter:
//                      None
// return:
//          NSUCCESS: scale out height     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_GetScaleOutputHeight(ISP_SCALE_PARAM_T * param_ptr)
{
    if(ISP_SERVICE_SCALE_NORMAL==param_ptr->scale_mode)
    {
        return param_ptr->output_size.h;
    }
    else if(ISP_SERVICE_SCALE_SLICE==param_ptr->scale_mode)
    {
        return ((param_ptr->output_size.h*param_ptr->slice_line)/param_ptr->input_rect.h);
    }

    return 0x00;
}

/****************************************************************************************/
// Description: Check the scale parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_SCALE_PARAM_T * param_ptr: the pointer of scaling parameter
// output parameter:
//                      None
// return:
//          NSUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_CheckScaleParam(ISP_SCALE_PARAM_T * param_ptr)
{
    uint32 output_addr=0x00;       
    uint32 output_size=0x00;

    if((param_ptr->input_size.w < (param_ptr->input_rect.x + param_ptr->input_rect.w)) ||
        (param_ptr->input_size.h < (param_ptr->input_rect.y + param_ptr->input_rect.h)))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Input rect range x:%d, y%d, w%d, h%d  is out of input size w%d, h%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5897_112_2_18_1_43_47_129,(uint8*)"dddddd",param_ptr->input_rect.x, param_ptr->input_rect.y, param_ptr->input_rect.w, param_ptr->input_rect.h, param_ptr->input_size.w, param_ptr->input_size.h);
        return SCI_ERROR;
    }

    if((param_ptr->input_rect.x&ISP_INPTU_REVIEW_ALIGN)||(param_ptr->input_rect.w&ISP_INPTU_REVIEW_ALIGN))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Scale Paramter not align Input rect: x%d, w%d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5903_112_2_18_1_43_47_130,(uint8*)"dd",param_ptr->input_rect.x, param_ptr->input_rect.w);
        return SCI_ERROR;
    } 

    if(ISP_DATA_RGB565==param_ptr->output_format)
    {
        if(param_ptr->output_size.w&ISP_OUTPUT_RGB_SCALE_ALIGN)
        {
            //SCI_TRACE_ISP:"ISP_SERVICE: Scale Paramter not align  Output: w%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5911_112_2_18_1_43_47_131,(uint8*)"d", param_ptr->output_size.w);
            return SCI_ERROR;
        } 
    }
    else
    {
        if(param_ptr->output_size.w&ISP_OUTPUT_SCALE_ALIGN)
        {
            //SCI_TRACE_ISP:"ISP_SERVICE: Scale Paramter not align  Output: w%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5919_112_2_18_1_43_47_132,(uint8*)"d", param_ptr->output_size.w);
            return SCI_ERROR;
        } 
    }     

    if((param_ptr->input_rect.w*ISP_GetMaxScaleUpCoeff()<param_ptr->output_size.w)
        ||(param_ptr->input_rect.h*ISP_GetMaxScaleUpCoeff()<param_ptr->output_size.h)
        ||(param_ptr->input_rect.w>param_ptr->output_size.w*ISP_GetMaxScaleDownCoeff()*ISP_GetMaxSubSampleMul())
        ||(param_ptr->input_rect.h>param_ptr->output_size.h*ISP_GetMaxScaleDownCoeff()*ISP_GetMaxSubSampleMul()))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Scale multiply error input_rect: w%d, h%d output: w%d, h%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5929_112_2_18_1_43_47_133,(uint8*)"dddd", param_ptr->input_rect.w,param_ptr->input_rect.h,param_ptr->output_size.w,param_ptr->output_size.h);
        return SCI_ERROR;
    }

    if((ISP_GetMaxScaleWidth()<param_ptr->output_size.w)&&(ISP_SERVICE_SCALE_SLICE!=param_ptr->scale_mode))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Scale mode error out_width: w%d, scale_mode:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5935_112_2_18_1_43_47_134,(uint8*)"dd", param_ptr->output_size.w, param_ptr->scale_mode);
        return SCI_ERROR;
    }

    if((ISP_SERVICE_SCALE_SLICE==param_ptr->scale_mode)&&((PNULL==param_ptr->get_scale_data)))
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Slice scale callback error: get_ptr:0x=%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5941_112_2_18_1_43_47_135,(uint8*)"d",  param_ptr->get_scale_data);
        return SCI_ERROR;
    } 


    if(NULL==param_ptr->input_addr.y_addr)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE:The input adr is null error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5948_112_2_18_1_43_47_136,(uint8*)"");
        return SCI_ERROR;
    }
    
    output_size=ISP_Align(param_ptr->output_size.w*_ISP_GetScaleOutputHeight(param_ptr), ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);
    output_addr=ISP_Align(param_ptr->output_addr.y_addr, ISP_BUFFER_ADDR_ALIGN, ISP_BUFFER_ALIGN_BIT);    

    switch(param_ptr->output_format)
    {
        case ISP_DATA_RGB565:
            output_addr+=((param_ptr->output_size.w*_ISP_GetScaleOutputHeight(param_ptr))<<ISP_SHIFT_BIT1);            
            break;
            
        case ISP_DATA_YUV422:
            output_addr+=(output_size+(param_ptr->output_size.w*_ISP_GetScaleOutputHeight(param_ptr)));            
            break;  
            
        default :
            //SCI_TRACE_ISP:"ISP_SERVICE:The scale data format %d error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5966_112_2_18_1_43_47_137,(uint8*)"d",param_ptr->input_format);
            return SCI_ERROR;
    }    
    
    if((param_ptr->output_addr.y_addr+param_ptr->output_buf_size)<output_addr)
    {
        //SCI_PASSERT(0, ("ISP_SERVICE:The out size w:%d h:%d ,The source review buffer 0x%x is not enough",param_ptr->output_size.w, param_ptr->output_size.h, param_ptr->output_buf_size));
    }

    if(PNULL==param_ptr->get_scale_data)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: The mjpg get capture data callback is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_5977_112_2_18_1_43_47_138,(uint8*)"");
        return SCI_ERROR;
    }   

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Isp Fifo over flow handle
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_PathFifoOverFlowHandle(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    
    switch(pISPContext->service)
    {  
        case ISP_SERVICE_PREVIEW:
        {
            SCI_Sleep(150);
            ISP_FreeISP();
            _ISP_StartPreview();
            break;
        }

        case ISP_SERVICE_CAPTURE:
        {
            ISP_FreeISP();
            pISPContext->output_addr.y_addr=PNULL;
            pISPContext->output_addr.uv_addr=PNULL;
            pISPContext->output_size.w=ISP_ZERO;
            pISPContext->output_size.h=ISP_ZERO;
            pISPContext->is_success=SCI_ERROR;            
            
            if(PNULL!=pISPContext->get_data)
            {
                pISPContext->get_data(pISPContext->output_addr, (pISPContext->output_size.w), (pISPContext->output_size.h));	
            }
            else
            {
                //_ISP_SetEvent(ISP_CAPTURE_EVENT);
            }
            break;
        }
        
        case ISP_SERVICE_REVIEW:
        {
            ISP_FreeISP();
            pISPContext->is_success=SCI_ERROR;            
            _ISP_SetEvent(ISP_REVIEW_EVENT);
            break;
        }
        
        case ISP_SERVICE_SCALE:            
        {
            ISP_FreeISP();
            pISPContext->output_addr.y_addr=PNULL;
            pISPContext->output_addr.uv_addr=PNULL;
            pISPContext->output_size.w=ISP_ZERO;
            pISPContext->output_size.h=ISP_ZERO;
            pISPContext->is_success=SCI_ERROR;               
            
            if(PNULL!=pISPContext->get_data)
            {
                pISPContext->get_data(pISPContext->output_addr, (pISPContext->output_size.w), (pISPContext->output_size.h));	
            }            
            _ISP_SetEvent(ISP_SCALE_EVENT);
            break;
        }        

        case ISP_SERVICE_MOTION_JPEG:      
        case ISP_SERVICE_UPCC:               
        {
            ISP_FreeISP();
            _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_SIG,0,0,0);
            break;
        }
        
        case ISP_SERVICE_ATV_PREVIEW: 
        case ISP_SERVICE_ATV_RECORDER:
        {
            SCI_Sleep(50);
            ISP_FreeISP();
            _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_SIG,0,0,0);
            break;
        }           

        default:
            //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_PathFifoOverFlowHandle default %d line error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_6071_112_2_18_1_43_47_139,(uint8*)"d", __LINE__);
            break;
    }

}    
#if 0
/****************************************************************************************/
// Description: Isp err handle
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/

LOCAL void _ISP_ErrorHandle(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    
    ISP_FreeISP();
    
    _ISP_PeekSignal();

    SCI_Sleep(50);
    
    switch(pISPContext->service)
    {  
        case ISP_SERVICE_PREVIEW:
        {
            pISPContext->is_first_frame = SCI_TRUE;
            _ISP_ServiceSendMsg(pISPContext->service, ISP_SERVICE_START_SIG,0,0,0);
            break;
        }
            
        case ISP_SERVICE_CAPTURE:
        {            
            break;
        }
        
        case ISP_SERVICE_REVIEW:
        {
            break;
        }
        
        case ISP_SERVICE_SCALE:            
        {
            break;
        }        

        case ISP_SERVICE_MOTION_JPEG:      
        case ISP_SERVICE_UPCC:               
        {
            break;
        }             

        default:
            //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ErrorHandle default %d line error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_6129_112_2_18_1_43_47_140,(uint8*)"d", __LINE__);
            break;
    }

} 
#endif
/****************************************************************************************/
// Description: isp Preview TX done handle
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                     None
// output parameter:
//                     None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_PreviewTXDoneHandle(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    if(0x00!=isp_trace_eb)
    {
        //SCI_TRACE_ISP:"ISP Service:preview fram rate time: %dms"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_6151_112_2_18_1_43_48_141,(uint8*)"d",SCI_GetTickCount()-isp_start_time);
        isp_start_time=SCI_GetTickCount();
    }
    
    #if (defined(PLATFORM_SC6600L))

    if(SCI_TRUE==pISPContext->is_first_frame)
    {
        pISPContext->is_first_frame= SCI_FALSE;

        IVsp_RegisterIntFuncEx(CAP_SOF_ID, _ISP_ISR_CapStartOfFrameLow, _ISP_ISR_CapStartOfFrameHigh);
        IVsp_ClearInt(CAP_SOF_ID);
        IVsp_OpenInt(CAP_SOF_ID); 

        IVsp_RegisterIntFuncEx(CAP_EOF_ID, _ISP_ISR_CapEndOfFrameLow, _ISP_ISR_CapEndOfFrameHigh);
        IVsp_ClearInt(CAP_EOF_ID);
        IVsp_OpenInt(CAP_EOF_ID);  

        _ISP_SetEvent(ISP_DISPLAY_EVENT);
    }
    else
    {
        _ISP_InvalidateRect_Image(pISPContext->lcd_id,
                                    pISPContext->disp_rect.x,
                                    pISPContext->disp_rect.y,
                                    pISPContext->disp_rect.w,
                                    pISPContext->disp_rect.h,
                                    pISPContext->target_rect.x,
                                    pISPContext->target_rect.y,
                                    pISPContext->target_rect.w,
                                    pISPContext->target_rect.h,
                                    pISPContext->display_buf_addr,
                                    pISPContext->rotation,
                                    pISPContext->disp_format);
    }

    if(SCI_TRUE==pISPContext->is_full_view)
    {
        SCI_MEMSET((pISPContext->preview_isp_out_buf_addr.y_addr+((pISPContext->disp_rect.w*pISPContext->full_screen_rect.h)<<ISP_SHIFT_BIT1)), ISP_ZERO, s_offset_addr);
        pISPContext->preview_isp_out_buf_addr.y_addr-=s_offset_addr; 
        SCI_MEMSET(pISPContext->preview_isp_out_buf_addr.y_addr, ISP_ZERO, s_offset_addr);
    }  

    _ISP_Osd_Pileup((uint16*)pISPContext->preview_isp_out_buf_addr.y_addr);	

    pISPContext->display_buf_addr.y_addr=pISPContext->preview_isp_out_buf_addr.y_addr;
    
    pISPContext->display_state=ISP_SERVICE_DISPLAY_END; 
    
    #elif (defined(PLATFORM_SC6800H))

    if(SCI_TRUE==pISPContext->is_first_frame)
    {
        pISPContext->is_first_frame=SCI_FALSE;
        _ISP_SetEvent(ISP_DISPLAY_EVENT);
    }

    pISPContext->display_buf_addr.y_addr=pISPContext->preview_isp_out_buf_addr.y_addr;
    pISPContext->display_buf_addr.uv_addr=pISPContext->preview_isp_out_buf_addr.uv_addr;
    pISPContext->display_buf_addr.v_addr=pISPContext->preview_isp_out_buf_addr.v_addr;
    
   #ifdef WRE_SUPPORT	
   	{
		extern int32 wre_in_camera_scan_mode(void);
		extern  void wre_camera_snap_callback(uint32 ptr);
		if(1 == wre_in_camera_scan_mode()){
			 if (pISPContext->preview_isp_out_buf_addr.y_addr!=0){
			       wre_camera_snap_callback(pISPContext->preview_isp_out_buf_addr.y_addr);
    			}
		}
	}					
    #endif		
	
   
   
    
    _ISP_ServiceSendMsg(ISP_ZERO, ISP_SERVICE_DISP_SIG,0,0,0); 
    #endif
    
    return SCI_SUCCESS;

}

/****************************************************************************************/
// Description: isp capture TX done handle
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                     None
// output parameter:
//                     None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_CaptureTXDoneHandle(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
            
    if(ISP_DATA_JPEG == pISPContext->input_image_format)
    {
        pISPContext->cap_jpg_size=ISP_GetJpegLength();
    }
        
    ISP_FreeISP();
    
    if(PNULL!=pISPContext->get_data)
    {//async
        if(ISP_DATA_JPEG == pISPContext->input_image_format)
        {
            if(PNULL!=pISPContext->get_data)
            {
                pISPContext->get_data(pISPContext->output_addr,pISPContext->cap_jpg_size,0);
                pISPContext->get_data=PNULL;
            }	
        }
        else
        {
            if(PNULL!=pISPContext->get_data)
            {
                pISPContext->get_data(pISPContext->output_addr,(pISPContext->output_size.w),(pISPContext->output_size.h));
                pISPContext->get_data=PNULL;
            }
        }
        pISPContext->state = ISP_SERVICE_IDLE_STATE;
    }
    else
    { // syns
        pISPContext->state = ISP_SERVICE_IDLE_STATE;
        _ISP_SetEvent(ISP_CAPTURE_EVENT);
    }

    return SCI_SUCCESS;    
}

/****************************************************************************************/
// Description: isp Review TX done handle
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                     None
// output parameter:
//                     None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_ReviewTXDoneHandle(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    ISP_DATA_ADDR_T src_addr={0x00};
    ISP_DATA_ADDR_T dst_addr={0x00};

    if(!((ISP_DATA_RGB565==pISPContext->input_image_format)
        ||((ISP_DATA_YUV422==pISPContext->disp_format)
            &&(pISPContext->cap_output_size.w==pISPContext->scale_trim_rect.w)
            &&(pISPContext->cap_output_size.w==pISPContext->target_rect.w))))
    {                          
        ISP_FreeISP();
    }
    
    _ISP_Osd_Pileup((uint16*)pISPContext->review_display_buf_addr.y_addr);

    if(ISP_DATA_RGB565==pISPContext->disp_format)
    {
        src_addr.y_addr=pISPContext->review_display_buf_addr.y_addr+(pISPContext->target_rect.w*ISP_TWO)*(pISPContext->target_rect.h-ISP_TWO);
        dst_addr.y_addr=pISPContext->review_display_buf_addr.y_addr+(pISPContext->target_rect.w*ISP_TWO)*(pISPContext->target_rect.h-ISP_ONE);
        SCI_MEMCPY((void*)dst_addr.y_addr,(void*)src_addr.y_addr,(pISPContext->target_rect.w<<ISP_SHIFT_BIT1));
    }

    if(PNULL != s_isp_mpeg_display_callback)
    {
    
       _ISP_DispImgCallback(pISPContext->target_rect.x, 
                                    pISPContext->target_rect.y, 
                                    pISPContext->target_rect.w,
                                    pISPContext->target_rect.h,
                                    pISPContext->review_display_buf_addr,
                                    pISPContext->rotation,
                                    pISPContext->disp_format);
    }
    else
    {
        _ISP_InvalidateRect_Image(pISPContext->lcd_id,
                                pISPContext->disp_rect.x,
                                pISPContext->disp_rect.y,
                                pISPContext->disp_rect.w,
                                pISPContext->disp_rect.h,
                                pISPContext->target_rect.x,
                                pISPContext->target_rect.y,
                                pISPContext->target_rect.w,
                                pISPContext->target_rect.h,
                                pISPContext->review_display_buf_addr,
                                pISPContext->rotation,
                                pISPContext->disp_format);

        _ISP_SetEvent(ISP_REVIEW_EVENT);
    }
    
    
    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: isp Scale TX done handle
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                     None
// output parameter:
//                     None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_ScaleTXDoneHandle(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint32 out_height=0x00;

    if(ISP_SERVICE_SCALE_SLICE==pISPContext->scale_mode)
    {
        out_height=ISP_GetScaleSliceDoneHeight();
        pISPContext->slice_complete_line=out_height;
    }
    else
    {
        out_height=pISPContext->output_size.h;
    }

    ISP_FreeISP();

    if(PNULL!=pISPContext->get_data)
    {
        pISPContext->get_data(pISPContext->output_addr,(pISPContext->output_size.w),out_height);
    }

    _ISP_SetEvent(ISP_SCALE_EVENT);
    
    return SCI_SUCCESS;
    
}

/****************************************************************************************/
// Description: isp video recorder TX done handle
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                     None
// output parameter:
//                     None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_VideoRecorderTXDoneHandle(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    if(SCI_TRUE!=pISPContext->is_stop)
    {
        if(ISP_SERVICE_CAPTURE_STATE==pISPContext->state)
        { //convert the yuv to rgb565
            _ISP_TimerDActive();
            if(SCI_TRUE==pISPContext->is_first_frame)
            {
                pISPContext->is_first_frame= SCI_FALSE;	
                _ISP_SetEvent(ISP_MOTION_JPEG_START_EVENT);
            }
            ISP_FreeISP();
            _ISP_GetFrameData(ISP_DATA_YUV422);
            if(SCI_TRUE==pISPContext->is_stop)
            {
                _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_STOP_SIG,0,0,0);
                return SCI_SUCCESS;
            }
            _ISP_SetMjpegReviewParam();
            _ISP_StartReview();
            pISPContext->state=ISP_SERVICE_REVIEW_STATE; 
        }
        else if(ISP_SERVICE_REVIEW_STATE==pISPContext->state)
        { // display & yuv scale
            ISP_FreeISP();
            _ISP_GetFrameData(ISP_DATA_RGB565);
            if(SCI_TRUE==pISPContext->is_stop)
            {
                _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_STOP_SIG,0,0,0);
                return SCI_SUCCESS;
            }
            _ISP_SetMjpegScaleParam(); 
            _ISP_StartScale();
            pISPContext->state=ISP_SERVICE_SCALE_STATE;
            // pileup osd
            if(SCI_TRUE==pISPContext->is_full_view)
            {
                SCI_MEMSET((void *)pISPContext->review_display_buf_addr.y_addr, ISP_ZERO, ((pISPContext->disp_rect.w*pISPContext->full_screen_rect.y)<<ISP_SHIFT_BIT1));
                SCI_MEMSET((void *)(pISPContext->review_display_buf_addr.y_addr+((pISPContext->disp_rect.w*(pISPContext->full_screen_rect.y+pISPContext->full_screen_rect.h))<<ISP_SHIFT_BIT1)), ISP_ZERO, 
                            ((pISPContext->disp_rect.w*(pISPContext->disp_rect.h-pISPContext->full_screen_rect.y-pISPContext->full_screen_rect.h))<<ISP_SHIFT_BIT1));
            }

            _ISP_Osd_Pileup((uint16*)pISPContext->review_display_buf_addr.y_addr);
        }
        else if(ISP_SERVICE_SCALE_STATE==pISPContext->state)
        {  // capture
            if((SCI_TRUE!=pISPContext->is_pause)&&(PNULL!=pISPContext->get_data))
            {//jpg encoder callback
                ISP_FreeISP();
                if(PNULL!=pISPContext->get_data)
                {
                    pISPContext->get_data(pISPContext->mjpg_info.jpeg_yuv_addr,(pISPContext->mjpg_info.jpeg_target_size.w),(pISPContext->mjpg_info.jpeg_target_size.h));	
                }
            }                               

            if(SCI_TRUE==pISPContext->is_zoom)
            {
                _ISP_StartDvZoom();
            }

            //start capture
            _ISP_SetMjpegCpatureParam();
            _ISP_StartCaptureNormal();
            _ISP_TimerActive(ISP_CAPTURE_TIMEOUT);
            pISPContext->state = ISP_SERVICE_CAPTURE_STATE;
            // display    
            if((NULL != pISPContext->review_display_buf_addr.y_addr)&&(SCI_FALSE!=pISPContext->is_disp)) 
            {
                _ISP_InvalidateRect_Image(pISPContext->lcd_id,
                                            pISPContext->disp_rect.x,
                                            pISPContext->disp_rect.y,
                                            pISPContext->disp_rect.w,
                                            pISPContext->disp_rect.h,
                                            pISPContext->target_rect.x,
                                            pISPContext->target_rect.y,
                                            pISPContext->target_rect.w,
                                            pISPContext->target_rect.h,
                                            pISPContext->review_display_buf_addr,
                                            pISPContext->rotation,
                                            pISPContext->disp_format);
            }

        }   

    }
    else
    {
        _ISP_TimerDActive();
        _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_STOP_SIG,0,0,0);
    }

    return SCI_SUCCESS;
    
}

/****************************************************************************************/
// Description: isp ATV recorder TX done handle
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                     None
// output parameter:
//                     None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_ATVRecorderTXDoneHandle(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    if(SCI_TRUE!=pISPContext->is_stop)
    {
        if(ISP_SERVICE_CAPTURE_STATE==pISPContext->state) //convert the yuv to rgb565
        { // mjpg yuv to rgb  
            _ISP_ATVTimerDActive();
            if(SCI_TRUE==pISPContext->is_first_frame)
            {
                pISPContext->is_first_frame= SCI_FALSE;	
                _ISP_SetEvent(ISP_MOTION_JPEG_START_EVENT);
            }

            ISP_FreeISP();
            _ISP_SetMjpegReviewParam();
            _ISP_StartReview();
            pISPContext->state=ISP_SERVICE_REVIEW_STATE; 
        }
        else if(ISP_SERVICE_REVIEW_STATE==pISPContext->state) // display & yuv scale
        {// mjpg scale yuv for encoder                             
            ISP_FreeISP();
            _ISP_SetMjpegScaleParam(); 
            _ISP_StartScale();
            pISPContext->state=ISP_SERVICE_SCALE_STATE;
            // pileup osd
            if(SCI_FALSE!=pISPContext->is_disp) 
            {
                _ISP_Osd_Pileup((uint16*)pISPContext->review_display_buf_addr.y_addr);		                                      
            }                                
        }
        else if(ISP_SERVICE_SCALE_STATE==pISPContext->state)  // capture
        {
            if((SCI_TRUE!=pISPContext->is_pause)&&(PNULL!=pISPContext->get_data))
            {//jpg encoder callback
                ISP_FreeISP();
                if(PNULL!=pISPContext->get_data)
                {
                    pISPContext->get_data(pISPContext->mjpg_info.jpeg_yuv_addr,(pISPContext->mjpg_info.jpeg_target_size.w),(pISPContext->mjpg_info.jpeg_target_size.h));	
                }
            } 

            _ISP_InvalidateRect_Image(pISPContext->lcd_id,
                                        pISPContext->disp_rect.x,
                                        pISPContext->disp_rect.y,
                                        pISPContext->disp_rect.w,
                                        pISPContext->disp_rect.h,
                                        pISPContext->target_rect.x,
                                        pISPContext->target_rect.y,
                                        pISPContext->target_rect.w,
                                        pISPContext->target_rect.h,
                                        pISPContext->review_display_buf_addr,
                                        pISPContext->rotation,
                                        pISPContext->disp_format);
            //start capture
            _ISP_SetMjpegCpatureParam();
            _ISP_StartCaptureNormal();
            _ISP_ATVTimerActive();
            pISPContext->state = ISP_SERVICE_CAPTURE_STATE;
        }   

    }
    else
    {
        _ISP_ATVTimerDActive();                           
        _ISP_ResumDMA();
        _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_STOP_SIG,0,0,0);
    }
    
    return SCI_SUCCESS;
    
}

/****************************************************************************************/
// Description: isp ATV Preview TX done handle
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                     None
// output parameter:
//                     None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_ATVPreviewTXDoneHandle(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    if(SCI_TRUE!=pISPContext->is_stop)
    {

	    _ISP_ATVTimerDActive();
        if(ISP_SERVICE_CAPTURE_STATE==pISPContext->state) //convert the yuv to rgb565
        {
            ISP_FreeISP();
            if(SCI_TRUE==pISPContext->is_first_frame)
            {
                pISPContext->is_first_frame=SCI_FALSE;	
                _ISP_SetEvent(ISP_DISPLAY_EVENT);
            }
            _ISP_SetATVReviewParam();
            _ISP_StartReview();
            pISPContext->state=ISP_SERVICE_REVIEW_STATE;
        }
        else if(ISP_SERVICE_REVIEW_STATE==pISPContext->state)
        {
            _ISP_InvalidateRect_Image(pISPContext->lcd_id,
                                        pISPContext->disp_rect.x,
                                        pISPContext->disp_rect.y,
                                        pISPContext->disp_rect.w,
                                        pISPContext->disp_rect.h,
                                        pISPContext->target_rect.x,
                                        pISPContext->target_rect.y,
                                        pISPContext->target_rect.w,
                                        pISPContext->target_rect.h,
                                        pISPContext->review_display_buf_addr,
                                        pISPContext->rotation,
                                        pISPContext->disp_format);
            ISP_FreeISP();
            _ISP_SetSensorParam();
            _ISP_SetSensorParamCallback();
            _ISP_SetATVPreviewParam(); 
            _ISP_StartCaptureNormal(); 
            pISPContext->state=ISP_SERVICE_CAPTURE_STATE;
            _ISP_ATVTimerActive(); 
        }
        else
        {
            _ISP_ATVTimerActive(); 
        }

    }
    else
    {
        _ISP_ATVTimerDActive();
        _ISP_ServiceSendMsg(ISP_SERVICE_ATV_PREVIEW, ISP_SERVICE_STOP_SIG,0,0,0);
    }
    
    return SCI_SUCCESS;
    
}

/****************************************************************************************/
// Description: isp upcc TX done handle
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                     None
// output parameter:
//                     None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_UPCCTXDoneHandle(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    if(SCI_TRUE!=pISPContext->is_stop)
    {
        _ISP_TimerDActive();
        ISP_FreeISP();
        if(SCI_TRUE==pISPContext->is_first_frame)
        {
            pISPContext->is_first_frame= SCI_FALSE;	
            _ISP_SetEvent(ISP_UPCC_EVENT);
        }

        if((SCI_TRUE!=pISPContext->is_pause)&&(PNULL!=pISPContext->get_data))
        {//upcc encoder callback
            if(PNULL!=pISPContext->get_data)
            {
                pISPContext->get_data(pISPContext->output_addr,(pISPContext->output_size.w),(pISPContext->output_size.h));	
            }
        }
        
        //start capture
        _ISP_StartCaptureNormal();
        _ISP_TimerActive(ISP_CAPTURE_TIMEOUT);
    }
    else
    {
        _ISP_TimerDActive();
        _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_STOP_SIG,0,0,0);
    }
    
    return SCI_SUCCESS;
    
}

/****************************************************************************************/
// Description: isp sensor eof handle
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                     None
// output parameter:
//                     None
// return:
//          SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_SensorEofHandle(void)
{   
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    if(ISP_ZERO==pISPContext->isp_sensor_frame_counter.num)
    {
        IVsp_UnRegisterIntFunc(SENSOR_EOF_ID);
    }
    else
    {
        if(pISPContext->isp_sensor_frame_counter.num==pISPContext->isp_sensor_frame_counter.cur_num)
        {
            IVsp_UnRegisterIntFunc(SENSOR_EOF_ID);

            pISPContext->isp_sensor_frame_counter.num=ISP_ZERO;

            if(NULL!=pISPContext->isp_sensor_frame_counter.callback)
            {
                pISPContext->isp_sensor_frame_counter.callback(SCI_TRUE);
            }
            else
            {
                _ISP_SetEvent(ISP_SENSOR_EOF_EVENT);
            }
        }
        pISPContext->isp_sensor_frame_counter.cur_num++;
    }

    return SCI_SUCCESS;    
}

/****************************************************************************************/
// Description: get current prioty of Isp service task 
// Global resource dependence: 
// Author: Xi.zhang
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32 prioty     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_Service_GetIspPriority(void)
{
	char thread_name[SCI_MAX_NAME_SIZE];
	char queue_name[SCI_MAX_NAME_SIZE];
	uint32 state;
	uint32 preempt;
	uint32 priority = ISP_SERVICE_PRIO;

	BLOCK_ID thread_id = s_isp_service_thread_id;

	if(SCI_SUCCESS !=SCI_GetThreadInfo(thread_id, thread_name, queue_name, &state,  &priority, &preempt))
	{
		SCI_PASSERT(0,("get thread info error!"));    /*assert verified*/
	}
	
	//SCI_TRACE_LOW:"ISP_SERVICE:_ISP_Service_GetIspPriority: %s, queue_name: %s, priority: %d "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_6760_112_2_18_1_43_49_142,(uint8*)"ssd" , thread_name, queue_name, priority);

	return  priority;

}

/****************************************************************************************/
// Description: set the prioty of Isp service task 
// Global resource dependence: 
// Author: Xi.zhang
// input parameter:
//                      task priority
// output parameter:
//                      None
// return:
//          uint32 prioty     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_Service_SetIspPriority(uint32 task_priority)
{
    uint32 priority = ISP_SERVICE_PRIO;
 
	if(task_priority != _ISP_Service_GetIspPriority())
	{

		 //SCI_TRACE_LOW:"ISP_SERVICE:_ISP_Service_Set_Priority, priority: %d"
		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_6783_112_2_18_1_43_49_143,(uint8*)"d" , task_priority);
		 if(SCI_ERROR == SCI_ChangeThreadPriority(s_isp_service_thread_id, task_priority, &priority))
		 {
		 	SCI_PASSERT(0,("change thread priority is error!"));    /*assert verified*/
		 }
	}

	return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: get the prioty of caller of isp service task 
// Global resource dependence: 
// Author: Xi.zhang
// input parameter:
//                      task priority
// output parameter:
//                      None
// return:
//          uint32 prioty     
// Note:
/*****************************************************************************************/
LOCAL uint32 _ISP_Service_GetCallerPriority(void)
{
	char thread_name[SCI_MAX_NAME_SIZE];
	char queue_name[SCI_MAX_NAME_SIZE];
	uint32 state;
	uint32 preempt;
	uint32 priority = ISP_SERVICE_PRIO;

	BLOCK_ID thread_id;

	thread_id = SCI_IdentifyThread();

	if(SCI_SUCCESS !=SCI_GetThreadInfo(thread_id, thread_name, queue_name, &state,  &priority, &preempt))
	{
		SCI_PASSERT(0,("get thread info error!"));    /*assert verified*/
	}
	
	//SCI_TRACE_LOW:"ISP_SERVICE:_ISP_Service_GetCallerPriority: %s, queue_name: %s, priority: %d "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_6820_112_2_18_1_43_49_144,(uint8*)"ssd" , thread_name, queue_name, priority);


	return  priority;
	
}

/****************************************************************************************/
// Description: Isp service task handle that handle the preview/capture/review message
// Global resource dependence: 
// Author: Jianping.wang
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
LOCAL void _ISP_ServiceHandleRoutine(uint32 argc, void *argv)//(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint32 service=0x00;
    uint32 command=0x00;
    uint32 param0=0x00;
    uint32 param1=0x00;
    void * ptr=PNULL;
    ISP_SIGNAL_T *sig_ptr=PNULL;
	
    while(TRUE)/*lint !e716*/
    {
        //Get signal from UPM's queue ;

        sig_ptr = (ISP_SIGNAL_T *)SCI_GetSignal(s_isp_service_thread_id);

        if(NULL==sig_ptr)
        {
            continue ;
        }

        if(ISP_SERVICE_ID_MAX<sig_ptr->sig.SignalCode)
        {
            //SCI_TRACE_ISP:"ISP_SERVICE: _ISP_ServiceHandleRoutine the SignalCode 0x%x, error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_6860_112_2_18_1_43_49_145,(uint8*)"d", sig_ptr->sig.SignalCode);
            SCI_FREE(sig_ptr);
            sig_ptr=PNULL;
            continue ;
        }

        //Get variable from pointer ;
        service  = sig_ptr->sig.SignalCode;
        service  = sig_ptr->service;        
        command  = sig_ptr->command;
        param0   = sig_ptr->param0;
        param1   = sig_ptr->param1;
        ptr      = sig_ptr->ptr;
        
        if(0x00!=isp_trace_eb)
	    {
            //SCI_TRACE_ISP:"ISP_SERVICE: Recieve Request, service=0x%x command=0x%x, cur time=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_6876_112_2_18_1_43_49_146,(uint8*)"ddd",service, command,SCI_GetTickCount());
        }

        if(ISP_SERVICE_CLOSE_STATE != pISPContext->state)
        {
            switch(command)
            {
                case ISP_SERVICE_OPEN_SIG:
                {
                    _ISP_SetEvent(ISP_OPEN_EVENT);
                    break;
                }
                case ISP_SERVICE_CLOSE_SIG:
                {
                    pISPContext->state=ISP_SERVICE_CLOSE_STATE; 
                    _ISP_TimerDActive();
                     _ISP_ServiceClose();
                     _ISP_PeekSignal();  //hansen@0728 temp use 
                    pISPContext->service=ISP_SERVICE_IDLE;    
                    
                    _ISP_SetEvent(ISP_CLOSE_EVENT);
                    break;
                }

                case ISP_SERVICE_START_SIG:
                {
                    switch(service)
                    {
                        case ISP_SERVICE_PREVIEW:
                        {
                            _ISP_StartPreview();
                            pISPContext->state = ISP_SERVICE_PREVIEW_STATE;
                            pISPContext->display_state = ISP_SERVICE_DISPLAY_END;
                            break;
                        }
                        case ISP_SERVICE_CAPTURE:
                        {
                            _ISP_StartCapture();                     
                            pISPContext->state = ISP_SERVICE_CAPTURE_STATE;
                            break;
                        }
                        case ISP_SERVICE_REVIEW:
                        {                           
                            _ISP_StartReview();

                            if(PNULL != s_isp_mpeg_display_callback)
                            {
                                _ISP_SetEvent(ISP_REVIEW_EVENT);
                            }
                            
                            pISPContext->state = ISP_SERVICE_REVIEW_STATE;
                            break;
                        }
                        case ISP_SERVICE_SLICE_SCALE:
                        {                           
                            _ISP_StartSliceScale();
                            pISPContext->state = ISP_SERVICE_SCALE_STATE;
                            break;
                        }                        
                        case ISP_SERVICE_SCALE:
                        {                           
                            _ISP_StartScale();
                            pISPContext->state = ISP_SERVICE_SCALE_STATE;
                            break;
                        }                        
                        case ISP_SERVICE_ATV_RECORDER:                        
                        {
                            if(SCI_TRUE==pISPContext->is_stop)
                            {
                                _ISP_ResumDMA();    
                                _ISP_ATVTimerDActive();
                                ISP_SoftReset();
                                ISP_FreeISP();
                                _ISP_SetEvent(ISP_MOTION_JPEG_STOP_EVENT);
                                break;
                            }
                            
                            if(SCI_TRUE==pISPContext->is_timeout)
                            {
                                pISPContext->is_timeout=SCI_FALSE;
                                ISP_SoftReset();
                                ISP_FreeISP();
                            }
            
                            _ISP_SetMjpegCpatureParam();
                            _ISP_StartCaptureNormal(); 
                            _ISP_ATVTimerActive();
                            pISPContext->state = ISP_SERVICE_CAPTURE_STATE;
                            break;
                        }
                        case ISP_SERVICE_MOTION_JPEG:                        
                        {
                            if(SCI_TRUE==pISPContext->is_stop)
                            {
                                _ISP_TimerDActive();
                                ISP_SoftReset();
                                ISP_FreeISP();
                                _ISP_SetEvent(ISP_MOTION_JPEG_STOP_EVENT);
                                break;
                            }
                            
                            if(SCI_TRUE==pISPContext->is_timeout)
                            {
                                pISPContext->is_timeout=SCI_FALSE;
                                ISP_SoftReset();
                                ISP_FreeISP();
                            }                            
                            _ISP_SetMjpegCpatureParam();
                            _ISP_StartCaptureNormal(); 
                            _ISP_TimerActive(ISP_CAPTURE_TIMEOUT);
                            pISPContext->state = ISP_SERVICE_CAPTURE_STATE;
                            break;
                        }
                        case ISP_SERVICE_UPCC:                            
                        {
                            if(SCI_TRUE==pISPContext->is_stop)
                            {
                                _ISP_TimerDActive();
                                ISP_SoftReset();
                                ISP_FreeISP();
                                _ISP_SetEvent(ISP_UPCC_EVENT);
                                break;
                            }
                            
                            if(SCI_TRUE==pISPContext->is_timeout)
                            {
                                pISPContext->is_timeout=SCI_FALSE;
                                ISP_SoftReset();
                                ISP_FreeISP();
                            }
                            
                            _ISP_StartCaptureNormal(); 
                            _ISP_TimerActive(ISP_CAPTURE_TIMEOUT);
                            pISPContext->state = ISP_SERVICE_CAPTURE_STATE;
                            break;
                        }                        
                        case ISP_SERVICE_ATV_PREVIEW:
                        {
                            if(SCI_TRUE==pISPContext->is_stop)
                            {
                                _ISP_ResumDMA();
                                _ISP_ATVTimerDActive();
                                ISP_SoftReset();
                                ISP_FreeISP();
                                _ISP_SetEvent(ISP_STOP_EVENT);
                                break;
                            }

                            if(SCI_TRUE==pISPContext->is_timeout)
                            {
                                if(ISP_ATV_TIME_OUT_HANDLE_NUM<pISPContext->time_out_counter)
                                {
                                    ATVErrHandler();
                                }
                                ISP_SoftReset(); 
                                ISP_FreeISP();
                                pISPContext->is_timeout=SCI_FALSE;
                            }

                            _ISP_SetATVPreviewParam(); 
                            _ISP_StartCaptureNormal();
                            pISPContext->state = ISP_SERVICE_CAPTURE_STATE;
                            _ISP_ATVTimerActive();
                            break;
                        }
                        case ISP_SERVICE_SET_SENSOR_PARAM:
                        {
                            ISP_SENSOR_PARAM_T_PTR sensor_param_ptr=(ISP_SENSOR_PARAM_T_PTR)param0;
                            _ISP_SetSensorParamCtrl(sensor_param_ptr);
                            break;
                        }
                        case ISP_SERVICE_SET_SENSOR_CALLBACK:
                        {
                            _ISP_SetSensorParamCallback();                           
                            break;
                        }                              
                        case ISP_SERVICE_DC_ZOOM:
                        {
                            _ISP_StartDcZoom();
                            break;
                        }  
                        case ISP_SERVICE_DC_ZOOM_CALLBACK:
                        {                       
                            if(PNULL!=pISPContext->zoom_ctl.zoom_success)
                            {
                                pISPContext->zoom_ctl.zoom_success(SCI_TRUE);
                            }
                            break; 
                        }
                        case ISP_SERVICE_SENSOR_EOF:
                        {
                            ISP_SENSOR_FRAME_COUNTER_T_PTR sensor_frame_ctl_ptr=(ISP_SENSOR_FRAME_COUNTER_T_PTR)param0;
                            _ISP_SetSensorEofCtrl(sensor_frame_ctl_ptr);
                            break; 
                        }
                        default:
                            //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ServiceHandleRoutine default %d line error!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7072_112_2_18_1_43_49_147,(uint8*)"d", __LINE__);
                            break;						
                    }
                    break;
                }
                case ISP_SERVICE_STOP_SIG:
                {
                    ISP_SoftReset();
                    _ISP_PeekSignal();
                                        
                    switch(service)
                    {
                        case ISP_SERVICE_PREVIEW:
                        {
                            _ISP_StopPreview();                           
                            pISPContext->state = ISP_SERVICE_IDLE_STATE; 
                            _ISP_SetEvent(ISP_STOP_EVENT);
                            break;
                        }                    
                        case ISP_SERVICE_CAPTURE:
                        {                            
                            _ISP_StopCapture();
                            pISPContext->state=ISP_SERVICE_IDLE_STATE;
                            _ISP_SetEvent(ISP_STOP_EVENT);
                            break;
                        }
                        case ISP_SERVICE_REVIEW:
                        {
                            ISP_FreeISP();
                            pISPContext->state=ISP_SERVICE_IDLE_STATE;                          
                            _ISP_SetEvent(ISP_STOP_EVENT);
                            break;
                        }  
                        case ISP_SERVICE_ATV_PREVIEW:
                        {
                            _ISP_ATVTimerDActive();
                            _ISP_PeekSignal();
                            ISP_FreeISP();
                            pISPContext->state=ISP_SERVICE_IDLE_STATE;
                            _ISP_SetEvent(ISP_STOP_EVENT);
                            break;
                        }                          

                        case ISP_SERVICE_SCALE:
                        {
                            _ISP_StopScale();
                            pISPContext->state=ISP_SERVICE_IDLE_STATE;
                            _ISP_SetEvent(ISP_STOP_EVENT);
                            break;
                        }     

                        case ISP_SERVICE_ATV_RECORDER:
                        {
                            _ISP_ATVTimerDActive();   
                            _ISP_PeekSignal();
                            _ISP_ResumDMA();
                            _ISP_StopMjpg();
                            pISPContext->state=ISP_SERVICE_IDLE_STATE;  
                            _ISP_SetEvent(ISP_STOP_EVENT);
                            break;	
                        }
                        case ISP_SERVICE_MOTION_JPEG:
                        {
                            _ISP_TimerDActive();
                            _ISP_PeekSignal();
                            _ISP_StopMjpg();
                            pISPContext->state=ISP_SERVICE_IDLE_STATE;	
                            _ISP_SetEvent(ISP_STOP_EVENT);
                            break;	
                        }
                        case ISP_SERVICE_UPCC:
                        {
                            _ISP_TimerDActive();
                            _ISP_PeekSignal();
                            _ISP_StopMjpg();
                            pISPContext->state=ISP_SERVICE_IDLE_STATE;	
                            _ISP_SetEvent(ISP_STOP_EVENT);
                            break;	
                        }                        
                        default:
                            //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ServiceHandleRoutine default %d line error!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7152_112_2_18_1_43_50_148,(uint8*)"d", __LINE__);
                            break;						
                    }
                    
                    break;
                }

                case ISP_SERVICE_TRANSFER_DONE_SIG:
                {
                    switch(service)
                    {                         
                        case ISP_SERVICE_PREVIEW:
                        {
                            _ISP_PreviewTXDoneHandle();
                            break;
                        }
                        case ISP_SERVICE_CAPTURE:
                        {
                            _ISP_CaptureTXDoneHandle();
                            break;
                        }
                        case ISP_SERVICE_REVIEW:
                        {
                            _ISP_ReviewTXDoneHandle();
                            break;
                        }

                        case ISP_SERVICE_SCALE:
                        case ISP_SERVICE_SLICE_SCALE:
                        {
                            _ISP_ScaleTXDoneHandle();
                            break;
                        }                            

                        case ISP_SERVICE_MOTION_JPEG:
                        {
                            _ISP_VideoRecorderTXDoneHandle();
                            break;
                        }

                        case ISP_SERVICE_ATV_RECORDER:
                        {
                            _ISP_ATVRecorderTXDoneHandle();
                            break;
                        }
                        
                        case ISP_SERVICE_ATV_PREVIEW:
                        {
                            _ISP_ATVPreviewTXDoneHandle();
                            break;
                        } 
                        
                        case ISP_SERVICE_UPCC:
                        {  
                            _ISP_UPCCTXDoneHandle();
                            break;
                        }
                        default:
                            //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ServiceHandleRoutine default %d line error!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7210_112_2_18_1_43_50_149,(uint8*)"d", __LINE__);
                            break;
                    }
                    break;
                }

                case ISP_SERVICE_DISP_SIG:                
                {
                    _ISP_InvalidateRect_Image(pISPContext->lcd_id,
                                                pISPContext->disp_rect.x,
                                                pISPContext->disp_rect.y,
                                                pISPContext->disp_rect.w,
                                                pISPContext->disp_rect.h,
                                                pISPContext->target_rect.x,
                                                pISPContext->target_rect.y,
                                                pISPContext->target_rect.w,
                                                pISPContext->target_rect.h,
                                                pISPContext->display_buf_addr,
                                                pISPContext->rotation,
                                                pISPContext->disp_format);
                    pISPContext->display_state=ISP_SERVICE_DISPLAY_END; 
                    break;
                }
                
                case ISP_SERVICE_OVERFLOW_SIG:                
                {
                    _ISP_PathFifoOverFlowHandle();
                    break;
                }

                case ISP_SERVICE_VIDEO_RECORDING_TIMEOUT_SIG:
                {

                    break;
                }
				
                case ISP_SERVICE_START_TIMEOUT_SIG:
                {
                    _ISP_TimerDActive();
                    _ISP_PeekSignal();
                    if(ISP_SERVICE_IDLE_STATE!=pISPContext->state)
                    {
                        ISP_SoftReset();
                        ISP_FreeISP();
                    }
                    
                    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_SERVICE_START_TIMEOUT_SIG, service=0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7256_112_2_18_1_43_50_150,(uint8*)"d",pISPContext->service);
                    _ISP_SetEvent(ISP_START_TIMEOUT_EVENT);                    
                    break;
                }

                case ISP_SERVICE_STOP_TIMEOUT_SIG:
                {
                    ISP_SoftReset();
                    ISP_FreeISP();
                    
                    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_SERVICE_STOP_TIMEOUT_SIG, service=0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7266_112_2_18_1_43_50_151,(uint8*)"d",pISPContext->service);
                    break;
                }  
                case ISP_SERVICE_SENSOR_EOF_SIG:
                {
                    _ISP_SensorEofHandle();
                    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_SERVICE_SENSOR_EOF_SIG, service=0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7272_112_2_18_1_43_50_152,(uint8*)"d",pISPContext->service);
                    break;
                }                      

                default:
                    //SCI_TRACE_ISP:"ISP_SERVICE:_ISP_ServiceHandleRoutine default %d line error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7277_112_2_18_1_43_50_153,(uint8*)"d", __LINE__);
                    break;
            }

        }
        else
        {
            ISP_FreeISP();
        }

        SCI_FREE(sig_ptr);
        sig_ptr=PNULL;
    }

}




/******************************************************************************
// Description:	isp num align
// Author:		Tim.zhu
// Input:		pLen: source length
//              align_coeff: align coeff
//              align_len: algin bits
// Return:		align length
// Note:			
******************************************************************************/ 
PUBLIC uint32 ISP_Align(uint32 pLen, uint32 align_coeff, uint32 align_len)
{
    uint32 align_data=pLen+align_coeff;
    
    align_data=((align_data>>align_len)<<align_len);

    return align_data;
}


/*****************************************************************************/
//  Description: Lock ISP Service
//	Global resource dependence:
// Author: Tim.zhu
// input parameter:
//                      owner:		the locker
//						wait_option:  SCI_WAIT_FOREVER or max milliseconds to wait
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: lock successful
//			other:		  lock timeout
// Note:
/*****************************************************************************/
PUBLIC uint32 ISP_ServiceLock(ISP_SERVICE_OWNER_E owner,  uint32 wait_option)
{
	uint32 ret = SCI_SUCCESS;
	
#if !defined(PLATFORM_SC6600L)

	uint32 sem_count = 0;	//semphore count
	uint32 susp_count = 0;	//semphore suspend thread count on the semphore

	_ISP_CreateSemaphore();	
	
	SCI_GetSemaphoreInfo(s_isp_semap_ptr, NULL, &sem_count, &susp_count);

	//SCI_TRACE_LOW:"[ISPLOCK: ISP_ServiceLock] owner = %d, cur owner = %d, sem_count = %d, wait option = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7340_112_2_18_1_43_50_154,(uint8*)"dddd", owner, s_isp_owner, sem_count, wait_option);
	
	ret = SCI_GetSemaphore(s_isp_semap_ptr, wait_option); 
	if (SCI_SUCCESS == ret)
	{
		s_isp_owner = owner;

		//SCI_TRACE_LOW:"[ISPLOCK: ISP_ServiceLock] lock done, owner = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7347_112_2_18_1_43_50_155,(uint8*)"d", s_isp_owner);
	}
	else//timeout
	{
		//SCI_TRACE_LOW:"[ISPLOCK: ISP_ServiceLock] lock timeout, owner = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7351_112_2_18_1_43_50_156,(uint8*)"d", s_isp_owner);
	}

	return ret;
#endif	
}


/*****************************************************************************/
//  Description: Unlock ISP Service
//	Global resource dependence:
// Author: Tim.zhu
// input parameter:
//                      owner:		the locker
// output parameter:
//                      None
// return:				None
// Note: Only the owner can unlock the ISP Service
/*****************************************************************************/
PUBLIC void ISP_ServiceUnlock(ISP_SERVICE_OWNER_E owner)
{
#if !defined(PLATFORM_SC6600L)	

	uint32 sem_count = 0;	//semphore count
	uint32 susp_count = 0;	//semphore suspend thread count on the semphore

	SCI_GetSemaphoreInfo(s_isp_semap_ptr, NULL, &sem_count, &susp_count);
	
	//SCI_TRACE_LOW:"[ISPLOCK: ISP_ServiceUnlock] owner = %d, cur owner = %d, sem_count = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7378_112_2_18_1_43_50_157,(uint8*)"ddd", owner, s_isp_owner, sem_count);

	if (0 == sem_count && s_isp_owner == owner && NULL != s_isp_semap_ptr)
	{
		s_isp_owner = ISP_SERVICE_OWNER_NONE;
		SCI_PutSemaphore(s_isp_semap_ptr);			

		//SCI_TRACE_LOW:"[ISPLOCK: ISP_ServiceUnlock] unlock done, owner = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7385_112_2_18_1_43_50_158,(uint8*)"d", s_isp_owner);
	}
#endif	
}

/*****************************************************************************/
//  Description: Is ISP idle
//	Global resource dependence:
// Author: Tim.zhu
// input parameter:		
//						None
// output parameter:
//                      None
// return:				TRUE: idle. 
//						FALSE: busy
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISP_ServiceIsIdle(void)
{
	uint32 sem_count = 0;	//semphore count
	uint32 susp_count = 0;	//semphore suspend thread count on the semphore
	
	_ISP_CreateSemaphore();	
	
	SCI_GetSemaphoreInfo(s_isp_semap_ptr, NULL, &sem_count, &susp_count);

	//SCI_TRACE_LOW:"[ISPLOCK: ISP_ServiceIsIdle] sem_count = %d, susp_count = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7410_112_2_18_1_43_50_159,(uint8*)"dd", sem_count, susp_count);

	if (sem_count > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*****************************************************************************/
//  Description: Put mutex
//	Global resource dependence:
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************/
PUBLIC void  ISP_ServiceGetMutex(void)
{
    _ISP_GetMutex();	
}

/*****************************************************************************/
//  Description: Put mutex
//	Global resource dependence:
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************/
PUBLIC void  ISP_ServicePutMutex(void)
{
    _ISP_PutMutex();
}

/****************************************************************************************/
// Description: Isp srevice init that creat the isp service thread/event/timer
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
PUBLIC void ISP_ServiceInit(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint32 thread_pori =  PRI_ISP_SERVICE_TASK ;

	_ISP_FreeScaleCoeff();

    SCI_MEMSET(pISPContext,0,sizeof(ISP_SERVICE_T));
    pISPContext->service = ISP_SERVICE_IDLE;
    pISPContext->state = ISP_SERVICE_IDLE_STATE;

    if(SCI_INVALID_BLOCK_ID == s_isp_service_thread_id)
    {
        s_isp_service_thread_id = SCI_CreateAppThread("T_ISP_SERVICE",
                                                        "T_ISP_QUEUE",
                                                        _ISP_ServiceHandleRoutine,
                                                        0,
                                                        0,
                                                        ISP_STACK_SIZE,
                                                        ISP_QUEUE_NUM,
                                                        thread_pori,
                                                        SCI_PREEMPT,
                                                        SCI_DONT_START);

        if(SCI_INVALID_BLOCK_ID!=s_isp_service_thread_id)
        {
            SCI_ResumeThread(s_isp_service_thread_id);
            s_is_isp_service_thread_active = SCI_TRUE;
        }
    }

    SCI_PASSERT (SCI_INVALID_BLOCK_ID!=s_isp_service_thread_id, ("ISP_SERVICE: Create isp service thread  error!"));/*assert verified*/

    // Create an isp event group     
    if (NULL == s_isp_event)
    {
        s_isp_event = SCI_CreateEvent("SC6600L_ISP_EVENT"); 
    }

    if(NULL==s_isp_timer)
    {
        s_isp_timer = SCI_CreateTimer("ISP Timer", _ISP_TimerOutHandler, 0, ISP_WAIT_TIME, SCI_NO_ACTIVATE);
    }

}

/****************************************************************************************/
// Description: Isp service open that dcam open, set dcam clk and set isp thread active
// Global resource dependence: 
// Author: Jianping.wang
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
PUBLIC void ISP_ServiceOpen(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceOpen"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7522_112_2_18_1_43_50_160,(uint8*)"");

	if (!_ISP_AllocScaleCoeff())
	{
		_ISP_FreeScaleCoeff();
		//SCI_TRACE_ISP:"ISP_SERVICE: _ISP_AllocScaleCoeff failed !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7527_112_2_18_1_43_50_161,(uint8*)"");
		return;
	}	

    pISPContext->service = ISP_SERVICE_IDLE;
    pISPContext->state   = ISP_SERVICE_IDLE_STATE;

    s_is_isp_service_thread_active = SCI_TRUE; 

    return ;
}
/****************************************************************************************/
// Description: Isp service close that del the isp service thread, close the dcam clk
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
PUBLIC void ISP_ServiceClose (void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint32 status = 0;    
    uint32 priority_origin = ISP_SERVICE_PRIO;
    uint32 priority_change = ISP_SERVICE_PRIO;

    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceClose, cur task: 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7555_112_2_18_1_43_50_162,(uint8*)"d",SCI_IdentifyThread());
    //protect the close action -- msg and getEvt
    _ISP_GetMutex();
    
    _ISP_GetPeekSigMutex();     //peek signal protection
    _ISP_PutPeekSigMutex();
     s_isp_mpeg_display_callback = PNULL;
 
    if(ISP_SERVICE_CLOSE_STATE==pISPContext->state)
    { 
    	_ISP_FreeScaleCoeff();  
        _ISP_PutMutex();
         return ;
    }

    _ISP_PeekSignal();

    if((ISP_SERVICE_IDLE_STATE==pISPContext->state)&&(ISP_MODE_IDLE==ISP_GetMode()))
    {  
    	_ISP_FreeScaleCoeff();

        pISPContext->state=ISP_SERVICE_CLOSE_STATE;            
        pISPContext->service=ISP_SERVICE_IDLE;
        s_is_isp_service_thread_active=SCI_FALSE;
        //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceClose not msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7579_112_2_18_1_43_50_163,(uint8*)"");
        _ISP_PutMutex();
        return ;
    }

    //**piriority set, to avoid queue error because of the peakSignal when multi-task ----
    //get current isp priority and caller priority
    priority_origin = _ISP_Service_GetIspPriority();
    priority_change = _ISP_Service_GetCallerPriority();

    if(priority_change != priority_origin)
    {
        _ISP_Service_SetIspPriority(priority_change);
        _ISP_Service_GetIspPriority();
    }
    
    s_Isp_Peek_Stop_State = SCI_TRUE;

    _ISP_ServiceSendMsg(0,ISP_SERVICE_CLOSE_SIG,0,0,0);

    s_is_isp_service_thread_active=SCI_FALSE;      

    status=_ISP_GetEvent(ISP_CLOSE_EVENT, ISP_WAIT_TIME);
    
    s_Isp_Peek_Stop_State = SCI_FALSE;

   

        
    //restore the isp_service priority
    if(priority_origin != _ISP_Service_GetIspPriority())
    {
        _ISP_Service_SetIspPriority(priority_origin);
        _ISP_Service_GetIspPriority();
    }

	//free the coefficient buffer after the isp task stop
	_ISP_FreeScaleCoeff();

    _ISP_PutMutex();

    return ;
}

/****************************************************************************************/
// Description: Isp service register the dc call back function
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_SERVICE_CALLBACK_ID_E id: the callback function id  
//                      int (*func)(): the ponter of register function
// output parameter:
//                      None
// return:
//          None     
// Note:
/*****************************************************************************************/
PUBLIC int32 ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ID_E id, int (*func)())
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    if(PNULL!=func)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Register Function is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7640_112_2_18_1_43_51_164,(uint8*)"");
    }

    switch(id)
    {
        case ISP_SERVICE_GET_DATA_CALLBACK:   
            pISPContext->get_data = func; 
            break;     
            
        case ISP_SERVICE_MJPG_CALLBACK_NOTICE:   
            pISPContext->mjpeg_record_notice_func = func; 
            break;
            
        case ISP_SERVICE_DC_CALLBACK:
            pISPContext->dc_callback = func;
            break;
            
        case ISP_SERVICE_STOP_CALLBACK:
            pISPContext->stop_callback = func;
            break;
            
        default:
            //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceRegister default %d line error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7662_112_2_18_1_43_51_165,(uint8*)"d", __LINE__);
            break;
    }

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Isp set the preview paramter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      param_ptr: the pointer of preview parameter
// output parameter:
//                      None
// return:
//          SCI_SUCCESS:success     
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceSetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
    //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceSetPreviewParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7681_112_2_18_1_43_51_166,(uint8*)"");

    if(NULL==param_ptr)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Preview Param Ptr is PNULL %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7685_112_2_18_1_43_51_167,(uint8*)"d", __LINE__);
        return SCI_ERROR;
    }

    if(0x00!=isp_trace_eb)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7692_112_2_18_1_43_51_168,(uint8*)"dddd",param_ptr->input_rect.x,param_ptr->input_rect.y,param_ptr->input_rect.w,param_ptr->input_rect.h);

        //SCI_TRACE_ISP:"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x = %d,disp_range.y = %d,disp_range.w = %d,disp_range.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7695_112_2_18_1_43_51_169,(uint8*)"dddddd",param_ptr->disp_lcd_id,param_ptr->disp_block_id,param_ptr->disp_rect.x,param_ptr->disp_rect.y,param_ptr->disp_rect.w,param_ptr->disp_rect.h);

        //SCI_TRACE_ISP:"ISP_SERVICE:target_rect.x = %d,target_rect.y = %d,target_rect.w = %d,target_rect.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7698_112_2_18_1_43_51_170,(uint8*)"dddd",param_ptr->target_rect.x,param_ptr->target_rect.y,param_ptr->target_rect.w,param_ptr->target_rect.h);

        //SCI_TRACE_ISP:"ISP_SERVICE: param_ptr->init_zoom_level = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7700_112_2_18_1_43_51_171,(uint8*)"d",param_ptr->init_zoom_level);

        //SCI_TRACE_ISP:"ISP_SERVICE: param_ptr->disp_rotation preview = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7702_112_2_18_1_43_51_172,(uint8*)"d",param_ptr->disp_rotation);
    }

    ISP_RETURN_IF_ERR(_ISP_CheckPreviewParam(param_ptr), SCI_ERROR, ("ISP_SERVICE: ISP_ServiceSetPreviewParam %d line error",__LINE__));

    ISP_RETURN_IF_ERR(_ISP_SetPreviewParam(param_ptr), SCI_ERROR, ("ISP_SERVICE: ISP_ServiceSetPreviewParam %d line error",__LINE__));

    if(ISP_ServiceGetAtvIsEnable())
    {
        _ISP_SetATVPreviewParam();
    }
    
    return SCI_SUCCESS;
}
/****************************************************************************************/
// Description: Send the message of start preview to isp service thread
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail    
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceStartPreview(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceStartPreview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7731_112_2_18_1_43_51_173,(uint8*)"");
    
    _ISP_GetMutex();

    if(ISP_SERVICE_CLOSE_STATE==pISPContext->state)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: The state is not idle before Preview"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7737_112_2_18_1_43_51_174,(uint8*)"");
        _ISP_PutMutex();
        return SCI_ERROR;
    }

    pISPContext->is_copy_display_data = TRUE;
        
    if(!(ISP_ServiceGetAtvIsEnable()))
    {
        pISPContext->service=ISP_SERVICE_PREVIEW; 
        pISPContext->invalidate_mode=ISP_SERVICE_INVALIDATE_ASYNC;
    }
    else
    {
        pISPContext->service=ISP_SERVICE_ATV_PREVIEW;
        pISPContext->invalidate_mode=ISP_SERVICE_INVALIDATE_SYNC;
    }

    pISPContext->is_success=SCI_SUCCESS;   


    _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_SIG,0,0,0);  

    if(SCI_SUCCESS!=_ISP_GetEvent(ISP_DISPLAY_EVENT, ISP_EVENT_TIMEOUT))
    {
        _ISP_PeekSignal();
        pISPContext->is_success=SCI_ERROR;
         _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_TIMEOUT_SIG,0,0,0);
        _ISP_GetEvent(ISP_START_TIMEOUT_EVENT, ISP_EVENT_TIMEOUT_HANDLE);
        //SCI_TRACE_LOW:"ISP_ServiceStartPreview: preview start error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7766_112_2_18_1_43_51_175,(uint8*)"");
    }

    _ISP_PutMutex();

    return  pISPContext->is_success;
}

/****************************************************************************************/
// Description: Set the flag of analog tv
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail    
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceSetAtvEnable(BOOLEAN is_eb)
{
    return  Set_ATV_Enable(is_eb);
}

/****************************************************************************************/
// Description: Get the analog tv is enable
// Global resource dependence: 
// Author: Jianping.wang
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          BOOLEAN     
// Note:
/*****************************************************************************************/
PUBLIC BOOLEAN ISP_ServiceGetAtvIsEnable(void)
{
    return  ISP_TV_IsEnable();
}

/****************************************************************************************/
// Description: Set capture parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//              ISP_JPEG_PARAM_T *param_ptr : the pointer of capture parameter
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail    
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceSetCaptureParam(ISP_JPEG_PARAM_T *param_ptr)
{
    if(NULL==param_ptr)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Capture Param Ptr is PNULL %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7820_112_2_18_1_43_51_176,(uint8*)"d", __LINE__);
        return SCI_ERROR;
    }

    if(0x00!=isp_trace_eb)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceSetCaptureParam"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7826_112_2_18_1_43_51_177,(uint8*)"");

        //SCI_TRACE_ISP:"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7829_112_2_18_1_43_51_178,(uint8*)"dddd",param_ptr->input_rect.x,param_ptr->input_rect.y,param_ptr->input_rect.w,param_ptr->input_rect.h);

        //SCI_TRACE_ISP:"ISP_SERVICE:output.w = %d,output.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7831_112_2_18_1_43_51_179,(uint8*)"dd",param_ptr->output_size.w,param_ptr->output_size.h);

        //SCI_TRACE_ISP:"ISP_SERVICE: skip num: %d, in_data_format: %d, out_data_format: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7833_112_2_18_1_43_51_180,(uint8*)"ddd",param_ptr->skip_num,param_ptr->sensor_data_format,param_ptr->output_data_format);

        //SCI_TRACE_ISP:"ISP_SERVICE: cap_addr: 0x%x, size: 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7835_112_2_18_1_43_51_181,(uint8*)"dd",param_ptr->capture_addr,param_ptr->capture_buf_size);
    }

    ISP_RETURN_IF_ERR(_ISP_CheckCaptureParam(param_ptr), SCI_ERROR, ("ISP_SERVICE: ISP_ServiceSetCaptureParam %d line error",__LINE__));

    ISP_RETURN_IF_ERR(_ISP_SetCaptureParam(param_ptr), SCI_ERROR, ("ISP_SERVICE: ISP_ServiceSetCaptureParam %d line error",__LINE__));

    return SCI_SUCCESS;

}
/****************************************************************************************/
// Description: Sync capture
// Global resource dependence: 
// Author: Tim.zhu
// input parameter
//              ISP_DATA_ADDR_T* capture_addr_ptr: the capture buf pointer that yuv mode y addr and uv addr, 
//                                                                          jpg mode y addr is the jpeg stream pointer
//output parameter
//              uint32* width: the width of image
//              uint32* height: the height of image
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceStartSyncCapture(ISP_DATA_ADDR_T* capture_addr_ptr, uint32* width_ptr,uint32* height_ptr)
{	
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceStartSyncCapture, in time: %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7862_112_2_18_1_43_51_182,(uint8*)"d", SCI_GetTickCount());

    _ISP_GetMutex();

    if(ISP_SERVICE_CLOSE_STATE==pISPContext->state)
    { 
        //SCI_TRACE_ISP:"ISP_SERVICE: The state is not idle before Sync Capture"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7868_112_2_18_1_43_51_183,(uint8*)"");
        _ISP_PutMutex();
        return SCI_ERROR;
    }    

    pISPContext->is_success=SCI_SUCCESS;
    pISPContext->service=ISP_SERVICE_CAPTURE; 
                    
    _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_SIG,0,0,0);

    if(SCI_SUCCESS!=_ISP_GetEvent(ISP_CAPTURE_EVENT, ISP_EVENT_TIMEOUT))
    {
        ISP_SoftReset();
        _ISP_PeekSignal();
        
        _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_TIMEOUT_SIG,0,0,0); 
        
        _ISP_GetEvent(ISP_START_TIMEOUT_EVENT, ISP_EVENT_TIMEOUT_HANDLE);

        pISPContext->output_addr.y_addr=PNULL;
        pISPContext->output_addr.uv_addr=PNULL;
        pISPContext->output_size.w=ISP_ZERO;
        pISPContext->output_size.h=ISP_ZERO; 
        pISPContext->is_success=SCI_ERROR;
        //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceStartSyncCapture time out"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7892_112_2_18_1_43_51_184,(uint8*)"");
    }

    if(ISP_DATA_JPEG==pISPContext->input_image_format)
    {
        capture_addr_ptr->y_addr=pISPContext->output_addr.y_addr;
        capture_addr_ptr->uv_addr=NULL;
        *width_ptr=pISPContext->cap_jpg_size;
        *height_ptr=ISP_ZERO;
    }
    else
    {
        capture_addr_ptr->y_addr=pISPContext->output_addr.y_addr;
        capture_addr_ptr->uv_addr=pISPContext->output_addr.uv_addr;
        *width_ptr=(uint32)pISPContext->output_size.w;
        *height_ptr=(uint32)pISPContext->output_size.h;   
    }
    //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceStartSyncCapture, out time: %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7909_112_2_18_1_43_51_185,(uint8*)"d", SCI_GetTickCount());
    _ISP_PutMutex();

    return pISPContext->is_success;
    
}

/****************************************************************************************/
// Description: Sync capture
// Global resource dependence: 
// Author: Tim.zhu
// input parameter
//              ISP_DATA_ADDR_T* capture_addr_ptr: the capture buf pointer that yuv mode y addr and uv addr, 
//                                                                          jpg mode y addr is the jpeg stream pointer
//				timer_out: timer out for sync capture
//output parameter
//              uint32* width: the width of image
//              uint32* height: the height of image
//return:
//          SCI_SUCCESS: success
//          other: fail
// Note:		
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceStartSyncCaptureExt(ISP_DATA_ADDR_T* capture_addr_ptr, uint16* width_ptr,uint16* height_ptr, uint32 timer_out)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceStartSyncCapture, in time: %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7934_112_2_18_1_43_51_186,(uint8*)"d", SCI_GetTickCount());

    _ISP_GetMutex();
    
    if(ISP_SERVICE_CLOSE_STATE==pISPContext->state)
    { 
        //SCI_TRACE_ISP:"ISP_SERVICE: The state is not idle before Sync Capture"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7940_112_2_18_1_43_51_187,(uint8*)"");
        _ISP_PutMutex();
        return SCI_ERROR;
    }    

    pISPContext->is_success=SCI_SUCCESS;
    pISPContext->service=ISP_SERVICE_CAPTURE; 

    _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_SIG,0,0,0);

    if(SCI_SUCCESS!=_ISP_GetEvent(ISP_CAPTURE_EVENT, ISP_EVENT_TIMEOUT))
    {
        ISP_SoftReset();
        _ISP_PeekSignal();


        _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_TIMEOUT_SIG,0,0,0); 
        
        _ISP_GetEvent(ISP_START_TIMEOUT_EVENT, ISP_EVENT_TIMEOUT_HANDLE);

        
        pISPContext->output_addr.y_addr=PNULL;
        pISPContext->output_addr.uv_addr=PNULL;
        pISPContext->output_size.w=ISP_ZERO;
        pISPContext->output_size.h=ISP_ZERO;
        pISPContext->is_success=SCI_ERROR;
        //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceStartSyncCapture time out"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7966_112_2_18_1_43_51_188,(uint8*)"");
    } 

    capture_addr_ptr->y_addr=pISPContext->output_addr.y_addr;
    capture_addr_ptr->uv_addr=pISPContext->output_addr.uv_addr;
    *width_ptr=pISPContext->output_size.w;
    *height_ptr=pISPContext->output_size.h;    

    //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceStartSyncCapture, out time: %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7974_112_2_18_1_43_51_189,(uint8*)"d", SCI_GetTickCount());
    _ISP_PutMutex();

    return pISPContext->is_success;

}


/****************************************************************************************/
// Description: Async capture that send the message to isp service thread
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail   
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceStartAsyncCapture(void)
{	
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    BOOLEAN is_capture_success =TRUE;

    //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceStartAsyncCapture"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_7998_112_2_18_1_43_51_190,(uint8*)"");

    if(ISP_SERVICE_CLOSE_STATE==pISPContext->state)
    {  
        //SCI_TRACE_ISP:"ISP_SERVICE: The state is not idle before Async Capture"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8002_112_2_18_1_43_51_191,(uint8*)"");
        return SCI_ERROR;
    }    

    pISPContext->service=ISP_SERVICE_CAPTURE;     
    
    _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_SIG,0,0,0);
	
    if(!is_capture_success)/*lint !e774*/
    {        
        _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_TIMEOUT_SIG,0,0,0);
        
        return SCI_ERROR;
    } 

    return SCI_SUCCESS;
    
}

/****************************************************************************************/
// Description: Get capture data 
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_CAPTRUE_DATA_T *data_info_ptr: capture data pointer
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetCaptureData(ISP_CAPTRUE_DATA_T *data_info_ptr)
{
    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Set review the parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                     ISP_REVIEW_PARAM_T *param_ptr: Review parameter
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceSetReviewParam(ISP_REVIEW_PARAM_T *param_ptr)
{   
    //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceSetReviewParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8050_112_2_18_1_43_51_192,(uint8*)"");

    if(NULL==param_ptr)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Review Param Ptr is PNULL %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8054_112_2_18_1_43_51_193,(uint8*)"d", __LINE__);
        return SCI_ERROR;
    }

    if(0x00!=isp_trace_eb)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE:input.w = %d, input.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8060_112_2_18_1_43_51_194,(uint8*)"dd",param_ptr->input_size.w,param_ptr->input_size.h);
        
        //SCI_TRACE_ISP:"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8063_112_2_18_1_43_51_195,(uint8*)"dddd",param_ptr->input_rect.x,param_ptr->input_rect.y,param_ptr->input_rect.w,param_ptr->input_rect.h);

        //SCI_TRACE_ISP:"ISP_SERVICE:display.x=%d,display.y=%d,display.w=%d,display.h=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8065_112_2_18_1_43_51_196,(uint8*)"dddd",param_ptr->disp_rect.x,param_ptr->disp_rect.y,param_ptr->disp_rect.w,param_ptr->disp_rect.h);

        //SCI_TRACE_ISP:"ISP_SERVICE:target.x=%d,target.y=%d,target.w=%d,target.h=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8067_112_2_18_1_43_51_197,(uint8*)"dddd",param_ptr->target_rect.x,param_ptr->target_rect.y,param_ptr->target_rect.w,param_ptr->target_rect.h);
     
        //SCI_TRACE_ISP:"ISP_SERVICE:scale line = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8069_112_2_18_1_43_51_198,(uint8*)"d",(((param_ptr->disp_rect.h+param_ptr->input_rect.h-ISP_ONE)/param_ptr->input_rect.h)-ISP_ONE));

        //SCI_TRACE_ISP:"ISP_SERVICE: param_ptr->disp_rotation = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8071_112_2_18_1_43_51_199,(uint8*)"d",param_ptr->disp_rotation);
        //SCI_TRACE_ISP:"ISP_SERVICE: param_ptr->disp_format = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8072_112_2_18_1_43_51_200,(uint8*)"d",param_ptr->disp_format);
        
        //SCI_TRACE_ISP:"ISP_SERVICE: src y_addr = 0x%x, uv_addr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8074_112_2_18_1_43_51_201,(uint8*)"dd",param_ptr->src_addr.y_addr,param_ptr->src_addr.uv_addr);
        //SCI_TRACE_ISP:"ISP_SERVICE: rot y_addr = 0x%x, uv_addr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8075_112_2_18_1_43_51_202,(uint8*)"dd",param_ptr->rot_addr.y_addr,param_ptr->rot_addr.uv_addr);
        //SCI_TRACE_ISP:"ISP_SERVICE: param_ptr->disp_addr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8076_112_2_18_1_43_51_203,(uint8*)"d",param_ptr->disp_addr);
    }

    if(SCI_SUCCESS!=_ISP_CheckReviewParam(param_ptr))
    {
        return SCI_ERROR;
    }

    _ISP_SetReviewParam(param_ptr);	
    
    return SCI_SUCCESS;
}
/****************************************************************************************/
// Description: Start the review that send the message to isp service thread
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceStartReview(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
 
    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceStartReview: %dms"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8103_112_2_18_1_43_51_204,(uint8*)"d",SCI_GetTickCount());

    _ISP_GetMutex();

    if(ISP_SERVICE_CLOSE_STATE==pISPContext->state)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: The staet is idle defore Review"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8109_112_2_18_1_43_51_205,(uint8*)"");
        _ISP_PutMutex();
        return SCI_ERROR;
    }

    pISPContext->is_success = SCI_SUCCESS;
    pISPContext->service=ISP_SERVICE_REVIEW;


    if((PNULL == s_isp_mpeg_display_callback) &&((ISP_DATA_RGB565==pISPContext->input_image_format)
        ||((ISP_DATA_YUV422==pISPContext->disp_format)
            &&(pISPContext->cap_output_size.w==pISPContext->scale_trim_rect.w)
            &&(pISPContext->cap_output_size.w==pISPContext->target_rect.w))))
    {
        pISPContext->review_display_buf_addr.y_addr=pISPContext->input_addr.y_addr;
        pISPContext->review_display_buf_addr.uv_addr=pISPContext->input_addr.uv_addr;        
        _ISP_ServiceSendMsg(ISP_SERVICE_REVIEW,ISP_SERVICE_TRANSFER_DONE_SIG, 0,0,0);
    }
    else
    {
        _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_SIG,0,0,0);
    }
    
    //start state or there have no callback, must have no timeout
    if(SCI_SUCCESS!=_ISP_GetEvent(ISP_REVIEW_EVENT, ISP_EVENT_TIMEOUT))
    {
        _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_TIMEOUT_SIG,0,0,0);
         pISPContext->is_success=SCI_ERROR;
    }
    
    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceStartReview end: %dms"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8139_112_2_18_1_43_52_206,(uint8*)"d",SCI_GetTickCount());

    _ISP_PutMutex();
    return pISPContext->is_success;

    
    
}
/****************************************************************************************/
// Description: Start scaling that send the message to isp service thread
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_SCALE_PARAM_T *param_ptr: the scale parameter
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceSetScaleParam(ISP_SCALE_PARAM_T *param_ptr)
{
    //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceSetScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8160_112_2_18_1_43_52_207,(uint8*)"");

    if(NULL==param_ptr)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Capture Param Ptr is PNULL %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8164_112_2_18_1_43_52_208,(uint8*)"d", __LINE__);
        return SCI_ERROR;
    }

    if(0x00!=isp_trace_eb)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8171_112_2_18_1_43_52_209,(uint8*)"dddd",param_ptr->input_rect.x,param_ptr->input_rect.y,param_ptr->input_rect.w,param_ptr->input_rect.h);

        //SCI_TRACE_ISP:"ISP_SERVICE:output.w = %d,output.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8173_112_2_18_1_43_52_210,(uint8*)"dd",param_ptr->output_size.w,param_ptr->output_size.h);
        //SCI_TRACE_ISP:"ISP_SERVICE: in y_addr = 0x%x, uv_addr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8174_112_2_18_1_43_52_211,(uint8*)"dd",param_ptr->input_addr.y_addr,param_ptr->input_addr.uv_addr);
        //SCI_TRACE_ISP:"ISP_SERVICE: out y_addr = 0x%x, uv_addr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8175_112_2_18_1_43_52_212,(uint8*)"dd",param_ptr->output_addr.y_addr,param_ptr->output_addr.uv_addr);
        //SCI_TRACE_ISP:"ISP_SERVICE: slice line: %d, is_f:%d,is_l:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8176_112_2_18_1_43_52_213,(uint8*)"ddd", param_ptr->slice_line, param_ptr->is_first, param_ptr->is_last);
    }

    if((ISP_SERVICE_SCALE_NORMAL==param_ptr->scale_mode)
        ||(SCI_TRUE==param_ptr->is_first))
    {
        _ISP_CheckScaleParam(param_ptr);

        _ISP_SetScaleParam(param_ptr);
    }
    else
    {
        _ISP_SetSliceScaleParam(param_ptr);	
    }

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Start scaling that send the message to isp service thread
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceStartScale(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceScale"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8209_112_2_18_1_43_52_214,(uint8*)"");

    _ISP_GetMutex();

    if(ISP_SERVICE_CLOSE_STATE==pISPContext->state)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: The staet is idle before Scale"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8215_112_2_18_1_43_52_215,(uint8*)"");
        _ISP_PutMutex();
        return SCI_ERROR;
    }
    
    pISPContext->is_success=SCI_SUCCESS;
    
    if((ISP_SERVICE_SCALE_SLICE==pISPContext->scale_mode)
        &&(SCI_FALSE==pISPContext->is_first))
    {   
        if(pISPContext->output_size.h<=pISPContext->slice_complete_line)
        {
            pISPContext->output_addr.y_addr=PNULL;
            pISPContext->output_addr.uv_addr=PNULL;
            pISPContext->output_size.w=ISP_ZERO;
            pISPContext->output_size.h=ISP_ZERO;
            pISPContext->get_data(pISPContext->output_addr, (pISPContext->output_size.w), (pISPContext->output_size.h));	
            pISPContext->get_data=PNULL;
    
            _ISP_PutMutex();

            return pISPContext->is_success;
        }
        pISPContext->service=ISP_SERVICE_SLICE_SCALE;
    }
    else
    {
        pISPContext->service=ISP_SERVICE_SCALE;
    }
    

    _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_SIG,0,0,0);

    if(SCI_SUCCESS!=_ISP_GetEvent(ISP_SCALE_EVENT, ISP_EVENT_TIMEOUT))
    {
        ISP_SoftReset();
        _ISP_PeekSignal();

        _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_TIMEOUT_SIG,0,0,0);
        
        pISPContext->output_addr.y_addr=PNULL;
        pISPContext->output_addr.uv_addr=PNULL;
        pISPContext->output_size.w=ISP_ZERO;
        pISPContext->output_size.h=ISP_ZERO;
        pISPContext->is_success=SCI_ERROR;

        if(PNULL!=pISPContext->get_data)
        {
            pISPContext->get_data(pISPContext->output_addr, (pISPContext->output_size.w), (pISPContext->output_size.h));	
            pISPContext->get_data=PNULL;
        } 
    }

    _ISP_PutMutex();
    
    return pISPContext->is_success;
}

/****************************************************************************************/
// Description: Set the Mjpg parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_MJPEG_PARAM_T *param_ptr: the mjpg recoder parameter
// output parameter:
//                      None
// return:
//          SCI_SUCCESS:success
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceSetMjpegParam(ISP_MJPEG_PARAM_T *param_ptr)
{
    if(NULL==param_ptr)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Mjpg Param Ptr is PNULL %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8288_112_2_18_1_43_52_216,(uint8*)"d", __LINE__);
        return SCI_ERROR;
    }
    
    if(0x00!=isp_trace_eb)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceSetMjpegParam"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8294_112_2_18_1_43_52_217,(uint8*)"");

        //SCI_TRACE_ISP:"ISP_SERVICE:input_size.w = %d,input_size.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8296_112_2_18_1_43_52_218,(uint8*)"dd",param_ptr->input_size.w,param_ptr->input_size.h);

        //SCI_TRACE_ISP:"ISP_SERVICE:input_rect.x = %d, input_rect.y = %d,input_rect.w = %d, input_rect.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8299_112_2_18_1_43_52_219,(uint8*)"dddd",param_ptr->input_rect.x,param_ptr->input_rect.y,param_ptr->input_rect.w,param_ptr->input_rect.h);

        //SCI_TRACE_ISP:"ISP_SERVICE:capture_size.w = %d,capture_size.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8301_112_2_18_1_43_52_220,(uint8*)"dd",param_ptr->capture_size.w,param_ptr->capture_size.h);

        //SCI_TRACE_ISP:"ISP_SERVICE:jpeg_target_rect.x = %d, jpeg_target_rect.y = %d,jpeg_target_rect.w = %d, jpeg_target_rect.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8304_112_2_18_1_43_52_221,(uint8*)"dddd",param_ptr->jpeg_target_rect.x,param_ptr->jpeg_target_rect.y,param_ptr->jpeg_target_rect.w,param_ptr->jpeg_target_rect.h);

        //SCI_TRACE_ISP:"ISP_SERVICE:jpeg_target_size.w = %d,jpeg_target_size.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8306_112_2_18_1_43_52_222,(uint8*)"dd",param_ptr->jpeg_target_size.w,param_ptr->jpeg_target_size.h);

        //SCI_TRACE_ISP:"ISP_SERVICE:disp_rect.x = %d, disp_rect.y = %d,disp_rect.w = %d, disp_rect.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8309_112_2_18_1_43_52_223,(uint8*)"dddd",param_ptr->disp_rect.x,param_ptr->disp_rect.y,param_ptr->disp_rect.w,param_ptr->disp_rect.h);

        //SCI_TRACE_ISP:"ISP_SERVICE:target_src_rect.x = %d, target_src_rect.y = %d,target_src_rect.w = %d, target_src_rect.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8312_112_2_18_1_43_52_224,(uint8*)"dddd",param_ptr->target_src_rect.x,param_ptr->target_src_rect.y,param_ptr->target_src_rect.w,param_ptr->target_src_rect.h);

        //SCI_TRACE_ISP:"ISP_SERVICE:target_rect.x = %d, target_rect.y = %d,target_rect.w = %d, target_rect.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8315_112_2_18_1_43_52_225,(uint8*)"dddd",param_ptr->target_rect.x,param_ptr->target_rect.y,param_ptr->target_rect.w,param_ptr->target_rect.h);
    }

    ISP_RETURN_IF_ERR(_ISP_CheckMjpegParam(param_ptr), SCI_ERROR, ("ISP_SERVICE: ISP_ServiceSetMjpegParam %d line error",__LINE__));

    ISP_RETURN_IF_ERR(_ISP_SetMjpegParam(param_ptr), SCI_ERROR, ("ISP_SERVICE: ISP_ServiceSetMjpegParam %d line error",__LINE__));

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Start the Mjpg
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail  
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceStartMjpeg(void)
{	
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceStartMjpeg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8340_112_2_18_1_43_52_226,(uint8*)"");

    if(ISP_SERVICE_CLOSE_STATE==pISPContext->state)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: The state is not idle before StartMjpeg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8344_112_2_18_1_43_52_227,(uint8*)"");
        return SCI_ERROR;
    }  
    
    _ISP_GetMutex();
    
    pISPContext->is_success=SCI_SUCCESS;

    if(ISP_ServiceGetAtvIsEnable())
    {
        pISPContext->service=ISP_SERVICE_ATV_RECORDER;
    }
    else
    {
        pISPContext->service=ISP_SERVICE_MOTION_JPEG;
    }
    

    _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_SIG,0,0,0);

    if(SCI_SUCCESS!=_ISP_GetEvent(ISP_MOTION_JPEG_START_EVENT, ISP_EVENT_TIMEOUT))
    {
        _ISP_PeekSignal();
        pISPContext->is_success=SCI_ERROR;

        _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_TIMEOUT_SIG,0,0,0);
        _ISP_GetEvent(ISP_START_TIMEOUT_EVENT, ISP_EVENT_TIMEOUT_HANDLE);   
    }             

    _ISP_PutMutex();

    return pISPContext->is_success;
}

/****************************************************************************************/
// Description: Set the Mjpg parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      ISP_MJPEG_PARAM_T *param_ptr: the mjpg recoder parameter
// output parameter:
//                      None
// return:
//          SCI_SUCCESS:success
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceSetUpccParam(ISP_MJPEG_PARAM_T *param_ptr)
{
    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceSetUpccParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8391_112_2_18_1_43_52_228,(uint8*)"");

    if(NULL==param_ptr)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Upcc Param Ptr is PNULL %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8395_112_2_18_1_43_52_229,(uint8*)"d", __LINE__);
        return SCI_ERROR;
    }

    if(0x00!=isp_trace_eb)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE:input_rect.x = %d, input_rect.y = %d,input_rect.w = %d, input_rect.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8402_112_2_18_1_43_52_230,(uint8*)"dddd",param_ptr->input_rect.x,param_ptr->input_rect.y,param_ptr->input_rect.w,param_ptr->input_rect.h);

        //SCI_TRACE_ISP:"ISP_SERVICE:capture_size.w = %d,capture_size.h = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8404_112_2_18_1_43_52_231,(uint8*)"dd",param_ptr->capture_size.w,param_ptr->capture_size.h);
    }

    ISP_RETURN_IF_ERR(_ISP_CheckUpccParam(param_ptr), SCI_ERROR, ("ISP_SERVICE: ISP_ServiceSetUpccParam %d line error",__LINE__));

    ISP_RETURN_IF_ERR(_ISP_SetUpccParam(param_ptr), SCI_ERROR, ("ISP_SERVICE: ISP_ServiceSetUpccParam %d line error",__LINE__));

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Start the UPCC
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail  
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceStartUpcc(void)
{	
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceStartUPCC"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8429_112_2_18_1_43_52_232,(uint8*)"");

    if(ISP_SERVICE_CLOSE_STATE==pISPContext->state)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: The state is not idle before upcc"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8433_112_2_18_1_43_52_233,(uint8*)"");
        return SCI_ERROR;
    }  
    
    _ISP_GetMutex();
    
    pISPContext->is_success=SCI_SUCCESS;
    
    pISPContext->service=ISP_SERVICE_UPCC;

    _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_SIG,0,0,0);

    if(SCI_SUCCESS!=_ISP_GetEvent(ISP_UPCC_EVENT, ISP_EVENT_TIMEOUT))
    {
        _ISP_PeekSignal();
        pISPContext->is_success=SCI_ERROR;

        _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_START_TIMEOUT_SIG,0,0,0);
        _ISP_GetEvent(ISP_START_TIMEOUT_EVENT, ISP_EVENT_TIMEOUT_HANDLE);

    }       

    _ISP_PutMutex();

    return pISPContext->is_success;
}

/****************************************************************************************/
// Description: Pause the Mjpg
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServicePauseMjpeg(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServicePauseMjpeg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8475_112_2_18_1_43_52_234,(uint8*)"");

    pISPContext->is_pause=SCI_TRUE;

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Resume the Mjpg
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceResumeMjpeg(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    
    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceResumeMjpeg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8497_112_2_18_1_43_52_235,(uint8*)"");

    pISPContext->is_pause=SCI_FALSE;

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Resume the Mjpg
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceGetSensorFrameCounter(ISP_SENSOR_FRAME_COUNTER_T_PTR param_ptr)
{
    uint32 rtn=SCI_SUCCESS;
    ISP_SENSOR_FRAME_COUNTER_T_PTR sensor_frame_ctl_ptr=NULL;

    if(NULL==param_ptr)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: get Sensor frame Ptr is PNULL %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8522_112_2_18_1_43_52_236,(uint8*)"d", __LINE__);
        return SCI_ERROR;
    }

    sensor_frame_ctl_ptr=(ISP_SENSOR_FRAME_COUNTER_T_PTR)SCI_ALLOCA(sizeof(ISP_SENSOR_FRAME_COUNTER_T)); 

    if(NULL!=sensor_frame_ctl_ptr)
    {
        SCI_MEMSET(sensor_frame_ctl_ptr, ISP_ZERO, sizeof(ISP_SENSOR_FRAME_COUNTER_T));
        SCI_MEMCPY(sensor_frame_ctl_ptr, param_ptr, sizeof(ISP_SENSOR_FRAME_COUNTER_T));

        _ISP_ServiceSendMsg(ISP_SERVICE_SENSOR_EOF,ISP_SERVICE_START_SIG,(uint32)sensor_frame_ctl_ptr, 0, 0);

        if(NULL==sensor_frame_ctl_ptr->callback)
        {
            if(SCI_SUCCESS!=_ISP_GetEvent(ISP_SENSOR_EOF_EVENT, ISP_EVENT_TIMEOUT))
            {
                SCI_FREE(sensor_frame_ctl_ptr);
                rtn=SCI_ERROR;
            }
        }
    }
    else
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: get Sensor frame alloc %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8546_112_2_18_1_43_52_237,(uint8*)"d", __LINE__);
        rtn=SCI_ERROR;
    }

    return rtn;    
}

/****************************************************************************************/
// Description: ISP Service Stop
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceFeatureStop(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint32 status = SCI_SUCCESS;
    uint32 priority_origin = ISP_SERVICE_PRIO;
    uint32 priority_change = ISP_SERVICE_PRIO;

    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceFeatureStop : service:0x%x, task: 0x%x, %dms"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8571_112_2_18_1_43_52_238,(uint8*)"ddd",pISPContext->service, SCI_IdentifyThread(),SCI_GetTickCount());
        
    //protect the close action
    _ISP_GetMutex();
    
    _ISP_GetPeekSigMutex();     //peek signal action pectection
    _ISP_PutPeekSigMutex();
   
    if((SCI_FALSE==pISPContext->isp_feature_enable)
        ||((ISP_SERVICE_IDLE_STATE==pISPContext->state)&&(ISP_MODE_IDLE==ISP_GetMode())))
    {
        _ISP_PeekSignal();
        pISPContext->isp_feature_enable=SCI_FALSE;
        //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceFeatureStop nothing end: %dms "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8584_112_2_18_1_43_52_239,(uint8*)"d",SCI_GetTickCount());
        _ISP_PutMutex();
        return status;
    }
    
    //**piriority set, to avoid queue error because of the peakSignal when multi-task ----
    //get current isp priority and caller priority
     priority_origin = _ISP_Service_GetIspPriority();
     priority_change = _ISP_Service_GetCallerPriority();

    if(priority_change != priority_origin)
    {
        _ISP_Service_SetIspPriority(priority_change);
        _ISP_Service_GetIspPriority();
    }

    pISPContext->isp_feature_enable=SCI_FALSE;
    
    s_Isp_Peek_Stop_State = SCI_TRUE;
    
    switch(pISPContext->service)
    {
        case ISP_SERVICE_PREVIEW:
        {
            if(SCI_ERROR!=pISPContext->is_success)
            {
                ISP_CapDisable();
                pISPContext->is_stop=SCI_TRUE;
                if(SCI_SUCCESS!=_ISP_GetEvent(ISP_STOP_EVENT, ISP_PREVIEW_STOP_TIMEOUT))
                {
                    _ISP_ServiceSendMsg(ISP_SERVICE_PREVIEW,ISP_SERVICE_STOP_SIG,0,0,0);
                    status=_ISP_GetEvent(ISP_CAPTURE_STOP_EVENT, ISP_EVENT_TIMEOUT);
                }
                pISPContext->is_copy_display_data=FALSE;
            }
            break;
        }
        
        case ISP_SERVICE_CAPTURE:
        {
            ISP_CapDisable();
            _ISP_ServiceSendMsg(pISPContext->service, ISP_SERVICE_STOP_SIG,0,0,0);
            status=_ISP_GetEvent(ISP_STOP_EVENT, ISP_EVENT_TIMEOUT);
            //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceFeatureStop Stop Capture"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8627_112_2_18_1_43_53_240,(uint8*)"");
            break;
        }
            
        case ISP_SERVICE_REVIEW:
        case ISP_SERVICE_SCALE:
        {     
            _ISP_ServiceSendMsg(pISPContext->service, ISP_SERVICE_STOP_SIG,0,0,0);
            status=_ISP_GetEvent(ISP_STOP_EVENT, ISP_EVENT_TIMEOUT);
            //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceFeatureStop Stop Review"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8636_112_2_18_1_43_53_241,(uint8*)"");
            break;
        }    

        case ISP_SERVICE_MOTION_JPEG:
        case ISP_SERVICE_ATV_RECORDER:            
        {
            if(SCI_ERROR!=pISPContext->is_success)
            {
                if(SCI_NULL!=pISPContext->get_frame_data)
                {
                    pISPContext->is_get_frame_data=SCI_TRUE;
                }
                else
                {
                    pISPContext->is_stop=SCI_TRUE;
                }

                if(SCI_SUCCESS!=_ISP_GetEvent(ISP_STOP_EVENT, ISP_STOP_TIMEOUT))
                {
                    pISPContext->is_stop=SCI_TRUE;
                    _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_STOP_SIG,0,0,0);
                    status=_ISP_GetEvent(ISP_STOP_EVENT, ISP_EVENT_TIMEOUT);
                }
            }
            break;
        }

        case ISP_SERVICE_SET_SENSOR_PARAM:
            break;      

        case ISP_SERVICE_SET_SENSOR_CALLBACK:
            break;   

        case ISP_SERVICE_DC_ZOOM:
            break; 

        case ISP_SERVICE_ATV_PREVIEW:
        case ISP_SERVICE_UPCC:
        {
            if(SCI_ERROR!=pISPContext->is_success)
            {
                pISPContext->is_stop=SCI_TRUE;
                
                if(SCI_SUCCESS!=_ISP_GetEvent(ISP_STOP_EVENT, ISP_STOP_TIMEOUT))
                {
                    _ISP_ServiceSendMsg(pISPContext->service,ISP_SERVICE_STOP_SIG,0,0,0);
                    status=_ISP_GetEvent(ISP_STOP_EVENT, ISP_EVENT_TIMEOUT);
                }
            }
            break;
        }          

        default:
            //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceFeatureStop default %d line error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8690_112_2_18_1_43_53_242,(uint8*)"d", __LINE__);
            break;       
            
    }
    
    //protection over
    s_Isp_Peek_Stop_State = SCI_FALSE;

     if(SCI_SUCCESS!=status)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceFeatureStop TimeOut"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8700_112_2_18_1_43_53_243,(uint8*)"");
        
        //ISP_SoftReset();
        _ISP_PeekSignal();
        _ISP_ServiceSendMsg(pISPContext->service, ISP_SERVICE_STOP_TIMEOUT_SIG,0,0,0);
    }

    pISPContext->service=ISP_SERVICE_IDLE;

    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceFeatureStop end: %dms "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8709_112_2_18_1_43_53_244,(uint8*)"d",SCI_GetTickCount());

    //restore the isp_service priority
    if(priority_origin != _ISP_Service_GetIspPriority())
    {
        _ISP_Service_SetIspPriority(priority_origin);
        _ISP_Service_GetIspPriority();
    }
    
    _ISP_PutMutex();
    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Get isp states
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          pISPContext->state: isp state    
// Note:
/*****************************************************************************************/
PUBLIC ISP_SERVICE_STATE_E ISP_ServiceGetState(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    return pISPContext->state;
}

/****************************************************************************************/
// Description: Camera digital zoom
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      uint32 zoom_level : the current zoom level of digital camera
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other :fail
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceZoom(uint32 zoom_level, int (*func)())
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceZoom, zoom_level : %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8753_112_2_18_1_43_53_245,(uint8*)"d", zoom_level);

    if(PNULL==func)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: The digital zoom callback is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8757_112_2_18_1_43_53_246,(uint8*)"");
        return SCI_ERROR;
    }

    pISPContext->zoom_ctl.zoom_success=func;    

    if(!((ISP_SERVICE_PREVIEW==pISPContext->service)||(ISP_SERVICE_MOTION_JPEG==pISPContext->service)))
    { // only preview and mjpg recorder states execute the zoom function
        if(PNULL!=pISPContext->zoom_ctl.zoom_success)
        {
            pISPContext->zoom_ctl.zoom_success(SCI_TRUE);
            pISPContext->zoom_ctl.zoom_success=PNULL;
        }
        //SCI_TRACE_ISP:"ISP_SERVICE: ISP Service States is : %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8770_112_2_18_1_43_53_247,(uint8*)"d", pISPContext->service);
        
        return SCI_SUCCESS;
    }    

    if(zoom_level==pISPContext->zoom_ctl.zoom_level)
    {
        if(PNULL!=pISPContext->zoom_ctl.zoom_success)
        {
            pISPContext->zoom_ctl.zoom_success(SCI_TRUE);
            //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceZoom call back"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8780_112_2_18_1_43_53_248,(uint8*)"");
        }
        return SCI_SUCCESS;
    }
    else if(zoom_level>pISPContext->zoom_ctl.zoom_level)
    {
        pISPContext->zoom_ctl.zoom_mode=ISP_SERVICE_ZOOM_IN;
    }
    else if(zoom_level<pISPContext->zoom_ctl.zoom_level)
    {
        pISPContext->zoom_ctl.zoom_mode=ISP_SERVICE_ZOOM_OUT;
    }    

    pISPContext->is_zoom=SCI_TRUE;
    pISPContext->zoom_ctl.zoom_level=zoom_level; 
    pISPContext->zoom_ctl.zoom_step=0x01;
    pISPContext->is_send_zoom_sig=SCI_TRUE;

    return SCI_SUCCESS;    
}

/****************************************************************************************/
// Description: Set img sensor parameter
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail 
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceSetSensorParam(ISP_SENSOR_PARAM_T* param_ptr)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    uint32 rtn=SCI_SUCCESS;
    ISP_SENSOR_PARAM_T_PTR sensor_param_ptr=NULL;

    if(NULL==param_ptr)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: Sensor Param Ptr is PNULL %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8820_112_2_18_1_43_53_249,(uint8*)"d", __LINE__);
        return SCI_ERROR;
    }

    sensor_param_ptr=(ISP_SENSOR_PARAM_T_PTR)SCI_ALLOCA(sizeof(ISP_SENSOR_PARAM_T)); 

    SCI_MEMSET(sensor_param_ptr, ISP_ZERO, sizeof(ISP_SENSOR_PARAM_T));
    SCI_MEMCPY(sensor_param_ptr, param_ptr, sizeof(ISP_SENSOR_PARAM_T));

    //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceSetSensorParam, 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8829_112_2_18_1_43_53_250,(uint8*)"d", sensor_param_ptr);

    switch(pISPContext->service)
    {
        case ISP_SERVICE_PREVIEW:
        case ISP_SERVICE_ATV_PREVIEW:
        {
            if(ISP_SERVICE_PREVIEW==pISPContext->service)
            {
                sensor_param_ptr->syn_ctrl=SCI_TRUE;
            }
            else if(ISP_SERVICE_ATV_PREVIEW==pISPContext->service)
            {
                sensor_param_ptr->syn_ctrl=SCI_FALSE;
            }

            _ISP_ServiceSendMsg(ISP_SERVICE_SET_SENSOR_PARAM,ISP_SERVICE_START_SIG,(uint32)sensor_param_ptr, 0, 0);

            if(NULL==sensor_param_ptr->set_param_success)
            {
               _ISP_GetEvent(ISP_SET_PARAM_EVENT, ISP_EVENT_TIMEOUT);
            }
            break;
        }
        default :
        {
            sensor_param_ptr->set_param(sensor_param_ptr->cmd, sensor_param_ptr->arg);
            
            if(PNULL!=sensor_param_ptr->set_param_success)
            {
                sensor_param_ptr->set_param_success(SCI_SUCCESS);
            }

            SCI_FREE(sensor_param_ptr);
            break;
        }

    }

    return rtn;    
}

/****************************************************************************************/
// Description: Set img sensor signal information
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail 
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceSetSensorSignalInfo(ISP_SENSOR_SIGNAL_INFO_T_PTR param_ptr)
{
    uint32 rtn=SCI_SUCCESS;
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();
    ISP_SENSOR_SIGNAL_INFO_T_PTR sensor_signal_ptr=NULL;

    if(NULL!=param_ptr)
    {
        sensor_signal_ptr=param_ptr;
        pISPContext->input_image_format=sensor_signal_ptr->img_format;
        pISPContext->input_image_pattern=sensor_signal_ptr->img_pattern;
        pISPContext->vsync_polarity=sensor_signal_ptr->vsync_polarity;
        pISPContext->hsync_polarity=sensor_signal_ptr->hsync_polarity;
        pISPContext->preview_skip_frame_num=sensor_signal_ptr->preview_skip_num;
        pISPContext->capture_skip_frame_num=sensor_signal_ptr->capture_skip_num;
        pISPContext->preview_deci_frame_num=sensor_signal_ptr->preview_deci_num;
        pISPContext->video_preview_deci_frame_num=sensor_signal_ptr->video_preview_deci_num;
        pISPContext->threshold_eb=sensor_signal_ptr->threshold_eb;
        pISPContext->threshold_mode=sensor_signal_ptr->threshold_mode;
        pISPContext->threshold_start=sensor_signal_ptr->threshold_start;
        pISPContext->threshold_end=sensor_signal_ptr->threshold_end;

        if(0x00!=isp_trace_eb)
        {
            //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceSetSensorSignalInfo: p_skip:%d, img_pattern:%d, v_sync:%d, h_sync:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8906_112_2_18_1_43_53_251,(uint8*)"dddd", pISPContext->preview_skip_frame_num,pISPContext->input_image_pattern,pISPContext->vsync_polarity,pISPContext->hsync_polarity);
            //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceSetSensorSignalInfo: eb:%d, mode:%d, start:0x%x, end:0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8907_112_2_18_1_43_53_252,(uint8*)"dddd", pISPContext->threshold_eb,pISPContext->threshold_mode,pISPContext->threshold_start,pISPContext->threshold_end);
        }
    }
    else
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: ISP_ServiceSetSensorSignalInfo %d line error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8912_112_2_18_1_43_53_253,(uint8*)"d", __LINE__);
        rtn=SCI_ERROR;
    }

    return rtn;    
}

/****************************************************************************************/
// Description: Get isp frame data
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      int (*fun)(): get frame data call back
//                      ISP_DATA_FORMAT_E: get frame data format
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceGetFrameData(int (*func)(), ISP_DATA_FORMAT_E data_format)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();

    if(PNULL==func)
    {
        //SCI_TRACE_ISP:"ISP_SERVICE: The fun of get frame data is pnull"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_8937_112_2_18_1_43_53_254,(uint8*)"");
        return SCI_ERROR;
    }

    pISPContext->get_frame_data=func; /*lint !e64*/
    pISPContext->frame_data_format=data_format;
    pISPContext->is_get_frame_data=SCI_TRUE; //SCI_FALSE;
        
    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Set the flag of flag
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail    
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceSetVideFlag(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext(); 
    
    pISPContext->is_video_mode=SCI_TRUE;
    
    return SCI_SUCCESS;    
}

/****************************************************************************************/
// Description: Get isp information ptr
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          ISP_SERVICE_T*: isp infor ptr
// Note:
/*****************************************************************************************/
PUBLIC ISP_SERVICE_T_PTR ISP_ServiceGetISPInfo(void)
{
    return _ISP_GetContext();   
}

/****************************************************************************************/
// Description: get the pointer of preview zoom table
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      none
// output parameter:
//                      None
// return:
//          uint16* : the pointer of zoom table     
// Note:
/*****************************************************************************************/
PUBLIC ISP_RECT_T_PTR ISP_ServiceGetZoomTabPtr(void)
{
    return _ISP_GetZoomTabPtr();
}

/****************************************************************************************/
// Description: Clean the flag of video
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: The camera digital zoom max level    
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceCleanVideFlag(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();   
    
    pISPContext->is_video_mode=SCI_FALSE;
    
    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Get isp review display buffer addr
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: The address of display buffer    
// Note:
/*****************************************************************************************/
PUBLIC uint32  ISP_ServiceGetReviewDisplayBuf(void)
{
    ISP_SERVICE_T_PTR pISPContext=_ISP_GetContext();       
    return pISPContext->review_display_buf_addr.y_addr;
}

/****************************************************************************************/
// Description: Get isp input width align value
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      input width
//						output width
//						data format
// output parameter:
//                      None
// return:
//          uint32: The value of align
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetInputWidthAlign(uint32 input_width, uint32 input_height, 
												  uint32 output_width, uint32 output_height)
{
    uint32 align_value=1;
    uint32 subsample_shift=0x00;
	uint32 input_len = 0;
	uint32 output_len = 0;

	if (output_width * input_height < output_height * input_width)
	{
		input_len = input_width;
		output_len = output_width;
	}
	else
	{
		input_len = input_height;
		output_len = output_height;
	}
	
    if(SCI_TRUE==ISP_IsSupportSubsample())
    {
        if((output_len*ISP_SCALE_4X)<input_len)
        {
            if(input_len<((output_len<<ISP_SHIFT_BIT2)<<ISP_SHIFT_BIT1))
            {
                subsample_shift=ISP_SHIFT_BIT1;
            }
            else if(input_len<((output_len<<ISP_SHIFT_BIT2)<<ISP_SHIFT_BIT2))
            {
                subsample_shift=ISP_SHIFT_BIT2;
            }
            else if(input_len<((output_len<<ISP_SHIFT_BIT2)<<ISP_SHIFT_BIT3))
            {
                subsample_shift=ISP_SHIFT_BIT3;
            }
            else if(input_len<((output_len<<ISP_SHIFT_BIT2)<<ISP_SHIFT_BIT4))
            {
                subsample_shift=ISP_SHIFT_BIT4;
            } 
        }	
    }

	align_value = ISP_FOUR << subsample_shift;

	return align_value;
}

/****************************************************************************************/
// Description: Get isp input height align value
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      input height
//						output hieht
//						data format
// output parameter:
//                      None
// return:
//          uint32: The value of align    
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetInputHeightAlign(uint32 input_height, uint32 output_height)
{
    return ISP_TWO;
}

/****************************************************************************************/
// Description: Get isp output width align value
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: The value of align 
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetOutputWidthAlign(ISP_DATA_FORMAT_E data_format)
{
    uint32 output_align_value=0x1;

    switch(data_format)
    {
        case ISP_DATA_RGB888:
        {
            output_align_value=ISP_ONE;			
            break;
        }
        case ISP_DATA_RGB565:        
        case ISP_DATA_YUV422:
        case ISP_DATA_YUV420:
        case ISP_DATA_YUV400:
        case ISP_DATA_YUV420_3FRAME:			
        {
            output_align_value=ISP_FOUR;
            break;
        }
        default:
            //SCI_TRACE_ISP:"ISP_SERVICE:ISP_ServiceGetOutputWidthAlign default %d line error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_9140_112_2_18_1_43_54_255,(uint8*)"d", __LINE__);
            break;       

    }

    return output_align_value;    
}

/****************************************************************************************/
// Description: Get isp output height align value
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: the value of align
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetOutputHeightAlign(ISP_DATA_FORMAT_E data_format)
{
    return ISP_TWO;   
}

/****************************************************************************************/
// Description: set the camera digital zoom max level
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: The camera digital zoom max level    
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceSetMaxZoomLevel(uint32 max_zoom_level)
{
    return _ISP_SetMaxZoomLevel(max_zoom_level);    
}

/****************************************************************************************/
// Description: set the mem type
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      uint32 mem_type: memory type
// output parameter:
//                      None
// return:
//          uint32: SCI_SUCCESS
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceSetMemType(uint32 mem_type)
{
    ISP_SetMemType(mem_type);

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: Get the camera digital zoom max level
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: The camera digital zoom max level    
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetMaxZoomLevel(void)
{
    return _ISP_GetMaxZoomLevel();
}

/****************************************************************************************/
// Description: Get the capture max width
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          ISP_SIZE_T: The capture max width    
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxCaptureWidth(void)
{   
    return ISP_GetMaxCaptureWidth();    
}

/****************************************************************************************/
// Description: Get the capture max height
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          ISP_SIZE_T: The capture max height    
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxCaptureHeight(void)
{   
    return ISP_GetMaxCaptureHeight();    
}

/****************************************************************************************/
// Description: Get the isp display format
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: The format of display    
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetDisplayFormat(void)
{    
    return ISP_GetDispDataFormat();    
}

/****************************************************************************************/
// Description: Get the max width of display
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: The width of display    
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxDisplayWidth(void)
{    
    return ISP_GetMaxDisplayWidth();    
}

/****************************************************************************************/
// Description: Get the capture max height
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          ISP_SIZE_T: The capture max height    
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxDisplayHeight(void)
{
    return ISP_GetMaxDisplayHeight();
}

/****************************************************************************************/
// Description: Get the scale max height
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          ISP_SIZE_T: The max width of scale
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxScaleWidth(void)
{    
    return ISP_GetMaxScaleWidth();    
}

/****************************************************************************************/
// Description: Get the scale max height
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          ISP_SIZE_T: The max height of scale
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxScaleHeight(void)
{    
    return ISP_GetMaxScaleHeight();    
}

/****************************************************************************************/
// Description: Get the slice scale max height
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          ISP_SIZE_T: The max width of slice scale
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxSliceScaleWidth(void)
{    
    return ISP_GetMaxSliceScaleWidth();    
}

/****************************************************************************************/
// Description: Get the slice scale max height
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          ISP_SIZE_T: The max height of slice scale
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetMaxSliceScaleHeight(void)
{    
    return ISP_GetMaxSliceScaleHeight();    
}

/****************************************************************************************/
// Description: Get the slice scale line width
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          ISP_SIZE_T: The max height of slice scale
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetSliceScaleLineWidth(uint16 src_width)
{    
    return ISP_GetSliceScaleLineWidth(src_width);    
}

/****************************************************************************************/
// Description: Get the get max scale up coeff
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: scale up coeff  
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetMaxScaleUpCoeff(void)
{   
    return ISP_GetMaxScaleUpCoeff();    
}

/****************************************************************************************/
// Description: Get the get max scale down coeff
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: scale down coeff
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetMaxScaleDownCoeff(void)
{   
    return ISP_GetMaxScaleDownCoeff();    
}

/****************************************************************************************/
// Description: Get the get max preview scale up coeff
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: scale up coeff
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetMaxPreviewScaleUpCoeff(void)
{   
    return ISP_GetMaxPreviewScaleUpCoeff();    
}

/****************************************************************************************/
// Description: Get the get max scale level
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: the level of scale
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetMaxScaleLevel(void)
{   
    return ISP_SCALE_LEVEL;
}

/****************************************************************************************/
// Description: is support the update cache
// Global resource dependence: 
// Author:             Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          TRUE: 		supported
//			FALSE:		unsupported
// Note:
/*****************************************************************************************/
PUBLIC uint16 ISP_ServiceGetScaleMatchHeight(uint16 input_height, uint16 output_height)
{
    uint16 match_coeff=ISP_SCALE_LEVEL;
    uint16 match_height=output_height;

    if((output_height<input_height)
        &&(0x00!=output_height)
        &&(0x00!=input_height))
    {
        match_coeff=(output_height*ISP_SCALE_LEVEL)/input_height;
        match_height=input_height*match_coeff/ISP_SCALE_LEVEL;
    }
    
    return match_height;
}

/****************************************************************************************/
// Description: Get align factor of slice scaling height
// Global resource dependence: 
// Author: 
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          uint32: the shift value of align factor   
// Note:
/*****************************************************************************************/
PUBLIC uint32 ISP_ServiceGetScaleSliceHeightAlign(uint32 input_width, uint32 output_width,
												uint32 input_height, uint32 output_height)
{
    uint32 subsample_shift=0x00;
	uint32 input_len = 0;
	uint32 output_len = 0;
	uint32 scaling_shift = 0;

	if (output_width * input_height < output_height * input_width)
	{
		input_len = input_width;
		output_len = output_width;
	}
	else
	{
		input_len = input_height;
		output_len = output_height;
	}
	
    if(SCI_TRUE==ISP_IsSupportSubsample())
    {
        if((output_len*ISP_SCALE_4X)<input_len)
        {
            if(input_len<((output_len<<ISP_SHIFT_BIT2)<<ISP_SHIFT_BIT1))
            {
                subsample_shift=ISP_SHIFT_BIT1;
            }
            else if(input_len<((output_len<<ISP_SHIFT_BIT2)<<ISP_SHIFT_BIT2))
            {
                subsample_shift=ISP_SHIFT_BIT2;
            }
            else if(input_len<((output_len<<ISP_SHIFT_BIT2)<<ISP_SHIFT_BIT3))
            {
                subsample_shift=ISP_SHIFT_BIT3;
            }
            else if(input_len<((output_len<<ISP_SHIFT_BIT2)<<ISP_SHIFT_BIT4))
            {
                subsample_shift=ISP_SHIFT_BIT4;
            } 
        }	
    }

	input_len >>= subsample_shift;

	/*makesure that output one line at least per slice*/
	while (input_len > output_len)
	{
		scaling_shift++;
		input_len >>= scaling_shift;
	}

	return (scaling_shift + subsample_shift);
}

/****************************************************************************************/
// Description: Check if the scaling is supported
// Global resource dependence: 
// Author:
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          TRUE: 		supported
//			FALSE:		unsupported
// Note:
/*****************************************************************************************/
PUBLIC BOOLEAN ISP_ServiceIsScaleSupported(ISP_SCALE_PARAM_T * param_ptr)
{   
	uint32 slice_height_algin = 0;

    //SCI_TRACE_ISP:"ISP_SERVICE: _ISP_CheckScaleParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_9526_112_2_18_1_43_55_256,(uint8*)"");

	slice_height_algin = ISP_ServiceGetScaleSliceHeightAlign(param_ptr->input_rect.w,
														param_ptr->output_size.w,
														param_ptr->input_rect.h,
														param_ptr->output_size.h);
	//SCI_TRACE_ISP:"ISP_SERVICE: slice_height_algin = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_9532_112_2_18_1_43_55_257,(uint8*)"d", slice_height_algin);
	

	if (((param_ptr->input_rect.w << 10) / param_ptr->output_size.w) > (ISP_SCALE_LEVEL << 10))
	{
		//SCI_TRACE_ISP:"ISP_SERVICE: scaling factor can not support"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_9537_112_2_18_1_43_55_258,(uint8*)"");
		return FALSE;
	}

	if (((param_ptr->input_rect.h << 10) / param_ptr->output_size.h) > (ISP_SCALE_LEVEL << 10))
	{
		//SCI_TRACE_ISP:"ISP_SERVICE: scaling factor can not support"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_9543_112_2_18_1_43_55_259,(uint8*)"");
		return FALSE;
	}	

	if ((ISP_SERVICE_SCALE_SLICE == param_ptr->scale_mode) && (!param_ptr->is_last)		
		&& (0 != (param_ptr->slice_line & ((1 << slice_height_algin) - 1))))
	{	
		//SCI_TRACE_ISP:"ISP_SERVICE: slice height is not aligned, %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_9550_112_2_18_1_43_55_260,(uint8*)"d", param_ptr->slice_line);
		return FALSE;
	}
	
    if((param_ptr->input_size.w < (param_ptr->input_rect.x + param_ptr->input_rect.w)) ||
        (param_ptr->input_size.h < (param_ptr->input_rect.y + param_ptr->input_rect.h)))
    {
		return FALSE;
    }

    if((param_ptr->input_rect.x&ISP_INPTU_REVIEW_ALIGN)||(param_ptr->input_rect.w&ISP_INPTU_REVIEW_ALIGN))
    {
        return FALSE;
    }

    if(ISP_DATA_RGB565==param_ptr->output_format)
    {
        if(param_ptr->output_size.w&ISP_OUTPUT_RGB_SCALE_ALIGN)
        {
            return FALSE;
        } 
    }
    else
    {
        if(param_ptr->output_size.w&ISP_OUTPUT_SCALE_ALIGN)
        {
            return FALSE;
        } 
    }     

    if((param_ptr->input_rect.w*ISP_GetMaxScaleUpCoeff()<param_ptr->output_size.w)
        ||(param_ptr->input_rect.h*ISP_GetMaxScaleUpCoeff()<param_ptr->output_size.h)
        ||(param_ptr->input_rect.w>param_ptr->output_size.w*ISP_GetMaxScaleDownCoeff()*ISP_GetMaxSubSampleMul())
        ||(param_ptr->input_rect.h>param_ptr->output_size.h*ISP_GetMaxScaleDownCoeff()*ISP_GetMaxSubSampleMul()))
    {
        return FALSE;
    }

    if((ISP_GetMaxScaleWidth()<param_ptr->output_size.w)&&(ISP_SERVICE_SCALE_SLICE!=param_ptr->scale_mode))
    {
        return FALSE;
    }

    if((ISP_SERVICE_SCALE_SLICE==param_ptr->scale_mode)&&((PNULL==param_ptr->get_scale_data)))
    {
        return FALSE;
    } 


    if(NULL==param_ptr->input_addr.y_addr)
    {
        return FALSE;
    }
     
    if(PNULL==param_ptr->get_scale_data)
    {
        return FALSE;
    }   

    return TRUE;
}

/****************************************************************************************/
// Description: is support the update cache
// Global resource dependence: 
// Author:             Tim.zhu
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          TRUE: 		supported
//			FALSE:		unsupported
// Note:
/*****************************************************************************************/
PUBLIC BOOLEAN ISP_ServiceIsSupportedUpdateCache(void)
{
    return ISP_IsSupportUpdateCache();
}

/****************************************************************************************/
// Description: registe the isp mpeg background callback
// Global resource dependence: 
// Author:             Xi.Zhang
// input parameter:
//                      None
// output parameter:
//                      None
// return:
//          TRUE: 		supported
//			FALSE:		unsupported
// Note:
/*****************************************************************************************/
PUBLIC void ISP_Regist_Mpeg_Display_Callback(ISP_MPG_DISPLAY_CALLBACK callback_func)
{
    s_isp_mpeg_display_callback = callback_func;
}
/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                             					        *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

//end of sc6600l_isp_service.c
