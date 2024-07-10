/*****************************************************************************
** File Name:      mmi3d.h                                                   *
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

#ifndef _MMI3D_H_
#define _MMI3D_H_

#ifdef UI_P3D_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "guilcd.h"
#include "ui_special_effect.h"
#include "block_mem_def.h"
#include "mmi3d_export.h"
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


//3d menu
typedef struct 
{
    GUI_COLOR_T *prev_icon_ptr; 
    GUI_COLOR_T *next_icon_ptr;
    GUI_COLOR_T *out_icon_ptr;
    GUI_COLOR_T *temp_icon_ptr;
    GUI_COLOR_T *prev_bg_icon_ptr;
    GUI_COLOR_T *next_bg_icon_ptr;
    GUI_COLOR_T *restore_bg_ptr;
}MMIMENU_3DMENUBUF_T;       //����3DMenu��icon bufָ��


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/  



/*****************************************************************************/
//  Description : �õ�3d menu���ڴ�ָ�룬���������˵�����
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI3D_Get3DMenuBufInfo(
                                   MMIMENU_3DMENUBUF_T *menubuf_info_ptr    //out
                                   );


/*****************************************************************************/
//  Description : ǿ���ӳ�һ��ʱ�䣨��¼��ʼʱ�䣩
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI3D_ForceDelayStart(void);

/*****************************************************************************/
//  Description : ǿ���ӳ�һ��ʱ�䣨��¼����ʱ�䣬�������delay_ticks���㣩
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI3D_ForceDelayEnd(
                                uint32 delay_ticks
                                );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  

#endif
     
