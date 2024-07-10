/*****************************************************************************
** File Name:      mmipicview_menutable.h                                    *
** Author:                                                                   *
** Date:           24/03/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmimp3_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008       Jassmine           Create
******************************************************************************/
#ifdef PIC_VIEWER_SUPPORT
#ifndef _MMIPICVIEW_MENUTABLE_H_
#define _MMIPICVIEW_MENUTABLE_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_image.h"

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
        
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//menu id
typedef enum
{
    ID_PICVIEW_MENU_START = (MMI_MODULE_PIC_VIEWER << 16),

    ID_PICVIEW_CHANGE_FOLDER,
    ID_PICVIEW_SEND,
    ID_PICVIEW_SLIDE_PREVIEW,
    ID_PICVIEW_SLIDE_SET,
    ID_PICVIEW_VIEW,
    ID_PICVIEW_VIEW_LIST,
    ID_PICVIEW_VIEW_ICONLIST,
    ID_PICVIEW_WALLPAPER,
    ID_PICVIEW_POWER_ON_SET,
    ID_PICVIEW_POWER_OFF_SET,
    ID_PICVIEW_PB,
    ID_PICVIEW_ZOOM,
    ID_PICVIEW_EDIT,
    ID_PICVIEW_MARKED,
    ID_PICVIEW_DELETE,
    ID_PICVIEW_RENAME,
    ID_PICVIEW_DETAIL,

    ID_PICVIEW_MMS,
    ID_PICVIEW_BT,

    ID_PICVIEW_UNMARKED,
    ID_PICVIEW_MARKED_ALL,
    ID_PICVIEW_UNMARKED_ALL,

#ifndef PIC_PREVIEW_U200_SUPPORT
    ID_PICVIEW_SETTING,
#endif
#ifdef SNS_SUPPORT
    ID_PICVIEW_SHARE_TO_SINA,
    ID_PICVIEW_SHARE_TO_FACEBOOK,
    ID_PICVIEW_SHARE_TO_TIWTTER,
#endif

	ID_PICVIEW_MENU_MAX
} MMIPICVIEW_MENU_ID_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,
typedef enum
{
    MMIPICVIEW_MENU_START = (MMI_MODULE_PIC_VIEWER << 16),
    #include "mmipicview_menutable.def"
    MMIPICVIEW_MENU_MAX
} MMIPICVIEW_MENU_LABEL_E;
#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register picture viewer menu group
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_RegMenuGroup(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif //#ifdef PIC_VIEWER_SUPPORT
