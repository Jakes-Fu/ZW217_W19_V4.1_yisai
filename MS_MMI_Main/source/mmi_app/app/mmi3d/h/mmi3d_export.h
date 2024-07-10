/*****************************************************************************
** File Name:      mmi3d_export.h                                                   *
** Author:                                                                   *
** Date:           2/07/2009                                               *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe csk2 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2/07/2009       bin.ji                Create
******************************************************************************/

#ifndef _MMI3D_EXPORT_H_
#define _MMI3D_EXPORT_H_

#ifdef UI_P3D_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "guilcd.h"
#include "ui_special_effect.h"
#include "block_mem_def.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define MMI3D_KEY_COLOR         0x1111
#define MMI3D_KEY_COLOR_MEMSET  0x11


typedef enum
{
    PREV_BUF,       //上一张图片
    NEXT_BUF,       //下一张图片
    PREV_BG_BUF,    //上一张图片背景
    NEXT_BG_BUF,    //下一张图片背景
    
    MAX_BUF
}MMI3D_ICONMENU_BUF_E;


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/  

#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : 判断icon菜单动画是否开启
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPI3D_IsIconMenuAnimOpen(void);

/*****************************************************************************/
//  Description : 设置icon菜单动画的方向
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI3D_SetIconMenuDirection(
                               SE_DIRECTION_E dir
                               );

/*****************************************************************************/
//  Description : 将lcd_buf中指定区域内容保存到menuicon_buf中
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI3D_SaveMenuIcon(
                               MMI3D_ICONMENU_BUF_E buf_id,
                               SE_RECT_T *rect_ptr
                               );

/*****************************************************************************/
//  Description : 启动icon菜单动画
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPI3D_StartIconMenuAnim(void);
#endif

/*****************************************************************************/
//  Description : 分配3d menu的内存，进入主菜单或二级菜单时调用
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI3D_Alloc3DMenuBuf(
                                 BLOCK_MEM_ID_E block_id,
                                 uint32 icon_size,   //icon大小，字节数
                                 MMI_HANDLE_T    win_handle
                                 );

/*****************************************************************************/
//  Description : 释放3d menu的内存，退出主菜单或二级菜单时调用
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI3D_Free3DMenuBuf(BLOCK_MEM_ID_E block_id);


/*****************************************************************************/
//  Description : 将icon_id图片解码到icon_buf_ptr指向的buf中
//  Global resource dependence : 
//  Author: bin.ji
//  Note: part_rect_ptr=PNULL时，将整张icon解码
/*****************************************************************************/
PUBLIC void MMIAPI3D_DecodeIconBuf(
                                MMI_IMAGE_ID_T  icon_id,
                                MMI_WIN_ID_T    win_id,
                                GUI_COLOR_T     *icon_buff_ptr,   //out
                                uint32          icon_buf_width,
                                uint32          icon_buf_height,
                                int16           start_x,
                                int16           start_y,
                                GUI_RECT_T      *part_rect_ptr
                                );

/*****************************************************************************/
//  Description : start item anim,for menu and list,two item move
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI3D_StartItemAnim(BLOCK_MEM_ID_E block_id);

/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
#define MMI3D_IsIconMenuAnimOpen        MMIAPI3D_IsIconMenuAnimOpen
#define MMI3D_SetIconMenuDirection      MMIAPI3D_SetIconMenuDirection
#define MMI3D_SaveMenuIcon              MMIAPI3D_SaveMenuIcon
#define MMI3D_StartIconMenuAnim         MMIAPI3D_StartIconMenuAnim
#endif
#define MMI3D_Alloc3DMenuBuf            MMIAPI3D_Alloc3DMenuBuf
#define MMI3D_Free3DMenuBuf             MMIAPI3D_Free3DMenuBuf
#define MMI3D_DecodeIconBuf             MMIAPI3D_DecodeIconBuf
#define MMI3D_StartItemAnim             MMIAPI3D_StartItemAnim

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif


#endif 


#endif
     
