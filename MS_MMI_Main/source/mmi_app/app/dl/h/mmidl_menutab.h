/****************************************************************************
** File Name:      mmidl_menutab.h                                         *
** Author:                                                                  *
** Date:           06/15/2009                                               *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the window for download *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 07/2009        wancan.you       Create
** 
****************************************************************************/
#ifndef _MMIDL_MENUTABLE_H_
#define _MMIDL_MENUTABLE_H_

#include "mmi_module.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/

#define TIP_NULL 0

//MENU ID
typedef enum
{
    ID_DL_MENU_START = (MMI_MODULE_DL << 16),

    ID_DL_MANAGER_OPT_PAUSE,
    ID_DL_MANAGER_OPT_RESUME,
    ID_DL_MANAGER_OPT_REDOWNLOAD,
    ID_DL_MANAGER_OPT_DEL,
    ID_DL_MANAGER_OPT_DEL_ALL,
    ID_DL_MANAGER_OPT_DETAIL,
    ID_DL_MANAGER_OPT_FILE_EXPLORER,
    ID_DL_MANAGER_OPT_SETTINGS,

    ID_DL_MENU_MAX
}MMIDL_ID_MENU_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIDL_MENU_LABEL_START = (MMI_MODULE_DL << 16),
#include "mmidl_menutab.def"
    MMIDL_MENUTABLE_MAX
} MMIDL_MENULABEL_E;

#undef MENU_DEF


/*-------------------------------------------------------------------------*/
/*                         VARIABLES                                       */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/*                         FUNCTION DECLARE                                */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register menu group
//	Global resource dependence :mmidl_menu_table
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIDL_RegMenuGroup(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif /* _MMIDL_MENUTABLE_H_ */
