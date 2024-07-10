/******************************************************************************
 ** File Name:	dc_app.c                                               	  
 ** Author:		Frank.Yang		                                          
 ** DATE:		7/7/2008                                               
 ** Copyright:		2008 Spreatrum, Incoporated. All Rights Reserved.         
 ** Description:                                                              
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 ** ------------------------------------------------------------------------- 
 ** DATE              NAME             DESCRIPTION                            
 ******************************************************************************/

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_ref_dc_trc.h"
#include "sci_api.h"
#include "os_api.h"
#include "dc_app_6x00.h"
#include "sensor_drv.h"
#include "dc_framemgr.h"
#include "jpeg_interface_ex.h"
#include "dc_bufmgr.h"
#include "isp_service.h"

#include "dc_preview.h"
#include "dc_snapshot.h"
#include "dc_review.h"
#include "dc_misc.h"
#include "dc_param_ctrl.h"
#include "deep_sleep.h"
#include "sensor_drv.h"
#include "dc_codec.h"
#include "dal_lcd.h"
#include "display.h"
#include "lcdc_types.h"
#include "dc_cfg.h"

#ifdef MET_MEX_SUPPORT
#include "mex_ref.h"
#endif
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#define CLEAR_DCAM_CONTEXT_FLAGS(pContext) \
do{\
    pContext->zoom_level	= 0;\
    pContext->is_sensor_init = FALSE;\
    pContext->is_zoom_busy = FALSE;\
    pContext->is_sensor_open = FALSE;\
    pContext->is_smooth_zoom = FALSE;\
    pContext->is_burst_snapshot = FALSE;\
    pContext->is_interpolation_needed = FALSE;\
}while(0)

#define GET_LCD_W_H(pContext) \
do{\
    LCD_INFO_T lcd_info={0};\
    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);\
    pContext->lcd_width     = lcd_info.lcd_width;\
    pContext->lcd_height    = lcd_info.lcd_height;\
}while(0)

#define INIT_CONTEXT(pContext) \
do{\
    pContext->is_open = TRUE;\
    CLEAR_DCAM_CONTEXT_FLAGS(pContext);\
    SET_DCAM_STATE(DC_STATE_IDLE);\
    GET_LCD_W_H(pContext);\
}while(0)

DCAMERA_PREVIEW_PARAM_T *preview_param_ptr =  PNULL;

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*--------------------------- Global Data -----------------------------------*/
DCAM_CONTEXT_T g_dcam_context;
LOCAL BOOLEAN s_dc_is_capture_quick_view=SCI_FALSE;
LOCAL DCAMERA_CALL_BACK s_dc_mpeg_display_callback = PNULL;

/*--------------------------- External Data ---------------------------------*/
extern void Sensor_SelectSensorFormat(SENSOR_IMAGE_FORMAT image_format);
extern PUBLIC uint32 DC_AtvRssiHandler(void);

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
PUBLIC DCAM_CONTEXT_T* _GetDCAMContext(void)
{
	return (DCAM_CONTEXT_T*)&g_dcam_context;

}
/*lint -save -e765 */
/******************************************************************************/
// Description: 
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     none
// Note:       
/******************************************************************************/  
LOCAL  int _mjpeg_encode_callback( ISP_DATA_ADDR_T yyuv_buf,uint32 width,uint32 height)
{
    int nRet = DCAMERA_OP_SUCCESS;
    DC_FRAME_T *p_frame = PNULL;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();
    uint32 dc_frame_size=0x00;

    if(pContext->record_type == DCAM_RECORD_YUV)
    {
        if(pContext->mjpeg_record_callback)
        {
            dc_frame_size=width*height*2;
            p_frame=DC_FrameMgr_CreateAFrame(dc_frame_size);

            if (p_frame!=NULL)
            {
                p_frame->p_head=p_frame->sections[0].pbuf;
                if(NULL!=p_frame->p_head)
                {
                    SCI_MEMCPY(p_frame->p_head, (void *)yyuv_buf.y_addr, width *height);
                }
                
                p_frame->p_tail=p_frame->p_head + width * height;
                if(NULL!=p_frame->p_tail)
                {
                    SCI_MEMCPY(p_frame->p_tail, (void *)yyuv_buf.uv_addr, width * height);
                }

                pContext->i_frame++;
                (*pContext->mjpeg_record_callback)(pContext->i_frame, p_frame);
            }
        }
        
    }
    else
    {
    	JINF_ENC_IN_PARAM_T jpeg_in;
    	JINF_ENC_OUT_PARAM_T jpeg_out;
        
        SCI_MEMSET(&jpeg_in, 0x0, sizeof(JINF_ENC_IN_PARAM_T));
        SCI_MEMSET(&jpeg_out, 0x0, sizeof(JINF_ENC_OUT_PARAM_T));

        dc_frame_size=(width*height*2)/5;
        p_frame=DC_FrameMgr_CreateAFrame(dc_frame_size);
        if(NULL==p_frame)
        {
            //DC_LOG_TRACE:"DC_FrameMgr_CreateAFrame() failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_152_112_2_18_1_23_11_0,(uint8*)"");
            return SCI_ERROR;
        }
        //encoding jpeg in frame mode 
        jpeg_in.yuv_info.yuv_chn.y_chn_ptr=(uint8 *)yyuv_buf.y_addr;
        jpeg_in.yuv_info.yuv_chn.u_chn_ptr=(uint8 *)yyuv_buf.uv_addr;
        jpeg_in.yuv_info.sample_format=JINF_FORMAT_YUV422;
        jpeg_in.yuv_info.data_pattern=JINF_YUV_PATTERN_CHN_Y_UV;
        jpeg_in.yuv_info.data_endian=JINF_DATA_BIG_ENDIAN;

        jpeg_in.src_width=width;
        jpeg_in.src_height=height;
        jpeg_in.quality=JINF_QUALITY_MIDDLE_LOW;
        jpeg_in.encode_buf_ptr=pContext->jpeg_fw_ptr;
        jpeg_in.encode_buf_size=pContext->jpeg_fw_len;
        jpeg_in.target_buf_ptr=p_frame->sections[0].pbuf;
        jpeg_in.target_buf_size=p_frame->sections[0].dw_len;
        jpeg_in.read_yuv_func=PNULL;
        jpeg_in.write_file_func=PNULL;
        jpeg_in.async_callback=PNULL;

        if(JINF_SUCCESS!=IMGJPEG_EncodeEx(&jpeg_in, &jpeg_out))
        {
            nRet=DCAMERA_OP_ENCODE_ERR;
            p_frame->sections[0].pbuf=PNULL; //notice dv recorder some error occured
            //DCAM_PRINT:"_EncodeYYUVToJpeg(),jpeg encoding err,line %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_177_112_2_18_1_23_11_1,(uint8*)"d",__LINE__);
        }
        else
        {
            p_frame->sections[0].dw_len=jpeg_out.jpeg_buf_size;
        }

        pContext->target_width=jpeg_out.output_width;
        pContext->target_height=jpeg_out.output_height;
        pContext->jpeg_buf_ptr=jpeg_out.jpeg_buf_ptr;
        pContext->jpeg_size=jpeg_out.jpeg_buf_size;

        pContext->i_frame++;
        if(DC_IN_STATE(DC_STATE_UPCC_REC))
        {
            if(pContext->upcc_param.record_callback)
            {
                (*pContext->upcc_param.record_callback)(DCAMERA_OP_SUCCESS,pContext->jpeg_buf_ptr,pContext->jpeg_size);
            }
            else
            {
                //DC_LOG_TRACE:"DCAM:_mjpeg_encode_callback, record_callback is null err"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_198_112_2_18_1_23_11_2,(uint8*)"");
            }
            
            DC_FreeFrame(p_frame);
        }
        else
        {
            if(pContext->mjpeg_record_callback)
            {
                (*pContext->mjpeg_record_callback)(pContext->i_frame, p_frame);
            }
        }
    }

    return nRet;
}

/******************************************************************************/
// Description: 
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     none
// Note:       
/******************************************************************************/
LOCAL  int _mjpeg_notice_callback(void * buf)
{
	int nRet = DCAMERA_OP_SUCCESS;
	//DCAM_CONTEXT_T *pContext = _GetDCAMContext();

	return nRet;
}

/******************************************************************************/
// Description: 
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     none
// Note:       
/******************************************************************************/  
LOCAL BOOLEAN _IsDCamPreviewing(void)
{
	return (DC_STATE_PREVIEWING == g_dcam_context.state);
}

/******************************************************************************/
// Description: 
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     none
// Note:       
/******************************************************************************/  
LOCAL int _CheckMjpegRecParam(DC_MJPG_PARAM_T* mjpeg_ptr)
{
    int nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T *pContext = (DCAM_CONTEXT_T*)&g_dcam_context;
    uint16 isp_width_max=0;
    uint16 isp_height_max=0;

    isp_width_max=ISP_ServiceGetMaxScaleWidth();
    isp_height_max=ISP_ServiceGetMaxScaleHeight();
        
    //get isp_width_max & isp_height_max provided by ISP_service
    if((mjpeg_ptr->w_target_width>isp_width_max)||(mjpeg_ptr->w_target_height>isp_height_max))
    {
        //DC_LOG_TRACE:"DCAM:_CheckMjpegRecParam target size:%d,%d err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_259_112_2_18_1_23_11_3,(uint8*)"dd",mjpeg_ptr->w_target_width ,mjpeg_ptr->w_target_height);
        nRet=DCAMERA_OP_PARAM_ERR;
    }

    /* disp size should be smaller than lcd size */
    if((mjpeg_ptr->st_disp_rect.w > pContext->lcd_width) 
        ||(mjpeg_ptr->st_disp_rect.h > pContext->lcd_height))
    {
        //DC_LOG_TRACE:"DCAM:_CheckMjpegRecParam disply size:%d,%d err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_267_112_2_18_1_23_11_4,(uint8*)"dd",mjpeg_ptr->st_disp_rect.w ,mjpeg_ptr->st_disp_rect.h);
        nRet=DCAMERA_OP_PARAM_ERR;
    }

    /* jpeg target size must be 16pixel align at X dimension and be 8pixel align at Y dimension. */
    mjpeg_ptr->w_target_width=DC_LEFT_TRUNK16(mjpeg_ptr->w_target_width);
    mjpeg_ptr->w_target_height=DC_LEFT_TRUNK8(mjpeg_ptr->w_target_height);

    return nRet;		
}

/******************************************************************************/
// Description: 
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     none
// Note:       
/******************************************************************************/  
LOCAL int _SetMjpegRecParam(ISP_MJPEG_PARAM_T* isp_mjpeg_ptr)
{
    int nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();
    SENSOR_EXP_INFO_T_PTR sensor_info_ptr=pContext->sensor_info_ptr;
    DCAMERA_RECT_T src_rect={0x00};
    DCAMERA_IMG_SENSOR_TYPE_E sensor_type=DCAMERA_IMG_SENSOR_TYPE_MAX;
    DCAMERA_DISP_MODE_E disp_mode=DCAMERA_DISP_FULLSCREEN;
    ISP_SENSOR_SIGNAL_INFO_T isp_sensor_singnal={0};

    pContext->target_width=pContext->mjpeg_param.w_target_width;
    pContext->target_height=pContext->mjpeg_param.w_target_height;	

    // capture yuv for display and encoder
    if((NULL==pContext->isp_capture_ptr)
        ||(NULL==pContext->isp_capture_len))
    {
        //DC_LOG_TRACE:"DCAM:_SetMjpegRecParam caputre buf is null err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_301_112_2_18_1_23_11_5,(uint8*)"");
        return DCAMERA_OP_NO_ENOUGH_MEMORY;
    }
    isp_mjpeg_ptr->capture_addr=(uint32)pContext->isp_capture_ptr;
    isp_mjpeg_ptr->capture_buf_size=pContext->isp_capture_len;

    //display buffer
    if((NULL==pContext->disp_buf_ptr)
        ||(NULL==pContext->disp_buf_len))
    {
        //DC_LOG_TRACE:"DCAM:_SetMjpegRecParam display buf is null err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_311_112_2_18_1_23_11_6,(uint8*)"");
        return DCAMERA_OP_NO_ENOUGH_MEMORY;
    }
    isp_mjpeg_ptr->disp_addr=(uint32)pContext->disp_buf_ptr;
    isp_mjpeg_ptr->disp_buf_size=pContext->disp_buf_len;

    // scale capture yuv for encoder 
    if((NULL==pContext->yyuv_buf.y_addr)
        ||(NULL==pContext->yyuv_buf_len))
    {
        //DC_LOG_TRACE:"DCAM:_SetMjpegRecParam scale buf is null err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_321_112_2_18_1_23_11_7,(uint8*)"");
        return DCAMERA_OP_NO_ENOUGH_MEMORY;
    }
    isp_mjpeg_ptr->scale_addr=pContext->yyuv_buf.y_addr;
    isp_mjpeg_ptr->scale_buf_size=pContext->yyuv_buf_len;

    // rotation for display 
    if((NULL==pContext->rot_buf_ptr)
        ||(NULL==pContext->rot_buf_len))
    {
        //DC_LOG_TRACE:"DCAM:_SetMjpegRecParam rotation buf is null err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_331_112_2_18_1_23_11_8,(uint8*)"");
    }
    else
    {
        isp_mjpeg_ptr->rot_addr.y_addr=(uint32)pContext->rot_buf_ptr;
        isp_mjpeg_ptr->rot_addr.uv_addr=isp_mjpeg_ptr->rot_addr.y_addr+(pContext->rot_buf_len>>0x01);
        isp_mjpeg_ptr->rot_addr.uv_addr=DC_Algin(isp_mjpeg_ptr->rot_addr.uv_addr, 0xff, 0x08);
        isp_mjpeg_ptr->rot_buf_len.y_len=pContext->rot_buf_len;
        isp_mjpeg_ptr->rot_buf_len.uv_len=pContext->rot_buf_len>>0x01;
    }

    DC_GetImgSensorSignalInfo(SNAPSHOT_MODE,&isp_sensor_singnal);
    ISP_ServiceSetSensorSignalInfo(&isp_sensor_singnal);

    isp_mjpeg_ptr->pclk=sensor_info_ptr->sensor_mode_info[pContext->preview_mode].pclk;

    isp_mjpeg_ptr->input_size.w=sensor_info_ptr->sensor_mode_info[pContext->preview_mode].width;
    isp_mjpeg_ptr->input_size.h=sensor_info_ptr->sensor_mode_info[pContext->preview_mode].height;

    DCAMERA_GetInfo(DCAMERA_INFO_SENSOR_TYPE, &sensor_type);

    if(DCAMERA_IMG_SENSOR_TYPE_3P4==sensor_type)
    {
        isp_mjpeg_ptr->capture_size.w=240;
        isp_mjpeg_ptr->capture_size.h=320;
    }
    else if(DCAMERA_IMG_SENSOR_TYPE_EQ==sensor_type)
    {
        isp_mjpeg_ptr->capture_size.w=240;
        isp_mjpeg_ptr->capture_size.h=240;
    }
    else
    {
        isp_mjpeg_ptr->capture_size.w=320;
        isp_mjpeg_ptr->capture_size.h=240;
    }

    pContext->trim_rect.x=sensor_info_ptr->sensor_mode_info[pContext->preview_mode].trim_start_x;
    pContext->trim_rect.y=sensor_info_ptr->sensor_mode_info[pContext->preview_mode].trim_start_y;
    pContext->trim_rect.w=sensor_info_ptr->sensor_mode_info[pContext->preview_mode].trim_width;
    pContext->trim_rect.h=sensor_info_ptr->sensor_mode_info[pContext->preview_mode].trim_height;

    if(SCI_TRUE!=ISP_ServiceGetAtvIsEnable())
    {
        src_rect.x=0x00;
        src_rect.y=0x00;
        src_rect.w=isp_mjpeg_ptr->capture_size.w;
        src_rect.h=isp_mjpeg_ptr->capture_size.h;  

        DC_AdjRect_OnDistortion(&pContext->trim_rect, &src_rect, &src_rect, DCAMERA_DISP_FULLSCREEN, DCAMERA_ROTATION_0);
    }
    
    isp_mjpeg_ptr->input_rect=*(ISP_RECT_T*)&pContext->trim_rect;

    isp_mjpeg_ptr->jpeg_target_size.w=pContext->mjpeg_param.w_target_width;
    isp_mjpeg_ptr->jpeg_target_size.h=pContext->mjpeg_param.w_target_height;

    isp_mjpeg_ptr->jpeg_target_rect.x=0;
    isp_mjpeg_ptr->jpeg_target_rect.y=0;
    isp_mjpeg_ptr->jpeg_target_rect.w=isp_mjpeg_ptr->capture_size.w;
    isp_mjpeg_ptr->jpeg_target_rect.h=isp_mjpeg_ptr->capture_size.h;

    src_rect.x=0;
    src_rect.y=0;
    src_rect.w=isp_mjpeg_ptr->jpeg_target_size.w;
    src_rect.h=isp_mjpeg_ptr->jpeg_target_size.h;

    DC_AdjRect_OnDistortion((DCAMERA_RECT_T*)&(isp_mjpeg_ptr->jpeg_target_rect), 
                            &src_rect, 
                            &src_rect, 
                            DCAMERA_DISP_FULLSCREEN, 
                            DCAMERA_ROTATION_0);

    isp_mjpeg_ptr->target_src_rect.x=0;
    isp_mjpeg_ptr->target_src_rect.y=0;
    isp_mjpeg_ptr->target_src_rect.w=isp_mjpeg_ptr->capture_size.w;
    isp_mjpeg_ptr->target_src_rect.h=isp_mjpeg_ptr->capture_size.h;

    isp_mjpeg_ptr->target_rect.x=pContext->mjpeg_param.target_rect.x;
    isp_mjpeg_ptr->target_rect.y=pContext->mjpeg_param.target_rect.y;
    isp_mjpeg_ptr->target_rect.w=pContext->mjpeg_param.target_rect.w;
    isp_mjpeg_ptr->target_rect.h=pContext->mjpeg_param.target_rect.h;

    isp_mjpeg_ptr->disp_rect.x=pContext->mjpeg_param.st_disp_rect.x;
    isp_mjpeg_ptr->disp_rect.y=pContext->mjpeg_param.st_disp_rect.y;
    isp_mjpeg_ptr->disp_rect.w=pContext->mjpeg_param.st_disp_rect.w;
    isp_mjpeg_ptr->disp_rect.h=pContext->mjpeg_param.st_disp_rect.h;

    if(SCI_TRUE!=ISP_ServiceGetAtvIsEnable())
    {
        disp_mode=pContext->preview_param.disp_mode;
    }
    else
    {
        disp_mode=DCAMERA_DISP_CUSTOMIZE;
    }

    DC_AdjRect_OnDistortion((DCAMERA_RECT_T*)&(isp_mjpeg_ptr->target_src_rect), 
                            (DCAMERA_RECT_T*)&(isp_mjpeg_ptr->disp_rect), 
                            (DCAMERA_RECT_T*)&(isp_mjpeg_ptr->target_rect), 
                            disp_mode, 
                            pContext->preview_param.rotation_mode);

    isp_mjpeg_ptr->lcd_id=pContext->preview_param.lcd_id;
    isp_mjpeg_ptr->lcd_block_id=pContext->preview_param.lcd_block_id;
    isp_mjpeg_ptr->is_pause=0;

    isp_mjpeg_ptr->get_cap_data = _mjpeg_encode_callback;

    //fetch preivewing rotation here
    isp_mjpeg_ptr->disp_rotation = CONVERT_ROTAION_MODE(pContext->preview_param.rotation_mode);

    if(DC_IN_STATE(DC_STATE_UPCC_REC))
    {
        isp_mjpeg_ptr->is_disp=0;
    }
    else
    {
        isp_mjpeg_ptr->is_disp=1;
        isp_mjpeg_ptr->mode=ISP_SERVICE_INVALIDATE_ASYNC;
        isp_mjpeg_ptr->is_copy_display_data=TRUE;
    }

    return nRet;	
}

/******************************************************************************/
// Description: 
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     none
// Note:       
/******************************************************************************/  
LOCAL int _SetMjpegDecParam(ISP_REVIEW_PARAM_T* isp_review_ptr, 
											DISPLAY_DATA_FRAME_T* p_frame)
{
    int	nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T*	pContext=_GetDCAMContext();

    if((NULL==p_frame->y_addr)
        ||(NULL==p_frame->uv_addr))
    {
        //DC_LOG_TRACE:"DCAM:_SetMjpegDecParam y/uv buf is null err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_472_112_2_18_1_23_11_9,(uint8*)"");
        return DCAMERA_OP_NO_ENOUGH_MEMORY;
    }

    isp_review_ptr->input_size.w=pContext->target_width;
    isp_review_ptr->input_size.h=pContext->target_height;
    isp_review_ptr->input_rect.x=pContext->mjpeg_review_param.st_trim_rect.x;
    isp_review_ptr->input_rect.y=pContext->mjpeg_review_param.st_trim_rect.y;
    isp_review_ptr->input_rect.w=pContext->mjpeg_review_param.st_trim_rect.w;
    isp_review_ptr->input_rect.h=pContext->mjpeg_review_param.st_trim_rect.h;

    if(JINF_FORMAT_YUV422==p_frame->yuv_type)
    {
        isp_review_ptr->input_format=ISP_DATA_YUV422;
    }
    else if(JINF_FORMAT_YUV420==p_frame->yuv_type)
    {
        if(YUV_PLANE_2 == p_frame->yuv_plane)
        {
            isp_review_ptr->input_format=ISP_DATA_YUV420;
        }
        else
        {
            isp_review_ptr->input_format=ISP_DATA_YUV420_3FRAME;
        }
    }
    else if(JINF_FORMAT_YUV400==p_frame->yuv_type)
    {
        isp_review_ptr->input_format=ISP_DATA_YUV400;
    }
    else
    {
        isp_review_ptr->input_format=p_frame->yuv_type;
    }
  
    isp_review_ptr->src_addr.y_addr=(uint32)p_frame->y_addr;     
    isp_review_ptr->src_addr.uv_addr=(uint32)p_frame->uv_addr;

    isp_review_ptr->src_len.y_len=p_frame->y_data_len;
    isp_review_ptr->src_len.uv_len=p_frame->uv_data_len;

    isp_review_ptr->src_addr.v_addr=(uint32)p_frame->uv1_addr;
    isp_review_ptr->src_len.v_len=p_frame->uv1_data_len;

    isp_review_ptr->disp_addr=DC_Algin(((uint32)pContext->disp_buf_ptr), 0xff, 0x08);
    isp_review_ptr->disp_buf_size=pContext->disp_buf_len;

    isp_review_ptr->rot_addr.y_addr=DC_Algin(((uint32)pContext->rot_buf_ptr), 0xff, 0x08);
    isp_review_ptr->rot_buf_len.y_len=pContext->mjpeg_review_param.st_disp_rect.w*pContext->mjpeg_review_param.st_disp_rect.h;
    isp_review_ptr->rot_addr.uv_addr=isp_review_ptr->rot_addr.y_addr+DC_Algin(isp_review_ptr->rot_buf_len.y_len, 0xff, 0x08);
    isp_review_ptr->rot_buf_len.uv_len=isp_review_ptr->rot_buf_len.y_len;

    isp_review_ptr->disp_format=ISP_DATA_RGB565;
    isp_review_ptr->disp_lcd_id=pContext->mjpeg_review_param.i_lcd;
    isp_review_ptr->disp_block_id=0;
    
    //display rect
    isp_review_ptr->disp_rect.x=pContext->mjpeg_review_param.st_disp_rect.x;
    isp_review_ptr->disp_rect.y=pContext->mjpeg_review_param.st_disp_rect.y;
    isp_review_ptr->disp_rect.w=pContext->mjpeg_review_param.st_disp_rect.w;
    
    if(((pContext->mjpeg_review_param.st_disp_rect.w==pContext->lcd_width)&&(pContext->mjpeg_review_param.st_disp_rect.h  == pContext->lcd_height))
        ||((pContext->mjpeg_review_param.st_disp_rect.h==pContext->lcd_width)&&(pContext->mjpeg_review_param.st_disp_rect.w  == pContext->lcd_height)))
    {
        isp_review_ptr->disp_rect.h=pContext->mjpeg_review_param.st_disp_rect.h/2*2;
    }
    else
    {
        isp_review_ptr->disp_rect.h=pContext->mjpeg_review_param.st_disp_rect.h/2*2;
    }

    isp_review_ptr->target_rect.x=pContext->mjpeg_review_param.target_rect.x;
    isp_review_ptr->target_rect.y=pContext->mjpeg_review_param.target_rect.y;
    isp_review_ptr->target_rect.w=pContext->mjpeg_review_param.target_rect.w;
    isp_review_ptr->target_rect.h=pContext->mjpeg_review_param.target_rect.h;
    
    isp_review_ptr->disp_rotation=CONVERT_ROTAION_MODE(pContext->mjpeg_review_param.rotation);

    isp_review_ptr->mode = ISP_SERVICE_INVALIDATE_ASYNC;
    isp_review_ptr->is_copy_display_data = TRUE;

    return nRet;
}

/******************************************************************************/
// Description: 
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     none
// Note:       
/******************************************************************************/  
void _DCAMTimeOutHandler(uint32 id)
{
	DC_SendMsg(DCAM_MSG_OVERTIME, 0);
}
	
/******************************************************************************/
// Description:	get DCAM Ref buffer ,create DCAM timer
// Author:     		Frank.yang
// Input:			pContext--DCAM context pointer
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		none
/******************************************************************************/
LOCAL int	_DCAMRscInit(DCAM_CONTEXT_T *pContext)
{
	int nRet = DCAMERA_OP_SUCCESS;

        preview_param_ptr =  ( DCAMERA_PREVIEW_PARAM_T *)SCI_ALLOCA(sizeof(DCAMERA_PREVIEW_PARAM_T));
        if(PNULL == preview_param_ptr)
        {
            return DCAMERA_OP_PARAM_ERR;
        }

	return nRet;
}

/******************************************************************************/
// Description: 
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     
// Note:       
/******************************************************************************/  
LOCAL int _DCAMRscFree(DCAM_CONTEXT_T *pContext)
{
	int nRet = DCAMERA_OP_SUCCESS;
      	MM_Exit();

       if(PNULL != preview_param_ptr)
       {
            SCI_FREE(preview_param_ptr);
            preview_param_ptr = PNULL;
        }

	return nRet;
}


LOCAL  DCAMERA_ROTATION_MODE_E _DCAMERA_ROT_Convertor(DCAMERA_ROTATION_MODE_E src_rot, DCAMERA_ROTATION_MODE_E *img_rot_ptr )
{
    DCAMERA_ROTATION_MODE_E ret  = DCAMERA_ROTATION_0;

    //SCI_TRACE_LOW:"_DCAMERA_ROT_Convertor: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_612_112_2_18_1_23_12_10,(uint8*)"d", src_rot);

    *img_rot_ptr =  DCAMERA_ROTATION_0;
    
     switch(src_rot)
     {
     	
        case DCAMERA_ROTATION_180:
        case DCAMERA_ROTATION_MIRROR:       
        case DCAMERA_ROTATION_FLIP:
        case DCAMERA_ROTATION_0:
        {
        
            *img_rot_ptr = src_rot;
        }
        break; 
        
        case DCAMERA_ROTATION_270:

           *img_rot_ptr =   DCAMERA_ROTATION_180;
           ret  = DCAMERA_ROTATION_90;
        break;

        case DCAMERA_ROTATION_90:

        
            ret  = DCAMERA_ROTATION_90;

        break;
       
	default:

       break;
     
     }

    //SCI_TRACE_LOW:"_DCAMERA_ROT_Convertor: img: %d, ret: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_648_112_2_18_1_23_12_11,(uint8*)"dd", *img_rot_ptr, ret);

     return ret;
}

/******************************************************************************/
// Description: Get the id if currect img sensor
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     uint32:the id of img sensor
// Note:       
/******************************************************************************/  
void _BurstIntervalHandler(uint32 param)
{
	DC_SendMsg(DCAM_MSG_BURST_SNAPSHOT, 0);
}

/******************************************************************************/
// Description: init sensor
// Author:     jing.li
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E _DCAMERA_InitSensor(void)
{
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();

    Sensor_SelectSensorFormat(SENSOR_IMAGE_FORMAT_YUV422);

    if(!Sensor_IsInit())
    {
        if(!Sensor_Init())
        {
            //DC_LOG_TRACE:"DCAM:_DCAMERA_InitSensor img sensor init fail err!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_681_112_2_18_1_23_12_12,(uint8*)"");
            return DCAMERA_OP_NO_SENSOR_ERR;
        }
    }

    if(!Sensor_IsOpen())
    {
    	//SCI_TRACE_LOW:"_DCAMERA_InitSensor: before sensor_open, %d"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_688_112_2_18_1_23_12_13,(uint8*)"d", SCI_GetTickCount());
        if(DCAMERA_OP_SUCCESS!=Sensor_Open())
        {
            //DC_LOG_TRACE:"DCAM:_DCAMERA_InitSensor img sensor open fail err!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_691_112_2_18_1_23_12_14,(uint8*)"");
            return DCAMERA_OP_SENSOR_NOT_WORK_ERR;
        }
    	//SCI_TRACE_LOW:"_DCAMERA_InitSensor: after sensor_open, %d"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_694_112_2_18_1_23_12_15,(uint8*)"d", SCI_GetTickCount());
    }
    
    pContext->is_sensor_open = SCI_TRUE;
    pContext->sensor_info_ptr = Sensor_GetInfo();

    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************/
// Description:     Set analog tv enable
// Author:          Tim.zhu
// Input:           none
// Output:          none
// Return:          0           	successful
//                  others      failed
// Note:            before using DCAM device, be sure you have successful open it
/******************************************************************************/
PUBLIC uint32 DCAMERA_SetSensorType(DCAMERA_OP_MODE_E op_mode) 
{
    //SCI_TRACE_LOW:"DCAMERA_SetSensorType: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_712_112_2_18_1_23_12_16,(uint8*)"d", op_mode);
    
    if(DCAMERA_OP_NORMAL==op_mode || DCAMERA_OP_DV==op_mode)
    {
        ISP_ServiceSetAtvEnable(SCI_FALSE);
        Sensor_SetSensorType(SENSOR_TYPE_IMG_SENSOR);
    }
    else if(DCAMERA_OP_ATV==op_mode)
    {
        ISP_ServiceSetAtvEnable(SCI_TRUE);
        Sensor_SetSensorType(SENSOR_TYPE_ATV);
    }
    else
    {
        return DCAMERA_OP_ERROR;
    }
    
    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************/
// Description: open DCAM device
// Author:     jing.li
// Input:      none
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       before using DCAM device, be sure you have successful open it
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DC_Open(void) 
{
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();

    LCD_SetOpMode(MAIN_LCD_ID, OSD_OPERATION);
    LCD_SetOpMode(SUB_LCD_ID, OSD_OPERATION);
    LCD_SetInvalidateLimit(MAIN_LCD_ID, PERMIT_GUI_INVALIDATE);
    //DC_LOG_TRACE:"DCAM:DC_Open,time: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_746_112_2_18_1_23_12_17,(uint8*)"d", SCI_GetTickCount());

    SCI_DCAM_EnableDeepSleep(DISABLE_AHB_SLEEP);

    DC_RETURN_VAL_IF_FALSE_WARNING(!DCAMERA_IsOpen(), DCAMERA_OP_ERROR, ("DCAM:DC is already open"));

    INIT_CONTEXT(pContext);
    
    DC_RETURN_VAL_IF_FAIL_WARNING(_DCAMRscInit(pContext), DCAMERA_OP_ERROR, \
                                        ("DCAM:DC resource init error"));

    DC_RETURN_VAL_IF_FAIL_WARNING(DC_TaskOpen(DCAM_TASK_PRIORITY), DCAMERA_OP_ERROR, \
                                        ("DCAM:DC task open error"));

    //init ISP
    ISP_ServiceInit( );

    return DCAMERA_OP_SUCCESS;
}


/******************************************************************************/
// Description:     open DCAM device
// Author:          jing.li
// Input:           none
// Output:          none
// Return:          0           	successful
//                  others      failed
// Note:            before using DCAM device, be sure you have successful open it
/******************************************************************************/

//LOCAL uint8 *s_dc_display_buf_ptr = PNULL;

PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_Open(DCAMERA_OP_MODE_E op_mode) 
{
    DCAM_CONTEXT_T *pContext=_GetDCAMContext();
    DC_PRODUCT_CFG_T_PTR pDCProductCfg=DC_GeProductCfgPtr();
    DCAMERA_RETURN_VALUE_E ret=DCAMERA_OP_SUCCESS;
    LCD_INFO_T lcd_info = {0};

    /* check if dc is open */
    DC_RETURN_VAL_IF_FALSE_WARNING(!DCAMERA_IsOpen(), DCAMERA_OP_SUCCESS, \
                                        ("DCAM:DC is already open"));

    //SCI_TRACE_LOW:"DCAM: enter, mode %d, time %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_788_112_2_18_1_23_12_18,(uint8*)"dd", op_mode, SCI_GetTickCount());

    pContext->dcamera_mode=op_mode;
    ISP_ServiceSetMemType(pDCProductCfg->mem_type);    
    SCI_DCAM_EnableDeepSleep(DISABLE_AHB_SLEEP);

    LCD_GetInfo(LCD_ID_0, &lcd_info);
    if(lcd_info.is_use_fmark)
    {
        //SCI_TRACE_LOW:"---------DCAMERA_Open: USE fmark-------- "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_797_112_2_18_1_23_12_19,(uint8*)"");
        LCD_SetFmarkMode(LCD_ID_0,LCD_FMARK_DEVICE,(LCD_FMARK_POL_E)lcd_info.fmark_pol);
    }

    if((DCAMERA_OP_NORMAL==op_mode)
        ||(DCAMERA_OP_DV==op_mode)
        ||(DCAMERA_OP_ATV==op_mode))
    {
        DCAMERA_SetSensorType(op_mode);
    
        DC_RETURN_VAL_IF_FAIL_WARNING(_DCAMERA_InitSensor(), DCAMERA_OP_SENSOR_NOT_WORK_ERR, \
                                            ("DCAM: init sensor error at open"));
    }


    //protection for the isp_service operating
    if (SCI_SUCCESS != ISP_ServiceLock(ISP_SERVICE_OWNER_DC, DC_WAIT_TIME))
	{
		//DC_LOG_TRACE:"DCAM:DC lock isp error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_815_112_2_18_1_23_12_20,(uint8*)"");
		return DCAMERA_OP_ERROR;
	}


    ret = DC_Open();
	if (DCAMERA_OP_SUCCESS != ret)
	{
		ISP_ServiceUnlock(ISP_SERVICE_OWNER_DC);
	}	

    if(!ISP_ServiceGetAtvIsEnable())
    {
        LCD_SetInvalidateLimit(MAIN_LCD_ID, NO_PERMIT_GUI_INVALIDATE);
    }	
     //SCI_TRACE_LOW:"DCAM, end:  time %d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_830_112_2_18_1_23_12_21,(uint8*)"d", SCI_GetTickCount());

    return ret;
}

/******************************************************************************/
// Description:	close DCAM device
// Author:     		Frank.yang
// Input:			none
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		none
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_Close(void)
{
    DCAMERA_RETURN_VALUE_E eRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();
    LCD_INFO_T lcd_info = {0};


    //DC_LOG_TRACE:"DCAM:DCAMERA_Close start: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_849_112_2_18_1_23_12_22,(uint8*)"d", SCI_GetTickCount());

    LCD_GetInfo(LCD_ID_0,&lcd_info);

    if(lcd_info.is_use_fmark)
    {
        LCD_SetFmarkMode(LCD_ID_0,LCD_FMARK_NONE,(LCD_FMARK_POL_E)lcd_info.fmark_pol);
    }

    DC_RETURN_VAL_IF_FALSE_WARNING(DCAMERA_IsOpen(), 
                DCAMERA_OP_NOT_OPEN, ("DCAM:DC is NOT open."));

    pContext->is_open=FALSE;
    SET_DCAM_STATE(DC_STATE_UNKNOWN);


    DC_TaskClose();

    ISP_ServiceFeatureStop();
    ISP_ServiceClose();

    ISP_ServiceUnlock(ISP_SERVICE_OWNER_DC);

    LCD_DisableBlock(MAIN_LCD_ID, 0);

    _DCAMRscFree(pContext);

    Sensor_Close();
    
    if(ISP_ServiceGetAtvIsEnable())
    {
        ISP_ServiceSetAtvEnable(SCI_FALSE);
    }

    LCD_SetInvalidateLimit(MAIN_LCD_ID, PERMIT_GUI_INVALIDATE);

    LCD_SetOpMode(MAIN_LCD_ID, UI_OPERATION);
    LCD_SetOpMode(SUB_LCD_ID, UI_OPERATION);

    
    pContext->dcamera_mode = DCAMERA_OP_ERR;

    SCI_DCAM_EnableDeepSleep(ENABLE_AHB_SLEEP);
    
    //DC_LOG_TRACE:"DCAM:DCAMERA_Close end: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_893_112_2_18_1_23_12_23,(uint8*)"d", SCI_GetTickCount());


    return eRet;
}

  

/******************************************************************************/
// Description:     start preview
// Author:          Jing.Li
// Input:           param_ptr   point to the preview parameter
// Output:          none
// Return:          0           successful
//                  others      failed
// Note:            if return successful, the LCD will be on bridge mode
/******************************************************************************/

//LOCAL uint32 rot_enable = 1;
LOCAL volatile uint32 rot_mode = DCAMERA_ROTATION_MIRROR;
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StartPreview(
							DCAMERA_PREVIEW_PARAM_T const * param_ptr)
{
    int32 ops_ret = DCAMERA_OP_SUCCESS;
    PREVIEW_OPS_HANDLER_T* preview_handlers = DC_Preview_GetOpsHandler();
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();
    DCAMERA_ROTATION_MODE_E  disp_rot = param_ptr->rotation_mode;
    DCAMERA_ROTATION_MODE_E  img_rot = param_ptr->rotation_mode;

    DCAMERA_PREVIEW_PARAM_T *pre_param_ptr =  preview_param_ptr;//( DCAMERA_PREVIEW_PARAM_T *)SCI_ALLOCA(sizeof(DCAMERA_PREVIEW_PARAM_T));

    //DC_LOG_TRACE:"DCAM:DCAMERA_StartPreview, start time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_922_112_2_18_1_23_12_24,(uint8*)"d", CURRENT_TIME);

    if(NULL==param_ptr)
    {
        //DC_LOG_TRACE:"DCAM:DCAMERA_StartPreview param_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_926_112_2_18_1_23_12_25,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }
  
    if(PNULL == pre_param_ptr)
    {
        return DCAMERA_OP_PARAM_ERR;
    }


    SCI_MEMCPY(pre_param_ptr, param_ptr, sizeof(DCAMERA_PREVIEW_PARAM_T));

    if(SENSOR_TYPE_ATV != _Sensor_GetSensorType())
    {
        disp_rot = _DCAMERA_ROT_Convertor(pre_param_ptr->rotation_mode, &img_rot);

        pre_param_ptr->rotation_mode = disp_rot;
     
    //rot_value = img_rot;
        DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_ROT_MODE, img_rot,  NULL);
    }
    /* check input parameter */

    ops_ret = preview_handlers->check_status();
    if(ops_ret < 0)
    {

        return DCAMERA_OP_SUCCESS;
    }
    else if(ops_ret > 0)
    {
        return ops_ret;
    }

    DC_RETURN_VAL_IF_FAIL(preview_handlers->check_param((DCAMERA_PREVIEW_PARAM_T *)pre_param_ptr), DCAMERA_OP_PARAM_ERR);

    ops_ret=preview_handlers->set_param((DCAMERA_PREVIEW_PARAM_T *)pre_param_ptr);
    DC_RETURN_VAL_IF_FAIL(ops_ret, ops_ret);

    ops_ret = preview_handlers->start();

    DC_RETURN_VAL_IF_FAIL(ops_ret, ops_ret);

    if(ISP_ServiceGetAtvIsEnable())
    {
        pContext->atv_callback = param_ptr->dc_atv_callback;
        DC_TimerActive(3000, DC_AtvRssiHandler);
    }

    //DC_LOG_TRACE:"DCAM:DCAMERA_StartPreview end: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_975_112_2_18_1_23_13_26,(uint8*)"d", SCI_GetTickCount());
   
    return ops_ret;
}


/******************************************************************************/
// Description:     stop preview
// Author:          jing.li
// Input:           none
// Output:          none
// Return:          0       successful
//                  others  failed
// Note:       		the caller will be pended until the last frame be displayed on
//             		lcd. after calling this function lcd will be on bypass mode
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StopPreview(void)
{
    DCAMERA_RETURN_VALUE_E eRet = DCAMERA_OP_SUCCESS;
    PREVIEW_OPS_HANDLER_T* preview_handlers = DC_Preview_GetOpsHandler();

    //DCAM_PRINT:"DCAM: DCAMERA_StopPreview, time %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_994_112_2_18_1_23_13_27,(uint8*)"d", CURRENT_TIME);

    eRet =preview_handlers->check_status();
    DC_RETURN_VAL_IF_FAIL(eRet, eRet);

    eRet = preview_handlers->stop();
    DC_RETURN_VAL_IF_FAIL(eRet, eRet);

    
    return eRet;
}

/******************************************************************************/
// Description:analog tv start auto scan channel
// Author:     Tim.zhu
// Input:      fun: call back function
// Output:     none
// Return:     SCI_TRUE    open success
//             SCI_FALSE   open fail
// Note:       
/******************************************************************************/ 
/*
	DCAMERA_ATV_CALLBACK atv_search_callback;
	DCAMERA_ATV_CALLBACK atv_stop_callback;*/
PUBLIC uint32 DCAMERA_ATVAutoScanChn(DCAMERA_PREVIEW_PARAM_T* param_ptr)
{
    DCAM_CONTEXT_T *pContext=_GetDCAMContext();
    PREVIEW_OPS_HANDLER_T* preview_handlers = DC_Preview_GetOpsHandler();

    //SCI_TRACE_LOW:"DCAMERA_ATVAutoScanChn: In Time: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1021_112_2_18_1_23_13_28,(uint8*)"d", SCI_GetTickCount());

    /* check input parameter */
    if(NULL==param_ptr)
    {
        //DC_LOG_TRACE:"DCAM:DCAMERA_StartPreview param_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1026_112_2_18_1_23_13_29,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }

    DC_RETURN_VAL_IF_FAIL(preview_handlers->check_param(param_ptr), DCAMERA_OP_PARAM_ERR);

    DC_RETURN_VAL_IF_FAIL(preview_handlers->set_param(param_ptr), DCAMERA_OP_PARAM_ERR);

    SET_DCAM_STATE(DC_STATE_ATV_SCAN_CHN);

    pContext->atv_search_callback=param_ptr->dc_atv_callback;
    
    DC_SendMsg(DCAM_MSG_ATV, DC_ATV_CMD_SCAN_CHN_START);

    //SCI_TRACE_LOW:"DCAMERA_ATVAutoScanChn: Out Time: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1040_112_2_18_1_23_13_30,(uint8*)"d", SCI_GetTickCount());

    return SCI_SUCCESS;
    
}

/******************************************************************************/
// Description:analog tv stop auto scan channel
// Author:     Tim.zhu
// Input:      fun: call back function
// Output:     none
// Return:     SCI_TRUE    open success
//             SCI_FALSE   open fail
// Note:       
/******************************************************************************/ 
PUBLIC uint32 DCAMERA_ATVStopScanChn(uint32 (*fun)())
{
    DCAMERA_RETURN_VALUE_E rtn=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T *pContext=_GetDCAMContext();

    //SCI_TRACE_LOW:"DCAM: DCAMERA_ATVStopScanChn: Time: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1058_112_2_18_1_23_13_31,(uint8*)"d", SCI_GetTickCount());

    pContext->atv_stop_callback=fun;

    DC_SendMsg(DCAM_MSG_ATV, DC_ATV_CMD_SCAN_CHN_STOP);

    if(PNULL==pContext->atv_stop_callback)
    {
        if(SCI_SUCCESS!=DC_GetEvent(DC_ATV_STOP_EVENT, DC_WAIT_TIME))
        {
            rtn=DCAMERA_OP_IOCTL_ERR;
        }
    }

    return rtn;
}

/******************************************************************************/
// Description:   set the combine(panorama) max num
// Author:          Tim.zhu
// Input:           uint32 num: max capture num
// Output:         none
// Return:          0           	successful
//                  others      failed
// Note:            if return successful
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_SetCombineNum(uint32 num)
{
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();

    pContext->max_combine_num=num;

    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************/
// Description:   clean the combine(panorama fourinone) parameter
// Author:          Tim.zhu
// Input:           none
// Output:         none
// Return:          0           	successful
//                  others      failed
// Note:            if return successful
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_CleanCombineParam(void)
{
    SNAPSHOT_OPS_HANDLER_T* snapshot_handlers = DC_Snapshot_GetOpsHandler();

    snapshot_handlers->clean_combineparam();

    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************/
// Description:     start snapshot
// Author:          jing.li
// Input:           param_ptr--data struct for related capture informations
// Output:          return_param_ptr--jpeg addresses and lengths
// Return:          0           	successful
//                  others      failed
// Note:            if return successful, the LCD will be on bridge mode until capture is done
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_DoSnapshot(
                                       DCAMERA_SNAPSHOT_PARAM_T   const*        param_ptr,
                                       DCAMERA_SNAPSHOT_RETURN_PARAM_T*        return_param_ptr)
{
    DCAMERA_RETURN_VALUE_E eRet = DCAMERA_OP_SUCCESS;
    SNAPSHOT_OPS_HANDLER_T* snapshot_handlers = DC_Snapshot_GetOpsHandler();
    DCAM_CONTEXT_T*	pContext = _GetDCAMContext();

    //DC_LOG_TRACE:"DCAM: DCAMERA_DoSnapshot, time %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1122_112_2_18_1_23_13_32,(uint8*)"d", CURRENT_TIME);

    if((NULL==param_ptr)
        ||(NULL==return_param_ptr))
    {
        //DC_LOG_TRACE:"DCAM:DCAMERA_DoSnapshot param_ptr/return_param_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1127_112_2_18_1_23_13_33,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }

    eRet = snapshot_handlers->check_status();
    DC_RETURN_VAL_IF_FAIL(eRet, eRet);
	
    eRet = snapshot_handlers->check_param((DCAMERA_SNAPSHOT_PARAM_T *)param_ptr);
    DC_RETURN_VAL_IF_FAIL(eRet, eRet);

    eRet = snapshot_handlers->set_param((DCAMERA_SNAPSHOT_PARAM_T *)param_ptr);
    DC_RETURN_VAL_IF_FAIL(eRet, eRet);

    s_dc_is_capture_quick_view=SCI_TRUE;
    
    if(param_ptr->snapshot_callback)
    {
        //DC_LOG_TRACE:"DCAM: capture asynchronous"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1144_112_2_18_1_23_13_34,(uint8*)"");
        DC_SendMsg(DCAM_MSG_SNAPSHOT , 0);
	}
    else
    {
        //DC_LOG_TRACE:"DCAM: capture synchronous"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1149_112_2_18_1_23_13_35,(uint8*)"");

        eRet=snapshot_handlers->start();

        return_param_ptr->return_data_addr = pContext->jpeg_buf_ptr;
        return_param_ptr->return_data_len = pContext->jpeg_size;
    }
    
    return eRet;
}

/******************************************************************************/
// Description: 	snapshot more than one jpeg image
// Author:     		Frank.yang
// Input:      		param_ptr:		data struct for related capture informations
// Output:		return_param_ptr:	jpeg addresses and lengths
// Return:		0        	successful
//				others      failed
// Note:       		
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_DoBurstSnapshot(
				DCAMERA_BURST_SNAPSHOT_PARAM_T  const* param_ptr,
				DCAMERA_BURST_SNAPSHOT_RETURN_PARAM_T* return_param_ptr)
{
    DCAMERA_RETURN_VALUE_E eRet = DCAMERA_OP_SUCCESS;
    SNAPSHOT_OPS_HANDLER_T* snapshot_handlers = DC_Snapshot_GetOpsHandler();

    //DC_LOG_TRACE:"DCAM: DCAMERA_DoBurstSnapshot, time %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1174_112_2_18_1_23_13_36,(uint8*)"d", CURRENT_TIME);

    if((NULL==param_ptr)
        ||(NULL==return_param_ptr))
    {
        //DC_LOG_TRACE:"DCAM:DCAMERA_DoSnapshot param_ptr/return_param_ptr is null err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1179_112_2_18_1_23_13_37,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }

    if(PNULL==param_ptr->burst_snapshot_callback)
    {
        //DC_LOG_TRACE:"DCAM:DCAMERA_DoSnapshot burst_snapshot_callback is null err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1185_112_2_18_1_23_13_38,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }

    eRet = snapshot_handlers->check_status();
    DC_RETURN_VAL_IF_FAIL(eRet, eRet);
	
    eRet = snapshot_handlers->check_param(param_ptr);/*lint !e64*/
    DC_RETURN_VAL_IF_FAIL(eRet, eRet);

    s_dc_is_capture_quick_view=SCI_FALSE;
   
    eRet = snapshot_handlers->set_burst_param((DCAMERA_BURST_SNAPSHOT_PARAM_T *)param_ptr,(DCAMERA_SNAPSHOT_RETURN_PARAM_T *)return_param_ptr);
    DC_RETURN_VAL_IF_FAIL(eRet, eRet);

    DC_SendMsg(DCAM_MSG_BURST_SNAPSHOT, 0);

    return eRet;

}


/******************************************************************************/
// Description:     review the selected jpeg image
// Author:          jing.li
// Input:           param_ptr:		data struct for related jpeg ,lcd informations
// Output:          none
// Return:          0           	successful
//                  others      failed
// Note:
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_ReviewJpeg(DCAMERA_REVIEW_PARAM_T const* param_ptr)
{
    DCAMERA_RETURN_VALUE_E eRet = DCAMERA_OP_SUCCESS;
    REVIEW_OPS_HANDLER_T* review_handlers = DC_Review_GetOpsHandler();
    SNAPSHOT_OPS_HANDLER_T* snapshot_handlers = DC_Snapshot_GetOpsHandler();    

    //SCI_TRACE_LOW:"DCAM: DCAMERA_ReviewJpeg rot:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1220_112_2_18_1_23_13_39,(uint8*)"d", param_ptr->rotation_mode);

    if(SCI_TRUE==s_dc_is_capture_quick_view)
    {
  //      s_dc_is_capture_quick_view=SCI_FALSE;

        eRet=snapshot_handlers->capture_review();

        if(PNULL!=param_ptr->review_callback)
        {
            DC_GetMutex();
            param_ptr->review_callback(eRet, PNULL, 0); 
            DC_PutMutex();
        }

        return eRet; 
    }
    
    if(NULL==param_ptr)
    {
        //DC_LOG_TRACE:"DCAM: DCAMERA_ReviewJpeg param_ptr is null err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1240_112_2_18_1_23_13_40,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }
	
    eRet = review_handlers->check_status();
    DC_RETURN_VAL_IF_FAIL(eRet, eRet);
	
    eRet = review_handlers->check_param((DCAMERA_REVIEW_PARAM_T *)param_ptr);
    DC_RETURN_VAL_IF_FAIL(eRet, eRet);

    eRet = review_handlers->set_param((DCAMERA_REVIEW_PARAM_T *)param_ptr);
    DC_RETURN_VAL_IF_FAIL(eRet, eRet);

    if(param_ptr->review_callback)
    {
        //DC_LOG_TRACE:"DCAM: review asynchronous"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1255_112_2_18_1_23_13_41,(uint8*)"");
        DC_SendMsg(DCAM_MSG_DISPLAY, 0);
    }
    else
    {
        //DC_LOG_TRACE:"DCAM: review synchronous"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1260_112_2_18_1_23_13_42,(uint8*)"");
        eRet = review_handlers->start();
    }
   
    return eRet;
}



/******************************************************************************/
// Description: 	get info from driver
// Author:     		Frank.yang
// Input:      		param_cmd:	
// Output:		info_ptr:		
// Return:		0           	successful
//				others      failed
// Note:       		the caller will be pended until the last frame be displayed on
//             		lcd. after calling this function lcd will be on bypass mode
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_GetInfo(DCAMERA_INFO_CMD_E param_cmd, uint32* info_ptr)
{
    DCAMERA_RETURN_VALUE_E eRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();
    SENSOR_EXP_INFO_T* sensor_info_ptr = Sensor_GetInfo();

    if(SCI_NULL == sensor_info_ptr)   
    {
        return DCAMERA_OP_NO_SENSOR_ERR;    
    }
    
	//SCI_TRACE_LOW:"DCAM: DCAMERA_GetInfo start cmd:0x%x info:0x%x, %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1288_112_2_18_1_23_13_43,(uint8*)"ddd", param_cmd,(*info_ptr), SCI_GetTickCount());

    switch(param_cmd)
    {
        case DCAMERA_INFO_BRIGHTNESS:
        {
            (*info_ptr)=sensor_info_ptr->step_count&0x0f;
            break;
        }		
        case DCAMERA_INFO_CONTRAST:
        {
            (*info_ptr)=(sensor_info_ptr->step_count&0xf0)>>0x04;
            break;
        }
        case DCAMERA_INFO_SHARPNESS:
        {
            (*info_ptr)=(sensor_info_ptr->step_count&0xf00)>>0x08;
            break;
        }		
        case DCAMERA_INFO_SATURATION:
        {
            (*info_ptr)=(sensor_info_ptr->step_count&0xf000)>>0x0c;
            break;
        }
        case DCAMERA_INFO_EXPOSURE_COMPENSATION:
        {
            (*info_ptr)=(sensor_info_ptr->step_count&0xf0000)>>0x10;
            break;
        }
        case DCAMERA_INFO_ZOOM:
        {
            (*info_ptr)=DC_GetZoomMaxLevel(info_ptr);
            break;
        }
        case DCAMERA_INFO_EFFECT:
        {
            *info_ptr=sensor_info_ptr->image_effect;
            break;
        }		
        case DCAMERA_INFO_PRVIEW_MODE:
        {
            *info_ptr=sensor_info_ptr->environment_mode;
            break;
        }		
        case DCAMERA_INFO_WB_MODE:
        {
            *info_ptr=sensor_info_ptr->wb_mode;
            break;
        }		
        case DCAMERA_INFO_SENSOR_SIZE:
        {
            *info_ptr=DC_GetSensorSize();
            break;  
        }
        case DCAMERA_INFO_BURST_PERMIT_NUM:
        {
            *info_ptr=DC_GetContinueShootMaxNum(info_ptr);
            break;
        }
        case DCAMERA_INFO_ATV_CH_ALL_NUM:
        {
            uint32 param=(ATV_CMD_GET_ALL_CHN_NUM<<0x10)&0xffff0000;
            *info_ptr=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);
            break;
        }
        case DCAMERA_INFO_ATV_RSSI:
        {

            uint32 param=(ATV_CMD_GET_RSSI<<0x10)&0xffff0000;
            *info_ptr=Sensor_Ioctl(SENSOR_IOCTL_ATV, param);
            break;
        }
        case DCAMERA_INFO_FEATURE_SUPPORT:
        {
            DC_ExGetFeatureSupportInfo(pContext->feature_context.ex_mem_size);
            (*info_ptr)=pContext->feature_context.feature_support_bit;
            break;
        }
        case DCAMERA_INFO_IMG_SENSOR_NUM:
        {
            SENSOR_REGISTER_INFO_T_PTR sensor_reg_ptr=Sensor_GetRegisterInfo();
            *info_ptr=sensor_reg_ptr->img_sensor_num;
            break;
        }
        case DCAMERA_INFO_CUR_IMG_SENSOR:
        {
            SENSOR_REGISTER_INFO_T_PTR sensor_reg_ptr=Sensor_GetRegisterInfo();
            *info_ptr=(uint32)sensor_reg_ptr->cur_id;
            break;
        } 
        case DCAMERA_INFO_MAX_SENSOR_SIZE:
        {
            *info_ptr=DC_GetMaxSensorSize();
            break;
        } 
        case DCAMERA_INFO_SENSOR_TYPE:
        {
            (*info_ptr)=DC_GetImgSensorType(info_ptr);
            break;
        }
        case DCAMERA_INFO_FOCUS_MODE:
        {
            if((NULL!=sensor_info_ptr)
                &&(NULL!=sensor_info_ptr->ext_info_ptr))
            {
                (*info_ptr)=sensor_info_ptr->ext_info_ptr->focus_mode;
            }
            break;
        }
        case DCAMERA_INFO_EXPOSURE_MODE:
        {
            if((NULL!=sensor_info_ptr)
                &&(NULL!=sensor_info_ptr->ext_info_ptr))
            {
                (*info_ptr)=sensor_info_ptr->ext_info_ptr->exposure_mode;
            }
            break;
        }
        case DCAMERA_INFO_THUMBNAIL_SIZE:
        {
            DCAMERA_SIZE_T size = {0};
            size.w = THUMB_WIDTH;
            size.h = THUMB_HEIGHT;
            *info_ptr = DCAMERA_PHOTO_SIZE_320X240;//
            break;
            
        }   
        case DCAMERA_INFO_PREVIEW_ROT:

            if(_IsDCamPreviewing())
            {
                *info_ptr = pContext->preview_param.rotation_mode;
            }
            else
            {
                eRet = DCAMERA_OP_ERROR;
            }
            break;
            
        default :/*lint !e616*//*lint !e825*/
        {
            *info_ptr=0x00;
            break;
        }        
    }	

    //SCI_TRACE_LOW:"DCAM: DCAMERA_GetInfo end cmd:0x%x info:0x%x, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1434_112_2_18_1_23_14_44,(uint8*)"ddd", param_cmd,(*info_ptr), SCI_GetTickCount());

    return eRet;
}

/******************************************************************************/
// Description: 	Set control parameter
// Author:     		Frank.yang
// Input:      		param_cmd:	cmd selector
// Output:		arg:			the value of selected cmd
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_ParamCtrl(
						DCAMERA_PARAM_CTRL_CMD_E param_cmd, 
						uint32 arg,
						DCAMERA_PARAM_CTRL_CALL_BACK callback)
{
    DCAMERA_RETURN_VALUE_E eRet = DCAMERA_OP_SUCCESS;
    pf_dcamera_param_ctrl_process process_func_ptr = DC_GetParamCtrlFunc(param_cmd);

	//SCI_TRACE_LOW:"DCAM: DCAMERA_ParamCtrl  start: %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1454_112_2_18_1_23_14_45,(uint8*)"d", SCI_GetTickCount());

    if(process_func_ptr)
    {
        eRet=process_func_ptr(arg, callback);
    }
    else
    {
        eRet = DCAMERA_OP_IOCTL_ERR;
        if(callback)
            callback(0, 0);
    }

    //SCI_TRACE_LOW:"DCAM: DCAMERA_ParamCtrl end: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1467_112_2_18_1_23_14_46,(uint8*)"d",SCI_GetTickCount());

	return eRet;

}

/******************************************************************************/
// Description: 	Set smooth zoom
// Author:     		Frank.yang
// Input:      		zoom_level:		zoom level
// Output:		zoom_callback:	async callback function
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
/*PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_SetSmoothZoom(uint32 zoom_level, 
									DCAMERA_CALL_BACK smoothzoom_callback)
{
	DCAMERA_RETURN_VALUE_E eRet = DCAMERA_OP_SUCCESS;

	return eRet;
}

*/
/******************************************************************************
// Description: auto focus control
// Author:     frank.yang
// Input:      af_info_ptr
// Output:	none
// Return:     	result of operation
// Note:       
******************************************************************************/ 
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_SetAutoFocus(DCAMERA_AF_INFO_T const * af_info_ptr)
{	
	return DCAMERA_OP_SUCCESS;
}

/******************************************************************************/
// Description: 	set upcc parameters
// Author:     		Frank.yang
// Input:      		pst_param:		point to the mjpg parameter
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		before recording m-jpeg, call this function to set parameters
/******************************************************************************/
PUBLIC int DC_SetUpccParam(const DC_MJPG_PARAM_T *pst_param)
{
    int nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();

    //DC_LOG_TRACE:"DCAM: DC_SetUpccParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1513_112_2_18_1_23_14_47,(uint8*)"");
    
    if(NULL==pst_param)
    {
        //DC_LOG_TRACE:"DCAM:DC_SetUpccParam pst_param is null err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1517_112_2_18_1_23_14_48,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }

    DC_RETURN_VAL_IF_FALSE_WARNING(DCAMERA_IsOpen(), DCAMERA_OP_NOT_OPEN, \
                                        ("DCAM:DC is not open yet"));

    pContext->mjpeg_param = *pst_param;

    DC_RETURN_VAL_IF_FAIL_WARNING(Sensor_SetMode(pContext->preview_mode), DCAMERA_OP_SENSOR_NOT_WORK_ERR, ("DCAM: set preview mode sensor err"));

    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_VIDEO_MODE, DCAMERA_VIDEO_MODE, PNULL);
	
    return nRet;
}

/******************************************************************************/
// Description: 	start recording
// Author:     		Frank.yang
// Input:      		param_ptr:	
// Output:		none:		
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
//PUBLIC DCAMERA_RETURN_VALUE_E DC_UpccStart(DCAMERA_RECORD_PARAM_T* param_ptr)
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StartRecord ( 
									DCAMERA_RECORD_PARAM_T * param_ptr )
{
    DCAMERA_RETURN_VALUE_E eRet=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    ISP_MJPEG_PARAM_T isp_mjpeg_param={0x00};
    DC_MJPG_PARAM_T mjpeg_param={0x00};
    LCD_SetInvalidateLimit(MAIN_LCD_ID, PERMIT_GUI_INVALIDATE);
    
    if(NULL==param_ptr)
    {
        //DC_LOG_TRACE:"DCAM:DC_UpccStart param_ptr is null err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1552_112_2_18_1_23_14_49,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }
    SCI_MEMCPY(&pContext ->upcc_param, param_ptr, sizeof(DCAMERA_RECORD_PARAM_T));

    mjpeg_param.i_lcd=MAIN_LCD_ID;
    mjpeg_param.st_disp_rect.x=0;
    mjpeg_param.st_disp_rect.y=0;
    mjpeg_param.st_disp_rect.w=param_ptr->target_size.w;
    mjpeg_param.st_disp_rect.h=param_ptr->target_size.h;
    mjpeg_param.i_frame_rate=0;
    mjpeg_param.i_quality=2;
    mjpeg_param.w_target_width=param_ptr->target_size.w;
    mjpeg_param.w_target_height=param_ptr->target_size.h;
//    mjpeg_param.end_callback=PNULL;	
    pContext->preview_mode=SENSOR_MODE_PREVIEW_ONE;
    eRet=DC_SetUpccParam(&mjpeg_param);
    DC_RETURN_VAL_IF_FAIL(eRet, eRet);

    SET_DCAM_STATE(DC_STATE_UPCC_REC);

    pContext->record_type = DCAM_RECORD_MJPEG;

    eRet=DC_Malloc(DC_STATE_REC);
    DC_RETURN_IF_FAIL(eRet, ("DCAM: DCAMERA_StartRecord alloc memory err"));

    eRet=_SetMjpegRecParam(&isp_mjpeg_param);
    DC_RETURN_VAL_IF_FAIL(eRet, DCAMERA_OP_PARAM_ERR);

    isp_mjpeg_param.disp_rotation = ISP_ROTATION_90;

    ISP_ServiceOpen();

    ISP_ServiceRegister(ISP_SERVICE_GET_DATA_CALLBACK, _mjpeg_encode_callback);
    ISP_ServiceRegister(ISP_SERVICE_MJPG_CALLBACK_NOTICE, _mjpeg_notice_callback);

    if(DCAMERA_OP_SUCCESS!=ISP_ServiceSetUpccParam(&isp_mjpeg_param))
    {
        return DCAMERA_OP_ERROR;
    }

	eRet=ISP_ServiceStartUpcc();

    return eRet;
}

/******************************************************************************/
// Description: 	stop recording
// Author:     		Frank.yang
// Input:      		none	
// Output:		none:		
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_StopRecord(void)
{
    int nRet = DCAMERA_OP_SUCCESS;	

    DC_RETURN_VAL_IF_FALSE_WARNING(DC_IN_STATE(DC_STATE_UPCC_REC), DCAMERA_OP_NOT_OPEN,\
                                        ("DCAM: dcam state error upcc record"));

    DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceFeatureStop(), DCAMERA_OP_ERROR,\
                                    ("DCAM: stop isp service mjpg"));

    SET_DCAM_STATE(DC_STATE_IDLE);
	
    return nRet;
}

/******************************************************************************
// Description: To check if digical camera is open
// Author:     Jing.li
// Input:      none
// Output:     none
// Return:     SCI_TRUE    has opened
//             SCI_FALSE   has not opened
// Note:       
******************************************************************************/ 
PUBLIC BOOLEAN DCAMERA_IsOpen(void) 
{
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();

    return (pContext->is_open);
}

/******************************************************************************/
// Description: 	set mjpg parameters
// Author:     		Frank.yang
// Input:      		pst_param:		point to the mjpg parameter
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		before recording m-jpeg, call this function to set parameters
/******************************************************************************/
PUBLIC int DC_SetMjpgRecParam(const DC_MJPG_PARAM_T *pst_param)
{
    int nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();

    if(NULL==pst_param)
    {
        //DC_LOG_TRACE:"DCAM:DC_SetMjpgRecParam pst_param is null err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1649_112_2_18_1_23_14_50,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }

    DC_RETURN_VAL_IF_FALSE_WARNING(DCAMERA_IsOpen(), DCAMERA_OP_NOT_OPEN, \
                                        ("DCAM:DC is not open yet"));

    pContext->mjpeg_param = *pst_param;

    nRet = _CheckMjpegRecParam(&(pContext->mjpeg_param));
	
    return nRet;
}

/******************************************************************************/
// Description: 	start mjpg capture
// Author:     		Frank.yang
// Input:      		dw_framenum:	total frame number, 0 is forbidden
//                           					(-1) refer to record infinite.
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
PUBLIC int DC_StartMjpgRec(uint32 dw_framenum, DC_RECORD_CALLBACK_FUNC pf_callback)
{
    int 	nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T *	pContext = _GetDCAMContext();
    ISP_MJPEG_PARAM_T	isp_mjpeg={0};

    //DCAM_PRINT:"DC_StartMjpgRec(%lu,0x%lx)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1677_112_2_18_1_23_14_51,(uint8*)"ud",dw_framenum, pf_callback);

    if(NULL==pf_callback)/*lint !e774*/
    {
        //DC_LOG_TRACE:"DCAM:DC_StartMjpgRec pf_callback is null err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1681_112_2_18_1_23_14_52,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }

    if(_IsDCamPreviewing())
        DCAMERA_StopPreview();

    DC_RETURN_VAL_IF_FALSE_WARNING(DC_IN_STATE(DC_STATE_IDLE), DCAMERA_OP_ERROR,
                                        ("DCAM: dcam state error before mjpg record"));

    SET_DCAM_STATE(DC_STATE_REC);
    
    if(VIDEO_MODE_YUV==pContext->mjpeg_param.video_mode)
    {
        pContext->record_type = DCAM_RECORD_YUV;
    }
    else
    {
         pContext->record_type = DCAM_RECORD_MJPEG;
    }

    pContext->mjpeg_record_callback=pf_callback;	

    nRet=DC_Malloc(DC_STATE_REC);
    DC_RETURN_IF_FAIL(nRet, ("DCAM: DC_StartMjpgRec alloc memory fail err"));
    
    nRet = _SetMjpegRecParam(&isp_mjpeg);
    DC_RETURN_VAL_IF_FAIL(nRet, DCAMERA_OP_PARAM_ERR);

    ISP_ServiceOpen();
    ISP_ServiceRegister(ISP_SERVICE_GET_DATA_CALLBACK, _mjpeg_encode_callback);
    ISP_ServiceRegister(ISP_SERVICE_MJPG_CALLBACK_NOTICE, _mjpeg_notice_callback);

    if(DCAMERA_OP_SUCCESS == ISP_ServiceSetMjpegParam(&isp_mjpeg))
    {
        nRet=ISP_ServiceStartMjpeg();
    }
    else
    {
        nRet = DCAMERA_OP_ERROR;
    }

    return nRet;
}

/******************************************************************************/
// Description: 	stop mjpg capture 
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
PUBLIC int DC_StopMjpgRec(void)
{
    int nRet = DCAMERA_OP_SUCCESS;

    DC_RETURN_VAL_IF_FALSE_WARNING(DC_IN_STATE(DC_STATE_REC) , DCAMERA_OP_NOT_OPEN,\
                                        ("DCAM: dcam state error mjpg record"));


    DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceFeatureStop(), DCAMERA_OP_ERROR,\
                                    ("DCAM: stop isp service mjpg"));

    SET_DCAM_STATE(DC_STATE_IDLE);

    return nRet;
}


/******************************************************************************/
// Description: 	free memory frame in pool 
// Author:     		Frank.yang
// Input:      		p_frame:	point to the frame structure
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
PUBLIC int DC_FreeFrame(DC_FRAME_T * p_frame)
{
	int nRet = DCAMERA_OP_SUCCESS;

	nRet = DC_FrameMgr_FreeAFrame(p_frame);

	return nRet;
}

/******************************************************************************/
// Description: 	pause m-jpeg recording
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
/*PUBLIC int DC_PauseMjpg(void)
{
    int nRet = DCAMERA_OP_SUCCESS;

    DC_RETURN_VAL_IF_FALSE_WARNING(DC_IN_STATE(DC_STATE_REC) ,\
                                        DCAMERA_OP_NOT_OPEN, ("DCAM: state err in pause mjpg"));

    SET_DCAM_STATE(DC_STATE_MJPEG_REC_PAUSE);
    nRet = ISP_ServicePauseMjpeg();

    return nRet;
}

*/

/******************************************************************************/
// Description: 	resume m-jpeg recording
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
/*PUBLIC int DC_ResumeMjpg(void)
{
    int nRet = DCAMERA_OP_SUCCESS;

    DC_RETURN_VAL_IF_FALSE_WARNING(DC_IN_STATE(DC_STATE_MJPEG_REC_PAUSE),\
                                        DCAMERA_OP_NOT_OPEN, ("DCAM: state err in resume mjpg"));

    SET_DCAM_STATE(DC_STATE_REC);
    nRet = ISP_ServiceResumeMjpeg();

    return nRet;
}
*/
/******************************************************************************/
// Description: 	video display init
// Author:     		Tim.zhu
// Input:      		pst_param:
// Output:		none
// Return:		0           successful
//				others      failed
// Note:       		
/******************************************************************************/
PUBLIC uint32 DC_VideoDisplayInit(void)
{
    uint32 Rtn=DCAMERA_OP_SUCCESS;

    DCAM_CONTEXT_T *pContext = _GetDCAMContext();

    //DC_LOG_TRACE:"DCAM:DC_VideoDisplayInit"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1821_112_2_18_1_23_14_53,(uint8*)"");

    SCI_DCAM_EnableDeepSleep(DISABLE_AHB_SLEEP);

    INIT_CONTEXT(pContext);

    //init ISP
    ISP_ServiceInit( );

    return Rtn;
}

/******************************************************************************/
// Description: 	video display close
// Author:     		Tim.zhu
// Input:      		none
// Output:		none
// Return:		0           successful
//				others      failed
// Note:       		
/******************************************************************************/
PUBLIC uint32 DC_VideoDisplayClose(void)
{
    uint32 Rtn = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();

    //DC_LOG_TRACE:"DCAM:DC_VideoDisplayClose"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1845_112_2_18_1_23_14_54,(uint8*)"");

    pContext->is_open = FALSE;

    SET_DCAM_STATE(DC_STATE_UNKNOWN);

    ISP_ServiceFeatureStop();
    
    ISP_ServiceClose();

    SCI_DCAM_EnableDeepSleep(ENABLE_AHB_SLEEP);

    return Rtn;
}

LOCAL uint32 _display_callback(DCAMERA_RETURN_VALUE_E ret_value, void* param_ptr, uint32 param_size)
{

    ISP_ServiceUnlock(ISP_SERVICE_OWNER_DC);
    return s_dc_mpeg_display_callback(ret_value, param_ptr, param_size);
}


/******************************************************************************/
// Description: 	set video display parameter
// Author:     		Frank.yang
// Input:      		pst_param:
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
PUBLIC int DC_VideoSetDisplayParam(const DC_DPLAYER_DISPLAY_PARAM_T *pst_param)
{
    int nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();

    //DCAM_PRINT:"DCAM:DC_VideoSetDisplayParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1880_112_2_18_1_23_14_55,(uint8*)"");
    
    if(NULL==pst_param)
    {
        //DC_LOG_TRACE:"DCAM:DC_VideoSetDisplayParam pst_param is null err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1884_112_2_18_1_23_14_56,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }

    SET_DCAM_STATE(DC_STATE_VIDEO_REVIEW);

    LCD_SetInvalidateLimit(MAIN_LCD_ID, PERMIT_GUI_INVALIDATE);

    SCI_MEMCPY(&(pContext->mjpeg_review_param), pst_param, sizeof(DC_DPLAYER_DISPLAY_PARAM_T));

    DC_AdjRect_OnDistortion((DCAMERA_RECT_T*)&pContext->mjpeg_review_param.st_trim_rect, 
                             &pContext->mjpeg_review_param.st_disp_rect, 
                             &pContext->mjpeg_review_param.target_rect, 
                             pContext->mjpeg_review_param.disp_mode, 
                             pContext->mjpeg_review_param.rotation);

    pContext->target_width 	= pContext->mjpeg_review_param.src_width;
    pContext->target_height	= pContext->mjpeg_review_param.src_height;

    nRet=DC_Malloc(DC_STATE_VIDEO_REVIEW);
    DC_RETURN_IF_FAIL(nRet, ("DCAM: DC_VideoSetDisplayParam alloc memory err"));

    s_dc_mpeg_display_callback = pst_param->review_callback;

  //  ISP_ServiceOpen( );

  
    if(PNULL != s_dc_mpeg_display_callback)
    {
        ISP_Regist_Mpeg_Display_Callback((ISP_MPG_DISPLAY_CALLBACK)_display_callback);      //need confirm  
        LCD_SetOpMode(MAIN_LCD_ID, UI_OPERATION);
        LCD_SetOpMode(SUB_LCD_ID, UI_OPERATION);

        LCD_GetLcdCtrlSemaphore(0);
        LCD_DisableBlock(MAIN_LCD_ID, 0);
        LCD_PutLcdCtrlSemaphore(0);
    }
    else
    {
       ISP_Regist_Mpeg_Display_Callback((ISP_MPG_DISPLAY_CALLBACK)PNULL); 
    }

    return nRet;
}

/******************************************************************************/
// Description: 	video display parameter
// Author:     		Tim.zhu
// Input:      		pst_param:
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
PUBLIC int DC_DisplayAFrame(DISPLAY_DATA_FRAME_T* p_frame)
//PUBLIC int DC_VideoDisplayFrame(DISPLAY_DATA_FRAME_T* p_frame)
{
    int nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();
    ISP_REVIEW_PARAM_T isp_review={0};

    if(NULL==p_frame)
    {
        //DC_LOG_TRACE:"DCAM:DC_DisplayAFrame p_frame is null err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1945_112_2_18_1_23_15_57,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }

    

    //protection for the isp_service operating
    if (SCI_SUCCESS != ISP_ServiceLock(ISP_SERVICE_OWNER_DC, DC_WAIT_TIME))
	{
		//DC_LOG_TRACE:"DCAM:DC lock isp error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1954_112_2_18_1_23_15_58,(uint8*)"");
		return DCAMERA_OP_ERROR;
	}

    ISP_ServiceOpen( );

	
    nRet=_SetMjpegDecParam(&isp_review, p_frame);
    DC_RETURN_VAL_IF_FAIL(nRet, nRet);

    pContext->status = DCAM_STATUS_IDLE;	
    if(DCAMERA_OP_SUCCESS == ISP_ServiceSetReviewParam(&isp_review))
    {
        nRet=ISP_ServiceStartReview( );
        pContext->status = DCAM_STATUS_DECODE_DONE;		
    }
    else
    {
        nRet = SCI_ERROR;
    }

    if(PNULL == s_dc_mpeg_display_callback)
    {
        ISP_ServiceUnlock(ISP_SERVICE_OWNER_DC);
    }

    return nRet;
}

/******************************************************************************/
// Description: Get the frame data from isp
// Author:     Tim.zhu
// Input:      int (*func)(): call back
//             data_format:get data format
// Output:     None
// Return:     DCAMERA_OP_SUCCESS: success
//             other: fail
// Note:       
/******************************************************************************/  
PUBLIC uint32  DC_GetFrameData(int (*func)(), DCAMERA_DATA_TYPE_E data_format)
{
    uint32 rtn=SCI_ERROR;
    ISP_DATA_FORMAT_E isp_data_format=ISP_DATA_MAX;

    if(NULL==func)
    {
        //DC_LOG_TRACE:"DCAM:DC_GetFrameData register func is null err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_1998_112_2_18_1_23_15_59,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    }

    if(DCAMERA_DATA_TYPE_YUV422==data_format)
    {
        isp_data_format=ISP_DATA_YUV422;
    }
    else
    {
        isp_data_format=ISP_DATA_RGB565;
    }
        
    rtn=ISP_ServiceGetFrameData(func, isp_data_format);
    
    return rtn;
}

/******************************************************************************/
// Description: Get the hight of scale match
// Author:     Tim.zhu
// Input:      input_height : input height
//             output_height : output height
// Output:     None
// Return:     uint16: match height
// Note:       
/******************************************************************************/  
PUBLIC uint16 DCAMERA_GetScaleMatchHeight(uint16 input_height, uint16 output_height)
{    
    return ISP_ServiceGetScaleMatchHeight(input_height, output_height);
}

/******************************************************************************/
// Description: Set ext mem size
// Author:     Tim.zhu
// Input:      mem_size : ext mem size
//             mem_ptr : the point of ext mem addr
// Output:     None
// Return:     DCAMERA_OP_SUCCESS: success
// Note:       
/******************************************************************************/  
PUBLIC uint32 DCAMERA_SetExtMem(uint32 mem_size,void* mem_ptr)
{
    uint32 ret=DCAMERA_OP_SUCCESS;

    DCAM_CONTEXT_T* pContext=_GetDCAMContext();

    pContext->feature_context.ex_mem_size = mem_size;

    pContext->feature_context.mem_ptr = mem_ptr;
    return ret;
}

#if 0
/******************************************************************************/
// Description: Get the info of register img sensor
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     SENSOR_REGISTER_INFO_T_PTR:the info pointer of register img sensor
// Note:       
/******************************************************************************/  
PUBLIC SENSOR_REGISTER_INFO_T_PTR DC_GetImgSensorRegisterInfo(void) 
{
    return Sensor_GetRegisterInfo();
} 
#endif
/******************************************************************************/
// Description: Get the id if currect img sensor
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     uint32:the id of img sensor
// Note:       
/******************************************************************************/  
PUBLIC uint32 DC_GetCurImgSensorId(void) 
{
    return Sensor_GetCurId();
} 

/******************************************************************************/
// Description: Set the id if currect img sensor
// Author:     Tim.zhu
// Input:      None
// Output:     None
// Return:     DCAMERA_OP_SUCCESS: success
// Note:       
/******************************************************************************/  
PUBLIC uint32 DC_SetCurImgSensorId(uint32 sensor_id) 
{
    return Sensor_SetCurId(sensor_id);
}  

/******************************************************************************/
// Description: 	query YUV running state
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:		
// 				DCAM_STATUS_IDLE         			 0
// 				DCAM_STATUS_PREVIEW_FIRST	1
// 				DCAM_STATUS_DEC_FIRST			2
// 				DCAM_STATUS_ENCODE_DONE 	4
// 				DCAM_STATUS_DECODE_DONE		8
// 				DCAM_STATUS_DMA_DONE			0x10
// 				DCAM_STATUS_LCM_BUSY			0x20
// 				DCAM_STATUS_DISPLAY_DONE		0x40
// 				DCAM_STATUS_ERROR				0xffff0000
// Note:       		
/******************************************************************************/
PUBLIC int DC_QueryStatus(void)
{
	return g_dcam_context.status;
}

/******************************************************************************/
// Description: Set Third part get data param
// Author:     Tim.zhu
// Input:      DCAMERA_EXT_GET_DATA_T_PTR : the pointer of display param
// Output:     ISP_JPEG_PARAM_T : the pointer of isp capture
// Return:     SCI_TRUE    success
//             SCI_FALSE   fail
// Note:       
/******************************************************************************/  
LOCAL uint32 _ExtSetGetDataParam(DCAMERA_EXT_GET_DATA_T_PTR capture_param, ISP_JPEG_PARAM_T* isp_param) 
{
    int	nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();

    SENSOR_EXP_INFO_T*      sensor_info_ptr = pContext->sensor_info_ptr;
    DCAMERA_RECT_T          src_rect={0};

    pContext->exif_eb=SCI_FALSE;
    pContext->snapshot_param.capture_date.eb=SCI_FALSE;

    pContext->jpeg_fw_len=DC_RIGHT_TRUNK((40*1024),256);
    pContext->jpeg_fw_ptr=(uint8*)((capture_param->buf_addr+capture_param->buf_len)-pContext->jpeg_fw_len);

    pContext->isp_capture_len=DC_RIGHT_TRUNK((capture_param->img_size.w*capture_param->img_size.h),256)*2;  
    pContext->isp_capture_ptr=pContext->jpeg_fw_ptr-pContext->isp_capture_len;
    pContext->isp_capture_ptr=(uint8*)DC_LEFT_TRUNK((uint32)pContext->isp_capture_ptr,256);    

    pContext->disp_buf_len=DC_RIGHT_TRUNK((capture_param->target_rect.w*capture_param->target_rect.h),256)*2;  
    pContext->disp_buf_ptr=pContext->isp_capture_ptr-pContext->disp_buf_len;  
    pContext->disp_buf_ptr=(uint8 *)(DC_LEFT_TRUNK((uint32)pContext->disp_buf_ptr,256));  

    pContext->rot_buf_len=DC_RIGHT_TRUNK((capture_param->target_rect.w*capture_param->target_rect.h),256)*2;  
    pContext->rot_buf_ptr=pContext->disp_buf_ptr-pContext->rot_buf_len;  
    pContext->rot_buf_ptr=(uint8 *)(DC_LEFT_TRUNK((uint32)pContext->rot_buf_ptr,256)); 

    pContext->jpeg_buf_ptr=(uint8 *)(capture_param->buf_addr);
    pContext->jpeg_buf_len=capture_param->buf_len-pContext->isp_capture_len; 

    pContext->snapshot_param.enc_quality_level=JINF_QUALITY_MIDDLE_LOW;        

    if(capture_param->buf_len<(pContext->isp_capture_len+pContext->disp_buf_len+pContext->rot_buf_len))   
    {
        //DCAM_PRINT:"DCM: _ExtSetGetDataParam jpg buf is not enough err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_2141_112_2_18_1_23_15_60,(uint8*)"");
        return SCI_ERROR;
    } 

    isp_param->skip_num=0x00;
    
    isp_param->input_size.w=sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].width;
    isp_param->input_size.h=sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].height;

    isp_param->input_rect.x=sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].trim_start_x;
    isp_param->input_rect.y=sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].trim_start_y;
    isp_param->input_rect.w=sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].trim_width;
    isp_param->input_rect.h=sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].trim_height;

    src_rect.x=0x00;
    src_rect.y=0x00;
    src_rect.w=capture_param->img_size.w;
    src_rect.h=capture_param->img_size.h;    

    DC_AdjRect_OnDistortion((DCAMERA_RECT_T*)&isp_param->input_rect, 
                            &src_rect, 
                            &src_rect, 
                            DCAMERA_DISP_FULLSCREEN, 
                            DCAMERA_ROTATION_0);

    isp_param->output_size.w=capture_param->img_size.w;
    isp_param->output_size.h=capture_param->img_size.h;    

    isp_param->capture_addr=(uint32)pContext->isp_capture_ptr;
    isp_param->capture_buf_size=pContext->isp_capture_len;
    
    isp_param->sensor_data_format=ISP_DATA_YUV422;    
    isp_param->output_data_format=ISP_DATA_YUV422;

    return nRet;
} 

/******************************************************************************/
// Description: Set Third part quick view param
// Author:     Tim.zhu
// Input:      DCAMERA_EXT_GET_DATA_T_PTR : the pointer of capture param
// Output:     ISP_REVIEW_PARAM_T : the pointer of isp review
// Return:     SCI_TRUE    success
//             SCI_FALSE   fail
// Note:       
/******************************************************************************/  
LOCAL uint32 _ExtSetQuickReviewParam(DCAMERA_EXT_GET_DATA_T_PTR capture_param, ISP_REVIEW_PARAM_T* review_param) 
{
    int	nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    
    review_param->input_size.w=pContext->actual_width;
    review_param->input_size.h=pContext->actual_height;

    review_param->input_rect.x=0x00;
    review_param->input_rect.y=0x00;
    review_param->input_rect.w=review_param->input_size.w;
    review_param->input_rect.h=review_param->input_size.h;

    review_param->disp_rect.x=capture_param->display_rect.x;
    review_param->disp_rect.y=capture_param->display_rect.y;
    review_param->disp_rect.w=capture_param->display_rect.w;
    review_param->disp_rect.h=capture_param->display_rect.h;   

    review_param->target_rect.x=capture_param->target_rect.x;
    review_param->target_rect.y=capture_param->target_rect.y;
    review_param->target_rect.w=capture_param->target_rect.w;
    review_param->target_rect.h=capture_param->target_rect.h;  

    DC_AdjRect_OnDistortion((DCAMERA_RECT_T*)&review_param->input_rect, 
                             (DCAMERA_RECT_T*)&review_param->disp_rect, 
                             (DCAMERA_RECT_T*)&review_param->target_rect, 
                             DCAMERA_DISP_FULLSCREEN, 
                             capture_param->display_rotation);
    
    review_param->input_format=ISP_DATA_YUV422;
    review_param->src_addr.y_addr=pContext->yyuv_buf.y_addr;
    review_param->src_addr.uv_addr=pContext->yyuv_buf.uv_addr;   
    review_param->src_len.y_len=DC_LEFT_TRUNK(pContext->actual_width*pContext->actual_height,256);
    review_param->src_len.uv_len=DC_LEFT_TRUNK(pContext->actual_width*pContext->actual_height,256);
    
    review_param->disp_format=ISP_DATA_RGB565;
    review_param->disp_addr=(uint32)pContext->disp_buf_ptr;
    review_param->disp_buf_size=pContext->disp_buf_len;

    review_param->rot_buf_len.y_len=DC_RIGHT_TRUNK((review_param->target_rect.w*review_param->target_rect.h), 256)*2;
    review_param->rot_buf_len.uv_len=review_param->target_rect.w*review_param->target_rect.h;
    review_param->rot_addr.y_addr=(uint32)pContext->rot_buf_ptr;
    review_param->rot_addr.uv_addr=DC_RIGHT_TRUNK((review_param->rot_addr.y_addr+review_param->rot_buf_len.uv_len), 256);

    review_param->disp_lcd_id=capture_param->lcd_id;
    review_param->disp_rotation=CONVERT_ROTAION_MODE(capture_param->display_rotation);  
    
    return nRet;
}

/******************************************************************************/
// Description: Set third part decoder jpg param
// Author:     Tim.zhu
// Input:      DCAMERA_EXT_DISPLAY_T_PTR : the pointer of display param
// Output:     none
// Return:     SCI_TRUE    success
//             SCI_FALSE   fail
// Note:       
/******************************************************************************/  
LOCAL uint32 _ExtSetDecoderJpegParam(DCAMERA_EXT_DISPLAY_T_PTR display_param) 
{
    uint32 rtn = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();

    pContext->review_param.jpeg_buffer=(uint8 *)display_param->src_jpg_addr;
    pContext->review_param.jpeg_len=display_param->src_jpg_buf_len;

    pContext->review_param.disp_rect.x=display_param->display_rect.x;
    pContext->review_param.disp_rect.y=display_param->display_rect.y;
    pContext->review_param.disp_rect.w=display_param->display_rect.w;
    pContext->review_param.disp_rect.h=display_param->display_rect.h;

    pContext->yyuv_buf.y_addr=display_param->src_yuv_addr.y_chn_addr;
    pContext->yyuv_buf_len=display_param->src_yuv_buf_len;

    pContext->jpeg_fw_ptr=(uint8 *)display_param->jpg_decoder_addr;
    pContext->jpeg_fw_len=display_param->src_decoder_buf_len;      

    return rtn;
}
 
/******************************************************************************/
// Description: Get review data format
// Author:     Tim.zhu
// Input:      DCAMERA_DATA_TYPE_E : Dcam data format
// Output:     None
// Return:     ISP_DATA_RGB565: isp rgb565
//             ISP_DATA_YUV422 : isp yuv422
// Note:       
/******************************************************************************/  
LOCAL uint32 _ExtGetReviewDataFormat(DCAMERA_DATA_TYPE_E data_format) 
{
    if(DCAMERA_DATA_TYPE_RGB565==data_format)
    {
        return ISP_DATA_RGB565;
    }
    else
    {
        return ISP_DATA_YUV422;
    }

    //return ISP_DATA_YUV422;
} 

/******************************************************************************/
// Description: Set the third part review param
// Author:     Tim.zhu
// Input:      DCAMERA_EXT_DISPLAY_T_PTR : the pointer of display param
// Output:     ISP_REVIEW_PARAM_T : the pointer of isp review param
// Return:     SCI_TRUE    success
//             SCI_FALSE   fail
// Note:       
/******************************************************************************/  
LOCAL uint32 _ExtSetReviewParam(DCAMERA_EXT_DISPLAY_T_PTR display_param, ISP_REVIEW_PARAM_T* review_param) 
{
    uint32 rtn = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    
    review_param->input_size.w=pContext->target_width;
    review_param->input_size.h=pContext->target_height;

    review_param->input_rect.x=0x00;
    review_param->input_rect.y=0x00;
    review_param->input_rect.w=review_param->input_size.w;
    review_param->input_rect.h=review_param->input_size.h;

    review_param->disp_rect.x=display_param->display_rect.x;
    review_param->disp_rect.y=display_param->display_rect.y;
    review_param->disp_rect.w=display_param->display_rect.w;
    review_param->disp_rect.h=display_param->display_rect.h;  

    review_param->target_rect.x=display_param->target_rect.x;
    review_param->target_rect.y=display_param->target_rect.y;
    review_param->target_rect.w=display_param->target_rect.w;
    review_param->target_rect.h=display_param->target_rect.h;  

    DC_AdjRect_OnDistortion((DCAMERA_RECT_T*)&review_param->input_rect, 
                             (DCAMERA_RECT_T*)&review_param->disp_rect, 
                             (DCAMERA_RECT_T*)&review_param->target_rect, 
                             DCAMERA_DISP_FULLSCREEN, 
                             display_param->display_rotation);

    review_param->input_format=_ExtGetReviewDataFormat(display_param->src_format);
    
    review_param->src_addr.y_addr=pContext->yyuv_buf.y_addr;
    review_param->src_addr.uv_addr=pContext->yyuv_buf.uv_addr;  
    review_param->src_len.y_len=DC_RIGHT_TRUNK((pContext->target_width*pContext->target_height), 256);
    review_param->src_len.uv_len=DC_RIGHT_TRUNK((pContext->target_width*pContext->target_height), 256);
    
    review_param->disp_format=ISP_DATA_RGB565;
    review_param->disp_addr=display_param->display_addr;
    review_param->disp_buf_size=display_param->display_buf_len;


    review_param->rot_buf_len.y_len=DC_RIGHT_TRUNK((display_param->target_rect.w*display_param->target_rect.h), 256)*2;
    review_param->rot_buf_len.uv_len=display_param->target_rect.w*display_param->target_rect.h;
    review_param->rot_addr.y_addr=display_param->rotation_addr;
    review_param->rot_addr.uv_addr=DC_RIGHT_TRUNK((review_param->rot_addr.y_addr+review_param->rot_buf_len.uv_len), 256);

    review_param->disp_lcd_id=display_param->lcd_id;
    review_param->disp_rotation=CONVERT_ROTAION_MODE(display_param->display_rotation);    

    return rtn;
}

/******************************************************************************/
// Description: Third part open sensor
// Author:     Tim.zhu
// Input:      DCAMERA_IMG_SENSOR_MODE : img sensor mode (main or sub)
// Output:     none
// Return:     SCI_TRUE    open success
//             SCI_FALSE   open fail
// Note:       
/******************************************************************************/ 
PUBLIC uint32 DCAMERA_ExtOpenSensor(DCAMERA_IMG_SENSOR_MODE img_sensor_mode, DCAMERA_PARAM_VIDEO_MODE_E work_mode)
{
    DCAM_CONTEXT_T *pContext=_GetDCAMContext();
    uint32 img_sensor_num=0x00;
    uint32 cur_img_sensor_id=0x00;
    ISP_SENSOR_SIGNAL_INFO_T isp_sensor_singnal={0};

    pContext->ext_open=SCI_FALSE;

    SCI_DCAM_EnableDeepSleep(DISABLE_AHB_SLEEP);
    
    DCAMERA_SetSensorType(DCAMERA_OP_NORMAL);
    
    DC_RETURN_VAL_IF_FAIL_WARNING(_DCAMERA_InitSensor(), DCAMERA_OP_SENSOR_NOT_WORK_ERR, ("DCAM: init sensor error at open"));

    DCAMERA_GetInfo(DCAMERA_INFO_IMG_SENSOR_NUM, &img_sensor_num);

    if(0x01<img_sensor_num)
    {
        DCAMERA_GetInfo(DCAMERA_INFO_CUR_IMG_SENSOR, &cur_img_sensor_id);

        if(img_sensor_mode!=cur_img_sensor_id)
        {
            DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_IMG_SENSOR_MODE, img_sensor_mode, PNULL);
        }
    }

    Sensor_SetMode(SENSOR_MODE_PREVIEW_ONE);

    if(DCAMERA_MODE_MAX>work_mode)
    {
        DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_VIDEO_MODE, work_mode, PNULL);
    }

    DC_GetImgSensorSignalInfo(SENSOR_MODE_PREVIEW_ONE,&isp_sensor_singnal);

    ISP_ServiceSetSensorSignalInfo(&isp_sensor_singnal);

    pContext->ext_open=SCI_TRUE;

    return DCAMERA_OP_SUCCESS;
} 

/******************************************************************************/
// Description: Third part close sensor
// Author:     Tim.zhu
// Input:      none
// Output:     none
// Return:     SCI_TRUE    close success
//             SCI_FALSE   close fail
// Note:       
/******************************************************************************/  
PUBLIC uint32 DCAMERA_ExtCloseSensor(void)
{
    uint32 ret=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T *pContext=_GetDCAMContext();
    
    ret=Sensor_Close();

    SCI_DCAM_EnableDeepSleep(DISABLE_AHB_SLEEP);

    pContext->ext_open=SCI_FALSE;

    return ret;
} 

/******************************************************************************/
// Description: Third part get img data
// Author:     Tim.zhu
// Input:      DCAMERA_EXT_GET_DATA_T_PTR : the pointer of get data param
// Output:     DCAMERA_YUV_CHN_ADDR_T : image addr
//             DCAMERA_SIZE_T : img size
// Return:     SCI_SUCCESS    get data success
//             SCI_ERROR   get data fail
// Note:       
/******************************************************************************/  
PUBLIC uint32 DCAMERA_ExtGetData(DCAMERA_EXT_GET_DATA_T_PTR get_param, DCAMERA_YUV_CHN_ADDR_T* rtn_img_addr, DCAMERA_SIZE_T* rtn_size)
{
    uint32 rtn=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();
    uint32 capture_rtn_width=0x00;
    uint32 capture_rtn_height=0x00; 
    ISP_JPEG_PARAM_T isp_capture={0x00};   

    if((NULL==get_param)
        ||(SCI_FALSE==pContext->ext_open))
    {
        //DCAM_PRINT:"DCM: DCAMERA_ExtGetData param is null or img sesor is not open"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_2434_112_2_18_1_23_16_61,(uint8*)"");
        rtn=SCI_ERROR;
        return rtn;
    }
#ifdef MET_MEX_SUPPORT    
    {
        if(mex_Phs_Is_Camera_Working())
        {//解决获取到jpeg图像后又发生解码jpeg时导致之后获取的图像有绿边的问题
            void *sensor_signal_ptr = NULL;
            sensor_signal_ptr=DC_GetImgSensorSignalInfo(SENSOR_MODE_PREVIEW_ONE);    
            ISP_ServiceSetSensorSignalInfo(sensor_signal_ptr);
        }
    }
#endif

    _ExtSetGetDataParam(get_param, &isp_capture);

    ISP_ServiceOpen( );

    pContext->status = DCAM_STATUS_IDLE;	

    if(DCAMERA_OP_SUCCESS==ISP_ServiceSetCaptureParam((&isp_capture)))
    {
        if(DCAMERA_OP_SUCCESS!=ISP_ServiceStartSyncCapture(&pContext->yyuv_buf, &capture_rtn_width, &capture_rtn_height))
        {
            ISP_ServiceClose();
            return SCI_ERROR;
        }
        pContext->actual_width=(uint16)capture_rtn_width;
        pContext->actual_height=(uint16)capture_rtn_height; 
        pContext->target_width=pContext->actual_width;
        pContext->target_height=pContext->actual_height;        
    }
    else
    {
        //DCAM_PRINT:"DCM: DCAMERA_ExtGetData capture err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_2469_112_2_18_1_23_16_62,(uint8*)"");
        ISP_ServiceClose();
        return SCI_ERROR;
    }

    if(SCI_TRUE==get_param->is_display)
    {
        ISP_REVIEW_PARAM_T isp_review={0x00};

        _ExtSetQuickReviewParam(get_param, &isp_review);
        
        if(DCAMERA_OP_SUCCESS == ISP_ServiceSetReviewParam(&isp_review))
        {
            LCD_DisableBlock(MAIN_LCD_ID, 0x02);   
            ISP_ServiceStartReview();	
            LCD_EnableBlock(MAIN_LCD_ID, 0x02);
        }
        else
        {
            //DCAM_PRINT:"DCM: DCAMERA_ExtGetData review err"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_2488_112_2_18_1_23_16_63,(uint8*)"");
            ISP_ServiceClose();
            return SCI_ERROR;
        }
    }

    ISP_ServiceClose();

    if(DCAMERA_DATA_TYPE_JPEG==get_param->data_format)
    {
        if(DCAMERA_OP_SUCCESS!=_EncodeYYUVToJpeg((uint32*)&pContext->jpeg_buf_ptr,&pContext->jpeg_size))
        {
            //DCAM_PRINT:"DCM: DCAMERA_ExtGetData encoder err"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_APP_2500_112_2_18_1_23_16_64,(uint8*)"");
            rtn = SCI_ERROR;
            pContext->jpeg_buf_ptr = PNULL;
            pContext->jpeg_size    = 0;
        }
        rtn_img_addr->y_chn_addr=(uint32)pContext->jpeg_buf_ptr;
        rtn_img_addr->u_chn_addr=NULL;
        rtn_size->w=pContext->jpeg_size;
        rtn_size->h=0x00;
    }
    else
    {
        rtn_img_addr->y_chn_addr=pContext->yyuv_buf.y_addr;
        rtn_img_addr->u_chn_addr=pContext->yyuv_buf.uv_addr;
        rtn_size->w=pContext->actual_width;
        rtn_size->h=pContext->actual_width;
    }

    return rtn;
}
 
/******************************************************************************/
// Description: Third part display data
// Author:     Tim.zhu
// Input:       DCAMERA_EXT_DISPLAY_T_PTR : the pointer of display param
// Output:     none
// Return:     SCI_SUCCESS    display success
//             SCI_ERROR      display fail
// Note:       
/******************************************************************************/ 
PUBLIC uint32 DCAMERA_ExtDisplay(DCAMERA_EXT_DISPLAY_T_PTR display_param )
{
    uint32 rtn=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    ISP_REVIEW_PARAM_T isp_review={0x00};

    pContext->target_width=display_param->src_img_size.w;
    pContext->target_height=display_param->src_img_size.h;
    pContext->yyuv_buf.y_addr=display_param->src_yuv_addr.y_chn_addr;
    pContext->yyuv_buf.uv_addr=display_param->src_yuv_addr.u_chn_addr;    
    
    if(DCAMERA_DATA_TYPE_JPEG==display_param->src_format)
    {
        _ExtSetDecoderJpegParam(display_param);
        _DecodeJpegToYYUV();
    }
    
    _ExtSetReviewParam(display_param, &isp_review);

    if((0x00==isp_review.input_size.w)
        ||(0x00==isp_review.disp_rect.w))
    {
        return SCI_ERROR;
    }
    
    ISP_ServiceOpen( );
    if(DCAMERA_OP_SUCCESS == ISP_ServiceSetReviewParam(&isp_review))
    {
        LCD_DisableBlock(MAIN_LCD_ID, 0x02);   
        ISP_ServiceStartReview();	
        LCD_EnableBlock(MAIN_LCD_ID, 0x02);
    }
    else
    {
        rtn=SCI_ERROR;
    }    
    ISP_ServiceClose();
    
    return rtn;
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif// End
