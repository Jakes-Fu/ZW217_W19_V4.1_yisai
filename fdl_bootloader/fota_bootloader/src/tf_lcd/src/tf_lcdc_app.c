/***************************************************************************************
** File Name:      lcdc_app.c                                                          *
** DATE:           06/01/2009                                                          *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.                   *
** Description:                                                                        *
****************************************************************************************/

/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/01/2009    Tim.zhu   Create.                                     *
 ******************************************************************************/

/**------------------------------------------------------------------------------------*
 **                         Dependencies                                               *
 **------------------------------------------------------------------------------------*/
#include "fdl_bootloader_trc.h"
#include "lcdc_drv.h"
#include "lcdc_app.h"
#include "sc_reg.h"
//#include "pwm_drvapi.h"
//#include "boot_drvapi.h"
#include "task_monitor.h"

/**------------------------------------------------------------------------------------*
 **                         Compiler Flag                                              *
 **------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**------------------------------------------------------------------------------------*
 **                         struct define                                              *
 **------------------------------------------------------------------------------------*/
#if (defined(PLATFORM_SC6800H) ||defined(PLATFORM_SC6530))
#define LCDC_OSD_LAYER_NUM            3
#define LCDC_LAYER_NUM                4
#else
#define LCDC_OSD_LAYER_NUM 	          5
#define LCDC_LAYER_NUM		          6
#endif
typedef struct lcdc_config_info_tag
{
    LCD_IMG_LAYER_PARAM_T       img_layer;
    LCD_OSD_LAYER_PARAM_T       osd_layer[LCDC_OSD_LAYER_NUM];
    uint32                      layer_pitch[LCDC_LAYER_NUM];        //layer_pitch[0] for image layer, layer_pitch[1] for OSD1 layer, etc.
    LCD_YUV2RGB_PARAM_T         yuv2rgb;
    LCDC_CAP_PARAM_T            cap_param;
    uint32                      cap_pitch;
    uint32                      bg_color;    
    LCDC_SIZE_T                 lcdc_logic_size;
    LCD_FMARK_MODE_E            fmark_mode[2];
    LCD_FMARK_POL_E             fmark_pol[2];
    LCDC_RECT_T                 display_rect;   //display rect of lcdc logic plane
    LCDC_RECT_T                 refresh_rect; //refresh rect of lcd device    
    uint32                      ahb_req_gap;
    uint32                      hi_addr;
    BOOLEAN                     dither_en;
    BOOLEAN                     is_init;
    BOOLEAN                     is_check_done_state;
    uint8                       padding;//reserved
    uint32                      expand_mode;
} LCDC_CONFIG_INFO_T;

LOCAL LCD_IRQ_FUNC s_lcdc_int_callback[LCD_INT_MAX] = {0};
/**------------------------------------------------------------------------------------*
 **                         Macro define                                               *
 **------------------------------------------------------------------------------------*/
#define LCDC_SHIFT_BIT1     0x01
#define LCDC_SHIFT_BIT2     0x02
#define LCDC_SHIFT_BIT3     0x03
#define LCDC_SHIFT_BIT4     0x04
#define LCDC_SHIFT_BIT5     0x05
#define LCDC_SHIFT_BIT6     0x06
#define LCDC_SHIFT_BIT7     0x07
#define LCDC_SHIFT_BIT8     0x08

#define LCDC_ALGIN_1        0x00
#define LCDC_ALGIN_2        0x01
#define LCDC_ALGIN_4        0x03
#define LCDC_ALGIN_8        0x07
#define LCDC_ALGIN_16       0x0f
/**------------------------------------------------------------------------------------*
 **                         local variables define                                             *
 **------------------------------------------------------------------------------------*/
LOCAL LCDC_CONFIG_INFO_T    s_lcdc_cfg       = {0};
//LOCAL LCD_IRQ_FUNC          s_irq_async_func = {0};

//LOCAL uint32                s_lcdc_run_time  = 0;

#ifdef PLATFORM_SC8800G
LOCAL uint32 s_record_cs_pin[2] = {0};
#endif
/**------------------------------------------------------------------------------------*
 **                         local functions                                                                        *
 **------------------------------------------------------------------------------------*/
/****************************************************************************************/
// Description: The interrupt handle of lcdc complete display or capture
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************************/
LOCAL void _LCDC_ISR0_LcdcDone (void)
{
    LCD_IRQ_FUNC callback = s_lcdc_int_callback[LCD_INT_LCDC_DONE];

//  SCI_TRACE_LOW("lcd time = %d",(SCI_GetTickCount()-s_lcdc_run_time));

    if (callback)
    {
        callback();
    }
}
/****************************************************************************************/
// Description: The interrupt handle of NLCM complete display
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************************/
LOCAL void _LCDC_ISR1_LcmEof (void)
{
    //SCI_TraceLow:"LCDC_APP:_LCDC_ISR1_LcmEof\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_APP_109_112_2_18_1_1_39_23,(uint8*)"");
}
/****************************************************************************************/
// Description: The interrupt handle of NLCM begin display
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************************/
LOCAL void _LCDC_ISR2_LcmSof (void)
{
    //SCI_TraceLow:"LCDC_APP:_LCDC_ISR2_LcmSof\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_APP_116_112_2_18_1_1_39_24,(uint8*)"");
}
/****************************************************************************************/
// Description: The interrupt handle of detect lcd fmark
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************************/
LOCAL void _LCDC_ISR3_Fmark (void)
{
    LCDC_DrvCloseInt (LCD_INT_FMARK);
    LCDC_DrvSetFmark(LCD_FMARK_NONE,LCD_FMARK_HIGH);
    
    LCDC_DrvRun();
    //SCI_TraceLow:"LCDC_APP:_LCDC_ISR3_Fmark\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_APP_127_112_2_18_1_1_39_25,(uint8*)"");
}
/****************************************************************************************/
// Description: lcdc module init
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************************/
LOCAL BOOLEAN _LCDC_GetDataCoeffParam (LCD_DATA_FORMAT_E data_format, 
                                                 uint32 *data_coeff)
{
    uint32 coeff=0x00;

    switch (data_format)
    {
        case LCD_YUV422:
        case LCD_YUV420:
        case LCD_YUV400:
        case LCD_GREY:            
            coeff=LCDC_ONE;
            break;            
        case LCD_RGB888:
        case LCD_RGB666:           
            coeff=LCDC_FOUR;
            break;            
        case LCD_RGB565:
        case LCD_RGB555:            
            coeff=LCDC_TWO;
            break;        
        default:
            //SCI_TRACE_LOW:"[_LCDC_GetDataCoeffParam ERROR],LCDC:img data format err format:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_APP_154_112_2_18_1_1_39_26,(uint8*)"d", data_format);
            break;
    }
    *data_coeff=coeff;

    return SCI_TRUE;
}
/****************************************************************************************/
// Description: lcdc module init
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************************/
LOCAL BOOLEAN _LCDC_GetAlginParam (LCD_DATA_FORMAT_E data_format, 
                                           uint32 *width_algin, 
                                           uint32 *height_algin)
{
    uint32 l_width_algin=0x00;
    uint32 l_height_algin=0x00;

    switch (data_format)
    {
        case LCD_YUV422:
        case LCD_YUV400:
        case LCD_GREY:            
            l_width_algin=LCDC_ALGIN_4;
            l_height_algin=LCDC_ALGIN_1;
            break;            
        case LCD_YUV420:            
            l_width_algin=LCDC_ALGIN_8;
            l_height_algin=LCDC_ALGIN_2;
            break;            
        case LCD_RGB888:
        case LCD_RGB666:            
            l_width_algin=LCDC_ALGIN_1;
            l_height_algin=LCDC_ALGIN_1;
            break;            
        case LCD_RGB565:
        case LCD_RGB555:            
            l_width_algin=LCDC_ALGIN_2;
            l_height_algin=LCDC_ALGIN_1;
            break;          
        default:
            //SCI_TRACE_LOW:"[_LCDC_GetAlginParam ERROR],LCDC:img data format err format:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_APP_194_112_2_18_1_1_39_27,(uint8*)"d", data_format);
            break;
    }

    *width_algin=l_width_algin;
    *height_algin=l_height_algin;

    return SCI_TRUE;
}
/****************************************************************************************/
// Description: lcdc module init
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************************/
LOCAL uint32 _LCDC_CheckTopParam()
{
#ifdef PLATFORM_SC6800H
    uint32                  hi_addr = 0;
    uint32                  i = 0;
    LCD_OSD_LAYER_PARAM_T   *layer_ptr    = NULL;
    BOOLEAN                 is_hi_addr_en = 0;
#endif    

    if ( (LCDC_ZERO==s_lcdc_cfg.lcdc_logic_size.w)
            || (LCDC_ZERO==s_lcdc_cfg.lcdc_logic_size.h)
            || (LCDC_DrvGetMaxWidth() <s_lcdc_cfg.lcdc_logic_size.w)
            || (LCDC_DrvGetMaxHeight() <s_lcdc_cfg.lcdc_logic_size.h))

    {
        SCI_PASSERT (0, ("LCDC:lcdc logic size err  width:%d, height:%d",    /*assert verified*/
                         s_lcdc_cfg.lcdc_logic_size.w, s_lcdc_cfg.lcdc_logic_size.h));
    }
#ifdef PLATFORM_SC6800H
    if (s_lcdc_cfg.img_layer.layer_en)
    {
        hi_addr = (s_lcdc_cfg.img_layer.src_base_addr.y_addr >> 26);

        if ( (LCD_YUV420 == s_lcdc_cfg.img_layer.format) || (LCD_YUV422 == s_lcdc_cfg.img_layer.format))
        {
            if (hi_addr != (s_lcdc_cfg.img_layer.src_base_addr.uv_addr >> 26))
            {
                SCI_PASSERT (0, ("LCDC: img layer address error, y = 0x%x, uv = 0x%x",     /*assert verified*/
                                 s_lcdc_cfg.img_layer.src_base_addr.y_addr,
                                 s_lcdc_cfg.img_layer.src_base_addr.uv_addr));
            }
        }

        is_hi_addr_en = TRUE;
    }

    for (i=0; i<LCDC_OSD_LAYER_NUM; i++)
    {
        layer_ptr = &s_lcdc_cfg.osd_layer[i];

        if (layer_ptr->layer_en)
        {
            if (is_hi_addr_en)
            {
                SCI_PASSERT (hi_addr == (layer_ptr->src_base_addr >> 26),        /*assert verified*/
                             ("LCDC: osd layer address error, hi_addr = 0x%x, osd_addr = 0x%x",
                              hi_addr,
                              layer_ptr->src_base_addr));
            }

            is_hi_addr_en = TRUE;
            hi_addr = (layer_ptr->src_base_addr >> 26);
        }
    }

    s_lcdc_cfg.hi_addr = hi_addr;

    SCI_PASSERT (0 == (hi_addr & 0XF0), ("LCDC: high address error, hi_addr = 0x%x",    /*assert verified*/
                                         hi_addr));
#endif
    return SCI_SUCCESS;
}
/****************************************************************************************/
// Description: lcdc module init
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************************/
LOCAL uint32 _LCDC_CheckRefreshParam()
{
    if (s_lcdc_cfg.cap_param.cap_en)
    {
        return SCI_SUCCESS;
    }

    if ( (s_lcdc_cfg.display_rect.w != s_lcdc_cfg.refresh_rect.w)
            || (s_lcdc_cfg.display_rect.h != s_lcdc_cfg.refresh_rect.h)
            || ( (s_lcdc_cfg.display_rect.x + s_lcdc_cfg.display_rect.w) > s_lcdc_cfg.lcdc_logic_size.w)
            || ( (s_lcdc_cfg.display_rect.y + s_lcdc_cfg.display_rect.h) > s_lcdc_cfg.lcdc_logic_size.h))
    {
        SCI_PASSERT (0, ("LCDC:trim rect error, x: %d, y : %d, w: %d, h: %d",      /*assert verified*/
                         s_lcdc_cfg.display_rect.x, s_lcdc_cfg.display_rect.y,
                         s_lcdc_cfg.display_rect.w, s_lcdc_cfg.display_rect.h));
    }

    return SCI_SUCCESS;
}
/****************************************************************************************/
// Description: lcdc module init
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************************/
LOCAL uint32 _LCDC_CheckImageLayerParam()
{
    LCD_IMG_LAYER_PARAM_T   *layer_ptr          = &s_lcdc_cfg.img_layer;
    uint32                  width_align_coef    = 0;
    uint32                  height_align_coef   = 0;   

    if (!layer_ptr->layer_en)
    {
        return SCI_SUCCESS;
    }

    SCI_PASSERT (layer_ptr->src_base_addr.y_addr, ("src y address is null"));/*assert verified*/
    SCI_PASSERT (0 == (layer_ptr->src_base_addr.y_addr & 0x3), ("y address is not word aligned"));/*assert verified*/

    if (LCD_YUV422 == layer_ptr->format || LCD_YUV420 == layer_ptr->format)
    {
        SCI_PASSERT (layer_ptr->src_base_addr.uv_addr, ("src y address is null"));/*assert verified*/
        SCI_PASSERT (0 == (layer_ptr->src_base_addr.uv_addr & 0x3), ("u address is not word aligned"));/*assert verified*/
    }

    if (! ( (LCD_YUV422==layer_ptr->format)
            || (LCD_YUV420==layer_ptr->format)
            || (LCD_YUV400==layer_ptr->format)
            || (LCD_RGB888==layer_ptr->format)
            || (LCD_RGB666==layer_ptr->format)
            || (LCD_RGB565==layer_ptr->format)
            || (LCD_RGB555==layer_ptr->format)))
    {
        SCI_PASSERT (0, ("LCDC:img data format err format:%d", layer_ptr->format));/*assert verified*/
    }

    _LCDC_GetAlginParam (layer_ptr->format, &width_align_coef, &height_align_coef);

    if ( (LCDC_ZERO!= (layer_ptr->src_size.w&width_align_coef))
            || (LCDC_ZERO==layer_ptr->src_size.w)
            || (LCDC_ZERO==layer_ptr->src_size.h))
    {
        SCI_PASSERT (0, ("LCDC:img src size not algin data_format:%d, width:%d, height:%d",   /*assert verified*/
                         layer_ptr->format, layer_ptr->src_size.w, layer_ptr->src_size.h));
    }

    if ( (LCDC_ZERO!= (layer_ptr->src_trim_rect.x&width_align_coef))
            || (LCDC_ZERO!= (layer_ptr->src_trim_rect.y&height_align_coef))
            || (LCDC_ZERO!= (layer_ptr->src_trim_rect.w&width_align_coef))
            || (LCDC_ZERO!= (layer_ptr->src_trim_rect.h&height_align_coef))
            || (LCDC_ZERO==layer_ptr->src_trim_rect.w)
            || (LCDC_ZERO==layer_ptr->src_trim_rect.h)
            || (layer_ptr->src_size.w< (layer_ptr->src_trim_rect.x+layer_ptr->src_trim_rect.w))
            || (layer_ptr->src_size.h< (layer_ptr->src_trim_rect.y+layer_ptr->src_trim_rect.h)))
    {
        SCI_PASSERT (0, ("LCDC:img rect size err data_format:%d, x:%d, y:%d, width:%d, height:%d",    /*assert verified*/
                         layer_ptr->format, layer_ptr->src_trim_rect.x, layer_ptr->src_trim_rect.y,
                         layer_ptr->src_trim_rect.w, layer_ptr->src_trim_rect.h));
    }

    if ( (LCDC_ZERO!= (layer_ptr->disp_pos.x&width_align_coef))
            || (LCDC_ZERO!= (layer_ptr->disp_pos.y&height_align_coef)))
    {
        SCI_PASSERT (0, ("LCDC:img postion not algin data_format:%d, x:%d, y:%d",          /*assert verified*/
                         layer_ptr->format, layer_ptr->disp_pos.x, layer_ptr->disp_pos.y));
    }

    if ( (LCDC_DrvGetMaxWidth() < (layer_ptr->disp_pos.x+layer_ptr->src_trim_rect.w))
            || (LCDC_DrvGetMaxHeight() < (layer_ptr->disp_pos.y+layer_ptr->src_trim_rect.h)))
    {
        SCI_PASSERT (0, ("LCDC:img size err x:%d, y:%d, w:%d, h:%d", layer_ptr->disp_pos.x,     /*assert verified*/
                         layer_ptr->disp_pos.y, layer_ptr->src_trim_rect.w, layer_ptr->src_trim_rect.h));
    }

    return SCI_SUCCESS;
}
/****************************************************************************************/
// Description: lcdc module init
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************************/
LOCAL uint32 _LCDC_CheckOSDLayerParam (LCD_LAYER_ID_E layer_id)
{
    LCD_OSD_LAYER_PARAM_T   *layer_ptr          = NULL;
    uint32                  lcdc_width          = s_lcdc_cfg.lcdc_logic_size.w;
    uint32                  lcdc_height         = s_lcdc_cfg.lcdc_logic_size.h;
    uint32                  width_align_coef    = 0;
    uint32                  height_align_coef   = 0;

    layer_ptr = &s_lcdc_cfg.osd_layer[ (uint32) layer_id - 1];

    if (!layer_ptr->layer_en)
    {
        return SCI_SUCCESS;
    }

    if (! ( (LCD_RGB888==layer_ptr->format)
            || (LCD_RGB666==layer_ptr->format)
            || (LCD_RGB565==layer_ptr->format)
            || (LCD_RGB555==layer_ptr->format)
            || (LCD_GREY==layer_ptr->format)))
    {
        SCI_PASSERT (0, ("LCDC:osd1 data format err format:%d", layer_ptr->format));/*assert verified*/
    }

    _LCDC_GetAlginParam (layer_ptr->format, &width_align_coef, &height_align_coef);

    if ( (LCDC_ZERO!= (layer_ptr->src_size.w&width_align_coef))
            || (LCDC_ZERO!= (layer_ptr->src_size.h&height_align_coef))
            || (LCDC_ZERO==layer_ptr->src_size.w)
            || (LCDC_ZERO==layer_ptr->src_size.h))
    {
        SCI_PASSERT (0, ("LCDC:osd1 src size not algin data_format:%d, width:%d, height:%d",layer_ptr->format, layer_ptr->src_size.w, layer_ptr->src_size.h));/*assert verified*/
    }

    if ( (LCDC_ZERO!= (layer_ptr->src_trim_rect.x&width_align_coef))
            || (LCDC_ZERO!= (layer_ptr->src_trim_rect.y&height_align_coef))
            || (LCDC_ZERO!= (layer_ptr->src_trim_rect.w&width_align_coef))
            || (LCDC_ZERO!= (layer_ptr->src_trim_rect.h&height_align_coef))
            || (LCDC_ZERO==layer_ptr->src_trim_rect.w)
            || (LCDC_ZERO==layer_ptr->src_trim_rect.h)
            || (layer_ptr->src_size.w< (layer_ptr->src_trim_rect.x+layer_ptr->src_trim_rect.w))
            || (layer_ptr->src_size.h< (layer_ptr->src_trim_rect.y+layer_ptr->src_trim_rect.h)))
    {
        SCI_PASSERT (0, ("LCDC:osd1 rect size err data_format:%d, x:%d, y:%d, width:%d, height:%d",  /*assert verified*/
                         layer_ptr->format, layer_ptr->src_trim_rect.x, layer_ptr->src_trim_rect.y,
                         layer_ptr->src_trim_rect.w, layer_ptr->src_trim_rect.h));
    }

    if ( (LCDC_ZERO!= (layer_ptr->disp_pos.x&width_align_coef))
            || (LCDC_ZERO!= (layer_ptr->disp_pos.y&height_align_coef)))
    {
        SCI_PASSERT (0, ("LCDC:osd1 postion not algin data_format:%d, x:%d, y:%d",  /*assert verified*/
                         layer_ptr->format, layer_ptr->disp_pos.x, layer_ptr->disp_pos.y));
    }

    if ( (lcdc_width< (layer_ptr->disp_pos.x+layer_ptr->src_trim_rect.w))
            || (lcdc_height< (layer_ptr->disp_pos.y+layer_ptr->src_trim_rect.h)))
    {
        SCI_PASSERT (0, ("LCDC:osd1 size err x:%d, y:%d, w:%d, h:%d", /*assert verified*/
                         layer_ptr->disp_pos.x, layer_ptr->disp_pos.y, layer_ptr->src_trim_rect.w,
                         layer_ptr->src_trim_rect.h));
    }

    if (LCDC_ZERO != (layer_ptr->src_base_addr & LCDC_ALGIN_4))
    {
        SCI_PASSERT (0, ("src base addr is not word-aligned, src addr = 0x%x",  /*assert verified*/
                         layer_ptr->src_base_addr));
    }

    if ( (LCD_LAYER_OSD1 == layer_id) && (LCD_RGB565 == layer_ptr->format)
            && (LCD_OSD_PIXEL_ALPHA == layer_ptr->alpha_mode))
    {
        SCI_PASSERT (layer_ptr->alpha_base_addr, ("alpha base address is invalid"));/*assert verified*/
        SCI_PASSERT (LCDC_ZERO == (layer_ptr->alpha_base_addr & LCDC_ALGIN_4),  /*assert verified*/
                     ("alpha base address is not word aligned, addr = 0x%x", layer_ptr->alpha_base_addr));
    }

    return SCI_SUCCESS;

}
/****************************************************************************************/
// Description: lcdc module init
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************************/
LOCAL uint32 _LCDC_CheckCapParam()
{
    LCDC_CAP_PARAM_T    *cap_ptr          = &s_lcdc_cfg.cap_param;
    uint32              width_align_coef  = 0;
    uint32              height_align_coef = 0;    

    if (!cap_ptr->cap_en)
    {
        return SCI_SUCCESS;
    }

    if (! ( (LCD_RGB888==cap_ptr->format)
            || (LCD_RGB666==cap_ptr->format)
            || (LCD_RGB565==cap_ptr->format)))
    {
        SCI_PASSERT (0, ("LCDC:cap data format err format:%d", cap_ptr->format));/*assert verified*/
    }

    _LCDC_GetAlginParam (cap_ptr->format, &width_align_coef, &height_align_coef);

    if ( (LCDC_ZERO!= (cap_ptr->dst_trim_rect.x&width_align_coef))
            || (LCDC_ZERO!= (cap_ptr->dst_trim_rect.y&height_align_coef))
            || (LCDC_ZERO!= (cap_ptr->dst_trim_rect.w&width_align_coef))
            || (LCDC_ZERO!= (cap_ptr->dst_trim_rect.h&height_align_coef))
            || (LCDC_ZERO==cap_ptr->dst_trim_rect.w)
            || (LCDC_ZERO==cap_ptr->dst_trim_rect.h)
            || (cap_ptr->dst_size.w< (cap_ptr->dst_trim_rect.x+cap_ptr->dst_trim_rect.w))
            || (cap_ptr->dst_size.h< (cap_ptr->dst_trim_rect.y+cap_ptr->dst_trim_rect.h)))
    {
        SCI_PASSERT (0, ("LCDC:capture rect size err data_format:%d, x:%d, y:%d, width:%d, height:%d",  /*assert verified*/
                         cap_ptr->format, cap_ptr->dst_trim_rect.x, cap_ptr->dst_trim_rect.y,
                         cap_ptr->dst_trim_rect.w, cap_ptr->dst_trim_rect.h));/*assert verified*/
    }

    if ( (LCDC_ZERO!= (cap_ptr->cap_rect.x&width_align_coef))
            || (LCDC_ZERO!= (cap_ptr->cap_rect.y&height_align_coef))
            || (cap_ptr->cap_rect.w != cap_ptr->dst_trim_rect.w)
            || (cap_ptr->cap_rect.h != cap_ptr->dst_trim_rect.h))
    {
        SCI_PASSERT (0, ("LCDC:capture rect error, x:%d, y:%d, width:%d, height:%d",  /*assert verified*/
                         cap_ptr->cap_rect.x, cap_ptr->cap_rect.y, cap_ptr->cap_rect.w,
                         cap_ptr->cap_rect.h));/*assert verified*/
    }

    if (LCDC_ZERO != (cap_ptr->dst_base_addr & LCDC_ALGIN_4))
    {
        SCI_PASSERT (0, ("dst base addr is not word-aligned, dst addr = 0x%x",  /*assert verified*/
                         cap_ptr->dst_base_addr));/*assert verified*/
    }

    return SCI_SUCCESS;
}
/****************************************************************************************/
// Description: check lcdc param
// Global resource dependence:
// Author: Shan.he
/*****************************************************************************************/
LOCAL void _LCDC_CheckParam()
{
    uint32 i = 0;    
    
    _LCDC_CheckTopParam();
    _LCDC_CheckImageLayerParam();
    
#if (defined(PLATFORM_SC6800H) || defined(PLATFORM_SC6530))
    for (i=LCD_LAYER_OSD1; i<=LCD_LAYER_OSD3; i++)
#else
	for (i=LCD_LAYER_OSD1; i<=LCD_LAYER_OSD5; i++)
#endif
    {
        _LCDC_CheckOSDLayerParam ( (LCD_LAYER_ID_E) i);
    }

    _LCDC_CheckCapParam();
    _LCDC_CheckRefreshParam();
}
/****************************************************************************************/
// Description: set image layer parameters to the lcdc controller
// Global resource dependence:
// Author: Shan.he
/*****************************************************************************************/
LOCAL void _LCDC_SetImageLayerParam()
{
    LCD_IMG_LAYER_PARAM_T   *layer_ptr = & (s_lcdc_cfg.img_layer);   
    LCDC_RECT_T             disp_rect  = {0};
    LCD_LAYER_ID_E          layer_id   = LCD_LAYER_IMAGE;

    LCDC_DrvSetLayerBlockEnable (layer_id, layer_ptr->layer_en);

    if (!layer_ptr->layer_en)
    {
        return;
    }

    disp_rect.x = layer_ptr->disp_pos.x;
    disp_rect.y = layer_ptr->disp_pos.y;
    disp_rect.w = layer_ptr->src_trim_rect.w;
    disp_rect.h = layer_ptr->src_trim_rect.h;

    LCDC_DrvSetDataFormat (layer_id, layer_ptr->format);
    LCDC_DrvSetEndian (layer_id, layer_ptr->y_endian_switch,layer_ptr->uv_endian_switch);
    LCDC_DrvSetAhbMasterAddr (layer_id, layer_ptr->src_base_addr.y_addr,
                              layer_ptr->src_base_addr.uv_addr);
    LCDC_DrvSetLayerRect (layer_id, &disp_rect);
    LCDC_DrvSetPitch (layer_id, s_lcdc_cfg.layer_pitch[layer_id]);

}
/****************************************************************************************/
// Description: set osd1 layer parameters to the lcdc controller
// Global resource dependence:
// Author: Shan.he
/*****************************************************************************************/
LOCAL void _LCDC_SetOSDLayerParam (LCD_LAYER_ID_E layer_id)
{
    LCD_OSD_LAYER_PARAM_T   *layer_ptr = NULL;
    LCDC_RECT_T             disp_rect  = {0};
    uint32                  ck         = 0;
    uint32                  tmp_val_0  = 0;
    uint32                  tmp_val_1  = 0;
    
    layer_ptr = &s_lcdc_cfg.osd_layer[ (uint32) layer_id - 1];

    LCDC_DrvSetLayerBlockEnable (layer_id, layer_ptr->layer_en);

    if (!layer_ptr->layer_en)
    {
        return;
    }

    disp_rect.x = layer_ptr->disp_pos.x;
    disp_rect.y = layer_ptr->disp_pos.y;
    disp_rect.w = layer_ptr->src_trim_rect.w;
    disp_rect.h = layer_ptr->src_trim_rect.h;

    LCDC_DrvSetColorKeyEnable (layer_id, layer_ptr->colorkey_en);
#ifdef CHIP_VER_6531EFM
    if (layer_ptr->format == LCD_RGB565)
    {
        LCDC_DrvSetAlphaMode (layer_id, LCD_OSD_BLOCK_ALPHA);
    }
    else 
    {
        LCDC_DrvSetAlphaMode (layer_id, layer_ptr->alpha_mode);
    }
#else //CHIP_VER_6531EFM
    LCDC_DrvSetAlphaMode (layer_id, layer_ptr->alpha_mode);
#endif //CHIP_VER_6531EFM
    LCDC_DrvSetDataFormat (layer_id, layer_ptr->format);
    LCDC_DrvSetEndian (layer_id, layer_ptr->endian_switch, NULL);
    LCDC_DrvSetAhbMasterAddr (layer_id, layer_ptr->src_base_addr, NULL);
    LCDC_DrvSetLayerRect (layer_id, &disp_rect);
    LCDC_DrvSetAlphaValue (layer_id, layer_ptr->alpha);
    LCDC_DrvSetGreyRgb (layer_id, layer_ptr->grey_rgb);
    LCDC_DrvSetPitch (layer_id, s_lcdc_cfg.layer_pitch[layer_id]);

    if (LCD_LAYER_OSD1 == layer_id)
    {
        LCDC_DrvSetAlphaBaseAddr (layer_id, layer_ptr->alpha_base_addr);
    }
    if((layer_ptr->format==LCD_RGB565)||(layer_ptr->format==LCD_RGB555))
    {
        ck = layer_ptr->colorkey;
       // layer_ptr->colorkey = (((ck&0x1F)<<3)|(((ck&0x7e0)>>5)<<10)|(((ck&0xF800)>>11)<<19));
        tmp_val_0 = (((ck&0xF800)>>11)<<19);
        tmp_val_1 = ((ck&0xF800)>>11)&0x1;
        tmp_val_1 = tmp_val_1 | (tmp_val_1<<1)| (tmp_val_1<<2);        
        tmp_val_0 |= (tmp_val_1<<16);

        tmp_val_0 |= (((ck&0x7e0)>>5)<<10);
        tmp_val_1 = ((ck&0x7e0)>>5)&0x1;
        tmp_val_1 = tmp_val_1 | (tmp_val_1<<1);        
        tmp_val_0 |= (tmp_val_1<<8);
        
        tmp_val_0 |= (ck&0x1F)<<3;
        tmp_val_1 = (ck&0x1);
        tmp_val_1 = tmp_val_1 | (tmp_val_1<<1) | (tmp_val_1<<2);
        tmp_val_0 |= tmp_val_1;
        layer_ptr->colorkey = tmp_val_0;
        
    }
    LCDC_DrvSetColorkey (layer_id, layer_ptr->colorkey);
}
/****************************************************************************************/
// Description: set capture parameters to the lcdc controller
// Global resource dependence:
// Author: Shan.he
/*****************************************************************************************/
LOCAL void _LCDC_SetCaptureParam(uint32 lcd_id)
{
    LCDC_CAP_PARAM_T    *cap_ptr = & (s_lcdc_cfg.cap_param); 

    LCDC_DrvSetCapEnable (cap_ptr->cap_en);

    if (!cap_ptr->cap_en)
    {
        return;
    }

    LCDC_DrvSetCapDataFormat (cap_ptr->format);
    LCDC_DrvSetCapDataEndian (cap_ptr->endian_switch);
    LCDC_DrvSetCapAddr (cap_ptr->dst_base_addr);

    LCDC_DrvSetCapRect (&cap_ptr->cap_rect);
    LCDC_DrvSetCapPitch (s_lcdc_cfg.cap_pitch);
}
/****************************************************************************************/
// Description: set top parameters to the lcdc controller
// Global resource dependence:
// Author: Shan.he
/*****************************************************************************************/
LOCAL void _LCDC_SetTopParam(uint32 lcd_id)
{
    
    LCDC_DrvSetBGColor (s_lcdc_cfg.bg_color);
    LCDC_DrvSetDitherEnable (s_lcdc_cfg.dither_en);
    if(!s_lcdc_cfg.cap_param.cap_en)
    {
        LCDC_DrvSetFmark (s_lcdc_cfg.fmark_mode[lcd_id], s_lcdc_cfg.fmark_pol[lcd_id]);
    }
    else
    {
        LCDC_DrvSetFmark(LCD_FMARK_NONE,s_lcdc_cfg.fmark_pol[lcd_id]);
    }
    if((s_lcdc_cfg.lcdc_logic_size.w<s_lcdc_cfg.display_rect.w) \
        || (s_lcdc_cfg.lcdc_logic_size.h<s_lcdc_cfg.display_rect.h))
    {
        SCI_PASSERT(0,("display size is less than lcm size!"));/*assert verified*/
    }
    LCDC_DrvSetLCDCSize (&s_lcdc_cfg.lcdc_logic_size);
    LCDC_DrvSetLcmRect (&s_lcdc_cfg.display_rect);

    //set yuv to rgb parameters
    LCDC_DrvSetY2RBrightness (s_lcdc_cfg.yuv2rgb.brightness);
    LCDC_DrvSetY2RContrast (s_lcdc_cfg.yuv2rgb.contrast);
    LCDC_DrvSetY2RSaturation (s_lcdc_cfg.yuv2rgb.saturation);
    LCDC_DrvSetY2RUpsampleMode (s_lcdc_cfg.yuv2rgb.upsample_mode);
    LCDC_DrvSetAhbReqestGap (s_lcdc_cfg.ahb_req_gap);
#ifdef PLATFORM_SC6800H	
    LCDC_DrvSetAhbHiAddr (s_lcdc_cfg.hi_addr);
#endif	
}
/****************************************************************************************/
// Description: set lcdc param
// Global resource dependence:
// Author: Shan.he
/*****************************************************************************************/
LOCAL uint32 _LCDC_SetParam(uint32 lcd_id)
{
    uint32 i = 0;
    //check after refresh rect being set
    _LCDC_CheckParam();

    _LCDC_SetTopParam(lcd_id);
    _LCDC_SetImageLayerParam();
#if (defined(PLATFORM_SC6800H) || defined(PLATFORM_SC6530))
    for (i=LCD_LAYER_OSD1; i<=LCD_LAYER_OSD3; i++)
#else
	for (i=LCD_LAYER_OSD1; i<=LCD_LAYER_OSD5; i++)
#endif	
    {
        _LCDC_SetOSDLayerParam ( (LCD_LAYER_ID_E) i);
    }

    _LCDC_SetCaptureParam(lcd_id);

    return SCI_SUCCESS;
}
/****************************************************************************************/
// Description: config the image layer
// Global resource dependence:
// Author: Shan.he
/*****************************************************************************************/
LOCAL uint32 _LCDC_ConfigImageLayer (LCD_IMG_LAYER_PARAM_T *param_ptr)
{
    LCD_IMG_LAYER_PARAM_T   *layer_ptr              = &s_lcdc_cfg.img_layer;
 //   uint32                  bytes_per_pixel         = 0;    

    SCI_ASSERT (PNULL != param_ptr);/*assert verified*/

    *layer_ptr = *param_ptr;
#if 0
    //get the bytes number per pixel according to the data format
    _LCDC_GetDataCoeffParam (param_ptr->format, &bytes_per_pixel);

    //calculate the start address of trim rect
    addr_offset = (param_ptr->src_size.w * (param_ptr->src_trim_rect.y-param_ptr->disp_pos.y)
                   + (param_ptr->src_trim_rect.x-param_ptr->disp_pos.x)) * bytes_per_pixel;

    layer_ptr->src_base_addr.y_addr += addr_offset;

    switch (param_ptr->format)
    {
        case LCD_YUV422:
            addr_offset = (param_ptr->src_size.w * (param_ptr->src_trim_rect.y -param_ptr->disp_pos.y)
                           + (param_ptr->src_trim_rect.x-param_ptr->disp_pos.x)) * bytes_per_pixel;
            layer_ptr->src_base_addr.uv_addr += addr_offset;
            break;

        case LCD_YUV420:
            addr_offset = (param_ptr->src_size.w * ( (param_ptr->src_trim_rect.y-param_ptr->disp_pos.y) >> LCDC_SHIFT_BIT1)
                           + (param_ptr->src_trim_rect.x-param_ptr->disp_pos.x)) * bytes_per_pixel;
            layer_ptr->src_base_addr.uv_addr += addr_offset;
            break;

        default :
            layer_ptr->src_base_addr.uv_addr = NULL;
            break;
    }

#endif
    s_lcdc_cfg.layer_pitch[LCD_LAYER_IMAGE] = param_ptr->src_size.w;

    return SCI_SUCCESS;
}
/****************************************************************************************/
// Description: config the OSD1 layer
// Global resource dependence:
// Author: Shan.he
/*****************************************************************************************/
LOCAL uint32 _LCDC_ConfigOSDLayer (LCD_LAYER_ID_E layer_id, LCD_OSD_LAYER_PARAM_T *param_ptr)
{
    LCD_OSD_LAYER_PARAM_T       *layer_ptr          = PNULL;
//    uint32                      bytes_per_pixel     = 0;   

    SCI_ASSERT (PNULL != param_ptr);/*assert verified*/

    layer_ptr = &s_lcdc_cfg.osd_layer[ (uint32) layer_id - 1];

    *layer_ptr = *param_ptr;
#if 0
    //get the bytes number per pixel according to the data format
    _LCDC_GetDataCoeffParam (param_ptr->format, &bytes_per_pixel);

    //calculate the start address of trim rect
    addr_offset = (param_ptr->src_size.w * (param_ptr->src_trim_rect.y-param_ptr->disp_pos.y)
                   + (param_ptr->src_trim_rect.x-param_ptr->disp_pos.x)) * bytes_per_pixel;
    layer_ptr->src_base_addr += addr_offset;

    if (LCD_RGB565 == layer_ptr->format && LCD_OSD_PIXEL_ALPHA == layer_ptr->alpha_mode)
    {
        // 1 bytes per pixel
        addr_offset = param_ptr->src_size.w * (param_ptr->src_trim_rect.y-param_ptr->disp_pos.y)
                      + (param_ptr->src_trim_rect.x-param_ptr->disp_pos.x);
        layer_ptr->alpha_base_addr += addr_offset;
    }

#endif
    s_lcdc_cfg.layer_pitch[layer_id] = layer_ptr->src_size.w;

    return SCI_SUCCESS;
}

/**------------------------------------------------------------------------------------*
 **                         public functions                                                                           *
 **------------------------------------------------------------------------------------*/
 
/********************************************************************************************/
//Description: register lcdc module irq handle func
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_AppRegIntFunc (LCD_INT_TYPE_E irq_num, LCD_IRQ_FUNC func)
{
    SCI_PASSERT ( (uint32) LCD_INT_MAX > (uint32) irq_num, ("LCDC_AppRegisterIntFunc: The interrupt num is invalid %d", irq_num));/*assert verified*/

    LCDC_DrvCleanInt (irq_num);
    LCDC_DrvOpenInt (irq_num, func);

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: register lcdc module irq handle func
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_AppUnRegIntFunc (LCD_INT_TYPE_E irq_num)
{
    SCI_PASSERT ( (uint32) LCD_INT_MAX > (uint32) irq_num, ("LCDC_AppUnRegisterIntFunc: The interrupt num is invalid %d", irq_num));/*assert verified*/

    LCDC_DrvCloseInt (irq_num);
    LCDC_DrvCleanInt (irq_num);

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: config the layer parameters
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC uint32 LCDC_AppConfigBlock (LCD_LAYER_ID_E layer_id, void *layer_param_ptr)
{
    switch (layer_id)
    {
        case LCD_LAYER_IMAGE:
            return _LCDC_ConfigImageLayer ( (LCD_IMG_LAYER_PARAM_T *) layer_param_ptr);
        case LCD_LAYER_OSD1:
        case LCD_LAYER_OSD2:
        case LCD_LAYER_OSD3:
#ifdef PLATFORM_SC8800G
    	case LCD_LAYER_OSD4:
    	case LCD_LAYER_OSD5:
#endif	
            return _LCDC_ConfigOSDLayer (layer_id, (LCD_OSD_LAYER_PARAM_T *) layer_param_ptr);
        default:
            //SCI_TRACE_LOW:"LCDC_AppConfigBlock, layer error, layer id: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_APP_820_112_2_18_1_1_40_28,(uint8*)"d", layer_id);
            break;
    }
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: config capture parameters
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC uint32 LCDC_AppConfigCapture (LCDC_CAP_PARAM_T *param_ptr)
{
    LCDC_CAP_PARAM_T  *cap_ptr         = &s_lcdc_cfg.cap_param;
    uint32             bytes_per_pixel = 0;
    uint32             addr_offset     = 0;    

    *cap_ptr = *param_ptr;

#ifndef PLATFORM_SC6800H
	if(!cap_ptr->cap_en)
    {
        LCDC_DrvSetCapEnable(FALSE);
    }    
#endif   

    #ifdef CHIP_ENDIAN_LITTLE
    if(param_ptr->set_endian_flag)
    {
        cap_ptr->endian_switch = param_ptr->endian_switch;
    }
    else
    {
        if(LCD_RGB565 == cap_ptr->format)
        {
            cap_ptr->endian_switch = 2;
        }
        else
        {
            cap_ptr->endian_switch = 0;
        }
    }
    #else
        cap_ptr->endian_switch = 0;
    #endif

    //get the bytes number per pixel according to the data format
    _LCDC_GetDataCoeffParam (param_ptr->format, &bytes_per_pixel);
    //calculate the start address of trim rect
    addr_offset = (param_ptr->dst_size.w * param_ptr->dst_trim_rect.y
                   + param_ptr->dst_trim_rect.x) * bytes_per_pixel;
    cap_ptr->dst_base_addr += addr_offset;

    s_lcdc_cfg.cap_pitch = param_ptr->dst_size.w;

    SCI_PASSERT (param_ptr->cap_rect.w == param_ptr->dst_trim_rect.w,  /*assert verified*/
                 ("LCDC_AppConfigCapture: wrong cap rect and trim rect, cap_w = %d, trim_w = %d",
                  param_ptr->cap_rect.w, param_ptr->dst_trim_rect.w));/*assert verified*/
    SCI_PASSERT (param_ptr->cap_rect.h == param_ptr->dst_trim_rect.h,  /*assert verified*/
                 ("LCDC_AppConfigCapture: wrong cap rect and trim rect, cap_h = %d, trim_h = %d",
                  param_ptr->cap_rect.h, param_ptr->dst_trim_rect.h));/*assert verified*/

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set lcdc background color
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC uint32 LCDC_AppSetBGColor (uint32 bg_color)
{
    s_lcdc_cfg.bg_color = bg_color;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: enable lcdc dither
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC uint32 LCDC_AppEnableDither (void)
{
    s_lcdc_cfg.dither_en = TRUE;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: disable lcdc dither
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC uint32 LCDC_AppDisableDither (void)
{
    s_lcdc_cfg.dither_en = FALSE;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set fmark mode and pol
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC uint32 LCDC_AppSetFmark (uint32 lcd_id,LCD_FMARK_MODE_E mode, LCD_FMARK_POL_E pol)
{
    SCI_PASSERT((lcd_id<2),("LCDC_AppSetFmark:lcd id is error! %d",lcd_id));/*assert verified*/
    s_lcdc_cfg.fmark_mode[lcd_id] = mode;
    s_lcdc_cfg.fmark_pol[lcd_id] = pol;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set logic size of LCD
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC uint32 LCDC_AppSetLogicSize (LCDC_SIZE_T *size)
{
    s_lcdc_cfg.lcdc_logic_size = *size;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set display rect
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC uint32 LCDC_AppSetDisplayRect (LCDC_RECT_T *rect)
{
    s_lcdc_cfg.display_rect = *rect;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set parameters for yuv2rgb
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC uint32 LCDC_AppSetYUV2RGBParam (LCD_YUV2RGB_PARAM_T *param_ptr)
{
    s_lcdc_cfg.yuv2rgb = *param_ptr;

    return SCI_SUCCESS;
}

#ifdef PLATFORM_SC8800G
/********************************************************************************************/
//Description: set cs pin
//Global resource dependence: 
//Author: Shan.he
/********************************************************************************************/
PUBLIC void LCDC_AppSetCSPin(uint32 lcd_id,uint32 cs_pin)
{
	SCI_PASSERT((lcd_id<2),("LCDC_AppSetCSPin:lcd id is error!"));/*assert verified*/
	s_record_cs_pin[lcd_id] = cs_pin&0x1;/*lint !e662 !e661*/
}
#endif

/********************************************************************************************/
//Description:   invalidate a rectangle of the lcdc logic plane to the lcd device,
//                must be called after all the parameters having been set
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC uint32 LCDC_AppInvalidateRect (
            									uint32 lcd_id,
                                                uint16 left,     //left of the rectangle
                                                uint16 top,     //top of the rectangle
                                                uint16 right,     //right of the rectangle
                                                uint16 bottom    //bottom of the rectangl)
                                             )
{
    BOOLEAN         result      = SCI_TRUE;
#ifndef PLATFORM_SC6800H	
	uint32          lcm_ctrl_val = 0;
#endif

    s_lcdc_cfg.refresh_rect.x = left;
    s_lcdc_cfg.refresh_rect.y = top;
    s_lcdc_cfg.refresh_rect.w = right - left + 1;
    s_lcdc_cfg.refresh_rect.h = bottom - top + 1;

    _LCDC_SetParam(lcd_id);

    if (!s_lcdc_cfg.cap_param.cap_en)
    {
#if !defined(PLATFORM_SC6800H) && !defined(PLATFORM_SC6530)	


	    lcm_ctrl_val = (right-left+1)*(bottom-top+1);
    	lcm_ctrl_val |= s_record_cs_pin[lcd_id]<<26;
		TM_SendTestPointRequest(0x4c434443, 14);
    	//set lcm ctrl reg
    	LCDC_DrvSetLCMCtrl(lcm_ctrl_val);
		TM_SendTestPointRequest(0x4c434443, 15);
#endif		
        LCDC_DrvRun(); 
		TM_SendTestPointRequest(0x4c434443, 16);
    }
    else
    {
        LCDC_DrvRun();
    }

    if (LCDC_AppGetCheckState())
    {
        result = LCDC_DrvWaitIntDone (LCD_INT_LCDC_DONE, 200);
    }
    
    return result ? SCI_SUCCESS : SCI_ERROR;
}
/********************************************************************************************/
//Description:   initialize lcdc
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC uint32 LCDC_AppOpen (void)
{   
    if (s_lcdc_cfg.is_init)
    {
        return SCI_SUCCESS;
    }

    s_lcdc_cfg.yuv2rgb.upsample_mode = LCD_UPSAMPLE_AVERAGE;
    s_lcdc_cfg.yuv2rgb.contrast = 64;
    s_lcdc_cfg.yuv2rgb.saturation = 64;
    s_lcdc_cfg.yuv2rgb.brightness = 0;

//    if(!BOOT_GetBLStatus())  //bl is turn off
    {
        LCDC_DrvReset();
    }

    //reset and enable lcdc
    LCDC_DrvEnable (SCI_TRUE);

    LCDC_SetClock(LCDC_CLOCK_96MHZ);//64MHz for 6800,96MHz for 8800g

    s_lcdc_cfg.is_init = SCI_TRUE;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set the interval between 2 AHB master requests for each client.
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_AppSetAhbReqestGap (uint8 cycles)
{
    s_lcdc_cfg.ahb_req_gap = cycles;
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description:   close lcdc
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC uint32 LCDC_AppClose (void)
{
    if (s_lcdc_cfg.is_init)
    {
        LCDC_DrvEnable (SCI_FALSE);
        s_lcdc_cfg.is_init = SCI_FALSE;
        SCI_MEMSET (s_lcdc_int_callback,0,sizeof (LCD_IRQ_FUNC) *LCD_INT_MAX);
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description:   close lcdc
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC uint32 LCDC_AppDisableAllLayer (void)
{
    LCDC_DrvDisableAllLayer();
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description:   register callback
//Global resource dependence:
//Author: Jianping.Wang
/********************************************************************************************/
PUBLIC uint32 LCDC_AppRegisterCallBack (LCD_INT_TYPE_E type, LCD_IRQ_FUNC callback)
{
    uint32 ret = SCI_SUCCESS;

    if (callback != PNULL)
    {
        if (type<LCD_INT_MAX)
        {
            s_lcdc_int_callback[type] = callback;
        }
        else
        {
            ret = SCI_PARAM_ERROR;
        }
    }

    return ret;
}
/********************************************************************************************/
//Description: open lcdc irq
//Global resource dependence:
//Author: Jianping.Wang
/********************************************************************************************/
PUBLIC uint32 LCDC_AppOpenInt (LCD_INT_TYPE_E int_type,LCD_IRQ_FUNC irq_func)
{
    LCDC_DrvOpenInt (int_type, irq_func);
#ifdef PLATFORM_SC6800H    
    TB_EnableINT(TB_LCD_INT);  //enable LCDC IRQ
#else
    TB_EnableINT(TB_LCDC_INT);	//enable LCDC IRQ
#endif    
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: close lcdc irq
//Global resource dependence:
//Author: Jianping.Wang
/********************************************************************************************/
PUBLIC uint32 LCDC_AppCloseInt (LCD_INT_TYPE_E irq_num)
{
    LCDC_DrvCloseInt (irq_num);
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: close lcdc irq
//Global resource dependence:
//Author: Jianping.Wang
/********************************************************************************************/
PUBLIC void LCDC_AppLcdcDone (void)
{
    _LCDC_ISR0_LcdcDone();
}
/********************************************************************************************/
//Description: set if check state of lcdc done
//Global resource dependence:
//Author: Jianping.Wang
/********************************************************************************************/
PUBLIC void LCDC_AppSetCheckState (BOOLEAN is_check)
{
    s_lcdc_cfg.is_check_done_state = is_check;
}
/********************************************************************************************/
//Description:
//Global resource dependence:
//Author: Jianping.Wang
/********************************************************************************************/
PUBLIC BOOLEAN LCDC_AppGetCheckState (void)
{
    return s_lcdc_cfg.is_check_done_state;
}
PUBLIC uint32 LCDC_AppSetExpandMode (LCDC_EXPAND_MODE_E expand_mode)
{
    s_lcdc_cfg.expand_mode = expand_mode;
    return SCI_SUCCESS;
}
/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                                                       *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif


