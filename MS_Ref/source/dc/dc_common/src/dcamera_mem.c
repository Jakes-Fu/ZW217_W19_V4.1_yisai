/*******************************************************************************
 ** FileName:      dcamera_mem.c			                                   *
 ** Author:        Baggiohe                                                    *
 ** Date:          2011/1/21                                                   *
 ** CopyRight:     2011, Spreatrum, Incoporated, All right reserved            *
 ** Description:   define DCAM application                                     *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2011/1/21      Baggiohe      Created                          *
 *******************************************************************************/

#include "ms_ref_dc_trc.h"
#include "dcamera_mem.h"
#include "dal_dcamera.h"
#include "dcamera_common.h"
#include "dc_cfg.h"

#define DC_JPG_SIZE(w,h)            (uint32)((w)*(h)/DC_JPEG_COMPRESS_LEVEL)
#define DC_JPG_420_SIZE(w,h)        (uint32)((w)*(h)/(DC_JPEG_COMPRESS_LEVEL+2))

#if defined(PLATFORM_SC8800G)
#define DC_SCALE_LINE_BUF_SIZE(w)   (((w + 640 - 1) / 640) * 640 * 2 * 4 + DCAMERA_PADDING_MEM_SIZE)
#else
#define DC_SCALE_LINE_BUF_SIZE(w)   (w * 8 + DCAMERA_PADDING_MEM_SIZE)
#endif
#define DC_SLICE_OUT_ESTIMATE(n)    ((n) + 6)
//(((n)+DC_SLICE_HEIGHT-1)/DC_SLICE_HEIGHT*DC_SLICE_HEIGHT+4)
#define DC_MEM_TRY_TIMES            10
#define DC_MEM_ADDR_HIGH            0xFC000000
#define DC_MEM_IS_IN_SAME_SEGE(x,y) ((((uint32)(x))&DC_MEM_ADDR_HIGH) == (((uint32)(y))&DC_MEM_ADDR_HIGH))

typedef struct _dcaemra_mem_context
{
    uint32       dc_mem_base;
    uint32       dc_mem_start;
    uint32       dc_mem_end;
    uint32       dc_mem_size;
    uint32       dc_mem_preview;
    uint32       dc_mem_prv_len;
    uint32       dc_mem_cap_yuv;
    uint32       dc_mem_cap_yuv_len;
    uint32       dc_mem_cap_yuv_rot;
    uint32       dc_mem_cap_yuv_rot_len;
    uint32       dc_mem_cap_review;
    uint32       dc_mem_cap_rev_len;
    uint32       dc_mem_cap_jpg;
    uint32       dc_mem_cap_jpg_len;
    uint32       dc_mem_cap_jpg_zoom;
    uint32       dc_mem_cap_jpg_zoom_len;
    uint32       dc_mem_cap_jpg_tmp;
    uint32       dc_mem_cap_jpg_tmp_len;
    uint32       dc_mem_cap_line_buf;
    uint32       dc_mem_cap_line_buf_len;
    uint32       dc_mem_cap_swap_buf_y;
    uint32       dc_mem_cap_swap_buf_y_len;
    uint32       dc_mem_cap_swap_buf_uv;
    uint32       dc_mem_cap_swap_buf_uv_len;
    uint32       dc_mem_cap_src_slice_buf_y;
    uint32       dc_mem_cap_src_slice_buf_y_len;
    uint32       dc_mem_cap_src_slice_buf_uv;
    uint32       dc_mem_cap_src_slice_buf_uv_len;
    uint32       dc_mem_cap_slice_buf_y;
    uint32       dc_mem_cap_slice_buf_y_len;
    uint32       dc_mem_cap_slice_buf_uv;
    uint32       dc_mem_cap_slice_buf_uv_len;
    uint32       dc_mem_cap_thum_buf_y;
    uint32       dc_mem_cap_thum_buf_y_len;
    uint32       dc_mem_cap_thum_buf_uv;
    uint32       dc_mem_cap_thum_buf_uv_len;
    uint32       dc_mem_cap_burst_num;
    uint32       dc_mem_base_y;
    uint32       dc_mem_cap_osd;
    uint32       dc_mem_cap_osd_len;
    uint32       dc_mem_cap_y;
    uint32       dc_mem_cap_y_len;
    uint32       dc_mem_cap_uv;
    uint32       dc_mem_cap_uv_len;
}DCAMERA_MEM_CXT_T;


LOCAL DCAMERA_MEM_CXT_T dc_mem_cxt = {0};

PUBLIC uint32 DCamera_MemConfig_CaptureYBuf(uint16 sensor_out_w, uint16 sensor_out_h)
{
    if(SCI_NULL == dc_mem_cxt.dc_mem_base_y)
    {
        uint32 mem_len = sensor_out_w * sensor_out_h + DCAMERA_PADDING_MEM_SIZE;

        dc_mem_cxt.dc_mem_base_y = (uint32)SCI_ALLOCA(mem_len);

        if(dc_mem_cxt.dc_mem_base_y)
        {
            dc_mem_cxt.dc_mem_cap_y = DC_ADDR(dc_mem_cxt.dc_mem_base_y);
            dc_mem_cxt.dc_mem_cap_y_len = dc_mem_cxt.dc_mem_base_y + mem_len - dc_mem_cxt.dc_mem_cap_y;
        }
        else
        {
            dc_mem_cxt.dc_mem_cap_y = 0;
            dc_mem_cxt.dc_mem_cap_y_len = 0;
            SCI_TRACE_LOW("DC: 2M Camera No Memory");
        }
    }

    return dc_mem_cxt.dc_mem_cap_y;
}

PUBLIC void DCamera_MemFree_Ybuffer(void)
{
    if(SCI_NULL != dc_mem_cxt.dc_mem_base_y)
    {
        SCI_Free((void*)dc_mem_cxt.dc_mem_base_y);
        dc_mem_cxt.dc_mem_base_y = 0;
        dc_mem_cxt.dc_mem_cap_y = 0;
    }
}
#if 0
PUBLIC uint32 DCamera_MemConfig(uint32 op_mode, uint32 *p_mem_size)
{
    DC_PRODUCT_CFG_T_PTR pDCProductCfg = DC_GeProductCfgPtr();
    uint32               i = 0, j = 0;
    uint32	             end_addr;
    void                 *p_mem_to_be_freed[DC_MEM_TRY_TIMES] = {0};

    switch((DCAMERA_OP_MODE_E)op_mode)
    {
        case DCAMERA_OP_NORMAL:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->dc_mode_mem;
            break;

        case DCAMERA_OP_DV:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->dv_mode_mem;
            break;

        case DCAMERA_OP_VT:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->vt_mode_mem;
            break;

        case DCAMERA_OP_ATV:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->atv_mode_mem;
            break;

        case DCAMERA_OP_SPECIAL:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->sp_mode_mem;
            break;

        case DCAMERA_OP_MINIDC:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->minidc_mode_mem;
            break;

        case DCAMERA_OP_MINIDV:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->minidv_mode_mem;
            break;

        case DCAMERA_OP_UPCC:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->upcc_mode_mem;
            break;

        default:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->minidc_mode_mem;
            break;
    }

    if(dc_mem_cxt.dc_mem_size != 0)
    {
        for(i = 0; i < DC_MEM_TRY_TIMES; i++)
        {
            p_mem_to_be_freed[i] = SCI_ALLOCA(dc_mem_cxt.dc_mem_size);
            if(SCI_NULL == p_mem_to_be_freed[i])
            {
                //SCI_TRACE_LOW:"DCamera_MemConfig, NO memory reserved for DC"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_MEM_118_112_2_18_1_24_29_566,(uint8*)"");
                break;
            }

            end_addr = (uint32)p_mem_to_be_freed[i] + dc_mem_cxt.dc_mem_size;
            if(DC_MEM_IS_IN_SAME_SEGE(p_mem_to_be_freed[i], end_addr))
            {
                dc_mem_cxt.dc_mem_base = (uint32)p_mem_to_be_freed[i];
                break;
            }
            //SCI_TRACE_LOW:"DCamera_MemConfig, Memory not in one 64M Segement,try again"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_MEM_127_112_2_18_1_24_29_567,(uint8*)"");
        }

        for(j = 0; j < i; j++)
        {
            if(SCI_NULL != p_mem_to_be_freed[j])
            {
                SCI_Free(p_mem_to_be_freed[j]);
            }
            //SCI_TRACE_LOW:"DCamera_MemConfig, Number %d block mem freed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_MEM_136_112_2_18_1_24_29_568,(uint8*)"d", j);
        }

        if(i != DC_MEM_TRY_TIMES && SCI_NULL != dc_mem_cxt.dc_mem_base)
        {
            //SCI_TRACE_LOW:"DCamera_MemConfig, Memory successfully,total try times %d ,addr 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_MEM_142_112_2_18_1_24_29_569,(uint8*)"dd",i,dc_mem_cxt.dc_mem_base);
        }
        else
        {
            //SCI_TRACE_LOW:"DCamera_MemConfig, No memory reserved for DC"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_MEM_146_112_2_18_1_24_29_570,(uint8*)"");
            dc_mem_cxt.dc_mem_size = 0;
            return 0;
        }

    }
    dc_mem_cxt.dc_mem_end = dc_mem_cxt.dc_mem_base + dc_mem_cxt.dc_mem_size;
    dc_mem_cxt.dc_mem_start = DC_ADDR(dc_mem_cxt.dc_mem_base);
    dc_mem_cxt.dc_mem_size = dc_mem_cxt.dc_mem_end - dc_mem_cxt.dc_mem_start;

    if(DCAMERA_OP_DV == op_mode ||
       DCAMERA_OP_ATV == op_mode ||
       DCAMERA_OP_MINIDV == op_mode||
       DCAMERA_OP_UPCC == op_mode)
    {
        if(dc_mem_cxt.dc_mem_size <= DC_JPEG_TMP_BUF_LEN)
        {
            return 0;
        }
        dc_mem_cxt.dc_mem_cap_jpg_tmp_len = DC_JPEG_TMP_BUF_LEN;//(uint32)(64*1024); //DC_JPEG_TMP_BUF_LEN;
        dc_mem_cxt.dc_mem_end -= dc_mem_cxt.dc_mem_cap_jpg_tmp_len;
        dc_mem_cxt.dc_mem_size -= dc_mem_cxt.dc_mem_cap_jpg_tmp_len;
        dc_mem_cxt.dc_mem_cap_jpg_tmp = dc_mem_cxt.dc_mem_end;

		//uix8910
		dc_mem_cxt.dc_mem_end  += dc_mem_cxt.dc_mem_cap_jpg_tmp_len;
		dc_mem_cxt.dc_mem_size += dc_mem_cxt.dc_mem_cap_jpg_tmp_len;
    }

    //SCI_TRACE_LOW:"DCamera_MemConfig, Memory alloc successfully base 0x%x,start 0x%x,size 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_MEM_172_112_2_18_1_24_29_571,(uint8*)"ddd", dc_mem_cxt.dc_mem_base, dc_mem_cxt.dc_mem_start,dc_mem_cxt.dc_mem_size);

    if(SCI_NULL != p_mem_size)
    {
        *p_mem_size = dc_mem_cxt.dc_mem_size;
    }
    return dc_mem_cxt.dc_mem_start;
}
#endif
PUBLIC uint32 DCamera_MemConfig(uint32 op_mode, uint32 *p_mem_size)
{
    DC_PRODUCT_CFG_T_PTR pDCProductCfg = DC_GeProductCfgPtr();
    uint32               i = 0, j = 0;
    uint32	             end_addr;
    void                 *p_mem_to_be_freed = 0;

    switch((DCAMERA_OP_MODE_E)op_mode)
    {
        case DCAMERA_OP_NORMAL:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->dc_mode_mem;
            break;

        case DCAMERA_OP_DV:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->dv_mode_mem;
            break;

        case DCAMERA_OP_VT:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->vt_mode_mem;
            break;

        case DCAMERA_OP_ATV:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->atv_mode_mem;
            break;

        case DCAMERA_OP_SPECIAL:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->sp_mode_mem;
            break;

        case DCAMERA_OP_MINIDC:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->minidc_mode_mem;
            break;

        case DCAMERA_OP_MINIDV:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->minidv_mode_mem;
            break;

        case DCAMERA_OP_UPCC:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->upcc_mode_mem;
            break;

        case DCAMERA_OP_VIDEOCALL:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->vt_mode_mem;
            break;

        default:
            dc_mem_cxt.dc_mem_size = pDCProductCfg->minidc_mode_mem;
            break;
    }

    SCI_TRACE_LOW("DC:MemConfig size=%d", dc_mem_cxt.dc_mem_size);

    if(dc_mem_cxt.dc_mem_size != 0)
    {
        p_mem_to_be_freed = SCI_ALLOCA(dc_mem_cxt.dc_mem_size);
        if(SCI_NULL == p_mem_to_be_freed)
        {
            //SCI_TRACE_LOW:"DCamera_MemConfig, NO memory reserved for DC"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_MEM_118_112_2_18_1_24_29_566,(uint8*)"");
        }
        else
		{
			dc_mem_cxt.dc_mem_base = (uint32)p_mem_to_be_freed;
		}
    }

    if(SCI_NULL != dc_mem_cxt.dc_mem_base)
    {
            //SCI_TRACE_LOW:"DCamera_MemConfig, Memory successfully,total try times %d ,addr 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_MEM_142_112_2_18_1_24_29_569,(uint8*)"dd",i,dc_mem_cxt.dc_mem_base);
    }
    else
    {
            //SCI_TRACE_LOW:"DCamera_MemConfig, No memory reserved for DC"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_MEM_146_112_2_18_1_24_29_570,(uint8*)"");
        dc_mem_cxt.dc_mem_size = 0;
        return 0;
    }

    dc_mem_cxt.dc_mem_end = dc_mem_cxt.dc_mem_base + dc_mem_cxt.dc_mem_size;
    dc_mem_cxt.dc_mem_start = DC_ADDR(dc_mem_cxt.dc_mem_base);
    dc_mem_cxt.dc_mem_size = dc_mem_cxt.dc_mem_end - dc_mem_cxt.dc_mem_start;

    if(DCAMERA_OP_DV == op_mode ||
       DCAMERA_OP_ATV == op_mode ||
       DCAMERA_OP_MINIDV == op_mode||
       DCAMERA_OP_UPCC == op_mode)
    {
        if(dc_mem_cxt.dc_mem_size <= DC_JPEG_TMP_BUF_LEN)
        {
            return 0;
        }
        dc_mem_cxt.dc_mem_cap_jpg_tmp_len = DC_JPEG_TMP_BUF_LEN;//(uint32)(64*1024); //DC_JPEG_TMP_BUF_LEN;
        dc_mem_cxt.dc_mem_end -= dc_mem_cxt.dc_mem_cap_jpg_tmp_len;
        dc_mem_cxt.dc_mem_size -= dc_mem_cxt.dc_mem_cap_jpg_tmp_len;
        dc_mem_cxt.dc_mem_cap_jpg_tmp = dc_mem_cxt.dc_mem_end;
    }

    //SCI_TRACE_LOW:"DCamera_MemConfig, Memory alloc successfully base 0x%x,start 0x%x,size 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_MEM_172_112_2_18_1_24_29_571,(uint8*)"ddd", dc_mem_cxt.dc_mem_base, dc_mem_cxt.dc_mem_start,dc_mem_cxt.dc_mem_size);

    if(SCI_NULL != p_mem_size)
    {
        *p_mem_size = dc_mem_cxt.dc_mem_size;
    }
    return dc_mem_cxt.dc_mem_start;
}

PUBLIC void   DCamera_MemFree(void)
{
    if(0 != dc_mem_cxt.dc_mem_base)
    {
        SCI_Free((void*)dc_mem_cxt.dc_mem_base);
        dc_mem_cxt.dc_mem_base = 0;
        SCI_MEMSET((void*)&dc_mem_cxt, 0, sizeof(DCAMERA_MEM_CXT_T)); //lint !e718
        //SCI_TRACE_LOW:"DCamera_MemFree OK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_MEM_188_112_2_18_1_24_29_572,(uint8*)"");
    }
}


PUBLIC void*  DCAMERA_PreviewGetMem(DCAMERA_MEM_PARAM_T* p_cap_mem_cfg, uint32 *p_buf_len)
{
    uint32  pre_frm_cnt = 2;
    uint32  cap_frm_count = 0;
    uint32  frm_size = 0;
    uint32  mem_start = dc_mem_cxt.dc_mem_start;
    uint32  mem_end = dc_mem_cxt.dc_mem_end;
    uint32  mem_len = dc_mem_cxt.dc_mem_size;


    if(SCI_NULL == p_cap_mem_cfg || 0 == dc_mem_cxt.dc_mem_start)
    {
        return SCI_NULL;
    }

    frm_size = (p_cap_mem_cfg->review_width * p_cap_mem_cfg->review_height);
    frm_size += DCAMERA_PADDING_MEM_SIZE;
    frm_size <<= 1;
    pre_frm_cnt = mem_len / frm_size;
    if(pre_frm_cnt > DC_DISPLAY_ROUND_BUFFER)
    {
        pre_frm_cnt = DC_DISPLAY_ROUND_BUFFER;
    }

    if(pre_frm_cnt > 1)
    {
        pre_frm_cnt -= 1;
    }

    if(DCAMERA_ROTATION_0 != p_cap_mem_cfg->review_rot)
    {
       pre_frm_cnt += 2;
    }

    dc_mem_cxt.dc_mem_prv_len = (pre_frm_cnt * frm_size)<mem_len?(pre_frm_cnt * frm_size):mem_len;
    dc_mem_cxt.dc_mem_preview = dc_mem_cxt.dc_mem_end - dc_mem_cxt.dc_mem_prv_len;
    mem_end -= dc_mem_cxt.dc_mem_prv_len;

    if(p_cap_mem_cfg->b_personal_cap) // if in personal capture , it will preview for a few frames, then start capture
    {
        //mem_end -= (frm_size << 1); //for LCDC capture and scaling
        dc_mem_cxt.dc_mem_cap_jpg_tmp = dc_mem_cxt.dc_mem_start;
        dc_mem_cxt.dc_mem_cap_jpg_tmp_len = DC_JPEG_TMP_BUF_LEN;
        mem_start += dc_mem_cxt.dc_mem_cap_jpg_tmp_len;
        if(mem_start > mem_end)
        {
            SCI_TraceLow("DC:not enough memory!");
            return 0;
        }
        else
        {
            mem_len = mem_end - mem_start;
        }

        cap_frm_count = mem_len / frm_size; // the reserved memory is too few to save one jpg file;
        p_cap_mem_cfg->burst_number = cap_frm_count;

        dc_mem_cxt.dc_mem_cap_jpg = mem_start;
        dc_mem_cxt.dc_mem_cap_jpg_len = mem_len;
    }
/*
    if(p_cap_mem_cfg->sensor_out_width &&
       p_cap_mem_cfg->sensor_out_height)
    {
        frm_size = (p_cap_mem_cfg->sensor_out_width * p_cap_mem_cfg->sensor_out_height * 2);
        frm_size += DCAMERA_PADDING_MEM_SIZE;
        frm_size = (uint32)(frm_size * (DC_ENCODER_ROUND_BUFFER + 1));
        mem_end -= frm_size;
        dc_mem_cxt.dc_mem_prv_len += frm_size;
        dc_mem_cxt.dc_mem_preview = mem_end;
    }
*/
    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_prv_len;
    }
    //SCI_TRACE_LOW:"DCAMERA_PreviewGetMem, start 0x%x, size 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_MEM_269_112_2_18_1_24_29_573,(uint8*)"dd",dc_mem_cxt.dc_mem_preview,dc_mem_cxt.dc_mem_prv_len);

    return (void*)dc_mem_cxt.dc_mem_preview;
}

PUBLIC uint32 DCAMERA_CaptureMemReq(DCAMERA_MEM_PARAM_T* p_cap_mem_cfg)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_start;
    uint32  mem_end = dc_mem_cxt.dc_mem_end;
    uint32  mem_len = dc_mem_cxt.dc_mem_size;
    uint32  frm_size = 0;
    uint32  frm_count = 0;
    uint32  out_frm_count = 0;
    uint32  slice_height_out = 0;
    uint32  rot_src_start = 0;
    uint32  rot_src_size = 0;
    uint32  img_fmt = 0;
    uint32  jpg_exif = 0;
    uint32  cap_y_len = 0;
    uint32  sensor_out_w = 0;
    uint32  sensor_out_h = 0;
    uint16  img_w = 0;
    uint32  review_buf_size = 0;

    if(SCI_NULL == p_cap_mem_cfg || 0 == dc_mem_cxt.dc_mem_start)
    {
        return 0;
    }

    if(DCAMERA_ROTATION_90 == p_cap_mem_cfg->capture_rot ||
       DCAMERA_ROTATION_270 == p_cap_mem_cfg->capture_rot)
    {
        sensor_out_w = p_cap_mem_cfg->sensor_out_height;
        sensor_out_h = p_cap_mem_cfg->sensor_out_width;
    }
    else
    {
        sensor_out_w = p_cap_mem_cfg->sensor_out_width;
        sensor_out_h = p_cap_mem_cfg->sensor_out_height;
    }

    rot_src_start = dc_mem_cxt.dc_mem_start;

    for(img_fmt = ISP_IMAGE_FORMAT_YUV422; img_fmt < ISP_IMAGE_FORMAT_YUV400; img_fmt++)
    {
        mem_end = dc_mem_cxt.dc_mem_end;
        mem_start = dc_mem_cxt.dc_mem_start;
        frm_count = 1;
        if(DCAMERA_ROTATION_0 != p_cap_mem_cfg->review_rot)
        {
            frm_count += 1;
        }
        if(ISP_IMAGE_FORMAT_YUV422 == img_fmt)
        {
            frm_size = (uint32)(p_cap_mem_cfg->review_width * p_cap_mem_cfg->review_height * 2);
        }
        else
        {
            frm_size = (uint32)(p_cap_mem_cfg->review_width * p_cap_mem_cfg->review_height * 3) >> 1;
        }
        frm_size += DCAMERA_PADDING_MEM_SIZE * 2;
        review_buf_size = (uint32)(p_cap_mem_cfg->review_width * p_cap_mem_cfg->review_height * 2) + DCAMERA_PADDING_MEM_SIZE * 2;

        if(DCAMERA_IsSupport2MSnapshot() &&
           (p_cap_mem_cfg->sensor_out_width > VGA_WIDTH) &&
           (p_cap_mem_cfg->image_width > VGA_WIDTH))
        {
            /*get review buffer*/
            dc_mem_cxt.dc_mem_cap_review = dc_mem_cxt.dc_mem_start;
            dc_mem_cxt.dc_mem_cap_rev_len = (frm_count * frm_size);
            mem_start += dc_mem_cxt.dc_mem_cap_rev_len;
            if(mem_end < mem_start)
            {
                continue;
            }

            /*get  osd   buffer*/
            dc_mem_cxt.dc_mem_cap_osd = mem_start;
            dc_mem_cxt.dc_mem_cap_osd_len = (uint32)(p_cap_mem_cfg->review_width * p_cap_mem_cfg->review_height * 2) + 64;
            mem_start += dc_mem_cxt.dc_mem_cap_osd_len;
            if(mem_end < mem_start)
            {
                continue;
            }

            /*get UV buffer*/
            cap_y_len = (uint32)(sensor_out_w * sensor_out_h);
            if(ISP_IMAGE_FORMAT_YUV422 == img_fmt)
            {
                frm_size = cap_y_len;
            }
            else
            {
                cap_y_len = cap_y_len >> 1;
                frm_size = cap_y_len;
            }
            frm_size += (uint32)(DCAMERA_PADDING_MEM_SIZE);
            mem_end -= frm_size;
            if(mem_end < mem_start)
            {
                continue;
            }
            dc_mem_cxt.dc_mem_cap_uv = mem_end;
            dc_mem_cxt.dc_mem_cap_uv_len = frm_size;
        }
        else
        {
            /*get review buffer*/
            dc_mem_cxt.dc_mem_cap_rev_len = (frm_count * frm_size);
            mem_end -= dc_mem_cxt.dc_mem_cap_rev_len;
            if(mem_end < mem_start)
            {
                continue;
            }
            dc_mem_cxt.dc_mem_cap_review = mem_end;

            /*get YUV buffer*/
            cap_y_len = (uint32)(sensor_out_w * sensor_out_h);
            if(ISP_IMAGE_FORMAT_YUV422 == img_fmt)
            {
                frm_size = (uint32)(cap_y_len * 2);
            }
            else
            {
                frm_size = (uint32)(cap_y_len * 3) >> 1;
            }
            frm_size += (DC_JPEG_YUV_PADDING_BYTES + (uint32)(DCAMERA_PADDING_MEM_SIZE * 2));
            frm_size = frm_size < review_buf_size ? review_buf_size : frm_size;
            mem_end -= frm_size;
            if(mem_end < mem_start)
            {
                continue;
            }
            dc_mem_cxt.dc_mem_cap_yuv = mem_end;
            dc_mem_cxt.dc_mem_cap_yuv_len = frm_size;
        }

        if((p_cap_mem_cfg->is_jpg_sensor == 0) && //YUV sensor , need interpolation
           (sensor_out_w != p_cap_mem_cfg->image_width ||
            sensor_out_h != p_cap_mem_cfg->image_height)) //must do slice scaling after capture
        {
            /*get line buffer*/
            frm_size = DC_SCALE_LINE_BUF_SIZE(p_cap_mem_cfg->image_width); // line buffer size
            dc_mem_cxt.dc_mem_cap_line_buf = mem_start;
            mem_start += frm_size;
            dc_mem_cxt.dc_mem_cap_line_buf_len = frm_size;

            /*get swap buffer y*/
            frm_size = p_cap_mem_cfg->slice_height * p_cap_mem_cfg->image_width; // swap buffer size
            frm_size += DCAMERA_PADDING_MEM_SIZE;
            dc_mem_cxt.dc_mem_cap_swap_buf_y = mem_start;
            mem_start += frm_size;
            dc_mem_cxt.dc_mem_cap_swap_buf_y_len = frm_size;
            /*get swap buffer uv*/
            dc_mem_cxt.dc_mem_cap_swap_buf_uv = mem_start;
            mem_start += frm_size;
            dc_mem_cxt.dc_mem_cap_swap_buf_uv_len = frm_size;

#if defined(PLATFORM_SC8800G)    // if the chip version is older than 8800G,the frame address need 256 bytes aligned

            /*get src slice buffer,used in slice encode if the offset of src image buffer is not aligned by 256 bytes*/
            frm_size = p_cap_mem_cfg->slice_height * sensor_out_w;
            frm_size += DCAMERA_PADDING_MEM_SIZE;
            dc_mem_cxt.dc_mem_cap_src_slice_buf_y = mem_start;
            mem_start += frm_size;
            dc_mem_cxt.dc_mem_cap_src_slice_buf_y_len = frm_size;
            /*get src slice buffer uv*/
            dc_mem_cxt.dc_mem_cap_src_slice_buf_uv = mem_start;
            mem_start += frm_size;
            dc_mem_cxt.dc_mem_cap_src_slice_buf_uv_len = frm_size;

#endif

            /*get slice buffer y*/
            slice_height_out =
              (uint32)((p_cap_mem_cfg->slice_height * p_cap_mem_cfg->image_height) / sensor_out_h) + 2;
            slice_height_out = DC_SLICE_OUT_ESTIMATE(slice_height_out);
            frm_size = slice_height_out * p_cap_mem_cfg->image_width; // slice out buffer size
            frm_size += DCAMERA_PADDING_MEM_SIZE;
            dc_mem_cxt.dc_mem_cap_slice_buf_y = mem_start;
            mem_start += frm_size;
            dc_mem_cxt.dc_mem_cap_slice_buf_y_len = frm_size;
            /*get slice buffer uv*/
            dc_mem_cxt.dc_mem_cap_slice_buf_uv = mem_start;
            mem_start += frm_size;
            dc_mem_cxt.dc_mem_cap_slice_buf_uv_len = frm_size;

            /*calculate jpeg tmp buffer size*/
            frm_size = 0;

            /* jpeg tmp buffer: Align Buffer */
            if((p_cap_mem_cfg->image_width % 16 ||
                p_cap_mem_cfg->image_height % 8) && (p_cap_mem_cfg->image_width >= 240))
            {
                img_w = ((p_cap_mem_cfg->image_width + 15)>>4)<<4;
                frm_size = (p_cap_mem_cfg->slice_height * img_w)<<1;
            }

            /* jpeg tmp buffer: Slice YUV Buffer*/
            img_w = ((p_cap_mem_cfg->image_width + 15)>>4)<<4;
            frm_size += (p_cap_mem_cfg->slice_height * img_w + 255)<<2;
            dc_mem_cxt.dc_mem_cap_jpg_tmp_len = frm_size;

            /* jpeg tmp buffer: Slice Stream Buffer */
            frm_size = (p_cap_mem_cfg->slice_height * img_w + 255)<<1;
            frm_size = frm_size < DC_JPEG_SLICE_BS_LEN ? DC_JPEG_SLICE_BS_LEN : frm_size;

            dc_mem_cxt.dc_mem_cap_jpg_tmp_len += frm_size;

            if(DC_JPEG_TMP_BUF_LEN > dc_mem_cxt.dc_mem_cap_jpg_tmp_len)
            {
                dc_mem_cxt.dc_mem_cap_jpg_tmp_len = DC_JPEG_TMP_BUF_LEN;
            }

            SCI_TRACE_LOW("DC:jpeg temp buffer lenth is %d", dc_mem_cxt.dc_mem_cap_jpg_tmp_len);

        }
        else
        {
            if(img_fmt == ISP_IMAGE_FORMAT_YUV422)
            {
                frm_size = DC_JPEG_TMP_BUF_LEN;
                dc_mem_cxt.dc_mem_cap_jpg_tmp_len = frm_size;
            }
            else
            {
                slice_height_out = DC_SLICE_HEIGHT;
                frm_size = slice_height_out * p_cap_mem_cfg->image_width; // slice out buffer size
                frm_size += DCAMERA_PADDING_MEM_SIZE;
                dc_mem_cxt.dc_mem_cap_slice_buf_y = mem_start;
                mem_start += frm_size;
                dc_mem_cxt.dc_mem_cap_slice_buf_y_len = frm_size;
                dc_mem_cxt.dc_mem_cap_slice_buf_uv = mem_start;
                mem_start += frm_size;
                dc_mem_cxt.dc_mem_cap_slice_buf_uv_len = frm_size;

                /*calculate jpeg tmp buffer size*/
                frm_size = 0;

                /* jpeg tmp buffer: Align Buffer */
                if((p_cap_mem_cfg->image_width % 16 ||
                    p_cap_mem_cfg->image_height % 8) && (p_cap_mem_cfg->image_width >= 240))
                {
                    img_w = ((p_cap_mem_cfg->image_width + 15)>>4)<<4;
                    frm_size = (p_cap_mem_cfg->slice_height * img_w)<<1;
                }

                /* jpeg tmp buffer: Slice YUV Buffer*/
                img_w = ((p_cap_mem_cfg->image_width + 15)>>4)<<4;
                frm_size += (p_cap_mem_cfg->slice_height * img_w + 255)<<2;
                dc_mem_cxt.dc_mem_cap_jpg_tmp_len = frm_size;

                /* jpeg tmp buffer: Slice Stream Buffer */
                frm_size = (p_cap_mem_cfg->slice_height * img_w + 255)<<1;
                frm_size = frm_size < DC_JPEG_SLICE_BS_LEN ? DC_JPEG_SLICE_BS_LEN : frm_size;

                dc_mem_cxt.dc_mem_cap_jpg_tmp_len += frm_size;

                if(DC_JPEG_TMP_BUF_LEN > dc_mem_cxt.dc_mem_cap_jpg_tmp_len)
                {
                    dc_mem_cxt.dc_mem_cap_jpg_tmp_len = DC_JPEG_TMP_BUF_LEN;
                }

            }

        }

        dc_mem_cxt.dc_mem_cap_jpg_tmp = mem_start;

        if(p_cap_mem_cfg->b_add_thumbnail &&
           p_cap_mem_cfg->image_width > DC_WIDTH_TO_ADD_THUM)
        {
            frm_size = (uint32)(DC_THUMBNAIL_WIDTH * DC_THUMBNAIL_HEIGHT) + DCAMERA_PADDING_MEM_SIZE;
            if(ISP_IMAGE_FORMAT_YUV422 == img_fmt)
            {
                frm_size = frm_size << 1;
            }
            else
            {
                frm_size = (uint32)(frm_size * 3) >> 1;
            }
            if(dc_mem_cxt.dc_mem_start + frm_size > dc_mem_cxt.dc_mem_cap_jpg_tmp)
            {
                dc_mem_cxt.dc_mem_cap_jpg_tmp = dc_mem_cxt.dc_mem_start + frm_size;
            }
            dc_mem_cxt.dc_mem_cap_thum_buf_y = dc_mem_cxt.dc_mem_start;
            dc_mem_cxt.dc_mem_cap_thum_buf_y_len = frm_size >> 1;
            dc_mem_cxt.dc_mem_cap_thum_buf_uv = dc_mem_cxt.dc_mem_cap_thum_buf_y + dc_mem_cxt.dc_mem_cap_thum_buf_y_len;
            dc_mem_cxt.dc_mem_cap_thum_buf_uv_len = frm_size - dc_mem_cxt.dc_mem_cap_thum_buf_y_len;

        }

        mem_start += dc_mem_cxt.dc_mem_cap_jpg_tmp_len;


        if((mem_start + DC_JPEG_YUV_MIN_INTERVAL) > mem_end)
        {
            continue;
        }

        if(DCAMERA_ROTATION_0 != p_cap_mem_cfg->capture_rot)
        {
            rot_src_size = mem_start - rot_src_start;
            if(rot_src_size < dc_mem_cxt.dc_mem_cap_yuv_len)
            {
                //it will use jpg buffer to save yuv data for rotation,so it won't support burst snapshot
                rot_src_size = mem_end - rot_src_start;
                if(rot_src_size < dc_mem_cxt.dc_mem_cap_yuv_len)
                {
                    //no memory to rotation ,
                    //SCI_TRACE_LOW:"DCAMERA_CaptureMemReq, not enough memory for rotation capture"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DCAMERA_MEM_461_112_2_18_1_24_30_574,(uint8*)"");
                    continue;
                }
                else
                {
                    mem_start = dc_mem_cxt.dc_mem_start + dc_mem_cxt.dc_mem_cap_yuv_len;

                }
            }
            dc_mem_cxt.dc_mem_cap_yuv_rot = rot_src_start;
            dc_mem_cxt.dc_mem_cap_yuv_rot_len = dc_mem_cxt.dc_mem_cap_yuv_len;
        }

        /*get jpg encode destination buffer*/
        if(ISP_IMAGE_FORMAT_YUV422 == img_fmt &&
           0 != dc_mem_cxt.dc_mem_cap_line_buf_len) //if Frame encode and the format should be YUV422
        {
            frm_size = DC_JPG_SIZE(p_cap_mem_cfg->image_width,p_cap_mem_cfg->image_height);
        }
        else
        {
            frm_size = DC_JPG_420_SIZE(p_cap_mem_cfg->image_width,p_cap_mem_cfg->image_height);
        }

        jpg_exif = 0;
        if(p_cap_mem_cfg->b_add_thumbnail &&
           p_cap_mem_cfg->image_width > DC_WIDTH_TO_ADD_THUM) //add thumbnail
        {
            frm_size += DC_THUMBNAIL_LENGTH;
            jpg_exif += DC_THUMBNAIL_LENGTH;
        }
        if(p_cap_mem_cfg->b_add_exif) //add exif
        {
            frm_size += DC_EXIF_LENGTH;
            jpg_exif += DC_EXIF_LENGTH;
        }

        if((mem_start + jpg_exif) > mem_end)
        {
            continue;
        }
        else
        {
            mem_len = mem_end - mem_start;
            mem_len = mem_len + cap_y_len - jpg_exif; // to reuse the start region of Y as JPG destination buffer;
        }

        out_frm_count = mem_len / frm_size; // the reserved memory is too few to save one jpg file;
        if(out_frm_count)
        {
            break;
        }
    }

    if(out_frm_count < 1)
    {
        return 0;
    }

    p_cap_mem_cfg->image_format = (uint8)img_fmt;
    if(p_cap_mem_cfg->is_jpg_sensor == 1)
    {
        dc_mem_cxt.dc_mem_cap_jpg_zoom = mem_start;
        dc_mem_cxt.dc_mem_cap_jpg_zoom_len = frm_size;
        out_frm_count -= 1;
        mem_start += frm_size;
        mem_len -= frm_size;
    }

    p_cap_mem_cfg->burst_number = out_frm_count;
    dc_mem_cxt.dc_mem_cap_burst_num = out_frm_count;
    dc_mem_cxt.dc_mem_cap_jpg = mem_start;
    dc_mem_cxt.dc_mem_cap_jpg_len = mem_len;

    return out_frm_count;
}

PUBLIC void* DCAMERA_CaptureGetStartBuf(uint32* p_buf_len)
{
    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_size;
    }

    return (void*)dc_mem_cxt.dc_mem_start;
}

PUBLIC void* DCAMERA_CaptureGetOSDBuf(uint32* p_buf_len)
{
    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_osd_len;
    }

    return (void*)dc_mem_cxt.dc_mem_cap_osd;
}

PUBLIC void* DCAMERA_CaptureGetYBuf(uint32* p_buf_len)
{
    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_y_len;
    }

    return (void*)dc_mem_cxt.dc_mem_cap_y;
}

PUBLIC void* DCAMERA_CaptureGetUVBuf(uint32* p_buf_len)
{
    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_uv_len;
    }

    return (void*)dc_mem_cxt.dc_mem_cap_uv;
}

PUBLIC void* DCAMERA_CaptureGetYUVBuf(uint32* p_buf_len)
{
    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_yuv_len;
    }

    return (void*)dc_mem_cxt.dc_mem_cap_yuv;
}

PUBLIC void*  DCAMERA_CaptureGetYUVRotBuf(uint32* p_buf_len)
{
    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_yuv_rot_len;
    }

    return (void*)dc_mem_cxt.dc_mem_cap_yuv_rot;
}

PUBLIC void* DCAMERA_CaptureGetQuickViewBuf(uint32* p_buf_len)
{
    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_rev_len;
    }

    return (void*)dc_mem_cxt.dc_mem_cap_review;

}

PUBLIC void* DCAMERA_CaptureGetJPGBuf(uint32* p_buf_len)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_cap_jpg;

    mem_start = DC_ADDR(mem_start);

    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_jpg + dc_mem_cxt.dc_mem_cap_jpg_len - mem_start;
    }

    return (void*)mem_start;

}

PUBLIC void* DCAMERA_CaptureGetJPGZoomBuf(uint32* p_buf_len)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_cap_jpg_zoom;

    mem_start = DC_ADDR(mem_start);

    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_jpg_zoom + dc_mem_cxt.dc_mem_cap_jpg_zoom_len - mem_start;
    }

    return (void*)mem_start;

}
PUBLIC void* DCAMERA_CaptureGetJPGTmpBuf(uint32* p_buf_len)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_cap_jpg_tmp;

    mem_start = DC_ADDR(mem_start);

    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_jpg_tmp + dc_mem_cxt.dc_mem_cap_jpg_tmp_len - mem_start;
    }

    return (void*)mem_start;

}

PUBLIC void* DCAMERA_CaptureGetLineBuf(uint32* p_buf_len)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_cap_line_buf;

    mem_start = DC_ADDR(mem_start);

    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_line_buf + dc_mem_cxt.dc_mem_cap_line_buf_len - mem_start;
    }

    return (void*)mem_start;

}

PUBLIC void* DCAMERA_CaptureGetSwapBufY(uint32* p_buf_len)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_cap_swap_buf_y;

    mem_start = DC_ADDR(mem_start);

    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_swap_buf_y + dc_mem_cxt.dc_mem_cap_swap_buf_y_len - mem_start;
    }

    return (void*)mem_start;
}

PUBLIC void* DCAMERA_CaptureGetSwapBufUV(uint32* p_buf_len)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_cap_swap_buf_uv;

    mem_start = DC_ADDR(mem_start);

    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_swap_buf_uv + dc_mem_cxt.dc_mem_cap_swap_buf_uv_len - mem_start;
    }

    return (void*)mem_start;

}

PUBLIC void* DCAMERA_CaptureGetSliceBufY(uint32* p_buf_len)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_cap_slice_buf_y;

    mem_start = DC_ADDR(mem_start);

    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_slice_buf_y + dc_mem_cxt.dc_mem_cap_slice_buf_y_len - mem_start;
    }

    return (void*)mem_start;

}

PUBLIC void* DCAMERA_CaptureGetSliceBufUV(uint32* p_buf_len)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_cap_slice_buf_uv;

    mem_start = DC_ADDR(mem_start);

    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_slice_buf_uv + dc_mem_cxt.dc_mem_cap_slice_buf_uv_len - mem_start;
    }

    return (void*)mem_start;

}

PUBLIC void*  DCAMERA_CaptureGetSrcSliceBufY(uint32* p_buf_len)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_cap_src_slice_buf_y;

    mem_start = DC_ADDR(mem_start);

    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_src_slice_buf_y + dc_mem_cxt.dc_mem_cap_src_slice_buf_y_len - mem_start;
    }

    return (void*)mem_start;

}
PUBLIC void*  DCAMERA_CaptureGetSrcSliceBufUV(uint32* p_buf_len)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_cap_src_slice_buf_uv;

    mem_start = DC_ADDR(mem_start);

    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_src_slice_buf_uv + dc_mem_cxt.dc_mem_cap_src_slice_buf_uv_len - mem_start;
    }

    return (void*)mem_start;
}


PUBLIC void*  DCAMERA_CaptureGetThumBufY(uint32* p_buf_len)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_cap_thum_buf_y;

    mem_start = DC_ADDR(mem_start);

    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_thum_buf_y + dc_mem_cxt.dc_mem_cap_thum_buf_y_len- mem_start;
    }

    return (void*)mem_start;

}
PUBLIC void*  DCAMERA_CaptureGetThumBufUV(uint32* p_buf_len)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_cap_thum_buf_uv;

    mem_start = DC_ADDR(mem_start);

    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_thum_buf_uv + dc_mem_cxt.dc_mem_cap_thum_buf_uv_len- mem_start;
    }

    return (void*)mem_start;

}
PUBLIC uint32 DCAMERA_CaptureGetBurstNum(void)
{
    return dc_mem_cxt.dc_mem_cap_burst_num;
}

PUBLIC void*  DCAMERA_CaptureExGetScaleBuf(uint32* p_buf_len)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_cap_line_buf;

    if(mem_start < dc_mem_cxt.dc_mem_start) //it's zero,no sclice encode
    {
        mem_start = dc_mem_cxt.dc_mem_cap_jpg_tmp;
    }
    mem_start = DC_ADDR(mem_start);

    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_cap_yuv - mem_start;
    }

    return (void*)mem_start;
}

PUBLIC void*  DCAMERA_PreviewExGetBuf(uint32* p_buf_len)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_start;

    mem_start = DC_ADDR(mem_start);

    if(SCI_NULL != p_buf_len)
    {
        *p_buf_len = dc_mem_cxt.dc_mem_end - mem_start;
    }
    return (void*)mem_start;
}

PUBLIC BOOLEAN   DCAMERA_Capture2JpegRev(uint16 img_width,
                                                                        uint16 img_height,
                                                                        uint8 img_fmt,
                                                                        uint8 disp_fmt,
                                                                        uint16 review_width,
                                                                        uint16 review_height,
                                                                        uint8 rot)
{
    uint32  mem_start = dc_mem_cxt.dc_mem_start;
    uint32  mem_end = dc_mem_cxt.dc_mem_end;
    uint32  frm_count = 1;
    uint32  frm_size = 0;

    //get review buffer
    if(DCAMERA_ROTATION_0 != rot)
    {
        frm_count += 1;
    }

    if(ISP_IMAGE_FORMAT_YUV422 == disp_fmt)
    {
        frm_size = (uint32)(review_width *review_height * 2);
    }
    else
    {
        frm_size = (uint32)(review_width * review_height * 3) >> 1;
    }

    frm_size += DCAMERA_PADDING_MEM_SIZE * 2;

    dc_mem_cxt.dc_mem_cap_rev_len = (frm_count * frm_size);
    mem_end -= dc_mem_cxt.dc_mem_cap_rev_len;
    if(mem_end < mem_start)
        return SCI_FALSE;
    dc_mem_cxt.dc_mem_cap_review = mem_end;

    //get YUV buffer
    dc_mem_cxt.dc_mem_cap_yuv_len = img_width * img_height;
    if(ISP_IMAGE_FORMAT_YUV422 == img_fmt)
    {
        dc_mem_cxt.dc_mem_cap_yuv_len = (dc_mem_cxt.dc_mem_cap_yuv_len * 2);
    }
    else
    {
        dc_mem_cxt.dc_mem_cap_yuv_len = ((dc_mem_cxt.dc_mem_cap_yuv_len * 3) >> 1) + DCAMERA_PADDING_MEM_SIZE;
    }
    dc_mem_cxt.dc_mem_cap_yuv_len += (DC_JPEG_YUV_PADDING_BYTES + (uint32)(DCAMERA_PADDING_MEM_SIZE * 2));
    mem_end = mem_end - dc_mem_cxt.dc_mem_cap_yuv_len;
    if(mem_end < mem_start)
        return SCI_FALSE;

    dc_mem_cxt.dc_mem_cap_yuv = mem_end;

    if((mem_start + DC_JPEG_TMP_BUF_LEN) > mem_end)
    {
        dc_mem_cxt.dc_mem_cap_jpg_tmp_len = 0;
        dc_mem_cxt.dc_mem_cap_jpg_tmp = SCI_NULL;
        return SCI_FALSE;
    }
    else
    {
        dc_mem_cxt.dc_mem_cap_jpg_tmp_len = mem_end - mem_start;
        dc_mem_cxt.dc_mem_cap_jpg_tmp = mem_start;
    }

    return SCI_TRUE;
}


