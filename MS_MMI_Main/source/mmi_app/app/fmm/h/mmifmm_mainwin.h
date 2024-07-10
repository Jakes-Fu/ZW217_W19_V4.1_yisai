/*****************************************************************************
** File Name:      mmifmm_mainwin.h                                          *
** Author:                                                                   *
** Date:           07/2012                                                   *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file manager module         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012        jian.ma          Create                                    *
******************************************************************************/

#ifndef _MMIFMM_MAINWIN_H_
#define _MMIFMM_MAINWIN_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_export.h"
#include "mmifmm_interface.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : open any fmm win with callback function
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_OpenExplorerWin (MMIFMM_OPENWIN_INFO_T * openwin_info);

/*****************************************************************************/
//  Description : 清除列表数据
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_RemoveListData (MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : open menu pub win
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_OpenMeunPubWin (
    MMI_WIN_ID_T         dst_win_id,
    MMI_MENU_GROUP_ID_T  menu_group_id,
    MMI_WIN_ID_T         menu_win_id,
    MMI_CTRL_ID_T        menu_ctrl_id
);

/*****************************************************************************/
//  Description : handle menu option function
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_HandleMenuOpt (MMIFMM_OPTION_TYPE_E type);

/*****************************************************************************/
//  Description : set tab title
//  Global resource dependence :
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SetWinTitle (BOOLEAN is_normal_type,
                                MMI_WIN_ID_T win_id,
                                MMI_CTRL_ID_T tab_ctrl_id,
                                wchar * current_path,
                                uint16  current_path_len);

/*****************************************************************************/
// Description : update file list
// Global resource dependence :
// Author:  jian.ma
// RETRUN:
// Note:
/*****************************************************************************/
PUBLIC void MMIFMM_UpdateFmmList (uint32 device, BOOLEAN is_enter_root);


/*****************************************************************************/
//  Description :if is FmmMainWinOpen
//  Global resource dependence :
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsFmmMainWinOpen (void);

/*****************************************************************************/
//  Description : get display file type
//  Global resource dependence :
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIFMM_GetDisplayFileType (void);

/*****************************************************************************/
//  Description : get select tartget path
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_OpenTartgetWin (MMIFMM_OpenFileCallBack callback);

#ifdef MMI_SYNCTOOL_SUPPORT
/*****************************************************************************/
//  Description : get current path
//  Global resource dependence :
//  Author: jian.ma
//  Note: only for auto test
/*****************************************************************************/
PUBLIC MMIFMM_PATH_INFO_T *MMIFMM_GetCurrentPath (void);
#endif
#endif