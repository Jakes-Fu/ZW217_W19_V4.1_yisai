/*****************************************************************************
** File Name:      mmiset_mainmenu_internal.h                                                  *
** Author:                                                                   *
** Date:           10/02/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2004       Jassmine.Meng       Create
** 02/2012       xiyuan.ma           Edite
******************************************************************************/
#ifndef _MMISET__MAINMENU_INTERNAL_H_
#define _MMISET__MAINMENU_INTERNAL_H_ 

#include "mmiset_mainmenu_export.h"
/*****************************************************************************/
//  Description : judge is mainmenu redraw time
//  Global resource dependence : 
//  Author: MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN IsMainMenuRedrawTimer(uint8 timer);
/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void StopSetingRedrawTimer(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void StartSetingRedrawTimer(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void SetMainmenuPrePoint(int16 x, int16 y);
/*****************************************************************************/
//  Description : handle main menu preview set win
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MainmenuStylePreviewDispImage(
                                         uint16 item_index, 
                                         int16 offset_x,
                                         GUI_RECT_T client_rect,
                                         MMI_WIN_ID_T win_id
                                         );
/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MoveSetingPreview(
                                GUI_POINT_T tp_point,
                                uint16 item_index,
                                uint16 max_item_num,
                                MMI_WIN_ID_T  win_id
                                );
/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 ResumeSetingPreview(
                                 GUI_POINT_T  tp_point,
                                 uint16       item_index,
                                 uint16       max_item_num,
                                 MMI_WIN_ID_T win_id
                                 );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC void SetMainmenuStartPoint(int16 x, int16 y);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif

