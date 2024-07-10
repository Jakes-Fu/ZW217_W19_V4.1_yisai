/*****************************************************************************
** File Name:      mmi_pubmenuwin.h                                          *
** Author:                                                                   *
** Date:           01/13/2009                                                *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2009      James.Zhang           Creat                                  *
******************************************************************************/

#ifndef _MMI_PUBMENUWIN_H_
#define _MMI_PUBMENUWIN_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
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
#include "mmk_type.h"
#include "guimenu.h"
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
    MMI_WIN_ID_T        win_id;
    MMI_CTRL_ID_T       ctrl_id;
    MMI_HANDLE_T        win_handle;
    MMI_HANDLE_T        ctrl_handle;
    MMI_HANDLE_T        dst_handle;
    MMI_MENU_GROUP_ID_T group_id;
    MMI_MENU_ID_T       menu_id;
    
} MMIPUB_MENU_NOTIFY_T;

//brief menu process function
typedef struct
{
    void    (*OnMenuInit)   ( MMIPUB_MENU_NOTIFY_T* param );
    void    (*OnMenuDestory)( MMIPUB_MENU_NOTIFY_T* param );
    void    (*OnMenuOk)     ( MMIPUB_MENU_NOTIFY_T* param );
    void    (*OnMenuCancel) ( MMIPUB_MENU_NOTIFY_T* param );
    
} MMIPUB_MENU_FUNC_T;
//pub menu win param
typedef struct
{
    MMI_MENU_GROUP_ID_T     group_id;
    MMI_WIN_ID_T            win_id;                     //用户指定的窗口id, 如为0则使用默认的id
    MMI_CTRL_ID_T           ctrl_id;                    //用户指定的控件id, 如为0则使用默认的id
    MMI_HANDLE_T            applet_handle;              //窗口的父应用句柄, 为0则使用默认的first applet handle
    MMI_HANDLE_T            dst_handle;                 //窗口是由谁创建出来的
    MMIPUB_MENU_FUNC_T      func;
    GUIMENU_STYLE_E         menu_style;                  //仅供动态菜单指定风格
} MMIPUB_MENU_PARAM_T;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : open pub menu win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenPubMenuWin(
                                  MMIPUB_MENU_PARAM_T* param
                                  );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
