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
    MMIIDLE_SPRITE_NONE,        //��sprite

    MMIIDLE_BOTTOM_SPRITE,      //�ײ�sprite
    MMIIDLE_TOP_SPRITE,         //�ϲ�sprite

    MMIIDLE_SPRITE_MAX
}MMIIDLE_SPRITE_E;  //sprite�����

typedef struct 
{
    MMI_IMAGE_ID_T icon_id; //ͼƬid
    uint16 *buf_ptr;    //icon_id���������bufָ��
    uint32 buf_size;    //icon_id���������buf��С
    uint32 buf_width;       //icon_id���������buf���
    uint32 buf_height;      //icon_id���������buf�߶�
    uint32 offset_y;    //y�����ƫ��
    uint16 *restore_bg_ptr; //�����ָ��ı���buf����С����buf_size
    uint32 old_offset_y;    //�ɵ�y�����ƫ�ƣ������ָ�����
}MMIIDLE_SPRITE_T;  //sprite�ṹ

typedef struct 
{
    MMIIDLE_SPRITE_T unit[MMIIDLE_SPRITE_MAX];  //ÿ��item
    BOOLEAN is_sprite_enable;    //spriteЧ���Ƿ���
}MMIIDLE_SPRITE_GROUP_T;    //sprite��


/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : ��ʾsprite
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_ShowSprite(void);

/*****************************************************************************/
// 	Description : ��ʼ��sprite��������ʱ�����
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void  MMIIDLE_InitSprite(void);

/*****************************************************************************/
// 	Description : ����Up��Ϣ
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_PressUpSprite(
                   int16 x,
                   int16 y
                   );

/*****************************************************************************/
// 	Description : ������Down��Ϣ
//	Global resource dependence :
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_PressDownSprite(
                     int16 x,
                     int16 y
                     );

/*****************************************************************************/
// 	Description : ������Move��Ϣ
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