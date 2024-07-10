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

#define MAX_TURN_NUM            20                  //���ת����Ŀ
#define SNAKE_COLOR             MMI_YELLOW_COLOR    //���ߡ�����ɫ
#define OPPO_DIR(dir)           ((dir+2)&0x3)       //�෴����
#define LEFT_DIR(dir)           ((dir+3)&0x3)       //��ת����
#define RIGHT_DIR(dir)          ((dir+1)&0x3)      //��ת����

#define SNAKE_UNIT_WIDTH        5   //�ߵ�Ԫ��
#define SNAKE_UNIT_HEIGHT       5   //�ߵ�Ԫ��
#define SNAKE_X_BORDER_WIDTH    0   //������Χ����Ե��
#define SNAKE_Y_BORDER_HEIGHT   0   //������Χ����Ե��

#define SNAKE_X_POS_MIN         SNAKE_X_BORDER_WIDTH   //������Χ��x min��
#define SNAKE_Y_POS_MIN         SNAKE_Y_BORDER_HEIGHT   //������Χ��y min��

#define SNAKE_VITE              2   //���н��ٶȣ�ÿһ��ǰ��������Ԫ��


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

typedef struct 
{
    uint8 dir;      //����(0,1,2,3)��Ӧ�������ϣ���������
    uint16 len;     //����
}SNAKE_TURN_T;      //ת����Ϣ

typedef struct 
{
    uint16 start_x;     //���ߡ�ͷλ��
    uint16 start_y;     //���ߡ�ͷλ��
    uint32 turn_num;    //ת����Ŀ
    SNAKE_TURN_T next_turn[MAX_TURN_NUM];   //ת����Ϣ
}SNAKE_TRACK_T; //�ߵĹ켣

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : ��ʼ������
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void InitSnakeScr(void);

/*****************************************************************************/
// 	Description : �ػ���Ļ
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void RedrawScr(void);

/*****************************************************************************/
// 	Description : ������ʱ���¼�����
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void TriggerSnakeScr(void);

 /**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

//��̰���ߡ�����
MMIIDLE_SPESCR_T g_snake_scr = 
{
    InitSnakeScr,
    RedrawScr,
    TriggerSnakeScr
};


//���ߡ��Ķ�̬�켣
LOCAL SNAKE_TRACK_T s_snake_track = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : ���ߡ�����һ��ǰ������ѡ��
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL uint8 FindPossibleDirChange(void);

/*****************************************************************************/
// 	Description : ����
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void ClearScreen(void);

/*****************************************************************************/
// 	Description : ���ߡ��������ж�
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsPossibleDir(uint8 dir);

/*****************************************************************************/
// 	Description : �����ߡ������ˮƽ��
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
// 	Description : �����ߡ�����Ĵ�ֱ��
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
// 	Description : �����ߡ�ͷ
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
// 	Description : ��ʸ���ӡ����㣬dirΪʸ������lenΪʸ������
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
// 	Description : ��ǰ
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void Forward(void);

/*****************************************************************************/
// 	Description : ����ת
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void LeftTurn(void);

/*****************************************************************************/
// 	Description : ����ת
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void RightTurn(void);

/*****************************************************************************/
// 	Description : �ߵĹ켣�ָ�Ϊ��ʼֵ
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void ResetSnakeTrack(void);

/*****************************************************************************/
// 	Description : �����ߵ�ת�䷽�򣬼����µĹ켣
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CalcNewTrack(uint8 dir_change);

/*****************************************************************************/
// 	Description : ���ߵĹ켣��ʾ����
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
// 	Description : ������ʱ���¼�����
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
// 	Description : ���ߵĹ켣��ʾ����
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
// 	Description : �����ߵ�ת�䷽�򣬼����µĹ켣
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
        //�쳣
        return FALSE;
    }
    //���̺����β��
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
        //SNAKE_VITE�������ܳ��ȣ��쳣
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
// 	Description : �ߵĹ켣�ָ�Ϊ��ʼֵ
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void ResetSnakeTrack(void)
{
/*, @cr235885 renwei modify begin*/
    GUI_RECT_T rect  = {0};

    //���ߡ��ĳ�ʼ�켣
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
// 	Description : ��ǰ
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
// 	Description : ����ת
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
// 	Description : ����ת
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
// 	Description : ���ߡ�����һ��ǰ������ѡ��
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
// 	Description : ��ʸ���ӡ����㣬dirΪʸ������lenΪʸ������
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
// 	Description : ��ʼ������
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
// 	Description : �ػ���Ļ
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void RedrawScr(void)
{
    DisplayOneFrame();
}

/*****************************************************************************/
// 	Description : �����ߡ������ˮƽ��
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
// 	Description : �����ߡ�����Ĵ�ֱ��
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
// 	Description : �����ߡ�ͷ
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
// 	Description : ����
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
// 	Description : ���ߡ��������ж�
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
