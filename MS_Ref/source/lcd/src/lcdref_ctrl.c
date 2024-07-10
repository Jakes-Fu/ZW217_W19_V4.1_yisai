/******************************************************************************
 ** File Name:     lcdref_ctrl.c                                              *
 ** Description:                                                              *
 ** This file contains display contrl on the lcd                              *
 ** Author:         Jianping.Wang                                             *
 ** DATE:           26/5/2010                                                 *
 ** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 26/5/2009      Jianping.Wang    Create.                                   *
 ******************************************************************************/

 /**--------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_ref_lcd_trc.h"
#include "lcd_ctrl.h"
#include "lcdc_drv.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         local variable define                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 s_debug_info[2] = {0};/*lint !e551*/

/**---------------------------------------------------------------------------*
 **                         local function define                             *
 **---------------------------------------------------------------------------*/
extern void* DCAMERA_CaptureGetOSDBuf(uint32* p_buf_len);


/*****************************************************************************/
//  Description:  setting if gui is permitted to invalidate lcd
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
ERR_LCD_E LCD_SetInvalidateLimit (
                                       LCD_ID_E lcd_id,
                                       LCD_LIMIT_TYPE_E limit_type
                                       )
{
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL;

    if(lcd_id>(SUPPORT_LCD_NUM-1))
    {
        return ERR_LCD_PARAMETER_WRONG;
    }

    s = _get_ctrl_info();

    LCD_GetLcdCtrlSemaphore(lcd_id);

    s->limit_type [lcd_id] = limit_type;

    LCD_PutLcdCtrlSemaphore(lcd_id);

    return ERR_LCD_NONE;
}
/*****************************************************************************/
//  Description: settting buffer for display
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
ERR_LCD_E LCD_SetDisplayBuffer (
                                      LCD_ID_E  lcd_id,
                                      uint32 buf_sum,
                                      LCD_BUF_INFO_T buf_array[]
                                      )
{
#if 0
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL;
    uint32 i = 0;
    LCD_INFO_T lcd_info = {0};

    s = _get_ctrl_info();

    if(!s->is_init)
    {
        _display_init();
    }

    LCD_GetLcdCtrlSemaphore(lcd_id);

    LCD_GetInfo(lcd_id, &lcd_info);

    //SCI_TRACE_LOW:"LCD_SetDisplayBuffer:lcd_id=%d,sum=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDREF_CTRL_90_112_2_18_1_45_44_184,(uint8*)"dd",lcd_id,buf_sum);

    buf_sum = (SUPPORT_MENU_NUM<buf_sum) ? SUPPORT_MENU_NUM : buf_sum;
    if(buf_array != SCI_NULL)
    {
        for( i=0 ; i<buf_sum ; i++)
        {
            s->menu_buffer_dc[lcd_id][i] = buf_array[i];
            s->menu_buffer_dc[lcd_id][i].width  = lcd_info.lcd_width;
            s->menu_buffer_dc[lcd_id][i].height = lcd_info.lcd_height;
        }
        s->menu_buf_sum_dc[lcd_id] = buf_sum;
        s->full_copy_flag[lcd_id] = 1;
    }
    else
    {
        SCI_MEMSET(&s->menu_buffer_dc[lcd_id][0],0,SUPPORT_MENU_NUM*sizeof(LCD_BUF_INFO_T));
        s->menu_buf_sum_dc[lcd_id] = 0;
        //SCI_TRACE_LOW:"LCD_SetDisplayBuffer:buf_array is NULL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDREF_CTRL_108_112_2_18_1_45_44_185,(uint8*)"");
    }
    LCD_PutLcdCtrlSemaphore(lcd_id);
#endif
    return ERR_LCD_NONE;
}
/*****************************************************************************/
//  Description: set if use display control semaphore
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_SetIsUseCtrlSm(BOOLEAN is_use)
{
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL;

    s = _get_ctrl_info();

    s->is_used = is_use;
}
/*****************************************************************************/
//  Description: get lcd display control semaphor
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_GetLcdCtrlSemaphore(LCD_ID_E lcd_id)
{
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL;

    if(!SCI_InThreadContext())
    {
        return;
    }

    s = _get_ctrl_info();

    SCI_PASSERT(( PNULL != s->lcd_ctrl_sm_ptr[0] ),("lcd_ctrl_sm_ptr is PNULL, %d!,lcd_id"));/*assert verified*/
    if( SCI_ERROR == SCI_GetSemaphore(s->lcd_ctrl_sm_ptr[0], SCI_WAIT_FOREVER) )
    {
        //SCI_TRACE_LOW:"--LCDGUI:Get s_lcd_ctrl_sm_ptr Semaphore Fail!--"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDREF_CTRL_144_112_2_18_1_45_44_186,(uint8*)"");
    }
    s_debug_info[0] = SCI_IdentifyThread();
    s_debug_info[1]++;
    s->is_used = SCI_TRUE;
}
/*****************************************************************************/
//  Description: put lcd display control semaphor
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_PutLcdCtrlSemaphore(LCD_ID_E lcd_id)
{
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL;
    uint32 ret = 0;

    if(!SCI_InThreadContext())
    {
        return;
    }

    s = _get_ctrl_info();

    SCI_PASSERT(( PNULL != s->lcd_ctrl_sm_ptr[0] ),("lcd_ctrl_sm_ptr is PNULL, %d!,lcd_id"));/*assert verified*/
    if(s->is_use_ctrl_event)
    {
        _set_ctrl_event(LCD_CTRL_EVENT);
    }
    if(s->is_used != SCI_TRUE)
    {
       return;
    }
    s_debug_info[0] = 0;
    s_debug_info[1]--;
    s->is_used = SCI_FALSE;
    ret = SCI_PutSemaphore( s->lcd_ctrl_sm_ptr[0] );
    if(ret != SCI_SUCCESS)
    {
        //SCI_TRACE_LOW:"--LCDGUI:Put s_lcd_ctrl_sm_ptr Semaphore Fail!--"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDREF_CTRL_181_112_2_18_1_45_44_187,(uint8*)"");
    }

}
/*****************************************************************************/
//  Description:  invalidate lcd for dislapy image
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_InvalidateRectEx(
                                          LCD_ID_E  lcd_id,
                                          DISPLAY_LAYER_CFG_T *layer_arr[],
                                          uint32 layer_num,
                                          DISPLAY_MEMORY_INFO_T  *mem_info_ptr,
                                          LCD_BRUSH_MODE_E brush_mode,
                                          DISPLAY_RECT_T *rect_ptr
                                         )
{
#ifndef WIN32
    DISPLAY_LAYER_INFO_T *blk_info_ptr = PNULL;
    BLOCKCFG_T cfg = {0};
    uint32 blk_num = 0;
    uint32 byte_num = 0;
    LCD_MEMORY_INFO_T lcd_mem_info = {0};
    uint32 i=0;
    uint16 set_endian_flag = 0;
    uint16 endian_mode = 0;

    if(mem_info_ptr!=PNULL)
    {
        if((mem_info_ptr->dst_trim_rect.top<0) || (mem_info_ptr->dst_trim_rect.left<0))
        {
            //SCI_TRACE_LOW:"LCD_InvalidateRectEx:[error] dst_trim_rect.top=%d,dst_trim_rect.left=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDREF_CTRL_213_112_2_18_1_45_44_188,(uint8*)"dd", mem_info_ptr->dst_trim_rect.top,mem_info_ptr->dst_trim_rect.left);
            return ERR_LCD_PARAMETER_WRONG;
        }
    }
    for(i=0;i<layer_num;i++)
    {
        blk_info_ptr = &(layer_arr[i]->layer_data);
        set_endian_flag = layer_arr[i]->set_endian_flag;
        endian_mode = layer_arr[i]->endian_mode;
        SCI_PASSERT((blk_info_ptr!=0),("LCD_InvalidateRectEx layer%d is NULL!",i));/*assert verified*/
        if((blk_info_ptr->is_enable)&&(layer_arr[i]->is_set_layer))
        {
            blk_num = blk_info_ptr->layer_id;
            _convert_cfg(blk_info_ptr,&cfg);
            cfg.set_endian_flag = set_endian_flag;
            cfg.endian_switch = (uint8)endian_mode;
            LCD_ConfigBlock(lcd_id, blk_num, cfg);
            LCD_SetBlockBuffer(lcd_id, blk_num, (uint32 *)blk_info_ptr->buf_addr);
            LCD_SetUVBuffer(lcd_id,blk_num,(uint32*)blk_info_ptr->uv_buf_addr);
            LCD_EnableBlock(lcd_id, blk_num);
        }
    }
    LCD_SetBrushMode(lcd_id, brush_mode);
    if(mem_info_ptr != PNULL)
    {
        byte_num = _get_byte_num(mem_info_ptr->data_type);
        lcd_mem_info.buf_addr = (uint32)(mem_info_ptr->buf_addr + (mem_info_ptr->dst_trim_rect.top+mem_info_ptr->dst_trim_rect.left)*mem_info_ptr->dst_size.w*byte_num);/*lint !e737*/
        lcd_mem_info.cap_rect.left  = rect_ptr->left;
        lcd_mem_info.cap_rect.top   = rect_ptr->top;
        lcd_mem_info.cap_rect.right = rect_ptr->right;
        lcd_mem_info.cap_rect.bottom = rect_ptr->bottom;
        lcd_mem_info.data_type = _cap_data_format_convert(mem_info_ptr->data_type);
        lcd_mem_info.dst_size.w = mem_info_ptr->dst_size.w;
        lcd_mem_info.dst_size.h = mem_info_ptr->dst_size.h;
        lcd_mem_info.dst_trim_rect.left   = mem_info_ptr->dst_trim_rect.left;
        lcd_mem_info.dst_trim_rect.top    = mem_info_ptr->dst_trim_rect.top;
        lcd_mem_info.dst_trim_rect.right  = mem_info_ptr->dst_trim_rect.right;
        lcd_mem_info.dst_trim_rect.bottom = mem_info_ptr->dst_trim_rect.bottom;
        LCD_InvalidateRectToMemory(lcd_id,&lcd_mem_info);
    }
    else
    {
        LCD_InvalidateRect(lcd_id,rect_ptr->left,rect_ptr->top,rect_ptr->right,rect_ptr->bottom);
    }
#endif
    return ERR_LCD_NONE;
}

/*****************************************************************************/
//  Description:  set operation mode of invalidate lcd
//  Global resource dependence:
//  Author: Eddy.wei
//  Note:
/*****************************************************************************/
LOCAL ERR_LCD_E _LCD_ModeSwitch(LCD_ID_E lcd_id,uint32 buf_addr, uint32 buf_len)
{
    ERR_LCD_E ret = ERR_LCD_NONE;
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL;
    BLOCK_T blk_info[HW_BLOCK_NUM] = {0};
    uint32 i = 0,layer_number = 0, layer_index= 0;

    s = _get_ctrl_info();

    LCD_GetBlockInfo(lcd_id,&blk_info[0]);
    for(i=1;i<HW_BLOCK_NUM;i++)
    {
        if(blk_info[i].is_enable)
        {
            layer_number ++ ;
            layer_index = i;
        }
    }

    if(layer_number == 1)
    {
        LCD_SetBlockBuffer(lcd_id, layer_index,(uint32*)buf_addr);
        LCD_EnableBlock(lcd_id, layer_index);
    }
    else
    {
        SCI_TRACE_LOW("[_LCD_ModeSwitch] layer_number : %d ", layer_number);

        return ERR_LCD_OPERATE_FAIL;
    }

    if(s->menu_buffer_info[lcd_id][0].addr && buf_addr)
    {
        SCI_MEMCPY((void *)buf_addr, (void *)s->menu_buffer_info[lcd_id][0].addr, buf_len);
    }

    return ret;
}

/*****************************************************************************/
//  Description:  set operation mode of invalidate lcd
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetOpMode(LCD_ID_E lcd_id,DISPLAY_OPERATION_MODE_E op_mode)
{
    ERR_LCD_E ret = ERR_LCD_NONE;
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL;
    LCD_INFO_T lcd_info = {0};

    s = _get_ctrl_info();

    if(lcd_id>=LCD_SUPPORT_MAX)
    {
        return ERR_LCD_PARAMETER_WRONG;
    }

    LCD_GetLcdCtrlSemaphore(lcd_id);

    if(op_mode == OSD_OPERATION)
    {
        s->full_copy_flag[lcd_id] = 1;/*lint !e662 !e661*/

        if(SCI_FALSE == s->menu_buffer_info[lcd_id][0].is_alloc)
        {
            uint32 i=0;
            uint32 size = 0;
            uint32 alloc_addr;

            LCD_GetInfo(lcd_id, &lcd_info);
            #ifdef MOCOR_SUPPORT_ARGB888_MODE
            size = (uint32)lcd_info.lcd_width* lcd_info.lcd_height * 4;
            #else
            size = (uint32)lcd_info.lcd_width* lcd_info.lcd_height * 2;
            #endif
            alloc_addr = (uint32)SCI_ALLOCA(size + 256);

            if(PNULL ==  alloc_addr)
            {
                SCI_TRACE_LOW("[LCD_SetOpMode] can not alloc memory:  size: %d ", size);

                LCD_PutLcdCtrlSemaphore(lcd_id);
                return ERR_LCD_OPERATE_FAIL;
            }

            s->menu_buffer_info[lcd_id][0].alloc_addr = alloc_addr;
            s->menu_buffer_info[lcd_id][0].is_alloc = SCI_TRUE;
            size = (size + 31)/32*32;
            alloc_addr = (alloc_addr + 31)/32*32;

            if(OSD_OPERATION_SPECIAL== s->previous_mode[lcd_id])
            {
                if(ERR_LCD_NONE != _LCD_ModeSwitch(lcd_id,alloc_addr,size))
                {
                    LCD_PutLcdCtrlSemaphore(lcd_id);
                    return ERR_LCD_OPERATE_FAIL;
                }
            }

            for(i=0; i<1; i++ )
            {
                s->menu_buffer_info[lcd_id][i].addr = alloc_addr;
                s->menu_buffer_info[lcd_id][i].size = size;
                s->menu_buffer_info[lcd_id][i].width = lcd_info.lcd_width;
                s->menu_buffer_info[lcd_id][i].height = lcd_info.lcd_height;
                alloc_addr = alloc_addr + size;
            }

            s->previous_mode[lcd_id] = OSD_OPERATION;
        }
    }
    else if(op_mode == OSD_OPERATION_SPECIAL)
    {
        uint32 alloc_size = 0;
        uint32 alloc_addr = 0;
        uint32 size = 0;

        LCD_GetInfo(lcd_id, &lcd_info);
        size = (uint32)lcd_info.lcd_width* lcd_info.lcd_height * 2 + 64;
        alloc_addr = (uint32)DCAMERA_CaptureGetOSDBuf(&alloc_size);

        if(PNULL == alloc_addr || alloc_size < size)
        {
            SCI_TRACE_LOW("[LCD_SetOpMode] OSD Buffer is NULL");

            LCD_PutLcdCtrlSemaphore(lcd_id);
            return ERR_LCD_OPERATE_FAIL;
        }

        if((SCI_TRUE == s->menu_buffer_info[lcd_id][0].is_alloc) && (s->menu_buffer_info[lcd_id][0].alloc_addr) && (OSD_OPERATION == s->previous_mode[lcd_id]))
        {
            size = (lcd_info.lcd_width* lcd_info.lcd_height * 2 + 31)/32*32;
            alloc_addr = (alloc_addr + 31)/32*32;

            if(ERR_LCD_NONE != _LCD_ModeSwitch(lcd_id,alloc_addr,size))
            {
                LCD_PutLcdCtrlSemaphore(lcd_id);
                return ERR_LCD_OPERATE_FAIL;
            }

            SCI_Free((uint8 *)s->menu_buffer_info[lcd_id][0].alloc_addr);
            s->menu_buffer_info[lcd_id][0].alloc_addr = 0;

            s->menu_buffer_info[lcd_id][0].addr       = alloc_addr;
            s->menu_buffer_info[lcd_id][0].size       = size;
            s->menu_buffer_info[lcd_id][0].width      = lcd_info.lcd_width;
            s->menu_buffer_info[lcd_id][0].height     = lcd_info.lcd_height;
            s->menu_buffer_info[lcd_id][0].is_alloc   = SCI_FALSE;


            s->previous_mode[lcd_id] = OSD_OPERATION_SPECIAL;
        }
        else
        {
             SCI_TRACE_LOW("[LCD_SetOpMode] OSD_OPERATION_SPECIAL Fail");

             LCD_PutLcdCtrlSemaphore(lcd_id);
             return ERR_LCD_OPERATE_FAIL;
        }
    }
    else
    {
#ifndef PLATFORM_UWS6121E
        //uix8910 rotation has no buf , need rotation buf , do not free.
        if((SCI_TRUE == s->menu_buffer_info[lcd_id][0].is_alloc) && (0 != s->menu_buffer_info[lcd_id][0].alloc_addr))
        {
            //SCI_TRACE_LOW:"[DISPLAY] Free memory: addr: 0x%x "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCDREF_CTRL_288_112_2_18_1_45_44_189,(uint8*)"d", s->menu_buffer_info[lcd_id][0].alloc_addr );

            SCI_Free((uint8 *)s->menu_buffer_info[lcd_id][0].alloc_addr);
        }

        s->menu_buffer_info[lcd_id][0].alloc_addr = 0;
        s->menu_buffer_info[lcd_id][0].addr = 0;
        s->menu_buffer_info[lcd_id][0].is_alloc = SCI_FALSE;
#endif

        LCD_DisableAllBlock(lcd_id);

        s->previous_mode[lcd_id] = UI_OPERATION;
    }

    s->op_mode[lcd_id] = op_mode;

    LCD_PutLcdCtrlSemaphore(lcd_id);

    return ret;

}

/*****************************************************************************/
//  Description:  get operation mode of invalidate lcd
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC DISPLAY_OPERATION_MODE_E LCD_GetOpMode(LCD_ID_E lcd_id)
{
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL;

    s = _get_ctrl_info();

    return   s->op_mode[lcd_id];

}
/*****************************************************************************/
//  Description:  get if is osd mode
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN LCD_IsOSDMode(LCD_ID_E lcd_id)
{
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL;
    BOOLEAN ret = FALSE;

    s = _get_ctrl_info();

    LCD_GetLcdCtrlSemaphore(lcd_id);

    if(OSD_OPERATION == s->op_mode[lcd_id])
    {
        ret = TRUE;
    }

    LCD_PutLcdCtrlSemaphore(lcd_id);

    return ret;

}
/*****************************************************************************/
//  Description:  set osd copy mode
//  Global resource dependence:
//  Author: Eddy.wei
//  Note:
/*****************************************************************************/
PUBLIC uint32 LCD_SetCopyOsdMode(LCD_ID_E lcd_id,uint32 is_copy_osd)
{
    LCD_DISPLAY_CTRL_INFO_T *s = PNULL;
    BOOLEAN ret = SCI_TRUE;

    s = _get_ctrl_info();

    LCD_GetLcdCtrlSemaphore(lcd_id);

    s->is_copy_osd[lcd_id] = is_copy_osd;

    LCD_PutLcdCtrlSemaphore(lcd_id);

    return ret;

}
/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                                                       *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

