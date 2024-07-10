/****************************************************************************
** File Name:      mmidle_sprite.h                                               *
** Author:         bin.ji                                              *
** Date:           06/30/2008                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the main application.     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/30/2008         bin.ji       Create
** 
****************************************************************************/
#ifndef  _MMIIDLE_SPRITE_H_    
#define  _MMIIDLE_SPRITE_H_   


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmk_type.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

#define BOTTOM_SPRITE_WIDTH     200
#define BOTTOM_SPRITE_HEIGHT    100
#define TOP_SPRITE_WIDTH        150
#define TOP_SPRITE_HEIGHT       150

#define DEFAULT_BOTTOM_SPRITE_OFFSET_Y  110
#define DEFAULT_TOP_SPRITE_OFFSET_Y     135


/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/

typedef enum
{
    MMIIDLE_SPRITE_NONE,        //无sprite

    MMIIDLE_BOTTOM_SPRITE,      //底层sprite
    MMIIDLE_TOP_SPRITE,         //上层sprite

    MMIIDLE_SPRITE_MAX
}MMIIDLE_SPRITE_E;  //sprite的类别

typedef struct 
{
    MMI_IMAGE_ID_T icon_id; //图片id
    uint16 *buf_ptr;    //icon_id解码出来的buf指针
    uint32 buf_size;    //icon_id解码出来的buf大小
    uint32 buf_width;       //icon_id解码出来的buf宽度
    uint32 buf_height;      //icon_id解码出来的buf高度
    uint32 offset_y;    //y方向的偏移
    uint16 *restore_bg_ptr; //用来恢复的背景buf，大小等于buf_size
    uint32 old_offset_y;    //旧的y方向的偏移，用来恢复背景
}MMIIDLE_SPRITE_T;  //sprite结构

typedef struct 
{
    MMIIDLE_SPRITE_T unit[MMIIDLE_SPRITE_MAX];  //每个item
    BOOLEAN is_sprite_enable;    //sprite效果是否开启
}MMIIDLE_SPRITE_GROUP_T;    //sprite组


/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 显示sprite
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_ShowSprite(void);

/*****************************************************************************/
// 	Description : 初始化sprite，开机的时候调用
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void  MMIIDLE_InitSprite(void);

/*****************************************************************************/
// 	Description : 处理触Up消息
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_PressUpSprite(
                   int16 x,
                   int16 y
                   );

/*****************************************************************************/
// 	Description : 处理触笔Down消息
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_PressDownSprite(
                     int16 x,
                     int16 y
                     );

/*****************************************************************************/
// 	Description : 处理触笔Move消息
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_PressMoveSprite(
                     int16 x,
                     int16 y
                     );
/*****************************************************************************/
//  Description : create layer test
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIML_CreateLayerTest(void);

/*****************************************************************************/
//  Description : create layer test
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIML_DrawLayerTest(void);
/*****************************************************************************/
//  Description : handle layer test message
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIML_HandleLayerTest(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif