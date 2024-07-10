/****************************************************************************
** File Name:      mmidle_sprite.c                                                *
** Author:                                                                 *
** Date:           06/30/2008                                              *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/30/2008       bin.ji        Create
** 
****************************************************************************/
#define _MMIIDLE_SPRITE_C_  


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_screensaver_trc.h"
#include "mmk_app.h"
#include "mmi_image.h"
#include "block_mem_def.h"
#include "mmi_id.h"
#include "mmiss_sprite.h"
#include "mmidisplay_data.h"
#include "block_mem.h"
#include "guiblock.h"
#ifdef UI_P3D_SUPPORT
#include "mmi3d_export.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

#define DIV8(color)     ((color&0xc718)>>3)
#define DIV4(color)     ((color&0xe79c)>>2)
#define DIV2(color)     ((color&0xf7de)>>1)
#define DIV2_16(color)     ((color&0xf7de)>>1)
#define DIV2_32(color)     ((color&0xf7def7de)>>1)

#define K1(color)       DIV8(color)
#define K2(color)       DIV4(color)
#define K3(color)       (DIV8(color)+DIV4(color))
#define K4(color)       DIV2(color)
#define K5(color)       (DIV8(color)+DIV2(color))
#define K6(color)       (DIV2(color)+DIV4(color))
#define K7(color)       (color-DIV8(color))

//计算color1和color2的混合结果
//#define ALPHA_BLEND(color1, color2)  (K2(color1)+K6(color2))
#define ALPHA_BLEND(color1, color2)  (K4(color1)+K4(color2))
#define ALPHA_BLEND16(color1, color2)  (DIV2_16(color1)+DIV2_16(color2))
#define ALPHA_BLEND32(color1, color2)  (DIV2_32(color1)+DIV2_32(color2))

//判断(x,y)点是否在rect区域内
#define IsInRect(x, y, rect) ((x>=rect.left&&x<=rect.right)&&(y>=rect.top&&y<=rect.bottom))

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#ifdef UI_MULTILAYER_TEST
static UILAYER_HANDLE_T s_test_layer_handle = PNULL;
static int16 s_offset_x = 50;
static int16 s_offset_y = 50;
#endif
//保存sprite信息的全局结构
MMIIDLE_SPRITE_GROUP_T s_sprite_group = {0};

//记住最后一次tp_move事件的y位置，（为了持续移动sprite用）
int16 s_last_pressed_move_y = 0;

//记住tp_move事件的次数，（为了来不及响应时丢弃部分消息用）
int16 s_press_move_count = 0;

//记住tp_down时点击的sprite类型，（为了tp_move的时候用）
MMIIDLE_SPRITE_E s_pressed_sprite = MMIIDLE_SPRITE_NONE;
/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 检查当前位置移动后是否还在屏幕范围内
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsInLimit(
                        MMIIDLE_SPRITE_E sprite_type,
                        int32 move_offset
                        );

/*****************************************************************************/
// 	Description : 更新某一幅sprite
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void UpdateOneSprite(
                           MMIIDLE_SPRITE_E sprite_type
                           );

/*****************************************************************************/
// 	Description : 更新所有sprite
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void ShowAllSprite(void);

/*****************************************************************************/
// 	Description : 恢复背景
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void RestorePartBG(
                         MMIIDLE_SPRITE_E sprite_type,
                         GUI_COLOR_T *restore_bg_ptr
                         );

/*****************************************************************************/
// 	Description : 保存背景
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void SavePartBG(
                      MMIIDLE_SPRITE_E sprite_type,
                      GUI_COLOR_T *restore_bg_ptr
                      );

/*****************************************************************************/
// 	Description : 释放buf，退出待机界面的时候调用
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void FreeSpriteBuf(void);

/*****************************************************************************/
// 	Description : 分配buf，进入待机界面的时候调用
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void  AllocSpriteBuf(void);

/*****************************************************************************/
// 	Description : 判断触笔是否点击在sprite上
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSpritePressed(
                        MMIIDLE_SPRITE_E sprite_id,
                        int16 x,
                        int16 y
                        );

/*****************************************************************************/
// 	Description : 半透显示Icon，叠加在lcd背景上
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DisplayHalfIcon(
                     GUI_COLOR_T *icon_buff_ptr,
                     GUI_RECT_T rect
                     );

/*****************************************************************************/
// 	Description : 计算触笔点击在sprite上的相对位置
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CalcOutXY(
                  MMIIDLE_SPRITE_E sprite_id,
                  int16 x,
                  int16 y,
                  int16 *out_x_ptr,
                  int16 *out_y_ptr
                  );

/*****************************************************************************/
// 	Description : 计算sprite在lcd上（左右居中）的显示位置
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void GetSpriteRect(
                    MMIIDLE_SPRITE_E sprite_id,
                    GUI_RECT_T *rect_ptr
                    );

/*****************************************************************************/
// 	Description : 计算sprite在lcd上（左右居中）的显示位置
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void GetSpriteOldRect(
                    MMIIDLE_SPRITE_E sprite_id,
                    GUI_RECT_T *rect_ptr
                    );

#ifdef UI_MULTILAYER_TEST
/*****************************************************************************/
//  Description : draw layer test
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DrawLayerTest(void);

/*****************************************************************************/
//  Description : create layer test
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void CreateLayerTest(
                           int16 offset_x,
                           int16 offset_y
                           );

/*****************************************************************************/
//  Description : handle layer test message
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLayerTestMsg(
                         MMI_WIN_ID_T         win_id,
                         MMI_MESSAGE_ID_E     msg_id,
                         DPARAM               param
                         );
#endif
/**---------------------------------------------------------------------------*
 **                         Difination                                        *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 初始化sprite，开机的时候调用
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void  MMIIDLE_InitSprite(void)
{
    s_sprite_group.unit[MMIIDLE_BOTTOM_SPRITE].icon_id      = IMAGE_ADJUST_VOL_BLOCK_BG;//IMAGE_BT_HANFREE_NO_PAIRE;
    s_sprite_group.unit[MMIIDLE_BOTTOM_SPRITE].offset_y     = DEFAULT_BOTTOM_SPRITE_OFFSET_Y;
    s_sprite_group.unit[MMIIDLE_BOTTOM_SPRITE].old_offset_y     = DEFAULT_BOTTOM_SPRITE_OFFSET_Y;
    s_sprite_group.unit[MMIIDLE_BOTTOM_SPRITE].buf_ptr      = PNULL;
    s_sprite_group.unit[MMIIDLE_BOTTOM_SPRITE].restore_bg_ptr   = PNULL;
    s_sprite_group.unit[MMIIDLE_BOTTOM_SPRITE].buf_width        = BOTTOM_SPRITE_WIDTH;
    s_sprite_group.unit[MMIIDLE_BOTTOM_SPRITE].buf_height       = BOTTOM_SPRITE_HEIGHT;

    s_sprite_group.unit[MMIIDLE_TOP_SPRITE].icon_id      = IMAGE_ADJUST_VOL_BG;//IMAGE_BT_HANFREE_PAIRE;
    s_sprite_group.unit[MMIIDLE_TOP_SPRITE].offset_y        = DEFAULT_TOP_SPRITE_OFFSET_Y;
    s_sprite_group.unit[MMIIDLE_TOP_SPRITE].old_offset_y        = DEFAULT_TOP_SPRITE_OFFSET_Y;
    s_sprite_group.unit[MMIIDLE_TOP_SPRITE].buf_ptr         = PNULL;
    s_sprite_group.unit[MMIIDLE_BOTTOM_SPRITE].restore_bg_ptr   = PNULL;
    s_sprite_group.unit[MMIIDLE_TOP_SPRITE].buf_width           = TOP_SPRITE_WIDTH;
    s_sprite_group.unit[MMIIDLE_TOP_SPRITE].buf_height          = TOP_SPRITE_HEIGHT;
}

/*****************************************************************************/
// 	Description : 分配buf，进入待机界面的时候调用
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void  AllocSpriteBuf(void)
{
    GUI_COLOR_T *icon_buff_ptr = PNULL;
    uint32 total_size = 0;
    uint32 icon_size = 0;
    int32 i = 0;

    icon_buff_ptr = MMI_BL_MALLOC(BLOCK_MEM_POOL_MMI_SPRITE);/*lint !e64*/
    for (i = MMIIDLE_BOTTOM_SPRITE; i < MMIIDLE_SPRITE_MAX; i++)
    {
        s_sprite_group.unit[i].buf_ptr = icon_buff_ptr;
        icon_size = (s_sprite_group.unit[i].buf_width * s_sprite_group.unit[i].buf_height);
        s_sprite_group.unit[i].buf_size = icon_size*2;
        icon_buff_ptr += icon_size;
        total_size += icon_size;

        s_sprite_group.unit[i].restore_bg_ptr = icon_buff_ptr;
        icon_buff_ptr += icon_size;
        total_size += icon_size;
    }    
    //SCI_ASSERT(total_size <= BL_GetSize(BLOCK_MEM_POOL_MMI_SPRITE));/*lint !e64*//*assert to do*/
    if(total_size > BL_GetSize(BLOCK_MEM_POOL_MMI_SPRITE))
    {
        MMI_BL_FREE(BLOCK_MEM_POOL_MMI_SPRITE);
        return;
    }
    
    for (i = MMIIDLE_BOTTOM_SPRITE; i < MMIIDLE_SPRITE_MAX; i++)
    {
        MMIAPI3D_DecodeIconBuf(
            s_sprite_group.unit[i].icon_id,
            MAIN_IDLE_WIN_ID,
            s_sprite_group.unit[i].buf_ptr,
            s_sprite_group.unit[i].buf_width,
            s_sprite_group.unit[i].buf_height,
            0,
            0,
            PNULL
            );
    }
}

/*****************************************************************************/
// 	Description : 释放buf，退出待机界面的时候调用
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void FreeSpriteBuf(void)
{
    MMI_BL_FREE(BLOCK_MEM_POOL_MMI_SPRITE);/*lint !e64*/
}

/*****************************************************************************/
// 	Description : 显示sprite
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_ShowSprite(void)
{
//    GUI_RECT_T rect = {0};
    int32 i = 0;

    AllocSpriteBuf();

    for (i = MMIIDLE_BOTTOM_SPRITE; i < MMIIDLE_SPRITE_MAX; i++)
    {
        SavePartBG(i, s_sprite_group.unit[i].restore_bg_ptr);/*lint !e64*/
    }    

    ShowAllSprite();

    FreeSpriteBuf();
}

/*****************************************************************************/
// 	Description : 更新所有sprite
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void ShowAllSprite(void)
{
    int32 i = 0;
    GUI_RECT_T rect = {0};
    
    for (i = MMIIDLE_BOTTOM_SPRITE; i < MMIIDLE_SPRITE_MAX; i++)
    {
        GetSpriteRect(i, &rect);/*lint !e64*/
        DisplayHalfIcon(s_sprite_group.unit[i].buf_ptr, rect);
    }
}

/*****************************************************************************/
// 	Description : 更新点中sprite
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_UpdatePressedSprite(void)
{
    GUI_RECT_T rect = {0};
    GUI_RECT_T old_rect = {0};
    GUI_RECT_T total_rect = {0};

    GetSpriteRect(s_pressed_sprite, &rect);
    GetSpriteOldRect(s_pressed_sprite, &old_rect);
    total_rect.left = rect.left;
    total_rect.top = MIN(old_rect.top, rect.top);
    total_rect.right = rect.right;
    total_rect.bottom = MAX(old_rect.bottom, rect.bottom);

    UpdateOneSprite(s_pressed_sprite);

    GUILCD_Invalidate(0,0);
}

/*****************************************************************************/
// 	Description : 更新某一幅sprite
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void UpdateOneSprite(
                           MMIIDLE_SPRITE_E sprite_type
                           )
{
//    GUI_RECT_T rect = {0};
    int32 i = 0;
    //uint32 tick = 0;

    AllocSpriteBuf();

    for (i = MMIIDLE_BOTTOM_SPRITE; i < MMIIDLE_SPRITE_MAX; i++)
    {
        RestorePartBG(i, s_sprite_group.unit[i].restore_bg_ptr);/*lint !e64*/
    }
    SavePartBG(sprite_type, s_sprite_group.unit[sprite_type].restore_bg_ptr);

    //tick = SCI_GetTickCount(); 
    ShowAllSprite();
/*
    for (i = MMIIDLE_BOTTOM_SPRITE; i < MMIIDLE_SPRITE_MAX; i++)
    {
        GetSpriteRect(i, &rect);
        DisplayHalfIcon(s_sprite_group.unit[i].buf_ptr, rect);
    }
    */
    //SCI_TRACE_LOW("sprite ShowAllSprite tick= %d", SCI_GetTickCount() - tick);
    //tick = SCI_GetTickCount(); 

    FreeSpriteBuf();
}

/*****************************************************************************/
// 	Description : 半透显示Icon，叠加在lcd背景上
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DisplayHalfIcon(
                     GUI_COLOR_T *icon_buff_ptr,
                     GUI_RECT_T rect
                     )
{
    int16 iX = 0;
    int16 iY = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    GUI_COLOR_T *lcd_buff_ptr = PNULL;
    GUI_COLOR_T *temp_lcd_buff_ptr = PNULL;
    GUI_COLOR_T *temp_icon_buff_ptr = PNULL;
    uint32 *temp32_lcd_buff_ptr = PNULL;
    uint32 *temp32_icon_buff_ptr = PNULL;
//    GUI_COLOR_T color = 0;
//    GUI_COLOR_T icon_color = 0;
    uint32 temp32_color = 0;
    uint32 temp32_icon_color = 0;
    uint32 icon_width = rect.right+1-rect.left;

    lcd_buff_ptr = (GUI_COLOR_T*)GUIBLOCK_GetBuf(&lcd_dev_info);

    temp_icon_buff_ptr = icon_buff_ptr;
    temp_lcd_buff_ptr = lcd_buff_ptr + rect.top * MMI_MAINSCREEN_WIDTH + rect.left;
    //top rect
#if 0
    for (iY = rect.top; iY <= rect.bottom; iY++)
    {
        for (iX = rect.left; iX <= rect.right; iX++)
        {
            icon_color = *temp_icon_buff_ptr;
            if (0x00 != icon_color)  //不为透明色
            {
                color = *temp_lcd_buff_ptr;
                *temp_lcd_buff_ptr = ALPHA_BLEND(color, icon_color);
            }
            temp_icon_buff_ptr++;
            temp_lcd_buff_ptr++;
        }
        temp_icon_buff_ptr += (icon_width + rect.left - rect.right - 1);
        temp_lcd_buff_ptr += (MMI_MAINSCREEN_WIDTH + rect.left - rect.right - 1);
    }
#else   //优化算法
    for (iY = rect.top; iY <= rect.bottom; iY++)
    {
        temp32_lcd_buff_ptr = (uint32*)temp_lcd_buff_ptr;
        temp32_icon_buff_ptr = (uint32*)temp_icon_buff_ptr;
        for (iX = rect.left; iX <= rect.right; iX+=2)
        {
            temp32_icon_color = *temp32_icon_buff_ptr;
            
            if ((temp32_icon_color>>16))   //高16位不为0
            {
                if ((temp32_icon_color<<16))  //低16位不为0
                {
                    temp32_color = *temp32_lcd_buff_ptr;
                    *temp32_lcd_buff_ptr = ALPHA_BLEND32(temp32_color, temp32_icon_color);
                }
                else
                {
                    temp32_color = *temp32_lcd_buff_ptr;
                    *temp32_lcd_buff_ptr = (temp32_color & 0xffff) | (ALPHA_BLEND32(temp32_color, temp32_icon_color) & 0xffff0000);
                }
            }
            else if ((temp32_icon_color<<16))  //低16位不为0
            {
                temp32_color = *temp32_lcd_buff_ptr;
                *temp32_lcd_buff_ptr = (temp32_color & 0xffff0000) | (ALPHA_BLEND32(temp32_color, temp32_icon_color) & 0xffff);
            }

            temp32_icon_buff_ptr++;
            temp32_lcd_buff_ptr++;
        }
        temp_icon_buff_ptr = (GUI_COLOR_T*)temp32_icon_buff_ptr;
        temp_lcd_buff_ptr = (GUI_COLOR_T*)temp32_lcd_buff_ptr;
        temp_icon_buff_ptr += ((int16)icon_width + rect.left - rect.right - 1);
        temp_lcd_buff_ptr += (MMI_MAINSCREEN_WIDTH + rect.left - rect.right - 1);
    }
#endif
}

/*****************************************************************************/
// 	Description : 处理触笔Down消息
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_PressDownSprite(
                     int16 x,
                     int16 y
                     )
{
    BOOLEAN result = FALSE;

    s_press_move_count = 0;
    s_last_pressed_move_y = y;

    if (IsSpritePressed(MMIIDLE_TOP_SPRITE, x, y))
    {
        s_pressed_sprite = MMIIDLE_TOP_SPRITE;
        result = TRUE;
    }
    else if (IsSpritePressed(MMIIDLE_BOTTOM_SPRITE, x, y))
    {
        s_pressed_sprite = MMIIDLE_BOTTOM_SPRITE;
        result = TRUE;
    }
    else
    {
        s_pressed_sprite = MMIIDLE_SPRITE_NONE;
        result = FALSE;
    }
    return result;
}

/*****************************************************************************/
// 	Description : 处理触笔Move消息
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_PressMoveSprite(
                     int16 x,
                     int16 y
                     )
{
    if (MMIIDLE_SPRITE_NONE == s_pressed_sprite)
    {
        //SCI_TRACE_LOW:"MMIIDLE_PressMoveSprite ERROR! MMIIDLE_SPRITE_NONE == s_pressed_sprite"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SPRITE_507_112_2_18_2_49_51_11,(uint8*)"");
        return FALSE;
    }
    else if (MMIIDLE_SPRITE_MAX <= s_pressed_sprite)
    {
        //SCI_ASSERT(0);
        //SCI_TRACE_LOW:"MMIIDLE_PressMoveSprite ERROR! s_pressed_sprite =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SPRITE_513_112_2_18_2_49_51_12,(uint8*)"d",s_pressed_sprite);
        return FALSE;
    }
    else
    {
        s_press_move_count++;
        if (s_press_move_count > 2)
        {
            s_press_move_count = 0;
            if (IsInLimit(s_pressed_sprite, (y-s_last_pressed_move_y)))
            {
                (int16)s_sprite_group.unit[s_pressed_sprite].offset_y += (y-s_last_pressed_move_y);
                s_last_pressed_move_y = y;
                return TRUE;
            }
        }
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : 处理触Up消息
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_PressUpSprite(
                   int16 x,
                   int16 y
                   )
{
    if (MMIIDLE_SPRITE_NONE != s_pressed_sprite)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : 判断触笔是否点击在sprite上
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSpritePressed(
                        MMIIDLE_SPRITE_E sprite_id,
                        int16 x,
                        int16 y
                        )
{
    int16 out_x = 0;
    int16 out_y = 0;
    BOOLEAN result = FALSE;

    result = CalcOutXY(sprite_id, x, y, &out_x, &out_y);
    
    return result;
}

/*****************************************************************************/
// 	Description : 计算触笔点击在sprite上的相对位置
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CalcOutXY(
                  MMIIDLE_SPRITE_E sprite_id,
                  int16 x,
                  int16 y,
                  int16 *out_x_ptr,
                  int16 *out_y_ptr
                  )
{
    GUI_RECT_T rect = {0};
    GUI_COLOR_T *icon_buff_ptr = PNULL;
    int16 out_x = 0;
    int16 out_y = 0;
    uint32 icon_buf_width = 0;

    GetSpriteRect(sprite_id, &rect);

    if (!IsInRect(x, y, rect))
    {
        return FALSE;
    }
    out_x = x - rect.left;
    out_y = y - rect.top;

    icon_buff_ptr = s_sprite_group.unit[sprite_id].buf_ptr;
    icon_buf_width = s_sprite_group.unit[sprite_id].buf_width;
    if (0x00 != *(icon_buff_ptr+out_y*(int16)icon_buf_width+out_x))    //不为透明色
    {
        *out_x_ptr = out_x;
        *out_y_ptr = out_y;
        return TRUE;
    }
    return FALSE;    
}

/*****************************************************************************/
// 	Description : 计算sprite在lcd上（左右居中）的显示位置
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void GetSpriteRect(
                    MMIIDLE_SPRITE_E sprite_id,
                    GUI_RECT_T *rect_ptr
                    )
{
    //SCI_ASSERT(rect_ptr);
    if(PNULL == rect_ptr)
    {
        //SCI_TRACE_LOW:"GetSpriteRect ERROR! PNULL == rect_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SPRITE_618_112_2_18_2_49_51_13,(uint8*)"");
        return;
    }
    rect_ptr->left = (MMI_MAINSCREEN_WIDTH - s_sprite_group.unit[sprite_id].buf_width)/2;
    rect_ptr->right = rect_ptr->left + (int16)s_sprite_group.unit[sprite_id].buf_width - 1;
    rect_ptr->top = s_sprite_group.unit[sprite_id].offset_y;
    rect_ptr->bottom = rect_ptr->top + (int16)s_sprite_group.unit[sprite_id].buf_height - 1;
}

/*****************************************************************************/
// 	Description : 计算sprite在lcd上（左右居中）的显示位置
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void GetSpriteOldRect(
                    MMIIDLE_SPRITE_E sprite_id,
                    GUI_RECT_T *rect_ptr
                    )
{
    //SCI_ASSERT(rect_ptr);
    if(PNULL == rect_ptr)
    {
        //SCI_TRACE_LOW:"GetSpriteOldRect ERROR! PNULL == rect_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISS_SPRITE_639_112_2_18_2_49_51_14,(uint8*)"");
        return;
    }
    rect_ptr->left = (MMI_MAINSCREEN_WIDTH - s_sprite_group.unit[sprite_id].buf_width)/2;
    rect_ptr->right = rect_ptr->left + (int16)s_sprite_group.unit[sprite_id].buf_width - 1;
    rect_ptr->top = s_sprite_group.unit[sprite_id].old_offset_y;
    rect_ptr->bottom = rect_ptr->top + (int16)s_sprite_group.unit[sprite_id].buf_height - 1;
}

/*****************************************************************************/
// 	Description : 恢复背景
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void RestorePartBG(
                         MMIIDLE_SPRITE_E sprite_type,
                         GUI_COLOR_T *restore_bg_ptr
                         )
{
    GUI_RECT_T rect = {0};
    
    GetSpriteOldRect(sprite_type, &rect);

    LCD_CopyMemToMainLcdBufNoKeyColor(
        restore_bg_ptr,
        rect.right + 1 - rect.left, 
        rect.bottom + 1 - rect.top,
        rect.left,
        rect.top,
        FALSE
        );
}

/*****************************************************************************/
// 	Description : 保存背景
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void SavePartBG(
                      MMIIDLE_SPRITE_E sprite_type,
                      GUI_COLOR_T *restore_bg_ptr
                      )
{
    GUI_RECT_T rect = {0};
    
    GetSpriteRect(sprite_type, &rect);

    s_sprite_group.unit[s_pressed_sprite].old_offset_y = s_sprite_group.unit[s_pressed_sprite].offset_y;            
    
    LCD_CopyMainLcdBufToMem(
        restore_bg_ptr,
        rect.right + 1 - rect.left, 
        rect.bottom + 1 - rect.top,
        rect.left,
        rect.top,
        FALSE
        );
}

/*****************************************************************************/
// 	Description : 检查当前位置移动后是否还在屏幕范围内
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsInLimit(
                        MMIIDLE_SPRITE_E sprite_type,
                        int32 move_offset
                        )
{
    int32 offset_y = 0;

    offset_y = (int16)s_sprite_group.unit[s_pressed_sprite].offset_y + move_offset;

    if ((offset_y >= 0)
        && (offset_y<=(MMI_MAINSCREEN_HEIGHT-(int16)s_sprite_group.unit[s_pressed_sprite].buf_height)))
    {
        return TRUE;
    }
    return FALSE;
}


#ifdef UI_MULTILAYER_TEST
/*****************************************************************************/
//  Description : draw layer test
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void DrawLayerTest(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    GUI_RECT_T rect = {0, 0, 99, 49};
    wchar test_str[] = {'t', 'e', 's', 't'};
    UILAYER_HANDLE_T ui_layer_arr[2] = {0};

    GUISTR_STYLE_T      text_style      = {0};
    GUISTR_STATE_T      state =         GUISTR_STATE_WORDBREAK;
    MMI_STRING_T        string = {0};


    if (UILAYER_IsLayerActive(s_test_layer_handle))
    {
        lcd_dev_info.block_id = s_test_layer_handle;
        LCD_FillRect(&lcd_dev_info, rect, 0x8080);
//        UILAYER_FillColor(s_test_layer_handle, 0x8080);

        string.wstr_len = ARR_SIZE(test_str);
        string.wstr_ptr = test_str;
        
        text_style.font_color = MMI_YELLOW_COLOR;
        text_style.font = MMI_DEFAULT_NORMAL_FONT;

        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&rect,       //the fixed display area
            (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
            (const MMI_STRING_T     *)&string,
            (GUISTR_STYLE_T         *)&text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );

        LCD_DrawRect(&lcd_dev_info, rect, MMI_YELLOW_COLOR);
        LCD_DrawLine(&lcd_dev_info, 30, 30, 40, 40, MMI_YELLOW_COLOR);
        LCD_DrawLine(&lcd_dev_info, 70, 10, 40, 40, MMI_YELLOW_COLOR);

        ui_layer_arr[0] = UILAYER_MAIN_HANDLE;
        ui_layer_arr[1] = s_test_layer_handle;
        UILAYER_SetBltLayer(lcd_dev_info.lcd_id, ui_layer_arr, ARR_SIZE(ui_layer_arr));
    }
}

/*****************************************************************************/
//  Description : create layer test
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void CreateLayerTest(
                           int16 offset_x,
                           int16 offset_y
                           )
{
    UILAYER_CreateLayer(MAIN_IDLE_WIN_ID, offset_x, offset_y, 100, 50, &s_test_layer_handle);
    
    UILAYER_SetLayerAlpha(s_test_layer_handle, 128);    
}

/*****************************************************************************/
//  Description : handle layer test message
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLayerTestMsg(
                         MMI_WIN_ID_T         win_id,
                         MMI_MESSAGE_ID_E     msg_id,
                         DPARAM               param
                         )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_APP_RIGHT:
        s_offset_x+=10;
        UILAYER_SetPosition(s_test_layer_handle, s_offset_x, s_offset_y);
        break;

    case MSG_APP_LEFT:
        s_offset_x-=10;
        UILAYER_SetPosition(s_test_layer_handle, s_offset_x, s_offset_y);
        break;
    case MSG_APP_UP:
        s_offset_y-=10;
        UILAYER_SetPosition(s_test_layer_handle, s_offset_x, s_offset_y);
        break;
    case MSG_APP_DOWN:
        s_offset_y+=10;
        UILAYER_SetPosition(s_test_layer_handle, s_offset_x, s_offset_y);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
#endif
/*****************************************************************************/
//  Description : create layer test
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIML_CreateLayerTest(void)
{
#ifdef UI_MULTILAYER_TEST
        CreateLayerTest(s_offset_x, s_offset_y);
#endif 
}
/*****************************************************************************/
//  Description : create layer test
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIML_DrawLayerTest(void)
{
#ifdef UI_MULTILAYER_TEST
    DrawLayerTest();
#endif 
}

/*****************************************************************************/
//  Description : handle layer test message
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIML_HandleLayerTest(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
#ifdef UI_MULTILAYER_TEST        
        if (MMI_RESULT_TRUE != HandleLayerTestMsg(win_id,msg_id,param))
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
#else
        return FALSE;
#endif
}
