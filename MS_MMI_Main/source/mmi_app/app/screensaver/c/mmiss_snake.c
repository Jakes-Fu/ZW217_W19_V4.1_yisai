/*****************************************************************************
** File Name:      mmiidle_screensaver_snake.c                                     *
** Author:                                                                   *
** Date:           05/12/2008                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe special screensaver                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/12/2008       bin.ji             Creat
******************************************************************************/

#define _MMIIDLE_SCREENSAVER_SNAKE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef SCREENSAVER_SUPPORT
#include "window_parse.h"
#include "mmidisplay_data.h"
//#include "mmiss_export.h"
#include "mmiss_special.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmk_timer.h"
//#include "mmi_default.h"
#include "mmi_appmsg.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define MAX_TURN_NUM            20                  //最大转弯数目
#define SNAKE_COLOR             MMI_YELLOW_COLOR    //“蛇”的颜色
#define OPPO_DIR(dir)           ((dir+2)&0x3)       //相反方向
#define LEFT_DIR(dir)           ((dir+3)&0x3)       //左转方向
#define RIGHT_DIR(dir)          ((dir+1)&0x3)      //右转方向

#define SNAKE_UNIT_WIDTH        5   //蛇单元宽
#define SNAKE_UNIT_HEIGHT       5   //蛇单元高
#define SNAKE_X_BORDER_WIDTH    0   //环境范围（边缘）
#define SNAKE_Y_BORDER_HEIGHT   0   //环境范围（边缘）

#define SNAKE_X_POS_MIN         SNAKE_X_BORDER_WIDTH   //环境范围（x min）
#define SNAKE_Y_POS_MIN         SNAKE_Y_BORDER_HEIGHT   //环境范围（y min）

#define SNAKE_VITE              2   //蛇行进速度（每一次前进几个单元）


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

typedef struct 
{
    uint8 dir;      //方向(0,1,2,3)对应（东，南，西，北）
    uint16 len;     //长度
}SNAKE_TURN_T;      //转弯信息

typedef struct 
{
    uint16 start_x;     //“蛇”头位置
    uint16 start_y;     //“蛇”头位置
    uint32 turn_num;    //转弯数目
    SNAKE_TURN_T next_turn[MAX_TURN_NUM];   //转弯信息
}SNAKE_TRACK_T; //蛇的轨迹

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 初始化屏保
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void InitSnakeScr(void);

/*****************************************************************************/
// 	Description : 重绘屏幕
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void RedrawScr(void);

/*****************************************************************************/
// 	Description : 屏保定时器事件处理
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void TriggerSnakeScr(void);

 /**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

//“贪吃蛇”屏保
MMIIDLE_SPESCR_T g_snake_scr = 
{
    InitSnakeScr,
    RedrawScr,
    TriggerSnakeScr
};


//“蛇”的动态轨迹
LOCAL SNAKE_TRACK_T s_snake_track = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : “蛇”的下一步前进方向选择
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL uint8 FindPossibleDirChange(void);

/*****************************************************************************/
// 	Description : 清屏
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void ClearScreen(void);

/*****************************************************************************/
// 	Description : “蛇”的碰壁判断
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPossibleDir(uint8 dir);

/*****************************************************************************/
// 	Description : 画“蛇”身体的水平段
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DrawSnakeHLine(
                          uint16 x1,
                          uint16 y,
                          uint16 x2
                          );

/*****************************************************************************/
// 	Description : 画“蛇”身体的垂直段
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DrawSnakeVLine(
                          uint16 x,
                          uint16 y1,
                          uint16 y2
                          );

/*****************************************************************************/
// 	Description : 画“蛇”头
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DrawSnakeHead(
                         uint16 x,
                         uint16 y,
                         uint32 dir
                         );

/*****************************************************************************/
// 	Description : “矢量加”计算，dir为矢量方向，len为矢量长度
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void CalVector(
                uint8 dir,
                uint16 len,
                int16 x1,
                int16 y1,
                int16 *x2_ptr,
                int16 *y2_ptr
                );

/*****************************************************************************/
// 	Description : 向前
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void Forward(void);

/*****************************************************************************/
// 	Description : 向左转
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void LeftTurn(void);

/*****************************************************************************/
// 	Description : 向右转
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void RightTurn(void);

/*****************************************************************************/
// 	Description : 蛇的轨迹恢复为初始值
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void ResetSnakeTrack(void);

/*****************************************************************************/
// 	Description : 根据蛇的转弯方向，计算新的轨迹
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CalcNewTrack(uint8 dir_change);

/*****************************************************************************/
// 	Description : 将蛇的轨迹显示出来
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DisplayOneFrame(void);


/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 屏保定时器事件处理
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void TriggerSnakeScr(void)
{
    uint8 dir_change = 0;

    dir_change = FindPossibleDirChange();
    CalcNewTrack(dir_change);
    DisplayOneFrame();
}

/*****************************************************************************/
// 	Description : 将蛇的轨迹显示出来
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DisplayOneFrame(void)
{
    //static GUI_RECT_T s_snake_rect = {0, 0, 0, 0};
    uint8 dir = 0;
    uint16 x1 = 0;
    uint16 x2 = 0;
    uint16 y1 = 0;
    uint16 y2 = 0;
    uint32 i = 0;

    ClearScreen();
    x1 = s_snake_track.start_x;
    y1 = s_snake_track.start_y;
    dir = s_snake_track.next_turn[0].dir;
    DrawSnakeHead(x1, y1, OPPO_DIR(dir));
    for (i = 0; i < s_snake_track.turn_num; i++)
    {
        dir = s_snake_track.next_turn[i].dir;
        CalVector(dir, s_snake_track.next_turn[i].len, x1, y1, &x2, &y2);/*lint !e64*/
        if (0 == dir
            || 2 == dir)     //x++
        {
            DrawSnakeHLine(x1, y1, x2);
        }
        else
        {
            DrawSnakeVLine(x1, y1, y2);
        }
        x1 = x2;
        y1 = y2;
    }
}

/*****************************************************************************/
// 	Description : 根据蛇的转弯方向，计算新的轨迹
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CalcNewTrack(
                           uint8 dir_change
                           )
{
    int16 x1 = 0;
    int16 y1 = 0;
    uint32 i = 0;
    int32 tail_len = SNAKE_VITE;

    x1 = s_snake_track.start_x;
    y1 = s_snake_track.start_y;

    if (0 == s_snake_track.turn_num)
    {
        //异常
        return FALSE;
    }
    //缩短后面的尾巴
    for (i = s_snake_track.turn_num-1; i > 0; i--)
    {
        if (tail_len < s_snake_track.next_turn[i].len)
        {
            break;
        }
        tail_len-=s_snake_track.next_turn[i].len;
    }
    if (s_snake_track.next_turn[i].len < tail_len)
    {
        //SNAKE_VITE大于蛇总长度，异常
        return FALSE;
    }
    s_snake_track.next_turn[i].len-=tail_len;
    s_snake_track.turn_num = (i+1);
        
    if (0 == dir_change)       //forward
    {
        Forward();
    }
    else if (1 == dir_change)  //turn left
    {
        LeftTurn();
        s_snake_track.turn_num++;
    }
    else    //turn right
    {
        RightTurn();    
        s_snake_track.turn_num++;
    }
    return TRUE;         
}

/*****************************************************************************/
// 	Description : 蛇的轨迹恢复为初始值
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void ResetSnakeTrack(void)
{
/*, @cr235885 renwei modify begin*/
    GUI_RECT_T rect  = {0};

    //“蛇”的初始轨迹
    SNAKE_TRACK_T s_init_snake_track = 
    {
        0,
        0,
        6,
        {{0, 3}, {1, 3}, {2, 6}, {3, 6}, {0, 9}, {1, 9}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}
    };

    rect = MMITHEME_GetFullScreenRect();
    
    s_init_snake_track.start_x = (rect.right/SNAKE_UNIT_WIDTH/2);
    s_init_snake_track.start_y = (rect.bottom/SNAKE_UNIT_HEIGHT/2);
    s_snake_track              = s_init_snake_track;
/*, @cr235885 renwei modify end*/ 
}

/*****************************************************************************/
// 	Description : 向前
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void Forward(void)
{
    int16 x1 = 0;
    int16 x2 = 0;
    int16 y1 = 0;
    int16 y2 = 0;
    uint8 dir = 0;

    x1 = s_snake_track.start_x;
    y1 = s_snake_track.start_y;
    dir = s_snake_track.next_turn[0].dir;
    CalVector(OPPO_DIR(dir), SNAKE_VITE, x1, y1, &x2, &y2);
    if (x2 <= 0
        || x2 >= MMI_MAINSCREEN_RIGHT_MAX_PIXEL
        ||y2 <= 0
        || y2 >= MMI_MAINSCREEN_BOTTOM_MAX_PIXEL)
    {
        //return FALSE;
        return;
    }

    s_snake_track.next_turn[0].dir = dir;
    s_snake_track.next_turn[0].len+=SNAKE_VITE;
    s_snake_track.start_x = x2;
    s_snake_track.start_y = y2;
}

/*****************************************************************************/
// 	Description : 向左转
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void LeftTurn(void)
{
    int32 i = 0;
    int16 x1 = 0;
    int16 x2 = 0;
    int16 y1 = 0;
    int16 y2 = 0;
    uint8 dir = 0;
    
    x1 = s_snake_track.start_x;
    y1 = s_snake_track.start_y;
    dir = s_snake_track.next_turn[0].dir;
    CalVector(RIGHT_DIR(dir), SNAKE_VITE, x1, y1, &x2, &y2);
    if (x2 <= 0
        || x2 >= MMI_MAINSCREEN_RIGHT_MAX_PIXEL
        ||y2 <= 0
        || y2 >= MMI_MAINSCREEN_BOTTOM_MAX_PIXEL)
    {
        //return FALSE;
        return;
    }
    for (i = s_snake_track.turn_num-1; i >= 0; i--)
    {
        s_snake_track.next_turn[i+1] = s_snake_track.next_turn[i];
    }
    s_snake_track.next_turn[0].dir = LEFT_DIR(dir);
    s_snake_track.next_turn[0].len = SNAKE_VITE;
    s_snake_track.start_x = x2;
    s_snake_track.start_y = y2;
}

/*****************************************************************************/
// 	Description : 向右转
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void RightTurn(void)
{
    int32 i = 0;
    int16 x1 = 0;
    int16 x2 = 0;
    int16 y1 = 0;
    int16 y2 = 0;
    uint8 dir = 0;
    
    x1 = s_snake_track.start_x;
    y1 = s_snake_track.start_y;
    dir = s_snake_track.next_turn[0].dir;
    CalVector(LEFT_DIR(dir), SNAKE_VITE, x1, y1, &x2, &y2);
    if (x2 <= 0
        || x2 >= MMI_MAINSCREEN_RIGHT_MAX_PIXEL
        ||y2 <= 0
        || y2 >= MMI_MAINSCREEN_BOTTOM_MAX_PIXEL)
    {
        //return FALSE;
        return;
    }
    for (i = s_snake_track.turn_num-1; i >= 0; i--)
    {
        s_snake_track.next_turn[i+1] = s_snake_track.next_turn[i];
    }
    s_snake_track.next_turn[0].dir = RIGHT_DIR(dir);
    s_snake_track.next_turn[0].len = SNAKE_VITE;
    s_snake_track.start_x = x2;
    s_snake_track.start_y = y2;
}

/*****************************************************************************/
// 	Description : “蛇”的下一步前进方向选择
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL uint8 FindPossibleDirChange(void)
{
    uint8 forward_dir = 0;
    uint8 new_dir = 0;

    forward_dir = OPPO_DIR(s_snake_track.next_turn[0].dir);

    //forward
    if ((rand()&0x07)>2)
    {
        if (IsPossibleDir(forward_dir))
        {
            return 0;
        }
    }
    //left or right
    new_dir = ((rand()&0x01)) ? LEFT_DIR(forward_dir) : RIGHT_DIR(forward_dir);
    if (!IsPossibleDir(new_dir))
    {
        new_dir = OPPO_DIR(new_dir);
    }
    if (LEFT_DIR(forward_dir) == new_dir)
    {
        return 1;
    }    
    return 2;
}

/*****************************************************************************/
// 	Description : “矢量加”计算，dir为矢量方向，len为矢量长度
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void CalVector(
                uint8 dir,
                uint16 len,
                int16 x1,
                int16 y1,
                int16 *x2_ptr,
                int16 *y2_ptr
                )
{    
    if (0 == dir)     //x++
    {
        *x2_ptr = (x1+len);
        *y2_ptr = y1;
    }
    else if (1 == dir)    //y++
    {
        *x2_ptr = x1;
        *y2_ptr = (y1+len);
    }
    else if (2 == dir)    //x--
    {
        *x2_ptr = (x1-len);
        *y2_ptr = y1;
    }
    else    //y--
    {
        *x2_ptr = x1;
        *y2_ptr = (y1-len);
    }
}

/*****************************************************************************/
// 	Description : 初始化屏保
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void InitSnakeScr(void)
{
    //srand( (unsigned)time( NULL ) );

    ResetSnakeTrack();
}

/*****************************************************************************/
// 	Description : 重绘屏幕
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void RedrawScr(void)
{
    DisplayOneFrame();
}

/*****************************************************************************/
// 	Description : 画“蛇”身体的水平段
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DrawSnakeHLine(
                          uint16 x1,
                          uint16 y,
                          uint16 x2
                          )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    GUI_RECT_T rect = {0};
    
    //LCD_DrawHLine(&lcd_dev_info, x1, y, x2, SNAKE_COLOR);
    rect.left = MIN(x1,x2) * SNAKE_UNIT_WIDTH;
    rect.top = y * SNAKE_UNIT_HEIGHT;
    rect.right = MAX(x1,x2) * SNAKE_UNIT_WIDTH+SNAKE_UNIT_WIDTH;
    rect.bottom = rect.top + SNAKE_UNIT_HEIGHT;
    LCD_FillRect(&lcd_dev_info, rect, SNAKE_COLOR);
}

/*****************************************************************************/
// 	Description : 画“蛇”身体的垂直段
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DrawSnakeVLine(
                          uint16 x,
                          uint16 y1,
                          uint16 y2
                          )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    GUI_RECT_T rect = {0};
    
    //LCD_DrawVLine(&lcd_dev_info, x, y1, y2, SNAKE_COLOR);
    rect.left = x * SNAKE_UNIT_WIDTH;
    rect.top = MIN(y1,y2) * SNAKE_UNIT_HEIGHT;
    rect.right = rect.left + SNAKE_UNIT_WIDTH;
    rect.bottom = MAX(y1,y2) * SNAKE_UNIT_HEIGHT+SNAKE_UNIT_HEIGHT;
    LCD_FillRect(&lcd_dev_info, rect, SNAKE_COLOR);
}

/*****************************************************************************/
// 	Description : 画“蛇”头
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DrawSnakeHead(
                         uint16 x,
                         uint16 y,
                         uint32 dir
                         )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    uint16 x1 = 0;
    uint16 y1 = 0;
    uint16 x2 = 0;
    uint16 y2 = 0;

    if (0 == dir)
    {
        x1 = x * SNAKE_UNIT_WIDTH + SNAKE_UNIT_WIDTH;
        x2 = x * SNAKE_UNIT_WIDTH;
        y1 = y * SNAKE_UNIT_HEIGHT - SNAKE_UNIT_HEIGHT/2;
        LCD_DrawHLine(&lcd_dev_info, x1, y1, x2, SNAKE_COLOR);
    }
    else if (1 == dir)
    {
        y1 = y * SNAKE_UNIT_HEIGHT + SNAKE_UNIT_WIDTH;
        y2 = y * SNAKE_UNIT_HEIGHT;
        x1 = x * SNAKE_UNIT_WIDTH - SNAKE_UNIT_HEIGHT/2;
        LCD_DrawVLine(&lcd_dev_info, x1, y1, y2, SNAKE_COLOR);
    }
    else if (2 == dir)
    {
        x1 = x * SNAKE_UNIT_WIDTH - SNAKE_UNIT_WIDTH;
        x2 = x * SNAKE_UNIT_WIDTH;
        y1 = y * SNAKE_UNIT_HEIGHT - SNAKE_UNIT_HEIGHT/2;
        LCD_DrawHLine(&lcd_dev_info, x1, y1, x2, SNAKE_COLOR);
    }
    else
    {
        y1 = y * SNAKE_UNIT_HEIGHT - SNAKE_UNIT_WIDTH;
        y2 = y * SNAKE_UNIT_HEIGHT;
        x1 = x * SNAKE_UNIT_WIDTH - SNAKE_UNIT_HEIGHT/2;
        LCD_DrawVLine(&lcd_dev_info, x1, y1, y2, SNAKE_COLOR);
    }
}

/*****************************************************************************/
// 	Description : 清屏
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void ClearScreen(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();

    LCD_FillRect(&lcd_dev_info, rect, 0x00);
}

/*****************************************************************************/
// 	Description : “蛇”的碰壁判断
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPossibleDir(uint8 dir)
{

/*, @cr235885 renwei modify begin*/

    uint16 x         = 0;
    uint16 y         = 0;
    uint16 x_pos_max = 0;
    uint16 y_pos_max = 0;
    GUI_RECT_T rect  = {0};
    BOOLEAN   result = TRUE;
    
    x    = s_snake_track.start_x;
    y    = s_snake_track.start_y;
    rect = MMITHEME_GetFullScreenRect();

    if((0 >= rect.right) || (0 >= rect.bottom))
    {
        return FALSE;
    }
    
    x_pos_max = ((rect.right - SNAKE_X_BORDER_WIDTH*2)/SNAKE_UNIT_WIDTH-1) ;
    y_pos_max = ((rect.bottom - SNAKE_Y_BORDER_HEIGHT*2)/SNAKE_UNIT_HEIGHT-1);
    
    if (0 == dir)     //x++
    {
        if (x>=(x_pos_max-SNAKE_VITE))
        {
            result = FALSE;
        }
    }
    else if (1 == dir)    //y++
    {
        if (y>=(y_pos_max-SNAKE_VITE))
        {
            result = FALSE;
        }
    }
    else if (2 == dir)    //x--
    {
        if (x<=(SNAKE_X_POS_MIN+SNAKE_VITE))
        {
            result = FALSE;
        }
    }
    else    //y--
    {
        if (y<=(SNAKE_Y_POS_MIN+SNAKE_VITE))
        {
            result = FALSE;
        }
    }
    return result;
/*, @cr235885 renwei modify end*/
}
#endif
