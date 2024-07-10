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
    PREV_BUF,       //��һ��ͼƬ
    NEXT_BUF,       //��һ��ͼƬ
    PREV_BG_BUF,    //��һ��ͼƬ����
    NEXT_BG_BUF,    //��һ��ͼƬ����
    
    MAX_BUF
}MMI3D_ICONMENU_BUF_E;


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/  

#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : �ж�icon�˵������Ƿ���
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPI3D_IsIconMenuAnimOpen(void);

/*****************************************************************************/
//  Description : ����icon�˵������ķ���
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI3D_SetIconMenuDirection(
                               SE_DIRECTION_E dir
                               );

/*****************************************************************************/
//  Description : ��lcd_buf��ָ���������ݱ��浽menuicon_buf��
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI3D_SaveMenuIcon(
                               MMI3D_ICONMENU_BUF_E buf_id,
                               SE_RECT_T *rect_ptr
                               );

/*****************************************************************************/
//  Description : ����icon�˵�����
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPI3D_StartIconMenuAnim(void);
#endif

/*****************************************************************************/
//  Description : ����3d menu���ڴ棬�������˵�������˵�ʱ����
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI3D_Alloc3DMenuBuf(
                                 BLOCK_MEM_ID_E block_id,
                                 uint32 icon_size,   //icon��С���ֽ���
                                 MMI_HANDLE_T    win_handle
                                 );

/*****************************************************************************/
//  Description : �ͷ�3d menu���ڴ棬�˳����˵�������˵�ʱ����
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI3D_Free3DMenuBuf(BLOCK_MEM_ID_E block_id);


/*****************************************************************************/
//  Description : ��icon_idͼƬ���뵽icon_buf_ptrָ���buf��
//  Global resource dependence : 
//  Author: bin.ji
//  Note: part_rect_ptr=PNULLʱ��������icon����
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
//  ���º궨������ʵ�ֶ���ӿڸ���ǰ��ļ���
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
     
