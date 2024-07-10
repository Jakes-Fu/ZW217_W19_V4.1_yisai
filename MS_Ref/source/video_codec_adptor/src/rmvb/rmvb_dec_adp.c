
/*****************************************************************************
 * head files.
 *****************************************************************************/
#include "sci_types.h"
#include "os_api.h"

#include "video_codec_adptor.h"

//
#include "rvdec.h"
#include "mmcodec.h"


/*****************************************************************************
 * macro
 *****************************************************************************/
#define RMVB_DEC_INTER_FAST_MEM_SIZE    (115<<10)
#define RMVB_DEC_IMG_FRM_NUM_MAX        0x3

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
typedef struct
{
    VCODEC_ADP_COMP_FRM_T frm_grp[RMVB_DEC_IMG_FRM_NUM_MAX];
    //
    uint32 using_flg[RMVB_DEC_IMG_FRM_NUM_MAX]; // 0, free; 1,using.
}IMG_FRM_GRP_T;


typedef struct
{
    VCODEC_ADP_HANDLE dec_handle;
    uint32 err_id;
    
    //
    VCODEC_ADP_STRM_FMT_T       strm_fmt;
    VCODEC_ADP_CALLBACK_PORT_T  err_port;
    VCODEC_ADP_IMG_FMT_T    img_out_fmt;
    //
    uint8 *p_dec_buf_inter;
    uint8 *p_dec_buf_ext;
    //
    uint32 b_expected_IVOP;
    uint32 b_disp_skip;
    uint32 b_need_downsize;
    //
    IMG_FRM_GRP_T  img_frm_grp;
    VCODEC_ADP_COMP_FRM_T *p_frm_cur;   // dec out but don't get by app.
}RVDEC_CONTAINER_T;




/*****************************************************************************
 * Local functions
 *****************************************************************************/

#define RVDEC_PRT_ERR    SCI_TRACE_LOW//SCI_TraceLow0
#define RVDEC_PRT_NONE   //(...)

#define RMVB_DEC_STR_CMP(p_str1,p_str2)  (strcmp((p_str1),(p_str2)))

#define _RMVB_DEC_DIS_INT_      SCI_DisableIRQ()
#define _RMVB_DEC_EN_INT_       SCI_RestoreIRQ()


static VCODEC_ADP_COMP_FRM_T *_rvdec_frmctx_alloc(IMG_FRM_GRP_T *p_grp)
{
    VCODEC_ADP_COMP_FRM_T *p_node_valid = PNULL;
    uint32 *p_using_flg_peek = PNULL;
    uint32 idx = 0;

    if( PNULL == p_grp )
    {
        return PNULL;
    }
    p_using_flg_peek = &(p_grp->using_flg[0]);

_RMVB_DEC_DIS_INT_;
    for( idx = 0; idx < RMVB_DEC_IMG_FRM_NUM_MAX; idx++)
    {
        if( p_using_flg_peek[idx] == 0 )
        {
            p_node_valid = &(p_grp->frm_grp[idx]);
            p_using_flg_peek[idx] = 1;
            break;
        }
    }
_RMVB_DEC_EN_INT_;

    if(PNULL != p_node_valid)
    {
        memset(p_node_valid,0,sizeof(VCODEC_ADP_COMP_FRM_T));
    }

    return p_node_valid;
}


static int _rvdec_frmctx_free(IMG_FRM_GRP_T *p_grp, VCODEC_ADP_COMP_FRM_T *p_frm)
{
    VCODEC_ADP_COMP_FRM_T *p_frm_peek = PNULL;
    uint32 *p_using_flg_peek = PNULL;
    uint32 idx = 0;
    
    if( (PNULL == p_grp) || (PNULL == p_frm) )
    {
        return PNULL;
    }
    p_using_flg_peek = &(p_grp->using_flg[0]);

_RMVB_DEC_DIS_INT_;
    for( idx = 0; idx < RMVB_DEC_IMG_FRM_NUM_MAX; idx++)
    {
        p_frm_peek = &(p_grp->frm_grp[idx]);
        if( (p_frm_peek == p_frm) && (p_using_flg_peek[idx] == 1) )
        {
            p_using_flg_peek[idx] = 0;
            break;
        }
    }
_RMVB_DEC_EN_INT_;


    return SCI_SUCCESS;
}


LOCAL void _rvdec_free_resource(RVDEC_CONTAINER_T *p_rvdec_ctx)
{
    RVDEC_PRT_ERR("_rvdec_free_resource,err_id = %d",p_rvdec_ctx->err_id);

    if(PNULL == p_rvdec_ctx)
    {
        return;
    }
    
    // free the decoder buffer.
    if(PNULL != p_rvdec_ctx->p_dec_buf_inter)
    {
        SCI_FREE(p_rvdec_ctx->p_dec_buf_inter);
        p_rvdec_ctx->p_dec_buf_inter = PNULL;
    }
    if(PNULL != p_rvdec_ctx->p_dec_buf_ext)
    {
        SCI_FREE(p_rvdec_ctx->p_dec_buf_ext);
        p_rvdec_ctx->p_dec_buf_ext = PNULL;
    }    

    // free the container.
    SCI_FREE(p_rvdec_ctx);
}


// create a decoder and check the format.
LOCAL VCODEC_ADP_HANDLE rvdec_open( VCODEC_ADP_STRM_FMT_T       *p_strm_fmt,
                                    VCODEC_ADP_CALLBACK_PORT_T  *p_err_port
                                  )
{
    RVDEC_CONTAINER_T *p_rvdec_ctx = PNULL;
    //
    MMDecVideoFormat format;
    MMCodecBuffer Buffer;
    MMDecMemSize mem_size = {0};
    MMCodecBuffer dec_ext_buffer={0};

    if(PNULL == p_strm_fmt)
    {
        return PNULL;
    }
    RVDEC_PRT_ERR("rvdec_open,codec_name=%s",p_strm_fmt->codec_name);

    p_rvdec_ctx = (RVDEC_CONTAINER_T *)SCI_ALLOCA(sizeof(RVDEC_CONTAINER_T));
    if(PNULL == p_rvdec_ctx)
    {
        return PNULL;
    }
    // used to check whether the conatiner is destroyed or not.
    memset(p_rvdec_ctx,0,sizeof(RVDEC_CONTAINER_T));
    p_rvdec_ctx->dec_handle = (VCODEC_ADP_HANDLE)p_rvdec_ctx;

    // save the parameter in the container.
    if(PNULL != p_err_port)
    {
        memcpy( &(p_rvdec_ctx->err_port),p_err_port,sizeof(VCODEC_ADP_CALLBACK_PORT_T));
    }
    //
    memcpy(&(p_rvdec_ctx->strm_fmt),p_strm_fmt,sizeof(VCODEC_ADP_STRM_FMT_T));
    // configure data needn't be saved.
    p_rvdec_ctx->strm_fmt.cfg_data.list_len = 0;
    p_rvdec_ctx->strm_fmt.cfg_data.p_first = PNULL;


    //
    if( !RMVB_DEC_STR_CMP(p_strm_fmt->codec_name,"RV40") )
    {
        // support.
        format.video_std = 6;  // 6: RV9
    }
#if 0
    else if( !RMVB_DEC_STR_CMP(p_strm_fmt->codec_name,"RV30") )
    {
        // unsupport.
        format.video_std = 5;  //5: RV8
        return PNULL;
    }
#endif
    else
    {
        // unsupport.
        _rvdec_free_resource(p_rvdec_ctx);
        return PNULL;
    }

    // init the format.
    if( p_strm_fmt->cfg_data.list_len > 0 )
    {
        // TODO: check the configure data, and configure the decoder.
 
        if(PNULL != p_strm_fmt->cfg_data.p_first)
        {
	        format.i_extra = p_strm_fmt->cfg_data.p_first->len;
	        format.p_extra = p_strm_fmt->cfg_data.p_first->p_data;
        }
    }
	format.frame_width = p_strm_fmt->width;
	format.frame_height = p_strm_fmt->height;
	format.frame_rate = p_strm_fmt->frm_rate;// frm_rate/frm_rate_base;

    // allocate inter buffer
    p_rvdec_ctx->p_dec_buf_inter = SCI_ALLOCA(RMVB_DEC_INTER_FAST_MEM_SIZE);
	if(PNULL == p_rvdec_ctx->p_dec_buf_inter)
	{
        _rvdec_free_resource(p_rvdec_ctx);
        return PNULL;
	}
	Buffer.int_buffer_ptr = p_rvdec_ctx->p_dec_buf_inter;
	Buffer.int_size = RMVB_DEC_INTER_FAST_MEM_SIZE;
	if (RvDecInit(&Buffer, &format))
	{
        _rvdec_free_resource(p_rvdec_ctx);
        return PNULL;
	}

    // calculate the size of external memory.
    RVDecMemSizeCal(p_strm_fmt->width, p_strm_fmt->height, &mem_size);
    if(0 == mem_size.memSize)
    {
        _rvdec_free_resource(p_rvdec_ctx);
        return PNULL;
    }
    // allocate external buffer
	p_rvdec_ctx->p_dec_buf_ext = SCI_ALLOCA(mem_size.memSize);
	if(PNULL == p_rvdec_ctx->p_dec_buf_ext)
	{
        _rvdec_free_resource(p_rvdec_ctx);
        return PNULL;
	}
	dec_ext_buffer.common_buffer_ptr = p_rvdec_ctx->p_dec_buf_ext;
	dec_ext_buffer.size = mem_size.memSize;
	if (RvDecMemInit(&dec_ext_buffer) != MMDEC_OK)
	{
        _rvdec_free_resource(p_rvdec_ctx);
        return PNULL;
	}

    return (VCODEC_ADP_HANDLE)p_rvdec_ctx;
}


// close decoder.
LOCAL int rvdec_close(VCODEC_ADP_HANDLE *p_decoder)
{
    RVDEC_CONTAINER_T *p_rvdec_ctx = PNULL;

    RVDEC_PRT_ERR("rvdec_close,p_decoder=0x%x",p_decoder);

    // check the parameter.
    if(PNULL == p_decoder)
    {
        return VCODEC_ADP_RET_PARAM_ERR;
    }
    p_rvdec_ctx = (RVDEC_CONTAINER_T *)p_decoder;
    if(p_rvdec_ctx->dec_handle != p_decoder)
    {
        return VCODEC_ADP_RET_PARAM_ERR;
    }

    // close and free the decoder.
    if(RvDecRelease() != MMDEC_OK)
    {
        RVDEC_PRT_ERR("rvdec_close, RvDecRelease failed!");
    }
    _rvdec_free_resource(p_rvdec_ctx);

    return VCODEC_ADP_RET_SUCCESS;
}

// set/get the media file information/user data.
LOCAL int rvdec_setfmt(VCODEC_ADP_HANDLE *p_decoder,VCODEC_ADP_STRM_FMT_T *p_strm_fmt)
{
#if 0
    RVDEC_CONTAINER_T *p_rvdec_ctx = PNULL;

    RVDEC_PRT_ERR("rvdec_setfmt,p_decoder=0x%x,p_fmt=0x%x",p_decoder,p_strm_fmt);
    // check the parameter.
    if( (PNULL == p_decoder) || (PNULL == p_strm_fmt))
    {
        return VCODEC_ADP_RET_PARAM_ERR;
    }
    p_rvdec_ctx = (RVDEC_CONTAINER_T *)p_decoder;
    if(p_rvdec_ctx->dec_handle != p_decoder)
    {
        return VCODEC_ADP_RET_PARAM_ERR;
    }

    //
    memcpy(&(p_rvdec_ctx->strm_fmt),p_strm_fmt,sizeof(VCODEC_ADP_STRM_FMT_T));
    // configure data needn't be saved.
    p_rvdec_ctx->strm_fmt.cfg_data.list_len = 0;
    p_rvdec_ctx->strm_fmt.cfg_data.p_first = PNULL;
    //
    if( p_strm_fmt->cfg_data.list_len > 0 )
    {
        // TODO: check the configure data, and configure the decoder.
    }
#endif

    return VCODEC_ADP_RET_SUCCESS;
}
LOCAL int rvdec_getfmt(VCODEC_ADP_HANDLE *p_decoder,VCODEC_ADP_STRM_FMT_T *p_strm_fmt)
{
    return VCODEC_ADP_RET_SUCCESS;
}

// parse the stream data.
LOCAL int rvdec_getfrminfo( VCODEC_ADP_HANDLE *p_decoder, 
                            VCODEC_ADP_STRM_T *p_frm,
                            VCODEC_ADP_FRM_INFO_T *p_frm_info
                          )
{
    return VCODEC_ADP_RET_SUCCESS;
}

// set the output image fmt.
LOCAL int rvdec_setoutputfmt(VCODEC_ADP_HANDLE *p_decoder,VCODEC_ADP_IMG_FMT_T *p_img_fmt)
{
    return VCODEC_ADP_RET_SUCCESS;
}

// decder the input stream,packet/frame data will send to output port by callback func.
LOCAL int rvdec_decstrm( VCODEC_ADP_HANDLE *p_decoder,VCODEC_ADP_STRM_T *p_strm)
{
    RVDEC_CONTAINER_T *p_rvdec_ctx = PNULL;
    VCODEC_ADP_DATA_PACKET_LIST_T *p_strm_list = PNULL;
    VCODEC_ADP_DATA_PACKET_T *p_packet;
    VCODEC_ADP_COMP_FRM_T *p_frm = PNULL;
    int adp_ret = VCODEC_ADP_RET_SUCCESS;
    //
    MMDecInput t_decinput = {0};
    MMDecOutput t_decoutput = {0};
    MMDecRet decode_return = {0};

    RVDEC_PRT_ERR("rvdec_decstrm,p_decoder=0x%x,p_strm=0x%x",p_decoder,p_strm);
    // check the parameter.
    if( (PNULL == p_decoder) || (PNULL == p_strm))
    {
        return VCODEC_ADP_RET_PARAM_ERR;
    }
    p_rvdec_ctx = (RVDEC_CONTAINER_T *)p_decoder;
    if(p_rvdec_ctx->dec_handle != p_decoder)
    {
        return VCODEC_ADP_RET_PARAM_ERR;
    }
    p_strm_list = &(p_strm->packet_list);
    p_packet = p_strm_list->p_first;

    // decode the input stream and output one frame if it's ready.
    if( (p_strm_list->list_len > 0 ) && (PNULL != p_packet) )
    {
        uint32 idx = 0;
        uint32 total_data_len = 0;
        uint8 *p_strm_buf_head = p_packet->p_data;
        
        /*set decoder paramaters*/
        for( idx = 0; 
             (idx < MAX_SLICE_NUM) && (idx < p_strm_list->list_len) && (PNULL != p_packet);
             idx++
           )
        {
            t_decinput.slice_valid[idx] = p_packet->flag;
            t_decinput.slice_offset[idx] = total_data_len;
            //
            total_data_len += p_packet->len;
            p_packet = p_packet->p_next;
        }
        // NOTE: all the packet data in the strm list should be in one continuous buffer.
        t_decinput.pStream  = p_strm_buf_head;
        t_decinput.dataLen = total_data_len;
        t_decinput.total_slice_num = p_strm_list->list_len;
        
        // other control flag.
        t_decinput.expected_IVOP = p_rvdec_ctx->b_expected_IVOP;
        t_decinput.beDisplayed   = !(p_rvdec_ctx->b_disp_skip);
        t_decinput.need_downsize = p_rvdec_ctx->b_need_downsize;

        /*decoding one VOP*/
        decode_return = RvDecDecode(&t_decinput,&t_decoutput);
        RVDEC_PRT_ERR("rvdec_decstrm,ret=%d",decode_return);

        // free the invalid dec out frame buffer.
        if( (t_decoutput.frameEffective > 0) &&
            ( (MMDEC_OK != decode_return) || (p_rvdec_ctx->b_disp_skip > 0) )
          )
        {
            RV_DecReleaseDispBfr(t_decoutput.pOutFrameY);
        }

        switch(decode_return)
        {
            case MMDEC_OK:
                // decoder one usable fram, stop skipping.
                p_rvdec_ctx->b_expected_IVOP = 0;
                
                // fill the output image frame.
                if( (t_decoutput.frameEffective > 0) && (p_rvdec_ctx->b_disp_skip == 0) )
                {
                    // alloc one frame container.
                    p_frm = _rvdec_frmctx_alloc( &(p_rvdec_ctx->img_frm_grp) );
                    if(PNULL == p_frm)
                    {
                        // no empty frame contaer.
                        RV_DecReleaseDispBfr(t_decoutput.pOutFrameY);
                        adp_ret = VCODEC_ADP_RET_SUCCESS;
                        break;
                    }

                    //
                    adp_ret = VCODEC_ADP_RET_FRM_RDY;
                    p_frm->img_fmt.data_type = VCODEC_ADP_DATATYP_YUV420_3PLANE;
                    if(t_decoutput.is_downsized)
                    {
                        p_frm->img_fmt.data_w = t_decoutput.frame_width + 16;
                        p_frm->img_fmt.data_h = t_decoutput.frame_height + 16;
                        p_frm->img_fmt.trim_x =  16;
                        p_frm->img_fmt.trim_y =  16;
                    }
                    else
                    {
                        p_frm->img_fmt.data_w = t_decoutput.frame_width + 32;
                        p_frm->img_fmt.data_h = t_decoutput.frame_height + 32;
                        p_frm->img_fmt.trim_x =  32;
                        p_frm->img_fmt.trim_y =  32;
                    }
                    p_frm->img_fmt.trim_w = t_decoutput.frame_width;
                    p_frm->img_fmt.trim_h = t_decoutput.frame_height;

                    // data buffer.
                    p_frm->img_data.yuv3plane.p_y = t_decoutput.pOutFrameY;
                    p_frm->img_data.yuv3plane.p_u = t_decoutput.pOutFrameU;
                    p_frm->img_data.yuv3plane.p_v = t_decoutput.pOutFrameV;

_RMVB_DEC_DIS_INT_;
                    // save the new frame.
                    p_rvdec_ctx->p_frm_cur = p_frm;
_RMVB_DEC_EN_INT_;

                }
                break;

            case MMDEC_OUTPUT_BUFFER_OVERFLOW:
                adp_ret = VCODEC_ADP_RET_BUSY;
                break;

            case MMDEC_FRAME_SEEK_IVOP:
                adp_ret = VCODEC_ADP_RET_SUCCESS;
                break;
            
            case MMDEC_ERROR:
            case MMDEC_STREAM_ERROR:
            case MMDEC_HW_ERROR:
                adp_ret = VCODEC_ADP_RET_STRM_ERR;
                
                // force to seek one Key frame.
                p_rvdec_ctx->b_expected_IVOP = 1;
                break;

            case MMDEC_MEMORY_ERROR:
            case MMDEC_NOT_SUPPORTED:
                adp_ret = VCODEC_ADP_RET_UNSUPPORT;
                p_rvdec_ctx->err_id = VCODEC_ADP_RET_UNSUPPORT;
                break;

            default:
                adp_ret = VCODEC_ADP_RET_UNKNOW_ERR;
                break;
        } // end of switch.
    }

    return adp_ret;
}


LOCAL int rvdec_getfrm(VCODEC_ADP_HANDLE *p_decoder, VCODEC_ADP_COMP_FRM_T **pp_frm)
{
    RVDEC_CONTAINER_T *p_rvdec_ctx = PNULL;

    RVDEC_PRT_ERR("rvdec_getfrm,p_decoder=0x%x,pp_frm=0x%x",p_decoder,pp_frm);
    // check the parameter.
    if( (PNULL == p_decoder) || (PNULL == pp_frm))
    {
        return VCODEC_ADP_RET_PARAM_ERR;
    }
    p_rvdec_ctx = (RVDEC_CONTAINER_T *)p_decoder;
    if(p_rvdec_ctx->dec_handle != p_decoder)
    {
        return VCODEC_ADP_RET_PARAM_ERR;
    }

    // get one image frame from the output frame list.
    *pp_frm = PNULL;
_RMVB_DEC_DIS_INT_;
    if(PNULL != p_rvdec_ctx->p_frm_cur)
    {
        *pp_frm = p_rvdec_ctx->p_frm_cur;
    }
_RMVB_DEC_EN_INT_;

    return VCODEC_ADP_RET_SUCCESS;
}


LOCAL int rvdec_freefrm(VCODEC_ADP_HANDLE *p_decoder, VCODEC_ADP_COMP_FRM_T *p_frm)
{
    RVDEC_CONTAINER_T *p_rvdec_ctx = PNULL;

    RVDEC_PRT_ERR("rvdec_freefrm,p_decoder=0x%x,p_frm=0x%x",p_decoder,p_frm);
    // check the parameter.
    if( (PNULL == p_decoder) || (PNULL == p_frm))
    {
        return VCODEC_ADP_RET_PARAM_ERR;
    }
    p_rvdec_ctx = (RVDEC_CONTAINER_T *)p_decoder;
    if(p_rvdec_ctx->dec_handle != p_decoder)
    {
        return VCODEC_ADP_RET_PARAM_ERR;
    }

    // free the image frame buffer.
    RV_DecReleaseDispBfr(p_frm->img_data.yuv3plane.p_y); 

    // free the image frame container.
    _rvdec_frmctx_free(&(p_rvdec_ctx->img_frm_grp),p_frm);

    return VCODEC_ADP_RET_SUCCESS;
}

// IOControl for externtion.
LOCAL int rvdec_IoControl(VCODEC_ADP_HANDLE *p_decoder, uint32 cmd, void *p_param)
{
    RVDEC_CONTAINER_T *p_rvdec_ctx = PNULL;

    RVDEC_PRT_ERR("rvdec_IoControl,p_decoder=0x%x,cmd=0x%x,param=0x%x",p_decoder,cmd,p_param);
    // check the parameter.
    if( PNULL == p_decoder)
    {
        return VCODEC_ADP_RET_PARAM_ERR;
    }
    p_rvdec_ctx = (RVDEC_CONTAINER_T *)p_decoder;
    if(p_rvdec_ctx->dec_handle != p_decoder)
    {
        return VCODEC_ADP_RET_PARAM_ERR;
    }
    
    switch(cmd)
    {
        case VCODEC_ADP_CMD_SEEK_KEY_FRM:
            p_rvdec_ctx->b_expected_IVOP = ( (uint32)p_param > 0 )? 1 : 0;
            break;
            
        case VCODEC_ADP_CMD_DISP_SKIP:
            p_rvdec_ctx->b_disp_skip = ( (uint32)p_param > 0 )? 1 : 0;
            break;
            
        case VCODEC_ADP_CMD_SCALE_DOWN:
            p_rvdec_ctx->b_need_downsize = ( (uint32)p_param > 0 )? 1 : 0;
            break;

        default:
            break;
    }

    return VCODEC_ADP_RET_SUCCESS;
}


// Global entry, registed in user decoder table.
PUBLIC VCODEC_ADP_DEC_ENTRY_T g_rmvb_dec_entry = 
{
    rvdec_open,
    rvdec_close,
    //
    rvdec_setfmt,
    rvdec_getfmt,
    //
    rvdec_getfrminfo,
    rvdec_setoutputfmt,
    //
    rvdec_decstrm,
    rvdec_getfrm,
    rvdec_freefrm,
    //
    rvdec_IoControl
};




