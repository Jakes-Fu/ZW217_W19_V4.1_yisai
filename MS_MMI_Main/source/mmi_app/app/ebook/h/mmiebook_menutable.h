/*****************************************************************************
** File Name:      mmiebook_menutable.h                                         *
** Author:         songbin.zeng                                              *
** Date:           2006/05/17                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to ebook menu                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/05/17     songbin.zeng      Create                                   *
******************************************************************************/

#ifndef _MMIEBOOK_MENUTABLE_H_
#define _MMIEBOOK_MENUTABLE_H_

#include "mmi_module.h"
#ifdef EBOOK_SUPPORT
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


//Menu ID
typedef enum
{
    MMIEBOOK_MENU_ID_MIN = (MMI_MODULE_EBOOK << 16),
    ID_EBOOK_READ,
    ID_EBOOK_DETAIL,
    ID_EBOOK_RENAME,//@maggie add for ebook
    ID_EBOOK_COPY,
    ID_EBOOK_DELETE,
    ID_EBOOK_DELALL,
    ID_EBOOK_SENDBT,
    ID_EBOOK_BACKLIGHT_TURN_OFF,
    ID_EBOOK_BACKLIGHT_TURN_ON,
    ID_EBOOK_CODING_TYPE,    
    ID_EBOOK_ADD,
    ID_EBOOK_UPDATE_LIST,
    ID_EBOOK_LIST_MAKEABLE,
    ID_EBOOK_LIST_MARK,
    ID_EBOOK_LIST_CANCEL_MARK,
    ID_EBOOK_LIST_MARKALL,
    ID_EBOOK_LIST_CANCEL_MARKALL,    
    ID_EBOOK_CURRENT_BOOKS,
    ID_EBOOK_CLEAR,
#ifdef TTS_SUPPORT
    ID_EBOOK_SET_TTS_OPEN_CLOSE,
    ID_EBOOK_SET_TTS_VOL,
#endif
    ID_EBOOK_SENDMMS,
    ID_EBOOK_AUTO_READ,
    ID_EBOOK_ADVANCE_OPT,
    MMIEBOOK_MENU_ID_MAX
}MMIEBOOK_MENU_ID_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIEBOOK_MENU_TABEL_ID_MIN = (MMI_MODULE_EBOOK << 16),
    #include "mmiebook_menutable.def"
    MMIEBOOK_MENU_TABLE_ID_MAX
} MMIEBOOK_MENU_TABEL_ID_E;

#undef MENU_DEF





/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/
#endif//ENG_SUPPORT
#endif /* MENUTABLE_H */

