/****************************************************************************
** File Name:      mmiikeybackup_menutable.h                                     *
** Author:         haiwu.chen                                               *
** Date:           12/25/2011                                               *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:                                                             *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/25/2011     haiwu.chen        Create                                 *
****************************************************************************/

#ifdef IKEYBACKUP_SUPPORT

#ifndef _MMIIKEYBACKUP_MENUTABLE_H_
#define _MMIIKEYBACKUP_MENUTABLE_H_


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
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//menu id
typedef enum
{
    ID_IKEYBACKUP_MENU_START = (MMI_MODULE_IKEYBACKUP << 16),
        
    ID_IKEYBACKUP_MENU_MAX
} MMIIKB_MENU_ID_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,
     
typedef enum
{
    MMIIKEYBACKUP_MENU_START = (MMI_MODULE_IKEYBACKUP << 16),
    #include "mmiikeybackup_menutable.def"
    MMIIKEYBACKUP_MENU_MAX
} MMIIKB_MENU_LABEL_E;
#undef MENU_DEF


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init 1-keybackup module nv
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIKB_RegNv(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //IKEYBACKUP_SUPPORT

#endif //_MMIIKEYBACKUP_MENUTABLE_H_

