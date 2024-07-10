/*******************************************************************************
 ** FileName:      dc_snapshot.c                                                        *
 ** Author:        jing.li                                                              *
 ** Date:          2008/12/01                                                           *
 ** CopyRight:     2008, Spreatrum, Incoporated, All right reserved                     *
 ** Description:                                                                        *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                                 *
 ** ------------------------------------------------------------------------------------ *
 ** DATE           NAME                   DESCRIPTION                                   *
 ** 2008/12/01     jing.li                 Created                                      *
 *******************************************************************************/

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_ref_dc_trc.h"
#include "sensor_drv.h"
#include "display.h"
#include "dc_app_6x00.h"
#include "dc_snapshot.h"
#include "dc_misc.h"
#include "dc_codec.h"
#include "jpeg_interface_ex.h" 
#include "mem_prod.h"
#include "dc_bufmgr.h"
#include "img_transform.h"
#include "dal_lcd.h"
#include "ref_lcd.h"	//zx added 20110526
#include "rotation_app.h"	//zx added 20110531

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif
/*lint -save -e64 */
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#define CAPTURE_ROTATION g_dcam_context.snapshot_param.rotation_mode
#define CLIP(input,top, bottom) {if (input>top) input = top; if (input < bottom) input = bottom;}

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef ISP_JPEG_PARAM_T ISP_SNAPSHOT_PARAM_T;

typedef enum{
    FOURINONE_CAPTURE,    
    PANORAMA_CAPTURE,
    PERSONAL_CAPTURE,
    INTERPOLATION_CAPTURE,
    ZOOM_CAPTURE,
    JPEG_CAPTURE,
    NORMAL_CAPTURE,
    MAX_TYPE_CAPTURE
}CAPTURE_TYPE_E;

typedef struct scale_datemark_t
{
    uint32 dst_dateimg_addr;
    uint32 src_dateimg_addr;

    DCAMERA_RECT_T imgsrc_rect;
    DCAMERA_RECT_T imgdst_rect;

    DCAMERA_RECT_T imgsrc_date_rect;
    DCAMERA_RECT_T imgdst_date_rect;
    
}SCALE_DATEMARK_T;


/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
LOCAL int32 _check_status(void);
LOCAL int32 _check_parameters(DCAMERA_SNAPSHOT_PARAM_T* param_ptr);
LOCAL int32 _set_parameters(DCAMERA_SNAPSHOT_PARAM_T* param_ptr);
LOCAL int32 _do_snapshot(void);
LOCAL int32 _capture_normal(void);
LOCAL int32 _capture_jpeg(void);
LOCAL int32 _capture_zoom(void);
LOCAL int32 _capture_interpolation(void);
LOCAL uint32 _capture_burst(void);
LOCAL int32 _do_capture_jpeg(void);
LOCAL int32 _do_capture(ISP_SNAPSHOT_PARAM_T* isp_snapshot_ptr);
LOCAL int32 _capture_personal(void);
LOCAL uint32 _get_capture_type(void);
LOCAL int32 _quick_review(void);
LOCAL int32 _quick_review_jpeg(void);
LOCAL int32 _set_burst_parameters(DCAMERA_BURST_SNAPSHOT_PARAM_T  *param_ptr,
									DCAMERA_SNAPSHOT_RETURN_PARAM_T  *return_param_ptr);
LOCAL uint32 _do_burstshot(void);	
LOCAL int32 _capture_review(void);
LOCAL uint32 _DC_CleanCombineParam(void);
LOCAL uint32 _DC_ScaleDataMark(SCALE_DATEMARK_T *scale_datemark_ptr);
LOCAL uint32 _DC_GetScaleDataInfo(ISP_DATA_ADDR_T addr, uint32 width,uint32 height);
/*--------------------------- Local Data ------------------------------------*/
LOCAL const  SNAPSHOT_OPS_HANDLER_T s_snapshot_ops_handlers = {
    _check_status,
    _check_parameters,
    _set_parameters,
    _do_snapshot,
    _set_burst_parameters,
    _do_burstshot,
    _capture_review,
    _DC_CleanCombineParam
};

/*--------------------------- Global Data -----------------------------------*/
PUBLIC ISP_SNAPSHOT_PARAM_T s_st_isp_snapshot_param = {0};
LOCAL ISP_REVIEW_PARAM_T s_isp_quick_view_param = {0};

/*--------------------------- External Data ---------------------------------*/
DECLARE_GLOBAL_CONTEXT;

/*--------------------------- External Function ------------------------------*/
extern PUBLIC DCAM_CONTEXT_T* _GetDCAMContext(void);
extern PUBLIC ERR_LCD_E LCDC_GetConfigBlock(LCD_ID_E lcd_id, uint32 blk_num, BLOCKCFG_T *cfg_ptr);

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/******************************************************************************
// Description: 	get snapshot operation handlers
// Author:     		jing.li
// Input:      		none
// Output:		    none
// Return:          snapshot operation handler struct pointer
// Note:       		
******************************************************************************/
PUBLIC SNAPSHOT_OPS_HANDLER_T* DC_Snapshot_GetOpsHandler(void)
{
    return (SNAPSHOT_OPS_HANDLER_T* )(&s_snapshot_ops_handlers);
}

/******************************************************************************
// Description: 	check dc state before preview
// Author:     		jing.li
// Input:      		none
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		if return successful, the dc state is ok
******************************************************************************/
LOCAL int32 _check_status(void)
{
    if(!DCAMERA_IsOpen()){
        //DC_LOG_TRACE:"DCAM: %s, _check_status, DC is not open"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_140_112_2_18_1_23_41_224,(uint8*)"s", FILENAME_STR);
        return DCAMERA_OP_NOT_OPEN;
    }
    
	if(DC_IN_STATE(DC_STATE_PREVIEWING)){
        //DC_LOG_TRACE:"DCAM: %s, _check_status, still preview, stop it"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_145_112_2_18_1_23_41_225,(uint8*)"s", FILENAME_STR);
		DCAMERA_StopPreview( );
    }

    if(!DC_IN_STATE(DC_STATE_IDLE))
        return DCAMERA_OP_ERROR;
    
    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************
// Description: 	check parameters before preview
// Author:     		jing.li
// Input:      		param_ptr: preview parameters struct pointer
// Output:		    none
// Return:          0           successful
//                  -1          no need to start again
//                  others      failed
// Note:       		if return successful, the dc state is ok
******************************************************************************/
LOCAL int32 _check_parameters(DCAMERA_SNAPSHOT_PARAM_T* param_ptr)
{
    int32 Rtn = DCAMERA_OP_SUCCESS;

    if(SCI_NULL != param_ptr->file_name)
    {
        //DC_LOG_TRACE:"DCAM: capture file name is null err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_170_112_2_18_1_23_41_226,(uint8*)"");
        Rtn=DCAMERA_OP_PARAM_ERR;
    }        

    return Rtn;
}

/******************************************************************************
// Description:	DC buffer right 256 align
// Author:		Tim.zhu
// Input:			source length
// Return:		align length
// Note:			256 bytes align
******************************************************************************/ 
LOCAL uint32 _DC_Buffer_Algin(uint32 pLen , uint16 algin_blk, uint16 algin_bits)
{
    uint32 algin_len=0x00;
    
    algin_len=pLen;
    algin_len=(((algin_len+algin_blk)>>algin_bits)<<algin_bits);

    return algin_len;
}

/******************************************************************************
// Description:	DC buffer right 256 align
// Author:		Tim.zhu
// Input:			source length
// Return:		align length
// Note:			256 bytes align
******************************************************************************/ 
LOCAL void _DC_CalcCombineCaptureBuf(uint32* addr_ptr, uint32* buf_len_ptr)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    
    //DC_LOG_TRACE:"TIM_TEST: _DC_CalcPanoramaBuf 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_203_112_2_18_1_23_41_227,(uint8*)"d",pContext->yyuv_buf.y_addr);
    *addr_ptr=_DC_Buffer_Algin(pContext->yyuv_buf.y_addr, 0xff, 0x08)+_DC_Buffer_Algin(QVGA_WIDTH*QVGA_HEIGHT, 0xff, 0x08)*2*pContext->combine_num;
    *buf_len_ptr = _DC_Buffer_Algin(QVGA_WIDTH*QVGA_HEIGHT, 0xff, 0x08)*2;  
    
    return ;
}

/******************************************************************************
// Description:	DC buffer right 256 align
// Author:		Tim.zhu
// Input:			source length
// Return:		align length
// Note:			256 bytes align
******************************************************************************/ 
LOCAL BOOLEAN _DC_CalcPanoramaIsEnd(void)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();

    if(pContext->yyuv_buf_len<((_DC_Buffer_Algin(QVGA_WIDTH*QVGA_HEIGHT, 0xff, 0x08)<<0x01)*pContext->combine_num)
        ||(pContext->max_combine_num==pContext->combine_num))
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
    
    //return SCI_TRUE;
}

/******************************************************************************
// Description: 	set parameters before preview
// Author:     		jing.li
// Input:      		param_ptr: preview parameters struct pointer
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		set the parameters to the global context
******************************************************************************/
LOCAL int32 _set_parameters(DCAMERA_SNAPSHOT_PARAM_T* param_ptr)
{
    int nRet=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();
    SENSOR_EXP_INFO_T_PTR sensor_info_ptr=pContext->sensor_info_ptr;
    ISP_SNAPSHOT_PARAM_T* isp_snapshot_ptr=&s_st_isp_snapshot_param;
    DCAMERA_RECT_T_PTR st_trim_rect_ptr=&pContext->trim_rect;
    DCAMERA_RECT_T src_rect={0x00}; 
    ISP_SENSOR_SIGNAL_INFO_T isp_sensor_singnal={0};
    uint32 tmp=0x00;

    SNAPSHOT_PARAM_ST=*param_ptr;

    if((SCI_TRUE==pContext->snapshot_param.capture_date.eb)
        &&(NULL!=pContext->snapshot_param.capture_date.date_update))
    {
        pContext->snapshot_param.capture_date.date_update(NULL);
    }

    pContext->snapshot_param.capture_date.osd_offset=0x00;

    DC_GetImgSensorSignalInfo(SNAPSHOT_MODE,&isp_sensor_singnal);
    ISP_ServiceSetSensorSignalInfo(&isp_sensor_singnal);
    
    isp_snapshot_ptr->skip_num=sensor_info_ptr->capture_skip_num;    
    isp_snapshot_ptr->pclk= pContext->sensor_info_ptr->sensor_mode_info[pContext->snapshot_mode].pclk;

    isp_snapshot_ptr->input_size.w = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].width;
    isp_snapshot_ptr->input_size.h = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].height;

    st_trim_rect_ptr->x = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_start_x;
    st_trim_rect_ptr->y = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_start_y;
    st_trim_rect_ptr->w = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_width;
    st_trim_rect_ptr->h = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_height;

    src_rect.x = 0;
    src_rect.y = 0;
    if((sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_width!=pContext->target_width)
        && (ISP_ServiceGetMaxScaleWidth()<pContext->target_width))
    {
        src_rect.w = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_width;
        src_rect.h = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_height;
    }
    else
    {
    	src_rect.w = pContext->target_width;
    	src_rect.h = pContext->target_height;
    }
    
    if(CAPTURE_ROTATION)
    {
        tmp = src_rect.w;
        src_rect.w = src_rect.h;
        src_rect.h = tmp;
    }

    DC_AdjRect_OnDistortion(&pContext->trim_rect, &src_rect, &src_rect, DCAMERA_DISP_FULLSCREEN, DCAMERA_ROTATION_0);

    isp_snapshot_ptr->input_rect = *(ISP_RECT_T*)&pContext->trim_rect;

    if((FOURINONE_CAPTURE==_get_capture_type())
        ||(PANORAMA_CAPTURE==_get_capture_type()))
    {
        isp_snapshot_ptr->output_size.w = QVGA_WIDTH;
        isp_snapshot_ptr->output_size.h = QVGA_HEIGHT;	    
    }
    else
    {
        if((isp_snapshot_ptr->input_rect.w!=pContext->target_width)
            && (ISP_ServiceGetMaxScaleWidth()<pContext->target_width))
        {
            isp_snapshot_ptr->output_size.w = isp_snapshot_ptr->input_rect.w;
            isp_snapshot_ptr->output_size.h = isp_snapshot_ptr->input_rect.h;	    
        }
        else
        {
            isp_snapshot_ptr->output_size.w = pContext->target_width;
            isp_snapshot_ptr->output_size.h = pContext->target_height;
        }
    }

    if(CAPTURE_ROTATION)
    {
        tmp = isp_snapshot_ptr->output_size.w;
        isp_snapshot_ptr->output_size.w = isp_snapshot_ptr->output_size.h;
        isp_snapshot_ptr->output_size.h = tmp;
    }

    pContext->sensor_format = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].image_format;
    isp_snapshot_ptr->sensor_data_format = CONVERT_SENSOR_FORMAT(pContext->sensor_format);

    if(5>pContext->snapshot_param.enc_quality_level)
    {
        pContext->snapshot_param.enc_quality_level=JINF_QUALITY_MIDDLE_LOW;
    }
    else if(7<pContext->snapshot_param.enc_quality_level)
    {
        pContext->snapshot_param.enc_quality_level=JINF_QUALITY_MIDDLE_HIGH;
    }
    else
    {
        pContext->snapshot_param.enc_quality_level=JINF_QUALITY_MIDDLE;
    }

    pContext->jpeg_size=0x00;

    pContext->is_first=SCI_TRUE;

    pContext->exif_eb=SCI_TRUE;

    if(DCAMERA_CAPTURE_PANORAMA==pContext->preview_param.capture_mode)
    {
        nRet=DC_Malloc(DC_STATE_PANORAMA);
        _DC_CalcCombineCaptureBuf(&isp_snapshot_ptr->capture_addr, &isp_snapshot_ptr->capture_buf_size);
    }
    else if(DCAMERA_CAPTURE_FOURINONE==pContext->preview_param.capture_mode)
    {
        nRet=DC_Malloc(DC_STATE_FOURINONE);
        _DC_CalcCombineCaptureBuf(&isp_snapshot_ptr->capture_addr, &isp_snapshot_ptr->capture_buf_size);
    }
    else
    {
        nRet=DC_Malloc(DC_STATE_CAPTURING);
        DC_RETURN_IF_FAIL(nRet, ("DCAM: Capture alloc memory fail err"));

        //DCAM_PRINT:"DCAM: _set_parameters(),snap mode: 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_366_112_2_18_1_23_42_228,(uint8*)"d",SNAPSHOT_MODE);

        
        if(SENSOR_IMAGE_FORMAT_JPEG==DC_GetSensorCurFormat(SNAPSHOT_MODE))
        {
            isp_snapshot_ptr->capture_addr=pContext->src_jpg_addr;
            isp_snapshot_ptr->capture_buf_size=pContext->src_jpg_len;       
        }
        else
        {
            isp_snapshot_ptr->capture_addr=pContext->yyuv_buf.y_addr;
            isp_snapshot_ptr->capture_buf_size=pContext->yyuv_buf_len; 
        }
    }
    
    return nRet;
}

/******************************************************************************
// Description:	set the scale param of capture data
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			scalue output buf share with jpg fw buf
******************************************************************************/ 
LOCAL uint32 _DC_SetCaptureDataScaleParam(ISP_SCALE_PARAM_T* scale_param_ptr)
{
    uint32 rtn=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();

    scale_param_ptr->input_size.w=pContext->actual_width;
    scale_param_ptr->input_size.h=pContext->actual_height;

    if((pContext->target_width!=pContext->actual_width)
        ||(pContext->target_height!=pContext->actual_height))
    {
        scale_param_ptr->input_rect.x=(pContext->snapshot_param.get_data.trim_size.x*pContext->actual_width)/pContext->target_width;
        scale_param_ptr->input_rect.y=(pContext->snapshot_param.get_data.trim_size.y*pContext->actual_height)/pContext->target_height;
        scale_param_ptr->input_rect.w=(pContext->snapshot_param.get_data.trim_size.w*pContext->actual_width)/pContext->target_width;
        scale_param_ptr->input_rect.h=(pContext->snapshot_param.get_data.trim_size.h*pContext->actual_height)/pContext->target_height;
    }
    else
    {
        scale_param_ptr->input_rect.x=pContext->snapshot_param.get_data.trim_size.x;
        scale_param_ptr->input_rect.y=pContext->snapshot_param.get_data.trim_size.y;
        scale_param_ptr->input_rect.w=pContext->snapshot_param.get_data.trim_size.w;
        scale_param_ptr->input_rect.h=pContext->snapshot_param.get_data.trim_size.h;
    }

    scale_param_ptr->input_format=ISP_DATA_YUV422;
    scale_param_ptr->input_addr.y_addr=pContext->yyuv_buf.y_addr;
    scale_param_ptr->input_addr.uv_addr=pContext->yyuv_buf.uv_addr;
    scale_param_ptr->input_buf_size=pContext->yyuv_buf_len;

    scale_param_ptr->output_format=ISP_DATA_YUV422;
    scale_param_ptr->output_size.w=pContext->snapshot_param.get_data.target_size.w;
    scale_param_ptr->output_size.h=pContext->snapshot_param.get_data.target_size.h;
    
    scale_param_ptr->output_addr.y_addr=_DC_Buffer_Algin((uint32)pContext->jpeg_fw_ptr, 0xff, 0x08);
    scale_param_ptr->output_addr.uv_addr=scale_param_ptr->output_addr.y_addr+_DC_Buffer_Algin((scale_param_ptr->output_size.w*scale_param_ptr->output_size.h),0xff,0x08);;
    scale_param_ptr->output_buf_size=pContext->jpeg_fw_len;

    scale_param_ptr->scale_mode=ISP_SERVICE_SCALE_NORMAL;
    scale_param_ptr->is_first=SCI_TRUE;
    scale_param_ptr->is_last=SCI_FALSE;
    scale_param_ptr->get_scale_data=(get_data)_DC_GetScaleDataInfo;

    return rtn;
}

/******************************************************************************
// Description:	dc get the capture data for mmi 
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL uint32 _DC_GetCaptureData(void)
{
    uint32 rtn=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();

    if((0x00!=pContext->snapshot_param.get_data.eb)
        &&(NULL!=pContext->snapshot_param.get_data.callback))
    {
        ISP_SCALE_PARAM_T scale_param={0x00};
        ISP_SCALE_PARAM_T* scale_param_ptr=&scale_param;
        DCAMERA_CALLBACK_DATA_T capture_data={0x00};
        DCAMERA_CALLBACK_DATA_T_PTR capture_data_ptr=&capture_data;

        DC_RETURN_VAL_IF_FAIL_WARNING(_DC_SetCaptureDataScaleParam(scale_param_ptr), DCAMERA_OP_PARAM_ERR, ("DCAM: get capture data scale set param err!"));

        DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceSetScaleParam(scale_param_ptr), DCAMERA_OP_PARAM_ERR, ("DCAM: get capture data do scale set param err!"));

        DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceStartScale(), DCAMERA_OP_ERROR, ("DCAM: get capture data do scale err!"));

        capture_data_ptr->format=scale_param_ptr->output_format;
        SCI_MEMCPY((void*)&(capture_data_ptr->data_addr), (void*)&(pContext->rtn_buf), sizeof(DCAMERA_DATA_ADDR_T));
        SCI_MEMCPY((void*)&(capture_data_ptr->data_len), (void*)&(pContext->rtn_buf_len), sizeof(DCAMERA_DATA_LEN_T));
        SCI_MEMCPY((void*)&(capture_data_ptr->size), (void*)&(pContext->rtn_size), sizeof(DCAMERA_SIZE_T));

        pContext->snapshot_param.get_data.callback((void*)capture_data_ptr);
    }
    else
    {
        //DCAM_PRINT:"DCAM: _DC_GetCaptureData(), eb=0x%x, callback:0x%x, line %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_469_112_2_18_1_23_42_229,(uint8*)"ddd", pContext->snapshot_param.get_data.eb, pContext->snapshot_param.get_data.callback, __LINE__);
    }

    return rtn;	
}

/******************************************************************************
// Description:	quick view jpg 
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL uint32 _DC_DecoderJpeg(uint32 jpg_addr, uint32 jpg_size)
{
    int32 nRet = DCAMERA_OP_SUCCESS;
    LCD_INFO_T lcd_info={0x00};
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();
    JINF_DEC_IN_PARAM_T jpeg_in={0x00};
    JINF_DEC_OUT_PARAM_T jpeg_out={0x00};
    DCAMERA_DATA_ADDR_T addr={0x00};
    DCAMERA_SIZE_T size={0x00};

    SCI_MEMSET(&jpeg_in, 0x00, sizeof(JINF_DEC_IN_PARAM_T));
    SCI_MEMSET(&jpeg_out, 0x00, sizeof(JINF_DEC_OUT_PARAM_T));
    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    jpeg_in.jpeg_buf_ptr=(uint8*)jpg_addr;
    jpeg_in.jpeg_buf_size=jpg_size;
    jpeg_in.crop_rect.x=0x0;
    jpeg_in.crop_rect.y=0x0;   
    jpeg_in.crop_rect.w=pContext->actual_width;
    jpeg_in.crop_rect.h=pContext->actual_height;
    #if 0
    jpeg_in.target_height=DC_MAX(lcd_info.lcd_width, lcd_info.lcd_height);
    jpeg_in.target_width=jpeg_in.crop_rect.w*jpeg_in.target_height/jpeg_in.crop_rect.h;
    jpeg_in.target_width=DC_Algin(jpeg_in.target_width, 0x03, 0x02);
    #else
    jpeg_in.target_width=pContext->sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].width;
    jpeg_in.target_height=pContext->sensor_info_ptr->sensor_mode_info[SENSOR_MODE_PREVIEW_ONE].height;
    #endif
    jpeg_in.target_buf_ptr=(uint8*)DC_RIGHT_TRUNK(pContext->minature_swap_buf_addr,256);
    jpeg_in.target_buf_size=pContext->minature_swap_buf_len;
    jpeg_in.decode_buf_ptr=(uint8*)DC_RIGHT_TRUNK((uint32)pContext->jpeg_fw_ptr,256);
    jpeg_in.decode_buf_size=pContext->jpeg_fw_len;

    jpeg_in.target_data_type=JINF_DATA_TYPE_YUV;
    jpeg_in.read_file_func=PNULL;
    jpeg_in.async_callback=PNULL;
	
    if(JINF_SUCCESS!=IMGJPEG_DecodeEx(&jpeg_in, &jpeg_out))
    {
        nRet = DCAMERA_OP_ENCODE_ERR;
        //DCAM_PRINT:"_quick_review_jpeg(),jpeg decoding error,line %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_521_112_2_18_1_23_42_230,(uint8*)"d",__LINE__);
    }

    pContext->yyuv_buf.y_addr=(uint32)jpeg_out.out_data_info.yuv.yuv_chn.y_chn_ptr;
    pContext->yyuv_buf.uv_addr=(uint32)jpeg_out.out_data_info.yuv.yuv_chn.u_chn_ptr;
    pContext->actual_width=jpeg_out.output_width;
    pContext->actual_height=jpeg_out.output_height;
    pContext->yyuv_buf_len=jpeg_in.target_buf_size;

    addr.y_addr=pContext->yyuv_buf.y_addr;
    addr.uv_addr=pContext->yyuv_buf.uv_addr;
    size.w=pContext->actual_width;
    size.h=pContext->actual_height;
    DC_ImgTerminalLineCopy(&addr, &size, DCAM_DATA_TYPE_YUV422);

    return nRet;	
}


/******************************************************************************
// Description:	Get direct quick view param 
// Author:		Tim.zhu
// Input:			param_ptr: the pointer of review parameter
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL void _DC_GetDirectQuickVeviewParam(ISP_REVIEW_PARAM_T* param_ptr)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();    
    DCAMERA_RECT_T disp_rect={0x00};
    DCAMERA_RECT_T target_rect={0x00};

    param_ptr->input_size.w=pContext->actual_width;
    param_ptr->input_size.h=pContext->actual_height;

    param_ptr->input_rect.x=0;
    param_ptr->input_rect.y=0;
    param_ptr->input_rect.w=pContext->actual_width;
    param_ptr->input_rect.h=pContext->actual_height;

    SCI_MEMCPY((void*)&disp_rect,(void*)&(pContext->preview_param.disp_rect),sizeof(DCAMERA_RECT_T));
    SCI_MEMCPY((void*)&target_rect,(void*)&(pContext->preview_param.target_rect),sizeof(DCAMERA_RECT_T));

    DC_AdjRect_OnDistortion((DCAMERA_RECT_T*)&param_ptr->input_rect, 
                             &disp_rect, 
                             &target_rect, 
                             pContext->preview_param.disp_mode, 
                             pContext->preview_param.rotation_mode);

    param_ptr->input_format=ISP_DATA_YUV422;
    
    param_ptr->src_addr.y_addr=pContext->yyuv_buf.y_addr;
    param_ptr->src_addr.uv_addr=pContext->yyuv_buf.uv_addr;
    param_ptr->src_len.y_len=pContext->actual_width*pContext->actual_height;
    param_ptr->src_len.uv_len=pContext->actual_width*pContext->actual_height;

    param_ptr->disp_format=ISP_ServiceGetDisplayFormat();

    param_ptr->disp_addr=(uint32)pContext->disp_buf_ptr;
    param_ptr->disp_buf_size=pContext->disp_buf_len;
    
    param_ptr->disp_lcd_id=pContext->preview_param.lcd_id;
    param_ptr->disp_block_id=pContext->preview_param.lcd_block_id;
    //display rect
    param_ptr->disp_rect.x=disp_rect.x;
    param_ptr->disp_rect.y=disp_rect.y;
    param_ptr->disp_rect.w=disp_rect.w;
    param_ptr->disp_rect.h=disp_rect.h;

    param_ptr->target_rect.x=target_rect.x;
    param_ptr->target_rect.y=target_rect.y;
    param_ptr->target_rect.w=target_rect.w;
    param_ptr->target_rect.h=target_rect.h;

    param_ptr->rot_buf_len.y_len=pContext->rot_buf_len;
    param_ptr->rot_buf_len.uv_len=DC_Algin((disp_rect.w*disp_rect.h),0xff, 0x08);
    param_ptr->rot_addr.y_addr=(uint32)pContext->rot_buf_ptr;
    param_ptr->rot_addr.uv_addr=(uint32)(pContext->rot_buf_ptr+param_ptr->rot_buf_len.uv_len);

    param_ptr->is_copy_display_data=FALSE;

    param_ptr->mode=ISP_SERVICE_INVALIDATE_SYNC;

    param_ptr->disp_rotation = CONVERT_ROTAION_MODE(pContext->preview_param.rotation_mode);

    SCI_MEMCPY((void*)&(s_isp_quick_view_param), (void*)(param_ptr), sizeof(ISP_REVIEW_PARAM_T));

    s_isp_quick_view_param.input_size.w=disp_rect.w;
    s_isp_quick_view_param.input_size.h=disp_rect.h;

    s_isp_quick_view_param.input_rect.x=0;
    s_isp_quick_view_param.input_rect.y=0;
    s_isp_quick_view_param.input_rect.w=disp_rect.w;
    s_isp_quick_view_param.input_rect.h=disp_rect.h;    

    s_isp_quick_view_param.src_len.y_len=disp_rect.w*disp_rect.h;
    s_isp_quick_view_param.src_len.uv_len=disp_rect.w*disp_rect.h;
    s_isp_quick_view_param.src_addr.y_addr=(uint32)pContext->disp_buf_ptr;
    s_isp_quick_view_param.src_addr.uv_addr=s_isp_quick_view_param.src_addr.y_addr+DC_Algin(s_isp_quick_view_param.src_len.y_len, 0xff, 0x08);

    return ;
}

/******************************************************************************
// Description:	Direct quick view
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL int32 _DC_DirectQuickView(void)
{
    int32 nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();
    ISP_REVIEW_PARAM_T st_isp_quick_view_param={0};

    //DC_LOG_TRACE:"DCAM:_DC_QuickVeviewDirect"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_635_112_2_18_1_23_42_231,(uint8*)"");

    if(SENSOR_IMAGE_FORMAT_JPEG==pContext->sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].image_format)
    {
        DC_RETURN_VAL_IF_FAIL_WARNING(_DC_DecoderJpeg(pContext->src_jpg_addr, pContext->jpeg_size),DCAMERA_OP_ERROR, ("DCAM: _DC_DirectQuickView decoder err!"));
    }
    
    _DC_GetDirectQuickVeviewParam(&st_isp_quick_view_param);

    DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceSetReviewParam(&st_isp_quick_view_param), DCAMERA_OP_PARAM_ERR, ("DCAM: _DC_DirectQuickView set rview param err!"));

    DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceStartReview(), DCAMERA_OP_REVIEW_ERR, ("DCAM: _DC_DirectQuickView rview err!"));
   
    return nRet;
}

/******************************************************************************
// Description:	Swap the panorama buf flow 
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL void _DC_SwitchPanoramaBuffer(IMG_PANORAMA_PARAM_T* panorama_param_ptr)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    ISP_DATA_ADDR_T swap_addr={0x00};
    uint32 swap_yuv_addr=NULL;
    uint32 swap_buf_size=0x00;
    uint8 i=0x00;

    //DC_LOG_TRACE:"DCAM:_DC_SwitchPanoramaBuffer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_665_112_2_18_1_23_42_232,(uint8*)"");

    swap_buf_size=(_DC_Buffer_Algin(pContext->actual_width*pContext->actual_height, 0xff, 0x08)<<0x01);
    swap_yuv_addr=_DC_Buffer_Algin(pContext->combine_buf[0].y_addr, 0xff, 0x08)+swap_buf_size*pContext->combine_num;    
    
    for(i=0; i<(pContext->combine_num>>0x01); i++)
    {
        if(!((0x00==(pContext->combine_num&0x01))&&(i==(pContext->combine_num>>0x01))))
        {
            if(DC_PANORAMA_HORIZONTAL_RIGHT==pContext->preview_param.panorama_dir)
            {
                SCI_MEMCPY((void *)swap_yuv_addr, (void *)(pContext->combine_buf[pContext->combine_num-0x01-i].y_addr), swap_buf_size);
                SCI_MEMCPY((void *)(pContext->combine_buf[pContext->combine_num-0x01-i].y_addr), (void *)(pContext->combine_buf[i].y_addr), swap_buf_size);
                SCI_MEMCPY((void *)(pContext->combine_buf[i].y_addr), (void *)swap_yuv_addr, swap_buf_size);
            }

            swap_addr.y_addr=pContext->combine_buf[pContext->combine_num-0x01-i].y_addr;
            swap_addr.uv_addr=pContext->combine_buf[pContext->combine_num-0x01-i].uv_addr;
            pContext->combine_buf[pContext->combine_num-0x01-i].y_addr=pContext->combine_buf[i].y_addr;
            pContext->combine_buf[pContext->combine_num-0x01-i].uv_addr=pContext->combine_buf[i].uv_addr;
            pContext->combine_buf[i].y_addr=swap_addr.y_addr;
            pContext->combine_buf[i].uv_addr=swap_addr.uv_addr; 
        }
        
    }

    return ;
}

/******************************************************************************
// Description:	Set Panorama parameter 
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL uint32 _DC_SetPanoramaParam(IMG_PANORAMA_PARAM_T* panorama_param_ptr)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    uint8 i=0x00;
    
    panorama_param_ptr->dir=pContext->preview_param.panorama_dir;/*lint !e64*/
    panorama_param_ptr->input_size.width=pContext->actual_width;
    panorama_param_ptr->input_size.height=pContext->actual_height;
    panorama_param_ptr->input_num=pContext->combine_num;    
    panorama_param_ptr->swap_buf_addr=(uint32)pContext->cx_buf_ptr;
    panorama_param_ptr->swap_buf_len=pContext->cx_buf_len;
    
    if(pContext->cx_buf_len<(uint32)(pContext->actual_width<<0x03))
    {
        //DC_LOG_TRACE:"DCAM:_DC_SetPanoramaParam the swap buf is not enough err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_714_112_2_18_1_23_43_233,(uint8*)"");
        return DCAMERA_OP_NO_ENOUGH_MEMORY;
    }
    
    for(i=0x00; i<pContext->combine_num; i++)
    {
        panorama_param_ptr->input_img[i]=&pContext->combine_buf[i];/*lint !e64*/
    }
    
    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************
// Description:	Set panorama encoder parameter
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL uint32 _DC_SetPanoramaEncoderParam(IMG_PANORAMA_PARAM_T* panorama_param_ptr)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();

    if((NULL==panorama_param_ptr->output_addr.y_addr)
        ||(0x00==panorama_param_ptr->output_size.width)
        ||(0x00==panorama_param_ptr->output_size.height))
    {
        //SCI_TRACE_ISP:"DCAM:_DC_SetPanoramaEncoderParam err out_addr:0x%x, out_w:%d, out_h:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_740_112_2_18_1_23_43_234,(uint8*)"ddd",panorama_param_ptr->output_addr.y_addr, panorama_param_ptr->output_size.width, panorama_param_ptr->output_size.width);
        return DCAMERA_OP_NO_ENOUGH_MEMORY;
    }

    if(pContext->quick_view_buf.y_addr<(panorama_param_ptr->output_addr.uv_addr+_DC_Buffer_Algin((panorama_param_ptr->output_size.width*panorama_param_ptr->output_size.height), 0xff, 0x08)))
    {
        //SCI_TRACE_ISP:"DCAM:_DC_SetPanoramaEncoder buf err quick_addr:0x%x, data_addr:0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_746_112_2_18_1_23_43_235,(uint8*)"dd",pContext->quick_view_buf.y_addr, (panorama_param_ptr->output_addr.uv_addr+_DC_Buffer_Algin((panorama_param_ptr->output_size.width*panorama_param_ptr->output_size.height), 0xff, 0x08)));
        return DCAMERA_OP_NO_ENOUGH_MEMORY;
    }
    
    pContext->target_width=panorama_param_ptr->output_size.width;
    pContext->target_height=panorama_param_ptr->output_size.height;     

    pContext->actual_width=panorama_param_ptr->output_size.width;
    pContext->actual_height=panorama_param_ptr->output_size.height;        

    pContext->yyuv_buf.y_addr=panorama_param_ptr->output_addr.y_addr;
    pContext->yyuv_buf.uv_addr=panorama_param_ptr->output_addr.uv_addr;
    pContext->yyuv_buf_len=_DC_Buffer_Algin(pContext->actual_width*pContext->actual_height, 0xff, 0x08)<<0x01; 

    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************
// Description:	Clean panorama parameter 
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL uint32 _DC_CleanCombineParam(void)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    uint8 i=0x00;

    for(i=0x00; i<MAX_COMBINE_NUM; i++)
    {
        pContext->combine_buf[i].y_addr=NULL;
        pContext->combine_buf[i].uv_addr=NULL;  
    }

    pContext->combine_num=0x00;
    
    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************/
//  Description: Set soft scale down y uv 422 parameter 
//  Author:      Tim.zhu
//  Input:     
// 		    YUV_IMG_SCALE_INFO_T: scale info parameters
//  Output: 
//  Return: none
//  Note:   
/******************************************************************************/
PUBLIC void _DC_SetSoftScaleDownYuv422Param(IMG_SCALE_PARAM_T* img_scale_ptr, ISP_SCALE_PARAM_T* isp_scale_ptr)
{  
    DCAM_CONTEXT_T* pContext    = _GetDCAMContext();

    img_scale_ptr->input_size.w=isp_scale_ptr->input_size.w;
    img_scale_ptr->input_size.h=isp_scale_ptr->input_size.h;  

    img_scale_ptr->input_rect.x=isp_scale_ptr->input_rect.x;
    img_scale_ptr->input_rect.y=isp_scale_ptr->input_rect.y;  
    img_scale_ptr->input_rect.w=isp_scale_ptr->input_rect.w;
    img_scale_ptr->input_rect.h=isp_scale_ptr->input_rect.h;  

    img_scale_ptr->input_yuv.y_addr=isp_scale_ptr->input_addr.y_addr;
    img_scale_ptr->input_yuv.uv_addr=isp_scale_ptr->input_addr.uv_addr; 
    
    img_scale_ptr->output_size.w=isp_scale_ptr->output_size.w;
    img_scale_ptr->output_size.h=isp_scale_ptr->output_size.h;        

    img_scale_ptr->output_yuv.y_addr=isp_scale_ptr->output_addr.y_addr;
    img_scale_ptr->output_yuv.uv_addr=_DC_Buffer_Algin((isp_scale_ptr->output_addr.y_addr+(isp_scale_ptr->output_size.w*isp_scale_ptr->output_size.h)),0xff,0x08);  

    pContext->quick_view_buf.y_addr=img_scale_ptr->output_yuv.y_addr;
    pContext->quick_view_buf.uv_addr=img_scale_ptr->output_yuv.uv_addr;
    
}

/******************************************************************************
// Description: 	start Panorama capture
// Author:     		Tim.zhu
// Input:      		none
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		
******************************************************************************/
LOCAL int32 _DC_PanoramaCapture(void)
{
    int32 nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext    = _GetDCAMContext();
    ISP_SNAPSHOT_PARAM_T *isp_snapshot_ptr = &s_st_isp_snapshot_param;
    IMG_PANORAMA_PARAM_T panorama_param={0x00};
    IMG_PANORAMA_PARAM_T* panorama_param_ptr=&panorama_param;    

    //DC_LOG_TRACE:"DCAM:_DC_PanoramaCapture"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_834_112_2_18_1_23_43_236,(uint8*)"");

    isp_snapshot_ptr->output_data_format = ISP_DATA_YUV422;	
    isp_snapshot_ptr->sensor_data_format = CONVERT_SENSOR_FORMAT(SENSOR_IMAGE_FORMAT_YUV422);

    DC_RETURN_VAL_IF_FAIL_WARNING(_do_capture(isp_snapshot_ptr), SCI_ERROR, ("DCAM: capture yuv fail err!"));

    pContext->combine_buf[pContext->combine_num].y_addr=pContext->yyuv_buf.y_addr;
    pContext->combine_buf[pContext->combine_num].uv_addr=pContext->yyuv_buf.uv_addr;    

    DC_RETURN_VAL_IF_FAIL_WARNING(_DC_DirectQuickView(), SCI_ERROR, ("DCAM: Panorama direct quick view err"));
    
    pContext->combine_num++;

    if(_DC_CalcPanoramaIsEnd())
    {
        _DC_SwitchPanoramaBuffer(panorama_param_ptr);  

        DC_RETURN_VAL_IF_FAIL_WARNING(_DC_SetPanoramaParam(panorama_param_ptr), DCAMERA_OP_PARAM_ERR, ("DCAM: Panorama param err"));

        DC_RETURN_VAL_IF_FAIL_WARNING(Img_PanoramaMosaic(panorama_param_ptr), DCAMERA_OP_ERROR, ("DCAM: Panorama mosaic err"));

        DC_RETURN_VAL_IF_FAIL_WARNING(_DC_SetPanoramaEncoderParam(panorama_param_ptr), DCAMERA_OP_PARAM_ERR, ("DCAM: Panorama encoder param err"));

        DC_RETURN_VAL_IF_FAIL_WARNING(_quick_review(), DCAMERA_OP_ERROR, ("DCAM: Panorama quick view err"));

        _DC_CleanCombineParam();
        
        if(DCAMERA_OP_SUCCESS  != _EncodeYYUVToJpeg((uint32 *)(pContext->jpeg_buf_ptr),(uint32 *)(&(pContext->jpeg_size))))
        {
            pContext->jpeg_buf_ptr = PNULL;
            pContext->jpeg_size=NULL;
            nRet= DCAMERA_OP_ENCODE_ERR;            
        }

    }
    else
    {
        if((PNULL!=pContext->snapshot_param.combine_callback))
        {
            //DC_LOG_TRACE:"DCAM: Panorama capture call back is pnull err!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_874_112_2_18_1_23_43_237,(uint8*)"");
        }

        pContext->snapshot_param.combine_callback(DCAMERA_OP_SUCCESS, (void*)ISP_ServiceGetReviewDisplayBuf(), &pContext->disp_rect);
    }
    
    return nRet;

}

/******************************************************************************
// Description:	Combine the two image of FourInOne 
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL void _DC_CombineFourInOneImg(uint32 dst_addr, uint32 img1_addr, uint32 img2_addr, uint16 img_width, uint16 img_height)
{
    uint32 dst_buf_addr=dst_addr;
    uint32 img1_buf_addr=img1_addr;
    uint32 img2_buf_addr=img2_addr;    
    uint16 data_len=img_width;
    uint16 i=0x00;
    
    for(i=0x00; i<img_height; i++)
    {
        SCI_MEMCPY((void *)dst_buf_addr, (void *)img1_buf_addr, data_len);
        dst_buf_addr+=data_len;
        img1_buf_addr+=data_len;
        
        SCI_MEMCPY((void *)dst_buf_addr, (void *)img2_buf_addr, data_len);
        dst_buf_addr+=data_len;
        img2_buf_addr+=data_len;        
    }

    return ;
}

/******************************************************************************
// Description:	Swap the panorama buf flow 
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL void _DC_SwitchFourInOneBuffer(void)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    uint32 swap_yuv_addr=NULL;
    uint32 swap_data_size=0x00;

    swap_yuv_addr=pContext->combine_buf[pContext->combine_num-0x01].uv_addr+_DC_Buffer_Algin(pContext->actual_width*pContext->actual_height, 0xff, 0x08);
    swap_data_size=_DC_Buffer_Algin(pContext->actual_width*pContext->actual_height, 0xff, 0x08)<<0x02;
    
    _DC_CombineFourInOneImg(swap_yuv_addr,
                            pContext->combine_buf[0].y_addr,
                            pContext->combine_buf[1].y_addr,
                            pContext->actual_width,
                            pContext->actual_height*2);

    SCI_MEMCPY((void *)pContext->combine_buf[0].y_addr, (void *)swap_yuv_addr, swap_data_size);    
    
    _DC_CombineFourInOneImg(swap_yuv_addr,
                            pContext->combine_buf[2].y_addr,
                            pContext->combine_buf[3].y_addr,
                            pContext->actual_width,
                            pContext->actual_height*2);

    SCI_MEMCPY((void *)pContext->combine_buf[2].y_addr, (void *)swap_yuv_addr, swap_data_size);   

    swap_data_size=_DC_Buffer_Algin(pContext->actual_width*pContext->actual_height, 0xff, 0x08)<<0x01; 

    SCI_MEMCPY((void *)swap_yuv_addr, (void *)pContext->combine_buf[1].y_addr, swap_data_size);       

    SCI_MEMCPY((void *)pContext->combine_buf[1].y_addr, (void *)pContext->combine_buf[2].y_addr, swap_data_size);           
    
    SCI_MEMCPY((void *)pContext->combine_buf[2].y_addr, (void *)swap_yuv_addr, swap_data_size);     
    
    //DC_LOG_TRACE:"DCAM:_DC_SwitchFourInOneBuffer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_951_112_2_18_1_23_43_238,(uint8*)"");

    return ;
}

/******************************************************************************
// Description:	Set panorama encoder parameter
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL uint32 _DC_SetFourInoneEncoderParam(void)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();

    
    pContext->target_width=pContext->preview_param.target_rect.w;
    pContext->target_height=pContext->preview_param.target_rect.h;     

    pContext->actual_width<<=0x01;
    pContext->actual_height<<=0x01;

    pContext->yyuv_buf.y_addr=pContext->combine_buf[0].y_addr;
    pContext->yyuv_buf.uv_addr=pContext->combine_buf[2].y_addr;
    pContext->yyuv_buf_len=_DC_Buffer_Algin(pContext->actual_width*pContext->actual_height, 0xff, 0x08)<<0x01; 

    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************
// Description:	Set panorama encoder parameter
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL uint32 _DC_SetFourInoneEncoder(void)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    uint32 nRet=DCAMERA_OP_SUCCESS;

    if(pContext->actual_width<pContext->target_width)
    {
        nRet=_EncodeYYUVSliceToJpeg();
    }
    else
    {
        nRet=_EncodeYYUVToJpeg((uint32 *)(pContext->jpeg_buf_ptr),(uint32 *)(&(pContext->jpeg_size)));     
    }

    if(DCAMERA_OP_SUCCESS!=nRet)
    {
        pContext->jpeg_buf_ptr = PNULL;
        pContext->jpeg_size=NULL;
        //DC_LOG_TRACE:"DCAM:_DC_SetFourInoneEncoder encoder err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1004_112_2_18_1_23_43_239,(uint8*)"");
        return DCAMERA_OP_ENCODE_ERR;            
    }    

    return nRet;
}

/*
    uint32 dst_dateimg_addr;
    uint32 src_dateimg_addr;

    IMGREF_RECT_T imgsrc_rect;
    IMGREF_RECT_T imgdst_rect;

    IMGREF_RECT_T imgsrc_date_rect;
    IMGREF_RECT_T imgdst_date_rect;
*/
LOCAL uint32 _DC_GetDateRectParam(DCAMERA_RECT_T trim_rect,
                                    DCAMERA_RECT_T *imgsrc_rect_ptr,               
                                    DCAMERA_RECT_T *imgdst_rect_ptr)
{
    uint tempSize = 0;
    //judge if the date rect is out of the trim rect range
#if 1
    //SCI_TRACE_LOW:"trim rect x:%d, y:%d, w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1027_112_2_18_1_23_43_240,(uint8*)"dddd", trim_rect.x, trim_rect.y, trim_rect.w, trim_rect.h);
    //SCI_TRACE_LOW:"src_rect x:%d, y:%d, w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1028_112_2_18_1_23_43_241,(uint8*)"dddd", imgsrc_rect_ptr->x, imgsrc_rect_ptr->y, imgsrc_rect_ptr->w, imgsrc_rect_ptr->h);
#endif

    if((imgsrc_rect_ptr->x < (trim_rect.x+trim_rect.w-1))&&   \
        (imgsrc_rect_ptr->y < (trim_rect.y+trim_rect.h-1)))
    {
        //caculate the size
        imgdst_rect_ptr->x = imgsrc_rect_ptr->x;
        tempSize =((trim_rect.x + trim_rect.w - imgsrc_rect_ptr->x)>>1)<<1;
        if(tempSize < imgsrc_rect_ptr->w)
        {
            imgdst_rect_ptr->w = tempSize;
        }
        else
        {
            imgdst_rect_ptr->w = imgsrc_rect_ptr->w;
        }
        imgdst_rect_ptr->y = imgsrc_rect_ptr->y;
        tempSize =((trim_rect.y + trim_rect.h - imgsrc_rect_ptr->y)>>1)<<1;
        if(tempSize < imgsrc_rect_ptr->h)
        {
            imgdst_rect_ptr->h = tempSize;
        }
        else
        {
            imgdst_rect_ptr->h = imgsrc_rect_ptr->h;
        }
    }
    else
    {
        imgdst_rect_ptr->x = 0;
        imgdst_rect_ptr->y = 0;
        imgdst_rect_ptr->w = 0;
        imgdst_rect_ptr->h = 0;
        //SCI_TRACE_LOW:"DCAM: snapshot the data rect out of trim range"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1062_112_2_18_1_23_43_242,(uint8*)"");
        return SCI_ERROR;
    }

#if 1
    //SCI_TRACE_LOW:"dst_rect x:%d, y:%d, w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1071_112_2_18_1_23_43_243,(uint8*)"dddd", imgdst_rect_ptr->x, imgdst_rect_ptr->y, imgdst_rect_ptr->w, imgdst_rect_ptr->h);
#endif    

    return SCI_SUCCESS;
}

LOCAL void _DC_GetDateRectData(       DCAMERA_RECT_T *dst_rect_ptr,            
                                    DCAMERA_RECT_T *src_rect_ptr,            
                                    uint32 *src_date_addr_ptr,
                                    uint32 *dst_date_addr_ptr)
{
    uint32 j = 0;

    uint32 *src_ptr = src_date_addr_ptr;
    uint32 *dst_ptr = dst_date_addr_ptr;

    uint32 dst_rect_width = dst_rect_ptr->w;
    uint32 dst_rect_h = dst_rect_ptr->h;
    uint32 src_rect_w = src_rect_ptr->w;
 
    //get the date rect date in the trim from source 
    for(j = 0; j < dst_rect_h; j++)        //column
    {   
        SCI_MEMCPY(dst_ptr,src_ptr,(dst_rect_width<<2));   //line
        dst_ptr = dst_ptr + dst_rect_width;
        src_ptr = src_ptr + src_rect_w;
    }

    return;
}

LOCAL uint32 _DC_Proc_DispDate(DCAMERA_RECT_T *imgsrc_rect_ptr,   DCAMERA_RECT_T *imgdst_rect_ptr, DCAMERA_RECT_T *trim_rect_ptr, ISP_DATA_ADDR_T * img_addr_ptr)
{
    DCAM_CONTEXT_T* pContext    = _GetDCAMContext();
    SCALE_DATEMARK_T scale_datemark_output = {0};
    SCALE_DATEMARK_T scale_datemark = {0};
    IMGREF_YUV_SRC param_src  ={0};
    IMGREF_OSD_T capture_date = {0};
    DCAMERA_RECT_T trim_rect_output = {0};    //the trim rect after scaling to output

    uint32 dstdate_addr = 0;
    uint32 img_ratio = 0;
    uint32 img_size = 0;
    uint32 rotdate_addr = 0;
    uint32* rotdata_align_addr = 0;

    //SCI_TRACE_LOW:"DCAM: _DC_Proc_DispDate"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1117_112_2_18_1_23_43_244,(uint8*)"");
    
#if 0        
    //for debug
    //SCI_TRACE_LOW:"DCAM: org_dateimg_addr 0x%x, w: %d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1121_112_2_18_1_23_43_245,(uint8*)"ddd", scale_datemark_output.src_dateimg_addr, scale_datemark_output.imgsrc_date_rect.w, scale_datemark_output.imgsrc_date_rect.h);
    SCI_PASSERT(0,("DCAM: src_dateimg_addr 0x%x, w: %d, h:%d", scale_datemark.src_dateimg_addr, scale_datemark.imgsrc_date_rect.w, scale_datemark.imgsrc_date_rect.h));
#endif

    scale_datemark.src_dateimg_addr = (uint32)pContext->snapshot_param.capture_date.osd_addr.y_addr;
    scale_datemark.imgdst_rect = *imgdst_rect_ptr;
    scale_datemark.imgsrc_rect = *imgsrc_rect_ptr;
    scale_datemark.imgsrc_date_rect = pContext->snapshot_param.capture_date.overlay_rect;
    
    DC_GetMutex();  //to avoid memory leak as delete thread

    //process 
    if(CAPTURE_ROTATION)
    {
        register uint32 i,j;
        register uint32 width, height;
        register uint32 *src_ptr = (uint32 *)scale_datemark.src_dateimg_addr; 
        register uint32 *dst_ptr= PNULL;
        rotdate_addr =(uint32)SCI_ALLOCA(scale_datemark.imgsrc_date_rect.w*scale_datemark.imgsrc_date_rect.h*4+255);
        
        if(PNULL == rotdate_addr)
        {
            DC_PutMutex(); 
            return 1;
        }
        rotdata_align_addr = (uint32 *)(((rotdate_addr+3)>>2)<<2); 
        dst_ptr = (uint32 *)rotdata_align_addr;

        width = scale_datemark.imgsrc_date_rect.w;
        height = scale_datemark.imgsrc_date_rect.h;
        //do rotation
        for(i=0; i<height; i++)
        {
            dst_ptr = rotdata_align_addr + height*(width-1)+i;
            for(j=0; j<width; j++)    
            {
                  *dst_ptr = *src_ptr++;
                  dst_ptr = dst_ptr-height;
            }
        }

        //change rect
        scale_datemark.src_dateimg_addr = (uint32)rotdata_align_addr;
        i = scale_datemark.imgsrc_date_rect.x ;
        scale_datemark.imgsrc_date_rect.x = scale_datemark.imgsrc_date_rect.y;
        scale_datemark.imgsrc_date_rect.y =  scale_datemark.imgsrc_rect.h-i-width ;
        scale_datemark.imgsrc_date_rect.w = height;
        scale_datemark.imgsrc_date_rect.h = width;        
                
    }

    img_ratio = (scale_datemark.imgdst_rect.w <<8)/ scale_datemark.imgsrc_rect.w ;
    img_size =( (( (scale_datemark.imgsrc_date_rect.w * img_ratio) >> 8)+7)>>3)<<3;
    img_size *= ((( (scale_datemark.imgsrc_date_rect.h * img_ratio)>>8)+7)>>3)<<3;
    img_size = (img_size <<2) + 1024;

    dstdate_addr = (uint32)SCI_ALLOCA(img_size);
    if(PNULL == dstdate_addr)
    {
        if(PNULL !=rotdate_addr)
        {
            SCI_FREE(rotdate_addr);
            rotdate_addr = PNULL;
        }
        DC_PutMutex(); 
        return 1;  //error return
    }
    scale_datemark.dst_dateimg_addr =  dstdate_addr;

    _DC_ScaleDataMark(&scale_datemark);

    //calculate the image trim rect after scale
    if(trim_rect_ptr->x)
    {
        trim_rect_output.x = (((trim_rect_ptr->x * img_ratio)>>8)>>1)<<1;
    }
    if(trim_rect_ptr->y)
    {
        trim_rect_output.y = (((trim_rect_ptr->y * img_ratio)>>8)>>1)<<1;
    }
    trim_rect_output.w = pContext->preview_param.target_rect.w;
    trim_rect_output.h = pContext->preview_param.target_rect.h;
    
#if 0        
    //for debug
    //SCI_TRACE_LOW:"DCAM: very org_dateimg_addr 0x%x, w: %d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1207_112_2_18_1_23_44_246,(uint8*)"ddd""DCAM: very org_dateimg_addr 0x%x, w: %d, h:%d");
        pContext->snapshot_param.capture_date.overlay_rect.w, 
        pContext->snapshot_param.capture_date.overlay_rect.h);

    //SCI_TRACE_LOW:"DCAM: very target_img_addr 0x%x, w: %d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1214_112_2_18_1_23_44_247,(uint8*)"ddd", (uint32)img_addr_ptr->y_addr, pContext->preview_param.target_rect.w, pContext->preview_param.target_rect.h);
#endif

    //get the trim rect and data of date
    _DC_GetDateRectParam(trim_rect_output,                      \
        &(scale_datemark.imgdst_date_rect),                     \
        &(scale_datemark_output.imgsrc_date_rect));        //get gate img trim rect

    if(!scale_datemark_output.imgsrc_date_rect.w)           //avoid mem leak      
    {
        if(PNULL !=rotdate_addr)
        {
            SCI_FREE(rotdate_addr);
            rotdate_addr = PNULL;
        }  
        if(PNULL !=dstdate_addr)        //lint !e774
        {
            SCI_FREE(dstdate_addr);
            dstdate_addr = PNULL;
        }
        DC_PutMutex(); 
        return SCI_ERROR;
    }
    
    scale_datemark_output.src_dateimg_addr =(uint32)SCI_ALLOCA(scale_datemark_output.imgsrc_date_rect.w*scale_datemark_output.imgsrc_date_rect.h*4+255);

    if(PNULL == scale_datemark_output.src_dateimg_addr)     //avoid mem leak
	{
        if(PNULL !=rotdate_addr)
        {
            SCI_FREE(rotdate_addr);
            rotdate_addr = PNULL;
        }  
        if(PNULL !=dstdate_addr)        //lint !e774
        {
            SCI_FREE(dstdate_addr);
            dstdate_addr = PNULL;
        }
        DC_PutMutex(); 
	 	return 1;
	}
    
    _DC_GetDateRectData(&scale_datemark_output.imgsrc_date_rect,         
                         &scale_datemark.imgdst_date_rect,
        (uint32 *)scale_datemark.dst_dateimg_addr,          
        (uint32 *)scale_datemark_output.src_dateimg_addr);     //get date img data


    //SCI_TraceLow:"output date addr 0x%x, width %d, height %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1262_112_2_18_1_23_44_248,(uint8*)"ddd", scale_datemark_output.src_dateimg_addr , scale_datemark_output.imgsrc_date_rect.w, scale_datemark_output.imgsrc_date_rect.h);

    //ready to combine the image and date image, calculate params
    param_src.src.y_addr = img_addr_ptr->y_addr;
    param_src.src.uv_addr = img_addr_ptr->uv_addr;

    param_src.src_buf_rect.x = 0;
    param_src.src_buf_rect.y = 0;    
    param_src.src_buf_rect.w = pContext->preview_param.target_rect.w;
    param_src.src_buf_rect.h = pContext->preview_param.target_rect.h;

    param_src.src_size.w = param_src.src_buf_rect.w;
    param_src.src_size.h = param_src.src_buf_rect.h;    

    capture_date.color_key = 0;
    capture_date.dataformat = IMGREF_FORMAT_ARGB888;

    capture_date.overlay_rect.x = scale_datemark_output.imgsrc_date_rect.x - trim_rect_output.x;
    capture_date.overlay_rect.y = scale_datemark_output.imgsrc_date_rect.y - trim_rect_output.y;
    capture_date.overlay_rect.w = scale_datemark_output.imgsrc_date_rect.w;
    capture_date.overlay_rect.h = scale_datemark_output.imgsrc_date_rect.h;

    capture_date.osd_addr.y_addr= scale_datemark_output.src_dateimg_addr;

    //SCI_TraceLow:"cap date addr 0x%x, x %d y %dwidth %d, height %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1286_112_2_18_1_23_44_249,(uint8*)"ddddd", capture_date.osd_addr.y_addr ,capture_date.overlay_rect.x, capture_date.overlay_rect.y, capture_date.overlay_rect.w, capture_date.overlay_rect.h);
    
    DC_ArgbOverlayer(&param_src, &capture_date);

#if 0        
    //SCI_TraceLow:"brush date addr 0x%x, width %d, height %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1291_112_2_18_1_23_44_250,(uint8*)"ddd", param_src.src.y_addr, param_src.src_buf_rect.w, param_src.src_buf_rect.h);
//    SCI_PASSERT(0,("DCAM: quick view addr 0x%x, w: %d, h:%d",param_src.src.y_addr, param_src.src_buf_rect.w, param_src.src_buf_rect.h));
#endif

    if(PNULL !=dstdate_addr)       //lint !e774 
    {
        SCI_FREE(dstdate_addr);
        dstdate_addr = PNULL;
    }

    if(PNULL !=rotdate_addr)        //lint !e774 
    {
        SCI_FREE(rotdate_addr);
        rotdate_addr = PNULL;
    }
    
    if(PNULL != scale_datemark_output.src_dateimg_addr) 
    {
	    SCI_FREE(scale_datemark_output.src_dateimg_addr);
        scale_datemark_output.src_dateimg_addr = PNULL;
 	}
    
    DC_PutMutex();   
    return 0;
}



LOCAL uint32 _DC_ScaleDataMark(SCALE_DATEMARK_T *scale_datemark_ptr)
{
    IMGPROC_SCALE_IN_T scale_in_param = {0};
    IMGPROC_SCALE_OUT_T scale_out_param = {0};
    IMGREF_RECT_T imgdst_date_rect = {0};
    uint32 date_endx = scale_datemark_ptr->imgsrc_date_rect.x + scale_datemark_ptr->imgsrc_date_rect.w-1;
    uint32 date_endy = scale_datemark_ptr->imgsrc_date_rect.y + scale_datemark_ptr->imgsrc_date_rect.h-1;    

    uint32 x_offset = 0;
    uint32 y_offset = 0;

    uint32 img_ratio = 0;
    
#if 0
    //SCI_TRACE_LOW:"scale datemark src zie x: %d,y: %d,w: %d,h: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1337_112_2_18_1_23_44_251,(uint8*)"dddd",scale_datemark_ptr->imgsrc_date_rect.x,scale_datemark_ptr->imgsrc_date_rect.y,scale_datemark_ptr->imgsrc_date_rect.w,scale_datemark_ptr->imgsrc_date_rect.h);
#endif

    //dst/src
    img_ratio = (scale_datemark_ptr->imgdst_rect.w <<8)/scale_datemark_ptr->imgsrc_rect.w ;
	x_offset = scale_datemark_ptr->imgsrc_rect.x ;
	y_offset = scale_datemark_ptr->imgsrc_rect.y ;

    //scaling use the end point
    imgdst_date_rect.w = ( (((scale_datemark_ptr->imgsrc_date_rect.w * img_ratio )>>8)+1)>>1)<<1;
    imgdst_date_rect.h = ( (((scale_datemark_ptr->imgsrc_date_rect.h * img_ratio)>>8)+1)>>1)<<1;
    imgdst_date_rect.x = ((((date_endx * img_ratio) >> 8)>>1)<<1) -  imgdst_date_rect.w+1;
    imgdst_date_rect.y = ((((date_endy * img_ratio) >> 8)>>1)<<1) - imgdst_date_rect.h+1;

    scale_in_param.is_isp_open = TRUE;
    scale_in_param.scale_mode = IMGPROC_SCALE_MODE_NO_DISTORT;
    scale_in_param.src_chn.chn0.ptr = scale_datemark_ptr->src_dateimg_addr;
    scale_in_param.src_chn.chn0.size =  scale_datemark_ptr->imgsrc_date_rect.w * scale_datemark_ptr->imgsrc_date_rect.h * 4;
    scale_in_param.src_format = IMGREF_FORMAT_ARGB888;
    scale_in_param.src_size.w = scale_datemark_ptr->imgsrc_date_rect.w ;
    scale_in_param.src_size.h = scale_datemark_ptr->imgsrc_date_rect.h ;
    scale_in_param.src_trim_rect.x = 0;
    scale_in_param.src_trim_rect.y= 0;
    scale_in_param.src_trim_rect.w = scale_datemark_ptr->imgsrc_date_rect.w;
    scale_in_param.src_trim_rect.h = scale_datemark_ptr->imgsrc_date_rect.h;

    scale_in_param.target_buf.ptr = scale_datemark_ptr->dst_dateimg_addr;
    scale_in_param.target_buf.size =( (imgdst_date_rect.w * imgdst_date_rect.h )<< 2) +1024;
    scale_in_param.target_format = IMGREF_FORMAT_ARGB888;
    scale_in_param.target_size.w =  imgdst_date_rect.w;
    scale_in_param.target_size.h =  imgdst_date_rect.h;

    IMGPROC_ScaleImage(&scale_in_param, &scale_out_param);

    scale_datemark_ptr->imgdst_date_rect.x = imgdst_date_rect.x;
    scale_datemark_ptr->imgdst_date_rect.y = imgdst_date_rect.y;
    scale_datemark_ptr->imgdst_date_rect.w = scale_out_param.output_size.w;
    scale_datemark_ptr->imgdst_date_rect.h = scale_out_param.output_size.h;
    scale_datemark_ptr->dst_dateimg_addr = scale_out_param.output_chn.chn0.ptr;

#if 0
    //SCI_TRACE_LOW:"scale datemark dst zie x: %d,y: %d,w: %d,h: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1382_112_2_18_1_23_44_252,(uint8*)"dddd",scale_datemark_ptr->imgdst_date_rect.x,scale_datemark_ptr->imgdst_date_rect.y,scale_datemark_ptr->imgdst_date_rect.w,scale_datemark_ptr->imgdst_date_rect.h);
#endif

    return SCI_SUCCESS;
}




/******************************************************************************
// Description: 	start Four In One capture
// Author:     		Tim.zhu
// Input:      		none
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		
******************************************************************************/
LOCAL int32 _DC_FourInOneCapture(void)
{
    int32 nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext    = _GetDCAMContext();
    ISP_SNAPSHOT_PARAM_T *isp_snapshot_ptr = &s_st_isp_snapshot_param; 

    //DC_LOG_TRACE:"DCAM:_DC_FourInOneCapture"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1405_112_2_18_1_23_44_253,(uint8*)"");

    isp_snapshot_ptr->output_data_format = ISP_DATA_YUV422;	
    isp_snapshot_ptr->sensor_data_format = CONVERT_SENSOR_FORMAT(SENSOR_IMAGE_FORMAT_YUV422);

    DC_RETURN_VAL_IF_FAIL_WARNING(_do_capture(isp_snapshot_ptr), DCAMERA_OP_ERROR, ("DCAM: FourInOne capture yuv err!"));

    pContext->combine_buf[pContext->combine_num].y_addr=pContext->yyuv_buf.y_addr;
    pContext->combine_buf[pContext->combine_num].uv_addr=pContext->yyuv_buf.uv_addr;    

    DC_RETURN_VAL_IF_FAIL_WARNING(_DC_DirectQuickView(), DCAMERA_OP_ERROR, ("DCAM: FourInOne direct quick view err!"));
    
    pContext->combine_num++;

    if(0x03<pContext->combine_num)
    {
        _DC_SwitchFourInOneBuffer();  

        _DC_SetFourInoneEncoderParam();

        DC_RETURN_VAL_IF_FAIL_WARNING(_quick_review(), DCAMERA_OP_ERROR, ("DCAM: FourInOne quick view err!"));

        _DC_CleanCombineParam();
        
        nRet=_DC_SetFourInoneEncoder();            
    }
    else
    {
        if(PNULL!=pContext->snapshot_param.combine_callback)
        {
            //DC_LOG_TRACE:"DCAM: FourInOne capture combine call back is pnull err!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1435_112_2_18_1_23_44_254,(uint8*)"");
            return DCAMERA_OP_PARAM_ERR;
        }
        pContext->snapshot_param.combine_callback(DCAMERA_OP_SUCCESS, NULL, NULL);
    }
    
    return nRet;

}

/******************************************************************************
// Description: 	start preview actually
// Author:     		jing.li
// Input:      		none
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		
******************************************************************************/
LOCAL int32 _do_snapshot(void)
{
    DCAMERA_RETURN_VALUE_E	eRet = DCAMERA_OP_SUCCESS;
    uint32 capture_type = 0;

    SET_DCAM_STATE(DC_STATE_CAPTURING);

    capture_type = _get_capture_type();


    DC_GetMutex();
    //DC_LOG_TRACE:"DCAM:_do_snapshot capture type : %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1464_112_2_18_1_23_44_255,(uint8*)"d",capture_type);

    if(MAX_TYPE_CAPTURE<=capture_type)
    {
        //DC_LOG_TRACE:"DCAM:_do_snapshot capture mode err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1468_112_2_18_1_23_44_256,(uint8*)"");
        return DCAMERA_OP_PARAM_ERR;
    } 

    /* change sensor mode if need*/
    if(DCAMERA_OP_SUCCESS!=Sensor_Ioctl(SENSOR_IOCTL_BEFORE_SNAPSHOT, SNAPSHOT_MODE))
    {
        //DC_LOG_TRACE:"DCAM: capture before capture mode set err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1475_112_2_18_1_23_44_257,(uint8*)"");
        return DCAMERA_OP_SENSOR_NOT_WORK_ERR;
    }    

    switch(capture_type)
    {
        case FOURINONE_CAPTURE:
            eRet=_DC_FourInOneCapture();
            break;

        case PANORAMA_CAPTURE:
            eRet=_DC_PanoramaCapture();
            break;            
            
        case JPEG_CAPTURE:
            eRet =_capture_jpeg();
            break;

        case PERSONAL_CAPTURE:
            eRet=_capture_personal();
            break;

        case INTERPOLATION_CAPTURE:
            eRet = _capture_interpolation();
            break;

        case ZOOM_CAPTURE:
            eRet = _capture_zoom();
            break;

        case NORMAL_CAPTURE:
            eRet = _capture_normal();
            break;

        default:
            break;
    }

    

    LCD_SetInvalidateLimit(MAIN_LCD_ID, NO_PERMIT_GUI_INVALIDATE);

   

    if(DCAMERA_OP_SUCCESS!=Sensor_Ioctl(SENSOR_IOCTL_AFTER_SNAPSHOT, PREVIEW_MODE))
    {
        //DC_LOG_TRACE:"DCAM: capture after capture mode set err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1521_112_2_18_1_23_44_258,(uint8*)"");
        return DCAMERA_OP_SENSOR_NOT_WORK_ERR;
    }

    SET_DCAM_STATE(DC_STATE_IDLE);

    DC_PutMutex();
    

    return eRet;
}

LOCAL int32 _capture_normal(void)
{
    int32 nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext    = _GetDCAMContext();
    ISP_SNAPSHOT_PARAM_T *isp_snapshot_ptr = &s_st_isp_snapshot_param;
    uint16 swap_value=0x00;

    //DC_LOG_TRACE:"DCAM: _capture_normal"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1540_112_2_18_1_23_44_259,(uint8*)"");
    
    isp_snapshot_ptr->output_data_format = ISP_DATA_YUV422;	
    isp_snapshot_ptr->sensor_data_format = CONVERT_SENSOR_FORMAT(SENSOR_IMAGE_FORMAT_YUV422);

    DC_RETURN_VAL_IF_FAIL_WARNING(_do_capture(isp_snapshot_ptr), DCAMERA_OP_SNAPSHOT_ERR, ("DCAM: capture normal err!"));
    
    DC_RETURN_VAL_IF_FAIL_WARNING(_quick_review(), DCAMERA_OP_ERROR, ("DCAM review yuv err @ _capture_normal"));

    if(CAPTURE_ROTATION)
    {
        DC_RETURN_VAL_IF_FAIL_WARNING(DC_Rotation(CAPTURE_ROTATION), DCAMERA_OP_ERROR, ("DCAM: rotation yuv err @ _capture_normal"));
        swap_value=pContext->actual_width;
        pContext->actual_width=pContext->actual_height;
        pContext->actual_height=swap_value;
    }

    DC_RETURN_VAL_IF_FAIL_WARNING(_DC_GetCaptureData(), DCAMERA_OP_ERROR, ("DCAM: get capture data err @ _capture_normal"));
    
    /*encode to jpeg.If interpolation needed,jpeg encoder will get source data */
    /*through callback function.OK,interpolation will be done in the callback function*/
    if(DCAMERA_OP_SUCCESS  != _EncodeYYUVToJpeg((uint32 *)(pContext->jpeg_buf_ptr),(uint32 *)(&(pContext->jpeg_size))))
    {
        //DC_LOG_TRACE:"DCAM: _capture_normal encoder err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1563_112_2_18_1_23_44_260,(uint8*)"");
        nRet = DCAMERA_OP_ENCODE_ERR;
        pContext->jpeg_buf_ptr = PNULL;
        pContext->jpeg_size    = 0;
    }

   return nRet;
}

LOCAL int32 _capture_jpeg(void)
{
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();
    ISP_SNAPSHOT_PARAM_T* isp_snapshot_ptr=&s_st_isp_snapshot_param;
    int32 nRet = DCAMERA_OP_SUCCESS;

    pContext->snapshot_param.capture_date.eb=SCI_FALSE;

    DC_RETURN_VAL_IF_FAIL_WARNING(_do_capture_jpeg(), DCAMERA_OP_SNAPSHOT_ERR, ("DCAM: capture jpeg err!"));

    DC_RETURN_VAL_IF_FAIL_WARNING(_quick_review_jpeg(), DCAMERA_OP_ERROR, ("DCAM: review jpg err @ _capture_jpeg"));

    DC_RETURN_VAL_IF_FAIL_WARNING(_DC_GetCaptureData(), DCAMERA_OP_ERROR, ("DCAM: get capture data err @ _capture_jpeg"));

    DC_RETURN_VAL_IF_FAIL_WARNING(DC_JpegProcess((uint32*)isp_snapshot_ptr), DCAMERA_OP_ERROR, ("DCAM: jpg process err @ _capture_jpeg"));

    return nRet;
}


LOCAL int32 _capture_interpolation(void)
{
    int32 nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext = _GetDCAMContext();
    ISP_SNAPSHOT_PARAM_T *isp_snapshot_ptr = &s_st_isp_snapshot_param;

    //DC_LOG_TRACE:"DCAM: _capture_interpolation"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1598_112_2_18_1_23_44_261,(uint8*)"");
    
    isp_snapshot_ptr->output_data_format = ISP_DATA_YUV422;	
    isp_snapshot_ptr->sensor_data_format = CONVERT_SENSOR_FORMAT(SENSOR_IMAGE_FORMAT_YUV422);
    
    DC_RETURN_VAL_IF_FAIL_WARNING(_do_capture(isp_snapshot_ptr), DCAMERA_OP_SNAPSHOT_ERR, ("DCAM: capture interpolation err!"));

    DC_RETURN_VAL_IF_FAIL_WARNING(_quick_review(), DCAMERA_OP_ERROR, ("DCAM: review yuv error @ _capture_interpolation"));

    DC_RETURN_VAL_IF_FAIL_WARNING(_DC_GetCaptureData(), DCAMERA_OP_ERROR, ("DCAM: get capture data err @ _capture_interpolation"));

    if(nRet != _EncodeYYUVSliceToJpeg())
    {
        //DC_LOG_TRACE:"DCAM: _capture_interpolation encoder err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1611_112_2_18_1_23_44_262,(uint8*)"");
        pContext->jpeg_buf_ptr = PNULL;
        pContext->jpeg_size = 0;
        nRet=DCAMERA_OP_ENCODE_ERR;
    }

    return nRet;
}

LOCAL int32 _do_capture_jpeg(void)
{
    DCAM_CONTEXT_T* pContext    = _GetDCAMContext();
    ISP_SNAPSHOT_PARAM_T *isp_snapshot_ptr = &s_st_isp_snapshot_param;
    DCAMERA_SNAPSHOT_RETURN_PARAM_T jpeg_return_t={0};	
    
    //DC_LOG_TRACE:"DCAM: _capture_jpeg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1626_112_2_18_1_23_44_263,(uint8*)"");

    /* change sensor mode to jpeg */
    Sensor_Ioctl(SENSOR_IOCTL_CHANGE_IMAGE_FORMAT, SENSOR_IMAGE_FORMAT_JPEG);
    
    SCI_MEMSET(((uint8*)(pContext->src_jpg_addr)),0x00,(pContext->src_jpg_len));		
    DC_RETURN_VAL_IF_FAIL_WARNING(_do_capture(isp_snapshot_ptr), DCAMERA_OP_SNAPSHOT_ERR, ("DCAM: capture jpeg fail!"));

	/* pick jpeg stream out of capture buffer */
    jpeg_return_t.return_data_addr=pContext->src_jpg_addr; 
    jpeg_return_t.return_data_len=pContext->jpeg_size;
    
    pContext->jpeg_size=Sensor_Ioctl(SENSOR_IOCTL_PICK_JPEG_STREAM, &jpeg_return_t);/*lint !e64*/ 

    if(0x00==pContext->jpeg_size)
    {
        //DC_LOG_TRACE:"DCAM: _do_capture_jpeg jpg stream is null err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1642_112_2_18_1_23_44_264,(uint8*)"");
        return DCAMERA_OP_ERROR;
    }

    return DCAMERA_OP_SUCCESS;
}

LOCAL int32 _do_capture(ISP_SNAPSHOT_PARAM_T *isp_snapshot_ptr)
{
    ISP_DATA_ADDR_T isp_retruned_data={0};
    uint32 width=0x00;
    uint32 height=0x00;
    DCAM_CONTEXT_T* pContext    = _GetDCAMContext();
    int32 nRet = DCAMERA_OP_SUCCESS;

    //get YY... UVUV... data
    //start ISP capture service
    if(DCAMERA_OP_SUCCESS == ISP_ServiceSetCaptureParam(isp_snapshot_ptr))
    {
        if(DCAMERA_OP_SUCCESS==ISP_ServiceStartSyncCapture(&isp_retruned_data, &width, &height))
        {
            if(SENSOR_IMAGE_FORMAT_JPEG==pContext->sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].image_format)
            {
                pContext->src_jpg_addr=isp_retruned_data.y_addr;
                pContext->jpeg_size=width;
                pContext->actual_width=pContext->sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_width;
                pContext->actual_height=pContext->sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_height;
                //DC_LOG_TRACE:"DCAM: capture end jpg:0x%x, w:%d, h:%d, jpg_size:0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1669_112_2_18_1_23_44_265,(uint8*)"dddd",pContext->src_jpg_addr, pContext->actual_width, pContext->actual_height,pContext->jpeg_size);
            }
            else
            {
                pContext->yyuv_buf.y_addr=isp_retruned_data.y_addr;
                pContext->yyuv_buf.uv_addr=isp_retruned_data.uv_addr;
                pContext->actual_width=(uint16)width;
                pContext->actual_height=(uint16)height;
                //DC_LOG_TRACE:"DCAM: capture end y:0x%x, uv:0x%x, w:%d, h:%d,"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1677_112_2_18_1_23_44_266,(uint8*)"dddd",pContext->yyuv_buf.y_addr, pContext->yyuv_buf.uv_addr, pContext->actual_width, pContext->actual_height);
            }
        }
        else
        {
            //DC_LOG_TRACE:"DCAM: capture err!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1682_112_2_18_1_23_44_267,(uint8*)"");
            pContext->actual_width=0x00;
            pContext->actual_height=0x00;
            pContext->yyuv_buf.y_addr=NULL;
            pContext->yyuv_buf.uv_addr=NULL;
            pContext->jpeg_size=0x00;
            nRet=DCAMERA_OP_SNAPSHOT_ERR;
        }
    }
    else
    {				
        nRet = DCAMERA_OP_PARAM_ERR;
    }

    DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_FLASH, SCI_FALSE, NULL);

    return nRet;
}

/******************************************************************************
// Description:	get Image data from lcdc
// Author:		xi.zhang
// Input:		LCD_ID_E lcd_id, uint32 lcd_block_id, uint32 *dst_base_addr, LCDC_RECT_T cap_rect
// Return:		None
// Global Resource dependence: 	
// Note:			
******************************************************************************/ 
LOCAL int32 _DC_GetImageFromLCDC(LCD_ID_E lcd_id, uint32 lcd_block_id, uint32 *dst_base_addr, LCDC_RECT_T cap_rect)
{
	LCDC_RECT_T		lcd_rect = {0};
	LCD_CAP_PARAM_T         lcd_cap_t = {0};
	LCD_BRUSH_MODE_E        brush_mode = LCD_BRUSH_MODE_ASYNC;
	BLOCKCFG_T			    blk_info = {0};
	uint8 _orig_resolution = 0;

	//DC_LOG_TRACE:"DCAM: DC_GetImageFromLCDC"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1716_112_2_18_1_23_45_268,(uint8*)"");

	//config the blockbuffer
	LCD_GetConfigBlock(lcd_id, lcd_block_id, &blk_info);
	_orig_resolution = blk_info.resolution;
	blk_info.resolution = LCD_RESOLUTION_RGB565;
	LCD_ConfigBlock(lcd_id, lcd_block_id, blk_info);
    LCD_SetBlockBuffer(lcd_id, lcd_block_id, dst_base_addr);

	//set lcd capture parameters
	lcd_cap_t.format = LCD_RGB565;
	lcd_cap_t.dst_base_addr = (uint32)dst_base_addr;

	lcd_cap_t.cap_rect.x = cap_rect.x;
	lcd_cap_t.cap_rect.y = cap_rect.y;	
	lcd_cap_t.cap_rect.w = cap_rect.w;
	lcd_cap_t.cap_rect.h = cap_rect.h;
	lcd_cap_t.dst_size.w = cap_rect.w;
	lcd_cap_t.dst_size.h = cap_rect.h;
	lcd_cap_t.dst_trim_rect.x = 0;
	lcd_cap_t.dst_trim_rect.y = 0;	
	lcd_cap_t.dst_trim_rect.w = cap_rect.w;
	lcd_cap_t.dst_trim_rect.h = cap_rect.h;
	lcd_cap_t.set_endian_flag = 1;
	
#ifdef CHIP_ENDIAN_LITTLE
    lcd_cap_t.endian_switch = 1;
#else	
    lcd_cap_t.endian_switch = 0;
#endif

	lcd_cap_t.cap_en = SCI_TRUE;

	//lcd parameters protect
	brush_mode = LCD_GetBrushMode(lcd_id);
	LCD_SetBrushMode(lcd_id, LCD_BRUSH_MODE_SYNC);
	LCD_ConfigCapture(lcd_id,&lcd_cap_t);

	lcd_rect.x = 0;
	lcd_rect.y = 0;
    lcd_rect.w= cap_rect.w;
    lcd_rect.h = cap_rect.h; 
	
	//get the composed image
	LCD_InvalidateRect(lcd_id, 
                       lcd_rect.x, 
                       lcd_rect.y,
                       lcd_rect.x + lcd_rect.w - 1, 
                       lcd_rect.y + lcd_rect.h - 1
                       );

	lcd_cap_t.cap_en = SCI_FALSE;
	
	//parameters restore
	LCD_ConfigCapture(lcd_id,&lcd_cap_t);
	LCD_SetBrushMode(lcd_id, brush_mode);   
	
	blk_info.resolution = _orig_resolution;
	LCD_ConfigBlock(lcd_id, lcd_block_id, blk_info);

	return DCAMERA_OP_SUCCESS;
}

LOCAL int32 _capture_personal(void)
{
    int32 nRet=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    ISP_SNAPSHOT_PARAM_T *isp_snapshot_ptr=&s_st_isp_snapshot_param;
	LCDC_RECT_T cap_rect = {0};
    BLOCKCFG_T lcdc_osd_block_cfg={0x00};  
	ROTATION_PARAM_T _rotation_params = {0};
	DCAMERA_ROTATION_MODE_E rot_mode = pContext->preview_param.rotation_mode;
	
    //DC_LOG_TRACE:"DCAM: _capture_personal"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1789_112_2_18_1_23_45_269,(uint8*)"");
	
	//capture from isp
    isp_snapshot_ptr->output_data_format = ISP_DATA_RGB565;	
    DC_RETURN_VAL_IF_FAIL_WARNING(_do_capture(isp_snapshot_ptr), DCAMERA_OP_SNAPSHOT_ERR, ("DCAM: _capture_personal capture err!"));

    LCDC_GetConfigBlock(0x00, 0x02, &lcdc_osd_block_cfg);    

	//rotation handle
	if(CAPTURE_ROTATION||(pContext->preview_param.rotation_mode))
    {
		uint16 swap_data=0x00;
		_rotation_params.data_format = ROTATION_RGB565;
		_rotation_params.img_size.w = pContext->actual_width;
		_rotation_params.img_size.h = pContext->actual_height;
		_rotation_params.src_addr.y_addr = pContext->yyuv_buf.y_addr;
		_rotation_params.dst_addr.y_addr = (uint32)pContext->rot_buf_ptr;
		_rotation_params.rotation_mode = (ROTATION_MODE_E)(pContext->preview_param.rotation_mode - 1);

		Rotation_AppStart(&(_rotation_params));

        swap_data=pContext->actual_width;
        pContext->actual_width=pContext->actual_height;
        pContext->actual_height=swap_data;
		pContext->preview_param.rotation_mode=DCAMERA_ROTATION_0;
		if(CAPTURE_ROTATION)
		{
			rot_mode = DCAMERA_ROTATION_0;
		}

    }
    else
    {

        SCI_MEMCPY((uint8*)pContext->rot_buf_ptr,(uint8*)pContext->yyuv_buf.y_addr,((pContext->actual_width*pContext->actual_height)<<0x01));
    }

	cap_rect.w = pContext->actual_width;
	cap_rect.h = pContext->actual_height;

	//get personal capture image data from LCDC
	DC_GetMutex();
	_DC_GetImageFromLCDC(pContext->preview_param.lcd_id, 
							pContext->preview_param.lcd_block_id, 
							(uint16 *)pContext->rot_buf_ptr, 
							cap_rect);
    DC_PutMutex();
	
	pContext->yyuv_buf.y_addr=DC_Algin(pContext->yyuv_buf.y_addr, 0xff, 0x08);
	pContext->yyuv_buf.uv_addr=pContext->yyuv_buf.y_addr+DC_Algin((pContext->actual_width*pContext->actual_height), 0xff, 0x08);

	DC_RGB2YUV(pContext->actual_width,
                pContext->actual_height,
                (uint32)pContext->rot_buf_ptr,
                pContext->yyuv_buf.y_addr,
                pContext->yyuv_buf.uv_addr);
  
	DC_RETURN_VAL_IF_FAIL_WARNING(_quick_review(),DCAMERA_OP_ERROR,  ("DCAM: capture personal quick view err!"));

    /*encode to jpeg.If interpolation needed,jpeg encoder will get source data */
    /*through callback function.OK,interpolation will be done in the callback function*/
	
	//rotation handle before encode
 	if(rot_mode)
	{
		_rotation_params.data_format = ROTATION_YUV422;
		_rotation_params.img_size.w = pContext->actual_width;
		_rotation_params.img_size.h = pContext->actual_height;
		_rotation_params.src_addr.y_addr = pContext->yyuv_buf.y_addr;
		_rotation_params.src_addr.uv_addr = pContext->yyuv_buf.uv_addr;
		_rotation_params.dst_addr.y_addr = (uint32)pContext->rot_buf_ptr;
		_rotation_params.dst_addr.uv_addr = (uint32)(pContext->rot_buf_ptr+pContext->actual_width*pContext->actual_height);
		
		
		if(DCAMERA_ROTATION_90 == rot_mode)
		{
			_rotation_params.rotation_mode = ROTATION_270;
		}
		else if(DCAMERA_ROTATION_270 == rot_mode)
		{
			_rotation_params.rotation_mode = ROTATION_90;
		}

		Rotation_AppStart(&(_rotation_params));

 		SCI_MEMCPY((uint8*)pContext->yyuv_buf.y_addr,(uint8*)_rotation_params.dst_addr.y_addr, pContext->actual_width*pContext->actual_height);
		SCI_MEMCPY((uint8*)pContext->yyuv_buf.uv_addr,(uint8*)_rotation_params.dst_addr.uv_addr, pContext->actual_width*pContext->actual_height);
	}
	
    if(DCAMERA_OP_SUCCESS!=_EncodeYYUVToJpeg((uint32 *)(pContext->jpeg_buf_ptr),(uint32 *)(&(pContext->jpeg_size))))
    {
        nRet=SCI_ERROR;
        pContext->jpeg_buf_ptr=PNULL;
        pContext->jpeg_size=0x00;
        //DC_LOG_TRACE:"DCAM: _capture_personal encoder err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1883_112_2_18_1_23_45_270,(uint8*)"");
    }

    return nRet;
}


LOCAL int32 _capture_zoom(void)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    ISP_SNAPSHOT_PARAM_T *isp_snapshot_ptr = &s_st_isp_snapshot_param;
    int32 nRet = DCAMERA_OP_SUCCESS;
//    uint32 actual_width,actual_height = 0;
    uint16 swap_value=0x00;

    //get YY... UVUV... data
    //start ISP capture service
    if(!((SNAPSHOT_TRIM_WIDTH <= VGA_WIDTH)&&(SNAPSHOT_TRIM_HEIGHT <= VGA_HEIGHT)))
    {    //vga zoom, use isp scale
        //use sensor scale
        uint32 param[2];
        param[0]=SNAPSHOT_MODE;
        param[1]=pContext->zoom_level;
        Sensor_Ioctl(SENSOR_IOCTL_ZOOM,&param);/*lint !e64*/ 
    }

    isp_snapshot_ptr->output_data_format = ISP_DATA_YUV422;	
    isp_snapshot_ptr->sensor_data_format = CONVERT_SENSOR_FORMAT(SENSOR_IMAGE_FORMAT_YUV422);

    DC_RETURN_VAL_IF_FAIL_WARNING(_do_capture(isp_snapshot_ptr), DCAMERA_OP_SNAPSHOT_ERR, ("DCAM: _capture_zoom capture yuv fail!"));

    DC_RETURN_VAL_IF_FAIL_WARNING(_quick_review(), DCAMERA_OP_ERROR,  ("DCAM: capture zoom quick view err!"));

    if(CAPTURE_ROTATION)
    {
        DC_RETURN_VAL_IF_FAIL_WARNING(DC_Rotation(CAPTURE_ROTATION), DCAMERA_OP_ERROR, ("DCAM: capture zoom rotation err!"));
        swap_value=pContext->actual_width;
        pContext->actual_width=pContext->actual_height;
        pContext->actual_height=swap_value;
    }

    DC_RETURN_VAL_IF_FAIL_WARNING(_DC_GetCaptureData(), DCAMERA_OP_ERROR, ("DCAM: get capture data err @ _capture_zoom"));

    if((pContext->target_width==pContext->actual_width)&&(pContext->target_height==pContext->actual_height))
    {
        //frame encode mode
        if(DCAMERA_OP_SUCCESS!=_EncodeYYUVToJpeg((uint32 *)(pContext->jpeg_buf_ptr),(uint32 *)(&(pContext->jpeg_size))))
        {
            nRet = DCAMERA_OP_ENCODE_ERR;
            pContext->jpeg_buf_ptr = PNULL;
            pContext->jpeg_size    = 0;
            //DC_LOG_TRACE:"DCAM: _capture_zoom encoder err!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1934_112_2_18_1_23_45_271,(uint8*)"");
        }
    }
    else
    {
        //slice encode mode
        if(DCAMERA_OP_SUCCESS!=_EncodeYYUVSliceToJpeg())
        {
            nRet = DCAMERA_OP_ENCODE_ERR;
            pContext->jpeg_buf_ptr = PNULL;
            pContext->jpeg_size    = 0;
            //DC_LOG_TRACE:"DCAM: _capture_zoom slice encoder err!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_1945_112_2_18_1_23_45_272,(uint8*)"");
        }
    }

    return nRet;
}

LOCAL void _date_mark_blend( DCAMERA_SIZE_T  *img_size_ptr,  DCAMERA_DATA_ADDR_T *img_addr_ptr, DCAMERA_CAPTURE_DATE_T *date_ptr, DCAMERA_ROTATION_MODE_E	rotation_mode )
{
	
       IMGREF_YUV_SRC yuv_src = {0x00};
	IMGREF_OSD_T  capture_date = {0};
	uint32 rotdate_addr = 0;   

	yuv_src.src_size.w= img_size_ptr->w;
	yuv_src.src_size.h=img_size_ptr->h;
	yuv_src.src.y_addr=img_addr_ptr->y_addr;
	yuv_src.src.uv_addr=img_addr_ptr->uv_addr;
	yuv_src.src_buf_rect.x=0x00;
	yuv_src.src_buf_rect.y=0x00;
	yuv_src.src_buf_rect.w=yuv_src.src_size.w;
	yuv_src.src_buf_rect.h=yuv_src.src_size.h;

	capture_date.color_key = date_ptr->color_key;
	capture_date.dataformat = IMGREF_FORMAT_ARGB888;       
	capture_date.osd_addr.y_addr= date_ptr->osd_addr.y_addr; 
	capture_date.osd_offset= date_ptr->osd_offset;
	capture_date.overlay_rect.x=date_ptr->overlay_rect.x;
	capture_date.overlay_rect.y=date_ptr->overlay_rect.y;
	capture_date.overlay_rect.w=date_ptr->overlay_rect.w;
	capture_date.overlay_rect.h=date_ptr->overlay_rect.h;

	 DC_GetMutex();

    //process 
    	if(DCAMERA_ROTATION_90 == rotation_mode)
    	{
        register uint32 i,j;
        register uint32 width, height;
        register uint32 *src_ptr = (uint32 *)capture_date.osd_addr.y_addr; 
        register uint32 *dst_ptr= PNULL;

	 uint32  * rotdata_align_addr;
        rotdate_addr =(uint32)SCI_ALLOCA(capture_date.overlay_rect.w*capture_date.overlay_rect.h*4+255);
        rotdata_align_addr = (uint32 *)((rotdate_addr+3)/4*4); 
        dst_ptr = (uint32 *)rotdata_align_addr;
        
        width = capture_date.overlay_rect.w;
        height = capture_date.overlay_rect.h;
//do rotation
        for(i=0; i<height; i++)
        {
            dst_ptr = rotdata_align_addr + height*(width-1)+i;
            for(j=0; j<width; j++)    
            {
                  *dst_ptr = *src_ptr++;
                  dst_ptr = dst_ptr-height;
            }
        }

        
 //change rect

        capture_date.osd_addr.y_addr = (uint32)rotdata_align_addr;
        i = capture_date.overlay_rect.x ;
        capture_date.overlay_rect.x = capture_date.overlay_rect.y;
        capture_date.overlay_rect.y =  yuv_src.src_buf_rect.h-i-width ;
        capture_date.overlay_rect.w = height;
        capture_date.overlay_rect.h = width;

        
                
    }



	DC_ArgbOverlayer(&yuv_src,&capture_date);
	if(PNULL != rotdate_addr)
	{
		SCI_FREE(((uint32 *)rotdate_addr));         //lint !e63
       }
	 DC_PutMutex();
	 
}

LOCAL uint32 _capture_burst(void)
{
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();
    ISP_SNAPSHOT_PARAM_T *isp_snapshot_ptr = &s_st_isp_snapshot_param;
    DCAMERA_CALL_BACK burst_snapshot_callback = pContext->burst_snapshot_param.burst_snapshot_callback;
    uint32 i=0;
    uint32 nRet = DCAMERA_OP_SUCCESS;
    uint32 tmp_date_eb = pContext->snapshot_param.capture_date.eb;
    uint16 swap_value=0x00;

   pContext->snapshot_param.capture_date.eb = SCI_FALSE;

    /* change sensor mode if need*/
    if(DCAMERA_OP_SUCCESS!=Sensor_Ioctl(SENSOR_IOCTL_BEFORE_SNAPSHOT, SNAPSHOT_MODE))
    {
        //DC_LOG_TRACE:"DCAM: continue shot before capture mode set err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_2045_112_2_18_1_23_45_273,(uint8*)"");
        return DCAMERA_OP_SENSOR_NOT_WORK_ERR;
    }


    for(i=0; i<pContext->burst_snapshot_param.image_count; i++)
    {   
        pContext->is_first = SCI_TRUE;
        if(0x00!=i)
        {
            isp_snapshot_ptr->skip_num=0x00;
        }

        if(SENSOR_IMAGE_FORMAT_JPEG==pContext->sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].image_format)
        {
            isp_snapshot_ptr->capture_addr=pContext->src_jpg_addr;
            isp_snapshot_ptr->capture_buf_size=pContext->src_jpg_len;
            pContext->burst_snapshot_param.capture_date.eb=SCI_FALSE;
        }

        if(DCAMERA_OP_SUCCESS != _do_capture(isp_snapshot_ptr) )
        {
            burst_snapshot_callback(SCI_ERROR, 0, 0);
            //DC_LOG_TRACE:"DCAM: _capture_burst capture err"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_2068_112_2_18_1_23_45_274,(uint8*)"");
            return DCAMERA_OP_SNAPSHOT_ERR;	  
        }

	
        if(tmp_date_eb) 
        {
        	DCAMERA_SIZE_T  img_size;
		DCAMERA_DATA_ADDR_T *img_addr_ptr;
		DCAMERA_CAPTURE_DATE_T *date_ptr = &pContext->snapshot_param.capture_date;
		img_size.w = pContext->actual_width;
		img_size.h = pContext->actual_height;
		img_addr_ptr = &pContext->yyuv_buf;
        	_date_mark_blend(&img_size, img_addr_ptr, date_ptr, pContext->burst_snapshot_param.rotation_mode);
        }	
		
        if(DCAMERA_OP_SUCCESS != _DC_DirectQuickView())
        {
            //DC_LOG_TRACE:"DCAM: _capture_burst quick view err"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_2086_112_2_18_1_23_45_275,(uint8*)"");
            burst_snapshot_callback(SCI_ERROR, 0, 0);
            return DCAMERA_OP_ERROR;	  
        }

        if((DCAMERA_ROTATION_90==pContext->burst_snapshot_param.rotation_mode)
            ||(DCAMERA_ROTATION_270==pContext->burst_snapshot_param.rotation_mode))
        {
            if(DCAMERA_OP_SUCCESS!=DC_Rotation(pContext->burst_snapshot_param.rotation_mode))
            {
                //DC_LOG_TRACE:"DCAM: _capture_burst rotation err"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_2096_112_2_18_1_23_45_276,(uint8*)"");
                burst_snapshot_callback(SCI_ERROR, 0, 0);
                return DCAMERA_OP_ERROR;
            }
            swap_value=pContext->actual_width;
            pContext->actual_width=pContext->actual_height;
            pContext->actual_height=swap_value;
        }

        DC_RETURN_VAL_IF_FAIL_WARNING(_DC_GetCaptureData(), DCAMERA_OP_ERROR, ("DCAM: get capture data err @ _capture_burst"));

        if(SENSOR_IMAGE_FORMAT_JPEG==pContext->sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].image_format)
        {
            nRet=DC_ExifGreate();
            DC_RETURN_IF_FAIL(nRet, ("DCAM: DC_JpegProcess greate exif err ,line %d",__LINE__));

            pContext->burst_snapshot_return_param.return_data_addr[i]=pContext->jpeg_buf_ptr;
            pContext->burst_snapshot_return_param.return_data_len[i]=pContext->jpeg_size;

            pContext->exif_buf_addr=pContext->jpeg_buf_ptr+pContext->jpeg_size;
            pContext->src_jpg_addr=pContext->exif_buf_addr+MM_GetCaptureExifBufSize();/*lint !e718*/ /*lint !e18*/ 
            pContext->src_jpg_addr=DC_Algin(pContext->src_jpg_addr,0xff,0x08);
            pContext->jpeg_buf_ptr=(uint8*)pContext->src_jpg_addr;

            pContext->src_jpg_len=pContext->jpeg_buf_len-(pContext->dc_buf_addr-pContext->src_jpg_addr);
        }
        else
        {
            if((SCI_TRUE==pContext->burst_snapshot_param.capture_date.eb)
                &&(NULL!=pContext->burst_snapshot_param.capture_date.date_update))
            {
                pContext->burst_snapshot_param.capture_date.date_update(NULL);
            }

            pContext->snapshot_param.capture_date.osd_offset=0x00;

            if(INTERPOLATION_CAPTURE == _get_capture_type())
            {
                nRet = _EncodeYYUVSliceToJpeg();
            }
            else
            {
                nRet = _EncodeYYUVToJpeg(&(pContext->burst_snapshot_return_param.return_data_addr[i]), &(pContext->burst_snapshot_return_param.return_data_len[i]));
            }

            if(DCAMERA_OP_SUCCESS != nRet)
            {
                //DC_LOG_TRACE:"DCAM: _capture_burst encoder err!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_2143_112_2_18_1_23_45_277,(uint8*)"");
                burst_snapshot_callback(SCI_ERROR, 0, 0);
                return DCAMERA_OP_ENCODE_ERR;
            }
            else
            {
                pContext->burst_snapshot_return_param.return_data_addr[i] = pContext->jpeg_buf_ptr; 
                pContext->burst_snapshot_return_param.return_data_len[i] = pContext->jpeg_size;
                pContext->exif_buf_addr=((uint32)pContext->burst_snapshot_return_param.return_data_addr[i]+pContext->burst_snapshot_return_param.return_data_len[i]);
                pContext->jpeg_buf_ptr=pContext->exif_buf_addr+MM_GetCaptureExifBufSize();
                pContext->jpeg_buf_ptr=(uint8*)DC_Algin(pContext->jpeg_buf_ptr,0xff,0x08);

                pContext->jpeg_buf_len=pContext->src_jpg_len-(pContext->dc_buf_addr-(uint32)pContext->jpeg_buf_ptr);
            }
        }

        if(burst_snapshot_callback)
        {
            if(pContext->burst_snapshot_param.image_count-1==i)
            {
                /* restore sensor mode */
                if(DCAMERA_OP_SUCCESS!=Sensor_Ioctl(SENSOR_IOCTL_AFTER_SNAPSHOT, PREVIEW_MODE))
                {
                    //DC_LOG_TRACE:"DCAM: continue shot after capture mode set err!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_2166_112_2_18_1_23_45_278,(uint8*)"");
//                  return DCAMERA_OP_SENSOR_NOT_WORK_ERR;
                }

                SET_DCAM_STATE(DC_STATE_IDLE);
            }

            nRet = burst_snapshot_callback(nRet, pContext->burst_snapshot_return_param.return_data_addr[i], pContext->burst_snapshot_return_param.return_data_len[i]);
            if(0xff == nRet)
            {
                nRet = DCAMERA_OP_SUCCESS;
                SET_DCAM_STATE(DC_STATE_IDLE);
                break;
            }
        }
        SCI_Sleep(pContext->burst_snapshot_param.interval_of_frame);
    }

   pContext->snapshot_param.capture_date.eb = tmp_date_eb;  //restore;
    return nRet;
}

/******************************************************************************
// Description:	Get quick view paramter 
// Author:		Tim.zhu
// Input:			isp_review_ptr: review parameter pointer
// Return:		success
// Note:			
******************************************************************************/ 
LOCAL uint32 _DC_GetQuickViewParam(ISP_REVIEW_PARAM_T* isp_review_ptr)
{
    DCAM_CONTEXT_T*	pContext	 = _GetDCAMContext();

    isp_review_ptr->input_size.w=pContext->preview_param.target_rect.w;
    isp_review_ptr->input_size.h=pContext->preview_param.target_rect.h;

    isp_review_ptr->input_rect.x=0;
    isp_review_ptr->input_rect.y=0;
    isp_review_ptr->input_rect.w=pContext->preview_param.target_rect.w;
    isp_review_ptr->input_rect.h=pContext->preview_param.target_rect.h;

    isp_review_ptr->input_format=ISP_DATA_YUV422;

    isp_review_ptr->src_addr.y_addr=pContext->quick_view_buf.y_addr;
    isp_review_ptr->src_addr.uv_addr=pContext->quick_view_buf.uv_addr; 
    isp_review_ptr->src_len.y_len=pContext->preview_param.target_rect.w*pContext->preview_param.target_rect.h;
    isp_review_ptr->src_len.uv_len=pContext->preview_param.target_rect.w*pContext->preview_param.target_rect.h;

    isp_review_ptr->disp_format=ISP_ServiceGetDisplayFormat();

    isp_review_ptr->disp_addr=(uint32)pContext->disp_buf_ptr;
    isp_review_ptr->disp_buf_size=pContext->disp_buf_len;

    isp_review_ptr->rot_buf_len.y_len=pContext->rot_buf_len;
    isp_review_ptr->rot_buf_len.uv_len=DC_Algin((pContext->preview_param.target_rect.w*pContext->preview_param.target_rect.h),0xff, 0x08);
    isp_review_ptr->rot_addr.y_addr=(uint32)pContext->rot_buf_ptr;
    isp_review_ptr->rot_addr.uv_addr=isp_review_ptr->rot_addr.y_addr+isp_review_ptr->rot_buf_len.uv_len;

    isp_review_ptr->disp_lcd_id=pContext->preview_param.lcd_id;
    isp_review_ptr->disp_block_id=pContext->preview_param.lcd_block_id;
    //display rect
    isp_review_ptr->disp_rect.x=pContext->preview_param.disp_rect.x;
    isp_review_ptr->disp_rect.y=pContext->preview_param.disp_rect.y;
    isp_review_ptr->disp_rect.w=pContext->preview_param.disp_rect.w;
    isp_review_ptr->disp_rect.h=pContext->preview_param.disp_rect.h;

    isp_review_ptr->target_rect.x=pContext->preview_param.target_rect.x;
    isp_review_ptr->target_rect.y=pContext->preview_param.target_rect.y;
    isp_review_ptr->target_rect.w=pContext->preview_param.target_rect.w;
    isp_review_ptr->target_rect.h=pContext->preview_param.target_rect.h;

    isp_review_ptr->mode = ISP_SERVICE_INVALIDATE_SYNC;
    isp_review_ptr->is_copy_display_data = FALSE;

    isp_review_ptr->disp_rotation = CONVERT_ROTAION_MODE(DCAMERA_ROTATION_0);

    //isp_review_ptr->callback	= _display_callback;
    
    return DCAMERA_OP_SUCCESS;
    
}

/******************************************************************************
// Description:	Get quick view scale yuv buf informat 
// Author:		Tim.zhu
// Input:			addr: img buf pointer
//                         width: image width
//                         height: image height
// Return:		success
// Note:			
******************************************************************************/ 
LOCAL uint32 _DC_GetScaleDataInfo(ISP_DATA_ADDR_T addr, uint32 width,uint32 height)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext(); 

    pContext->rtn_buf.y_addr=addr.y_addr;
    pContext->rtn_buf.uv_addr=addr.uv_addr;
    pContext->rtn_buf.v_addr=addr.v_addr;

    pContext->rtn_buf_len.y_len=width*height;
    pContext->rtn_buf_len.uv_len=width*height;
    pContext->rtn_buf_len.v_len=0x00;

    pContext->rtn_size.w=width;
    pContext->rtn_size.h=height;

    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************
// Description:	quick view scale
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL BOOLEAN _DC_IsSoftScale(void)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();   
    	
    if((PANORAMA_CAPTURE==_get_capture_type()))
    {
        return SCI_TRUE;	
    } 
    
    if(0x00!=(pContext->preview_param.disp_rect.w&0x0f))
    {
        return SCI_TRUE;	
    }      

    return SCI_FALSE;	
}

/******************************************************************************
// Description:	quick view scale
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL uint32 _DC_QuickViewScale(void)
{
    uint32 nRet=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    ISP_SNAPSHOT_PARAM_T* isp_snapshot_ptr=&s_st_isp_snapshot_param;
    ISP_SCALE_PARAM_T scale_param={0};
    IMG_SCALE_PARAM_T soft_scale_param={0x00};
    IMG_SCALE_PARAM_T* soft_scale_param_ptr=NULL;
    uint16 swap_data=0x00;
    DCAMERA_RECT_T imgsrc_rect = {0};
    //SCALE_DATEMARK_T scale_datemark = {0};
    soft_scale_param_ptr=&soft_scale_param;

    scale_param.input_size.w=pContext->actual_width;
    scale_param.input_size.h=pContext->actual_height;

    if((JPEG_CAPTURE==_get_capture_type())
        &&(0x00!=ZOOM_LEVEL))
    {
        SCI_MEMCPY((void*)&(scale_param.input_rect), (void*)&(isp_snapshot_ptr->zoom_rect), sizeof(ISP_RECT_T));
    }
    else
    {
        scale_param.input_rect.x  = 0;
        scale_param.input_rect.y  = 0;
        scale_param.input_rect.w  = scale_param.input_size.w;
        scale_param.input_rect.h  = scale_param.input_size.h;

        DC_AdjRect_OnDistortion((DCAMERA_RECT_T*)&scale_param.input_rect, 
                                 &pContext->preview_param.disp_rect, 
                                 &pContext->preview_param.target_rect, 
                                 pContext->preview_param.disp_mode, 
                                 pContext->preview_param.rotation_mode);
    }

    scale_param.input_format = ISP_DATA_YUV422;
    scale_param.input_addr.y_addr = pContext->yyuv_buf.y_addr;
    scale_param.input_addr.uv_addr = pContext->yyuv_buf.uv_addr;
    scale_param.input_buf_size = pContext->yyuv_buf_len;

    scale_param.output_format = ISP_DATA_YUV422;
    scale_param.output_size.w = pContext->preview_param.target_rect.w;
    scale_param.output_size.h = pContext->preview_param.target_rect.h;

    pContext->quick_view_buf.y_addr=_DC_Buffer_Algin(pContext->quick_view_buf.y_addr,0xff,0x08);
    pContext->quick_view_buf.uv_addr=pContext->quick_view_buf.y_addr+_DC_Buffer_Algin((scale_param.output_size.w*scale_param.output_size.h),0xff,0x08);

    if(DCAMERA_ROTATION_0==pContext->preview_param.rotation_mode)
    {
        scale_param.output_addr.y_addr=pContext->quick_view_buf.y_addr;
        scale_param.output_addr.uv_addr=pContext->quick_view_buf.uv_addr;
        scale_param.output_buf_size=pContext->quick_view_buf_len;
    }
    else
    {
        scale_param.output_addr.y_addr=_DC_Buffer_Algin((uint32)pContext->rot_buf_ptr,0xff,0x08);
        scale_param.output_addr.uv_addr=scale_param.output_addr.y_addr+_DC_Buffer_Algin((scale_param.output_size.w*scale_param.output_size.h),0xff,0x08);
        scale_param.output_buf_size=pContext->rot_buf_len;
    }


    scale_param.scale_mode=ISP_SERVICE_SCALE_NORMAL;
    scale_param.is_first=SCI_TRUE;
    scale_param.is_last=SCI_FALSE;
    
    scale_param.get_scale_data=(get_data)_DC_GetScaleDataInfo;

    if(_DC_IsSoftScale())
    {
        _DC_SetSoftScaleDownYuv422Param(soft_scale_param_ptr, &scale_param);
        DC_ScaleDown_Y_UV_422(soft_scale_param_ptr);	
    }
    else if((scale_param.input_size.w==scale_param.output_size.w) && (scale_param.input_size.h==scale_param.output_size.h))
    {
        SCI_MEMCPY((void*)scale_param.output_addr.y_addr, (void*)pContext->yyuv_buf.y_addr, (scale_param.output_size.w*scale_param.output_size.h));
        SCI_MEMCPY((void*)scale_param.output_addr.uv_addr, (void*)pContext->yyuv_buf.uv_addr, (scale_param.output_size.w*scale_param.output_size.h));
    }
    else if((scale_param.output_size.w==scale_param.input_rect.w)||(scale_param.output_size.h==scale_param.input_rect.h))
    {
        DCAMERA_YUV_CHN_ADDR_T src={0x00};
        DCAMERA_YUV_CHN_ADDR_T dst={0x00};
        DCAMERA_SIZE_T src_size={0x00};
        DCAMERA_RECT_T src_rect={0x00};
        DCAMERA_SIZE_T dst_size={0x00};

        src.y_chn_addr=scale_param.input_addr.y_addr;
        src.u_chn_addr=scale_param.input_addr.uv_addr;

        dst.y_chn_addr=scale_param.output_addr.y_addr;
        dst.u_chn_addr=scale_param.output_addr.uv_addr;

        src_size.w=scale_param.input_size.w;
        src_size.h=scale_param.input_size.h;

        src_rect.x=scale_param.input_rect.x;
        src_rect.y=scale_param.input_rect.y;
        src_rect.w=scale_param.input_rect.w;
        src_rect.h=scale_param.input_rect.h;

        DC_Trim_Y_UV_422(&src, &dst, src_size, src_rect, dst_size);
    }
    else
    {
        DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceSetScaleParam(&scale_param), DCAMERA_OP_PARAM_ERR, ("DCAM: do scale set param err!"));

        DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceStartScale(), DCAMERA_OP_ERROR, ("DCAM: do scale  scale err!"));
    }

   
    if(pContext->snapshot_param.capture_date.eb)
    {
        if(0 == scale_param.input_rect.x && 0 == scale_param.input_rect.y)
        {
            imgsrc_rect.w = pContext->target_width;
            imgsrc_rect.h = pContext->target_height;
        }

        else if(0 == scale_param.input_rect.x)
        {
            imgsrc_rect.w = pContext->target_width;
            imgsrc_rect.h =(( (scale_param.input_rect.h * imgsrc_rect.w+(scale_param.input_rect.w>>1))/scale_param.input_rect.w)>>1)<<1;
            imgsrc_rect.x = 0;
            imgsrc_rect.y = (((pContext->target_height -imgsrc_rect.h+1)>>1)>>1)<<1;
        }
        else
        {
            imgsrc_rect.h = pContext->target_height;
            imgsrc_rect.w = ((((scale_param.input_rect.w * imgsrc_rect.h)+(scale_param.input_rect.h>>1))/scale_param.input_rect.h)>>1)<<1;
            imgsrc_rect.y = 0;
            imgsrc_rect.x = (((pContext->target_width -imgsrc_rect.w+1)>>1)>>1)<<1 ;
        }
		  
        if(CAPTURE_ROTATION)
        {
            uint32 tmp =  imgsrc_rect.h;
            imgsrc_rect.h =  imgsrc_rect.w;
            imgsrc_rect.w = tmp;
            tmp = imgsrc_rect.x;
            imgsrc_rect.x = imgsrc_rect.y;
            imgsrc_rect.x = tmp;
        }

        _DC_Proc_DispDate(&imgsrc_rect, &(pContext->preview_param.target_rect), &(imgsrc_rect),&(scale_param.output_addr));
//      SCI_PASSERT(0,("DCAM: quick view addr 0x%x, w: %d, h:%d", scale_param.output_addr.y_addr, pContext->preview_param.target_rect.w, pContext->preview_param.target_rect.h));
        //SCI_TRACE_LOW:"DCAM: quick view target w: %d h: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_2446_112_2_18_1_23_46_279,(uint8*)"dd", pContext->preview_param.target_rect.w, pContext->preview_param.target_rect.h);
    }
    
    if(DCAMERA_ROTATION_0!=pContext->preview_param.rotation_mode)
    {
        JINF_YUV_CHN_T src_yuv_t={0x00};
        JINF_YUV_CHN_T dst_yuv_t={0x00};

        src_yuv_t.y_chn_ptr=(uint8*)(scale_param.output_addr.y_addr);
        src_yuv_t.u_chn_ptr=(uint8*)(scale_param.output_addr.uv_addr);

        dst_yuv_t.y_chn_ptr=(uint8*)(pContext->quick_view_buf.y_addr);
        dst_yuv_t.u_chn_ptr=(uint8*)(pContext->quick_view_buf.uv_addr);

        DC_RotateYUV422_Y_UV(&dst_yuv_t, &src_yuv_t, scale_param.output_size.w, scale_param.output_size.h, pContext->preview_param.rotation_mode);

        if((DCAMERA_ROTATION_90==pContext->preview_param.rotation_mode)
            ||(DCAMERA_ROTATION_270==pContext->preview_param.rotation_mode))
        {
            swap_data=pContext->preview_param.target_rect.w;
            pContext->preview_param.target_rect.w=pContext->preview_param.target_rect.h;
            pContext->preview_param.target_rect.h=swap_data;
        }

    }

    return nRet;
    
}

/******************************************************************************
// Description:	quick view 
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL int32 _quick_review(void)
{
    int32 nRet = DCAMERA_OP_SUCCESS;
    ISP_REVIEW_PARAM_T st_isp_quick_view_param = {0};
    //get osd position
    DCAMERA_OSD_POS_T * osd_pos=NULL; 
    DCAM_CONTEXT_T* pContext = _GetDCAMContext();

    if((pContext->snapshot_param.quickview_callback)&&(DC_STATE_CAPTURING == pContext->state))
    {
        DC_GetMutex();
        osd_pos = pContext->snapshot_param.quickview_callback();
        DC_PutMutex();
    }

    DC_RETURN_VAL_IF_FAIL_WARNING(_DC_QuickViewScale(),DCAMERA_OP_PARAM_ERR, ("DCAM: _quick_review scale err!"));

    _DC_GetQuickViewParam(&st_isp_quick_view_param);      
   
    if(JPEG_CAPTURE == _get_capture_type())
    {
        JINF_INFO_T	jpeg_info = {0};
        JINF_GET_INFO_IN_T jpeg_get_info_in = {0};
        jpeg_get_info_in.jpeg_buf_ptr=pContext->src_jpg_addr;
        jpeg_get_info_in.jpeg_buf_size=pContext->jpeg_size;	

        IMGJPEG_GetInfo(&jpeg_get_info_in, &jpeg_info);

        if(JINF_FORMAT_YUV420 == jpeg_info.sample_format)
        {
            st_isp_quick_view_param.input_format=ISP_DATA_YUV420;
        }
        else 
        {
            if(JINF_FORMAT_YUV422!=jpeg_info.sample_format)
            {
                //DC_LOG_TRACE:"DCAM: _quick_review jpg sample format err!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_2518_112_2_18_1_23_46_280,(uint8*)"");
                return DCAMERA_OP_PARAM_ERR;
            }
        }
    }
    
    DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceSetReviewParam(&st_isp_quick_view_param), SCI_ERROR, ("DCAM: quick view set rview param err!"));

    SCI_MEMCPY((void*)&(s_isp_quick_view_param), (void*)&(st_isp_quick_view_param), sizeof(ISP_REVIEW_PARAM_T));

    DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceStartReview(), SCI_ERROR, ("DCAM: quick view rview err!"));

    DC_GetMutex();          //mutex to protect lcd operation finished operately
    LCD_SetInvalidateLimit(MAIN_LCD_ID, PERMIT_GUI_INVALIDATE);
    DC_PutMutex();

    return nRet;
}

/******************************************************************************
// Description:	Capture rview 
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL int32 _capture_review(void)
{
    int32 nRet = DCAMERA_OP_SUCCESS;

    //DC_LOG_TRACE:"_capture_review"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_2547_112_2_18_1_23_46_281,(uint8*)"");

    DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceSetReviewParam(&s_isp_quick_view_param), DCAMERA_OP_PARAM_ERR, ("DCAM: capture review set rview param err!"));

    DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceStartReview(), DCAMERA_OP_ERROR, ("DCAM: capture review review err!"));

    return nRet;
}

/******************************************************************************
// Description:	quick view jpg 
// Author:		Tim.zhu
// Input:			None
// Return:		None
// Note:			
******************************************************************************/ 
LOCAL int32 _quick_review_jpeg(void)
{
    int32 nRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();

    DC_RETURN_VAL_IF_FAIL_WARNING(_DC_DecoderJpeg(pContext->src_jpg_addr, pContext->jpeg_size),DCAMERA_OP_ERROR, ("DCAM: _quick_review_jpeg decoder err!"));
 
    DC_RETURN_VAL_IF_FAIL_WARNING(_quick_review(),DCAMERA_OP_ERROR, ("DCAM: _quick_review_jpeg review err!"));
	
    return nRet;	
}

LOCAL uint32 _get_capture_type(void)
{
    if(SENSOR_IMAGE_FORMAT_JPEG==DC_GetSensorCurFormat(SNAPSHOT_MODE))
    {
        return JPEG_CAPTURE;
    }

    if(DC_IN_STATE(DC_STATE_CAPTURING))
    {
	    if(SNAPSHOT_PARAM_ST.personal_snapshot.select_personal)
	        return PERSONAL_CAPTURE;
    }
    
    if(DCAMERA_CAPTURE_FOURINONE==PREVIEW_PARAM_ST.capture_mode)
    {
        return FOURINONE_CAPTURE;
    }
    
    if(DCAMERA_CAPTURE_PANORAMA==PREVIEW_PARAM_ST.capture_mode)
    {
        return PANORAMA_CAPTURE;
    }
    
    if((SNAPSHOT_TARGET_WIDTH>ISP_ServiceGetMaxScaleWidth())
        &&(SNAPSHOT_TARGET_WIDTH!=SNAPSHOT_TRIM_WIDTH))
    {
        return INTERPOLATION_CAPTURE;
    }

    if(ZOOM_LEVEL)
        return ZOOM_CAPTURE;

    return NORMAL_CAPTURE;
        
}

LOCAL int32 _set_burst_parameters(DCAMERA_BURST_SNAPSHOT_PARAM_T  *param_ptr,
									DCAMERA_SNAPSHOT_RETURN_PARAM_T  *return_param_ptr)
{
    int nRet=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();
    SENSOR_EXP_INFO_T_PTR sensor_info_ptr=pContext->sensor_info_ptr;
    ISP_SNAPSHOT_PARAM_T* isp_snapshot_ptr=&s_st_isp_snapshot_param;
    DCAMERA_RECT_T_PTR st_trim_rect_ptr=&pContext->trim_rect;
    DCAMERA_RECT_T src_rect={0x00};
    ISP_SENSOR_SIGNAL_INFO_T isp_sensor_singnal={0};
    uint32 tmp=0x00;

    BURSTSNAPSHOT_PARAM_ST = *param_ptr;
    pContext->is_first = SCI_TRUE;

    SCI_MEMCPY((void*)&(pContext->snapshot_param.capture_date),(void*)&(pContext->burst_snapshot_param.capture_date),sizeof(DCAMERA_CAPTURE_DATE_T));

    DC_GetImgSensorSignalInfo(SNAPSHOT_MODE,&isp_sensor_singnal);
    ISP_ServiceSetSensorSignalInfo(&isp_sensor_singnal);

    pContext->sensor_format=sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].image_format;
    isp_snapshot_ptr->sensor_data_format=CONVERT_SENSOR_FORMAT(pContext->sensor_format);
    isp_snapshot_ptr->output_data_format=ISP_DATA_YUV422;	

    isp_snapshot_ptr->skip_num=sensor_info_ptr->capture_skip_num;
    isp_snapshot_ptr->pclk= pContext->sensor_info_ptr->sensor_mode_info[pContext->snapshot_mode].pclk;
    isp_snapshot_ptr->input_size.w = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].width;
    isp_snapshot_ptr->input_size.h = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].height;

    st_trim_rect_ptr->x = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_start_x;
    st_trim_rect_ptr->y = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_start_y;
    st_trim_rect_ptr->w = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_width;
    st_trim_rect_ptr->h = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_height;

    src_rect.x=0x00;
    src_rect.y=0x00;
    
    if((sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_width !=pContext->target_width)
        && (ISP_ServiceGetMaxScaleWidth()<pContext->target_width))
    {
        src_rect.w = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_width;
        src_rect.h = sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_height;
    }
    else
    {
    	src_rect.w = pContext->target_width;
    	src_rect.h = pContext->target_height;
    }
    
    if((DCAMERA_ROTATION_90==pContext->burst_snapshot_param.rotation_mode)
        ||(DCAMERA_ROTATION_270==pContext->burst_snapshot_param.rotation_mode))
    {
        tmp = src_rect.w;
        src_rect.w = src_rect.h;
        src_rect.h = tmp;
    }
    
    DC_AdjRect_OnDistortion(&pContext->trim_rect, &src_rect, &src_rect, DCAMERA_DISP_FULLSCREEN, DCAMERA_ROTATION_0);
    
    isp_snapshot_ptr->input_rect = *(ISP_RECT_T*)&pContext->trim_rect;

    if((sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].trim_width!=pContext->target_width)
        &&(ISP_ServiceGetMaxScaleWidth()<pContext->target_width))    
    {
        isp_snapshot_ptr->output_size.w = isp_snapshot_ptr->input_rect.w;
        isp_snapshot_ptr->output_size.h = isp_snapshot_ptr->input_rect.h;	    
    }
    else
    {
        isp_snapshot_ptr->output_size.w = pContext->target_width;
        isp_snapshot_ptr->output_size.h = pContext->target_height;
    }

    if((DCAMERA_ROTATION_90==pContext->burst_snapshot_param.rotation_mode)
        ||(DCAMERA_ROTATION_270==pContext->burst_snapshot_param.rotation_mode))
    {
        tmp = isp_snapshot_ptr->output_size.w;
        isp_snapshot_ptr->output_size.w = isp_snapshot_ptr->output_size.h;
        isp_snapshot_ptr->output_size.h = tmp;
    } 

    pContext->snapshot_param.enc_quality_level=JINF_QUALITY_MIDDLE_LOW;

    nRet=DC_Malloc(DC_STATE_BURST_CAPTURING);
    DC_RETURN_IF_FAIL(nRet, ("DCAM: Continue Shot alloc memory fail err"));

    if(SENSOR_IMAGE_FORMAT_JPEG==pContext->sensor_info_ptr->sensor_mode_info[SNAPSHOT_MODE].image_format)
    {
        isp_snapshot_ptr->capture_addr=pContext->jpeg_buf_ptr;
        isp_snapshot_ptr->capture_buf_size=pContext->jpeg_buf_len;
    }
    else
    {
        isp_snapshot_ptr->capture_addr=pContext->yyuv_buf.y_addr;
        isp_snapshot_ptr->capture_buf_size=pContext->yyuv_buf_len;
    }

    return nRet;
}


/******************************************************************************
// Description: 	start continue shoot
// Author:     		Tim.zhu
// Input:      		none
// Output:		    none
// Return:          0           successful
//                  others      failed
// Note:       		
******************************************************************************/
LOCAL uint32 _do_burstshot(void)
{
    DCAMERA_RETURN_VALUE_E	eRet = DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext = _GetDCAMContext();
    uint32 capture_type = 0;

    capture_type = _get_capture_type();

    if(!((NORMAL_CAPTURE==capture_type)
        ||(JPEG_CAPTURE==capture_type)
        ||(INTERPOLATION_CAPTURE==capture_type)))
        
    {
        //DC_LOG_TRACE:"DCAM: continue shot capture mode %d err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_SNAPSHOT_2732_112_2_18_1_23_47_282,(uint8*)"d",capture_type);
        pContext->burst_snapshot_param.burst_snapshot_callback(SCI_ERROR, 0, 0);
        return DCAMERA_OP_ERROR;
    }

    SET_DCAM_STATE(DC_STATE_BURST_CAPTURING);

    pContext->is_burst_snapshot=SCI_TRUE;
    
    switch(capture_type)
    {
        case NORMAL_CAPTURE:
        case JPEG_CAPTURE:
        case INTERPOLATION_CAPTURE:
            eRet = _capture_burst();
            break;

        default:
            break;
    }

    pContext->is_burst_snapshot=SCI_FALSE;

    return eRet;
}


/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
 
#ifdef   __cplusplus
    }
#endif// End
