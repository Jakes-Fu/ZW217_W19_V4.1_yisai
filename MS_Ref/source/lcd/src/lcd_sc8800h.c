/******************************************************************************
 ** File Name:     lcd.c                                                      *
 ** Description:                                                              *
 **    This file contains common driver for lcd modul                         *
 ** Author:         Jim zhang                                                 *
 ** DATE:           05/18/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/18/2003     Jim.zhang        Create.
 ******************************************************************************/

#include "ms_ref_lcd_trc.h"
#include "sci_types.h"
#include "dal_lcd.h"
#include "os_api.h"
#include "sc_reg.h"
#include "lcm_drv.h"
#include "lcm_prod.h"
#include "lcdc_data_type.h"
#include "lcdc_common.h"
#include "lcd_controller.h"
#include "mem_prod.h"
#include "chip.h"
#include "arm_mmu.h"
#include "ref_outport.h"
#include "deep_sleep.h"
#include "chng_freq.h"
#include "arm_scaledown.h"
#include "task_monitor.h"
#include "gpio_prod_api.h"
#include "isp_service.h"
#include "display.h"
#include "ref_lcd.h"

#ifdef PLATFORM_SC6600V
#include "Lcd_tv.h"
#endif

#include "lcd_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         macro
 **---------------------------------------------------------------------------*/
#define BLOCK_GROUP_NUM         3


#define ENABLE_BLOCK            1
#define DISABLE_BLOCK           0
#define MEMORY_ALIGN_BENCHMARK  32
#define LCDC_PAL_MODE           1
#define LCDC_NTSC_MODE          0
#define LCDC_BLOCK_NUM           6
#define LCD_BLOCK_NUM_DEFAULT    1
#define LCDC_UPDATE_PIXELS_MIN  64


#define LCD_SEMAPHORE_PTR       SCI_SEMAPHORE_PTR;
#define LCD_BRUSH_TIME_OUT      1000
#define LCDC_LCD_SIZE_WIDTH_MAX     4096
#define LCDC_LCD_SIZE_HEIGHT_MAX    4096

#define _DEBUG_LCDC 1
#define IS_USE_SCALING_FUNC  0
#define REG32(x)      (*((volatile uint32 *)(x)))

#if _DEBUG_LCDC
#define LCDC_REG_NUM        25
#define LCDC_MCU_REG_NUM    13
#endif
#define LCD_DONE_EVENT		1
LOCAL uint32 s_debug_lcd_info[2][2] = {0};
LOCAL BOOLEAN s_is_check = SCI_FALSE;
/**---------------------------------------------------------------------------*
 **                         data structure
 **---------------------------------------------------------------------------*/

typedef struct _lcd_operation_ctrl_tag
{
    uint16            left;
    uint16            top;
    uint16            right;
    uint16            bottom;    
    SCI_SEMAPHORE_PTR sm;
    LCD_ANGLE_E       angle;
    BOOLEAN           is_full;
    BOOLEAN           work_flag; //TRUE: whole LCD ; FALSE : Rectagle LCD
    uint8             padding0;//reserced
    uint8             padding1;//reserved
} LCD_OPERATION_CTRL_T;

typedef struct _blk_position_tag
{
    uint16  left;
    uint16  top;
    uint16  right;
    uint16  bottom;
    BOOLEAN work_flag;//TRUE: whole LCD ; FALSE : Rectagle LCD
    uint8   padding0;//reserced
    uint8   padding1;//reserved
    uint8   padding2;//reserved
} BLOCK_POSITION_T;

/**---------------------------------------------------------------------------*
 **                         variables
 **---------------------------------------------------------------------------*/
LOCAL SYS_STATE_E s_sys_run_state = SYS_STATE_NORMAL;
LOCAL LCD_PROPERTY_T g_lcd_property_table [ (LCD_SUM+1) ];
LOCAL  LCD_SPEC_INFO_T_PTR  s_lcd_spec_info_ptr[ (LCD_SUM+1) ];
LOCAL BLOCK_GROUP_T g_block[BLOCK_GROUP_NUM];
LOCAL uint32 g_lcd_used_num = 0;
LOCAL uint32 g_lcdc_is_used = 0;

LOCAL SCI_SEMAPHORE_PTR    g_lcdc_sm_ptr;
LOCAL SCI_SEMAPHORE_PTR    g_tv_sm_ptr;
LOCAL SCI_SEMAPHORE_PTR    g_lcd_sm_ptr[LCD_SUM];
LOCAL uint32 g_lcd_index[2] = {0};
LOCAL BOOLEAN s_use_lcd_scan_sync_flag[LCD_SUM] = {SCI_FALSE};
LOCAL BOOLEAN s_fmark_pin_state[LCD_SUM] = {SCI_FALSE};
LOCAL   SCI_EVENT_GROUP_PTR  s_lcd_event = PNULL;
//Scaling down
#define TV_ROW_PIXELS       216
#define TV_COLUMN_PIXELS    288
#if IS_USE_SCALING_FUNC
__align (32) uint16 tv_0_memory[3][TV_ROW_PIXELS *TV_COLUMN_PIXELS];
__align (32) uint16 tv_1_memory[3][TV_ROW_PIXELS *TV_COLUMN_PIXELS];
#endif
LOCAL lcd_invalidate_done_func_t s_lcd_register_callback;
LOCAL lcd_invalidate_done_func_t s_lcddone_callback = PNULL;
/*lint -e64*/
extern void UpdateLcdCtrl_NoticeLcdDone (void);

//Scaling down
/**---------------------------------------------------------------------------*
 **                         extern interfaces
 **---------------------------------------------------------------------------*/
extern unsigned long _tx_thread_system_state;
extern uint16 s_lcdc_blk_buffer[];
extern uint32 LCD_GetUsedNum (void);
extern void LCD_GetRegTable (LCD_ID_E lcd_id, LCD_SPEC_T *ptr);
extern void LCD_SetBuffer (LCD_ID_E lcd_id, uint32 *ptr);
extern void LCD_AutoIdentify (void);
extern PUBLIC void LCDC_SetDisplayWindows (uint16 left,
        uint16 top,
        uint16 right,
        uint16 bottom);

extern uint32 LCDC_GetClock (void);
extern const uint32    g_mem_limit;
LOCAL uint32 g_lcdc_tv_flag = 0;
LOCAL void LCD_FreeSemaphore (void);
LOCAL void LCD_DisplayOn (void);
PUBLIC ERR_LCD_E LCD_UpdateTiming (uint32 ahb_clk);
#if _DEBUG_LCDC
PUBLIC uint32 g_lcdc_reg_info[LCDC_REG_NUM];
PUBLIC uint32 g_lcdc_mcu_reg_info[LCDC_MCU_REG_NUM];
PUBLIC uint32 g_clock[8];
PUBLIC uint32 g_lcd_start_time = 0x55ff;
PUBLIC uint32 g_lcd_end_time   = 0xff55;
#endif
extern uint32 LCDC_GetTvMode (void);
extern PUBLIC void LCD_SetIsOpenFmarkFunc (LCD_ID_E lcd_id, BOOLEAN is_open);

LOCAL void LCD_GetSemaphore (SCI_SEMAPHORE_PTR sm, SYS_STATE_E flag);
LOCAL void LCD_PutSemaphore (SCI_SEMAPHORE_PTR sm, SYS_STATE_E flag);
LOCAL uint32 _LCD_GetEvent(uint32 requested_flags, uint32 wait_option);
LOCAL void _LCD_Clearevent(void);
LOCAL void LCD_SetSyncEvent (void);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL void LCD_RecordSemaphore (LCD_ID_E lcd_id, SCI_SEMAPHORE_PTR lcd_sm);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL void LCD_RecordLcdIndex (LCD_CTRL_MODULE_E ctrl_mode, uint32 lcd_id);
/**---------------------------------------------------------------------------*
 **                         implementation
 **---------------------------------------------------------------------------*/
PUBLIC void LCD_Delayms (uint32 ms)
{
    if (SCI_InThreadContext())
    {
        SCI_Sleep (ms);
    }
    else
    {
        OS_TickDelay (ms);
    }
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL void _SetCheckFlag(BOOLEAN is_check)
{
    s_is_check = is_check;
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _GetCheckFlag(void)
{
    return s_is_check;
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL ERR_LCD_E ProtectionOperation (LCD_ID_E lcd_id)
{
    SCI_SEMAPHORE_PTR sm = NULL;
    uint32            ctrl_mode = 0;

    if (lcd_id == TV_ID)
    {       
        return ERR_LCD_FUNC_NOT_SUPPORT;
    }

    SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("ProtectionOperation,lcd_id = %d", lcd_id));
    ctrl_mode = g_lcd_property_table[lcd_id].lcd_spec.controller;

    if ( (g_lcdc_tv_flag == 1) &&
            ( (LCD_CTRL_MODULE_LCDC == ctrl_mode)
              || (LCD_CTRL_MODULE_LCDC_LCM0 == ctrl_mode)))
    {
        return ERR_LCD_FUNC_NOT_SUPPORT;
    }

    MMU_InvalideDCACHE ();



    sm = g_lcd_property_table[lcd_id].semaphore;
    LCD_GetSemaphore (sm, SYS_STATE_NORMAL);

    if ( (LCD_CTRL_MODULE_LCDC == ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == ctrl_mode))
    {
        LCD_GetSemaphore (g_lcdc_sm_ptr, SYS_STATE_NORMAL);
    }

    if (g_lcd_property_table[lcd_id].sleep_state == SCI_TRUE)
    {
        if ( (LCD_CTRL_MODULE_LCDC == ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == ctrl_mode))
        {
            LCD_PutSemaphore (g_lcdc_sm_ptr, SYS_STATE_NORMAL);
        }

        LCD_PutSemaphore (sm, SYS_STATE_NORMAL);
        LCD_PutLcdCtrlSemaphore(lcd_id);
        //SCI_TRACE_LOW:"ProtectionOperation:err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC8800H_259_112_2_18_1_45_18_106,(uint8*)"");
        return ERR_LCD_OPERATE_FAIL;
    }

    g_lcd_property_table[lcd_id].state = LCD_BUSY;
    SCI_LCDBacklightBrightless_EnableDeepSleep (DISABLE_DEEP_SLEEP);

    DPSLP_Lcdc_EnableAhbSleep (DISABLE_AHB_SLEEP);
    CHNG_FREQ_Event_Clr (FREQ_CHNG_EVENT_LCDC,SYS_MODE_NORMAL);

    return ERR_LCD_NONE;
}


#if defined CHIP_ENDIAN_LITTLE && defined SC8800S_LITTLE_ENDIAN

LOCAL uint32 s_buffer_index = 0;

//define word endian swap
#define WORD_ENDIAN_SWAP(word) (((word & 0xFFFF)<<16) |((word & 0xFFFF0000) >>16))

__align (16) LOCAL uint16 s_buffer[3][240*320];

void _convert_data_format (uint32 dst_addr,uint32 src_addr,LCD_BLOCK_T *blk)
{
    int32 i = 0;
    int32 j = 0;
    uint32 src_data;
    uint32 *src_ptr = (uint32 *) src_addr;
    uint32 *dst_ptr = (uint32 *) dst_addr;
    uint16 width = blk->blk_end.mBits.width <<3;
    uint16 blk_height = blk->blk_end.mBits.end_y - blk->blk_start.mBits.start_y + 1;
    uint16 blk_width = blk->blk_end.mBits.end_x - blk->blk_start.mBits.start_x +  1;
    uint32 temp = 0;

  //  temp = (blk->blk_start.mBits.start_y*width +   blk->blk_start.mBits.start_x) >>1;
  //  src_ptr += temp;
 //   dst_ptr += temp;

    for (i =0 ; i<blk_height ; i++)
    {
        for (j=0; j< (blk_width>>1); j++)
        {
            src_data = *src_ptr++;
            *dst_ptr++ =  WORD_ENDIAN_SWAP (src_data);
        }

        src_ptr += (width-blk_width) >>1;
        dst_ptr += (width-blk_width) >>1;
    }

    blk->sdram_addr  = dst_addr;
}

#endif

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL void ConfigBlock (LCD_ID_E lcd_id, BLOCK_POSITION_T blk_pos)
{
    uint16 left        = 0;
    uint16 top         = 0;
    uint16 right       = 0;
    uint16 bottom      = 0;
    uint32 index       = 0;
    uint32 i           = 0;
    uint32 start_x     = 0;
    uint32 start_y     = 0;
    uint32 start_x_tmp = 0;
    uint32 start_y_tmp = 0;
    uint32 end_x       = 0;
    uint32 end_y       = 0;
    uint32 width       = 0;
    uint32 temp        = 0;
    LCD_BLOCK_T blk    = {0};    
    uint16 lcd_width   = 0;
    uint16 lcd_height  = 0;
    LCD_INFO_T lcd_info = {0};

    left   = blk_pos.left;
    top    = blk_pos.top;
    right  = blk_pos.right;
    bottom = blk_pos.bottom;

    index = g_lcd_property_table[lcd_id].blk_group_num;

#if defined  CHIP_ENDIAN_LITTLE && defined SC8800S_LITTLE_ENDIAN

    s_buffer_index = 0;
#endif    

    LCD_GetInfo(lcd_id, &lcd_info);
    lcd_width  = lcd_info.lcd_width;
    lcd_height = lcd_info.lcd_height;

    //  if(blk_pos.work_flag)
    {
        for (i=0 ; i<BLOCK_SUM ; i++)
        {
            if(i==0)
            {
                g_block[index].block[i].state = MEMORY_BUSY;
                blk.blk_start.mBits.enable      = 1;
                blk.blk_start.mBits.priority    = 0;
                blk.blk_start.mBits.start_x     = 0;
                blk.blk_start.mBits.start_y     = 0;
                blk.blk_end.mBits.width         = lcd_width>>3;
                blk.blk_end.mBits.end_x         = lcd_width-1;
                blk.blk_end.mBits.end_y         = lcd_height-1;
                blk.blk_config.mBits.resolution = 2;
                blk.blk_config.mBits.type       = 1;
                blk.blk_config.mBits.colorkey   = 0;
                blk.blk_config.mBits.alpha      = 255;
                blk.sdram_addr                  = (uint32) &s_lcdc_blk_buffer[0];
                blk.blk_num                     = 0;
                blk.blk_config.mBits.colorkey_en= 0;
                blk.blk_config.mBits.alpha_sel  = 1;

                LCDC_SetLcdBlock (&blk, 0);
            }
            else
            {
                if (ENABLE_BLOCK == g_block[index].block[i].is_enable)
                {
                    g_block[index].block[i].state = MEMORY_BUSY;
                    blk.blk_start.mBits.enable      = 1;
                    blk.blk_start.mBits.priority    = g_block[index].block[i].cfg.priority;
                    blk.blk_start.mBits.start_x     = g_block[index].block[i].cfg.start_x;
                    blk.blk_start.mBits.start_y     = g_block[index].block[i].cfg.start_y;
                    blk.blk_end.mBits.width         = (g_block[index].block[i].cfg.width >> 3);
                    blk.blk_end.mBits.end_x         = g_block[index].block[i].cfg.end_x;
                    blk.blk_end.mBits.end_y         = g_block[index].block[i].cfg.end_y;
                    blk.blk_config.mBits.resolution = g_block[index].block[i].cfg.resolution;
                    blk.blk_config.mBits.type       = g_block[index].block[i].cfg.type;
                    blk.blk_config.mBits.colorkey   = g_block[index].block[i].cfg.colorkey;
                    blk.blk_config.mBits.alpha      = g_block[index].block[i].cfg.alpha;
                    blk.sdram_addr                  = (uint32) g_block[index].block[i].mem_ptr;
                    blk.blk_num                     = i;
                    blk.blk_config.mBits.colorkey_en= g_block[index].block[i].cfg.colorkey_en;
                    blk.blk_config.mBits.alpha_sel  = g_block[index].block[i].cfg.alpha_sel;
#if defined  CHIP_ENDIAN_LITTLE && defined SC8800S_LITTLE_ENDIAN
                    _convert_data_format ( (uint32) &s_buffer[s_buffer_index++], (uint32) g_block[index].block[i].mem_ptr,&blk);
#endif
                    LCDC_SetLcdBlock (&blk, 0);
                }
                else
                {
                    blk.blk_start.mBits.enable = 0;
                    blk.blk_num = i;
                    LCDC_SetLcdBlock (&blk, 0);
                }
            }
        }
    }
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL void LCD_InitOpearion (BOOLEAN is_flag)
{
    uint32              i     = 0;
    uint32                              lcd_id = 0;
    uint32                              cs_id = 0;
    uint32                              error = 0;
    uint32              ahb_clk;
    uint32              ctrl_module;
    uint32              size;
    uint32              blk_group_index;
    LCDC_SPEC_T             mcu_lcd_spec = {0};
    LCDC_TYPE_T                 lcdc_type = {0};
    SCI_SEMAPHORE_PTR   sm = SCI_NULL;
    BLOCKCFG_T          cfg;
    AHB_SOFT_RST_U      *ahb_soft_rst_ptr = (AHB_SOFT_RST_U *) AHB_SOFT_RST;
    LCD_INFO_T  lcd_info = {0};

    ahb_clk = CHIP_GetAhbClk();
    if (NULL == s_lcd_event)
   	{
       	s_lcd_event = SCI_CreateEvent("SC8800H event"); 
   	}   

    //get lcd spec and reg the info
    for (lcd_id = 0; lcd_id < LCD_SUPPORT_MAX; lcd_id++)
    {
        s_lcd_spec_info_ptr[lcd_id] = LCM_GetLcdSpecInfo (lcd_id);

        if (PNULL != s_lcd_spec_info_ptr[lcd_id])
        {
            SCI_ASSERT (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation);
            g_lcd_used_num++;
        }

    }

    SCI_ASSERT (g_lcd_used_num <= LCD_SUM);

    blk_group_index = 0;
    g_lcdc_sm_ptr = SCI_CreateSemaphore ("LCDC OPERATION SEMAPHORE", 1);
    g_tv_sm_ptr   = SCI_CreateSemaphore ("LCDC TV OPERATION SEMAPHORE", 1);

    SCI_ASSERT (PNULL != g_tv_sm_ptr);

    for (i=0 ; i<g_lcd_used_num ; i++)
    {
        sm = SCI_CreateSemaphore ("LCD ASYNC OPERATION SEMAPHORE", 1);

        SCI_ASSERT (PNULL != sm);
        g_lcd_property_table[i].semaphore        = sm;
        g_lcd_property_table[i].brush_mode       = LCD_BRUSH_MODE_SYNC;
        g_lcd_property_table[i].state                   = LCD_IDLE;
        g_lcd_property_table[i].sleep_state      = SCI_FALSE;
        g_lcd_property_table[i].buffer = (uint32 *) NULL;

        memcpy (&g_lcd_property_table[i].lcd_spec, s_lcd_spec_info_ptr[i], sizeof (LCD_SPEC_INFO_T));

        ctrl_module = g_lcd_property_table[i].lcd_spec.controller;
        lcdc_type.controller = ctrl_module;

        if ( (LCD_CTRL_MODULE_LCDC  == ctrl_module) || (LCD_CTRL_MODULE_LCDC_LCM0== ctrl_module))
        {

            SCI_ASSERT (blk_group_index < (BLOCK_GROUP_NUM - 1));

            if (is_flag)
            {
                LCDC_Reset();
                LCDC_SelectRstFuc (1);
            }
            else
            {
                LCDC_SelectRstFuc (0);
                ahb_soft_rst_ptr->mBits.lcdc_soft_rst = 0x1;
                OS_TickDelay (1);
                ahb_soft_rst_ptr->mBits.lcdc_soft_rst = 0x0;
            }

            g_lcd_property_table[i].operation        = BLOCK_OPERATION;
            g_lcd_property_table[i].blk_group_num    = blk_group_index;
            blk_group_index++;
            g_lcd_property_table[i].buffer = (uint32 *) NULL;
            g_lcdc_is_used = 1;

            if (g_lcd_property_table[i].lcd_spec.spec_ptr->mode == LCD_MCU)
            {
                lcdc_type.lcdc_mode = 1;

                if (i == MAIN_LCD_ID)
                {
                    lcdc_type.main_lcd_width  = g_lcd_property_table[i].lcd_spec.spec_ptr->width;
                    lcdc_type.main_lcd_height = g_lcd_property_table[i].lcd_spec.spec_ptr->height;
                    if(0 == g_lcd_property_table[i].lcd_spec.cs_pin)
                    {
                        mcu_lcd_spec.mcu_lcd_spec.modesel.mBits.modesel0     = g_lcd_property_table[i].lcd_spec.spec_ptr->bus_mode;
                        mcu_lcd_spec.mcu_lcd_spec.rgbmode.mBits.cs0_rgb_mode = g_lcd_property_table[i].lcd_spec.spec_ptr->bus_width;
                    }
                    else
                    {
                        mcu_lcd_spec.mcu_lcd_spec.modesel.mBits.modesel1     = g_lcd_property_table[i].lcd_spec.spec_ptr->bus_mode;
                        mcu_lcd_spec.mcu_lcd_spec.rgbmode.mBits.cs1_rgb_mode = g_lcd_property_table[i].lcd_spec.spec_ptr->bus_width;
                    }
                    lcdc_type.main_lcd_cs_pin = g_lcd_property_table[i].lcd_spec.cs_pin;
                }
                else if (i == SUB_LCD_ID)
                {
                    lcdc_type.sub_lcd_width  = g_lcd_property_table[i].lcd_spec.spec_ptr->width;
                    lcdc_type.sub_lcd_height = g_lcd_property_table[i].lcd_spec.spec_ptr->height;
                    if(0 == g_lcd_property_table[i].lcd_spec.cs_pin)
                    {
                        mcu_lcd_spec.mcu_lcd_spec.modesel.mBits.modesel0     = g_lcd_property_table[i].lcd_spec.spec_ptr->bus_mode;
                        mcu_lcd_spec.mcu_lcd_spec.rgbmode.mBits.cs0_rgb_mode = g_lcd_property_table[i].lcd_spec.spec_ptr->bus_width;
                    }
                    else
                    {
                        mcu_lcd_spec.mcu_lcd_spec.modesel.mBits.modesel1     = g_lcd_property_table[i].lcd_spec.spec_ptr->bus_mode;
                        mcu_lcd_spec.mcu_lcd_spec.rgbmode.mBits.cs1_rgb_mode = g_lcd_property_table[i].lcd_spec.spec_ptr->bus_width;
                    }
                    lcdc_type.sub_lcd_cs_pin = g_lcd_property_table[i].lcd_spec.cs_pin;
                }
                else
                {
                    SCI_PASSERT (0, ("lcd num is too much!"));
                }
            }
            else
            {
                SCI_MEMSET ( (uint8 *) &lcdc_type,0,sizeof (LCDC_TYPE_T));
                lcdc_type.main_lcd_width  = g_lcd_property_table[i].lcd_spec.spec_ptr->width;
                lcdc_type.main_lcd_height = g_lcd_property_table[i].lcd_spec.spec_ptr->height;
                lcdc_type.lcd_background  = LCD_BACKGROUND;
            }
        }
        else
        {
            size = g_lcd_property_table[i].lcd_spec.spec_ptr->width * g_lcd_property_table[i].lcd_spec.spec_ptr->height * 2;
            g_lcd_property_table[i].operation        = AHB_COMMAND_OPERATION;
            g_lcd_property_table[i].buffer = (uint32 *) SCI_ALLOC_APP (size);
        }
    }

    if (g_lcdc_is_used)
    {
        if (lcdc_type.lcdc_mode == 1)
        {
            lcdc_type.mcu_lcd_sum    = cs_id;
            lcdc_type.lcd_background = LCD_BACKGROUND;

            LCDC_SetLcdType (&lcdc_type, g_lcd_property_table[0].lcd_spec.cs_pin);
            LCDC_Init();
            error = LCDC_Configure (&mcu_lcd_spec);
        }
        else//rgb mode
        {
            LCDC_SetLcdType (&lcdc_type, g_lcd_property_table[0].lcd_spec.cs_pin);
            LCDC_Init();
        }
    }

    LCD_UpdateTiming (ahb_clk);


    g_lcd_property_table[TV_ID].semaphore            = SCI_CreateSemaphore ("LCD ASYNC OPERATION SEMAPHORE", 1);
    g_lcd_property_table[TV_ID].brush_mode           = LCD_BRUSH_MODE_SYNC;
    g_lcd_property_table[TV_ID].state                = LCD_IDLE;
    g_lcd_property_table[TV_ID].buffer     = (uint32 *) NULL;
    g_lcd_property_table[TV_ID].blk_group_num        = (BLOCK_GROUP_NUM - 1);
    g_lcd_property_table[TV_ID].operation            = BLOCK_OPERATION;
    g_lcd_property_table[TV_ID].lcd_spec.controller = LCD_CTRL_MODULE_LCDC;


    for (i=0 ; i<g_lcd_used_num ; i++)
    {
        if ( (LCD_CTRL_MODULE_LCDC == g_lcd_property_table[i].lcd_spec.controller)
                || (LCD_CTRL_MODULE_LCDC_LCM0 == g_lcd_property_table[i].lcd_spec.controller))
        {
            LCD_GetInfo (i,&lcd_info);

            cfg.start_x    = 0;
            cfg.start_y    = 0;
            cfg.end_x      = lcd_info.lcd_width - 1;
            cfg.end_y      = lcd_info.lcd_height- 1;
            cfg.colorkey   = 0;
            cfg.width      = lcd_info.lcd_width;
            cfg.resolution = 0x2;
            cfg.type       = 0x1;
            cfg.priority   = 1;
            cfg.alpha      = 255;
            cfg.colorkey_en= 0;
            cfg.alpha_sel  = 1;

            LCD_ConfigBlock (i, 0, cfg);
            LCD_SetBlockBuffer (i, 0, (uint32 *) lcd_info.lcdbuff_ptr);
            LCD_EnableBlock (i, 0);
        }
    }
    SCI_MEMSET((void*)&s_lcdc_blk_buffer[0],0,((lcd_info.lcd_width+15)/16*16)*((lcd_info.lcd_height+7)/8*8)*2);

}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL void LCD_BlockOperation (LCD_ID_E lcd_id, LCD_OPERATION_CTRL_T ctrl_data)
{
    uint32            ctrl_mode   = 0;
    uint32            index       = 0;
    uint32            i           = 0;
    uint16            left        = 0;
    uint16            top         = 0;
    uint16            right       = 0;
    uint16            bottom      = 0;
    int32             ret         = 0;
    LCD_OPERATIONS_T  *op_ptr     = NULL;
    LCD_BRUSH_MODE_E  mode;
    BLOCK_POSITION_T  blk_pos     = {0};
    LCD_INT_ENA_T     int_type;
    BOOLEAN           is_open_int = SCI_FALSE;
    
    _SetCheckFlag(SCI_FALSE);

    blk_pos.left      = ctrl_data.left;
    blk_pos.top       = ctrl_data.top;
    blk_pos.right     = ctrl_data.right;
    blk_pos.bottom    = ctrl_data.bottom;
    blk_pos.work_flag = ctrl_data.work_flag;

    left   = blk_pos.left;
    top    = blk_pos.top;
    right  = blk_pos.right;
    bottom = blk_pos.bottom;
    op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;

    index = g_lcd_property_table[lcd_id].blk_group_num;

    SCI_ASSERT (PNULL != op_ptr);

    ctrl_mode  = g_lcd_property_table[lcd_id].lcd_spec.controller;
    
    _LCD_Clearevent();   
    
    if ( (LCD_CTRL_MODULE_LCDC == ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == ctrl_mode))
    {
        ConfigBlock (lcd_id, blk_pos);     
        
        mode = g_lcd_property_table[lcd_id].brush_mode;

        LCDC_SetDisplayWindows (left, top, right, bottom);

        if (mode == LCD_BRUSH_MODE_SYNC)
        {           
            if(SCI_InThreadContext())
            {
                int_type.mcu_endofosd_en = 1;
                int_type.rgb_int_enable.end_of_frame =0;
                LCDC_SpecialMode (&int_type);
                is_open_int = SCI_TRUE;
                LCDC_RegisterInterrupt (0, LCD_FreeSemaphore);
                _SetCheckFlag(SCI_FALSE);
            }
            else
            {
                int_type.mcu_endofosd_en = 0;
                int_type.rgb_int_enable.end_of_frame =0;
                LCDC_SpecialMode (&int_type);
                is_open_int = SCI_FALSE;
                _SetCheckFlag(SCI_TRUE);
            }
           
            s_fmark_pin_state[lcd_id] = SCI_FALSE;

            SCI_ASSERT (LCDC_CheckBrushLcdStatue() == 0);
            op_ptr->lcd_SetBrushDirection (ctrl_data.angle);

            SCI_ASSERT (g_lcd_property_table[lcd_id].state == LCD_BUSY);

            if (s_use_lcd_scan_sync_flag[lcd_id] == SCI_TRUE)
            {
                while (!s_fmark_pin_state[lcd_id]);

                LCD_Delayms (ctrl_data.is_full);
            }

            if (ctrl_data.angle == LCD_ANGLE_0)
            {
                ret = op_ptr->lcd_InvalidateRect (left, top, right, bottom);
                LCDC_TransCmd ( (right-left+1), (bottom-top+1),g_lcd_property_table[lcd_id].lcd_spec.cs_pin);
            }
            else
            {
                ret = op_ptr->lcd_RotationInvalidateRect (left, top, right, bottom,ctrl_data.angle);
                LCDC_TransCmd ( (right-left+1), (bottom-top+1),g_lcd_property_table[lcd_id].lcd_spec.cs_pin);
            }

            SCI_ASSERT (ret == LCD_OK);
            
            if((SCI_InThreadContext()) && (is_open_int==SCI_TRUE))
            {         
            	_LCD_GetEvent(LCD_DONE_EVENT,0);            	
            }
            else
            {
                //SCI_TRACE_LOW:"check lcdc state!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC8800H_722_112_2_18_1_45_19_107,(uint8*)"");
            	g_lcd_start_time = SCI_GetTickCount();
                _SetCheckFlag(SCI_TRUE);

	            while (LCDC_CheckBrushLcdStatue() == 0)
	            {
	                g_lcd_end_time = SCI_GetTickCount();
	                SCI_ASSERT ( ( (g_lcd_end_time - g_lcd_start_time) < LCD_BRUSH_TIME_OUT) || (LCDC_CheckBrushLcdStatue() == SCI_TRUE));
	            }

	            LCDC_ClearStatus();            
	            g_lcd_start_time = 0x66ff;
	            g_lcd_end_time   = 0xff66;            
                LCD_FreeSemaphore();            
            }              
        }
        else//async
        {
            int_type.mcu_endofosd_en = 1;
            int_type.rgb_int_enable.end_of_frame =0;
            LCDC_SpecialMode (&int_type);
            s_fmark_pin_state[lcd_id] = SCI_FALSE;
            LCDC_RegisterInterrupt (0, LCD_FreeSemaphore);
            LCD_RecordSemaphore (lcd_id, ctrl_data.sm);
            LCD_RecordLcdIndex (LCD_CTRL_MODULE_LCDC, lcd_id);
            op_ptr->lcd_SetBrushDirection (ctrl_data.angle);

            if (s_use_lcd_scan_sync_flag[lcd_id] == SCI_TRUE)
            {
                while (!s_fmark_pin_state[lcd_id]);

                LCD_Delayms (ctrl_data.is_full);
            }

            if (ctrl_data.angle == LCD_ANGLE_0)
            {
                ret = op_ptr->lcd_InvalidateRect (left, top, right, bottom);
                LCDC_TransCmd ( (right-left+1), (bottom-top+1),g_lcd_property_table[lcd_id].lcd_spec.cs_pin);
            }
            else
            {
                ret = op_ptr->lcd_RotationInvalidateRect (left, top, right, bottom,ctrl_data.angle);
                LCDC_TransCmd ( (right-left+1), (bottom-top+1),g_lcd_property_table[lcd_id].lcd_spec.cs_pin);
            }

            SCI_ASSERT (ret == LCD_OK);
        }

    }
    else
    {
        //LCM DMA fashion
    }
}
/*****************************************************************************/
//  Description: Set if use lcd scan sync function
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//  Note:
//      input:
//          is_use_lcd_scan_sync - SCI_TRUE:use this function ;SCI_FALSE:don't use
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
PUBLIC void LCD_SetIsUseLcdScanSync (LCD_ID_E lcd_id, BOOLEAN is_use_lcd_scan_sync)
{
    if (g_lcdc_tv_flag)
    {
        is_use_lcd_scan_sync = SCI_FALSE;
        s_use_lcd_scan_sync_flag[lcd_id] = is_use_lcd_scan_sync;
        LCD_SetIsOpenFmarkFunc (lcd_id, is_use_lcd_scan_sync);
        return;
    }

    LCD_GetSemaphore (g_lcdc_sm_ptr, SYS_STATE_NORMAL);
    s_use_lcd_scan_sync_flag[lcd_id] = is_use_lcd_scan_sync;
    LCD_SetIsOpenFmarkFunc (lcd_id, is_use_lcd_scan_sync);
    LCD_PutSemaphore (g_lcdc_sm_ptr, SYS_STATE_NORMAL);

}//end of LCD_SetIsUseLcdScanSync
/*****************************************************************************/
//  Description: get if use lcd scan sync function
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//  Note:
//      input:
//          none
//      output:
//          none
//      return:
//          use state:SCI_TRUE:use this function;SCI_FALSE:don't use
/*****************************************************************************/
PUBLIC BOOLEAN LCD_GetIsUseLcdScanSync (LCD_ID_E lcd_id)
{
    return s_use_lcd_scan_sync_flag[lcd_id];
}//end of LCD_GetIsUseLcdScanSync
/*****************************************************************************/
//  Description: set fmark value
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//  Note:
//      input:
//          none
//      output:
//          none
//      return:
//          use state:SCI_TRUE:use this function;SCI_FALSE:don't use
/*****************************************************************************/
PUBLIC void LCD_ScanSyncCallBack (LCD_ID_E lcd_id)
{
    s_fmark_pin_state[lcd_id] = SCI_TRUE;
}//end of LCD_ScanSyncCallBack
/*****************************************************************************/
//  Description: get system run state
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//  Note:
//      input:
//          none
//      output:
//          system run state
//      return:
//          result
/*****************************************************************************/
LOCAL SYS_STATE_E LCD_GetRunState()
{
    return s_sys_run_state;
}
/*****************************************************************************/
//  Description: When assert ,Invalidate Rectagule LCD,pixel_sum%16 == 0
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//  Note:
//      input:
//          lcd_id - specify LCD
//          left   - left of the rectangle
//          top    - top of the rectangle
//          right  - right of the rectangle
//          bottom - bottom of the rectangle.
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
LOCAL ERR_LCD_E LCD_AssertInvalidateRect (
    LCD_ID_E lcd_id,
    uint16 left,
    uint16 top,
    uint16 right,
    uint16 bottom
)
{
    LCD_BRUSH_MODE_E  mode;
    LCD_OPERATIONS_T  *op_ptr;
    LCD_MODE_E        lcd_mode;
    uint32            ctrl_mode;
    BLOCK_POSITION_T  blk_pos    = {0};
    LCD_INT_ENA_T     int_type = {0};

    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_AssertInvalidateRect,lcd_id = %d", lcd_id));
    }
    else
    {
        return ERR_LCD_NONE;
    }

    SCI_ASSERT ( (left < right) && (right < g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width));
    SCI_ASSERT ( (top < bottom) && (bottom < g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height));

    op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;

    SCI_ASSERT (PNULL != op_ptr);

    MMU_InvalideDCACHE ();

    mode      = g_lcd_property_table[lcd_id].brush_mode;
    lcd_mode  = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->mode;
    ctrl_mode = g_lcd_property_table[lcd_id].lcd_spec.controller;

    int_type.mcu_endofosd_en = 0;
    int_type.rgb_int_enable.end_of_frame =0;
    LCDC_SpecialMode (&int_type);
    if (g_lcd_property_table[lcd_id].operation == BLOCK_OPERATION)
    {
        if ( (LCD_CTRL_MODULE_LCDC == ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == ctrl_mode))
        {
            blk_pos.left      = left;
            blk_pos.top       = top;
            blk_pos.right     = right;
            blk_pos.bottom    = bottom;
            blk_pos.work_flag = SCI_TRUE;

            ConfigBlock (lcd_id, blk_pos);

            LCDC_SetDisplayWindows (left, top, right, bottom);

            if (lcd_mode == LCD_RGB)
            {
                //data memory copy
            }

            g_lcd_property_table[lcd_id].state = LCD_BUSY;
            op_ptr->lcd_SetBrushDirection (LCD_ANGLE_0);
            op_ptr->lcd_Invalidate();
            LCDC_TransCmd ( (g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width), (g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height),g_lcd_property_table[lcd_id].lcd_spec.cs_pin);

            while (LCDC_CheckBrushLcdStatue() == 0);

            LCDC_ClearStatus();
            g_lcd_property_table[lcd_id].state = LCD_READY;

        }
        else
        {
            //LCM DMA fashion
        }
    }
    else
    {
        //set buffer address for AHB command
        g_lcd_property_table[lcd_id].state = LCD_BUSY;
        LCD_SetBuffer (lcd_id, g_lcd_property_table[lcd_id].buffer);
        op_ptr->lcd_InvalidateRect (left, top, right, bottom);
        g_lcd_property_table[lcd_id].state = LCD_READY;
    }

    return ERR_LCD_NONE;
}//end of LCD_AssertInvalidateRect

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL void LCD_RecordSemaphore (LCD_ID_E lcd_id, SCI_SEMAPHORE_PTR lcd_sm)
{
    SCI_ASSERT (PNULL != lcd_sm);

    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_RecordSemaphore,lcd_id = %d", lcd_id));
        g_lcd_sm_ptr[lcd_id] = lcd_sm;
    }
}//end of LCD_RecordSemaphore
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL void LCD_RecordLcdIndex (LCD_CTRL_MODULE_E ctrl_mode, uint32 lcd_id)
{
    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_RecordLcdIndex,lcd_id = %d", lcd_id));
        g_lcd_index[LCD_CTRL_MODULE_LCDC] = lcd_id;
    }
}//end of LCD_RecordLcdIndex
/*****************************************************************************/
//  Description:Initialize LCD oneself
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id    - specify LCD
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
PUBLIC void LCD_Initialize (LCD_ID_E lcd_id)
{
    LCD_OPERATIONS_T  *op_ptr;

    if (lcd_id != TV_ID)
    {
        SCI_ASSERT (lcd_id < g_lcd_used_num);

        op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;

        SCI_ASSERT (PNULL != op_ptr);

        op_ptr->lcd_Init();
    }

}//end of LCD_Initialize
/*****************************************************************************/
//  Description:LCD Get Semaphore
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          sm    -  semaphore
//          flag  -  system run state
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
LOCAL void LCD_GetSemaphore (SCI_SEMAPHORE_PTR sm, SYS_STATE_E flag)
{
    SCI_ASSERT (PNULL != sm);
    
    if ( (SCI_InThreadContext()) && (flag == SYS_STATE_NORMAL))
    {
        if(sm == g_lcdc_sm_ptr)
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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC8800H_1031_112_2_18_1_45_20_108,(uint8*)"");
        }
    }
    
}//end of LCD_GetSemaphore
/*****************************************************************************/
//  Description:LCD Put Semaphor
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          sm    -  semaphore
//          flag  -  system run state
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
LOCAL void LCD_PutSemaphore (SCI_SEMAPHORE_PTR sm, SYS_STATE_E flag)
{
    uint32 ret;

    SCI_ASSERT (PNULL != sm);
    
    if ( (SCI_InThreadContext()) && (flag == SYS_STATE_NORMAL))
    {
        if(sm == g_lcdc_sm_ptr)
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
        SCI_ASSERT (ret == SCI_SUCCESS);
    }

    if (flag == SYS_STATE_INT)
    {
        ret = SCI_PutSemaphore (sm);
        SCI_ASSERT (ret == SCI_SUCCESS);
    }
    
}//end of LCD_PutSemaphore
/****************************************************************************************/
// Description: lcd set event
// Author: Jianping.Wang
// input parameter:
//                      uint32 set_flags : event put flags
/*****************************************************************************************/
LOCAL void _LCD_SetEvent(uint32 set_flags)
{
    if(PNULL!=s_lcd_event)
    {   
        SCI_SetEvent(s_lcd_event, set_flags, SCI_OR);
    }  
}
/****************************************************************************************/
// Description: lcd get event
// Author: Jianping.Wang
// input parameter:
//                      uint32 requested_flags : event request flags
//                      uint32 wait_option : event wait option
// output parameter:
//                      None
// return:
//          SCI_SUCCESS: success
//          other: fail   
// Note:
/*****************************************************************************************/
LOCAL uint32 _LCD_GetEvent(uint32 requested_flags, uint32 wait_option)
{
    uint32 actual_event = 0;
    uint32 status = SCI_ERROR;
    if(PNULL!=s_lcd_event)
    {  
        status = SCI_GetEvent(s_lcd_event, 
                                requested_flags, 
                                SCI_OR_CLEAR,
                                &actual_event, 
                                SCI_WAIT_FOREVER);        
    }  

    return status;
}
LOCAL void _LCD_Clearevent(void)
{
    if(PNULL!=s_lcd_event)
    {   
        SCI_SetEvent(s_lcd_event, 0, SCI_AND);
    }  
}
/*****************************************************************************/
//  Description: free semaphore when use LCDC by asyn fashion
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL void LCD_SetSyncEvent (void)
{
	_LCD_SetEvent(LCD_DONE_EVENT);
	//SCI_TRACE_LOW:"set event"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC8800H_1127_112_2_18_1_45_20_109,(uint8*)"");
}
/*****************************************************************************/
//  Description: free semaphore when use LCDC by asyn fashion
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL void LCD_FreeSemaphore (void)
{
    uint32 i = 0;
    uint32 index;
    uint32 lcd_id;
    LCD_OPERATIONS_T  *op_ptr;

    SCI_ASSERT (PNULL != g_lcdc_sm_ptr);

    lcd_id = g_lcd_index[LCD_CTRL_MODULE_LCDC];
    index  = g_lcd_property_table[lcd_id].blk_group_num;
    op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;
    SCI_ASSERT (PNULL != op_ptr);

    if (s_lcd_register_callback)
    {
        s_lcd_register_callback (0);
    }

    for (i=0 ; i<LCDC_BLOCK_NUM ; i++)
    {
        if (ENABLE_BLOCK == g_block[index].block[i].is_enable)
        {
            g_block[index].block[i].state = MEMORY_READY;
        }
    }
    if(SCI_InThreadContext())
    {
        SCI_ASSERT (g_lcd_property_table[lcd_id].state == LCD_BUSY);
    }
    g_lcd_property_table[lcd_id].state = LCD_READY;
    DPSLP_Lcdc_EnableAhbSleep (ENABLE_AHB_SLEEP);
    CHNG_FREQ_Event_Set (FREQ_CHNG_EVENT_LCDC,SYS_MODE_INT);
    SCI_ASSERT (PNULL != g_lcd_property_table[lcd_id].semaphore);
    op_ptr->lcd_SetBrushDirection (LCD_ANGLE_0);
    _LCD_SetEvent(LCD_DONE_EVENT);
    if(SCI_TRUE == _GetCheckFlag())
    {
        LCD_PutSemaphore (g_lcdc_sm_ptr, SYS_STATE_NORMAL);
        LCD_PutSemaphore (g_lcd_property_table[lcd_id].semaphore, SYS_STATE_NORMAL);
    }
    else
    {
        LCD_PutSemaphore (g_lcdc_sm_ptr, SYS_STATE_INT);
        LCD_PutSemaphore (g_lcd_property_table[lcd_id].semaphore, SYS_STATE_INT);
    }
    //LCD_PutLcdCtrlSemaphore(lcd_id);
    if(s_lcddone_callback)
    {
        s_lcddone_callback(SYS_STATE_INT);
    }
    else
    {
        //SCI_TRACE_LOW:"LCD ERR:s_lcddone_callback is NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC8800H_1186_112_2_18_1_45_20_110,(uint8*)"");
    }
    SCI_LCDBacklightBrightless_EnableDeepSleep (ENABLE_DEEP_SLEEP);    
    //  SCI_TRACE_LOW("Close LCDC INT!");

}//end of LCD_FreeSemaphore
/*****************************************************************************/
//  Description: get lcd controller type
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id - specify LCD
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC LCD_CTRL_MODULE_E LCD_GetCtrlModule (LCD_ID_E lcd_id)
{
    LCD_CTRL_MODULE_E ctrl_mode;

    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_GetCtrlModule,lcd_id = %d", lcd_id));
    }

    ctrl_mode = g_lcd_property_table[lcd_id].lcd_spec.controller;

    return ctrl_mode;

}//end of LCD_GetCtrlModule
/*****************************************************************************/
//  Description: set brush lcd fashion
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id - specify LCD
//          mode   - sync or async
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBrushMode (LCD_ID_E lcd_id, LCD_BRUSH_MODE_E mode)
{
    LCD_CTRL_MODULE_E ctrl_mode;
    LCD_CTRL_MODULE_E lcd_ctrl_mode;
    LCD_INT_ENA_T     int_type;
    LCD_MODE_E        lcd_mode;
    LCD_BRUSH_MODE_E  brush_mode;
    SCI_SEMAPHORE_PTR sm;
    SYS_STATE_E       flag;
    uint32            i = 0;

    //SCI_TRACE_LOW:"LCD_SetBrushMode Enter!mode:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC8800H_1238_112_2_18_1_45_20_111,(uint8*)"d",mode);

    if (lcd_id == TV_ID)
    {
        return ERR_LCD_NONE;
    }

    if ( (g_lcdc_tv_flag == 1) &&
            ( (g_lcd_property_table[lcd_id].lcd_spec.controller == LCD_CTRL_MODULE_LCDC)
              || (g_lcd_property_table[lcd_id].lcd_spec.controller == LCD_CTRL_MODULE_LCDC_LCM0)))
    {
        return 0;
    }

    flag = LCD_GetRunState();

    sm = g_lcd_property_table[lcd_id].semaphore;
    LCD_GetSemaphore (sm, flag);

    if (g_lcd_property_table[lcd_id].sleep_state == SCI_TRUE)
    {
        LCD_PutSemaphore (sm, flag);
        return ERR_LCD_NONE;
    }

    LCD_PutSemaphore (sm, flag);

    SCI_ASSERT (lcd_id < g_lcd_used_num);

    lcd_mode   = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->mode;
    brush_mode = g_lcd_property_table[lcd_id].brush_mode;
    ctrl_mode  = g_lcd_property_table[lcd_id].lcd_spec.controller;

    if (brush_mode == mode)
    {
        return ERR_LCD_NONE;
    }

    

    if ( (LCD_CTRL_MODULE_LCDC == ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == ctrl_mode))
    {
        for (i=0 ; i<g_lcd_used_num ; i++)
        {
            lcd_ctrl_mode  = g_lcd_property_table[i].lcd_spec.controller;

            if ( (LCD_CTRL_MODULE_LCDC == lcd_ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == lcd_ctrl_mode))
            {
                while (g_lcd_property_table[i].state == LCD_BUSY)
                {
                    SCI_Sleep (10);
                }

                sm = g_lcd_property_table[i].semaphore;
                LCD_GetSemaphore (sm, flag);
                TM_SendTestPointRequest (mode,20061130);
                g_lcd_property_table[i].brush_mode = mode;
            }
        }

        if (lcd_mode == LCD_MCU)
        {
            //enable LCDC interrupt
            int_type.mcu_endofosd_en = 1;
            int_type.rgb_int_enable.end_of_frame =0;
            LCDC_SpecialMode (&int_type);
            if(mode == LCD_BRUSH_MODE_ASYNC)
            {
            	LCDC_RegisterInterrupt (0, LCD_FreeSemaphore);
            }
            else
            {
            	LCDC_RegisterInterrupt (0, LCD_SetSyncEvent);
            }
        }
        else//rgb mode
        {
            int_type.mcu_endofosd_en = 0;
            int_type.rgb_int_enable.end_of_frame = 1;
            LCDC_SpecialMode (&int_type);
            LCDC_RegisterInterrupt (0, LCD_FreeSemaphore);
        }

        for (i=0 ; i<g_lcd_used_num ; i++)
        {
            lcd_ctrl_mode  = g_lcd_property_table[i].lcd_spec.controller;

            if ( (LCD_CTRL_MODULE_LCDC == lcd_ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == lcd_ctrl_mode))
            {
                sm = g_lcd_property_table[i].semaphore;
                LCD_PutSemaphore (sm, flag);
            }
        }

    }
    else if ( (mode == LCD_BRUSH_MODE_ASYNC) && (ctrl_mode == LCD_CTRL_MODULE_LCM))
    {
        //enable dma interrupt,set BLOCK_OPERATION .register interrupt
    }

    //SCI_TRACE_LOW:"LCD_SetBrushMode Exit!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC8800H_1338_112_2_18_1_45_20_112,(uint8*)"");
    return ERR_LCD_NONE;

}//end of LCD_SetBrushMode
/*****************************************************************************/
//  Description: get brush lcd fashion
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id - specify LCD
//      output:
//          none
//      return:
//          brush LCD mode
/*****************************************************************************/
PUBLIC LCD_BRUSH_MODE_E LCD_GetBrushMode (LCD_ID_E lcd_id)
{
    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_GetBrushMode,lcd_id = %d", lcd_id));
    }

    return g_lcd_property_table[lcd_id].brush_mode;
}
/*****************************************************************************/
//  Description: configure block parameters
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//          blk_num - LCDC Block number ,0~5
//          config  - Block parameter structure
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_ConfigBlock (
    LCD_ID_E lcd_id,
    uint32 blk_num,
    BLOCKCFG_T config
)
{
    uint32            index;
    SCI_SEMAPHORE_PTR sm = SCI_NULL;
    SYS_STATE_E       flag;

	uint8 type = 0;

    blk_num+=1;

    flag = LCD_GetRunState();

    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_ConfigBlock,lcd_id = %d", lcd_id));
        sm = g_lcd_property_table[lcd_id].semaphore;
        LCD_GetSemaphore (sm, flag);
    }

    SCI_ASSERT ( (config.width%8) == 0);
    SCI_ASSERT (config.resolution < 3);
    SCI_ASSERT ( (config.type == 0) || (config.type == 1));


	type = config.type;
	
      if(2 == config.resolution)
      {
    		type = 1;
       }
    


     index = g_lcd_property_table[lcd_id].blk_group_num;

    if ( (index >= BLOCK_GROUP_NUM) || (blk_num >= BLOCK_SUM))
    {
        SCI_ASSERT (0);
    }
    else
    {
        g_block[index].lcd_id = lcd_id;
        g_block[index].block[blk_num].cfg.start_x    = config.start_x;
        g_block[index].block[blk_num].cfg.start_y    = config.start_y;
        g_block[index].block[blk_num].cfg.end_x      = config.end_x;
        g_block[index].block[blk_num].cfg.end_y      = config.end_y;
        g_block[index].block[blk_num].cfg.colorkey   = config.colorkey;
        g_block[index].block[blk_num].cfg.width      = config.width;
        g_block[index].block[blk_num].cfg.resolution = config.resolution;
        g_block[index].block[blk_num].cfg.type       = type;
        g_block[index].block[blk_num].cfg.priority   = blk_num;//config.priority;
        g_block[index].block[blk_num].cfg.alpha      = config.alpha;
        g_block[index].block[blk_num].cfg.colorkey_en= config.colorkey_en;
        g_block[index].block[blk_num].cfg.alpha_sel  = config.alpha_sel;
    }

    if (lcd_id != TV_ID)
    {
        LCD_PutSemaphore (sm, flag);
    }

    return ERR_LCD_NONE;

}//end of LCD_ConfigBlock
/*****************************************************************************/
//  Description: get block parameters
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//          blk_num - LCDC Block number ,0~5
//          cfg_ptr  - pointer to Block parameter structure
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_GetConfigBlock (
    LCD_ID_E lcd_id,
    uint32 blk_num,
    BLOCKCFG_T *cfg_ptr
)
{
    uint32 index;

    blk_num += 1;

    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_GetConfigBlock,lcd_id = %d", lcd_id));
    }

    SCI_ASSERT (cfg_ptr != PNULL);

    index = g_lcd_property_table[lcd_id].blk_group_num;

    if ( (blk_num >= BLOCK_SUM) || (index >= BLOCK_GROUP_NUM))
    {
        SCI_ASSERT (0);
    }
    else
    {
        cfg_ptr->start_x    = g_block[index].block[blk_num].cfg.start_x;
        cfg_ptr->start_y    = g_block[index].block[blk_num].cfg.start_y;
        cfg_ptr->end_x      = g_block[index].block[blk_num].cfg.end_x;
        cfg_ptr->end_y      = g_block[index].block[blk_num].cfg.end_y;
        cfg_ptr->colorkey   = g_block[index].block[blk_num].cfg.colorkey;
        cfg_ptr->width      = g_block[index].block[blk_num].cfg.width;
        cfg_ptr->resolution = g_block[index].block[blk_num].cfg.resolution;
        cfg_ptr->type       = g_block[index].block[blk_num].cfg.type;
        cfg_ptr->priority   = g_block[index].block[blk_num].cfg.priority;
        cfg_ptr->alpha      = g_block[index].block[blk_num].cfg.alpha;
        cfg_ptr->colorkey_en= g_block[index].block[blk_num].cfg.colorkey_en;
        cfg_ptr->alpha_sel  = g_block[index].block[blk_num].cfg.alpha_sel;


    }

    return ERR_LCD_NONE;
}//end of LCD_GetConfigBlock
/*****************************************************************************/
//  Description: get maximum block number
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//      output:
//          none
//      return:
//          maximum block number
/*****************************************************************************/
PUBLIC uint32 LCD_GetMaxBlockNum (LCD_ID_E lcd_id)
{
    uint32 ret_max_blk_num = 0;


    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_GetMaxBlockNum,lcd_id = %d", lcd_id));
    }

    if ( (lcd_id == TV_ID) ||
            (g_lcd_property_table[lcd_id].lcd_spec.controller == LCD_CTRL_MODULE_LCDC)
            || (g_lcd_property_table[lcd_id].lcd_spec.controller == LCD_CTRL_MODULE_LCDC_LCM0))
    {
        ret_max_blk_num = LCDC_BLOCK_NUM;
    }
    else
    {
        ret_max_blk_num = LCD_BLOCK_NUM_DEFAULT;
    }

    SCI_ASSERT (ret_max_blk_num);

    return ret_max_blk_num;


}//end of LCD_GetMaxBlockNum
/*****************************************************************************/
//  Description: Enable Block
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//          blk_num - LCDC Block number ,0~5
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_EnableBlock (LCD_ID_E lcd_id, uint32 blk_num)
{
    uint32            index;
    SYS_STATE_E       flag;
    SCI_SEMAPHORE_PTR sm = SCI_NULL;

    flag = LCD_GetRunState();

    blk_num += 1;

    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_EnableBlock,lcd_id = %d", lcd_id));
        sm = g_lcd_property_table[lcd_id].semaphore;
        LCD_GetSemaphore (sm, flag);
    }

    index = g_lcd_property_table[lcd_id].blk_group_num;

    if ( (blk_num >= BLOCK_SUM) || (index >= BLOCK_GROUP_NUM))
    {
        SCI_ASSERT (0);
    }
    else
    {
        g_block[index].block[blk_num].is_enable = ENABLE_BLOCK;
        g_block[index].block[blk_num].state     = MEMORY_READY;
    }

    if (lcd_id != TV_ID)
    {
        LCD_PutSemaphore (sm, flag);
    }

    return ERR_LCD_NONE;

}//end of LCD_EnableBlock
/*****************************************************************************/
//  Description:disable block
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//          blk_num - LCDC Block number ,0~5
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_DisableBlock (LCD_ID_E lcd_id, uint32 blk_num)
{
    uint32            index;
    SYS_STATE_E       flag;
    SCI_SEMAPHORE_PTR sm = SCI_NULL;
    flag = LCD_GetRunState();

    blk_num += 1;

    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_DisableBlock,lcd_id = %d", lcd_id));
        sm = g_lcd_property_table[lcd_id].semaphore;
        LCD_GetSemaphore (sm, flag);
    }



    index = g_lcd_property_table[lcd_id].blk_group_num;

    if ( (blk_num >= BLOCK_SUM) || (index >= BLOCK_GROUP_NUM))
    {
        SCI_ASSERT (0);
    }
    else
    {
        g_block[index].block[blk_num].is_enable = DISABLE_BLOCK;
        g_block[index].block[blk_num].state     = MEMORY_IDLE;
    }

    if (lcd_id != TV_ID)
    {
        LCD_PutSemaphore (sm, flag);
    }

    return ERR_LCD_NONE;
}//end of LCD_DisableBlock
/*****************************************************************************/
//  Description:Get Block is enabled or not
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//          blk_num - LCDC Block number ,0~5
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC BOOLEAN LCD_GetBlockIsEnable (LCD_ID_E lcd_id, uint32 blk_num)
{
    uint32 index = 0;
    BOOLEAN ret = SCI_FALSE;

    blk_num += 1;

    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_GetBlockIsEnable,lcd_id = %d", lcd_id));
    }

    SCI_PASSERT ( (blk_num < BLOCK_SUM), ("LCD_GetBlockIsEnable,blk_num = %d", blk_num));

    index = g_lcd_property_table[lcd_id].blk_group_num;

    if ( (blk_num >= BLOCK_SUM) || (index >= BLOCK_GROUP_NUM))
    {
        SCI_ASSERT (0);
    }
    else
    {
        ret = g_block[index].block[blk_num].is_enable;
    }

    return ret;

}//end of LCD_GetBlockState
/*****************************************************************************/
//  Description: Malloc memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          size  - byte sum of memory
//      output:
//          none
//      return:
//          memory addr
/*****************************************************************************/
PUBLIC uint32 *LCD_Malloc (uint32 size)
{
    uint32 *ptr;
    uint32 *align_ptr;
    uint32 benchmark;

    benchmark = MEMORY_ALIGN_BENCHMARK;
    ptr = (uint32 *) SCI_ALLOC_APP ( (size+2*benchmark));

    SCI_ASSERT (ptr != PNULL);

    align_ptr = (uint32 *) ( ( (uint32) ptr + (benchmark-1)) /benchmark*benchmark);

    *align_ptr = (uint32) ptr;

    align_ptr += (benchmark>>2);

    return align_ptr;

}//end of LCD_Malloc
/*****************************************************************************/
//  Description:  free memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          addr_ptr  - memory address that will be freed
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
PUBLIC void LCD_Free (uint32 *addr_ptr)
{
    uint32 *free_ptr;
    uint32 benchmark;

    SCI_ASSERT (addr_ptr != PNULL);

    benchmark = MEMORY_ALIGN_BENCHMARK;
    free_ptr = (uint32 *) (* (addr_ptr - (benchmark>>2)));

    SCI_ASSERT (free_ptr != PNULL);

    SCI_FREE (free_ptr);
}//end of LCD_Free
/*****************************************************************************/
//  Description: set display memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//          blk_num - LCDC Block number ,0~5
//          buf_ptr - buffer address
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBlockBuffer (
    LCD_ID_E lcd_id,
    uint32 blk_num,
    uint32 *buf_ptr
)
{
    uint32            index;
    uint32            op_mode;
    uint32            ctrl_mode;
    SCI_SEMAPHORE_PTR sm = SCI_NULL;
    SYS_STATE_E       flag;

    blk_num += 1;

    flag = LCD_GetRunState();

    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_SetBlockBuffer,lcd_id = %d", lcd_id));
        sm = g_lcd_property_table[lcd_id].semaphore;
        LCD_GetSemaphore (sm, flag);
    }

    SCI_ASSERT (buf_ptr != PNULL);

    op_mode   = g_lcd_property_table[lcd_id].operation;
    ctrl_mode = g_lcd_property_table[lcd_id].lcd_spec.controller;

    if ( (op_mode == BLOCK_OPERATION) &&
            ( (LCD_CTRL_MODULE_LCDC == ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == ctrl_mode)))
    {
        index = g_lcd_property_table[lcd_id].blk_group_num;

        if ( (blk_num >= BLOCK_SUM) || (index >= BLOCK_GROUP_NUM))
        {
            SCI_ASSERT (0);
        }
        else
        {
            g_block[index].block[blk_num].mem_ptr = buf_ptr;
            g_block[index].block[blk_num].state   = MEMORY_READY;
        }
    }
    else
    {
        g_lcd_property_table[lcd_id].buffer = buf_ptr;
        g_lcd_property_table[lcd_id].state            = MEMORY_READY;
    }

    if (lcd_id != TV_ID)
    {
        LCD_PutSemaphore (sm, flag);
    }

    return ERR_LCD_NONE;

}//end of LCD_SetBlockBuffer
/*****************************************************************************/
//  Description: get display memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//          blk_num - LCDC Block number ,0~5
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC uint32 *LCD_GetBlockBuffer (LCD_ID_E lcd_id, uint32 blk_num)
{
    uint32 index;
    uint32 op_mode;
    uint32 ctrl_mode;
    uint32 *buf_ptr = PNULL;

    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_GetBlockBuffer,lcd_id = %d", lcd_id));
    }

    SCI_ASSERT (blk_num < BLOCK_SUM);

    blk_num += 1;

    op_mode   = g_lcd_property_table[lcd_id].operation;
    ctrl_mode = g_lcd_property_table[lcd_id].lcd_spec.controller;

    if ( (op_mode == BLOCK_OPERATION) &&
            ( (LCD_CTRL_MODULE_LCDC == ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == ctrl_mode)))
    {
        index = g_lcd_property_table[lcd_id].blk_group_num;

        if ( (blk_num >= BLOCK_SUM) || (index >= BLOCK_GROUP_NUM))
        {
            SCI_ASSERT (0);
        }
        else
        {
            buf_ptr = g_block[index].block[blk_num].mem_ptr;
        }
    }
    else
    {
        buf_ptr = g_lcd_property_table[lcd_id].buffer;
    }

    return buf_ptr;

}//end of LCD_GetBlockBuffer
/*****************************************************************************/
//  Description: initialize all LCD and global information
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          none
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_Init (void)
{
    uint32 i = 0;
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
    {
#endif

        if (g_lcd_used_num != 0)
        {
            return ERR_LCD_NONE;
        }

        LCD_InitOpearion (SCI_TRUE);

        for (i=0 ; i<g_lcd_used_num ; i++)
        {
            if (PNULL != g_lcd_property_table[i].lcd_spec.spec_ptr->operation)
            {
                g_lcd_property_table[i].lcd_spec.spec_ptr->operation->lcd_Init();
            }
        }

#ifdef PLATFORM_SC6600V
    }
    else
    {
        return LCDTV_Init();
    }

#endif
    return ERR_LCD_NONE;
}//end of LCD_Init
/*****************************************************************************/
//  Description: Close all LCD
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//  Note:
//      input:
//          none
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC void LCD_Close (void)
{
    uint32            i = 0;
    SYS_STATE_E       flag;
    SCI_SEMAPHORE_PTR sm;

    flag = LCD_GetRunState();

    for (i=0 ; i<g_lcd_used_num ; i++)
    {
        if (PNULL != g_lcd_property_table[i].lcd_spec.spec_ptr->operation)
        {
            sm = g_lcd_property_table[i].semaphore;
            LCD_GetSemaphore (sm, flag);

            g_lcd_property_table[i].lcd_spec.spec_ptr->operation->lcd_Close();

            LCD_PutSemaphore (sm,flag);
        }
    }

    LCDC_Close (SCI_TRUE);
}//end of LCD_Close
/*****************************************************************************/
//  Description: get LCD information
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//  Note:
//      input:
//          none
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E  LCD_GetInfo (LCD_ID_E  lcd_id, LCD_INFO_T *lcd_info_ptr)
{
  //  TV_MODE_E       tv_mode;
    uint16          lcd_width  = 0;
	uint16          lcd_height = 0;

    SCI_ASSERT (lcd_info_ptr != PNULL);
    SCI_MEMSET ( (uint8 *) lcd_info_ptr,0,sizeof (LCD_INFO_T));

    if (lcd_id != TV_ID)
    {

        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_GetInfo,lcd_id = %d", lcd_id));

        lcd_width     = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width;
        lcd_height    = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height;

        LCM_GetLcdDevSize(lcd_id,&lcd_info_ptr->lcd_width,&lcd_info_ptr->lcd_height);

        lcd_info_ptr->r_bitmask     = 0xf800;
        lcd_info_ptr->g_bitmask     = 0x07e0;
        lcd_info_ptr->b_bitmask     = 0x001f;
        lcd_info_ptr->bits_per_pixel    = 16;
        lcd_info_ptr->contrast_min  = 0x00;
        lcd_info_ptr->contrast_max      = 63;
        lcd_info_ptr->contrast_defaut   = 0x0A;

        if((lcd_width<lcd_info_ptr->lcd_width)||(lcd_height<lcd_info_ptr->lcd_height))
    	{
    		SCI_PASSERT(0,("lcd devsize is error! lcd_id=%d",lcd_id));
    	}

        if (MAIN_LCD_ID == lcd_id)
        {            
            lcd_info_ptr->lcdbuff_ptr   = (void *) MEM_GetMainLCDBuffer();
        }
        else if (SUB_LCD_ID == lcd_id)
        {
            lcd_info_ptr->lcdbuff_ptr   = (void *) MEM_GetSubLCDBuffer();
        }
    }
   
    return ERR_LCD_NONE;

}//end of LCD_GetInfo
/*****************************************************************************/
//  Description: Invalidate whole LCD
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//  Note:
//      input:
//          lcd_id - specify LCD
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_Invalidate (LCD_ID_E  lcd_id)
{
    LCD_OPERATIONS_T     *op_ptr   = NULL;
    int32                ret       = 0;
    LCD_OPERATION_CTRL_T ctrl_data = {0};
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        ret = ProtectionOperation (lcd_id);

        if (ret != ERR_LCD_NONE)
        {
            return ERR_LCD_NONE;
        }

        if ( (g_lcd_property_table[lcd_id].operation == BLOCK_OPERATION))
        {
            ctrl_data.left      = 0;
            ctrl_data.top       = 0;
            ctrl_data.right     = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width - 1;
            ctrl_data.bottom    = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height - 1;
            ctrl_data.sm        = g_lcd_property_table[lcd_id].semaphore;
            ctrl_data.work_flag = SCI_TRUE;
            ctrl_data.angle     = LCD_ANGLE_0;
            ctrl_data.is_full   = TRUE;
            LCD_BlockOperation (lcd_id, ctrl_data);
        }
        else
        {
            //set buffer address for AHB command
            op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;
            SCI_ASSERT (PNULL != op_ptr);
            g_lcd_property_table[lcd_id].state = LCD_BUSY;
            LCD_SetBuffer (lcd_id, g_lcd_property_table[lcd_id].buffer);
            ret = op_ptr->lcd_Invalidate();
            SCI_ASSERT (ret == LCD_OK);
            g_lcd_property_table[lcd_id].state = LCD_READY;
        }
    }

#ifdef PLATFORM_SC6600V
    else
    {

        return LCDTV_Invalidate (lcd_id);
    }

#endif
    return ERR_LCD_NONE;
}//end of LCD_Invalidate
/*****************************************************************************/
//  Description: Invalidate Rectagule LCD,pixel_sum%16 == 0
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//  Note:
//      input:
//          lcd_id - specify LCD
//          left   - left of the rectangle
//          top    - top of the rectangle
//          right  - right of the rectangle
//          bottom - bottom of the rectangle.
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_InvalidateRect(
    LCD_ID_E lcd_id,
    uint16 left,
    uint16 top,
    uint16 right,
    uint16 bottom
)
{
    LCD_OPERATIONS_T     *op_ptr    = NULL;
    uint32               temp       = 0;
    uint32               lcd_width  = 0;
    uint32               lcd_height = 0;
    int32                ret        = 0;
    LCD_OPERATION_CTRL_T ctrl_data  = {0};

#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        ret = ProtectionOperation (lcd_id);

        if (ret != ERR_LCD_NONE)
        {
            return ERR_LCD_NONE;
        }

        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_InvalidateRect,lcd_id = %d", lcd_id));
        SCI_PASSERT ( ( (left < right) && (right < g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width)), ("LCD_InvalidateRect,left = %d,right=%d", left,right));
        SCI_PASSERT ( ( (top <= bottom) && (bottom < g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height)), ("LCD_InvalidateRect,top = %d,bottom=%d", top,bottom));

        lcd_width  = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width - 1;
        lcd_height = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height - 1;


        left  = left - left%8;
        right = ( (right-left+1) %8 == 0) ? right : (right + 8 - (right-left+1) %8);
        right = (right > lcd_width) ? (lcd_width - (lcd_width-left+1) %8) : right;

        temp = (right-left+1) * (bottom-top+1);

        if (temp < LCDC_UPDATE_PIXELS_MIN)
        {
            if ( (top - 8) >= 0)
            {
                top -= 8;
            }
            else if ( (bottom + 8) <= lcd_height)
            {
                bottom += 8;
            }
        }

        temp = (right-left+1) * (bottom-top+1);

        if ( (temp%16) != 0)
        {
            if ( (top - 1) >= 0)
            {
                top -= 1;
            }
            else if ( (bottom + 1) <= lcd_height)
            {
                bottom += 1;
            }
        }

        if (g_lcd_property_table[lcd_id].operation == BLOCK_OPERATION)
        {
            ctrl_data.left      = left;
            ctrl_data.top       = top;
            ctrl_data.right     = right;
            ctrl_data.bottom    = bottom;
            ctrl_data.sm        = g_lcd_property_table[lcd_id].semaphore;
            ctrl_data.work_flag = SCI_FALSE;
            ctrl_data.angle     = LCD_ANGLE_0;
            ctrl_data.is_full   = ( (left==0) && (top==0) && (right==lcd_width) && (bottom==lcd_height)) ? TRUE : FALSE;
            LCD_BlockOperation (lcd_id, ctrl_data);
        }
        else
        {
            //set buffer address for AHB command
            op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;
            SCI_ASSERT (PNULL != op_ptr);
            g_lcd_property_table[lcd_id].state = LCD_BUSY;
            LCD_SetBuffer (lcd_id, g_lcd_property_table[lcd_id].buffer);
            ret = op_ptr->lcd_InvalidateRect (left, top, right, bottom);
            LCDC_TransCmd ( (right-left+1), (bottom-top+1),g_lcd_property_table[lcd_id].lcd_spec.cs_pin);
            SCI_ASSERT (ret == LCD_OK);
            g_lcd_property_table[lcd_id].state = LCD_READY;
        }
    }

#ifdef PLATFORM_SC6600V
    else
    {
        return LCDTV_InvalidateRect (lcd_id,left,top,right,bottom);
    }

#endif
    return ERR_LCD_NONE;
}//end of LCD_InvalidateRec
/*****************************************************************************/
//  Description: Invalidate Rectagule LCD,pixel_sum%16 == 0
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//  Note:
//      input:
//          lcd_id - specify LCD
//          left   - left of the rectangle
//          top    - top of the rectangle
//          right  - right of the rectangle
//          bottom - bottom of the rectangle.
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_VerticalInvalidateRect (
    LCD_ID_E lcd_id,
    uint16 left,
    uint16 top,
    uint16 right,
    uint16 bottom
)
{
    LCD_OPERATIONS_T     *op_ptr   = NULL;
    int32                ret       = 0;
    LCD_OPERATION_CTRL_T ctrl_data = {0};

    ret = ProtectionOperation (lcd_id);

    if (ret != ERR_LCD_NONE)
    {
        return ERR_LCD_NONE;
    }

    SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_VerticalInvalidateRect,lcd_id = %d", lcd_id));
    SCI_PASSERT ( ( (left < right) && (right < g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height + 4)), ("LCD_VerticalInvalidateRect,left = %d,right=%d", left,right));
    SCI_PASSERT ( ( (top <= bottom) && (bottom < g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height)), ("LCD_VerticalInvalidateRect,top = %d,bottom=%d", top,bottom));

    if (g_lcd_property_table[lcd_id].operation == BLOCK_OPERATION)
    {
        ctrl_data.left      = left;
        ctrl_data.top       = top;
        ctrl_data.right     = right;
        ctrl_data.bottom    = bottom;
        ctrl_data.sm        = g_lcd_property_table[lcd_id].semaphore;
        ctrl_data.work_flag = SCI_TRUE;
        ctrl_data.angle     = LCD_ANGLE_270;
        ctrl_data.is_full   = TRUE;
        LCD_BlockOperation (lcd_id, ctrl_data);
    }
    else
    {
        //set buffer address for AHB command
        op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;
        SCI_ASSERT (PNULL != op_ptr);
        op_ptr->lcd_SetBrushDirection (LCD_ANGLE_270);
        g_lcd_property_table[lcd_id].state = LCD_BUSY;
        LCD_SetBuffer (lcd_id, g_lcd_property_table[lcd_id].buffer);
        ret = op_ptr->lcd_InvalidateRect (left, top, right, bottom);
        SCI_ASSERT (ret == LCD_OK);
        g_lcd_property_table[lcd_id].state = LCD_READY;
        op_ptr->lcd_SetBrushDirection (LCD_ANGLE_0);
    }

    return ERR_LCD_NONE;
}//end of LCD_VerticalInvalidateRec
/*****************************************************************************/
//  Description:    Invalidate a rectangle of LCD--Refresh it with angle.
//  Global resource dependence:
//  Author:         Jianping.wang
//  Return:         error code.
//  Note:1. the left up point is (0,0)
//       2. Before call InvalidateRect()£¬write LCD buffer with color,
//          and then InvalidateRect() the spefic area.
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_RotateInvalidateRect ( //Return the operate status
    LCD_ID_E    lcd_id,                 //id of lcd to operate
    uint16      left,                   //left of the windows to be refreshed.
    uint16      top,                    //top of the widonws to be refreshed.
    uint16      right,                  //right of the windows to be refreshed.
    uint16      bottom,                 //bottom of the windows to be refreshed.
    LCD_ANGLE_E angle                   //rotation angle
)
{
    LCD_OPERATIONS_T     *op_ptr    = NULL;
    uint32               temp       = 0;
    uint32               lcd_width  = 0;
    uint32               lcd_height = 0;
    int32                ret        = 0;
    LCD_OPERATION_CTRL_T ctrl_data  = {0};

    ret = ProtectionOperation (lcd_id);

    if (ret != ERR_LCD_NONE)
    {
        return ERR_LCD_NONE;
    }

    SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_RotateInvalidateRect,lcd_id = %d", lcd_id));
    SCI_PASSERT ( ( (left < right) && (right < g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height + 4)), ("LCD_RotateInvalidateRect,left = %d,right=%d", left,right));
    SCI_PASSERT ( ( (top <= bottom) && (bottom < g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height)), ("LCD_RotateInvalidateRect,top = %d,bottom=%d", top,bottom));

    if ( (angle == LCD_ANGLE_0) || (angle == LCD_ANGLE_180))
    {
        lcd_width  = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width - 1;
        lcd_height = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height - 1;
    }
    else
    {
        lcd_width  = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height - 1;
        lcd_height = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width - 1;
    }

    left  = left - left%8;
    right = ( (right-left+1) %8 == 0) ? right : (right + 8 - (right-left+1) %8);
    right = (right > lcd_width) ? (lcd_width - (lcd_width-left+1) %8) : right;

    temp = (right-left+1) * (bottom-top+1);

    if (temp < LCDC_UPDATE_PIXELS_MIN)
    {
        if ( (top - 8) >= 0)
        {
            top -= 8;
        }
        else if ( (bottom + 8) <= lcd_height)
        {
            bottom += 8;
        }
    }

    temp = (right-left+1) * (bottom-top+1);

    if ( (temp%16) != 0)
    {
        if ( (top - 1) >= 0)
        {
            top -= 1;
        }
        else if ( (bottom + 1) <= lcd_height)
        {
            bottom += 1;
        }
    }

    if (g_lcd_property_table[lcd_id].operation == BLOCK_OPERATION)
    {
        ctrl_data.left      = left;
        ctrl_data.top       = top;
        ctrl_data.right     = right;
        ctrl_data.bottom    = bottom;
        ctrl_data.sm        = g_lcd_property_table[lcd_id].semaphore;
        ctrl_data.angle     = angle;
        ctrl_data.work_flag = SCI_FALSE;
        ctrl_data.is_full   = ( (left==0) && (top==0) && (right==lcd_width) && (bottom==lcd_height)) ? TRUE : FALSE;
        LCD_BlockOperation (lcd_id, ctrl_data);
    }
    else
    {
        //set buffer address for AHB command
        op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;
        SCI_ASSERT (PNULL != op_ptr);
        op_ptr->lcd_SetBrushDirection (angle);
        g_lcd_property_table[lcd_id].state = LCD_BUSY;
        LCD_SetBuffer (lcd_id, g_lcd_property_table[lcd_id].buffer);
        ret = op_ptr->lcd_RotationInvalidateRect (left, top, right, bottom,angle);
        SCI_ASSERT (ret == LCD_OK);
        g_lcd_property_table[lcd_id].state = LCD_READY;
        op_ptr->lcd_SetBrushDirection (LCD_ANGLE_0);
    }

    return ERR_LCD_NONE;

}//end of LCD_RotateInvalidateRect
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id   - specify LCD
//          is_sleep - specify that LCD enter into sleep mode
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_EnterSleep (LCD_ID_E  lcd_id, BOOLEAN is_sleep)
{
    LCD_OPERATIONS_T  *op_ptr;
    LCD_CTRL_MODULE_E lcd_ctrl_mode;
    LCD_CTRL_MODULE_E ctrl_mode;
    SCI_SEMAPHORE_PTR sm;
    SYS_STATE_E       flag;
    uint32 i = 0;
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {

        if (lcd_id == TV_ID)
        {
            return ERR_LCD_NONE;
        }

        while (g_lcd_property_table[lcd_id].state == LCD_BUSY)
        {
            OS_TickDelay (10);
        }

        TM_SendTestPointRequest (0x1123,20061124);
        flag = LCD_GetRunState();
        lcd_ctrl_mode  = g_lcd_property_table[lcd_id].lcd_spec.controller;

        if ( (LCD_CTRL_MODULE_LCDC == lcd_ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == lcd_ctrl_mode))
        {
            for (i=0 ; i<g_lcd_used_num ; i++)
            {
                ctrl_mode  = g_lcd_property_table[i].lcd_spec.controller;

                if ( (LCD_CTRL_MODULE_LCDC == ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == ctrl_mode))
                {
                    sm = g_lcd_property_table[i].semaphore;
                    LCD_GetSemaphore (sm, flag);
                }
            }
        }

        g_lcd_property_table[lcd_id].sleep_state = is_sleep;
        SCI_ASSERT (lcd_id < g_lcd_used_num);

        op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;
        SCI_ASSERT (PNULL != op_ptr);

        if ( (LCD_CTRL_MODULE_LCDC == lcd_ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == lcd_ctrl_mode))
        {
            for (i=0 ; i<g_lcd_used_num ; i++)
            {
                ctrl_mode  = g_lcd_property_table[i].lcd_spec.controller;

                if ( (LCD_CTRL_MODULE_LCDC == ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == ctrl_mode))
                {
                    sm = g_lcd_property_table[i].semaphore;
                    LCD_PutSemaphore (sm, flag);
                }
            }
        }

        if (SCI_TRUE == is_sleep)
        {
            op_ptr->lcd_EnterSleep (SCI_TRUE);
            LCDC_Close (SCI_TRUE);
        }
        else
        {
            LCDC_Close (SCI_FALSE);
            op_ptr->lcd_EnterSleep (SCI_FALSE);
        }

    }

#ifdef PLATFORM_SC6600V
    else
    {
        return ERR_LCD_NONE;
    }

#endif

    TM_SendTestPointRequest (0x1124,20061124);
    return ERR_LCD_NONE;
}//end of LCD_EnterSleep
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          none
//      output:
//          none
//      return:
//          SCI_TRUE : permit close LCDC
//          SCI_FALSE: don't permit close LCDC
/*****************************************************************************/
PUBLIC BOOLEAN LCD_IsPermitLcdcClose (void)
{
    uint32  i = 0;
    uint32  lcd_sleep_num = 0;
    uint32  lcdc_num = 0;
    BOOLEAN ret = 0;

    for (i=0 ; i<g_lcd_used_num ; i++)
    {
        if ( (LCD_CTRL_MODULE_LCDC == g_lcd_property_table[i].lcd_spec.controller) || (LCD_CTRL_MODULE_LCDC_LCM0 == g_lcd_property_table[i].lcd_spec.controller))
        {
            lcdc_num++;
            SCI_ASSERT (lcdc_num <= 2);

            if (g_lcd_property_table[i].sleep_state == SCI_TRUE)
            {
                lcd_sleep_num++;
            }
        }
    }

    //SCI_TRACE_LOW:"---lcdc_num=%d,lcd_sleep_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC8800H_2458_112_2_18_1_45_23_113,(uint8*)"dd",lcdc_num,lcd_sleep_num);
    ret = (lcd_sleep_num == lcdc_num) ? SCI_TRUE : SCI_FALSE;

    return ret;

}//end of LCD_IsPermitLcdcClose
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          none
//      output:
//          none
//      return:
//          SCI_TRUE : permit open LCDC
//          SCI_FALSE: don't permit open LCDC again
/*****************************************************************************/
PUBLIC BOOLEAN LCD_IsPermitLcdcOpen (void)
{
    uint32  i = 0;
    uint32  lcd_wake_num = 0;
    uint32  lcdc_num = 0;
    BOOLEAN ret = 0;

    for (i=0 ; i<g_lcd_used_num ; i++)
    {
        if ( (LCD_CTRL_MODULE_LCDC == g_lcd_property_table[i].lcd_spec.controller) || (LCD_CTRL_MODULE_LCDC_LCM0 == g_lcd_property_table[i].lcd_spec.controller))
        {
            lcdc_num++;
            SCI_ASSERT (lcdc_num <= 2);

            if (g_lcd_property_table[i].sleep_state == SCI_FALSE)
            {
                lcd_wake_num++;
            }
        }
    }

    if (lcdc_num != 1)
    {
        ret = (lcd_wake_num == lcdc_num) ? SCI_FALSE : SCI_TRUE;
    }
    else
    {
        ret = SCI_TRUE;
    }

    return ret;

}//end of LCD_IsLcdcExitSleepStatus

/*****************************************************************************/
//  Description: set contrast
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id   - specify LCD
//          contrast - contrast value
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetContrast (LCD_ID_E  lcd_id, uint16  contrast)
{
    LCD_OPERATIONS_T  *op_ptr;
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        if (lcd_id == TV_ID)
        {
            return ERR_LCD_NONE;
        }

        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_SetContrast,lcd_id = %d", lcd_id));

        op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;
        SCI_ASSERT (PNULL != op_ptr);

        return (op_ptr->lcd_SetContrast (contrast));
    }

#ifdef PLATFORM_SC6600V
    else
    {
        return LCDTV_SetContrast (lcd_id,contrast);
    }

#endif

}//end of LCD_SetContrast
/*****************************************************************************/
//  Description: set brightness
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id     - specify LCD
//          brightness - brightness value
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBrightness (LCD_ID_E lcd_id,uint16 brightness)
{
    LCD_OPERATIONS_T  *op_ptr;
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        if (lcd_id == TV_ID)
        {
            return ERR_LCD_NONE;
        }

        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_SetBrightness,lcd_id = %d", lcd_id));

        op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;
        SCI_ASSERT (PNULL != op_ptr);

        return (op_ptr->lcd_SetBrightness (brightness));
    }

#ifdef PLATFORM_SC6600V
    else
    {
        return LCDTV_SetBrightness (lcd_id,brightness);
    }

#endif
}//end of LCD_SetBrightness
/*****************************************************************************/
//  Description: set LCD display window
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id - specify LCD
//          left   - left of the rectangle
//          top    - top of the rectangle
//          right  - right of the rectangle
//          bottom - bottom of the rectangle.
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E  LCD_SetDisplayWindow (
    LCD_ID_E lcd_id,
    uint16 left,
    uint16 top,
    uint16 right,
    uint16 bottom
)
{
    LCD_OPERATIONS_T  *op_ptr;

    if (lcd_id == TV_ID)
    {
        return ERR_LCD_NONE;
    }

#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_SetDisplayWindow,lcd_id = %d", lcd_id));
        SCI_ASSERT ( (left < right) && (right < g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width));
        SCI_ASSERT ( (top < bottom) && (bottom < g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height));
        op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;
        SCI_ASSERT (PNULL != op_ptr);

        return (op_ptr->lcd_SetDisplayWindow (left, top, right, bottom));
    }

#ifdef PLATFORM_SC6600V
    else
    {
        return  LCDTV_SetDisplayWindow (lcd_id,left,top,right,bottom);

    }

#endif
}//end of LCD_SetDisplayWindow
/*****************************************************************************/
//  Description:  set start address of lcd
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id - specify LCD
//          left   - left of the rectangle
//          top    - top of the rectangle
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E  LCD_SetStartAddress (
    LCD_ID_E lcd_id,
    uint16 left,
    uint16 top
)
{
#if 0
    LCD_OPERATIONS_T  *op_ptr;

    if (lcd_id == TV_ID)
    {
        return ERR_LCD_NONE;
    }

    SCI_ASSERT (lcd_id < g_lcd_used_num);
    SCI_ASSERT ( (left < g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width));
    SCI_ASSERT ( (top < g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height));

    op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;
    SCI_ASSERT (PNULL != op_ptr);

    return (op_ptr->lcd_SetStartAddress (left, top));
#else
    ERR_LCD_E re_value=ERR_LCD_NONE;

    return re_value;
#endif

}//end of LCD_SetStartAddress
/*****************************************************************************/
//  Description:  clear the whole lcd to one color.
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id - specify LCD
//          color  - color to fill the whole lcd.
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
PUBLIC void LCD_Clear (
    LCD_ID_E lcd_id,
    uint32 color
)
{
    LCD_OPERATIONS_T  *op_ptr;
    LCD_BRUSH_MODE_E  mode;
    SCI_SEMAPHORE_PTR sm;
    SYS_STATE_E       flag;
    uint32            ctrl_mode;
    uint32            index = 0;
    uint32            i = 0;
    uint32            start_time = 0;
    uint32            end_time   = 0;
    LCD_INT_ENA_T     int_type = {0};
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        LCDC_DisableLcdBlock();

        flag = LCD_GetRunState();

        if (lcd_id == TV_ID)
        {
            LCDC_SetBackground (color);
            LCDC_UpdateRegisters();
        }
        else
        {
            LCDC_SetBackground (color);
            SCI_ASSERT (lcd_id < g_lcd_used_num);
            mode      = g_lcd_property_table[lcd_id].brush_mode;
            ctrl_mode = g_lcd_property_table[lcd_id].lcd_spec.controller;

            if (mode == LCD_BRUSH_MODE_ASYNC)
            {
                LCD_SetBrushMode (lcd_id, LCD_BRUSH_MODE_SYNC);
            }

            sm = g_lcd_property_table[lcd_id].semaphore;
            LCD_GetSemaphore (sm, flag);
            int_type.mcu_endofosd_en = 0;
            int_type.rgb_int_enable.end_of_frame =0;
            LCDC_SpecialMode (&int_type);
            g_lcd_property_table[lcd_id].state = LCD_BUSY;

            DPSLP_Lcdc_EnableAhbSleep (DISABLE_AHB_SLEEP);
            CHNG_FREQ_Event_Clr (FREQ_CHNG_EVENT_LCDC,SYS_MODE_NORMAL);

            if (g_lcd_property_table[lcd_id].operation == BLOCK_OPERATION)
            {
                if ( (LCD_CTRL_MODULE_LCDC == ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == ctrl_mode))
                {
                    index = g_lcd_property_table[lcd_id].blk_group_num;

                    for (i=0 ; i<BLOCK_SUM ; i++)
                    {
                        g_block[index].block[i].state = MEMORY_READY;
                    }

                    op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;
                    SCI_ASSERT (PNULL != op_ptr);

                    op_ptr->lcd_Invalidate();

                    LCDC_TransCmd ( (g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width), (g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height),g_lcd_property_table[lcd_id].lcd_spec.cs_pin);
                    start_time = SCI_GetTickCount();

                    while (LCDC_CheckBrushLcdStatue() == 0)
                    {
                        end_time = SCI_GetTickCount();
                        SCI_ASSERT ( (end_time - start_time) < LCD_BRUSH_TIME_OUT);
                    }

                    LCDC_ClearStatus();
                    g_lcd_property_table[lcd_id].state = LCD_READY;
                    DPSLP_Lcdc_EnableAhbSleep (ENABLE_AHB_SLEEP);
                    CHNG_FREQ_Event_Set (FREQ_CHNG_EVENT_LCDC,SYS_MODE_NORMAL);
                    int_type.mcu_endofosd_en = 1;
                    int_type.rgb_int_enable.end_of_frame =0;
                    LCDC_SpecialMode (&int_type);
                    LCD_PutSemaphore (sm,flag);
                }
            }
            else
            {
                //set buffer address for AHB command
                g_lcd_property_table[lcd_id].state = LCD_BUSY;

                DPSLP_Lcdc_EnableAhbSleep (DISABLE_AHB_SLEEP);
                CHNG_FREQ_Event_Clr (FREQ_CHNG_EVENT_LCDC,SYS_MODE_NORMAL);
                LCD_SetBuffer (lcd_id, g_lcd_property_table[lcd_id].buffer);
                op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;
                SCI_ASSERT (PNULL != op_ptr);

                op_ptr->lcd_Invalidate();
                g_lcd_property_table[lcd_id].state = LCD_READY;
                DPSLP_Lcdc_EnableAhbSleep (ENABLE_AHB_SLEEP);
                CHNG_FREQ_Event_Set (FREQ_CHNG_EVENT_LCDC,SYS_MODE_NORMAL);
            }


            if (mode == LCD_BRUSH_MODE_ASYNC)
            {
                LCD_SetBrushMode (lcd_id, LCD_BRUSH_MODE_ASYNC);
            }


        }
    }

#ifdef PLATFORM_SC6600V
    else
    {
        LCDTV_Clear (lcd_id,color);
    }

#endif
}//end of LCD_Clear


/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_GetLCDCSemaphore (void)
{
    SYS_STATE_E         flag;

    flag = LCD_GetRunState();
    LCD_GetSemaphore (g_lcdc_sm_ptr, flag);
}//end of LCD_GetLCDCSemaphor
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_PutLCDCSemaphore (void)
{
    SYS_STATE_E         flag;

    flag = LCD_GetRunState();
    LCD_PutSemaphore (g_lcdc_sm_ptr, flag);
}//end of LCD_PutLCDCSemaphore
/*****************************************************************************/
//  Description: Update LCD Timing
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_UpdateTiming (uint32 ahb_clk)
{
    uint32              i,j;
    uint32              ctrl_module;
    uint32              ahb_clk_cycle;
    uint32              cs_id = 0;
    BOOLEAN             lcd_sleep_flag = SCI_TRUE;
    SYS_STATE_E         flag;
    LCD_MODE_E          lcd_mode;
    LCD_INFO_T          lcd_info;
    LCD_TIMING_U        timing_para;
 

    if (g_lcdc_tv_flag == 1)
    {
        //SCI_TRACE_LOW:"----LCD_UpdateTiming-----"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC8800H_2858_112_2_18_1_45_23_114,(uint8*)"");
        return LCD_OK;
    }

    SCI_ASSERT ( (ahb_clk > 0) && (ahb_clk<100000000));
    ahb_clk_cycle = 1000000000/ahb_clk;
    //SCI_TRACE_LOW:"---LCD_UpdateTiming---: AHB Clock:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC8800H_2864_112_2_18_1_45_24_115,(uint8*)"d",ahb_clk);

    flag = LCD_GetRunState();

    for (i=0 ; i<g_lcd_used_num ; i++)
    {
        if (g_lcd_property_table[i].sleep_state == SCI_FALSE)
        {
            lcd_sleep_flag = SCI_FALSE;
            break;
        }
    }

    if (lcd_sleep_flag == SCI_FALSE)
    {
        for (i=0 ; i<g_lcd_used_num ; i++)
        {
            if ( (LCD_CTRL_MODULE_LCDC == g_lcd_property_table[i].lcd_spec.controller) || (LCD_CTRL_MODULE_LCDC_LCM0 == g_lcd_property_table[i].lcd_spec.controller))
            {
                //SCI_TRACE_LOW:"LCD_UpdateTiming:get semaphore 1"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC8800H_2883_112_2_18_1_45_24_116,(uint8*)"");
                LCD_GetSemaphore (g_lcdc_sm_ptr, flag);
                break;
            }
        }

        LCDC_Close (SCI_FALSE);
    }

    for (i=0 ; i<g_lcd_used_num ; i++)
    {
        ctrl_module = g_lcd_property_table[i].lcd_spec.controller;
        lcd_mode    = g_lcd_property_table[i].lcd_spec.spec_ptr->mode;
        LCD_GetInfo (i, &lcd_info);
        cs_id = g_lcd_property_table[i].lcd_spec.cs_pin;


        if ( ( (LCD_CTRL_MODULE_LCDC == ctrl_module) || (LCD_CTRL_MODULE_LCDC_LCM0 == ctrl_module)) && (lcd_mode == LCD_MCU))
        {
            timing_para.mcu_mode_timing.rcss = ( (g_lcd_property_table[i].lcd_spec.spec_ptr->timing->mcu_mode_timing.rcss/ahb_clk_cycle + 1) <=3)  ? (g_lcd_property_table[i].lcd_spec.spec_ptr->timing->mcu_mode_timing.rcss/ahb_clk_cycle + 1) : 3;
            timing_para.mcu_mode_timing.rlpw = ( (g_lcd_property_table[i].lcd_spec.spec_ptr->timing->mcu_mode_timing.rlpw/ahb_clk_cycle + 1) <=15) ? (g_lcd_property_table[i].lcd_spec.spec_ptr->timing->mcu_mode_timing.rlpw/ahb_clk_cycle + 1) : 15;
            timing_para.mcu_mode_timing.rhpw = ( (g_lcd_property_table[i].lcd_spec.spec_ptr->timing->mcu_mode_timing.rhpw/ahb_clk_cycle + 1) <=15) ? (g_lcd_property_table[i].lcd_spec.spec_ptr->timing->mcu_mode_timing.rhpw/ahb_clk_cycle + 1) : 15;
            timing_para.mcu_mode_timing.wcss = ( (g_lcd_property_table[i].lcd_spec.spec_ptr->timing->mcu_mode_timing.wcss/ahb_clk_cycle + 1) <=3)  ? (g_lcd_property_table[i].lcd_spec.spec_ptr->timing->mcu_mode_timing.wcss/ahb_clk_cycle + 1) : 3;
            timing_para.mcu_mode_timing.wlpw = ( (g_lcd_property_table[i].lcd_spec.spec_ptr->timing->mcu_mode_timing.wlpw/ahb_clk_cycle + 1) <=15) ? (g_lcd_property_table[i].lcd_spec.spec_ptr->timing->mcu_mode_timing.wlpw/ahb_clk_cycle + 1) : 15;
            timing_para.mcu_mode_timing.whpw = ( (g_lcd_property_table[i].lcd_spec.spec_ptr->timing->mcu_mode_timing.whpw/ahb_clk_cycle + 1) <=15) ? (g_lcd_property_table[i].lcd_spec.spec_ptr->timing->mcu_mode_timing.whpw/ahb_clk_cycle + 1) : 15;
            SCI_ASSERT (cs_id < 2);
            LCDC_ChangePulseWidth (LCD_MCU, cs_id, timing_para);
            cs_id++;
        }
        else if ( (ctrl_module == LCD_CTRL_MODULE_LCDC) && (lcd_mode == LCD_RGB))
        {
            timing_para.rgb_mode_timing.bfw = g_lcd_property_table[i].lcd_spec.spec_ptr->timing->rgb_mode_timing.bfw/ahb_clk_cycle - 1;
            timing_para.rgb_mode_timing.efw = g_lcd_property_table[i].lcd_spec.spec_ptr->timing->rgb_mode_timing.efw/ahb_clk_cycle - 1;
            timing_para.rgb_mode_timing.vsw = g_lcd_property_table[i].lcd_spec.spec_ptr->timing->rgb_mode_timing.vsw/ahb_clk_cycle - 1;
            timing_para.rgb_mode_timing.blw = g_lcd_property_table[i].lcd_spec.spec_ptr->timing->rgb_mode_timing.blw/ahb_clk_cycle - 1;
            timing_para.rgb_mode_timing.elw = g_lcd_property_table[i].lcd_spec.spec_ptr->timing->rgb_mode_timing.elw/ahb_clk_cycle - 1;
            timing_para.rgb_mode_timing.hsw = g_lcd_property_table[i].lcd_spec.spec_ptr->timing->rgb_mode_timing.hsw/ahb_clk_cycle - 1;
            LCDC_ChangePulseWidth (LCD_RGB, 0 ,timing_para);
            LCDC_UpdateRegisters();
        }

    }
    if (lcd_sleep_flag == SCI_FALSE)
    {
        LCDC_Close (SCI_TRUE);

        for (j=0 ; j<g_lcd_used_num ; j++)
        {
            if ( (LCD_CTRL_MODULE_LCDC == g_lcd_property_table[j].lcd_spec.controller) || (LCD_CTRL_MODULE_LCDC_LCM0 == g_lcd_property_table[j].lcd_spec.controller))
            {
                LCD_PutSemaphore (g_lcdc_sm_ptr, flag);
                //SCI_TRACE_LOW:"LCD_UpdateTiming:put semaphore 1"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC8800H_2934_112_2_18_1_45_24_117,(uint8*)"");
                break;
            }
        }
    }
    return ERR_LCD_NONE;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          is_used - SCI_TRUE:use scaling down; SCI_FALSE:don't use scaling down
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
PUBLIC void LCD_IsSelectScalingDown (BOOLEAN is_used)
{
#if IS_USE_SCALING_FUNC

    SYS_STATE_E       flag;


    flag = LCD_GetRunState();

    LCD_GetSemaphore (g_tv_sm_ptr, flag);
    s_scaling_used_falg = is_used;
    LCD_PutSemaphore (g_tv_sm_ptr,flag);

#endif

}//end of LCD_IsSelectScalingDown

/*****************************************************************************/
//  Description: Compound Photo to ISP Module
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//  Note:
//      input:
//          lcd_id - specify LCD
//          left   - left of the rectangle
//          top    - top of the rectangle
//          right  - right of the rectangle
//          bottom - bottom of the rectangle.
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_CompoundPhoto (
    LCD_ID_E lcd_id,
    uint16 left,
    uint16 top,
    uint16 right,
    uint16 bottom
)
{

    LCDC_LCD_CTRL_REG_U *lcdc_ctrl_ptr = (LCDC_LCD_CTRL_REG_U *) LCDC_LCD_CTRL_REG;
    LCD_BRUSH_MODE_E    mode;
    SCI_SEMAPHORE_PTR   sm;
    LCD_OPERATIONS_T    *op_ptr;
    uint32              index;
    uint32              ctrl_mode;
    uint32              i = 0;
    uint32              temp = 0;
    uint32              lcd_width;
    uint32              lcd_height;
    uint32              cur_lcdc_clk = 0;
    BLOCK_POSITION_T    blk_pos     = {0};
    uint32              u32_cs_back = 0;
    LCD_INT_ENA_T       int_type ={0};

    if (lcd_id == TV_ID)
    {
        return ERR_LCD_NONE;
    }

    SCI_ASSERT (lcd_id < g_lcd_used_num);

    if ( (g_lcdc_tv_flag == 1) &&
            ( (g_lcd_property_table[lcd_id].lcd_spec.controller == LCD_CTRL_MODULE_LCDC)
              || (g_lcd_property_table[lcd_id].lcd_spec.controller == LCD_CTRL_MODULE_LCDC_LCM0)))
    {
        return 0;
    }

    SCI_ASSERT (2* (right-left+1) <= LCDC_LCD_SIZE_WIDTH_MAX);
    SCI_ASSERT ( (bottom-top+1)   <= LCDC_LCD_SIZE_HEIGHT_MAX);
    op_ptr = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;

    SCI_ASSERT (PNULL != op_ptr);

    MMU_InvalideDCACHE ();

    mode       = g_lcd_property_table[lcd_id].brush_mode;
    ctrl_mode  = g_lcd_property_table[lcd_id].lcd_spec.controller;
    lcd_width  = LCDC_LCD_SIZE_WIDTH_MAX/2 - 1;
    lcd_height = LCDC_LCD_SIZE_HEIGHT_MAX  - 1;
    index      = g_lcd_property_table[lcd_id].blk_group_num;

    if ( (LCD_CTRL_MODULE_LCDC != ctrl_mode) && (LCD_CTRL_MODULE_LCDC_LCM0 != ctrl_mode))
    {
        return ERR_LCD_NONE;
    }

    left  = left - left%8;
    right = ( (right-left+1) %8 == 0) ? right : (right + 8 - (right-left+1) %8);
    right = (right > lcd_width) ? (lcd_width - (lcd_width-left+1) %8) : right;

    temp = (right-left+1) * (bottom-top+1);

    if (temp < LCDC_UPDATE_PIXELS_MIN)
    {
        if ( (top - 8) >= 0)
        {
            top -= 8;
        }
        else if ( (bottom + 8) <= lcd_height)
        {
            bottom += 8;
        }
    }

    temp = (right-left+1) * (bottom-top+1);

    if ( (temp%16) != 0)
    {
        if ( (top - 1) >= 0)
        {
            top -= 1;
        }
        else if ( (bottom + 1) <= lcd_height)
        {
            bottom += 1;
        }
    }

    if (mode == LCD_BRUSH_MODE_ASYNC)
    {
        LCD_SetBrushMode (lcd_id, LCD_BRUSH_MODE_SYNC);
    }

    cur_lcdc_clk = LCDC_GetClock();
    LCDC_RestoreClock();

    sm = g_lcd_property_table[lcd_id].semaphore;
    LCD_GetSemaphore (sm, SYS_STATE_NORMAL);
    LCD_GetSemaphore (g_lcdc_sm_ptr, SYS_STATE_NORMAL);
    int_type.mcu_endofosd_en = 0;
    int_type.rgb_int_enable.end_of_frame =0;
    LCDC_SpecialMode (&int_type);
    //disable LCDC output to LCD
    lcdc_ctrl_ptr->mBits.feedback_mode = 0x1;
    lcdc_ctrl_ptr->mBits.disable_lcd_out = 0x1;
    LCDC_SetDisplayWindows (left,top,right,bottom);

    if (g_lcd_property_table[lcd_id].operation == BLOCK_OPERATION)
    {
        blk_pos.left = left;
        blk_pos.top  = top;
        blk_pos.right = right;
        blk_pos.bottom = bottom;
        blk_pos.work_flag = SCI_FALSE;
        ConfigBlock (lcd_id, blk_pos);

        REG32 (LCDC_LCD_BLOCK2CONFIG) |= BIT_27|BIT_28;
        REG32 (LCDC_LCD_BLOCK2CONFIG) |= 0Xff;

        REG32 (0x8b000060) |= BIT_4;

        u32_cs_back = REG32 (0x8c000278);
        REG32 (0x8c000278) |= 0xc0;
        LCDC_TransCmd ( (right-left+1), (bottom-top+1),g_lcd_property_table[lcd_id].lcd_spec.cs_pin);

        while (LCDC_CheckBrushLcdStatue() == 0);

        LCDC_ClearStatus();

        for (i=0 ; i<BLOCK_SUM ; i++)
        {
            g_block[index].block[i].state = MEMORY_READY;
        }

        //enable LCDC output to LCD
        lcdc_ctrl_ptr->mBits.feedback_mode = 0x0;
        lcdc_ctrl_ptr->mBits.disable_lcd_out = 0x0;
        REG32 (0x8c000278) = u32_cs_back;
        int_type.mcu_endofosd_en = 1;
        int_type.rgb_int_enable.end_of_frame =0;
        LCDC_SpecialMode (&int_type);
        LCD_PutSemaphore (g_lcdc_sm_ptr, SYS_STATE_NORMAL);
        LCD_PutSemaphore (sm, SYS_STATE_NORMAL);
        LCDC_SetClock (cur_lcdc_clk);
    }

    if (mode == LCD_BRUSH_MODE_ASYNC)
    {
        LCD_SetBrushMode (lcd_id, LCD_BRUSH_MODE_ASYNC);
    }

    REG32 (0x8b000060) &= ~BIT_4;
    LCD_PutLcdCtrlSemaphore(lcd_id);
    return ERR_LCD_NONE;
}//end of LCD_CompoundPhoto
/*****************************************************************************/
//  Description: LCD out data to memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          pos_param - out position parameter
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
ERR_LCD_E LCD_OutputDataToMemory (LCD_OUT_POSITION_T pos_param)
{
    uint32 param_arr[5] = {0};
    ERR_LCD_E ret = ERR_LCD_NONE;

    param_arr[0] = (uint32) (pos_param.data_buf);
    param_arr[1] = 0;
    param_arr[2] = 0;
    param_arr[3] = pos_param.width;
    param_arr[4] = pos_param.height;

    ISP_ServiceOpen();

    ISP_BeforeGetdataFromLCDC (param_arr);

    //start LCDC
    ret = LCD_CompoundPhoto (pos_param.lcd_id,pos_param.start_x,pos_param.start_y, (pos_param.start_x+pos_param.width-1), (pos_param.start_y + pos_param.height-1));
    //SCI_Sleep(10);
    ISP_AfterGetdataFromLCDC();

    ISP_ServiceClose();

    return ret;
}//end of LCD_OutputDataToMemory
/*****************************************************************************/
//  Description: LCD out data to memory
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          pos_param - out position parameter
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_InvalidateRectToMemory(LCD_ID_E lcd_id,LCD_MEMORY_INFO_T *param_ptr)
{
    LCD_OUT_POSITION_T out_param = {0};
    out_param.data_buf = (uint32*)param_ptr->buf_addr;
    out_param.lcd_id   = lcd_id;
    out_param.start_x  = param_ptr->cap_rect.left;
    out_param.start_y  = param_ptr->cap_rect.top;
    out_param.width    = param_ptr->dst_size.w;
    out_param.height   = param_ptr->dst_size.h;    
    LCD_OutputDataToMemory(out_param );    

    return ERR_LCD_NONE;
}
/*****************************************************************************/
//  Description: set UV buffer for block 0
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetUVBuffer(
			                            LCD_ID_E lcd_id, 
                                        uint32 blk_num, 
                                        uint32 *buf_ptr
                                        )
{
    return ERR_LCD_NONE;
}
/*****************************************************************************/
//  Description: Initialize LCD controller when occur assert info
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          none
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_InitInAssertRestMode (void)
{
    LCD_InitOpearion (SCI_FALSE);

    return ERR_LCD_NONE;
}//end of LCD_InitInAssertRestMode
/*****************************************************************************/
//  Description: initialize all LCD when occur assert
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          none
//      output:
//          none
//      return:
//          result
/*****************************************************************************/
LOCAL void LCD_AssertInit (void)
{
    uint32              i      = 0;
    volatile uint32     cs_id  = 0;
    uint32              lcd_id = 0;
    uint32              error  = 0;
    uint32              ahb_clk;
    uint32              ctrl_module;
    uint32              size;
    LCDC_SPEC_T         mcu_lcd_spec      = {0};
    LCDC_TYPE_T         lcdc_type         = {0};
    AHB_SOFT_RST_U      *ahb_soft_rst_ptr = (AHB_SOFT_RST_U *) AHB_SOFT_RST;
    uint32              lcdc_reg_addr;
    uint32              mcu_reg_addr;
    ahb_clk = CHIP_GetAhbClk();

#if _DEBUG_LCDC
    lcdc_reg_addr = LCDC_LCD_MODE;

    for (i=0 ; i<LCDC_REG_NUM ; i++)
    {
        g_lcdc_reg_info[i] = REG32 (lcdc_reg_addr+i*4);
    }

    mcu_reg_addr = LCDC_LCMPARAMETER0;

    for (i=0 ; i<LCDC_MCU_REG_NUM ; i++)
    {
        g_lcdc_mcu_reg_info[i] = REG32 (mcu_reg_addr+i*4);
    }

    g_clock[0] = CHIP_GetVPllClk();
    g_clock[1] = REG32 (GR_GEN4);
    g_clock[2] = CHIP_GetAhbClk();
    g_clock[3] = CHIP_GetMPllClk();
    g_clock[4] = REG32 (LCDC_LCD_INT_ENABLE);
    g_clock[5] = REG32 (LCDC_LCD_INT_STATUS);
    g_clock[6] = REG32 (LCDC_LCD_INT_CLEAR);
    g_clock[7] = g_lcd_property_table[0].state;
#endif

    ctrl_module = g_lcd_property_table[lcd_id].lcd_spec.controller;
    lcdc_type.controller = ctrl_module;

    if ( (LCD_CTRL_MODULE_LCDC == ctrl_module) || (LCD_CTRL_MODULE_LCDC_LCM0 == ctrl_module))
    {
        LCDC_SelectRstFuc (0);
        ahb_soft_rst_ptr->mBits.lcdc_soft_rst = 0x1;
        OS_TickDelay (1);
        ahb_soft_rst_ptr->mBits.lcdc_soft_rst = 0x0;
        g_lcdc_is_used = 1;

        if (g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->mode == LCD_MCU)
        {
            lcdc_type.lcdc_mode = 1;

            if (cs_id == 0)
            {
                lcdc_type.main_lcd_width  = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width;
                lcdc_type.main_lcd_height = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height;
                mcu_lcd_spec.mcu_lcd_spec.modesel.mBits.modesel0     = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->bus_mode;
                mcu_lcd_spec.mcu_lcd_spec.rgbmode.mBits.cs0_rgb_mode = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->bus_width;
                cs_id++;
            }
            else
            {
                lcdc_type.sub_lcd_width  = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width;
                lcdc_type.sub_lcd_height = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height;
                mcu_lcd_spec.mcu_lcd_spec.modesel.mBits.modesel1     = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->bus_mode;
                mcu_lcd_spec.mcu_lcd_spec.rgbmode.mBits.cs1_rgb_mode = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->bus_width;
                cs_id++;
            }

            SCI_ASSERT (cs_id <= 2);
        }
        else
        {
            SCI_MEMSET ( (uint8 *) &lcdc_type, 0 , sizeof (LCDC_TYPE_T));
            lcdc_type.main_lcd_width  = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width;
            lcdc_type.main_lcd_height = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height;
            lcdc_type.lcd_background  = LCD_BACKGROUND;
        }
    }
    else
    {
        size = g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->width * g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->height * 2;
        g_lcd_property_table[lcd_id].operation        = AHB_COMMAND_OPERATION;
        g_lcd_property_table[lcd_id].buffer = (uint32 *) SCI_ALLOC_APP (size);
    }


    if (g_lcdc_is_used)
    {
        if (lcdc_type.lcdc_mode == 1)
        {
            lcdc_type.mcu_lcd_sum    = cs_id;
            lcdc_type.lcd_background = LCD_BACKGROUND;
            LCDC_SetLcdType (&lcdc_type, g_lcd_property_table[lcd_id].lcd_spec.cs_pin);
            LCDC_Init();
            error = LCDC_Configure (&mcu_lcd_spec);
        }
        else//rgb mode
        {
            LCDC_SetLcdType (&lcdc_type, g_lcd_property_table[lcd_id].lcd_spec.cs_pin);
            LCDC_Init();
        }
    }

    LCD_UpdateTiming (ahb_clk);

    for (i=0 ; i<g_lcd_used_num ; i++)
    {
        g_lcd_property_table[i].brush_mode = LCD_BRUSH_MODE_SYNC;
        g_lcd_property_table[lcd_id].state = LCD_READY;
    }


}//end of LCD_AssertInit
/*****************************************************************************/
//  Description:  Get lcd function pointer
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id - specify LCD
//      output:
//          none
//      return:
//          the pointer to LCD OPERATIONS structure
/*****************************************************************************/
PUBLIC  LCD_OPERATIONS_T *LCD_GetLCDPointer (LCD_ID_E lcd_id)
{
    if (lcd_id == TV_ID)
    {
        return ERR_LCD_NONE;
    }

    SCI_ASSERT (lcd_id < g_lcd_used_num);

    return (g_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation);
}//end of LCD_GetLCDPointer
/*****************************************************************************/
//  Description:get memory status
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//          mem_ptr - memory address
//      output:
//          none
//      return:
//          memory status
/*****************************************************************************/
PUBLIC LCD_MEMORY_STATE_E LCD_GetMemoryStatus (LCD_ID_E lcd_id, uint32 *mem_ptr)
{
    uint32 i = 0;
    uint32 index = 0;
    uint32 ctrl_mode;
    LCD_MEMORY_STATE_E ret = MEMORY_IDLE;

    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_GetMemoryStatus,lcd_id = %d", lcd_id));
    }

    SCI_ASSERT (mem_ptr != PNULL);

    ctrl_mode = g_lcd_property_table[lcd_id].lcd_spec.controller;

    if (g_lcd_property_table[lcd_id].operation == BLOCK_OPERATION)
    {
        if ( (LCD_CTRL_MODULE_LCDC == ctrl_mode) || (LCD_CTRL_MODULE_LCDC_LCM0 == ctrl_mode))
        {
            index = g_lcd_property_table[lcd_id].blk_group_num;

            for (i=0 ; i<LCDC_BLOCK_NUM ; i++)
            {
                if (mem_ptr == g_block[index].block[i].mem_ptr)
                {
                    ret = (LCD_MEMORY_STATE_E) (g_block[index].block[i].state);
                }

            }
        }
    }
    else
    {
        ret = (LCD_MEMORY_STATE_E) (g_lcd_property_table[lcd_id].state);
    }

    return ret;

}//end of LCD_GetMemoryStatus
/*****************************************************************************/
//  Description:get memory status
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          lcd_id  - specify LCD
//      output:
//          none
//      return:
//          lcd status: LCD_IDLE  - don't use LCD ID
//                      LCD_BUSY  - is brushing this LCD
//                      LCD_READY - finish brushing this LCD
//                      LCD_ERROR - error
/*****************************************************************************/
PUBLIC LCD_STATE_E LCD_GetLCDState (LCD_ID_E lcd_id)
{
    LCD_STATE_E lcd_state;

    if (lcd_id != TV_ID)
    {
        SCI_PASSERT ( (lcd_id < g_lcd_used_num), ("LCD_GetLCDState,lcd_id = %d", lcd_id));
    }

    lcd_state = g_lcd_property_table[lcd_id].state;

    return lcd_state;
}//end of LCD_GetLCDState

/**********************************************************************************************************/
// Description: software reset LCDC and TV Encoder
// Global resource dependence:
// Author: Jianping.wang
// Note:wirte 1-0
/**********************************************************************************************************/
PUBLIC void LCD_Reset (void)
{
    LCDC_ResetLcd (5);

}

/**********************************************************************************************************/
// Description: send the command and data to LCD
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      input:
//          cmd - lcd command
//          lcd_data - lcd command data
//          lcd_id - only is used for lcdc mcu mode
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCD_SendCmdData (uint32 cmd,uint32 lcd_data,uint32 lcd_id)
{
    LCDC_SendCmdData (cmd, lcd_data, lcd_id);

    return LCD_OK;
}//end of LCDC_SendCmdData
/**********************************************************************************************************/
// Description: send command to lcd
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      lcd_id parameter is used when lcdc mcu mode
//      input:
//          cmd - lcd command
//          lcd_id - LCD ID
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCD_SendCmd (uint32 cmd,uint32 lcd_id)
{
    LCDC_SendCmd (cmd, lcd_id);

    return LCD_OK;
}//end of LCDC_SendCmd
/**********************************************************************************************************/
// Description: send display data to LCD
// Global resource dependence: g_lcdc_mode
// Author: Jianping.wang
// Note:
//      this interface is used in lcdc mcu mode
//      input:
//          lcd_data - display data
//          lcd_id - LCD ID
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC int32 LCD_SendData (uint32 lcd_data,uint32 lcd_id)
{
    LCDC_SendData (lcd_data, lcd_id);

    return LCD_OK;
}//end of LCDC_SendData

/**********************************************************************************************************/
// Description: set LCD background color
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/

PUBLIC void LCD_SetBackground (uint32 color)
{
    LCDC_SetBackground (color);
}

/*****************************************************************************/
//  Description:  save LCD operation structure pointer
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
//      input:
//          none
//      output:
//          none
//      return:
//          none
/*****************************************************************************/
PUBLIC void LCD_PointerSaveInAssertResetMode (void)
{
    uint32 i = 0;

    for (i=0 ; i<g_lcd_used_num ; i++)
    {
        REG32 (g_mem_limit - (8 - (i<<2))) = (uint32) LCD_GetLCDPointer (i);
    }
}//end of LCD_PointerSaveInAssertResetMode
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


LOCAL uint16 font_color = 0x00;


/*****************************************************************************/
//  Description:  Set 8 pixels from(x,y) to(x+7,y).
//  Global resource dependence:
//  Author: Jim.zhang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_Set8Pixels (uint16 cx, uint16 cy, uint8 bit_mask)
{
    uint16 *buffer_ptr = 0;
    uint16 i;
    LCD_INFO_T  lcd_info;
    uint16 width;

    LCD_GetInfo (MAIN_LCD_ID,&lcd_info);

    buffer_ptr = lcd_info.lcdbuff_ptr;
    width = lcd_info.lcd_width;

    for (i=0; i<8; i++)
    {
        if ( (bit_mask & (0x01<<i)) !=0x00)
        {
            * (buffer_ptr+cy*width+cx+i) = font_color;    //( cx+i, cy, color);
        }
    }
}


/*****************************************************************************/
//  Description:  display the text(assert information) when assert.(just english)
//  Global resource dependence:
//  Author: Jim.zhang
//  Note:
/*****************************************************************************/
PUBLIC void LCD_DisplayText (uint16 x, uint16 y, char *text, uint16 len)
{
    uint32  index;
    uint16  pos_x = x, pos_y = y;
    uint32  offset, i;
    uint16  width;
    uint16  height;
    uint16  *mem_ptr = PNULL;
    uint32  *mem_copy = PNULL;
    uint8   *data_ptr;
    char    ch;
    LCD_INFO_T lcd_info;

    LCD_GetInfo (MAIN_LCD_ID,&lcd_info);

    //assert state
    s_sys_run_state = SYS_STATE_ASSERT;

    if (g_lcdc_tv_flag)
    {
        //      LCDC_CloseTvDAC();
    }

    LCD_AssertInit();

    width = g_lcd_property_table[LCD_ID_0].lcd_spec.spec_ptr->width;
    height = g_lcd_property_table[LCD_ID_0].lcd_spec.spec_ptr->height;

    if ( (g_lcd_property_table[LCD_ID_0].lcd_spec.controller == LCD_CTRL_MODULE_LCDC)
            || (g_lcd_property_table[LCD_ID_0].lcd_spec.controller == LCD_CTRL_MODULE_LCDC_LCM0))
    {

        mem_ptr = (uint16 *) lcd_info.lcdbuff_ptr;
        mem_copy = (uint32 *) mem_ptr;

        for (i=0 ; i< (width*height/2) ; i++)
        {
            *mem_copy++ = 0x07e007e0;
        }
    }

    if ( (len==0) || (text == NULL))
    {
        return;
    }

    if (x > (width-8))
    {
        pos_x = 0;
        pos_y += 16;
    }

    if (pos_y > (height-16))
    {
        return;
    }

    //@lin.liu(2004-04-21). clear the screen

    LCDC_DisableLcdBlock();
    LCDC_SetBackground (0x0f0f);

    while (len--)
    {
        ch = *text++;

        if ( (ch <= 0x20) || (ch & 0x80))
        {
            offset = 0;  // invalid char, use the default char(' ')
        }
        else
        {
            // 16 bytes per char.
            offset = (uint16) (ch - 0x20) * 16; //lint !e571
        }

        data_ptr = (uint8 *) &ascii_8X16[offset];

        // show this char
        for (i = 0; i < 16; i++)
        {
            LCD_Set8Pixels (pos_x, pos_y+i, *data_ptr++);
        }

        pos_x +=8;

        if ( (pos_x + 8) > width) // Next line.
        {
            pos_y += 16;
            pos_x  = 0;
        }

        if (pos_y > (height-16))
        {
            break;
        }
    }

    if ( (g_lcd_property_table[LCD_ID_0].lcd_spec.controller == LCD_CTRL_MODULE_LCDC)
            || (g_lcd_property_table[LCD_ID_0].lcd_spec.controller == LCD_CTRL_MODULE_LCDC_LCM0))
    {
        index = g_lcd_property_table[0].blk_group_num;
        g_block[index].block[1].is_enable       = 1;
        g_block[index].block[1].cfg.priority    = 7;
        g_block[index].block[1].cfg.start_x     = 0;
        g_block[index].block[1].cfg.start_y     = 0;
        g_block[index].block[1].cfg.width       = width;
        g_block[index].block[1].cfg.end_x       = width - 1;
        g_block[index].block[1].cfg.end_y       = height - 1;;
        g_block[index].block[1].cfg.resolution  = 2;
        g_block[index].block[1].cfg.type        = 1;
        g_block[index].block[1].cfg.colorkey    = 0;
        g_block[index].block[1].cfg.alpha       = 255;
        g_block[index].block[1].mem_ptr         = (uint32 *) mem_ptr;
        g_block[index].lcd_id = 0;
        g_block[index].block[1].cfg.colorkey_en = 0;
        g_block[index].block[1].cfg.alpha_sel   = 1;

        g_block[index].block[0].is_enable       = 0;
        g_block[index].block[2].is_enable       = 0;
        g_block[index].block[3].is_enable       = 0;
        g_block[index].block[4].is_enable       = 0;
        g_block[index].block[5].is_enable       = 0;

    }

    /* end CR:MS10504 */
    LCD_DisplayOn();
    LCD_AssertInvalidateRect (LCD_ID_0, 0, y, width-1, pos_y+15);
    /* @jim.zhang CR:MS10504 2004-07-05 */
}
/*****************************************************************************/
//  Description:  display the text(assert information) when assert.(just english)
//  Global resource dependence:
//  Author: Jim.zhang
//  Note: DM module used only.
/*****************************************************************************/
PUBLIC void LCD_DisplayText_Fota (uint16 x, uint16 y, char *text, uint16 len)
{
    uint32  index;
    uint16  pos_x = x, pos_y = y;
    uint16  offset, i;
    uint16  width;
    uint16  height;
    uint16  *mem_ptr = PNULL;
    uint32  *mem_copy = PNULL;
    uint8   *data_ptr;
    char    ch;
    LCD_INFO_T lcd_info;

    LCD_GetInfo (MAIN_LCD_ID,&lcd_info);

    s_sys_run_state = SYS_STATE_ASSERT;

    width = g_lcd_property_table[LCD_ID_0].lcd_spec.spec_ptr->width;
    height = g_lcd_property_table[LCD_ID_0].lcd_spec.spec_ptr->height;

    if ( (g_lcd_property_table[LCD_ID_0].lcd_spec.controller == LCD_CTRL_MODULE_LCDC)
            || (g_lcd_property_table[LCD_ID_0].lcd_spec.controller == LCD_CTRL_MODULE_LCDC_LCM0))
    {

        mem_ptr = (uint16 *) lcd_info.lcdbuff_ptr;
        mem_copy = (uint32 *) mem_ptr;

        for (i=0 ; i< (width*height/2) ; i++)
        {
            *mem_copy++ = 0xFFFFFFFF;
        }
    }

    if ( (len==0) || (text == NULL))
    {
        return;
    }

    if (x > (width-8))
    {
        pos_x = 0;
        pos_y += 16;
    }

    if (pos_y > (height-16))
    {
        return;
    }

    while (len--)
    {
        ch = *text++;

        if ( (ch <= 0x20) || (ch & 0x80))
        {
            offset = 0;  // invalid char, use the default char(' ')
        }
        else
        {
            // 16 bytes per char.
            offset = (uint16) (ch - 0x20) * 16; //lint !e571
        }

        data_ptr = (uint8 *) &ascii_8X16[offset];

        // show this char
        for (i = 0; i < 16; i++)
        {
            LCD_Set8Pixels (pos_x, pos_y+i, *data_ptr++);
        }

        pos_x +=8;

        if ( (pos_x + 8) > width) // Next line.
        {
            pos_y += 16;
            pos_x  = 0;
        }

        if (pos_y > (height-16))
        {
            break;
        }
    }

    if ( (g_lcd_property_table[LCD_ID_0].lcd_spec.controller == LCD_CTRL_MODULE_LCDC)
            || (g_lcd_property_table[LCD_ID_0].lcd_spec.controller == LCD_CTRL_MODULE_LCDC_LCM0))
    {
        index = g_lcd_property_table[0].blk_group_num;
        g_block[index].block[0].is_enable       = 1;
        g_block[index].block[0].cfg.priority     = 7;
        g_block[index].block[0].cfg.start_x     = 0;
        g_block[index].block[0].cfg.start_y     = 0;
        g_block[index].block[0].cfg.width        = width;
        g_block[index].block[0].cfg.end_x       = width - 1;
        g_block[index].block[0].cfg.end_y       = height - 1;;
        g_block[index].block[0].cfg.resolution  = 2;
        g_block[index].block[0].cfg.type        = 1;
        g_block[index].block[0].cfg.colorkey    = 0;
        g_block[index].block[0].cfg.alpha       = 1;
        g_block[index].block[0].mem_ptr      = (uint32 *) mem_ptr;
        g_block[index].lcd_id = 0;
        g_block[index].block[0].cfg.colorkey_en = 0;
        g_block[index].block[0].cfg.alpha_sel   = 1;

        g_block[index].block[1].is_enable        = 0;
        g_block[index].block[2].is_enable        = 0;
        g_block[index].block[3].is_enable        = 0;
        g_block[index].block[4].is_enable        = 0;
        g_block[index].block[5].is_enable        = 0;

    }

    LCD_AssertInvalidateRect (LCD_ID_0, 0, y, width-1, pos_y+15);

}
PUBLIC uint32 LCD_GetUsedNum (void)
{
    return g_lcd_used_num;
}

/*******************************************************************************/
// Description: open or close fmark pin
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          lcd_id - specify LCD
//      output:
//          none
//      return
//          void
/********************************************************************************/
PUBLIC void LCD_SetIsOpenFmarkFunc (LCD_ID_E lcd_id,BOOLEAN is_open)
{

}

/*******************************************************************************/
// Description: Set Display data memory address
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          lcd_id - specify LCD
//          buf_ptr - the pointer to data
//      output:
//          none
//      return
//          none
/********************************************************************************/
PUBLIC void LCD_SetBuffer (LCD_ID_E lcd_id, uint32 *buf_ptr)
{

}//end of LCD_SetBuffer
/*******************************************************************************/
// Description:
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          lcd_id - specify LCD
//      output:
//          none
//      return
//          data buffer address
/********************************************************************************/
PUBLIC uint32 *LCD_GetBuffer (LCD_ID_E lcd_id)
{
    return 0;
}//end of LCD_GetBuffer

/******************************************************************************/
//  Description:  let the main lcd display on to display assert information
//  Global resource dependence:
//  Author: Jim.zhang
//  Note:
//      input:
//          none
//      output:
//          none
//      return
//          none
/******************************************************************************/
LOCAL void LCD_DisplayOn (void)
{
    // exit sleep mode
    LCD_EnterSleep (MAIN_LCD_ID, SCI_FALSE);
    // open back light.
    GPIO_SetLcdBackLight (SCI_TRUE);
}

/**********************************************************************************************************/
// Description:Read data from lcm
// Global resource dependence: NONE
// Author: Younger.yang
// Note:
//      this interface is read data from lcm
//      input:
//            index_value - index value
//                  lcd_id-MAIN_LCD_ID/SUB_LCD_ID
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC uint32 LCD_ReadRegVal (uint32 lcd_id, uint32 reg_addr)
{
    uint32 read_value = 0;

    LCDC_ReadLcdIndex (&read_value,reg_addr,lcd_id);

    return read_value;
}

PUBLIC void LCD_InvalidatePixel (
    LCD_ID_E lcd_id,
    uint16 x,
    uint16 y,
    uint16 color
)
{
    s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_InvalidateRect (x, y, x, y);
    LCD_SendData (color,lcd_id);
}

/**********************************************************************************************************/
// Description:init lcdc for read index value of lcd
// Global resource dependence: NONE
// Author: Jianping.Wang
// Note:
//      this interface is read data from lcm
//      input:
//            param_ptr - parameter pointer
//      return:
//          ERR_LCD_NONE,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC ERR_LCD_E LCD_SimpleInit (LCD_SIMPLE_INIT_PARAM_T *param_ptr)
{
    LCDC_TYPE_T                 lcdc_type = {0};
    LCDC_SPEC_T             mcu_lcd_spec = {0};
    LCD_TIMING_U        timing_para;
    LCD_TIMING_U timing= {0};
    uint32 cs_id = 0;
    uint32 ahb_clk_cycle = 0;
    uint32 ahb_clk = 0;
    ERR_LCD_E ret = ERR_LCD_NONE;

    ahb_clk = CHIP_GetAhbClk();

    LCDC_Reset();
    lcdc_type.controller = param_ptr->controller;
    lcdc_type.main_lcd_cd = param_ptr->cd_pol;
    lcdc_type.lcdc_mode = 1;
    lcdc_type.mcu_lcd_sum    = 1;
    cs_id = param_ptr->cs_id;
    timing = param_ptr->timing;

    switch (cs_id)
    {
        case 0:
            //ModeSel1:0: 8080;1: 6800
            //CS0 device RGB mode:2'b00£º16bit (565);2'b01£º18bit (666);2'b10: 8bit;2'b11: 9bit
            mcu_lcd_spec.mcu_lcd_spec.rgbmode.mBits.cs0_rgb_mode = param_ptr->lcd_bits;
            mcu_lcd_spec.mcu_lcd_spec.modesel.mBits.modesel0 = param_ptr->interface_type;
            break;
        case 1:
            mcu_lcd_spec.mcu_lcd_spec.rgbmode.mBits.cs1_rgb_mode = param_ptr->lcd_bits;
            mcu_lcd_spec.mcu_lcd_spec.modesel.mBits.modesel1 = param_ptr->interface_type;
            break;
        default:
            ret = ERR_LCD_PARAMETER_WRONG;
    }

    LCDC_SetLcdType (&lcdc_type, cs_id);
    LCDC_Init();

    LCDC_Configure (&mcu_lcd_spec);
    ahb_clk_cycle = 1000000000/ahb_clk;

    timing_para.mcu_mode_timing.rcss = ( (timing.mcu_mode_timing.rcss/ahb_clk_cycle + 1) <=3)  ? (timing.mcu_mode_timing.rcss/ahb_clk_cycle + 1) : 3;
    timing_para.mcu_mode_timing.rlpw = ( (timing.mcu_mode_timing.rlpw/ahb_clk_cycle + 1) <=15) ? (timing.mcu_mode_timing.rlpw/ahb_clk_cycle + 1) : 15;
    timing_para.mcu_mode_timing.rhpw = ( (timing.mcu_mode_timing.rhpw/ahb_clk_cycle + 1) <=15) ? (timing.mcu_mode_timing.rhpw/ahb_clk_cycle + 1) : 15;
    timing_para.mcu_mode_timing.wcss = ( (timing.mcu_mode_timing.wcss/ahb_clk_cycle + 1) <=3)  ? (timing.mcu_mode_timing.wcss/ahb_clk_cycle + 1) : 3;
    timing_para.mcu_mode_timing.wlpw = ( (timing.mcu_mode_timing.wlpw/ahb_clk_cycle + 1) <=15) ? (timing.mcu_mode_timing.wlpw/ahb_clk_cycle + 1) : 15;
    timing_para.mcu_mode_timing.whpw = ( (timing.mcu_mode_timing.whpw/ahb_clk_cycle + 1) <=15) ? (timing.mcu_mode_timing.whpw/ahb_clk_cycle + 1) : 15;

    LCDC_ChangePulseWidth (LCD_MCU, cs_id, timing_para);

    return ERR_LCD_NONE;

}
/**********************************************************************************************************/
// Description:register callback when invalidate done
// Author: Jianping.Wang
/**********************************************************************************************************/
PUBLIC void LCD_RegisterCallBack (lcd_invalidate_done_func_t func)
{
    s_lcd_register_callback = func;
}
/**********************************************************************************************************/
// Description:register callback when invalidate done
// Author: Jianping.Wang
/**********************************************************************************************************/
PUBLIC void LCD_UnRegisterCallBack (void)
{
    s_lcd_register_callback = NULL;
}
/*****************************************************************************/
//Description: get block information
//Global resource dependence:
//Author: Jianping.wang
//Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_GetBlockInfo(LCD_ID_E lcd_id,BLOCK_T *blk_info_ptr)
{
    SCI_SEMAPHORE_PTR sm = SCI_NULL;
	SYS_STATE_E       flag;
    BLOCK_T *ret_ptr = PNULL;
    uint32 i = 0;

	SCI_ASSERT(lcd_id < g_lcd_used_num);       
	      
	flag = LCD_GetRunState();	
	
    sm = g_lcd_property_table[lcd_id].semaphore;
	
	LCD_GetSemaphore(sm, flag);	    
    SCI_MEMCPY((void*)blk_info_ptr,(void*)&g_block[lcd_id].block[0],LCDC_BLOCK_NUM*sizeof(BLOCK_T));
	LCD_PutSemaphore(sm, flag);
	
	return ERR_LCD_NONE;
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
