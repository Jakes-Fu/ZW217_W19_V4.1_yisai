/*****************************************************************************
 ** File Name:   mmiappsample_menutable.h
 ** Author:      sam.hua
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe menutable 
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  13/aug/2012      sam.hua       written
 ****************************************************************************/



#ifndef _MMIAPPSAMPLE_MENUTABLE_H_
#define _MMIAPPSAMPLE_MENUTABLE_H_

#include "mmi_module.h"
#include "mmiappsample_define.h"

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
    ID_APPSAMPLE_MENU_START = (MMI_MODULE_APPSAMPLE << 16),
    ID_APPSAMPLE_WINDATA,
    ID_APPSAMPLE_EVENT,
    ID_APPSAMPLE_MENU_MAX
}
MMIAPPSAMPLE_ID_MENU_E;



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
                 _ITEMCOUNT) \
_TABLEID,

    typedef enum
    {
        MMICC_MENU_LABEL_START = (MMI_MODULE_APPSAMPLE << 16),

#include "mmiappsample_menutable.def"

        MMIAPPSAMPLE_MENUTABLE_MAX
    } MMIAPPSAMPLE_MENU_LABEL_E;

#undef MENU_DEF


    /**---------------------------------------------------------------------------*
     **                         Compiler Flag                                     *
     **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/*-------------------------------------------------------------------------*/

#endif //_MMIAPPSAMPLE_MENUTABLE_H_

