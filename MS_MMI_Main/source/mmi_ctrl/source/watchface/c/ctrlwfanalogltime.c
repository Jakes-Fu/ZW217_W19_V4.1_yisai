/*****************************************************************************
** File Name:       ctrlwfanalogtime.c                                       *
** Author:          yuming.yang                                              *
** Date:            08/13/2021                                               *
** Copyright:                                                                *
** Description:    This file is used to define watch face control            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/13/2021       yuming.yang               Create                         *
******************************************************************************/

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_string.h"
#include "ctrlwfanalogtime.h"
#include "ctrlwf_export.h"
#include "ctrlwfcommon.h"
#include "mmk_timer.h"
#include "dal_time.h"
#include "graphics_draw.h"
#include "guilcd.h"
#include "ctrlownerdraw.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

#define ARGB_GET_A(_argb) (((_argb) >> 24) & 0xff)
#define ARGB_GET_R(_argb) (((_argb) >> 16) & 0xff)
#define ARGB_GET_G(_argb) (((_argb) >> 8) & 0xff)
#define ARGB_GET_B(_argb) ((_argb) & 0xff)

#define ARGB_SET_A(_argb, _alpha)   ((_argb) | (((_alpha) << 24) & 0xff000000))
#define ARGB_SET_R(_argb, _r)       ((_argb) | (((_r) << 16) & 0xff0000))
#define ARGB_SET_G(_argb, _g)       ((_argb) | (((_g) << 8) & 0xff00))
#define ARGB_SET_B(_argb, _b)       ((_argb) | (((_b)) & 0xff))

#define RGB565_GET_R(_color) (((_color) >> 8) & 0xf8)
#define RGB565_GET_G(_color) (((_color) >> 3) & 0xfc)
#define RGB565_GET_B(_color) (((_color) << 3) & 0xf8)

#define RGB565_SET_R(_color, _r) ((_color) | (((_r) << 8) & 0xf800))
#define RGB565_SET_G(_color, _g) ((_color) | (((_g) << 3) & 0x07e0))
#define RGB565_SET_B(_color, _b) ((_color) | (((_b) >> 3) & 0x001f))

#define RGB888_TO_RGB565(r, g, b)  (((r << 8) & 0xf800) | ((g << 3) & 0x07e0) | ((b >> 3) & 0x1f))
#define RGB8882RGB565(rgb888)   ((((rgb888)&0xf80000)>>8)|(((rgb888)&0xfc00)>>5)|(((rgb888)&0xf8)>>3))

#define RGB5652ARGB8888(rgb565,alpha) ( ((alpha)<<24) |((((rgb565) >> 8) & 0xf8)<<16)|((((rgb565) >> 3) & 0xfc)<<8)|(((rgb565) << 3)  & 0xf8))

#define BLEND_MASK 0x7E0F81F



/**--------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef struct
{
    uint16          width;
    uint16          height;
    uint8*          data_ptr;
    uint32          data_size;
}IMG_PROCESS_DATA_T;


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : decode and draw
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DecodeImageToBufferWithAngle(uint8*      dst_buf,
                                                        uint16      dst_width,
                                                        uint16      dst_height,
                                                        wchar*      p_img_hand,
                                                        uint32      angle
                                                        );

/*****************************************************************************/
//  Description : display analog clock
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void DisplayAnalogTime(WATCHFACE_ANALOG_TIME_DATA_T* p_info, wchar* p_hand[]);

/*****************************************************************************/
//  Description : update by timer
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void UpdateAnalogClock(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : create data
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_ANALOG_TIME_DATA_T* CreateAnalogTimeData(MMI_WIN_ID_T      win_id,
                                                                MMI_CTRL_ID_T   ctrl_id,
                                                                GUI_POINT_T         center_point,
                                                                uint32              update_interval,
                                                                char*               p_image_hour,
                                                                char*               p_image_minute,
                                                                char*               p_image_second,
                                                                float               scale
                                                                );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : blend RGB
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL __inline uint16 BlendRGB565(uint16 bk_color, uint16 fore_color, uint8 alpha)
{
    uint32 blend_color = 0;
    uint32 temp_back_color = 0;
    uint32 temp_fore_color = 0;

    alpha = (alpha + 1) >> 3;

    temp_fore_color = ( fore_color | (fore_color << 16)) & BLEND_MASK;
    temp_back_color = ( bk_color | (bk_color << 16)) & BLEND_MASK;
    blend_color = ((((temp_fore_color - temp_back_color) * alpha ) >> 5 ) + temp_back_color) & BLEND_MASK;
    blend_color = ((blend_color & 0xFFFF) | (blend_color >> 16));

    return (uint16)blend_color;
}


/*****************************************************************************/
//  Description : blend alpha
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProcessAlphaBlending(WATCHFACE_ALPHA_IMG_T* p_bk_img, WATCHFACE_ALPHA_IMG_T* p_fore_img)
{
    uint32      fore_width  = 0;
    uint32      fore_height = 0;
    uint32      bk_width = 0;
    uint32      bk_height = 0;
    int32       start_x = 0;
    int32       start_y = 0;
    uint32      blend_width = 0;
    uint32      blend_height = 0;
    uint32*     fore_ptr = PNULL;
    GUI_RECT_T  dst_rect = {0};
    GUI_RECT_T  bk_rect = {0};
    GUI_RECT_T  for_rect = {0};

    if ((PNULL == p_bk_img) || (PNULL == p_fore_img))
    {
        CTRL_WF_TRACE("p_bk_img = pnull or p_fore_img = pnull!");
        return FALSE;
    }

    fore_width  = p_fore_img->width;
    fore_height = p_fore_img->height;
    bk_width    = p_bk_img->width;
    bk_height   = p_bk_img->height;
    start_x     = p_fore_img->start_x;
    start_y     = p_fore_img->start_y;
    blend_width = fore_width;
    blend_height = fore_height;
    fore_ptr    = (uint32 *)p_fore_img->data_ptr;

    bk_rect.left  = p_bk_img->start_x;
    bk_rect.top   = p_bk_img->start_y;
    bk_rect.right = p_bk_img->start_x + p_bk_img->width;
    bk_rect.bottom = p_bk_img->start_y + p_bk_img->height;

    for_rect.left  = p_fore_img->start_x;
    for_rect.top   = p_fore_img->start_y;
    for_rect.right = p_fore_img->start_x + p_fore_img->width;
    for_rect.bottom = p_fore_img->start_y + p_fore_img->height;

    if (FALSE == GUI_IntersectRect(&dst_rect, bk_rect, for_rect))
    {
        CTRL_WF_TRACE("rect error.");
        return FALSE;
    }

    if (start_y < 0)
    {
        start_y = -start_y;
        blend_height -= start_y;
        fore_ptr += (start_y * fore_width);
        start_y = 0;
    }

    if (start_x < 0)
    {
        start_x = -start_x;
        blend_width -= start_x;
        fore_ptr += start_x;
        start_x = 0;
    }

    if ((int32)bk_width < start_x || (int32)bk_height < start_y)
    {
        CTRL_WF_TRACE("(bk_width < start_x) or (bk_height < start_y)");
        return FALSE;
    }

    blend_width = (start_x + blend_width) > bk_width
                    ? (bk_width - start_x) : blend_width;
    blend_height = (start_y + blend_height) > bk_height
                    ? (bk_height - start_y) : blend_height;

    if (IMGREF_FORMAT_RGB565 == p_bk_img->data_type) //output RGB565
    {
        uint32  i = 0, j = 0;
        uint16* bk_ptr = (uint16 *)p_bk_img->data_ptr;

        bk_ptr += (start_y * bk_width + start_x);

        for (i=0; i<blend_height; i++)
        {
            for (j=0; j<blend_width; j++)
            {
                uint32 alpha, r, g, b;
                uint32 fore_value = *(fore_ptr + j);
                uint16 bk_value = *(bk_ptr + j);

                alpha = ARGB_GET_A(fore_value);
                r = ARGB_GET_R(fore_value);
                g = ARGB_GET_G(fore_value);
                b = ARGB_GET_B(fore_value);

                fore_value = RGB888_TO_RGB565(r, g, b);
                bk_value = BlendRGB565(bk_value, (uint16)fore_value, alpha);
                *(bk_ptr + j) = bk_value;
            }

            bk_ptr += bk_width;
            fore_ptr += fore_width;
        }
    }
    else  //output ARGB888
    {
        uint32 i = 0, j = 0;
        uint32 *bk_ptr = (uint32 *)p_bk_img->data_ptr;

        bk_ptr += (start_y * bk_width + start_x);

        for (i = 0; i < blend_height; i++)
        {
            for (j = 0; j < blend_width; j++)
            {
                uint32 back_pix = *(bk_ptr+j);
                uint32 fore_pix = *(fore_ptr + j);
                uint32 fa = ARGB_GET_A(fore_pix);//alpha of foreground image
                uint32 fr; //R of fore img
                uint32 fg; //G of fore img
                uint32 fb; //B of fore img
                uint32 ba = ARGB_GET_A(back_pix);//alpha of background image.
                uint32 br; //R of bg img
                uint32 bg; //G of bg img
                uint32 bb; //B of bg img
                uint32 desta;
                uint32 destr;
                uint32 destg;
                uint32 destb;
                if (0 == ba)
                {
                    *(bk_ptr + j) = *(fore_ptr + j);
                    continue;
                }
                else if(0 == fa)
                {
                    continue;
                }
                else if(fa == 255)
                {
                    *(bk_ptr + j) = *(fore_ptr + j);
                    continue;
                }
                else
                {
                    fr = ARGB_GET_R(fore_pix);
                    fg = ARGB_GET_G(fore_pix);
                    fb = ARGB_GET_B(fore_pix);
                    br = ARGB_GET_R(back_pix);
                    bg = ARGB_GET_G(back_pix);
                    bb = ARGB_GET_B(back_pix);
                    desta = fa + ba - (fa*ba/255); //alpha = alpha1 + alpha2 - alpha1*alpha2
                    destr = (fr*fa+br*ba-(br*fa*ba/255))/desta; //c12 = (c1a1(1-a2)+c2a2)/alpha  1:bg, 2:fg
                    destg = (fg*fa+bg*ba-(bg*fa*ba/255))/desta;
                    destb = (fb*fa+bb*ba-(bb*fa*ba/255))/desta;
                    *(bk_ptr+j) = (desta<<24) | (destr<<16) | (destg<<8) | destb;
                }
            }
            bk_ptr += p_bk_img->width;
            fore_ptr += p_fore_img->width;
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : decode img
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Image_DecodeByPath(IMG_PROCESS_DATA_T decode_data, wchar* p_img_path)
{
    BOOLEAN             result          = FALSE;
    GUIIMG_SRC_T        src_info        = {0};
    GUIIMG_DISPLAY_T    img_display     = {0};
    GUIIMG_DEC_OUT_T    img_output      = {0};

    if (PNULL == decode_data.data_ptr)
    {
        CTRL_WF_TRACE("decode_data.data_ptr == PNULL.");
        return result;
    }

    if (PNULL == p_img_path)
    {
        CTRL_WF_TRACE("p_img_path == PNULL.");
        return result;
    }

    //set src info
    src_info.is_file = TRUE;
    src_info.full_path_ptr = p_img_path;
    //set display info
    img_display.is_handle_transparent = TRUE;
    img_display.dest_width  = decode_data.width;
    img_display.dest_height = decode_data.height;
    //set output data
    img_output.actual_width = decode_data.width;
    img_output.actual_height = decode_data.height;
    img_output.decode_data_ptr  = decode_data.data_ptr;
    img_output.decode_data_size = decode_data.data_size;

    result = GUIIMG_Decode(&src_info, &img_display, &img_output);
    return result;
}

/*****************************************************************************/
//  Description : extend img
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Image_ExtendToSquare(IMG_PROCESS_DATA_T src_data, IMG_PROCESS_DATA_T dst_data)
{
    BOOLEAN                 result = FALSE;
    WATCHFACE_ALPHA_IMG_T   bk = {0};
    WATCHFACE_ALPHA_IMG_T   fg = {0};

    if (PNULL == src_data.data_ptr)
    {
        CTRL_WF_TRACE("src_data.data_ptr == PNULL.");
        return result;
    }

    if (PNULL == dst_data.data_ptr)
    {
        CTRL_WF_TRACE("dst_data.data_ptr == PNULL.");
        return result;
    }

    fg.data_ptr = src_data.data_ptr;
    fg.data_type = 1;
    fg.start_x = (dst_data.width / 2) - (src_data.width / 2);
    fg.start_y = 0;
    fg.width = src_data.width;
    fg.height = src_data.height;

    bk.data_ptr = dst_data.data_ptr;
    bk.data_type = 1;
    bk.start_x = 0;
    bk.start_y = 0;
    bk.width = dst_data.width;
    bk.height = dst_data.height;

    result = ProcessAlphaBlending(&bk, &fg);
    return result;
}


/*****************************************************************************/
//  Description : scale img
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Image_ScaleToDstSize(IMG_PROCESS_DATA_T src_data, IMG_PROCESS_DATA_T dst_data)
{
    SCALE_IMAGE_IN_T    scale_in = {0};
    SCALE_IMAGE_OUT_T   scale_out = {0};

    if (PNULL == src_data.data_ptr)
    {
        CTRL_WF_TRACE("src_data.data_ptr == PNULL.");
        return FALSE;
    }

    if (PNULL == dst_data.data_ptr)
    {
        CTRL_WF_TRACE("dst_data.data_ptr == PNULL.");
        return FALSE;
    }

    //set src width and height
    scale_in.src_size.w = src_data.width;
    scale_in.src_size.h = src_data.height;
    //trim rect of src image
    scale_in.src_trim_rect.x = 0;
    scale_in.src_trim_rect.y = 0;
    scale_in.src_trim_rect.w = src_data.width;
    scale_in.src_trim_rect.h = src_data.height;
    //set src format
    scale_in.src_format = IMGREF_FORMAT_ARGB888;
    //set src buffer
    scale_in.src_chn.chn0.ptr = src_data.data_ptr;
    scale_in.src_chn.chn0.size = src_data.data_size;
    //set target width and height
    scale_in.target_size.w = dst_data.width;
    scale_in.target_size.h = dst_data.height;
    //set target format
    scale_in.target_format = IMGREF_FORMAT_ARGB888;
    //set target buffer
    scale_in.target_buf.ptr = dst_data.data_ptr;
    scale_in.target_buf.size = dst_data.data_size;
    scale_in.scale_mode = SCALE_MODE_NO_DISTORT;

    if (SCI_SUCCESS != GRAPH_ScaleImage(&scale_in, &scale_out))
    {
        CTRL_WF_TRACE("GRAPH_ScaleImage failed.");
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : rotate img
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Image_RotateWithAngle(IMG_PROCESS_DATA_T src_data, IMG_PROCESS_DATA_T dst_data, uint32 angle)
{
    ROTATE_ARBITRARY_IN_T   rot_in = {0};
    ROTATE_ARBITRARY_OUT_T  rot_out = {0};

    if (PNULL == src_data.data_ptr)
    {
        CTRL_WF_TRACE("src_data.data_ptr == PNULL.");
        return FALSE;
    }

    if (PNULL == dst_data.data_ptr)
    {
        CTRL_WF_TRACE("dst_data.data_ptr == PNULL.");
        return FALSE;
    }

    //set rot angle
    rot_in.angle = angle;
    //set src info
    rot_in.src_format = IMGREF_FORMAT_ARGB888;
    rot_in.src_size.w = src_data.width;
    rot_in.src_size.h = src_data.height;
    rot_in.src_center.x = src_data.width >> 1;
    rot_in.src_center.y = src_data.height >> 1;
    rot_in.src_chn.chn0.ptr = src_data.data_ptr;
    rot_in.src_chn.chn0.size = src_data.data_size;
    //set target info
    rot_in.target_format = IMGREF_FORMAT_ARGB888;
    rot_in.target_size.w = dst_data.width;
    rot_in.target_size.h = dst_data.height;
    rot_in.target_chn.chn0.ptr = dst_data.data_ptr;
    rot_in.target_chn.chn0.size = dst_data.data_size;
    rot_in.target_center.x = dst_data.width >> 1;
    rot_in.target_center.y = dst_data.height >> 1;
    //set mode
    rot_in.mode = ROTATE_ARBITRARY_AUTO_CROP;

    if (SCI_SUCCESS != GRAPH_RotateArbitrary(&rot_in, &rot_out))
    {
        CTRL_WF_TRACE("GRAPH_RotateArbitrary fail.");
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : decode and draw
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DecodeImageToBufferWithAngle(uint8*      dst_buf,
                                                        uint16      dst_width,
                                                        uint16      dst_height,
                                                        wchar*      p_img_hand,
                                                        uint32      angle
                                                        )
{
    GUIIMG_INFO_T       img_info = {0};
    uint16              img_width = 0;
    uint16              img_height = 0;
    BOOLEAN             is_need_extend = FALSE;
    BOOLEAN             is_need_scale = FALSE;
    IMG_PROCESS_DATA_T  decode_data = {0};
    IMG_PROCESS_DATA_T  extend_data = {0};
    IMG_PROCESS_DATA_T  scale_data = {0};
    IMG_PROCESS_DATA_T  rotate_data = {0};
    uint8*              p_decode_buf = PNULL;
    uint32              decode_buf_size = 0;
    uint8*              p_extend_buf = PNULL;
    uint32              extend_buf_size = 0;
    uint16              extend_buf_width = 0;
    uint8*              p_scale_buf = PNULL;
    uint32              scale_buf_size = 0;

    if (PNULL == dst_buf)
    {
        CTRL_WF_TRACE("dst_buf == PNULL.");
        return FALSE;
    }

    if (PNULL == p_img_hand)
    {
        CTRL_WF_TRACE("p_img_hand == PNULL.");
        return FALSE;
    }

    //step1. get image base info
    CTRLWF_Common_GetImgInfoByPath(p_img_hand, &img_info);
    img_width = img_info.image_width;
    img_height = img_info.image_height;

    if(img_width != img_height)
    {
        CTRL_WF_TRACE("img need extend, img_width = %d, img_height = %d", img_width, img_height);
        is_need_extend = TRUE;
    }

    if ((dst_width != img_width) || (dst_height != img_height))
    {
        CTRL_WF_TRACE("img need scale, dst_height = %d, img_height = %d", dst_height, img_height);
        is_need_scale = TRUE;
    }

    //step2. decode
    decode_buf_size = img_width * img_height * sizeof(uint32);
    p_decode_buf = (uint8 *)SCI_ALLOC_APPZ(decode_buf_size);
    if (PNULL == p_decode_buf)
    {
        CTRL_WF_TRACE("malloc p_decode_buf error.");
        return FALSE;
    }

    decode_data.width = img_width;
    decode_data.height = img_height;
    decode_data.data_ptr = p_decode_buf;
    decode_data.data_size = decode_buf_size;
    if(FALSE == Image_DecodeByPath(decode_data, p_img_hand))
    {
        CTRL_WF_TRACE("Image decode fail.");
        SCI_FREE(p_decode_buf);
        return FALSE;
    }

    //step3. extend
    if (TRUE == is_need_extend)
    {
        extend_buf_width = (img_width > img_height) ? img_width : img_height;
        extend_buf_size = extend_buf_width * extend_buf_width * sizeof(uint32);
        p_extend_buf = (uint8 *)SCI_ALLOC_APPZ(extend_buf_size);
        if (PNULL == p_extend_buf)
        {
            CTRL_WF_TRACE("malloc p_extend_buf error.");
            SCI_FREE(p_decode_buf);
            return FALSE;
        }

        extend_data.width = extend_buf_width;
        extend_data.height = extend_buf_width;
        extend_data.data_ptr = p_extend_buf;
        extend_data.data_size = extend_buf_size;
        if(FALSE == Image_ExtendToSquare(decode_data, extend_data))
        {
            CTRL_WF_TRACE("Image extend fail.");
            SCI_FREE(p_decode_buf);
            SCI_FREE(p_extend_buf);
            return FALSE;
        }
    }

    //step4 .scale image
    if (TRUE == is_need_scale)
    {
        BOOLEAN scale_result = FALSE;

        scale_buf_size = dst_width * dst_height * sizeof(uint32);
        p_scale_buf = (uint8 *)SCI_ALLOC_APPZ(scale_buf_size);
        if (PNULL == p_scale_buf)
        {
            CTRL_WF_TRACE("malloc p_scale_buf error.");
            SCI_FREE(p_decode_buf);
            SCI_FREE(p_extend_buf);
            return FALSE;
        }

        scale_data.width = dst_width;
        scale_data.height = dst_height;
        scale_data.data_ptr = p_scale_buf;
        scale_data.data_size = scale_buf_size;

        if (TRUE == is_need_extend)
        {
            //scale extend_data
            scale_result = Image_ScaleToDstSize(extend_data, scale_data);
        }
        else
        {
            //scale decode_data
            scale_result = Image_ScaleToDstSize(decode_data, scale_data);
        }

        if(FALSE == scale_result)
        {
            CTRL_WF_TRACE("Image scale fail.");
            SCI_FREE(p_decode_buf);
            SCI_FREE(p_extend_buf);
            SCI_FREE(p_scale_buf);
            return FALSE;
        }
    }

    //step5 .rotate image && draw it to dstbuf
    {
        BOOLEAN rotate_result = FALSE;

        rotate_data.width = dst_width;
        rotate_data.height = dst_height;
        rotate_data.data_ptr = dst_buf;
        rotate_data.data_size = dst_width * dst_height * sizeof(uint32);

        if (TRUE == is_need_scale)
        {
            //rotate scale_data
            rotate_result = Image_RotateWithAngle(scale_data, rotate_data, angle);
        }
        else if (TRUE == is_need_extend)
        {
            //scale extend_data
            rotate_result = Image_RotateWithAngle(extend_data, rotate_data, angle);
        }
        else
        {
            //scale decode_data
            rotate_result = Image_RotateWithAngle(decode_data, rotate_data, angle);
        }

        if(FALSE == rotate_result)
        {
            CTRL_WF_TRACE("Image rotate fail.");
            SCI_FREE(p_decode_buf);
            SCI_FREE(p_extend_buf);
            SCI_FREE(p_scale_buf);
            return FALSE;
        }
    }
    SCI_FREE(p_decode_buf);
    SCI_FREE(p_extend_buf);
    SCI_FREE(p_scale_buf);
    return TRUE;
}


/*****************************************************************************/
//  Description : display analog clock
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void DisplayAnalogTime(WATCHFACE_ANALOG_TIME_DATA_T* p_info, wchar* p_hand[])
{
#ifdef UI_MULTILAYER_SUPPORT
    uint8               i = 0;
    SCI_TIME_T          sys_time= {0};
    uint16              angle_arr[3] = {0};
    uint8*              p_rotate_buf = PNULL;
    uint32              rotate_buf_size = 0;
    uint16              rect_width = 0;
    uint16              rect_height = 0;

    if (PNULL == p_info)
    {
        CTRL_WF_TRACE("p_info = pnull!");
        return;
    }

    if (PNULL == p_hand)
    {
        CTRL_WF_TRACE("p_hand = pnull!");
        return;
    }

    rect_width = p_info->rect.right - p_info->rect.left;
    rect_height = p_info->rect.bottom - p_info->rect.top;

    //get system time
    TM_GetSysTime(&sys_time);
#ifdef WIN32
    do
    {
        LOCAL uint8 localsec = 0;
        localsec++;
        localsec %= 60;
        sys_time.sec= localsec;
    } while (0);
#endif
    //clear layer
    UILAYER_Clear(&(p_info->layer));

    //get angle by time
    GRAPH_GetAngleByTimeValue(sys_time.hour, sys_time.min, sys_time.sec, &angle_arr[0], &angle_arr[1], &angle_arr[2]);

    //malloc rotate buf
    rotate_buf_size = rect_width * rect_height * sizeof(uint32);
    if (PNULL == p_rotate_buf)
    {
        p_rotate_buf = SCI_ALLOC_APPZ(rotate_buf_size);
        CTRL_WF_TRACE("malloc p_rotate_buf, addr = 0x%0x.", (uint32)p_rotate_buf);// bug id  1941703 CID 421162
        if (PNULL == p_rotate_buf)
        {
            CTRL_WF_TRACE("malloc p_rotate_buf error.");
            return;
        }
    }

    //draw analog clock
    for (i = 0; i < 3; i++)
    {
        WATCHFACE_ALPHA_IMG_T   bk = {0};
        WATCHFACE_ALPHA_IMG_T   fg = {0};
        UILAYER_INFO_T  layer_info;
        SCI_MEMSET(p_rotate_buf, 0, rotate_buf_size);
        DecodeImageToBufferWithAngle(p_rotate_buf, rect_width, rect_height, p_hand[i], angle_arr[i]);
        UILAYER_GetLayerInfo(&(p_info->layer), &layer_info);
        if(layer_info.data_type!=DATA_TYPE_ARGB888 && layer_info.data_type!=DATA_TYPE_RGB565)
        {
            break;
        }
        bk.data_ptr = UILAYER_GetLayerBufferPtr(&(p_info->layer));
        bk.data_type = (layer_info.data_type==DATA_TYPE_ARGB888) ? IMGREF_FORMAT_ARGB888 : IMGREF_FORMAT_RGB565;//only support argb888& rgb565
//        bk.data_type = 1;
        bk.start_x = 0;
        bk.start_y = 0;
        bk.width = rect_width;
        bk.height = rect_height;

        fg.data_ptr = p_rotate_buf;
        fg.data_type = 1;
        fg.start_x = 0;
        fg.start_y = 0;
        fg.width = rect_width;
        fg.height = rect_height;
        ProcessAlphaBlending(&bk, &fg);
        CTRL_WF_TRACE("draw analog clock.");
    }

    //free rotate buf
    if (PNULL != p_rotate_buf)
    {
        CTRL_WF_TRACE("free p_rotate_buf, addr = 0x%0x.", (uint32)p_rotate_buf);// bug id 1941703 CID 421162
        SCI_FREE(p_rotate_buf);
    }

#endif
}


/*****************************************************************************/
//  Description : display analog clock
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void displayTime(WATCHFACE_ANALOG_TIME_DATA_T* p_info)
{
#ifdef UI_MULTILAYER_SUPPORT
    uint8               i = 0;
    SCI_TIME_T          sys_time= {0};
    uint16              angle_arr[3] = {0};
    uint8*              p_rotate_buf = PNULL;
    uint32              rotate_buf_size = 0;
    uint16              rect_width = 0;
    uint16              rect_height = 0;

    if (PNULL == p_info)
    {
        CTRL_WF_TRACE("p_info = pnull!");
        return;
    }

    rect_width = p_info->rect.right - p_info->rect.left;
    rect_height = p_info->rect.bottom - p_info->rect.top;
    CTRL_WF_TRACE("draw analog clock.");
    //get system time
    TM_GetSysTime(&sys_time);
#ifdef WIN32
    do
    {
        LOCAL uint8 localsec = 0;
        localsec++;
        localsec %= 60;
        sys_time.sec= localsec;
    } while (0);
#endif
#if defined(UI_MULTILAYER_SUPPORT) && !defined(WF_USE_SINGLELAYER) //2130014
    //clear layer
    UILAYER_Clear(&(p_info->layer)); // only clear layer when use multilayer for analogtime needle
#endif
    //get angle by time
    GRAPH_GetAngleByTimeValue(sys_time.hour, sys_time.min, sys_time.sec, &angle_arr[0], &angle_arr[1], &angle_arr[2]);

    //malloc rotate buf
    rotate_buf_size = rect_width * rect_height * sizeof(uint32);
    if (PNULL == p_rotate_buf)
    {
        p_rotate_buf = SCI_ALLOC_APPZ(rotate_buf_size);
        CTRL_WF_TRACE("malloc p_rotate_buf, addr = 0x%0x.", (uint32)p_rotate_buf);// bug id  1941703 CID 421162
        if (PNULL == p_rotate_buf)
        {
            CTRL_WF_TRACE("malloc p_rotate_buf error.");
            return;
        }
    }

    //draw analog clock
    for (i = 0; i < 3; i++)
    {
        WATCHFACE_ALPHA_IMG_T   bk = {0};
        WATCHFACE_ALPHA_IMG_T   fg = {0};
        UILAYER_INFO_T  layer_info;
        SCI_MEMSET(p_rotate_buf, 0, rotate_buf_size);
        DecodeImageToBufferWithAngle(p_rotate_buf, rect_width, rect_height, p_info->img_hands[i], angle_arr[i]);
        UILAYER_GetLayerInfo(&(p_info->layer), &layer_info);
        if(layer_info.data_type!=DATA_TYPE_ARGB888 && layer_info.data_type!=DATA_TYPE_RGB565)
        {
            break;
        }
        bk.data_ptr = UILAYER_GetLayerBufferPtr(&(p_info->layer));
        bk.data_type = (layer_info.data_type==DATA_TYPE_ARGB888) ? IMGREF_FORMAT_ARGB888 : IMGREF_FORMAT_RGB565;//only support argb888& rgb565
//        bk.data_type = 1;
        bk.start_x = 0;
        bk.start_y = 0;
        bk.width = rect_width;
        bk.height = rect_height;

        fg.data_ptr = p_rotate_buf;
        fg.data_type = 1;
        fg.start_x = 0;
        fg.start_y = 0;
        fg.width = rect_width;
        fg.height = rect_height;
        ProcessAlphaBlending(&bk, &fg);
    }

    //free rotate buf
    if (PNULL != p_rotate_buf)
    {
        CTRL_WF_TRACE("free p_rotate_buf, addr = 0x%0x.", (uint32)p_rotate_buf);// bug id 1941703 CID 421162
        SCI_FREE(p_rotate_buf);
    }
#endif
}


LOCAL void TimerCB(uint8 timer_id, uint32 param)
{
    WATCHFACE_ANALOG_TIME_DATA_T* p_info = (WATCHFACE_ANALOG_TIME_DATA_T*) param;
    if (PNULL == p_info)
    {
        CTRL_WF_TRACE("param = PNULL!");
        MMK_StopTimer(timer_id);
        timer_id = 0;
        return;
    }
    if (TRUE == MMK_IsFocusWin(p_info->win_id))
    {
        if (0 != p_info->timer_id)
        {
            if(timer_id == p_info->timer_id)
            {

#if defined(UI_MULTILAYER_SUPPORT) && !defined(WF_USE_SINGLELAYER)
                MMK_PostMsg(p_info->ctrl_id, MSG_CTL_PAINT, 0, 0); // update only analog time ctrl
#else
                MMK_PostMsg(p_info->win_id, MSG_FULL_PAINT, PNULL, 0); //update all ctrl
#endif
            }
        }
    }
}


/*****************************************************************************/
//  Description : update by timer
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void UpdateAnalogClock(uint8 timer_id, uint32 param)
{
    WATCHFACE_ANALOG_TIME_DATA_T* p_info = (WATCHFACE_ANALOG_TIME_DATA_T*) param;
    wchar*              p_img_hands[3];
    uint8               i = 0;

    if (PNULL == p_info)
    {
        CTRL_WF_TRACE("param = pnull!");
        MMK_StopTimer(timer_id);
        timer_id = 0;
        return;
    }

    if (TRUE == MMK_IsFocusWin(p_info->win_id))
    {
        if (0 != p_info->timer_id)
        {
            if(timer_id == p_info->timer_id)
            {
                for(i = 0; i < 3; i++)
                {
                    p_img_hands[i] = p_info->img_hands[i];
                }
                DisplayAnalogTime(p_info, (wchar**)p_img_hands);
            }
        }
    }
}



/*****************************************************************************/
//  Description : create data
//  Global resource dependence : none
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL WATCHFACE_ANALOG_TIME_DATA_T* CreateAnalogTimeData(MMI_WIN_ID_T      win_id,
                                                                MMI_CTRL_ID_T   ctrl_id,
                                                                GUI_POINT_T         center_point,
                                                                uint32              update_interval,
                                                                char*               p_image_hour,
                                                                char*               p_image_minute,
                                                                char*               p_image_second,
                                                                float               scale
                                                                )
{
    WATCHFACE_ANALOG_TIME_DATA_T*  p_info = PNULL;
    wchar                   hour_wstr[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN] = {0};
    wchar                   min_wstr[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN] = {0};
    wchar                   sec_wstr[WATCHFACE_IMAGE_FULL_PATH_MAX_LEN] = {0};
    uint8                   hour_wstr_len = 0;
    uint8                   min_wstr_len = 0;
    uint8                   sec_wstr_len = 0;

    //alloc space
    p_info = SCI_ALLOC_APPZ(sizeof(WATCHFACE_ANALOG_TIME_DATA_T));
    if(PNULL == p_info)
    {
        CTRL_WF_TRACE("The pInfo is PNULL");
        return PNULL;
    }
    SCI_MEMSET(p_info, 0, sizeof(WATCHFACE_ANALOG_TIME_DATA_T));

    //store img info
    p_info->win_id = win_id;
    p_info->center_point = center_point;
    p_info->update_interval = update_interval;
    p_info->scale = scale;

    //sec
    if(PNULL != p_image_second)
    {
        MMIAPICOM_StrToWstr((uint8 *)p_image_second, sec_wstr);
        sec_wstr_len = MMIAPICOM_Wstrlen(sec_wstr);
        MMI_WSTRNCPY(p_info->img_hands[2], WATCHFACE_IMAGE_FULL_PATH_MAX_LEN,(const wchar*)sec_wstr, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, sec_wstr_len);
    }
    //min
    if(PNULL != p_image_minute)
    {
        MMIAPICOM_StrToWstr((uint8 *)p_image_minute, min_wstr);
        min_wstr_len = MMIAPICOM_Wstrlen(min_wstr);
        MMI_WSTRNCPY(p_info->img_hands[1], WATCHFACE_IMAGE_FULL_PATH_MAX_LEN,(const wchar*)min_wstr, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, min_wstr_len);
    }
    //hour
    if(PNULL != p_image_hour)
    {
        MMIAPICOM_StrToWstr((uint8 *)p_image_hour, hour_wstr);
        hour_wstr_len = MMIAPICOM_Wstrlen(hour_wstr);
        MMI_WSTRNCPY(p_info->img_hands[0], WATCHFACE_IMAGE_FULL_PATH_MAX_LEN,(const wchar*)hour_wstr, WATCHFACE_IMAGE_FULL_PATH_MAX_LEN, hour_wstr_len);
    }
    return p_info;
}

PUBLIC void ownerdraw_callback(GUIOWNDRAW_INFO_T* owner_draw_ptr)
{
    BOOLEAN res = TRUE;
    ADD_DATA        add_data = 0;
    CTRL_WF_TRACE("Draw. ");
    res = CTRLOWNERDRAW_GetAddData(owner_draw_ptr->ctrl_handle, &add_data);
    if(res != TRUE || add_data==0)
    {
        CTRL_WF_TRACE("param error, ctrl_id:%d, add_data:0x%x", owner_draw_ptr->ctrl_handle, add_data);
        return;
    }
    else
    {
        WATCHFACE_ANALOG_TIME_DATA_T*   p_info = (WATCHFACE_ANALOG_TIME_DATA_T*)add_data;
        CTRL_WF_TRACE("p_info:0x%x at ownerdraw.", p_info);
        if(MMK_IsFocusWin(p_info->win_id))//bug 2143371/2143387
        {
            displayTime(p_info);
        }
        else
        {
            CTRL_WF_TRACE("APP is not focus.");
        }
    }
}


/*****************************************************************************/
//  Description : create watch face analog time ctrl
//  Parameter: [In] win_id
//             [In] center_position: 中心相对坐标
//             [In] update_interval: 刷新间隔/ms
//             [In] image_hour: 时针资源路径
//             [In] image_minute: 分针资源路径
//             [In] image_second: 秒针资源路径
//             [In] scale: 缩放比例
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLWF_AnalogTime_Create (MMI_WIN_ID_T      win_id,
                                                            MMI_CTRL_ID_T   ctrl_id,
                                                            GUI_POINT_T         center_point,
                                                            uint32              update_interval,
                                                            char*               p_image_hour,
                                                            char*               p_image_minute,
                                                            char*               p_image_second,
                                                            float               scale
                                                            )
{
    MMI_HANDLE_T        handle = 0;
    WATCHFACE_ANALOG_TIME_DATA_T*   p_info = {0};
    GUIIMG_INFO_T       img_info = {0};
    wchar*              p_img_hands[3];
    uint8               i = 0;

    //create data
    p_info = CreateAnalogTimeData(win_id, ctrl_id, center_point, update_interval, p_image_hour, p_image_minute, p_image_second, scale);
    if (PNULL == p_info)
    {
        CTRL_WF_TRACE("p_info = pnull!");
        return handle;
    }
    else
    {
        handle = (MMI_HANDLE_T)p_info;
        p_info->handle = handle;
    }

    CTRLWF_Common_GetImgInfoByPath(p_info->img_hands[0], &img_info);

    //modify the img_info temp to get display_rect, not change file info really
    if(img_info.image_width != img_info.image_height)
    {
        img_info.image_width = (img_info.image_width > img_info.image_height) ? img_info.image_width : img_info.image_height;
        img_info.image_height = img_info.image_width;
    }

    //get draw rect
    CTRLWF_Common_GetDisplayRect(p_info->center_point, img_info, &(p_info->rect), p_info->scale);
#if defined(UI_MULTILAYER_SUPPORT) && !defined(WF_USE_SINGLELAYER)
    //create layer
    CTRLWF_Common_CreateLayer(&(p_info->layer), win_id, (p_info->rect.right - p_info->rect.left), (p_info->rect.bottom - p_info->rect.top));
    if (p_info->layer.block_id == UILAYER_NULL_HANDLE)
    {
        CTRL_WF_TRACE("CreateCacheLayer [error].");
        SCI_MEMSET(p_info, 0, sizeof(WATCHFACE_ANALOG_TIME_DATA_T));
        SCI_FREE(p_info);
        p_info = PNULL;
        return 0;
    }
    //set position
    UILAYER_SetLayerPosition(&(p_info->layer), p_info->rect.left, p_info->rect.top);
#else
    MMK_GetWinLcdDevInfo(win_id, &(p_info->layer));
    CTRL_WF_TRACE("  win_id:[%d] layer:[%d]", win_id,  p_info->layer.block_id);
#endif
    {
        MMI_CONTROL_CREATE_T    ownerdraw_create = {0};
        CTRLOWNERDRAW_OBJ_T  * ctrl_ptr = PNULL;
        CTRLOWNERDRAW_INIT_PARAM_T ownerdraw_param;
        ownerdraw_param.base_ctrl_param.both_rect.v_rect = p_info->rect;
        ownerdraw_param.base_ctrl_param.both_rect.h_rect = p_info->rect;
        ownerdraw_param.OwnDrawFunc = ownerdraw_callback;
        //create anim
        MMK_DestroyControl(ctrl_id);
        ownerdraw_create.guid = SPRD_GUI_OWNDRAW_ID;
        ownerdraw_create.ctrl_id = ctrl_id;
        ownerdraw_create.init_data_ptr = &ownerdraw_param;
        ownerdraw_create.parent_win_handle = win_id;
        ctrl_ptr = (CTRLOWNERDRAW_OBJ_T*)MMK_CreateControl(&ownerdraw_create);
        CTRL_WF_TRACE("analogtime ownerdraw ctrl create.");
        if (PNULL != ctrl_ptr)
        {
            //set control info
            CTRLOWNERDRAW_SetRect(ctrl_id, &(p_info->rect));
            GUIAPICTRL_SetLcdDevInfo(ctrl_id, &(p_info->layer));
            CTRLOWNERDRAW_SetAddData(ctrl_id, p_info);
            CTRL_WF_TRACE("p_info:0x%x at create.", p_info);
            p_info->ctrl_id = ctrl_id;
        }
    }
//    //display clock
//    for(i = 0; i < 3; i++)
//    {
//        p_img_hands[i] = p_info->img_hands[i];
//    }
//    DisplayAnalogTime(p_info, p_img_hands);
    displayTime(p_info);
    //create timer
    if (0 != p_info->timer_id)
    {
        MMK_StopTimer(p_info->timer_id);
        p_info->timer_id = 0;
    }
    p_info->timer_id = MMK_CreateTimerCallback(update_interval, TimerCB, (WATCHFACE_ANALOG_TIME_DATA_T*)p_info, TRUE);
    CTRL_WF_TRACE("p_info:0x%x at create timer.", p_info);
//    p_info->timer_id = MMK_CreateTimerCallback(update_interval, UpdateAnalogClock, (WATCHFACE_ANALOG_TIME_DATA_T*)p_info, TRUE);

    return handle;

}


/*****************************************************************************/
//  Description : destory watch face analog time ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_AnalogTime_Destory (MMI_HANDLE_T handle)
{
    BOOLEAN     result = FALSE;
    WATCHFACE_ANALOG_TIME_DATA_T*  p_info = PNULL;

    CTRL_WF_TRACE("destory enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_ANALOG_TIME_DATA_T*)handle;

        if(handle == p_info->handle)
        {
            //stop timer
            MMK_StopTimer(p_info->timer_id);
            p_info->timer_id = 0;
            MMK_DestroyControl(p_info->ctrl_id);
#ifdef UI_MULTILAYER_SUPPORT
            //clear layer
            UILAYER_Clear(&(p_info->layer));
            //release cache layer
            UILAYER_RELEASELAYER(&(p_info->layer));
#endif
            //free space
            SCI_MEMSET(p_info, 0, sizeof(WATCHFACE_ANALOG_TIME_DATA_T));
            SCI_FREE(p_info);
            p_info = PNULL;
            result = TRUE;
        }
    }

    CTRL_WF_TRACE("destory exit, result = %d", result);
    return result;
}
/*****************************************************************************/
//  Description : stop timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:用于lose focus时，停止控件内timer
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_AnalogTime_StopTimer (MMI_HANDLE_T handle)
{
    BOOLEAN     result = FALSE;
    WATCHFACE_ANALOG_TIME_DATA_T*  p_info = PNULL;

    CTRL_WF_TRACE("stop timer enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_ANALOG_TIME_DATA_T*)handle;
        if(PNULL == p_info)
        {
            CTRL_WF_TRACE("p_info is pnull");
            return FALSE;
        }
        if (handle == p_info->handle)
        {
            if(0 != p_info->timer_id)
            {
                MMK_StopTimer(p_info->timer_id);
                p_info->timer_id = 0;
                result = TRUE;
            }
        }
    }
    CTRL_WF_TRACE("stop timer exit, result = %d", result);
    return result;

}
/*****************************************************************************/
//  Description : restart timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_AnalogTime_RestartTimer (MMI_HANDLE_T handle)
{
    WATCHFACE_ANALOG_TIME_DATA_T*   p_info = PNULL;
    wchar*              p_img_hands[3] = {0};
    uint8               i = 0;

    CTRL_WF_TRACE("update enter, handle = 0x%x", handle);
    if (0 != handle)
    {
        p_info = (WATCHFACE_ANALOG_TIME_DATA_T*)handle;
        if(PNULL == p_info)
        {
            CTRL_WF_TRACE("p_info is pnull");
            return;
        }
        if (handle == p_info->handle)
        {
            if (TRUE == MMK_IsFocusWin(p_info->win_id))
            {
                if (0 == p_info->timer_id)
                {
//                    p_info->timer_id = MMK_CreateTimerCallback(p_info->update_interval, UpdateAnalogClock, (WATCHFACE_ANALOG_TIME_DATA_T*)p_info, TRUE);
                    p_info->timer_id = MMK_CreateTimerCallback(p_info->update_interval, TimerCB, (WATCHFACE_ANALOG_TIME_DATA_T*)p_info, TRUE);
                }
                else
                {
                    CTRL_WF_TRACE("timer_id != 0");
                }
            }
        }
    }
}



