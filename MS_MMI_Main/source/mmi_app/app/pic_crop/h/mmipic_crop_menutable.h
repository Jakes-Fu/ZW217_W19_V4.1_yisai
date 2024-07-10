/*************************************************************************
 ** File Name:      mmipic_crop_menutable.h                                         *
 ** Author:           chunyou                                           *
 ** Date:             12/25/2011                                           *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about crop menu table             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 12/2011       chunyou         Create.                              *
*************************************************************************/

#ifdef MMI_PIC_CROP_SUPPORT
#ifndef _MMIPIC_CROP_MENUTABLE_H_
#define _MMIPIC_CROP_MENUTABLE_H_

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

//²Ëµ¥ID
typedef enum
{
    MMIPIC_CROP_ID_MENU_START = (MMI_MODULE_PIC_CROP << 16),
    //ID_PIC_EDIT_SAVE,
    MMIPIC_CROP_MENU_ID_MAX
}MMIPIC_CROP_MENU_ID_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIPIC_CROP_MENU_TABEL_MIN = (MMI_MODULE_PIC_CROP << 16),
    #include "mmipic_crop_menutable.def"
    MMIPIC_CROP_MENU_TABLE_MAX
} MMIPIC_CROP_MENU_TABEL_E;

#undef MENU_DEF

/*****************************************************************************/
//  Description : Register pic menu group
//  Global resource dependence : none
//  Author: vincent.shang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPIC_CropRegMenuGroup(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif /* _MMIENG_MENUTABLE_H_ */
#endif
