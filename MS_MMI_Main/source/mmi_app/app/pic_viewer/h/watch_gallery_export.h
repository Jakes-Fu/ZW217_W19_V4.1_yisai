/*****************************************************************************
** File Name:      watch_gallery_export.h                                    *
** Author:         longwei.qiao                                              *
** Date:           16/02/2020                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe gallery export interface    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2/2020       longwei.qiao         Create                                  *
******************************************************************************/
#ifdef PIC_VIEWER_SUPPORT
#ifndef _WATCH_GALLERY_EXPORT_H_
#define _WATCH_GALLERY_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmipicview_export.h"
#include "mmifilearray_export.h"

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : pause for CC
//  Paramter:
//          [In]:
//          [Out]:
//          [Return]:
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void WatchGallery_Vp_PauseForCC(void);

/*****************************************************************************/
//  Description : enter gallery main win
//  Paramter:
//          [In]:none
//          [Out]:none
//          [Return]:none
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void WatchGallery_MainWin_Enter(void);

PUBLIC void GalleryPicListMarked(MMI_CTRL_ID_T  list_ctrl_id);

PUBLIC void WatchGallery_Exit(void);

#endif
#endif
