
#include "ms_ref_dc_trc.h"
#include "dc_common.h"

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/*lint -save -e767 -e685 -e568 -e718 -e746 -e529 -e64*/
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
// YUV to RGB
//r = y + 1.402 * (v - 128);
#define YUV_TO_R(_y, _v) ((int32)(_y) + ((359 * ((int32)(_v) - 128)) >> 8))
//g =y - 0.34414 * (u - 128) - 0.71414 * (v - 128);
#define YUV_TO_G(_y, _u, _v) ((int32)(_y) - ((88 * ((int32)(_u) - 128) + 183 * ((int32)(_v) - 128)) >> 8))
//b = y + 1.772 * (u - 128);
#define YUV_TO_B(_y, _u) ((int32)(_y) + ((454 * ((int32)(_u) - 128)) >> 8))

// RGB to YUV
//Y = 0.299 * r + 0.587 * g + 0.114 * b;
#define RGB_TO_Y(_r, _g, _b)    (int32)((77 * (_r) + 150 * (_g) + 29 * (_b)) >> 8)
//U = 128 - 0.1687 * r - 0.3313 * g + 0.5 * b 
#define RGB_TO_U(_r, _g, _b)    (int32)(128 + ((128 * (_b) - 43 * (_r) - 85 * (_g)) >> 8))
//V = 128 + 0.5 * r - 0.4187 * g - 0.0813 * b 
#define RGB_TO_V(_r, _g, _b)    (int32)(128  + ((128 * (_r) - 107 * (_g) - 21 * (_b)) >> 8))

#define RGB565_GET_R(_color) (((_color) >> 8) & 0xf8)
#define RGB565_GET_G(_color) (((_color) >> 3) & 0xfc)
#define RGB565_GET_B(_color) (((_color) << 3) & 0xf8)

#define RGB565_SET_R(_color, _r) ((_color) | (((_r) << 8) & 0xf800))
#define RGB565_SET_G(_color, _g) ((_color) | (((_g) << 3) & 0x07e0))
#define RGB565_SET_B(_color, _b) ((_color) | (((_b) >> 3) & 0x001f))

#define CLIP(x, bottom, top) ((x) < bottom ? bottom : (x) > top ? top :(x))

/******************************************************************************
// Description: dc one pixel yuv to rgb888
// Author:   Tim.zhu
// Input:     y: the value of y
//              uv: the value of u
//              v: the value of v
// Output:   rgb0/rgb0: the value of rgb888
// Return:   SCI_SUCCESS: success
******************************************************************************/ 
PUBLIC uint32 DCAMERA_PixelYuv2Rgb888(uint8 y0, uint8 y1, uint8 uv, uint8 v, uint32* rgb0, uint32* rgb1)
{
    int32 r=0x00;
    int32 g=0x00;
    int32 b=0x00;
    
    r=YUV_TO_R(y0, v);
    r=CLIP(r, 0x00, 0xff);
    g=YUV_TO_G(y0, uv, v);
    g=CLIP(g, 0x00, 0xff);
    b=YUV_TO_B(y0, uv);
    b=CLIP(b, 0x00, 0xff);
    *rgb0=(uint32)(((r<<0x10)&0xff0000)|((g<<0x08)&0xff00)|(b&0xff));

    r=YUV_TO_R(y1, v);
    r=CLIP(r, 0x00, 0xff);
    g=YUV_TO_G(y1, uv, v);
    g=CLIP(g, 0x00, 0xff);
    b=YUV_TO_B(y1, uv);
    b=CLIP(b, 0x00, 0xff);
    *rgb1=(uint32)(((r<<0x10)&0xff0000)|((g<<0x08)&0xff00)|(b&0xff));

    return SCI_SUCCESS;
}

/******************************************************************************
// Description: dc two pixel rgb888 to yuv
// Author:      Tim.zhu
// Output:      rgb0/rgb1: the value of rgb888
// Input:        y: the value of y
//                 uv: the value of u
//                 v: the value of v
// Return:      SCI_SUCCESS: success
******************************************************************************/ 
PUBLIC uint32 DCAMERA_PixelRgb8882Yuv(uint32 rgb0, uint32 rgb1, uint8* y0, uint8* y1,uint8* uv, uint8* v)
{
    int32 r=0x00;
    int32 g=0x00;
    int32 b=0x00;

    int32 _y0=0x00;
    int32 uv0=0x00;
    int32 v0=0x00;

    int32 _y1=0x00;
    int32 uv1=0x00;
    int32 v1=0x00;

    r=((rgb0>>0x10)&0xff);
    g=((rgb0>>0x08)&0xff);
    b=(rgb0&0xff);

    _y0=RGB_TO_Y(r, g, b);
    *y0=(uint8)CLIP(_y0, 0x00, 0xff);
    uv0=RGB_TO_U(r, g, b);
    uv0=CLIP(uv0, 0x00, 0xff);
    v0=RGB_TO_V(r, g, b);
    v0=CLIP(v0, 0x00, 0xff);

    r=((rgb1>>0x10)&0xff);
    g=((rgb1>>0x08)&0xff);
    b=(rgb1&0xff);

    _y1=RGB_TO_Y(r, g, b);
    *y1=(uint8)CLIP(_y1, 0x00, 0xff);
    uv1=RGB_TO_U(r, g, b);
    uv1=CLIP(uv1, 0x00, 0xff);
    v1=RGB_TO_V(r, g, b);
    v1=CLIP(v1, 0x00, 0xff);

    *uv=(uint8)((uv0+uv1)>>0x01);
    *v=(uint8)((v0+v1)>>0x01);

    return SCI_SUCCESS;
}


/******************************************************************************/
//  Description: calculate the channel size of the specified data format
//  Author: 	Shan.He 
//  Input:   
//			width:		image width
//			height:		image height
//			format:		image data format
//			extra_bytes:  extra bytes added to each channel
//  Output: 
//          chn_ptr:	pointer of the channel information. the pointer of each channel is invalid
//  Return:	TRUE    		successful
//  		FALSE          	failed
//  Note:   
/******************************************************************************/
PUBLIC uint32 DCAMERA_GetChnSize(IMGREF_CHN_T *chn_ptr, uint16 width, uint16 height, 
									IMGREF_DATA_FORMAT_E format, uint32 extra_bytes)
{
	chn_ptr->chn0.size = 0;
	chn_ptr->chn1.size = 0;

	switch (format)
	{
	case IMGREF_FORMAT_YUV422:
		chn_ptr->chn0.size = width * height + extra_bytes;
		chn_ptr->chn1.size = chn_ptr->chn0.size;		
		break;

	case IMGREF_FORMAT_YUV420:
		chn_ptr->chn0.size = width * height + extra_bytes;
		chn_ptr->chn1.size = (width >> 1) * height + extra_bytes;		
		break;

	case IMGREF_FORMAT_YUV400:
		chn_ptr->chn0.size = width * height + extra_bytes;
		break;

	case IMGREF_FORMAT_RGB565:
		chn_ptr->chn0.size = width * height * 2 + extra_bytes;
		break;	

	case IMGREF_FORMAT_RGB888:
	case IMGREF_FORMAT_ARGB888:		
		chn_ptr->chn0.size = width * height * 4 + extra_bytes;
		break;

	case IMGREF_FORMAT_ARGB565:
		//RGB565 in channel 0 and alpha in channel 1
		chn_ptr->chn0.size = width * height * 2 + extra_bytes;
		chn_ptr->chn1.size = width * height + extra_bytes;
		break;

	default:
        //SCI_TRACE_LOW:"DCAM: unsupport channel format"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_COMMON_174_112_2_18_1_23_58_297,(uint8*)"");
		return SCI_ERROR;
	}

	return SCI_SUCCESS;
}


/******************************************************************************
// Description: dc yuv data overlayer for capture data
// Author:       Tim.zhu
// Input:         DCAMERA_YUV_SRC_T_PTR: the point of src yuv information
// Output:       none
// Return:        SCI_SUCCESS: success
******************************************************************************/ 
/*lint -save -e737 */
PUBLIC uint32 DCAMERA_ArgbOverlayer(IMGREF_YUV_SRC *param_ptr, BOOLEAN src_yuv_mode, IMGREF_OSD_T *capture_date_ptr)
{
    uint32 rtn=DCAMERA_OP_SUCCESS;
    uint32 i=0x00;
    uint32 j=0x00;
    uint32 overlayer_height=0x00;
    uint32 src_offset=0x00;
    uint8* src_overlayer_y=NULL;
    uint8* src_overlayer_uv=NULL;
    uint32* osd_addr=NULL;
    uint16* osd_overlayer_uv=NULL;

    uint8* base_y_addr = PNULL;
    uint8* base_uv_addr = PNULL;

    uint32* overlay_base_addr = PNULL;

    IMGREF_OSD_T *cap_date_ptr = capture_date_ptr;

    //DCAM_CONTEXT_T_PTR pContext=_GetDCAMContext();
    if(((param_ptr->src_buf_rect.y+param_ptr->src_buf_rect.h)<cap_date_ptr->overlay_rect.y)
        ||((cap_date_ptr->overlay_rect.y+cap_date_ptr->overlay_rect.h)<param_ptr->src_buf_rect.y))
    {
        //SCI_TRACE_LOW:"DCAM: DC_ArgbOverlayer src_buf_rect y=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_COMMON_211_112_2_18_1_23_58_298,(uint8*)"d",param_ptr->src_buf_rect.y);
        //SCI_TRACE_LOW:"DCAM: DC_ArgbOverlayer src_buf_rect h=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_COMMON_212_112_2_18_1_23_58_299,(uint8*)"d",param_ptr->src_buf_rect.h);
        //SCI_TRACE_LOW:"DCAM: DC_ArgbOverlayer overlay_rect.y=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_COMMON_213_112_2_18_1_23_58_300,(uint8*)"d",cap_date_ptr->overlay_rect.y);
        //SCI_TRACE_LOW:"DCAM: DC_ArgbOverlayer overlay_rect.h=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_COMMON_214_112_2_18_1_23_58_301,(uint8*)"d",cap_date_ptr->overlay_rect.h);
        return rtn;
    }

    if(param_ptr->src_size.w<(cap_date_ptr->overlay_rect.x+cap_date_ptr->overlay_rect.w+1))
    {
        //SCI_TRACE_LOW:"DCAM: DC_ArgbOverlayer error src_addr:x=%d, w=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_COMMON_220_112_2_18_1_23_58_302,(uint8*)"dd",cap_date_ptr->overlay_rect.x,cap_date_ptr->overlay_rect.w);
        rtn=DCAMERA_OP_PARAM_ERR;
        return rtn;
    }
    
    //handle the blender rect size
    if((param_ptr->src_buf_rect.y+param_ptr->src_buf_rect.h)<=(cap_date_ptr->overlay_rect.y+cap_date_ptr->overlay_rect.h))
    {
        overlayer_height=(param_ptr->src_buf_rect.y+param_ptr->src_buf_rect.h)
                            -(cap_date_ptr->overlay_rect.y+cap_date_ptr->osd_offset);
    }
    else
    {
        overlayer_height=cap_date_ptr->overlay_rect.h-cap_date_ptr->osd_offset;
    }

    src_offset=(cap_date_ptr->overlay_rect.y+cap_date_ptr->osd_offset)-param_ptr->src_buf_rect.y;

    src_overlayer_y=(uint8*)(param_ptr->src.y_addr+(param_ptr->src_buf_rect.w*src_offset)+cap_date_ptr->overlay_rect.x);
    if(src_yuv_mode)        
    {
        //YUV422 situation handle
        src_overlayer_uv=(uint8*)(param_ptr->src.uv_addr+(param_ptr->src_buf_rect.w*src_offset)+cap_date_ptr->overlay_rect.x);
    }
    else
    {
        src_overlayer_uv=(uint8*)(param_ptr->src.uv_addr+((param_ptr->src_buf_rect.w)*(src_offset>>1))+cap_date_ptr->overlay_rect.x);
    }
    
    osd_addr=(uint32*)(cap_date_ptr->osd_addr.y_addr+(cap_date_ptr->overlay_rect.w*cap_date_ptr->osd_offset*0x04));

    base_y_addr = src_overlayer_y;
    base_uv_addr = src_overlayer_uv;

    overlay_base_addr = osd_addr;
#if 0
    //SCI_TRACE_LOW:"DCAM: DC_ArgbOverlayer src_buf_rect x =%d y=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_COMMON_256_112_2_18_1_23_58_303,(uint8*)"dd",param_ptr->src_buf_rect.x,param_ptr->src_buf_rect.y);
    //SCI_TRACE_LOW:"DCAM: DC_ArgbOverlayer src_buf_rect w =%d h=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_COMMON_257_112_2_18_1_23_58_304,(uint8*)"dd",param_ptr->src_buf_rect.w,param_ptr->src_buf_rect.h);
    //SCI_TRACE_LOW:"DCAM: DC_ArgbOverlayer overlay_rect.x =%d y=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_COMMON_258_112_2_18_1_23_58_305,(uint8*)"dd",cap_date_ptr->overlay_rect.x,cap_date_ptr->overlay_rect.y);
    //SCI_TRACE_LOW:"DCAM: DC_ArgbOverlayer overlay_rect.w =%d h=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_COMMON_259_112_2_18_1_23_58_306,(uint8*)"dd",cap_date_ptr->overlay_rect.w ,cap_date_ptr->overlay_rect.h);
#endif
    for(i=0x00; i<overlayer_height; i++)
    {
        osd_addr = overlay_base_addr + i* (cap_date_ptr->overlay_rect.w);
        src_overlayer_y = base_y_addr+ i*param_ptr->src_buf_rect.w;
        
        if(src_yuv_mode)        
        {
            //YUV422 situation handle
            src_overlayer_uv = base_uv_addr + i*param_ptr->src_buf_rect.w;
        }
        else
        {
            src_overlayer_uv = base_uv_addr + (i>>1)*param_ptr->src_buf_rect.w;
        }

        for(j=0x00; j<cap_date_ptr->overlay_rect.w; j+=0x02)
        {
            if((0x00!=(osd_addr[0]&0xff000000))||(0x00!=(osd_addr[1]&0xff000000)))
            {
                uint32 osd_r=0x00;
                uint32 osd_g=0x00;
                uint32 osd_b=0x00;
                uint32 src_r=0x00;
                uint32 src_g=0x00;
                uint32 src_b=0x00;

                uint32 rgb0=0x00;
                uint32 rgb1=0x00;
                uint8 y0=  *src_overlayer_y;
                uint8 y1=  *(src_overlayer_y+1);
                uint8 uv=*(src_overlayer_uv);
                uint8 v=*(src_overlayer_uv+1);
                uint8 alpha=0x00;

                DCAMERA_PixelYuv2Rgb888(y0, y1, uv, v, &rgb0, &rgb1);
                
                if(0x00!=(osd_addr[0]&0xff000000))
                {
                    
                    alpha=(uint8)((osd_addr[0]>>0x18)&0xff);
                    if(255>alpha)
                    {
                        osd_r=((osd_addr[0]>>0x10)&0xff);
                        osd_g=((osd_addr[0]>>0x08)&0xff);
                        osd_b=(osd_addr[0]&0xff);

                        src_r=((rgb0>>0x10)&0xff);
                        src_g=((rgb0>>0x08)&0xff);
                        src_b=(rgb0&0xff);

                        src_r=(osd_r*alpha+src_r*(0xff-alpha))>>8;
                        src_r=CLIP(src_r, 0x00, 0xff);

                        src_g=(osd_g*alpha+src_g*(0xff-alpha))>>8;
                        src_g=CLIP(src_g, 0x00, 0xff);

                        src_b=(osd_b*alpha+src_b*(0xff-alpha))>>8;
                        src_b=CLIP(src_b, 0x00, 0xff);

                        rgb0=((src_r<<0x10)&0xff0000)|((src_g<<0x08)&0xff00)|(src_b&0xff);
                    }
                    else
                    {
                        rgb0 = osd_addr[0];
                    }

                }

                if(0x00!=(osd_addr[1]&0xff000000))
                {
                    alpha=(uint8)((osd_addr[1]>>0x18)&0xff);

                    if(255>alpha)
                    {
                        osd_r=((osd_addr[1]>>0x10)&0xff);
                        osd_g=((osd_addr[1]>>0x08)&0xff);
                        osd_b=(osd_addr[1]&0xff);

                        src_r=((rgb1>>0x10)&0xff);
                        src_g=((rgb1>>0x08)&0xff);
                        src_b=(rgb1&0xff);

                        src_r=(osd_r*alpha+src_r*(0xff-alpha))>>8;
                        src_r=CLIP(src_r, 0x00, 0xff);

                        src_g=(osd_g*alpha+src_g*(0xff-alpha))>>8;
                        src_g=CLIP(src_g, 0x00, 0xff);

                        src_b=(osd_b*alpha+src_b*(0xff-alpha))>>8;
                        src_b=CLIP(src_b, 0x00, 0xff);

                        rgb1=((src_r<<0x10)&0xff0000)|((src_g<<0x08)&0xff00)|(src_b&0xff);
                    }
                    else
                    {
                         rgb1 = osd_addr[1];
                    }

                }

                DCAMERA_PixelRgb8882Yuv(rgb0, rgb1, &y0, &y1, &uv, &v);

                src_overlayer_y[0]=y0;
                src_overlayer_y[1]=y1;
                src_overlayer_uv[0]=uv;
                src_overlayer_uv[1]=v;

            }

            osd_addr+=0x02;
            src_overlayer_y+=0x02;
            src_overlayer_uv+=0x02;


        }

        cap_date_ptr->osd_offset++;
    }

    return rtn;    
}

PUBLIC void DCAMERA_Convert_Coor( LCD_ID_E lcd_id, DCAMERA_CONVERT_COOR_T *src_ptr, DCAMERA_CONVERT_COOR_T *dst_ptr)
{

    DCAMERA_ROTATION_MODE_E dst_rot_degree = src_ptr->rot_degree;

    LCD_INFO_T lcd_info={0x00};

    LCD_GetInfo(lcd_id, &lcd_info);


    //SCI_TRACE_LOW:"DCAMERA_Convert_Coor,src x:%d, y:%d, w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_COMMON_393_112_2_18_1_23_59_307,(uint8*)"dddd", src_ptr->rect.x, src_ptr->rect.y, src_ptr->rect.w, src_ptr->rect.h);
    //SCI_TRACE_LOW:"DCAMERA_Convert_Coor,src degree:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_COMMON_394_112_2_18_1_23_59_308,(uint8*)"d", src_ptr->rot_degree);

    switch(dst_rot_degree)
    {
        case DCAMERA_ROTATION_90:
            dst_ptr->rect.w = src_ptr->rect.h;
            dst_ptr->rect.h = src_ptr->rect.w;
            dst_ptr->rect.x = src_ptr->rect.y;
            dst_ptr->rect.y = lcd_info.lcd_width-dst_ptr->rect.h - src_ptr->rect.x;
        break;

        case DCAMERA_ROTATION_180:
            dst_ptr->rect.x = lcd_info.lcd_width -src_ptr->rect.w - src_ptr->rect.x;
            dst_ptr->rect.y = lcd_info.lcd_height-src_ptr->rect.h - src_ptr->rect.y;
            dst_ptr->rect.w =  src_ptr->rect.w;
            dst_ptr->rect.h =  src_ptr->rect.h;
        break;

        case DCAMERA_ROTATION_270:
            dst_ptr->rect.w = src_ptr->rect.h;
            dst_ptr->rect.h = src_ptr->rect.w;
            dst_ptr->rect.x = lcd_info.lcd_height-dst_ptr->rect.w-src_ptr->rect.y;
            dst_ptr->rect.y = src_ptr->rect.x;
        break;
        
        default:
              SCI_MEMCPY(dst_ptr, src_ptr, sizeof(DCAMERA_CONVERT_COOR_T)); //lint !e628
        break;
        

    }

    dst_ptr->rot_degree = dst_rot_degree;

    //SCI_TRACE_LOW:"DCAMERA_Convert_Coor,dst x:%d, y:%d, w:%d, h:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_COMMON_428_112_2_18_1_23_59_309,(uint8*)"dddd", dst_ptr->rect.x, dst_ptr->rect.y, dst_ptr->rect.w, dst_ptr->rect.h);
    //SCI_TRACE_LOW:"DCAMERA_Convert_Coor,dst degree:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_COMMON_429_112_2_18_1_23_59_310,(uint8*)"d", dst_ptr->rot_degree);

    return;
}


/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif// End



