/******************************************************************************
 ** File Name:     lcd.c                                                      *
 ** Description:                                                              *
 **    This file contains common driver for lcd modul                          *
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
#include "lcm_drv.h"
#include "cmddef.h"
#include "chip.h"
#include "lcm_prod.h"
#include "Task_monitor.h"
#include "gpio_prod_api.h"
#include "ref_outport.h"
#include "power.h"
#include "driver_export.h"
#include "dal_display.h"
#include "ref_lcd.h"

#ifdef PLATFORM_SC6600V
#include "lcd_tv.h"
#include "gpio_mtv.h"
extern void MTVBOOT_SetOffFlagForAssert (void);
#endif


/**---------------------------------------------------------------------------*
 **                         Compiler Flag
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define LCD_ROT             0
#define DMA_ROT             1

#ifdef PLATFORM_SC6600L
#define LCD_DIS_ROT_TYPE   DMA_ROT
#elif defined (PLATFORM_SC8800H)
#define LCD_DIS_ROT_TYPE   LCD_ROT
#endif


LOCAL uint16 main_lcd_width;        // width of main lcd.
LOCAL uint16 main_lcd_height;        // height of main lcd.


extern void *MEM_GetMainLCDBuffer (void);
extern void *MEM_GetSubLCDBuffer (void);

LOCAL void LCD_GetSemaphore (SCI_SEMAPHORE_PTR sm, SYS_STATE_E flag);
LOCAL void LCD_PutSemaphore (SCI_SEMAPHORE_PTR sm, SYS_STATE_E flag);
LOCAL void LCD_FreeSemaphore (void);
LOCAL ERR_LCD_E LCD_SetParam(uint32 ahb_clk);
LOCAL uint32 LCD_GetBusRatio (void);

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL   BOOLEAN             s_is_lcd_sleeping[LCD_SUPPORT_MAX] ;
LOCAL   uint32              s_reg_flag[LCD_SUPPORT_MAX] ;
LOCAL   uint32              s_lcd_used_num = 0;
LOCAL   BLOCK_T             s_buffer_info[LCD_SUPPORT_MAX];
LOCAL   LCD_SPEC_INFO_T_PTR s_lcd_spec_info_ptr[LCD_SUPPORT_MAX];
LOCAL   LCD_DIRECT_E        s_lcd_direct[LCD_SUPPORT_MAX] = {0};
LOCAL   LCD_BRUSH_MODE_E    s_lcd_brush_mode[LCD_SUPPORT_MAX] = {0};
LOCAL   LCD_STATUS_E s_lcd_status = LCD_STATUS_IDLE;

#ifdef PLATFORM_SC6600L
LOCAL   LCD_ROT_MODE_E      s_lcd_rot_mode = LCD_ROT_MODE_DMA;
#elif defined (PLATFORM_SC8800H)
LOCAL   LCD_ROT_MODE_E      s_lcd_rot_mode = LCD_ROT_MODE_LCD;
#endif

LOCAL SCI_SEMAPHORE_PTR    s_lcd_sm_ptr;

LOCAL BOOLEAN s_is_invalidrect_enable = SCI_TRUE;
/*****************************************************************************/
//  Description:  LCD Reg Info
//    Global resource dependence:
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/
LOCAL void LCD_RegInfo (LCD_ID_E lcd_id, LCD_SPEC_INFO_T_PTR lcd_spec_ptr)
{
    LCM_RegLCDInfo (lcd_spec_ptr);

    //support low device
    if (lcd_spec_ptr != s_lcd_spec_info_ptr[lcd_id])
    {
        s_lcd_spec_info_ptr[lcd_id] = lcd_spec_ptr;
    }

    s_reg_flag[lcd_id] = 1;
}

/*****************************************************************************/
//  Description:  disable/enable lcd invalidate rect
/*****************************************************************************/
PUBLIC void LCD_InvalidateRect_Enable (BOOLEAN is_enable)
{
    s_is_invalidrect_enable = is_enable;
}

/*****************************************************************************/
//  Description:  set rotation mode
/*****************************************************************************/
PUBLIC void LCD_SetRotateMode (LCD_ROT_MODE_E mode)
{
    s_lcd_rot_mode = mode;
}

/*****************************************************************************/
//  Description: get rotation mode
/*****************************************************************************/
LOCAL LCD_ROT_MODE_E _LCD_GetRotateMode (void)
{
    return s_lcd_rot_mode;
}

/*****************************************************************************/
//  Description:  init the lcd, if have sub lcd, init the sub lcd .
//                [younger]don't change the LCD init sequency
//    Global resource dependence:
//  Author: Jim.zhang & younger.yang
//    Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_Init (void)
{
    uint16      lcd_id;
    LCD_INFO_T  lcd_info = {0};
    BLOCKCFG_T cfg;

    if(s_lcd_used_num != 0)
    {
        return ERR_LCD_NONE;
    }
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        LCM_Init();
#ifdef PLATFORM_SC6600V

        if (LCDTV_IsLCDNeedReset())
#endif
        {

            if (GetWatchdogMode() != SCI_TRUE)
            {
                LCM_Init();
                s_lcd_sm_ptr = SCI_CreateSemaphore ("LCD OPERATION SEMAPHORE", 1);
            }

            LCM_RegisterCallBack (0, LCD_FreeSemaphore);

            //get lcd spec and reg the info
            for (lcd_id = 0; lcd_id < LCD_SUPPORT_MAX; lcd_id++)
            {
                s_lcd_spec_info_ptr[lcd_id] = LCM_GetLcdSpecInfo (lcd_id);

                if (PNULL != s_lcd_spec_info_ptr[lcd_id])
                {
                    SCI_ASSERT (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation);
                    LCD_RegInfo (lcd_id, s_lcd_spec_info_ptr[lcd_id]);
                    LCM_SetCSAddr (s_lcd_spec_info_ptr[lcd_id]->cs_pin, s_lcd_spec_info_ptr[lcd_id]->cd_pin, lcd_id);
                    s_lcd_used_num++;
                }
            }

            SCI_ASSERT (s_lcd_used_num <= LCD_SUPPORT_MAX);

            //Reset LCD
            LCD_Reset();

        }

        //LCM RGBMODE
        LCD_SetRGBMode (MAIN_LCD_ID);

        //Set LCM/Nand Bus Ratio
        LCM_SetBusRatio (LCD_GetBusRatio());

        //init LCM timing
        //LCD_SetParam(CHIP_GetAhbClk());
        LCD_SetParam (26000000);

        /* init lcd buffer */
#ifdef PLATFORM_SC6600V

        if (LCDTV_IsLCDNeedReset())
#endif
        {
            for (lcd_id=0; lcd_id<s_lcd_used_num; lcd_id++)
            {
                LCD_GetInfo (lcd_id,&lcd_info);
                cfg.colorkey   = 0;
                cfg.resolution = 0;
                cfg.type       = 0;
                cfg.priority   = 0;
                cfg.alpha      = 0;
                cfg.start_x = 0;
                cfg.start_y = 0;
                cfg.end_x = lcd_info.lcd_width -1;
                cfg.end_y = lcd_info.lcd_height -1 ;
                cfg.width =  cfg.end_x - cfg.start_x +1;
                LCD_ConfigBlock (lcd_id, 0, cfg);
                LCD_SetBlockBuffer (lcd_id, 0, lcd_info.lcdbuff_ptr);

            }

            //init all the lcd support

            for (lcd_id = 0; lcd_id < s_lcd_used_num; lcd_id++)
            {
                if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
                {
                    s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_Init();
                    s_is_lcd_sleeping[lcd_id] = SCI_FALSE;
                }
            }

            main_lcd_width= s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->width;
            main_lcd_height = s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->height;
        }

        return ERR_LCD_NONE;
    }

#ifdef PLATFORM_SC6600V
    else
    {
        return LCDTV_Init();
    }

#endif

}

PUBLIC ERR_LCD_E LCD_InitInAssertRestMode (void)
{
    uint16      lcd_id;
    LCD_INFO_T  lcd_info = {0};
    BLOCKCFG_T cfg;

    LCM_Init();

    s_lcd_sm_ptr = SCI_CreateSemaphore ("LCD OPERATION SEMAPHORE", 1);

    LCM_RegisterCallBack (0, LCD_FreeSemaphore);

    //get lcd spec and reg the info
    for (lcd_id = 0; lcd_id < LCD_SUPPORT_MAX; lcd_id++)
    {
        s_lcd_spec_info_ptr[lcd_id] = LCM_GetLcdSpecInfo (lcd_id);

        if (PNULL != s_lcd_spec_info_ptr[lcd_id])
        {
            SCI_ASSERT (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation);
            LCD_RegInfo (lcd_id, s_lcd_spec_info_ptr[lcd_id]);
            LCM_SetCSAddr (s_lcd_spec_info_ptr[lcd_id]->cs_pin, s_lcd_spec_info_ptr[lcd_id]->cd_pin, lcd_id);
            s_lcd_used_num++;
        }
    }

    SCI_ASSERT (s_lcd_used_num <= LCD_SUPPORT_MAX);

    //LCM RGBMODE
    LCD_SetRGBMode (MAIN_LCD_ID);
    //Set LCM/Nand Bus Ratio
    LCM_SetBusRatio (LCD_GetBusRatio());
    //init LCM timing
    LCD_SetParam (CHIP_GetAhbClk());
    /* init lcd buffer */

    for (lcd_id=0; lcd_id<s_lcd_used_num; lcd_id++)
    {
        LCD_GetInfo (lcd_id,&lcd_info);
        cfg.colorkey   = 0;
        cfg.resolution = 0;
        cfg.type       = 0;
        cfg.priority   = 0;
        cfg.alpha      = 0;
        cfg.start_x = 0;
        cfg.start_y = 0;
        cfg.end_x = lcd_info.lcd_width -1;
        cfg.end_y = lcd_info.lcd_height -1 ;
        cfg.width =  cfg.end_x - cfg.start_x +1;
        LCD_ConfigBlock (lcd_id, 0, cfg);
        LCD_SetBlockBuffer (lcd_id, 0, lcd_info.lcdbuff_ptr);

    }

    //init all the lcd support
    for (lcd_id = 0; lcd_id < s_lcd_used_num; lcd_id++)
    {
        if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
        {
            //s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_Init();
            s_is_lcd_sleeping[lcd_id] = SCI_FALSE;
        }
    }

    main_lcd_width= s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->width;
    main_lcd_height = s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->height;



    return ERR_LCD_NONE;
}



#define DEFAULT_RESET_TIME  0x10
#define DEFAULT_RESET_LEVEL 0x0

void LCD_Reset (void)
{


    if ( (PNULL == s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation->lcd_Rst) ||
            (PNULL == s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation))
    {

        LCM_ResetLcd (DEFAULT_RESET_TIME,DEFAULT_RESET_LEVEL);
    }
    else if (PNULL != s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation->lcd_Rst)
    {
        s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation->lcd_Rst();
    }

}

/*****************************************************************************/
//  Description:   Close the lcd.(include sub lcd.)
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_Close (void)
{
    uint16 lcd_id;
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        //close all lcd support
        for (lcd_id = 0; lcd_id < s_lcd_used_num; lcd_id++)
        {
            if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
            {
                s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_Close();
            }
        }
    }

#ifdef PLATFORM_SC6600V
    else
    {
        //SCI_TRACE_LOW:"LCD_Close() in tv on mode"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC6600L_364_112_2_18_1_45_14_103,(uint8*)"");
    }

#endif

}

/*****************************************************************************/
//  Description:   get the lcd information.
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_GetInfo (
    LCD_ID_E lcd_id,            //ID of lcd to operate
    LCD_INFO_T *lcd_info_ptr    //lcd information struct pointer
)
{
    SCI_ASSERT (lcd_id < s_lcd_used_num);

    lcd_info_ptr->r_bitmask     = 0xf800;
    lcd_info_ptr->g_bitmask     = 0x07e0;
    lcd_info_ptr->b_bitmask     = 0x001f;
    lcd_info_ptr->bits_per_pixel    = 16;
    lcd_info_ptr->contrast_min  = 0x00;
    lcd_info_ptr->contrast_max      = 63;
    lcd_info_ptr->contrast_defaut   = 0x0A;
    lcd_info_ptr->lcd_width     = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->width;
    lcd_info_ptr->lcd_height        = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->height;

    if (MAIN_LCD_ID == lcd_id)
    {
        lcd_info_ptr->lcdbuff_ptr   = (void *) MEM_GetMainLCDBuffer();
    }
    else if (SUB_LCD_ID == lcd_id)
    {
        lcd_info_ptr->lcdbuff_ptr   = (void *) MEM_GetSubLCDBuffer();

    }

    return ERR_LCD_NONE;

}

/*****************************************************************************/
//  Description:   invalidate a rectang with the internal lcd data buffer
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_InvalidateRect (
    LCD_ID_E lcd_id,//ID of lcd to operate
    uint16 left,     //left of the rectangle
    uint16 top,     //top of the rectangle
    uint16 right,     //right of the rectangle
    uint16 bottom    //bottom of the rectangle.
)
{
    uint16 *buf_ptr     = 0;
#ifdef PLATFORM_SC8800H
    uint32 pPixel       = 0;
    uint32 width        = 0;
    uint32 height       = 0;
#endif
    uint16 buf_width    = 0;
    uint16 buf_height   = 0;
    uint16  buf_startx  = 0;
    uint16  buf_starty  = 0;
    uint16  buf_endx    = 0;
    uint16  buf_endy        = 0;
    BLOCKCFG_T  config;
    LCD_DATA_SIZE_E data_width = DATA_SIZE_BYTE;
    LCD_BRUSH_MODE_E brush_mode;

    if (SCI_FALSE == s_is_invalidrect_enable)
    {
        return ERR_LCD_NONE;
    }

    SCI_PASSERT ( ( (left <= right) && (top <= bottom)),
                  ("LCD left=%d,right=%d,top=%d,bottom=%d,",left,right,top,bottom));

    SCI_ASSERT (lcd_id < s_lcd_used_num);

#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        LCD_GetSemaphore (s_lcd_sm_ptr, SYS_STATE_NORMAL);

        if (SCI_InThreadContext())
        {
            //s_lcd_brush_mode[lcd_id] = LCD_BRUSH_MODE_ASYNC;
        }
        else
        {
            s_lcd_brush_mode[lcd_id] = LCD_BRUSH_MODE_SYNC;
        }

        LCD_SetLCDStatus (LCD_STATUS_BUSY);
        brush_mode = s_lcd_brush_mode[lcd_id];
        LCD_GetConfigBlock (lcd_id , 0, &config);

        if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
        {
            switch (s_lcd_spec_info_ptr[lcd_id]->spec_ptr->bus_width)
            {
                case WIDTH_8:
                    data_width = DATA_SIZE_BYTE;
                    break;

                case WIDTH_9:
                case WIDTH_16:
                    data_width = DATA_SIZE_HALFWORD;
                    break;

                case WIDTH_18:
                    data_width = DATA_SIZE_WORD;
                    break;
                default:
                    break;

            }

            buf_startx      = config.start_x;
            buf_starty      = config.start_y;
            buf_endx        = config.end_x;
            buf_endy        = config.end_y;

            buf_width   = config.width;
            buf_height  = config.end_y - config.start_y;

            buf_ptr = (uint16 *) LCD_GetBlockBuffer (lcd_id , 0);

            left   = (left<=buf_startx)   ? buf_startx  : left;
            right  = (right<=buf_endx)  ? right   : buf_endx;
            top    = (top<=buf_starty)    ? buf_starty  : top;
            bottom = (bottom<=buf_endy) ? bottom  : buf_endy;

            if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_SetBrushDirection)
            {
                if (LCD_ROT_MODE_LCD == _LCD_GetRotateMode())
                {
                    s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_SetBrushDirection (s_lcd_direct[lcd_id]);
                }
                else
                {
                    s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_SetBrushDirection (LCD_DIRECT_NORMAL);
                }
            }

            s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_InvalidateRect (left, top, right, bottom) ;

            buf_ptr = buf_ptr + (top-buf_starty) * buf_width + (left-buf_startx);     //pointer buffer start address

#ifdef PLATFORM_SC6600L
            {
                LCD_DIRECT_E dir_type = LCD_DIRECT_NORMAL;

                if (LCD_ROT_MODE_DMA == _LCD_GetRotateMode())
                {
                    dir_type = s_lcd_direct[lcd_id];
                }

                LCM_16BitDataWrite (
                    dir_type,
                    (uint32) buf_ptr,
                    LCM_GetDataAddr (lcd_id),
                    (right - left + 1),
                    (bottom - top + 1),
                    buf_startx,
                    buf_width,
                    brush_mode
                );

                LCD_SetDirection (MAIN_LCD_ID, LCD_DIRECT_NORMAL);                              
            }
#elif defined (PLATFORM_SC8800H)
            s_lcd_brush_mode[lcd_id] = LCD_BRUSH_MODE_SYNC;
            brush_mode = s_lcd_brush_mode[lcd_id];

            if ( (left == buf_startx) && ( (right-left+1) == buf_width))
            {
                pPixel = buf_width * (bottom - top + 1);
                LCM_DMA_Write ( (uint32 *) buf_ptr,LCM_GetDataAddr (lcd_id), pPixel * 2, data_width,brush_mode);
            }
            else
            {
                width   =  right  - (left - 1);                     //calculate width
                height  =  bottom - (top  - 1);                     //calculate height

                do
                {
                    pPixel = width;

                    LCM_DMA_Write ( (uint32 *) buf_ptr,LCM_GetDataAddr (lcd_id), pPixel * 2, data_width,brush_mode);
                    buf_ptr += pPixel + buf_width - width;  //buffer point to next line
                    height--;                                       //next line

                }
                while (height > 0);
            }


#endif

            if (LCD_BRUSH_MODE_SYNC == brush_mode)
            {
                LCD_SetLCDStatus (LCD_STATUS_IDLE);
                LCD_PutSemaphore (s_lcd_sm_ptr, SYS_STATE_NORMAL);
            }

            return ERR_LCD_NONE;

        }
        else
        {
            return ERR_LCD_POINTER_NULL;
        }
    }

#ifdef PLATFORM_SC6600V
    else
    {
        return LCDTV_InvalidateRect (lcd_id,left,top,right,bottom);
    }

#endif

    return ERR_LCD_NONE;  /*lint !e527*/
}

/*****************************************************************************/
//  Description:   invalidate a pixel(called by mmi special effect)
//    Global resource dependence:
//  Author: bin.ji
//    Note:
/*****************************************************************************/
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

/*****************************************************************************/
//  Description:   invalidate the whole lcd.
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_Invalidate (
    LCD_ID_E lcd_id    //ID of lcd to operate
)
{

    uint16 *buf_ptr = 0;
    uint32 pPixel   = 0;
    LCD_DATA_SIZE_E data_width = DATA_SIZE_BYTE;


    SCI_ASSERT (lcd_id < s_lcd_used_num);
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        buf_ptr = (uint16 *) LCD_GetBlockBuffer (lcd_id , 0);

        if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
        {
            LCD_GetSemaphore (s_lcd_sm_ptr, SYS_STATE_NORMAL);

            switch (s_lcd_spec_info_ptr[lcd_id]->spec_ptr->bus_width)
            {
                case WIDTH_8:
                    data_width = DATA_SIZE_BYTE;
                    break;

                case WIDTH_9:
                case WIDTH_16:
                    data_width = DATA_SIZE_HALFWORD;
                    break;

                case WIDTH_18:
                    data_width = DATA_SIZE_WORD;
                    break;
                default:
                    break;

            }

            s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_Invalidate() ;

            pPixel = (s_lcd_spec_info_ptr[lcd_id]->spec_ptr->width) * (s_lcd_spec_info_ptr[lcd_id]->spec_ptr->height);
            LCM_16BitDataWrite (
                LCD_DIRECT_NORMAL,
                (uint32) buf_ptr,
                LCM_GetDataAddr (lcd_id),
                (s_lcd_spec_info_ptr[lcd_id]->spec_ptr->width),
                (s_lcd_spec_info_ptr[lcd_id]->spec_ptr->height),
                0,
                (s_lcd_spec_info_ptr[lcd_id]->spec_ptr->width),
                LCD_BRUSH_MODE_SYNC
            );
            LCD_PutSemaphore (s_lcd_sm_ptr, SYS_STATE_NORMAL);
            return ERR_LCD_NONE;
        }
        else
        {
            return ERR_LCD_POINTER_NULL;
        }
    }

#ifdef PLATFORM_SC6600V
    else
    {
        return LCDTV_Invalidate (lcd_id);
    }

#endif
}

/*****************************************************************************/
//  Description:   entern/exit sleep mode
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E  LCD_EnterSleep (
    LCD_ID_E lcd_id,    //ID of lcd to operate
    BOOLEAN is_sleep    //true:enter sleep mode; false:exit sleep mode
)
{
    ERR_LCD_E ret;
    SCI_ASSERT (lcd_id < s_lcd_used_num);
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
        {
            if (s_is_lcd_sleeping[lcd_id] != is_sleep)
            {
                s_is_lcd_sleeping[lcd_id] = is_sleep;

                LCD_GetSemaphore (s_lcd_sm_ptr, SYS_STATE_NORMAL);

                ret =  s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_EnterSleep (is_sleep) ;

                LCD_PutSemaphore (s_lcd_sm_ptr, SYS_STATE_NORMAL);
            }
            else
            {
                ret =  ERR_LCD_NONE;
            }

            return  ret;
        }
        else
        {
            return ERR_LCD_POINTER_NULL;
        }
    }

#ifdef PLATFORM_SC6600V
    else
    {
        return ERR_LCD_NONE;
    }

#endif
}


/*****************************************************************************/
//  Description:  set the contrast value
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetContrast (
    LCD_ID_E lcd_id,    //ID of lcd to operate.
    uint16  contrast    //contrast value to set.
)
{
    SCI_ASSERT (lcd_id < s_lcd_used_num);
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
        {
            return (s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_SetContrast (contrast));
        }
        else
        {
            return ERR_LCD_POINTER_NULL;
        }
    }

#ifdef PLATFORM_SC6600V
    else
    {
        return LCDTV_SetContrast (lcd_id,contrast);
    }

#endif
}

/*****************************************************************************/
//  Description:    Set the brightness of LCD.
//    Global resource dependence:
//  Author:         Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBrightness (
    LCD_ID_E lcd_id,    //ID of lcd to operate.
    uint16 brightness    //brightness value to set.
)
{
    SCI_ASSERT (lcd_id < s_lcd_used_num);
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
        {
            return (s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_SetBrightness (brightness));
        }
        else
        {
            return ERR_LCD_POINTER_NULL;
        }
    }

#ifdef PLATFORM_SC6600V
    else
    {
        return LCDTV_SetBrightness (lcd_id,brightness);
    }

#endif
}


/*****************************************************************************/
//  Description:    Enable lcd to partial display mode, so can save power.
//    Global resource dependence:
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//    Note:           If all input parameters are 0, exit partial display mode.
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

    SCI_ASSERT (lcd_id < s_lcd_used_num);
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
        {
            LCD_GetSemaphore (s_lcd_sm_ptr, SYS_STATE_NORMAL);

            ret = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_SetDisplayWindow (left, top, right, bottom);

            LCD_PutSemaphore (s_lcd_sm_ptr, SYS_STATE_NORMAL);
        }
        else
        {
            ret = ERR_LCD_POINTER_NULL;
        }

        return ret;
    }

#ifdef PLATFORM_SC6600V
    else
    {
        ret = LCDTV_SetDisplayWindow (lcd_id,left,top,right,bottom);
        return ret;
    }

#endif
}


/*****************************************************************************/
//  Description:  set start address of lcd
//    Global resource dependence:
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/

PUBLIC ERR_LCD_E  LCD_SetStartAddress (
    LCD_ID_E lcd_id,//ID of lcd to operate
    uint16 left,     //left of the display window
    uint16 top     //top of the display window
)
{
    return ERR_LCD_NONE;
}

/*****************************************************************************/
//  Description:  Set LCD invalidate direction
//  Input:
//      direct_type:
//  Return:
//      None.
//  Note: Application should correct invalidate direction right after current
//        image displayed
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E  LCD_SetDirection (
    LCD_ID_E        lcd_id,//ID of lcd to operate
    LCD_DIRECT_E    direct_type
)
{
    SCI_ASSERT (direct_type < LCD_DIRECT_MAX);
    SCI_ASSERT (lcd_id < LCD_SUPPORT_MAX);
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {

        s_lcd_direct[lcd_id] = direct_type;
#ifdef PLATFORM_SC8800H

        if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_SetBrushDirection)
        {
            if (LCD_ROT_MODE_LCD == _LCD_GetRotateMode())
            {
                LCD_GetSemaphore (s_lcd_sm_ptr, SYS_STATE_NORMAL);

                s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_SetBrushDirection (s_lcd_direct[lcd_id]);

                LCD_PutSemaphore (s_lcd_sm_ptr, SYS_STATE_NORMAL);
            }
        }

#endif
    }

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
LOCAL LCD_DIRECT_E  LCD_GetDirection (LCD_ID_E lcd_id)
{
    return s_lcd_direct[lcd_id];
}

/*****************************************************************************/
//  Description:  clear the whole lcd to one color.
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
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
#ifndef __FOTA_COMPILE__
    SCI_ASSERT (lcd_id < s_lcd_used_num);
#endif
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        LCD_GetInfo (lcd_id,&lcd_info);

        dest_ptr = lcd_info.lcdbuff_ptr;

        for (i = 0; i < lcd_info.lcd_height * lcd_info.lcd_width; i++)
        {
            *dest_ptr++ = color;
        }

        cfg.colorkey   = 0;
        cfg.resolution = 0;
        cfg.type       = 0;
        cfg.priority   = 0;
        cfg.alpha      = 0;
        cfg.start_x = 0;
        cfg.start_y = 0;
        cfg.end_x = lcd_info.lcd_width -1;
        cfg.end_y = lcd_info.lcd_height -1 ;
        cfg.width =  cfg.end_x - cfg.start_x +1;
        LCD_ConfigBlock (lcd_id, 0, cfg);
        LCD_SetBlockBuffer (lcd_id, 0, lcd_info.lcdbuff_ptr);

        LCD_Invalidate (lcd_id);
    }

#ifdef PLATFORM_SC6600V
    else
    {
        LCDTV_Clear (lcd_id,color);
    }

#endif
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


LOCAL uint16 font_color = 0x00;


/*****************************************************************************/
//  Description:  Set 8 pixels from(x,y) to(x+7,y).
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
LOCAL void LCD_Set8Pixels (uint16 cx, uint16 cy, uint8 bit_mask)
{
    uint16 *buffer_ptr = 0;
    uint16 i;
    LCD_INFO_T  lcd_info;
    BLOCKCFG_T cfg;

    LCD_GetInfo (MAIN_LCD_ID,&lcd_info);
    buffer_ptr = lcd_info.lcdbuff_ptr;

    for (i=0; i<8; i++)
    {
        if ( (bit_mask & (0x01<<i)) !=0x00)
        {
            * (buffer_ptr+cy*main_lcd_width+cx+i) = font_color;    //( cx+i, cy, color);
        }
    }

    cfg.colorkey   = 0;
    cfg.resolution = 0;
    cfg.type       = 0;
    cfg.priority   = 0;
    cfg.alpha      = 0;
    cfg.start_x = 0;
    cfg.start_y = 0;
    cfg.end_x = lcd_info.lcd_width -1;
    cfg.end_y = lcd_info.lcd_height -1 ;
    cfg.width =  cfg.end_x - cfg.start_x +1;
    LCD_ConfigBlock (MAIN_LCD_ID, 0, cfg);
    LCD_SetBlockBuffer (MAIN_LCD_ID, 0, lcd_info.lcdbuff_ptr);
}

/*****************************************************************************/
//  Description:  display the text(assert information) when assert.(just english)
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_DisplayText (uint16 x, uint16 y, char *text, uint16 len)
{
    uint16  pos_x = x, pos_y = y;
    uint16  offset, i;
    uint8  *data_ptr;
    uint8    ch;

    if ( (len==0) || (text == NULL))
    {
        return;
    }

#ifdef PLATFORM_SC6600V

    if (LCDTV_IsOpen())

    {
        MTVBOOT_SetOffFlagForAssert();
        LCDTV_SetLCDNeedReset (FALSE);

        GPIO_MTVLCDBypass (TRUE);

        {
            //extern void CHIP_RestoreARMClk(void);
            //uint32 freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
            //freq_req_handler =  CHNG_FREQ_GetReqHandler("mtv");
            //SCI_ASSERT(CHNG_FREQ_REQ_HANDLER_NULL != freq_req_handler);
            //CHNG_FREQ_RestoreARMClk(freq_req_handler);
            //CHNG_FREQ_DeleteReqHandler(freq_req_handler);
        }

        LCD_Init();
    }

#endif

    if (x > (main_lcd_width-8))
    {
        pos_x = 0;
        pos_y += 16;
    }

    if (pos_y > (main_lcd_height-16))
    {
        return;
    }

    LCD_SetDirection (MAIN_LCD_ID,LCD_DIRECT_NORMAL);

    LCD_Clear (MAIN_LCD_ID,0x0f0f);

    while (len--)
    {
        ch = (uint8) (*text++);

        if ( (ch <= 0x20) || (ch & 0x80))
        {
            offset = 0;  // invalid char, use the default char(' ')
        }
        else
        {
            // 16 bytes per char.
            offset = (uint16) (ch - 0x20) * 16;
        }

        data_ptr = (uint8 *) &ascii_8X16[offset];

        // show this char
        for (i = 0; i < 16; i++)
        {
            LCD_Set8Pixels (pos_x, pos_y+i, *data_ptr++);
        }

        pos_x +=8;

        if ( (pos_x + 8) > main_lcd_width) // Next line.
        {
            pos_y += 16;
            pos_x  = 0;
        }

        if (pos_y > (main_lcd_height-16))
        {
            break;
        }
    }


    // exit sleep mode
    LCD_EnterSleep (MAIN_LCD_ID, SCI_FALSE);

    LCD_InvalidateRect_Enable (SCI_TRUE);
    LCD_InvalidateRect(MAIN_LCD_ID, 0, y, main_lcd_width-1, pos_y+15);

    GPIO_SetLcdBackLight (SCI_TRUE);
}

PUBLIC ERR_LCD_E LCD_SimpleInit (LCD_SIMPLE_INIT_PARAM_T *param_ptr)
{
    if (param_ptr == PNULL)
    {
        return ERR_LCD_PARAMETER_WRONG;
    }

    LCM_ResetLcd (DEFAULT_RESET_TIME,DEFAULT_RESET_LEVEL);

    LCM_SetCSAddr (param_ptr->cs_id, param_ptr->cd_pol, param_ptr->lcd_id);

    return ERR_LCD_NONE;
}

/*****************************************************************************/
//  Description:  set dest LCD's RGB mode to host
//    Global resource dependence:
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/

PUBLIC void LCD_SetRGBMode (LCD_ID_E lcd_id)
{
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        LCM_SetRGBMode (s_lcd_spec_info_ptr[lcd_id]->spec_ptr->bus_width);
    }

#ifdef PLATFORM_SC6600V
    else
    {
        LCM_SetRGBMode (18); //666
    }

#endif

}


/*****************************************************************************/
//  Description:  set the number of pixes in the next frame from OSD
//    Global resource dependence:
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_SetDataNumber (LCD_ID_E lcd_id, uint32 number)
{
    LCD_GetSemaphore (s_lcd_sm_ptr, SYS_STATE_NORMAL);

    LCM_SetDataNumber (s_lcd_spec_info_ptr[lcd_id], number);

    LCD_PutSemaphore (s_lcd_sm_ptr, SYS_STATE_NORMAL);

}

/*****************************************************************************/
//  Description:  set the timing of LCM
//    Global resource dependence:
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/

LOCAL ERR_LCD_E LCD_SetParam (uint32 ahb_clk)
{

    uint32 lcd_id = 0;
    uint32 ret = 0;
#ifdef PLATFORM_SC6600V

    if (!LCDTV_IsOpen())
#endif
    {
        for (lcd_id = 0; lcd_id < LCD_SUPPORT_MAX; lcd_id++)
        {
            if (s_reg_flag[lcd_id])
            {
                ret |= LCM_SetParam (s_lcd_spec_info_ptr[lcd_id], ahb_clk);
            }
        }
    }

    return ret;
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
    LCM_SendCmdData (cmd, lcd_data, lcd_id);

    return ERR_LCD_NONE;
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
    LCM_SendCmd (cmd, lcd_id);

    return ERR_LCD_NONE;
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
    LCM_SendData (lcd_data, lcd_id);

    return ERR_LCD_NONE;

}//end of LCDC_SendData

/**********************************************************************************************************/
// Description:Read data from lcm
// Global resource dependence: NONE
// Author: Younger.yang
// Note:
//      this interface is read data from lcm
//      input:
//            cd_value - cd value
//                  lcd_id-MAIN_LCD_ID/SUB_LCD_ID
//      return:
//          LCD_OK,if success
//          other,if fail
/**********************************************************************************************************/
PUBLIC uint32 LCD_ReadRegVal (uint32 lcd_id, uint32 reg_addr)
{
    uint32 reg_val = 0;

    LCM_SendCmd (reg_addr, lcd_id);
    reg_val = LCM_Read (lcd_id, DATA_LEVEL);

    return reg_val;
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
    SCI_ASSERT (lcd_id < s_lcd_used_num);

    SCI_ASSERT ( (config.start_x< config.end_x) && (config.start_y< config.end_y));

    s_buffer_info[lcd_id].cfg.start_x    = config.start_x;
    s_buffer_info[lcd_id].cfg.start_y    = config.start_y;
    s_buffer_info[lcd_id].cfg.end_x      = config.end_x;
    s_buffer_info[lcd_id].cfg.end_y      = config.end_y;
    s_buffer_info[lcd_id].cfg.colorkey   = config.colorkey;
    s_buffer_info[lcd_id].cfg.width      = config.width;
    s_buffer_info[lcd_id].cfg.resolution = config.resolution;
    s_buffer_info[lcd_id].cfg.type       = config.type;
    s_buffer_info[lcd_id].cfg.priority   = config.priority;
    s_buffer_info[lcd_id].cfg.alpha      = config.alpha;

    return ERR_LCD_NONE;
}

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
    SCI_ASSERT (lcd_id < s_lcd_used_num);

    s_buffer_info[lcd_id].mem_ptr = buf_ptr;

    return ERR_LCD_NONE;

}
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
    return ERR_LCD_FUNC_NOT_SUPPORT;
}

/**********************************************************************************************************/
// Description: set LCD background color
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/

PUBLIC void LCD_SetBackground (uint32 color)
{
    return;
}

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
    return ERR_LCD_FUNC_NOT_SUPPORT;
}

PUBLIC LCD_STATE_E LCD_GetLCDState (LCD_ID_E lcd_id)
{
    return ERR_LCD_FUNC_NOT_SUPPORT;
}

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
    uint32 *buf_ptr = PNULL;

    SCI_ASSERT (lcd_id < s_lcd_used_num);

    buf_ptr = s_buffer_info[lcd_id].mem_ptr;

    return buf_ptr;
}
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
    SCI_MEMCPY (cfg_ptr ,& (s_buffer_info [lcd_id].cfg) , sizeof (BLOCKCFG_T));

    return ERR_LCD_NONE;
}

PUBLIC uint32 LCD_GetUsedNum (void)
{
    return s_lcd_used_num;
}

/*****************************************************************************/
//  Description:    get the main lcd 's important parameter for Digital cammera
//    Global resource dependence:
//  Author:         Zhemin.lin
//    Note:
/*****************************************************************************/


PUBLIC void LCD_PointerSaveInAssertResetMode (void)
{
    uint32 i = 0;

    for (i = 0; i < s_lcd_used_num; i++)
    {
        LCD_EnterSleep (i, SCI_FALSE);
    }

    LCD_SetDirection (MAIN_LCD_ID,LCD_DIRECT_NORMAL);
}

/*******************************************************************************/
// Description:
// Global resource dependence:
// Author: Younger.yang
// Note:
/********************************************************************************/
LOCAL uint32 LCD_GetBusRatio (void)
{
    uint32 lcd_busratio = LCD_BUSRATIO;

    return lcd_busratio;
}

/*****************************************************************************/
//  Description:    get the main lcd 's important parameter for Digital cammera
//  Global resource dependence:
//  Author:         Zhemin.lin
//  Note:
/*****************************************************************************/
PUBLIC LCD_SPEC_INFO_T_PTR LCD_GetLcdSpec (
    LCD_ID_E lcd_id  //id of lcd to operate.
)
{
    return s_lcd_spec_info_ptr[lcd_id];
}

/*****************************************************************************/
//  Description:    LCD delay
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
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
        if (SCI_ERROR == SCI_GetSemaphore (sm, SCI_WAIT_FOREVER))
        {
            //SCI_TRACE_LOW:"LCD:Get Semaphore Fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC6600L_1615_112_2_18_1_45_17_104,(uint8*)"");
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
        ret = SCI_PutSemaphore (sm);
        SCI_ASSERT (ret == SCI_SUCCESS);
    }

    if (flag == SYS_STATE_INT)
    {
        ret = SCI_PutSemaphore (sm);
        SCI_ASSERT (ret == SCI_SUCCESS);
    }
}//end of LCD_PutSemaphore

/*****************************************************************************/
//  Description: free semaphore when use LCDC by asyn fashion
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
LOCAL void LCD_FreeSemaphore (void)
{
    SCI_ASSERT (PNULL != s_lcd_sm_ptr);

    //LCD_SetDirection(0, LCD_DIRECT_NORMAL);
    LCD_PutSemaphore (s_lcd_sm_ptr, SYS_STATE_INT);
    LCD_SetLCDStatus (LCD_STATUS_IDLE);

}//end of LCD_FreeSemaphore
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
    //#ifdef LCD_ASYNC_SUPPORT
    SCI_ASSERT (lcd_id < s_lcd_used_num);

    //SCI_TRACE_LOW:"LCD_SetBrushMode Enter!mode:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_SC6600L_1678_112_2_18_1_45_17_105,(uint8*)"d",mode);

    LCD_GetSemaphore (s_lcd_sm_ptr, SYS_STATE_NORMAL);

    if (s_lcd_brush_mode[lcd_id] != mode)
    {
        s_lcd_brush_mode[lcd_id] = mode;
    }

    TM_SendTestPointRequest (0x080950ff , mode);

    LCD_PutSemaphore (s_lcd_sm_ptr, SYS_STATE_NORMAL);
    //#endif



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
    SCI_PASSERT ( (lcd_id < s_lcd_used_num), ("LCD_GetBrushMode,lcd_id = %d", lcd_id));

    return s_lcd_brush_mode[lcd_id];
}

/*****************************************************************************/
//  Description: set lcd status
//  Global resource dependence: s_lcd_status
//  Author: Younger yang
//  Note:
/*****************************************************************************/

PUBLIC void LCD_SetLCDStatus (LCD_STATUS_E lcd_status)
{
    SCI_ASSERT ( (lcd_status == LCD_STATUS_BUSY) || (lcd_status == LCD_STATUS_IDLE));

    s_lcd_status = lcd_status;
}

/*****************************************************************************/
//  Description: get lcd status
//  Global resource dependence: s_lcd_status
//  Author: Younger yang
//  Note:
/*****************************************************************************/
PUBLIC LCD_STATUS_E LCD_GetLCDStatus (void)
{
    return s_lcd_status;
}
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
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

/* end of lcd.c*/

