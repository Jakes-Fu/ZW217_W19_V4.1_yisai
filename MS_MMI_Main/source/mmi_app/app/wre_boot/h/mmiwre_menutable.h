/*****************************************************************************
** File Name:     mmiwre_menutable.h                                                   *
** Author:                                                                   *
** Date:           11/21/2005                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2005       annie.an      Create
******************************************************************************/

#ifndef _MMIWRE_MENUTABLE_H_
#define _MMIWRE_MENUTABLE_H_
#ifdef WRE_SUPPORT
#include "mmiwre.h"
#include "mmi_module.h"
#include "guimenu.h"
#include "mmi_menutable.h"
#include "mmiwre_text.h"
#include "mmi_image.h"

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

typedef enum
{
	DEMO_ID_MENU_START = (MMI_MODULE_WRE << 16),
	IDDEMO_MENU_ITEM_ID,
	IDDEMO_MENU_ITEMEX_ID,
	
	DEMO_ID_MENU_MAX
}DEMO_ID_MENU_E;




#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    DEMO_MENU_LABEL_START = (MMI_MODULE_WRE << 16),
#include "mmiwre_menutable.def"
    DEMO_MENUTABLE_MAX
} DEMO_MENU_LABEL_E;

#undef MENU_DEF

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/
#endif //WRE_SUPPORT
#endif /* MENUTABLE_H */


