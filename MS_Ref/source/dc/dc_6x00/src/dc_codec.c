/*******************************************************************************
 ** FileName:      dc_codec.c                                                  *
 ** Author:        jing.li                                                     *
 ** Date:          2008/12/02                                                  *
 ** CopyRight:     2008, Spreatrum, Incoporated, All right reserved            *
 ** Description:                                                               *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                                 *
 ** ------------------------------------------------------------------------------------ *
 ** DATE           NAME                   DESCRIPTION                                   *
 ** 2008/12/02     jing.li                 Created                                      *
 *******************************************************************************/

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_ref_dc_trc.h"
#include "dc_app_6x00.h"
#include "jpeg_interface_ex.h"
#include "dc_misc.h"
#include "dc_codec.h"
#include "iram_mgr.h"
#include "dc_cfg.h"
#include "dc_bufmgr.h"
#include "exif_writer.h"

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/*lint -save -e18 */
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*--------------------------- Local Data ------------------------------------*/

extern PUBLIC DCAM_CONTEXT_T* _GetDCAMContext(void);

/****************************************************************************************/
// Description: set minature in param
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      param_ptr: minature in param pointer
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL void _DC_GetThumbnailSize(uint16* width, uint16* height)
{
    DCAM_CONTEXT_T* pDCContext=_GetDCAMContext();
    uint16 max_value=0x00;

    if(pDCContext->target_height<pDCContext->target_width)
    {
        max_value=pDCContext->target_width;
    }
    else
    {
        max_value=pDCContext->target_height;
    }
    
    *width=pDCContext->target_width;
    *height=pDCContext->target_height;
    
    while(640<max_value)
    {
        max_value>>=0x01;
        *width>>=0x01;
        *height>>=0x01;
    }
}

/****************************************************************************************/
// Description: set minature in param
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      param_ptr: minature in param pointer
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _DC_SetMinatureInParam(JINF_MINI_IN_PARAM_T* param_ptr, DCAMERA_RECT_T jpg_trim_rect)
{
    DCAM_CONTEXT_T* pDCContext=_GetDCAMContext();

    //DCAM_PRINT:"DCAM: _DC_SetMinatureInParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_94_112_2_18_1_23_27_81,(uint8*)"");

    param_ptr->jpeg_buf_ptr=pDCContext->src_jpg_addr;
    param_ptr->jpeg_buf_size=pDCContext->jpeg_size;
    param_ptr->crop_rect.x=jpg_trim_rect.x;
    param_ptr->crop_rect.y=jpg_trim_rect.y;
    param_ptr->crop_rect.w=jpg_trim_rect.w;
    param_ptr->crop_rect.h=jpg_trim_rect.h;
    param_ptr->target_width=pDCContext->target_width;
    param_ptr->target_height=pDCContext->target_height;
    param_ptr->quality=pDCContext->snapshot_param.enc_quality_level;/*lint !e64*/
    param_ptr->is_ouput_yuv=SCI_FALSE;
    param_ptr->decode_buf_ptr=(uint8*)pDCContext->minature_swap_buf_addr;
    param_ptr->decode_buf_size=pDCContext->minature_swap_buf_len;
    param_ptr->target_buf_ptr=(uint8*)pDCContext->jpeg_buf_ptr;
    param_ptr->target_buf_size=pDCContext->jpeg_buf_len;
    param_ptr->read_file_func=PNULL;
    param_ptr->write_file_func=PNULL;
    param_ptr->write_yuv_func=PNULL;
    param_ptr->async_callback=PNULL;

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: set minature in param
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      param_ptr: minature in param pointer
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _DC_SetThumbnailMinatureInParam(JINF_MINI_IN_PARAM_T* param_ptr)
{
    DCAM_CONTEXT_T* pDCContext=_GetDCAMContext();

    //DCAM_PRINT:"DCAM: _DC_SetThumbnailMinatureInParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_132_112_2_18_1_23_27_82,(uint8*)"");

    param_ptr->jpeg_buf_ptr=pDCContext->thumbnail_src_addr;
    param_ptr->jpeg_buf_size=pDCContext->thumbnail_src_size;
    _DC_GetThumbnailSize(&param_ptr->target_width, &param_ptr->target_height);
    param_ptr->quality=JINF_QUALITY_MIDDLE;
    param_ptr->is_ouput_yuv=SCI_FALSE;
    param_ptr->decode_buf_ptr=(uint8*)pDCContext->minature_swap_buf_addr;
    param_ptr->decode_buf_size=pDCContext->minature_swap_buf_len;
    param_ptr->target_buf_ptr=(uint8*)pDCContext->thumbnail_buf_addr;
    param_ptr->target_buf_size=pDCContext->thumbnail_buf_len;
    param_ptr->read_file_func=PNULL;
    param_ptr->write_file_func=PNULL;
    param_ptr->write_yuv_func=PNULL;
    param_ptr->async_callback=PNULL;

    return SCI_SUCCESS;
}

/****************************************************************************************/
// Description: set write exif in param
// Global resource dependence: 
// Author: Tim.zhu
// input parameter:
//                      param_ptr: exif in param pointer
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success     
// Note:
/*****************************************************************************************/
LOCAL uint32 _DC_SetWriteEixfInParam(JINF_WEXIF_IN_PARAM_T* param_ptr)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();

    //DCAM_PRINT:"DCAM: _DC_SetWriteEixfInParam"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_165_112_2_18_1_23_27_83,(uint8*)"");

    param_ptr->exif_info_ptr=DC_GetExifParameter();
    
    if((NULL!=param_ptr->exif_info_ptr)
        &&(NULL!=param_ptr->exif_info_ptr->spec_ptr))
    {
        param_ptr->exif_info_ptr->spec_ptr->basic.PixelXDimension=pContext->target_width;
        param_ptr->exif_info_ptr->spec_ptr->basic.PixelYDimension=pContext->target_height;
    }

    param_ptr->src_jpeg_buf_ptr=pContext->jpeg_buf_ptr;
    param_ptr->src_jpeg_size=pContext->jpeg_size;

    if(SCI_TRUE==pContext->thumbnail_eb)
    {
        param_ptr->thumbnail_buf_ptr=(uint8*)pContext->thumbnail_buf_addr;
        param_ptr->thumbnail_buf_size=pContext->thumbnail_size;
    }
    else
    {
        param_ptr->thumbnail_buf_ptr=NULL;
        param_ptr->thumbnail_buf_size=0x00;
    }

    param_ptr->temp_buf_ptr=(uint8*)pContext->exif_swap_buf_addr;
    param_ptr->temp_buf_size=pContext->exif_swap_buf_len;

    param_ptr->wrtie_file_func=PNULL;
    
    param_ptr->target_buf_ptr=(uint8*)pContext->exif_buf_addr;
    param_ptr->target_buf_size=pContext->exif_buf_len+MM_GetCaptureThumbnailBufSize()+pContext->jpeg_size;

    return SCI_SUCCESS;
}

/******************************************************************************/
// Description: afternoon, call back notice the data address and width/height
// Author:     
// Input:      
//		    dst_yuv_ptr: dest yuv channel info, y data in y channel, uv data in u channel.
//		    width: out image width
//          height: out image height
// Output: none
// Return: DCAMERA_OP_SUCCESS: success
//                      other: fail
// Note:   
/******************************************************************************/ 
PUBLIC uint32 DC_Get_HwSliceScaleLine(ISP_DATA_ADDR_T dst_yuv_ptr, uint32 width, uint32 height)
{
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();

    //DCAM_PRINT:"DCAM:DC_Get_HwSliceScaleLine: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_215_112_2_18_1_23_28_84,(uint8*)"d",height);

    if(0x00!=height)
    {
        pContext->slice_remain_line=height-pContext->back_line;
    }
    else
    {
        pContext->slice_remain_line=0x00;
    }
    pContext->back_line=height;
    pContext->rtn_buf.y_addr=dst_yuv_ptr.y_addr;
    pContext->rtn_buf.uv_addr=dst_yuv_ptr.uv_addr;
    pContext->rtn_buf.v_addr=dst_yuv_ptr.v_addr;

    return DCAMERA_OP_SUCCESS;
}

/******************************************************************************
// Description:	Get scale view scale yuv buf  
// Author:		Tim.zhu
// Input:			addr: img buf pointer
//                         width: image width
//                         height: image height
// Return:		success
// Note:			
******************************************************************************/ 
LOCAL uint32 _DC_GetScaleData(ISP_DATA_ADDR_T addr, uint32 width,uint32 height)
{
    DCAM_CONTEXT_T* pContext=_GetDCAMContext(); 

    pContext->rtn_buf.y_addr=addr.y_addr;
    pContext->rtn_buf.uv_addr=addr.uv_addr;
    pContext->rtn_buf.v_addr=addr.v_addr;

    return DCAMERA_OP_SUCCESS;
}

 /****************************************************************************************/
// Description: release jpeg slice encoder iram
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
LOCAL uint32 _GetSliceYUVData(JINF_YUV_CHN_T *yuv_chn, uint32 line_offset, uint32 width, uint32 read_lines)
{
    JINF_YUV_CHN_T src_yuv_ptr={0};
    uint32 src_buf_offset = 0;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();
    uint32 slice_line=read_lines;
    
    if(width!=pContext->target_width)
    {//now only support 2x scale up
        //SCI_TRACE_LOW:"DCAM:_GetSliceYUVData,photo_width=%d,get_slice_width=%d err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_272_112_2_18_1_23_28_85,(uint8*)"dd",pContext->target_width,width);
        return 0x00;
    }

    if(pContext->target_height<=line_offset)
    {
        //SCI_TRACE_LOW:"DCAM:_GetSliceYUVData,photo_height=%d,line_offsert=%d err"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_278_112_2_18_1_23_28_86,(uint8*)"dd",pContext->target_height,line_offset);
        return 0x00;
    }

    src_buf_offset=(line_offset*pContext->actual_width/pContext->target_width)*pContext->actual_width;
    src_yuv_ptr.y_chn_ptr=(uint8*)(pContext->yyuv_buf.y_addr + src_buf_offset);
    src_yuv_ptr.u_chn_ptr=(uint8*)(pContext->yyuv_buf.uv_addr + src_buf_offset);

    //SCI_TRACE_LOW("_GetSliceYUVData,src_yaddr=0x%x,src_uvaddr=0x%x,line_offset=%d,read_lines=%d",
    //				src_yuv_ptr.y_chn_ptr,src_yuv_ptr.u_chn_ptr,line_offset,read_lines);

    if((pContext->target_width<=ISP_ServiceGetMaxSliceScaleWidth())
        &&(pContext->target_width<=ISP_ServiceGetMaxSliceScaleHeight()))
    {       
        DC_HW_YuvSliceScale(yuv_chn,&src_yuv_ptr, read_lines);
        slice_line=pContext->slice_remain_line;
    }
    else
    {
        if((pContext->target_width*2)==(pContext->actual_width*5))
        {// vga-> uxga
            DC_SW_ScaleUpVGATo2M_Y_UV(yuv_chn,&src_yuv_ptr,pContext->actual_width,read_lines*pContext->actual_width/pContext->target_width,
                                                    pContext->cx_buf_ptr, pContext->cx_buf_len);

            if(!((line_offset+read_lines)<pContext->target_height))
            {
                SCI_MEMCPY(yuv_chn->y_chn_ptr+((read_lines-2)*pContext->target_width), yuv_chn->y_chn_ptr+((read_lines-3)*pContext->target_width), pContext->target_width);
                SCI_MEMCPY(yuv_chn->y_chn_ptr+((read_lines-1)*pContext->target_width), yuv_chn->y_chn_ptr+((read_lines-3)*pContext->target_width), pContext->target_width);            
                SCI_MEMCPY(yuv_chn->y_chn_ptr+((read_lines-3)*pContext->target_width), yuv_chn->y_chn_ptr+((read_lines-4)*pContext->target_width), pContext->target_width);
                
                SCI_MEMCPY(yuv_chn->u_chn_ptr+((read_lines-2)*pContext->target_width), yuv_chn->u_chn_ptr+((read_lines-3)*pContext->target_width), pContext->target_width);            
                SCI_MEMCPY(yuv_chn->u_chn_ptr+((read_lines-1)*pContext->target_width), yuv_chn->u_chn_ptr+((read_lines-3)*pContext->target_width), pContext->target_width);
                SCI_MEMCPY(yuv_chn->u_chn_ptr+((read_lines-3)*pContext->target_width), yuv_chn->u_chn_ptr+((read_lines-4)*pContext->target_width), pContext->target_width);
            } 
        }
        else if(pContext->target_width==(pContext->actual_width*2))
        {// vag-xga
             DC_SW_ScaleUp2X_Y_UV(yuv_chn,&src_yuv_ptr,pContext->actual_width,read_lines*pContext->actual_width/pContext->target_width);
        }
    }

    if((0x00!=slice_line)
        &&(SCI_TRUE==pContext->snapshot_param.capture_date.eb))
    {
        IMGREF_YUV_SRC yuv_data={0x00};
        IMGREF_OSD_T capture_date = {0};
        yuv_data.src_size.w=width;
        yuv_data.src_size.h=slice_line;
        yuv_data.src.y_addr=(uint32)yuv_chn->y_chn_ptr;
        yuv_data.src.uv_addr=(uint32)yuv_chn->u_chn_ptr;
        yuv_data.src_buf_rect.x=0x00;
        yuv_data.src_buf_rect.y=pContext->back_line-slice_line;
        yuv_data.src_buf_rect.w=width;
        yuv_data.src_buf_rect.h=slice_line;
        //DC_ArgbOverlayer(&yuv_data, &(pContext->snapshot_param.capture_date));

        capture_date.color_key = pContext->snapshot_param.capture_date.color_key;
        capture_date.dataformat = IMGREF_FORMAT_ARGB888;       
        capture_date.osd_addr.y_addr= pContext->snapshot_param.capture_date.osd_addr.y_addr; 
        capture_date.osd_offset= pContext->snapshot_param.capture_date.osd_offset;
        capture_date.overlay_rect.x=pContext->snapshot_param.capture_date.overlay_rect.x;
        capture_date.overlay_rect.y=pContext->snapshot_param.capture_date.overlay_rect.y;
        capture_date.overlay_rect.w=pContext->snapshot_param.capture_date.overlay_rect.w;
        capture_date.overlay_rect.h=pContext->snapshot_param.capture_date.overlay_rect.h;
        
        DC_ArgbOverlayer(&yuv_data, &capture_date);
        pContext->snapshot_param.capture_date.osd_offset = capture_date.osd_offset; //update offset
    }

    return slice_line;

}

/****************************************************************************************/
// Description: release jpeg slice encoder iram
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
PUBLIC uint32 _EncodeYYUVToThumbnail(uint32* param_ptr)
{
    uint32 nRet=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();   
    DC_PRODUCT_CFG_T_PTR pDCProductCfg=DC_GeProductCfgPtr();
    ISP_JPEG_PARAM_T* isp_snapshot_ptr=(ISP_JPEG_PARAM_T*)param_ptr;
    ISP_SCALE_PARAM_T scale_param={0};
    JINF_ENC_IN_PARAM_T jpeg_in={0};
    JINF_ENC_OUT_PARAM_T jpeg_out={0};

    IMGREF_YUV_SRC yuv_src = {0x00};
    IMGREF_OSD_T  capture_date = {0};

    pContext->thumbnail_eb=SCI_FALSE;

    if((SCI_FALSE==pContext->is_burst_snapshot)
        &&(SCI_TRUE==pContext->exif_eb)
        &&(SCI_TRUE==pDCProductCfg->exif_eb)
        &&(SCI_TRUE==pDCProductCfg->thumbnail_eb)
        &&(640<pContext->target_width))
    {
        scale_param.input_size.w=pContext->actual_width;
        scale_param.input_size.h=pContext->actual_height;

        if(SENSOR_IMAGE_FORMAT_YUV422==pContext->sensor_format)
        {// yuv
            scale_param.input_rect.x=0;
            scale_param.input_rect.y=0;
            scale_param.input_rect.w=scale_param.input_size.w;
            scale_param.input_rect.h=scale_param.input_size.h;
        }
        else
        {// jpg
            scale_param.input_rect.w=(isp_snapshot_ptr->input_rect.w*pContext->actual_width)/isp_snapshot_ptr->input_size.w;
            scale_param.input_rect.w=DC_Algin(scale_param.input_rect.w,0x00,0x02);
            scale_param.input_rect.h=(isp_snapshot_ptr->input_rect.h*pContext->actual_height)/isp_snapshot_ptr->input_size.h;
            scale_param.input_rect.h=DC_Algin(scale_param.input_rect.h,0x00,0x02);

            scale_param.input_rect.x=(pContext->actual_width-scale_param.input_rect.w)>>0x01;
            scale_param.input_rect.x=DC_Algin(scale_param.input_rect.x,0x00,0x02);
            scale_param.input_rect.y=(pContext->actual_height-scale_param.input_rect.h)>>0x01;
            scale_param.input_rect.y=DC_Algin(scale_param.input_rect.y,0x00,0x02);
        }

        scale_param.input_format=ISP_DATA_YUV422;
        scale_param.input_addr.y_addr=pContext->yyuv_buf.y_addr;
        scale_param.input_addr.uv_addr=pContext->yyuv_buf.uv_addr;
        scale_param.input_buf_size=pContext->yyuv_buf_len;
        scale_param.output_format=ISP_DATA_YUV422;
        scale_param.output_size.w=320;
        scale_param.output_size.h=240;

        if(pContext->jpeg_fw_len<(DC_Algin((scale_param.output_size.w*scale_param.output_size.h), DC_BUF_ALGIN, DC_BUF_ALGIN_BITS)<<0x01))
        {
            //DCAM_PRINT:"_EncodeYYUVToThumbnail(),thumbnail scale yuv buf err ,line %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_415_112_2_18_1_23_28_87,(uint8*)"d",__LINE__);
            return SCI_ERROR;
        }

        scale_param.output_addr.y_addr=pContext->jpeg_fw_ptr;
        scale_param.output_addr.uv_addr=pContext->jpeg_fw_ptr+DC_Algin((scale_param.output_size.w*scale_param.output_size.h), DC_BUF_ALGIN, DC_BUF_ALGIN_BITS);
        scale_param.output_buf_size=pContext->jpeg_fw_len;
        scale_param.scale_mode=ISP_SERVICE_SCALE_NORMAL;
        scale_param.is_first=SCI_TRUE;
        scale_param.is_last=SCI_FALSE;
        scale_param.get_scale_data=_DC_GetScaleData;

        DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceSetScaleParam(&scale_param), DCAMERA_OP_PARAM_ERR, ("DCAM: do scale set param err ,line %d",__LINE__));
        DC_RETURN_VAL_IF_FAIL_WARNING(ISP_ServiceStartScale(), DCAMERA_OP_ERROR, ("DCAM: do scale  scale err ,line %d",__LINE__));

        //add date and time mark
        if(SCI_TRUE==pContext->snapshot_param.capture_date.eb)
        {
            yuv_src.src_size.w=scale_param.output_size.w;
            yuv_src.src_size.h=scale_param.output_size.h;
            yuv_src.src.y_addr=scale_param.output_addr.y_addr;
            yuv_src.src.uv_addr=scale_param.output_addr.uv_addr;
            yuv_src.src_buf_rect.x=0x00;
            yuv_src.src_buf_rect.y=0x00;
            yuv_src.src_buf_rect.w=yuv_src.src_size.w;
            yuv_src.src_buf_rect.h=yuv_src.src_size.h;
#if 1
            capture_date.color_key = pContext->snapshot_param.capture_date.thumb_color_key;
            capture_date.dataformat = IMGREF_FORMAT_ARGB888;       
            capture_date.osd_addr.y_addr= pContext->snapshot_param.capture_date.thumb_osd_addr.y_addr; 
            capture_date.osd_offset= pContext->snapshot_param.capture_date.thumb_osd_offset;
            capture_date.overlay_rect.x=pContext->snapshot_param.capture_date.thumb_overlay_rect.x;
            capture_date.overlay_rect.y=pContext->snapshot_param.capture_date.thumb_overlay_rect.y;
            capture_date.overlay_rect.w=pContext->snapshot_param.capture_date.thumb_overlay_rect.w;
            capture_date.overlay_rect.h=pContext->snapshot_param.capture_date.thumb_overlay_rect.h;
#else   //for test convience.

            capture_date.color_key = pContext->snapshot_param.capture_date.color_key;
            capture_date.dataformat = IMGREF_FORMAT_ARGB888;       
            capture_date.osd_addr.y_addr= pContext->snapshot_param.capture_date.osd_addr.y_addr; 
            capture_date.osd_offset= pContext->snapshot_param.capture_date.osd_offset;
            capture_date.overlay_rect.x=0;
            capture_date.overlay_rect.y=0;
            capture_date.overlay_rect.w=pContext->snapshot_param.capture_date.overlay_rect.w;
            capture_date.overlay_rect.h=pContext->snapshot_param.capture_date.overlay_rect.h;


#endif
            
            DC_ArgbOverlayer(&yuv_src,&capture_date);
        }
            
        // start encoder thumbnail yuv
        jpeg_in.target_buf_ptr=pContext->thumbnail_buf_addr;
        jpeg_in.target_buf_size=pContext->thumbnail_buf_len;
        jpeg_in.yuv_info.yuv_chn.y_chn_ptr=(uint8 *)pContext->rtn_buf.y_addr;
        jpeg_in.yuv_info.yuv_chn.u_chn_ptr=(uint8 *)pContext->rtn_buf.uv_addr;
        jpeg_in.yuv_info.sample_format=JINF_FORMAT_YUV422;
        jpeg_in.yuv_info.data_pattern=JINF_YUV_PATTERN_CHN_Y_UV;
        jpeg_in.yuv_info.data_endian=JINF_DATA_BIG_ENDIAN;
        jpeg_in.src_width=scale_param.output_size.w;
        jpeg_in.src_height=scale_param.output_size.h;
        jpeg_in.quality=pContext->snapshot_param.enc_quality_level;
        jpeg_in.encode_buf_ptr=scale_param.output_addr.uv_addr+DC_Algin((scale_param.output_size.w*scale_param.output_size.h), DC_BUF_ALGIN, DC_BUF_ALGIN_BITS);
        jpeg_in.encode_buf_size=pContext->jpeg_fw_len-DC_Algin((scale_param.output_size.w*scale_param.output_size.h), DC_BUF_ALGIN, DC_BUF_ALGIN_BITS)*2;
        jpeg_in.read_yuv_func=PNULL;
        jpeg_in.write_file_func=PNULL;
        jpeg_in.async_callback=PNULL;

        if(JINF_SUCCESS != IMGJPEG_EncodeEx(&jpeg_in, &jpeg_out))
        {
            pContext->thumbnail_size=0x00;
            //DCAM_PRINT:"_EncodeYYUVToThumbnail(),jpeg encoding err ,line %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_487_112_2_18_1_23_28_88,(uint8*)"d",__LINE__);
            return SCI_ERROR;
        }

        pContext->thumbnail_buf_addr=jpeg_out.jpeg_buf_ptr; 
        pContext->thumbnail_size=jpeg_out.jpeg_buf_size;
        pContext->thumbnail_eb=SCI_TRUE;

        //DCAM_PRINT:"DCAM: _EncodeYYUVToThumbnail, thumbnail_addr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_495_112_2_18_1_23_28_89,(uint8*)"d",pContext->thumbnail_buf_addr);
        //DCAM_PRINT:"DCAM: _EncodeYYUVToThumbnail, thumbnail_size = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_496_112_2_18_1_23_28_90,(uint8*)"d",pContext->thumbnail_size);
    }

    return nRet;
}

/****************************************************************************************/
// Description: release jpeg slice encoder iram
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
PUBLIC uint32 _DC_JpegToThumbnail(void)
{
    uint32 nRet=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();   
    DC_PRODUCT_CFG_T_PTR pDCProductCfg=DC_GeProductCfgPtr();

    pContext->thumbnail_eb=SCI_FALSE;

    if((SCI_TRUE==pContext->exif_eb)
        &&(SCI_TRUE==pDCProductCfg->exif_eb)
        &&(SCI_TRUE==pDCProductCfg->thumbnail_eb)
        &&(SCI_FALSE==pContext->is_burst_snapshot)
        &&(640<pContext->target_width))
    {
        JINF_MINI_IN_PARAM_T minature_in_param={0x00};
        JINF_MINI_IN_PARAM_T* minature_in_param_ptr=NULL;
        JINF_MINI_OUT_PARAM_T minature_out_param={0x00};
        JINF_MINI_OUT_PARAM_T* minature_out_param_ptr=NULL;

        minature_in_param_ptr=&minature_in_param;
        minature_out_param_ptr=&minature_out_param;

        _DC_SetThumbnailMinatureInParam(minature_in_param_ptr);

        if(SCI_SUCCESS!=IMGJPEG_CreateMiniatureEx(minature_in_param_ptr, minature_out_param_ptr))
        {
            pContext->thumbnail_size=0x00;
            //DCAM_PRINT:"DCAM: _DC_JpegToThumbnail, Greate thumbnail err ,line %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_539_112_2_18_1_23_28_91,(uint8*)"d",__LINE__);
            return SCI_ERROR;
        }

        pContext->thumbnail_buf_addr=minature_out_param_ptr->jpeg_buf_ptr; 
        pContext->thumbnail_size=minature_out_param_ptr->jpeg_buf_size;
        pContext->thumbnail_eb=SCI_TRUE;

        //DCAM_PRINT:"DCAM: _DC_JpegToThumbnail, thumbnail_buf_addr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_547_112_2_18_1_23_28_92,(uint8*)"d",pContext->thumbnail_buf_addr);
        //DCAM_PRINT:"DCAM: _DC_JpegToThumbnail, thumbnail_size = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_548_112_2_18_1_23_28_93,(uint8*)"d",pContext->thumbnail_size);
    }

    return nRet;
}

/****************************************************************************************/
// Description: release jpeg slice encoder iram
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
PUBLIC uint32 DC_ExifGreate(void)
{
    uint32 nRet=DCAMERA_OP_SUCCESS;
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();   
    DC_PRODUCT_CFG_T_PTR pDCProductCfg=DC_GeProductCfgPtr();

    if((SCI_TRUE==pDCProductCfg->exif_eb)
        &&(SCI_TRUE==pContext->exif_eb))
    {
        JINF_WEXIF_IN_PARAM_T exif_in_param={0x00};
        JINF_WEXIF_OUT_PARAM_T exif_out_param={0x00};

        _DC_SetWriteEixfInParam(&exif_in_param);

        if(SCI_SUCCESS!=IMGJPEG_WriteExif(&exif_in_param, &exif_out_param))
        {
            pContext->jpeg_buf_ptr=PNULL; 
            pContext->jpeg_size=0x00;
            //DCAM_PRINT:"DCAM: DC_ExifGreate(),jpeg write exif error,line %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_582_112_2_18_1_23_28_94,(uint8*)"d",__LINE__);
            return SCI_ERROR;
        }

        pContext->jpeg_buf_ptr=exif_out_param.output_buf_ptr;
        pContext->jpeg_size=exif_out_param.output_size;
        //DCAM_PRINT:"DCAM: DC_ExifGreate(),jpeg_exif_buf_ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_588_112_2_18_1_23_28_95,(uint8*)"d",pContext->jpeg_buf_ptr);
        //DCAM_PRINT:"DCAM: DC_ExifGreate(),jpeg_exif_size = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_589_112_2_18_1_23_28_96,(uint8*)"d",pContext->jpeg_size);
    }

    return nRet;
}

/****************************************************************************************/
// Description: release jpeg slice encoder iram
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
PUBLIC int32 _EncodeYYUVToJpeg(uint32* jpeg_buf_addr,uint32*jpeg_buf_size)
{
    int nRet=SCI_SUCCESS;
    DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();
    JINF_ENC_IN_PARAM_T jpeg_in={0x00};
    JINF_ENC_OUT_PARAM_T jpeg_out={0x00};
    DCAMERA_YUV_SRC_T yuv_data={0x00};

    *jpeg_buf_size=0x00;

    _EncodeYYUVToThumbnail(NULL);
/*
    yuv_data.src_size.w=pContext->target_width;
    yuv_data.src_size.h=pContext->target_width;
    yuv_data.src.y_addr=pContext->yyuv_buf.y_addr;
    yuv_data.src.uv_addr=pContext->yyuv_buf.uv_addr;
    yuv_data.src_buf_rect.x=0x00;
    yuv_data.src_buf_rect.y=0x00;
    yuv_data.src_buf_rect.w=pContext->target_width;
    yuv_data.src_buf_rect.h=pContext->target_height;
    DC_ArgbOverlayer(&yuv_data,&(pContext->snapshot_param.capture_date));
*/
    if(SCI_TRUE==pContext->snapshot_param.capture_date.eb)
    {
//        IMGREF_YUV_SRC yuv_data={0x00};
        IMGREF_OSD_T capture_date = {0};
        yuv_data.src_size.w=pContext->target_width;
        yuv_data.src_size.h=pContext->target_height;
        yuv_data.src.y_addr=pContext->yyuv_buf.y_addr;
        yuv_data.src.uv_addr=pContext->yyuv_buf.uv_addr;
        yuv_data.src_buf_rect.x=0x00;
        yuv_data.src_buf_rect.y=0x00;
        yuv_data.src_buf_rect.w=pContext->target_width;
        yuv_data.src_buf_rect.h=pContext->target_height;
       
        capture_date.color_key = pContext->snapshot_param.capture_date.color_key;
        capture_date.dataformat = IMGREF_FORMAT_ARGB888;       
        capture_date.osd_addr.y_addr= pContext->snapshot_param.capture_date.osd_addr.y_addr; 
        capture_date.osd_offset= pContext->snapshot_param.capture_date.osd_offset;
        capture_date.overlay_rect.x=pContext->snapshot_param.capture_date.overlay_rect.x;
        capture_date.overlay_rect.y=pContext->snapshot_param.capture_date.overlay_rect.y;
        capture_date.overlay_rect.w=pContext->snapshot_param.capture_date.overlay_rect.w;
        capture_date.overlay_rect.h=pContext->snapshot_param.capture_date.overlay_rect.h;
        
        
        DC_ArgbOverlayer(&yuv_data, &capture_date);
        pContext->snapshot_param.capture_date.osd_offset = capture_date.osd_offset; //update offset
    }

    //SCI_PASSERT(0,("Y:0x%x, UV:0x%x,", yuv_data.src.y_addr,yuv_data.src.uv_addr));

    jpeg_in.target_buf_ptr=pContext->jpeg_buf_ptr;
    jpeg_in.target_buf_size=pContext->jpeg_buf_len;
    //encoding jpeg in frame mode 
    jpeg_in.yuv_info.yuv_chn.y_chn_ptr=(uint8 *)pContext->yyuv_buf.y_addr;
    jpeg_in.yuv_info.yuv_chn.u_chn_ptr=(uint8 *)pContext->yyuv_buf.uv_addr;
    jpeg_in.yuv_info.sample_format=JINF_FORMAT_YUV422;
    jpeg_in.yuv_info.data_pattern=JINF_YUV_PATTERN_CHN_Y_UV;
    jpeg_in.yuv_info.data_endian=JINF_DATA_BIG_ENDIAN;

    jpeg_in.src_width=pContext->target_width;
    jpeg_in.src_height=pContext->target_height;
    jpeg_in.quality=pContext->snapshot_param.enc_quality_level;/*lint !e64*///JINF_QUALITY_MIDDLE_LOW;

    jpeg_in.encode_buf_ptr=pContext->jpeg_fw_ptr;
    jpeg_in.encode_buf_size=pContext->jpeg_fw_len;
    jpeg_in.read_yuv_func=PNULL;
    jpeg_in.write_file_func=PNULL;
    jpeg_in.async_callback=PNULL;

    if(JINF_SUCCESS != IMGJPEG_EncodeEx(&jpeg_in, &jpeg_out))
    {
        //DCAM_PRINT:"_EncodeYYUVToJpeg(),jpeg encoding error,line %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_676_112_2_18_1_23_29_97,(uint8*)"d",__LINE__);
        return SCI_ERROR;
    }

    pContext->target_width=jpeg_out.output_width;
    pContext->target_height=jpeg_out.output_height;
    pContext->jpeg_buf_ptr=jpeg_out.jpeg_buf_ptr; 
    pContext->jpeg_size=jpeg_out.jpeg_buf_size;

    //DCAM_PRINT:"DCAM: _EncodeYYUVToJpeg(),jpeg_buf_ptr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_685_112_2_18_1_23_29_98,(uint8*)"d",pContext->jpeg_buf_ptr);
    //DCAM_PRINT:"DCAM: _EncodeYYUVToJpeg(),jpeg_size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_686_112_2_18_1_23_29_99,(uint8*)"d",pContext->jpeg_size);

    DC_RETURN_VAL_IF_FAIL_WARNING(DC_ExifGreate(), DCAMERA_OP_ERROR, ("DCAM: DC_JpegProcess greate exif err ,line %d",__LINE__));

    //*jpeg_buf_addr=(uint32)pContext->jpeg_buf_ptr;
    //*jpeg_buf_size=pContext->jpeg_size;

    return nRet;
}

/****************************************************************************************/
// Description: release jpeg slice encoder iram
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
PUBLIC int32 _EncodeYYUVSliceToJpeg(void)
{
    int nRet = SCI_SUCCESS;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();
    JINF_ENC_IN_PARAM_T jpeg_in={0};
    JINF_ENC_OUT_PARAM_T jpeg_out={0};
    uint32 interpolation_type=0x00;

    #if (defined(PLATFORM_SC6600L))
    DC_Get_JpegSliceEncoderIram();
    #endif

    _EncodeYYUVToThumbnail(NULL);

    interpolation_type=DC_GetInterpolationType();
    pContext->enc_slice_height=DC_GetInterpolationSliceLine(pContext->actual_width, pContext->target_width, interpolation_type);
    pContext->back_line=0x00;
    jpeg_in.target_buf_ptr=pContext->jpeg_buf_ptr;
    jpeg_in.target_buf_size=pContext->jpeg_buf_len;
    //encoding jpeg in frame mode 
    jpeg_in.yuv_info.yuv_chn.y_chn_ptr	= (uint8 *)pContext->yyuv_buf.y_addr;
    jpeg_in.yuv_info.yuv_chn.u_chn_ptr	= (uint8 *)pContext->yyuv_buf.uv_addr;
    jpeg_in.yuv_info.sample_format	= JINF_FORMAT_YUV422;
    jpeg_in.yuv_info.data_pattern	= JINF_YUV_PATTERN_CHN_Y_UV;
    jpeg_in.yuv_info.data_endian	= JINF_DATA_BIG_ENDIAN;
    jpeg_in.src_width		= pContext->target_width;
    jpeg_in.src_height		= pContext->target_height;
    jpeg_in.quality         = pContext->snapshot_param.enc_quality_level;/*lint !e64*/
    jpeg_in.encode_buf_ptr  = pContext->jpeg_fw_ptr;
    jpeg_in.encode_buf_size = pContext->jpeg_fw_len;
    jpeg_in.read_yuv_func   = _GetSliceYUVData;
    jpeg_in.write_file_func = PNULL;
    jpeg_in.async_callback  = PNULL;
    jpeg_in.enc_slice_height= pContext->enc_slice_height;

    if(JINF_SUCCESS!=IMGJPEG_EncodeEx(&jpeg_in, &jpeg_out))
    {
        pContext->jpeg_buf_ptr=PNULL;
        pContext->jpeg_size=0x00;
        //DCAM_PRINT:"DCAM: _EncodeYYUVSliceToJpeg(),jpeg encoding err,line %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_745_112_2_18_1_23_29_100,(uint8*)"d",__LINE__);
        return SCI_ERROR;
    }

    pContext->target_width=jpeg_out.output_width;
    pContext->target_height=jpeg_out.output_height;
    pContext->jpeg_buf_ptr=jpeg_out.jpeg_buf_ptr; 
    pContext->jpeg_size=jpeg_out.jpeg_buf_size;

    //DCAM_PRINT:"DCAM: _EncodeYYUVSliceToJpeg(),jpeg_buf_ptr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_754_112_2_18_1_23_29_101,(uint8*)"d",pContext->jpeg_buf_ptr);
    //DCAM_PRINT:"DCAM: _EncodeYYUVSliceToJpeg(),jpeg_size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_755_112_2_18_1_23_29_102,(uint8*)"d",pContext->jpeg_size);

    DC_RETURN_VAL_IF_FAIL_WARNING(DC_ExifGreate(), DCAMERA_OP_ERROR, ("DCAM: DC_JpegProcess greate exif err ,line %d",__LINE__));

    #if (defined(PLATFORM_SC6600L))            
    DC_Release_JpegSliceEncoderIram();
    #endif

    return nRet;
}

/****************************************************************************************/
// Description: jpeg process
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
PUBLIC uint32 DC_JpegProcess(uint32* process_ptr)
{
    uint32 rtn=SCI_SUCCESS;
    DCAM_CONTEXT_T* pContext=_GetDCAMContext();
    ISP_JPEG_PARAM_T* isp_snapshot_ptr=(ISP_JPEG_PARAM_T*)process_ptr;
    JINF_MINI_IN_PARAM_T minature_in_param={0x00};
    JINF_MINI_IN_PARAM_T* minature_in_param_ptr=NULL;
    JINF_MINI_OUT_PARAM_T minature_out_param={0x00};
    JINF_MINI_OUT_PARAM_T* minature_out_param_ptr=NULL;
    DCAMERA_RECT_T jpg_trim={0x00};

    _EncodeYYUVToThumbnail(process_ptr);

    if((isp_snapshot_ptr->input_size.w!=isp_snapshot_ptr->input_rect.w)
        ||(isp_snapshot_ptr->input_size.w!=pContext->target_width))
    {
        minature_in_param_ptr=&minature_in_param;
        minature_out_param_ptr=&minature_out_param;

        SCI_MEMCPY((void*)&(jpg_trim), (void*)&(isp_snapshot_ptr->input_rect), sizeof(DCAMERA_RECT_T));
        
        _DC_SetMinatureInParam(minature_in_param_ptr, jpg_trim);

        if(SCI_SUCCESS!=IMGJPEG_CreateMiniatureEx(minature_in_param_ptr, minature_out_param_ptr))
        {
            pContext->jpeg_buf_ptr=NULL;
            pContext->jpeg_size=0x00;
            //DCAM_PRINT:"DCAM: DC_JpegProcess,Greate Miniature err ,line %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_803_112_2_18_1_23_29_103,(uint8*)"d",__LINE__);
            return SCI_ERROR;
        }
        
        pContext->target_width=minature_out_param_ptr->output_width;
        pContext->target_height=minature_out_param_ptr->output_height;
        pContext->jpeg_buf_ptr=minature_out_param_ptr->jpeg_buf_ptr; 
        pContext->jpeg_size=minature_out_param_ptr->jpeg_buf_size;
    }
    else
    {
        pContext->jpeg_buf_ptr=pContext->src_jpg_addr; 
    }

    //DCAM_PRINT:"DCAM: DC_JpegProcess(),jpeg_buf_ptr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_817_112_2_18_1_23_29_104,(uint8*)"d",pContext->jpeg_buf_ptr);
    //DCAM_PRINT:"DCAM: DC_JpegProcess(),jpeg_size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_818_112_2_18_1_23_29_105,(uint8*)"d",pContext->jpeg_size);

    DC_RETURN_VAL_IF_FAIL_WARNING(DC_ExifGreate(), DCAMERA_OP_ERROR, ("DCAM: DC_JpegProcess greate exif err ,line %d",__LINE__));

    return rtn;
}

/****************************************************************************************/
// Description: release jpeg slice encoder iram
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
PUBLIC int32 _DecodeJpegToYYUV(void)
{
    int nRet = SCI_SUCCESS;
    DCAM_CONTEXT_T *pContext = _GetDCAMContext();
    JINF_DEC_IN_PARAM_T		jpeg_in={0};
    JINF_DEC_OUT_PARAM_T	jpeg_out={0};
    JINF_INFO_T				jpeg_info = {0};
    DCAMERA_RECT_T			jpeg_crop_rect = {0};
    JINF_GET_INFO_IN_T		jpeg_get_info_in = {0};

    jpeg_get_info_in.jpeg_buf_ptr = pContext->review_param.jpeg_buffer;
    jpeg_get_info_in.jpeg_buf_size = pContext->review_param.jpeg_len;
    IMGJPEG_GetInfo(&jpeg_get_info_in,&jpeg_info);

    SCI_MEMSET(&jpeg_in, 0x0, sizeof(JINF_DEC_IN_PARAM_T));
    SCI_MEMSET(&jpeg_out, 0x0, sizeof(JINF_DEC_OUT_PARAM_T));

    //this part should be modify after crop and scale of jpeg decoder are completed
    if(jpeg_info.image_width%pContext->review_param.disp_rect.w 
        ||jpeg_info.image_height%pContext->review_param.disp_rect.h)
    {
        jpeg_in.target_width=max((jpeg_info.image_width/4),QVGA_WIDTH);
        jpeg_in.target_height=max((jpeg_info.image_height/4),QVGA_HEIGHT);
    }
    else
    {
        jpeg_in.target_width=pContext->review_param.disp_rect.w;
        jpeg_in.target_height=pContext->review_param.disp_rect.h;
    }

    jpeg_in.jpeg_buf_ptr=pContext->review_param.jpeg_buffer;
    jpeg_in.jpeg_buf_size=pContext->review_param.jpeg_len;
    jpeg_in.crop_rect.x=jpeg_crop_rect.x;
    jpeg_in.crop_rect.y=jpeg_crop_rect.y;  
    jpeg_in.crop_rect.w=jpeg_crop_rect.w;
    jpeg_in.crop_rect.h=jpeg_crop_rect.h;
    jpeg_in.decode_buf_ptr=pContext->jpeg_fw_ptr;
    jpeg_in.decode_buf_size=pContext->jpeg_fw_len;
    jpeg_in.target_buf_ptr=(uint8 *)pContext->yyuv_buf.y_addr;
    jpeg_in.target_buf_size=pContext->yyuv_buf_len;
    jpeg_in.target_data_type=JINF_DATA_TYPE_YUV;
    jpeg_in.read_file_func=PNULL;
    jpeg_in.async_callback=PNULL;

    if(JINF_SUCCESS != IMGJPEG_DecodeEx(&jpeg_in, &jpeg_out))
    {
        nRet = SCI_ERROR;
        //DCAM_PRINT:"_DecodeJpegToYYUV(),jpeg decoding error,line %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CODEC_882_112_2_18_1_23_29_106,(uint8*)"d",__LINE__);
    }

    pContext->target_width=jpeg_out.output_width;
    pContext->target_height=jpeg_out.output_height;
    pContext->yyuv_buf.y_addr=(uint32)jpeg_out.out_data_info.yuv.yuv_chn.y_chn_ptr;
    pContext->yyuv_buf.uv_addr=(uint32)jpeg_out.out_data_info.yuv.yuv_chn.u_chn_ptr;

    return nRet;
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
/*lint -restore */ 
#ifdef   __cplusplus
    }
#endif// End
