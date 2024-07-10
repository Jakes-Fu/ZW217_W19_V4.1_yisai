/****************************************************************************
** File Name:      mmipic_menutable.h                                       *
** Author:         vincent.shang                                                    *
** Date:           2008.01.14                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:                                                             *
*****************************************************************************
**                         Important Edit History                                                   *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                  *
** 01/2008       vincent            Create
** 
****************************************************************************/
//macro replace
//#ifdef MMI_PIC_EDITOR_ENABLE
#ifdef PIC_EDITOR_SUPPORT
#ifndef _MMIPIC_MENUTABLE_H_
#define _MMIPIC_MENUTABLE_H_

#include "mmi_module.h"
#include "mmi_modu_main.h"
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
    MMIPIC_ID_MENU_START = (MMI_MODULE_PIC << 16),
    ID_PIC_EDIT_SAVE,
    ID_PIC_EDIT_SAVEAS,
    ID_PIC_EDIT_UNDO,
//    ID_PIC_EDIT_ZOOM,
    ID_PIC_EDIT_ROTATE,
    ID_PIC_EDIT_INSERT,
    ID_PIC_EDIT_ADJUST,
    ID_PIC_EDIT_EFFECT,

    ID_PIC_EDIT_PASTE,
    ID_PIC_EDIT_CHARACTER,
    ID_PIC_EDIT_FRAME,

    ID_PIC_EDIT_SIZE,
    ID_PIC_EDIT_COLOR,
    ID_PIC_EDIT_BRIGHTNESS,
    ID_PIC_EDIT_CONSTRACT,
    ID_PIC_EDIT_SATURATION,
    ID_PIC_EDIT_HUE,

    ID_PIC_EDIT_GRAY,
    ID_PIC_EDIT_SEPIA,
    ID_PIC_EDIT_SMOOTH,
    ID_PIC_EDIT_EMBOSS,
    ID_PIC_EDIT_SHARPEN,
    ID_PIC_EDIT_DIFFUSION,
    ID_PIC_EDIT_EXPOSURE,
    ID_PIC_EDIT_NEGATIVE_RGB,
    ID_PIC_EDIT_DENOISE,
    MMIPIC_MENU_ID_MAX
}MMIPIC_MENU_ID_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIPIC_MENU_TABEL_MIN = (MMI_MODULE_PIC << 16),
    #include "mmipic_menutable.def"
    MMIPIC_MENU_TABLE_MAX
} MMIPIC_MENU_TABEL_E;

#undef MENU_DEF

/*****************************************************************************/
//  Description : Register pic menu group
//  Global resource dependence : none
//  Author: vincent.shang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPIC_RegMenuGroup(void);





/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif /* _MMIENG_MENUTABLE_H_ */
#endif
