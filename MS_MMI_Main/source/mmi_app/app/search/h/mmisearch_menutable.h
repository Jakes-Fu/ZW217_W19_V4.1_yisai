/*****************************************************************************
** File Name:      mmisearch_menutable.h                                    *
** Author:                                                                   *
** Date:           03/2010                                                *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2010       plum.peng           Create
******************************************************************************/

#ifndef _MMISEARCH_MENUTABLE_H_
#define _MMISEARCH_MENUTABLE_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_module.h"
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
    ID_SEARCH_MENU_START = (MMI_MODULE_SEARCH << 16),

    ID_SEARCH_OPEN,
    ID_SEARCH_DELETE,
    ID_SEARCH_SEND,
    ID_SEARCH_DETAIL,
    ID_SEARCH_RENAME,
    ID_SEARCH_MARKED,
    ID_SEARCH_SORT,
    ID_SEARCH_SEND_MMS,
    
#ifdef BLUETOOTH_SUPPORT
    ID_SEARCH_SEND_BT,
#endif
    
    ID_SEARCH_UNMARKED,
    ID_SEARCH_MARKED_ALL,
    ID_SEARCH_UNMARKED_ALL,
    ID_SEARCH_NAME,
    ID_SEARCH_DATE,
    ID_SEARCH_SIZE,
    ID_SEARCH_TYPE,
    ID_SEARCH_TYPE_ALL,
    ID_SEARCH_TYPE_APP,
    ID_SEARCH_TYPE_FILE,
    ID_SEARCH_TYPE_PB,
    ID_SEARCH_TYPE_MSG,
    ID_SEARCH_TYPE_CL,
	ID_SEARCH_MENU_MAX
} MMISEARCH_MENU_ID_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,
typedef enum
{
    MMISEARCH_MENU_START = (MMI_MODULE_SEARCH << 16),
    #include "mmisearch_menutable.def"
    MMISEARCH_MENU_MAX
} MMISEARCH_MENU_LABEL_E;
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
PUBLIC void MMISEARCH_RegMenuGroup(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

