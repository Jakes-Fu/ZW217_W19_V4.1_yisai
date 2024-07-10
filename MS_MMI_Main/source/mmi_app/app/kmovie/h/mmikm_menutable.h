/***************************************************************************
** File Name:      mmikm_menutable.h                                     *
** Author:                                                                 *
** Date:           14/07/2011                                             *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE             NAME              DESCRIPTION                          *
** 07/2011         tonny.chen          Create                               *
**																		   *
****************************************************************************/
#ifndef _MMIKM_MENUTABLE_H_
#define _MMIKM_MENUTABLE_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "mmi_module.h"
#include "sci_types.h"
#include "mmk_type.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef _cplusplus
	extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define TIP_NULL 0

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
//Menu ID
typedef enum
{
    KM_ID_MENU_START = (MMI_MODULE_KMOVIE << 16),
	ID_LAST_PLAYED,
    ID_TYPE_ACTION,
    ID_TYPE_ADVENTURE,
    ID_TYPE_ANIMATION,
    ID_TYPE_COMEDY,
    ID_TYPE_DOCUMENTARY,
    ID_TYPE_DRAMA,
    ID_TYPE_FAMILY,
    ID_TYPE_FANTASY,
    ID_TYPE_HORROR,
    ID_TYPE_MUSIC,
    ID_TYPE_MYSTERY,
    ID_TYPE_PERFORMING,
    ID_TYPE_ROMANCE,
    ID_TYPE_SCIFI,
    ID_TYPE_SPORTS,
    ID_TYPE_SUSPENSE,
    ID_TYPE_WAR,
    ID_TYPE_WESTERN,
    ID_TYPE_OTHER,
    ID_KM_OPEN,
    ID_KM_UPDATE_LIST,
    ID_PLAY,
    ID_DELETE,
    ID_MORE_INFO,
    ID_KM_MARK,
    ID_KM_SUB_MARK,
    ID_KM_CANCEL_MARK,
    ID_KM_CANCEL_ALL_MARK,
    ID_KM_MARK_ALL,
    
    KM_ID_MENU_MAX
}MMIKM_ID_MENU_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    KM_MENU_LABEL_START = (MMI_MODULE_KMOVIE << 16),
    #include "mmikm_menutable.def"
    KM_MENUTABLE_MAX
} KM_MENU_LABEL_E;

#undef MENU_DEF
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register km menu group
//	Global resource dependence : none
//  Author: tonny.chen
//	Note:
/*****************************************************************************/
PUBLIC void MMIKM_RegMenuGroup(void);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif//_MMIKM_MENUTABLE_H_

