/***************************************************************************
** File Name:      mmiclipbrd_menutable.h                                  *
** Author:         hua.fang                                                *
** Date:           06/05/2010                                              *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2010        hua.fang         Create                                  *
**																		   *
****************************************************************************/

#ifndef _MMICLIPBRD_MENUTABLE_H_
#define _MMICLIPBRD_MENUTABLE_H_

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

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//Menu ID
typedef enum
{
    MMICLIPBRD_MENU_START = (MMI_MODULE_CLIPBRD << 16),

    CLIPBRD_MENU_CUT_ITEM_ID,
    CLIPBRD_MENU_COPY_ITEM_ID,
    CLIPBRD_MENU_PASTE_ITEM_ID,
    CLIPBRD_MENU_SELECTALL_ITEM_ID,

    MMICLIPBRD_MENU_MAX_ITEM_ID
} MMICLIPBRD_ID_MENU_E;


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMICLIPBRD_MENU_TABEL_START = (MMI_MODULE_CLIPBRD << 16),

    #include "mmiclipbrd_menutable.def"

    MMICLIPBRD_MENU_TABLE_MAX
} MMICLIPBRD_MENU_TABEL_E;

#undef MENU_DEF


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register clipboard menu group
//  Global resource dependence : 
//  Author: hua.fang
//  Note: 
/*****************************************************************************/
PUBLIC void MMICLIPBRD_RegMenuGroup(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //_MMICLIPBRD_MENUTABLE_H_

