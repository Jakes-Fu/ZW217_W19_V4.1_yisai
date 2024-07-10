/******************************************************************************
 ** File Name:      Interface_JPG.c                                           *
 ** Author:         Yi Wang                                                   *
 ** DATE:           05/14/2007                                                *
 ** Copyright:      20067 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic interfaces of mp4 codec       *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 05/14/2007    Yi Wang		  Create.                                     *
 *****************************************************************************/
#include "ms_ref_dc_trc.h"
#include "jpeg_interface.h"
#include "dal_jpeg.h"
#include "os_api.h"
#include "dc_cfg.h"
#include "chng_freq.h"
#include "dal_dcamera.h"
#include "dcamera_mem.h"

#define	JPEG_ENC_QUALITY_MID                    6
#define JPEG_CODEC_SEMA_NAME                    "Semaphore for JPEG Codec"	
#define	JPEG_CODEC_TIME_OUT                     2000 //s
#if defined(PLATFORM_SC8800G)
#define JPEG_CODEC_YUV_BUF_STRIDER              512
#else
#define JPEG_CODEC_YUV_BUF_STRIDER              0
#endif
#define JPEG_TMP_BUF_SIZE                       DC_JPEG_TMP_BUF_LEN

LOCAL uint32  s_jpg_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
LOCAL SCI_SEMAPHORE_PTR s_jpeg_codec_sema_ptr = NULL;
LOCAL BOOLEAN is_codec_exit = TRUE;
LOCAL void*   p_jpg_tmp_buf = SCI_NULL;

LOCAL void JPEG_GetDecodingMCUSize(JINF_YUV_FORMAT_E yuv_sample_format, DCAMERA_SIZE_T *mcu_size_ptr)
{
	switch (yuv_sample_format)
	{
	case JINF_FORMAT_YUV444:
	case JINF_FORMAT_YUV400:		
		mcu_size_ptr->w = 8;
		mcu_size_ptr->h = 8;
		break;

	case JINF_FORMAT_YUV422:
		mcu_size_ptr->w = 16;
		mcu_size_ptr->h = 8;
		break;

	case JINF_FORMAT_YUV422R:
		mcu_size_ptr->w = 8;
		mcu_size_ptr->h = 16;		
		break;

	case JINF_FORMAT_YUV420:
		mcu_size_ptr->w = 16;
		mcu_size_ptr->h = 16;
		break;			

	case JINF_FORMAT_YUV411:
		mcu_size_ptr->w = 32;
		mcu_size_ptr->h = 8;
		break;	

	case JINF_FORMAT_YUV411R:
		mcu_size_ptr->w = 8;
		mcu_size_ptr->h = 32;		
		break;		

	default:
		//SCI_TRACE_LOW:"[JPEG_GetMCUSize] unsupport quality"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,JPEG_71_112_2_18_1_23_47_283,(uint8*)"");
		mcu_size_ptr->w = 8;
		mcu_size_ptr->h = 8;		
		break;				
	}	
}

LOCAL JPEG_ENC_RET_E JPEG_CodecEnter(void)
{
    JPEG_ENC_RET_E      status = SCI_SUCCESS;	

    if(SCI_NULL == s_jpeg_codec_sema_ptr)
    {
        s_jpeg_codec_sema_ptr = SCI_CreateSemaphore(JPEG_CODEC_SEMA_NAME, 1);
    }
    status = SCI_GetSemaphore(s_jpeg_codec_sema_ptr, JPEG_CODEC_TIME_OUT);
    if(status != SCI_SUCCESS)
    {
        status =  JPEG_ENC_HW_TIMEOUT;
    }
    return  status;   
}

LOCAL void JPEG_CodecExit(void)   
{
    if(PNULL != p_jpg_tmp_buf)
    {
        SCI_TRACE_LOW("david free tmp buf 0x%x", p_jpg_tmp_buf);
        SCI_FREE(p_jpg_tmp_buf);
        p_jpg_tmp_buf = PNULL;
    }

    if(s_jpeg_codec_sema_ptr)	
    {
        SCI_PutSemaphore(s_jpeg_codec_sema_ptr);
    }
}

PUBLIC  JPEG_ENC_RET_E JPEG_EncodeJpeg(JPEG_ENC_IN_PARAM_T *in_param,JPEG_ENC_OUT_PARAM_T *out_param)
{
    uint16               width = in_param->image_rect.w;
    uint16               height = in_param->image_rect.h;
    uint32               compress_level = in_param->quality_level;
    uint8                *pOutStream = in_param->jpeg_buf_ptr; 
    uint32               status = SCI_SUCCESS;	
    uint8                *jpeg_enc_ptr = PNULL;
    JINF_ENC_IN_PARAM_T  jpeg_in={0};
    JINF_ENC_OUT_PARAM_T jpeg_out={0};

	//SCI_TRACE_LOW("david JPEG_EncodeJpeg ");

    status = JPEG_CodecEnter();
    if(status)  return status;

	
    //SCI_TRACE_LOW:"JPEG_EncodeJpeg ----start, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JPEG_122_112_2_18_1_23_47_284,(uint8*)"d", SCI_GetTickCount());

#if 0
    if(compress_level> JPEG_ENC_QUALITY_MID)//high quality
        compress_level = JINF_QUALITY_MIDDLE_HIGH;
    else if(compress_level < JPEG_ENC_QUALITY_MID)//low quality
        compress_level = JINF_QUALITY_LOW;
    else
        compress_level = JINF_QUALITY_MIDDLE;
#endif

	if(compress_level > JPEG_ENC_QUALITY_MID)
	{
		compress_level = JINF_QUALITY_MIDDLE_HIGH;
	}
	else if(compress_level <= 1)
	{
		compress_level = JINF_QUALITY_MIDDLE_LOW;
	}
	else
	{
		compress_level = JINF_QUALITY_MIDDLE;
	}
//	compress_level = JINF_QUALITY_MIDDLE;

    //encoding jpeg in frame mode 
    jpeg_in.yuv_info.yuv_chn.y_chn_ptr	= (uint8*)in_param->yuv_addr.y_chn_addr;
    jpeg_in.yuv_info.yuv_chn.u_chn_ptr	= (uint8*)in_param->yuv_addr.u_chn_addr;
    jpeg_in.yuv_info.yuv_chn.v_chn_ptr	= (uint8*)in_param->yuv_addr.v_chn_addr;
    jpeg_in.yuv_info.sample_format	= JINF_FORMAT_YUV420;

	//david
	in_param->enc_data_type = JPEG_ENC_DATA_TYPE_YUV422;
    if(in_param->enc_data_type == JPEG_ENC_DATA_TYPE_YUV422)
    {
        jpeg_in.yuv_info.sample_format	= JINF_FORMAT_YUV422;
    }
    else if(in_param->enc_data_type == JPEG_ENC_DATA_TYPE_YUV420)
    {
        jpeg_in.yuv_info.sample_format	= JINF_FORMAT_YUV420;
    }
    jpeg_in.yuv_info.data_pattern	= JINF_YUV_PATTERN_CHN_Y_UV;


#if defined(CHIP_ENDIAN_LITTLE)
    jpeg_in.yuv_info.data_endian	= JINF_DATA_BIG_ENDIAN;
#endif

//david 
#if 0  //#ifndef WIN32
    if(DCAMERA_IsOpen())
    {
        jpeg_enc_ptr = (uint8*)DCAMERA_CaptureGetJPGTmpBuf(&jpeg_in.encode_buf_size);
    }
    else
#endif

    if(in_param->p_jpg_tmp && in_param->jpg_tmp_len)
    {
       // SCI_TRACE_LOW("david Encode alloc buffer outside");
        jpeg_enc_ptr = (uint8*)in_param->p_jpg_tmp;
        jpeg_in.encode_buf_size = in_param->jpg_tmp_len;
    }
    else
    {
        //SCI_TRACE_LOW("david Encode alloc buffer inside");
        if(SCI_NULL == p_jpg_tmp_buf)
        {
            p_jpg_tmp_buf = SCI_ALLOCA(JPEG_TMP_BUF_SIZE);
        }
        jpeg_in.encode_buf_size = JPEG_TMP_BUF_SIZE; 
        jpeg_enc_ptr = (uint8*)p_jpg_tmp_buf;
    }

    if(PNULL == jpeg_enc_ptr)
    {
    	SCI_TRACE_LOW("david JPEG_EncodeJpeg alloc buffer fail size=%u exit", jpeg_in.encode_buf_size);
        JPEG_CodecExit();
        return JPEG_ENC_MEMORY_ERROR;
    }

    jpeg_in.src_width         = width;
    jpeg_in.src_height        = height;
    jpeg_in.quality           = compress_level;//JINF_QUALITY_MIDDLE_LOW;
    jpeg_in.target_buf_ptr    = pOutStream;
    jpeg_in.target_buf_size   = in_param->jpeg_buf_size;
    jpeg_in.encode_buf_ptr    = (uint8*)jpeg_enc_ptr;
    jpeg_in.read_yuv_func     = PNULL;
    jpeg_in.write_file_func   = PNULL;
    jpeg_in.async_callback    = PNULL;
    jpeg_in.enc_slice_height  = in_param->enc_slice_height;

    if(in_param->reserved == 1 )
	{
		jpeg_in.reserved = 1;
	}
	else
	{
		jpeg_in.reserved = 0;
	}
    if( JINF_SUCCESS != IMGJPEG_Encode(&jpeg_in, &jpeg_out))
    {
        JPEG_CodecExit();
        return JPEG_ENC_ERROR;
    }
	
    out_param->out_size = jpeg_out.jpeg_buf_size;	
    SCI_TRACE_LOW("david JPEG_EncodeJpeg over size: %d", out_param->out_size);
    //SCI_TRACE_LOW:"JPEG_EncodeJpeg ----end :%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JPEG_191_112_2_18_1_23_47_285,(uint8*)"d",SCI_GetTickCount());

    JPEG_CodecExit();
    return JPEG_ENC_OK;
}


PUBLIC JPEG_DEC_RET_E JPEG_DecodeJpeg(JPEG_DEC_IN_PARAM_T  *in_param, JPEG_DEC_OUT_PARAM_T *out_param )
{
    uint32                  size;
    uint8                   *pYUVFrame = PNULL;
    uint32                  rtn = JPEG_DEC_OK;
    uint32                  status = SCI_SUCCESS;	
    JINF_DEC_IN_PARAM_T     jpeg_in={0};
    JINF_DEC_OUT_PARAM_T    jpeg_out={0};
    JINF_INFO_T             jpeg_info = {0};
    JINF_GET_INFO_IN_T      jpeg_get_info_in = {0};
    uint8                   *jpeg_enc_ptr = PNULL;
	DCAMERA_SIZE_T          mcu_size = {0};

    status = JPEG_CodecEnter();
    if(status)  return status;

    //SCI_TRACE_LOW:"JPEG_DecodeJpeg ----start, %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JPEG_214_112_2_18_1_23_47_286,(uint8*)"d", SCI_GetTickCount());

    jpeg_get_info_in.jpeg_buf_ptr = (uint8*)in_param->jpeg_buf;
    jpeg_get_info_in.jpeg_buf_size = in_param->jpeg_size;
    if(JINF_SUCCESS != IMGJPEG_GetInfo(&jpeg_get_info_in,
                                       &jpeg_info))
    {
        JPEG_CodecExit();
        return JPEG_DEC_ERROR;
    }
    
    if(0 == in_param->target_width || 
    0 == in_param->target_height)
    {
        jpeg_in.target_width    = jpeg_info.image_width;
        jpeg_in.target_height   = jpeg_info.image_height;
    }
    else
    {
        jpeg_in.target_width    = in_param->target_width;
        jpeg_in.target_height   = in_param->target_height;
    }

    JPEG_GetDecodingMCUSize(jpeg_info.sample_format,&mcu_size);
    jpeg_in.target_width = (jpeg_in.target_width + mcu_size.w - 1) / mcu_size.w * mcu_size.w;
    jpeg_in.target_height= (jpeg_in.target_height + mcu_size.h - 1) / mcu_size.h * mcu_size.h;
    
    switch(jpeg_info.sample_format)
    {
        case JINF_FORMAT_YUV422:

            size = jpeg_in.target_width * jpeg_in.target_height * 2 + JPEG_CODEC_YUV_BUF_STRIDER;

        break;
        
        case JINF_FORMAT_YUV420:

            size = jpeg_in.target_width * jpeg_in.target_height * 3 /2 + JPEG_CODEC_YUV_BUF_STRIDER;

        break;
        
        default:
        {
            //SCI_TRACE_LOW:"JPEG_DecodeJpeg ----unsupported yuv format"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,JPEG_257_112_2_18_1_23_47_287,(uint8*)"");
            JPEG_CodecExit();
            return JPEG_DEC_ERROR;
        }
    }

    if((in_param->yuv_buf) && (in_param->yuv_size >= size))
    {
        pYUVFrame = (uint8*)in_param->yuv_buf;
        size = in_param->yuv_size;//use external memory
    }
    else
    {
        JPEG_CodecExit();
        return JPEG_DEC_MEMORY_ERROR;
    }
    if(in_param->p_jpg_tmp && in_param->jpg_tmp_len)
    {
        jpeg_enc_ptr = (uint8*)in_param->p_jpg_tmp;
        jpeg_in.decode_buf_size = in_param->jpg_tmp_len;
    }
    else
    {
        if(SCI_NULL == p_jpg_tmp_buf)
        {
            p_jpg_tmp_buf = SCI_ALLOCA(JPEG_TMP_BUF_SIZE);
        }
        jpeg_in.decode_buf_size = JPEG_TMP_BUF_SIZE; 
        jpeg_enc_ptr = (uint8*)p_jpg_tmp_buf;
    }

    if(PNULL == jpeg_enc_ptr)
    {
        JPEG_CodecExit();
        return JPEG_DEC_MEMORY_ERROR;
    }
    jpeg_in.jpeg_buf_ptr     = (uint8*)in_param->jpeg_buf;
    jpeg_in.jpeg_buf_size    = in_param->jpeg_size;
    jpeg_in.decode_buf_ptr   =  jpeg_enc_ptr;
    jpeg_in.target_buf_ptr   = pYUVFrame;
    jpeg_in.target_buf_size  = size;
    jpeg_in.target_data_type = JINF_DATA_TYPE_YUV;
    jpeg_in.read_file_func   = PNULL;
    jpeg_in.async_callback   = PNULL;

#if defined(CHIP_ENDIAN_LITTLE)	
    jpeg_in.target_data_endian	= JINF_DATA_BIG_ENDIAN;
#endif

    if(JINF_SUCCESS != IMGJPEG_Decode(&jpeg_in, &jpeg_out))
    {
        JPEG_CodecExit();
        return JPEG_DEC_ERROR;
    }
    out_param->data_addr.yuv_addr.y_chn_addr = (uint32)jpeg_out.out_data_info.yuv.yuv_chn.y_chn_ptr;
    out_param->data_addr.yuv_addr.u_chn_addr = (uint32)(jpeg_out.out_data_info.yuv.yuv_chn.u_chn_ptr);
    out_param->data_addr.yuv_addr.v_chn_addr = (uint32)(jpeg_out.out_data_info.yuv.yuv_chn.v_chn_ptr);
    out_param->output_width = jpeg_out.output_width;
    out_param->output_height = jpeg_out.output_height;

    if(JINF_FORMAT_YUV422== jpeg_out.out_data_info.yuv.sample_format)
    {
        out_param->output_data_type = JPEG_DEC_DATA_TYPE_YUV422;
    }
    else if(JINF_FORMAT_YUV420== jpeg_out.out_data_info.yuv.sample_format)
    {
        out_param->output_data_type = JPEG_DEC_DATA_TYPE_YUV420;
    }
    else
    {
        //SCI_TRACE_LOW:"JPEG_DecodeJpeg ----unsupported yuv format"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,JPEG_327_112_2_18_1_23_48_288,(uint8*)"");
        JPEG_CodecExit();
        return JPEG_DEC_ERROR;
    }


    //SCI_TRACE_LOW:"JPEG_DecodeJpeg : y_chn_addr = 0x%x,u_chn_addr = 0x%x,v_chn_addr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JPEG_336_112_2_18_1_23_48_289,(uint8*)"ddd", out_param->data_addr.yuv_addr.y_chn_addr ,out_param->data_addr.yuv_addr.u_chn_addr ,out_param->data_addr.yuv_addr.v_chn_addr );

    //SCI_TRACE_LOW:"JPEG_DecodeJpeg ----end :%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JPEG_338_112_2_18_1_23_48_290,(uint8*)"d",SCI_GetTickCount());

    JPEG_CodecExit();
    return rtn;
}

PUBLIC int JPEG_ZoomJpeg(JPEG_ZOOM_IN_PARAM_T *pIn, uint8 **pJpg, uint32 *size)
{
    int                     rVal = SCI_SUCCESS;
    JINF_MINI_IN_PARAM_T    in   = {0};
    JINF_MINI_OUT_PARAM_T   out  = {0};

    //SCI_TRACE_LOW:"DC: JPEG_ZoomJpeg -> start time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JPEG_351_112_2_18_1_23_48_291,(uint8*)"d",                                      SCI_GetTickCount());

    in.jpeg_buf_ptr     = pIn->jpeg_buf_ptr;
    in.jpeg_buf_size    = pIn->jpeg_buf_size;

    in.crop_rect.x      = pIn->crop_rect.x;
    in.crop_rect.y      = pIn->crop_rect.y;
    in.crop_rect.w      = pIn->crop_rect.w;
    in.crop_rect.h      = pIn->crop_rect.h;    

    in.target_width     = pIn->target_width;
    in.target_height    = pIn->target_height;
    in.quality          = (JINF_QUALITY_E)pIn->quality;
    
    in.target_buf_ptr   = pIn->target_buf_ptr;
    in.target_buf_size  = pIn->target_buf_size;

    in.decode_buf_ptr   = pIn->decode_buf_ptr;
    in.decode_buf_size  = pIn->decode_buf_size;
    
    rVal = IMGJPEG_CreateMiniature(&in, &out);
    
    if(!rVal)
    {
        *size = out.jpeg_buf_size;
        *pJpg  = out.jpeg_buf_ptr;
    }

    //SCI_TRACE_LOW:"DC: JPEG_ZoomJpeg -> end time = %d, rVal = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JPEG_380_112_2_18_1_23_48_292,(uint8*)"dd",                                      SCI_GetTickCount(), rVal);
    
    return rVal;
}
