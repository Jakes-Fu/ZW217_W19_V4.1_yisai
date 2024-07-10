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

#ifndef _MMIZDTHTTP_MENUTABLE_H_
#define _MMIZDTHTTP_MENUTABLE_H_

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
    ID_ZDTHTTP_MENU_START = (MMI_MODULE_ZDTHTTP << 16),
    ID_ZDTHTTP_MENU_TEST1,
    ID_ZDTHTTP_MENU_TEST2,
    ID_ZDTHTTP_MENU_MAX
}MMIZDTHTTP_ID_MENU_E;


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIZDTHTTP_MENU_LABEL_START = (MMI_MODULE_ZDTHTTP << 16),

    #include "zdthttp_menutable.def"

    MMIZDTHTTP_MENUTABLE_MAX
} MMIZDTHTTP_MENU_LABEL_E;

#undef MENU_DEF


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //_CC_MENUTABLE_H_

