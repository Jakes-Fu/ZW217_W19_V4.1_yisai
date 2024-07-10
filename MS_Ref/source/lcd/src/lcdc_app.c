/***************************************************************************************
** File Name:      lcdc_app.c                                                          *
** DATE:           06/01/2009                                                          *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.                   *
** Description:                                                                        *
****************************************************************************************/

/***************************************************************************************
 **                   Edit    History                                                  *
 **------------------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                          *
 ** 06/01/2009    Tim.zhu   Create.                                                    *
 ***************************************************************************************/

/**------------------------------------------------------------------------------------*
 **                         Dependencies                                               *
 **------------------------------------------------------------------------------------*/
#include "ms_ref_lcd_trc.h"
#include "lcdc_drv.h"
#include "lcdc_app.h"
#include "sc_reg.h"
//#include "pwm_drvapi.h"
#include "boot_drvapi.h"
#include "task_monitor.h"
#include "dal_display.h"
#include "lcd_ctrl.h"
#include "lcm_drv.h"
#include "lcm_prod.h"

/**------------------------------------------------------------------------------------*
 **                         Compiler Flag                                              *
 **------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**------------------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                           *
 **------------------------------------------------------------------------------------*/
#define LCDC_SHIFT_BIT1               0x01
#define LCDC_SHIFT_BIT2               0x02
#define LCDC_SHIFT_BIT3               0x03
#define LCDC_SHIFT_BIT4               0x04
#define LCDC_SHIFT_BIT5               0x05
#define LCDC_SHIFT_BIT6               0x06
#define LCDC_SHIFT_BIT7               0x07
#define LCDC_SHIFT_BIT8               0x08

#define LCDC_ALGIN_1                  0x00
#define LCDC_ALGIN_2                  0x01
#define LCDC_ALGIN_4                  0x03
#define LCDC_ALGIN_8                  0x07
#define LCDC_ALGIN_16                 0x0f

/**------------------------------------------------------------------------------------*
 **                         STRUCT  DEFINITION                                         *
 **------------------------------------------------------------------------------------*/
typedef struct lcdc_config_info_tag
{
    LCD_IMG_LAYER_PARAM_T       img_layer;
    LCD_OSD_LAYER_PARAM_T       osd_layer[LCDC_OSD_LAYER_NUM];
    uint32                      layer_pitch[LCDC_LAYER_NUM];
    LCD_YUV2RGB_PARAM_T         yuv2rgb;
    LCDC_CAP_PARAM_T            cap_param;
    uint32                      cap_pitch;
    uint32                      bg_color;    
    LCDC_SIZE_T                 lcdc_logic_size;
    LCD_FMARK_MODE_E            fmark_mode[2];
    LCD_FMARK_POL_E             fmark_pol[2];
    LCDC_RECT_T                 display_rect; //display rect of lcdc logic plane
    LCDC_RECT_T                 refresh_rect; //refresh rect of lcd device    
    uint32                      ahb_req_gap;
    uint32                      hi_addr;
    BOOLEAN                     dither_en;
    BOOLEAN                     is_init;
    BOOLEAN                     is_check_done_state;
    uint8                       padding; //reserved
    LCDC_RGB_EXPAND_MODE_E      rgb_expand_mode;
} LCDC_CONFIG_INFO_T;

/**------------------------------------------------------------------------------------*
 **                         local variables define                                     *
 **------------------------------------------------------------------------------------*/
LOCAL LCD_IRQ_FUNC s_lcdc_int_callback[LCD_INT_MAX] = {0};
LOCAL LCDC_CONFIG_INFO_T    s_lcdc_cfg       = {0};
LOCAL uint32 s_record_cs_pin[2] = {0};

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

 //   SCI_TRACE_LOW("_LCDC_ISR0_LcdcDone  %d",SCI_GetTickCount());

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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_APP_111_112_2_18_1_45_32_143,(uint8*)"");
}
/****************************************************************************************/
// Description: The interrupt handle of NLCM begin display
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************************/
LOCAL void _LCDC_ISR2_LcmSof (void)
{
    //SCI_TraceLow:"LCDC_APP:_LCDC_ISR2_LcmSof\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_APP_118_112_2_18_1_45_32_144,(uint8*)"");
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
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_APP_129_112_2_18_1_45_32_145,(uint8*)"");
}
/****************************************************************************************/
// Description: lcdc change Lcdc Angle to Display Angle
// Global resource dependence:
// Author: Eddy.wei
/*****************************************************************************************/
LOCAL DISPLAY_ANGLE_E _LCDC_ChangeLcdcAngle2DisplayAngle (LCDC_ROTATION_E lcdc_angle)
{   
    DISPLAY_ANGLE_E ret = DISPLAY_ANGLE_0;
        
    switch(lcdc_angle)
    {
        case LCDC_ROTATION_0:
             ret = DISPLAY_ANGLE_0;
             break;
        case LCDC_ROTATION_90:
             ret = DISPLAY_ANGLE_90;
             break;
        case LCDC_ROTATION_180:
             ret = DISPLAY_ANGLE_180;
             break;
        case LCDC_ROTATION_270:
             ret = DISPLAY_ANGLE_270;
             break;
        case LCDC_ROTATION_MIRROR:
             ret = DISPLAY_ANGLE_0;
             break;
        case LCDC_ROTATION_FLIP:
             ret = DISPLAY_ANGLE_0;
             break;
        default:
             ret = DISPLAY_ANGLE_0;
             break;
    }

    return ret;
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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_APP_156_112_2_18_1_45_32_146,(uint8*)"d", data_format);
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
            l_width_algin=LCDC_ALGIN_4;
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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_APP_196_112_2_18_1_45_32_147,(uint8*)"d", data_format);
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
LOCAL uint32 _LCDC_CheckTopParam(void)
{

    if ( (LCDC_ZERO==s_lcdc_cfg.lcdc_logic_size.w)
            || (LCDC_ZERO==s_lcdc_cfg.lcdc_logic_size.h)
            || (LCDC_DrvGetMaxWidth() <s_lcdc_cfg.lcdc_logic_size.w)
            || (LCDC_DrvGetMaxHeight() <s_lcdc_cfg.lcdc_logic_size.h))

    {
        SCI_PASSERT (0, ("LCDC:lcdc logic size err  width:%d, height:%d",    /*assert verified*/
                         s_lcdc_cfg.lcdc_logic_size.w, s_lcdc_cfg.lcdc_logic_size.h));
    }
    
    return SCI_SUCCESS;
}
/****************************************************************************************/
// Description: lcdc module init
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************************/
LOCAL uint32 _LCDC_CheckRefreshParam(void)
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
LOCAL uint32 _LCDC_CheckImageLayerParam(void)
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
LOCAL uint32 _LCDC_CheckCapParam(void)
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
LOCAL void _LCDC_CheckParam(void)
{
    uint32 i = 0;    
    
    _LCDC_CheckTopParam();
    _LCDC_CheckImageLayerParam();
    
    for (i=LCD_LAYER_OSD1; i<=LCDC_OSD_LAYER_NUM; i++)
    {
        _LCDC_CheckOSDLayerParam ( (LCD_LAYER_ID_E) i);
    }

    _LCDC_CheckCapParam();
    _LCDC_CheckRefreshParam();
}
/****************************************************************************************/
// Description: get color key
// Global resource dependence:
// Author: Eddy.wei
/*****************************************************************************************/
LOCAL uint32 _LCDC_GetColorKey(LCD_DATA_FORMAT_E layer_format, uint32 colorkey)
{
#if defined(PLATFORM_SC8800G)
    if(layer_format == LCD_RGB565 || layer_format == LCD_RGB555)
    {           
        uint32 tmp_val = 0, ret = 0;

        tmp_val = (colorkey&0xF800)>>11;
        ret |= ((tmp_val<<3)|((tmp_val&0x01) ? 7 : 0))<<16;

        tmp_val = (colorkey&0x7e0)>>5;
        ret |= ((tmp_val<<2)|((tmp_val&0x01) ? 3 : 0))<<8;

        tmp_val = colorkey&0x1F;
        ret |= (tmp_val<<3)|((tmp_val&0x01) ? 7 : 0);
        
        return ret;
    }
    else
    {
        return colorkey;
    }

    
#else
    if(layer_format == LCD_RGB565 || layer_format == LCD_RGB555)
    {           
        uint32 tmp_val = 0, ret = 0;

        switch((uint32)s_lcdc_cfg.rgb_expand_mode)
        {
            case 0:
                   tmp_val = (colorkey&0xF800)>>11;
                   ret |= ((tmp_val<<3)|(tmp_val>>2))<<16;

                   tmp_val = (colorkey&0x7e0)>>5;
                   ret |= ((tmp_val<<2)|(tmp_val>>4))<<8;
                   
                   tmp_val = colorkey&0x1F;
                   ret |= (tmp_val<<3)|(tmp_val>>2);                   
                   break;
                 
            case 1:
                   tmp_val = (colorkey&0xF800)>>11;
                   ret |= (tmp_val<<3)<<16;

                   tmp_val = (colorkey&0x7e0)>>5;
                   ret |= (tmp_val<<2)<<8;
                   
                   tmp_val = colorkey&0x1F;
                   ret |= tmp_val<<3;                   
                   break;
                 
            case 2:
                   tmp_val = (colorkey&0xF800)>>11;
                   ret |= ((tmp_val<<3)|((tmp_val&0x01) ? 7 : 0))<<16;

                   tmp_val = (colorkey&0x7e0)>>5;
                   ret |= ((tmp_val<<2)|((tmp_val&0x01) ? 3 : 0))<<8;
                   
                   tmp_val = colorkey&0x1F;
                   ret |= (tmp_val<<3)|((tmp_val&0x01) ? 7 : 0);                   
                   break;
                 
            default:
                   SCI_TraceLow("_LCDC_GetColorKey: expand_mode is error! ");
                   break;
        }
        
        return ret;
    }
    else
    {
        return colorkey;
    }

#endif    
}
/****************************************************************************************/
// Description: set image layer parameters to the lcdc controller
// Global resource dependence:
// Author: Shan.he
/*****************************************************************************************/
LOCAL void _LCDC_SetImageLayerParam(uint32 lcd_id)
{
    LCD_IMG_LAYER_PARAM_T   *layer_ptr = & (s_lcdc_cfg.img_layer);   
    LCDC_RECT_T             disp_rect  = {0};
    LCD_LAYER_ID_E          layer_id   = LCD_LAYER_IMAGE;
    uint16 left = s_lcdc_cfg.refresh_rect.x;
    uint16 top  = s_lcdc_cfg.refresh_rect.y;
    uint16 right  = s_lcdc_cfg.refresh_rect.w+left-1;
    uint16 bottom = s_lcdc_cfg.refresh_rect.h+top-1;
    int32 layer_left   = layer_ptr->disp_pos.x;
    int32 layer_top    = layer_ptr->disp_pos.y;
    int32 layer_right  = layer_ptr->src_trim_rect.w+layer_left-1;
    int32 layer_bottom = layer_ptr->src_trim_rect.h+layer_top-1;
    uint32 byte_num = 0;
    int32 temp = 0;
    LCD_DATA_ADDR_T src_base_addr = layer_ptr->src_base_addr;

    LCDC_DrvSetLayerBlockEnable (layer_id, layer_ptr->layer_en);

    if (!layer_ptr->layer_en)
    {
        return;
    }

    _LCDC_GetDataCoeffParam(layer_ptr->format,&byte_num);

    if((layer_bottom<=top)||(layer_right<=left)||(layer_left>=right)||(layer_top>=bottom))
    {
        LCDC_DrvSetLayerBlockEnable(layer_id,SCI_FALSE);
        return;
    }       
    
    layer_left -= left;
    layer_top  -= top;
    layer_right  -= left;
    layer_bottom -= top;

    if(layer_right>=(right-left))
    {
        layer_right = right-left;
    }

    if(layer_bottom>(bottom-top))
    {
        layer_bottom = bottom-top;
    }

    if(layer_top<0)
    {
        if(LCD_YUV420 == layer_ptr->format)
        {
        	if(((uint32)(-layer_top))&0x01)
        	{
        	    layer_top += 1;
        	}
            temp = ((int32)s_lcdc_cfg.layer_pitch[layer_id])*layer_top*(int32)byte_num;
			layer_ptr->src_base_addr.y_addr -= temp; /*lint !e737*/
            layer_ptr->src_base_addr.uv_addr -= temp>>1;/*lint !e737*/
        }
        else
        {
            temp = ((int32)s_lcdc_cfg.layer_pitch[layer_id])*layer_top*(int32)byte_num;
            layer_ptr->src_base_addr.y_addr -= temp; /*lint !e737*/
            layer_ptr->src_base_addr.uv_addr -= temp;/*lint !e737*/
        }
        layer_top = 0;
    }

    if(layer_left<0)
    {
        temp = layer_left*((int32)byte_num);
        layer_ptr->src_base_addr.y_addr -= temp; /*lint !e737*/
        if(LCD_YUV420 == layer_ptr->format)
        {
            //fix bug 654535
            //for YUV420 NV12 format uv_addr offset do not used to be half of y_addr
            layer_ptr->src_base_addr.uv_addr -= temp; /*lint !e737*/
        }
        else
        {
            layer_ptr->src_base_addr.uv_addr -= temp; /*lint !e737*/
        }
        layer_left = 0;
    }   
    disp_rect.x = layer_left;
    disp_rect.y = layer_top;
    disp_rect.w = layer_right-layer_left+1;
    disp_rect.h = layer_bottom-layer_top+1;

    if(LCDC_ROTATION_0 != layer_ptr->rotation )
    {
        DISPLAY_RECT_T crop_rect = {0};
        DISPLAY_RECT_T src_rect  = {0};
        DISPLAY_CONVERT_INPUT_T output = {0};
        DISPLAY_CONVERT_OUTPUT_T input = {0};

        if(LCDC_ROTATION_90 == layer_ptr->rotation || LCDC_ROTATION_270 == layer_ptr->rotation)
        {
            uint16 disp_temp = disp_rect.w;
            disp_rect.w = disp_rect.h;
            disp_rect.h = disp_temp;
        }

        //get crop_rect logic coordinate from physical lcd plane coordinate
        input.physical_coordinate.left = left;
        input.physical_coordinate.top = top;
        input.physical_coordinate.right = right;
        input.physical_coordinate.bottom = bottom;
        input.rotation_angle = _LCDC_ChangeLcdcAngle2DisplayAngle(layer_ptr->rotation);

        _convert_coordinate((LCD_ID_E)lcd_id,&output,&input);
        crop_rect = output.logic_coordinate;

        //get src_rect logic coordinate from physical lcd plane coordinate
        input.physical_coordinate.left = layer_ptr->disp_pos.x;
        input.physical_coordinate.top = layer_ptr->disp_pos.y;
        input.physical_coordinate.right = layer_ptr->src_trim_rect.w+input.physical_coordinate.left-1;
        input.physical_coordinate.bottom = layer_ptr->src_trim_rect.h+input.physical_coordinate.top-1;
        input.rotation_angle = _LCDC_ChangeLcdcAngle2DisplayAngle(layer_ptr->rotation);

        _convert_coordinate((LCD_ID_E)lcd_id,&output,&input);
        src_rect = output.logic_coordinate;

        src_rect.top  -= crop_rect.top;
        src_rect.left -= crop_rect.left;

        if(src_rect.top<0)
        {
            temp = ((int32)s_lcdc_cfg.layer_pitch[layer_id])*src_rect.top*((int32)byte_num);
            src_base_addr.y_addr -= temp; /*lint !e737*/
            if(LCD_YUV420 == layer_ptr->format)
            {
                src_base_addr.uv_addr -= temp>>1; /*lint !e737*/
            }
            else
            {
                src_base_addr.uv_addr -= temp; /*lint !e737*/
            }
        }

        if(src_rect.left<0)
        {
            temp = src_rect.left*((int32)byte_num);
            src_base_addr.y_addr -= temp;   /*lint !e737*/
            if(LCD_YUV420 == layer_ptr->format)
            {
                //for YUV420 NV12 format uv_addr offset do not used to be half of y_addr
                src_base_addr.uv_addr -= temp;   /*lint !e737*/
            }
            else
            {
                src_base_addr.uv_addr -= temp;   /*lint !e737*/
            }
        } 

        layer_ptr->src_base_addr = src_base_addr;
    }

    LCDC_DrvSetDataFormat (layer_id, layer_ptr->format);
    LCDC_DrvSetEndian (layer_id, layer_ptr->endian_switch, layer_ptr->uv_endian_switch);
    LCDC_DrvSetAhbMasterAddr (layer_id, layer_ptr->src_base_addr.y_addr,
                              layer_ptr->src_base_addr.uv_addr);
    LCDC_DrvSetLayerRect (layer_id, &disp_rect);
    LCDC_DrvSetPitch (layer_id, s_lcdc_cfg.layer_pitch[layer_id]);
    if(LCD_RGB565 == layer_ptr->format)
    {
        LCDC_AppDisableDither();
    }

    LCDC_DrvSetLayerRB_Switch(layer_id,layer_ptr->rb_switch);
    LCDC_DrvSetLayerRotation(layer_id,layer_ptr->rotation);
}
/****************************************************************************************/
// Description: set osd1 layer parameters to the lcdc controller
// Global resource dependence:
// Author: Shan.he
/*****************************************************************************************/
LOCAL void _LCDC_SetOSDLayerParam (LCD_LAYER_ID_E layer_id,uint32 lcd_id)
{
    LCD_OSD_LAYER_PARAM_T   *layer_ptr = &s_lcdc_cfg.osd_layer[ (uint32) layer_id - 1];
    LCDC_RECT_T             disp_rect  = {0};
    uint16 left = s_lcdc_cfg.refresh_rect.x;
    uint16 top  = s_lcdc_cfg.refresh_rect.y;
    uint16 right = s_lcdc_cfg.refresh_rect.w+left-1;
    uint16 bottom = s_lcdc_cfg.refresh_rect.h+top-1;
    volatile int32 layer_left = layer_ptr->disp_pos.x;
    volatile int32 layer_top = layer_ptr->disp_pos.y;
    volatile int32 layer_right = layer_ptr->src_trim_rect.w+layer_left-1;
    volatile int32 layer_bottom = layer_ptr->src_trim_rect.h+layer_top-1;
    uint32 byte_num = 0;
    uint32 src_base_addr = layer_ptr->src_base_addr;

    LCDC_DrvSetLayerBlockEnable (layer_id, layer_ptr->layer_en);

    if (!layer_ptr->layer_en)
    {
        return;
    }

    _LCDC_GetDataCoeffParam(layer_ptr->format,&byte_num);
    
    if((layer_bottom<=top)||(layer_right<=left)||(layer_left>=right)||(layer_top>=bottom))
    {
        LCDC_DrvSetLayerBlockEnable(layer_id,SCI_FALSE);
        return;
    }       

    layer_left -= left;
    layer_top  -= top;
    layer_right  -= left;
    layer_bottom -= top;

    if(layer_right>(right-left))
    {
        layer_right = right-left;
    }

    if(layer_bottom>(bottom-top))
    {
        layer_bottom = bottom-top;
    }

    if(layer_top<0)
    {
        layer_ptr->src_base_addr -= ((int32)s_lcdc_cfg.layer_pitch[layer_id])*layer_top*((int32)byte_num); /*lint !e737*/       
        layer_top = 0;
    }

    if(layer_left<0)
    {
        layer_ptr->src_base_addr -= layer_left*((int32)byte_num);   /*lint !e737*/     
        layer_left = 0;
    }   
    disp_rect.x = layer_left;
    disp_rect.y = layer_top;
    disp_rect.w = layer_right-layer_left+1;
    disp_rect.h = layer_bottom-layer_top+1;

    if(LCDC_ROTATION_0 != layer_ptr->rotation)
    {
        DISPLAY_RECT_T crop_rect = {0};
        DISPLAY_RECT_T src_rect  = {0};
        DISPLAY_CONVERT_INPUT_T output = {0};
        DISPLAY_CONVERT_OUTPUT_T input = {0};

        if(LCDC_ROTATION_90 == layer_ptr->rotation || LCDC_ROTATION_270 == layer_ptr->rotation)
        {
            uint16 disp_temp = disp_rect.w;
            disp_rect.w = disp_rect.h;
            disp_rect.h = disp_temp;
        }

        //get crop_rect logic coordinate from physical lcd plane coordinate
        input.physical_coordinate.left = left;
        input.physical_coordinate.top = top;
        input.physical_coordinate.right = right;
        input.physical_coordinate.bottom = bottom;
        input.rotation_angle = _LCDC_ChangeLcdcAngle2DisplayAngle(layer_ptr->rotation);

        _convert_coordinate((LCD_ID_E)lcd_id,&output,&input);
        crop_rect = output.logic_coordinate;

        //get src_rect logic coordinate from physical lcd plane coordinate
        input.physical_coordinate.left = layer_ptr->disp_pos.x;
        input.physical_coordinate.top = layer_ptr->disp_pos.y;
        input.physical_coordinate.right = layer_ptr->src_trim_rect.w+input.physical_coordinate.left-1;
        input.physical_coordinate.bottom = layer_ptr->src_trim_rect.h+input.physical_coordinate.top-1;
        input.rotation_angle = _LCDC_ChangeLcdcAngle2DisplayAngle(layer_ptr->rotation);

        _convert_coordinate((LCD_ID_E)lcd_id,&output,&input);
        src_rect = output.logic_coordinate;

        src_rect.top  -= crop_rect.top;
        src_rect.left -= crop_rect.left;

        if(src_rect.top<0)
        {
            src_base_addr -= ((int32)s_lcdc_cfg.layer_pitch[layer_id])*src_rect.top*((int32)byte_num); /*lint !e737*/       
        }

        if(src_rect.left<0)
        {
            src_base_addr -= src_rect.left*((int32)byte_num);   /*lint !e737*/     
        } 

        layer_ptr->src_base_addr = src_base_addr;
    }

    LCDC_DrvSetColorKeyEnable (layer_id, layer_ptr->colorkey_en);
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    if (layer_ptr->format == LCD_RGB565)
    {
        LCDC_DrvSetAlphaMode (layer_id, LCD_OSD_BLOCK_ALPHA);
    }
    else 
    {
        LCDC_DrvSetAlphaMode (layer_id, layer_ptr->alpha_mode);
    }
#else //PLATFORM_SC6531EFM
    LCDC_DrvSetAlphaMode (layer_id, layer_ptr->alpha_mode);
#endif //PLATFORM_SC6531EFM
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
    layer_ptr->colorkey = _LCDC_GetColorKey(layer_ptr->format,layer_ptr->colorkey);
    LCDC_DrvSetColorkey (layer_id, layer_ptr->colorkey);
    if(LCD_RGB565 == layer_ptr->format)
    {
        LCDC_AppDisableDither();
    }

    LCDC_DrvSetLayerRB_Switch(layer_id,layer_ptr->rb_switch);
    LCDC_DrvSetLayerRotation(layer_id,layer_ptr->rotation);
    LCDC_DrvSetOsdBlendMode(layer_id,layer_ptr->blend_mode);
}
/****************************************************************************************/
// Description: set capture parameters to the lcdc controller
// Global resource dependence:
// Author: Shan.he
/*****************************************************************************************/
LOCAL void _LCDC_SetCaptureParam(uint32 lcd_id)
{
    LCDC_CAP_PARAM_T    *cap_ptr = & (s_lcdc_cfg.cap_param); 
    uint16 left = s_lcdc_cfg.refresh_rect.x;
    uint16 top = s_lcdc_cfg.refresh_rect.y;
    uint16 right = s_lcdc_cfg.refresh_rect.w+left-1;
    uint16 bottom = s_lcdc_cfg.refresh_rect.h+top-1;
    int32 layer_left = cap_ptr->cap_rect.x;
    int32 layer_top = cap_ptr->cap_rect.y;
    int32 layer_right = cap_ptr->cap_rect.w+layer_left-1;
    int32 layer_bottom = cap_ptr->cap_rect.h+layer_top-1;
    uint32 byte_num = 0;

    _LCDC_GetDataCoeffParam(cap_ptr->format,&byte_num);
    LCDC_DrvSetCapEnable (cap_ptr->cap_en);

    if (!cap_ptr->cap_en)
    {
        return;
    }
    if((layer_bottom<=top)||(layer_right<=left)||(layer_left>=right)||(layer_top>=bottom))
    {
        LCDC_DrvSetCapEnable(SCI_FALSE);
        return;
    }   
    
    layer_left -= left;
    layer_top  -= top;
    layer_right  -= left;
    layer_bottom -= top;

    if(layer_right>(right-left))
    {
        layer_right = right-left;
    }

    if(layer_bottom>(bottom-top))
    {
        layer_bottom = bottom-top;
    }
    //SCI_TRACE_LOW:"_LCDC_SetCaptureParam:old addr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_APP_884_112_2_18_1_45_33_148,(uint8*)"d",cap_ptr->dst_base_addr);
    if(layer_top<0)
    {
        cap_ptr->dst_base_addr -= ((int32)s_lcdc_cfg.cap_pitch)*layer_top*((int32)byte_num);   /*lint !e737*/     
        layer_top = 0;
    }

    if(layer_left<0)
    {
        cap_ptr->dst_base_addr -= layer_left*((int32)byte_num); /*lint !e737*/
        layer_left = 0;
    }   
    cap_ptr->cap_rect.x = layer_left;
    cap_ptr->cap_rect.y = layer_top;
    cap_ptr->cap_rect.w = layer_right-layer_left+1;
    cap_ptr->cap_rect.h = layer_bottom-layer_top+1;

    LCDC_DrvSetCapDataFormat (cap_ptr->format);
    LCDC_DrvSetCapDataEndian (cap_ptr->endian_switch);
    //SCI_TRACE_LOW:"_LCDC_SetCaptureParam:new addr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_APP_903_112_2_18_1_45_34_149,(uint8*)"d",cap_ptr->dst_base_addr);
    LCDC_DrvSetCapAddr (cap_ptr->dst_base_addr);

    LCDC_DrvSetCapRect (&cap_ptr->cap_rect);
    LCDC_DrvSetCapPitch (s_lcdc_cfg.cap_pitch);
    
    LCDC_DrvSetMasterMode(LCDC_CAP_MODE);
    LCDC_DrvSetCapMode(cap_ptr->cap_mode);        
    LCDC_DrvSetCapRB_Switch(cap_ptr->rb_switch);
    LCDC_DrvSetCapRotation(cap_ptr->rotation);
}
/****************************************************************************************/
// Description: set display parameters to the lcdc controller
// Global resource dependence:
// Author: Eddy.wei
/*****************************************************************************************/
LOCAL void _LCDC_SetDisplayParam(uint32 lcd_id)
{
    LCDC_CAP_PARAM_T    *cap_ptr = & (s_lcdc_cfg.cap_param); 

#ifdef SUBLCD_PHONE_SIMU_SUPPORT
    lcd_id = MAIN_LCD_ID;
#endif

    if (!cap_ptr->cap_en)
    {
        extern void _lcm_set_buswidth ( uint32 bus_width, uint32 lcd_id);
        
        LCD_SPEC_INFO_T_PTR cur_lcd_spec_info_ptr = LCM_GetLcdSpecInfo (lcd_id);

        if(LCD_CTRL_MODULE_SPI == cur_lcd_spec_info_ptr->controller)
        {
            LCDC_DrvSetMasterMode(LCDC_DISP_SPI_MODE);
            LCDC_DrvSetDispAddr (LCM_GetDataAddr(lcd_id));
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
            #if defined(LCD_SPI_3WIRE_9BIT_1DATA)
                LCDC_DrvSetDispRgbMode(LCDC_DISP_RGB_565);
                SPI_ValidRect((s_lcdc_cfg.refresh_rect.w)*(s_lcdc_cfg.refresh_rect.h), lcd_id);
            #elif defined(LCD_SPI_3WIRE_9BIT_2DATA)
                LCDC_DrvSetDispRgbMode(LCDC_DISP_RGB_565X2);
                SPI_ValidRect((s_lcdc_cfg.refresh_rect.w)*(s_lcdc_cfg.refresh_rect.h), lcd_id);
            #elif defined(LCD_SPI_4WIRE_8BIT_1DATA)
                LCDC_DrvSetDispRgbMode(LCDC_DISP_RGB_565);
                SPI_ValidRect((s_lcdc_cfg.refresh_rect.w)*(s_lcdc_cfg.refresh_rect.h)*2, lcd_id);
            #elif defined(LCD_SPI_4WIRE_8BIT_2DATA) // not verify
                LCDC_DrvSetDispRgbMode(LCDC_DISP_RGB_565X2);
                SPI_ValidRect((s_lcdc_cfg.refresh_rect.w)*(s_lcdc_cfg.refresh_rect.h)*2, lcd_id);
            #endif
#else //PLATFORM_SC6531EFM
            LCDC_DrvSetDispRgbMode(LCDC_DISP_RGB_565);
            SPI_ValidRect((s_lcdc_cfg.refresh_rect.w)*(s_lcdc_cfg.refresh_rect.h)*2, lcd_id);
#endif //PLATFORM_SC6531EFM
        }
        else //parallel interface
        {
        #ifdef LCD_DATA_WIDTH_16BIT
            _lcm_set_buswidth(1,lcd_id);  //16bit,num=1
        #else
            _lcm_set_buswidth(0,lcd_id);  //8bit,num=2
        #endif //LCD_DATA_WIDTH_16BIT
            LCDC_DrvSetMasterMode(LCDC_DISP_PARALLEL_MODE);
            LCDC_DrvSetDispRgbMode(LCDC_DISP_RGB_565);
            LCDC_DrvSetDispAddr (LCM_GetDataAddr(lcd_id));
        }
    }
}
/****************************************************************************************/
// Description: set top parameters to the lcdc controller
// Global resource dependence:
// Author: Shan.he
/*****************************************************************************************/
LOCAL void _LCDC_SetTopParam(uint32 lcd_id)
{
    LCDC_SIZE_T lcd_size = {0};
    LCDC_DrvSetBGColor (s_lcdc_cfg.bg_color);
    
    if(!s_lcdc_cfg.cap_param.cap_en)
    {
        LCDC_DrvSetFmark (s_lcdc_cfg.fmark_mode[lcd_id], s_lcdc_cfg.fmark_pol[lcd_id]);
    }
    else
    {
        LCDC_DrvSetFmark(LCD_FMARK_NONE,s_lcdc_cfg.fmark_pol[lcd_id]);
    }
    if((s_lcdc_cfg.refresh_rect.w<s_lcdc_cfg.display_rect.w) \
        || (s_lcdc_cfg.refresh_rect.h<s_lcdc_cfg.display_rect.h))
    {
        SCI_PASSERT(0,("display size is less than lcm size!"));/*assert verified*/
    }

    lcd_size.w = s_lcdc_cfg.refresh_rect.w;
    lcd_size.h = s_lcdc_cfg.refresh_rect.h;
    LCDC_DrvSetLCDCSize (&lcd_size);
    s_lcdc_cfg.display_rect.x = 0;
    s_lcdc_cfg.display_rect.y = 0;
    s_lcdc_cfg.display_rect.w = s_lcdc_cfg.refresh_rect.w;
    s_lcdc_cfg.display_rect.h = s_lcdc_cfg.refresh_rect.h;    
    LCDC_DrvSetLcmRect (&s_lcdc_cfg.display_rect);

    //set yuv to rgb parameters
    LCDC_DrvSetY2RBrightness (s_lcdc_cfg.yuv2rgb.brightness);
    LCDC_DrvSetY2RContrast (s_lcdc_cfg.yuv2rgb.contrast);
    LCDC_DrvSetY2RSaturation (s_lcdc_cfg.yuv2rgb.saturation);
    LCDC_DrvSetY2RUpsampleMode (s_lcdc_cfg.yuv2rgb.upsample_mode);
    LCDC_DrvSetAhbReqestGap (s_lcdc_cfg.ahb_req_gap);

    LCDC_DrvSetRgbExpandMode (s_lcdc_cfg.rgb_expand_mode);
}
/****************************************************************************************/
// Description: set lcdc param
// Global resource dependence:
// Author: Shan.he
/*****************************************************************************************/
LOCAL uint32 _LCDC_SetParam(uint32 lcd_id)
{
    uint32 i = 0;
    
    _LCDC_CheckParam();

    _LCDC_SetTopParam(lcd_id);
      LCDC_AppEnableDither();
    _LCDC_SetImageLayerParam(lcd_id);
    
    for (i=LCD_LAYER_OSD1; i<=LCDC_OSD_LAYER_NUM; i++)
    {
        _LCDC_SetOSDLayerParam ( (LCD_LAYER_ID_E) i,lcd_id);
    }
    LCDC_DrvSetDitherEnable (s_lcdc_cfg.dither_en);
    _LCDC_SetCaptureParam(lcd_id);
    _LCDC_SetDisplayParam(lcd_id);

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
    if(LCDC_ROTATION_90 == layer_ptr->rotation || LCDC_ROTATION_270 == layer_ptr->rotation) 
    {
        s_lcdc_cfg.layer_pitch[LCD_LAYER_IMAGE] = param_ptr->src_size.h;
    }
    else
    {
        s_lcdc_cfg.layer_pitch[LCD_LAYER_IMAGE] = param_ptr->src_size.w;
    }

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
    if(LCDC_ROTATION_90 == layer_ptr->rotation || LCDC_ROTATION_270 == layer_ptr->rotation) 
    {
        s_lcdc_cfg.layer_pitch[layer_id] = layer_ptr->src_size.h;
    }
    else
    {
        s_lcdc_cfg.layer_pitch[layer_id] = layer_ptr->src_size.w;
    }

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
    if(layer_id > (LCDC_LAYER_NUM -1))
    {
        layer_id = LCD_LAYER_ID_MAX;
    }
    
    switch (layer_id)
    {
        case LCD_LAYER_IMAGE:
            return _LCDC_ConfigImageLayer ( (LCD_IMG_LAYER_PARAM_T *) layer_param_ptr);
        case LCD_LAYER_OSD1:
        case LCD_LAYER_OSD2:
        case LCD_LAYER_OSD3:
        case LCD_LAYER_OSD4:
        case LCD_LAYER_OSD5:
            return _LCDC_ConfigOSDLayer (layer_id, (LCD_OSD_LAYER_PARAM_T *) layer_param_ptr);
        default:
            //SCI_TRACE_LOW:"LCDC_AppConfigBlock, layer error, layer id: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDC_APP_1129_112_2_18_1_45_34_150,(uint8*)"d", layer_id);
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

    if(!cap_ptr->cap_en)
    {
        LCDC_DrvSetCapEnable(FALSE);
    }    

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
    uint32          lcm_ctrl_val = 0;

    s_lcdc_cfg.refresh_rect.x = left;
    s_lcdc_cfg.refresh_rect.y = top;
    s_lcdc_cfg.refresh_rect.w = right - left + 1;
    s_lcdc_cfg.refresh_rect.h = bottom - top + 1;

    _LCDC_SetParam(lcd_id);

    if (!s_lcdc_cfg.cap_param.cap_en)
    {
        lcm_ctrl_val = (right-left+1)*(bottom-top+1);
        lcm_ctrl_val |= s_record_cs_pin[lcd_id]<<26;

        LCDC_DrvSetLCMCtrl(lcm_ctrl_val);
        
        LCDC_DrvRun(); 
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

    if(!BOOT_GetBLStatus())  //bl is turn off
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
    
    TB_EnableINT(TB_LCDC_INT);  //enable LCDC IRQ
    
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: close lcdc irq
//Global resource dependence:
//Author: Jianping.Wang
/********************************************************************************************/
PUBLIC uint32 LCDC_AppCloseInt (LCD_INT_TYPE_E irq_type)
{
    LCDC_DrvCloseInt (irq_type);
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
/********************************************************************************************/
//Description: set expand mode
//Global resource dependence:
//Author: Eddy.wei
/********************************************************************************************/
PUBLIC uint32 LCDC_AppSetRgbExpandMode (LCDC_RGB_EXPAND_MODE_E rgb_expand_mode)
{
    s_lcdc_cfg.rgb_expand_mode = rgb_expand_mode;

    return SCI_SUCCESS;
}

/********************************************************************************************/
//Description: lcdc init
//Global resource dependence:
//Author: Eddy.wei
/********************************************************************************************/
PUBLIC uint32 LCDC_AppDrvInit (uint32 lcd_id)
{
    if(LCD_FMARK_DEVICE == s_lcdc_cfg.fmark_mode[lcd_id])
    {
        LCDC_DrvInit();
    }

    return SCI_SUCCESS;
}
/**------------------------------------------------------------------------------------------*
 **                         Compiler Flag                                                    *
 **------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

