/****************************************************************************
** File Name:      mmidyna_id.h                                             *
** Author:                                                                  *
** Date:           20/08/2008                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of         *
**                 system, applicaiton, window and control                  *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2008        peng              Create
** 
****************************************************************************/
#ifndef  _MMIDYNA_ID_H_    
#define  _MMIDYNA_ID_H_  

 /**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
    {
#endif

#define WIN_ID_DEF(win_id)          win_id

typedef enum
{
    DYNA_WIN_ID_START = (MMI_MODULE_DYNA_APPLIST<< 16),       
#include "mmidyna_win.def"
    DYNA_MAX_WIN_ID
}DYNA_WINDOW_ID_E;

#undef WIN_ID_DEF


// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: CC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{ 
    DYNA_CTRL_ID_ID_START = DYNA_MAX_WIN_ID, //(MMI_MODULE_GAME_MANAGER << 16),
    MMIDYNA_MAIN_MENU_LISTBOX_CTRL_ID, /*added by chenpeng for list of caf 08/11/08*/   
    MMIDYNA_MYAPP_LIST_OPT_CTRL_ID,
    
    MMIDYNA_APP_DETAIL_NAME_TITLE_LABEL_CTRL_ID,
    MMIDYNA_APP_DETAIL_NAME_LABEL_CTRL_ID,
    MMIDYNA_APP_DETAIL_VERSION_TITLE_LABEL_CTRL_ID,
    MMIDYNA_APP_DETAIL_VERSION_LABEL_CTRL_ID,
    MMIDYNA_APP_DETAIL_DATE_TITLE_LABEL_CTRL_ID,
    MMIDYNA_APP_DETAIL_DATE_LABEL_CTRL_ID,
    MMIDYNA_APP_DETAIL_MEMORY_TITLE_LABEL_CTRL_ID,
    MMIDYNA_APP_DETAIL_MEMORY_LABEL_CTRL_ID,
    MMIDYNA_APP_DETAIL_LOCATION_TITLE_LABEL_CTRL_ID,
    MMIDYNA_APP_DETAIL_LOCATION_LABEL_CTRL_ID,
    
    MMIDYNA_INSTALL_STORE_LIST_CTRL_ID,

    DYNA_MAX_CTRL_ID
}GAME_CTRL_ID_T;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
