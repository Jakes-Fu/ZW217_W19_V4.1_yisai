/******************************************************************************
 ** File Name:     lcd.c                                                      *
 ** Description:                                                              *
 **    This file contains common driver for lcd modul                         *
 ** Author:         Jim zhang                                                 *
 ** DATE:           05/18/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/18/2003     Jim.zhang        Create.                                   *
 ******************************************************************************/
 /*lint -save -e551*/

#include "ms_ref_lcd_trc.h"
#include "sci_types.h"
#include "dal_lcd.h"
#include "os_api.h"
#include "lcm_drv.h"
#include "chip.h"
#include "lcm_prod.h"
#include "tb_drv.h"
#include "ref_outport.h"
#include "lcdc_app.h"
#include "mmu.h"
#include "deep_sleep.h"
#include "chng_freq.h"
#include "arm_mmu.h"
#include "lcd_cfg.h"
#include "display_cfg.h"
#include "sc_reg.h"
#include "display.h"
#include "lcm_prod.h"
#include "ref_lcd.h"
#include "boot_drvapi.h"
#include "task_monitor.h"
#include "lcd_ctrl.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif


/**---------------------------------------------------------------------------*
 **                         MACRO
 **---------------------------------------------------------------------------*/
#define ENABLE_BLOCK         1
#define DISABLE_BLOCK        0
#define LCD_DONE_EVENT       1
#define DISPLANE_WIDTH       1023
#define DISPLANE_HEIGHT      1023
#define DEFAULT_RESET_TIME   0x10
#define DEFAULT_RESET_LEVEL  0x0
#define IS_USE_LCD_ROTATION  0
#define LCDC_REG_NUM         78

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
extern void *MEM_GetMainLCDBuffer (void);
extern void *MEM_GetSubLCDBuffer (void);
extern void GPIO_SetLcdBackLight (BOOLEAN b_on);

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL void LCD_GetSemaphore (SCI_SEMAPHORE_PTR sm, SYS_STATE_E flag);
LOCAL void LCD_PutSemaphore (SCI_SEMAPHORE_PTR sm, SYS_STATE_E flag);
LOCAL ERR_LCD_E LCD_SetParam (uint32 ahb_clk);
LOCAL void LCD_FreeSemaphore (void);
LOCAL void _LCD_SetDirection_DispRot (LCD_ID_E lcd_id,BOOLEAN disp_mode);

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL   uint32              s_reg_flag[LCD_SUPPORT_MAX] ;
LOCAL   uint32              s_lcd_used_num = 0;
LOCAL   BLOCK_T             s_buffer_info[LCD_SUPPORT_MAX][BLOCK_SUM];
LOCAL   LCD_SPEC_INFO_T_PTR s_lcd_spec_info_ptr[LCD_SUPPORT_MAX];
LOCAL   LCD_PROPERTY_T      s_lcd_property_table[LCD_SUM];
#if IS_USE_LCD_ROTATION
LOCAL   LCD_DIRECT_E        s_lcd_direct[LCD_SUPPORT_MAX] = {0};
LOCAL   LCD_ROT_MODE_E      s_lcd_rot_mode = LCD_ROT_MODE_LCD;
#endif
LOCAL   SCI_EVENT_GROUP_PTR s_lcd_event = PNULL;
LOCAL   SCI_SEMAPHORE_PTR   s_lcdc_sm_ptr;
LOCAL   SYS_STATE_E         s_sys_run_state = SYS_STATE_NORMAL;
LOCAL   BOOLEAN             s_is_cap_mode = SCI_FALSE;
LOCAL   uint32              s_debug_lcd_info[2][2] = {0}; /*lint !e551*/
LOCAL   uint32              s_save_ctrl_info[LCDC_REG_NUM]= {0};
LOCAL   uint16              font_color = 0x00;
LOCAL   uint32              s_lcd_index = 0;
LOCAL   lcd_invalidate_done_func_t s_lcddone_callback = PNULL;
LOCAL   LCDC_RGB_EXPAND_MODE_E     rgb_expand_mode = LCDC_RGB_EXPAND_MSB;
LOCAL   uint32				s_lcd_reset_by_lcd_id = 0;
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         LOCAL INTERFACE                                  *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//Description:
//Global resource dependence:
//Author: Younger.yang
/*****************************************************************************/
LOCAL uint32 LCD_GetBusRatio (void)
{
    uint32 lcd_busratio = LCD_BUSRATIO;

    return lcd_busratio;
}

/*****************************************************************************/
//Description: get rotation mode
//Global resource dependence:
//Author: Tim.Zhu
/*****************************************************************************/
LOCAL LCD_ROT_MODE_E _LCD_GetRotateMode (void)
{
#if IS_USE_LCD_ROTATION
    return s_lcd_rot_mode;
#else
    return LCD_ROT_MODE_MAX;
#endif    
}

/*****************************************************************************/
//Description: get system run state
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL SYS_STATE_E LCD_GetRunState()
{
    return s_sys_run_state;
}

/*****************************************************************************/
//Description: data format convertion
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
//0:RGB888; 1:RGB666; 2:RGB565(RGB555 if OSD);3:RGB555;4:GREY,5:YUV422;6:YUV420;7:YUV400
__inline LCD_DATA_FORMAT_E _lcd_data_format_convert (uint32 format)
{
    LCD_DATA_FORMAT_E new_format = LCD_RGB565;

    switch (format)
    {
        case 0:
            new_format = LCD_RGB888;
            break;
        case 1:
            new_format = LCD_RGB666;
            break;
        case 2:
            new_format = LCD_RGB565;
            break;
        case 3:
            new_format = LCD_RGB555;
            break;
        case 4:
            new_format = LCD_GREY;
            break;
        case 5:
            new_format = LCD_YUV422;
            break;
        case 6:
            new_format = LCD_YUV420;
            break;
        case 7:
            new_format = LCD_YUV400;
            break;
        default:
            //SCI_TRACE_LOW:"_lcd_data_format_convert is error! format = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_169_112_2_18_1_44_56_23,(uint8*)"d",format);
            new_format = LCD_MAX;
            break;
    }

    return new_format;
}

/*****************************************************************************/
//Description: set event
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL void _LCD_SetEvent (uint32 set_flags)
{
    if (PNULL!=s_lcd_event)
    {
        SCI_SetEvent (s_lcd_event, set_flags, SCI_OR);
    }
}

/*****************************************************************************/
//Description: get event
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL uint32 _LCD_GetEvent (uint32 requested_flags, uint32 wait_option)
{
    uint32 actual_event = 0;
    uint32 status = SCI_ERROR;

    if (PNULL!=s_lcd_event)
    {
        status = SCI_GetEvent (s_lcd_event,
                               requested_flags,
                               SCI_OR_CLEAR,
                               &actual_event,
                               SCI_WAIT_FOREVER);
    }

    return status;
}

/*****************************************************************************/
//Description: clean event
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL void _LCD_Clearevent (void)
{
    if (PNULL!=s_lcd_event)
    {
        SCI_SetEvent (s_lcd_event, 0, SCI_AND);
    }
}

/*****************************************************************************/
//Description:LCD Get Semaphore
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL void LCD_GetSemaphore (SCI_SEMAPHORE_PTR sm, SYS_STATE_E flag)
{
    SCI_ASSERT (PNULL != sm);   /*assert verified*/
    
    if ( SCI_InThreadContext())
    {
        if (sm == s_lcdc_sm_ptr)
        {
            s_debug_lcd_info[0][0] = SCI_IdentifyThread();
            s_debug_lcd_info[0][1]++;
        }
        else
        {
            s_debug_lcd_info[1][0] = SCI_IdentifyThread();
            s_debug_lcd_info[1][1]++;
        }

        if (SCI_ERROR == SCI_GetSemaphore (sm, SCI_WAIT_FOREVER))
        {
            //SCI_TRACE_LOW:"LCD:Get Semaphore Fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_241_112_2_18_1_44_56_24,(uint8*)"");
        }
    }
}

/*****************************************************************************/
//Description:LCD Put Semaphor
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL void LCD_PutSemaphore (SCI_SEMAPHORE_PTR sm, SYS_STATE_E flag)
{
    uint32 ret;

    SCI_ASSERT (PNULL != sm);/*assert verified*/
 
    if (SCI_InThreadContext()) 
    {
        if (sm == s_lcdc_sm_ptr)
        {
            s_debug_lcd_info[0][0] = 0;
            s_debug_lcd_info[0][1]--;
        }
        else
        {
            s_debug_lcd_info[1][0] = 0;
            s_debug_lcd_info[1][1]--;
        }

        ret = SCI_PutSemaphore (sm);
        SCI_ASSERT (ret == SCI_SUCCESS);/*assert verified*/
    }
}

/*****************************************************************************/
//Description: free semaphore when use LCDC by asyn fashion
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL void LCD_FreeSemaphore (void)
{   
    uint32 lcd_id;   
    LCD_CAP_PARAM_T    cap_param = {0};

    TM_SendTestPointRequest(0x4c434443, 10);
    LCDC_AppCloseInt (LCD_INT_LCDC_DONE);   
    lcd_id = s_lcd_index;  
    LCDC_AppDrvInit(lcd_id);
    
    DPSLP_Lcdc_EnableAhbSleep (ENABLE_AHB_SLEEP);
    CHNG_FREQ_Event_Set (FREQ_CHNG_EVENT_LCDC,SYS_MODE_INT);    
    cap_param.cap_en = FALSE;   
    s_is_cap_mode = FALSE;
    LCDC_AppConfigCapture ( (LCDC_CAP_PARAM_T *) &cap_param);     
    _LCD_SetEvent (LCD_DONE_EVENT);   
                 
    LCD_PutSemaphore (s_lcdc_sm_ptr, SYS_STATE_NORMAL);
    LCD_PutSemaphore (s_lcd_property_table[lcd_id].semaphore, SYS_STATE_NORMAL);
       
    if(s_lcddone_callback)
    {
        s_lcddone_callback(SYS_STATE_NORMAL);
    }
    else
    {
        //SCI_TRACE_LOW:"LCD ERR:s_lcddone_callback is NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_301_112_2_18_1_44_56_25,(uint8*)"");
    }   
}
/*****************************************************************************/
//Description:  LCD Reg Info
//Global resource dependence:
//Author: Younger.Yang
/*****************************************************************************/
LOCAL void LCD_RegInfo (LCD_ID_E lcd_id, LCD_SPEC_INFO_T_PTR lcd_spec_ptr)
{
    //support low device
    if (lcd_spec_ptr != s_lcd_spec_info_ptr[lcd_id])
    {
        s_lcd_spec_info_ptr[lcd_id] = lcd_spec_ptr;
    }

    s_reg_flag[lcd_id] = 1;
}

/*****************************************************************************/
//Description: brush lcd protection
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL ERR_LCD_E ProtectionOperation (LCD_ID_E lcd_id)
{
    SCI_SEMAPHORE_PTR sm = NULL;
    uint32            ctrl_mode = 0;
    SYS_STATE_E       flag = 0;

    if(lcd_id >= s_lcd_used_num)
    {
        LCD_PutLcdCtrlSemaphore (lcd_id);
        //SCI_TRACE_LOW:"[ProtectionOperation ERROR],lcd_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_334_112_2_18_1_44_56_26,(uint8*)"d", lcd_id);
        return ERR_LCD_PARAMETER_WRONG;
    }

    ctrl_mode = s_lcd_property_table[lcd_id].lcd_spec.controller;

    sm = s_lcd_property_table[lcd_id].semaphore;
    LCD_GetSemaphore (sm, flag);
    LCD_GetSemaphore (s_lcdc_sm_ptr, flag);

    if (s_lcd_property_table[lcd_id].sleep_state == SCI_TRUE)
    {
        LCD_PutSemaphore (s_lcdc_sm_ptr, flag);
        LCD_PutSemaphore (sm, flag);
        LCD_PutLcdCtrlSemaphore (lcd_id);
        //SCI_TRACE_LOW:"ProtectionOperation:err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_349_112_2_18_1_44_56_27,(uint8*)"");
        return ERR_LCD_OPERATE_FAIL;
    }

    if (PNULL == s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation)
    {
        LCD_PutSemaphore (s_lcdc_sm_ptr, flag);
        LCD_PutSemaphore (sm, flag);
        LCD_PutLcdCtrlSemaphore (lcd_id);
        //SCI_TRACE_LOW:"---ProtectionOperation: lcd operation is NUL!---"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_358_112_2_18_1_44_56_28,(uint8*)"");
        return ERR_LCD_POINTER_NULL;
    }
    
    DPSLP_Lcdc_EnableAhbSleep (DISABLE_AHB_SLEEP);
    CHNG_FREQ_Event_Clr (FREQ_CHNG_EVENT_LCDC,SYS_MODE_NORMAL);

    return ERR_LCD_NONE;
}

/*****************************************************************************/
//Description:  set the timing of LCM
//Global resource dependence:
//Author: Younger.Yang
/*****************************************************************************/
LOCAL ERR_LCD_E LCD_SetParam (uint32 ahb_clk)
{
    uint32 lcd_id = 0;
    uint32 ret = 0;

    for (lcd_id = 0; lcd_id < LCD_SUPPORT_MAX; lcd_id++)
    {
        if (s_reg_flag[lcd_id])
        {
            ret |= LCDC_DrvSetTiming (s_lcd_spec_info_ptr[lcd_id], ahb_clk, lcd_id);
        }
    }

    return ret;
}

LOCAL void _AdjustRect(LCD_ID_E lcd_id,uint16* left_ptr,uint16* right_ptr)
{
    uint16 left  = *left_ptr;
    uint16 right = *right_ptr;
    uint16 temp = 0;
    
    if((s_buffer_info[lcd_id][0].is_enable) \
        &&((s_buffer_info[lcd_id][0].cfg.resolution==LCD_RESOLUTION_YUV422) \
        ||(s_buffer_info[lcd_id][0].cfg.resolution==LCD_RESOLUTION_YUV420) \
        ||(s_buffer_info[lcd_id][0].cfg.resolution==LCD_RESOLUTION_YUV400)))
    {        
        //SCI_TRACE_LOW:"LCD:_AdjustRect:left=%d,right=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_401_112_2_18_1_44_56_29,(uint8*)"dd",left,right);
        left -= (left&0x3);
        temp = (right-left+1)&0x3;
        if(temp)
        {
            right += (4-temp);    
        }
        *left_ptr = left;
        *right_ptr = right;
    }
    else
    {
        left -= (left&0x1);
        right += ((right+1)&0x1);
        *left_ptr = left;
        *right_ptr = right;
    }
}
/*****************************************************************************/
//Description: config block before update lcd
//Global resource dependence:
//Author: Tim.Zhu
/*****************************************************************************/
LOCAL void _ConfigBlock (
                                LCD_ID_E lcd_id, //ID of lcd to operate
                                uint16 left,     //left of the rectangle
                                uint16 top,      //top of the rectangle
                                uint16 right,    //right of the rectangle
                                uint16 bottom    //bottom of the rectangle.
                            )
{
    LCD_IMG_LAYER_PARAM_T layer_cfg = {0};
    LCD_OSD_LAYER_PARAM_T osd_cfg = {0};
    LCDC_RECT_T               disp_rect = {0};
    int32 i = 0;
    
    LCDC_AppDisableAllLayer();

    for (i=0 ; i<LCDC_LAYER_NUM ; i++)
    {
        if (i==0) //image layer
        {
            if (s_buffer_info[lcd_id][i].is_enable)
            {
                layer_cfg.layer_en = TRUE;
                layer_cfg.src_base_addr.y_addr = (uint32) s_buffer_info[lcd_id][i].mem_ptr;
                layer_cfg.src_base_addr.uv_addr = (uint32) s_buffer_info[lcd_id][i].uv_addr;
                layer_cfg.format = _lcd_data_format_convert (s_buffer_info[lcd_id][i].cfg.resolution);
                layer_cfg.endian_switch = (LCDC_ENDIAN_SWITCH_E)s_buffer_info[lcd_id][i].cfg.endian_switch;
                layer_cfg.uv_endian_switch = (LCDC_ENDIAN_SWITCH_E)s_buffer_info[lcd_id][i].cfg.endian_switch;
                layer_cfg.src_size.w = s_buffer_info[lcd_id][i].cfg.width;
                layer_cfg.src_size.h = s_buffer_info[lcd_id][i].cfg.end_y-s_buffer_info[lcd_id][i].cfg.start_y+1;
                layer_cfg.src_trim_rect.x = 0;
                layer_cfg.src_trim_rect.y = 0;
                layer_cfg.src_trim_rect.w = s_buffer_info[lcd_id][i].cfg.end_x-s_buffer_info[lcd_id][i].cfg.start_x+1;
                layer_cfg.src_trim_rect.h = s_buffer_info[lcd_id][i].cfg.end_y - s_buffer_info[lcd_id][i].cfg.start_y+1;
                layer_cfg.disp_pos.x = s_buffer_info[lcd_id][i].cfg.start_x;
                layer_cfg.disp_pos.y = s_buffer_info[lcd_id][i].cfg.start_y;
                layer_cfg.rotation = s_buffer_info[lcd_id][i].cfg.rotation;
                layer_cfg.rb_switch = s_buffer_info[lcd_id][i].cfg.rb_switch;
            }
            else
            {
                layer_cfg.layer_en = FALSE;
            }

            LCDC_AppConfigBlock (LCD_LAYER_IMAGE, (void *) &layer_cfg);

        }
        else//osd layer
        {
            if (s_buffer_info[lcd_id][i].is_enable)
            {
                osd_cfg.layer_en = TRUE;
                osd_cfg.alpha = (uint8) s_buffer_info[lcd_id][i].cfg.alpha;
                osd_cfg.alpha_base_addr = (uint32) s_buffer_info[lcd_id][i].alpha_addr;
                osd_cfg.alpha_mode = (LCD_OSD_ALPHA_E) s_buffer_info[lcd_id][i].cfg.alpha_sel;
                osd_cfg.colorkey = s_buffer_info[lcd_id][i].cfg.colorkey;
                osd_cfg.colorkey_en = s_buffer_info[lcd_id][i].cfg.colorkey_en;
                osd_cfg.disp_pos.x = s_buffer_info[lcd_id][i].cfg.start_x;
                osd_cfg.disp_pos.y = s_buffer_info[lcd_id][i].cfg.start_y;
                osd_cfg.format =  _lcd_data_format_convert (s_buffer_info[lcd_id][i].cfg.resolution);
                osd_cfg.grey_rgb = s_buffer_info[lcd_id][i].cfg.grey_rgb;
                osd_cfg.endian_switch = (LCDC_ENDIAN_SWITCH_E)s_buffer_info[lcd_id][i].cfg.endian_switch;
                osd_cfg.src_base_addr = (uint32) s_buffer_info[lcd_id][i].mem_ptr;
                osd_cfg.src_size.w = s_buffer_info[lcd_id][i].cfg.width;
                osd_cfg.src_size.h = s_buffer_info[lcd_id][i].cfg.end_y-s_buffer_info[lcd_id][i].cfg.start_y+1;//config.end_y - config.start_y + 1;//bottom - top + 1;
                osd_cfg.src_trim_rect.x = 0;
                osd_cfg.src_trim_rect.y = 0;
                osd_cfg.src_trim_rect.w = s_buffer_info[lcd_id][i].cfg.end_x-s_buffer_info[lcd_id][i].cfg.start_x+1;
                osd_cfg.src_trim_rect.h = s_buffer_info[lcd_id][i].cfg.end_y - s_buffer_info[lcd_id][i].cfg.start_y+1;
                osd_cfg.rotation = s_buffer_info[lcd_id][i].cfg.rotation;
                osd_cfg.rb_switch = s_buffer_info[lcd_id][i].cfg.rb_switch;
                osd_cfg.blend_mode = s_buffer_info[lcd_id][i].cfg.blend_mode;
            }
            else
            {
                osd_cfg.layer_en = FALSE;
            }

            LCDC_AppConfigBlock (i, (void *) &osd_cfg);
        }

        disp_rect.x = left;
        disp_rect.y = top;
        disp_rect.w = right - left + 1;
        disp_rect.h = bottom - top + 1;
        LCDC_AppSetDisplayRect (&disp_rect);

        LCDC_AppSetRgbExpandMode (rgb_expand_mode); 
    }
}

/*****************************************************************************/
//Description: LCD initialization when assert
//Global resource dependence:
//Author: Tim.Zhu
/*****************************************************************************/
LOCAL ERR_LCD_E LCD_InitInAssertRestMode (void)
{
    uint16      lcd_id;
    uint16      lcd_width = 0;
    uint16      lcd_height = 0;
    LCD_INFO_T  lcd_info = {0};
    BLOCKCFG_T  cfg = {0};
    LCDC_SIZE_T lcd_size = {0};
    LCDC_RECT_T lcd_disp_rect = {0};
    LCD_CAP_PARAM_T cap_param = {0};

    LCDC_DrvSetLcmInit(MAIN_LCD_ID);
    LCDC_DrvSetLcmInit(SUB_LCD_ID);

    s_sys_run_state = SYS_STATE_ASSERT;
    s_lcd_used_num = 0;

    //get lcd spec and reg the info
    for (lcd_id = 0; lcd_id < LCD_SUPPORT_MAX; lcd_id++)
    {
        s_lcd_spec_info_ptr[lcd_id] = LCM_GetLcdSpecInfo (lcd_id);

        if (PNULL != s_lcd_spec_info_ptr[lcd_id])
        {
            SCI_ASSERT (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation);/*assert verified*/

            LCD_RegInfo ( (LCD_ID_E) lcd_id, s_lcd_spec_info_ptr[lcd_id]);

            LCDC_DrvSetCSAddr (s_lcd_spec_info_ptr[lcd_id]->cs_pin, s_lcd_spec_info_ptr[lcd_id]->cd_pin, lcd_id);

            LCDC_AppSetCSPin (lcd_id,s_lcd_spec_info_ptr[lcd_id]->cs_pin);

            s_lcd_used_num++;
        }
    }

    LCDC_AppClose();
    LCDC_AppOpen();
    for(lcd_id=0 ; lcd_id<s_lcd_used_num ; lcd_id++)
    {
        LCDC_AppSetFmark (lcd_id,LCD_FMARK_NONE, LCD_FMARK_HIGH);
    }
    LCDC_AppSetBGColor (0);

    lcd_width  = s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->width;
    lcd_height = s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->height;

    lcd_size.w = lcd_width;
    lcd_size.h = lcd_height;
    lcd_disp_rect.x = 0;
    lcd_disp_rect.y = 0;
    lcd_disp_rect.w = lcd_width;
    lcd_disp_rect.h = lcd_height;

    LCDC_AppSetLogicSize (&lcd_size);
    LCDC_AppSetDisplayRect (&lcd_disp_rect);
    LCDC_AppDisableDither();

    for (lcd_id=0 ; lcd_id<s_lcd_used_num ; lcd_id++)
    {
        s_lcd_property_table[lcd_id].brush_mode  = LCD_BRUSH_MODE_SYNC;
        s_lcd_property_table[lcd_id].lcd_spec    = *s_lcd_spec_info_ptr[lcd_id];
        s_lcd_property_table[lcd_id].operation   = BLOCK_OPERATION;
        s_lcd_property_table[lcd_id].sleep_state = SCI_FALSE;
    }

    //Reset LCD
    if(!BOOT_GetBLStatus())  //bl is turn off
    {
        LCD_Reset();
    }

    //LCM RGBMODE
    LCDC_DrvSetRGBMode (s_lcd_spec_info_ptr[MAIN_LCD_ID]->cs_pin,s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->bus_width);

    //init LCM timing
    LCD_SetParam (CHIP_GetAhbClk());
    /* init lcd buffer */
    LCD_GetInfo (0,&lcd_info);
    cfg.colorkey   = 0;
    cfg.resolution = LCD_RESOLUTION_RGB565;
    cfg.priority   = 0;
    cfg.start_x    = 0;
    cfg.start_y    = 0;
    cfg.end_x      = lcd_info.lcd_width -1;
    cfg.end_y      = lcd_info.lcd_height -1 ;
    cfg.width      =  cfg.end_x - cfg.start_x +1;
    cfg.colorkey_en = FALSE;
    cfg.alpha_sel   = 1;
    cfg.alpha       = 0xff;
    cfg.type        = 1;     

    LCD_DisableAllBlock(MAIN_LCD_ID);   

    LCD_ConfigBlock (MAIN_LCD_ID, 0, cfg);
    LCD_SetBlockBuffer (MAIN_LCD_ID, 0, lcd_info.lcdbuff_ptr);
    LCD_EnableBlock (MAIN_LCD_ID, 0);

    //init all the lcd support  
    if(!BOOT_GetBLStatus())  //bl is turn off
    {
        if (PNULL != s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation)
        {
            s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation->lcd_Init();
            s_lcd_property_table[MAIN_LCD_ID].sleep_state = SCI_FALSE;
        }
    }
    
    cap_param.cap_en = FALSE;
    LCDC_AppConfigCapture ( (LCDC_CAP_PARAM_T *) &cap_param);

    return ERR_LCD_NONE;
}

/**--------------------------------------------------------------------------*
 **                         PUBLIC INTERFACE                                 *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
//Description: LCD initialization
//Global resource dependence:
//Author: Tim.Zhu
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_Init (void)
{
    LCD_ID_E          lcd_id;
    uint16            lcd_width = 0;
    uint16            lcd_height = 0;
    LCD_INFO_T        lcd_info = {0};
    BLOCKCFG_T        cfg = {0};
    LCDC_SIZE_T       lcd_size = {0};
    LCDC_RECT_T       lcd_disp_rect = {0};
    SCI_SEMAPHORE_PTR sm = SCI_NULL;

    if (s_lcd_used_num != 0)
    {
        return ERR_LCD_NONE;
    }

    LCDC_DrvSetLcmInit(MAIN_LCD_ID);
    LCDC_DrvSetLcmInit(SUB_LCD_ID);

    s_lcdc_sm_ptr   = SCI_CreateSemaphore ("LCDC OPERATION SEMAPHORE", 1);
    s_sys_run_state = SYS_STATE_NORMAL;

    //get lcd spec and reg the info
    for (lcd_id = 0; lcd_id < LCD_SUPPORT_MAX; lcd_id++)
    {
        s_lcd_spec_info_ptr[lcd_id] = LCM_GetLcdSpecInfo (lcd_id);

        if (PNULL != s_lcd_spec_info_ptr[lcd_id])
        {
            SCI_ASSERT (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation);/*assert verified*/
            
            LCD_RegInfo (lcd_id, s_lcd_spec_info_ptr[lcd_id]);

            //init LCM timing
            LCM_SetParam(s_lcd_spec_info_ptr[lcd_id], CHIP_GetAhbClk(), lcd_id);
            LCDC_DrvSetCSAddr (s_lcd_spec_info_ptr[lcd_id]->cs_pin, s_lcd_spec_info_ptr[lcd_id]->cd_pin, lcd_id);
            
            LCDC_AppSetCSPin (lcd_id,s_lcd_spec_info_ptr[lcd_id]->cs_pin);

            s_lcd_used_num++;
        }
    }

    SCI_ASSERT ( (s_lcd_used_num <= LCD_SUPPORT_MAX) && (s_lcd_used_num != 0));/*assert verified*/

    LCDC_AppOpen();
    
    for(lcd_id=0 ; lcd_id<s_lcd_used_num ; lcd_id++)
    {
        LCDC_AppSetFmark (lcd_id,LCD_FMARK_NONE, LCD_FMARK_HIGH);
    }
    LCDC_AppSetBGColor (0);

    //begin by jianping.wang, below codes will be deleted.
    lcd_width  = s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->width;
    lcd_height = s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->height;

    lcd_size.w = DISPLANE_WIDTH;
    lcd_size.h = DISPLANE_HEIGHT;
    lcd_disp_rect.x = 0;
    lcd_disp_rect.y = 0;
    lcd_disp_rect.w = lcd_width;
    lcd_disp_rect.h = lcd_height;

    LCDC_AppSetLogicSize (&lcd_size);
    LCDC_AppSetDisplayRect (&lcd_disp_rect);
    LCDC_AppDisableDither();

    for (lcd_id=0 ; lcd_id<s_lcd_used_num ; lcd_id++)
    {
        sm = SCI_CreateSemaphore ("LCD ASYNC OPERATION SEMAPHORE", 1);

        SCI_ASSERT (PNULL != sm);/*assert verified*/
        s_lcd_property_table[lcd_id].semaphore = sm;
        s_lcd_property_table[lcd_id].brush_mode = LCD_BRUSH_MODE_SYNC;
        s_lcd_property_table[lcd_id].lcd_spec = *s_lcd_spec_info_ptr[lcd_id];
        s_lcd_property_table[lcd_id].operation = BLOCK_OPERATION;
        s_lcd_property_table[lcd_id].sleep_state = SCI_FALSE;
        //LCM RGBMODE
        LCDC_DrvSetRGBMode (s_lcd_spec_info_ptr[lcd_id]->cs_pin,s_lcd_spec_info_ptr[lcd_id]->spec_ptr->bus_width);
    }

    //Reset LCD
    if(!BOOT_GetBLStatus()) //bl is turn off
    {
        LCD_Reset();
    }

    /* init lcd buffer */
    for (lcd_id=0; lcd_id<s_lcd_used_num; lcd_id++)
    {
        LCD_GetInfo (lcd_id,&lcd_info);
        cfg.colorkey   = 0;
        cfg.resolution = LCD_RESOLUTION_RGB565;
        cfg.start_x    = 0;
        cfg.start_y    = 0;
        cfg.end_x      = lcd_info.lcd_width -1;
        cfg.end_y      = lcd_info.lcd_height -1;
        cfg.width      = cfg.end_x - cfg.start_x +1;
        cfg.colorkey_en = FALSE;
        cfg.alpha_sel = 1;
        cfg.alpha     = 0xff;
        cfg.priority  = 0;
        cfg.type      = 1;
        LCD_ConfigBlock (lcd_id, 0, cfg);
        LCD_SetBlockBuffer (lcd_id, 0, lcd_info.lcdbuff_ptr);
        LCD_EnableBlock (lcd_id, 0);
    }
    
    if(!BOOT_GetBLStatus())  //bl is turn off
    {  
        //init all the lcd support
        for (lcd_id = 0; lcd_id < s_lcd_used_num; lcd_id++)
        {
        	s_lcd_reset_by_lcd_id = lcd_id;
            if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
            {      
                s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_Init();
                s_lcd_property_table[MAIN_LCD_ID].sleep_state = FALSE;
            }
        }
    }
	s_lcd_reset_by_lcd_id = 0;
    if (NULL == s_lcd_event)
    {
        s_lcd_event = SCI_CreateEvent ("lcd abstract layer event");
    }

    _display_init();

    return ERR_LCD_NONE;
}
/*****************************************************************************/
//Description: Close the lcd.(include sub lcd.)
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
PUBLIC void LCD_Close (void)
{
    uint16 lcd_id;

    //fix bug 605210, pull down lcm_rst when power off 
    * (volatile uint32 *) APB_MISC_CTL1 &= ~LCM_RSTN;
    SCI_Sleep (120);
    
    //close all lcd support
    for (lcd_id = 0; lcd_id < s_lcd_used_num; lcd_id++)
    {
        if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
        {
            s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_Close();
        }
    }

    LCDC_AppClose();
}
/*****************************************************************************/
//Description:   get the lcd information.
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_GetInfo (
    LCD_ID_E lcd_id,            //ID of lcd to operate
    LCD_INFO_T *lcd_info_ptr    //lcd information struct pointer
)
{
    uint16 lcd_width  = 0;
    uint16 lcd_height = 0;
    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/

    lcd_info_ptr->r_bitmask       = 0xf800;
    lcd_info_ptr->g_bitmask       = 0x07e0;
    lcd_info_ptr->b_bitmask       = 0x001f;
    lcd_info_ptr->bits_per_pixel  = 16;
    lcd_info_ptr->contrast_min    = 0x00;
    lcd_info_ptr->contrast_max    = 63;
    lcd_info_ptr->contrast_defaut = 0x0A;
    lcd_info_ptr->is_use_fmark    = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->is_use_fmark;
    lcd_info_ptr->fmark_pol       = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->fmark_pol;
    lcd_width     = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->width;
    lcd_height    = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->height;

    LCM_GetLcdDevSize (lcd_id,&lcd_info_ptr->lcd_width,&lcd_info_ptr->lcd_height);

    if ( (lcd_width<lcd_info_ptr->lcd_width) || (lcd_height<lcd_info_ptr->lcd_height))
    {
        //SCI_TRACE_LOW:"[LCD_GetInfo ERROR],lcd devsize is error! lcd_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_820_112_2_18_1_44_57_30,(uint8*)"d",lcd_id);
    }

    if (MAIN_LCD_ID == lcd_id)
    {
        lcd_info_ptr->lcdbuff_ptr = (void *) MEM_GetMainLCDBuffer();
    }
    else if (SUB_LCD_ID == lcd_id)
    {
        lcd_info_ptr->lcdbuff_ptr = (void *) MEM_GetSubLCDBuffer();
    }

    return ERR_LCD_NONE;
}

#if 0
PUBLIC ERR_LCD_E LCD_InvalidateRect_DMA (
LCD_ID_E lcd_id,
DISPLAY_RECT_T disp_rect,
DISPLAY_RECT_T crop_rect ,
uint32 src_w,
LCD_DIRECT_E rot_angle
)
{

    ERR_LCD_E ret = 0;   
    LCD_BRUSH_MODE_E brush_mode;
    uint32 i=0;
    uint32 crop_width = 0;
    uint32 crop_height = 0;
    uint32 crop_addr = 0;
    uint32 layer_num = 0;

    SCI_PASSERT ( ( (disp_rect.left<disp_rect.right) && (disp_rect.top<disp_rect.bottom)), ("LCD_InvalidateRect:l=%d,t=%d,r=%d,b=%d",disp_rect.left,disp_rect.top,disp_rect.right,disp_rect.bottom));/*assert verified*/

    ret = ProtectionOperation (lcd_id);

    if (ret != ERR_LCD_NONE)
    {
        LCD_FreeSemaphore();
        return ERR_LCD_PARAMETER_WRONG;
    }

    brush_mode = s_lcd_property_table[lcd_id].brush_mode;

    s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation->lcd_InvalidateRect (disp_rect.left, disp_rect.top, disp_rect.right, disp_rect.bottom);

    _ConfigBlock (lcd_id,disp_rect.left,disp_rect.top,disp_rect.right,disp_rect.bottom);
    MMU_InvalideDCACHE();

    for(i=0; i<LCD_LAYER_ID_MAX; i++)
    {
        if (s_buffer_info[lcd_id][i].is_enable)
        {

            crop_addr = (uint32) s_buffer_info[lcd_id][i].mem_ptr;

            if(2!=s_buffer_info[lcd_id][i].cfg.resolution)
            {
                LCD_FreeSemaphore();
                return ERR_LCD_OPERATE_FAIL;
            }
            
            crop_addr += s_buffer_info[lcd_id][i].cfg.start_y * src_w*2 + s_buffer_info[lcd_id][i].cfg.start_x * 2; 
            
            crop_addr += (uint16)crop_rect.top * src_w*2 + (uint16)crop_rect.left * 2;
            
            crop_width = crop_rect.right-crop_rect.left+1;
            
            crop_height = crop_rect.bottom-crop_rect.top+1;
            
            layer_num ++;
            
            break;
        }

    }


    if(layer_num >1 || layer_num == 0)
    {
        LCD_FreeSemaphore();
        return ERR_LCD_PARAMETER_WRONG;
    }


    LCM_RegisterCallBack (0, LCD_FreeSemaphore);  

    LCM_DmaWrite (lcd_id, WIDTH_16, crop_addr, crop_width, crop_height, src_w, rot_angle, brush_mode);   

    if (LCD_BRUSH_MODE_SYNC == brush_mode)
    {
        LCD_FreeSemaphore();            
    }

    return ERR_LCD_NONE;
    
}
#endif

/*****************************************************************************/
//Description:   invalidate a rectang with the internal lcd data buffer
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_InvalidateRect (
    LCD_ID_E lcd_id,//ID of lcd to operate
    uint16 left,     //left of the rectangle
    uint16 top,     //top of the rectangle
    uint16 right,     //right of the rectangle
    uint16 bottom    //bottom of the rectangle.
)
{
    int32 ret = 0;   
    LCD_BRUSH_MODE_E brush_mode;

    SCI_PASSERT ( ( (left<right) && (top<bottom)), ("LCD_InvalidateRect:l=%d,t=%d,r=%d,b=%d",left,top,right,bottom));/*assert verified*/

    if (!SCI_InThreadContext())
    {
        LCD_SetBrushMode (lcd_id,LCD_BRUSH_MODE_SYNC);
    }

    ret = ProtectionOperation (lcd_id);

    if (ret != ERR_LCD_NONE)
    {
        return ERR_LCD_NONE;
    }
    _AdjustRect(lcd_id,&left,&right);
    _LCD_Clearevent();
    LCDC_AppSetCheckState (FALSE);

    brush_mode = s_lcd_property_table[lcd_id].brush_mode;
    s_lcd_index = lcd_id;

    _LCD_SetDirection_DispRot(lcd_id,SCI_TRUE);
    
#if IS_USE_LCD_ROTATION

    if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_SetBrushDirection)
    {
        if (LCD_ROT_MODE_LCD == _LCD_GetRotateMode())
        {
            s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation->lcd_SetBrushDirection (s_lcd_direct[lcd_id]);
        }
        else
        {
            s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation->lcd_SetBrushDirection (LCD_DIRECT_NORMAL);
        }
    }

#endif

    if (!s_is_cap_mode)
    {
        s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation->lcd_InvalidateRect (left, top, right, bottom);
    }

    _ConfigBlock (lcd_id,left,top,right,bottom);

    MMU_InvalideDCACHE();

    LCDC_AppOpenInt (LCD_INT_LCDC_DONE,LCDC_AppLcdcDone);
    LCDC_AppRegisterCallBack (LCD_INT_LCDC_DONE,LCD_FreeSemaphore);

    if (LCD_BRUSH_MODE_SYNC == brush_mode)
    {
        TM_SendTestPointRequest(0x4c434443, 13);
        if (SCI_InThreadContext())
        {
            LCDC_AppSetCheckState (FALSE);
        }
        else
        {
            LCDC_AppSetCheckState (TRUE);
            LCDC_AppCloseInt (LCD_INT_LCDC_DONE);
            //SCI_TRACE_LOW:"LCD_Invalidate:Check State"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_999_112_2_18_1_44_57_31,(uint8*)"");
        }
    }

    if (SCI_SUCCESS != LCDC_AppInvalidateRect (lcd_id,left, top, right, bottom))
    {
        SCI_PASSERT (0, ("lcd invalidate timeout"));/*assert verified*/
    }

//    MMU_InvalideDCACHE();

    if (LCD_BRUSH_MODE_SYNC == brush_mode)
    {
        if (SCI_InThreadContext() && !LCDC_AppGetCheckState())
        {
            _LCD_GetEvent (LCD_DONE_EVENT, 0);
        }

        if (LCDC_AppGetCheckState())
        {
            LCD_FreeSemaphore();            
        }
    }

    return ERR_LCD_NONE;
}
/*****************************************************************************/
//Description:   invalidate a pixel(called by mmi special effect)
//Global resource dependence:
//Author: bin.ji
/*****************************************************************************/
LOCAL void LCD_InvalidatePixel (
    LCD_ID_E lcd_id,
    uint16 x,
    uint16 y,
    uint16 color
)
{
    s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_InvalidateRect (x, y, x, y);
    LCD_SendData (color,lcd_id);
}
/*****************************************************************************/
//Description:   invalidate the whole lcd.
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_Invalidate (LCD_ID_E lcd_id)
{
    uint16 right  = 0;
    uint16 bottom = 0;
    LCD_INFO_T  lcd_info = {0};

    LCD_GetInfo (lcd_id,&lcd_info);
    right     = lcd_info.lcd_width-1;
    bottom = lcd_info.lcd_height-1;
    LCD_InvalidateRect (lcd_id, 0, 0, right, bottom);

    return ERR_LCD_NONE;
}
/*****************************************************************************/
//Description:   entern/exit sleep mode
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
PUBLIC ERR_LCD_E  LCD_EnterSleep (
    LCD_ID_E lcd_id,    //ID of lcd to operate
    BOOLEAN is_sleep    //true:enter sleep mode; false:exit sleep mode
)
{
    ERR_LCD_E ret;
    SCI_SEMAPHORE_PTR sm = PNULL;
    SYS_STATE_E flag = 0;
    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/

    if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
    {
        if (s_lcd_property_table[lcd_id].sleep_state != (uint32) is_sleep)
        {
            sm = s_lcd_property_table[lcd_id].semaphore;
            LCD_GetSemaphore (sm, flag);
            LCD_GetSemaphore (s_lcdc_sm_ptr, flag);

            s_lcd_property_table[lcd_id].sleep_state = (uint32) is_sleep;

            if (!is_sleep)
            {
                LCDC_DrvEnable (SCI_TRUE);
            }

            ret =  s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_EnterSleep (is_sleep) ;

            if (is_sleep)
            {
                LCDC_DrvEnable (SCI_FALSE);
            }

            LCD_PutSemaphore (s_lcdc_sm_ptr, flag);
            LCD_PutSemaphore (sm, flag);
        }
        else
        {
            ret =  ERR_LCD_NONE;
        }

        return  ret;
    }
    else
    {
        ret = ERR_LCD_POINTER_NULL;
    }

    return ret;
}
/*****************************************************************************/
//Description:  set the contrast value
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
LOCAL ERR_LCD_E LCD_SetContrast (
    LCD_ID_E lcd_id,    //ID of lcd to operate.
    uint16  contrast    //contrast value to set.
)
{
    ERR_LCD_E ret = ERR_LCD_NONE;
#if 0
    SCI_SEMAPHORE_PTR sm = PNULL;
    
    SCI_ASSERT (lcd_id < s_lcd_used_num);

    if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
    {
        sm = s_lcd_property_table[lcd_id].semaphore;
        LCD_GetSemaphore (sm, SYS_STATE_NORMAL);
        ret = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_SetContrast (contrast);
        LCD_PutSemaphore (sm, SYS_STATE_NORMAL);
        return ret;
    }
    else
    {
        ret = ERR_LCD_POINTER_NULL;
    }
#endif    
    ret = ERR_LCD_FUNC_NOT_SUPPORT;
    return ret;
}
/*****************************************************************************/
//Description:    Set the brightness of LCD.
//Global resource dependence:
//Author:         Jim.zhang
/*****************************************************************************/
LOCAL ERR_LCD_E LCD_SetBrightness (
    LCD_ID_E lcd_id,    //ID of lcd to operate.
    uint16 brightness    //brightness value to set.
)
{
    ERR_LCD_E ret = ERR_LCD_NONE;
#if 0    
    SCI_SEMAPHORE_PTR sm = PNULL;
    
    SCI_ASSERT (lcd_id < s_lcd_used_num);

    if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
    {
        sm = s_lcd_property_table[lcd_id].semaphore;
        LCD_GetSemaphore (sm, SYS_STATE_NORMAL);
        ret = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_SetBrightness (brightness);
        LCD_PutSemaphore (sm, SYS_STATE_NORMAL);
    }
    else
    {
        ret = ERR_LCD_POINTER_NULL;
    }
#endif 
    ret = ERR_LCD_FUNC_NOT_SUPPORT;
    return ret;
}
/*****************************************************************************/
//Description:    Enable lcd to partial display mode, so can save power.
//Global resource dependence:
//Author:         Jim.zhang
/*****************************************************************************/
PUBLIC ERR_LCD_E  LCD_SetDisplayWindow (
    LCD_ID_E lcd_id,//ID of lcd to operate
    uint16 left,     //left of the display window
    uint16 top,     //top of the display window
    uint16 right,     //right of the display window
    uint16 bottom    //bottom of the display window
)
{
    ERR_LCD_E ret;
    SCI_SEMAPHORE_PTR sm = PNULL;

    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/

    if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
    {
        sm = s_lcd_property_table[lcd_id].semaphore;

        LCD_GetSemaphore (sm, SYS_STATE_NORMAL);
        ret = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_SetDisplayWindow (left, top, right, bottom);
        LCD_PutSemaphore (sm, SYS_STATE_NORMAL);
    }
    else
    {
        ret = ERR_LCD_POINTER_NULL;
    }

    return ret;

}
/*****************************************************************************/
//Description:  Set LCD invalidate direction
//Global resource dependence:
//Author: Younger.Yang
/*****************************************************************************/
PUBLIC ERR_LCD_E  LCD_SetDirection (
    LCD_ID_E        lcd_id,//ID of lcd to operate
    LCD_DIRECT_E    direct_type
)
{
#if IS_USE_LCD_ROTATION
    SCI_SEMAPHORE_PTR sm = s_lcd_property_table[lcd_id].semaphore;
    SYS_STATE_E flag = 0;

    SCI_ASSERT (direct_type < LCD_DIRECT_MAX);/*assert verified*/
    SCI_ASSERT (lcd_id < LCD_SUPPORT_MAX);/*assert verified*/

    s_lcd_direct[lcd_id] = direct_type;

    if (PNULL != s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation->lcd_SetBrushDirection)
    {
        if (LCD_ROT_MODE_LCD == _LCD_GetRotateMode())
        {
            LCD_GetSemaphore (sm, flag);

            s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation->lcd_SetBrushDirection (s_lcd_direct[lcd_id]);

            LCD_PutSemaphore (sm, flag);
        }
    }
#endif
    return ERR_LCD_NONE;

}

/*****************************************************************************/
//  Description:  Get LCD invalidate direction type
//  Input:
//      direct_type:
//  Return:
//      None.
//  Note: Application should correct invalidate direction right after current
//        image displayed
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/
PUBLIC LCD_DIRECT_E  LCD_GetDirection (LCD_ID_E lcd_id)
{
#if IS_USE_LCD_ROTATION
    return s_lcd_direct[lcd_id];
#else
    return LCD_DIRECT_NORMAL;
#endif
}

/*****************************************************************************/
//Description:  Set LCD invalidate direction
//Global resource dependence:
//Author: Eddy.Wei
//Note: normal display:SCI_TRUE; assert display:SCI_FALSE
/*****************************************************************************/
LOCAL void  _LCD_SetDirection_DispRot (LCD_ID_E lcd_id,BOOLEAN disp_mode)
{
    if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_SetBrushDirection)
    {
        if(SCI_FALSE == disp_mode)
        {
#ifdef LCD_ROT_MODE_LCD180
            s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation->lcd_SetBrushDirection (LCD_DIRECT_ROT_180);
#else
            s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation->lcd_SetBrushDirection (LCD_DIRECT_NORMAL);
#endif
        }
        else
        {
#ifdef LCD_ROT_MODE_LCD180
            s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation->lcd_SetBrushDirection (LCD_DIRECT_ROT_180);
#else
            s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation->lcd_SetBrushDirection (LCD_DIRECT_NORMAL);
#endif
        }
    }
}

/*****************************************************************************/
//Description:  clear the whole lcd to one color.
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
PUBLIC void LCD_Clear (
    LCD_ID_E lcd_id,     //id of lcd to operate.
    uint32 color        //color to fill the whole lcd.
)
{
    LCD_INFO_T lcd_info;
    uint16     *dest_ptr;
    uint32 i;
    BLOCKCFG_T cfg;

    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/

    LCD_GetInfo (lcd_id,&lcd_info);

    dest_ptr = lcd_info.lcdbuff_ptr;

    for (i = 0; i < lcd_info.lcd_height * lcd_info.lcd_width; i++)
    {
        *dest_ptr++ = color;
    }

    LCDC_AppSetBGColor (color);
    cfg.colorkey         = 0;
    cfg.resolution       = 2;
    cfg.type             = 1;
    cfg.priority         = 0;
    cfg.start_x          = 0;
    cfg.start_y          = 0;
    cfg.end_x            = lcd_info.lcd_width -1;
    cfg.end_y            = lcd_info.lcd_height -1 ;
    cfg.width            =  cfg.end_x - cfg.start_x +1;
    cfg.alpha            = 0xff;
    cfg.alpha_sel        = 1;
    cfg.colorkey_en      = FALSE;
    LCD_ConfigBlock (lcd_id, 0, cfg);

    LCD_DisableAllBlock (lcd_id);
    LCD_SetBlockBuffer (lcd_id, 0, lcd_info.lcdbuff_ptr);

    LCD_Invalidate (lcd_id);

}

/**--------------------------------------------------------------------------*
 **                     Font library used in sci_assert                      *
 **--------------------------------------------------------------------------*/

//
//  8 X 16 ( width * height )
//
static const uint8 ascii_8X16[ 96 * 16] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
    0x00, 0x48, 0x6C, 0x24, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x24, 0x24, 0x24, 0x7F, 0x12, 0x12, 0x12, 0x7F, 0x12, 0x12, 0x12, 0x00, 0x00,
    0x00, 0x00, 0x08, 0x1C, 0x2A, 0x2A, 0x0A, 0x0C, 0x18, 0x28, 0x28, 0x2A, 0x2A, 0x1C, 0x08, 0x08,
    0x00, 0x00, 0x00, 0x22, 0x25, 0x15, 0x15, 0x15, 0x2A, 0x58, 0x54, 0x54, 0x54, 0x22, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0C, 0x12, 0x12, 0x12, 0x0A, 0x76, 0x25, 0x29, 0x11, 0x91, 0x6E, 0x00, 0x00,
    0x00, 0x06, 0x06, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x40, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x40, 0x00,
    0x00, 0x02, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x6B, 0x1C, 0x1C, 0x6B, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x7F, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x04, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x20, 0x20, 0x10, 0x08, 0x04, 0x42, 0x7E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x20, 0x18, 0x20, 0x40, 0x40, 0x42, 0x22, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x20, 0x30, 0x28, 0x24, 0x24, 0x22, 0x22, 0x7E, 0x20, 0x20, 0x78, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7E, 0x02, 0x02, 0x02, 0x1A, 0x26, 0x40, 0x40, 0x42, 0x22, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x38, 0x24, 0x02, 0x02, 0x1A, 0x26, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7E, 0x22, 0x22, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x24, 0x18, 0x24, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x64, 0x58, 0x40, 0x40, 0x24, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x04,
    0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x46, 0x40, 0x20, 0x10, 0x10, 0x00, 0x18, 0x18, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1C, 0x22, 0x5A, 0x55, 0x55, 0x55, 0x55, 0x2D, 0x42, 0x22, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x08, 0x18, 0x14, 0x14, 0x24, 0x3C, 0x22, 0x42, 0x42, 0xE7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1F, 0x22, 0x22, 0x22, 0x1E, 0x22, 0x42, 0x42, 0x42, 0x22, 0x1F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x01, 0x01, 0x01, 0x01, 0x01, 0x42, 0x22, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1F, 0x22, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x22, 0x1F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3F, 0x42, 0x12, 0x12, 0x1E, 0x12, 0x12, 0x02, 0x42, 0x42, 0x3F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3F, 0x42, 0x12, 0x12, 0x1E, 0x12, 0x12, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3C, 0x22, 0x22, 0x01, 0x01, 0x01, 0x71, 0x21, 0x22, 0x22, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x11, 0x0F,
    0x00, 0x00, 0x00, 0x77, 0x22, 0x12, 0x0A, 0x0E, 0x0A, 0x12, 0x12, 0x22, 0x22, 0x77, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x07, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x42, 0x7F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x77, 0x36, 0x36, 0x36, 0x36, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x6B, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xE3, 0x46, 0x46, 0x4A, 0x4A, 0x52, 0x52, 0x52, 0x62, 0x62, 0x47, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3F, 0x42, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x4D, 0x53, 0x32, 0x1C, 0x60, 0x00,
    0x00, 0x00, 0x00, 0x3F, 0x42, 0x42, 0x42, 0x3E, 0x12, 0x12, 0x22, 0x22, 0x42, 0xC7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x02, 0x04, 0x18, 0x20, 0x40, 0x42, 0x42, 0x3E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7F, 0x49, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x22, 0x24, 0x24, 0x14, 0x14, 0x18, 0x08, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x6B, 0x49, 0x49, 0x49, 0x49, 0x55, 0x55, 0x36, 0x22, 0x22, 0x22, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x18, 0x18, 0x18, 0x24, 0x24, 0x42, 0xE7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x77, 0x22, 0x22, 0x14, 0x14, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x7E, 0x21, 0x20, 0x10, 0x10, 0x08, 0x04, 0x04, 0x42, 0x42, 0x3F, 0x00, 0x00,
    0x00, 0x78, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x78, 0x00,
    0x00, 0x00, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x20, 0x40, 0x40,
    0x00, 0x1E, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1E, 0x00,
    0x00, 0x38, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
    0x00, 0x06, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x78, 0x44, 0x42, 0x42, 0xFC, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x1A, 0x26, 0x42, 0x42, 0x42, 0x26, 0x1A, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x02, 0x02, 0x02, 0x44, 0x38, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x60, 0x40, 0x40, 0x40, 0x78, 0x44, 0x42, 0x42, 0x42, 0x64, 0xD8, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x7E, 0x02, 0x02, 0x42, 0x3C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xF0, 0x88, 0x08, 0x08, 0x7E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x22, 0x22, 0x1C, 0x02, 0x3C, 0x42, 0x42, 0x3C,
    0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x3A, 0x46, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x1E,
    0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x72, 0x12, 0x0A, 0x16, 0x12, 0x22, 0x77, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x92, 0x92, 0x92, 0x92, 0x92, 0xB7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x46, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x26, 0x42, 0x42, 0x42, 0x22, 0x1E, 0x02, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x44, 0x42, 0x42, 0x42, 0x44, 0x78, 0x40, 0xE0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x4C, 0x04, 0x04, 0x04, 0x04, 0x1F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x42, 0x02, 0x3C, 0x40, 0x42, 0x3E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x42, 0x42, 0x42, 0x42, 0x62, 0xDC, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x14, 0x08, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEB, 0x49, 0x49, 0x55, 0x55, 0x22, 0x22, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x24, 0x18, 0x18, 0x18, 0x24, 0x6E, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x14, 0x18, 0x08, 0x08, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x22, 0x10, 0x08, 0x08, 0x44, 0x7E, 0x00, 0x00,
    0x00, 0xC0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xC0, 0x00,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x00, 0x06, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x06, 0x00,
    0x0C, 0x32, 0xC2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};

/*****************************************************************************/
//Description:  Set 8 pixels from(x,y) to(x+7,y).
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
LOCAL void LCD_Set8Pixels (uint16 cx, uint16 cy, uint8 bit_mask)
{
    uint16 *buffer_ptr = 0;
    uint16 i;
    LCD_INFO_T  lcd_info;   
    uint16 lcd_width = 0;

    LCD_GetInfo (MAIN_LCD_ID,&lcd_info);
    buffer_ptr = lcd_info.lcdbuff_ptr;

    lcd_width = lcd_info.lcd_width;

    for (i=0; i<8; i++)
    {
        if ( (bit_mask & (0x01<<i)) !=0x00)
        {
            * (buffer_ptr+cy*lcd_width+cx+i) = font_color;    //( cx+i, cy, color);
        }
    }
}

/*****************************************************************************/
//Description:  display the text(assert information) when assert.(just english)
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
PUBLIC void LCD_DisplayText (uint16 x, uint16 y, char *text, uint16 len)
{
    uint16  pos_x = x, pos_y = y;
    uint16  offset, i;
    uint8  *data_ptr;
    char    ch;
    LCD_INFO_T  lcd_info;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;

    s_sys_run_state = SYS_STATE_ASSERT;
    s_is_cap_mode = SCI_FALSE;

    if (s_save_ctrl_info[77]==0)
    {
        SCI_MEMCPY ( (void *) &s_save_ctrl_info[0], (void *)REG_LCDC_REG_BASE,LCDC_REG_NUM*sizeof(uint32)); /*lint !e718 -e718*/
    }

    s_save_ctrl_info[76] = s_lcd_property_table[0].brush_mode;
    s_save_ctrl_info[77] += 1;

    //assert state
    LCD_InitInAssertRestMode();

    LCD_GetInfo (MAIN_LCD_ID,&lcd_info);
    lcd_width = lcd_info.lcd_width;
    lcd_height = lcd_info.lcd_height;

    SCI_MEMSET ( (void *) lcd_info.lcdbuff_ptr,0xf,lcd_width*lcd_height*2); //lint !e746

    if ( (len==0) || (text == NULL))
    {
        return;
    }

    if (x > (lcd_width-8))
    {
        pos_x = 0;
        pos_y += 16;
    }

    if (pos_y > (lcd_height-16))
    {
        return;
    }

    _LCD_SetDirection_DispRot(MAIN_LCD_ID,SCI_FALSE);

    while (len--)
    {
        ch = *text++;

        if ( (ch <= 0x20) || (ch & 0x80))
        {
            offset = 0;  // invalid char, use the default char(' ')
        }
        else
        {
            offset = ( (uint16) ( (uint8) ch - 0x20)) * 16;
        }

        data_ptr = (uint8 *) &ascii_8X16[offset];

        // show this char
        for (i = 0; i < 16; i++)
        {
            LCD_Set8Pixels (pos_x, pos_y+i, *data_ptr++);
        }

        pos_x +=8;

        if ( (pos_x + 8) > lcd_width) // Next line.
        {
            pos_y += 16;
            pos_x  = 0;
        }

        if (pos_y > (lcd_height-16))
        {
            break;
        }
    }

    s_lcd_property_table[MAIN_LCD_ID].lcd_spec.spec_ptr->operation->lcd_InvalidateRect (0,0,lcd_width-1,lcd_height-1);
    LCD_EnableBlock (MAIN_LCD_ID, 0);
    LCDC_AppSetCheckState (TRUE);
    _ConfigBlock (MAIN_LCD_ID,0,0,lcd_width-1,lcd_height-1);
    LCDC_AppInvalidateRect (MAIN_LCD_ID,0,0,lcd_width-1,lcd_height-1);
    GPIO_SetLcdBackLight (SCI_TRUE);/*lint !e718*/
}
/*****************************************************************************/
//Description: configure block parameters
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_ConfigBlock (
                                        LCD_ID_E lcd_id,
                                        uint32 blk_num,
                                        BLOCKCFG_T config
                                       )
{
    SCI_SEMAPHORE_PTR sm = SCI_NULL;
    SYS_STATE_E       flag=0;

    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/
    SCI_PASSERT ( (blk_num<BLOCK_SUM), ("blk_num=%d",blk_num));/*assert verified*/
    if ((config.start_x >= config.end_x) || (config.start_y >= config.end_y))
    {
         return ERR_LCD_PARAMETER_WRONG;
    }

//    flag = LCD_GetRunState();
    sm = s_lcd_property_table[lcd_id].semaphore;
    LCD_GetSemaphore (sm, flag);

    s_buffer_info[lcd_id][blk_num].cfg.alpha = config.alpha;
    s_buffer_info[lcd_id][blk_num].cfg.width = config.width;
    s_buffer_info[lcd_id][blk_num].cfg.type  = config.type;
    s_buffer_info[lcd_id][blk_num].cfg.start_x = config.start_x;
    s_buffer_info[lcd_id][blk_num].cfg.start_y = config.start_y;
    s_buffer_info[lcd_id][blk_num].cfg.end_x = config.end_x;
    s_buffer_info[lcd_id][blk_num].cfg.end_y = config.end_y;
    s_buffer_info[lcd_id][blk_num].cfg.colorkey  = config.colorkey;
    s_buffer_info[lcd_id][blk_num].cfg.resolution = config.resolution;
    s_buffer_info[lcd_id][blk_num].cfg.colorkey_en = config.colorkey_en;
    s_buffer_info[lcd_id][blk_num].cfg.alpha_sel = config.alpha_sel;
    s_buffer_info[lcd_id][blk_num].cfg.rotation = config.rotation;
    s_buffer_info[lcd_id][blk_num].cfg.rb_switch = config.rb_switch;
    s_buffer_info[lcd_id][blk_num].cfg.blend_mode = config.blend_mode;

    rgb_expand_mode = config.rgb_expand_mode;

    if(config.set_endian_flag)
    {
        s_buffer_info[lcd_id][blk_num].cfg.endian_switch = config.endian_switch;
    }
    else
    {
        #ifdef CHIP_ENDIAN_LITTLE    
        switch(config.resolution)
        {
            case LCD_RESOLUTION_RGB565:
            case LCD_RESOLUTION_RGB555:
                s_buffer_info[lcd_id][blk_num].cfg.endian_switch = 2;
                break;
            default:
                s_buffer_info[lcd_id][blk_num].cfg.endian_switch = 0;
                break;
        }    
        #else
        s_buffer_info[lcd_id][blk_num].cfg.endian_switch = 0;
        #endif
    }
    
    s_buffer_info[lcd_id][blk_num].cfg.grey_rgb = config.grey_rgb;

    LCD_PutSemaphore (sm, flag);
    return ERR_LCD_NONE;
}

/*****************************************************************************/
//Description: set display memory
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBlockBuffer (
                                            LCD_ID_E lcd_id,
                                            uint32 blk_num,
                                            uint32 *buf_ptr
                                           )
{
    SCI_SEMAPHORE_PTR sm = SCI_NULL;
    SYS_STATE_E       flag=0;

    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/
    SCI_ASSERT (blk_num < BLOCK_SUM);/*assert verified*/
    SCI_ASSERT (buf_ptr != PNULL);/*assert verified*/

    sm = s_lcd_property_table[lcd_id].semaphore;

    LCD_GetSemaphore (sm, flag);
    s_buffer_info[lcd_id][blk_num].mem_ptr = buf_ptr;
    LCD_PutSemaphore (sm, flag);

    return ERR_LCD_NONE;

}

/*****************************************************************************/
//Description: Enable Block
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_EnableBlock (LCD_ID_E lcd_id, uint32 blk_num)
{
    SCI_SEMAPHORE_PTR sm = SCI_NULL;
    SYS_STATE_E       flag=0;

    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/
    SCI_ASSERT (blk_num<BLOCK_SUM);/*assert verified*/

    sm = s_lcd_property_table[lcd_id].semaphore;

    LCD_GetSemaphore (sm, flag);
    s_buffer_info[lcd_id][blk_num].is_enable = SCI_TRUE;
    LCD_PutSemaphore (sm, flag);

    return ERR_LCD_NONE;
}

/*****************************************************************************/
//Description: get block information
//Global resource dependence:
//Author: Jianping.wang
//Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_GetBlockInfo (LCD_ID_E lcd_id,BLOCK_T *blk_info_ptr)
{
    SCI_SEMAPHORE_PTR sm = SCI_NULL;
    SYS_STATE_E       flag=0;

    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/

    sm = s_lcd_property_table[lcd_id].semaphore;

    LCD_GetSemaphore (sm, flag);
    SCI_MEMCPY ( (void *) blk_info_ptr, (void *) &s_buffer_info[lcd_id][0],LCDC_LAYER_NUM*sizeof (BLOCK_T));
    LCD_PutSemaphore (sm, flag);

    return ERR_LCD_NONE;
}

/*****************************************************************************/
//Description: set LCD background color
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC void LCD_SetBackground (uint32 color)
{
    LCDC_AppSetBGColor (color);
    return;
}

/*****************************************************************************/
//Description: get enable state of the block
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC BOOLEAN LCD_GetBlockIsEnable (LCD_ID_E lcd_id, uint32 blk_num)
{
    BOOLEAN ret = SCI_FALSE;

    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < s_lcd_used_num), ("LCD_GetBlockIsEnable,lcd_id = %d", lcd_id));/*assert verified*/
    }

    SCI_PASSERT ( (blk_num < BLOCK_SUM), ("LCD_GetBlockIsEnable,blk_num = %d", blk_num));/*assert verified*/
   
    ret = s_buffer_info[lcd_id][blk_num].is_enable; /*lint !e662 !e661*/
  
    return ret;

}

/*****************************************************************************/
//  Description:disable block
//  Global resource dependence:
//  Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_DisableBlock (LCD_ID_E lcd_id, uint32 blk_num)
{
    SCI_SEMAPHORE_PTR sm = SCI_NULL;
    SYS_STATE_E       flag=0;

    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/
    SCI_ASSERT (blk_num<BLOCK_SUM);/*assert verified*/

    sm = s_lcd_property_table[lcd_id].semaphore;

    LCD_GetSemaphore (sm, flag);
    s_buffer_info[lcd_id][blk_num].is_enable = SCI_FALSE;
    LCD_PutSemaphore (sm, flag);

    return ERR_LCD_NONE;
}

/*****************************************************************************/
//  Description:disable block
//  Global resource dependence:
//  Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_DisableAllBlock (LCD_ID_E lcd_id)
{
    SCI_SEMAPHORE_PTR sm = SCI_NULL;
    SYS_STATE_E       flag=0;
    uint32 i = 0;

    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/
    sm = s_lcd_property_table[lcd_id].semaphore;

    LCD_GetSemaphore (sm, flag);
    for(i=0 ; i< LCDC_LAYER_NUM ; i++)
    {
        s_buffer_info[lcd_id][i].is_enable = SCI_FALSE;
    }
    LCD_PutSemaphore (sm, flag);

    return ERR_LCD_NONE;
}
/*****************************************************************************/
//Description: get display memory
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC uint32 *LCD_GetBlockBuffer (LCD_ID_E lcd_id, uint32 blk_num)
{
    uint32 *buf_ptr = PNULL;

    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/
    SCI_ASSERT (blk_num < BLOCK_SUM);/*assert verified*/

    buf_ptr = s_buffer_info[lcd_id][blk_num].mem_ptr;

    return buf_ptr;
}

/*****************************************************************************/
//Description: get UV memory
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC uint32 *LCD_GetBlockUVBuffer (LCD_ID_E lcd_id, uint32 blk_num)
{
    uint32 *buf_ptr = PNULL;

    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/
    SCI_ASSERT (blk_num < BLOCK_SUM);/*assert verified*/

    buf_ptr = s_buffer_info[lcd_id][blk_num].uv_addr;

    return buf_ptr;
}

/*****************************************************************************/
//Description: get block parameters
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_GetConfigBlock (
                                                LCD_ID_E lcd_id,
                                                uint32 blk_num,
                                                BLOCKCFG_T *cfg_ptr
                                            )
{
    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/
    SCI_ASSERT (blk_num < BLOCK_SUM);/*assert verified*/
    SCI_ASSERT (cfg_ptr != PNULL);/*assert verified*/

    SCI_MEMCPY (cfg_ptr ,& (s_buffer_info [lcd_id][blk_num].cfg) , sizeof (BLOCKCFG_T)); //lint !e516

    return ERR_LCD_NONE;
}

/*****************************************************************************/
//Description: get lcd number
//Global resource dependence:
//Author: Tim.Zhu
/*****************************************************************************/
PUBLIC uint32 LCD_GetUsedNum (void)
{
    return s_lcd_used_num;
}

/*****************************************************************************/
//Description:    get the main lcd 's important parameter for Digital cammera
//Global resource dependence:
//Author:         Zhemin.lin
/*****************************************************************************/
LOCAL void LCD_PointerSaveInAssertResetMode (void)
{
    uint32 i = 0;

    for (i = 0; i < s_lcd_used_num; i++)
    {
        LCD_EnterSleep (i, SCI_FALSE);
    }

    LCD_SetDirection (MAIN_LCD_ID,LCD_DIRECT_NORMAL);
}

/*****************************************************************************/
//Description: set brush lcd fashion
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBrushMode (LCD_ID_E lcd_id, LCD_BRUSH_MODE_E mode)
{
    SCI_SEMAPHORE_PTR sm = SCI_NULL;
    SYS_STATE_E       flag=0;

    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/

    sm = s_lcd_property_table[lcd_id].semaphore;

    LCD_GetSemaphore (sm, flag);

    if (mode == s_lcd_property_table[lcd_id].brush_mode)
    {
        LCD_PutSemaphore (sm, flag);
        return ERR_LCD_NONE;
    }

    LCD_GetSemaphore (s_lcdc_sm_ptr,flag);

    s_lcd_property_table[lcd_id].brush_mode = mode;
    LCD_PutSemaphore (s_lcdc_sm_ptr,flag);
    LCD_PutSemaphore (sm, flag);

    return ERR_LCD_NONE;

}

/*****************************************************************************/
//Description: get brush lcd fashion
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC LCD_BRUSH_MODE_E LCD_GetBrushMode (LCD_ID_E lcd_id)
{
    SCI_PASSERT ( (lcd_id < s_lcd_used_num), ("LCD_GetBrushMode,lcd_id = %d", lcd_id));/*assert verified*/

    return s_lcd_property_table[lcd_id].brush_mode;
}

/*****************************************************************************/
//Description: Update LCD Timing
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_UpdateTiming (uint32 ahb_clk)
{
    uint32 i;    
    BOOLEAN  lcd_sleep_flag = 0;
    SYS_STATE_E flag=0;   
    LCD_INFO_T  lcd_info;

    SCI_ASSERT ( (ahb_clk > 0) && (ahb_clk<=100000000));/*assert verified*/

    //SCI_TRACE_LOW:"---LCD_UpdateTiming---: AHB Clock:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_1914_112_2_18_1_44_59_32,(uint8*)"d",ahb_clk);

    for (i=0 ; i<s_lcd_used_num ; i++)
    {
        if (s_lcd_property_table[i].sleep_state == SCI_FALSE)
        {
            lcd_sleep_flag = SCI_TRUE;
            break;
        }
    }

    if (lcd_sleep_flag == SCI_FALSE)
    {
        LCD_GetSemaphore (s_lcdc_sm_ptr, flag);
    }

    for (i=0 ; i<s_lcd_used_num ; i++)
    {        
        LCD_GetInfo (i, &lcd_info);        

        LCDC_DrvSetTiming (&s_lcd_property_table[i].lcd_spec,ahb_clk,i);
    }

    if (lcd_sleep_flag == SCI_FALSE)
    {
        LCD_PutSemaphore (s_lcdc_sm_ptr, flag);
    }

    return ERR_LCD_NONE;
}

/*****************************************************************************/
//Description: config capture parameters of 6800h
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_ConfigCapture (LCD_ID_E lcd_id, LCD_CAP_PARAM_T *param_ptr)
{    
    LCD_GetSemaphore (s_lcdc_sm_ptr, 0);
    s_is_cap_mode = param_ptr->cap_en;
    LCDC_AppConfigCapture ( (LCDC_CAP_PARAM_T *) param_ptr);
    LCD_PutSemaphore (s_lcdc_sm_ptr, 0);
    return ERR_LCD_NONE;
}

/*****************************************************************************/
//Description: LCD out data to memory
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_InvalidateRectToMemory (
    LCD_ID_E lcd_id,
    LCD_MEMORY_INFO_T *param_ptr
)
{
    LCD_CAP_PARAM_T cap_param = {0};
    uint16 left = param_ptr->cap_rect.left;
    uint16 top  = param_ptr->cap_rect.top;
    uint16 right  = param_ptr->cap_rect.right;
    uint16 bottom = param_ptr->cap_rect.bottom;

    cap_param.cap_rect.x = left;
    cap_param.cap_rect.y = top;
    cap_param.cap_rect.w = right-left+1;
    cap_param.cap_rect.h = bottom-top+1;
    cap_param.dst_size.w = param_ptr->dst_size.w;
    cap_param.dst_size.h = param_ptr->dst_size.h;
    cap_param.dst_trim_rect.x = param_ptr->dst_trim_rect.left;
    cap_param.dst_trim_rect.y = param_ptr->dst_trim_rect.top;
    cap_param.dst_trim_rect.w = cap_param.cap_rect.w;
    cap_param.dst_trim_rect.h = cap_param.cap_rect.h;
    cap_param.format = param_ptr->data_type;
    cap_param.dst_base_addr = param_ptr->buf_addr;
    cap_param.cap_en = SCI_TRUE;
    LCD_ConfigCapture (lcd_id,&cap_param);
    LCD_InvalidateRect (lcd_id,left,top,right,bottom);
    cap_param.cap_en = SCI_FALSE;
    LCD_ConfigCapture (lcd_id,&cap_param);
    return ERR_LCD_NONE;
}

/*****************************************************************************/
//Description: set alpha buffer address when use pixel alpha ,
//             data format of block is RGB565
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetPixelAlphaBuffer (
    LCD_ID_E lcd_id,
    uint32 blk_num,
    uint32 *buf_ptr
)
{
    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/
    SCI_ASSERT (blk_num == BLOCK_ID_WITH_SINGLE_PIXEL_ALPHA);/*assert verified*/

    s_buffer_info[lcd_id][blk_num].alpha_addr = buf_ptr;

    return ERR_LCD_NONE;
}

/*****************************************************************************/
//Description: get alpha buffer address when use pixel alpha ,
//             data format of block is RGB565
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC uint32 *LCD_GetPixelAlphaBuffer (LCD_ID_E lcd_id, uint32 blk_num)
{
    uint32 *buf_ptr = PNULL;

    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/
    SCI_ASSERT (blk_num == BLOCK_ID_WITH_SINGLE_PIXEL_ALPHA);/*assert verified*/

    buf_ptr = s_buffer_info[lcd_id][blk_num].alpha_addr;

    return buf_ptr;
}

/*****************************************************************************/
//Description: set UV buffer for block 0
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetUVBuffer (
    LCD_ID_E lcd_id,
    uint32 blk_num,
    uint32 *buf_ptr
)
{
    SCI_ASSERT (lcd_id < s_lcd_used_num);/*assert verified*/
    SCI_ASSERT (blk_num == BLOCK_IMAGE_ID);/*assert verified*/

    s_buffer_info[lcd_id][blk_num].uv_addr = buf_ptr;

    return ERR_LCD_NONE;
}

/*****************************************************************************/
//Description: set fmark mode
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetFmarkMode (
                                            LCD_ID_E lcd_id,
                                            LCD_FMARK_MODE_E fmark_mode,
                                            LCD_FMARK_POL_E fmark_pol
                                          )
{
    SYS_STATE_E flag = 0;

    LCD_GetSemaphore (s_lcdc_sm_ptr, flag);
    LCDC_AppSetFmark ((uint32)lcd_id,fmark_mode, fmark_pol);
    LCD_PutSemaphore (s_lcdc_sm_ptr, flag);
    return ERR_LCD_NONE;
}

/**--------------------------------------------------------------------------*
 **                         PUBLIC INTERFACE FOR CUSTOMIZE MODULE            *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
//Description:Read data from lcm
//Global resource dependence: NONE
//Author: Younger.yang
/*****************************************************************************/
PUBLIC uint32 LCD_ReadRegVal (uint32 lcd_id, uint32 reg_addr)
{
    uint32 reg_val = 0;
    
    reg_val = LCDC_Read (lcd_id,reg_addr);

    return reg_val;
}

/*****************************************************************************/
//Description:
//Global resource dependence: NONE
//Author: Younger.yang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SimpleInit (LCD_SIMPLE_INIT_PARAM_T *param_ptr)
{
    if (param_ptr == PNULL)
    {
        return ERR_LCD_PARAMETER_WRONG;
    }

    if(!BOOT_GetBLStatus())  //bl is turn off
    {
        LCDC_DrvResetLcd (DEFAULT_RESET_TIME,DEFAULT_RESET_LEVEL);
    }

    LCDC_DrvSetCSAddr (param_ptr->cs_id,param_ptr->cd_pol,param_ptr->lcd_id);
    LCDC_AppSetCSPin (param_ptr->lcd_id,param_ptr->cs_id);

    LCDC_AppOpen();
    LCDC_AppSetFmark (param_ptr->lcd_id,LCD_FMARK_NONE, LCD_FMARK_HIGH);
    LCDC_DrvSetTimingInit();
    LCDC_DrvSetRGBMode (param_ptr->cs_id,param_ptr->lcd_bits);
    
    return ERR_LCD_NONE;
}

/**********************************************************************************************************/
//Description: send the command and data to LCD
//Global resource dependence: g_lcdc_mode
//Author: Jianping.wang
/**********************************************************************************************************/
PUBLIC int32 LCD_SendCmdData (uint32 cmd,uint32 lcd_data,uint32 lcd_id)
{
    LCDC_SendCmdData (cmd, lcd_data, lcd_id);

    return ERR_LCD_NONE;
}

/**********************************************************************************************************/
//Description: send command to lcd
//Global resource dependence: g_lcdc_mode
//Author: Jianping.wang
/**********************************************************************************************************/
PUBLIC int32 LCD_SendCmd (uint32 cmd,uint32 lcd_id)
{
    LCDC_SendCmd (cmd, lcd_id);

    return ERR_LCD_NONE;
}

/**********************************************************************************************************/
//Description: send display data to LCD
//Global resource dependence: g_lcdc_mode
//Author: Jianping.wang
/**********************************************************************************************************/
PUBLIC int32 LCD_SendData (uint32 lcd_data,uint32 lcd_id)
{
    LCDC_SendData (lcd_data, lcd_id);

    return ERR_LCD_NONE;

}

/*****************************************************************************/
//Description: LCD initialization when assert
//Global resource dependence:
//Author: Tim.Zhu
/*****************************************************************************/
PUBLIC void LCD_Reset (void)
{
	if(SUB_LCD_ID == s_lcd_reset_by_lcd_id)
		return;
    if ( (PNULL == s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation->lcd_Rst) ||
            (PNULL == s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation))
    {

        LCDC_DrvResetLcd (DEFAULT_RESET_TIME,DEFAULT_RESET_LEVEL);
    }
    else if (PNULL != s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation->lcd_Rst)
    {
        s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation->lcd_Rst();
    }
}

/*****************************************************************************/
//Description:    LCD delay
//Global resource dependence:
//Author:
/*****************************************************************************/
PUBLIC void LCD_Delayms (uint32 ms)
{
    if (SCI_InThreadContext())
    {
        SCI_Sleep (ms);
    }
    else
    {
        SCI_Sleep (ms);
    }
}
/******************************************************************************/
// Description:register callback when invalidate done
// Author: Jianping.Wang
/******************************************************************************/
PUBLIC void LCD_RegCallback(lcd_invalidate_done_func_t func)
{
    s_lcddone_callback = func;
}
/******************************************************************************/
// Description:register callback when invalidate done
// Author: Jianping.Wang
/******************************************************************************/
PUBLIC void LCD_UnRegCallback(void)
{
    s_lcddone_callback = PNULL;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

/* end of lcd.c*/

