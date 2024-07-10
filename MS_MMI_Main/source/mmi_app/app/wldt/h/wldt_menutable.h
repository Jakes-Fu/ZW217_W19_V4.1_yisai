/*****************************************************************************
** File Name:      mmicc_menutable.h                                         *
** Author:         bruce.chi                                                 *
** Date:           2006/09/25                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe cc menu id                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/25     bruce.chi        Create                                    *
******************************************************************************/

#ifndef _MMIWLDT_MENUTABLE_H_
#define _MMIWLDT_MENUTABLE_H_

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


//Menu ID
typedef enum
{
    ID_WLDT_MENU_START = (MMI_MODULE_WLDT << 16),
    ID_WLDT_MENU_TEST1,
    ID_WLDT_MENU_TEST2,
    ID_WLDT_MENU_MAX
}MMIWLDT_ID_MENU_E;


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIWLDT_MENU_LABEL_START = (MMI_MODULE_WLDT << 16),

    #include "wldt_menutable.def"

    MMIWLDT_MENUTABLE_MAX
} MMIWLDT_MENU_LABEL_E;

#undef MENU_DEF


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //_CC_MENUTABLE_H_

