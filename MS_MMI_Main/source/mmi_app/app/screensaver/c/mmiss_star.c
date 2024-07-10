/*****************************************************************************
** File Name:      mmiidle_screensaver_star.c                                     *
** Author:                                                                   *
** Date:           05/15/2008                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe special screensaver                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/15/2008       bin.ji             Create
******************************************************************************/

#define _MMIIDLE_SCREENSAVER_STAR_C_

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

#define STAR_COLOR             MMI_YELLOW_COLOR     //“星”的颜色
#define DX(len)     (len>>1)            //根据星的臂长计算顶点Y坐标
#define DY(len)     (len-(len>>3))      //根据星的臂长计算顶点X坐标

#define STAR_LEN_MIN    10      //星的臂长最小值
#define STAR_LEN_MAX    40      //星的臂长最大值
#define ENV_RECT_LEFT       0                                   //环境范围（left）
#define ENV_RECT_TOP        0                                   //环境范围（top）


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 初始化屏保
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void InitStarScr(void);

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
LOCAL void TriggerStarScr(void);

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

 /**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

//“星”屏保
PUBLIC MMIIDLE_SPESCR_T g_star_scr =
{
    InitStarScr,
    RedrawScr,
    TriggerStarScr        
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 画屏
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DrawStarScr(void);

/*****************************************************************************/
// 	Description : 产生>=min且<=max的一个随机数
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL uint16 GenRandNum(
                      uint16 min,
                      uint16 max
                      );

/*****************************************************************************/
// 	Description : 画屏
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DrawStar(
                    uint16 x,
                    uint16 y,
                    uint16 len
                    );

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 初始化屏保
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void InitStarScr(void)
{
}

/*****************************************************************************/
// 	Description : 重绘屏幕
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void RedrawScr(void)
{
    DrawStarScr();
}

/*****************************************************************************/
// 	Description : 屏保定时器事件处理
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void TriggerStarScr(void)
{
    DrawStarScr();
}

/*****************************************************************************/
// 	Description : 画屏
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DrawStarScr(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};
    GUI_RECT_T rect = MMITHEME_GetFullScreenRect();

    int32 count = 10;
    uint16 x = 0;
    uint16 y = 0;
    uint16 len = 0;
    uint16 half_x = 0;
    uint16 half_y = 0;

    LCD_FillRect(&lcd_dev_info, rect, 0x00);

    while (count > 0)
    {
        len = GenRandNum(STAR_LEN_MIN, STAR_LEN_MAX);
        half_x = len;
        half_y = DY(len);
        x = GenRandNum(ENV_RECT_LEFT+half_x, rect.right-half_x);// @cr235885 renwei modify 
        y = GenRandNum(ENV_RECT_TOP+half_y, rect.bottom-half_y);// @cr235885 renwei modify 
        DrawStar(x, y, len);
        count--;
    }
}

/*****************************************************************************/
// 	Description : 在某位置，以某一长度画一颗星
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DrawStar(
                    uint16 x,
                    uint16 y,
                    uint16 len
                    )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0, 0};

    LCD_DrawLine(&lcd_dev_info, x+DX(len), y+DY(len), x-DX(len), y-DY(len), STAR_COLOR);
    LCD_DrawLine(&lcd_dev_info, x+DX(len), y-DY(len), x-DX(len), y+DY(len), STAR_COLOR);
    LCD_DrawLine(&lcd_dev_info, x+len, y, x-len, y, STAR_COLOR);
}
 
/*****************************************************************************/
// 	Description : 产生>=min且<=max的一个随机数
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL uint16 GenRandNum(
                      uint16 min,
                      uint16 max
                      )
{
    uint16 range = (max-min);
    uint16 num = 0;

    num = min+(uint16)rand()%(range+1);
    return num;
}
#endif
